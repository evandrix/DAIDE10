package state;

import java.util.*;
import java.io.*;

public class Province extends Place {

    public Map map; // the map this province is in

    // info that changes each turn
    public Unit unit; // the unit which is here (null if this province is empty)
    public Power owner; //Country who controls this supply center(might not be the same as Unit_Country at the start of a Fall turn)

    //Constant map info
    public boolean supply;		// true if it is a supply center
    public List homeCentres;	// the list of Powers this is a homeCentre for
    public List coasts;

    // Variables that negotiator will use
    public boolean provinceInAgreement = false;     // used to check if any agreement is made in this province.
                                                    // Eg. TRUE if province is in agreement of a unit to move INTO it

    
    //===================================================================================
    // resetNegotiatorVariables()
    //
    // function called by negotiator
    // pre: null
    // post: reset all variables used by negotiator in province and in unit if unit exist
    //====================================================================================
    public void resetNegotiatorVariables(){
        provinceInAgreement = false;
        // reset unit varaiables
        if(unit!=null){
            unit.resetNegotiatorVariables();
        }
    }


    public Unit getUnit() {
        return unit;
    }

    public Province getProvince() {
        return this;
    }

    public boolean isAtSea() {
        // returns true iff this is a sea province and not coastal
        if (landConnections.size() == 0) {
            return true;
        }
        return false;
    }

    public int getNumberOfAdjacentUnits(Power power) {
        // returns the number of units the given power has which could invade this province next turn
        // + 1 if that power already has a unit here
        List surroundingUnits = getALLSurroundingUnits();
        Unit surroundingUnit;
        int number = 0;
        for (int i = 0; i < surroundingUnits.size(); i++) {
            surroundingUnit = (Unit) surroundingUnits.get(i);
            if (surroundingUnit.owner == power) {
                number++;
            }
        }
        return number;
    }

    public int[] getNumberOfAdjacentUnits(List powerList) {
        // returns the number of units each power in the list has which could invade this province next turn
        // +1 if that power already has a unit here
        List surroundingUnits = getALLSurroundingUnits(); // surrounding units, including your ones
        Unit surroundingUnit;
        int[] numbers = new int[powerList.size()];
        for (int i = 0; i < surroundingUnits.size(); i++) {
            surroundingUnit = (Unit) surroundingUnits.get(i);
            for (int j = 0; j < powerList.size(); j++) {
                if (surroundingUnit.owner == ((Power) powerList.get(j))) {
                    numbers[j]++;
                }
            }
        }
        return numbers;
    }

    public List getAdjacentPlaces() {
        // returns a duplicate free list of all Places adjacent to this one, whether by sea or land connections and for any coast
        List adjacents = new LinkedList();
        adjacents.addAll(landConnections);
        adjacents.addAll(seaConnections);
        for (int i = 0; i < coasts.size(); i++) {
            Coast thisCoast = (Coast) coasts.get(i);
            adjacents.addAll(thisCoast.seaConnections);
        }
        return removeDuplicates(adjacents);
    }

    public static List removeDuplicates(List l) {
        int lcount = 0;
        int mcount;
        while (lcount < l.size()) {
            Object thisObj = l.get(lcount);
            mcount = lcount + 1;
            while (mcount < l.size()) {
                if (thisObj == l.get(mcount)) {
                    l.remove(mcount);
                } else {
                    mcount++;
                }
            }
            lcount++;
        }
        return l;
    }

    public boolean isValuable() {
        // returns true iff the province is an enemy supply centre, or a threatened one of our supply centres
        if (supply && (owner != map.us || getSurroundingUnits().size() > 0)) {
            return true;
        }
        return false;
    }

    public double strongestAdjacentOpponentStrength() {
        // returns the strength of the strongest OPPONENT which has a unit which could move into this province
        List surroundingPowers = getSurroundingPowers();
        Power surroundingPower;
        double strongest = 0;
        double thisStrength;
        for (int i = 0; i < surroundingPowers.size(); i++) {
            surroundingPower = (Power) surroundingPowers.get(i);
            if (surroundingPower != map.us) {
                thisStrength = surroundingPower.getStrength();
                if (thisStrength > strongest) {
                    strongest = thisStrength;
                }
            }
        }
        return strongest;
    }

    public List getSurroundingPowers() {
        // returns a list of the units which could enter this place/province next turn
        List surroundingUnits = getSurroundingUnits();
        List surroundingPowers = new LinkedList();
        for (int i = 0; i < surroundingUnits.size(); i++) {
            Unit surroundingUnit = (Unit) surroundingUnits.get(i);
            Power thisOwner = surroundingUnit.owner;
            if (!surroundingPowers.contains(thisOwner)) { // if owner isn't already in surroundingPowers
                surroundingPowers.add(thisOwner);
            }
        }
        return surroundingPowers;
    }

    public List getSurroundingUnits() {
        // gets all the ENEMY units which could attack a unit here next turn
        List surroundingUnits = getSurroundingArmies();
        surroundingUnits.addAll(getSurroundingFleets());
        return surroundingUnits;
    }

    public List getALLSurroundingUnits() {
        // gets all the units (OURS AND ENEMIES) which could attack a unit here next turn
        List surroundingUnits = new LinkedList();
        for (int i = 0; i < landConnections.size(); i++) {
            Province landConnection = (Province) landConnections.get(i);
            if (landConnection.unit != null && landConnection.unit instanceof Army) {
                surroundingUnits.add(landConnection.unit);
            }
        }
        List seaConnections = new LinkedList();
        seaConnections.addAll(this.seaConnections);
        for (int i = 0; i < coasts.size(); i++) { // now get all the seaConnections of all the coasts this place has
            Coast coast = (Coast) coasts.get(i);
            seaConnections.addAll(coast.seaConnections);
        }
        for (int i = 0; i < seaConnections.size(); i++) {
            Place seaConnection = (Place) seaConnections.get(i);
            Unit unit = seaConnection.getUnit();
            if (unit != null && unit instanceof Fleet && unit.location == seaConnection) { // the last test here is to make sure the fleet is at a coast that is surrounding
                surroundingUnits.add(unit);
            }
        }
        return surroundingUnits;
    }

    public List getSurroundingArmies() {
        // gets all the ENEMY armies which could attack a unit here next turn (without being convoyed)
        List surroundingArmies = new LinkedList();
        for (int i = 0; i < landConnections.size(); i++) {
            Province landConnection = (Province) landConnections.get(i);
            if (landConnection.unit != null && landConnection.unit instanceof Army && landConnection.unit.owner != map.us) {
                //System.out.println(name + " surrounded by army in " + landConnection.name);
                surroundingArmies.add(landConnection.unit);
            }
        }
        return surroundingArmies;
    }

    public List getALLSurroundingArmies() {
        // gets all the ENEMY AND OUR armies which could attack a unit here next turn (without being convoyed)
        List surroundingArmies = new LinkedList();
        for (int i = 0; i < landConnections.size(); i++) {
            Province landConnection = (Province) landConnections.get(i);
            if (landConnection.unit != null && landConnection.unit instanceof Army) {
                surroundingArmies.add(landConnection.unit);
            }
        }
        return surroundingArmies;
    }

    public List getALLSurroundingFleets() {
        // gets all the ENEMY AND OUR fleets which could attack a unit here next turn
        List seaConnections = new LinkedList();
        seaConnections.addAll(this.seaConnections);
        for (int i = 0; i < coasts.size(); i++) { // now get all the seaConnections of all the coasts this place has
            Coast coast = (Coast) coasts.get(i);
            seaConnections.addAll(coast.seaConnections);
        }
        List surroundingFleets = new LinkedList();  // now get all the fleets that could move in via these sea connections
        for (int i = 0; i < seaConnections.size(); i++) {
            Place seaConnection = (Place) seaConnections.get(i);
            Unit unit = seaConnection.getUnit();
            if (unit != null && unit instanceof Fleet) {
                surroundingFleets.add(unit);
            }
        }
        return surroundingFleets;
    }

    public List getSurroundingFleets() {
        // gets all the ENEMY fleets which could attack a unit here next turn
        List seaConnections = new LinkedList();
        seaConnections.addAll(this.seaConnections);
        for (int i = 0; i < coasts.size(); i++) { // now get all the seaConnections of all the coasts this place has
            Coast coast = (Coast) coasts.get(i);
            seaConnections.addAll(coast.seaConnections);
        }
        List surroundingFleets = new LinkedList();  // now get all the fleets that could move in via these sea connections
        for (int i = 0; i < seaConnections.size(); i++) {
            Place seaConnection = (Place) seaConnections.get(i);
            Unit unit = seaConnection.getUnit();
            if (unit != null && unit instanceof Fleet && unit.owner != map.us) {
                //System.out.println(name + " surrounded by fleet in " + seaConnection.name);
                surroundingFleets.add(unit);
            }
        }
        return surroundingFleets;
    }

    public Coast getCoastCalled(String name) {
        // returns the coast this province has called name
        // if it doesn't have a coast called name, it returns null
        for (int i = 0; i < coasts.size(); i++) {
            Coast coast = (Coast) coasts.get(i);
            if (coast.name.compareTo(name) == 0) {
                return coast;
            }
        }
        return null;
    }


    public int getMaxUnitsAdjacentPower(List powers) {
        // returns the maximum number of units a power in the list has that can move next turn into this province
        int max = 0;
        Power powerFromList;
        int adjacentUnits;
        for (int i = 0; i < powers.size(); i++) { // for each power in the given list
            powerFromList = (Power) powers.get(i);
            adjacentUnits = getNumberOfAdjacentUnits(powerFromList);
            if (adjacentUnits > max) {
                max = adjacentUnits;
            }
        }
        return max;
    }

    public Province(String name, boolean supply_dep, List homeCentres, Map map) {
        coasts = new LinkedList();
// homeCentres is the list of powers this is a creation centre for (sometimes it can be a creation centre for more than one)
        this.map = map;
        this.name = name;
        supply = supply_dep;
        this.homeCentres = homeCentres;
    }

    public boolean equals(Object is_called) {
        return name.equals(is_called);
    }

    public String toString() {
        return name;
    }

    public void clear_Troop() {
        unit = null;
    }

    public void set_Troop(String Type, Power owner, Place place) {
        if (Type.compareTo("AMY") == 0) {
            unit = new Army(owner, place);
        } else {
            unit = new Fleet(owner, place);
        }
    }

    public void clear_Ownership() {
        owner = null;
    }

    public void set_Ownership(Power owner) {
        this.owner = owner;
    }

    public Power get_Ownership() {
        return owner;
    }

    public void printData(FileWriter out) throws IOException
// output the current data for this province as a comma-separated line in the given output stream
    {
// a row of data
        out.write(name);
        out.write(",");
        out.write(Double.toString(armyValue));
        out.write(",");
        out.write(Double.toString(fleetValue));
        out.write(",");
        for (int count = 0; count < 5; count++) {
            out.write(Double.toString(basicArmyValue[count]) + ",");
        }
        for (int count = 0; count < 5; count++) {
            out.write(Double.toString(basicFleetValue[count]) + ",");
        }
        out.write(Double.toString(strength));
        out.write(",");
        out.write(Double.toString(competition));
    }
}
