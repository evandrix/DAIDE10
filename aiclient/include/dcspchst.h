/**INC+***********************************************************************/
/* Header:    dcspchst.h                                                     */
/*                                                                           */
/* Purpose:   Host byte order codex external header file.                    */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef DCSPCHST_INCLUDED
#define DCSPCHST_INCLUDED

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
/* GROUP DCSP_HST_CATS                                                       */
/*                                                                           */
/* Client-Server Protocol categories.                                        */
/*****************************************************************************/
#define DCSP_HST_CATS

#define DCSP_HST_CAT_MISC               ((VOS_U8)0x40)
#define DCSP_HST_CAT_POWER              ((VOS_U8)0x41)
#define DCSP_HST_CAT_UNIT_TYPE          ((VOS_U8)0x42)
#define DCSP_HST_CAT_ORDER              ((VOS_U8)0x43)
#define DCSP_HST_CAT_ORDER_NOTE         ((VOS_U8)0x44)
#define DCSP_HST_CAT_RESULT             ((VOS_U8)0x45)
#define DCSP_HST_CAT_COAST              ((VOS_U8)0x46)
#define DCSP_HST_CAT_PHASE              ((VOS_U8)0x47)
#define DCSP_HST_CAT_COMMAND            ((VOS_U8)0x48)
#define DCSP_HST_CAT_PARAMETER          ((VOS_U8)0x49)
#define DCSP_HST_CAT_PRESS              ((VOS_U8)0x4A)
#define DCSP_HST_CAT_STRING             ((VOS_U8)0x4B)

#define DCSP_HST_CAT_PROV_LAND_NSC      ((VOS_U8)0x50)
#define DCSP_HST_CAT_PROV_LAND_SC       ((VOS_U8)0x51)
#define DCSP_HST_CAT_PROV_WATER_NSC     ((VOS_U8)0x52)
#define DCSP_HST_CAT_PROV_WATER_SC      ((VOS_U8)0x53)
#define DCSP_HST_CAT_PROV_COAST_NSC     ((VOS_U8)0x54)
#define DCSP_HST_CAT_PROV_COAST_SC      ((VOS_U8)0x55)
#define DCSP_HST_CAT_PROV_BICST_NSC     ((VOS_U8)0x56)
#define DCSP_HST_CAT_PROV_BICST_SC      ((VOS_U8)0x57)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_HST_TOKENS.                                                    */
/*                                                                           */
/* Client-Server Protocol token definitions in host order.  Note that the    */
/* provinces have been omitted because they might well be changed by a       */
/* Representation Message anyway.                                            */
/*****************************************************************************/
#define DCSP_HST_TOKENS

#define TOKEN VOS_U16

#define DCSP_HST_BRACKET_OPEN  ((TOKEN)((DCSP_HST_CAT_MISC << 8) | 0x00))
#define DCSP_HST_BRACKET_CLOSE ((TOKEN)((DCSP_HST_CAT_MISC << 8) | 0x01))

#define DCSP_HST_AUS           ((TOKEN)((DCSP_HST_CAT_POWER << 8) | 0x00))
#define DCSP_HST_ENG           ((TOKEN)((DCSP_HST_CAT_POWER << 8) | 0x01))
#define DCSP_HST_FRA           ((TOKEN)((DCSP_HST_CAT_POWER << 8) | 0x02))
#define DCSP_HST_GER           ((TOKEN)((DCSP_HST_CAT_POWER << 8) | 0x03))
#define DCSP_HST_ITA           ((TOKEN)((DCSP_HST_CAT_POWER << 8) | 0x04))
#define DCSP_HST_RUS           ((TOKEN)((DCSP_HST_CAT_POWER << 8) | 0x05))
#define DCSP_HST_TUR           ((TOKEN)((DCSP_HST_CAT_POWER << 8) | 0x06))

#define DCSP_HST_AMY           ((TOKEN)((DCSP_HST_CAT_UNIT_TYPE << 8) | 0x00))
#define DCSP_HST_FLT           ((TOKEN)((DCSP_HST_CAT_UNIT_TYPE << 8) | 0x01))

#define DCSP_HST_CTO           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x20))
#define DCSP_HST_CVY           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x21))
#define DCSP_HST_HLD           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x22))
#define DCSP_HST_MTO           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x23))
#define DCSP_HST_SUP           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x24))
#define DCSP_HST_VIA           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x25))
#define DCSP_HST_DSB           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x40))
#define DCSP_HST_RTO           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x41))
#define DCSP_HST_BLD           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x80))
#define DCSP_HST_REM           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x81))
#define DCSP_HST_WVE           ((TOKEN)((DCSP_HST_CAT_ORDER << 8) | 0x82))

#define DCSP_HST_MBV           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x00))
#define DCSP_HST_BPR           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x01))
#define DCSP_HST_CST           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x02))
#define DCSP_HST_ESC           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x03))
#define DCSP_HST_FAR           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x04))
#define DCSP_HST_HSC           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x05))
#define DCSP_HST_NAS           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x06))
#define DCSP_HST_NMB           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x07))
#define DCSP_HST_NMR           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x08))
#define DCSP_HST_NRN           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x09))
#define DCSP_HST_NRS           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x0A))
#define DCSP_HST_NSA           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x0B))
#define DCSP_HST_NSC           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x0C))
#define DCSP_HST_NSF           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x0D))
#define DCSP_HST_NSP           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x0E))
#define DCSP_HST_NSU           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x10))
#define DCSP_HST_NVR           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x11))
#define DCSP_HST_NYU           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x12))
#define DCSP_HST_YSC           ((TOKEN)((DCSP_HST_CAT_ORDER_NOTE << 8) | 0x13))

#define DCSP_HST_SUC           ((TOKEN)((DCSP_HST_CAT_RESULT << 8) | 0x00))
#define DCSP_HST_BNC           ((TOKEN)((DCSP_HST_CAT_RESULT << 8) | 0x01))
#define DCSP_HST_CUT           ((TOKEN)((DCSP_HST_CAT_RESULT << 8) | 0x02))
#define DCSP_HST_DSR           ((TOKEN)((DCSP_HST_CAT_RESULT << 8) | 0x03))
#define DCSP_HST_FLD           ((TOKEN)((DCSP_HST_CAT_RESULT << 8) | 0x04))
#define DCSP_HST_NSO           ((TOKEN)((DCSP_HST_CAT_RESULT << 8) | 0x05))
#define DCSP_HST_RET           ((TOKEN)((DCSP_HST_CAT_RESULT << 8) | 0x06))

#define DCSP_HST_NCS           ((TOKEN)((DCSP_HST_CAT_COAST << 8) | 0x00))
#define DCSP_HST_NEC           ((TOKEN)((DCSP_HST_CAT_COAST << 8) | 0x02))
#define DCSP_HST_ECS           ((TOKEN)((DCSP_HST_CAT_COAST << 8) | 0x04))
#define DCSP_HST_SEC           ((TOKEN)((DCSP_HST_CAT_COAST << 8) | 0x06))
#define DCSP_HST_SCS           ((TOKEN)((DCSP_HST_CAT_COAST << 8) | 0x08))
#define DCSP_HST_SWC           ((TOKEN)((DCSP_HST_CAT_COAST << 8) | 0x0A))
#define DCSP_HST_WCS           ((TOKEN)((DCSP_HST_CAT_COAST << 8) | 0x0C))
#define DCSP_HST_NWC           ((TOKEN)((DCSP_HST_CAT_COAST << 8) | 0x0E))

#define DCSP_HST_SPR           ((TOKEN)((DCSP_HST_CAT_PHASE << 8) | 0x00))
#define DCSP_HST_SUM           ((TOKEN)((DCSP_HST_CAT_PHASE << 8) | 0x01))
#define DCSP_HST_FAL           ((TOKEN)((DCSP_HST_CAT_PHASE << 8) | 0x02))
#define DCSP_HST_AUT           ((TOKEN)((DCSP_HST_CAT_PHASE << 8) | 0x03))
#define DCSP_HST_WIN           ((TOKEN)((DCSP_HST_CAT_PHASE << 8) | 0x04))

#define DCSP_HST_CCD           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x00))
#define DCSP_HST_DRW           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x01))
#define DCSP_HST_FRM           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x02))
#define DCSP_HST_GOF           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x03))
#define DCSP_HST_HLO           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x04))
#define DCSP_HST_HST           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x05))
#define DCSP_HST_HUH           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x06))
#define DCSP_HST_IAM           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x07))
#define DCSP_HST_LOD           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x08))
#define DCSP_HST_MAP           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x09))
#define DCSP_HST_MDF           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x0A))
#define DCSP_HST_MIS           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x0B))
#define DCSP_HST_NME           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x0C))
#define DCSP_HST_NOT           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x0D))
#define DCSP_HST_NOW           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x0E))
#define DCSP_HST_OBS           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x0F))
#define DCSP_HST_OFF           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x10))
#define DCSP_HST_ORD           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x11))
#define DCSP_HST_OUT           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x12))
#define DCSP_HST_PRN           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x13))
#define DCSP_HST_REJ           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x14))
#define DCSP_HST_SCO           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x15))
#define DCSP_HST_SLO           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x16))
#define DCSP_HST_SND           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x17))
#define DCSP_HST_SUB           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x18))
#define DCSP_HST_SVE           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x19))
#define DCSP_HST_THX           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x1A))
#define DCSP_HST_TME           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x1B))
#define DCSP_HST_YES           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x1C))
#define DCSP_HST_ADM           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x1D))
#define DCSP_HST_SMR           ((TOKEN)((DCSP_HST_CAT_COMMAND << 8) | 0x1E))

#define DCSP_HST_AOA           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x00))
#define DCSP_HST_BTL           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x01))
#define DCSP_HST_ERR           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x02))
#define DCSP_HST_LVL           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x03))
#define DCSP_HST_MRT           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x04))
#define DCSP_HST_MTL           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x05))
#define DCSP_HST_NPB           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x06))
#define DCSP_HST_NPR           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x07))
#define DCSP_HST_PDA           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x08))
#define DCSP_HST_PTL           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x09))
#define DCSP_HST_RTL           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x0A))
#define DCSP_HST_UNO           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x0B))
#define DCSP_HST_DSD           ((TOKEN)((DCSP_HST_CAT_PARAMETER << 8) | 0x0D))

#define DCSP_HST_ALY           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x00))
#define DCSP_HST_AND           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x01))
#define DCSP_HST_BWX           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x02))
#define DCSP_HST_DMZ           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x03))
#define DCSP_HST_ELS           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x04))
#define DCSP_HST_EXP           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x05))
#define DCSP_HST_FCT           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x06))
#define DCSP_HST_FOR           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x07))
#define DCSP_HST_FWD           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x08))
#define DCSP_HST_HOW           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x09))
#define DCSP_HST_IDK           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x0A))
#define DCSP_HST_IFF           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x0B))
#define DCSP_HST_INS           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x0C))
#define DCSP_HST_OCC           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x0E))
#define DCSP_HST_ORR           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x0F))
#define DCSP_HST_PCE           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x10))
#define DCSP_HST_POB           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x11))
#define DCSP_HST_PRP           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x13))
#define DCSP_HST_QRY           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x14))
#define DCSP_HST_SCD           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x15))
#define DCSP_HST_SRY           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x16))
#define DCSP_HST_SUG           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x17))
#define DCSP_HST_THK           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x18))
#define DCSP_HST_THN           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x19))
#define DCSP_HST_TRY           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x1A))
#define DCSP_HST_VSS           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x1C))
#define DCSP_HST_WHT           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x1D))
#define DCSP_HST_WHY           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x1E))
#define DCSP_HST_XDO           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x1F))
#define DCSP_HST_XOY           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x20))
#define DCSP_HST_YDO           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x21))
#define DCSP_HST_CHO           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x22))
#define DCSP_HST_BCC           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x23))
#define DCSP_HST_UNT           ((TOKEN)((DCSP_HST_CAT_PRESS << 8) | 0x24))
/*****************************************************************************/
/* END GROUP.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* Macro for converting characters into their equivalent token.              */
/*****************************************************************************/
#define DCSP_HST_CHAR(ch)      ((TOKEN)((DCSP_HST_CAT_STRING << 8) | ch))

/*****************************************************************************/
/* GROUP DCSP_HST_LOCAL_EVENTS                                               */
/*                                                                           */
/* o The CONNECTED event is seen by a client when it connects or fails to    */
/*   connect to a server.  If the error code is non-zero, it has failed to   */
/*   connect so the connection should not be considered open.                */
/*                                                                           */
/* o The ACCEPTED event is seen by the server when a client connects or      */
/*   attempts to connect.  If the error code is non-zero, it has failed to   */
/*   connect so the connection should not be considered open.                */
/*                                                                           */
/* o The CLOSE event indicates that the connection has been closed           */
/*   gracefully.                                                             */
/*                                                                           */
/* o The ERR_* events indicate that the connection has been aborted.  The    */
/*   precise event gives the reason for the abort.  The connection should no */
/*   longer be considered open.  These events are like the close event but   */
/*   indicate that the connection was closed due to an error rather than     */
/*   naturally or due to a request.                                          */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENTS

/*****************************************************************************/
/* The CONNECT event is internal to the DLL.  The user will never see it.    */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_CONNECT    ((VOS_U16)0x00)

/*****************************************************************************/
/* The CONNECTED event occurs when an attempt (by a client) to connect the   */
/* server completes.  The error code reveals whether or not the attempt was  */
/* successful.                                                               */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_CONNECTED  ((VOS_U16)0x01)

/*****************************************************************************/
/* The LISTEN event is internal to the DLL.  The user will never see it.     */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_LISTEN ((VOS_U16)0x02)

/*****************************************************************************/
/* The ERR_OUT_OF_MEM event occurs when the DLL has failed to allocate       */
/* memory to perform an operation.  As a result, a connection may fail, data */
/* may be lost or the DLL may become internally inconsistent.  The user      */
/* should immediately save all data before stopping the DLL.                 */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_OUT_OF_MEM ((VOS_U16)0x03)

/*****************************************************************************/
/* The ACCEPTED event occurs when a connection is made to the server.  The   */
/* error code reveals whether or not the connection was successfully         */
/* accepted by the server.                                                   */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_ACCEPTED   ((VOS_U16)0x04)

/*****************************************************************************/
/* The ERR_READ event occurs when the DLL fails to read data from a          */
/* connection due to an underlying connection error.  The connection is      */
/* closed as a result.                                                       */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_ERR_READ   ((VOS_U16)0x05)

/*****************************************************************************/
/* The ERR_WRITE event occurs when the DLL fails to write data to a          */
/* connection due to an underlying connection error.  The connection is      */
/* closed as a result.                                                       */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_ERR_WRITE  ((VOS_U16)0x06)

/*****************************************************************************/
/* The CLOSED event occurs when a connection is closed gracefully.           */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_CLOSED     ((VOS_U16)0x07)

/*****************************************************************************/
/* The CLOSE event is internal to the DLL.  The user will never see it.      */
/*****************************************************************************/
#define DCSP_HST_LOCAL_EVENT_CLOSE      ((VOS_U16)0x08)

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* Maximum size of the peer address in an ACCEPTED local message (LM).       */
/*****************************************************************************/
#define DCSP_HST_MAX_PEER_ADDR_LEN      14

/**STRUCT+********************************************************************/
/* Structure: DCSP_HST_MESSAGE                                               */
/*                                                                           */
/* Description: Structure of a host-order message.  This is followed by      */
/*            length bytes of data.  Each pair of bytes forms a token in     */
/*            HOST BYTE ORDER.                                               */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_hst_message
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
  /* The amount of data that follows.  This is in HOST BYTE ORDER.           */
  /***************************************************************************/
  VOS_U16 length;

} DCSP_HST_MESSAGE;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_HST_LM                                                    */
/*                                                                           */
/* Description: A local message.  This is not part of the CSP and is never   */
/*            trasmitted over the wire.  It is used to notify the user of    */
/*            events such as connection, disconnection and resource          */
/*            shortage.  It is also used for the user to make make requests. */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_hst_lm
{
  /***************************************************************************/
  /* The general host-order message header.                                  */
  /***************************************************************************/
  DCSP_HST_MESSAGE hdr;

  /***************************************************************************/
  /* The error code (if any) in HOST BYTE ORDER.                             */
  /***************************************************************************/
  VOS_S32 err;

  /***************************************************************************/
  /* The local event code in HOST BYTE ORDER.                                */
  /***************************************************************************/
  VOS_U16 event;                           /* One of: DCSP_HST_LOCAL_EVENTS; */

  /***************************************************************************/
  /* The address of the peer.  Only valid if event is _ACCEPTED.  Users may  */
  /* safely cast this to a SOCKADDR_IN structure.                            */
  /***************************************************************************/
  VOS_U8 peer_address[DCSP_HST_MAX_PEER_ADDR_LEN];
  VOS_PSINT peer_address_length;

} DCSP_HST_LM;

/**STRUCT-********************************************************************/

#endif

