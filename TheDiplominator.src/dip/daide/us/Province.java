/*     */ package dip.daide.us;
/*     */ 
/*     */ import java.util.ArrayList;
/*     */ import java.util.Hashtable;
/*     */ import java.util.List;
/*     */ 
/*     */ public class Province
/*     */ {
/*     */   public static final double PEACE_COEFFICIENT = 0.4D;
/*   7 */   private Node[] containsNodes = new Node[3];
/*     */   private String name;
/*   9 */   private boolean isSC = false;
/*     */   private Power ownedBy;
/*  11 */   private boolean beingMovedTo = false;
/*  12 */   private Unit beingMovedToBy = null;
/*  13 */   private int defValue = 0;
/*  14 */   private int attValue = 0;
/*  15 */   private int strValue = 0;
/*  16 */   private int compValue = 0;
/*  17 */   private Hashtable<String, Integer> adjUnitCount = new Hashtable();
/*     */ 
/*  19 */   private List<Power> demilitarisedBy = new ArrayList();
/*     */ 
/*     */   public Province(String name)
/*     */   {
/*  23 */     this.name = name;
/*     */   }
/*     */ 
/*     */   public void setSupplyCentre() {
/*  27 */     this.isSC = true;
/*     */   }
/*     */ 
/*     */   public boolean isSC() {
/*  31 */     return this.isSC;
/*     */   }
/*     */ 
/*     */   public String getName() {
/*  35 */     return this.name;
/*     */   }
/*     */ 
/*     */   public void capture(Power newOwner) {
/*  39 */     this.ownedBy = newOwner;
/*     */   }
/*     */ 
/*     */   public void setNode0(Node node) {
/*  43 */     this.containsNodes[0] = node;
/*     */   }
/*     */ 
/*     */   public void setNode1(Node node) {
/*  47 */     this.containsNodes[1] = node;
/*     */   }
/*     */ 
/*     */   public void setNode2(Node node) {
/*  51 */     this.containsNodes[2] = node;
/*     */   }
/*     */ 
/*     */   public Node[] getNodeArray() {
/*  55 */     return this.containsNodes;
/*     */   }
/*     */ 
/*     */   public List<Node> getNodeList() {
/*  59 */     List nodes = new ArrayList();
/*  60 */     for (int nodeCount = 0; nodeCount < 3; nodeCount++) {
/*  61 */       if (this.containsNodes[nodeCount] == null) continue; nodes.add(this.containsNodes[nodeCount]);
/*     */     }
/*  63 */     return nodes;
/*     */   }
/*     */ 
/*     */   public Power ownedBy() {
/*  67 */     return this.ownedBy;
/*     */   }
/*     */ 
/*     */   public boolean isOccupied() {
/*  71 */     for (int i = 0; i < 3; i++) {
/*  72 */       if ((this.containsNodes[i] != null) && 
/*  73 */         (this.containsNodes[i].isOccupied())) return true;
/*     */     }
/*     */ 
/*  76 */     return false;
/*     */   }
/*     */ 
/*     */   public void calcDefVal(Power me)
/*     */   {
/*  81 */     int defVal = 0;
/*     */ 
/*  83 */     for (int currentNode = 0; currentNode < 2; currentNode++) {
/*  84 */       if (this.containsNodes[currentNode] != null) {
/*  85 */         for (int i = 0; i < this.containsNodes[currentNode].getAdjNodeCount(); i++) {
/*  86 */           if (this.containsNodes[currentNode].getAdjacentNodes()[i].isOccupied()) {
/*  87 */             Power adjPower = this.containsNodes[currentNode].getAdjacentNodes()[i].getUnit().getController();
/*  88 */             if ((adjPower.getName().compareTo(me.getName()) != 0) && (adjPower.getPowerSize() > defVal)) {
/*  89 */               defVal = adjPower.getPowerSize();
/*     */             }
/*     */           }
/*     */         }
/*     */       }
/*     */     }
/*  95 */     this.defValue = defVal;
/*     */   }
/*     */ 
/*     */   public void resetAttVal() {
/*  99 */     this.attValue = 0;
/*     */   }
/*     */ 
/*     */   public void resetDefVal() {
/* 103 */     this.defValue = 0;
/*     */   }
/*     */ 
/*     */   public void setAttVal(int ownersPowerSize) {
/* 107 */     this.attValue = ownersPowerSize;
/* 108 */     if (this.ownedBy.acceptedPeace()) this.attValue = (int)(this.attValue * 0.4D);
/* 109 */     this.defValue = 0;
/*     */   }
/*     */ 
/*     */   public List<Node> getAdjacentProvinces()
/*     */   {
/* 114 */     List adjProvinces = new ArrayList();
/*     */ 
/* 116 */     for (int nodeCount = 0; nodeCount < 3; nodeCount++) {
/* 117 */       if (this.containsNodes[nodeCount] != null) {
/* 118 */         for (int i = 0; i < this.containsNodes[nodeCount].getAdjNodeCount(); i++)
/* 119 */           if (!adjProvinces.contains(this.containsNodes[nodeCount].getAdjacentNodes()[i]))
/* 120 */             adjProvinces.add(this.containsNodes[nodeCount].getAdjacentNodes()[i]);
/*     */       }
/*     */     }
/* 123 */     return adjProvinces;
/*     */   }
/*     */ 
/*     */   public void setStrengthValue(int units) {
/* 127 */     this.strValue = units;
/*     */   }
/*     */ 
/*     */   public void setCompValue(int units) {
/* 131 */     this.compValue = units;
/*     */   }
/*     */ 
/*     */   public int getCompValue() {
/* 135 */     return this.compValue;
/*     */   }
/*     */ 
/*     */   public int getStrengthValue() {
/* 139 */     return this.strValue;
/*     */   }
/*     */ 
/*     */   public Unit getUnit()
/*     */   {
/* 144 */     for (int i = 0; i < 3; i++) {
/* 145 */       if ((this.containsNodes[i] != null) && 
/* 146 */         (this.containsNodes[i].isOccupied())) {
/* 147 */         return this.containsNodes[i].getUnit();
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 152 */     return null;
/*     */   }
/*     */ 
/*     */   public void setBeingMovedTo(Unit movingUnit)
/*     */   {
/* 157 */     this.beingMovedToBy = movingUnit;
/* 158 */     this.beingMovedTo = true;
/*     */   }
/*     */ 
/*     */   public boolean isBeingMovedTo() {
/* 162 */     return this.beingMovedTo;
/*     */   }
/*     */ 
/*     */   public Unit getUnitBeingMovedTo() {
/* 166 */     return this.beingMovedToBy;
/*     */   }
/*     */ 
/*     */   public void resetBeingMovedTo() {
/* 170 */     this.beingMovedTo = false;
/* 171 */     this.beingMovedToBy = null;
/*     */   }
/*     */ 
/*     */   public int getDefVal() {
/* 175 */     return this.defValue;
/*     */   }
/*     */ 
/*     */   public int getAttVal() {
/* 179 */     return this.attValue;
/*     */   }
/*     */ 
/*     */   public void setAdjUnitCount(Hashtable<String, Integer> newAdjUnitCount) {
/* 183 */     this.adjUnitCount = newAdjUnitCount;
/*     */   }
/*     */ 
/*     */   public int getAdjUnitCount(String power) {
/* 187 */     return ((Integer)this.adjUnitCount.get(power)).intValue();
/*     */   }
/*     */ 
/*     */   public Unit getOccupyingUnit() {
/* 191 */     for (int i = 0; i < 3; i++) {
/* 192 */       if ((this.containsNodes[i] != null) && 
/* 193 */         (this.containsNodes[i].isOccupied())) return this.containsNodes[i].getUnit();
/*     */     }
/*     */ 
/* 196 */     return null;
/*     */   }
/*     */ 
/*     */   public void demilitarise(Power DMZpower) {
/* 200 */     this.demilitarisedBy.add(DMZpower);
/*     */   }
/*     */ 
/*     */   public void remilitarise(Power NOTDMZpower) {
/* 204 */     this.demilitarisedBy.remove(NOTDMZpower);
/*     */   }
/*     */ 
/*     */   public List<Power> getDemilitariseList() {
/* 208 */     return this.demilitarisedBy;
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Province
 * JD-Core Version:    0.6.0
 */