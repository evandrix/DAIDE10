/**
 * Diplomacy AI Client - Part of the DAIDE project.
 *
 * Error Logging Functions.
 *
 * (C) David Norman 2002 david@ellought.demon.co.uk
 *
 * This software may be reused for non-commercial purposes without charge, and
 * without notifying the author. Use of any part of this software for commercial 
 * purposes without permission from the Author is prohibited.
 **/

#include "token_message.h"

void enable_tcpip_logging( bool enable );
void log_error( char *error, ... );
void log_tcpip_comms( bool is_incoming, TokenMessage &message );
void log_tcpip_note( const char *note );
void close_logs( void );
