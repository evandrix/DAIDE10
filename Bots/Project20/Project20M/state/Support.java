package state;
import java.util.*;

public class Support extends Order{
    public Order orderToSupport;

    public Support(Unit unit, Order orderToSupport){
	this.unit = unit;
	this.orderToSupport = orderToSupport;
        this.target = unit.location;
    }

   public int getSupportsNeeded(){
	    Province target = unit.location.getProvince();
            int supportsNeeded = target.getSurroundingUnits().size();
            if (supportsNeeded > 0){
		supportsNeeded--;
            }
            return supportsNeeded;
    }

    public String toString(){
	if (orderToSupport.isStationary()){
	       return ("( " + unit + " ) SUP ( " + orderToSupport.unit + " )");
        }
        else{
            return "( " + unit + " ) SUP " + orderToSupport;
        }
    }
}
