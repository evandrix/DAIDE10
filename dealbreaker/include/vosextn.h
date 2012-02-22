/**INC+***********************************************************************/
/* Header:    vosextn.h                                                      */
/*                                                                           */
/* Purpose:   Virtual Operating System External Header File.                 */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef VOSEXTN_INCLUDED
#define VOSEXTN_INCLUDED

#include "vxttypes.h"

#ifdef VOS_EXPORTS
#ifdef __cplusplus
#define VOS_API extern "C" __declspec(dllexport)
#else
#define VOS_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define VOS_API extern "C" __declspec(dllimport)
#else
#define VOS_API __declspec(dllimport)
#endif
#endif

/*****************************************************************************/
/*                                                                           */
/* Constant Definitions.                                                     */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* AVL tree macros.                                                          */
/*****************************************************************************/
#define VOS_AVL_TREE_INIT(TREE, COMPARE, KEY_OFF, NODE_OFF)                   \
                                            (TREE).compare = &(COMPARE);      \
                                            (TREE).first = NULL;              \
                                            (TREE).last = NULL;               \
                                            (TREE).root = NULL;               \
                                            (TREE).key_offset = KEY_OFF;      \
                                            (TREE).node_offset = NODE_OFF;    \
                                            (TREE).node_count = 0
#define VOS_AVL_NODE_INIT(NODE)             (NODE).parent = NULL;             \
                                            (NODE).left = NULL;               \
                                            (NODE).right = NULL;              \
                                            (NODE).left_h = -1;               \
                                            (NODE).right_h = -1

#define VOS_AVL_NODE_INSERT(TREE, NODE)     vos_avl_insert(&(TREE), &(NODE))
#define VOS_AVL_NODE_REMOVE(TREE, NODE)     vos_avl_remove(&(TREE), &(NODE))
#define VOS_AVL_NODE_FIND(TREE, KEY)        vos_avl_find(&(TREE), (KEY))
#define VOS_AVL_NODE_FIRST(TREE)            (((&(TREE))->first != NULL) ?     \
  (VOS_VOID *)(((VOS_S8 *)(&(TREE))->first) - (&(TREE))->node_offset) : NULL)
#define VOS_AVL_NODE_LAST(TREE)             (((&(TREE))->last != NULL) ?      \
   (VOS_VOID *)((VOS_S8 *)((&(TREE))->last) - (&(TREE))->node_offset) : NULL)
#define VOS_AVL_NODE_NEXT(TREE, NODE)       vos_avl_next(&(TREE), &(NODE))
#define VOS_AVL_NODE_PREV(TREE, NODE)       vos_avl_prev(&(TREE), &(NODE))

#define VOS_AVL_NODE_IN_TREE(NODE)                                            \
                            (((NODE).left_h != -1) && ((NODE).right_h != -1))

#define VOS_AVL_TREE_NUM_NODES(TREE)        (TREE).node_count

#define VOS_AVL_TREE_EMPTY(TREE, CALLBACK, DATA)                              \
                                            if ((TREE).root != NULL)          \
                                            {                                 \
                                              vos_avl_node_empty(&(TREE),     \
                                                                 (TREE).root, \
                                                                 CALLBACK,    \
                                                                 DATA);       \
                                            }                                 \
                                            (TREE).first = NULL;              \
                                            (TREE).last = NULL;               \
                                            (TREE).root = NULL;               \
                                            (TREE).node_count = 0

/*****************************************************************************/
/* Byte-swapping macros.                                                     */
/*****************************************************************************/
#define VOS_NET_SET_USHORT(LOC, VAL)    (LOC) = tsck_htons((VAL))
#define VOS_NET_GET_USHORT(LOC, VAL)    (LOC) = tsck_ntohs((VAL))
#define VOS_NET_SET_ULONG(LOC, VAL)     (LOC) = tsck_htonl((VAL))
#define VOS_NET_GET_ULONG(LOC, VAL)     (LOC) = tsck_ntohl((VAL))

/*****************************************************************************/
/* Configuration macros.                                                     */
/*****************************************************************************/
#define VOS_CONFIG_STRING(TABLE, KEY, DATA)                                   \
                         vos_hash_get_entry(TABLE,                            \
                                            KEY,                              \
                                            (VOS_U8)(VOS_STRLEN(KEY) + 1),    \
                                            (VOS_VOID **)&DATA,               \
                                            NULL)

#define VOS_CONFIG_UINT(TABLE, KEY, DATA) vos_config_get_uint(TABLE,          \
                                                              KEY,            \
                                                              &DATA)

/*****************************************************************************/
/* File macros.                                                              */
/*****************************************************************************/
#define VOS_FILE                        FILE
#define VOS_FILE_OPEN(FILENAME, ACCESS) fopen(FILENAME, ACCESS)
#define VOS_FILE_WRITE_DATA(VFILE,                                            \
                            DATA,                                             \
                            SIZE)       fwrite(DATA, SIZE, 1, VFILE)
#define VOS_FILE_WRITE_S16(VFILE, VAL)  {                                     \
                                          VOS_S16 fw = VAL;                   \
                                          fwrite(&fw, 2, 1, VFILE);           \
                                        }
#define VOS_FILE_WRITE_U16(VFILE, VAL)  {                                     \
                                          VOS_U16 fw = VAL;                   \
                                          fwrite(&fw, 2, 1, VFILE);           \
                                        }
#define VOS_FILE_WRITE_S32(VFILE, VAL)  {                                     \
                                          VOS_S32 fw = VAL;                   \
                                          fwrite(&fw, 4, 1, VFILE);           \
                                        }
#define VOS_FILE_WRITE_U32(VFILE, VAL)  {                                     \
                                          VOS_U32 fw = VAL;                   \
                                          fwrite(&fw, 4, 1, VFILE);           \
                                        }
#define VOS_FILE_READ_DATA(VFILE,                                             \
                           DATA,                                              \
                           SIZE)        fread(DATA, SIZE, 1, VFILE)
#define VOS_FILE_READ_S16(VFILE, LOC)   fread(&(LOC), 2, 1, VFILE)
#define VOS_FILE_READ_U16(VFILE, LOC)   fread(&(LOC), 2, 1, VFILE)
#define VOS_FILE_READ_S32(VFILE, LOC)   fread(&(LOC), 4, 1, VFILE)
#define VOS_FILE_READ_U32(VFILE, LOC)   fread(&(LOC), 4, 1, VFILE)
#define VOS_FILE_READ_LINE(VFILE,                                             \
                           BUF,                                               \
                           SIZE)        fgets(BUF, SIZE, VFILE)
#define VOS_FILE_FLUSH(VFILE)           fflush(VFILE)
#define VOS_FILE_LENGTH(VFILE)          ftell(VFILE)
#define VOS_FILE_CLOSE(VFILE)           fclose(VFILE)
#define VOS_FILE_RENAME(OLD, NEW)       rename(OLD, NEW)
#define VOS_FILE_DELETE(FILENAME)       remove(FILENAME)

/*****************************************************************************/
/* Limited-size file macros.                                                 */
/*****************************************************************************/
#define VOS_LIM_FILE_ALLOC()            vos_lim_file_alloc()
#define VOS_LIM_FILE_OPEN(LFILE,                                              \
                          FILENAME,                                           \
                          ACCESS)       (LFILE)->file =                       \
                                                      VOS_FILE_OPEN(FILENAME, \
                                                                      ACCESS)
#define VOS_LIM_FILE_PRINT(ARGS)        vos_lim_file_print ARGS

#define VOS_LIM_FILE_PRINT_VAR(LFILE,                                         \
                               FIXED,                                         \
                               MARKER)  vfprintf((LFILE)->file, FIXED, MARKER)
#define VOS_LIM_FILE_LENGTH(LFILE)      VOS_FILE_LENGTH((LFILE)->file)
#define VOS_LIM_FILE_FLUSH(LFILE)       VOS_ASSERT((LFILE) != NULL);          \
                                        VOS_FILE_FLUSH((LFILE)->file)
#define VOS_LIM_FILE_FLIP(LFILE)        vos_lim_file_flip(LFILE)
#define VOS_LIM_FILE_CLOSE(LFILE)       VOS_ASSERT((LFILE) != NULL);          \
                                        VOS_FILE_CLOSE((LFILE)->file);        \
                                        (LFILE)->file = NULL
#define VOS_LIM_FILE_FREE(LFILE)        vos_lim_file_free(LFILE)

/*****************************************************************************/
/* Memory Management Macros.                                                 */
/*****************************************************************************/
#define VOS_MEM_ALLOC(SIZE)             malloc(SIZE)
#define VOS_MEM_SHRINK(LOC, SIZE)       LOC = realloc(LOC, SIZE) /* !! */
#define VOS_MEM_GROW(LOC, SIZE)         LOC = realloc(LOC, SIZE) /* !! */
#define VOS_MEM_COPY(DEST, SOURCE, SIZE)                                      \
       VOS_ASSERT((((VOS_S8 *)(DEST)) <= (((VOS_S8 *)(SOURCE)) - SIZE)) ||    \
                  (((VOS_S8 *)(DEST)) >= (((VOS_S8 *)(SOURCE)) + SIZE)));     \
          memcpy(DEST, SOURCE, SIZE)
#define VOS_MEM_MOVE(DEST, SOURCE, SIZE)                                      \
                                        memmove(DEST, SOURCE, SIZE)
#define VOS_MEM_COMPARE(LOC1, LOC2, SIZE)                                     \
                                        memcmp(LOC1, LOC2, SIZE)
#define VOS_MEM_FREE(LOC)               free(LOC);                            \
                                        LOC = NULL
#define VOS_MEM_SET(LOC, CHAR, LEN)     memset(LOC, CHAR, LEN)
#define VOS_MEM_ZERO(LOC, LEN)          VOS_MEM_SET(LOC, 0, LEN)

/*****************************************************************************/
/* Queue macros.                                                             */
/*****************************************************************************/
#define VOS_Q_INIT_ITEM(ITEM)           (ITEM).next = NULL
#define VOS_Q_INIT_ROOT(ROOT)           (ROOT).first = NULL;                  \
                                        (ROOT).last = NULL
#define VOS_Q_ENQUEUE(ITEM, ROOT)       VOS_ASSERT((ITEM).next == NULL);      \
                                        if ((ROOT).last == NULL)              \
                                        {                                     \
                                          VOS_ASSERT((ROOT).first == NULL);   \
                                          (ROOT).first = &(ITEM);             \
                                        }                                     \
                                        else                                  \
                                        {                                     \
                                          VOS_ASSERT((ROOT).first != NULL);   \
                                          ((ROOT).last)->next = &(ITEM);      \
                                        }                                     \
                                        (ROOT).last = &(ITEM)
#define VOS_Q_JUMP(ITEM, ROOT)          VOS_ASSERT((ITEM).next == NULL);      \
                                        if ((ROOT).last == NULL)              \
                                        {                                     \
                                          VOS_ASSERT((ROOT).first == NULL);   \
                                          (ROOT).first = &(ITEM);             \
                                          (ROOT).last = &(ITEM);              \
                                        }                                     \
                                        else                                  \
                                        {                                     \
                                          VOS_ASSERT((ROOT).first != NULL);   \
                                          (ITEM).next = (ROOT).first;         \
                                          (ROOT).first = &(ITEM);             \
                                        }
#define VOS_Q_DEQUEUE(ITEM, ROOT)       VOS_ASSERT(&(ITEM) == (ROOT).first);  \
                                        (ROOT).first = (ITEM).next;           \
                                        if ((ROOT).first == NULL)             \
                                        {                                     \
                                          (ROOT).last = NULL;                 \
                                        }                                     \
                                        (ITEM).next = NULL
#define VOS_Q_FIRST(ROOT)               (VOS_VOID *)((ROOT).first)
#define VOS_Q_NEXT(ITEM)                (VOS_VOID *)((ITEM).next)
#define VOS_Q_IS_EMPTY(ROOT)            ((ROOT).first == NULL)

/*****************************************************************************/
/* String Management Macros.                                                 */
/*****************************************************************************/
#define VOS_STRNCPY(DEST, SOURCE, LEN)  strncpy(DEST, SOURCE, LEN)
#define VOS_STRNCMP(STR1, STR2, LEN)    strncmp(STR1, STR2, LEN)
#define VOS_STRCMP(STR1, STR2)          strcmp(STR1, STR2)
#define VOS_STRLEN(SOURCE)              strlen(SOURCE)
#define VOS_STRCAT(DEST, SOURCE)        strcat(DEST, SOURCE)

/*****************************************************************************/
/* Time macros.                                                              */
/*****************************************************************************/
#define VOS_TIME_ELAPSED_INIT()         vos_time_elapsed_init()
#define VOS_TIME_ELAPSED_GET()          vos_time_elapsed_get()

/*****************************************************************************/
/* Trace macros.                                                             */
/*****************************************************************************/
#define VOS_TRACE_MAX_THREADS  3
#define VOS_TRACE_USE_MUTEX

#define VOS_TRACE_THREAD_START vos_trace_thread_start
#define VOS_TRACE_THREAD_STOP  vos_trace_thread_stop

#ifdef _DEBUG

#define VOS_TRACE_ENTRY(FUNC)                                                 \
          vos_trace_set_globals();                                            \
          vos_trace_push_function(FUNC);                                      \
          if (vos_trace_prolog(__FILE__, __LINE__))                           \
          {                                                                   \
            vos_trace_entry();                                                \
          }                                                                   \
          vos_trace_clear_globals()
#define VOS_TRACE_EXIT(FUNC)                                                  \
          vos_trace_set_globals();                                            \
          if (vos_trace_prolog(__FILE__, __LINE__))                           \
          {                                                                   \
            vos_trace_exit();                                                 \
          }                                                                   \
          vos_trace_pop_function(FUNC);                                       \
          vos_trace_clear_globals()
#define VOS_TRACE_BRANCH(DESCR)                                               \
          vos_trace_set_globals();                                            \
          if (vos_trace_prolog(__FILE__, __LINE__))                           \
          {                                                                   \
            vos_trace_branch DESCR;                                           \
          }                                                                   \
          vos_trace_clear_globals()
#define VOS_TRACE_DETAIL(DESCR)                                               \
          vos_trace_set_globals();                                            \
          if (vos_trace_prolog(__FILE__, __LINE__))                           \
          {                                                                   \
            vos_trace_branch DESCR;                                           \
          }                                                                   \
          vos_trace_clear_globals()
#define VOS_TRACE_PARAMETER(DESCR)                                            \
          vos_trace_set_globals();                                            \
          if (vos_trace_prolog(__FILE__, __LINE__))                           \
          {                                                                   \
            vos_trace_branch DESCR;                                           \
          }                                                                   \
          vos_trace_clear_globals()

/* !! Just entry/exit for now                                                */
// #undef  VOS_TRACE_BRANCH
// #undef  VOS_TRACE_DETAIL
// #undef  VOS_TRACE_PARAMETER
// #define VOS_TRACE_BRANCH(DESCR)
// #define VOS_TRACE_DETAIL(DESCR)
// #define VOS_TRACE_PARAMETER(DESCR)

#else

#define VOS_TRACE_ENTRY(FUNC)
#define VOS_TRACE_EXIT(FUNC)
#define VOS_TRACE_BRANCH(DESCR)
#define VOS_TRACE_DETAIL(DESCR)
#define VOS_TRACE_PARAMETER(DESCR)

#endif

/*****************************************************************************/
/* Miscellaneous macros.                                                     */
/*****************************************************************************/
#define VOS_ASSERT(X)                   _ASSERTE(X)
#define VOS_ASSERT_MEM(X)               VOS_ASSERT(X != NULL)
#define VOS_ASSERT_INVALID_BRANCH()     VOS_ASSERT(FALSE)
#define VOS_UNREFERENCED_PARAMETER(LOC) (LOC) = (LOC)
#define VOS_OFFSETOF(STRUCT, FIELD)                                           \
                          ((VOS_S8 *)(&((STRUCT *)0)->FIELD) - (VOS_S8 *)0)
#define VOS_MAX(A, B) max(A, B)

/*****************************************************************************/
/*                                                                           */
/* Type Definitions.                                                         */
/*                                                                           */
/*****************************************************************************/

/**CALLBACK+******************************************************************/
/*                                                                           */
/* Callback:   VOS_AVL_COMPARE                                               */
/*                                                                           */
/* Purpose:    User callback to compare two entries in an AVL tree.          */
/*                                                                           */
/* Parameters: IN     item1       - The first entry to compare.              */
/*             IN     item2       - The second entry to compare.             */
/*                                                                           */
/* Returns:    -1 if item1 < item2                                           */
/*              0 if item1 = item2                                           */
/*             +1 if item1 > item2                                           */
/*                                                                           */
/**CALLBACK-******************************************************************/
typedef VOS_S32(VOS_AVL_COMPARE)(VOS_VOID *item1, VOS_VOID *item2);

/**CALLBACK+******************************************************************/
/*                                                                           */
/* Callback:   VOS_AVL_FREE                                                  */
/*                                                                           */
/* Purpose:    User callback to free an entry in an AVL tree.                */
/*                                                                           */
/* Parameters: IN     node        - The item to free.                        */
/*             IN     data        - User-data as supplied on the call to     */
/*                                  VOS_AVL_TREE_EMPTY.                      */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**CALLBACK-******************************************************************/
typedef VOS_VOID(VOS_AVL_FREE)(VOS_VOID *node, VOS_VOID *data);

/*****************************************************************************/
/*                                                                           */
/* Structure Definitions.                                                    */
/*                                                                           */
/*****************************************************************************/

/**STRUCT+********************************************************************/
/* Structure: VOS_Q_ITEM                                                     */
/*                                                                           */
/* Description:                                                              */
/*                                                                           */
/*****************************************************************************/

typedef struct vos_q_item
{
  /***************************************************************************/
  /* The next item in the queue;                                             */
  /***************************************************************************/
  struct vos_q_item *next;

} VOS_Q_ITEM;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: VOS_Q_ROOT                                                     */
/*                                                                           */
/* Description: The root of a queue.                                         */
/*                                                                           */
/*****************************************************************************/

typedef struct vos_q_root
{
  /***************************************************************************/
  /* The first item in the queue.                                            */
  /***************************************************************************/
  VOS_Q_ITEM *first;

  /***************************************************************************/
  /* The last item in the queue.                                             */
  /***************************************************************************/
  VOS_Q_ITEM *last;

} VOS_Q_ROOT;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: VOS_AVL_NODE                                                   */
/*                                                                           */
/* Description: Node for an AVL tree.                                        */
/*                                                                           */
/*****************************************************************************/
typedef struct vos_avl_node
{
  /***************************************************************************/
  /* The parent of this node.                                                */
  /***************************************************************************/
  struct vos_avl_node *parent;

  /***************************************************************************/
  /* The left child of this node.                                            */
  /***************************************************************************/
  struct vos_avl_node *left;

  /***************************************************************************/
  /* The height of the left subtree.                                         */
  /***************************************************************************/
  VOS_S16 left_h;

  /***************************************************************************/
  /* The right child of this node.                                           */
  /***************************************************************************/
  struct vos_avl_node *right;

  /***************************************************************************/
  /* The height of the right subtree.                                        */
  /***************************************************************************/
  VOS_S16 right_h;

} VOS_AVL_NODE;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: VOS_AVL_TREE                                                   */
/*                                                                           */
/* Description: Root of an AVL tree.                                         */
/*                                                                           */
/*****************************************************************************/
typedef struct vos_avl_tree
{
  /***************************************************************************/
  /* Compare function for comparing nodes in the tree.                       */
  /***************************************************************************/
  VOS_AVL_COMPARE *compare;

  /***************************************************************************/
  /* Pointer to the root of the tree.                                        */
  /***************************************************************************/
  VOS_AVL_NODE *root;

  /***************************************************************************/
  /* Pointer to the first entry in the node thread.                          */
  /***************************************************************************/
  VOS_AVL_NODE *first;

  /***************************************************************************/
  /* Pointer to the last entry in the node thread.                           */
  /***************************************************************************/
  VOS_AVL_NODE *last;

  /***************************************************************************/
  /* Offset of the key within the node's parent structure.                   */
  /***************************************************************************/
  VOS_U16 key_offset;

  /***************************************************************************/
  /* Offset of the AVL_NODE within the node's parent structure.              */
  /***************************************************************************/
  VOS_U16 node_offset;

  /***************************************************************************/
  /* Count of nodes in the tree.                                             */
  /***************************************************************************/
  VOS_U32 node_count;

} VOS_AVL_TREE;

/**STRUCT-********************************************************************/


/*****************************************************************************/
/*                                                                           */
/* Function Prototypes.                                                      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* vosavl.c                                                                  */
/*****************************************************************************/
VOS_API VOS_VOID vos_avl_insert(VOS_AVL_TREE  *, VOS_AVL_NODE  *);
VOS_API VOS_VOID vos_avl_remove(VOS_AVL_TREE *, VOS_AVL_NODE *);
VOS_API VOS_VOID *vos_avl_find(VOS_AVL_TREE *, VOS_VOID *);
VOS_API VOS_VOID *vos_avl_next(VOS_AVL_TREE *, VOS_AVL_NODE *);
VOS_API VOS_VOID *vos_avl_prev(VOS_AVL_TREE *, VOS_AVL_NODE *);
VOS_API VOS_VOID vos_avl_node_empty(VOS_AVL_TREE *,
                                    VOS_AVL_NODE *,
                                    VOS_AVL_FREE,
                                    VOS_VOID *);

VOS_VOID vos_avl_swap_right_most(VOS_AVL_TREE *,VOS_AVL_NODE *, VOS_AVL_NODE *);
VOS_VOID vos_avl_swap_left_most(VOS_AVL_TREE *, VOS_AVL_NODE *, VOS_AVL_NODE *);
VOS_VOID vos_avl_balance_tree(VOS_AVL_TREE *, VOS_AVL_NODE *);
VOS_VOID vos_avl_rebalance(VOS_AVL_NODE **);
VOS_VOID vos_avl_rotate_right(VOS_AVL_NODE **);
VOS_VOID vos_avl_rotate_left(VOS_AVL_NODE **);

VOS_API VOS_AVL_COMPARE vos_avl_compare_s8;
VOS_API VOS_AVL_COMPARE vos_avl_compare_u8;
VOS_API VOS_AVL_COMPARE vos_avl_compare_s16;
VOS_API VOS_AVL_COMPARE vos_avl_compare_u16;
VOS_API VOS_AVL_COMPARE vos_avl_compare_s32;
VOS_API VOS_AVL_COMPARE vos_avl_compare_u32;
VOS_API VOS_AVL_COMPARE vos_avl_compare_ntstr;
VOS_API VOS_AVL_COMPARE vos_avl_compare_ntistr;

/*****************************************************************************/
/* vostrace.c                                                                */
/*****************************************************************************/
VOS_API VOS_BOOL vos_trace_initialise(VOS_CHAR *,
                                      VOS_U8,
                                      VOS_S32,
                                      VOS_CHAR *,
                                      VOS_CHAR *);
VOS_API VOS_VOID vos_trace_thread_start();
VOS_API VOS_VOID vos_trace_thread_stop();
VOS_API VOS_VOID vos_trace_set_globals();
VOS_API VOS_VOID vos_trace_clear_globals();
VOS_API VOS_VOID vos_trace_push_function(VOS_CHAR *);
VOS_API VOS_VOID vos_trace_pop_function(VOS_CHAR *);
VOS_API VOS_VOID vos_trace_entry();
VOS_API VOS_VOID vos_trace_exit();
VOS_API VOS_VOID __cdecl vos_trace_branch(VOS_CHAR *, ...);
VOS_API VOS_BOOL vos_trace_prolog(VOS_CHAR *, VOS_U32);
VOS_API VOS_VOID vos_trace_terminate();

#endif


/**INC+***********************************************************************/
/* Header:    tsckextn.h                                                     */
/*                                                                           */
/* Purpose:   Test-Sockets external interface definition.                    */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef TSCKEXTN_INCLUDED
#define TSCKEXTN_INCLUDED

#include "winsock2.h"

#ifdef TSCK_EXPORTS
#define TSCK_API __declspec(dllexport)
#else
#define TSCK_API __declspec(dllimport)
#endif

/*****************************************************************************/
/*                                                                           */
/* Type Definitions.                                                         */
/*                                                                           */
/*****************************************************************************/

typedef SOCKET(__stdcall TSCK_ACCEPT)(SOCKET, struct sockaddr *, int *);
typedef int (__stdcall TSCK_BIND)(SOCKET, const struct sockaddr *, int);
typedef int(__stdcall TSCK_CLOSESOCKET)(SOCKET);
typedef int (__stdcall TSCK_CONNECT)(SOCKET, const struct sockaddr *, int);
typedef struct hostent * (__stdcall TSCK_GETHOSTBYNAME)(const char *);
typedef int (__stdcall TSCK_GETHOSTNAME)(char *, int);
typedef u_short (__stdcall TSCK_HTONS)(u_short);
typedef unsigned long (__stdcall TSCK_INET_ADDR)(const char *);
typedef int(__stdcall TSCK_IOCTLSOCKET)(SOCKET, long, u_long *);
typedef int(__stdcall TSCK_LISTEN)(SOCKET, int );
typedef u_short(__stdcall TSCK_NTOHS)(u_short);
typedef int(__stdcall TSCK_RECV)(SOCKET, char *, int, int);
typedef int(__stdcall TSCK_SEND)(SOCKET, const char *, int, int);
typedef SOCKET(__stdcall TSCK_SOCKET)(int, int, int);
typedef DWORD(__stdcall TSCK_WAITFORMULTIPLEOBJECTS)(DWORD,
                                                     CONST HANDLE *,
                                                     BOOL,
                                                     DWORD);
typedef int(__stdcall TSCK_WSACLEANUP)(void);
typedef BOOL(__stdcall TSCK_WSACLOSEEVENT)(WSAEVENT);
typedef WSAEVENT(__stdcall TSCK_WSACREATEEVENT)(void);
typedef int(__stdcall TSCK_WSAENUMNETWORKEVENTS)(SOCKET,
                                                 WSAEVENT,
                                                 LPWSANETWORKEVENTS);
typedef int(__stdcall TSCK_WSAEVENTSELECT)(SOCKET, WSAEVENT, long);
typedef int(__stdcall TSCK_WSAGETLASTERROR)(void);
typedef int(__stdcall TSCK_WSASTARTUP)(WORD, LPWSADATA);
typedef SOCKET(__stdcall TSCK_WSASOCKET)(int,
                                         int,
                                         int,
                                         LPWSAPROTOCOL_INFO,
                                         GROUP,
                                         DWORD);

/*****************************************************************************/
/*                                                                           */
/* Structure Definitions.                                                    */
/*                                                                           */
/*****************************************************************************/

/**STRUCT+********************************************************************/
/* Structure: TSCK_FUNCS                                                     */
/*                                                                           */
/* Description: Structure containing the function pointers that the test     */
/*              sockets call library will call through.                      */
/*                                                                           */
/*****************************************************************************/

typedef struct tsck_funcs
{
  TSCK_ACCEPT *accept_func;
  TSCK_BIND *bind_func;
  TSCK_CLOSESOCKET *closesocket_func;
  TSCK_CONNECT *connect_func;
  TSCK_GETHOSTBYNAME *gethostbyname_func;
  TSCK_GETHOSTNAME *gethostname_func;
  TSCK_HTONS *htons_func;
  TSCK_INET_ADDR *inet_addr_func;
  TSCK_IOCTLSOCKET *ioctlsocket_func;
  TSCK_LISTEN *listen_func;
  TSCK_NTOHS *ntohs_func;
  TSCK_RECV *recv_func;
  TSCK_SEND *send_func;
  TSCK_SOCKET *socket_func;
  TSCK_WAITFORMULTIPLEOBJECTS *waitformultipleobjects;
  TSCK_WSACLEANUP *wsacleanup_func;
  TSCK_WSACLOSEEVENT *wsacloseevent_func;
  TSCK_WSACREATEEVENT *wsacreateevent_func;
  TSCK_WSAENUMNETWORKEVENTS *wsaenumnetworkevents_func;
  TSCK_WSAEVENTSELECT *wsaeventselect_func;
  TSCK_WSAGETLASTERROR *wsagetlasterror_func;
  TSCK_WSASTARTUP *wsastartup_func;
  TSCK_WSASOCKET *wsasocket_func;

} TSCK_FUNCS;

/**STRUCT-********************************************************************/


/*****************************************************************************/
/*                                                                           */
/* Global Variables.                                                         */
/*                                                                           */
/*****************************************************************************/
#pragma warning (push)
#pragma warning (disable : 4232)

#ifdef TSCK_EXPORTS
  #ifdef TSCK_DEFINE_VARS
    TSCK_API TSCK_FUNCS g_tsck_func_table = {accept,
                                             bind,
                                             closesocket,
                                             connect,
                                             gethostbyname,
                                             gethostname,
                                             htons,
                                             inet_addr,
                                             ioctlsocket,
                                             listen,
                                             ntohs,
                                             recv,
                                             send,
                                             socket,
                                             WaitForMultipleObjects,
                                             WSACleanup,
                                             WSACloseEvent,
                                             WSACreateEvent,
                                             WSAEnumNetworkEvents,
                                             WSAEventSelect,
                                             WSAGetLastError,
                                             WSAStartup,
                                             WSASocket};
  #else
    TSCK_FUNCS g_tsck_func_table;
  #endif
#else
  TSCK_API TSCK_FUNCS g_tsck_func_table;
#endif

#pragma warning (pop)

/*****************************************************************************/
/*                                                                           */
/* Constant Definitions.                                                     */
/*                                                                           */
/*****************************************************************************/

#define tsck_accept (g_tsck_func_table.accept_func)
#define tsck_bind (g_tsck_func_table.bind_func)
#define tsck_closesocket (g_tsck_func_table.closesocket_func)
#define tsck_connect (g_tsck_func_table.connect_func)
#define tsck_gethostbyname (g_tsck_func_table.gethostbyname_func)
#define tsck_gethostname (g_tsck_func_table.gethostname_func)
#define tsck_htons (g_tsck_func_table.htons_func)
#define tsck_inet_addr (g_tsck_func_table.inet_addr_func)
#define tsck_ioctlsocket (g_tsck_func_table.ioctlsocket_func)
#define tsck_listen (g_tsck_func_table.listen_func)
#define tsck_ntohs (g_tsck_func_table.ntohs_func)
#define tsck_recv (g_tsck_func_table.recv_func)
#define tsck_send (g_tsck_func_table.send_func)
#define tsck_socket (g_tsck_func_table.socket_func)
#define tsck_WaitForMultipleObjects (g_tsck_func_table.waitformultipleobjects)
#define tsck_WSACleanup (g_tsck_func_table.wsacleanup_func)
#define tsck_WSACloseEvent (g_tsck_func_table.wsacloseevent_func)
#define tsck_WSACreateEvent (g_tsck_func_table.wsacreateevent_func)
#define tsck_WSAEnumNetworkEvents (g_tsck_func_table.wsaenumnetworkevents_func)
#define tsck_WSAEventSelect (g_tsck_func_table.wsaeventselect_func)
#define tsck_WSAGetLastError (g_tsck_func_table.wsagetlasterror_func)
#define tsck_WSAStartup (g_tsck_func_table.wsastartup_func)
#define tsck_WSASocket (g_tsck_func_table.wsasocket_func)

#endif

