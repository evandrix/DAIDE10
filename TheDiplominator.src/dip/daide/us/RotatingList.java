/*    */ package dip.daide.us;
/*    */ 
/*    */ import java.util.ArrayList;
/*    */ import java.util.List;
/*    */ 
/*    */ public class RotatingList
/*    */ {
/*    */   private List<Integer> list;
/*    */   private int currentElement;
/*    */ 
/*    */   public RotatingList(int size)
/*    */   {
/* 11 */     this.list = new ArrayList();
/* 12 */     for (int i = 0; i < size; i++)
/* 13 */       this.list.add(Integer.valueOf(0));
/*    */   }
/*    */ 
/*    */   public void incrementLastElement()
/*    */   {
/* 19 */     this.currentElement += 1;
/*    */   }
/*    */ 
/*    */   public void flushChange() {
/* 23 */     this.list.remove(0);
/* 24 */     this.list.add(Integer.valueOf(this.currentElement));
/* 25 */     this.currentElement = 0;
/*    */   }
/*    */ 
/*    */   public int getSum() {
/* 29 */     int total = 0;
/* 30 */     for (int i = 0; i < this.list.size(); i++) {
/* 31 */       total += ((Integer)this.list.get(i)).intValue();
/*    */     }
/* 33 */     return total;
/*    */   }
/*    */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.RotatingList
 * JD-Core Version:    0.6.0
 */