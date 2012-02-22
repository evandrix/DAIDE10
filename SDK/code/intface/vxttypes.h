/**INC+***********************************************************************/
/* Header:    vxttypes.h                                                     */
/*                                                                           */
/* Purpose:   Virtual Operating System type definitions.                     */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef VXTTYPES_INCLUDED
#define VXTTYPES_INCLUDED

#define VOS_VOID    void               /* void                               */

#define VOS_BOOL    int                /* boolean                            */

#define VOS_S8      char               /*   signed  8 bits                   */
#define VOS_U8      unsigned char      /* unsigned  8 bits                   */
#define VOS_S16     short              /*   signed 16 bits                   */
#define VOS_U16     unsigned short     /* unsigned 16 bits                   */
#define VOS_S32     int                /*   signed 32 bits                   */
#define VOS_U32     unsigned int       /* unsigned 32 bits                   */

#define VOS_CHAR    char               /* For storing characters only        */

#define VOS_PSINT   int                /*   signed platform integer          */
#define VOS_PUINT   unsigned int       /* unsigned platform integer          */

#define VOS_MAX_S8                      0x7F
#define VOS_MAX_U8                      0xFF
#define VOS_MAX_S16                     0x7FFF
#define VOS_MAX_U16                     0xFFFF
#define VOS_MAX_S32                     0x7FFFFFFF
#define VOS_MAX_U32                     0xFFFFFFFF

#define VOS_MAX_PSINT                   0x7FFFFFFF
#define VOS_MAX_PUINT                   0xFFFFFFFF

#endif

