/**
 * Diplomacy AI Client - Part of the DAIDE project.
 *
 * BaseBot Code. This is the Base class for all Bots.
 *
 * (C) David Norman 2002 david@ellought.demon.co.uk
 *
 * This software may be reused for non-commercial purposes without charge, and
 * without notifying the author. Use of any part of this software for commercial 
 * purposes without permission from the Author is prohibited.
 **/

#include "stdafx.h"
#include "BaseBot.h"
#include "AiClient.h"
#include "AiClientDlg.h"
#include "dcspextn.h"
#include "dcspcraw.h"
#include "error_log.h"
#include "map_and_units.h"
#include "token_text_map.h"

BaseBot *BaseBot::m_the_bot;

BaseBot::BaseBot( void )
{
	m_main_dlg = NULL;
	m_the_bot = this;
	m_tcpip_client_started = false;
	m_map_and_units = MapAndUnits::get_instance();
	m_map_requested = false;
}

BaseBot::~BaseBot()
{
	if ( m_tcpip_client_started )
	{
//		dcsp_stop();
	}

	close_logs();
}

bool BaseBot::process_command_line_parameter( char param_token, CString &parameter )
{
	return false;
}

CString BaseBot::report_command_line_parameters( void )
{
	return CString( "" );
}

bool BaseBot::initialise( COMMAND_LINE_PARAMETERS &parameters, CAiClientDlg *main_dialog, int *error_code )
{
	DCSP_USAGE_OPTIONS *tcpip_options;	// Options for setting up the TCP/IP DLL
	bool tcpip_server_started_ok;		// Whether the server is started OK
	const int MAX_COMPUTER_NAME_LEN = 1000;
										// Max length of a computer name
	const unsigned short DEFAULT_PORT_NUMBER = 16713;
										// Default port number to connect on
	bool address_converted_ok;			// Whether the address was converted into an IP address OK

	// Store the command line parameters
	m_parameters = parameters;
	
	// Store the pointer to the main dialog
	m_main_dlg = main_dialog;

	// Create the mutex to protect the callback queue
	incoming_message_mutex = CreateMutex( NULL, false, NULL );

	// Get the log level
	if ( parameters.log_level_specified == false )
	{
		parameters.log_level = 0;
	}

	enable_tcpip_logging( parameters.log_level > 0 );

	// Start the TCP/IP
	tcpip_options = dcsp_configure("aiclient.cfg");
	tcpip_options->client = true;
	tcpip_options->scheduling = DCSP_SCHEDULE_THREAD;
	tcpip_options->notification = DCSP_NOTIFICATION_CALLBACK;
	tcpip_options->representation = DCSP_REPRESENTATION_HOST_ORDER;
	tcpip_options->message_callback = tcpip_message_callback;
	tcpip_options->message_callback_data = NULL;
	tcpip_options->connection_logging = ( parameters.log_level > 0 );
	tcpip_options->trace_level = parameters.log_level;
	tcpip_options->trace_filename = "tcpip_output.txt";
	tcpip_options->trace_size = 5000000;
	tcpip_options->trace_filter_in = "";
	tcpip_options->trace_filter_out = "";

	tcpip_server_started_ok = dcsp_start( tcpip_options ) != 0;

	if ( tcpip_server_started_ok == false )
	{
		log_error("Failed to start TCP/IP Server");

		if ( error_code != NULL )
		{
			*error_code = START_COULDNT_START_SERVER;
		}
	}
	else
	{
		// Set the IP address and port to use
		if ( ( parameters.ip_specified == false ) && ( parameters.name_specified == false ) )
		{
			gethostname( parameters.server_name.GetBuffer( MAX_COMPUTER_NAME_LEN ), MAX_COMPUTER_NAME_LEN );
			parameters.server_name.ReleaseBuffer();
			parameters.name_specified = true;
		}

		if ( parameters.port_specified == false )
		{
			parameters.port_number = DEFAULT_PORT_NUMBER;
		}

		if ( parameters.name_specified )
		{
			address_converted_ok = dcsp_convert_address( parameters.server_name.GetBuffer(0), DCSP_ADDR_FORMAT_HOST_NAME, parameters.port_number ) != 0;
		}
		else
		{
			address_converted_ok = dcsp_convert_address( parameters.server_name.GetBuffer(0), DCSP_ADDR_FORMAT_DOTTED_DECIMAL, parameters.port_number ) != 0;
		}

		if ( address_converted_ok == false )
		{
			tcpip_server_started_ok = false;

			log_error("Bad TCP/IP address '%s'", (const char *)( parameters.server_name ) );

			if ( error_code != NULL )
			{
				*error_code = START_BAD_ADDRESS;
			}
		}
		else
		{
			m_tcpip_client_started = true;

			// Connect to the server
			dcsp_client_connect();

			// Send the NME or OBS message
			send_nme_or_obs();
		}
	}

	if ( tcpip_server_started_ok == false )
	{
//		dcsp_stop();

		m_tcpip_client_started = false;
	}

	return tcpip_server_started_ok;
}

VOS_VOID BaseBot::tcpip_message_callback( VOS_CHAR *message, VOS_S32 message_length, 
										  VOS_S32 connection_id, VOS_VOID* )
{
	INCOMING_MESSAGE incoming_message;

	incoming_message.message = new char[ message_length ];

	if ( incoming_message.message == NULL )
	{
		log_error("Failed to allocate memory to queue message");
	}
	else
	{
		incoming_message.message_length = message_length;
		incoming_message.connection_id = connection_id;
		memcpy( incoming_message.message, message, message_length );

		WaitForSingleObject( m_the_bot->incoming_message_mutex, INFINITE );

		m_the_bot->incoming_message_list.push_back( incoming_message );

		ReleaseMutex( m_the_bot->incoming_message_mutex );

		m_the_bot->m_main_dlg->PostMessage( WM_TCPIP_MESSAGE );
	}
}

void BaseBot::send_nme_or_obs( void )
{
	TokenMessage obs_message( TOKEN_COMMAND_OBS );

	send_message_to_server( obs_message );
}

void BaseBot::send_message_to_server( TokenMessage &message )
{
	int message_length;							// Length of the message in tokens
	VOS_CHAR *tcpip_message;					// Message to send over the link
	DCSP_HST_MESSAGE *tcpip_message_header;		// Header of the message	
	VOS_BOOL send_ok;							// Whether the message was sent OK

	log_tcpip_comms( false, message );
	
	if ( m_tcpip_client_started )
	{
		// Get the length of the message
		message_length = message.get_message_length();

		tcpip_message = new VOS_CHAR[ message_length * 2 + 6 ];
		tcpip_message_header = (DCSP_HST_MESSAGE *)( tcpip_message );

		tcpip_message_header->type = DCSP_MSG_TYPE_DM;
		tcpip_message_header->length = message_length * 2;

		message.get_message( (Token *)( tcpip_message + 4 ), message_length + 1 );

		send_ok = dcsp_client_send_message( tcpip_message, message_length * 2 + 4 );

		if ( send_ok == FALSE )
		{
			log_error("Failed to send message");
		}

		delete [] tcpip_message;
	}
	else
	{
		log_tcpip_note("Message not sent - not connected");
	}
}

void BaseBot::send_orders_to_server( void )
{
	TokenMessage sub_command;

	sub_command = m_map_and_units->build_sub_command();

	if ( sub_command.get_message_length() > 1 )
	{
		send_message_to_server( m_map_and_units->build_sub_command() );
	}
}

void BaseBot::send_name_and_version_to_server( char name[], char version[] )
{
	TokenMessage name_message;
	CString name_in_quotes;
	CString version_in_quotes;
	TokenMessage name_tokens;
	TokenMessage version_tokens;

	name_in_quotes = "'" + (CString)( name ) + "'";
	version_in_quotes = "'" + (CString)( version ) + "'";

	name_tokens.set_message_from_text( (string)( (const char *)( name_in_quotes ) ) );
	version_tokens.set_message_from_text ( (string)( (const char *)( version_in_quotes ) ) );

	name_message = TOKEN_COMMAND_NME & name_tokens & version_tokens;

	send_message_to_server( name_message );
}

void BaseBot::disconnect_from_server( void )
{
	if ( m_tcpip_client_started )
	{
//		dcsp_stop();
		m_tcpip_client_started = false;
	}
}

void BaseBot::request_map( void )
{
	m_map_requested = true;

	send_message_to_server( TokenMessage( TOKEN_COMMAND_MAP ) );
}

bool BaseBot::is_map_requested( void )
{
	return m_map_requested;
}

void BaseBot::process_incoming_messages( void )
{
	INCOMING_MESSAGE incoming_message;

	WaitForSingleObject( incoming_message_mutex, INFINITE );

	while ( incoming_message_list.empty() == false )
	{
		incoming_message = *( incoming_message_list.begin() );

		incoming_message_list.pop_front();

		ReleaseMutex( incoming_message_mutex );

		process_message( incoming_message );

		delete [] incoming_message.message;

		WaitForSingleObject( incoming_message_mutex, INFINITE );
	}

	ReleaseMutex( incoming_message_mutex );
}

void BaseBot::process_message( INCOMING_MESSAGE &message )
{
	DCSP_HST_MESSAGE *header = (DCSP_HST_MESSAGE *)( message.message );
										// Message Header of the received message
	WORD error_code;					// Error code from an error message

	switch ( header->type )
	{
		case DCSP_MSG_TYPE_IM:
		{
			// Initial Message. Nothing to do.
			log_error("Error - Initial message received");
			log_tcpip_note("Initial Message received");

			break;
		}

		case DCSP_MSG_TYPE_RM:
		{
			// Representation message. Needs handling to update the text map eventually
			process_rm_message( message.message + sizeof( DCSP_HST_MESSAGE ),
								message.message_length - sizeof( DCSP_HST_MESSAGE ) );

			break;
		}

		case DCSP_MSG_TYPE_DM:
		{
			// Diplomacy Message

			process_diplomacy_message( message.message + sizeof( DCSP_HST_MESSAGE ), 
									   message.message_length - sizeof( DCSP_HST_MESSAGE ) );

			break;
		}

		case DCSP_MSG_TYPE_FM:
		{
			// Final Message. Nothing to do.
			log_tcpip_note("Final Message received");

			break;
		}

		case DCSP_MSG_TYPE_EM:
		{
			// Error Message. Report
			log_tcpip_note("Error Message received");

			error_code = *( (WORD *)( message.message + sizeof( DCSP_HST_MESSAGE ) ) );

			log_error("Error Message Received. Error Code = %d", error_code );

			break;
		}

		case DCSP_MSG_TYPE_LM:
		{
			// Local message. Process it.

			process_local_message( (DCSP_HST_LM *)( message.message ), message.connection_id );

			break;
		}

		default:
		{
			log_error("Unexpected Message type - %d", header->type );
		}
	}
}

void BaseBot::process_local_message( DCSP_HST_LM *message, int connection_id )
{
	CString tcpip_note;

	switch ( message->event )
	{
		case DCSP_HST_LOCAL_EVENT_CONNECTED:
		{
			log_tcpip_note("Connected Message received");

			process_connected_message( message );

			break;
		}

		case DCSP_HST_LOCAL_EVENT_CLOSED:
		{
			log_tcpip_note("Close Message received");

			process_close_message();

			break;
		}

		case DCSP_HST_LOCAL_EVENT_ERR_READ:
		{
			tcpip_note.Format("Read Error Message received. Error code %d", message->err );

			log_tcpip_note( (const char *)( tcpip_note ) );

			// Handle it as a close message
			process_close_message();

			break;
		}

		case DCSP_HST_LOCAL_EVENT_ERR_WRITE:
		{
			tcpip_note.Format("Write Error Message received. Error code %d", message->err );

			log_tcpip_note( (const char *)( tcpip_note ) );

			// Handle it as a close message
			process_close_message();

			break;
		}

		case DCSP_HST_LOCAL_EVENT_OUT_OF_MEM:
		{
			log_tcpip_note("Out of Memory Message received");

			log_error("Out of Memory");

			break;
		}

		default:
		{
			log_tcpip_note("Unexpected Local Message received");

			log_error("Unexpected Local message event %d", message->event );
		}
	}
}

void BaseBot::process_connected_message( DCSP_HST_LM *message )
{
	if ( message->err == 0 )
	{
		log_tcpip_note("Connected OK");
	}
	else
	{
		log_tcpip_note("Failed to connect to Server");
		AfxMessageBox("Failed to connect to Server");

		log_error("Failed to connect to Server. Error code %d", message->err );

		// Close the connection
//		dcsp_stop();

		m_tcpip_client_started = false;

		report_failed_connect();
	}
}

void BaseBot::report_failed_connect( void )
{
}

void BaseBot::process_close_message( void )
{
	// Close the connection
//	dcsp_stop();

	m_tcpip_client_started = false;

	report_close();
}

void BaseBot::report_close( void )
{
	AfxMessageBox("Server Disconnected");
}

void BaseBot::process_rm_message( char *message, int message_length )
{
	int message_counter;				// Counter to step through the message
	BYTE *token_value;					// The byte sequence for the token
	char *token_name;					// The textual version of the token
	Token token;						// The token
	TokenTextMap *token_text_map;		// The map of tokens to text

	if ( message_length == 0 )
	{
		// Nothing to do
	}
	else
	{
		// Get the token text map
		token_text_map = TokenTextMap::instance();

		// Clear all the old power and province tokens
		token_text_map->clear_power_and_province_categories();

		// For each entry
		for ( message_counter = 0; message_counter < message_length; message_counter += 6 )
		{
			token_value = (BYTE *)( message + message_counter );
			token_name = message + message_counter + 2;

			token = Token( token_value[ 1 ], token_value[ 0 ] );

			token_text_map->add_token( token, token_name );
		}
	}
}

void BaseBot::process_diplomacy_message( char *message, int message_length )
{
	Token *message_tokens;							// The tokens in the incoming message
	TokenMessage incoming_message;					// The incoming message
	Token lead_token;								// The first token of the message
	
	message_tokens = (Token *)( message );

	if ( message_tokens[ 0 ] == TOKEN_COMMAND_PRN )
	{
		process_prn_message( message_tokens, message_length );
	}
	else
	{
		incoming_message.set_message( message_tokens, message_length / 2 );

		log_tcpip_comms( true, incoming_message );

		// If the first subtoken is a single message
		if ( incoming_message.submessage_is_single_token( 0 ) )
		{
			// Act on that token
			lead_token = incoming_message.get_token( 0 );

			// Messages that BaseBot handles initially
			if ( lead_token == TOKEN_COMMAND_HLO )
			{
				process_hlo( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_MAP )
			{
				process_map( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_MDF )
			{
				process_mdf( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_NOW )
			{
				process_now( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_ORD )
			{
				process_ord( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_SCO )
			{
				process_sco( incoming_message );
			}

			// Messages which are broken down further before being passed on
			else if ( lead_token == TOKEN_COMMAND_NOT )
			{
				process_not( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_REJ )
			{
				process_rej( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_YES )
			{
				process_yes( incoming_message );
			}

			// Messages to be handled by Bot immediately
			else if ( lead_token == TOKEN_COMMAND_CCD )
			{
				process_ccd( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_DRW )
			{
				process_drw( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_FRM )
			{
				process_frm_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_HUH )
			{
				process_huh_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_LOD )
			{
				process_lod_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_MIS )
			{
				process_mis_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_OFF )
			{
				process_off_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_OUT )
			{
				process_out( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_SLO )
			{
				process_slo( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_SMR )
			{
				process_smr_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_SVE )
			{
				process_sve_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_THX )
			{
				process_thx_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_TME )
			{
				process_tme_message( incoming_message );
			}
			else if ( lead_token == TOKEN_COMMAND_ADM )
			{
				process_adm_message( incoming_message );
			}
			else
			{
				log_error("Unexpected first token in message : %s", 
							incoming_message.get_message_as_text().c_str() );
			}
		}
		else
		{
			log_error("Illegal message received");
		}
	}
}

// Handle the HLO message. Extract the information we need, then pass on
void BaseBot::process_hlo( TokenMessage &incoming_message )
{
	TokenMessage power_submessage;
	TokenMessage passcode_submessage;

	// Get the submessages
	power_submessage = incoming_message.get_submessage( 1 );
	passcode_submessage = incoming_message.get_submessage( 2 );

	// Store the details
	m_map_and_units->set_power_played( power_submessage.get_token() );
	m_map_and_units->passcode = passcode_submessage.get_token().get_number();
	m_map_and_units->variant = incoming_message.get_submessage( 3 );
	m_map_and_units->game_started = true;

	// Pass the message on
	process_hlo_message( incoming_message );
}

// Handle the MAP message. Store the map name, send the MDF, then pass on
void BaseBot::process_map( TokenMessage &incoming_message )
{
	TokenMessage mdf_message( TOKEN_COMMAND_MDF );
	TokenMessage name_submessage;

	// Store the map name
	name_submessage = incoming_message.get_submessage( 1 );
	m_map_and_units->map_name = name_submessage.get_message_as_text().c_str();
	remove_quotes( m_map_and_units->map_name );

	// Send an MDF
	send_message_to_server( mdf_message );

	// Store the map message
	m_map_message = incoming_message;

	// Call the users handler function
	process_map_message( incoming_message );
}

void BaseBot::remove_quotes( std::string &message_string )
{
	int start_quote_locn;
	int end_quote_locn;

	start_quote_locn = message_string.find('\'');
	end_quote_locn = message_string.find_last_of('\'');

	if ( end_quote_locn > start_quote_locn )
	{
		message_string = message_string.substr( start_quote_locn + 1, end_quote_locn - start_quote_locn - 1 );
	}
}


// Handle the MDF message. Store the map and pass on
void BaseBot::process_mdf( TokenMessage &incoming_message )
{
	int set_map_result;
	
	set_map_result = m_map_and_units->set_map( incoming_message );

	if ( set_map_result != ADJUDICATOR_NO_ERROR )
	{
		log_error("Failed to set map - Error location %d", set_map_result );
	}

	process_mdf_message( incoming_message );

	// If the map wasn't sent by request, then reply to accept the map
	if ( m_map_requested == false )
	{
		send_message_to_server( TOKEN_COMMAND_YES & m_map_message );
	}
	else
	{
		// The map was requested following an IAM, so also request a HLO, SCO and NOW.
		send_message_to_server( TokenMessage( TOKEN_COMMAND_HLO ) );
		send_message_to_server( TokenMessage( TOKEN_COMMAND_ORD ) );
		send_message_to_server( TokenMessage( TOKEN_COMMAND_SCO ) );
		send_message_to_server( TokenMessage( TOKEN_COMMAND_NOW ) );

		m_map_requested = false;
	}
}

// Process the NOW message. Store the position and pass on
void BaseBot::process_now( TokenMessage &incoming_message )
{
	m_map_and_units->set_units( incoming_message );

	process_now_message( incoming_message );
}

// Process the ORD message. Store the results and pass on
void BaseBot::process_ord( TokenMessage &incoming_message )
{
	m_map_and_units->store_result( incoming_message );

	process_ord_message( incoming_message );
}

// Process the SCO message. Store the centre ownership and pass on
void BaseBot::process_sco( TokenMessage &incoming_message )
{
	m_map_and_units->set_ownership( incoming_message );

	process_sco_message( incoming_message );
}

// Process the NOT message. Split according to next token
void BaseBot::process_not( TokenMessage &incoming_message )
{
	TokenMessage not_message = incoming_message.get_submessage( 1 );

	if ( not_message.get_token( 0 ) == TOKEN_COMMAND_CCD )
	{
		process_not_ccd( incoming_message, not_message.get_submessage( 1 ) );
	}
	else if ( not_message.get_token( 0 ) == TOKEN_COMMAND_TME )
	{
		process_not_tme_message( incoming_message, not_message.get_submessage( 1 ) );
	}
	else
	{
		process_unexpected_not_message( incoming_message );
	}
}

// Process the REJ message. Split according to next token
void BaseBot::process_rej( TokenMessage &incoming_message )
{
	TokenMessage rej_message = incoming_message.get_submessage( 1 );

	if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_NME )
	{
		process_rej_nme_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_IAM )
	{
		process_rej_iam_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_HLO )
	{
		process_rej_hlo_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_NOW )
	{
		process_rej_now_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_SCO )
	{
		process_rej_sco_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_HST )
	{
		process_rej_hst_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_SUB )
	{
		process_rej_sub_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_NOT ) // GOF, DRW
	{
		process_rej_not( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_GOF )
	{
		process_rej_gof_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_ORD )
	{
		process_rej_ord_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_TME )
	{
		process_rej_tme_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_DRW )
	{
		process_rej_drw_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_SND )
	{
		process_rej_snd( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_ADM )
	{
		process_rej_adm_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else if ( rej_message.get_token( 0 ) == TOKEN_COMMAND_MIS )
	{
		process_rej_mis_message( incoming_message, rej_message.get_submessage( 1 ) );
	}
	else
	{
		process_unexpected_rej_message( incoming_message );
	}
}

// Process the REJ(NOT()) message. Split according to next token
void BaseBot::process_rej_not( TokenMessage &incoming_message, TokenMessage &rej_not_parameters )
{
	if ( rej_not_parameters.get_token( 0 ) == TOKEN_COMMAND_GOF )
	{
		process_rej_not_gof_message( incoming_message, rej_not_parameters.get_submessage( 1 ) );
	}
	else if ( rej_not_parameters.get_token( 0 ) == TOKEN_COMMAND_DRW )
	{
		process_rej_not_drw_message( incoming_message, rej_not_parameters.get_submessage( 1 ) );
	}
	else
	{
		process_unexpected_rej_not_message( incoming_message );
	}
}

// Process the YES message. Split according to next token
void BaseBot::process_yes( TokenMessage &incoming_message )
{
	TokenMessage yes_message = incoming_message.get_submessage( 1 );

	if ( yes_message.get_token( 0 ) == TOKEN_COMMAND_NME )
	{
		process_yes_nme_message( incoming_message, yes_message.get_submessage( 1 ) );
	}
	else if ( yes_message.get_token( 0 ) == TOKEN_COMMAND_OBS )
	{
		process_yes_obs_message( incoming_message, yes_message.get_submessage( 1 ) );
	}
	else if ( yes_message.get_token( 0 ) == TOKEN_COMMAND_IAM )
	{
		process_yes_iam_message( incoming_message, yes_message.get_submessage( 1 ) );
	}
	else if ( yes_message.get_token( 0 ) == TOKEN_COMMAND_NOT ) // GOF, DRW
	{
		process_yes_not( incoming_message, yes_message.get_submessage( 1 ) );
	}
	else if ( yes_message.get_token( 0 ) == TOKEN_COMMAND_GOF )
	{
		process_yes_gof_message( incoming_message, yes_message.get_submessage( 1 ) );
	}
	else if ( yes_message.get_token( 0 ) == TOKEN_COMMAND_TME )
	{
		process_yes_tme_message( incoming_message, yes_message.get_submessage( 1 ) );
	}
	else if ( yes_message.get_token( 0 ) == TOKEN_COMMAND_DRW )
	{
		process_yes_drw_message( incoming_message, yes_message.get_submessage( 1 ) );
	}
	else if ( yes_message.get_token( 0 ) == TOKEN_COMMAND_SND )
	{
		process_yes_snd( incoming_message, yes_message.get_submessage( 1 ) );
	}
	else
	{
		process_unexpected_yes_message( incoming_message );
	}
}

// Process the YES(NOT()) message. Split according to next token
void BaseBot::process_yes_not( TokenMessage &incoming_message, TokenMessage &yes_not_parameters )
{
	if ( yes_not_parameters.get_token( 0 ) == TOKEN_COMMAND_GOF )
	{
		process_yes_not_gof_message( incoming_message, yes_not_parameters.get_submessage( 1 ) );
	}
	else if ( yes_not_parameters.get_token( 0 ) == TOKEN_COMMAND_DRW )
	{
		process_yes_not_drw_message( incoming_message, yes_not_parameters.get_submessage( 1 ) );
	}
	else if ( yes_not_parameters.get_token( 0 ) == TOKEN_COMMAND_SUB )
	{
		process_yes_not_sub_message( incoming_message, yes_not_parameters.get_submessage( 1 ) );
	}
	else
	{
		process_unexpected_yes_not_message( incoming_message );
	}
}

// Handle an incoming CCD message.
void BaseBot::process_ccd_message( TokenMessage &incoming_message, bool is_new_disconnection )
{
}

// Handle an incoming DRW message. Default sets the game to over
void BaseBot::process_drw( TokenMessage &incoming_message )
{
	m_map_and_units->game_over = true;

	process_drw_message( incoming_message );
}

void BaseBot::process_drw_message( TokenMessage &incoming_message )
{
}

// Handle an incoming FRM message. Default version replies with HUH( message ). TRY().
void BaseBot::process_frm_message( TokenMessage &incoming_message )
{
	TokenMessage huh_message;
	TokenMessage try_message;
	Token from_power;
	TokenMessage press_message;
	TokenMessage message_id;

	message_id = incoming_message.get_submessage( 1 );
	from_power = message_id.get_token();
	press_message = incoming_message.get_submessage( 3 );

	if ( ( press_message.get_token( 0 ) != TOKEN_COMMAND_HUH )
	  && ( press_message.get_token( 0 ) != TOKEN_PRESS_TRY ) )
	{
		// Changed as message IDs no longer used.
		huh_message = TOKEN_COMMAND_SND & from_power & ( TOKEN_COMMAND_HUH & ( TOKEN_PARAMETER_ERR + press_message ) );
		try_message = TOKEN_COMMAND_SND & from_power & ( TOKEN_PRESS_TRY & try_message );

		send_message_to_server( huh_message );
		send_message_to_server( try_message );
	}
}

// Handle an incoming HUH message. Default logs it
void BaseBot::process_huh_message( TokenMessage &incoming_message )
{
	log_error("HUH message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming LOD message. Default replies with REJ (LOD (...) )
void BaseBot::process_lod_message( TokenMessage &incoming_message )
{
	send_message_to_server( TOKEN_COMMAND_REJ & incoming_message );
}

// Handle an incoming MIS message.
void BaseBot::process_mis_message( TokenMessage &incoming_message )
{
}

// Handle an incoming OFF message. Default disconnects from server and exits
void BaseBot::process_off_message( TokenMessage &incoming_message )
{
	disconnect_from_server();

	m_main_dlg->end_dialog();
}

// Handle an incoming OUT message.
void BaseBot::process_out_message( TokenMessage &incoming_message )
{
}

// Handle an incoming PRN message. Default logs it
void BaseBot::process_prn_message( Token *incoming_message, int message_length )
{
	log_error("PRN message received. Starts %.4X %.4X %.4X %.4X", incoming_message[ 0 ], 
					incoming_message[ 1 ], incoming_message[ 2 ], incoming_message[ 3 ] );
}

// Handle an incoming SLO message. Sets the game to over
void BaseBot::process_slo( TokenMessage &incoming_message )
{
	m_map_and_units->game_over = true;

	process_slo_message( incoming_message );
}

void BaseBot::process_slo_message( TokenMessage &incoming_message )
{
}

void BaseBot::process_smr_message( TokenMessage &incoming_message )
{
}

// Handle an incoming SVE message. Default replies with YES (SVE( ... ) )
void BaseBot::process_sve_message( TokenMessage &incoming_message )
{
	send_message_to_server( TOKEN_COMMAND_YES & incoming_message );
}

// Handle an incoming THX message. Default supplies a simple replacement order if not MBV.
void BaseBot::process_thx_message( TokenMessage &incoming_message )
{
	TokenMessage order;					// The order submitted
	Token note;							// The order note returned
	TokenMessage unit;					// The unit ordered
	TokenMessage new_order;				// The replacement order to submit
	bool send_new_order = false;		// Whether to send a new order

	order = incoming_message.get_submessage( 1 );
	unit = order.get_submessage( 0 ).enclose();
	note = incoming_message.get_submessage( 2 ).get_token();
	
	if ( note == TOKEN_ORDER_NOTE_MBV )
	{
		// Everything is good. Nothing to do.
	}

	if ( ( note == TOKEN_ORDER_NOTE_FAR )
	  || ( note == TOKEN_ORDER_NOTE_NSP )
	  || ( note == TOKEN_ORDER_NOTE_NSU )
	  || ( note == TOKEN_ORDER_NOTE_NAS )
	  || ( note == TOKEN_ORDER_NOTE_NSF )
	  || ( note == TOKEN_ORDER_NOTE_NSA ) )
	{
		// Illegal movement order. Replace with a hold order.
		new_order = unit + TOKEN_ORDER_HLD;
		send_new_order = true;
	}

	if ( note == TOKEN_ORDER_NOTE_NVR )
	{
		// Illegal retreat order. Replace with a disband order
		new_order = unit + TOKEN_ORDER_DSB;
		send_new_order = true;
	}

	if ( ( note == TOKEN_ORDER_NOTE_YSC )
	  || ( note == TOKEN_ORDER_NOTE_ESC )
	  || ( note == TOKEN_ORDER_NOTE_HSC )
	  || ( note == TOKEN_ORDER_NOTE_NSC )
	  || ( note == TOKEN_ORDER_NOTE_CST ) )
	{
		// Illegal build order. Replace with a waive order
		new_order = unit.get_submessage( 0 ) + TOKEN_ORDER_WVE;
		send_new_order = true;
	}

	if ( ( note == TOKEN_ORDER_NOTE_NYU )
	  || ( note == TOKEN_ORDER_NOTE_NRS ) )
	{
		// Illegal order, but not much we can do about it
	}

	if ( ( note == TOKEN_ORDER_NOTE_NRN )
	  || ( note == TOKEN_ORDER_NOTE_NMB )
	  || ( note == TOKEN_ORDER_NOTE_NMR ) )
	{
		// Order wasn't needed in the first place!
	}

	if ( ( send_new_order ) && ( new_order != order ) )
	{
		log_error("THX returned %s for order '%s'. Replacing with '%s'",
					incoming_message.get_submessage( 2 ).get_message_as_text().c_str(),
					order.get_message_as_text().c_str(),
					new_order.get_message_as_text().c_str() );

		send_message_to_server( new_order );
	}
	else if ( note != TOKEN_ORDER_NOTE_MBV )
	{
		log_error("THX returned %s for order '%s'. No replacement order sent.",
					incoming_message.get_submessage( 2 ).get_message_as_text().c_str(),
					order.get_message_as_text().c_str() );
	}
}

// Handle an incoming TME message.
void BaseBot::process_tme_message( TokenMessage &incoming_message )
{
}

// Handle an incoming ADM message.
void BaseBot::process_adm_message( TokenMessage &incoming_message )
{
}

// Handle an incoming NOT( CCD() ) message. 
void BaseBot::process_not_ccd_message( TokenMessage &incoming_message, TokenMessage &message_parameters, bool is_new_reconnection )
{
}

// Handle an incoming NOT( TME() ) message. 
void BaseBot::process_not_tme_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming REJ( NME() ) message.
void BaseBot::process_rej_nme_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	bool attempt_reconnect;				// Whether to try and reconnect
	Token power_token = 0;				// The token for the power to reconnect as
	int passcode = 0;					// The passcode to reconnect as
	Token passcode_token;				// The passcode as a token

	attempt_reconnect = get_reconnect_details( power_token, passcode );

	if ( attempt_reconnect )
	{
		// Send an IAM message
		passcode_token.set_number( passcode );
		send_message_to_server( TOKEN_COMMAND_IAM & power_token & passcode_token );
	}
	else
	{
		// Disconnect
		disconnect_from_server();
	}
}

// Determine whether to try and reconnect to game. Default uses values passed on command line.
bool BaseBot::get_reconnect_details( Token &power, int &passcode )
{
	if ( m_parameters.reconnection_specified )
	{
		power = TokenTextMap::instance()->m_text_to_token_map[ (string)(const char *)( m_parameters.reconnect_power ) ];
		passcode = m_parameters.reconnect_passcode;
	}

	return m_parameters.reconnection_specified;
}

// Handle an incoming REJ( IAM() ) message. Default logs it
void BaseBot::process_rej_iam_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( IAM() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( HLO() ) message. Default logs it
void BaseBot::process_rej_hlo_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( HLO() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( NOW() ) message. Default logs it
void BaseBot::process_rej_now_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( NOW() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( SCO() ) message. Default logs it
void BaseBot::process_rej_sco_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( SCO() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( HST() ) message. Default logs it
void BaseBot::process_rej_hst_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( HST() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( SUB() ) message. Default logs it
void BaseBot::process_rej_sub_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( SUB() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( GOF() ) message. Default logs it
void BaseBot::process_rej_gof_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( GOF() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( ORD() ) message. Default logs it
void BaseBot::process_rej_ord_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( ORD() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( TME() ) message. Default logs it
void BaseBot::process_rej_tme_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( TME() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( DRW() ) message. Default logs it
void BaseBot::process_rej_drw_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( DRW() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( SND() ) message. Default logs it
void BaseBot::process_rej_snd_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( SND() ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( ADM() ) message. Default does nothing
void BaseBot::process_rej_adm_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming REJ( MIS() ) message. Default does nothing
void BaseBot::process_rej_mis_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming REJ( NOT( GOF() ) ) message. Default logs it
void BaseBot::process_rej_not_gof_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( NOT( GOF() ) ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming REJ( NOT( DRW() ) ) message. Default logs it
void BaseBot::process_rej_not_drw_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	log_error("REJ( NOT( DRW() ) ) message received : %s", incoming_message.get_message_as_text().c_str() );
}

// Handle an incoming YES( NME() ) message. 
void BaseBot::process_yes_nme_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming YES( OBS() ) message. 
void BaseBot::process_yes_obs_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming YES( IAM() ) message. 
void BaseBot::process_yes_iam_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	request_map();
}

// Handle an incoming YES( GOF() ) message. 
void BaseBot::process_yes_gof_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming YES( TME() ) message. 
void BaseBot::process_yes_tme_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming YES( DRW() ) message. 
void BaseBot::process_yes_drw_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming YES( SND() ) message. 
void BaseBot::process_yes_snd_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming YES( NOT( GOF() ) ) message. 
void BaseBot::process_yes_not_gof_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming YES( NOT( DRW() ) ) message. 
void BaseBot::process_yes_not_drw_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming YES( NOT( SUB() ) ) message. 
void BaseBot::process_yes_not_sub_message( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
}

// Handle an incoming NOT message with a parameter we don't expect
void BaseBot::process_unexpected_not_message( TokenMessage &incoming_message )
{
}

// Handle an incoming REJ message with a parameter we don't expect
void BaseBot::process_unexpected_rej_message( TokenMessage &incoming_message )
{
}

// Handle an incoming REJ( NOT( ) ) message with a parameter we don't expect
void BaseBot::process_unexpected_rej_not_message( TokenMessage &incoming_message )
{
}

// Handle an incoming YES message with a parameter we don't expect
void BaseBot::process_unexpected_yes_message( TokenMessage &incoming_message )
{
}

// Handle an incoming YES( NOT() ) message with a parameter we don't expect
void BaseBot::process_unexpected_yes_not_message( TokenMessage &incoming_message )
{
}

// Process a HLO message. Power played has already been stored.
void BaseBot::process_hlo_message( TokenMessage &incoming_message )
{
}

// Process a MAP message. Map name has already been stored, and an MDF sent to the server.
void BaseBot::process_map_message( TokenMessage &incoming_message )
{
}

// Process a MDF message. Map Definition has already been stored.
void BaseBot::process_mdf_message( TokenMessage &incoming_message )
{
}

// Process an ORD message. Results have already been stored.
void BaseBot::process_ord_message( TokenMessage &incoming_message )
{
}

// Process an SCO message. Supply Centre Ownership has already been stored.
void BaseBot::process_sco_message( TokenMessage &incoming_message )
{
}

// Process a NOW message. Position has already been stored.
void BaseBot::process_now_message( TokenMessage &incoming_message )
{
}

void BaseBot::send_press_to_server( TokenMessage press_to, TokenMessage press_message, bool resend_partial )
{
	SentPressInfo sent_press_record;

	sent_press_record.original_receiving_powers = press_to;
	sent_press_record.receiving_powers = press_to;
	sent_press_record.press_message = press_message;
	sent_press_record.resend_partial = resend_partial;
	sent_press_record.is_broadcast = false;

	m_sent_press.push_back( sent_press_record );

	send_message_to_server( TOKEN_COMMAND_SND & press_to & press_message );
}

void BaseBot::send_broadcast_to_server( TokenMessage broadcast_message )
{
	TokenMessage receiving_powers;
	int power_counter;
	SentPressInfo sent_press_record;

	for ( power_counter = 0; power_counter < m_map_and_units->number_of_powers; power_counter++ )
	{
		if ( ( m_map_and_units->power_played.get_subtoken() != power_counter )
		  && ( m_cd_powers.find( Token( CATEGORY_POWER, power_counter ) ) == m_cd_powers.end() ) )
		#pragma message("**** Also check for eliminated powers")
		{
			receiving_powers = receiving_powers + Token( CATEGORY_POWER, power_counter );
		}
	}

	sent_press_record.original_receiving_powers = receiving_powers;
	sent_press_record.receiving_powers = receiving_powers;
	sent_press_record.press_message = broadcast_message;
	sent_press_record.resend_partial = true;
	sent_press_record.is_broadcast = true;

	m_sent_press.push_back( sent_press_record );

	send_message_to_server( TOKEN_COMMAND_SND & receiving_powers & broadcast_message );
}

void BaseBot::process_ccd( TokenMessage &incoming_message )
{
	Token cd_power;
	CString message;
	bool is_new_disconnection = false;

	cd_power = incoming_message.get_submessage( 1 ).get_token();

	check_sent_press_for_missing_power( cd_power );

	if ( m_cd_powers.find( cd_power ) == m_cd_powers.end() )
	{
		m_cd_powers.insert( cd_power );

		is_new_disconnection = true;
	}

	process_ccd_message( incoming_message, is_new_disconnection );
}

void BaseBot::process_not_ccd( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	Token cd_power;
	bool is_new_reconnection = false;

	cd_power = message_parameters.get_token();

	if ( m_cd_powers.find( cd_power ) != m_cd_powers.end() )
	{
		m_cd_powers.erase( cd_power );

		is_new_reconnection = true;
	}

	process_not_ccd_message( incoming_message, message_parameters, is_new_reconnection );
}

void BaseBot::process_out( TokenMessage &incoming_message )
{
	Token out_power;

	out_power = incoming_message.get_submessage( 1 ).get_token();

	check_sent_press_for_missing_power( out_power );

	process_out_message( incoming_message );
}

void BaseBot::check_sent_press_for_missing_power( Token &missing_power )
{
	SentPressList::iterator sent_press_iterator;
	TokenMessage receiving_powers;
	bool missing_power_found;
	int power_counter;

	for ( sent_press_iterator = m_sent_press.begin();
		  sent_press_iterator != m_sent_press.end();
		  sent_press_iterator++ )
	{
		receiving_powers = sent_press_iterator->receiving_powers;

		for ( ( power_counter = 0, missing_power_found = false );
			  ( power_counter < receiving_powers.get_message_length() && missing_power_found == false );
			  power_counter++ )
		{
			if ( receiving_powers.get_token( power_counter ) == missing_power )
			{
				if ( sent_press_iterator->resend_partial )
				{
					send_to_reduced_powers( sent_press_iterator, missing_power );

					missing_power_found = true;
				}
				else
				{
					report_failed_press( sent_press_iterator->is_broadcast, 
										 sent_press_iterator->original_receiving_powers,
										 sent_press_iterator->press_message );

					missing_power_found = true;
				}
			}
		}
	}
}

void BaseBot::send_to_reduced_powers( SentPressList::iterator &sent_press_iterator, Token &cd_power )
{
	TokenMessage receiving_powers;
	TokenMessage reduced_powers;
	int power_counter;

	receiving_powers = sent_press_iterator->receiving_powers;	

	for ( power_counter = 0; power_counter < receiving_powers.get_message_length(); power_counter++ )
	{
		if ( receiving_powers.get_token( power_counter ) != cd_power )
		{
			reduced_powers = reduced_powers + receiving_powers.get_token( power_counter );
		}
	}

	sent_press_iterator->receiving_powers = reduced_powers;

	send_message_to_server( TOKEN_COMMAND_SND & reduced_powers & sent_press_iterator->press_message );
}

void BaseBot::report_failed_press( bool is_broadcast, TokenMessage &receiving_powers, TokenMessage &press_message )
{
}

void BaseBot::process_yes_snd( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	TokenMessage send_message;

	send_message = incoming_message.get_submessage( 1 );

	remove_sent_press( send_message );

	process_yes_snd_message( incoming_message, message_parameters );
}

void BaseBot::process_rej_snd( TokenMessage &incoming_message, TokenMessage &message_parameters )
{
	TokenMessage send_message;

	send_message = incoming_message.get_submessage( 1 );

	remove_sent_press( send_message );

	process_rej_snd_message( incoming_message, message_parameters );
}

void BaseBot::remove_sent_press( TokenMessage &send_message )
{
	SentPressList::iterator sent_press_iterator;
	SentPressList::iterator sent_press_iterator_copy;
	TokenMessage to_powers;
	TokenMessage press_message;

	to_powers = send_message.get_submessage( 1 );
	press_message = send_message.get_submessage( 2 );

	// Remove the message from the sent press
	sent_press_iterator = m_sent_press.begin();

	while ( sent_press_iterator != m_sent_press.end() )
	{
		sent_press_iterator_copy = sent_press_iterator;

		sent_press_iterator++;

		if ( ( sent_press_iterator_copy->receiving_powers == to_powers )
		  && ( sent_press_iterator_copy->press_message == press_message ) )
		{
			m_sent_press.erase( sent_press_iterator_copy );
		}
	}
}
