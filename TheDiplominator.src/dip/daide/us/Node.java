/*     */ package dip.daide.us;
/*     */ 
/*     */ import java.util.ArrayList;
/*     */ import java.util.Hashtable;
/*     */ import java.util.List;
/*     */ 
/*     */ public class Node
/*     */ {
/*     */   private String name;
/*     */   private Unit unit;
/*     */   private Province province;
/*  12 */   private Node[] adjacentNodes = new Node[20];
/*  13 */   private Hashtable<String, Integer> destValue = new Hashtable();
/*  14 */   private int[] proximityMap = new int[10];
/*  15 */   private int adjNodeCount = 0;
/*  16 */   private boolean buildHere = false;
/*     */ 
/*     */   public Node(String name)
/*     */   {
/*  20 */     this.name = name;
/*     */   }
/*     */ 
/*     */   public Province getProvince() {
/*  24 */     return this.province;
/*     */   }
/*     */ 
/*     */   public String toString() {
/*  28 */     return this.name;
/*     */   }
/*     */ 
/*     */   public void occupy(Unit unit) {
/*  32 */     this.unit = unit;
/*     */   }
/*     */ 
/*     */   public Node[] getAdjacentNodes() {
/*  36 */     return this.adjacentNodes;
/*     */   }
/*     */ 
/*     */   public int getAdjNodesCount() {
/*  40 */     return this.adjNodeCount;
/*     */   }
/*     */ 
/*     */   public List<Node> getAdjacentNodesList() {
/*  44 */     List nodes = new ArrayList();
/*  45 */     for (int nodeCount = 0; nodeCount < this.adjNodeCount; nodeCount++) {
/*  46 */       nodes.add(this.adjacentNodes[nodeCount]);
/*     */     }
/*  48 */     return nodes;
/*     */   }
/*     */ 
/*     */   public void setProvince(Province province) {
/*  52 */     this.province = province;
/*     */   }
/*     */ 
/*     */   public boolean isOccupied() {
/*  56 */     return this.unit != null;
/*     */   }
/*     */ 
/*     */   public void unoccupy() {
/*  60 */     this.unit = null;
/*     */   }
/*     */ 
/*     */   void addAdjacentNode(Node adjNode) {
/*  64 */     this.adjacentNodes[this.adjNodeCount] = adjNode;
/*  65 */     this.adjNodeCount += 1;
/*     */   }
/*     */ 
/*     */   public String getName() {
/*  69 */     return this.name;
/*     */   }
/*     */ 
/*     */   public int getAdjNodeCount() {
/*  73 */     return this.adjNodeCount;
/*     */   }
/*     */ 
/*     */   public Unit getUnit() {
/*  77 */     return this.unit;
/*     */   }
/*     */ 
/*     */   public void setProximityMap(int prox, int val) {
/*  81 */     this.proximityMap[prox] = val;
/*     */   }
/*     */ 
/*     */   public int getProximity(int prox) {
/*  85 */     return this.proximityMap[prox];
/*     */   }
/*     */ 
/*     */   public void setDestValue(String power, int value) {
/*  89 */     this.destValue.put(power, Integer.valueOf(value));
/*     */   }
/*     */ 
/*     */   public int getDestValue(String power) {
/*  93 */     return ((Integer)this.destValue.get(power)).intValue();
/*     */   }
/*     */ 
/*     */   public boolean buildHere() {
/*  97 */     if (this.buildHere) {
/*  98 */       this.buildHere = false;
/*  99 */       return true;
/*     */     }
/* 101 */     return false;
/*     */   }
/*     */ 
/*     */   public void setBuildHere() {
/* 105 */     this.buildHere = true;
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Node
 * JD-Core Version:    0.6.0
 */