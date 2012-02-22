/*     */ package dip.daide.us;
/*     */ 
/*     */ public class Army extends Unit
/*     */ {
/*     */   public Army(Power controller, Node location)
/*     */   {
/*   6 */     super(controller, location);
/*     */   }
/*     */ 
/*     */   public String[] getCompleteOrder()
/*     */   {
/*  13 */     String first = this.location.getName().substring(0, 3);
/*  14 */     String firstOL = "";
/*  15 */     String firstOSU = "";
/*  16 */     String second = "";
/*  17 */     String secondOL = "";
/*  18 */     String secondOSU = "";
/*     */ 
/*  20 */     if (this.orderLocation != null) {
/*  21 */       firstOL = this.orderLocation.getName().substring(0, 3);
/*  22 */       if ((!this.orderLocation.getName().substring(3).equals("FLT")) && (!this.orderLocation.getName().substring(3).equals("AMY"))) {
/*  23 */         secondOL = this.orderLocation.getName().substring(3);
/*     */       }
/*     */     }
/*     */ 
/*  27 */     if (this.orderSupportUnit != null) {
/*  28 */       firstOSU = this.orderSupportUnit.location.getName().substring(0, 3);
/*  29 */       if ((!this.orderSupportUnit.location.getName().substring(3).equals("FLT")) && (!this.orderSupportUnit.location.getName().substring(3).equals("AMY"))) {
/*  30 */         secondOSU = this.orderSupportUnit.location.getName().substring(3);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/*  39 */     if (this.order.equals("HLD")) {
/*  40 */       if (second.equals("")) {
/*  41 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "HLD", ")" };
/*  42 */         return orders;
/*     */       }
/*  44 */       String[] orders = { "(", "(", this.controller.getName(), "AMY", first, second, ")", "HLD", ")" };
/*  45 */       return orders;
/*     */     }
/*  47 */     if (this.order.equals("MTO")) {
/*  48 */       if (second.equals("")) {
/*  49 */         if (secondOL.equals("")) {
/*  50 */           String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "MTO", firstOL, ")" };
/*  51 */           return orders;
/*     */         }
/*  53 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "MTO", firstOL, ")" };
/*  54 */         return orders;
/*     */       }
/*     */ 
/*  57 */       if (secondOL.equals("")) {
/*  58 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "MTO", firstOL, ")" };
/*  59 */         return orders;
/*     */       }
/*  61 */       String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "MTO", firstOL, ")" };
/*  62 */       return orders;
/*     */     }
/*     */ 
/*  65 */     if (this.order.equals("SUP")) {
/*  66 */       if (this.orderLocation != null) {
/*  67 */         String[] orderSUnit = this.orderSupportUnit.getCompleteOrder();
/*  68 */         int len = orderSUnit.length;
/*     */         String[] orders;
/*     */         int j;
/*  71 */         if (second.equals(""))
/*     */         {
/*     */           String[] orders;
/*     */           String[] orders;
/*  72 */           if (orderSUnit[(len - 3)].equals("MTO"))
/*  73 */             orders = new String[8 + len - 2];
/*     */           else {
/*  75 */             orders = new String[5 + len - 2];
/*     */           }
/*  77 */           int j = 0;
/*  78 */           orders[4] = first;
/*     */         }
/*     */         else
/*     */         {
/*     */           String[] orders;
/*  80 */           if (orderSUnit[(len - 3)].equals("MTO"))
/*  81 */             orders = new String[9 + len - 2];
/*     */           else {
/*  83 */             orders = new String[6 + len - 2];
/*     */           }
/*  85 */           j = 1;
/*  86 */           orders[4] = first;
/*  87 */           orders[5] = second;
/*     */         }
/*  89 */         orders[0] = "(";
/*  90 */         orders[1] = "(";
/*  91 */         orders[2] = this.controller.getName();
/*  92 */         orders[3] = "AMY";
/*  93 */         orders[(5 + j)] = ")";
/*  94 */         orders[(6 + j)] = "SUP";
/*  95 */         if (orderSUnit[(len - 3)].equals("MTO")) {
/*  96 */           for (int i = 1; i < len - 1; i++)
/*  97 */             orders[(i + 6 + j)] = orderSUnit[i];
/*     */         }
/*     */         else {
/* 100 */           for (int i = 1; i < len - 1; i++) {
/* 101 */             if (i < len - 5)
/* 102 */               orders[(i + 6 + j)] = orderSUnit[i];
/* 103 */             else if (i == len - 4)
/* 104 */               orders[(i + 5 + j)] = orderSUnit[i];
/* 105 */             else if (i > len - 2) {
/* 106 */               orders[(i + 3 + j)] = orderSUnit[i];
/*     */             }
/*     */           }
/*     */         }
/* 110 */         orders[(orders.length - 1)] = ")";
/* 111 */         return orders;
/*     */       }
/* 113 */       String unitType = this.orderSupportUnit.getUnitType();
/* 114 */       if (second.equals("")) {
/* 115 */         if (secondOSU.equals("")) {
/* 116 */           String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "SUP", "(", this.orderSupportUnit.controller.getName(), unitType, firstOSU, ")", ")" };
/* 117 */           return orders;
/*     */         }
/* 119 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "SUP", "(", this.orderSupportUnit.controller.getName(), unitType, "(", firstOSU, secondOSU, ")", ")", ")" };
/* 120 */         return orders;
/*     */       }
/*     */ 
/* 123 */       if (secondOSU.equals("")) {
/* 124 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "SUP", "(", this.orderSupportUnit.controller.getName(), unitType, firstOSU, ")", ")" };
/* 125 */         return orders;
/*     */       }
/* 127 */       String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "SUP", "(", this.orderSupportUnit.controller.getName(), unitType, "(", firstOSU, secondOSU, ")", ")", ")" };
/* 128 */       return orders;
/*     */     }
/*     */ 
/* 135 */     if (this.order.equals("RTO")) {
/* 136 */       if (second.equals("")) {
/* 137 */         if (secondOL.equals("")) {
/* 138 */           String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "RTO", firstOL, ")" };
/* 139 */           return orders;
/*     */         }
/* 141 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "RTO", firstOL, secondOL, ")" };
/* 142 */         return orders;
/*     */       }
/*     */ 
/* 145 */       if (secondOL.equals("")) {
/* 146 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, second, ")", "RTO", firstOL, ")" };
/* 147 */         return orders;
/*     */       }
/* 149 */       String[] orders = { "(", "(", this.controller.getName(), "AMY", first, second, ")", "RTO", firstOL, secondOL, ")" };
/* 150 */       return orders;
/*     */     }
/*     */ 
/* 153 */     if (this.order.equals("DSB")) {
/* 154 */       if (second.equals("")) {
/* 155 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "DSB", ")" };
/* 156 */         return orders;
/*     */       }
/* 158 */       String[] orders = { "(", "(", this.controller.getName(), "AMY", first, second, ")", "DSB", ")" };
/* 159 */       return orders;
/*     */     }
/* 161 */     if (this.order.equals("REM")) {
/* 162 */       if (second.equals("")) {
/* 163 */         String[] orders = { "(", "(", this.controller.getName(), "AMY", first, ")", "REM", ")" };
/* 164 */         return orders;
/*     */       }
/* 166 */       String[] orders = { "(", "(", this.controller.getName(), "AMY", first, second, ")", "REM", ")" };
/* 167 */       return orders;
/*     */     }
/*     */ 
/* 170 */     String[] orders = new String[0];
/* 171 */     return orders;
/*     */   }
/*     */ 
/*     */   public String getUnitType()
/*     */   {
/* 176 */     return "AMY";
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Army
 * JD-Core Version:    0.6.0
 */