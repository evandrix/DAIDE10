package state;
import java.util.*;

public abstract class Place{

    public String name;
    public List landConnections = new LinkedList();
    public List seaConnections = new LinkedList();

    // values that Tactics works out each turn
    public double armyValue; // the value of us having an army here (as worked out by the tactics module)
    public double fleetValue; // the value of us having a fleet here (as worked out by the tactics module)
    public double[] basicArmyValue = new double[10];  // DumbBot style proximity value
    public double[] basicFleetValue = new double[10];  // DumbBot style proximity value
    public double strength, competition; // values used as part of calculating the main value

    public double getArmyValue(){
	return armyValue;
    }
    public double getFleetValue(){
	return fleetValue;
    }

    public abstract boolean isAtSea(); // returns true if this is a sea province, and not coastal

    public abstract boolean isValuable();

    public abstract Unit getUnit();

    public abstract Province getProvince(); // if this is a province, returns itself, if this is a coast, returns the province its a coast of

    public abstract List getSurroundingArmies();

    public abstract List getSurroundingFleets();

    public List getSurroundingUnits(){
	// gets all of the units which could attack a unit in this place next turn
        List surroundingUnits = getSurroundingArmies();
        surroundingUnits.addAll(getSurroundingFleets());
        return surroundingUnits;
    }


}
