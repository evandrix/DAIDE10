import java.util.*;
import state.*;
import java.lang.Math;

class Tactics{

 static state.Map map;
 static Random random;


  int numberProvinces;

/****************************************public functions***************************************************************/


 public Tactics(state.Map map){
   // constructor
   this.map = map;
   this.random = new Random();
 }

 public void valueProvinces(){
   // sets overall value for each province
   enumerateValues();
 }

 public List getBestOrders(){
   // find a list of best independent orders for all our units
   return createOrders();
 }

 public Order getNextBestOrder(Order order){
     // return the next best order this unit could do, after the given order
     List orders = order.unit.getValidOrders();
     List filteredOrders = filter(orders);
     Order bestOrder = null;
     double bestScore = -9999999;
     ListIterator iterator = filteredOrders.listIterator();
     while (iterator.hasNext())
     {
       Order possibleOrder = (Order) iterator.next();
       double thisScore = possibleOrder.evaluate();
       if (thisScore > bestScore && thisScore < order.evaluate()){
         bestOrder = possibleOrder; // point best order so far to this one
         bestScore = thisScore; // set highest score so far to this one
       }
     }
     if (bestOrder == null){
       iterator = orders.listIterator();
       while (iterator.hasNext())
       {
         Order possibleOrder = (Order) iterator.next();
         double thisScore = possibleOrder.evaluate();
         if (thisScore > bestScore && thisScore < order.evaluate()){
           bestOrder = possibleOrder; // point best order so far to this one
           bestScore = thisScore; // set highest score so far to this one
         }
       }
     }
     if (bestOrder == null){
	 System.out.println("getNextBestOrder returned null!? Returning a hold");
         bestOrder = new Hold(order.unit);
     }
     return bestOrder;
 }

 public List adjustments(){
   // return a list of Build orders, or a list of Remove orders
   int numberBuilds = map.us.getOwnedSupplyCentres().size() - map.us.units.size();
   if(numberBuilds>0){
     System.out.println("Suggesting builds");
     return suggestBuilds(numberBuilds);
   }
   else if(numberBuilds<0){
     System.out.println("Suggesting removes");
     return suggestRemoves(-numberBuilds);
   }
   else return new LinkedList();  // no adjustments to make
 }

  public Order getBestRetreat(Unit unit, List canRetreatTo){
        List retreatOrders = new LinkedList();
        ListIterator iterator = canRetreatTo.listIterator();
        while (iterator.hasNext())
        {
          Place thisPlace = (Place)iterator.next();
	    if (thisPlace.getProvince().unit == null){ // we might have retreated another unit to there
		retreatOrders.add(new Retreat(unit, thisPlace));
            }
        }
        if (retreatOrders.size() == 0){
	    return new Disband(unit);
        }
        Collections.sort(retreatOrders);
        Order bestRetreat = (Order) retreatOrders.get(0);
        bestRetreat.target.getProvince().unit = unit; // assume there's a unit there now
        System.out.println("retreating with: " + bestRetreat);
        return (Order)retreatOrders.get(0);
  }

  public static boolean isIn(Object thing, List list){
      // stupid function. This shouldn't be here
      // using this because List.contains(Object) seems to not work... does this work?
      // returns true iff thing is in list
     ListIterator iterator = list.listIterator();
     while (iterator.hasNext())
     {
	 Object compareTo = iterator.next();
       if (thing == compareTo || (thing instanceof Order && ((Order)thing).toString().equals(((Order)compareTo).toString()))){
         return true;
       }
      }
      return false;
  }

  public static List removeDuplicates(List l){
	for (int i = 0; i < l.size(); i++){
            for (int j = i+1; j < l.size(); j++){
		if (l.get(i) == l.get(j)){
		    l.remove(j);
                    j--;
                }
            }
        }
        return l;
    }

/****************************************private functions***************************************************************/

 private void enumerateValues(){
    // updates those all-important provincial values in the map depending on the season
    numberProvinces = map.provinces.size();
    if (map.season.equals("SPR") || map.season.equals("SUM") || map.season.equals("WIN")){
      setBasicValues(Constants.spring_defence_weight, Constants.spring_attack_weight);  // updates those all-important provincial values in the GPD;
      setStrengthAndCompetitionValues();  // update the strength and competition values in the GPD
      setFinalValues(Constants.spring_iteration_weight, Constants.spring_strength_weight, Constants.spring_competition_weight); // use basic, strength and competition values to calculate an overall value for each province
    }
    else{ // map.season must be "FAL" or "AUT"
      setBasicValues(Constants.autumn_defence_weight, Constants.autumn_attack_weight);  // updates those all-important provincial values in the GPD;
      setStrengthAndCompetitionValues();  // update the strength and competition values in the GPD
      setFinalValues(Constants.autumn_iteration_weight, Constants.autumn_strength_weight, Constants.autumn_competition_weight); // use basic, strength and competition values to calculate an overall value for each province
    }
    // map.printMap(map.us+".csv");
  }

  private void setBasicValues(int defence_weight, int attack_weight){
  /* the basic value is found as follows
  1) every province is given a basicValue[0] where
       basicValue[0] = size of strongest adjacent power * weighting for supply centres that are under our control
       basicValue[0] = size of owning power for supply centres under enemy control
       basicValue[0] = 0 in all other situations
  2) basicValue[0..n] are calculated iteratively, where
       basicValue[i] for a province is the sum of basicValue[i-1] for each adjacent province added to basicValue[i-1] for current province
  */
    calculateInitialValues(defence_weight, attack_weight);  // sets the basicValue[0] in each province for spring
    iterateValues(Constants.numberIterations); // iterates up to basicValue[n] in each province FOR NOW SET TO 10
  }

 private void setStrengthAndCompetitionValues(){
    /* the strength and competition values are found as follows
    Strength value for a province = number of units we have that can move into this province next turn
    Competition value for a province = greatest number of units any other power has that can move into this province next turn
    */
    Province theProvince, thisAP;
    Power thisOwner;
    for (int thisProvince=0; thisProvince<numberProvinces; thisProvince++) {
      // for each province
        theProvince = (Province) map.provinces.get(thisProvince);
        int[] numbers = theProvince.getNumberOfAdjacentUnits(map.powers);
        for(int i = 0; i < numbers.length; i++){
          if(((Power)map.powers.get(i)) == map.us){
            theProvince.strength = numbers[i];
            numbers[i] = 0;
          }
        }
        Arrays.sort(numbers);
        theProvince.competition = numbers[numbers.length-1];
    }
  }

  private void setFinalValues(int[] iteration_weight, int strength_weight, int competition_weight){
    // this function finds final armyValue and fleetValue for each Place
    List places = map.getAllPlaces();
    ListIterator iterator = places.listIterator();
     while (iterator.hasNext())
     {
      Place thePlace = (Place)iterator.next(); // for each Place
      thePlace.armyValue = 0;
      thePlace.fleetValue = 0;

      for (int j = 0; j<Constants.numberIterations; j++){
        thePlace.armyValue += thePlace.basicArmyValue[j] * iteration_weight[j];
        thePlace.fleetValue += thePlace.basicFleetValue[j] * iteration_weight[j];
      }

    }
  }




  private void calculateInitialValues(int defence_weight, int attack_weight){
    // this function finds basicValue[0] for each place
    Province theProvince;

    for (int thisProvince=0; thisProvince<numberProvinces; thisProvince++) {
      theProvince = (Province) map.provinces.get(thisProvince);
      if (theProvince.supply) { // if its a supply centre
	if (theProvince.owner == null){ // and its neutral
          theProvince.basicArmyValue[0] = getWeakestPower() * attack_weight;
          theProvince.basicFleetValue[0] = getWeakestPower() * attack_weight;
        }
        else{
	  if (theProvince.owner == map.us) { // if it belongs to us
            // set value to strongest adjacent power
            double strongestAdjacentOpponent = theProvince.strongestAdjacentOpponentStrength();
            theProvince.basicArmyValue[0] = strongestAdjacentOpponent * defence_weight;
            theProvince.basicFleetValue[0] = strongestAdjacentOpponent * defence_weight;
          }
	  else { // it belongs to an opponent
            // set value to strength of owner
            double ownerStrength = theProvince.owner.getStrength();
            theProvince.basicArmyValue[0] = ownerStrength * attack_weight;
            theProvince.basicFleetValue[0] = ownerStrength * attack_weight;
          }
        }
        if (isIn(map.us,theProvince.homeCentres)){
	    // if its a home/build centre for us, its worth more
            theProvince.basicArmyValue[0] *= 1.1; // constant alert!
            theProvince.basicFleetValue[0] *= 1.1; // constant alert!
        }
      }
      else { // it isn't a supply centre
        // set value to 0
        theProvince.basicArmyValue[0] =  0;
        theProvince.basicFleetValue[0] =  0;
      }

      // finally, if the province has any coasts, set their values to the values of the province
      ListIterator iterator = theProvince.coasts.listIterator();
     while (iterator.hasNext())
     {
       Coast thisCoast = (Coast) iterator.next();
       thisCoast.basicArmyValue[0] = theProvince.basicArmyValue[0];
       thisCoast.basicFleetValue[0] = theProvince.basicFleetValue[0];
     }

    }

  }

  private double getAveragePower(){
    double averagePower = 0;
    ListIterator iterator = map.powers.listIterator();
    while (iterator.hasNext())
    {
      averagePower += ((Power) iterator.next()).getStrength();
    }
    averagePower = averagePower / map.powers.size();
    return averagePower;
  }

  private double getWeakestPower(){
    ListIterator iterator = map.powers.listIterator();
    Power thisPower = (Power) iterator.next();
    double weakestPower = thisPower.getStrength();
    while (iterator.hasNext())
    {
      thisPower = (Power) iterator.next();
      if (thisPower.getStrength() < weakestPower){
	weakestPower = thisPower.getStrength();
      }
    }
    return weakestPower;
  }

  private void iterateValues(int n){
    //  this function iterates up to basicFleetValue[n] and basicArmyValue[n] for each place
    List allPlaces = map.getAllPlaces();
    for (int i = 1; i < n; i++){
      // for each iteration
      ListIterator iterator = allPlaces.listIterator();
      while (iterator.hasNext())
      {
            // for each place
            Place thePlace = (Place)iterator.next();
            thePlace.basicArmyValue[i] = 0;
            thePlace.basicFleetValue[i] = 0;
            List landConnections = thePlace.landConnections;
            List seaConnections = thePlace.seaConnections;

            List canGetToByConvoy = new LinkedList();
            if (!thePlace.isAtSea()){ // if this place is not out at sea
              for (int k = 0; k < seaConnections.size(); k++){
	      Place seaConnection = (Place)seaConnections.get(k);
                if (seaConnection.getProvince().unit != null && seaConnection.getProvince().unit instanceof Fleet && seaConnection.isAtSea()){
		  // this Fleet could convoy us somewhere
                  for (int j = 0; j < seaConnection.seaConnections.size(); j++){
                    // for each place that fleet could move to
                    Province couldConvoyTo = ((Place)seaConnection.seaConnections.get(j)).getProvince();
		    if (!couldConvoyTo.isAtSea() && !isIn(couldConvoyTo,landConnections)){
                        canGetToByConvoy.add(couldConvoyTo);
                    }
                  }
                }
              }
            }
            canGetToByConvoy = removeDuplicates(canGetToByConvoy);
            ListIterator convoyIterator = canGetToByConvoy.listIterator();
            while (convoyIterator.hasNext()){
              Place thisConnection = (Place)convoyIterator.next();
	      thePlace.basicArmyValue[i] += thisConnection.basicArmyValue[i-1] * 0.05; // constant!
            }

            ListIterator landIterator = landConnections.listIterator();
            while (landIterator.hasNext())
            {
              Place thisConnection = (Place) landIterator.next();
	      thePlace.basicArmyValue[i] += thisConnection.basicArmyValue[i-1];
	      if (!isIn(thisConnection,seaConnections)){
                thePlace.basicFleetValue[i] += thisConnection.basicArmyValue[i-1] * 0.001; // constant!
              }
            }
            ListIterator seaIterator = seaConnections.listIterator();
            while (seaIterator.hasNext())
            {
              Place thisConnection = (Place) seaIterator.next();
              if (!isIn(thisConnection,landConnections)){
	        thePlace.basicArmyValue[i] += thisConnection.basicFleetValue[i-1] * 0.02; // constant!
              }
              thePlace.basicFleetValue[i] += thisConnection.basicFleetValue[i-1];
            }
            thePlace.basicArmyValue[i] += thePlace.basicArmyValue[i-1];
            thePlace.basicFleetValue[i] += thePlace.basicFleetValue[i-1];
            thePlace.basicArmyValue[i] /=  Constants.iteration_army_divisor;
            thePlace.basicFleetValue[i] /=  Constants.iteration_fleet_divisor;
        }
    }
  }

    private static List lastTurnsOrders = new LinkedList(); // the independant orders we generated last turn

  private static List createOrders(){
    // this function uses the overall values for each province to find each units best move independent of any other move decisions
    // basically they find all the provinces they could move to
    // (including places they could move to by convoy (including being convoyed by the enemy))
    // then they move to the best one (or hold if its already in the best place it can immediately get to)

    List orders = new LinkedList();
    List allOurUnits = map.us.units;
    //System.out.println("******* creating basic orders");
    ListIterator iterator = allOurUnits.listIterator();
    while (iterator.hasNext())
    {
      Unit theUnit = (Unit) iterator.next();
      orders.add(getBestOrder(theUnit));
    }
    //iterator = orders.listIterator();
    //while (iterator.hasNext()){
    //System.out.println("Order: ( " + iterator.next() + " )");
    //}
    //System.out.println("******* finished creating basic orders");
    lastTurnsOrders = orders;
    return orders;
  }

    private static List filter(List orders){
	// filters the list of orders a unit can do
        // to remove stupid ones

        // it can move into or stay in valuable places
      List filteredOrders = new LinkedList();
      ListIterator iterator = orders.listIterator();
      while (iterator.hasNext())
      {
        Order thisOrder = (Order) iterator.next();
        Place target = thisOrder.target;
        if (target.isValuable()){ // if its an enemy supply centre, or a threatened one of ours
	    filteredOrders.add(thisOrder);
        }
      }
      // If its SPR it can also move into / stay in places within 1 move of a valuable place
      if (map.season.equals("SPR")){
      iterator = orders.listIterator();
      while (iterator.hasNext())
      {
            Order thisOrder = (Order) iterator.next();
	    List withinTwo = new LinkedList();
            if (((Order)orders.get(0)).unit instanceof Army){
		withinTwo = thisOrder.target.landConnections;
            }
            else{
		withinTwo = thisOrder.target.seaConnections;
            }
            if (hasValuable(withinTwo)){ // if withinTwo contains a valuable place, add it to filteredAdjacents
		filteredOrders.add(thisOrder);
	    }
        }
      }
      if (filteredOrders.size() == 0){ // if none of the orders made it through the filtering, then let it choose the best from all its orders
        filteredOrders.addAll(orders);
      }
      return filteredOrders;
    }

  private static Order getBestOrder(Unit u){
    // get the best order for this unit

    List orders = u.getValidOrders(); // hold, all its moves, and all its MoveByConvoys

    // Now filter the orders.
    List filteredOrders = filter(orders);
    Collections.sort(filteredOrders);
    Order bestOrder = (Order)filteredOrders.get(0);
    if (isIn(bestOrder,lastTurnsOrders)){
    	// should choose the next best order with some random probability
        if (filteredOrders.size() > 1 && random.nextFloat() > 0.9){ // 10% probability
          System.out.println("RANDOM: CHOOSING THE 2ND BEST ORDER FOR " + u);
          bestOrder = (Order)filteredOrders.get(1);
        }
    }

    // a little hack... if the bestOrder is to hold on a province we own, threatened by just 1 enemy, move into where he is
    if (bestOrder instanceof Hold && bestOrder.target.getProvince().owner == map.us){ // if the order is to hold on a province we own

        // if there's a supply centre we don't own and its within reach, move there
	List canMoveTo = bestOrder.unit.canMoveTo();
        ListIterator iterator = canMoveTo.listIterator();
        while(iterator.hasNext()){
          Place thisPlace = (Place) iterator.next();
	    if (thisPlace.getProvince().supply && thisPlace.getProvince().owner != map.us){
		bestOrder = new Move(u,thisPlace);
                return bestOrder;
            }
        }

        List surroundingUnits = bestOrder.target.getProvince().getSurroundingUnits();
        if (surroundingUnits.size() == 1){ // if there's just one enemy unit around us
            Place location = ((Unit)surroundingUnits.get(0)).location;
            if (isIn(location,u.canMoveTo())){ // if we can move into where he is
                System.out.println(u + " IS HOLDING ON A SUPPLY CENTRE ONLY THREATENED BY " + surroundingUnits.get(0));
                System.out.println("SO IT IS MOVING INTO " + ((Unit)surroundingUnits.get(0)).location);
		bestOrder = new Move(u,((Unit)surroundingUnits.get(0)).location);
            }
        }
    }

    return bestOrder;
  }

    private static boolean hasValuable(List places){
	// returns true iff any of the places is a supply centre we don't own, or a supply centre we own which is under threat
        ListIterator iterator = places.listIterator();
        while(iterator.hasNext()){
	    if (((Place)iterator.next()).isValuable()){
		return true;
            }
        }
        return false;
    }

    private Order getBestBuild(){
	// pre: assumes the provinces are valued
        List ownedBuildCentres = map.us.getOwnedBuildCentres(); // getCanBuild
        List canBuild = new LinkedList();
        ListIterator iterator = ownedBuildCentres.listIterator();
        while(iterator.hasNext()){ // only if there isn't a unit there can we build there
          Province thisProvince = (Province) iterator.next();
	    if (thisProvince.unit == null){
		canBuild.add(thisProvince);
            }
        }
        if (canBuild.size() == 0){
	    return new Waive(map.us);
        }
        Build bestSoFar = new Build(new Army(map.us,(Place)canBuild.get(0)));
        iterator = canBuild.listIterator();
        while(iterator.hasNext()){
            Province thisProvince = (Province) iterator.next();
	    Build newArmy = new Build(new Army(map.us, thisProvince));
            if (newArmy.evaluate() > bestSoFar.evaluate()){
		bestSoFar = newArmy;
            }
            // BUG: does this code for dealing with build centres with coasts work?
	    for (int i = 0; i < thisProvince.coasts.size(); i++){
		Coast thisCoast = (Coast)thisProvince.coasts.get(i);
                Build newFleet = new Build(new Fleet(map.us, thisCoast));
                if (newFleet.evaluate() > bestSoFar.evaluate()){
		  bestSoFar = newFleet;
                }
            }
	    if (thisProvince.coasts.size() == 0){
              Build newFleet = new Build(new Fleet(map.us, thisProvince));
              if (newFleet.evaluate() > bestSoFar.evaluate()){
		  bestSoFar = newFleet;
              }
            }
        }
        return bestSoFar;
    }

  private List suggestBuilds(int numberBuilds){
     List builds = new LinkedList();
     for (int i = 0; i<numberBuilds; i++){
	 // probably don't have to check if its coastal b4 building a fleet, as if it isn't coastal
         // fleet score should be less than army score because it has no connections
	 valueProvinces();
         Order bestBuild = getBestBuild();
         if (bestBuild instanceof Build){
           bestBuild.unit.location.getProvince().unit = bestBuild.unit; // add that unit to the map
         }
         builds.add(bestBuild);
     }
     return builds;
  }

    private Remove getBestRemove(){
        // pre: assumes we have at least 1 unit to remove
        //      and that the provinces have been valued
	List ourUnits = map.us.units;
        Remove bestSoFar = new Remove((Unit)ourUnits.get(0));
        ListIterator iterator = ourUnits.listIterator();
        while(iterator.hasNext()){
	    Remove newRemove = new Remove((Unit)iterator.next());
            if (newRemove.evaluate() > bestSoFar.evaluate()){
		bestSoFar = newRemove;
            }
        }
        return bestSoFar;
    }

    private List suggestRemoves(int numRemoves){
        List removes = new LinkedList();
	for (int i = 0; i < numRemoves; i++){
	    valueProvinces();
            Remove bestRemove = getBestRemove();
            bestRemove.unit.location.getProvince().unit = null; // remove that unit from the map
            for (int j = 0; j < map.us.units.size(); j++){
		if (map.us.units.get(j) == bestRemove.unit){
		    map.us.units.remove(j);
                }
            }
            removes.add(bestRemove);
        }
        return removes;
    }


   List suggestRetreat(Unit u, List retreatTos){
System.out.println("Tactics: Suggesting Retreats");
     // pseudocode for working out retreats:
     // bug: two units might compete for the same place to retreat to, which one should get it? Currently whichever one gets picked first. but it should be the one with the highest 2nd best place to retreat to. Keep the algorithm as it is unless this becomes a problem (or you can think up a way of solving the problem)
     //
     // t = a temporary list of all the supply centres we can build in we own

     //  r = list of places this unit could retreat to
     //  if this is a winter retreat and the dumbot_value of the highest valued build-centre in "t" is higher than the highest valued place in "r",
     //      disband the unit
     //      and remove the highest valued build centre from "t"
     //  otherwise
     //    retreat to the place in "r" with the highest dumbbot_value

     List canBuildAts = map.us.getOwnedBuildCentres();
     List decisions = new LinkedList();  // a list of Orders

     int canRetreatToCount;
     Province highestValuedRetreat = (Province) retreatTos.get(0);
     double retreatValue, thisValue;
     if(u instanceof Army){
       retreatValue = highestValuedRetreat.armyValue;
     }
     else{
       retreatValue = highestValuedRetreat.fleetValue;
     }
     for (canRetreatToCount = 1; canRetreatToCount < retreatTos.size(); canRetreatToCount++){

       Province retreatTo = (Province) retreatTos.get(canRetreatToCount);
System.out.println("Tactics: considering retreat to " + retreatTo.name);
       if(u instanceof Army){
         thisValue = retreatTo.armyValue;
       }
       else{
         thisValue = retreatTo.fleetValue;
       }
       if (thisValue > retreatValue){
         highestValuedRetreat = retreatTo;
         retreatValue = thisValue;
       }
     }
     System.out.println("Tactics: HighestValuedRetreat = " + highestValuedRetreat.name);

     if(map.season.equals("WIN")){  // a winter retreat, so consider disbanding and building elsewhere
       int canBuildCount;
       Build possibleBuild;
       Build highestValuedBuild = new Build(new Army(map.us, (Province) canBuildAts.get(0)));
       for (canBuildCount = 0; canBuildCount < canBuildAts.size(); canBuildCount++){  // 0 as have to check fleet in first location
         Province canBuildAt = (Province) canBuildAts.get(canBuildCount);
         possibleBuild = new Build(new Army(map.us, canBuildAt));
         if (possibleBuild.evaluate() > highestValuedBuild.evaluate()){
           highestValuedBuild = possibleBuild;
         }
         possibleBuild = new Build(new Fleet(map.us, canBuildAt));
         if (possibleBuild.evaluate() > highestValuedBuild.evaluate()){
           highestValuedBuild = possibleBuild;
         }
       }
       Province highestValued = (Province) canBuildAts.get(0);
       for (canBuildCount = 0; canBuildCount < canBuildAts.size(); canBuildCount++){
         Province thisProvince = (Province) canBuildAts.get(canBuildCount);
	 //         if (thisProvince.value > highestValued.value){
	 //    highestValued = thisProvince;
	 //   }
       }
       if(highestValuedBuild.evaluate() >= retreatValue){
         map.us.units.add(highestValuedBuild.unit);
         decisions.add(new Disband(u));
         decisions.add(highestValuedBuild);
       }
       else{
         decisions.add(new Move(u, highestValuedRetreat));
       }
     }
     else{ // not winter, must retreat
       decisions.add(new Move(u, highestValuedRetreat));
     }
     return decisions;
   }
 }
