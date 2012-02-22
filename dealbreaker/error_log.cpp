/**
 * Diplomacy AI Client - Part of the DAIDE project.
 *
 * Error Logging
 *
 * (C) David Norman 2002 david@ellought.demon.co.uk
 *
 * This software may be reused for non-commercial purposes without charge, and
 * without notifying the author. Use of any part of this software for commercial 
 * purposes without permission from the Author is prohibited.
 **/

#include "stdafx.h"
#include "error_log.h"

FILE *error_log = NULL;
FILE *tcpip_log = NULL;
bool tcpip_logging = false;

const char ERROR_LOG_FILENAME[] = "errorlog.txt";
const char TCPIP_LOG_FILENAME[] = "tcpiplog.txt";

void enable_tcpip_logging( bool enable )
{
	tcpip_logging = enable;
}

void log_error( char *error, ... )
{
	va_list arg_list;

	va_start( arg_list, error );

	if ( error_log == NULL )
	{
		error_log = fopen( ERROR_LOG_FILENAME, "w");
	}

	if ( error_log != NULL )
	{
		vfprintf( error_log, error, arg_list );
		fprintf( error_log, "\n");

		fflush( error_log );
	}

	#ifdef _DEBUG
	{
		static char error_text[ 10000 ];

		vsprintf( error_text, error, arg_list );

//		AfxMessageBox( error_text );
	}
	#endif

	va_end( arg_list );
}

void log_tcpip_comms( bool is_incoming, TokenMessage &message )
{
	if ( tcpip_logging )
	{
		if ( tcpip_log == NULL )
		{
			tcpip_log = fopen( TCPIP_LOG_FILENAME, "w");
		}

		if ( tcpip_log != NULL )
		{
			if ( is_incoming )
			{
				fprintf( tcpip_log, ">> ");
			}
			else
			{
				fprintf( tcpip_log, "<< ");
			}

			fprintf( tcpip_log, "%s\n", message.get_message_as_text().c_str() );

			fflush( tcpip_log );
		}
	}
}

void log_tcpip_note( const char *note )
{
	if ( tcpip_logging )
	{
		if ( tcpip_log == NULL )
		{
			tcpip_log = fopen( TCPIP_LOG_FILENAME, "w");
		}

		if ( tcpip_log != NULL )
		{
			fprintf( tcpip_log, "== %s\n", (const char *)( note ) );

			fflush( tcpip_log );
		}
	}
}

void close_logs( void )
{
	if ( error_log != NULL )
	{
		fclose( error_log );
	}

	if ( tcpip_log != NULL )
	{
		fclose( tcpip_log );
	}
}
