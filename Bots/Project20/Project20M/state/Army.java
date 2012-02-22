package state;
import java.util.*;

public class Army extends Unit{

    public Army(Power owner, Place location){
        // System.out.println("New army for " + owner.name + " at " + location.name + " created");
	this.owner = owner;
        this.location = location;
    }

    public List getValidOrders(){
	List validOrders = new LinkedList();
        validOrders.add(new Hold(this));
        for (int i = 0; i < canMoveTo().size(); i++){
	    validOrders.add(new Move(this, (Province)canMoveTo().get(i)));
        }
        // now add all the MoveByConvoys it could do :-o
        // BUG: only adds convoys through 1 sea province
        for (int i = 0; i < location.seaConnections.size(); i++){
	    Place seaConnection = (Place)location.seaConnections.get(i);
            if (seaConnection.getProvince().unit != null && seaConnection.getProvince().unit instanceof Fleet && seaConnection.isAtSea()){
		// this Fleet could convoy us somewhere
                for (int j = 0; j < seaConnection.seaConnections.size(); j++){
                    // for each place that fleet could move to
                    Province couldConvoyTo = ((Place)seaConnection.seaConnections.get(j)).getProvince();
		    if (!couldConvoyTo.isAtSea()){
                        List seaProvinces = new LinkedList();
                        seaProvinces.add(seaConnection);
			validOrders.add(new MoveByConvoy(this,seaProvinces,couldConvoyTo));
                    }
                }
            }

        }

        return validOrders;
    }

    public String toString(){
      return owner.name + " AMY " + location;
    }

    public boolean isArmy(){
      return true;
    }

    public List canMoveTo(){
	return location.landConnections;
    }

}
