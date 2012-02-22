/**INC+***********************************************************************/
/* Header:    randfunc.h                                                     */
/*                                                                           */
/* Purpose:   Functions for the Randbot.                                     */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef RANDFUNC_INCLUDED
#define RANDFUNC_INCLUDED

/*****************************************************************************/
/* randimsg.c                                                                */
/*****************************************************************************/
VOS_VOID rand_process_msg(RAND_LOCAL_DATA *, RAND_MSG *);
VOS_BOOL rand_process_now(RAND_LOCAL_DATA *, RAND_MSG *, DCSP_LSS_NOW *);
VOS_VOID rand_remove_orders(RAND_LOCAL_DATA *, AIUT_POSITION *);
VOS_BOOL rand_should_remove(AIUT_ORDER *, AIUT_POWER);
VOS_BOOL rand_orders_consistent(AIUT_POSITION *, AIUT_POWER);
AIUT_MSG_FREE_CALLBACK rand_msg_free_callback;

/*****************************************************************************/
/* randmain.c                                                                */
/*****************************************************************************/
VOS_S32 __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, VOS_S32);
VOS_VOID rand_receive_message(VOS_CHAR *, int, int, VOS_VOID *);
VOS_BOOL rand_set_parameters(VOS_CHAR *,
                             VOS_CHAR **,
                             VOS_U8 *,
                             VOS_U16 *,
                             VOS_PUINT *);

#endif

