/**INC+***********************************************************************/
/* Header:    randincl.h                                                     */
/*                                                                           */
/* Purpose:   Randbot include file.                                          */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef RANDINCL_INCLUDED
#define RANDINCL_INCLUDED

/*****************************************************************************/
/* Platform headers.                                                         */
/*****************************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "malloc.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

/*****************************************************************************/
/* VirOS external headers.                                                   */
/*****************************************************************************/
#include "vosextn.h"

/*****************************************************************************/
/* Diplomacy AI Client-Server Protocol DLL external headers.                 */
/*****************************************************************************/
#include "dcspextn.h"
#include "dcspcraw.h"
#include "dcspclss.h"

/*****************************************************************************/
/* AI Utility external header.                                               */
/*****************************************************************************/
#include "aiutextn.h"

/*****************************************************************************/
/* Randbot Internal Headers.                                                 */
/*****************************************************************************/
#include "randstrc.h"
#include "randfunc.h"

#endif

