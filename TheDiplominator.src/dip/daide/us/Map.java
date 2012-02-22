/*      */ package dip.daide.us;
/*      */ 
/*      */ import java.io.PrintStream;
/*      */ import java.util.ArrayList;
/*      */ import java.util.Collection;
/*      */ import java.util.Collections;
/*      */ import java.util.Enumeration;
/*      */ import java.util.Hashtable;
/*      */ import java.util.Iterator;
/*      */ import java.util.List;
/*      */ import java.util.Random;
/*      */ 
/*      */ public class Map
/*      */ {
/*   16 */   Hashtable<String, Province> listOfProvinces = new Hashtable();
/*   17 */   Hashtable<String, Power> listOfPowers = new Hashtable();
/*   18 */   Hashtable<String, Node> listOfNodes = new Hashtable();
/*   19 */   private boolean firstTurn = true;
/*      */ 
/*   21 */   private List<String[]> sentProposals = new ArrayList();
/*   22 */   private List<Unit> movesWeAgreed = new ArrayList();
/*   23 */   private List<Unit> listOfNegUnits = new ArrayList();
/*      */   private int season;
/*   26 */   private final int SPR = 0;
/*   27 */   private final int SUM = 1;
/*   28 */   private final int FAL = 2;
/*   29 */   private final int AUT = 3;
/*   30 */   private final int WIN = 4;
/*      */ 
/*   32 */   private final int PCE = 1;
/*   33 */   private final int ALY = 2;
/*   34 */   private final int DMZ = 3;
/*   35 */   private final int XDO = 4;
/*   36 */   private final int DRW = 5;
/*   37 */   private final int SLO = 6;
/*      */ 
/*   39 */   private final int m_prox_spr_att_weight = 700;
/*   40 */   private final int m_prox_spr_def_weight = 200;
/*   41 */   private final int m_prox_fall_att_weight = 600;
/*   42 */   private final int m_prox_fall_def_weight = 300;
/*   43 */   private final int m_spr_str_weight = 1000;
/*   44 */   private final int m_spr_comp_weight = 1000;
/*   45 */   private final int[] m_spr_prox_weight = { 100, 1000, 30, 10, 6, 5, 4, 3, 2, 1 };
/*   46 */   private final int m_fall_str_weight = 1000;
/*   47 */   private final int m_fall_comp_weight = 1000;
/*   48 */   private final int[] m_fall_prox_weight = { 1000, 100, 30, 10, 6, 5, 4, 3, 2, 1 };
/*   49 */   private final int PROXIMITY_DEPTH = 10;
/*   50 */   private final int m_rem_def_weight = 1000;
/*   51 */   private final int m_build_def_weight = 1000;
/*   52 */   private final int[] m_rem_prox_weight = { 1000, 100, 30, 10, 6, 5, 4, 3, 2, 1 };
/*   53 */   private final int[] m_build_prox_weight = { 1000, 100, 30, 10, 6, 5, 4, 3, 2, 1 };
/*   54 */   private final int m_alternative_difference_modifier = 500;
/*   55 */   private final int m_play_alternative = 30;
/*      */   public static final double PCE_THRESHOLD = 0.4D;
/*   59 */   private final int ANGRY = 2;
/*   60 */   private final int NEUTRAL = 1;
/*      */   List<Unit> globalSupUnit;
/*   63 */   private int waives = 0;
/*      */ 
/*      */   public Map(String[] mdf) {
/*   66 */     processMDF(takeRight(mdf, 1));
/*      */   }
/*      */ 
/*      */   public Power getPower(String powerString) {
/*   70 */     return (Power)this.listOfPowers.get(powerString);
/*      */   }
/*      */ 
/*      */   public void updateSCO(String[] sco)
/*      */   {
/*   76 */     killAllSCs();
/*      */ 
/*   79 */     sco = takeRight(sco, 1);
/*      */ 
/*   83 */     while (sco.length > 0) {
/*   84 */       int closingBracket = findClose(sco);
/*   85 */       String[] singlePower = takeLeft(sco, closingBracket);
/*   86 */       Power pandaPower = (Power)this.listOfPowers.get(singlePower[1]);
/*      */ 
/*   89 */       for (int i = 2; i < singlePower.length - 1; i++) {
/*   90 */         Province owned = (Province)this.listOfProvinces.get(singlePower[i]);
/*   91 */         pandaPower.addSC(owned);
/*   92 */         owned.capture(pandaPower);
/*   93 */         if (this.firstTurn) {
/*   94 */           pandaPower.addHome(owned);
/*      */         }
/*      */       }
/*   97 */       pandaPower.calcPower();
/*   98 */       sco = takeRight(sco, closingBracket);
/*      */     }
/*  100 */     this.firstTurn = false;
/*      */   }
/*      */ 
/*      */   private void killAllSCs() {
/*  104 */     for (Iterator i = this.listOfPowers.values().iterator(); i.hasNext(); ) {
/*  105 */       Power current = (Power)i.next();
/*  106 */       current.resetSCs();
/*      */     }
/*      */ 
/*  109 */     for (Iterator i = this.listOfProvinces.values().iterator(); i.hasNext(); ) {
/*  110 */       Province currentProvince = (Province)i.next();
/*  111 */       currentProvince.capture((Power)this.listOfPowers.get("UNO"));
/*      */     }
/*      */   }
/*      */ 
/*      */   private void handleUnit(String[] chunk)
/*      */   {
/*  117 */     Power currentPower = (Power)this.listOfPowers.get(chunk[1]);
/*      */ 
/*  119 */     if (chunk[2].equals("AMY")) {
/*  120 */       Node[] tempNodeArray = ((Province)this.listOfProvinces.get(chunk[3])).getNodeArray();
/*  121 */       Unit currentUnit = new Army(currentPower, tempNodeArray[0]);
/*  122 */       tempNodeArray[0].occupy(currentUnit);
/*  123 */       currentPower.addUnit(currentUnit);
/*      */ 
/*  125 */       if (chunk[4].equals("MRT")) {
/*  126 */         currentUnit.setMRT(updatePossibleRetreats(chunk, currentUnit));
/*      */       }
/*      */ 
/*      */     }
/*  130 */     else if (!chunk[3].equals("("))
/*      */     {
/*  132 */       Node[] tempNodeArray = ((Province)this.listOfProvinces.get(chunk[3])).getNodeArray();
/*      */ 
/*  134 */       Unit currentUnit = new Fleet(currentPower, tempNodeArray[1]);
/*  135 */       tempNodeArray[1].occupy(currentUnit);
/*  136 */       currentPower.addUnit(currentUnit);
/*  137 */       if (chunk[4].equals("MRT"))
/*  138 */         currentUnit.setMRT(updatePossibleRetreats(chunk, currentUnit));
/*      */     }
/*      */     else
/*      */     {
/*  142 */       Node currentNode = (Node)this.listOfNodes.get(chunk[4] + chunk[5]);
/*  143 */       Unit currentUnit = new Fleet(currentPower, currentNode);
/*      */ 
/*  145 */       currentNode.occupy(currentUnit);
/*  146 */       currentPower.addUnit(currentUnit);
/*  147 */       if (chunk[7].equals("MRT"))
/*  148 */         currentUnit.setMRT(updatePossibleRetreats(chunk, currentUnit));
/*      */     }
/*      */   }
/*      */ 
/*      */   private List<Node> updatePossibleRetreats(String[] chunk, Unit currentUnit)
/*      */   {
/*  158 */     List retreatNodes = new ArrayList();
/*      */ 
/*  160 */     if (currentUnit.getUnitType().equals("AMY")) {
/*  161 */       chunk = takeRight(chunk, 5);
/*  162 */       int closingBracket = findClose(chunk);
/*  163 */       if (closingBracket != 1)
/*      */       {
/*  165 */         for (int i = 1; i < closingBracket - 1; i++)
/*      */         {
/*  167 */           retreatNodes.add(((Province)this.listOfProvinces.get(chunk[i])).getNodeArray()[0]);
/*      */         }
/*      */       }
/*      */     }
/*  171 */     else if (chunk[3].equals("("))
/*      */     {
/*  173 */       chunk = takeRight(chunk, 8);
/*  174 */       int closingBracket = findClose(chunk);
/*  175 */       if (closingBracket != 1)
/*      */       {
/*  177 */         for (int i = 1; i < closingBracket - 1; i++)
/*  178 */           if (chunk[i].equals("("))
/*      */           {
/*  180 */             retreatNodes.add((Node)this.listOfNodes.get(chunk[(i + 1)] + chunk[(i + 2)]));
/*  181 */             i += 3; } else {
/*  182 */             retreatNodes.add(((Province)this.listOfProvinces.get(chunk[i])).getNodeArray()[1]);
/*      */           }
/*      */       }
/*      */     }
/*      */     else {
/*  187 */       chunk = takeRight(chunk, 5);
/*  188 */       int closingBracket = findClose(chunk);
/*  189 */       if (closingBracket != 1)
/*      */       {
/*  191 */         for (int i = 1; i < closingBracket - 1; i++) {
/*  192 */           if (chunk[i].equals("("))
/*      */           {
/*  194 */             retreatNodes.add((Node)this.listOfNodes.get(chunk[(i + 1)] + chunk[(i + 2)]));
/*  195 */             i += 3; } else {
/*  196 */             retreatNodes.add(((Province)this.listOfProvinces.get(chunk[i])).getNodeArray()[1]);
/*      */           }
/*      */         }
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/*  203 */     return retreatNodes;
/*      */   }
/*      */ 
/*      */   private void createPowers(String[] powers)
/*      */   {
/*  208 */     for (int i = 1; !powers[i].equals(")"); i++) {
/*  209 */       this.listOfPowers.put(powers[i], new Power(powers[i]));
/*      */     }
/*      */ 
/*  212 */     this.listOfPowers.put("UNO", new Power("UNO"));
/*      */   }
/*      */ 
/*      */   private void processMDF(String[] tokens)
/*      */   {
/*  217 */     int powerEnds = findClose(tokens);
/*      */ 
/*  219 */     String[] powers = takeLeft(tokens, powerEnds);
/*  220 */     String[] provAndAdj = takeRight(tokens, powerEnds);
/*      */ 
/*  222 */     int provinceEnds = findClose(provAndAdj);
/*      */ 
/*  224 */     String[] provinces = takeLeft(provAndAdj, provinceEnds);
/*  225 */     String[] adjacencies = takeRight(provAndAdj, provinceEnds);
/*      */ 
/*  227 */     createPowers(powers);
/*  228 */     createProvinces(provinces);
/*  229 */     createNodes(adjacencies);
/*  230 */     processAdjacencies(adjacencies);
/*      */   }
/*      */ 
/*      */   private void processAdjacencies(String[] adjacencies)
/*      */   {
/*  238 */     adjacencies = takeRight(adjacencies, 1);
/*  239 */     int provinceClosingBracket = findClose(adjacencies);
/*      */ 
/*  241 */     while (provinceClosingBracket != -1)
/*      */     {
/*  243 */       processProvinceAdjacencies(takeLeft(adjacencies, provinceClosingBracket));
/*      */ 
/*  245 */       adjacencies = takeRight(adjacencies, provinceClosingBracket);
/*  246 */       provinceClosingBracket = findClose(adjacencies);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void processProvinceAdjacencies(String[] provinceAdjacencies)
/*      */   {
/*  254 */     Province province = (Province)this.listOfProvinces.get(provinceAdjacencies[1]);
/*      */ 
/*  257 */     provinceAdjacencies = takeRight(provinceAdjacencies, 2);
/*  258 */     int movementClosingBracket = findClose(provinceAdjacencies);
/*      */ 
/*  260 */     while (movementClosingBracket != -1)
/*      */     {
/*  266 */       int i = 2;
/*      */       Node workingNode;
/*      */       Node workingNode;
/*  267 */       if (provinceAdjacencies[1].equals("AMY")) {
/*  268 */         workingNode = (Node)this.listOfNodes.get(province.getName() + "AMY");
/*      */       }
/*      */       else
/*      */       {
/*      */         Node workingNode;
/*  269 */         if (provinceAdjacencies[1].equals("FLT")) {
/*  270 */           workingNode = (Node)this.listOfNodes.get(province.getName() + "FLT");
/*      */         } else {
/*  272 */           workingNode = (Node)this.listOfNodes.get(province.getName() + provinceAdjacencies[3]);
/*  273 */           i = 5;
/*      */         }
/*      */       }
/*      */ 
/*  277 */       for (; i < movementClosingBracket - 1; i++) {
/*  278 */         if (provinceAdjacencies[i].equals("(")) {
/*  279 */           workingNode.addAdjacentNode((Node)this.listOfNodes.get(provinceAdjacencies[(i + 1)] + provinceAdjacencies[(i + 2)]));
/*      */ 
/*  281 */           i += 3;
/*      */         } else {
/*  283 */           String nodeName = provinceAdjacencies[i] + "FLT";
/*      */ 
/*  285 */           workingNode.addAdjacentNode((Node)this.listOfNodes.get(nodeName));
/*      */         }
/*      */       }
/*      */ 
/*  289 */       provinceAdjacencies = takeRight(provinceAdjacencies, movementClosingBracket);
/*  290 */       movementClosingBracket = findClose(provinceAdjacencies);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void createProvinces(String[] provinces)
/*      */   {
/*  298 */     provinces = takeRight(provinces, 1);
/*  299 */     int closingBracket = findClose(provinces);
/*      */ 
/*  301 */     String[] supplyCentres = takeLeft(provinces, closingBracket);
/*  302 */     String[] nonSupplyCentres = takeRight(provinces, closingBracket);
/*      */ 
/*  304 */     createSupplyCentres(supplyCentres);
/*  305 */     createNonSupplyCentres(nonSupplyCentres);
/*      */   }
/*      */ 
/*      */   private void createNonSupplyCentres(String[] nonSupplyCentres)
/*      */   {
/*  312 */     int closingBracket = findClose(nonSupplyCentres);
/*      */ 
/*  317 */     for (int i = 1; i < closingBracket - 1; i++) {
/*  318 */       Province newProvince = new Province(nonSupplyCentres[i]);
/*  319 */       this.listOfProvinces.put(nonSupplyCentres[i], newProvince);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void createSupplyCentres(String[] supplyCentres)
/*      */   {
/*  328 */     supplyCentres = takeRight(supplyCentres, 1);
/*  329 */     int closingBracket = findClose(supplyCentres);
/*      */ 
/*  332 */     while (closingBracket != -1) {
/*  333 */       String[] singlePower = takeLeft(supplyCentres, closingBracket);
/*  334 */       Power tempPower = (Power)this.listOfPowers.get(singlePower[1]);
/*      */ 
/*  336 */       for (int i = 2; i < singlePower.length - 1; i++)
/*      */       {
/*  338 */         Province owned = new Province(singlePower[i]);
/*  339 */         tempPower.addSC(owned);
/*  340 */         owned.capture(tempPower);
/*  341 */         owned.setSupplyCentre();
/*  342 */         this.listOfProvinces.put(singlePower[i], owned);
/*      */       }
/*  344 */       supplyCentres = takeRight(supplyCentres, closingBracket);
/*  345 */       closingBracket = findClose(supplyCentres);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void createNodes(String[] adjacencies)
/*      */   {
/*  353 */     String[] inputRemaining = takeRight(adjacencies, 1);
/*  354 */     int closingBracket = findClose(inputRemaining);
/*      */ 
/*  356 */     while (closingBracket != -1)
/*      */     {
/*  358 */       Province workingProvince = (Province)this.listOfProvinces.get(inputRemaining[1]);
/*      */ 
/*  360 */       String[] twoCoasts = new String[2];
/*  361 */       twoCoasts = hasCoasts(inputRemaining);
/*      */ 
/*  363 */       if (twoCoasts[0] == null) {
/*  364 */         if (isCoastal(inputRemaining)) {
/*  365 */           String node0str = inputRemaining[1] + "AMY";
/*  366 */           String node1str = inputRemaining[1] + "FLT";
/*      */ 
/*  368 */           Node node0 = new Node(node0str);
/*  369 */           Node node1 = new Node(node1str);
/*      */ 
/*  371 */           this.listOfNodes.put(node0str, node0);
/*  372 */           this.listOfNodes.put(node1str, node1);
/*      */ 
/*  374 */           node0.setProvince(workingProvince);
/*  375 */           node1.setProvince(workingProvince);
/*      */ 
/*  377 */           workingProvince.setNode0(node0);
/*  378 */           workingProvince.setNode1(node1);
/*      */         }
/*  381 */         else if (inputRemaining[3].equals("AMY")) {
/*  382 */           Node node0 = new Node(inputRemaining[1] + "AMY");
/*  383 */           this.listOfNodes.put(inputRemaining[1] + "AMY", node0);
/*  384 */           node0.setProvince(workingProvince);
/*  385 */           workingProvince.setNode0(node0);
/*      */         } else {
/*  387 */           Node node1 = new Node(inputRemaining[1] + "FLT");
/*  388 */           this.listOfNodes.put(inputRemaining[1] + "FLT", node1);
/*  389 */           node1.setProvince(workingProvince);
/*  390 */           workingProvince.setNode1(node1);
/*      */         }
/*      */ 
/*      */       }
/*      */       else
/*      */       {
/*  397 */         String node0str = inputRemaining[1] + "AMY";
/*  398 */         String node1str = inputRemaining[1] + twoCoasts[0];
/*  399 */         String node2str = inputRemaining[1] + twoCoasts[1];
/*      */ 
/*  401 */         Node node0 = new Node(node0str);
/*  402 */         Node node1 = new Node(node1str);
/*  403 */         Node node2 = new Node(node2str);
/*      */ 
/*  405 */         this.listOfNodes.put(node0str, node0);
/*  406 */         this.listOfNodes.put(node1str, node1);
/*  407 */         this.listOfNodes.put(node2str, node2);
/*      */ 
/*  409 */         node0.setProvince(workingProvince);
/*  410 */         node1.setProvince(workingProvince);
/*  411 */         node2.setProvince(workingProvince);
/*      */ 
/*  413 */         workingProvince.setNode0(node0);
/*  414 */         workingProvince.setNode1(node1);
/*  415 */         workingProvince.setNode2(node2);
/*      */       }
/*      */ 
/*  419 */       inputRemaining = takeRight(inputRemaining, closingBracket);
/*  420 */       closingBracket = findClose(inputRemaining);
/*      */     }
/*      */   }
/*      */ 
/*      */   private boolean isCoastal(String[] inputRemaining)
/*      */   {
/*  428 */     int closingBracket = findClose(inputRemaining);
/*  429 */     String[] testProvince = takeLeft(inputRemaining, closingBracket);
/*  430 */     int i = 0;
/*      */ 
/*  432 */     for (i = 0; i < testProvince.length - 1; i++) {
/*  433 */       if ((testProvince[i].equals(")")) && (testProvince[(i + 1)].equals("("))) {
/*  434 */         return true;
/*      */       }
/*      */     }
/*      */ 
/*  438 */     return false;
/*      */   }
/*      */ 
/*      */   private String[] hasCoasts(String[] inputRemaining) {
/*  442 */     int closingBracket = findClose(inputRemaining);
/*  443 */     String[] testProvince = takeLeft(inputRemaining, closingBracket);
/*  444 */     int i = 0;
/*  445 */     String[] coasts = new String[2];
/*      */ 
/*  447 */     for (i = 0; i < testProvince.length - 2; i++) {
/*  448 */       if ((testProvince[i].equals("(")) && (testProvince[(i + 1)].equals("(")) && (testProvince[(i + 2)].equals("FLT"))) {
/*  449 */         if (coasts[0] == null) coasts[0] = testProvince[(i + 3)]; else {
/*  450 */           coasts[1] = testProvince[(i + 3)];
/*      */         }
/*      */       }
/*      */     }
/*      */ 
/*  455 */     return coasts;
/*      */   }
/*      */ 
/*      */   private void printMessage(String[] message) {
/*  459 */     for (int i = 0; i < message.length; i++) {
/*  460 */       System.out.print(message[i] + " ");
/*      */     }
/*  462 */     System.out.println();
/*      */   }
/*      */ 
/*      */   private String[] takeLeft(String[] tokens, int end) {
/*  466 */     int i = 0;
/*  467 */     String[] left = new String[end];
/*  468 */     for (i = 0; i < end; i++) {
/*  469 */       left[i] = tokens[i];
/*      */     }
/*      */ 
/*  472 */     return left;
/*      */   }
/*      */ 
/*      */   private String[] takeRight(String[] tokens, int start)
/*      */   {
/*  479 */     int length = tokens.length;
/*  480 */     String[] right = new String[length - start];
/*      */ 
/*  482 */     for (int i = 0; i < length - start; i++) {
/*  483 */       right[i] = tokens[(start + i)];
/*      */     }
/*      */ 
/*  486 */     return right;
/*      */   }
/*      */ 
/*      */   private int findClose(String[] tokens) {
/*  490 */     int bracketCount = 0;
/*  491 */     int i = 0;
/*      */ 
/*  493 */     if (!tokens[0].equals("(")) return -1;
/*      */ 
/*  495 */     for (i = 0; i < tokens.length; i++)
/*      */     {
/*  497 */       if (tokens[i].equals("(")) bracketCount++;
/*  498 */       if (tokens[i].equals(")")) bracketCount--;
/*  499 */       if (bracketCount == 0) return i + 1;
/*      */     }
/*      */ 
/*  502 */     return -1;
/*      */   }
/*      */ 
/*      */   private void killAllUnits() {
/*  506 */     for (Iterator i = this.listOfNodes.values().iterator(); i.hasNext(); ) {
/*  507 */       Node current = (Node)i.next();
/*  508 */       current.unoccupy();
/*      */     }
/*  510 */     for (Iterator i = this.listOfPowers.values().iterator(); i.hasNext(); ) {
/*  511 */       Power current = (Power)i.next();
/*  512 */       current.killAllUnits();
/*      */     }
/*      */   }
/*      */ 
/*      */   public void updateUnits(String[] message)
/*      */   {
/*  521 */     killAllUnits();
/*      */ 
/*  523 */     message = takeRight(message, 5);
/*      */ 
/*  527 */     while (message.length > 0) {
/*  528 */       int closingBracket = findClose(message);
/*  529 */       handleUnit(takeLeft(message, closingBracket));
/*  530 */       message = takeRight(message, closingBracket);
/*      */     }
/*      */   }
/*      */ 
/*      */   public void storeSeason(String curSeason)
/*      */   {
/*  536 */     if (curSeason.compareTo("SPR") == 0) this.season = 0;
/*  537 */     else if (curSeason.compareTo("SUM") == 0) this.season = 1;
/*  538 */     else if (curSeason.compareTo("FAL") == 0) this.season = 2;
/*  539 */     else if (curSeason.compareTo("AUT") == 0) this.season = 3; else
/*  540 */       this.season = 4;
/*      */   }
/*      */ 
/*      */   public int getSeason() {
/*  544 */     return this.season;
/*      */   }
/*      */ 
/*      */   public List<String[]> processNOW(Power me) {
/*  548 */     if (this.season == 1)
/*      */     {
/*  550 */       calcFactors(me, 700, 200);
/*  551 */       initStrCompValues(me);
/*  552 */       calcDestValue(me, this.m_spr_prox_weight, 1000, 1000);
/*      */     }
/*  554 */     else if (this.season == 3)
/*      */     {
/*  556 */       calcFactors(me, 600, 300);
/*  557 */       initStrCompValues(me);
/*  558 */       calcDestValue(me, this.m_fall_prox_weight, 1000, 1000);
/*      */     }
/*      */     else
/*      */     {
/*  562 */       calcFactors(me, 700, 200);
/*  563 */       initStrCompValues(me);
/*  564 */       if (me.getArmySize() + me.getFleetSize() > me.countSCs())
/*      */       {
/*  566 */         calcWinDestVal(me, this.m_rem_prox_weight, 1000);
/*      */       }
/*      */       else
/*      */       {
/*  570 */         calcWinDestVal(me, this.m_build_prox_weight, 1000);
/*      */       }
/*      */     }
/*      */ 
/*  574 */     if ((this.season == 1) || (this.season == 3)) {
/*  575 */       genRetreatOrders(me);
/*      */     }
/*  578 */     else if (me.getArmySize() + me.getFleetSize() > me.countSCs()) {
/*  579 */       genRemoveOrders(me, me.getArmySize() + me.getFleetSize() - me.countSCs());
/*      */     }
/*      */     else {
/*  582 */       genBuildOrders(me, me.countSCs() - me.getArmySize() - me.getFleetSize());
/*      */     }
/*      */ 
/*  586 */     return submitOrders(me);
/*      */   }
/*      */ 
/*      */   public void processFactors(Power me) {
/*  590 */     if (this.season == 0)
/*      */     {
/*  592 */       calcFactors(me, 700, 200);
/*      */     }
/*      */     else
/*      */     {
/*  596 */       calcFactors(me, 600, 300);
/*      */     }
/*      */   }
/*      */ 
/*      */   public void calcFactors(Power me, int prox_att_weight, int prox_def_weight)
/*      */   {
/*  616 */     Enumeration provinceIterator = this.listOfProvinces.elements();
/*      */ 
/*  620 */     while (provinceIterator.hasMoreElements()) {
/*  621 */       Province province = (Province)provinceIterator.nextElement();
/*  622 */       province.resetAttVal();
/*  623 */       province.resetDefVal();
/*      */     }
/*      */ 
/*  627 */     provinceIterator = this.listOfProvinces.elements();
/*  628 */     while (provinceIterator.hasMoreElements()) {
/*  629 */       Province province = (Province)provinceIterator.nextElement();
/*      */ 
/*  631 */       province.resetBeingMovedTo();
/*      */ 
/*  633 */       if (!province.isSC())
/*      */         continue;
/*  635 */       if (province.ownedBy() == me) {
/*  636 */         province.calcDefVal(me);
/*      */       }
/*      */       else
/*      */       {
/*  640 */         province.setAttVal(province.ownedBy().getPowerSize());
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/*  646 */     provinceIterator = this.listOfProvinces.elements();
/*  647 */     while (provinceIterator.hasMoreElements()) {
/*  648 */       Province province = (Province)provinceIterator.nextElement();
/*  649 */       List nodes = province.getNodeList();
/*      */ 
/*  651 */       while (!nodes.isEmpty())
/*      */       {
/*  653 */         ((Node)nodes.get(0)).setProximityMap(0, province.getAttVal() * prox_att_weight + province.getDefVal() * prox_def_weight);
/*  654 */         for (int proxCount = 1; proxCount < 10; proxCount++)
/*      */         {
/*  656 */           ((Node)nodes.get(0)).setProximityMap(proxCount, 0);
/*      */         }
/*  658 */         nodes.remove(0);
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/*  668 */     for (int proxCount = 1; proxCount < 10; proxCount++)
/*      */     {
/*  670 */       provinceIterator = this.listOfProvinces.elements();
/*  671 */       while (provinceIterator.hasMoreElements()) {
/*  672 */         Province province = (Province)provinceIterator.nextElement();
/*  673 */         List nodes = province.getNodeList();
/*      */ 
/*  675 */         while (!nodes.isEmpty()) {
/*  676 */           ((Node)nodes.get(0)).setProximityMap(proxCount, ((Node)nodes.get(0)).getProximity(proxCount - 1));
/*  677 */           List adjacentNodes = ((Node)nodes.get(0)).getAdjacentNodesList();
/*  678 */           Node multipleCoasts = null;
/*      */ 
/*  680 */           while (!adjacentNodes.isEmpty())
/*      */           {
/*  682 */             if ((((Node)adjacentNodes.get(0)).getName().substring(4).compareTo("CS") == 0) && (multipleCoasts != null))
/*      */             {
/*  684 */               if (((Node)adjacentNodes.get(0)).getProximity(proxCount - 1) > multipleCoasts.getProximity(proxCount - 1)) {
/*  685 */                 ((Node)nodes.get(0)).setProximityMap(proxCount, ((Node)nodes.get(0)).getProximity(proxCount) - multipleCoasts.getProximity(proxCount - 1) + ((Node)adjacentNodes.get(0)).getProximity(proxCount - 1));
/*      */               }
/*      */             }
/*      */             else
/*      */             {
/*  690 */               ((Node)nodes.get(0)).setProximityMap(proxCount, ((Node)nodes.get(0)).getProximity(proxCount) + ((Node)adjacentNodes.get(0)).getProximity(proxCount - 1));
/*      */ 
/*  692 */               if (((Node)adjacentNodes.get(0)).getName().substring(4).compareTo("CS") == 0) {
/*  693 */                 multipleCoasts = (Node)adjacentNodes.get(0);
/*      */               }
/*      */             }
/*  696 */             adjacentNodes.remove(0);
/*      */           }
/*  698 */           ((Node)nodes.get(0)).setProximityMap(proxCount, ((Node)nodes.get(0)).getProximity(proxCount) / 5);
/*      */ 
/*  700 */           nodes.remove(0);
/*      */         }
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   public void initStrCompValues(Power me)
/*      */   {
/*  709 */     Enumeration provinceIterator = this.listOfProvinces.elements();
/*  710 */     while (provinceIterator.hasMoreElements()) {
/*  711 */       Province province = (Province)provinceIterator.nextElement();
/*  712 */       province.setStrengthValue(0);
/*  713 */       province.setCompValue(0);
/*      */ 
/*  716 */       Hashtable adjUnitCount = new Hashtable();
/*  717 */       Enumeration powerIterator = this.listOfPowers.elements();
/*  718 */       while (powerIterator.hasMoreElements()) {
/*  719 */         Power power = (Power)powerIterator.nextElement();
/*  720 */         adjUnitCount.put(power.getName(), Integer.valueOf(0));
/*      */       }
/*      */ 
/*  723 */       List adjProvinces = province.getAdjacentProvinces();
/*      */ 
/*  726 */       List nodes = province.getNodeList();
/*  727 */       while (!nodes.isEmpty()) {
/*  728 */         adjProvinces.add((Node)nodes.get(0));
/*  729 */         nodes.remove(0);
/*      */       }
/*      */ 
/*  732 */       while (!adjProvinces.isEmpty()) {
/*  733 */         if (((Node)adjProvinces.get(0)).isOccupied()) {
/*  734 */           adjUnitCount.put(((Node)adjProvinces.get(0)).getUnit().getController().getName(), Integer.valueOf(((Integer)adjUnitCount.get(((Node)adjProvinces.get(0)).getUnit().getController().getName())).intValue() + 1));
/*      */         }
/*  736 */         adjProvinces.remove(0);
/*      */       }
/*      */ 
/*  739 */       powerIterator = this.listOfPowers.elements();
/*  740 */       while (powerIterator.hasMoreElements()) {
/*  741 */         Power power = (Power)powerIterator.nextElement();
/*  742 */         if (power == me) {
/*  743 */           province.setStrengthValue(((Integer)adjUnitCount.get(me.getName())).intValue());
/*      */         }
/*  745 */         else if (((Integer)adjUnitCount.get(power.getName())).intValue() > province.getCompValue()) {
/*  746 */           province.setCompValue(((Integer)adjUnitCount.get(power.getName())).intValue());
/*      */         }
/*      */       }
/*  749 */       province.setAdjUnitCount(adjUnitCount);
/*      */     }
/*      */   }
/*      */ 
/*      */   public void calcDest(Power me) {
/*  754 */     if (this.season == 0)
/*      */     {
/*  756 */       calcDestValue(me, this.m_spr_prox_weight, 1000, 1000);
/*      */     }
/*      */     else
/*      */     {
/*  760 */       calcDestValue(me, this.m_fall_prox_weight, 1000, 1000);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void calcDestValue(Power me, int[] proxWeight, int strWeight, int compWeight)
/*      */   {
/*  769 */     Enumeration nodeIterator = this.listOfNodes.elements();
/*  770 */     while (nodeIterator.hasMoreElements()) {
/*  771 */       Node node = (Node)nodeIterator.nextElement();
/*  772 */       int destWeight = 0;
/*      */ 
/*  774 */       for (int proxCount = 0; proxCount < 10; proxCount++) {
/*  775 */         destWeight += node.getProximity(proxCount) * proxWeight[proxCount];
/*      */       }
/*      */ 
/*  778 */       destWeight += strWeight * node.getProvince().getStrengthValue();
/*  779 */       destWeight -= compWeight * node.getProvince().getCompValue();
/*      */ 
/*  781 */       if ((node.isOccupied()) && 
/*  782 */         (node.getUnit().getController() != me)) destWeight = (int)(destWeight * (node.getUnit().getController().acceptedPeace() ? 0.4D : 1.0D));
/*      */ 
/*  788 */       if ((node.isOccupied()) && 
/*  789 */         (node.getUnit().getController() != me)) destWeight = (int)(destWeight * (node.getUnit().getController().wantToStab() ? 1.5D : 1.0D));
/*      */ 
/*  791 */       if ((node.getProvince().isSC()) && 
/*  792 */         (node.getProvince().ownedBy() != me)) destWeight = (int)(destWeight * (node.getProvince().ownedBy().wantToStab() ? 1.5D : 1.0D));
/*      */ 
/*  798 */       node.setDestValue(me.getName(), destWeight);
/*      */     }
/*      */   }
/*      */ 
/*      */   public void calcWinDestVal(Power me, int[] proxWeight, int defWeight)
/*      */   {
/*  804 */     Enumeration nodeIterator = this.listOfNodes.elements();
/*  805 */     while (nodeIterator.hasMoreElements()) {
/*  806 */       Node node = (Node)nodeIterator.nextElement();
/*  807 */       int destWeight = 0;
/*      */ 
/*  809 */       for (int proxCount = 0; proxCount < 10; proxCount++) {
/*  810 */         destWeight += node.getProximity(proxCount) * proxWeight[proxCount];
/*      */       }
/*      */ 
/*  813 */       destWeight += defWeight * node.getProvince().getDefVal();
/*  814 */       node.setDestValue(me.getName(), destWeight);
/*      */     }
/*      */   }
/*      */ 
/*      */   public void genMoveOrders(Power me) {
/*  819 */     List units = genRandomUnitList(me);
/*      */ 
/*  825 */     while (!units.isEmpty()) {
/*  826 */       Unit currentUnit = (Unit)units.get(0);
/*      */ 
/*  828 */       if (this.movesWeAgreed.contains(currentUnit)) {
/*  829 */         units.remove(0);
/*      */       } else {
/*  834 */         List destNodes = currentUnit.getLocation().getAdjacentNodesList();
/*  835 */         destNodes.add(currentUnit.getLocation());
/*  836 */         destNodes = sortNodeListByDest(me, destNodes);
/*      */         boolean selectionIsOK;
/*      */         do {
/*  840 */           Node currentNode = (Node)destNodes.get(0);
/*  841 */           boolean tryNextNode = true;
/*  842 */           int provCount = 1;
/*  843 */           while (tryNextNode) {
/*  844 */             if (provCount < destNodes.size()) {
/*  845 */               Node nextNode = (Node)destNodes.get(provCount++);
/*      */               int nextNodeChance;
/*      */               int nextNodeChance;
/*  846 */               if (currentNode.getDestValue(me.getName()) == 0) {
/*  847 */                 nextNodeChance = 0;
/*      */               }
/*      */               else {
/*  850 */                 nextNodeChance = (currentNode.getDestValue(me.getName()) - nextNode.getDestValue(me.getName())) * 500 / currentNode.getDestValue(me.getName());
/*      */               }
/*  852 */               if ((randNo(100) < 30) && (randNo(100) >= nextNodeChance)) {
/*  853 */                 currentNode = nextNode;
/*      */               }
/*      */               else
/*  856 */                 tryNextNode = false;
/*      */             }
/*      */             else
/*      */             {
/*  860 */               tryNextNode = false;
/*      */             }
/*      */ 
/*      */           }
/*      */ 
/*  865 */           selectionIsOK = true;
/*  866 */           boolean orderUnitToMove = true;
/*      */ 
/*  869 */           if (currentUnit.getLocation().getName().compareTo(currentNode.getName()) == 0) {
/*  870 */             currentUnit.setHold();
/*      */           }
/*      */           else
/*      */           {
/*  874 */             if ((currentNode.getProvince().isOccupied()) && 
/*  875 */               (currentNode.getProvince().getUnit().getController().getName().compareTo(me.getName()) == 0))
/*      */             {
/*  877 */               if (currentNode.getProvince().getUnit().getOrderToken() == null)
/*      */               {
/*  880 */                 units.add(units.indexOf(currentNode.getProvince().getUnit()) + 1, currentUnit);
/*  881 */                 orderUnitToMove = false;
/*      */               }
/*  884 */               else if ((currentNode.getProvince().getUnit().getOrderToken().compareTo("MTO") != 0) && (currentNode.getProvince().getUnit().getOrderToken().compareTo("CTO") != 0))
/*      */               {
/*  886 */                 if (currentNode.getProvince().getCompValue() > 0)
/*      */                 {
/*  888 */                   currentUnit.setSupportHold(currentNode.getProvince().getUnit());
/*  889 */                   orderUnitToMove = false;
/*      */                 }
/*      */                 else
/*      */                 {
/*  893 */                   selectionIsOK = false;
/*  894 */                   destNodes.remove(currentNode);
/*      */                 }
/*      */               }
/*      */ 
/*      */             }
/*      */ 
/*  900 */             if (currentNode.getProvince().isBeingMovedTo())
/*      */             {
/*  902 */               if (currentNode.getProvince().getCompValue() > 0) {
/*  903 */                 currentUnit.setSupportMove(currentNode.getProvince().getUnitBeingMovedTo(), currentNode);
/*  904 */                 orderUnitToMove = false;
/*      */               }
/*      */               else
/*      */               {
/*  908 */                 selectionIsOK = false;
/*  909 */                 destNodes.remove(currentNode);
/*      */               }
/*      */             }
/*      */ 
/*  913 */             if ((selectionIsOK) && (orderUnitToMove))
/*  914 */               currentUnit.setMove(currentNode);
/*      */           }
/*      */         }
/*  839 */         while (!
/*  917 */           selectionIsOK);
/*      */ 
/*  920 */         units.remove(0);
/*      */       }
/*      */     }
/*  923 */     checkForWastedHolds(me);
/*      */   }
/*      */ 
/*      */   private void checkForWastedHolds(Power me) {
/*  927 */     List units = genRandomUnitList(me);
/*  928 */     while (!units.isEmpty()) {
/*  929 */       Unit currentUnit = (Unit)units.get(0);
/*  930 */       if (this.movesWeAgreed.contains(currentUnit)) {
/*  931 */         units.remove(0);
/*      */       }
/*      */       else {
/*  934 */         Node destination = null;
/*  935 */         Unit unitSupported = null;
/*      */ 
/*  937 */         if (currentUnit.getOrderToken().compareTo("HLD") == 0)
/*      */         {
/*  939 */           int maxDestValue = 0;
/*  940 */           List destNodes = currentUnit.getLocation().getAdjacentNodesList();
/*  941 */           destNodes = sortNodeListByDest(me, destNodes);
/*      */ 
/*  943 */           while (!destNodes.isEmpty()) {
/*  944 */             Node currentNodes = (Node)destNodes.get(0);
/*      */ 
/*  946 */             if (currentNodes.getProvince().isBeingMovedTo())
/*      */             {
/*  948 */               if (currentNodes.getProvince().getCompValue() > 0)
/*      */               {
/*  950 */                 if (currentNodes.getDestValue(me.getName()) > maxDestValue) {
/*  951 */                   maxDestValue = currentNodes.getDestValue(me.getName());
/*  952 */                   destination = currentNodes;
/*  953 */                   unitSupported = currentNodes.getProvince().getUnitBeingMovedTo();
/*      */                 }
/*      */ 
/*      */               }
/*      */ 
/*      */             }
/*  959 */             else if ((currentNodes.isOccupied()) && 
/*  960 */               (currentNodes.getUnit().getController().getName().compareTo(me.getName()) == 0) && 
/*  961 */               (currentNodes.getUnit().getOrderToken().compareTo("MTO") != 0) && (currentNodes.getUnit().getOrderToken().compareTo("CTO") != 0))
/*      */             {
/*  963 */               if (currentNodes.getProvince().getCompValue() > 1)
/*      */               {
/*  965 */                 if (currentNodes.getDestValue(me.getName()) > maxDestValue) {
/*  966 */                   maxDestValue = currentNodes.getDestValue(me.getName());
/*  967 */                   destination = currentNodes;
/*  968 */                   unitSupported = currentNodes.getProvince().getUnit();
/*      */                 }
/*      */ 
/*      */               }
/*      */ 
/*      */             }
/*      */ 
/*  976 */             if (maxDestValue > 0) {
/*  977 */               if ((unitSupported.getOrderToken().compareTo("MTO") != 0) && (unitSupported.getOrderToken().compareTo("CTO") != 0)) {
/*  978 */                 currentUnit.setSupportHold(unitSupported);
/*      */               }
/*      */               else {
/*  981 */                 currentUnit.setSupportMove(unitSupported, destination);
/*      */               }
/*      */             }
/*  984 */             destNodes.remove(0);
/*      */           }
/*      */         }
/*  987 */         units.remove(0);
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   private void genRetreatOrders(Power me)
/*      */   {
/*  996 */     Army[] ourArmies = new Army[me.getArmySize()];
/*  997 */     for (int i = 0; i < me.getArmySize(); i++) {
/*  998 */       ourArmies[i] = me.getArmies()[i];
/*      */     }
/*      */ 
/* 1001 */     Fleet[] ourFleets = new Fleet[me.getFleetSize()];
/* 1002 */     for (int i = 0; i < me.getFleetSize(); i++) {
/* 1003 */       ourFleets[i] = me.getFleets()[i];
/*      */     }
/*      */ 
/* 1009 */     List units = genRandomRETUnitList(me);
/*      */ 
/* 1013 */     while (!units.isEmpty()) {
/* 1014 */       Unit currentUnit = (Unit)units.get(0);
/* 1015 */       if (currentUnit.mustRetreat()) {
/* 1016 */         List destNodes = sortNodeListByDest(me, currentUnit.getMRTList());
/* 1017 */         boolean selectionIsOK = true;
/*      */         do
/* 1019 */           if (destNodes.isEmpty()) {
/* 1020 */             currentUnit.setDisband();
/* 1021 */             selectionIsOK = true;
/*      */           }
/*      */           else {
/* 1024 */             Node currentNode = (Node)destNodes.get(0);
/* 1025 */             boolean tryNextNode = true;
/* 1026 */             int nodeCount = 1;
/* 1027 */             while (tryNextNode) {
/* 1028 */               if (nodeCount < destNodes.size()) {
/* 1029 */                 Node nextNode = (Node)destNodes.get(nodeCount++);
/*      */                 int nextNodeChance;
/*      */                 int nextNodeChance;
/* 1030 */                 if (currentNode.getDestValue(me.getName()) == 0) {
/* 1031 */                   nextNodeChance = 0;
/*      */                 }
/*      */                 else {
/* 1034 */                   nextNodeChance = (currentNode.getDestValue(me.getName()) - nextNode.getDestValue(me.getName())) * 500 / currentNode.getDestValue(me.getName());
/*      */                 }
/* 1036 */                 if ((randNo(100) < 30) && (randNo(100) >= nextNodeChance)) {
/* 1037 */                   currentNode = nextNode;
/*      */                 }
/*      */                 else
/* 1040 */                   tryNextNode = false;
/*      */               }
/*      */               else
/*      */               {
/* 1044 */                 tryNextNode = false;
/*      */               }
/*      */ 
/*      */             }
/*      */ 
/* 1049 */             selectionIsOK = true;
/*      */ 
/* 1052 */             if (currentNode.getProvince().isBeingMovedTo()) {
/* 1053 */               selectionIsOK = false;
/* 1054 */               destNodes.remove(currentNode);
/*      */             }
/* 1056 */             if (selectionIsOK)
/* 1057 */               currentUnit.setRetreatTo(currentNode);
/*      */           }
/* 1018 */         while (!
/* 1061 */           selectionIsOK);
/*      */       }
/* 1063 */       units.remove(0);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void genRemoveOrders(Power me, int removeCount)
/*      */   {
/* 1069 */     List units = genOrderedUnitList(me);
/* 1070 */     for (int removeCounter = 0; removeCounter < removeCount; removeCounter++)
/*      */     {
/* 1096 */       ((Unit)units.get(removeCounter)).setRemoval();
/*      */     }
/*      */   }
/*      */ 
/*      */   private void genBuildOrders(Power me, int buildCount)
/*      */   {
/* 1103 */     List homes = sortNodeListByDest(me, getBuildHomeList(me));
/*      */ 
/* 1106 */     while ((!homes.isEmpty()) && (buildCount > 0)) {
/* 1107 */       boolean tryNextHome = true;
/* 1108 */       Node currentHome = (Node)homes.get(0);
/* 1109 */       int homeCounter = 0;
/* 1110 */       while (tryNextHome) {
/* 1111 */         if (homeCounter < homes.size()) {
/* 1112 */           Node nextHome = (Node)homes.get(homeCounter++);
/*      */           int nextHomeChance;
/*      */           int nextHomeChance;
/* 1113 */           if (nextHome.getDestValue(me.getName()) == 0) {
/* 1114 */             nextHomeChance = 0;
/*      */           }
/*      */           else {
/* 1117 */             nextHomeChance = (currentHome.getDestValue(me.getName()) - nextHome.getDestValue(me.getName())) * 500 / currentHome.getDestValue(me.getName());
/*      */           }
/* 1119 */           if ((randNo(100) < 30) && (randNo(100) >= nextHomeChance)) {
/* 1120 */             currentHome = nextHome;
/*      */           }
/*      */           else
/* 1123 */             tryNextHome = false;
/*      */         }
/*      */         else
/*      */         {
/* 1127 */           tryNextHome = false;
/*      */         }
/*      */       }
/*      */ 
/* 1131 */       currentHome.setBuildHere();
/*      */ 
/* 1133 */       List sameNodeList = currentHome.getProvince().getNodeList();
/* 1134 */       for (int i = 0; i < sameNodeList.size(); i++) {
/* 1135 */         homes.remove(sameNodeList.get(i));
/*      */       }
/* 1137 */       buildCount--;
/*      */     }
/* 1139 */     if (buildCount > 0)
/* 1140 */       this.waives = buildCount;
/*      */   }
/*      */ 
/*      */   private List<Unit> genRandomUnitList(Power me)
/*      */   {
/* 1146 */     List units = new ArrayList(me.getArmySize() + me.getFleetSize());
/* 1147 */     for (int x = 0; x < me.getArmySize(); x++) {
/* 1148 */       units.add(me.getArmies()[x]);
/*      */     }
/* 1150 */     for (int x = me.getArmySize(); x < me.getArmySize() + me.getFleetSize(); x++) {
/* 1151 */       units.add(me.getFleets()[(x - me.getArmySize())]);
/*      */     }
/* 1153 */     Collections.shuffle(units);
/* 1154 */     return units;
/*      */   }
/*      */ 
/*      */   private List<Unit> genRandomRETUnitList(Power me)
/*      */   {
/* 1159 */     List units = new ArrayList(me.getArmySize() + me.getFleetSize());
/* 1160 */     for (int x = 0; x < me.getArmySize(); x++)
/*      */     {
/* 1162 */       if (me.getArmies()[x].mustRetreat()) {
/* 1163 */         units.add(me.getArmies()[x]);
/*      */       }
/*      */     }
/*      */ 
/* 1167 */     for (int x = 0; x < me.getFleetSize(); x++) {
/* 1168 */       if (me.getFleets()[x].mustRetreat()) {
/* 1169 */         units.add(me.getFleets()[x]);
/*      */       }
/*      */     }
/* 1172 */     Collections.shuffle(units);
/* 1173 */     return units;
/*      */   }
/*      */ 
/*      */   private List<Unit> genOrderedUnitList(Power me)
/*      */   {
/* 1179 */     List units = new ArrayList(me.getArmySize() + me.getFleetSize());
/* 1180 */     for (int x = 0; x < me.getArmySize(); x++) {
/* 1181 */       units.add(me.getArmies()[x]);
/*      */     }
/* 1183 */     for (int x = me.getArmySize(); x < me.getArmySize() + me.getFleetSize(); x++) {
/* 1184 */       units.add(me.getFleets()[(x - me.getArmySize())]);
/*      */     }
/* 1186 */     return sortUnitsByDestVal(me, units);
/*      */   }
/*      */ 
/*      */   private List<Unit> sortUnitsByDestVal(Power me, List<Unit> units)
/*      */   {
/* 1192 */     int i = units.size();
/*      */     do { boolean flipped = false;
/* 1194 */       for (int j = 0; j < i; j++) {
/* 1195 */         if (((Unit)units.get(j)).getLocation().getDestValue(me.getName()) > ((Unit)units.get(j + 1)).getLocation().getDestValue(me.getName())) {
/* 1196 */           Unit panda = (Unit)units.get(j);
/* 1197 */           units.remove(j);
/* 1198 */           units.add(j + 1, panda);
/* 1199 */           flipped = true;
/*      */         }
/*      */       }
/* 1202 */       if (!flipped)
/* 1203 */         return units;
/* 1192 */       i--; } while (i >= 0);
/*      */ 
/* 1206 */     return units;
/*      */   }
/*      */ 
/*      */   private List<Node> sortNodeListByDest(Power me, List<Node> nodes)
/*      */   {
/* 1212 */     for (int panda = 0; panda < nodes.size(); panda++);
/* 1216 */     int i = nodes.size();
/*      */     do { boolean flipped = false;
/* 1218 */       for (int j = 0; j < i; j++) {
/* 1219 */         if (((Node)nodes.get(j)).getDestValue(me.getName()) < ((Node)nodes.get(j + 1)).getDestValue(me.getName())) {
/* 1220 */           Node panda = (Node)nodes.get(j);
/* 1221 */           nodes.remove(j);
/* 1222 */           nodes.add(j + 1, panda);
/* 1223 */           flipped = true;
/*      */         }
/*      */       }
/* 1226 */       if (!flipped)
/* 1227 */         return nodes;
/* 1216 */       i--; } while (i >= 0);
/*      */ 
/* 1231 */     for (int panda = 0; panda < nodes.size(); panda++);
/* 1236 */     return nodes;
/*      */   }
/*      */ 
/*      */   private List<Node> getBuildHomeList(Power me) {
/* 1240 */     List orighomes = me.getHomes();
/* 1241 */     List homes = new ArrayList();
/*      */ 
/* 1243 */     for (int i = 0; i < orighomes.size(); i++) {
/* 1244 */       if ((!((Province)orighomes.get(i)).isOccupied()) && (((Province)orighomes.get(i)).ownedBy().equals(me))) {
/* 1245 */         for (int j = 0; j < ((Province)orighomes.get(i)).getNodeList().size(); j++) {
/* 1246 */           homes.add((Node)((Province)orighomes.get(i)).getNodeList().get(j));
/*      */         }
/*      */       }
/*      */     }
/* 1250 */     return homes;
/*      */   }
/*      */ 
/*      */   private int randNo(int max)
/*      */   {
/* 1255 */     Random r = new Random();
/* 1256 */     int no = r.nextInt(max);
/* 1257 */     return no;
/*      */   }
/*      */ 
/*      */   public List<String[]> submitOrders(Power me) {
/* 1261 */     List listOfOrders = new ArrayList();
/* 1262 */     String[] order = new String[0];
/* 1263 */     Army[] myArmies = me.getArmies();
/* 1264 */     int armyCount = me.getArmySize();
/* 1265 */     Fleet[] myFleets = me.getFleets();
/* 1266 */     int fleetCount = me.getFleetSize();
/* 1267 */     List supUnits = new ArrayList(me.getArmySize() + me.getFleetSize());
/*      */ 
/* 1269 */     if ((this.season == 0) || (this.season == 2)) {
/* 1270 */       for (int i = 0; i < armyCount; i++) {
/* 1271 */         if (myArmies[i].getOrderToken().compareTo("SUP") == 0) {
/* 1272 */           supUnits.add(myArmies[i]);
/*      */         }
/*      */         else {
/* 1275 */           order = new String[myArmies[i].getCompleteOrder().length + 1];
/* 1276 */           order[0] = "SUB";
/* 1277 */           for (int j = 0; j < myArmies[i].getCompleteOrder().length; j++) {
/* 1278 */             order[(j + 1)] = myArmies[i].getCompleteOrder()[j];
/*      */           }
/* 1280 */           listOfOrders.add(order);
/*      */         }
/*      */       }
/*      */ 
/* 1284 */       for (int i = 0; i < fleetCount; i++) {
/* 1285 */         if (myFleets[i].getOrderToken().compareTo("SUP") == 0) {
/* 1286 */           supUnits.add(myFleets[i]);
/*      */         }
/*      */         else {
/* 1289 */           order = new String[myFleets[i].getCompleteOrder().length + 1];
/* 1290 */           order[0] = "SUB";
/* 1291 */           for (int j = 0; j < myFleets[i].getCompleteOrder().length; j++) {
/* 1292 */             order[(j + 1)] = myFleets[i].getCompleteOrder()[j];
/*      */           }
/* 1294 */           listOfOrders.add(order);
/*      */         }
/*      */       }
/*      */ 
/* 1298 */       for (int i = 0; i < supUnits.size(); i++) {
/* 1299 */         order = new String[((Unit)supUnits.get(i)).getCompleteOrder().length + 1];
/* 1300 */         order[0] = "SUB";
/* 1301 */         for (int j = 0; j < ((Unit)supUnits.get(i)).getCompleteOrder().length; j++) {
/* 1302 */           order[(j + 1)] = ((Unit)supUnits.get(i)).getCompleteOrder()[j];
/*      */         }
/* 1304 */         listOfOrders.add(order);
/*      */       }
/*      */ 
/* 1307 */       this.globalSupUnit = supUnits;
/*      */     }
/* 1309 */     else if ((this.season == 1) || (this.season == 3)) {
/* 1310 */       for (int i = 0; i < armyCount; i++) {
/* 1311 */         if (myArmies[i].mustRetreat()) {
/* 1312 */           order = new String[myArmies[i].getCompleteOrder().length + 1];
/* 1313 */           order[0] = "SUB";
/* 1314 */           for (int j = 0; j < myArmies[i].getCompleteOrder().length; j++) {
/* 1315 */             order[(j + 1)] = myArmies[i].getCompleteOrder()[j];
/*      */           }
/* 1317 */           listOfOrders.add(order);
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/* 1322 */       for (int i = 0; i < fleetCount; i++) {
/* 1323 */         if (myFleets[i].mustRetreat()) {
/* 1324 */           order = new String[myFleets[i].getCompleteOrder().length + 1];
/* 1325 */           order[0] = "SUB";
/* 1326 */           for (int j = 0; j < myFleets[i].getCompleteOrder().length; j++) {
/* 1327 */             order[(j + 1)] = myFleets[i].getCompleteOrder()[j];
/*      */           }
/* 1329 */           listOfOrders.add(order);
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/*      */     }
/* 1335 */     else if (me.getArmySize() + me.getFleetSize() > me.countSCs()) {
/* 1336 */       for (int i = 0; i < armyCount; i++) {
/* 1337 */         if (myArmies[i].beingRemoved()) {
/* 1338 */           order = new String[myArmies[i].getCompleteOrder().length + 1];
/* 1339 */           order[0] = "SUB";
/* 1340 */           for (int j = 0; j < myArmies[i].getCompleteOrder().length; j++) {
/* 1341 */             order[(j + 1)] = myArmies[i].getCompleteOrder()[j];
/*      */           }
/* 1343 */           listOfOrders.add(order);
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/* 1348 */       for (int i = 0; i < fleetCount; i++) {
/* 1349 */         if (myFleets[i].beingRemoved()) {
/* 1350 */           order = new String[myFleets[i].getCompleteOrder().length + 1];
/* 1351 */           order[0] = "SUB";
/* 1352 */           for (int j = 0; j < myFleets[i].getCompleteOrder().length; j++) {
/* 1353 */             order[(j + 1)] = myFleets[i].getCompleteOrder()[j];
/*      */           }
/* 1355 */           listOfOrders.add(order);
/*      */         }
/*      */       }
/*      */     }
/*      */     else
/*      */     {
/* 1361 */       List homes = getBuildHomeList(me);
/* 1362 */       for (int i = 0; i < homes.size(); i++)
/*      */       {
/* 1364 */         if (!((Node)homes.get(i)).buildHere())
/*      */           continue;
/* 1366 */         order = ((Node)homes.get(i)).getName().substring(4).compareTo("CS") != 0 ? new String[9] : new String[12];
/*      */ 
/* 1368 */         order[0] = "SUB";
/* 1369 */         order[1] = "(";
/* 1370 */         order[2] = "(";
/* 1371 */         order[3] = me.getName();
/* 1372 */         order[4] = (((Node)homes.get(i)).getName().substring(3).compareTo("AMY") == 0 ? "AMY" : "FLT");
/*      */ 
/* 1374 */         int orderIndex = 5;
/*      */ 
/* 1376 */         if (((Node)homes.get(i)).getName().substring(4).compareTo("CS") != 0) {
/* 1377 */           order[(orderIndex++)] = ((Node)homes.get(i)).getProvince().getName();
/*      */         }
/*      */         else
/*      */         {
/* 1381 */           order[(orderIndex++)] = "(";
/* 1382 */           order[(orderIndex++)] = ((Node)homes.get(i)).getProvince().getName();
/* 1383 */           order[(orderIndex++)] = ((Node)homes.get(i)).getName().substring(3);
/* 1384 */           order[(orderIndex++)] = ")";
/*      */         }
/*      */ 
/* 1387 */         order[(orderIndex++)] = ")";
/* 1388 */         order[(orderIndex++)] = "BLD";
/* 1389 */         order[(orderIndex++)] = ")";
/*      */ 
/* 1391 */         listOfOrders.add(order);
/*      */       }
/*      */ 
/* 1395 */       while (this.waives > 0) {
/* 1396 */         String[] waiveMsg = { "SUB", "(", me.getName(), "WVE", ")" };
/* 1397 */         this.waives -= 1;
/* 1398 */         listOfOrders.add(waiveMsg);
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/* 1403 */     return listOfOrders;
/*      */   }
/*      */ 
/*      */   public void processDiplomaticMessage(String[] message)
/*      */   {
/*      */   }
/*      */ 
/*      */   public void handleORD(List<String[]> listORD, Power me)
/*      */   {
/* 1415 */     this.movesWeAgreed.clear();
/*      */ 
/* 1420 */     for (int i = 0; i < listORD.size(); i++) {
/* 1421 */       String[] message = (String[])listORD.get(i);
/* 1422 */       Power power = (Power)this.listOfPowers.get(message[7]);
/*      */ 
/* 1424 */       Province destination = null;
/*      */       Unit tempUnit;
/*      */       String order;
/* 1429 */       if (!message[9].equals("(")) {
/* 1430 */         Unit tempUnit = ((Province)this.listOfProvinces.get(message[9])).getUnit();
/* 1431 */         String order = message[11];
/* 1432 */         if (order.equals("MTO"))
/* 1433 */           destination = message[12].equals("(") ? (Province)this.listOfProvinces.get(message[13]) : (Province)this.listOfProvinces.get(message[12]);
/*      */       }
/*      */       else
/*      */       {
/* 1437 */         tempUnit = ((Province)this.listOfProvinces.get(message[10])).getUnit();
/* 1438 */         order = message[14];
/* 1439 */         if (order.equals("MTO")) {
/* 1440 */           destination = message[15].equals("(") ? (Province)this.listOfProvinces.get(message[16]) : (Province)this.listOfProvinces.get(message[15]);
/*      */         }
/*      */       }
/*      */ 
/* 1444 */       if (order.equals("HLD")) {
/* 1445 */         tempUnit.setStance(1);
/*      */       }
/* 1447 */       else if (order.equals("MTO")) {
/* 1448 */         if (destination.ownedBy() != me) tempUnit.setStance(1);
/*      */ 
/* 1450 */         if ((destination.isSC()) && (destination.ownedBy() == me)) {
/* 1451 */           tempUnit.setStance(2);
/* 1452 */           power.increaseTimesAttackedUs();
/*      */         }
/* 1455 */         else if ((destination.isOccupied()) && (destination.getUnit().getController() == me)) {
/* 1456 */           tempUnit.setStance(2);
/* 1457 */           power.increaseTimesAttackedUs();
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/* 1465 */     for (int i = 0; i < listORD.size(); i++) {
/* 1466 */       String[] message = (String[])listORD.get(i);
/* 1467 */       Power power = (Power)this.listOfPowers.get(message[7]);
/*      */ 
/* 1469 */       Unit supUnit = null;
/*      */ 
/* 1471 */       String supOrder = null;
/*      */       Unit tempUnit;
/*      */       String order;
/* 1473 */       if (!message[9].equals("(")) {
/* 1474 */         Unit tempUnit = ((Province)this.listOfProvinces.get(message[9])).getUnit();
/* 1475 */         String order = message[11];
/* 1476 */         if (order.equals("SUP")) {
/* 1477 */           supOrder = message[15].equals("(") ? message[20] : message[17];
/* 1478 */           supUnit = message[15].equals("(") ? ((Province)this.listOfProvinces.get(message[16])).getUnit() : ((Province)this.listOfProvinces.get(message[15])).getUnit();
/*      */         }
/*      */       }
/*      */       else {
/* 1482 */         tempUnit = ((Province)this.listOfProvinces.get(message[10])).getUnit();
/* 1483 */         order = message[14];
/* 1484 */         if (order.equals("SUP")) {
/* 1485 */           supOrder = message[18].equals("(") ? message[23] : message[20];
/* 1486 */           supUnit = message[18].equals("(") ? ((Province)this.listOfProvinces.get(message[19])).getUnit() : ((Province)this.listOfProvinces.get(message[18])).getUnit();
/*      */         }
/*      */       }
/*      */ 
/* 1490 */       if ((order.equals("SUP")) && (supOrder.equals("MTO"))) {
/* 1491 */         tempUnit.setStance(supUnit.getStance());
/* 1492 */         if (supUnit.getStance() != 2) continue; power.increaseTimesAttackedUs();
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/* 1497 */     Enumeration powerIterator2 = this.listOfPowers.elements();
/* 1498 */     while (powerIterator2.hasMoreElements()) {
/* 1499 */       Power power2 = (Power)powerIterator2.nextElement();
/* 1500 */       if (power2 != me) {
/* 1501 */         power2.flushAttacks();
/* 1502 */         power2.calculateEnemyFactor();
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   public void handleChances(Power me)
/*      */   {
/* 1510 */     Army[] tempArmies = new Army[40];
/* 1511 */     Fleet[] tempFleets = new Fleet[40];
/*      */ 
/* 1515 */     Node[] adjNodes = new Node[40];
/*      */ 
/* 1517 */     Enumeration powerIterator = this.listOfPowers.elements();
/*      */ 
/* 1519 */     boolean unitChanceUsed = false;
/*      */ 
/* 1521 */     while (powerIterator.hasMoreElements()) {
/* 1522 */       Power power = (Power)powerIterator.nextElement();
/* 1523 */       if (power != me) {
/* 1524 */         tempArmies = power.getArmies();
/* 1525 */         int armyCount = power.getArmySize();
/* 1526 */         tempFleets = power.getFleets();
/* 1527 */         int fleetCount = power.getFleetSize();
/*      */ 
/* 1529 */         for (int i = 0; i < armyCount; i++) {
/* 1530 */           Node tempNode = tempArmies[i].getLocation();
/* 1531 */           adjNodes = tempNode.getAdjacentNodes();
/* 1532 */           int adjNodeCount = tempNode.getAdjNodeCount();
/* 1533 */           for (int j = 0; (j < adjNodeCount) && (!unitChanceUsed); j++) {
/* 1534 */             if (adjNodes[j].getProvince().ownedBy() == me) {
/* 1535 */               power.increaseChancesToAttack();
/* 1536 */               unitChanceUsed = true;
/*      */             } else {
/* 1538 */               if ((adjNodes[j].getUnit() == null) || 
/* 1539 */                 (adjNodes[j].getUnit().getController() != me)) continue;
/* 1540 */               power.increaseChancesToAttack();
/* 1541 */               unitChanceUsed = true;
/*      */             }
/*      */           }
/*      */ 
/* 1545 */           unitChanceUsed = false;
/*      */         }
/*      */ 
/* 1548 */         for (i = 0; i < fleetCount; i++) {
/* 1549 */           Node tempNode = tempFleets[i].getLocation();
/* 1550 */           adjNodes = tempNode.getAdjacentNodes();
/* 1551 */           int adjNodeCount = tempNode.getAdjNodeCount();
/* 1552 */           for (int j = 0; (j < adjNodeCount) && (!unitChanceUsed); j++) {
/* 1553 */             if (adjNodes[j].getProvince().ownedBy() == me) {
/* 1554 */               power.increaseChancesToAttack();
/* 1555 */               unitChanceUsed = true;
/*      */             } else {
/* 1557 */               if ((adjNodes[j].getUnit() == null) || 
/* 1558 */                 (adjNodes[j].getUnit().getController() != me)) continue;
/* 1559 */               power.increaseChancesToAttack();
/* 1560 */               unitChanceUsed = true;
/*      */             }
/*      */           }
/*      */ 
/* 1564 */           unitChanceUsed = false;
/*      */         }
/*      */       }
/*      */ 
/* 1568 */       power.flushChances();
/*      */     }
/*      */   }
/*      */ 
/*      */   public String[] generateReply(String[] message, Power me)
/*      */   {
/* 1578 */     int msgType = determineOrder(message);
/* 1579 */     Proposal prop = null;
/* 1580 */     String[] returnMsg = (String[])null;
/*      */ 
/* 1582 */     if (msgType == 1) {
/* 1583 */       prop = new PeaceProp(message);
/* 1584 */       PeaceProp tempProp = new PeaceProp(message);
/* 1585 */       if (tempProp.isSendingPowerIncluded(prop.getPRP())) {
/* 1586 */         ((Power)this.listOfPowers.get(prop.getSender())).setAcceptedPeace();
/*      */       }
/* 1588 */       returnMsg = prop.getReply();
/* 1589 */     } else if (msgType == 4)
/*      */     {
/* 1591 */       prop = new OrdProp(message);
/* 1592 */       OrdProp xdo = new OrdProp(message);
/* 1593 */       Power askingPower = (Power)this.listOfPowers.get(xdo.getSender());
/* 1594 */       Node nodeToCredit = null;
/* 1595 */       Unit unitUs = null;
/* 1596 */       Unit unitToSup = null;
/*      */ 
/* 1598 */       if (xdo.getOrder().equals("HLD")) {
/* 1599 */         nodeToCredit = (Node)this.listOfNodes.get(xdo.getNode1());
/* 1600 */         if (askingPower.hasEnoughCredit(nodeToCredit.getDestValue(askingPower.getName()))) {
/* 1601 */           xdo.setTakeOrder();
/* 1602 */           askingPower.makeWithdrawal(nodeToCredit.getDestValue(askingPower.getName()));
/* 1603 */           unitUs = nodeToCredit.getUnit();
/* 1604 */           unitUs.setHold();
/*      */ 
/* 1606 */           this.movesWeAgreed.add(unitUs);
/*      */         }
/* 1608 */       } else if (xdo.getOrder().equals("MTO")) {
/* 1609 */         nodeToCredit = (Node)this.listOfNodes.get(xdo.getNode2());
/*      */ 
/* 1611 */         if (askingPower.hasEnoughCredit(nodeToCredit.getDestValue(askingPower.getName()))) {
/* 1612 */           xdo.setTakeOrder();
/* 1613 */           askingPower.makeWithdrawal(nodeToCredit.getDestValue(askingPower.getName()));
/* 1614 */           unitUs = ((Node)this.listOfNodes.get(xdo.getNode1())).getUnit();
/* 1615 */           unitUs.setMove(nodeToCredit);
/*      */ 
/* 1618 */           this.movesWeAgreed.add(unitUs);
/*      */         }
/*      */ 
/*      */       }
/* 1622 */       else if (xdo.getOrder().equals("SUP"))
/*      */       {
/* 1626 */         if (xdo.getAllyOrder().equals("HLD")) {
/* 1627 */           nodeToCredit = (Node)this.listOfNodes.get(xdo.getNode3());
/*      */ 
/* 1630 */           if (askingPower.hasEnoughCredit(nodeToCredit.getDestValue(askingPower.getName()))) {
/* 1631 */             xdo.setTakeOrder();
/* 1632 */             askingPower.makeWithdrawal(nodeToCredit.getDestValue(askingPower.getName()));
/* 1633 */             unitUs = ((Node)this.listOfNodes.get(xdo.getNode1())).getUnit();
/* 1634 */             unitToSup = nodeToCredit.getUnit();
/* 1635 */             unitToSup.setHold();
/* 1636 */             unitUs.setSupportHold(unitToSup);
/*      */ 
/* 1638 */             this.movesWeAgreed.add(unitUs);
/*      */           }
/*      */         }
/* 1641 */         else if (xdo.getAllyOrder().equals("MTO")) {
/* 1642 */           nodeToCredit = (Node)this.listOfNodes.get(xdo.getNode4());
/* 1643 */           if (askingPower.hasEnoughCredit(nodeToCredit.getDestValue(askingPower.getName()))) {
/* 1644 */             xdo.setTakeOrder();
/* 1645 */             askingPower.makeWithdrawal(nodeToCredit.getDestValue(askingPower.getName()));
/* 1646 */             unitUs = ((Node)this.listOfNodes.get(xdo.getNode1())).getUnit();
/* 1647 */             unitToSup = ((Node)this.listOfNodes.get(xdo.getNode3())).getUnit();
/* 1648 */             unitToSup.setMove((Node)this.listOfNodes.get(xdo.getNode4()));
/* 1649 */             unitUs.setSupportMove(unitToSup, (Node)this.listOfNodes.get(xdo.getNode4()));
/*      */ 
/* 1651 */             this.movesWeAgreed.add(unitUs);
/*      */           }
/*      */         }
/*      */       }
/*      */ 
/* 1656 */       returnMsg = xdo.getReply();
/*      */     }
/*      */ 
/* 1659 */     String[] nothing = { "HUH" };
/*      */ 
/* 1661 */     if (returnMsg == null) return nothing;
/* 1662 */     return returnMsg;
/*      */   }
/*      */ 
/*      */   private int determineOrder(String[] message) {
/* 1666 */     String[] temp = message;
/*      */ 
/* 1668 */     String[] temp2 = takeRight(temp, 4);
/* 1669 */     String[] temp3 = takeRight(temp2, findClose(temp2) - 1);
/*      */ 
/* 1671 */     if (temp3[4].equals("PCE")) return 1;
/* 1672 */     if (temp3[4].equals("ALY")) return 2;
/* 1673 */     if (temp3[4].equals("DMZ")) return 3;
/* 1674 */     if (temp3[4].equals("XDO")) return 4;
/* 1675 */     if (temp3[4].equals("DRW")) return 5;
/* 1676 */     if (temp3[4].equals("SLO")) return 6;
/*      */ 
/* 1678 */     return 0;
/*      */   }
/*      */ 
/*      */   public void addAcceptance(String[] message, Power me)
/*      */   {
/* 1685 */     Power fromPower = (Power)this.listOfPowers.get(message[2]);
/* 1686 */     List toPowers = new ArrayList();
/*      */ 
/* 1688 */     String[] pandaString = takeLeft(takeRight(message, 4), findClose(takeRight(message, 4)));
/*      */ 
/* 1690 */     for (int i = 1; i < pandaString.length - 1; i++) {
/* 1691 */       toPowers.add((Power)this.listOfPowers.get(pandaString[i]));
/*      */     }
/*      */ 
/* 1694 */     String[] rightOfPowers = takeRight(takeRight(message, 4), findClose(takeRight(message, 4)));
/*      */ 
/* 1697 */     String[] pandaString2 = takeRight(rightOfPowers, 4);
/*      */ 
/* 1700 */     String order = pandaString2[1];
/*      */ 
/* 1702 */     if (order.equals("XDO")) makeUnitDo(takeLeft(takeRight(pandaString2, 2), findClose(takeRight(pandaString2, 2))), me);
/* 1703 */     else if (order.equals("DMZ")) doNotMakeUnitDo(takeLeft(takeRight(pandaString2, 2), findClose(takeRight(pandaString2, 2))));
/* 1704 */     else if (order.equals("PCE")) addPCE(fromPower, takeLeft(takeRight(pandaString2, 2), findClose(takeRight(pandaString2, 2))));
/* 1705 */     else if (order.equals("ALY")) addALY(takeLeft(takeRight(pandaString2, 2), findClose(takeRight(pandaString2, 2))));
/* 1706 */     else if (order.equals("DRW")) makeDRW();
/* 1707 */     else if (order.equals("SLO")) makeSLO(takeLeft(takeRight(pandaString2, 2), findClose(takeRight(pandaString2, 2))));
/* 1708 */     else if (order.equals("NOT")) makeNOT(takeLeft(takeRight(pandaString2, 2), findClose(takeRight(pandaString2, 2)))); 
/*      */   }
/*      */ 
/*      */   private void makeNOT(String[] pandaString)
/*      */   {
/*      */   }
/*      */ 
/*      */   private void makeSLO(String[] pandaString)
/*      */   {
/*      */   }
/*      */ 
/*      */   private void makeDRW()
/*      */   {
/*      */   }
/*      */ 
/*      */   private void addALY(String[] pandaString)
/*      */   {
/*      */   }
/*      */ 
/*      */   private void addPCE(Power fromPower, String[] pandaString)
/*      */   {
/* 1728 */     fromPower.setAcceptedPeace();
/*      */   }
/*      */ 
/*      */   private void doNotMakeUnitDo(String[] pandaString) {
/* 1732 */     String[] DMZpowers = takeLeft(pandaString, findClose(pandaString));
/* 1733 */     String[] DMZprovinces = takeRight(pandaString, findClose(pandaString));
/*      */ 
/* 1735 */     for (int i = 1; i < DMZprovinces.length - 1; i++)
/* 1736 */       for (int j = 1; j < DMZpowers.length - 1; j++)
/* 1737 */         ((Province)this.listOfProvinces.get(DMZprovinces[i])).demilitarise((Power)this.listOfPowers.get(DMZpowers[j]));
/*      */   }
/*      */ 
/*      */   private void makeUnitDo(String[] pandaString, Power me)
/*      */   {
/* 1748 */     Unit currentUnit = ((Province)this.listOfProvinces.get(pandaString[4])).getOccupyingUnit();
/* 1749 */     String order = pandaString[6];
/*      */ 
/* 1751 */     if (order.equals("HLD")) {
/* 1752 */       currentUnit.setHold();
/* 1753 */       currentUnit.getController().setMoneyToGain(currentUnit.getLocation().getDestValue(me.getName()));
/* 1754 */       currentUnit.getController().increaseDealsMade();
/*      */     }
/* 1757 */     else if (order.equals("MTO"))
/*      */     {
/* 1762 */       int MTOProvinceIndex = pandaString[4].equals("(") ? 10 : 7;
/* 1763 */       String nodeName = pandaString[MTOProvinceIndex] + (
/* 1764 */         pandaString[MTOProvinceIndex].equals("(") ? pandaString[(MTOProvinceIndex + 2)] : pandaString[3]);
/* 1765 */       currentUnit.setMove((Node)this.listOfNodes.get(nodeName));
/*      */     }
/* 1768 */     else if (order.equals("SUP")) {
/* 1769 */       int SUPUnitIndex = pandaString[4].equals("(") ? 10 : 7;
/* 1770 */       Unit supportUnit = ((Province)this.listOfProvinces.get(pandaString[(SUPUnitIndex + 3)])).getOccupyingUnit();
/* 1771 */       if (pandaString[(findClose(takeRight(pandaString, SUPUnitIndex)) + SUPUnitIndex)].equals("MTO")) {
/* 1772 */         String supportLocation = pandaString[(findClose(takeRight(pandaString, SUPUnitIndex)) + SUPUnitIndex + 1)].equals("(") ? pandaString[(findClose(takeRight(pandaString, SUPUnitIndex)) + SUPUnitIndex + 2)] : pandaString[(findClose(takeRight(pandaString, SUPUnitIndex)) + SUPUnitIndex + 1)];
/* 1773 */         currentUnit.setSupportMove(supportUnit, (Node)((Province)this.listOfProvinces.get(supportLocation)).getNodeList().get(0));
/*      */       }
/*      */       else
/*      */       {
/* 1777 */         currentUnit.setSupportHold(supportUnit);
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/* 1784 */     this.listOfNegUnits.add(currentUnit);
/*      */   }
/*      */ 
/*      */   public void sendOutgoingMessages(Power me)
/*      */   {
/* 1793 */     sendPCEInvites(me);
/*      */   }
/*      */ 
/*      */   private void sendPCEInvites(Power me)
/*      */   {
/* 1798 */     Enumeration powerIterator = this.listOfPowers.elements();
/* 1799 */     while (powerIterator.hasMoreElements()) {
/* 1800 */       Power power = (Power)powerIterator.nextElement();
/*      */ 
/* 1802 */       if ((!power.wantToStab()) && (!power.isOut()) && (!power.isNoPress()) && (power != me) && (!power.getName().equals("UNO")) && (!power.acceptedPeace()) && (power.getEnemyFactor() < 0.4D)) {
/* 1803 */         String[] pceInvite = { "SND", "(", power.getName(), ")", "(", "PRP", "(", "PCE", "(", power.getName(), me.getName(), ")", ")", ")" };
/*      */ 
/* 1806 */         TheDiplominator.sendMessage(pceInvite);
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   public void shouldWeStab(Power me)
/*      */   {
/* 1813 */     Power largest = me;
/* 1814 */     Enumeration powerIterator = this.listOfPowers.elements();
/* 1815 */     while (powerIterator.hasMoreElements()) {
/* 1816 */       Power power = (Power)powerIterator.nextElement();
/* 1817 */       largest = power.countSCs() > largest.countSCs() ? power : largest;
/*      */     }
/*      */ 
/* 1821 */     if ((largest == me) && (me.countSCs() >= 10)) {
/* 1822 */       Enumeration powerIterator2 = this.listOfPowers.elements();
/* 1823 */       while (powerIterator2.hasMoreElements()) {
/* 1824 */         Power temp = (Power)powerIterator2.nextElement();
/* 1825 */         temp.setBackstab();
/*      */       }
/*      */     }
/*      */ 
/* 1829 */     if (me.countSCs() < 9) {
/* 1830 */       Enumeration powerIterator3 = this.listOfPowers.elements();
/* 1831 */       while (powerIterator3.hasMoreElements()) {
/* 1832 */         Power temp = (Power)powerIterator3.nextElement();
/* 1833 */         temp.unSetBackstab();
/*      */       }
/*      */     }
/*      */   }
/*      */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Map
 * JD-Core Version:    0.6.0
 */