/**MOD+***********************************************************************/
/* Module:    randmain.c                                                     */
/*                                                                           */
/* Purpose:   Diplomacy randbot.                                             */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**MOD-***********************************************************************/

#include "randincl.h"

VOS_S32 __stdcall WinMain(HINSTANCE hInst,
                          HINSTANCE hPrevInst,
                          LPSTR cmdLine,
                          VOS_S32 show)
{
#if defined(CONSBOT)
  VOS_CHAR *bot_name = "consbot";
#elif defined(SELFBOT)
  VOS_CHAR *bot_name = "selfbot";
#else
  VOS_CHAR *bot_name = "randbot";
#endif
  VOS_CHAR *bot_version = "00.02";

  DCSP_USAGE_OPTIONS *options;
  RAND_THREAD_DATA *thread_data = NULL;
  RAND_MSG *rand_msg;
  VOS_S32 wait_rc;
  RAND_LOCAL_DATA rlocal;
  VOS_CHAR *host = "localhost";
  VOS_U8 host_type = DCSP_ADDR_FORMAT_HOST_NAME;
  VOS_U16 port = 16713;
  VOS_PUINT seed = 0;

  VOS_UNREFERENCED_PARAMETER(hInst);
  VOS_UNREFERENCED_PARAMETER(hPrevInst);
  VOS_UNREFERENCED_PARAMETER(show);

  /***************************************************************************/
  /* Check and set the options from the command line.                        */
  /***************************************************************************/
  if (!rand_set_parameters(cmdLine,
                           &host,
                           &host_type,
                           &port,
                           &seed))
  {
    MessageBox(
           NULL,
           "Syntax: randbot [-n<host_name>|-i<ip_address>] [-p<port>] "
                                                                  "[-s<seed>]",
           "Randbot",
           0);
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Seed the random number generator.                                       */
  /***************************************************************************/
  if (seed == 0)
  {
    seed = (VOS_PUINT)time(NULL);
  }
  srand(seed);

  /***************************************************************************/
  /* Allocate thread-control objects.                                        */
  /***************************************************************************/
  thread_data = VOS_MEM_ALLOC(sizeof(RAND_THREAD_DATA));
  if (thread_data == NULL)
  {
    MessageBox(NULL, "Out of memory", "Randbot", 0);
    goto EXIT_LABEL;
  }
  thread_data->semaphore = CreateSemaphore(NULL, 0, VOS_MAX_PSINT, NULL);
  thread_data->mutex = CreateMutex(NULL, FALSE, NULL);
  if ((thread_data->semaphore == NULL) || (thread_data->mutex == NULL))
  {
    MessageBox(NULL, "Failed to allocate thread controls", "Randbot", 0);
    goto EXIT_LABEL;
  }
  VOS_Q_INIT_ROOT(thread_data->msg_q);

  /***************************************************************************/
  /* Set up the client options.                                              */
  /***************************************************************************/
  options = dcsp_configure("randbot.cfg");

  options->client = TRUE;
  options->scheduling = DCSP_SCHEDULE_THREAD;
  options->notification = DCSP_NOTIFICATION_CALLBACK;
  options->representation = DCSP_REPRESENTATOIN_STRUCTURED;
  options->message_callback = rand_receive_message;
  options->message_callback_data = (VOS_VOID *)thread_data;

  /***************************************************************************/
  /* Setup the local data.                                                   */
  /***************************************************************************/
  rlocal.keep_going = TRUE;
  rlocal.free_msg = FALSE;

  /***************************************************************************/
  /* Initialise the DLL.                                                     */
  /***************************************************************************/
  if (!dcsp_start(options))
  {
    MessageBox(NULL, "Failed to initialise the DLL", "Randbot", 0);
    goto EXIT_LABEL;
  }

  VOS_TRACE_ENTRY("WinMain (randbot)");
  VOS_TRACE_DETAIL(("Using random seed %d", seed));

  /***************************************************************************/
  /* Display an error message and exit if the server details aren't valid.   */
  /***************************************************************************/
  if (!dcsp_convert_address(host, host_type, port))
  {
    MessageBox(NULL,
               "Invalid server address (check -n/-i parameter).",
               "Randbot",
               0);
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Initialise the AI Utilities Library.                                    */
  /***************************************************************************/
  aiut_initialise(&(rlocal.aiut), rand_msg_free_callback);

  /***************************************************************************/
  /* Join the game.                                                          */
  /***************************************************************************/
  aiut_game_join(&(rlocal.aiut), bot_name, bot_version);

  while (rlocal.keep_going)
  {
    /*************************************************************************/
    /* Wait for a message.                                                   */
    /*************************************************************************/
    wait_rc = WaitForSingleObject(thread_data->semaphore, INFINITE);
    VOS_ASSERT((wait_rc == WAIT_ABANDONED) || (wait_rc == WAIT_OBJECT_0));

    /*************************************************************************/
    /* Get the first message from the queue.                                 */
    /*************************************************************************/
    wait_rc = WaitForSingleObject(thread_data->mutex, INFINITE);
    rand_msg = (RAND_MSG *)VOS_Q_FIRST(thread_data->msg_q);
    VOS_ASSERT(rand_msg != NULL);
    VOS_Q_DEQUEUE(rand_msg->q_hdr, thread_data->msg_q);
    ReleaseMutex(thread_data->mutex);

    /*************************************************************************/
    /* Process the message.  The message is no longer valid after            */
    /* processing.                                                           */
    /*************************************************************************/
    rand_process_msg(&rlocal, rand_msg);
    rand_msg = NULL;
  }

  /***************************************************************************/
  /* Terminate the AI Utilities Library.                                     */
  /***************************************************************************/
  aiut_terminate(&(rlocal.aiut));

  /***************************************************************************/
  /* Stop the protocol DLL.                                                  */
  /***************************************************************************/
  VOS_TRACE_EXIT("WinMain (randbot)");
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
/* Procedure:  rand_receive_message                                          */
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

VOS_VOID rand_receive_message(VOS_CHAR *msg,
                              VOS_S32 length,
                              VOS_S32 conn_id,
                              VOS_VOID *data)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_S32 wait_rc;
  RAND_THREAD_DATA *thread_data = (RAND_THREAD_DATA *)data;
  RAND_MSG *rand_msg;
  VOS_CHAR *msg_ptr;

  VOS_TRACE_ENTRY("rand_receive_message");

  /***************************************************************************/
  /* Copy the message into a structure ready for putting on the message      */
  /* queue.                                                                  */
  /***************************************************************************/
  rand_msg = VOS_MEM_ALLOC(sizeof(RAND_MSG) + length);
  if (rand_msg == NULL)
  {
    /*************************************************************************/
    /* Failed to allocate memory to process the message.  If this message    */
    /* is one of the messages we care about then we're done for.  However,   */
    /* it's quite likely that we don't care about the message so just ignore */
    /* it.                                                                   */
    /*************************************************************************/
    VOS_TRACE_BRANCH(("Failed to allocate memory"));
    goto EXIT_LABEL;
  }

  VOS_Q_INIT_ITEM(rand_msg->q_hdr);
  rand_msg->length = length;
  rand_msg->conn_id = conn_id;
  msg_ptr = ((VOS_CHAR *)rand_msg) + sizeof(RAND_MSG);
  VOS_MEM_COPY(msg_ptr, msg, length);

  /***************************************************************************/
  /* Queue the message for processing and signal that there is another       */
  /* message waiting.                                                        */
  /***************************************************************************/
  wait_rc = WaitForSingleObject(thread_data->mutex, INFINITE);
  VOS_ASSERT((wait_rc == WAIT_OBJECT_0) || (wait_rc == WAIT_ABANDONED));
  VOS_Q_ENQUEUE(rand_msg->q_hdr, thread_data->msg_q);
  ReleaseMutex(thread_data->mutex);
  if (!ReleaseSemaphore(thread_data->semaphore, 1, NULL))
  {
    /*************************************************************************/
    /* We failed to signal that there was another message waiting.  This     */
    /* means that there are already MAX_INT messages waiting.  Something has */
    /* gone very badly wrong.                                                */
    /*************************************************************************/
    VOS_TRACE_BRANCH(("Failed to release semaphore"));
    VOS_ASSERT_INVALID_BRANCH();
  }

  VOS_TRACE_EXIT("rand_receive_message");

EXIT_LABEL:

  return;
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  rand_set_parameters                                           */
/*                                                                           */
/* Purpose:    Parse the command line and set the parameters.                */
/*                                                                           */
/* Parameters: IN     cmd_line    - The command line                         */
/*             OUT    host        - The specified host name/address.         */
/*             OUT    host_type   - Type (name or IP address) of host.       */
/*             OUT    port        - The specified port.                      */
/*             OUT    seed        - The random number seed.                  */
/*                                                                           */
/* Returns:    TRUE if the command line was valid.  FALSE otherwise.         */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_BOOL rand_set_parameters(VOS_CHAR *cmd_line,
                             VOS_CHAR **host,
                             VOS_U8 *host_type,
                             VOS_U16 *port,
                             VOS_PUINT *seed)
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
    /* 's' for random number seed                                            */
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

      case 's':
      case 'S':
        /*********************************************************************/
        /* Random number seed.  Must not be 0.                               */
        /*********************************************************************/
        num = atoi(&param[2]);
        if (num == 0)
        {
          rc = FALSE;
          goto EXIT_LABEL;
        }
        *seed = (VOS_PUINT)num;
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

