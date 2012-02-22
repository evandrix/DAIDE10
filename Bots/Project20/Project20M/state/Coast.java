package state;
import java.util.*;

public class Coast extends Place{

    public Province coastOf;

    public Coast(Province coastOf, String name){
        this.coastOf = coastOf;
        this.name = name;
    }

    public String toString(){
	return ("( " + coastOf.name + " " + name + " )");
    }

    public Unit getUnit(){
	return coastOf.unit;
    }

    public Province getProvince(){
	return this.coastOf;
    }

    public boolean isValuable(){
	return getProvince().isValuable();
    }

    public boolean isAtSea(){
	return false;
    }

    public List getSurroundingFleets(){
	// gets all the fleets which could attack a unit here next turn
        return coastOf.getSurroundingFleets();
    }

    public List getSurroundingArmies(){
	// gets all the fleets which could attack a unit here next turn
        return coastOf.getSurroundingArmies();
    }



}
