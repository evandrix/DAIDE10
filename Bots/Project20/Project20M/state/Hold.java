package state;
import java.util.*;

public class Hold extends Supportable{

  public Hold(Unit u){
    this.unit = u;
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
    	return "( " + unit + " ) HLD";
    }

}
