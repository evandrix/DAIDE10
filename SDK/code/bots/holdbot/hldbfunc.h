/**INC+***********************************************************************/
/* Header:    hldbfunc.h                                                     */
/*                                                                           */
/* Purpose:   Functions for the Holdbot.                                     */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef HLDBFUNC_INCLUDED
#define HLDBFUNC_INCLUDED

/*****************************************************************************/
/* hldbmain.c                                                                */
/*****************************************************************************/
VOS_BOOL hldb_process_msg(HLDB_LOCAL_DATA *, VOS_CHAR *);
VOS_VOID hldb_accept_message(DCSP_HST_MESSAGE *);
VOS_VOID hldb_process_now(HLDB_LOCAL_DATA *, DCSP_HST_MESSAGE *);
VOS_VOID hldb_process_sco(HLDB_LOCAL_DATA *, DCSP_HST_MESSAGE *);
VOS_BOOL hldb_process_local_message(DCSP_HST_MESSAGE *);
VOS_VOID hldb_receive_message(VOS_CHAR *, int, int, VOS_VOID *);
VOS_U16 *hldb_locate_power(VOS_U16 *, VOS_U16, VOS_U16 *);
VOS_BOOL hldb_set_parameters(VOS_CHAR *,
                             VOS_CHAR **,
                             VOS_U8 *,
                             VOS_U16 *);

#endif

