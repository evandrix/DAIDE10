package state;
import java.util.*;
import java.lang.Math;


public abstract class Order implements Comparable{
    public Unit unit;
    public Order helping = null; // the order this order is helping, if any
    public Place target;

    public double evaluate(){
        double value;
        if (unit.owner == unit.location.getProvince().map.us){
	  if (unit instanceof Army){
            value = 0.001 * unit.location.getArmyValue();
          }
          else{
            value = 0.001 * unit.location.getFleetValue();
          }
          value = doStrengthCompetition(value);
          return value;
        }
	else{
	    return 0;
	}
    }

    public int getSupportsNeeded(){
	return 0;
    }

    public List couldBeSupportedBy(){
	// get the list of units surrounding the target province
        // excluding this order's unit
        List aroundTarget = target.getProvince().getALLSurroundingUnits();
        for (int i = 0; i < aroundTarget.size(); i++){
	    if (aroundTarget.get(i) == this.unit){
		aroundTarget.remove(i);
            }
        }
        return aroundTarget;
    }

    private double doStrengthCompetition(double value){
	// adjusts the value given, according to the strength (people who could help)
	// and competition (people who could stop us) of the order
        if(getSupportsNeeded() > 1){
	    value = value * ( Math.pow(2, target.strength)); // strength weight = 2
        }
        return value;
    }

    public boolean isStationary() {return true;}

    // Returns negative if this order is worth less than the given order,
    // 0 if they're worth the same
    // and positive if this order is better than the specified order.
    // BUT we're actually doing this the other way round, so that when
    // we use java's built in list sorting function it sorts them from high to low
    public int compareTo(Object o){
       	return (int)(((Order)o).evaluate() - this.evaluate());
    }

    public boolean equals(Object o){
	return this.toString().equals(o.toString());
    }

}
