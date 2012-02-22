/**INC+***********************************************************************/
/* Header:    hldbincl.h                                                     */
/*                                                                           */
/* Purpose:   Holdbot include file.                                          */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef HLDBINCL_INCLUDED
#define HLDBINCL_INCLUDED

/*****************************************************************************/
/* Platform headers.                                                         */
/*****************************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "malloc.h"
#include "stdio.h"
#include "stdlib.h"

/*****************************************************************************/
/* VirOS external headers.                                                   */
/*****************************************************************************/
#include "vosextn.h"

/*****************************************************************************/
/* Diplomacy AI Client-Server Protocol DLL External Headers.                 */
/*****************************************************************************/
#include "dcspextn.h"
#include "dcspchst.h"

/*****************************************************************************/
/* Hold-bot Internal Headers.                                                */
/*****************************************************************************/
#include "hldbstrc.h"
#include "hldbfunc.h"

#endif

