/*    */ package dip.daide.us;
/*    */ 
/*    */ public abstract class Proposal
/*    */ {
/*    */   private String[] prp;
/*    */   private String[] to;
/*    */   private String from;
/*  8 */   boolean takeOrder = false;
/*    */ 
/*    */   public Proposal(String[] origMsg)
/*    */   {
/* 12 */     String[] tempMessage = origMsg;
/* 13 */     tempMessage = takeRight(tempMessage, 1);
/* 14 */     this.from = tempMessage[1];
/*    */ 
/* 16 */     int close = findClose(tempMessage);
/* 17 */     tempMessage = takeRight(tempMessage, close);
/*    */ 
/* 19 */     close = findClose(tempMessage);
/* 20 */     this.to = new String[close - 2];
/* 21 */     for (int i = 1; i < close - 2; i++) {
/* 22 */       this.to[(i - 1)] = tempMessage[i];
/*    */     }
/* 24 */     tempMessage = takeRight(tempMessage, close);
/* 25 */     this.prp = new String[tempMessage.length];
/* 26 */     for (int i = 0; i < tempMessage.length; i++)
/* 27 */       this.prp[i] = tempMessage[i];
/*    */   }
/*    */ 
/*    */   public String getSender()
/*    */   {
/* 32 */     return this.from;
/*    */   }
/*    */ 
/*    */   public String[] getPRP() {
/* 36 */     return this.prp;
/*    */   }
/*    */ 
/*    */   protected String[] takeRight(String[] tokens, int start) {
/* 40 */     int length = tokens.length;
/* 41 */     String[] right = new String[length - start];
/*    */ 
/* 43 */     for (int i = 0; i < length - start; i++) {
/* 44 */       right[i] = tokens[(start + i)];
/*    */     }
/*    */ 
/* 47 */     return right;
/*    */   }
/*    */ 
/*    */   protected int findClose(String[] tokens) {
/* 51 */     int bracketCount = 0;
/* 52 */     int i = 0;
/*    */ 
/* 54 */     if (!tokens[0].equals("(")) return -1;
/*    */ 
/* 56 */     for (i = 0; i < tokens.length; i++)
/*    */     {
/* 58 */       if (tokens[i].equals("(")) bracketCount++;
/* 59 */       if (tokens[i].equals(")")) bracketCount--;
/* 60 */       if (bracketCount == 0) return i + 1;
/*    */     }
/*    */ 
/* 63 */     return -1;
/*    */   }
/*    */ 
/*    */   public String[] concat(String[] first, String[] second) {
/* 67 */     String[] third = new String[first.length + second.length];
/* 68 */     int i = 0;
/* 69 */     for (i = 0; i < first.length; i++) {
/* 70 */       third[i] = first[i];
/*    */     }
/*    */ 
/* 73 */     for (i = first.length; i < first.length + second.length; i++) {
/* 74 */       third[i] = second[(i - first.length)];
/*    */     }
/* 76 */     return third;
/*    */   }
/*    */ 
/*    */   public abstract String[] getReply();
/*    */ 
/*    */   public abstract void setTakeOrder();
/*    */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Proposal
 * JD-Core Version:    0.6.0
 */