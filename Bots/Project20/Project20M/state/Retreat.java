package state;
import java.util.*;
import java.lang.Math;

public class Retreat extends Order{
   
    public Retreat(Unit u, Place target){
	this.unit = u;
        this.target = target;
    }

    public double evaluate(){
        double value;
        if (unit instanceof Army){
            value = target.getArmyValue() - unit.location.getArmyValue();
        }
        else{
            value = target.getFleetValue() - unit.location.getFleetValue();
        }
        return value;
    }

    public boolean isStationary(){
	return false;
    }

    public String toString(){
    	return "( " + unit + " ) RTO " + target;
    }

}
