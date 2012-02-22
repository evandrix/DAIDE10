/*     */ package dip.daide.us;
/*     */ 
/*     */ import java.io.PrintStream;
/*     */ import java.util.ArrayList;
/*     */ import java.util.Enumeration;
/*     */ import java.util.Hashtable;
/*     */ import java.util.List;
/*     */ import java.util.concurrent.LinkedBlockingQueue;
/*     */ import java.util.concurrent.TimeUnit;
/*     */ 
/*     */ public class Negotiator extends Thread
/*     */ {
/*   9 */   private final long INITIAL_WAIT_TIME = 50L;
/*  10 */   private final long LOOP_WAIT_TIME = 50L;
/*     */   private Map map;
/*     */   private LinkedBlockingQueue<String[]> q;
/*     */   private Power me;
/*     */   private int pressLevel;
/*     */ 
/*     */   public Negotiator(Map map, LinkedBlockingQueue<String[]> q, Power me, int pressLevel)
/*     */   {
/*  19 */     this.pressLevel = pressLevel;
/*  20 */     this.map = map;
/*  21 */     this.q = q;
/*  22 */     this.me = me;
/*  23 */     new Thread(this);
/*     */   }
/*     */ 
/*     */   public void run()
/*     */   {
/*  32 */     String[] message = (String[])null;
/*  33 */     List pendingProposals = new ArrayList();
/*     */ 
/*  36 */     Enumeration powerIterator = this.map.listOfPowers.elements();
/*  37 */     while (powerIterator.hasMoreElements()) {
/*  38 */       Power power = (Power)powerIterator.nextElement();
/*  39 */       if (power != this.me) {
/*  40 */         this.map.processFactors(power);
/*  41 */         this.map.initStrCompValues(power);
/*  42 */         this.map.calcDest(power);
/*     */       }
/*     */     }
/*     */ 
/*  46 */     this.map.processFactors(this.me);
/*  47 */     this.map.initStrCompValues(this.me);
/*     */ 
/*  50 */     boolean firstTimeIn = true;
/*     */ 
/*  57 */     if (this.pressLevel > 0)
/*     */     {
/*     */       try
/*     */       {
/*  62 */         message = (String[])this.q.poll(50L, TimeUnit.MILLISECONDS);
/*     */       }
/*     */       catch (InterruptedException localInterruptedException1)
/*     */       {
/*     */       }
/*     */ 
/*     */       do
/*     */       {
/*  72 */         firstTimeIn = false;
/*  73 */         this.map.sendOutgoingMessages(this.me);
/*     */ 
/*  75 */         while (message != null)
/*     */         {
/*  85 */           if (isProposal(message))
/*     */           {
/*  87 */             pendingProposals.add(message);
/*  88 */           } else if (isAccepted(message))
/*     */           {
/*  91 */             this.map.addAcceptance(message, this.me);
/*     */           }
/*     */ 
/*  94 */           message = (String[])this.q.poll();
/*     */         }
/*     */ 
/*  98 */         handlePending(pendingProposals);
/*     */         try
/*     */         {
/* 103 */           message = (String[])this.q.poll(50L, TimeUnit.MILLISECONDS);
/*     */         } catch (InterruptedException e) {
/* 105 */           System.out.println("We got interrupted - Java's fault");
/*     */         }
/*     */       }
/*  70 */       while ((message != null) || (firstTimeIn));
/*     */     }
/*     */ 
/* 113 */     this.map.calcDest(this.me);
/*     */ 
/* 116 */     this.map.genMoveOrders(this.me);
/* 117 */     List orders = this.map.submitOrders(this.me);
/* 118 */     for (int i = 0; i < orders.size(); i++)
/* 119 */       TheDiplominator.sendMessage((String[])orders.get(i));
/*     */   }
/*     */ 
/*     */   private void handlePending(List<String[]> pendingProposals)
/*     */   {
/* 128 */     while (!pendingProposals.isEmpty()) {
/* 129 */       String[] currentProposal = (String[])pendingProposals.remove(0);
/* 130 */       TheDiplominator.sendMessage(this.map.generateReply(currentProposal, this.me));
/*     */     }
/*     */   }
/*     */ 
/*     */   private boolean isAccepted(String[] message)
/*     */   {
/* 138 */     String[] tempMessage = message;
/* 139 */     tempMessage = takeRight(tempMessage, 1);
/* 140 */     int close = findClose(tempMessage);
/* 141 */     tempMessage = takeRight(tempMessage, close);
/*     */ 
/* 143 */     close = findClose(tempMessage);
/*     */ 
/* 145 */     return tempMessage[(close + 1)].equals("YES");
/*     */   }
/*     */ 
/*     */   private boolean isProposal(String[] message)
/*     */   {
/* 152 */     String[] tempMessage = message;
/* 153 */     tempMessage = takeRight(tempMessage, 1);
/* 154 */     int close = findClose(tempMessage);
/* 155 */     tempMessage = takeRight(tempMessage, close);
/*     */ 
/* 157 */     close = findClose(tempMessage);
/*     */ 
/* 159 */     return tempMessage[(close + 1)].equals("PRP");
/*     */   }
/*     */ 
/*     */   private String[] takeLeft(String[] tokens, int end)
/*     */   {
/* 164 */     int i = 0;
/* 165 */     String[] left = new String[end];
/* 166 */     for (i = 0; i < end; i++) {
/* 167 */       left[i] = tokens[i];
/*     */     }
/*     */ 
/* 170 */     return left;
/*     */   }
/*     */ 
/*     */   private String[] takeRight(String[] tokens, int start)
/*     */   {
/* 177 */     int length = tokens.length;
/* 178 */     String[] right = new String[length - start];
/*     */ 
/* 180 */     for (int i = 0; i < length - start; i++) {
/* 181 */       right[i] = tokens[(start + i)];
/*     */     }
/*     */ 
/* 184 */     return right;
/*     */   }
/*     */ 
/*     */   private int findClose(String[] tokens) {
/* 188 */     int bracketCount = 0;
/* 189 */     int i = 0;
/*     */ 
/* 191 */     if (!tokens[0].equals("(")) return -1;
/*     */ 
/* 193 */     for (i = 0; i < tokens.length; i++)
/*     */     {
/* 195 */       if (tokens[i].equals("(")) bracketCount++;
/* 196 */       if (tokens[i].equals(")")) bracketCount--;
/* 197 */       if (bracketCount == 0) return i + 1;
/*     */     }
/*     */ 
/* 200 */     return -1;
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.Negotiator
 * JD-Core Version:    0.6.0
 */