package state;
import java.util.*;

public class Convoy extends Order{
    public Province convoyTo;
    public Unit unitToConvoy;

    public Convoy(Unit unit, Unit unitToConvoy, Province convoyTo){
      this.unit = unit;
      this.target = unit.location;
      this.unitToConvoy = unitToConvoy;
      this.convoyTo = convoyTo;
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
        // (unit) CVY (unit) CTO province
	return "( " + unit + " ) CVY ( " + unitToConvoy + " ) CTO " + convoyTo;
    }

}
