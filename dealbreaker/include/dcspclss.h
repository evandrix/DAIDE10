/**INC+***********************************************************************/
/* Header:    dcspclss.h                                                     */
/*                                                                           */
/* Purpose:   Language-syntax structured codex external header file.         */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef DCSPCLSS_INCLUDED
#define DCSPCLSS_INCLUDED

/*****************************************************************************/
/*****************************************************************************/
/* Definitions.                                                              */
/*****************************************************************************/
/*****************************************************************************/

#define DCSP_LSS_TOKEN                  VOS_U16

/*****************************************************************************/
/* The maximum length of an AI name as defined in the syntax document.       */
/*****************************************************************************/
#define DCSP_LSS_MAX_PLAYER_NAME_LEN    64

/*****************************************************************************/
/* The maximum length of an AI version string as defined in the syntax       */
/* document.                                                                 */
/*****************************************************************************/
#define DCSP_LSS_MAX_VER_STRING_LEN     64

/*****************************************************************************/
/* The maximum length of an administrative (ADM) message.                    */
/*****************************************************************************/
#define DCSP_LSS_MAX_ADM_MESSAGE_LEN   512

/*****************************************************************************/
/* The maximum length of a convoy route.  !! This is not defined in the      */
/* protocol but is needed in this implementation.  See DCSP_LSS_ORDER for    */
/* details.  The value here is hopefully larger than needed.                 */
/*****************************************************************************/
#define DCSP_LSS_MAX_CONVOY_ROUTE_LEN   20

/*****************************************************************************/
/* The maximum number of convoys available to an army.  !!  This is not      */
/* defined in the protocol but is needed in this implementation.  An array   */
/* with this number of entries only exists transiently so the number can be  */
/* moderately large.                                                         */
/*****************************************************************************/
#define DCSP_LSS_MAX_CONVOY_ROUTES      1024

/*****************************************************************************/
/* The maximum number of retreat locations.  !! This is not defined in the   */
/* protocol but is needed in this implementation.  See DCSP_LSS_UNIT for     */
/* details.  The value here is hopefully larger than needed.                 */
/*****************************************************************************/
#define DCSP_LSS_MAX_RETREATS           20

/*****************************************************************************/
/* Maximum size of the peer address in an ACCEPTED local message (LM).       */
/*****************************************************************************/
#define DCSP_LSS_MAX_PEER_ADDR_LEN      14

/*****************************************************************************/
/* GROUP DCSP_LSS_TYPES                                                      */
/*****************************************************************************/
#define DCSP_LSS_TYPES

#define DCSP_LSS_TYPE_IM                ((VOS_U8)0x00)
#define DCSP_LSS_TYPE_RM                ((VOS_U8)0x01)
#define DCSP_LSS_TYPE_DM                ((VOS_U8)0x02)
#define DCSP_LSS_TYPE_EM                ((VOS_U8)0x03)
#define DCSP_LSS_TYPE_FM                ((VOS_U8)0x04)
#define DCSP_LSS_TYPE_LM                ((VOS_U8)0x05)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_LSS_REQ_RSP_TYPES                                              */
/*****************************************************************************/
#define DCSP_LSS_REQ_RSP_TYPES

#define DCSP_LSS_REQ_RSP_TYPE_NONE      ((VOS_U8)0x00)
#define DCSP_LSS_REQ_RSP_TYPE_REQUEST   ((VOS_U8)0x01)
#define DCSP_LSS_REQ_RSP_TYPE_ACCEPT    ((VOS_U8)0x02)
#define DCSP_LSS_REQ_RSP_TYPE_REJECT    ((VOS_U8)0x03)
#define DCSP_LSS_REQ_RSP_TYPE_UNIDIRECT ((VOS_U8)0x04)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_LSS_CATS                                                       */
/*                                                                           */
/* Client-Server Protocol categories.                                        */
/*****************************************************************************/
#define DCSP_LSS_CATS

#define DCSP_LSS_CAT_MISC               ((VOS_U8)0x40)
#define DCSP_LSS_CAT_POWER              ((VOS_U8)0x41)
#define DCSP_LSS_CAT_UNIT_TYPE          ((VOS_U8)0x42)
#define DCSP_LSS_CAT_ORDER              ((VOS_U8)0x43)
#define DCSP_LSS_CAT_ORDER_NOTE         ((VOS_U8)0x44)
#define DCSP_LSS_CAT_RESULT             ((VOS_U8)0x45)
#define DCSP_LSS_CAT_COAST              ((VOS_U8)0x46)
#define DCSP_LSS_CAT_PHASE              ((VOS_U8)0x47)
#define DCSP_LSS_CAT_COMMAND            ((VOS_U8)0x48)
#define DCSP_LSS_CAT_PARAMETER          ((VOS_U8)0x49)
#define DCSP_LSS_CAT_PRESS              ((VOS_U8)0x4A)
#define DCSP_LSS_CAT_STRING             ((VOS_U8)0x4B)

#define DCSP_LSS_CAT_PROV_LAND_NSC      ((VOS_U8)0x50)
#define DCSP_LSS_CAT_PROV_LAND_SC       ((VOS_U8)0x51)
#define DCSP_LSS_CAT_PROV_WATER_NSC     ((VOS_U8)0x52)
#define DCSP_LSS_CAT_PROV_WATER_SC      ((VOS_U8)0x53)
#define DCSP_LSS_CAT_PROV_COAST_NSC     ((VOS_U8)0x54)
#define DCSP_LSS_CAT_PROV_COAST_SC      ((VOS_U8)0x55)
#define DCSP_LSS_CAT_PROV_BICST_NSC     ((VOS_U8)0x56)
#define DCSP_LSS_CAT_PROV_BICST_SC      ((VOS_U8)0x57)

#define DCSP_LSS_CAT_RESVD1             ((VOS_U8)0x58)
#define DCSP_LSS_CAT_RESVD2             ((VOS_U8)0x59)
#define DCSP_LSS_CAT_RESVD3             ((VOS_U8)0x5A)
#define DCSP_LSS_CAT_RESVD4             ((VOS_U8)0x5B)
#define DCSP_LSS_CAT_RESVD5             ((VOS_U8)0x5C)
#define DCSP_LSS_CAT_RESVD6             ((VOS_U8)0x5D)
#define DCSP_LSS_CAT_RESVD7             ((VOS_U8)0x5E)
#define DCSP_LSS_CAT_RESVD8             ((VOS_U8)0x5F)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_LSS_TOKENS.                                                    */
/*                                                                           */
/* Client-Server Protocol token definitions in network order.  Provinces     */
/* have been omitted because they might well be changed by a Representation  */
/* Message anyway.                                                           */
/*****************************************************************************/
#define DCSP_LSS_TOKENS

#define TOKEN VOS_U16

#define DCSP_LSS_T_BRA         ((TOKEN)(DCSP_LSS_CAT_MISC | (0x00 << 8)))
#define DCSP_LSS_T_KET         ((TOKEN)(DCSP_LSS_CAT_MISC | (0x01 << 8)))

#define DCSP_LSS_T_AUS         ((TOKEN)(DCSP_LSS_CAT_POWER | (0x00 << 8)))
#define DCSP_LSS_T_ENG         ((TOKEN)(DCSP_LSS_CAT_POWER | (0x01 << 8)))
#define DCSP_LSS_T_FRA         ((TOKEN)(DCSP_LSS_CAT_POWER | (0x02 << 8)))
#define DCSP_LSS_T_GER         ((TOKEN)(DCSP_LSS_CAT_POWER | (0x03 << 8)))
#define DCSP_LSS_T_ITA         ((TOKEN)(DCSP_LSS_CAT_POWER | (0x04 << 8)))
#define DCSP_LSS_T_RUS         ((TOKEN)(DCSP_LSS_CAT_POWER | (0x05 << 8)))
#define DCSP_LSS_T_TUR         ((TOKEN)(DCSP_LSS_CAT_POWER | (0x06 << 8)))

#define DCSP_LSS_T_AMY         ((TOKEN)(DCSP_LSS_CAT_UNIT_TYPE | (0x00 << 8)))
#define DCSP_LSS_T_FLT         ((TOKEN)(DCSP_LSS_CAT_UNIT_TYPE | (0x01 << 8)))

#define DCSP_LSS_T_CTO         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x20 << 8)))
#define DCSP_LSS_T_CVY         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x21 << 8)))
#define DCSP_LSS_T_HLD         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x22 << 8)))
#define DCSP_LSS_T_MTO         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x23 << 8)))
#define DCSP_LSS_T_SUP         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x24 << 8)))
#define DCSP_LSS_T_VIA         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x25 << 8)))
#define DCSP_LSS_T_DSB         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x40 << 8)))
#define DCSP_LSS_T_RTO         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x41 << 8)))
#define DCSP_LSS_T_BLD         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x80 << 8)))
#define DCSP_LSS_T_REM         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x81 << 8)))
#define DCSP_LSS_T_WVE         ((TOKEN)(DCSP_LSS_CAT_ORDER | (0x82 << 8)))

#define DCSP_LSS_T_MBV         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x00 << 8)))
#define DCSP_LSS_T_BPR         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x01 << 8)))
#define DCSP_LSS_T_CST         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x02 << 8)))
#define DCSP_LSS_T_ESC         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x03 << 8)))
#define DCSP_LSS_T_FAR         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x04 << 8)))
#define DCSP_LSS_T_HSC         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x05 << 8)))
#define DCSP_LSS_T_NAS         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x06 << 8)))
#define DCSP_LSS_T_NMB         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x07 << 8)))
#define DCSP_LSS_T_NMR         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x08 << 8)))
#define DCSP_LSS_T_NRN         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x09 << 8)))
#define DCSP_LSS_T_NRS         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x0A << 8)))
#define DCSP_LSS_T_NSC         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x0B << 8)))
#define DCSP_LSS_T_NSF         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x0C << 8)))
#define DCSP_LSS_T_NSP         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x0D << 8)))
#define DCSP_LSS_T_NSU         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x0F << 8)))
#define DCSP_LSS_T_NYU         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x10 << 8)))
#define DCSP_LSS_T_YSC         ((TOKEN)(DCSP_LSS_CAT_ORDER_NOTE | (0x11 << 8)))

#define DCSP_LSS_T_SUC         ((TOKEN)(DCSP_LSS_CAT_RESULT | (0x00 << 8)))
#define DCSP_LSS_T_BNC         ((TOKEN)(DCSP_LSS_CAT_RESULT | (0x01 << 8)))
#define DCSP_LSS_T_CUT         ((TOKEN)(DCSP_LSS_CAT_RESULT | (0x02 << 8)))
#define DCSP_LSS_T_DSR         ((TOKEN)(DCSP_LSS_CAT_RESULT | (0x03 << 8)))
#define DCSP_LSS_T_FLD         ((TOKEN)(DCSP_LSS_CAT_RESULT | (0x04 << 8)))
#define DCSP_LSS_T_NSO         ((TOKEN)(DCSP_LSS_CAT_RESULT | (0x05 << 8)))
#define DCSP_LSS_T_RET         ((TOKEN)(DCSP_LSS_CAT_RESULT | (0x06 << 8)))

#define DCSP_LSS_T_NCS         ((TOKEN)(DCSP_LSS_CAT_COAST | (0x00 << 8)))
#define DCSP_LSS_T_NEC         ((TOKEN)(DCSP_LSS_CAT_COAST | (0x02 << 8)))
#define DCSP_LSS_T_ECS         ((TOKEN)(DCSP_LSS_CAT_COAST | (0x04 << 8)))
#define DCSP_LSS_T_SEC         ((TOKEN)(DCSP_LSS_CAT_COAST | (0x06 << 8)))
#define DCSP_LSS_T_SCS         ((TOKEN)(DCSP_LSS_CAT_COAST | (0x08 << 8)))
#define DCSP_LSS_T_SWC         ((TOKEN)(DCSP_LSS_CAT_COAST | (0x0A << 8)))
#define DCSP_LSS_T_WCS         ((TOKEN)(DCSP_LSS_CAT_COAST | (0x0C << 8)))
#define DCSP_LSS_T_NWC         ((TOKEN)(DCSP_LSS_CAT_COAST | (0x0E << 8)))

#define DCSP_LSS_T_SPR         ((TOKEN)(DCSP_LSS_CAT_PHASE | (0x00 << 8)))
#define DCSP_LSS_T_SUM         ((TOKEN)(DCSP_LSS_CAT_PHASE | (0x01 << 8)))
#define DCSP_LSS_T_FAL         ((TOKEN)(DCSP_LSS_CAT_PHASE | (0x02 << 8)))
#define DCSP_LSS_T_AUT         ((TOKEN)(DCSP_LSS_CAT_PHASE | (0x03 << 8)))
#define DCSP_LSS_T_WIN         ((TOKEN)(DCSP_LSS_CAT_PHASE | (0x04 << 8)))

#define DCSP_LSS_T_CCD         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x00 << 8)))
#define DCSP_LSS_T_DRW         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x01 << 8)))
#define DCSP_LSS_T_FRM         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x02 << 8)))
#define DCSP_LSS_T_GOF         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x03 << 8)))
#define DCSP_LSS_T_HLO         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x04 << 8)))
#define DCSP_LSS_T_HST         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x05 << 8)))
#define DCSP_LSS_T_HUH         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x06 << 8)))
#define DCSP_LSS_T_IAM         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x07 << 8)))
#define DCSP_LSS_T_LOD         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x08 << 8)))
#define DCSP_LSS_T_MAP         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x09 << 8)))
#define DCSP_LSS_T_MDF         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x0A << 8)))
#define DCSP_LSS_T_MIS         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x0B << 8)))
#define DCSP_LSS_T_NME         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x0C << 8)))
#define DCSP_LSS_T_NOT         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x0D << 8)))
#define DCSP_LSS_T_NOW         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x0E << 8)))
#define DCSP_LSS_T_OBS         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x0F << 8)))
#define DCSP_LSS_T_OFF         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x10 << 8)))
#define DCSP_LSS_T_ORD         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x11 << 8)))
#define DCSP_LSS_T_OUT         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x12 << 8)))
#define DCSP_LSS_T_PRN         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x13 << 8)))
#define DCSP_LSS_T_REJ         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x14 << 8)))
#define DCSP_LSS_T_SCO         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x15 << 8)))
#define DCSP_LSS_T_SLO         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x16 << 8)))
#define DCSP_LSS_T_SND         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x17 << 8)))
#define DCSP_LSS_T_SUB         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x18 << 8)))
#define DCSP_LSS_T_SVE         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x19 << 8)))
#define DCSP_LSS_T_THX         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x1A << 8)))
#define DCSP_LSS_T_TME         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x1B << 8)))
#define DCSP_LSS_T_YES         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x1C << 8)))
#define DCSP_LSS_T_ADM         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x1D << 8)))
#define DCSP_LSS_T_SMR         ((TOKEN)(DCSP_LSS_CAT_COMMAND | (0x1E << 8)))

#define DCSP_LSS_T_AOA         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x00 << 8)))
#define DCSP_LSS_T_BTL         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x01 << 8)))
#define DCSP_LSS_T_ERR         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x02 << 8)))
#define DCSP_LSS_T_LVL         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x03 << 8)))
#define DCSP_LSS_T_MRT         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x04 << 8)))
#define DCSP_LSS_T_MTL         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x05 << 8)))
#define DCSP_LSS_T_NPB         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x06 << 8)))
#define DCSP_LSS_T_NPR         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x07 << 8)))
#define DCSP_LSS_T_PDA         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x08 << 8)))
#define DCSP_LSS_T_PTL         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x09 << 8)))
#define DCSP_LSS_T_RTL         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x0A << 8)))
#define DCSP_LSS_T_UNO         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x0B << 8)))
#define DCSP_LSS_T_DSD         ((TOKEN)(DCSP_LSS_CAT_PARAMETER | (0x0D << 8)))

#define DCSP_LSS_T_ALY         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x00 << 8)))
#define DCSP_LSS_T_AND         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x01 << 8)))
#define DCSP_LSS_T_BWX         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x02 << 8)))
#define DCSP_LSS_T_DMZ         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x03 << 8)))
#define DCSP_LSS_T_ELS         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x04 << 8)))
#define DCSP_LSS_T_EXP         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x05 << 8)))
#define DCSP_LSS_T_FCT         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x06 << 9)))
#define DCSP_LSS_T_FOR         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x07 << 8)))
#define DCSP_LSS_T_FWD         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x08 << 8)))
#define DCSP_LSS_T_HOW         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x09 << 8)))
#define DCSP_LSS_T_IDK         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x0A << 8)))
#define DCSP_LSS_T_IFF         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x0B << 8)))
#define DCSP_LSS_T_INS         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x0C << 8)))
#define DCSP_LSS_T_OCC         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x0E << 8)))
#define DCSP_LSS_T_ORR         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x0F << 8)))
#define DCSP_LSS_T_PCE         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x10 << 8)))
#define DCSP_LSS_T_POB         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x11 << 8)))
#define DCSP_LSS_T_PRP         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x13 << 8)))
#define DCSP_LSS_T_QRY         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x14 << 8)))
#define DCSP_LSS_T_SCD         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x15 << 8)))
#define DCSP_LSS_T_SRY         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x16 << 8)))
#define DCSP_LSS_T_SUG         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x17 << 8)))
#define DCSP_LSS_T_THK         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x18 << 8)))
#define DCSP_LSS_T_THN         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x19 << 8)))
#define DCSP_LSS_T_TRY         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x1A << 8)))
#define DCSP_LSS_T_VSS         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x1C << 8)))
#define DCSP_LSS_T_WHT         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x1D << 8)))
#define DCSP_LSS_T_WHY         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x1E << 8)))
#define DCSP_LSS_T_XDO         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x1F << 8)))
#define DCSP_LSS_T_XOY         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x20 << 8)))
#define DCSP_LSS_T_YDO         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x21 << 8)))
#define DCSP_LSS_T_CHO         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x22 << 8)))
#define DCSP_LSS_T_BCC         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x23 << 8)))
#define DCSP_LSS_T_UNT         ((TOKEN)(DCSP_LSS_CAT_PRESS | (0x24 << 8)))

/*****************************************************************************/
/* Tokens indicating that a field is not valid.                              */
/*                                                                           */
/* IVC - Invalid Coast                                                       */
/* IVP - Invalid Province                                                    */
/* IVS - Invalid Season                                                      */
/*****************************************************************************/
#define DCSP_LSS_T_IVC         ((TOKEN)(DCSP_LSS_CAT_RESVD1 | (0x00 << 8)))
#define DCSP_LSS_T_IVP         ((TOKEN)(DCSP_LSS_CAT_RESVD1 | (0x01 << 8)))
#define DCSP_LSS_T_IVS         ((TOKEN)(DCSP_LSS_CAT_RESVD1 | (0x02 << 8)))

/*****************************************************************************/
/* Other user defined tokens.                                                */
/*                                                                           */
/* MNC - Maybe No Convoy (used during adjudication).                         */
/*****************************************************************************/
#define DCSP_LSS_T_MNC         ((TOKEN)(DCSP_LSS_CAT_RESVD2 | (0x00 << 8)))

/*****************************************************************************/
/* END GROUP.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_LSS_LOC_FLAGS.                                                 */
/*****************************************************************************/
#define DCSP_LSS_LOC_FLAGS

#define DCSP_LSS_LOC_FLAG_LAND          ((VOS_U16)0x0001)
#define DCSP_LSS_LOC_FLAG_SEA           ((VOS_U16)0x0002)
#define DCSP_LSS_LOC_FLAG_PORT          ((VOS_U16)0x0004)
#define DCSP_LSS_LOC_FLAG_NCS           ((VOS_U16)0x0008)
#define DCSP_LSS_LOC_FLAG_NEC           ((VOS_U16)0x0010)
#define DCSP_LSS_LOC_FLAG_ECS           ((VOS_U16)0x0020)
#define DCSP_LSS_LOC_FLAG_SEC           ((VOS_U16)0x0040)
#define DCSP_LSS_LOC_FLAG_SCS           ((VOS_U16)0x0080)
#define DCSP_LSS_LOC_FLAG_SWC           ((VOS_U16)0x0100)
#define DCSP_LSS_LOC_FLAG_WCS           ((VOS_U16)0x0200)
#define DCSP_LSS_LOC_FLAG_NWC           ((VOS_U16)0x0400)
#define DCSP_LSS_LOC_FLAG_UNKNOWN       ((VOS_U16)0x8000)
/*****************************************************************************/
/* END GROUP.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP DCSP_LSS_LOCAL_EVENTS                                               */
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
#define DCSP_LSS_LOCAL_EVENTS

/*****************************************************************************/
/* The CONNECT event is internal to the DLL.  The user will never see it.    */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_CONNECT    ((VOS_U16)0x00)

/*****************************************************************************/
/* The CONNECTED event occurs when an attempt (by a client) to connect the   */
/* server completes.  The error code reveals whether or not the attempt was  */
/* successful.                                                               */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_CONNECTED  ((VOS_U16)0x01)

/*****************************************************************************/
/* The LISTEN event is internal to the DLL.  The user will never see it.     */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_LISTEN ((VOS_U16)0x02)

/*****************************************************************************/
/* The ERR_OUT_OF_MEM event occurs when the DLL has failed to allocate       */
/* memory to perform an operation.  As a result, a connection may fail, data */
/* may be lost or the DLL may become internally inconsistent.  The user      */
/* should immediately save all data before stopping the DLL.                 */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_OUT_OF_MEM ((VOS_U16)0x03)

/*****************************************************************************/
/* The ACCEPTED event occurs when a connection is made to the server.  The   */
/* error code reveals whether or not the connection was successfully         */
/* accepted by the server.                                                   */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_ACCEPTED   ((VOS_U16)0x04)

/*****************************************************************************/
/* The ERR_READ event occurs when the DLL fails to read data from a          */
/* connection due to an underlying connection error.  The connection is      */
/* closed as a result.                                                       */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_ERR_READ   ((VOS_U16)0x05)

/*****************************************************************************/
/* The ERR_WRITE event occurs when the DLL fails to write data to a          */
/* connection due to an underlying connection error.  The connection is      */
/* closed as a result.                                                       */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_ERR_WRITE  ((VOS_U16)0x06)

/*****************************************************************************/
/* The CLOSED event occurs when a connection is closed gracefully.           */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_CLOSED     ((VOS_U16)0x07)

/*****************************************************************************/
/* The CLOSE event is internal to the DLL.  The user will never see it.      */
/*****************************************************************************/
#define DCSP_LSS_LOCAL_EVENT_CLOSE      ((VOS_U16)0x08)

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* Macro for converting from a coast token to a location flag.               */
/*****************************************************************************/
#define DCSP_LSS_CONV_LOC_TO_FLAGS(TOK) (1 << (((TOK & 0xFF00) >> 9) + 3))
#define DCSP_LSS_CONV_FLAG_TO_LOC(FLAG, LOC)                                  \
          {                                                                   \
            VOS_U16 flg_copy = FLAG;                                          \
            VOS_U8 bit_pos = 0;                                               \
            VOS_ASSERT(((flg_copy) == DCSP_LSS_LOC_FLAG_NCS) ||               \
                       ((flg_copy) == DCSP_LSS_LOC_FLAG_NEC) ||               \
                       ((flg_copy) == DCSP_LSS_LOC_FLAG_ECS) ||               \
                       ((flg_copy) == DCSP_LSS_LOC_FLAG_SEC) ||               \
                       ((flg_copy) == DCSP_LSS_LOC_FLAG_SCS) ||               \
                       ((flg_copy) == DCSP_LSS_LOC_FLAG_SWC) ||               \
                       ((flg_copy) == DCSP_LSS_LOC_FLAG_WCS) ||               \
                       ((flg_copy) == DCSP_LSS_LOC_FLAG_NWC));                \
            while ((flg_copy) != 1)                                           \
            {                                                                 \
              (flg_copy) = (flg_copy) >> 1;                                   \
              bit_pos++;                                                      \
            }                                                                 \
            LOC = (TOKEN)(((bit_pos - 3) << 9) | DCSP_LSS_CAT_COAST);         \
          }                                                                   \
          LOC = LOC

/*****************************************************************************/
/* Macro for converting between characters and their equivalent tokens.      */
/*****************************************************************************/
#define DCSP_LSS_DEC_CHAR(ch)        ((TOKEN)(DCSP_HST_CAT_STRING | (ch << 8)))
#define DCSP_LSS_ENC_CHAR(tok)       ((VOS_CHAR)(tok >> 8))

/*****************************************************************************/
/* Macros for converting between a token and a province/power index.         */
/*****************************************************************************/
#define DCSP_LSS_CONV_POWER_TO_INDEX(X) (VOS_U8)(X >> 8)
#define DCSP_LSS_CONV_INDEX_TO_POWER(X) (VOS_U16)((X << 8) | \
                                                            DCSP_LSS_CAT_POWER)
#define DCSP_LSS_CONV_PROV_TO_INDEX(X)  (VOS_U8)(X >> 8)

/*****************************************************************************/
/*****************************************************************************/
/* Structures.                                                               */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/* Several of the structures have data following them so they use a 0 length */
/* array at the end of the structure for convenience is accessing the        */
/* appended data.                                                            */
/*****************************************************************************/
#pragma warning (push)
#pragma warning (disable : 4200)

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_PROVINCE                                              */
/*                                                                           */
/* Description: A province.                                                  */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_province
{
  /***************************************************************************/
  /* The token that represents this province.                                */
  /***************************************************************************/
  DCSP_LSS_TOKEN token;

  /***************************************************************************/
  /* Whether the SC is a province or not.                                    */
  /***************************************************************************/
  VOS_BOOL sc;

  /***************************************************************************/
  /* If the province is an SC, the power who has the SC as an HSC.  If the   */
  /* province is not an SC or is a neutral SC, this field is set to          */
  /* num_powers (as described in the MDF message).                           */
  /***************************************************************************/
  VOS_U16 hsc_power_index;

  /***************************************************************************/
  /* The locations that are valid for this province.  This is a bit field    */
  /* with the following bits being valid.                                    */
  /*                                                                         */
  /* DCSP_LSS_LOC_FLAG_LAND                                                  */
  /* DCSP_LSS_LOC_FLAG_SEA                                                   */
  /* DCSP_LSS_LOC_FLAG_ONLY_CST                                              */
  /* DCSP_LSS_LOC_FLAG_NCS                                                   */
  /* DCSP_LSS_LOC_FLAG_NEC                                                   */
  /* DCSP_LSS_LOC_FLAG_ECS                                                   */
  /* DCSP_LSS_LOC_FLAG_SEC                                                   */
  /* DCSP_LSS_LOC_FLAG_SCS                                                   */
  /* DCSP_LSS_LOC_FLAG_SWC                                                   */
  /* DCSP_LSS_LOC_FLAG_WCS                                                   */
  /* DCSP_LSS_LOC_FLAG_NWC                                                   */
  /*                                                                         */
  /* Note that onlt the LAND location supports armies.  All the other        */
  /* locations support fleets alone.  Note that (uni-)coastal provinces will */
  /* have LAND and ONLY_CST as the valid locations.                          */
  /***************************************************************************/
  VOS_U16 valid_locations;

} DCSP_LSS_PROVINCE;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_LOCATION                                              */
/*                                                                           */
/* Description: Location of a unit.                                          */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_location
{
  /***************************************************************************/
  /* The province that the unit is located in.                               */
  /***************************************************************************/
  VOS_U16 province_index;

  /***************************************************************************/
  /* The location of the unit within the province.                           */
  /***************************************************************************/
  DCSP_LSS_TOKEN location;                    /* One of: DCSP_LSS_LOC_FLAGS; */

} DCSP_LSS_LOCATION;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_ADJACENCY                                             */
/*                                                                           */
/* Description: Pair of adjacent locations.                                  */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_adjacency
{
  /***************************************************************************/
  /* Source location.                                                        */
  /***************************************************************************/
  DCSP_LSS_LOCATION source;

  /***************************************************************************/
  /* Destination location.                                                   */
  /***************************************************************************/
  DCSP_LSS_LOCATION destination;

} DCSP_LSS_ADJACENCY;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_UNIT                                                  */
/*                                                                           */
/* Description: A unit.                                                      */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_unit
{
  /***************************************************************************/
  /* The index of the power owning the unit.                                 */
  /***************************************************************************/
  VOS_U16 power_index;

  /***************************************************************************/
  /* Type (army/fleet).                                                      */
  /***************************************************************************/
  DCSP_LSS_TOKEN type;

  /***************************************************************************/
  /* Location of the unit.                                                   */
  /***************************************************************************/
  DCSP_LSS_LOCATION location;

  /***************************************************************************/
  /* Whether or not the unit is dislodged.                                   */
  /***************************************************************************/
  VOS_BOOL dislodged;

  /***************************************************************************/
  /* Possible retreat locations.  These fields are only valid if the unit is */
  /* dislodged.                                                              */
  /***************************************************************************/
  VOS_U16 num_retreat_locations;
  DCSP_LSS_LOCATION retreat_location[DCSP_LSS_MAX_RETREATS];

} DCSP_LSS_UNIT;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_ORDER                                                 */
/*                                                                           */
/* Description: An order for a unit.                                         */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_order
{
  /***************************************************************************/
  /* The unit being ordered.                                                 */
  /***************************************************************************/
  DCSP_LSS_UNIT unit;

  /***************************************************************************/
  /* The type of order.  The possibilities are listed below.  This field     */
  /* determines which of the remaining fields are valid.                     */
  /*                                                                         */
  /* o HLD - A hold order.                                                   */
  /* o MTO - A move order.                                                   */
  /* o SUP - A support order.                                                */
  /* o CVY - A convoy order.                                                 */
  /* o CTO - A move by convoy order.                                         */
  /*                                                                         */
  /* o RTO - Retreat.                                                        */
  /* o DSB - Disband.                                                        */
  /*                                                                         */
  /* o BLD - Build.                                                          */
  /* o REM - Remove.                                                         */
  /* o WVE - Waive a build.  The unit that this is applied to is not valid.  */
  /***************************************************************************/
  DCSP_LSS_TOKEN order;

  /***************************************************************************/
  /* Destination.  Mandatory for MTO, CTO, RTO, & CVY.  Optional for SUP.    */
  /* If not used for SUP, the province field of location will be set to      */
  /* DCSP_LSS_T_IVP (invalid province).                                      */
  /*                                                                         */
  /* For all orders except MTO, the location field of destination will be    */
  /* set to DCSP_LSS_LOC_FLAG_UNKNOWN.  For MTO orders where coast is        */
  /* irrelevant, location will also be DCSP_LSS_LOC_FLAG_UNKNOWN.            */
  /***************************************************************************/
  DCSP_LSS_LOCATION destination;

  /***************************************************************************/
  /* Additional unit.  Valid for SUP and CVY.                                */
  /***************************************************************************/
  DCSP_LSS_UNIT other_unit;

  /***************************************************************************/
  /* The number of sea provinces involved in the convoy path.  This cannot   */
  /* exceed DCSP_LSS_MAX_CONVOY_ROUTE_LEN.                                   */
  /***************************************************************************/
  VOS_U16 convoy_path_length;

  /***************************************************************************/
  /* The convoy path for moves by convoy.  Each element is a province.       */
  /*                                                                         */
  /* Note that this must be a fixed size because there are arrays of orders. */
  /***************************************************************************/
  DCSP_LSS_TOKEN convoy_path[DCSP_LSS_MAX_CONVOY_ROUTE_LEN];

} DCSP_LSS_ORDER;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_SCO_PAIR                                              */
/*                                                                           */
/* Description:                                                              */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_sco_pair
{
  /***************************************************************************/
  /* The index of the power owning the SC.                                   */
  /***************************************************************************/
  VOS_U16 pow_ix;

  /***************************************************************************/
  /* The province index of the owned SC.                                     */
  /***************************************************************************/
  VOS_U16 sc_prov_index;

} DCSP_LSS_SCO_PAIR;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_MSG                                                   */
/*                                                                           */
/* Description: A language-syntax structured message.                        */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_msg
{
  /***************************************************************************/
  /* The type of message.  One of...                                         */
  /* o Initial Message (IM)                                                  */
  /* o Representation Message (RM)                                           */
  /* o Diplomacy Message (DM)                                                */
  /* o Error Message (EM)                                                    */
  /* o Final Message (FM)                                                    */
  /* o Local Message (LM)                                                    */
  /***************************************************************************/
  VOS_U8 type;                                    /* One of: DCSP_LSS_TYPES; */

  /***************************************************************************/
  /* Data follows for all but IMs and FMs.                                   */
  /***************************************************************************/

} DCSP_LSS_MSG;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_LM                                                    */
/*                                                                           */
/* Description: Local message.  This is never converted for transmission     */
/*            over the wire.                                                 */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_lm
{
  /***************************************************************************/
  /* The error code (if any) in HOST BYTE ORDER.                             */
  /***************************************************************************/
  VOS_S32 err;

  /***************************************************************************/
  /* The local event code in HOST BYTE ORDER.                                */
  /***************************************************************************/
  VOS_U16 event;                               /* One of: DCSP_LOCAL_EVENTS; */

  /***************************************************************************/
  /* The address of the peer.  Only valid if event is _ACCEPTED.  Users may  */
  /* safely cast this to a SOCKADDR_IN structure.                            */
  /***************************************************************************/
  VOS_U8 peer_address[DCSP_LSS_MAX_PEER_ADDR_LEN];
  VOS_PSINT peer_address_length;

} DCSP_LSS_LM;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_EM                                                    */
/*                                                                           */
/* Description: An CSP error message.                                        */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_em
{
  /***************************************************************************/
  /* The error in NETWORK BYTE ORDER.                                        */
  /***************************************************************************/
  VOS_U16 err;

} DCSP_LSS_EM;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_TLM_HDR                                               */
/*                                                                           */
/* Description: Top-level message header.                                    */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_tlm_hdr
{
  /***************************************************************************/
  /* The type of message.  (e.g. NME, HLO, etc.)                             */
  /***************************************************************************/
  DCSP_LSS_TOKEN type;

  /***************************************************************************/
  /* The size of the message including all sub-messages.                     */
  /***************************************************************************/
  VOS_U16 size;

  /***************************************************************************/
  /* The request/response type as described below.                           */
  /*                                                                         */
  /* o NONE is used for non-top level messages.                              */
  /* o REQUEST is used for requests.  E.g. NME('MyAI')('1.0')                */
  /* o ACCEPT is used for YES messages.  E.g. YES(NME('MyAI')('1.0'))        */
  /* o REJECT is used for REJ messages.  E.g. REJ(NME('MyAI')('1.0'))        */
  /* o UNIDIRECT is used for messages with no response.                      */
  /***************************************************************************/
  VOS_U8 req_rsp_type;                    /* One of: DCSP_LSS_REQ_RSP_TYPES; */

} DCSP_LSS_TLM_HDR;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_ADM                                                   */
/*                                                                           */
/* Description: Administrative message.                                      */
/*                                                                           */
/* AI-Server: Administrative message.                                        */
/*                                                                           */
/* Server-AI: Administrative message.                                        */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_adm
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* Null-terminated case-insensitive AI name (not more than                 */
  /* MAX_PLAYER_NAME_LEN characters excluding the null terminator) using     */
  /* only [a-z0-9_].                                                         */
  /***************************************************************************/
  VOS_CHAR player_name[DCSP_LSS_MAX_PLAYER_NAME_LEN + 1];

  /***************************************************************************/
  /* Null-terminated case-sensitive message (not more than                   */
  /* MAX_ADM_MESSAGE_LEN characters excluding the null terminator) using     */
  /* only [a-z0-9_ ].                                                        */
  /***************************************************************************/
  VOS_CHAR message[DCSP_LSS_MAX_ADM_MESSAGE_LEN + 1];

} DCSP_LSS_ADM;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_CCD                                                   */
/*                                                                           */
/* Description: Civil Disorder message.                                      */
/*                                                                           */
/* Server-AI: A power is in civil disorder or has just come out of it.       */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_ccd
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The power in civil disorder.                                            */
  /***************************************************************************/
  DCSP_LSS_TOKEN disorder_power;

  /***************************************************************************/
  /* Has the power gone into or recovered from civil disorder?               */
  /***************************************************************************/
  VOS_BOOL in_civil_disorder;

} DCSP_LSS_CCD;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_DRW                                                   */
/*                                                                           */
/* Description: Draw message.                                                */
/*                                                                           */
/* Server-AI: The game has finished in a draw between the powers listed.     */
/*            There will be at least two powers.                             */
/*                                                                           */
/* AI-Server: Request that the game is finished in a draw between the        */
/*            listed powers.                                                 */
/*                                                                           */
/* Press:     A draw is proposed including the listed countires.             */
/*                                                                           */
/* WARNING:   Data follows this structure.                                   */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_drw
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The number of powers that are part of the actual/requested/proposed     */
  /* draw.  The power indicies (each a VOS_U8) follow this message.          */
  /***************************************************************************/
  VOS_U16 num_draw_participants;
  VOS_U8 draw_participant[0];

} DCSP_LSS_DRW;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_FRM                                                   */
/*                                                                           */
/* Description: From message.                                                */
/*                                                                           */
/* Server-AI: Notification of a message from another power.                  */
/*                                                                           */
/* WARNING:   Data follows this structure.                                   */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_frm
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The sender.                                                             */
  /***************************************************************************/
  DCSP_LSS_TOKEN sender;

  /***************************************************************************/
  /* The offset from the start of this structure of the top-level message    */
  /* header for the message received from the power.  This will always be    */
  /* immediately after the recipients.                                       */
  /***************************************************************************/
  VOS_U16 message_offset;

  /***************************************************************************/
  /* The number of recipients of the message.  The power index for each      */
  /* recipient (each a VOS_U8) follows this message.                         */
  /***************************************************************************/
  VOS_U16 num_recipients;
  VOS_U8 recipient[0];

} DCSP_LSS_FRM;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_GOF                                                   */
/*                                                                           */
/* Description: Go flag.                                                     */
/*                                                                           */
/* AI-Server: Whether The AI is happy for the server to process the turn     */
/*            before the deadline.                                           */
/*                                                                           */
/**STRUCT-********************************************************************/

typedef struct dcsp_lss_gof
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* Is the AI happy for the server to process the turn before the deadline? */
  /***************************************************************************/
  VOS_BOOL ready;

} DCSP_LSS_GOF;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_HLO                                                   */
/*                                                                           */
/* Description: Hello message.                                               */
/*                                                                           */
/* Server-AI: Game start message from the server specifying the power that   */
/*            the AI will play along with variant options.                   */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_hlo
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The power that the AI is playing.                                       */
  /***************************************************************************/
  DCSP_LSS_TOKEN your_power;

  /***************************************************************************/
  /* The passcode for taking the power back after disconnection.             */
  /***************************************************************************/
  VOS_S16 passcode;

  /***************************************************************************/
  /* The maximum press level for the game.  !! No default specified in the   */
  /* syntax document.                                                        */
  /***************************************************************************/
  VOS_U16 max_press_level;

  /***************************************************************************/
  /* The deadline (in seconds) for moves of zero for no limit.               */
  /***************************************************************************/
  VOS_U16 move_time;

  /***************************************************************************/
  /* The deadline (in seconds) for retreats or zero for no limit.            */
  /***************************************************************************/
  VOS_U16 retreat_time;

  /***************************************************************************/
  /* The deadline (in seconds) for builds or zero for no limit.              */
  /***************************************************************************/
  VOS_U16 build_time;

  /***************************************************************************/
  /* Whether any orders are allowed (as opposed to any orders).              */
  /***************************************************************************/
  VOS_BOOL any_orders_allowed;

  /***************************************************************************/
  /* Whether partial draw are allowed.                                       */
  /***************************************************************************/
  VOS_BOOL partial_draws_allowed;

  /***************************************************************************/
  /* No-press period.  The time (in seconds) before the deadline during      */
  /* which press cannot be sent.                                             */
  /***************************************************************************/
  VOS_U16 no_press_period;

  /***************************************************************************/
  /* Whether press is disabled during the retreat phase.                     */
  /***************************************************************************/
  VOS_BOOL retreat_press_disabled;

  /***************************************************************************/
  /* Whether press is disabled during the build phase.                       */
  /***************************************************************************/
  VOS_BOOL build_press_disabled;

} DCSP_LSS_HLO;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/* DCSP_LSS_HST !! This is still under consideration by the syntax group.    */
/*****************************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_HUH                                                   */
/*                                                                           */
/* Description: Huh? message.                                                */
/*                                                                           */
/* Server-AI: The syntax of the message is incorrect.  The ERR token is      */
/*            inserted immediately before the offending token.               */
/*                                                                           */
/* Press:     The recipient of the original message did not understand it    */
/*            and has inserted ERR before the token that it did not          */
/*            understand.                                                    */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_huh
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The offending message follows immediately as a byte array.              */
  /***************************************************************************/

} DCSP_LSS_HUH;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_IAM                                                   */
/*                                                                           */
/* Description: "I am" message.                                              */
/*                                                                           */
/* AI-Server: Request to take back a power following loss of connection.     */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_iam
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The power which is being recovered.                                     */
  /***************************************************************************/
  DCSP_LSS_TOKEN power;

  /***************************************************************************/
  /* The passcode to take back the power.                                    */
  /***************************************************************************/
  VOS_S16 passcode;

} DCSP_LSS_IAM;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_LOD                                                   */
/*                                                                           */
/* Description: Load message.                                                */
/*                                                                           */
/* Server-AI: Instruction from the server to load a game.                    */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_lod
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* Null-terminated case-insensitive gamename (not more than 8 characters   */
  /* excluding the null terminator) using only [a-z0-9_].                    */
  /***************************************************************************/
  VOS_CHAR gamename[9];

} DCSP_LSS_LOD;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_MAP                                                   */
/*                                                                           */
/* Description: Map name message.                                            */
/*                                                                           */
/* Server-AI: Notice from the server of the map being used for the game.     */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_map
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* Null-terminated case-insensitive map name (not more than 8 characters   */
  /* excluding the null terminator) using only [a-z0-9_].                    */
  /***************************************************************************/
  VOS_CHAR gamename[9];

} DCSP_LSS_MAP;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_MDF                                                   */
/*                                                                           */
/* Description: Map description.                                             */
/*                                                                           */
/* Server-AI: Details of the map being used for the game.                    */
/*                                                                           */
/* AI-Server: Requests that details of the map are sent.                     */
/*                                                                           */
/* WARNING:   Data follows this structure (2 sets).                          */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_mdf
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The number of powers that play on the map.  Each power has a power      */
  /* index from 0 to num_powers - 1.                                         */
  /***************************************************************************/
  VOS_U16 num_powers;

  /***************************************************************************/
  /* The number of provinces that make up the map.  The message is followed  */
  /* by a DCSP_LSS_PROVINCE for each province.                               */
  /***************************************************************************/
  VOS_U16 num_provinces;

  /***************************************************************************/
  /* The number of adjacent province pairs that follow the provinces.        */
  /***************************************************************************/
  VOS_U32 num_adjacencies;

} DCSP_LSS_MDF;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_MIS                                                   */
/*                                                                           */
/* Description: Missing orders message.                                      */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_mis
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* Whether this message indicates missing adjustments.  If true, only the  */
  /* num_removals field is valid.  If false, only the unit fields are valid. */
  /***************************************************************************/
  VOS_BOOL missing_adjustments;

  /***************************************************************************/
  /* When missing_adjustments, the number of removals that are missing (-ve  */
  /* for builds).                                                            */
  /***************************************************************************/
  VOS_S16 num_removals;

  /***************************************************************************/
  /* When not missing_adjustments, the number of DCSP_LSS_UNITs that follow. */
  /* The order and result associated with each unit is not valid.            */
  /***************************************************************************/
  VOS_U16 num_units;
  DCSP_LSS_UNIT unit[0];

} DCSP_LSS_MIS;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_NME                                                   */
/*                                                                           */
/* Description: Name message.                                                */
/*                                                                           */
/* AI-Server: Request to join a game.                                        */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_nme
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* Null-terminated case-insensitive AI name (not more than                 */
  /* MAX_PLAYER_NAME_LEN characters excluding the null terminator) using     */
  /* only [a-z0-9_].                                                         */
  /***************************************************************************/
  VOS_CHAR player_name[DCSP_LSS_MAX_PLAYER_NAME_LEN + 1];

  /***************************************************************************/
  /* Null-terminated case-insensitive version string (not more than          */
  /* MAX_VER_STRING_LEN characters excluding the null terminator) using only */
  /* [a-z0-9_].                                                              */
  /***************************************************************************/
  VOS_CHAR version_string[DCSP_LSS_MAX_VER_STRING_LEN + 1];

} DCSP_LSS_NME;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_NOT                                                   */
/*                                                                           */
/* Description: Not message.                                                 */
/*                                                                           */
/* Server-AI: Used before CCD only to inform the AI of a returning power.    */
/*            !! Not true any more.  See CCD.                                */
/*                                                                           */
/* AI-Server: Cancels a previous request.  (Only some requests can be        */
/*            cancelled.)                                                    */
/*                                                                           */
/* Press:     Negation of a message.                                         */
/*                                                                           */
/* WARNING:   Data follows this structure.                                   */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_not
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The negated message follows immediately.                                */
  /***************************************************************************/

} DCSP_LSS_NOT;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_NOW                                                   */
/*                                                                           */
/* Description: Now message.                                                 */
/*                                                                           */
/* AI-Server: Request for information about the current state of the board.  */
/*                                                                           */
/* Server-AI: Information about the current state of the board.              */
/*                                                                           */
/* WARNING:   Data follows this structure.                                   */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_now
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The season.                                                             */
  /***************************************************************************/
  DCSP_LSS_TOKEN season;

  /***************************************************************************/
  /* The year.                                                               */
  /***************************************************************************/
  VOS_U16 year;

  /***************************************************************************/
  /* The number of DCSP_LSS_UNITs that follow.  The order and result         */
  /* associated with each unit is not valid.                                 */
  /***************************************************************************/
  VOS_U16 num_units;
  DCSP_LSS_UNIT unit[0];

} DCSP_LSS_NOW;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/* Structure: DCSP_LSS_OBS                                                   */
/*                                                                           */
/* Description: Observer signon.                                             */
/*                                                                           */
/* AI-Server: The client wishes to observe the game.  It will not submit any */
/*            orders.                                                        */
/*                                                                           */
/* This top-level command does not have any fields.                          */
/*****************************************************************************/

typedef struct dcsp_lss_obs
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* No fields in the message.                                               */
  /***************************************************************************/

} DCSP_LSS_OBS;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_ORD                                                   */
/*                                                                           */
/* Description: Order results.                                               */
/*                                                                           */
/* Server-AI: Sent when a turn is processed with the results.                */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_ord
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The season of the turn that has just processed.                         */
  /***************************************************************************/
  DCSP_LSS_TOKEN season;

  /***************************************************************************/
  /* The year of the turn that has just processed.                           */
  /***************************************************************************/
  VOS_U16 year;

  /***************************************************************************/
  /* The order (including details of the unit being ordered).                */
  /***************************************************************************/
  DCSP_LSS_ORDER order;

  /***************************************************************************/
  /* The result of the order.                                                */
  /***************************************************************************/
  DCSP_LSS_TOKEN result;

} DCSP_LSS_ORD;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_PRN                                                   */
/*                                                                           */
/* Description: Parenthesis mismatch message.                                */
/*                                                                           */
/* Server-AI: There were a different number of open and close parentheses in */
/*            a message sent to the server.                                  */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_prn
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The message which caused the problem.  This is the raw message as       */
  /* received off the wire (because messages with mismatched brackets cannot */
  /* be decoded).                                                            */
  /***************************************************************************/
  DCSP_RAW_MESSAGE *raw_message;

} DCSP_LSS_PRN;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_REJ                                                   */
/*                                                                           */
/* Description: Reject message.                                              */
/*                                                                           */
/* Server-AI: The server has rejected a message sent to it.                  */
/*                                                                           */
/* Press:     The recipient of a message rejected a proposal.                */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_rej
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The rejected message follows immediately.                               */
  /***************************************************************************/

} DCSP_LSS_REJ;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_SCO                                                   */
/*                                                                           */
/* Description: Supply centre ownership message.                             */
/*                                                                           */
/* AI-Server: Request for information about supply centre ownership.         */
/*                                                                           */
/* Server-AI: Information about supply centre ownership.                     */
/*                                                                           */
/* WARNING:   Data follows this structure.                                   */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_sco
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The number of DCSP_LSS_SCO_PAIR structures that follow this message.    */
  /***************************************************************************/
  VOS_U16 num_sco_pairs;
  DCSP_LSS_SCO_PAIR sco_pair[0];

} DCSP_LSS_SCO;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_SLO                                                   */
/*                                                                           */
/* Description: Solo message.                                                */
/*                                                                           */
/* Server-AI: The game has finished in a solo.                               */
/*                                                                           */
/* Press:     A solo is proposed.                                            */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_slo
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The winning power.                                                      */
  /***************************************************************************/
  DCSP_LSS_TOKEN winner;

} DCSP_LSS_SLO;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_SND                                                   */
/*                                                                           */
/* Description: Send message.                                                */
/*                                                                           */
/* AI-Server: Request to pass a message to another power.                    */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_snd
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The offset from the start of this structure of the top-level message    */
  /* header for the message to send.  This will always be immediately after  */
  /* the recipients.                                                         */
  /***************************************************************************/
  VOS_U16 message_offset;

  /***************************************************************************/
  /* The number of powers that the message is sent to.  The power index of   */
  /* each recipient (each a VOS_U8) follows this message.                    */
  /***************************************************************************/
  VOS_U16 num_recipients;

} DCSP_LSS_SND;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_SUB                                                   */
/*                                                                           */
/* Description: Submit a set of orders.                                      */
/*                                                                           */
/* AI-Server: Submission of a set of orders.                                 */
/*                                                                           */
/* WARNING:   Data follows this structure.                                   */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_sub
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The number of DCSP_LSS_ORDER structures that follow this message.       */
  /***************************************************************************/
  VOS_U16 num_orders;
  DCSP_LSS_ORDER order[0];

} DCSP_LSS_SUB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_SVE                                                   */
/*                                                                           */
/* Description: Save message.                                                */
/*                                                                           */
/* Server-AI: Instruction from the server to save a game.                    */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_sve
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* Null-terminated case-insensitive gamename (not more than 8 characters   */
  /* excluding the null terminator) using only [a-z0-9_].                    */
  /***************************************************************************/
  VOS_CHAR gamename[9];

} DCSP_LSS_SVE;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_THX                                                   */
/*                                                                           */
/* Description: Thanks message.                                              */
/*                                                                           */
/* Server-AI: Acknowledgement of order receipt.                              */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_thx
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The order being acknowledged.                                           */
  /***************************************************************************/
  DCSP_LSS_ORDER order;

  /***************************************************************************/
  /* The order note.                                                         */
  /***************************************************************************/
  DCSP_LSS_TOKEN note;

} DCSP_LSS_THX;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_TME                                                   */
/*                                                                           */
/* Description: Time message.                                                */
/*                                                                           */
/* AI-Server: Request that the server notifies the AI when there are 'time'  */
/*            seconds until the deadline.                                    */
/*                                                                           */
/* Server-AI: Notification that there are 'time' seconds until the deadline. */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_tme
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* Time until the deadline.                                                */
  /***************************************************************************/
  VOS_S16 time;

} DCSP_LSS_TME;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: DCSP_LSS_YES                                                   */
/*                                                                           */
/* Description: Yes message.                                                 */
/*                                                                           */
/* Server-AI: A message/request has been accepted.                           */
/*                                                                           */
/* Press:     A proposal has been accepted.                                  */
/*                                                                           */
/*****************************************************************************/

typedef struct dcsp_lss_yes
{
  /***************************************************************************/
  /* The top-level message header.                                           */
  /***************************************************************************/
  DCSP_LSS_TLM_HDR hdr;

  /***************************************************************************/
  /* The accepted message follows immediately.                               */
  /***************************************************************************/

} DCSP_LSS_YES;

/**STRUCT-********************************************************************/

#pragma warning (pop)

/*****************************************************************************/
/*****************************************************************************/
/* Callback Prototypes.                                                      */
/*****************************************************************************/
/*****************************************************************************/
typedef VOS_VOID(DCSP_LSS_TLM_DECODER)(VOS_U16 **, DCSP_LSS_TLM_HDR *);
typedef VOS_BOOL(DCSP_LSS_TLM_ENCODER)(VOS_U16 **,
                                       VOS_U16,
                                       VOS_U8,
                                       DCSP_LSS_TLM_HDR *);

#endif

