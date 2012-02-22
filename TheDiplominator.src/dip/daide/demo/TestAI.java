/*     */ package dip.daide.demo;
/*     */ 
/*     */ import dip.daide.comm.DisconnectedException;
/*     */ import dip.daide.comm.MessageListener;
/*     */ import dip.daide.comm.Server;
/*     */ import dip.daide.comm.UnknownTokenException;
/*     */ import java.io.BufferedReader;
/*     */ import java.io.IOException;
/*     */ import java.io.InputStreamReader;
/*     */ import java.io.PrintStream;
/*     */ import java.net.InetAddress;
/*     */ import java.net.UnknownHostException;
/*     */ 
/*     */ public class TestAI
/*     */   implements MessageListener
/*     */ {
/*  31 */   static String VERSION = "v 0.7";
/*     */   String name;
/*     */   Server serv;
/*     */   boolean atPrompt;
/*     */ 
/*     */   void printPrompt()
/*     */   {
/*  37 */     if (!this.atPrompt) {
/*  38 */       System.out.print("Enter message: ");
/*  39 */       this.atPrompt = true;
/*     */     }
/*     */   }
/*     */ 
/*     */   void printMsg(String who, String[] message) {
/*  44 */     StringBuffer sbuf = new StringBuffer();
/*  45 */     for (int i = 0; i < message.length; i++) {
/*  46 */       String m = message[i];
/*  47 */       sbuf.append(m);
/*  48 */       boolean last = i + 1 == message.length;
/*  49 */       if ((m.equals("(")) || ((!last) && (message[(i + 1)].equals(")")))) {
/*     */         continue;
/*     */       }
/*  52 */       if (!last) {
/*  53 */         sbuf.append(' ');
/*     */       }
/*     */     }
/*     */ 
/*  57 */     if (this.atPrompt) {
/*  58 */       System.out.println();
/*  59 */       this.atPrompt = false;
/*     */     }
/*  61 */     System.out.println("<" + who + ">: " + sbuf.toString());
/*     */   }
/*     */ 
/*     */   public TestAI(InetAddress ip, int port, String name) {
/*  65 */     this.name = name;
/*     */     try {
/*  67 */       this.serv = new Server(ip, port);
/*  68 */       this.serv.addMessageListener(this);
/*  69 */       this.serv.connect();
/*  70 */       String[] nme = { "NME", 
/*  71 */         "(", "'" + name + "'", ")", 
/*  72 */         "(", "'" + VERSION + "'", ")" };
/*  73 */       printMsg(name, nme);
/*  74 */       this.serv.send(nme);
/*  75 */       BufferedReader br = 
/*  76 */         new BufferedReader(new InputStreamReader(System.in));
/*     */       while (true) {
/*  78 */         printPrompt();
/*  79 */         String line = br.readLine();
/*  80 */         String[] order = line.split(" ");
/*  81 */         this.atPrompt = false;
/*     */         try {
/*  83 */           printMsg(name, order);
/*  84 */           this.serv.send(order);
/*     */         } catch (UnknownTokenException ute) {
/*  86 */           System.err.println("Unknown token '" + ute.getToken() + 
/*  87 */             "' - Message not sent.");
/*     */         } catch (DisconnectedException de) {
/*  89 */           System.err.println("Disconnected from server, command not sent");
/*     */         }
/*     */       }
/*     */     }
/*     */     catch (IOException ioe) {
/*  94 */       ioe.printStackTrace();
/*     */     } catch (DisconnectedException de) {
/*  96 */       System.out.println("Ok, we're disconnected. Exiting...");
/*  97 */       System.exit(0);
/*     */     } catch (UnknownTokenException ute) {
/*  99 */       System.err.println("Unknown token '" + ute.getToken() + "'");
/* 100 */       System.exit(1);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void messageReceived(String[] message)
/*     */   {
/* 107 */     printMsg("srv", message);
/*     */ 
/* 109 */     if (message[0].equals("MAP")) {
/*     */       try {
/* 111 */         String[] tokens = new String[message.length + 3];
/* 112 */         tokens[0] = "YES";
/* 113 */         tokens[1] = "(";
/* 114 */         System.arraycopy(message, 0, tokens, 2, message.length);
/* 115 */         tokens[(tokens.length - 1)] = ")";
/* 116 */         printMsg(this.name, tokens);
/* 117 */         this.serv.send(tokens);
/*     */       } catch (UnknownTokenException ute) {
/* 119 */         ute.printStackTrace();
/* 120 */         System.exit(1);
/*     */       } catch (DisconnectedException de) {
/* 122 */         System.err.println("Disconnected, exiting");
/* 123 */         System.exit(1);
/*     */       }
/*     */     }
/* 126 */     printPrompt();
/*     */   }
/*     */ 
/*     */   public static void main(String[] args)
/*     */   {
/*     */     try {
/* 132 */       new TestAI(InetAddress.getByName(args[0]), 
/* 133 */         Integer.parseInt(args[1]), 
/* 134 */         args[2]);
/*     */     } catch (ArrayIndexOutOfBoundsException be) {
/* 136 */       usage();
/*     */     } catch (UnknownHostException uhe) {
/* 138 */       System.err.println("Unknown host: " + uhe.getMessage());
/*     */     } catch (NumberFormatException nfe) {
/* 140 */       usage();
/*     */     }
/*     */   }
/*     */ 
/*     */   public static void usage() {
/* 145 */     System.err.println("Usage:\n  TestAI <ip> <port> <name>");
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.demo.TestAI
 * JD-Core Version:    0.6.0
 */