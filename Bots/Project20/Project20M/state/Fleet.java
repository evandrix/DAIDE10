package state;
import java.util.*;

public class Fleet extends Unit{

    public Fleet(Power owner, Place location){
        // System.out.println("New fleet for " + owner.name + " at " + location.name + " created");
	this.owner = owner;
        this.location = location;
    }

    public List getValidOrders(){
	List validOrders = new LinkedList();
        validOrders.add(new Hold(this));
        for (int i = 0; i < canMoveTo().size(); i++){
	    validOrders.add(new Move(this, (Place)canMoveTo().get(i)));
        }
        return validOrders;
    }

    public String toString(){
	return owner.name + " FLT " + location;
    }

    public boolean isArmy(){
      return false;
    }

    public List canMoveTo(){
	return location.seaConnections;
    }

}
