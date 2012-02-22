/**MOD+***********************************************************************/
/* Module:    randmain.c                                                     */
/*                                                                           */
/* Purpose:   Process incoming messages for the randbot.                     */
/*                                                                           */
/* Copyright (c) 2002 - 2005 Andrew Rose.  All rights reserved.              */
/*                                                                           */
/**MOD-***********************************************************************/

#include "randincl.h"

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  rand_process_msg                                              */
/*                                                                           */
/* Purpose:    Process a message.                                            */
/*                                                                           */
/* Parameters: IN/OUT rlocal      - The local randbot data.                  */
/*             IN     msg         - The message to be processed.             */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_VOID rand_process_msg(RAND_LOCAL_DATA *rlocal, RAND_MSG *msg)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  DCSP_LSS_MSG *lss = (DCSP_LSS_MSG *)(msg + 1);
  DCSP_LSS_TLM_HDR *tlm = (DCSP_LSS_TLM_HDR *)(lss + 1);

  VOS_TRACE_ENTRY("rand_process_msg");

  /***************************************************************************/
  /* Set up the control structure.  The default is to continue processing    */
  /* and free the message.                                                   */
  /***************************************************************************/
  rlocal->keep_going = TRUE;
  rlocal->free_msg = TRUE;

  /***************************************************************************/
  /* First pass the message to the AI utilities library for procesing.       */
  /***************************************************************************/
  aiut_process_msg(&(rlocal->aiut),
                   lss,
                   &(rlocal->keep_going),
                   &(rlocal->free_msg));

  /***************************************************************************/
  /* Now do any processing of our own.                                       */
  /***************************************************************************/
  switch (lss->type)
  {
    case DCSP_LSS_TYPE_DM:
      /***********************************************************************/
      /* Diplomacy message.  We may need to act on it depending on the       */
      /* message type.                                                       */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("DCSP_LSS_TYPE_DM"));

      switch (tlm->type)
      {
        case DCSP_LSS_T_NOW:
          /*******************************************************************/
          /* Process the NOW message.                                        */
          /*******************************************************************/
          VOS_TRACE_BRANCH(("DCSP_LSS_T_NOW"));
          rand_process_now(rlocal, msg, (DCSP_LSS_NOW *)tlm);
          break;

        default:
          /*******************************************************************/
          /* We don't care about any other messages.  Don't do anything.     */
          /*******************************************************************/
          VOS_TRACE_BRANCH(("Unwanted TLM type"));
          break;
      }
      break;

    case DCSP_LSS_TYPE_LM:
    case DCSP_LSS_TYPE_IM:
    case DCSP_LSS_TYPE_RM:
    case DCSP_LSS_TYPE_FM:
    case DCSP_LSS_TYPE_EM:
      /***********************************************************************/
      /* We don't care about these messages.  Don't do anything and keep     */
      /* going.                                                              */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("Unwanted message type"));
      break;

    default:
      /***********************************************************************/
      /* Invalid message type.                                               */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("Invalid message type %d", lss->type));
      VOS_ASSERT_INVALID_BRANCH();
      break;
  }

  /***************************************************************************/
  /* Free the message if it hasn't been stored by the processing function.   */
  /***************************************************************************/
  if (rlocal->free_msg)
  {
    VOS_TRACE_BRANCH(("Free the message"));
    VOS_MEM_FREE(msg);
    rlocal->free_msg = FALSE;
  }

  VOS_TRACE_EXIT("rand_process_msg");

  return;
}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  rand_process_now                                              */
/*                                                                           */
/* Purpose:    Process a NOW message.                                        */
/*                                                                           */
/* Parameters: IN     rlocal      - The local randbot data.                  */
/*             IN     msg         - The top level message.                   */
/*             IN     now         - The NOW message.                         */
/*                                                                           */
/* Returns:    TRUE if the message was queued for sending, FALSE if there    */
/*             was a resource shortage.                                      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_BOOL rand_process_now(RAND_LOCAL_DATA *rlocal,
                          RAND_MSG *msg,
                          DCSP_LSS_NOW *now)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_U16 ii;
  VOS_U16 rn;
  AIUT_POSITION *position;
  AIUT_POWER our_power;
  VOS_U16 unordered_units;
  VOS_U16 unordered_removals;
  VOS_U32 still_available;
  AIUT_ORDER *temp_order;
  AIUT_ORDER *temp_order2;
  AIUT_PROVINCE *used_province;
  VOS_AVL_TREE *order_tree;

  VOS_TRACE_ENTRY("rand_process_now");

  VOS_UNREFERENCED_PARAMETER(msg);
  VOS_UNREFERENCED_PARAMETER(now);

  /***************************************************************************/
  /* Get our power.                                                          */
  /***************************************************************************/
  our_power = DCSP_LSS_CONV_POWER_TO_INDEX(rlocal->aiut.hlo.your_power);

  position = rlocal->aiut.current_position;

#if defined(SELFBOT) || defined(CONSBOT)
  /***************************************************************************/
  /* Selfbot + Consbot: Remove all orders that involve foreign units.        */
  /***************************************************************************/
  rand_remove_orders(rlocal, position);
#endif

  /***************************************************************************/
  /* For movement and retreat phases, step through each unit, picking an     */
  /* order from the possibles.                                               */
  /***************************************************************************/
  if (position->season != DCSP_LSS_T_WIN)
  {
    VOS_TRACE_BRANCH(("Movement or retreat phase"));

#ifdef CONSBOT
    /*************************************************************************/
    /* Consbot: Keep trying order sets until we find a consistent lot.       */
    /*************************************************************************/
    do
    {
#endif
      for (ii = 0; ii < position->num_units; ii++)
      {
        VOS_TRACE_BRANCH(("Found a unit"));

        if (position->unit[ii]->owner == our_power)
        {
          /*******************************************************************/
          /* Select an order for the unit if it needs ordering.              */
          /*******************************************************************/
          VOS_TRACE_BRANCH(("It's one of ours"));

          if (VOS_AVL_TREE_NUM_NODES(position->unit[ii]->order_tree) != 0)
          {
            VOS_TRACE_BRANCH(("Give the unit an order"));

            /*****************************************************************/
            /* Pick a random number between 0 and the number of orders.      */
            /*****************************************************************/
            rn = aiut_rand(0,
                           (VOS_U16)VOS_AVL_TREE_NUM_NODES(
                                          position->unit[ii]->order_tree) - 1);

            /*****************************************************************/
            /* Step along the tree to find the selected order.               */
            /*****************************************************************/
            for (temp_order =
                            VOS_AVL_NODE_FIRST(position->unit[ii]->order_tree);
                 rn > 0;
                 rn--, temp_order = (AIUT_ORDER *)
                              VOS_AVL_NODE_NEXT(position->unit[ii]->order_tree,
                                                temp_order->node))
            {
            }
            VOS_ASSERT(rn == 0);

            /*****************************************************************/
            /* Copy the order details.                                       */
            /*****************************************************************/
            VOS_MEM_COPY((VOS_U8 *)&position->unit[ii]->order,
                         (VOS_U8 *)temp_order,
                         sizeof(AIUT_ORDER));
          }
        }
      }
#ifdef CONSBOT
    } while (!rand_orders_consistent(position, our_power));
#endif
  }
  else
  {
    /*************************************************************************/
    /* Winter adjustments.                                                   */
    /*************************************************************************/
    VOS_TRACE_BRANCH(("Adjustments phase"));

    /*************************************************************************/
    /* Check if our power has builds or removals or neither.                 */
    /*************************************************************************/
    if (position->num_power_builds[our_power] < 0)
    {
      /***********************************************************************/
      /* Removals.                                                           */
      /*                                                                     */
      /* For each of our units in turn, produce a random number between 1    */
      /* and the number of units still requiring an order.  If the number is */
      /* greater than the number of removals still required then use the     */
      /* second (hold) order - otherwise use the first (remove) order.       */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("%d removals required",
                                   0 - position->num_power_builds[our_power]));
      unordered_units = position->num_power_units[our_power];
      unordered_removals = -position->num_power_builds[our_power];

      for (ii = 0; ii < position->num_units; ii++)
      {
        if (position->unit[ii]->owner == our_power)
        {
          /*******************************************************************/
          /* If there are no removals left to order then make sure we give   */
          /* the unit under consideration a HLD order.  Otherwise pick a     */
          /* random number to decide whether to keep it or not.              */
          /*******************************************************************/
          if (unordered_removals != 0)
          {
            VOS_TRACE_BRANCH(("Maybe remove the unit"));
            rn = aiut_rand(1, unordered_removals);
          }
          else
          {
            VOS_TRACE_BRANCH(("All removals already ordered"));
            rn = 1;
          }

          if (rn > unordered_removals)
          {
            /*****************************************************************/
            /* Hold order.                                                   */
            /*****************************************************************/
            VOS_TRACE_BRANCH(("Do not remove the unit"));
            VOS_MEM_COPY(&(position->unit[ii]->order),
                         VOS_AVL_NODE_FIRST(position->unit[ii]->order_tree),
                         sizeof(AIUT_ORDER));
            VOS_ASSERT(position->unit[ii]->order.order == DCSP_LSS_T_HLD);
          }
          else
          {
            /*****************************************************************/
            /* Remove order.                                                 */
            /*****************************************************************/
            VOS_TRACE_BRANCH(("Remove the unit"));
            VOS_ASSERT(unordered_removals != 0);
            VOS_MEM_COPY(&(position->unit[ii]->order),
                         VOS_AVL_NODE_LAST(position->unit[ii]->order_tree),
                         sizeof(AIUT_ORDER));
            VOS_ASSERT(position->unit[ii]->order.order == DCSP_LSS_T_REM);
            unordered_removals--;
          }
          unordered_units--;
        }
      }

      VOS_ASSERT(unordered_units == 0);
      VOS_ASSERT(unordered_removals == 0);
    }
    else if (position->num_power_builds[our_power] > 0)
    {
      /***********************************************************************/
      /* Builds.                                                             */
      /*                                                                     */
      /* Find the first unit to be built.  This unit has the list of all     */
      /* possible builds attached to it.  Select at random from the list     */
      /* (discarding once selected except for WVE which is always valid).    */
      /* It is also necessary to discard all builds in the same province as  */
      /* a build that has been made.  This is to prvent attempts to build    */
      /* both an army and a fleet (for example) in the same province.        */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("%d builds required",
                                       position->num_power_builds[our_power]));
      for (ii = 0; ii < position->num_units; ii++)
      {
        if ((position->unit[ii]->owner == our_power) &&
            (position->unit[ii]->spot == NULL))
        {
          VOS_TRACE_BRANCH(("Found the unit with our build list"));
          break;
        }
      }

      order_tree = &(position->unit[ii]->order_tree);
      still_available = VOS_AVL_TREE_NUM_NODES(*order_tree);

      /***********************************************************************/
      /* Continue from this 1st unit, giving orders to all the units         */
      /* matching the criteria of being ours and not being placed on the     */
      /* board yet.                                                          */
      /***********************************************************************/
      for (; ii < position->num_units; ii++)
      {
        if ((position->unit[ii]->owner == our_power) &&
            (position->unit[ii]->spot == NULL))
        {
          /*******************************************************************/
          /* Select an order.                                                */
          /*******************************************************************/
          VOS_TRACE_BRANCH(("Found a unit requiring an order"));
          rn = aiut_rand(0, (VOS_U16)(still_available - 1));

          /*******************************************************************/
          /* Step along the tree to find the selected order.                 */
          /*******************************************************************/
          for (temp_order = VOS_AVL_NODE_FIRST(*order_tree);
               rn > 0;
               rn--, temp_order = (AIUT_ORDER *)VOS_AVL_NODE_NEXT(
                                                *order_tree,
                                                temp_order->node))
          {
          }
          VOS_ASSERT(rn == 0);

          /*******************************************************************/
          /* Copy the selected order for the unit.                           */
          /*******************************************************************/
          VOS_MEM_COPY(&(position->unit[ii]->order),
                       temp_order,
                       sizeof(AIUT_ORDER));

          /*******************************************************************/
          /* Fixup the unit type to be a fleet if we didn't build in a land  */
          /* location.                                                       */
          /*******************************************************************/
          if ((temp_order->order == DCSP_LSS_T_BLD) &&
              (temp_order->destination_spot->location !=
                                                       DCSP_LSS_LOC_FLAG_LAND))
          {
            VOS_TRACE_BRANCH(("Build a fleet"));
            position->unit[ii]->type = DCSP_LSS_T_FLT;
          }

          /*******************************************************************/
          /* If the unit wasn't a waive order then find the province that    */
          /* the unit was built in.  Remove all build orders for that        */
          /* province from the list.  Do this by swapping them out into last */
          /* position on the list and then decrementing the number of        */
          /* available positions.                                            */
          /*******************************************************************/
          if (position->unit[ii]->order.order != DCSP_LSS_T_WVE)
          {
            /*****************************************************************/
            /* Since we've just found a build order, there must be at least  */
            /* 2 orders still available (the build order and the waive       */
            /* orer).                                                        */
            /*****************************************************************/
            VOS_TRACE_BRANCH(("Remove other builds for the same province"));
            VOS_ASSERT(still_available > 1);

            /*****************************************************************/
            /* Get the province used.                                        */
            /*****************************************************************/
            used_province = temp_order->destination_spot->territory->
                                                               static_province;

            /*****************************************************************/
            /* Search through the order tree looking for build orders with   */
            /* the same province.                                            */
            /*****************************************************************/
            for (temp_order = (AIUT_ORDER *)VOS_AVL_NODE_FIRST(*order_tree);
                 temp_order != NULL;
                 temp_order = temp_order2)
            {

              /***************************************************************/
              /* Get the next order now in case we free this one.            */
              /***************************************************************/
              temp_order2 = (AIUT_ORDER *)VOS_AVL_NODE_NEXT(*order_tree,
                                                            temp_order->node);

              if ((temp_order->order != DCSP_LSS_T_WVE) &&
                  (temp_order->destination_spot->territory->static_province ==
                                                                used_province))
              {
                /*************************************************************/
                /* This order can't be used any more since we already have a */
                /* build in this province.                                   */
                /*************************************************************/
                VOS_TRACE_BRANCH(("Free a build order for the same province"));
                VOS_AVL_NODE_REMOVE(*order_tree, temp_order->node);
                aiut_position_free_order(&(rlocal->aiut), temp_order);

                /*************************************************************/
                /* There is now one less order available for selecting.      */
                /*************************************************************/
                still_available--;
              }
            }
          }
        }
      }
    }
  }

  /***************************************************************************/
  /* Submit the orders.                                                      */
  /***************************************************************************/
  aiut_order_submit(&(rlocal->aiut), position, rlocal->aiut.hlo.your_power);

  VOS_TRACE_PARAMETER(("RET - Orders submitted? %d", TRUE));
  VOS_TRACE_EXIT("rand_process_now");

  return(TRUE);
}

#if defined(SELFBOT) || defined(CONSBOT)
/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  rand_remove_orders                                            */
/*                                                                           */
/* Purpose:    Remove all orders involving foreign units.                    */
/*                                                                           */
/* Parameters: IN     rlocal      - The local randbot data.                  */
/*             IN/OUT position    - The position from which the orders       */
/*                                  should be removed.                       */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_VOID rand_remove_orders(RAND_LOCAL_DATA *rlocal, AIUT_POSITION *position)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  AIUT_POWER our_power;
  AIUT_ORDER *order;
  AIUT_ORDER *next_order;
  VOS_U32 ii;

  VOS_TRACE_ENTRY("rand_remove_orders");

  /***************************************************************************/
  /* Trace the entry parameters.                                             */
  /***************************************************************************/
  VOS_TRACE_PARAMETER(("IN     rlocal   = %p", rlocal));
  VOS_TRACE_PARAMETER(("IN/OUT position = %p", position));

  /***************************************************************************/
  /* Check the entry parameters.                                             */
  /***************************************************************************/
  VOS_ASSERT(rlocal != NULL);
  VOS_ASSERT(position != NULL);

  /***************************************************************************/
  /* Get our power.                                                          */
  /***************************************************************************/
  our_power = DCSP_LSS_CONV_POWER_TO_INDEX(rlocal->aiut.hlo.your_power);

  /***************************************************************************/
  /* Step through the units looking for ours.                                */
  /***************************************************************************/
  for (ii = 0; ii < position->num_units; ii++)
  {
    VOS_TRACE_BRANCH(("Found a unit"));

    /*************************************************************************/
    /* Only remove the unwanted orders for out own units.                    */
    /*************************************************************************/
    if (position->unit[ii]->owner == our_power)
    {
      VOS_TRACE_BRANCH(("It's one of ours"));

      /***********************************************************************/
      /* Step through all the orders.                                        */
      /***********************************************************************/
      for (order = VOS_AVL_NODE_FIRST(position->unit[ii]->order_tree);
           order != NULL;
           order = next_order)
      {
        /*********************************************************************/
        /* Get the next order now because we might delete this one.          */
        /*********************************************************************/
        next_order = VOS_AVL_NODE_NEXT(position->unit[ii]->order_tree,
                                                                  order->node);

        /*********************************************************************/
        /* Check if the order involves any foreign units.  This includes...  */
        /*                                                                   */
        /* o Supporting a foreign unit                                       */
        /* o Moving by convoy via a foreign unit                             */
        /* o Convoying a foreign unit                                        */
        /*                                                                   */
        /*********************************************************************/
        if (rand_should_remove(order, our_power))
        {
          VOS_TRACE_BRANCH(("The order involves a foreign unit"));

          VOS_AVL_NODE_REMOVE(position->unit[ii]->order_tree, order->node);
        }
      }
    }
  }

  VOS_TRACE_EXIT("rand_remove_orders");

}

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  rand_should_remove                                            */
/*                                                                           */
/* Purpose:    Determine if an order should be removed.  It should be        */
/*             removed if a foreign unit is involved.                        */
/*                                                                           */
/* Parameters: IN     order       - The order in question.                   */
/*             IN     power       - Our power.                               */
/*                                                                           */
/* Returns:    TRUE if the order should be removed.  FALSE otherwise.        */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_BOOL rand_should_remove(AIUT_ORDER *order, AIUT_POWER power)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_BOOL rc;
  VOS_U32 ii;

  VOS_TRACE_ENTRY("rand_should_remove");

  /***************************************************************************/
  /* Trace the entry parameters.                                             */
  /***************************************************************************/
  VOS_TRACE_PARAMETER(("IN     order = %p", order));
  VOS_TRACE_PARAMETER(("IN     power = %d", power));

  /***************************************************************************/
  /* Check the entry parameters.                                             */
  /***************************************************************************/
  VOS_ASSERT(order != NULL);

  switch (order->order)
  {
    case DCSP_LSS_T_HLD:
    case DCSP_LSS_T_MTO:
    case DCSP_LSS_T_DSB:
    case DCSP_LSS_T_RTO:
    case DCSP_LSS_T_BLD:
    case DCSP_LSS_T_REM:
    case DCSP_LSS_T_WVE:
      /***********************************************************************/
      /* These orders never include a foreign unit.                          */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("Order never includes foreign unit"));
      rc = FALSE;
      break;

    case DCSP_LSS_T_SUP:
    case DCSP_LSS_T_CVY:
      /***********************************************************************/
      /* Check that the supported/convoyed unit is ours.                     */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("DCSP_LSS_T_SUP/_CVY"));

      if (order->other_unit->owner == power)
      {
        VOS_TRACE_BRANCH(("Supporting our own unit"));
        rc = FALSE;
      }
      else
      {
        VOS_TRACE_BRANCH(("Supporting a foreign unit"));
        rc = TRUE;
      }
      break;

    case DCSP_LSS_T_CTO:
      /***********************************************************************/
      /* Check that the convoy path only includes our units.                 */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("DCSP_LSS_T_CTO"));

      rc = FALSE;
      for (ii = 0; ii < order->convoy_path_length; ii++)
      {
        if (order->convoy_path[ii]->occupier->owner != power)
        {
          rc = TRUE;
          break;
        }
      }
      break;

    default:
      VOS_TRACE_BRANCH(("Invalid order type %d", order->order));
      VOS_ASSERT_INVALID_BRANCH();
      rc = TRUE;
      break;
  }

  VOS_TRACE_PARAMETER(("RET - Remove order? %d", remove));
  VOS_TRACE_EXIT("rand_should_remove");

  return(rc);

}
#endif

#ifdef CONSBOT
/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  rand_orders_consistent                                        */
/*                                                                           */
/* Purpose:    Determine if orders are self-consistent.  Supported units     */
/*             must be performing the supported action and convoys must be   */
/*             correct.                                                      */
/*                                                                           */
/* Parameters: IN     position    - The position to check for consistency.   */
/*             IN     our_power   - The power to check for consistency.      */
/*                                                                           */
/* Returns:    TRUE if the orders are self-consistent.  FALSE otherwise.     */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_BOOL rand_orders_consistent(AIUT_POSITION *position, AIUT_POWER our_power)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  VOS_BOOL rc;
  AIUT_ORDER *order;
  VOS_U32 ii;
  VOS_U32 jj;

  VOS_TRACE_ENTRY("rand_orders_consistent");

  /***************************************************************************/
  /* Trace the entry parameters.                                             */
  /***************************************************************************/
  VOS_TRACE_PARAMETER(("IN     position = %p", position));
  VOS_TRACE_PARAMETER(("IN     power    = %d", our_power));

  /***************************************************************************/
  /* Check the entry parameters.                                             */
  /***************************************************************************/
  VOS_ASSERT(position != NULL);

  /***************************************************************************/
  /* Loop through the units looking for ours.                                */
  /***************************************************************************/
  rc = TRUE;
  for (ii = 0; ii < position->num_units; ii++)
  {
    VOS_TRACE_BRANCH(("Found a unit"));

    if (position->unit[ii]->owner == our_power)
    {
      /***********************************************************************/
      /* Check the order consistency of our unit.  Checks are required for:- */
      /*                                                                     */
      /* - Support orders                                                    */
      /* - Convoy orders                                                     */
      /* - Move via convoy orders                                            */
      /*                                                                     */
      /***********************************************************************/
      VOS_TRACE_BRANCH(("It's one of ours"));

      order = &(position->unit[ii]->order);

      if (order->order == DCSP_LSS_T_SUP)
      {
        /*********************************************************************/
        /* Check that the supported unit is using the support.               */
        /*********************************************************************/
        VOS_TRACE_BRANCH(("DCSP_LSS_T_SUP"));

        if ((order->destination_spot == NULL) &&
            (order->other_unit->order.order != DCSP_LSS_T_HLD) &&
            (order->other_unit->order.order != DCSP_LSS_T_SUP) &&
            (order->other_unit->order.order != DCSP_LSS_T_CVY))
        {
          /*******************************************************************/
          /* Support to remain in position but unit is not remaining.        */
          /*******************************************************************/
          VOS_TRACE_BRANCH(("Support H unit is trying to move"));
          rc = FALSE;
          break;
        }
        else
        {
          if ((order->destination_spot !=
                                  order->other_unit->order.destination_spot) ||
              (order->other_unit->order.order == DCSP_LSS_T_HLD) ||
              (order->other_unit->order.order == DCSP_LSS_T_SUP) ||
              (order->other_unit->order.order == DCSP_LSS_T_CVY))
          {
            /*****************************************************************/
            /* Support to move but the unit is remaining in place.           */
            /*****************************************************************/
            VOS_TRACE_BRANCH(("Support to move but unit not moving"));
            rc = FALSE;
            break;
          }
        }
      }

      if (order->order == DCSP_LSS_T_CVY)
      {
        /*********************************************************************/
        /* Check that the unit being convoyed wishes to take it.             */
        /*********************************************************************/
        VOS_TRACE_BRANCH(("DCSP_LSS_T_CVY"));

        /*********************************************************************/
        /* Check that the unit is moving via convoy to the same destination. */
        /*********************************************************************/
        if ((order->other_unit->order.order != DCSP_LSS_T_CTO) ||
            (order->other_unit->order.destination_spot !=
                                                      order->destination_spot))
        {
          VOS_TRACE_BRANCH(("Unit not moving by convoy to right destination"));
          rc = FALSE;
          break;
        }

        /*********************************************************************/
        /* Check that the unit is using us in the convoy.                    */
        /*********************************************************************/
        rc = FALSE;
        for (jj = 0; jj < order->other_unit->order.convoy_path_length; jj++)
        {
          if (order->other_unit->order.convoy_path[jj]->occupier ==
                                                            position->unit[ii])
          {
            VOS_TRACE_BRANCH(("Unit is using our convoy"));
            rc = TRUE;
            break;
          }
        }
        if (!rc)
        {
          VOS_TRACE_BRANCH(("Unit moving by convoy but not ours"));
          break;
        }
      }

      if (order->order == DCSP_LSS_T_CTO)
      {
        /*********************************************************************/
        /* Check that the convoy route is complete for a unit moving via     */
        /* convoy.                                                           */
        /*********************************************************************/
        VOS_TRACE_BRANCH(("DCSP_LSS_T_CTO"));

        for (jj = 0; jj < order->convoy_path_length; jj++)
        {
          if ((order->convoy_path[jj]->occupier->order.order !=
                                                             DCSP_LSS_T_CVY) ||
              (order->convoy_path[jj]->occupier->order.other_unit !=
                                                         position->unit[ii]) ||
              (order->convoy_path[jj]->occupier->order.destination_spot !=
                                                      order->destination_spot))
          {
            VOS_TRACE_BRANCH(("The convoy route is complete"));
            rc = FALSE;
            break;
          }
        }
      }
    }
  }

  VOS_TRACE_EXIT("rand_orders_consistent");

  return(rc);
}
#endif

/**PROC+**********************************************************************/
/*                                                                           */
/* Procedure:  rand_msg_free_callback                                        */
/*                                                                           */
/* Purpose:    Callback to free a message previously held by AIUT.           */
/*                                                                           */
/* Parameters: IN     msg         - The message that was held.               */
/*                                                                           */
/* Returns:    Nothing.                                                      */
/*                                                                           */
/**PROC-**********************************************************************/

VOS_VOID rand_msg_free_callback(DCSP_LSS_MSG *msg)
{
  /***************************************************************************/
  /* Local Variables.                                                        */
  /***************************************************************************/
  RAND_MSG *rmsg;

  VOS_TRACE_ENTRY("rand_msg_free_callback");

  /***************************************************************************/
  /* Trace the entry parameters.                                             */
  /***************************************************************************/
  VOS_TRACE_PARAMETER(("IN     msg = %p", msg));

  /***************************************************************************/
  /* Check the entry parameters.                                             */
  /***************************************************************************/
  VOS_ASSERT(msg != NULL);

  /***************************************************************************/
  /* Step back from the LSS message to find the RAND_MSG that it is embedded */
  /* in.  Free the RAND_MSG.                                                 */
  /***************************************************************************/
  rmsg = (RAND_MSG *)(((VOS_U8 *)msg) - sizeof(RAND_MSG));
  VOS_MEM_FREE(rmsg);

  VOS_TRACE_EXIT("rand_msg_free_callback");

  return;
}

