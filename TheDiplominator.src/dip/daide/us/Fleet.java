/*     */ package dip.daide.us;
/*     */ 
/*     */ public class Fleet extends Unit
/*     */ {
/*     */   public Fleet(Power controller, Node location)
/*     */   {
/*   6 */     super(controller, location);
/*     */   }
/*     */ 
/*     */   public String[] getCompleteOrder()
/*     */   {
/*  11 */     String first = this.location.getName().substring(0, 3);
/*  12 */     String firstOL = "";
/*  13 */     String firstOSU = "";
/*  14 */     String second = "";
/*  15 */     String secondOL = "";
/*  16 */     String secondOSU = "";
/*     */ 
/*  18 */     if (!this.location.getName().substring(3).equals("FLT")) {
/*  19 */       second = this.location.getName().substring(3);
/*     */     }
/*     */ 
/*  22 */     if (this.orderLocation != null) {
/*  23 */       firstOL = this.orderLocation.getName().substring(0, 3);
/*  24 */       if ((!this.orderLocation.getName().substring(3).equals("FLT")) && (!this.orderLocation.getName().substring(3).equals("AMY"))) {
/*  25 */         secondOL = this.orderLocation.getName().substring(3);
/*     */       }
/*     */     }
/*     */ 
/*  29 */     if (this.orderSupportUnit != null) {
/*  30 */       firstOSU = this.orderSupportUnit.location.getName().substring(0, 3);
/*  31 */       if ((!this.orderSupportUnit.location.getName().substring(3).equals("FLT")) && (!this.orderSupportUnit.location.getName().substring(3).equals("AMY"))) {
/*  32 */         secondOSU = this.orderSupportUnit.location.getName().substring(3);
/*     */       }
/*     */     }
/*     */ 
/*  36 */     if (this.order.equals("HLD")) {
/*  37 */       if (second.equals("")) {
/*  38 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "HLD", ")" };
/*  39 */         return orders;
/*     */       }
/*  41 */       String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "HLD", ")" };
/*  42 */       return orders;
/*     */     }
/*  44 */     if (this.order.equals("MTO")) {
/*  45 */       if (second.equals("")) {
/*  46 */         if (secondOL.equals("")) {
/*  47 */           String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "MTO", firstOL, ")" };
/*  48 */           return orders;
/*     */         }
/*  50 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "MTO", "(", firstOL, secondOL, ")", ")" };
/*  51 */         return orders;
/*     */       }
/*     */ 
/*  54 */       if (secondOL.equals("")) {
/*  55 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "MTO", firstOL, ")" };
/*  56 */         return orders;
/*     */       }
/*  58 */       String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "MTO", "(", firstOL, secondOL, ")", ")" };
/*  59 */       return orders;
/*     */     }
/*     */ 
/*  62 */     if (this.order.equals("SUP")) {
/*  63 */       if (this.orderLocation != null) {
/*  64 */         String[] orderSUnit = this.orderSupportUnit.getCompleteOrder();
/*  65 */         int len = orderSUnit.length;
/*     */         String[] orders;
/*     */         int j;
/*  68 */         if (second.equals(""))
/*     */         {
/*     */           String[] orders;
/*     */           String[] orders;
/*  69 */           if (orderSUnit[(len - 3)].equals("MTO"))
/*  70 */             orders = new String[8 + len - 2];
/*     */           else {
/*  72 */             orders = new String[5 + len - 2];
/*     */           }
/*  74 */           int j = 0;
/*  75 */           orders[4] = first;
/*     */         }
/*     */         else
/*     */         {
/*     */           String[] orders;
/*  77 */           if (orderSUnit[(len - 3)].equals("MTO"))
/*  78 */             orders = new String[9 + len];
/*     */           else {
/*  80 */             orders = new String[6 + len];
/*     */           }
/*  82 */           j = 3;
/*  83 */           orders[4] = "(";
/*  84 */           orders[5] = first;
/*  85 */           orders[6] = second;
/*  86 */           orders[7] = ")";
/*     */         }
/*  88 */         orders[0] = "(";
/*  89 */         orders[1] = "(";
/*  90 */         orders[2] = this.controller.getName();
/*  91 */         orders[3] = "FLT";
/*  92 */         orders[(5 + j)] = ")";
/*  93 */         orders[(6 + j)] = "SUP";
/*  94 */         if (orderSUnit[(len - 3)].equals("MTO")) {
/*  95 */           for (int i = 1; i < len - 1; i++)
/*  96 */             orders[(i + 6 + j)] = orderSUnit[i];
/*     */         }
/*     */         else {
/*  99 */           for (int i = 1; i < len - 1; i++) {
/* 100 */             if (i < len - 5) {
/* 101 */               orders[(i + 6 + j)] = orderSUnit[i]; if (this.orderSupportUnit != null) {
/* 102 */                 firstOSU = this.orderSupportUnit.location.getName().substring(0, 3);
/* 103 */                 if (this.orderSupportUnit.location.getName().length() > 3)
/* 104 */                   secondOSU = this.orderSupportUnit.location.getName().substring(3);
/*     */               }
/*     */             }
/* 107 */             else if (i == len - 4) {
/* 108 */               orders[(i + 5 + j)] = orderSUnit[i];
/* 109 */             } else if (i > len - 2) {
/* 110 */               orders[(i + 3 + j)] = orderSUnit[i];
/*     */             }
/*     */           }
/*     */         }
/* 114 */         orders[(orders.length - 1)] = ")";
/* 115 */         return orders;
/*     */       }
/* 117 */       String unitType = this.orderSupportUnit.getUnitType();
/* 118 */       if (second.equals("")) {
/* 119 */         if (secondOSU.equals("")) {
/* 120 */           String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "SUP", "(", this.orderSupportUnit.controller.getName(), unitType, firstOSU, ")", ")" };
/* 121 */           return orders;
/*     */         }
/* 123 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "SUP", "(", this.orderSupportUnit.controller.getName(), unitType, "(", firstOSU, secondOSU, ")", ")", ")" };
/* 124 */         return orders;
/*     */       }
/*     */ 
/* 127 */       if (secondOSU.equals("")) {
/* 128 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "SUP", "(", this.orderSupportUnit.controller.getName(), unitType, firstOSU, ")", ")" };
/* 129 */         return orders;
/*     */       }
/* 131 */       String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "SUP", "(", this.orderSupportUnit.controller.getName(), unitType, "(", firstOSU, secondOSU, ")", ")", ")" };
/* 132 */       return orders;
/*     */     }
/*     */ 
/* 136 */     if (this.order.equals("RTO")) {
/* 137 */       if (second.equals("")) {
/* 138 */         if (secondOL.equals("")) {
/* 139 */           String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "RTO", firstOL, ")" };
/* 140 */           return orders;
/*     */         }
/* 142 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "RTO", "(", firstOL, secondOL, ")", ")" };
/* 143 */         return orders;
/*     */       }
/*     */ 
/* 146 */       if (secondOL.equals("")) {
/* 147 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "RTO", firstOL, ")" };
/* 148 */         return orders;
/*     */       }
/* 150 */       String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "RTO", "(", firstOL, secondOL, ")", ")" };
/* 151 */       return orders;
/*     */     }
/*     */ 
/* 154 */     if (this.order.equals("DSB")) {
/* 155 */       if (second.equals("")) {
/* 156 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "DSB", ")" };
/* 157 */         return orders;
/*     */       }
/* 159 */       String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "DSB", ")" };
/* 160 */       return orders;
/*     */     }
/* 162 */     if (this.order.equals("REM")) {
/* 163 */       if (second.equals("")) {
/* 164 */         String[] orders = { "(", "(", this.controller.getName(), "FLT", first, ")", "REM", ")" };
/* 165 */         return orders;
/*     */       }
/* 167 */       String[] orders = { "(", "(", this.controller.getName(), "FLT", "(", first, second, ")", ")", "REM", ")" };
/* 168 */       return orders;
/*     */     }
/*     */ 
/* 171 */     String[] orders = new String[0];
/* 172 */     return orders;
/*     */   }
/*     */ 
/*     */   public String getUnitType()
/*     */   {
/* 177 */     return "FLT";
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Fleet
 * JD-Core Version:    0.6.0
 */