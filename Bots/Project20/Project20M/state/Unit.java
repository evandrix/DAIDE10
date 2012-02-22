package state;

import java.util.*;

public abstract class Unit{
    public Power owner;
    public Place location;
    public Order order;

    // variables needed by Negotiator
    public String ord = null;                   // current order of unit
    private String ordRollback = null;          // for reverting ord back if proposal is rejected
    public String dest = null;                  // destination the unit is going given in proposal
    private String destRollback = null;

    // reset all variables needed by negotiator
    public void resetNegotiatorVariables(){
        ord = null;
        ordRollback = null;
        dest = null;
        destRollback = null;
    }

    // backup the ord variable before being replaced by new value which is proposed
    public void setOrd(String r){
        ordRollback = ord;
        ord = r;
        destRollback = dest;
    }

    // PRP has failed, roll back the ord to previous values
    public void rollback(){
        // only rollback if there is something to rollback

        // System.out.println("    unit: rollback called");

            // DEBUG PRINT
            // System.out.println("    unit: old ord " + ord);
            // System.out.println("    unit: old ordRollback " + ordRollback);

        // rollback != NONE => unit is invovled in something else, rollback to its previous action 
        // rollback = null => unit is invovled. Unit was doing nothing before, rollback to doing nothing
        if(ordRollback==null || !ordRollback.equals("NONE")){

            ord = ordRollback;
            dest = destRollback;
            ordRollback = "NONE";
            destRollback = null;

            // DEBUG PRINT
            // System.out.println("    unit: new ord " + ord);
            // System.out.println("    unit: new ordRollback " + ordRollback);

        }
    }

    public abstract List canMoveTo(); // returns the list of places this unit can move into on the next turn
    public abstract List getValidOrders(); // hold + all the Moves + all the MoveByConvoys it can do

}
