/*    */ package dip.daide.us;
/*    */ 
/*    */ public class PeaceProp extends Proposal
/*    */ {
/*    */   public PeaceProp(String[] origMsg)
/*    */   {
/*  6 */     super(origMsg);
/*    */   }
/*    */ 
/*    */   public boolean isSendingPowerIncluded(String[] prp)
/*    */   {
/* 16 */     boolean included = false;
/* 17 */     for (int i = 4; (i < prp.length) && (!prp[i].equals(")")); i++) {
/* 18 */       if (!prp[i].equals(super.getSender())) continue; included = true;
/*    */     }
/* 20 */     return included;
/*    */   }
/*    */ 
/*    */   public String[] getReply() {
/* 24 */     String yesOrRej = isSendingPowerIncluded(super.getPRP()) ? "YES" : "REJ";
/* 25 */     String[] temp = { "SND", "(", super.getSender(), ")", "(", yesOrRej };
/* 26 */     String[] temp2 = { ")" };
/* 27 */     String[] toReturn = super.concat(temp, super.getPRP());
/* 28 */     toReturn = super.concat(toReturn, temp2);
/*    */ 
/* 31 */     return toReturn;
/*    */   }
/*    */ 
/*    */   public void setTakeOrder()
/*    */   {
/*    */   }
/*    */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.PeaceProp
 * JD-Core Version:    0.6.0
 */