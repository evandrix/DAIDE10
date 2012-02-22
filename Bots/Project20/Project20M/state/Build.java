package state;
import java.util.*;

public class Build extends Order{

  public Build(Unit u){
    this.unit = u;
  }

    public double evaluate(){
        if (unit instanceof Army){
	    return unit.location.getArmyValue();
        }
        return unit.location.getFleetValue();
    }

    public String toString(){
    	return "( " + unit + " ) BLD";
    }

}
