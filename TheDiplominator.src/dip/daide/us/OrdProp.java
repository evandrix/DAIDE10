/*     */ package dip.daide.us;
/*     */ 
/*     */ import java.io.PrintStream;
/*     */ 
/*     */ public class OrdProp extends Proposal
/*     */ {
/*     */   private String powerToExeMove;
/*     */   private String node2;
/*     */   private String node1;
/*     */   private String node3;
/*     */   private String node4;
/*     */   private String order;
/*     */   private String allyOrder;
/*     */ 
/*     */   public OrdProp(String[] origMsg)
/*     */   {
/*  15 */     super(origMsg);
/*  16 */     establishNodesAndOrder();
/*     */   }
/*     */ 
/*     */   public String[] getReply()
/*     */   {
/*  21 */     String[] toReturn = (String[])null;
/*  22 */     if (this.takeOrder) {
/*  23 */       String[] temp = { "SND", "(", super.getSender(), ")", "(", "YES" };
/*  24 */       String[] temp2 = { ")" };
/*  25 */       toReturn = super.concat(temp, super.getPRP());
/*  26 */       toReturn = super.concat(toReturn, temp2);
/*     */     }
/*     */     else {
/*  29 */       String[] temp = { "SND", "(", super.getSender(), ")", "(", "REJ" };
/*  30 */       String[] temp2 = { ")" };
/*  31 */       toReturn = super.concat(temp, super.getPRP());
/*  32 */       toReturn = super.concat(toReturn, temp2);
/*     */     }
/*     */ 
/*  35 */     return toReturn;
/*     */   }
/*     */ 
/*     */   public void establishNodesAndOrder()
/*     */   {
/*  43 */     String[] prp = super.getPRP();
/*     */ 
/*  45 */     prp = takeRight(prp, 5);
/*     */ 
/*  50 */     this.powerToExeMove = prp[1];
/*  51 */     this.node1 = (prp[3] + prp[2]);
/*  52 */     this.order = prp[findClose(prp)];
/*  53 */     System.out.println("Do I have the correct order? " + this.order);
/*     */ 
/*  55 */     if (this.order.equals("MTO"))
/*     */     {
/*  59 */       String[] moveTo = takeRight(prp, findClose(prp) + 1);
/*     */ 
/*  61 */       this.node2 = (moveTo[0] + prp[2]);
/*  62 */       System.out.print("I think Node 2 is: " + this.node2);
/*     */     }
/*  64 */     else if (this.order.equals("SUP")) {
/*  65 */       String[] allyMove = takeRight(prp, findClose(prp) + 1);
/*     */ 
/*  67 */       this.node3 = (allyMove[3] + allyMove[2]);
/*  68 */       String[] allyTo2 = takeRight(allyMove, findClose(allyMove));
/*     */ 
/*  71 */       this.allyOrder = (allyTo2[0].equals(")") ? "HLD" : allyTo2[0]);
/*     */ 
/*  76 */       if (this.allyOrder.equals("MTO"))
/*  77 */         this.node4 = (allyTo2[1] + allyMove[2]);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void setTakeOrder()
/*     */   {
/*  86 */     this.takeOrder = true;
/*     */   }
/*     */ 
/*     */   public void isGoodMove(Power me, Power power)
/*     */   {
/*     */   }
/*     */ 
/*     */   public String getAllyOrder() {
/*  94 */     return this.allyOrder;
/*     */   }
/*     */ 
/*     */   public String getOrder() {
/*  98 */     return this.order;
/*     */   }
/*     */ 
/*     */   public String getNode4() {
/* 102 */     return this.node4;
/*     */   }
/*     */ 
/*     */   public String getNode3()
/*     */   {
/* 107 */     return this.node3;
/*     */   }
/*     */ 
/*     */   public String getNode2() {
/* 111 */     return this.node2;
/*     */   }
/*     */ 
/*     */   public String getNode1() {
/* 115 */     return this.node1;
/*     */   }
/*     */ 
/*     */   public String powerToMove() {
/* 119 */     return this.powerToExeMove;
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.OrdProp
 * JD-Core Version:    0.6.0
 */