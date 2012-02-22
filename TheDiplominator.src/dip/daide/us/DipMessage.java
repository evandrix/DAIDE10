/*    */ package dip.daide.us;
/*    */ 
/*    */ import java.util.Hashtable;
/*    */ 
/*    */ public abstract class DipMessage
/*    */ {
/*  5 */   private Power[] to = new Power[6];
/*    */   private Power from;
/*  7 */   private int countTo = 0;
/*    */   private Map map;
/*    */ 
/*    */   public DipMessage(String[] origMsg, Map map)
/*    */   {
/* 12 */     this.map = map;
/* 13 */     this.from = ((Power)map.listOfPowers.get(origMsg[2]));
/*    */ 
/* 15 */     for (int i = 5; !origMsg[i].equals(")"); i++) {
/* 16 */       this.to[this.countTo] = ((Power)map.listOfPowers.get(origMsg[i]));
/* 17 */       this.countTo += 1;
/*    */     }
/*    */   }
/*    */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.DipMessage
 * JD-Core Version:    0.6.0
 */