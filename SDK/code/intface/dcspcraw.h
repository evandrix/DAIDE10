/**INC+***********************************************************************/
/* Header:    dcspcraw.h                                                     */
/*                                                                           */
/* Purpose:   Raw 'codex' external header file.                              */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef DCSPCRAW_INCLUDED
#define DCSPCRAW_INCLUDED

/*****************************************************************************/
/* GROUP DCSP_MSG_TYPES.                                                     */
/*                                                                           */
/* Client-Server Protocol message types.                                     */
/*   - Initial Message                                                       */
/*   - Representation Message                                                */
/*   - Diplomacy Message                                                     */
/*   - Final Message                                                         */
/*   - Error Message                                                         */
/*   - Local Message.  This is not a CSP defined type but is used locally.   */
/*****************************************************************************/
#define DCSP_MSG_TYPES

#define DCSP_MSG_TYPE_IM                ((VOS_U8)0x00)
#define DCSP_MSG_TYPE_RM                ((VOS_U8)0x01)
#define DCSP_MSG_TYPE_DM                ((VOS_U8)0x02)
#define DCSP_MSG_TYPE_FM                ((VOS_U8)0x03)
#define DCSP_MSG_TYPE_EM                ((VOS_U8)0x04)
#define DCSP_MSG_TYPE_LM                ((VOS_U8)0xFF)
/*****************************************************************************/
/* END GROUP.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_LOCAL_EVENTS                                                   */
/*                                                                           */
/* o The CLOSE event indicates that the connection has been closed           */
/*   gracefully.                                                             */
/*                                                                           */
/* o The ERR_* events indicate that the connection has been aborted.  The    */
/*   precise event gives the reason for the abort.                           */
/*                                                                           */
/* o The CONNECT event is seen by a client when it connects or fails to      */
/*   connect to a server.  If the error code is non-zero, it has failed to   */
/*   connect so the connection should not be considered open.                */
/*                                                                           */
/* o The ACCEPTED event is seen by the server when a client connects or      */
/*   attempts to connect.  If the error code is non-zero, it has failed to   */
/*   connect so the connection should not be considered open.                */
/*****************************************************************************/
#define DCSP_LOCAL_EVENTS

/*****************************************************************************/
/* The CONNECT event is internal to the DLL.  The user will never see it.    */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_CONNECT        ((VOS_U16)0x00)

/*****************************************************************************/
/* The CONNECTED event occurs when an attempt (by a client) to connect the   */
/* server completes.  The error code reveals whether or not the attempt was  */
/* successful.                                                               */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_CONNECTED      ((VOS_U16)0x01)

/*****************************************************************************/
/* The LISTEN event is internal to the DLL.  The user will never see it.     */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_LISTEN     ((VOS_U16)0x02)

/*****************************************************************************/
/* The ERR_OUT_OF_MEM event occurs when the DLL has failed to allocate       */
/* memory to perform an operation.  As a result, a connection may fail, data */
/* may be lost or the DLL may become internally inconsistent.  The user      */
/* should immediately save all data before stopping the DLL.                 */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_ERR_OUT_OF_MEM ((VOS_U16)0x03)

/*****************************************************************************/
/* The ACCEPTED event occurs when a connection is made to the server.  The   */
/* error code reveals whether or not the connection was successfully         */
/* accepted by the server.                                                   */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_ACCEPTED       ((VOS_U16)0x04)

/*****************************************************************************/
/* The ERR_READ event occurs when the DLL fails to read data from a          */
/* connection due to an underlying connection error.  The connection is      */
/* closed as a result.                                                       */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_ERR_READ       ((VOS_U16)0x05)

/*****************************************************************************/
/* The ERR_WRITE event occurs when the DLL fails to write data to a          */
/* connection due to an underlying connection error.  The connection is      */
/* closed as a result.                                                       */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_ERR_WRITE      ((VOS_U16)0x06)

/*****************************************************************************/
/* The CLOSED event occurs when a connection is closed gracefully either by  */
/* the peer or due to a CLOSE request.                                       */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_CLOSED         ((VOS_U16)0x07)

/*****************************************************************************/
/* The CLOSE event is internal to the DLL.  The user will never see it.      */
/*****************************************************************************/
#define DCSP_LOCAL_EVENT_CLOSE          ((VOS_U16)0x08)

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_RAW_MESSAGE                                               */
/*                                                                           */
/* Description: Structure of a raw message.  This is followed by length      */
/*            bytes of data.  Each pair of bytes forms a token in NETWORK    */
/*            BYTE ORDER.                                                    */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_raw_message
{
  /***************************************************************************/
  /* The message type.                                                       */
  /***************************************************************************/
  VOS_U8 type;                                    /* One of: DCSP_MSG_TYPES; */

  /***************************************************************************/
  /* Padding so the next field is naturally aligned.                         */
  /***************************************************************************/
  VOS_U8 pad;

  /***************************************************************************/
  /* The amount of data that follows.  This is in NETWORK BYTE ORDER.        */
  /***************************************************************************/
  VOS_U16 length;

} DCSP_RAW_MESSAGE;

/**STRUCT-********************************************************************/

#endif

