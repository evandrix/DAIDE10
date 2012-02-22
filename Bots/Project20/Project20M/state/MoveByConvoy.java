package state;

import java.util.*;
import java.lang.Math;

public class MoveByConvoy extends Supportable{
    public List seaProvinces; // the sea provinces the convoy is through

    public MoveByConvoy(Unit unit, List seaProvinces, Province target){
        if (unit instanceof Fleet){
	    System.out.println("ERROR! Trying to convoy a fleet!");
        }
	this.unit = unit;
        this.seaProvinces = seaProvinces;
        this.target = target;
    }

    public List couldBeSupportedBy(){
	return new LinkedList(); // apparently you can't support convoys
    }

    public int getSupportsNeeded(){
     // post: returns the number of uncuttable supports we need to be sure the order will succeed
            int supportsNeeded = target.getProvince().getSurroundingUnits().size(); // gets all the enemy units which could enter target next turn
            if (target.getProvince().unit != null && target.getProvince().unit.owner != target.getProvince().map.us){
		supportsNeeded++;
            }
            return supportsNeeded;
    }

    public boolean isStationary(){
	return false;
    }

    public double evaluate(){
        // value shouldn't be affected by strength, as MoveByConvoy's can't be supported
        double value;
        if (unit instanceof Army){
            value = target.getArmyValue() - unit.location.getArmyValue();
        }
        else{
            value = target.getFleetValue() - unit.location.getFleetValue();
        }
        int conflict = getSupportsNeeded() + 1; // should be getResistance
        if (target.getProvince().unit != null){
	    conflict++;
        }
        value = value / (conflict * conflict);
        if (target.getProvince().unit != null && target.getProvince().unit.owner == target.getProvince().map.us){
            // the move is worth less if we already have someone there
	    value = value / 200;
        }
        // the move should be worth less for every seaProvince it passes through
	value = value * 0.9; // just to make MoveByConvoys worth less than Moves
        return value;
    }

    // (unit) CTO province VIA (sea_province sea_province ...)   - Move by convoy
    public String toString(){
        String theString = "( " + unit + " ) CTO " + target + " VIA ( ";

        ListIterator iterator = seaProvinces.listIterator();
        // always at least one seaProvince
        theString += iterator.next();
        while(iterator.hasNext()){
          theString += " " + (iterator.next());
        }
        theString += " )";
	return theString;
    }

}
