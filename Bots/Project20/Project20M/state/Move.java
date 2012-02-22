package state;
import java.util.*;
import java.lang.Math;

public class Move extends Supportable{
   
    public Move(Unit u, Place target){
	this.unit = u;
        this.target = target;
    }

    public int getSupportsNeeded(){
     // post: returns the number of uncuttable supports we need to be sure the order will succeed
            int supportsNeeded = target.getProvince().getSurroundingUnits().size(); // gets all the enemy units which could enter target next turn
            if (target.getProvince().unit != null && target.getProvince().unit.owner != target.getProvince().map.us){
		supportsNeeded++;
            }
            // but when its our supply centre which is under threat
            // and there isn't an enemy unit there
            // then we don't really want to move into that place
            // just make sure nobody else moves in there
	    if (target.getProvince().owner == target.getProvince().map.us  && supportsNeeded > 0 && (target.getProvince().unit == null || target.getProvince().unit.owner == target.getProvince().map.us)){
		supportsNeeded--;
            }
            return supportsNeeded;
    }

    public List couldBeSupportedBy(){
	// get the list of units surrounding the target province
        // excluding this order's unit
        if (target instanceof Coast){
	    return new LinkedList(); // you can't support moves into coasts apparently
        }
        List aroundTarget = target.getProvince().getALLSurroundingUnits();
        for (int i = 0; i < aroundTarget.size(); i++){
	    if (aroundTarget.get(i) == this.unit){
		aroundTarget.remove(i);
            }
        }
        return aroundTarget;
    }

    public int getResistance(int hops){
	// returns the supports this move needs + the supports the unit infront needs, if we are moving
        // into a place where a unit we own already is
        // hops is the max distance ahead we look
        if (hops == 0){
	    return 0;
        }
        int resistance = getSupportsNeeded();
        if (target.getProvince().unit != null && target.getProvince().unit.owner == unit.owner && target.getProvince().unit.order != null){
            Unit inWay = target.getProvince().unit;
            if (inWay.order instanceof Move){
	      resistance += ((Move)inWay.order).getResistance(hops - 1);
            }
            else{
		resistance += inWay.order.getSupportsNeeded();
            }
        }
        return resistance;
    }

    public double evaluate(){
        // for working out possible resistance, add in all the possible resistance of the place we're moving into
        if (unit.owner == unit.location.getProvince().map.us){
	if (unit.location == target){
	    // this /should/ never happen - you're moving into a place you're already at
            return super.evaluate();
	}
        double value;
        if (unit instanceof Army){
            value = target.getArmyValue() - unit.location.getArmyValue();
        }
        else{
            value = target.getFleetValue() - unit.location.getFleetValue();
        }

        int conflict = getResistance(100) + 1;

        if(getSupportsNeeded() > 1){
	    value = value * ( Math.pow(2, target.strength)); // strength weight = 2
        }

        if (target.getProvince().unit != null){
	    conflict++;
        }
        value = value / (conflict * conflict);

        if (target.getProvince().unit != null && target.getProvince().unit.owner == target.getProvince().map.us){
            // the move is worth less if we already have someone there
	    value = value / 200;
        }
        return value;
        }
        else{
	    return 0;
        }
    }

    public boolean isStationary(){
        if (target == unit.location){
	    return true;
        }
	return false;
    }

    public String toString(){
    	return "( " + unit + " ) MTO " + target;
    }

}
