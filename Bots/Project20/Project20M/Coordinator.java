import java.util.*; // for List
import state.*;

public class Coordinator{

    private state.Map map;
    private Tactics tactics;
    private List wantedOrders;

/****************************************public functions***************************************************************/

    public Coordinator(state.Map map, Tactics tactics){
	this.map = map;
        this.tactics = tactics;
    }

    public List coordinateUnits(){
	return coordinateUnits(new LinkedList(), new LinkedList());
    }

    public List coordinateUnits(Order weDo, Order theyDo){
	LinkedList weDoList = new LinkedList();
        if (weDo != null){
          weDoList.add(weDo);
        }
        LinkedList theyDoList = new LinkedList();
        if (theyDo != null){
          theyDoList.add(theyDo);
        }
        return coordinateUnits(weDoList, theyDoList);
    }

    public List coordinateUnits(List weDo, List theyDo){
        // PRE: assumes that the provinces have been valued
        //      weDo is a list of Orders we promised to do
        //      theyDo is a list of Orders other players promised to do
        // POST: returns a list of orders our units should do

        wantedOrders = new LinkedList();
        List orderList = tactics.getBestOrders(); // get the units best individual orders

        // set the orders we've promised to do
        for (int i = 0; i < weDo.size(); i++){
	    Support order = (Support)weDo.get(i);
            if (order != null){
              order.helping = order.orderToSupport;
	      changeOrder(order.unit,order,orderList);
            }
        }

        for (int i = 0; i < orderList.size(); i++){ // set the units as having these orders
	    ((Order)orderList.get(i)).unit.order = (Order)orderList.get(i);
        }
	//System.out.println("**** coordinating units ****");
	Collections.sort(orderList); // sort the orderList from most valuable to least
        for (int i = 0; i < orderList.size(); i++){
	    orderList = coordinateUnit(orderList, i, theyDo);
	}
        //System.out.println("**** finished coordinating units ****");
        return orderList;
    }

    public List getWantedOrders(){
        // returns the list of orders we would like other ppl to do
        // pre: assumes that coordinate units has been run
        System.out.println("******wantedOrders:");
        for (int i = 0; i < wantedOrders.size(); i++){
            Order order = (Order)wantedOrders.get(i);
            if (order instanceof Support
                && ((Support)order).orderToSupport.target.getProvince().unit != null
                && ((Support)order).orderToSupport.target.getProvince().unit.owner == order.unit.owner){
		// we're asking someone to support a move into where they have a unit
                // which is silly
                wantedOrders.remove(i);
                i--;
                continue;
            }
	    System.out.println(order);
        }
        System.out.println("*******************");
	return wantedOrders;
    }


/****************************************private functions***************************************************************/

    private List coordinateUnit(List orderList, int i, List theyDo){

	    // coordinates the unit at index i in orderList
            Order order = (Order)orderList.get(i); // for each order in the orderList
            //System.out.println("looking at " + order);
            List avaliables = getUnitsAvaliable(orderList, i); // units which are avaliable to help order

            // should first check it will be able to get any essential help it may need

            // if theres somebody, who's orders target is the same as ours, 
            // try to get them to go somewhere else
            for (int j = 0; j < orderList.size(); j++){
		Order clash = (Order)orderList.get(j);
                if (clash != order && clash.target.getProvince() == order.target.getProvince()){
                    Order outOfWay = getBestOutOfTheWayOrder(clash.unit,orderList);
		    if (Tactics.isIn(clash.unit,avaliables) && outOfWay != null){
                        orderList = changeOrder(clash.unit,outOfWay,orderList);
                        //System.out.println(outOfWay.unit + " agrees to do " + outOfWay);
                    }
                    else{
                      Order nextBestOrder = tactics.getNextBestOrder(order);
                      orderList = changeOrder(order.unit, nextBestOrder, orderList);
                      //System.out.println("changing to " + nextBestOrder + " because of " + clash);
                      orderList = coordinateUnit(orderList, i, theyDo);
                      return orderList;
                    }
                }
            }

            // alsoAvaliable are units which are avaliable so long as the help they give doesn't require them to move
            List alsoAvaliables = getUnitsAlsoAvaliable(orderList, i);
            avaliables.addAll(0, alsoAvaliables); // puts all the units in alsoAvaliable at the front of the list

            if (order instanceof MoveByConvoy){
                Fleet needed = (Fleet)((Province)((MoveByConvoy)order).seaProvinces.get(0)).unit;
		if (tactics.isIn(needed,avaliables)){ // if the unit we need to convoy us is avaliable, get it to do the convoy
		  Order newOrder = new Convoy(needed,order.unit,(Province)order.target);     
                  newOrder.helping = order;
                  orderList = changeOrder(needed, newOrder, orderList);             
                  //System.out.println(needed + " agrees to do " + newOrder);
                }
                else{
		  if (needed.owner != map.us){
                      Order wantedOrder = new Convoy(needed,order.unit,(Province)order.target);     
                      wantedOrder.helping = order;
		      wantedOrders.add(wantedOrder);
		  }
                  Order nextBestOrder = tactics.getNextBestOrder(order);
                  orderList = changeOrder(order.unit, nextBestOrder, orderList);
                  //System.out.println("Couldn't get the convoy orders done, so now doing " + nextBestOrder);
                  orderList = coordinateUnit(orderList, i, theyDo);
                  return orderList;
                }
            }

            int supportsNeeded = order.getSupportsNeeded();
            //System.out.println(supportsNeeded + " supports needed");
            if (order instanceof Move && order.target.getProvince().unit != null && order.target.getProvince().unit.owner == map.us){
		supportsNeeded = 0;
                //System.out.println("But we're not going to ask for any support as we're following one of our units");
            }

            List couldGiveSupport  = order.couldBeSupportedBy();
            for (int j = 0; j < couldGiveSupport.size() && supportsNeeded > 0; j++){
                Unit couldHelp = (Unit)couldGiveSupport.get(j);
                //System.out.println(couldHelp + " could support us...");
		if (tactics.isIn(couldHelp, avaliables)){
                    //System.out.println(couldHelp + " will support us");
                    Order supportOrder = new Support(couldHelp, order); // avaliable unit supporting order
                    supportOrder.helping = order;
                    orderList = changeOrder(couldHelp, supportOrder, orderList);
                    avaliables.remove(couldHelp);
                    j--;
                    supportsNeeded--; // this shouldn't strictly be here
                }
                else{
		    if (couldHelp.owner != map.us){
			// BUG: it should only ask for this if there are units attacking here, which don't belong to couldHelp.owner
                        // otherwise it should ask for couldHelp to NOT move into order.target
                      Order wantedOrder = new Support(couldHelp, order); // avaliable unit supporting order
                      wantedOrder.helping = order;
                      wantedOrders.add(wantedOrder);
                    }
                }
            }
	    return orderList;

    }

    private double evaluate(List orderList){
	// works out how good a set of orders are for us
        double value = 0;
        for (int i = 0; i < orderList.size(); i++){
            Order order = (Order)orderList.get(i);
	    value += order.evaluate();
            if (order instanceof Support && ((Support)order).orderToSupport.unit.owner == map.us){
		value += 0.75 * ((Support)order).orderToSupport.evaluate(); // constant alert!
            }
        }
        if (value < 0){
	    value *= -1; // bug: what the hell!??
        }
        return value;
    }

                private Order getBestOutOfTheWayOrder(Unit unit, List orderList){
		    // returns the best move unit can do not into anywhere there is an order with that target,
                    // check to make sure this isn't null!
                    List validOrders = unit.getValidOrders();
                    List possibleOrders = new LinkedList();
                    for (int i = 0; i < validOrders.size(); i++){ // for each validOrder
			Order validOrder = (Order)validOrders.get(i);
                        boolean possible = true; // say its possible...
                        for (int j = 0; j < orderList.size(); j++){  // now for each order we're doing
			    Order inList = (Order)orderList.get(j);
                            if (validOrder instanceof MoveByConvoy || validOrder.target == inList.target
                                || (validOrder.target.getProvince().unit != null && validOrder.target.getProvince().unit.owner == map.us)){
				possible = false; // then the validOrder wasn't possible as an outOfTheWay order
                            }
                        }
                        if (possible){ // if it was possible add it to possibleOrders
			    possibleOrders.add(validOrder);
                        }
                    }
                    if (possibleOrders.size() == 0){
			return null;
                    }
                    Collections.sort(possibleOrders);
                    return (Order)possibleOrders.get(0);
                }

		private Order getOrderFor(Unit unit, List orderList){
		    // post: gets the Order for unit in orderList
                    for (int i = 0; i < orderList.size(); i++){
			Order order = (Order)orderList.get(i);
                        if (order.unit == unit){
			    return order;
                        }
                    }
                    return null; // should never happen!
                }

		private List changeOrder(Unit unit, Order order, List orderList){
		    // post: changes the order of Unit unit to Order order in orderList and returns the new list
                    // first it removes the old order for that unit from the list - use getOrderFor
                    // then it inserts (ordered) the new order into the list
                    for (int i = 0; i < orderList.size(); i++){
			Order orderInList = (Order)orderList.get(i);
                        if (unit == orderInList.unit){
			    orderList.remove(i);
                        }
                    }
                    orderList.add(order); // this is a dum way of doing this
                    Collections.sort(orderList);
                    order.unit.order = order;
                    return orderList;
                }

    private List getUnitsAvaliable(List orderList, int i){
	// post: returns the units in orderList after i which aren't already helping someone
        List avaliableToHelp = new LinkedList();
        for (int j = i+1; j < orderList.size(); j++){
          Order order = (Order)orderList.get(j);
	  if (order.helping == null || order.helping == orderList.get(i)){ // if this Order is below order and isn't helping someone already, or is helping this order
	    avaliableToHelp.add(order.unit);
          }
        }
        return avaliableToHelp;
    }

    private List getUnitsAlsoAvaliable(List orderList, int i){
        // post: returns the units in orderList before i which are currently holding
	List alsoAvaliable = new LinkedList();
        for (int j = 0; j < i; j++){
	    Order order = (Order)orderList.get(j);
            if (order instanceof Hold){
		alsoAvaliable.add(order.unit);
            }
        }
        return alsoAvaliable;
    }

}
