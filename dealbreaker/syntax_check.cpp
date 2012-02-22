/**
 * Diplomacy AI Server - Part of the DAIDE project.
 *
 * Syntax Checker. Checks a message against the language definition.
 *
 * (C) David Norman 2002 david@ellought.demon.co.uk
 *
 * This software may be reused for non-commercial purposes without charge, and
 * without notifying the author. Use of any part of this software for commercial 
 * purposes without permission from the Author is prohibited.
 **/

#include "stdafx.h"

#define DECLARE_SYNTAX
#include "syntax_check.h"
#include "token_text_map.h"

const int MAX_LANGUAGE_REPRESENTATION_LEN = 1000;
const int MAX_TOKEN_LEN = 20;

const LANGUAGE_CATEGORY CATEGORY_CATEGORY = 0x58;
const LANGUAGE_CATEGORY CATEGORY_SUBMESSAGE = 0x59;
const LANGUAGE_CATEGORY CATEGORY_INTERNAL = 0x5A;
const LANGUAGE_CATEGORY CATEGORY_SUBTOKEN = 0x5B;

const LANGUAGE_TOKEN TOKEN_INTERNAL_REPEAT = 0x5A00;
const LANGUAGE_TOKEN TOKEN_INTERNAL_ANY = 0x5A01;
const LANGUAGE_TOKEN TOKEN_INTERNAL_STRING = 0x5A02;
const LANGUAGE_TOKEN TOKEN_INTERNAL_NUMBER = 0x5A03;
const LANGUAGE_TOKEN TOKEN_INTERNAL_PROVINCE = 0x5A04;

int check_message_against_syntax( TokenMessage &message, DAIDE_LANGUAGE_MAP &language, int language_level, Token category );
int check_message_against_syntax_option( TokenMessage &message, DAIDE_LANGUAGE_MAP &language, TokenMessage &syntax, int language_level );

DaideLanguage *DaideLanguage::m_instance = NULL;

DaideLanguage *DaideLanguage::instance( void )
{
	if ( m_instance == NULL )
	{
		m_instance = new DaideLanguage;
	}

	return m_instance;
}

DaideLanguage::DaideLanguage( void )
{
	FILE *language_file;
	char line[ MAX_LANGUAGE_REPRESENTATION_LEN ];
	char *next_token;
	Token syntax[ MAX_LANGUAGE_REPRESENTATION_LEN ];
	SyntaxOption syntax_option;
	Token key_token;
	int language_level;
	int token_counter;

	language_file = fopen("server_language.txt", "r");

	if ( language_file != NULL )
	{
		fgets( line, MAX_LANGUAGE_REPRESENTATION_LEN, language_file );

		while ( !feof( language_file ) )
		{
			next_token = get_key_token( line, key_token );

			if ( next_token != NULL )
			{
				next_token = get_int( next_token, language_level );

				token_counter = 0;

				while ( next_token != NULL )
				{
					next_token = get_token( next_token, syntax[ token_counter ] );

					token_counter++;
				}

				syntax_option.language_level = language_level;
				syntax_option.syntax.set_message( syntax, token_counter );

				m_daide_language.insert( DAIDE_LANGUAGE_MAP::value_type( key_token, syntax_option ) );
			}

			fgets( line, MAX_LANGUAGE_REPRESENTATION_LEN, language_file );
		}

		fclose( language_file );
	}
}

char *DaideLanguage::get_token( char *text, Token &token )
{
	char *next_token = text;
	char token_text[ MAX_TOKEN_LEN ];
	int token_counter = 0;
	TokenTextMap *token_map = TokenTextMap::instance();
	TEXT_TO_TOKEN_MAP::iterator token_iterator;

	while ( isspace( *next_token ) )
	{
		next_token++;
	}

	while ( ( !isspace( *next_token ) ) && ( *next_token != '\0' ) && ( *next_token != ';' )
		 && ( token_counter < 20 ) )
	{
		token_text[ token_counter ] = *next_token;
		token_counter++;
		next_token++;
	}

	token_text[ token_counter ] = '\0';

	while ( isspace( *next_token ) )
	{
		next_token++;
	}

	if ( ( *next_token == '\0' ) || ( *next_token == ';' ) )
	{
		next_token = NULL;
	}

	if ( token_text[ 0 ] == '[' )
	{
		if ( strnicmp( token_text, "[cat", 4 ) == 0 )
		{
			token = strtol( &token_text[4], NULL, 16 ) | ( CATEGORY_CATEGORY << 8 );
		}
		else if ( strnicmp( token_text, "[sub", 4 ) == 0 )
		{
			token = strtol( &token_text[4], NULL, 16 ) | ( CATEGORY_SUBMESSAGE << 8 );
		}
		else if ( stricmp( token_text, "[repeat]" ) == 0 )
		{
			token = TOKEN_INTERNAL_REPEAT;
		}
		else if ( stricmp( token_text, "[any]" ) == 0 )
		{
			token = TOKEN_INTERNAL_ANY;
		}
		else if ( stricmp( token_text, "[string]" ) == 0 )
		{
			token = TOKEN_INTERNAL_STRING;
		}
		else if ( stricmp( token_text, "[number]" ) == 0 )
		{
			token = TOKEN_INTERNAL_NUMBER;
		}
		else if ( stricmp( token_text, "[province]" ) == 0 )
		{
			token = TOKEN_INTERNAL_PROVINCE;
		}
		else if ( strnicmp( token_text, "[token", 6 ) == 0 )
		{
			token = strtol( &token_text[6], NULL, 16 ) | ( CATEGORY_SUBTOKEN << 8 );
		}
		else
		{
			token = TOKEN_END_OF_MESSAGE;
		}
	}
	else if ( token_counter == 3 )
	{
		token_iterator = token_map->m_text_to_token_map.find( token_text );

		if ( token_iterator == token_map->m_text_to_token_map.end() )
		{
			token = TOKEN_END_OF_MESSAGE;
		}
		else
		{
			token = token_iterator->second;
		}
	}
	else if ( token_counter == 4 )
	{
		token = strtol( token_text, NULL, 16 );
	}
	else if ( token_counter != 0 )
	{
		token = TOKEN_END_OF_MESSAGE;
	}

	return next_token;
}

char *DaideLanguage::get_key_token( char *text, Token &token )
{
	char *next_token = text;
	char token_text[ MAX_TOKEN_LEN ];
	int token_counter = 0;
	TokenTextMap *token_map = TokenTextMap::instance();
	TEXT_TO_TOKEN_MAP::iterator token_iterator;

	while ( isspace( *next_token ) )
	{
		next_token++;
	}

	while ( ( !isspace( *next_token ) ) && ( *next_token != '\0' ) && ( *next_token != ';' )
		 && ( token_counter < 20 ) )
	{
		token_text[ token_counter ] = *next_token;
		token_counter++;
		next_token++;
	}

	token_text[ token_counter ] = '\0';

	while ( isspace( *next_token ) )
	{
		next_token++;
	}

	if ( ( *next_token == '\0' ) || ( *next_token == ';' ) )
	{
		next_token = NULL;
	}

	token = ( strtol( token_text, NULL, 16 ) ) | ( CATEGORY_SUBMESSAGE << 8 );

	return next_token;
}

char *DaideLanguage::get_int( char *text, int &value )
{
	char *next_token = text;
	int sign = 1;

	value = 0;

	while ( ( !isdigit( *next_token ) ) && ( *next_token != '\0' ) 
		 && ( *next_token != ';' ) && ( *next_token != '-' ) )
	{
		next_token++;
	}

	if ( *next_token == '-' )
	{
		sign = -1;
		next_token++;
	}

	while ( isdigit( *next_token ) )
	{
		value = value * 10 + ( *next_token - '0' );
		next_token++;
	}

	while ( ( isspace( *next_token ) ) && ( *next_token != '\0' ) )
	{
		next_token++;
	}

	if ( ( *next_token == '\0' ) || ( *next_token == ';' ) )
	{
		next_token = NULL;
	}

	value = value * sign;

	return next_token;
}

int check_message_against_language( TokenMessage &message, int language_level )
{
	DaideLanguage *language = DaideLanguage::instance();
	 
	return check_message_against_syntax( message, language->m_daide_language, language_level, CATEGORY_SUBMESSAGE << 8 );
}

int check_message_against_syntax( TokenMessage &message, DAIDE_LANGUAGE_MAP &language, int language_level, Token category )
{
	int best_error_location = 0;
	int error_location;
	DAIDE_LANGUAGE_MAP::iterator syntax_option_iterator;

	for ( syntax_option_iterator = language.lower_bound( category );
		  syntax_option_iterator != language.upper_bound( category );
		  syntax_option_iterator++ )
	{
		if ( syntax_option_iterator->second.language_level <= language_level )
		{
			error_location = check_message_against_syntax_option( message, language, syntax_option_iterator->second.syntax, language_level );

			if ( ( error_location == ADJUDICATOR_NO_ERROR )
			  || ( ( best_error_location != ADJUDICATOR_NO_ERROR )
				&& ( error_location > best_error_location ) ) )
			{
				best_error_location = error_location;
			}
		}
	}

	return best_error_location;
}

int check_message_against_syntax_option( TokenMessage &message, DAIDE_LANGUAGE_MAP &language, TokenMessage &syntax, int language_level )
{
	int best_error_location = 0;
	int error_location;
	int syntax_counter = 0;
	int message_counter = 0;
	int repeat_syntax_start = 0;
	int repeat_message_start = 0;
	bool process_repeat = false;
	Token syntax_token;
	Token message_token;
	TokenMessage submessage;
	int sub_error_location;
	int syntax_error_location;

	do
	{
		if ( process_repeat )
		{
			syntax_counter = repeat_syntax_start;
			message_counter = repeat_message_start;

			repeat_message_start++;
		}

		error_location = ADJUDICATOR_NO_ERROR;
		syntax_error_location = ADJUDICATOR_NO_ERROR;

		while ( ( error_location == ADJUDICATOR_NO_ERROR ) 
			 && ( message_counter < message.get_submessage_count() )
			 && ( syntax_counter < syntax.get_message_length() ) )
		{
			syntax_token = syntax.get_token( syntax_counter );

			if ( syntax_token.get_category() == CATEGORY_CATEGORY )
			{
				if ( message.submessage_is_single_token( message_counter ) == false )
				{
					error_location = message.get_submessage_start( message_counter ) - 1;
					syntax_error_location = syntax_counter;
				}
				else
				{
					message_token = message.get_submessage( message_counter ).get_token();

					if ( message_token.get_category() != syntax_token.get_subtoken() )
					{
						error_location = message.get_submessage_start( message_counter );
						syntax_error_location = syntax_counter;
					}
				}

				message_counter++;
				syntax_counter++;
			}
			else if ( syntax_token.get_category() == CATEGORY_SUBMESSAGE )
			{
				if ( message.submessage_is_single_token( message_counter ) )
				{
					error_location = message.get_submessage_start( message_counter );
					syntax_error_location = syntax_counter;
				}
				else
				{
					submessage = message.get_submessage( message_counter );

					sub_error_location = check_message_against_syntax( submessage, language, language_level, syntax_token );

					if ( sub_error_location != ADJUDICATOR_NO_ERROR )
					{
						error_location = sub_error_location + message.get_submessage_start( message_counter );
						syntax_error_location = syntax_counter;
					}
				}

				message_counter++;
				syntax_counter++;
			}
			else if ( syntax_token.get_category() == CATEGORY_INTERNAL )
			{
				if ( syntax_token == TOKEN_INTERNAL_REPEAT )
				{
					syntax_counter++;
					process_repeat = true;
					repeat_syntax_start = syntax_counter;
					repeat_message_start = message_counter + 1;
				}
				else if ( syntax_token == TOKEN_INTERNAL_ANY )
				{
					message_counter = message.get_submessage_count();
					syntax_counter++;
				}
				else if ( syntax_token == TOKEN_INTERNAL_STRING )
				{
					if ( message.submessage_is_single_token( message_counter ) == false )
					{
						error_location = message.get_submessage_start( message_counter ) - 1;
						syntax_error_location = syntax_counter;
					}
					else
					{
						message_token = message.get_submessage( message_counter ).get_token();

						if ( message_token.get_category() != CATEGORY_ASCII )
						{
							error_location = message.get_submessage_start( message_counter );
							syntax_error_location = syntax_counter;
						}
					}

					while ( ( message.submessage_is_single_token( message_counter ) )
						 && ( message.get_submessage( message_counter ).get_token().get_category() == CATEGORY_ASCII ) )
					{
						message_counter++;
					}

					syntax_counter++;
				}
				else if ( syntax_token == TOKEN_INTERNAL_NUMBER )
				{
					if ( message.submessage_is_single_token( message_counter ) == false )
					{
						error_location = message.get_submessage_start( message_counter ) - 1;
						syntax_error_location = syntax_counter;
					}
					else
					{
						message_token = message.get_submessage( message_counter ).get_token();

						if ( message_token.is_number() == false )
						{
							error_location = message.get_submessage_start( message_counter );
							syntax_error_location = syntax_counter;
						}
					}

					message_counter++;
					syntax_counter++;
				}
				else if ( syntax_token == TOKEN_INTERNAL_PROVINCE )
				{
					if ( message.submessage_is_single_token( message_counter ) == false )
					{
						error_location = message.get_submessage_start( message_counter ) - 1;
						syntax_error_location = syntax_counter;
					}
					else
					{
						message_token = message.get_submessage( message_counter ).get_token();

						if ( message_token.is_province() == false )
						{
							error_location = message.get_submessage_start( message_counter );
							syntax_error_location = syntax_counter;
						}
					}

					message_counter++;
					syntax_counter++;
				}
				else
				{
					error_location = message.get_submessage_start( message_counter );
					syntax_error_location = syntax_counter;
				}
			}
			else if ( syntax_token.get_category() == CATEGORY_SUBTOKEN )
			{
				submessage = message.get_submessage( message_counter );

				if ( message.submessage_is_single_token( message_counter ) == false )
				{
					submessage.enclose_this();
				}

				syntax_token = ( CATEGORY_SUBMESSAGE << 8 ) | ( syntax_token.get_subtoken() );

				sub_error_location = check_message_against_syntax( submessage, language, language_level, syntax_token );

				if ( sub_error_location != ADJUDICATOR_NO_ERROR )
				{
					error_location = sub_error_location + message.get_submessage_start( message_counter );
					syntax_error_location = syntax_counter;

					if ( message.submessage_is_single_token( message_counter ) == false )
					{
						error_location--;
					}
				}

				message_counter++;
				syntax_counter++;
			}
			else // Individual token
			{
				if ( message.submessage_is_single_token( message_counter ) == false )
				{
					error_location = message.get_submessage_start( message_counter ) - 1;
					syntax_error_location = syntax_counter;
				}
				else
				{
					message_token = message.get_submessage( message_counter ).get_token();

					if ( message_token != syntax_token )
					{
						error_location = message.get_submessage_start( message_counter );
						syntax_error_location = syntax_counter;
					}
				}

				message_counter++;
				syntax_counter++;
			}
		}

		if ( error_location == ADJUDICATOR_NO_ERROR ) 
		{
			if ( message_counter != message.get_submessage_count() )
			{
				// End of syntax reached, but still more tokens in the message

				error_location = message.get_submessage_start( message_counter );
				
				syntax_error_location = syntax_counter;
			}
			else if ( syntax_counter != syntax.get_message_length() )
			{
				// End of message reached, but still more tokens in the syntax

				error_location = message.get_message_length();

				syntax_error_location = syntax_counter;
			}
		}

		if ( ( error_location == ADJUDICATOR_NO_ERROR )
		  || ( ( best_error_location != ADJUDICATOR_NO_ERROR )
			&& ( error_location > best_error_location ) ) )
		{
			best_error_location = error_location;
		}

		if ( ( process_repeat ) && ( syntax_error_location == repeat_syntax_start ) )
		{
			process_repeat = false;
		}
	}
	while ( process_repeat );

	return best_error_location;
}

