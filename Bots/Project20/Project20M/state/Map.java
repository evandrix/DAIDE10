package state;
import java.util.*;
import java.io.*;

public class Map
{

/* * * *  NAMING * * * *
 * opponents = all powers not including "us"
 * us = the power the bot is playing
 */
        public List powers; // the list of Power objects
        public List provinces; // the list of Province objects
        public Power us; // the power we are playing as
        public String season; // "SPR" (Spring Moves), "SUM" (Spring retreats), "FAL" (Fall Moves), "AUT" (Fall retreats), WIN (Adjustments)
    public List getAllPlaces(){
	// returns all the Places in the Map
        List allPlaces = new LinkedList();
        for (int i = 0; i < provinces.size(); i++){
            allPlaces.add(provinces.get(i));
	    allPlaces.addAll(((Province)provinces.get(i)).coasts);
        }
        return allPlaces;
    }
  
        public void printMap(String filename){
            System.out.println("We are " + us.name);
            System.out.println("Province data has been written to " + filename);
            try{
            // open file
            File outputFile = new File(filename);
            // create stream
            FileWriter out = new FileWriter(outputFile);
            // write row of column headers
            out.write("Name,armyValue,fleetValue,");
            for (int i=0; i<5; i++){
              out.write("basicarmyvalue["+i+"],");
            }
            for (int i=0; i<5; i++){
              out.write("basicfleetvalue["+i+"],");
            }
            out.write("strength,competition");
            out.write("\n");

            // write province data
	    for (int i = 0; i < provinces.size(); i++){
		((Province)provinces.get(i)).printData(out);
                out.write("\n");
            }
            // close file
            out.close();
            }
            catch (IOException e){
              System.out.println("Error writing to " + filename + ":\n" + e);
            }
        }

        public Power getPowerCalled(String powerName){
	    for (int i = 0; i < powers.size(); i++){
		Power power = (Power)powers.get(i);
                if (power.name.compareTo(powerName) == 0){
		    return power;
                }
            }
	    //   System.out.println("getPowerCalled: No such power: " + powerName);
            return null;
        }

       public Province getProvinceCalled(String provinceName){
	  for (int i = 0; i < provinces.size(); i++){
		Province province = (Province)provinces.get(i);
                if (province.name.compareTo(provinceName) == 0){
		    return province;
                }
            }
	  //   System.out.println("getProvinceCalled: No such province: " + provinceName);
            return null;
       }

       public double getHighestArmyValue(){
         double highest = 0;
	  for (int i = 0; i < provinces.size(); i++){
		Province province = (Province)provinces.get(i);
                if (province.armyValue > highest)
                  highest = province.armyValue;
          }
          return highest;
       }

       public double getHighestFleetValue(){
         double highest = 0;
	  for (int i = 0; i < provinces.size(); i++){
		Province province = (Province)provinces.get(i);
                if (province.fleetValue > highest)
                  highest = province.fleetValue;
          }
          return highest;
       }
       
        public List getOpponents(){      //prev. getPowersExceptUs()
          List result = new LinkedList();
          for (int i = 0; i < powers.size(); i++){
            if((Power) powers.get(i) != us){
              result.add((Power)powers.get(i));
            }
          }
          return result;
        }

        private int readPowers(int i, String[] tokens){
              while(!tokens[i].equals(")")){
                        powers.add(new Power(tokens[i], this));
			i++;
	      }
              return i + 4;
        }

    private int readProvinces(int i, String[] tokens){
        // remember, some supply centres can be a home centre for more than one power!
            provinces = new LinkedList();
            i = readSupplyCentres(i,tokens);
            i = readNonSupplyCentres(i,tokens);
	    return i;
    }

    private int readSupplyCentres(int i, String[] tokens){
        List powers = new LinkedList();
        powers.add("test");
        while (powers.size() > 0){ // assumes that the neutral supply centres are listed last
          powers = new LinkedList(); // the powers this is a home centre for
          if (tokens[i].equals("(")){ // this is the start of a list of powers that these are home-centres for
	    while(!tokens[i].equals(")")){ // first get the powers these are home centres for
                Power power = getPowerCalled(tokens[i]);
                if (power != null){
		  powers.add(getPowerCalled(tokens[i]));
                }
                i++;
            }
          }
          else{ // this is the power this is a homecentre for
            Power power = getPowerCalled(tokens[i]);
            if (power != null){
	      powers.add(getPowerCalled(tokens[i]));
            }
          }
          i++;
          while(tokens[i].compareTo(")") != 0){ // now add these supply centres
	     //System.out.println("AHHH");
             Province newOne = new Province(tokens[i], true, powers, this);
	     provinces.add(newOne);
             i++;
          }
          i = i + 2;
        }        
        return i + 1;
    }

    private int readNonSupplyCentres(int i, String[] tokens){
        while(!tokens[i].equals(")")){
             Province newOne = new Province(tokens[i], false, new LinkedList(), this);
	     provinces.add(newOne);
             i++;
        }
	return i+2;
    }

    private int readAdjacencies(int i, String[] tokens){
        while (i < tokens.length){
            if (getProvinceCalled(tokens[i]) != null){
  	      i = readAdjacency(i, tokens);
            }
            else{
		i++;
            }
        }
	return i;
    }

    private int readAdjacency(int i, String[] tokens){
        Province thisOne = getProvinceCalled(tokens[i]); // the place we're getting the adjacencies for
	i++;
        while (tokens[i].compareTo(")") != 0){
            i++;
	    i = readProv_adj(i, tokens, thisOne);
        }
	return i + 1;
    }

    private int readProv_adj(int i, String[] tokens, Province thisOne){
        String unitType;
	if (tokens[i].compareTo("(") == 0){
            i++;
            i++; // ++ twice, because is it's a coastal connection we know it must be a fleet
            String thisCoastName = tokens[i];
            Coast thisCoast = thisOne.getCoastCalled(thisCoastName);
	    if (thisCoast == null){
                  thisCoast = new Coast(thisOne, thisCoastName);
                  thisOne.coasts.add(thisCoast);
            }
            i++; i++;// this time to get past the ")"
                 while(tokens[i].compareTo(")") != 0){
                      // just make all the fleet connections
                        if (tokens[i].compareTo("(") == 0){
                            i++;
                            Province connectedTo = getProvinceCalled(tokens[i]);
                            i++;
                            String coastName = tokens[i];
                            Coast theCoast = connectedTo.getCoastCalled(coastName);
                            if (theCoast == null){
				theCoast = new Coast(connectedTo, coastName);
                                connectedTo.coasts.add(theCoast);
			    }
                            List seaConnections = thisCoast.seaConnections;
                            seaConnections.add(theCoast);
                            i++;
                        }
                        else{
                          Province connectedTo = getProvinceCalled(tokens[i]);
                          if (connectedTo != null){
	  	             List seaConnections = thisCoast.seaConnections;
	                     seaConnections.add(connectedTo);
                           }
                        }
                        i++;
                  }
		 i--; // don't ask
        }
          unitType = tokens[i];
          i++;
          if (unitType.compareTo("AMY") == 0){
             while(tokens[i].compareTo(")") != 0){ // get the army connections
                 Province connectedTo = getProvinceCalled(tokens[i]);
                 if (connectedTo != null){
		   List landConnections = thisOne.landConnections;
	           landConnections.add(connectedTo);
                 }
	         i++;
             }
          } 
          if (unitType.compareTo("FLT") == 0){
                  while(tokens[i].compareTo(")") != 0){
                      // just make all the fleet connections
                        if (tokens[i].compareTo("(") == 0){
                            i++;
                            Province connectedTo = getProvinceCalled(tokens[i]);
                            i++;
                            String coastName = tokens[i];
                            Coast theCoast = connectedTo.getCoastCalled(coastName);
                            if (theCoast == null){
				theCoast = new Coast(connectedTo, coastName);
                                connectedTo.coasts.add(theCoast);
			    }
                            List seaConnections = thisOne.seaConnections;
                            seaConnections.add(theCoast);
                            i++;
                        }
                        else{
                          Province connectedTo = getProvinceCalled(tokens[i]);
                          if (connectedTo != null){
	  	             List seaConnections = thisOne.seaConnections;
	                     seaConnections.add(connectedTo);
                           }
                        }
		      i++;
                  }
          }
          return i + 1;
    } 

	public Map(String[] tokens)
	{
	        System.out.println("creating a Map (parsing MDF file)");
                provinces = new LinkedList();
                powers = new LinkedList();
                int i=2;
                i = readPowers(i, tokens); // creates the Power objects
                i = readProvinces(i, tokens); // creates the Provinces
                i = readAdjacencies(i, tokens);	// creates the coasts and connections	
                System.out.println("created a Map (MDF file parsed)");
	}

	public boolean isAPower(String Who)
	{

		Power Temp;
		for(int i=0;i<powers.size();i++)
		{
			Temp = (Power) powers.get(i);
			if(Temp.equals(Who))
			{
				return true;
			}
		}
		return false;
	}

	public void currentTroops(String[] tokens)
	{
            // the format of the message is something like:
            // NOW (SPR 1901) (AUS FLT TRI) (AUS AMY BUD) (AUS AMY VIE) (RUS FLT (STP SCS)) (ENG FLT LON) ...

            // Before a retreat turn, units may have a list of retreat options, prefixed by MRT (Must retreat to). E.g. 
            // (ENG FLT NTH MRT (LON YOR EDI NWG))

	        //System.out.println("deleting last turn's troops");

                for (int i = 0; i < powers.size(); i++){
		    Power power = (Power)powers.get(i);
                    power.units = new LinkedList();
                }
		int i=0;
		Province Temp;
		while(i<provinces.size()){
			Temp = (Province) provinces.get(i);
			Temp.clear_Troop();
			i++;
		}

                //System.out.println("Placing this turn's troops");
		for(i=0;i<tokens.length;i++) // now add all the units for this turn
		{
			if(isAPower(tokens[i]))
			{
				if(!tokens[i+2].equals("(")) {
					Temp = (Province) provinces.get(indexOf(tokens[i+2]));
					Power owner = getPowerCalled(tokens[i]);
					Temp.set_Troop(tokens[i+1], owner, Temp);
					owner.units.add(Temp.unit);
				} else	if(tokens[i+2].equals("(")) 	//weird costal case
				{
					Temp = (Province)provinces.get(indexOf(tokens[i+3]));
                                        Coast coast = (Coast)Temp.getCoastCalled(tokens[i+4]);
					Power owner = getPowerCalled(tokens[i]);

					Temp.set_Troop(tokens[i+1], owner, coast);

					owner.units.add(Temp.unit);
				}
			}
		}
                //System.out.println("finished placing this turn's troops");

	}
	public int numOfProvs()
	{
		Province Temp;
		int j=0;
		for(int i=0; i<provinces.size();i++)
		{
			Temp = (Province) provinces.get(i);
			if(Temp.owner != null)
			{
				if(Temp.owner.toString().equals(us.toString()))
				{
					j++;
				}
			}

		}
		return j;
	}

	public int numOfUnits()
	{
		Province Temp;
		int j=0;
		for(int i=0; i<provinces.size();i++)
		{
			Temp = (Province) provinces.get(i);
			if(Temp.unit != null)
			{

				if(Temp.unit.owner.toString().equals(us.toString()))
				{
					j++;
				}
			}

		}
		return j;
	}

	public String[] remRandom()
	{
		Province Temp;
		int j=0;
		for(int i=0; i<provinces.size();i++)
		{
			Temp = (Province) provinces.get(i);
			if(Temp.unit != null)
			{
				if(Temp.unit.owner.toString().equals(us.toString()))
				{
					if(Temp.unit instanceof Army){
						return new String[]{"SUB","(","(",Temp.unit.owner.toString(), "AMY", Temp.unit.location.toString(), ")", "REM",")"};
					} else {
						return new String[]{"SUB","(","(",Temp.unit.owner.toString(), "FLT", Temp.unit.location.toString(), ")", "REM",")"};
					}
				}
			}

		}
		return new String[]{};
	}

	public void updateCentreOwnership(String[] tokens){
                // pre: takes a supply centre ownership message, of the form:
                //      SCO (country centre centre ...) (country centre centre ...) ...
                // post: updates the ownership of all the centres
            int i=1;
	    while(i < tokens.length){
  		i++; // get past the first "("
                if (tokens[i].equals("UNO")){
                  while(tokens[i].compareTo(")") != 0){
                    i++;
                  }
                }
                else{
                  Power owner = getPowerCalled(tokens[i]);
                  i++;
                  while(tokens[i].compareTo(")") != 0){
                      Province centre = getProvinceCalled(tokens[i]);
                      centre.owner = owner;
		      i++;
                  }
                }
                i++; // get past the last ")"
            }
	}

        public void printCentreOwnership(){
          // for debugging purposes
	    //   System.out.println("*************Province Ownership************");
          for (int thisProvince = 0; thisProvince < provinces.size(); thisProvince++){
            Province thisP = (Province) provinces.get(thisProvince);
            if (thisP.supply && thisP.owner != null){
		//      System.out.println(thisP.name + " owned by " + thisP.owner.name);
            }
          }
	  //     System.out.println("**************End Province Ownership**********");
        }

	int indexOf(String Where)
	{
		int i=0;
		Province Temp;
		while(i<provinces.size())
		{
			Temp = (Province) provinces.get(i);
			if(Temp.equals(Where))
			{
				break;
			}
			i++;
		}
		if(i==provinces.size())
		{
			i=-1;
		}
		return i;
	}
}
