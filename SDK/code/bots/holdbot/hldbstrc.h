/**INC+***********************************************************************/
/* Header:    hldbstrc.h                                                     */
/*                                                                           */
/* Purpose:   Holdbot structures.                                            */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef HLDBSTRC_INCLUDED
#define HLDBSTRC_INCLUDED

/**STRUCT+********************************************************************/
/* Structure: HLDB_LOCAL_DATA                                                */
/*                                                                           */
/* Description: Local holdbot data.                                          */
/*                                                                           */
/*****************************************************************************/

typedef struct hldb_local_data
{
  /***************************************************************************/
  /* The token representing the holdbot's assigned power.                    */
  /***************************************************************************/
  VOS_U16 power;

  /***************************************************************************/
  /* The number of supply centres that the holdbot owns.  This is set when   */
  /* a SCO message is counted and used in the NOW message for the adjustment */
  /* phase which follows (if any).  After the NOW message has been processed */
  /* it will be set back to 0.                                               */
  /***************************************************************************/
  VOS_U16 num_scs;

} HLDB_LOCAL_DATA;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: HLDB_THREAD_DATA                                               */
/*                                                                           */
/* Description: Data used by the main holdbot thread that is passed back by  */
/*            the callback for synchronisation.                              */
/*                                                                           */
/*****************************************************************************/

typedef struct hldb_thread_data
{
  /***************************************************************************/
  /* A mutex to protext access to the message queue.                         */
  /***************************************************************************/
  HANDLE mutex;

  /***************************************************************************/
  /* An semaphore to signal when there are messages on the queue to be       */
  /* processed.                                                              */
  /***************************************************************************/
  HANDLE semaphore;

  /***************************************************************************/
  /* The message queue.                                                      */
  /***************************************************************************/
  VOS_Q_ROOT msg_q;

} HLDB_THREAD_DATA;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: HLDB_MSG                                                       */
/*                                                                           */
/* Description: Structure used to hold messages waiting to be processed by   */
/*            the holdbot.  The message data follows immediately after this  */
/*            control block.                                                 */
/*                                                                           */
/*****************************************************************************/

typedef struct hldb_msg
{
  /***************************************************************************/
  /* Queue header.                                                           */
  /***************************************************************************/
  VOS_Q_ITEM q_hdr;

  /***************************************************************************/
  /* The length of the message.                                              */
  /***************************************************************************/
  VOS_S32 length;

  /***************************************************************************/
  /* The connection that the message was received on.                        */
  /***************************************************************************/
  VOS_S32 conn_id;

} HLDB_MSG;

/**STRUCT-********************************************************************/

#endif

