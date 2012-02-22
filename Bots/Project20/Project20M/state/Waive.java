package state;
import java.util.*;

public class Waive extends Order{

  private Power us;

  public Waive(Power us){
    this.us = us;
  }

    public double evaluate(){
        return 0;
    }

    public String toString(){
    	return us.name + " WVE";
    }

}
