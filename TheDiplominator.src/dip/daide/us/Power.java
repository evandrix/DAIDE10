/*     */ package dip.daide.us;
/*     */ 
/*     */ import java.util.ArrayList;
/*     */ import java.util.List;
/*     */ 
/*     */ public class Power
/*     */ {
/*     */   private String name;
/*   9 */   private Province[] ownedSC = new Province[34];
/*  10 */   private int countSC = 0;
/*     */ 
/*  12 */   private Army[] armies = new Army[34];
/*  13 */   private int armyCount = 0;
/*     */ 
/*  15 */   private Fleet[] fleets = new Fleet[34];
/*  16 */   private int fleetCount = 0;
/*     */ 
/*  18 */   private int powerSize = 0;
/*  19 */   private final int M_SIZE_SQUARE_COEFFICIENT = 1;
/*  20 */   private final int M_SIZE_COEFFICIENT = 4;
/*  21 */   private final int M_SIZE_CONSTANT = 16;
/*     */ 
/*  23 */   private final double INITIAL_CREDIT_RATING = 0.8D;
/*  24 */   private final double INTEREST = 1.08D;
/*  25 */   private final int TURNS_TO_REMEMBER_HATRED = 20;
/*  26 */   private final int MIN_BALANCE = -80000000;
/*     */ 
/*  28 */   private int dealsBroken = 0;
/*  29 */   private int dealsKept = 0;
/*  30 */   private int totalDeals = 0;
/*     */ 
/*  32 */   private int balance = 0;
/*     */   private int moneyToGain;
/*  36 */   private RotatingList timesAttackedUs = new RotatingList(20);
/*  37 */   private RotatingList chancesToAttackUs = new RotatingList(20);
/*     */   private double enemyFactor;
/*  40 */   private boolean acceptedPeace = false;
/*  41 */   private boolean isOut = false;
/*  42 */   private boolean noPressBot = false;
/*  43 */   private boolean backstab = false;
/*     */ 
/*  45 */   private List<Province> homes = new ArrayList();
/*     */ 
/*     */   public void setMoneyToGain(int mtg) {
/*  48 */     this.moneyToGain = mtg;
/*     */   }
/*     */ 
/*     */   public double getEnemyFactor() {
/*  52 */     return this.enemyFactor;
/*     */   }
/*     */ 
/*     */   public void increaseChancesToAttack() {
/*  56 */     this.chancesToAttackUs.incrementLastElement();
/*     */   }
/*     */ 
/*     */   public void flushChances() {
/*  60 */     this.chancesToAttackUs.flushChange();
/*     */   }
/*     */ 
/*     */   public void flushAttacks() {
/*  64 */     this.timesAttackedUs.flushChange();
/*     */   }
/*     */ 
/*     */   public void increaseTimesAttackedUs()
/*     */   {
/*  69 */     this.timesAttackedUs.incrementLastElement();
/*     */   }
/*     */ 
/*     */   public void calculateEnemyFactor()
/*     */   {
/*  74 */     double tempChances = this.chancesToAttackUs.getSum();
/*     */ 
/*  76 */     double tempAttacks = this.timesAttackedUs.getSum();
/*     */ 
/*  78 */     this.enemyFactor = (tempChances == 0.0D ? tempAttacks / 1.0D : tempAttacks / tempChances);
/*  79 */     this.acceptedPeace = ((this.acceptedPeace) && (this.enemyFactor <= 0.4D) && (!this.backstab));
/*     */   }
/*     */ 
/*     */   public boolean hasEnoughCredit(int cost)
/*     */   {
/*  85 */     return 1.0D / ((this.dealsKept + 0.8D) / (this.dealsBroken + this.dealsKept + 1)) * cost <= this.balance + 80000000;
/*     */   }
/*     */ 
/*     */   public void makeDeposit(int amount)
/*     */   {
/*  90 */     this.balance = (int)(this.balance + amount * ((this.dealsKept + 0.8D) / (this.dealsBroken + this.dealsKept + 1)));
/*     */   }
/*     */ 
/*     */   public void makeWithdrawal(int amount) {
/*  94 */     this.balance = (int)(this.balance - amount * 1.08D);
/*     */   }
/*     */ 
/*     */   public void dealKept() {
/*  98 */     this.dealsKept += 1;
/*  99 */     this.dealsBroken = (this.totalDeals - this.dealsKept);
/*     */   }
/*     */ 
/*     */   public void increaseDealsMade() {
/* 103 */     this.totalDeals += 1;
/* 104 */     this.dealsBroken = (this.totalDeals - this.dealsKept);
/*     */   }
/*     */ 
/*     */   public double getCreditRating() {
/* 108 */     return (this.dealsKept + 0.8D) / (this.dealsBroken + this.dealsKept + 1);
/*     */   }
/*     */ 
/*     */   public void resetSCs() {
/* 112 */     this.countSC = 0;
/*     */   }
/*     */ 
/*     */   public Power(String name) {
/* 116 */     this.name = name;
/*     */   }
/*     */ 
/*     */   public String getName() {
/* 120 */     return this.name;
/*     */   }
/*     */ 
/*     */   public Army[] getArmies() {
/* 124 */     return this.armies;
/*     */   }
/*     */ 
/*     */   public Fleet[] getFleets() {
/* 128 */     return this.fleets;
/*     */   }
/*     */ 
/*     */   public int getFleetSize() {
/* 132 */     return this.fleetCount;
/*     */   }
/*     */ 
/*     */   public int getArmySize() {
/* 136 */     return this.armyCount;
/*     */   }
/*     */ 
/*     */   public int countSCs() {
/* 140 */     return this.countSC;
/*     */   }
/*     */ 
/*     */   public void addUnit(Unit unit) {
/* 144 */     if (unit.getClass().getName().equals("dip.daide.us.Army")) {
/* 145 */       this.armies[this.armyCount] = ((Army)unit);
/* 146 */       this.armyCount += 1;
/*     */     } else {
/* 148 */       this.fleets[this.fleetCount] = ((Fleet)unit);
/* 149 */       this.fleetCount += 1;
/*     */     }
/*     */   }
/*     */ 
/*     */   public void killAllUnits()
/*     */   {
/* 155 */     this.fleetCount = 0;
/* 156 */     this.armyCount = 0;
/*     */   }
/*     */ 
/*     */   public void addSC(Province sc) {
/* 160 */     this.ownedSC[this.countSC] = sc;
/* 161 */     this.countSC += 1;
/*     */   }
/*     */ 
/*     */   public Province[] ownedSCs() {
/* 165 */     Province[] powerSC = new Province[this.countSC];
/* 166 */     for (int i = 0; i < this.countSC; i++) {
/* 167 */       powerSC[i] = this.ownedSC[i];
/*     */     }
/* 169 */     return powerSC;
/*     */   }
/*     */ 
/*     */   public void calcPower() {
/* 173 */     this.powerSize = (1 * this.countSC * this.countSC + 4 * this.countSC + 16);
/*     */   }
/*     */ 
/*     */   public int getPowerSize() {
/* 177 */     return this.powerSize;
/*     */   }
/*     */ 
/*     */   public void addHome(Province province) {
/* 181 */     this.homes.add(province);
/*     */   }
/*     */ 
/*     */   public List<Province> getHomes()
/*     */   {
/* 186 */     return this.homes;
/*     */   }
/*     */ 
/*     */   public List<Unit> getUnitList() {
/* 190 */     List units = new ArrayList();
/* 191 */     for (int unitCount = 0; unitCount < this.armyCount; unitCount++) {
/* 192 */       units.add(this.armies[unitCount]);
/*     */     }
/* 194 */     for (int unitCount = 0; unitCount < this.fleetCount; unitCount++) {
/* 195 */       units.add(this.fleets[unitCount]);
/*     */     }
/* 197 */     return units;
/*     */   }
/*     */ 
/*     */   public int stabGain(Power retardbot)
/*     */   {
/* 203 */     int stabVulVal = 0;
/*     */ 
/* 205 */     List units = getUnitList();
/*     */ 
/* 207 */     while (!units.isEmpty()) {
/* 208 */       List nodes = ((Unit)units.get(0)).getLocation().getAdjacentNodesList();
/* 209 */       int unitStabability = 0; int noOfAdjNodes = 0;
/* 210 */       Node multipleCoasts = null;
/*     */ 
/* 212 */       while (!nodes.isEmpty()) {
/* 213 */         if ((((Node)nodes.get(0)).getName().substring(4).compareTo("CS") == 0) && (multipleCoasts != null)) {
/* 214 */           if (((Node)nodes.get(0)).getDestValue(this.name) > multipleCoasts.getDestValue(this.name))
/* 215 */             unitStabability += -multipleCoasts.getDestValue(this.name) + ((Node)nodes.get(0)).getDestValue(this.name);
/*     */         }
/*     */         else
/*     */         {
/* 219 */           boolean maybeStabbed = false;
/* 220 */           if ((((Node)nodes.get(0)).getProvince().isSC()) && 
/* 221 */             (((Node)nodes.get(0)).getProvince().ownedBy().equals(retardbot.getName()))) {
/* 222 */             maybeStabbed = true;
/*     */           }
/*     */ 
/* 225 */           if ((((Node)nodes.get(0)).getProvince().isOccupied()) && 
/* 226 */             (((Node)nodes.get(0)).getUnit().getController().getName().equals(retardbot.getName()))) {
/* 227 */             maybeStabbed = true;
/*     */           }
/*     */ 
/* 230 */           if (maybeStabbed) {
/* 231 */             unitStabability += ((Node)nodes.get(0)).getDestValue(this.name);
/* 232 */             noOfAdjNodes++;
/* 233 */             if (((Node)nodes.get(0)).getName().substring(4).equals("CS")) {
/* 234 */               multipleCoasts = (Node)nodes.get(0);
/*     */             }
/*     */           }
/*     */         }
/* 238 */         nodes.remove(0);
/*     */       }
/*     */ 
/* 241 */       stabVulVal += unitStabability / noOfAdjNodes;
/* 242 */       units.remove(0);
/*     */     }
/*     */ 
/* 245 */     return stabVulVal;
/*     */   }
/*     */ 
/*     */   public int stabLoss(Power retardbot)
/*     */   {
/* 251 */     int stabVulVal = 0;
/*     */ 
/* 253 */     List units = retardbot.getUnitList();
/*     */ 
/* 255 */     while (!units.isEmpty()) {
/* 256 */       List nodes = ((Unit)units.get(0)).getLocation().getAdjacentNodesList();
/* 257 */       int unitStabability = 0; int noOfAdjNodes = 0;
/* 258 */       Node multipleCoasts = null;
/*     */ 
/* 260 */       while (!nodes.isEmpty()) {
/* 261 */         if ((((Node)nodes.get(0)).getName().substring(4).compareTo("CS") == 0) && (multipleCoasts != null)) {
/* 262 */           if (((Node)nodes.get(0)).getDestValue(this.name) > multipleCoasts.getDestValue(this.name))
/* 263 */             unitStabability += -multipleCoasts.getDestValue(this.name) + ((Node)nodes.get(0)).getDestValue(this.name);
/*     */         }
/*     */         else
/*     */         {
/* 267 */           boolean maybeStabbed = false;
/* 268 */           if ((((Node)nodes.get(0)).getProvince().isSC()) && 
/* 269 */             (((Node)nodes.get(0)).getProvince().ownedBy().equals(this.name))) {
/* 270 */             maybeStabbed = true;
/*     */           }
/*     */ 
/* 273 */           if ((((Node)nodes.get(0)).getProvince().isOccupied()) && 
/* 274 */             (((Node)nodes.get(0)).getUnit().getController().getName().equals(this.name))) {
/* 275 */             maybeStabbed = true;
/*     */           }
/*     */ 
/* 278 */           if (maybeStabbed) {
/* 279 */             unitStabability += ((Node)nodes.get(0)).getDestValue(this.name);
/* 280 */             noOfAdjNodes++;
/* 281 */             if (((Node)nodes.get(0)).getName().substring(4).equals("CS")) {
/* 282 */               multipleCoasts = (Node)nodes.get(0);
/*     */             }
/*     */           }
/*     */         }
/* 286 */         nodes.remove(0);
/*     */       }
/*     */ 
/* 289 */       stabVulVal += unitStabability / noOfAdjNodes;
/* 290 */       units.remove(0);
/*     */     }
/*     */ 
/* 293 */     return stabVulVal;
/*     */   }
/*     */ 
/*     */   public void setAcceptedPeace() {
/* 297 */     this.acceptedPeace = true;
/*     */   }
/*     */ 
/*     */   public boolean acceptedPeace() {
/* 301 */     return this.acceptedPeace;
/*     */   }
/*     */ 
/*     */   public void setOut() {
/* 305 */     this.isOut = true;
/*     */   }
/*     */ 
/*     */   public boolean isOut() {
/* 309 */     return this.isOut;
/*     */   }
/*     */ 
/*     */   public boolean isNoPress() {
/* 313 */     return this.noPressBot;
/*     */   }
/*     */ 
/*     */   public void setNoPress() {
/* 317 */     this.noPressBot = true;
/*     */   }
/*     */ 
/*     */   public boolean wantToStab() {
/* 321 */     return this.backstab;
/*     */   }
/*     */ 
/*     */   public void setBackstab() {
/* 325 */     this.acceptedPeace = false;
/* 326 */     this.backstab = true;
/*     */   }
/*     */ 
/*     */   public void unSetBackstab() {
/* 330 */     this.backstab = false;
/*     */   }
/*     */ 
/*     */   public int getBalance() {
/* 334 */     return this.balance;
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Power
 * JD-Core Version:    0.6.0
 */