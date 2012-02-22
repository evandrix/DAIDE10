/*     */ package dip.daide.comm;
/*     */ 
/*     */ import java.io.IOException;
/*     */ import java.net.InetAddress;
/*     */ import java.util.Iterator;
/*     */ import java.util.LinkedList;
/*     */ 
/*     */ public class Server
/*     */   implements DiplomaticMessageListener
/*     */ {
/*     */   Connection conn;
/*     */   int port;
/*     */   InetAddress ip;
/*     */   LinkedList listeners;
/*     */ 
/*     */   public Server(InetAddress ip, int port)
/*     */   {
/*  48 */     this.ip = ip;
/*  49 */     this.port = port;
/*  50 */     this.listeners = new LinkedList();
/*     */   }
/*     */ 
/*     */   public void connect()
/*     */     throws IOException, DisconnectedException
/*     */   {
/*  61 */     this.conn = new Connection(this.ip, this.port, this);
/*     */   }
/*     */ 
/*     */   public void addMessageListener(MessageListener listener)
/*     */   {
/*  72 */     synchronized (this.listeners) {
/*  73 */       this.listeners.add(listener);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void diplomaticMessageReceived(String[] message)
/*     */   {
/*  83 */     synchronized (this.listeners) {
/*  84 */       Iterator iter = this.listeners.iterator();
/*  85 */       while (iter.hasNext()) {
/*  86 */         MessageListener ml = (MessageListener)iter.next();
/*  87 */         ml.messageReceived(message);
/*     */       }
/*     */     }
/*     */   }
/*     */ 
/*     */   public void send(String[] msg)
/*     */     throws UnknownTokenException, DisconnectedException
/*     */   {
/* 102 */     this.conn.send(msg);
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.comm.Server
 * JD-Core Version:    0.6.0
 */