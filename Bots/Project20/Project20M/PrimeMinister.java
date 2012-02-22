
import java.util.*;
import java.io.*;

import state.*;
import dip.daide.comm.MessageListener;
import dip.daide.comm.DisconnectedException;
import dip.daide.comm.UnknownTokenException;
import dip.daide.comm.Server;

// *** note to all ***
// vc02 has tidied the code and added comments and other random crap. actual code hasnt been modified


// ====================================================================
// PrimeMinister Class
// --------------------------------------------------------------------

public class PrimeMinister extends Thread {


    static final int defaultProposeLength = -1; // how long to propose. length = -1 to specify a time at each proposal
                                                // otherwise it will use the time given

    // declare all variables and classes needed
    //-----------------------------------------

    Server serv;
    state.Map map;
    Tactics t;
    Coordinator n;
    Timer timer;

    LinkedList messages;
    String[] message; // message from server
    int messageIndex; // position in the message

    boolean atPrompt;
    boolean Connected = false;
    boolean Started = false;
    String Country;
    String Passcode;

    int proposalLength = 10;

    //==========================================================
    // PrimeMinister Constructor class
    //==========================================================
    public PrimeMinister(Server srv) {

        messages = new LinkedList();
        serv = srv;

    } // --- end PrimeMinister Constructor class --


    //==========================================================
    // run()
    //==========================================================
    public void run() {

        ////////////////////////////////////while (true) { // or until bot is terminated
        
        	// ajh02: BUSY LOOP HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            if (messages.size() != 0) {
                message = (String[]) messages.removeFirst();
                //printMsg("Server", message);

                // message case
                //=============================================

                // MAP
                //-----------------------------------
                if (message[0].equals("MAP")) {

                    try {
                        printMsg("Bot", new String[]{"MDF"});
                        //                       serv.setMDF(true);
                        serv.send(new String[]{"MDF"});

                        String[] tokens = new String[message.length + 3];
                        tokens[0] = "YES";
                        tokens[1] = "(";
                        System.arraycopy(message, 0, tokens, 2, message.length);
                        tokens[tokens.length - 1] = ")";
                        printMsg("Bot", tokens);
                        serv.send(tokens);

                        Connected = true;
                        System.out.println("Connected");

                    } catch (UnknownTokenException ute) {
                        ute.printStackTrace();
                        System.exit(1);
                    } catch (DisconnectedException de) {
                        System.err.println("Disconnected, exiting");
                        System.exit(1);
                    }
                }


                // HLO
                //-----------------------------------
                else if (message[0].equals("HLO")) {
                    Country = message[2];
                    Passcode = message[5];
                    Started = true;
                    map.us = map.getPowerCalled(Country);
                    System.out.println("Game Started, you are playing " + Country + ", your passcode is " + Passcode);

                }


                // NOW
                //-----------------------------------
                else if (message[0].equals("NOW") && map != null) {
                    System.out.println("PrimeMinister: ******************* Season " + message[2] + " ********************** ");

                    map.season = message[2]; // update the season
                    map.currentTroops(message); // update the units on the map

                    t.valueProvinces();  // update tactic values

                    BufferedReader kb = new BufferedReader(new InputStreamReader(System.in));
                    String letter = "";
                    /*
                 if(map.powers.size() == 7 && map.provinces.size()==75){

                   System.out.println("Do you wish Tactics to display a map (Y/N)");
                   try{
                   letter = kb.readLine();
                   }
                   catch (IOException e){}
                   if(letter.equals("y")||letter.equals("Y")){
                     t.launchMap();
                   }

                 }
                   */
                    List orders = new LinkedList();

                    // WIN (inside NOW)
                    //-----------------------------------
                    if (message[2].equals("WIN")) {
                        //  if it is winter then need to carry out any adjustments (builds and disbands)
                        orders = t.adjustments();
                        sendOrders(orders);
                    }


                    // SUM, AUT (iniside NOW)
                    //-----------------------------------
                    else if (message[2].equals("SUM") || message[2].equals("AUT")) {
                        // if it is summer or autumn then need to carry out any retreats
                        // we are given a list of where all the units are, with some having a (possibly empty) list
                        // of places they have to retreat to
                        messageIndex = 5;
                        //     for (int i = 0; i < message.length; i++){
                        //    System.out.println(message[i]);
                        //}
                        while (messageIndex + 3 < message.length) { // while units remain
                            System.err.println("busy loop here?");
                            if (message[messageIndex + 1].equals(Country)) {  // one of our units are involved // BUG IN THIS LINE!
                                if (message[messageIndex + 3].equals("(")) {  // unit is in multi-coastal province
                                    Province thisProvince = map.getProvinceCalled(message[messageIndex + 4]);
                                    Place thisPlace = thisProvince.getCoastCalled(message[messageIndex + 5]);
                                    Unit thisUnit = map.us.getUnitAt(thisPlace);
                                    System.out.println(message[messageIndex + 7]);
                                    if (message[messageIndex + 7].equals("MRT")) {
                                        messageIndex += 9;
                                        orders.add(getRetreat(thisUnit));
                                    } else {
                                        messageIndex += 7;
                                    }
                                } else {  // unit is not in multi-coastal province
                                    Place thisPlace = map.getProvinceCalled(message[messageIndex + 3]);
                                    Unit thisUnit = map.us.getUnitAt(thisPlace);
                                    if (message[messageIndex + 4].equals("MRT")) {
                                        messageIndex += 6;
                                        orders.add(getRetreat(thisUnit));
                                    } else {
                                        messageIndex += 4;
                                    }
                                }
                            } else { // not one of our units, need to skip details
                                messageIndex += 2;
                                while (messageIndex < message.length && map.getPowerCalled(message[messageIndex]) == null) {
                                    messageIndex++;
                                }
                                if (messageIndex == message.length) {
                                    messageIndex -= 1;
                                } else {
                                    messageIndex -= 2;
                                }
                            }

                            // advance to next unit
                            messageIndex++;
                        } // --- end while(i<message.length) ---
                        sendOrders(orders);
                    } else {
                        // if it is spring or fall then need a Negotiator thread to listen to proposals
                        // PrimeMinister needs to schedule a SendOrdersTask to ensure orders are eventually sent
                        n = new Coordinator(map, t);
                        // should it value the provinces here?
                        List theOrders = n.coordinateUnits();
                        sendOrders(theOrders);
                    }
                }

                // MDF
                //-----------------------------------
                else if (message[0].equals("MDF")) {
                    map = new state.Map(message); // have to tell it it's country when everyone else has joined the game
                    t = new Tactics(map);
                }



                // SCO
                //-----------------------------------
                else if (message[0].equals("SCO") && map != null) { // supply centre ownership message
                    map.updateCentreOwnership(message);
                    System.out.println("We have " + map.numOfProvs() + " supply centers");
                    printMsg("Unhandled", message);
                }


            } // --- end if(msg size) ---

       ///////////////////////////////////////////////////////////// } // --- end while(true) loop ---

    } // --- end run() method ---

    public Order getRetreat(Unit thisUnit) {
        // this function takes a unit and uses current messageIndex:int value to indicate the start of a (possibly empty) list of provinces
        // it returns a single order to be carried out (either a retreat or a disband)
        System.out.println("PrimeMinister: suggesting retreat for unit in " + thisUnit.location.getProvince().name);
        List retreatTos = new LinkedList();
        Place thisPlace;
        while (!message[messageIndex].equals(")")) { // go through each retreat choice
            if (message[messageIndex].equals("(")) { // must be a coast
                Province thisProvince = map.getProvinceCalled(message[messageIndex + 1]);
                thisPlace = thisProvince.getCoastCalled(message[messageIndex + 2]);
                retreatTos.add(thisPlace);
                messageIndex += 4;
            } else { // simply a province
                thisPlace = map.getProvinceCalled(message[messageIndex]);
                retreatTos.add(thisPlace);
                messageIndex += 1;
            }
        }
        return t.getBestRetreat(thisUnit, retreatTos);
    }

    /*
    String [] sendOrder = new String[]{"SUB", "(", "(",message[i], message[i+1], message[i+2], message[i+3], message[i+4], message[i+5],")", "RTO", message[i+8], ")"};
    printMsg("Bot", sendOrder);
    try {
        serv.send(sendOrder);
    } catch (UnknownTokenException ute){
        ute.printStackTrace();
        System.exit(1);
    } catch (DisconnectedException de){
        System.err.println("Disconnected, exiting");
        System.exit(1);
    }


    LinkedList provinces = new LinkedList();
    Province temp = map.getProvinceCalled(message[i+2]);
    i += 8;

    while(!message[i].equals(")")) {
        provinces.add(map.getProvinceCalled(message[i]));
        i++;

    }
    */

    public void sendOrders(List orders) {
        if (orders.size() > 0) {
            String orderToSend = "SUB ";
            System.out.println("");
            System.out.println("PrimeMinister:Final Orders");
            for (int i = 0; i < orders.size(); i++) {
                Order order = (Order) orders.get(i);

                orderToSend += "( " + order + " ) ";
                System.out.println((i + 1) + ") " + order);
            }

            String[] SendOrder = orderToSend.split(" ");

            try {
                serv.send(SendOrder);
            } catch (UnknownTokenException ute) {
                ute.printStackTrace();
                System.exit(1);
            } catch (DisconnectedException de) {
                System.err.println("Disconnected, exiting");
                System.exit(1);
            }
            System.out.println("PrimeMinister: Orders sent");
        } else {
            System.out.println("PrimeMinister: No orders to send");
        }
    }

    //==========================================================
    // addMessage
    //==========================================================
    public void addMessage(String[] Message) {

        messages.add(Message);
        this.run();
        /*           ListIterator iterator = messages.listIterator();
                   System.out.print("PrimeMinister: pending messages = ");
                   while(iterator.hasNext()){
                     String[] thisMessage = (String[]) iterator.next();
                     System.out.print(thisMessage[0] + " ");
                   }
                   System.out.println("");
                   */

    } // --- end addMessage function ---


    //==========================================================
    // printMsh
    //==========================================================
    void printMsg(String who, String[] message) {
        StringBuffer sbuf = new StringBuffer();
        for (int i = 0; i < message.length; i++) {
            String m = message[i];
            sbuf.append(m);
            boolean last = (i + 1) == message.length;
            if (m.equals("(") || (!last && message[i + 1].equals(")"))) {
                continue;
            } else {
                if (!last) {
                    sbuf.append(' ');
                }
            }
        }

        if (atPrompt) {
            System.out.println();
            atPrompt = false;
        }
        System.out.println("<" + who + ">: " + sbuf.toString());

    } // --- end printMsg function ---

} // --- end PrimeMinister class ---

