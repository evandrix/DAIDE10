/**MOD+***********************************************************************/
/* Module:    hldbmain.c                                                     */
/*                                                                           */
/* Purpose:   Diplomacy holdbot.                                             */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**MOD-***********************************************************************/

#include "hldbincl.h"

VOS_S32 __stdcall WinMain(HINSTANCE hInst,
                          HINSTANCE hPrevInst,
                          LPSTR cmdLine,
                          VOS_S32 show)
{
  DCSP_USAGE_OPTIONS *options;
  VOS_U16 ai_name_msg[] = {0,
                           34,
                           DCSP_HST_NME,
                           DCSP_HST_BRACKET_OPEN,
                           DCSP_HST_CHAR('h'),
                           DCSP_HST_CHAR('o'),
                           DCSP_HST_CHAR('l'),
                           DCSP_HST_CHAR('d'),
                           DCSP_HST_CHAR('b'),
                           DCSP_HST_CHAR('o'),
                           DCSP_HST_CHAR('t'),
                           DCSP_HST_BRACKET_CLOSE,
                           DCSP_HST_BRACKET_OPEN,
                           DCSP_HST_CHAR('0'),
                           DCSP_HST_CHAR('0'),
                           DCSP_HST_CHAR('.'),
                           DCSP_HST_CHAR('0'),
                           DCSP_HST_CHAR('1'),
                           DCSP_HST_BRACKET_CLOSE};
  VOS_BOOL keep_going = TRUE;
  HLDB_THREAD_DATA *thread_data = NULL;
  HLDB_MSG *hldb_msg;
  VOS_S32 wait_rc;
  HLDB_LOCAL_DATA local;
  VOS_CHAR *host = "localhost";
  VOS_U8 host_type = DCSP_ADDR_FORMAT_HOST_NAME;
  VOS_U16 port = 16713;

  VOS_UNREFERENCED_PARAMETER(hInst);
  VOS_UNREFERENCED_PARAMETER(hPrevInst);
  VOS_UNREFERENCED_PARAMETER(show);

  /***************************************************************************/
  /* Set-up the message type for the NME message.                            */
  /***************************************************************************/
  *((VOS_U8 *)ai_name_msg) = DCSP_MSG_TYPE_DM;

  /***************************************************************************/
  /* Check and set the options from the command line.                        */
  /***************************************************************************/
  if (!hldb_set_parameters(cmdLine, &host, &host_type, &port))
  {
    MessageBox(
           NULL,
           "Syntax: holdbot [-nhost_name|-iip_address] [-pport]",
           "Hold-bot",
           0);
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Allocate thread-control objects.                                        */
  /***************************************************************************/
  thread_data = malloc(sizeof(HLDB_THREAD_DATA));
  if (thread_data == NULL)
  {
    MessageBox(NULL, "Out of memory", "Hold-bot", 0);
    goto EXIT_LABEL;
  }
  thread_data->semaphore = CreateSemaphore(NULL, 0, VOS_MAX_PSINT, NULL);
  thread_data->mutex = CreateMutex(NULL, FALSE, NULL);
  if ((thread_data->semaphore == NULL) || (thread_data->mutex == NULL))
  {
    MessageBox(NULL, "Failed to allocate thread controls", "Hold-bot", 0);
    goto EXIT_LABEL;
  }
  VOS_Q_INIT_ROOT(thread_data->msg_q);

  /***************************************************************************/
  /* Initialise the local data.                                              */
  /***************************************************************************/
  local.power = 0;
  local.num_scs = 0;

  /***************************************************************************/
  /* Set up the client options.  For ease of use, the holdbot uses           */
  /* Host-Order messages.  (Real AIs will probably wish to use network order */
  /* to improve performance.)                                                */
  /***************************************************************************/
  options = dcsp_configure("holdbot.cfg");

  options->client = TRUE;
  options->scheduling = DCSP_SCHEDULE_THREAD;
  options->notification = DCSP_NOTIFICATION_CALLBACK;
  options->representation = DCSP_REPRESENTATION_HOST_ORDER;
  options->message_callback = hldb_receive_message;
  options->message_callback_data = (VOS_VOID *)thread_data;

  /***************************************************************************/
  /* Initialise the DLL.                                                     */
  /***************************************************************************/
  if (!dcsp_start(options))
  {
    MessageBox(NULL, "Failed to initialise the DLL", "Hold-bot", 0);
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Display an error message and exit if the server details aren't valid.   */
  /***************************************************************************/
  if (!dcsp_convert_address(host, host_type, port))
  {
    MessageBox(NULL,
               "Invalid server address (check -n/-i parameter).",
               "Hold-bot",
               0);
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Open a connection to the specified server.  Default is localhost:16713. */
  /***************************************************************************/
  dcsp_client_connect();

  /***************************************************************************/
  /* Send the NME message.                                                   */
  /***************************************************************************/
  dcsp_client_send_message((VOS_CHAR *)ai_name_msg,
                           sizeof(ai_name_msg));

  do
  {
    /*************************************************************************/
    /* Wait for a message.                                                   */
    /*************************************************************************/
    wait_rc = WaitForSingleObject(thread_data->semaphore, INFINITE);
    VOS_ASSERT((wait_rc == WAIT_ABANDONED) || (wait_rc == WAIT_OBJECT_0));

    /*************************************************************************/
    /* If a message is waiting, process it.                                  */
    /*************************************************************************/
    wait_rc = WaitForSingleObject(thread_data->mutex, INFINITE);
    hldb_msg = (HLDB_MSG *)VOS_Q_FIRST(thread_data->msg_q);
    VOS_ASSERT(hldb_msg != NULL);
    VOS_Q_DEQUEUE(hldb_msg->q_hdr, thread_data->msg_q);
    ReleaseMutex(thread_data->mutex);

    /*************************************************************************/
    /* Process and free the message.                                         */
    /*************************************************************************/
    keep_going = hldb_process_msg(&local,
                                  ((VOS_CHAR *)(hldb_msg)) + sizeof(HLDB_MSG));
    free(hldb_msg);

  } while (keep_going);

  /***************************************************************************/
  /* Stop the protocol DLL.                                                  */
  /***************************************************************************/
  dcsp_stop();

EXIT_LABEL:

  /***************************************************************************/
  /* Tidy up the thread data.                                                */
  /***************************************************************************/
  if (thread_data != NULL)
  {
    /*************************************************************************/
    /* Free the semaphore if it exists.                                      */
    /*************************************************************************/
    if (thread_data->semaphore != NULL)
    {
      CloseHandle(thread_data->semaphore);
    }

    /*************************************************************************/
    /* Free the mutex if it exists.                                          */
    /*************************************************************************/
    if (thread_data->mutex != NULL)
    {
      CloseHandle(thread_data->mutex);
    }

    /*************************************************************************/
    /* Free the memory.                                                      */
    /*************************************************************************/
    free(thread_data);
  }

  return(0);
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  hldb_process_msg                                              */
/*                                                                           */
/* Purpose:    Process a message.                                            */
/*                                                                           */
/* Parameters: IN     local       - The local holdbot data.                  */
/*             IN     msg         - The message to be processed.             */
/*                                                                           */
/* Returns:    Whether the holdbot should continue processing messages.      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_BOOL hldb_process_msg(HLDB_LOCAL_DATA *local, VOS_CHAR *msg)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_BOOL rc = TRUE;
  DCSP_HST_MESSAGE *hdr = (DCSP_HST_MESSAGE *)msg;
  VOS_U16 *token = (VOS_U16 *)(hdr + 1);

  switch (hdr->type)
  {
    case DCSP_MSG_TYPE_RM:
      /***********************************************************************/
      /* Representation message.  Ignore it.                                 */
      /***********************************************************************/
      break;

    case DCSP_MSG_TYPE_DM:
      /***********************************************************************/
      /* Diplomacy message.  We may need to act on it depending on the first */
      /* token.                                                              */
      /***********************************************************************/
      switch (token[0])
      {
        case DCSP_HST_MAP:
          /*******************************************************************/
          /* Accept all maps.                                                */
          /*******************************************************************/
          hldb_accept_message(hdr);
          break;

        case DCSP_HST_HLO:
          /*******************************************************************/
          /* Extract the power from the HLO message.  (It is the third       */
          /* token.  It follows the HLO token and an open bracket.)          */
          /*******************************************************************/
          local->power = token[2];
          break;

        case DCSP_HST_NOW:
          /*******************************************************************/
          /* Process the NOW message.                                        */
          /*******************************************************************/
          hldb_process_now(local, hdr);
          break;

        case DCSP_HST_OFF:
          /*******************************************************************/
          /* OFF message.  Stop the holdbot.                                 */
          /*******************************************************************/
          rc = FALSE;
          break;

        case DCSP_HST_SCO:
          /*******************************************************************/
          /* SCO message.  Waive builds as required.                         */
          /*******************************************************************/
          hldb_process_sco(local, hdr);
          break;

        default:
          /*******************************************************************/
          /* We don't care about any other messages.  Don't do anything.     */
          /*******************************************************************/
          break;
      }
      break;

    case DCSP_MSG_TYPE_LM:
      /***********************************************************************/
      /* Local message.  We may need to act on it.                           */
      /***********************************************************************/
      rc = hldb_process_local_message(hdr);
      break;

    default:
      /***********************************************************************/
      /* We don't care about any other messages.  Don't do anything.         */
      /***********************************************************************/
      break;
  }

  return(rc);
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  hldb_accept_message                                           */
/*                                                                           */
/* Purpose:    Convert a message into YES(msg).                              */
/*                                                                           */
/* Parameters: IN     msg         - The message to convert.                  */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_VOID hldb_accept_message(DCSP_HST_MESSAGE *msg)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_U16 *token = (VOS_U16 *)(msg + 1);
  DCSP_HST_MESSAGE *new_msg;
  VOS_U16 *new_token;

  /***************************************************************************/
  /* Allocate a buffer 6 bytes larger than the message.  This is enough for  */
  /* the additional three tokens YES, ( and ).                               */
  /***************************************************************************/
  new_msg = (DCSP_HST_MESSAGE *)malloc(sizeof(DCSP_HST_MESSAGE) +
                                       msg->length +
                                       6);
  if (new_msg == NULL)
  {
    /*************************************************************************/
    /* Failed to allocate the new message.  The only time this function is   */
    /* called is converting a MAP message into a YES(MAP) message.  The      */
    /* game can't continue unless we ACK the MAP message so exit.            */
    /*************************************************************************/
    VOS_ASSERT_INVALID_BRANCH();
  }

  new_token = (VOS_U16 *)(new_msg + 1);

  /***************************************************************************/
  /* Fill in the message header.                                             */
  /***************************************************************************/
  new_msg->type = msg->type;
  new_msg->length = msg->length + 6;

  /***************************************************************************/
  /* Add the YES and ( to the start of the message.                          */
  /***************************************************************************/
  new_token[0] = DCSP_HST_YES;
  new_token[1] = DCSP_HST_BRACKET_OPEN;

  /***************************************************************************/
  /* Copy the message.                                                       */
  /***************************************************************************/
  memcpy(&new_token[2], token, msg->length);

  /***************************************************************************/
  /* Add the closing ) to the end of the message.                            */
  /***************************************************************************/
  new_token[(new_msg->length >> 1) - 1] = DCSP_HST_BRACKET_CLOSE;

  /***************************************************************************/
  /* Send the message.                                                       */
  /***************************************************************************/
  dcsp_client_send_message((VOS_CHAR *)new_msg,
                           sizeof(DCSP_HST_MESSAGE) + new_msg->length);

  return;
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  hldb_process_now                                              */
/*                                                                           */
/* Purpose:    Process a NOW message.                                        */
/*                                                                           */
/* Parameters: IN     local       - The local holdbot data.                  */
/*             IN     msg         - The message to be processed.             */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_VOID hldb_process_now(HLDB_LOCAL_DATA *local, DCSP_HST_MESSAGE *msg)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_BOOL found = FALSE;
  VOS_U16 order[400];
  VOS_U16 *next_order;
  VOS_U16 *last_order;
  VOS_U8 nested = 0;
  VOS_U16 *token;
  VOS_U16 *token_ptr;
  VOS_U8 *end_ptr;

  /***************************************************************************/
  /* The tokens start after the header.                                      */
  /***************************************************************************/
  token = (VOS_U16 *)(((VOS_U8 *)msg) + sizeof(DCSP_HST_MESSAGE));
  end_ptr = ((VOS_U8 *)token) + msg->length;

  /***************************************************************************/
  /* Fill in the order header.                                               */
  /***************************************************************************/
  ((DCSP_HST_MESSAGE *)order)->type = DCSP_MSG_TYPE_DM;
  next_order = (VOS_U16 *)(((VOS_U8 *)order + sizeof(DCSP_HST_MESSAGE)));
  *next_order = DCSP_HST_SUB;
  next_order++;

  /***************************************************************************/
  /* Find our units.  If we're processing a retreat phase, find all those    */
  /* units of ours which are dislodged.                                      */
  /***************************************************************************/
  for (token_ptr = hldb_locate_power(token,
                                     local->power,
                                     (VOS_U16 *)end_ptr);
       token_ptr != NULL;
       token_ptr = hldb_locate_power(token_ptr,
                                     local->power,
                                     (VOS_U16 *)end_ptr))
  {
    /*************************************************************************/
    /* Store the current position in the order so that this order can be     */
    /* undone.                                                               */
    /*************************************************************************/
    last_order = next_order;

    /*************************************************************************/
    /* Add the opening bracket for the order for this unit.                  */
    /*************************************************************************/
    *next_order = DCSP_HST_BRACKET_OPEN;
    next_order++;

    /*************************************************************************/
    /* Copy the unit into the order string.                                  */
    /*************************************************************************/
    do
    {
      /***********************************************************************/
      /* Note the level of nesting.                                          */
      /***********************************************************************/
      if (*token_ptr == DCSP_HST_BRACKET_OPEN)
      {
        nested++;
      }
      else if (*token_ptr == DCSP_HST_BRACKET_CLOSE)
      {
        nested--;
      }

      /***********************************************************************/
      /* Copy a character from the unit definition into the order.           */
      /***********************************************************************/
      *next_order = *token_ptr;
      next_order++;
      token_ptr++;

      /***********************************************************************/
      /* Keep going until we've written out all the unit information (and    */
      /* therefore become completely unnested) or we've discovered that a    */
      /* retreat is required (in which case we'll be doubly nested).         */
      /***********************************************************************/
    } while ((nested > 0) && (*token_ptr != DCSP_HST_MRT));

    /*************************************************************************/
    /* Check the phase and the reason that we've stopped writing out the     */
    /* unit name.                                                            */
    /*************************************************************************/
    if ((token[2] == DCSP_HST_SUM) || (token[2] == DCSP_HST_AUT))
    {
      if (nested)
      {
        /*********************************************************************/
        /* We're in a retreat phase and still nested.  Therefore, a retreat  */
        /* order is required.  Once we've added this, we'll no longer be     */
        /* nested.                                                           */
        /*********************************************************************/
        next_order[0] = DCSP_HST_BRACKET_CLOSE;
        next_order[1] = DCSP_HST_DSB;
        next_order[2] = DCSP_HST_BRACKET_CLOSE;
        next_order += 3;
        nested = 0;

        /*********************************************************************/
        /* Note that at least one unit has been ordered.                     */
        /*********************************************************************/
        found = TRUE;
      }
      else
      {
        /*********************************************************************/
        /* We're in a retreat phase and haven't found an MRT.  Therefore, no */
        /* order is required for this unit.  Rollback to the last order      */
        /* position (so we'll overwrite this attempt to order a unit which   */
        /* didn't need a retreat).                                           */
        /*********************************************************************/
        next_order = last_order;
      }
    }
    else if ((token[2] == DCSP_HST_SPR) || (token[2] == DCSP_HST_FAL))
    {
      /***********************************************************************/
      /* We're in a movement phase.  An order is always required and we'll   */
      /* never be nested still.                                              */
      /***********************************************************************/
      next_order[0] = DCSP_HST_HLD;
      next_order[1] = DCSP_HST_BRACKET_CLOSE;
      next_order += 2;
      VOS_ASSERT(nested == 0);

      /***********************************************************************/
      /* Note that at least one unit has been ordered.                       */
      /***********************************************************************/
      found = TRUE;
    }
    else if (token[2] == DCSP_HST_WIN)
    {
      /***********************************************************************/
      /* At the start of this function local->num_scs contained the number   */
      /* of SCs that the holdbot owned.  This is decremented as each unit is */
      /* found in the NOW message.  If the number of SCs remaining has       */
      /* reached zero and we've got here then remove the unit.               */
      /*                                                                     */
      /* If we have more SCs than units then at the end of the for-loop,     */
      /* local->num_scs will not be zero.  In that cause, waive orders are   */
      /* sent.                                                               */
      /***********************************************************************/
      if (local->num_scs > 0)
      {
        /*********************************************************************/
        /* We're allowed to have this unit but it counts against the number  */
        /* of SCs.  No order is required for this unit.  Rollback to the     */
        /* last order position (so we'll overwrite this attempt to order a   */
        /* unit which didn't need ordering).                                 */
        /*********************************************************************/
        local->num_scs--;
        next_order = last_order;
      }
      else
      {
        /*********************************************************************/
        /* We don't have enough SCs to support this unit.  Add a remove      */
        /* order for it.                                                     */
        /*********************************************************************/
        next_order[0] = DCSP_HST_REM;
        next_order[1] = DCSP_HST_BRACKET_CLOSE;
        next_order += 2;
        VOS_ASSERT(nested == 0);

        /*********************************************************************/
        /* Note that at least one unit has been ordered.                     */
        /*********************************************************************/
        found = TRUE;
      }
    }
    else
    {
      /***********************************************************************/
      /* Invalid season.                                                     */
      /***********************************************************************/
      VOS_ASSERT_INVALID_BRANCH();
    }
  }

  /***************************************************************************/
  /* If we're in a winter adjustment phase and we've got more SCs than units */
  /* (local->num_scs > 0) then add waive orders for the remaining units.     */
  /***************************************************************************/
  if ((token[2] == DCSP_HST_WIN) && (local->num_scs > 0))
  {
    /*************************************************************************/
    /* Order WVEs for each extra SC.                                         */
    /*************************************************************************/
    while (local->num_scs > 0)
    {
      /***********************************************************************/
      /* Order a waive.                                                      */
      /***********************************************************************/
      next_order[0] = DCSP_HST_BRACKET_OPEN;
      next_order[1] = local->power;
      next_order[2] = DCSP_HST_WVE;
      next_order[3] = DCSP_HST_BRACKET_CLOSE;
      next_order += 4;

      /***********************************************************************/
      /* There is one less build available now.                              */
      /***********************************************************************/
      local->num_scs--;
    }
    VOS_ASSERT(local->num_scs == 0);

    /*************************************************************************/
    /* Note that at least one unit has been ordered.                         */
    /*************************************************************************/
    found = TRUE;
  }

  /***************************************************************************/
  /* If we've found at least 1 unit to order then fill in the message length */
  /* and send the message.                                                   */
  /***************************************************************************/
  if (found)
  {
    ((DCSP_HST_MESSAGE *)order)->length = (VOS_U16)
           ((VOS_U8 *)next_order - (VOS_U8 *)order) - sizeof(DCSP_HST_MESSAGE);
    dcsp_client_send_message(
                            (VOS_CHAR *)order,
                            (VOS_S32)((VOS_U8 *)next_order - (VOS_U8 *)order));
  }

  return;
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  hldb_process_sco                                              */
/*                                                                           */
/* Purpose:    Process a SCO message.                                        */
/*                                                                           */
/* Parameters: IN     local       - The local holdbot data.                  */
/*             IN     msg         - The message to be processed.             */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_VOID hldb_process_sco(HLDB_LOCAL_DATA *local, DCSP_HST_MESSAGE *msg)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_U16 *start;
  VOS_U16 *end_ptr;
  VOS_U16 *token_ptr;

  /***************************************************************************/
  /* Locate the holdbot's power.                                             */
  /***************************************************************************/
  start = (VOS_U16 *)(((VOS_U8 *)msg) + sizeof(DCSP_HST_MESSAGE));
  end_ptr = (VOS_U16 *)(((VOS_U8 *)start) + msg->length);
  token_ptr = hldb_locate_power(start, local->power, end_ptr);

  /***************************************************************************/
  /* If the holdbot's power is listed in the SCO message then count the      */
  /* number of SCs that the holdbot owns (otherwise it doesn't own any).     */
  /* This is all there is to do on a SCO message.  The adjustments will been */
  /* done when the subsequent NOW arrives.                                   */
  /***************************************************************************/
  local->num_scs = 0;
  if (token_ptr != NULL)
  {
    for (token_ptr += 2; *token_ptr != DCSP_HST_BRACKET_CLOSE; token_ptr++)
    {
      local->num_scs++;
    }
  }

  return;

}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  hldb_process_local_message                                    */
/*                                                                           */
/* Purpose:    Process a local message.                                      */
/*                                                                           */
/* Parameters: IN     msg         - The local message.                       */
/*                                                                           */
/* Returns:    TRUE if the holdbot should keep going.  FALSE if it should    */
/*             quit.                                                         */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_BOOL hldb_process_local_message(DCSP_HST_MESSAGE *msg)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_BOOL rc;
  DCSP_HST_LM *lm = (DCSP_HST_LM *)msg;

  if ((lm->event == DCSP_HST_LOCAL_EVENT_CONNECTED) && (lm->err == 0))
  {
    /*************************************************************************/
    /* This is a successful connect message so keep going.                   */
    /*************************************************************************/
    rc = TRUE;
  }
  else
  {
    /*************************************************************************/
    /* Anything other than an okay connect (including failed connect and     */
    /* disconnect causes us to stop).  Print out what happened first.        */
    /*************************************************************************/
    switch (lm->event)
    {
      case DCSP_HST_LOCAL_EVENT_CONNECTED:
        /*********************************************************************/
        /* Connection failure.                                               */
        /*********************************************************************/
        MessageBox(NULL,
                   "Connection failure.  Check host (-n/-i) and port (-p).",
                   "Hold-bot",
                   0);
        break;

      case DCSP_HST_LOCAL_EVENT_CLOSED:
        /*********************************************************************/
        /* The connection to the server has been lost.                       */
        /*********************************************************************/
        MessageBox(NULL,
                   "The connection to the server has been lost.",
                   "Hold-bot",
                   0);
        break;

      case DCSP_HST_LOCAL_EVENT_OUT_OF_MEM:
        /*********************************************************************/
        /* Out of memory.                                                    */
        /*********************************************************************/
        MessageBox(NULL,
                   "Out of memory.  (Exiting.)",
                   "Hold-bot",
                   0);
        break;

      default:
        VOS_ASSERT_INVALID_BRANCH();
        break;
    }

    /*************************************************************************/
    /* Stop the holdbot as a result.                                         */
    /*************************************************************************/
    rc = FALSE;
  }

  return(rc);
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  hldb_receive_message                                          */
/*                                                                           */
/* Purpose:    Callback to receive messages.                                 */
/*                                                                           */
/* Parameters: IN     msg         - The received message.                    */
/*             IN     length      - The length of the message.               */
/*             IN     conn_id     - The connection ID.                       */
/*             IN     data        - The callback data passed to the DLL in   */
/*                                  the opt structure.                       */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_VOID hldb_receive_message(VOS_CHAR *msg,
                              VOS_S32 length,
                              VOS_S32 conn_id,
                              VOS_VOID *data)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_S32 wait_rc;
  HLDB_THREAD_DATA *thread_data = (HLDB_THREAD_DATA *)data;
  HLDB_MSG *hldb_msg;
  VOS_CHAR *msg_ptr;

  /***************************************************************************/
  /* Copy the message into a structure ready for putting on the message      */
  /* queue.                                                                  */
  /***************************************************************************/
  hldb_msg = malloc(sizeof(HLDB_MSG) + length);
  if (hldb_msg == NULL)
  {
    /*************************************************************************/
    /* Failed to allocate memory to process the message.  If this message    */
    /* is one of the messages we care about then we're done for.  However,   */
    /* it's quite likely that we don't care about the message so just ignore */
    /* it.                                                                   */
    /*************************************************************************/
    goto EXIT_LABEL;
  }

  VOS_Q_INIT_ITEM(hldb_msg->q_hdr);
  hldb_msg->length = length;
  hldb_msg->conn_id = conn_id;
  msg_ptr = ((VOS_CHAR *)hldb_msg) + sizeof(HLDB_MSG);
  memcpy(msg_ptr, msg, length);

  /***************************************************************************/
  /* Queue the message for processing and signal that there is another       */
  /* message waiting.                                                        */
  /***************************************************************************/
  wait_rc = WaitForSingleObject(thread_data->mutex, INFINITE);
  VOS_ASSERT((wait_rc == WAIT_OBJECT_0) || (wait_rc == WAIT_ABANDONED));
  VOS_Q_ENQUEUE(hldb_msg->q_hdr, thread_data->msg_q);
  ReleaseMutex(thread_data->mutex);
  if (!ReleaseSemaphore(thread_data->semaphore, 1, NULL))
  {
    /*************************************************************************/
    /* We failed to signal that there was another message waiting.  This     */
    /* means that there are already MAX_INT messages waiting.  Something has */
    /* gone very badly wrong.                                                */
    /*************************************************************************/
    VOS_ASSERT_INVALID_BRANCH();
  }

EXIT_LABEL:

  return;
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  hldb_locate_power                                             */
/*                                                                           */
/* Purpose:    Locate the holdbot's power within a set of tokens.            */
/*                                                                           */
/* Parameters: IN     token       The tokens to search through.              */
/*             IN     power       The power to search for.                   */
/*             IN     end         A pointer to end of the tokens.            */
/*                                                                           */
/* Returns:    A pointer to the open bracket immediately before the first    */
/*             matching power token.                                         */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_U16 *hldb_locate_power(VOS_U16 *token,
                           VOS_U16 power,
                           VOS_U16 *end)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/

  /***************************************************************************/
  /* If we're already too near the end of the message then there aren't any  */
  /* more matching tokens.                                                   */
  /***************************************************************************/
  if (token >= (end - 1))
  {
    token = NULL;
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Search for an open bracket followed by the power.                       */
  /***************************************************************************/
  while (token < (end - 1))
  {
    if ((*token == DCSP_HST_BRACKET_OPEN) && (*(token + 1) == power))
    {
      goto EXIT_LABEL;
    }
    token++;
  }
  VOS_ASSERT(token == (end - 1));

  /***************************************************************************/
  /* We haven't found the power.                                             */
  /***************************************************************************/
  token = NULL;

EXIT_LABEL:

  return(token);
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  hldb_set_parameters                                           */
/*                                                                           */
/* Purpose:    Parse the command line and set the parameters.                */
/*                                                                           */
/* Parameters: IN     cmd_line    - The command line                         */
/*             OUT    host        - The specified host name/address.         */
/*             OUT    host_type   - Type (name or IP address) of host.       */
/*             OUT    port        - The specified port.                      */
/*             OUT    log_level   - The specified log level.                 */
/*                                                                           */
/* Returns:    TRUE if the command line was valid.  FALSE otherwise.         */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_BOOL hldb_set_parameters(VOS_CHAR *cmd_line,
                             VOS_CHAR **host,
                             VOS_U8 *host_type,
                             VOS_U16 *port)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_BOOL rc;
  VOS_CHAR *param;
  VOS_S32 num;

  /***************************************************************************/
  /* The empty command line is always valid but contains no parameters to be */
  /* parsed.                                                                 */
  /***************************************************************************/
  if ((cmd_line == NULL) || (cmd_line[0] == '\0'))
  {
    rc = TRUE;
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Get the first parameter.                                                */
  /***************************************************************************/
  for (param = strtok(cmd_line, " ");
       param != NULL;
       param = strtok(NULL, " "))
  {
    /*************************************************************************/
    /* All parameters must start with a '-' otherwise the command line is    */
    /* invalid.                                                              */
    /*************************************************************************/
    if (param[0] != '-')
    {
      rc = FALSE;
      goto EXIT_LABEL;
    }

    /*************************************************************************/
    /* The next character must be one of...                                  */
    /*                                                                       */
    /* 'n' for a host name                                                   */
    /* 'i' for an IP address                                                 */
    /* 'p' for a port                                                        */
    /* 'l' for a logging level                                               */
    /*************************************************************************/
    switch (param[1])
    {
      case 'n':
      case 'N':
        /*********************************************************************/
        /* Host name.                                                        */
        /*********************************************************************/
        *host_type = DCSP_ADDR_FORMAT_HOST_NAME;
        *host = &param[2];
        break;

      case 'i':
      case 'I':
        /*********************************************************************/
        /* IP address.                                                       */
        /*********************************************************************/
        *host_type = DCSP_ADDR_FORMAT_DOTTED_DECIMAL;
        *host = &param[2];
        break;

      case 'p':
      case 'P':
        /*********************************************************************/
        /* Port number.  Must be 0 <= p <= VOS_MAX_U16.                      */
        /*********************************************************************/
        num = atoi(&param[2]);
        if ((num < 0) || (num > VOS_MAX_U16))
        {
          rc = FALSE;
          goto EXIT_LABEL;
        }
        *port = (VOS_U16)num;
        break;

      default:
        /*********************************************************************/
        /* Invalid switch.                                                   */
        /*********************************************************************/
        rc = FALSE;
        goto EXIT_LABEL;
        /* break;                                                            */
    }
  }

  /***************************************************************************/
  /* All parameters converted.                                               */
  /***************************************************************************/
  rc = TRUE;

EXIT_LABEL:

  return(rc);
}

