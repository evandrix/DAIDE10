/**INC+***********************************************************************/
/* Header:    aiutextn.h                                                     */
/*                                                                           */
/* Purpose:   AI Utilities External Interface Definition.                    */
/*                                                                           */
/* Copyright (c) 2003 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef AIUTEXTN_INCLUDED
#define AIUTEXTN_INCLUDED

#ifdef AIUT_EXPORTS
#define AIUT_API __declspec(dllexport)
#else
#define AIUT_API __declspec(dllimport)
#endif

/*****************************************************************************/
/*                                                                           */
/* Constant Definitions.                                                     */
/*                                                                           */
/*****************************************************************************/

#define AIUT_TOKEN DCSP_LSS_TOKEN
#define AIUT_POWER VOS_U16

/*****************************************************************************/
/* GROUP AIUT_SC_TYPES                                                       */
/*****************************************************************************/
#define AIUT_SC_TYPES
#define AIUT_SC_TYPE_NONE               ((VOS_U8)0)
#define AIUT_SC_TYPE_SC                 ((VOS_U8)1)
#define AIUT_SC_TYPE_HSC                ((VOS_U8)2)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP AIUT_PROVINCE_TYPES                                                 */
/*                                                                           */
/* The province types are land, sea and coast.  Land provinces only support  */
/* armies, sea provinces only support fleets and coastal provinces support   */
/* both.  The values used in this group are set so that the UNIT_SUP_FLAGS   */
/* below represent this.                                                     */
/*****************************************************************************/
#define AIUT_PROVINCE_TYPES
#define AIUT_PROVINCE_TYPE_LAND         ((VOS_U8)1)
#define AIUT_PROVINCE_TYPE_SEA          ((VOS_U8)2)
#define AIUT_PROVINCE_TYPE_COAST        ((VOS_U8)3)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* Maximum values.  These can be increased freely if required.               */
/*****************************************************************************/
#define AIUT_MAX_PROVINCES              300
#define AIUT_MAX_UNITS                  100
#define AIUT_MAX_LOCATIONS              (AIUT_MAX_PROVINCES * 2)
#define AIUT_MAX_ADJ_PER_PROV           20
#define AIUT_MAX_POWERS                 20

/*****************************************************************************/
/* GROUP AIUT_UNIT_SUP_FLAGS                                                 */
/*                                                                           */
/* The group contains flags for determining if a province type supports a    */
/* particular unit type.  See the comment on AIUT_PROVINCE_TYPES for further */
/* details.                                                                  */
/*****************************************************************************/
#define AIUT_UNIT_SUP_FLAG_ARMY         ((VOS_U8)1)
#define AIUT_UNIT_SUP_FLAG_FLEET        ((VOS_U8)2)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Type Definitions.                                                         */
/*                                                                           */
/*****************************************************************************/

/**CALLBACK+******************************************************************/
/*                                                                           */
/* Callback:   AIUT_MSG_FREE_CALLBACK                                        */
/*                                                                           */
/* Purpose:    User callback for freeing messages previously retained by the */
/*             AI Utilities Library.                                         */
/*                                                                           */
/* Parameters: IN     msg         - The message retained.                    */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**CALLBACK-******************************************************************/
typedef VOS_VOID(AIUT_MSG_FREE_CALLBACK)(DCSP_LSS_MSG *);

/*****************************************************************************/
/*                                                                           */
/* Structure Definitions.                                                    */
/*                                                                           */
/*****************************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_MEASURE_MAP                                               */
/*                                                                           */
/* Description: Measures about a map.  These are computed once when the MDF  */
/*              is passed to the AIUT library.                               */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_measure_map
{
  /***************************************************************************/
  /* The 'amphibious distance' between provinces.                            */
  /***************************************************************************/
  VOS_U8 distance[AIUT_MAX_PROVINCES][AIUT_MAX_PROVINCES];

  /***************************************************************************/
  /* The tempi gained for a particular power having a unit in a particular   */
  /* province.                                                               */
  /***************************************************************************/
  VOS_U8 tempi[AIUT_MAX_POWERS][AIUT_MAX_PROVINCES];

} AIUT_MEASURE_MAP;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/* The AIUT structures come as static (per-map) data and dynamic             */
/* (per-position data).                                                      */
/*                                                                           */
/* Static Data                                                               */
/* -----------                                                               */
/*                                                                           */
/* Map - This structure represents the map that the game is played on.  It   */
/*       contains pointers to every province and location within the map.    */
/*                                                                           */
/* Province - This structure contains static information about a province    */
/*            (type, valid locations, SC, who's HSC, etc.).  It does not     */
/*            contain adjacency information.                                 */
/*                                                                           */
/* Location - Each province is split into a number of locations (e.g. land,  */
/*            north coast, port, sea).  A province with exactly one coast    */
/*            would have land and port locations associated with it.  The    */
/*            location objects contian adjacency information.                */
/*                                                                           */
/* Static structures only point to other static structures.  They never      */
/* point to dynamic structures.  All the static structures for the entire    */
/* game are created when the map description (MDF) is received from the      */
/* server and passed to aiut_map_new.                                        */
/*                                                                           */
/* Dynamic Data                                                              */
/* ------------                                                              */
/*                                                                           */
/* Position - Dynamic counterpart of 'Map'.  Contains overall information    */
/*            about a position including game year and season and a pointer  */
/*            to each unit.                                                  */
/*                                                                           */
/* Territory - Dynamic counterpart of 'Province'.  Contains occupation       */
/*             details (e.g. occupying unit, dislodged unit, last owner).    */
/*                                                                           */
/* Spot - Dynamic counterpart of 'Location'.  This is just a simplified      */
/*        version of location without the adjacency information.             */
/*                                                                           */
/* Unit - Information about a unit on the map.  Contains information such as */
/*        owner, location, dislodged or not, type, current order.            */
/*                                                                           */
/* Order - Information about the order given to a particular unit.  There is */
/*         one order for each unit in a position.                            */
/*                                                                           */
/* Each dynamic structure may contain a pointer to its static counterpart    */
/* (if any).  Other than that, all dynamic structures point to other dynamic */
/* structures.  The dynamic Position and Territory structures are created    */
/* each turn when a NOW message is received and passed to                    */
/* aiut_position_new.                                                        */
/*****************************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_ADJ_PROV                                                  */
/*                                                                           */
/* Description: Information about an adjacent province.                      */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_adj_prov
{
  /***************************************************************************/
  /* The adjacent province.                                                  */
  /***************************************************************************/
  struct aiut_province *adj_province;

  /***************************************************************************/
  /* The types of unit for which the provinces are adjacent.                 */
  /***************************************************************************/
  VOS_U8 unit_flags;

} AIUT_ADJ_PROV;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_PROVINCE                                                  */
/*                                                                           */
/* Description: Unchanging information about a province.                     */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_province
{
  /***************************************************************************/
  /* The token used to represent this province.                              */
  /***************************************************************************/
  AIUT_TOKEN token;

  /***************************************************************************/
  /* Province type.  Either land, sea or coast.                              */
  /***************************************************************************/
  VOS_U8 province_type;                      /* One of: AIUT_PROVINCE_TYPES; */

  /***************************************************************************/
  /* Supply Centre type.  Either HSC, SC or NONE.                            */
  /***************************************************************************/
  VOS_U8 sc_type;                                  /* One of: AIUT_SC_TYPES; */

  /***************************************************************************/
  /* The power that has this province as a home supply centre.  This field   */
  /* is only valid if sc_type is HSC.                                        */
  /***************************************************************************/
  AIUT_POWER hsc_owner;

  /***************************************************************************/
  /* Adjacency information for the province.                                 */
  /***************************************************************************/
  VOS_U16 num_adjacent_provinces;
  AIUT_ADJ_PROV adjacent_province[AIUT_MAX_ADJ_PER_PROV];

} AIUT_PROVINCE;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_LOCATION                                                  */
/*                                                                           */
/* Description: The location of a unit (province & coast).                   */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_location
{
  /***************************************************************************/
  /* The location index.                                                     */
  /***************************************************************************/
  VOS_U16 index;

  /***************************************************************************/
  /* The province.                                                           */
  /***************************************************************************/
  AIUT_PROVINCE *province;

  /***************************************************************************/
  /* The location within that province.                                      */
  /***************************************************************************/
  AIUT_TOKEN location;

  /***************************************************************************/
  /* Adjacency information for the location.                                 */
  /***************************************************************************/
  VOS_U16 num_adjacent_locations;
  struct aiut_location *adjacent_location[AIUT_MAX_ADJ_PER_PROV];

} AIUT_LOCATION;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_MAP                                                       */
/*                                                                           */
/* Description: Static information about the board.                          */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_map
{
  /***************************************************************************/
  /* The provinces that make up the board.                                   */
  /***************************************************************************/
  VOS_U16 num_provinces;
  AIUT_PROVINCE province[AIUT_MAX_PROVINCES];

  /***************************************************************************/
  /* The locations within the provinces.                                     */
  /***************************************************************************/
  VOS_U16 num_locations;
  AIUT_LOCATION location[AIUT_MAX_LOCATIONS];

  /***************************************************************************/
  /* The number of powers that are playing on the board (including any       */
  /* eliminted powers).                                                      */
  /***************************************************************************/
  VOS_U16 num_powers;

  /***************************************************************************/
  /* The number of supply centres of the board.                              */
  /***************************************************************************/
  VOS_U16 num_scs;

  /***************************************************************************/
  /* Per-map measures.                                                       */
  /***************************************************************************/
  AIUT_MEASURE_MAP measures;

} AIUT_MAP;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_TERRITORY                                                 */
/*                                                                           */
/* Description: Information about a province that varies as the game         */
/*            progresses.                                                    */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_territory
{
  /***************************************************************************/
  /* Static province information about this territory.                       */
  /***************************************************************************/
  AIUT_PROVINCE *static_province;

  /***************************************************************************/
  /* The unit that is currently occupying the territory or NULL.             */
  /***************************************************************************/
  struct aiut_unit *occupier;

  /***************************************************************************/
  /* The dislodged unit that is currently occupying the territory.           */
  /***************************************************************************/
  struct aiut_unit *dislodged;

  /***************************************************************************/
  /* The units that can reach this territory in the current phase, both      */
  /* directly and via convoy.  The direct reachers come first.  The units    */
  /* that can only reach via convoy come next.                               */
  /***************************************************************************/
  struct aiut_unit *reacher[AIUT_MAX_UNITS];
  VOS_U16 num_reachers;
  VOS_U16 num_direct_reachers;

  /***************************************************************************/
  /* For SCs, the last power to occupy the centre during an adjustment       */
  /* phase.                                                                  */
  /***************************************************************************/
  AIUT_POWER last_occupying_power;

} AIUT_TERRITORY;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_SPOT                                                      */
/*                                                                           */
/* Description: The location of a unit (territory and location).             */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_spot
{
  /***************************************************************************/
  /* Static data about this spot.                                            */
  /***************************************************************************/
  AIUT_LOCATION *static_location;

  /***************************************************************************/
  /* The territory containing this spot.                                     */
  /***************************************************************************/
  AIUT_TERRITORY *territory;

  /***************************************************************************/
  /* The location within that territory.                                     */
  /***************************************************************************/
  AIUT_TOKEN location;

} AIUT_SPOT;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_ORDER                                                     */
/*                                                                           */
/* Description: The order given to a unit.                                   */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_order
{
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
  /* o WVE - Waive a build.                                                  */
  /***************************************************************************/
  AIUT_TOKEN order;

  /***************************************************************************/
  /* Destination.  Mandatory for MTO, CTO, RTO, & CVY.  Optional for SUP.    */
  /* If not used, this field will be NULL.                                   */
  /*                                                                         */
  /* For all orders except MTO, the location field of destination will be    */
  /* set to DCSP_LSS_LOC_FLAG_UNKNOWN.  For MTO orders where coast is        */
  /* irrelevant, location will also be DCSP_LSS_LOC_FLAG_UNKNOWN.            */
  /***************************************************************************/
  AIUT_SPOT *destination_spot;

  /***************************************************************************/
  /* Additional unit.  Valid for SUP and CVY.                                */
  /***************************************************************************/
  struct aiut_unit *other_unit;

  /***************************************************************************/
  /* The convoy path for moves by convoy.                                    */
  /***************************************************************************/
  VOS_U16 convoy_path_length;
  AIUT_TERRITORY *convoy_path[DCSP_LSS_MAX_CONVOY_ROUTE_LEN];

  /***************************************************************************/
  /* The node for inserting this order into a tree of orders.                */
  /***************************************************************************/
  VOS_AVL_NODE node;

} AIUT_ORDER;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_UNIT                                                      */
/*                                                                           */
/* Description: Information about a unit.                                    */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_unit
{
  /***************************************************************************/
  /* The unit type.  Either army or fleet.                                   */
  /***************************************************************************/
  AIUT_TOKEN type;

  /***************************************************************************/
  /* The power that owns this unit.                                          */
  /***************************************************************************/
  AIUT_POWER owner;

  /***************************************************************************/
  /* The spot that the unit is in.  If the unit is waiting to be built, this */
  /* field will be NULL.                                                     */
  /***************************************************************************/
  AIUT_SPOT *spot;

  /***************************************************************************/
  /* Whether the unit is dislodged or not.                                   */
  /***************************************************************************/
  VOS_BOOL dislodged;

  /***************************************************************************/
  /* The tree of all possible orders for this unit.                          */
  /***************************************************************************/
  VOS_AVL_TREE order_tree;

  /***************************************************************************/
  /* The order given to this unit.                                           */
  /***************************************************************************/
  AIUT_ORDER order;

  /***************************************************************************/
  /* Reserved field for algorithms that need to store per-unit information.  */
  /***************************************************************************/
  VOS_VOID *reserved;

} AIUT_UNIT;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_POWER_SCO                                                 */
/*                                                                           */
/* Description: Supply Centre Ownership information for a single power.      */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_power_sco
{
  /***************************************************************************/
  /* The number of centres owned.                                            */
  /***************************************************************************/
  VOS_U16 num_centres;

  /***************************************************************************/
  /* The province index of each owned centre.                                */
  /***************************************************************************/
  VOS_U16 province_index[AIUT_MAX_PROVINCES];

} AIUT_POWER_SCO;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_SCO                                                       */
/*                                                                           */
/* Description: Supply Centre Ownership information for all powers.          */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_sco
{
  /***************************************************************************/
  /* Supply Centre Ownership information for each power.  The number of      */
  /* powers can be obtained from the corresponding map.                      */
  /***************************************************************************/
  AIUT_POWER_SCO *power[AIUT_MAX_POWERS];

  /***************************************************************************/
  /* Reference count.                                                        */
  /***************************************************************************/
  VOS_U16 refs;

} AIUT_SCO;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_MEASURE_POSITION                                          */
/*                                                                           */
/* Description: Per-position measures.                                       */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_measure_position
{
  /***************************************************************************/
  /* Progress index (0 - 100).                                               */
  /***************************************************************************/
  VOS_U8 progress;

} AIUT_MEASURE_POSITION;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_MEASURE_POWER                                             */
/*                                                                           */
/* Description: Per-power measures.                                          */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_measure_power
{
  /***************************************************************************/
  /* Progress index (0 - 100).                                               */
  /***************************************************************************/
  VOS_U8 progress;

  /***************************************************************************/
  /* Tempi (0+)                                                              */
  /***************************************************************************/
  VOS_U16 tempi;

} AIUT_MEASURE_POWER;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_MEASURES                                                  */
/*                                                                           */
/* Description: Per-position and per-power measures.                         */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_measures
{
  /***************************************************************************/
  /* The per-position measures.                                              */
  /***************************************************************************/
  AIUT_MEASURE_POSITION position;

  /***************************************************************************/
  /* The per-power measures.                                                 */
  /***************************************************************************/
  AIUT_MEASURE_POWER power[AIUT_MAX_POWERS];

} AIUT_MEASURES;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_POSITION                                                  */
/*                                                                           */
/* Description: A position on the board.                                     */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_position
{
  /***************************************************************************/
  /* The map that is being played on.                                        */
  /***************************************************************************/
  AIUT_MAP *static_map;

  /***************************************************************************/
  /* The game year.                                                          */
  /***************************************************************************/
  VOS_U16 year;

  /***************************************************************************/
  /* The game season.                                                        */
  /***************************************************************************/
  AIUT_TOKEN season;

  /***************************************************************************/
  /* The territory information.  See the num_provinces field of 'map' for    */
  /* the number of territories that are valid.                               */
  /***************************************************************************/
  AIUT_TERRITORY territory[AIUT_MAX_PROVINCES];

  /***************************************************************************/
  /* The spot information.  See the num_locations field of 'map' for the     */
  /* number of spots that are valid.                                         */
  /***************************************************************************/
  AIUT_SPOT spot[AIUT_MAX_LOCATIONS];

  /***************************************************************************/
  /* The units on the board at the moment including units that are waiting   */
  /* to be built/waived.                                                     */
  /***************************************************************************/
  VOS_U16 num_units;
  AIUT_UNIT *unit[AIUT_MAX_UNITS];

  /***************************************************************************/
  /* The number of builds for each power (negative for removals) and the     */
  /* number of units currently on the board.  Only valid in the winter       */
  /* builds season.                                                          */
  /***************************************************************************/
  VOS_S16 num_power_builds[AIUT_MAX_POWERS];
  VOS_U16 num_power_units[AIUT_MAX_POWERS];

  /***************************************************************************/
  /* The SCO message that applies to this position.                          */
  /***************************************************************************/
  AIUT_SCO *sco;

  /***************************************************************************/
  /* The measures.                                                           */
  /***************************************************************************/
  AIUT_MEASURES measures;

} AIUT_POSITION;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_DATA                                                      */
/*                                                                           */
/* Description: Private AI Utilities Library Data.                           */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_data
{
  /***************************************************************************/
  /* A copy of the HLO message received at start of day.                     */
  /***************************************************************************/
  DCSP_LSS_HLO hlo;

  /***************************************************************************/
  /* The MAP message.  This is stored from when the MAP message is received  */
  /* until the MDF message has been processed.  At that point, it is freed.  */
  /***************************************************************************/
  DCSP_LSS_MSG *map_msg;

  /***************************************************************************/
  /* The map that the game is being played on.                               */
  /***************************************************************************/
  AIUT_MAP *map;

  /***************************************************************************/
  /* The current position.                                                   */
  /***************************************************************************/
  AIUT_POSITION *current_position;

  /***************************************************************************/
  /* The most recently processed supply centre ownership (SCO) message.      */
  /***************************************************************************/
  AIUT_SCO *last_sco;

  /***************************************************************************/
  /* Callback for freeing messages that were retained by the library.        */
  /***************************************************************************/
  AIUT_MSG_FREE_CALLBACK *msg_free_callback;

  /***************************************************************************/
  /* Recovery flag.  This is set if the game state is being recovered        */
  /* following a disconnection.                                              */
  /***************************************************************************/
  VOS_BOOL recovering;

  /***************************************************************************/
  /* Bot name and version.                                                   */
  /***************************************************************************/
  VOS_CHAR *bot_name;
  VOS_CHAR *bot_version;

} AIUT_DATA;

/**STRUCT-********************************************************************/

/**CALLBACK+******************************************************************/
/*                                                                           */
/* Callback:   AIUT_POS_EVAL_CALLBACK                                        */
/*                                                                           */
/* Purpose:    User callback for evaluating a position.                      */
/*                                                                           */
/* Parameters: IN     position    - The position to be evaluated.            */
/*             IN     data        - User supplied callback data.             */
/*                                                                           */
/* Returns:    The score for the position.                                   */
/*                                                                           */
/**CALLBACK-******************************************************************/
typedef VOS_U32(AIUT_POS_EVAL_CALLBACK)(AIUT_POSITION *, VOS_VOID *);

/**STRUCT+********************************************************************/
/* Structure: AIUT_AGENDA                                                    */
/*                                                                           */
/* Description: Agenda for position evaluation.                              */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_agenda
{
  /***************************************************************************/
  /* The trees of items.                                                     */
  /***************************************************************************/
  VOS_AVL_TREE score_tree;
  VOS_AVL_TREE order_tree;

  /***************************************************************************/
  /* The position evaluation function associated with this agenda.           */
  /***************************************************************************/
  AIUT_POS_EVAL_CALLBACK *evaluate;

  /***************************************************************************/
  /* User data for the position evaluation callback.                         */
  /***************************************************************************/
  VOS_VOID *eval_data;

} AIUT_AGENDA;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: AIUT_AGENDA_ITEM                                               */
/*                                                                           */
/* Description: An item on the agenda for consideration.                     */
/*                                                                           */
/*****************************************************************************/

typedef struct aiut_agenda_item
{
  /***************************************************************************/
  /* The nodes used to insert this item into an agenda.                      */
  /***************************************************************************/
  VOS_AVL_NODE score_node;
  VOS_AVL_NODE order_node;

  /***************************************************************************/
  /* The position to which the agenda item refers.                           */
  /***************************************************************************/
  AIUT_POSITION *position;

  /***************************************************************************/
  /* The orders to consider.                                                 */
  /***************************************************************************/
  AIUT_ORDER *order[AIUT_MAX_UNITS];

  /***************************************************************************/
  /* The score for this set of orders.                                       */
  /***************************************************************************/
  VOS_U32 score;

  /***************************************************************************/
  /* Whether we should now start considering enemy orders.  This is true if  */
  /* our own units have an order in this item.                               */
  /***************************************************************************/
  VOS_BOOL consider_enemy;

} AIUT_AGENDA_ITEM;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Function Prototypes. !! Some of these aren't external so don't belong     */
/*                      !! here.  Those that do need full descriptions.      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* aiudadct.c                                                                */
/*****************************************************************************/
AIUT_API AIUT_POSITION *aiut_adjudicate(AIUT_POSITION *);

/*****************************************************************************/
/* aiutagnd.c                                                                */
/*****************************************************************************/
AIUT_API AIUT_AGENDA *aiut_agenda_new(AIUT_POSITION *,
                                      AIUT_POS_EVAL_CALLBACK *,
                                      VOS_VOID *);
AIUT_API VOS_VOID aiut_agenda_add_item(AIUT_AGENDA *, AIUT_AGENDA_ITEM *);
AIUT_API VOS_VOID aiut_agenda_add_successors(AIUT_DATA *,
                                             AIUT_AGENDA *,
                                             AIUT_AGENDA_ITEM *);
AIUT_API VOS_BOOL aiut_agenda_step(AIUT_DATA *, AIUT_AGENDA *);
AIUT_API VOS_VOID aiut_agenda_free(AIUT_AGENDA *);
AIUT_API AIUT_AGENDA_ITEM *aiut_agenda_item_new(AIUT_DATA *,
                                                AIUT_AGENDA_ITEM *,
                                                VOS_U16,
                                                AIUT_ORDER *);
AIUT_API VOS_VOID aiut_agenda_item_free(VOS_VOID *, VOS_VOID *);
VOS_AVL_COMPARE aiut_agenda_item_compare_score;
VOS_AVL_COMPARE aiut_agenda_item_compare_orders;

/*****************************************************************************/
/* aiutmain.c                                                                */
/*****************************************************************************/
AIUT_API VOS_BOOL aiut_initialise(AIUT_DATA *, AIUT_MSG_FREE_CALLBACK *);
AIUT_API VOS_VOID aiut_terminate(AIUT_DATA *);
AIUT_API VOS_BOOL aiut_game_join(AIUT_DATA *, VOS_CHAR *, VOS_CHAR *);

/*****************************************************************************/
/* aiutimsg.c                                                                */
/*****************************************************************************/
AIUT_API VOS_VOID aiut_process_msg(AIUT_DATA *,
                                   DCSP_LSS_MSG *,
                                   VOS_BOOL *,
                                   VOS_BOOL *);
VOS_BOOL aiut_process_hlo(AIUT_DATA *, DCSP_LSS_MSG *, DCSP_LSS_HLO *);
VOS_BOOL aiut_process_iam(AIUT_DATA *, DCSP_LSS_MSG *, DCSP_LSS_IAM *);
VOS_BOOL aiut_process_map(AIUT_DATA *, DCSP_LSS_MSG *, DCSP_LSS_MAP *);
VOS_BOOL aiut_process_mdf(AIUT_DATA *, DCSP_LSS_MSG *, DCSP_LSS_MDF *);
VOS_BOOL aiut_process_now(AIUT_DATA *, DCSP_LSS_MSG *, DCSP_LSS_NOW *);
VOS_BOOL aiut_process_sco(AIUT_DATA *, DCSP_LSS_MSG *, DCSP_LSS_SCO *);
VOS_BOOL aiut_process_lm(DCSP_LSS_LM *);

/*****************************************************************************/
/* aiutmap.c                                                                 */
/*****************************************************************************/
AIUT_MAP *aiut_map_new(DCSP_LSS_MDF *);
VOS_VOID aiut_map_free(AIUT_MAP *);

/*****************************************************************************/
/* aiutmsg1.c                                                                */
/*****************************************************************************/
DCSP_LSS_TLM_HDR *aiut_init_dm(DCSP_LSS_MSG *, TOKEN, VOS_U16);
VOS_BOOL aiut_accept_message(DCSP_LSS_MSG *);
VOS_BOOL aiut_reject_message(DCSP_LSS_MSG  *);
AIUT_API VOS_BOOL aiut_build_send_gof();
VOS_BOOL aiut_build_send_hlo();
VOS_BOOL aiut_build_send_iam(TOKEN, VOS_S16);
VOS_BOOL aiut_build_send_map();
VOS_BOOL aiut_build_send_mdf();
VOS_BOOL aiut_build_send_nme(VOS_CHAR *, VOS_CHAR *);
VOS_BOOL aiut_build_send_now();
VOS_BOOL aiut_build_send_sco();

/*****************************************************************************/
/* aiutordr.c                                                                */
/*****************************************************************************/
AIUT_API VOS_VOID aiut_order_submit(AIUT_DATA *, AIUT_POSITION *, AIUT_TOKEN);

/*****************************************************************************/
/* aiutpos.c                                                                 */
/*****************************************************************************/
AIUT_POSITION *aiut_position_new(AIUT_MAP *,
                                 AIUT_SCO *,
                                 DCSP_LSS_NOW *,
                                 VOS_BOOL);
AIUT_API VOS_VOID aiut_position_free(AIUT_POSITION *);
AIUT_API VOS_VOID aiut_position_free_order(AIUT_DATA *, AIUT_ORDER *);

/*****************************************************************************/
/* aiutrand.c                                                                */
/*****************************************************************************/
AIUT_API VOS_U16 aiut_rand(VOS_U16, VOS_U16);

/*****************************************************************************/
/* aiutsco.c                                                                 */
/*****************************************************************************/
AIUT_SCO *aiut_sco_new(AIUT_MAP *, DCSP_LSS_SCO *);
VOS_VOID aiut_sco_reference(AIUT_SCO *);
VOS_VOID aiut_sco_unreference(AIUT_MAP *, AIUT_SCO *);

#endif

