package state;

public class Remove extends Order{

  public Remove(Unit u){
    this.unit = u;
  }

    public double evaluate(){
	if (unit instanceof Army){
          return -(unit.location.getArmyValue());
        }
        return -(unit.location.getFleetValue());
    }

    public String toString(){
    	return "( " + unit + " ) REM";
    }

}
