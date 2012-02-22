package state;

import java.util.*;

public class Power {
    public List units;
    public String name;
    public Map map;


    // ========================================================================
    // Power Constructor method
    // =========================================================================
    public Power(String name, Map map) {
        this.name = name;
        this.map = map;
    }


    public Unit getUnitAt(Place unitLocation) {
        ListIterator iterator = units.listIterator();
        while (iterator.hasNext()) {
            Unit thisUnit = (Unit) iterator.next();
            if (thisUnit.location == unitLocation) {
                return thisUnit;
            }
        }
        return null;
    }

    public List getOwnedSupplyCentres() {
        // returns the list of all the supply centre provinces this power owns
        List provinces = map.provinces;
        List ownedSupply = new LinkedList();
        for (int i = 0; i < provinces.size(); i++) {
            Province province = (Province) provinces.get(i);
            if (province.owner == this && province.supply) {
                ownedSupply.add(province);
            }
        }
        return ownedSupply;
    }

    public List getOwnedBuildCentres() {
        // returns the list of all the provinces this power owns and can build in
        List ownedSupply = getOwnedSupplyCentres();
        int i = 0;
        while (i < ownedSupply.size()) {
            Province province = (Province) ownedSupply.get(i);
            boolean included = false;
            for (int j = 0; j < province.homeCentres.size(); j++) {
                Power power = (Power) province.homeCentres.get(j);
                if (power == this) {
                    included = true;
                }
            }
            if (included) {
                i++;
            } else {
                ownedSupply.remove(i);
            }
        }
        return ownedSupply;
    }

    public double getStrength() {
        // strength is:
        // A * x^2 + B * x + C
        // where A, B and C are constants
        // and x is the number of supply centres the power owns

        // these values are just stolen from dumbbot, they may well not be the best
        double A = 1.0;
        double B = 4.0;
        double C = 16.0;
        List ownedProvinces = getOwnedSupplyCentres();
        double x = ownedProvinces.size();
        return A * x * x + B * x + C;
    }

    public String toString() {
        return name;
    }

    public boolean equals(Object is_called) {
        return name.equals(is_called);
    }

}
