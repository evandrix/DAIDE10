package state;

public class Disband extends Order{

  public Disband(Unit u){
    this.unit = u;
  }

    public double evaluate(){
	if (unit instanceof Army){
          return -(unit.location.getArmyValue());
        }
        return -(unit.location.getFleetValue());
    }

    public String toString(){
    	return "( " + unit + " ) DSB";
    }

}
