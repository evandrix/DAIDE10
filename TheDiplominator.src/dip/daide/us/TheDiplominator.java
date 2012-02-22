/*     */ package dip.daide.us;
/*     */ 
/*     */ import dip.daide.comm.DisconnectedException;
/*     */ import dip.daide.comm.MessageListener;
/*     */ import dip.daide.comm.Server;
/*     */ import dip.daide.comm.UnknownTokenException;
/*     */ import java.io.IOException;
/*     */ import java.io.PrintStream;
/*     */ import java.net.InetAddress;
/*     */ import java.net.UnknownHostException;
/*     */ import java.util.ArrayList;
/*     */ import java.util.Enumeration;
/*     */ import java.util.Hashtable;
/*     */ import java.util.List;
/*     */ import java.util.concurrent.LinkedBlockingQueue;
/*     */ 
/*     */ public class TheDiplominator
/*     */   implements MessageListener
/*     */ {
/*     */   static final String VERSION = "0.2";
/*     */   private String name;
/*     */   private static Server serv;
/*  21 */   private static LinkedBlockingQueue<String[]> messageQueue = new LinkedBlockingQueue();
/*  22 */   static boolean hasStarted = false;
/*     */   private Map map;
/*     */   private Power me;
/*     */   private int passcode;
/*  26 */   private List<String[]> ordList = new ArrayList();
/*  27 */   private int iteration = 0;
/*     */   private int pressLevel;
/*     */   String year;
/*  31 */   private int turn = 0;
/*     */ 
/*  33 */   private final int SPR = 0;
/*  34 */   private final int SUM = 1;
/*  35 */   private final int FAL = 2;
/*  36 */   private final int AUT = 3;
/*  37 */   private final int WIN = 4;
/*     */ 
/*     */   public TheDiplominator(InetAddress ip, int port, String name) {
/*  40 */     this.name = name;
/*     */     try {
/*  42 */       serv = new Server(ip, port);
/*  43 */       serv.addMessageListener(this);
/*  44 */       serv.connect();
/*  45 */       String[] msg = { 
/*  46 */         "NME", 
/*  47 */         "(", "'" + this.name + "'", ")", 
/*  48 */         "(", "'0.2'", ")" };
/*     */ 
/*  51 */       serv.send(msg);
/*     */     }
/*     */     catch (IOException ioe)
/*     */     {
/*  55 */       ioe.printStackTrace();
/*     */     } catch (DisconnectedException de) {
/*  57 */       System.out.println("Ok, we're disconnected. Exiting...");
/*  58 */       System.exit(0);
/*     */     } catch (UnknownTokenException ute) {
/*  60 */       System.err.println("Unknown token '" + ute.getToken() + "'");
/*  61 */       System.exit(1);
/*     */     }
/*     */   }
/*     */ 
/*     */   public static void main(String[] args)
/*     */   {
/*     */     try {
/*  68 */       new TheDiplominator(InetAddress.getByName(args[0]), 
/*  69 */         Integer.parseInt(args[1]), 
/*  70 */         args[2]);
/*     */     }
/*     */     catch (ArrayIndexOutOfBoundsException be) {
/*  73 */       usage();
/*     */     } catch (UnknownHostException uhe) {
/*  75 */       System.err.println("Unknown host: " + uhe.getMessage());
/*     */     } catch (NumberFormatException nfe) {
/*  77 */       usage();
/*     */     }
/*     */   }
/*     */ 
/*     */   public static void usage() {
/*  82 */     System.err.println("Usage:\n  Diplominator <ip> <port> <name>");
/*     */   }
/*     */ 
/*     */   public void messageReceived(String[] message)
/*     */   {
/*  88 */     System.out.print("Server: ");
/*  89 */     printMessage(message);
/*     */ 
/*  91 */     if (!hasStarted) {
/*  92 */       handlePreGameMessage(message);
/*  93 */     } else if (message[0].equals("HLO")) {
/*  94 */       handleHLO(message);
/*  95 */     } else if (message[0].equals("ORD")) {
/*  96 */       if (!message[7].equals(this.me.getName())) this.ordList.add(message); 
/*     */     }
/*  97 */     else if (message[0].equals("NOW"))
/*     */     {
/*  99 */       Enumeration powerIterator = this.map.listOfPowers.elements();
/* 100 */       System.out.println("--------------------------CHECKING IF I HAVE ANY FREINDS----------------------");
/* 101 */       while (powerIterator.hasMoreElements()) {
/* 102 */         Power power = (Power)powerIterator.nextElement();
/*     */ 
/* 104 */         if ((power != this.me) && (!power.getName().equals("UNO")) && (!power.isOut()) && (power.acceptedPeace())) {
/* 105 */           System.out.println(power.getName() + " is our friend");
/*     */         }
/*     */       }
/* 108 */       System.out.println("--------------------------_______________________________----------------------");
/*     */ 
/* 110 */       if (message[2].equals("SPR")) this.turn = 0;
/* 111 */       if (message[2].equals("SUM")) this.turn = 1;
/* 112 */       if (message[2].equals("FAL")) this.turn = 2;
/* 113 */       if (message[2].equals("AUT")) this.turn = 3;
/* 114 */       if (message[2].equals("WIN")) this.turn = 4;
/* 115 */       this.year = message[3];
/* 116 */       if (this.turn == 2) {
/* 117 */         this.map.handleChances(this.me);
/*     */       }
/*     */ 
/* 120 */       if ((this.turn == 1) || (this.turn == 4) || (this.turn == 3) || (this.turn == 2)) {
/* 121 */         this.map.handleORD(this.ordList, this.me);
/*     */       }
/* 123 */       this.ordList.removeAll(this.ordList);
/*     */ 
/* 125 */       this.map.storeSeason(message[2]);
/* 126 */       this.map.updateUnits(message);
/* 127 */       this.map.shouldWeStab(this.me);
/*     */ 
/* 129 */       if ((this.turn == 2) || (this.turn == 0)) {
/* 130 */         messageQueue.clear();
/* 131 */         new Negotiator(this.map, messageQueue, this.me, this.pressLevel).start();
/*     */       }
/*     */       else
/*     */       {
/* 135 */         List orders = this.map.processNOW(this.me);
/* 136 */         for (int i = 0; i < orders.size(); i++) {
/* 137 */           sendMessage((String[])orders.get(i));
/*     */         }
/*     */       }
/*     */     }
/* 141 */     else if (message[0].equals("SCO"))
/*     */     {
/* 143 */       this.map.handleChances(this.me);
/* 144 */       this.map.handleORD(this.ordList, this.me);
/* 145 */       this.ordList.removeAll(this.ordList);
/* 146 */       this.map.updateSCO(message);
/* 147 */     } else if (!message[0].equals("YES"))
/*     */     {
/* 149 */       if (!message[0].equals("MIS"))
/*     */       {
/* 151 */         if (message[0].equals("OUT")) {
/* 152 */           ((Power)this.map.listOfPowers.get(message[2])).setOut();
/*     */         }
/* 154 */         else if (!message[0].equals("HST"))
/*     */         {
/* 156 */           if (message[0].equals("OFF"))
/*     */           {
/* 169 */             System.exit(0);
/* 170 */           } else if (!message[0].equals("THX"))
/*     */           {
/* 173 */             if (!message[0].equals("CCD"))
/*     */             {
/* 175 */               if ((message.length >= 11) && (message[8].equals("TRY")) && (message[9].equals("(")) && (message[10].equals(")")))
/*     */               {
/* 178 */                 ((Power)this.map.listOfPowers.get(message[2])).setNoPress();
/*     */               }
/* 180 */               else if (message[0].equals("SLO"))
/*     */               {
/* 182 */                 if (message[2].equals(this.me.getName())) {
/* 183 */                   System.out.println("The game is over. We won.");
/* 184 */                   System.exit(0);
/*     */                 }
/*     */                 else {
/* 187 */                   System.out.println("The game is over. " + message[2] + " won.");
/* 188 */                   System.exit(0);
/*     */                 }
/*     */               }
/*     */               else
/* 192 */                 messageQueue.add(message); 
/*     */             }
/*     */           }
/*     */         }
/*     */       }
/*     */     }
/*     */   }
/*     */ 
/*     */   private void handleHLO(String[] message) {
/* 199 */     this.me = this.map.getPower(message[2]);
/* 200 */     this.passcode = Integer.parseInt(message[5]);
/*     */ 
/* 203 */     this.pressLevel = Integer.parseInt(message[10]);
/*     */   }
/*     */ 
/*     */   private void handlePreGameMessage(String[] message)
/*     */   {
/* 211 */     if ((message[0].equalsIgnoreCase("MAP")) && 
/* 212 */       (message[2].equalsIgnoreCase("'STANDARD'"))) {
/* 213 */       sendMessage(new String[] { "MDF" });
/*     */     }
/*     */ 
/* 216 */     if (message[0].equalsIgnoreCase("MDF")) {
/* 217 */       this.map = new Map(message);
/* 218 */       sendMessage(new String[] { "YES", "(", "MAP", "(", "'STANDARD'", ")", ")" });
/*     */ 
/* 220 */       hasStarted = true;
/*     */     }
/*     */   }
/*     */ 
/*     */   static void printMessage(String[] msg)
/*     */   {
/* 226 */     for (int i = 0; i < msg.length; i++) {
/* 227 */       System.out.print(msg[i] + " ");
/*     */     }
/* 229 */     System.out.println();
/*     */   }
/*     */ 
/*     */   static void sendMessage(String[] msg) {
/*     */     try {
/* 234 */       serv.send(msg);
/* 235 */       System.out.print("Diplominator: ");
/* 236 */       printMessage(msg);
/*     */     }
/*     */     catch (DisconnectedException de) {
/* 239 */       System.out.println("No longer connected to server. Exiting.");
/* 240 */       System.exit(0);
/*     */     } catch (UnknownTokenException ute) {
/* 242 */       System.err.println("Unknown token '" + ute.getToken() + "'");
/* 243 */       System.exit(1);
/*     */     }
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.TheDiplominator
 * JD-Core Version:    0.6.0
 */