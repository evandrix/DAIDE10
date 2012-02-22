/**INC+***********************************************************************/
/* Header:    dcspextn.h                                                     */
/*                                                                           */
/* Purpose:   Diplomacy Client-Server Protocol DLL external interface        */
/*            definition.                                                    */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef DCSPEXTN_INCLUDED
#define DCSPEXTN_INCLUDED

#define WIN32_LEAN_AND_MEAN
#include "winsock2.h"
#include "crtdbg.h"

/*****************************************************************************/
/* NOTE:      Diplomacy clients & servers must not define the symbol         */
/*            DCSP_EXPORTS before including this header file.                */
/*                                                                           */
/*            C++ modules must have the __cplusplus symbol defined when      */
/*            including this header.  This is done automatically by (at      */
/*            least) Microsoft C++ compilers.                                */
/*****************************************************************************/
#ifdef DCSP_EXPORTS
#ifdef __cplusplus
#define DCSP_API extern "C" __declspec(dllexport)
#else
#define DCSP_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define DCSP_API extern "C" __declspec(dllimport)
#else
#define DCSP_API __declspec(dllimport)
#endif
#endif

/*****************************************************************************/
/*                                                                           */
/* Constant Definitions.                                                     */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_SCHEDULES.                                                     */
/*****************************************************************************/
#define DCSP_SCHEDULES

#define DCSP_SCHEDULE_THREAD             ((VOS_U8)0)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_NOTIFICATIONS.                                                 */
/*****************************************************************************/
#define DCSP_NOTIFICATIONS

#define DCSP_NOTIFICATION_POLL           ((VOS_U8)0)
#define DCSP_NOTIFICATION_CALLBACK       ((VOS_U8)1)
/*****************************************************************************/
/* END GROUP.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_REPRESENTATIONS.                                               */
/*****************************************************************************/
#define DCSP_REPRESENTATIONS

#define DCSP_REPRESENTATION_TLA          ((VOS_U8)0)
#define DCSP_REPRESENTATION_HOST_ORDER   ((VOS_U8)1)
#define DCSP_REPRESENTATOIN_STRUCTURED   ((VOS_U8)2)
#define DCSP_REPRESENTATION_RAW          ((VOS_U8)255)

#define DCSP_NUM_REPRESENTATIONS         ((VOS_U8)3)
/*****************************************************************************/
/* END GROUP.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_ADDR_FORMATS.                                                  */
/*****************************************************************************/
#define DCSP_ADDR_FORMATS

#define DCSP_ADDR_FORMAT_DOTTED_DECIMAL  ((VOS_U8)0)
#define DCSP_ADDR_FORMAT_HOST_NAME       ((VOS_U8)1)
#define DCSP_ADDR_FORMAT_WILD            ((VOS_U8)2)
/*****************************************************************************/
/* END GROUP.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_TRACE_LEVELS.                                                  */
/*****************************************************************************/
#define DCSP_TRACE_LEVELS

#define DCSP_TRACE_LEVEL_NONE            ((VOS_U8)0)
#define DCSP_TRACE_LEVEL_FUNCTION        ((VOS_U8)20)
#define DCSP_TRACE_LEVEL_PARAMETER       ((VOS_U8)40)
#define DCSP_TRACE_LEVEL_BRANCH          ((VOS_U8)60)
#define DCSP_TRACE_LEVEL_DETAIL          ((VOS_U8)80)

#define DCSP_TRACE_LEVEL_NORMAL          ((VOS_U8)80)
#define DCSP_TRACE_LEVEL_MAXIMUM         ((VOS_U8)100)
/*****************************************************************************/
/* END GROUP.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Type Definitions.                                                         */
/*                                                                           */
/*****************************************************************************/

/**CALLBACK+******************************************************************/
/*                                                                           */
/* Callback:   DCSP_MESSAGE_CALLBACK                                         */
/*                                                                           */
/* Purpose:    User callback to receive messages from the DLL.               */
/*                                                                           */
/* Parameters: IN     msg         - The received message.                    */
/*             IN     length      - The length of the message.               */
/*             IN     conn_id     - The connection ID.                       */
/*             IN     data        - The callback data passed to the DLL in   */
/*                                  the opt structure.                       */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**CALLBACK-******************************************************************/
typedef VOS_VOID(DCSP_MESSAGE_CALLBACK)(VOS_CHAR *,
                                        VOS_S32,
                                        VOS_S32,
                                        VOS_VOID *);

/*****************************************************************************/
/*                                                                           */
/* Structure Definitions.                                                    */
/*                                                                           */
/*****************************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_USAGE_OPTIONS                                             */
/*                                                                           */
/* Description: Structure passed to the DLL in a call to dcsp_start which    */
/*            specifies the options to be used.                              */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_usage_options
{
  /***************************************************************************/
  /* Whether the DLL should start in client or server mode.  TRUE for client */
  /* mode, FALSE for server mode.                                            */
  /***************************************************************************/
  VOS_BOOL client;

  /***************************************************************************/
  /* The scheduling option.                                                  */
  /***************************************************************************/
  VOS_U8 scheduling;                              /* One of: DCSP_SCHEDULES; */

  /***************************************************************************/
  /* The notification option.                                                */
  /***************************************************************************/
  VOS_U8 notification;                        /* One of: DCSP_NOTIFICATIONS; */

  /***************************************************************************/
  /* The message representation.                                             */
  /***************************************************************************/
  VOS_U8 representation;                    /* One of: DCSP_REPRESENTATIONS; */

  /***************************************************************************/
  /* For clients, the address of the server.  For servers, the local address */
  /* that the DLL should listen for incoming connections on.  See            */
  /* dcsp_convert_address for further details.                               */
  /***************************************************************************/
  SOCKADDR_IN address;

  /***************************************************************************/
  /* Message callback function.  Only valid when notification is _CALLBACK.  */
  /***************************************************************************/
  DCSP_MESSAGE_CALLBACK *message_callback;

  /***************************************************************************/
  /* Application specific data which will be passed into the callback        */
  /* function.  This allows the user to get to its own locking and queue (or */
  /* other) data.                                                            */
  /***************************************************************************/
  VOS_VOID *message_callback_data;

  /***************************************************************************/
  /* Pointer to an internal structure used by the DLL for the storage of the */
  /* Representation Message to send to clients.                              */
  /*                                                                         */
  /* This field must not be modified by clients or servers.  It is filled in */
  /* automatically by the DLL when the server calls dcsp_server_set_rm.      */
  /***************************************************************************/
  VOS_VOID *rm_data;

  /***************************************************************************/
  /***************************************************************************/
  /* Debug data.                                                             */
  /***************************************************************************/
  /***************************************************************************/

  /***************************************************************************/
  /* Whether or not connection logging should take place.  Connection        */
  /* logging produces a binary file for each network connection made.  The   */
  /* file contains all the information that was sent and received over the   */
  /* connection.                                                             */
  /*                                                                         */
  /* Recommended Value: TRUE for debug versions, FALSE for release versions. */
  /*                                                                         */
  /***************************************************************************/
  VOS_BOOL connection_logging;

  /***************************************************************************/
  /* Level of diagnostics that should be traced.  Ranges from 100 (full      */
  /* diagnostics) to 0 (no diagnostics).                                     */
  /*                                                                         */
  /* Default Value: 100 for debug versions, 0 for release verions.           */
  /*                                                                         */
  /***************************************************************************/
  VOS_U8 trace_level;

  /***************************************************************************/
  /* Name of the file (in the working directory) to which trace will be      */
  /* output.                                                                 */
  /*                                                                         */
  /* Default Value: trace.txt                                                */
  /*                                                                         */
  /***************************************************************************/
  VOS_CHAR *trace_filename;

  /***************************************************************************/
  /* Size the the log file is permitted to grow to before it is flipped over */
  /* to a backup and a new file written.  Therefore, the total diskspace     */
  /* that can be used by tracing is twice the value specified here.          */
  /*                                                                         */
  /* Default Value: 5000000                                                  */
  /*                                                                         */
  /***************************************************************************/
  VOS_U32 trace_size;

  /***************************************************************************/
  /* Filename filters for tracing.  Tracing from all source files happens by */
  /* default (if both trace strings are simply a null-terminator).  If       */
  /* trace_filter_in is specified then only those source files whose prefix  */
  /* matches one of the comma seperated filter items are traced.  From that  */
  /* set (either all or those specified by filter_in), those in filter_out   */
  /* are then excluded.                                                      */
  /*                                                                         */
  /* Recommended Value (in) : dcsp,vos                                       */
  /* Recommended Value (out): dcspcdx                                        */
  /*                                                                         */
  /* If the application makes use of the VOS_TRACE_* macros, consider        */
  /* leaving filter_in blank (for tracing of all funtions) or specifying     */
  /* your own.                                                               */
  /*                                                                         */
  /***************************************************************************/
  VOS_CHAR *trace_filter_in;
  VOS_CHAR *trace_filter_out;

} DCSP_USAGE_OPTIONS;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Function Prototypes.                                                      */
/*                                                                           */
/*****************************************************************************/

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_configure                                                */
/*                                                                           */
/* Purpose:    Get the default configuration and any user overrides.         */
/*                                                                           */
/* Parameters: IN     filename    - The file to read.                        */
/*                                                                           */
/* Returns:    An options structure with the combined configuration.  NULL   */
/*             if memory could not be allocated.                             */
/*                                                                           */
/* Notes:      Configuration files have lines of the form "key = value".     */
/*             The only other valid lines are blank lines and lines          */
/*             beginning with ";", which are treated as comments.            */
/*                                                                           */
/*             The returned structure can be read and written by the caller  */
/*             until the call to dcsp_start.  At this point, the caller must */
/*             not make any further access to the structure.                 */
/*                                                                           */
/*             At start-up, an application should do the following.          */
/*                                                                           */
/*             1. Call dcsp_configure to initialise the configuration and    */
/*                read user-specified values from a configuration file.      */
/*             2. Optionally provide a UI for the user to change any of the  */
/*                values specified in the file.  If an application does      */
/*                this, it should present the options structure from step 1  */
/*                as the default for the user.                               */
/*             3. Complete the non-user configuration (e.g. client/server).  */
/*             4. Perform any other pre-start initialisation.                */
/*             5. Call dcsp_start (below).                                   */
/*                                                                           */
/*             This example would make a sensible default configuration file */
/*             to ship with debug versions of your application.              */
/* ------                                                                    */
/*                                                                           */
/* ; Enable per-connection logging of all network trafic.                    */
/* ;                                                                         */
/* connection_logging = 1                                                    */
/*                                                                           */
/* ; Turn on full tracing of the internals of the application except for the */
/* ; codexes which are well tested and produce very verbose trace that is    */
/* ; likely to obscure any relevant information.                             */
/* ;                                                                         */
/* trace_level = 100                                                         */
/* trace_filename = trace.txt                                                */
/* trace_flip_size = 5000000                                                 */
/* trace_filter_in =                                                         */
/* trace_filter_out = dcspcdx                                                */
/*                                                                           */
/* ------                                                                    */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API DCSP_USAGE_OPTIONS* dcsp_configure(VOS_CHAR *);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_start                                                    */
/*                                                                           */
/* Purpose:    Start the DLL.                                                */
/*                                                                           */
/* Parameters: IN     options     - The usage options structure returned by  */
/*                                  the call to dcsp_configure.              */
/*                                                                           */
/* Returns:    TRUE if the DLL started, FALSE otherwise.                     */
/*                                                                           */
/* User:       This function can be called by both the client and server.    */
/*                                                                           */
/* Notes:      dcsp_configure must be called to get a DCSP_USAGE_OPTIONS     */
/*             structure.  (The caller must not allocate this structure for  */
/*             themselves.)  dcsp_configure is the only dcsp_ function that  */
/*             can be called before dcsp_start.                              */
/*                                                                           */
/*             If this function returns TRUE then the caller must not read   */
/*             or write the options structure any more.                      */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_BOOL dcsp_start(DCSP_USAGE_OPTIONS *);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_stop                                                     */
/*                                                                           */
/* Purpose:    Stop the client DLL.                                          */
/*                                                                           */
/* Parameters: None.                                                         */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/* User:       This function can be called by the client or the server.      */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_VOID dcsp_stop();

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_convert_address                                          */
/*                                                                           */
/* Purpose:    Convert an address from host name (e.g. "localhost") or       */
/*             dotted decimal (e.g. 127.0.0.1).                              */
/*                                                                           */
/*                                                                           */
/* Parameters: IN     addr        - The address string to convert.           */
/*             IN     format      - The format of the address string.  One   */
/*                                  of DCSP_ADDR_FORMATS.                    */
/*             IN     port        - The port to use.                         */
/*                                                                           */
/* Returns:    TRUE if the address was converted.  FALSE otherwise.          */
/*                                                                           */
/* User:       This function can be called by the client or the server.      */
/*                                                                           */
/* Notes:      This function must be called after dcsp_start and before      */
/*             dcsp_client_connect or dcsp_server_listen.                    */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_BOOL dcsp_convert_address(VOS_CHAR *, VOS_U8, VOS_U16);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_convert_message_format                                   */
/*                                                                           */
/* Purpose:    Convert a message from one format to another.                 */
/*                                                                           */
/* Parameters: IN     input       - The message to convert.                  */
/*             IN     input_format                                           */
/*                                - The format of the message to convert.    */
/*             OUT    output      - Buffer to put the converted message in.  */
/*             IN     output_format                                          */
/*                                - The desired format for the message.      */
/*             IN     max_length  - The maximum output length (in bytes)     */
/*                                  that will fit in the output buffer.      */
/*                                                                           */
/* Returns:    The length of the converted message.  Note that if this is    */
/*             greater than max_length then the message has not been         */
/*             converted.                                                    */
/*                                                                           */
/* User:       This function can be called by the client or the server.      */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_U32 dcsp_convert_message_format(VOS_CHAR *,
                                             VOS_U8,
                                             VOS_CHAR *,
                                             VOS_U8,
                                             VOS_U16);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_client_connect                                           */
/*                                                                           */
/* Purpose:    Attempt to make a connection to the server.                   */
/*                                                                           */
/* Parameters: None.                                                         */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/* User:       This is a client-only function.                               */
/*                                                                           */
/* Notes:      Before calling this function, the client must have called     */
/*             dcsp_convert_address or filled in the server address details  */
/*             in the option structure.                                      */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_VOID dcsp_client_connect();

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_client_receive_message                                   */
/*                                                                           */
/* Purpose:    Take the top message off the queue, copy it into the user     */
/*             supplied buffer and free the message CB.                      */
/*                                                                           */
/* Parameters: IN     buffer      - Buffer which the data should be copied   */
/*                                  into.  If NULL, no data is copied but    */
/*                                  the return value is still set up.        */
/*                                                                           */
/* Returns:    The length of the first message on the queue or 0 if there    */
/*             are no messages waiting.                                      */
/*                                                                           */
/* User:       This is a client-only function.                               */
/*                                                                           */
/* Notes:      The buffer must be large enough to have the entire message    */
/*             copied into it.  The client can call this function with a     */
/*             NULL buffer to determine the size of the buffer that it will  */
/*             need to allocate before calling it again.  Alternatively,     */
/*             the user could call the function with a buffer that it knows  */
/*             will always be large enough.                                  */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_S32 dcsp_client_receive_message(VOS_CHAR *);
DCSP_API VOS_S32 dcsp_client_receive_message_vb(VOS_CHAR **);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_client_send_message                                      */
/*                                                                           */
/* Purpose:    Send a message to the server.                                 */
/*                                                                           */
/* Parameters: IN     buffer      - The message to send.                     */
/*             IN     length      - The length of the message.               */
/*                                                                           */
/* Returns:    TRUE if resources were available to queue the message for     */
/*             sending.  FALSE otherwise.                                    */
/*                                                                           */
/* User:       This is a client-only function.                               */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_BOOL dcsp_client_send_message(VOS_CHAR *, VOS_S32);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_server_set_rm                                            */
/*                                                                           */
/* Purpose:    Set's the RM that the server will send to each client when it */
/*             connects.                                                     */
/*                                                                           */
/* Parameters: IN     rm          - The RM exactly as it should go out on    */
/*                                  the wire.  The tokens must be in NETWORK */
/*                                  BYTE ORDER.                              */
/*                                                                           */
/* Returns:    TRUE if the the DLL was able to set the RM, FALSE if it       */
/*             failed to allocated the necessary resources.                  */
/*                                                                           */
/* User:       This is a server-only function.                               */
/*                                                                           */
/* Notes:      This function must be called before the server calls          */
/*             dcsp_server_listen (to begin accepting connections).          */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_BOOL dcsp_server_set_rm(VOS_S8 *);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_server_use_default_address                               */
/*                                                                           */
/* Purpose:    Instruct the server to use the default IP address of the      */
/*             machine on which it is running.                               */
/*                                                                           */
/* Parameters: IN     port        - The port to listen on.                   */
/*                                                                           */
/* Returns:    TRUE if DLL could determine the default address.  FALSE       */
/*             otherwise.                                                    */
/*                                                                           */
/* User:       This is a server-only function.                               */
/*                                                                           */
/* Notes:      If called, this function must be called before the server     */
/*             calls dcsp_server_listen (to begin accepting connections).    */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_BOOL dcsp_server_use_default_address(VOS_U16);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_server_listen                                            */
/*                                                                           */
/* Purpose:    Start listening for connections.                              */
/*                                                                           */
/* Parameters: None.                                                         */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/* User:       This is a server-only function.                               */
/*                                                                           */
/* Notes:      Before calling this function, the server must have called     */
/*             dcsp_convert_address or filled in the address details in the  */
/*             option structure.  The address must be an address of the      */
/*             local host.  Alternatively, the server may call               */
/*             dcsp_server_use_default_address.                              */
/*                                                                           */
/*             Before calling this function, the server must also have       */
/*             called dcsp_server_set_rm.                                    */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_VOID dcsp_server_listen();

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_server_receive_message                                   */
/*                                                                           */
/* Purpose:    Take the top message off the queue, copy it into the user     */
/*             supplied buffer and free the message CB.                      */
/*                                                                           */
/* Parameters: IN     buffer      - Buffer which the data should be copied   */
/*                                  into.  If NULL, no data is copied but    */
/*                                  the return value is still set up.        */
/*             OUT    conn_id     - DLL indentifier for the connection.      */
/*                                                                           */
/* Returns:    The length of the first message on the queue or 0 if there    */
/*             are no messages waiting.                                      */
/*                                                                           */
/* User:       This is a server-only function.                               */
/*                                                                           */
/* Notes:      The buffer must be large enough to have the entire message    */
/*             copied into it.  The server can call this function with a     */
/*             NULL buffer to determine the size of the buffer that it will  */
/*             need to allocate before calling it again.  Alternatively,     */
/*             the server could call the function with a buffer that it      */
/*             knows will always be large enough.                            */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_S32 dcsp_server_receive_message(VOS_CHAR *, VOS_S32 *);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_server_send_message                                      */
/*                                                                           */
/* Purpose:    Send a message to the server a client.                        */
/*                                                                           */
/* Parameters: IN     buffer      - The message to send.                     */
/*             IN     length      - The length of the message.               */
/*             IN     conn_id     - The conn_id as returned on a call to     */
/*                                  dcsp_server_receive_message.             */
/*                                                                           */
/* Returns:    TRUE if resources were available to queue the message for     */
/*             sending.  FALSE otherwise.                                    */
/*                                                                           */
/* User:       This is a server-only function.                               */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_BOOL dcsp_server_send_message(VOS_CHAR *, VOS_S32, VOS_S32);

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  dcsp_server_close                                             */
/*                                                                           */
/* Purpose:    Forecefully close a connection.                               */
/*                                                                           */
/*             IN     conn_id     - The conn_id as returned on a call to     */
/*                                  dcsp_server_receive_message.             */
/*                                                                           */
/* Returns:    TRUE if resources were available to close the connection.     */
/*             FALSE otherwise.                                              */
/*                                                                           */
/* User:       This is a server-only function.                               */
/*                                                                           */
/* Notes:      The normal way to close a connection is to send an OFF        */
/*             message to the client and wait for it to close the            */
/*             connection.  However, if the server needs to forcefully close */
/*             the connection (having waited for a suitable amount of time   */
/*             after sending the OFF message, it can call this function to   */
/*             close the underlying connection.                              */
/*                                                                           */
/**PROC-**********************************************************************/
DCSP_API VOS_BOOL dcsp_server_close(VOS_S32);

DCSP_API VOS_VOID __cdecl dcsp_trace_branch(VOS_CHAR *, ...);

#endif

