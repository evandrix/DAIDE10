/*     */ package dip.daide.us;
/*     */ 
/*     */ import java.util.List;
/*     */ 
/*     */ public abstract class Unit
/*     */ {
/*     */   protected Node location;
/*     */   protected Power controller;
/*   9 */   protected String order = null;
/*  10 */   protected Node orderLocation = null;
/*  11 */   protected Unit orderSupportUnit = null;
/*  12 */   protected boolean mustRetreat = false; protected boolean beingRemoved = false;
/*     */   private List<Node> retreatNodeList;
/*  14 */   private int stance = 0;
/*     */ 
/*     */   public void setStance(int stance) {
/*  17 */     this.stance = stance;
/*     */   }
/*     */ 
/*     */   public int getStance() {
/*  21 */     return this.stance;
/*     */   }
/*     */ 
/*     */   public Unit(Power controller, Node location) {
/*  25 */     this.controller = controller;
/*  26 */     this.location = location;
/*     */   }
/*     */ 
/*     */   public Power getController() {
/*  30 */     return this.controller;
/*     */   }
/*     */ 
/*     */   public Node getLocation() {
/*  34 */     return this.location;
/*     */   }
/*     */ 
/*     */   public void setLocation(Node location) {
/*  38 */     this.location = location;
/*     */   }
/*     */ 
/*     */   public void setHold() {
/*  42 */     this.order = "HLD";
/*     */   }
/*     */ 
/*     */   public void setMove(Node MTOProvNode) {
/*  46 */     this.order = "MTO";
/*  47 */     this.orderLocation = MTOProvNode;
/*  48 */     MTOProvNode.getProvince().setBeingMovedTo(this);
/*     */   }
/*     */ 
/*     */   public void setSupportHold(Unit SUPUnit) {
/*  52 */     this.order = "SUP";
/*  53 */     this.orderSupportUnit = SUPUnit;
/*  54 */     this.orderLocation = null;
/*     */   }
/*     */ 
/*     */   public void setSupportMove(Unit SUPUnit, Node SUPNode) {
/*  58 */     this.order = "SUP";
/*  59 */     this.orderSupportUnit = SUPUnit;
/*  60 */     this.orderLocation = SUPNode;
/*     */   }
/*     */ 
/*     */   public String getOrderToken()
/*     */   {
/*  77 */     return this.order;
/*     */   }
/*     */ 
/*     */   public Node getOrderLocation() {
/*  81 */     return this.orderLocation;
/*     */   }
/*     */ 
/*     */   public void setMRT(List<Node> retreatNodeList)
/*     */   {
/*  86 */     this.mustRetreat = true;
/*  87 */     this.retreatNodeList = retreatNodeList;
/*     */   }
/*     */ 
/*     */   public List<Node> getMRTList() {
/*  91 */     return this.retreatNodeList;
/*     */   }
/*     */ 
/*     */   public boolean mustRetreat() {
/*  95 */     return this.mustRetreat;
/*     */   }
/*     */ 
/*     */   public void setDisband() {
/*  99 */     this.order = "DSB";
/*     */   }
/*     */ 
/*     */   public void setRetreatTo(Node MRTProv) {
/* 103 */     this.order = "RTO";
/* 104 */     this.orderLocation = MRTProv;
/* 105 */     MRTProv.getProvince().setBeingMovedTo(this);
/*     */   }
/*     */ 
/*     */   public void setRemoval() {
/* 109 */     this.order = "REM";
/* 110 */     this.beingRemoved = true;
/*     */   }
/*     */ 
/*     */   public boolean beingRemoved() {
/* 114 */     if (this.beingRemoved) {
/* 115 */       this.beingRemoved = false;
/* 116 */       return true;
/*     */     }
/* 118 */     return false;
/*     */   }
/*     */ 
/*     */   public abstract String[] getCompleteOrder();
/*     */ 
/*     */   public abstract String getUnitType();
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Unit
 * JD-Core Version:    0.6.0
 */