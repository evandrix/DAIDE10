/*     */ package dip.daide.comm;
/*     */ 
/*     */ import java.io.IOException;
/*     */ import java.io.InputStream;
/*     */ import java.io.OutputStream;
/*     */ import java.io.PrintStream;
/*     */ import java.net.InetAddress;
/*     */ import java.net.Socket;
/*     */ import java.util.ArrayList;
/*     */ import java.util.Iterator;
/*     */ 
/*     */ public class Connection
/*     */ {
/*     */   private static final int IDLE = 0;
/*     */   private static final int SENDING = 256;
/*     */   private static final int RECEIVING = 512;
/*     */   private static final int HANDSHAKE = 1024;
/*     */   private static final int ERROR = 2048;
/*     */   private static final int CONN = 0;
/*     */   private static final int RECV = 16;
/*     */   private static final int SEND = 32;
/*     */   private static final int IM = 0;
/*     */   private static final int RM = 1;
/*     */   private static final int DM = 2;
/*     */   private static final int FM = 4;
/*     */   private static final int EM = 8;
/*     */   private static final int DM_SENT = 34;
/*     */   private static final int RM_RECEIVED = 17;
/*     */   private static final int DM_RECEIVED = 18;
/*     */   private static final int SENDING_FM = 292;
/*     */   private static final int SENDING_EM = 296;
/*     */   private static final int SENDING_DM = 290;
/*     */   private static final int RECEIVING_DM = 530;
/*     */   private static final int RECEIVING_EM = 536;
/*     */   private static final int RECEIVING_FM = 532;
/*     */   private static final int CONNECTING = 1024;
/*     */   private static final int CONNECTED = 1025;
/*     */   private static final int SENDING_IM = 1026;
/*     */   private static final int IM_SENT = 1028;
/*     */   private static final int WAITING_FOR_RM = 1032;
/*     */   private static final int EM_RECEIVED = 2072;
/*     */   private static final int EM_SENT = 2088;
/*     */   private static final int FM_RECEIVED = 2068;
/*     */   private static final int FM_SENT = 2068;
/*     */   private static final int DISCONNECTED = 2048;
/*     */   int state;
/*     */   OutputStream output;
/*     */   DiplomaticMessageListener listener;
/*     */ 
/*     */   public Connection(InetAddress ip, int port, DiplomaticMessageListener listener)
/*     */     throws IOException, DisconnectedException
/*     */   {
/* 103 */     this.listener = listener;
/* 104 */     connect(ip, port);
/*     */   }
/*     */ 
/*     */   void connect(InetAddress ip, int port)
/*     */     throws IOException, DisconnectedException
/*     */   {
/* 126 */     setState(1024);
/* 127 */     Socket socket = new Socket(ip, port);
/* 128 */     Consumer consumer = new Consumer(socket.getInputStream());
/* 129 */     consumer.start();
/* 130 */     this.output = socket.getOutputStream();
/* 131 */     setState(1025);
/*     */ 
/* 134 */     byte[] im = createInitialMessage();
/* 135 */     send(1026, im, 1028);
/*     */ 
/* 138 */     setState(1032);
/* 139 */     waitForState(17);
/*     */   }
/*     */ 
/*     */   public void disconnect()
/*     */     throws IOException, DisconnectedException
/*     */   {
/* 151 */     byte[] fm = createFinalMessage();
/* 152 */     send(292, fm, 2068);
/* 153 */     setState(2048);
/*     */   }
/*     */ 
/*     */   public void send(String[] message)
/*     */     throws UnknownTokenException, DisconnectedException
/*     */   {
/* 176 */     byte[] dm = createDiplomaticMessage(convert(message));
/* 177 */     send(290, dm, 34);
/*     */   }
/*     */ 
/*     */   void send(int preState, byte[] message, int postState)
/*     */     throws DisconnectedException
/*     */   {
/* 193 */     synchronized (this.output) {
/* 194 */       if ((this.state & 0x800) > 0)
/* 195 */         throw new DisconnectedException();
/*     */       try
/*     */       {
/* 198 */         setState(preState);
/*     */ 
/* 209 */         this.output.write(message);
/* 210 */         setState(postState);
/*     */       } catch (IOException ioe) {
/* 212 */         ioe.printStackTrace();
/* 213 */         setState(2048);
/*     */       }
/*     */     }
/*     */   }
/*     */ 
/*     */   byte[] createInitialMessage()
/*     */   {
/* 225 */     return 
/* 226 */       new byte[] { 
/* 228 */       0, 0, 0, 4, 
/* 229 */       0, 1, 
/* 230 */       -38, 16 };
/*     */   }
/*     */ 
/*     */   byte[] createDiplomaticMessage(byte[] data)
/*     */   {
/* 241 */     byte[] bytes = new byte[data.length + 4];
/* 242 */     bytes[0] = 2;
/* 243 */     bytes[1] = 0;
/* 244 */     bytes[2] = (byte)(data.length >> 8);
/* 245 */     bytes[3] = (byte)(data.length & 0xFF);
/* 246 */     System.arraycopy(data, 0, bytes, 
/* 247 */       4, data.length);
/* 248 */     return bytes;
/*     */   }
/*     */ 
/*     */   byte[] createErrorMessage(byte code)
/*     */   {
/* 258 */     return 
/* 259 */       new byte[] { 4, 
/* 261 */       0, 0, 2, 
/* 262 */       0, code };
/*     */   }
/*     */ 
/*     */   byte[] createFinalMessage()
/*     */   {
/* 272 */     return 
/* 273 */       new byte[] { 3 };
/*     */   }
/*     */ 
/*     */   void handleMessage(byte type, byte[] message)
/*     */   {
/* 288 */     switch (type) {
/*     */     case 1:
/* 290 */       handleRepresentationMessage(message);
/* 291 */       break;
/*     */     case 2:
/* 293 */       handleDiplomaticMessage(message);
/* 294 */       break;
/*     */     case 3:
/* 296 */       handleFinalMessage();
/* 297 */       break;
/*     */     case 4:
/* 299 */       handleErrorMessage(message);
/* 300 */       break;
/*     */     default:
/* 302 */       unhandledMessage(type, message);
/*     */     }
/*     */   }
/*     */ 
/*     */   void unhandledMessage(int type, byte[] message)
/*     */   {
/*     */   }
/*     */ 
/*     */   void handleRepresentationMessage(byte[] message)
/*     */   {
/* 330 */     for (int i = 0; i < message.length; i += 6) {
/* 331 */       byte[] bits = { message[i], message[(i + 1)] };
/* 332 */       byte[] token = { message[(i + 2)], 
/* 333 */         message[(i + 3)], 
/* 334 */         message[(i + 4)] };
/* 335 */       Token.add(bits, new String(token));
/*     */     }
/*     */ 
/* 338 */     setState(17);
/*     */   }
/*     */ 
/*     */   void handleDiplomaticMessage(byte[] message)
/*     */   {
/* 358 */     String[] tokens = convert(message);
/* 359 */     this.listener.diplomaticMessageReceived(tokens);
/* 360 */     setState(18);
/*     */   }
/*     */ 
/*     */   void handleFinalMessage()
/*     */   {
/* 369 */     setState(2068);
/*     */   }
/*     */ 
/*     */   void handleErrorMessage(byte[] message)
/*     */   {
/* 382 */     String detail = "Server reports: ";
/* 383 */     switch (message[1]) {
/*     */     case 1:
/* 385 */       detail = detail + "IM timer popped";
/* 386 */       break;
/*     */     case 2:
/* 388 */       detail = detail + "IM was not the first message sent by the client";
/* 389 */       break;
/*     */     case 3:
/* 391 */       detail = detail + "IM indicated the wrong endian";
/* 392 */       break;
/*     */     case 4:
/* 394 */       detail = detail + "IM had an incorrect magic number";
/* 395 */       break;
/*     */     case 5:
/* 397 */       detail = detail + "Version incompatibility";
/* 398 */       break;
/*     */     case 6:
/* 400 */       detail = detail + "More than 1 IM sent";
/* 401 */       break;
/*     */     case 7:
/* 403 */       detail = detail + "IM sent by server";
/* 404 */       break;
/*     */     case 8:
/* 406 */       detail = detail + "Unknown message received";
/* 407 */       break;
/*     */     case 9:
/* 409 */       detail = detail + "Message shorter than expected";
/* 410 */       break;
/*     */     case 10:
/* 412 */       detail = detail + "DM sent before RM";
/* 413 */       break;
/*     */     case 11:
/* 415 */       detail = detail + "RM was not the first message sent by the server";
/* 416 */       break;
/*     */     case 12:
/* 418 */       detail = detail + "More than 1 RM sent";
/* 419 */       break;
/*     */     case 13:
/* 421 */       detail = detail + "RM sent by client";
/* 422 */       break;
/*     */     case 14:
/* 424 */       detail = detail + "Invalid token in DM";
/* 425 */       break;
/*     */     default:
/* 427 */       detail = "[Unknown error code " + message[0] + message[1] + "]";
/*     */     }
/* 429 */     System.out.println(detail);
/* 430 */     setState(2072);
/*     */   }
/*     */ 
/*     */   synchronized void setState(int newState)
/*     */   {
/* 442 */     this.state = newState;
/* 443 */     if ((this.state & 0x800) != 0) {
/* 444 */       System.err.println("Entering state 0x" + 
/* 445 */         Integer.toHexString(newState) + 
/* 446 */         " which is an error state.");
/*     */     }
/* 448 */     notifyAll();
/*     */   }
/*     */ 
/*     */   synchronized void waitForState(int targetState)
/*     */   {
/* 458 */     while ((this.state & targetState) != targetState)
/*     */       try {
/* 460 */         wait();
/*     */       }
/*     */       catch (InterruptedException localInterruptedException)
/*     */       {
/*     */       }
/*     */   }
/*     */ 
/*     */   String[] convert(byte[] bits)
/*     */   {
/* 475 */     StringBuffer str = null;
/*     */ 
/* 477 */     boolean inString = false;
/* 478 */     ArrayList list = new ArrayList(bits.length / 2);
/*     */ 
/* 480 */     for (int i = 0; i < bits.length; i += 2)
/*     */     {
/* 482 */       if (bits[i] == 75) {
/* 483 */         if (!inString) {
/* 484 */           str = new StringBuffer();
/* 485 */           str.append('\'');
/* 486 */           inString = true;
/*     */         }
/* 488 */         str.append((char)bits[(i + 1)]);
/*     */       }
/*     */       else
/*     */       {
/* 496 */         if (inString) {
/* 497 */           str.append('\'');
/* 498 */           list.add(str.toString());
/* 499 */           str = null;
/* 500 */           inString = false;
/*     */         }
/*     */         try {
/* 503 */           list.add(Token.convert(new byte[] { bits[i], bits[(i + 1)] }));
/*     */         } catch (UnknownTokenException ute) {
/* 505 */           if ((list.size() > 0) && 
/* 506 */             (((String)list.get(0)).equals("HUH"))) {
/* 507 */             list.add(Integer.toHexString(bits[i]) + 
/* 508 */               "," + 
/* 509 */               Integer.toHexString(bits[(i + 1)]));
/*     */           } else {
/* 511 */             System.err.println("Unknown token encountered after these tokens:");
/*     */ 
/* 513 */             Iterator iter = list.iterator();
/* 514 */             while (iter.hasNext()) {
/* 515 */               System.err.print(iter.next() + " ");
/*     */             }
/* 517 */             System.err.println();
/*     */             try
/*     */             {
/* 522 */               byte[] errMessage = new byte[bits.length + 8];
/* 523 */               byte[] HUH = Token.convert("HUH");
/* 524 */               errMessage[0] = HUH[0];
/* 525 */               errMessage[1] = HUH[1];
/* 526 */               byte[] PAR = Token.convert("(");
/* 527 */               errMessage[2] = PAR[0];
/* 528 */               errMessage[3] = PAR[1];
/* 529 */               System.arraycopy(bits, 0, errMessage, 4, i);
/* 530 */               byte[] ERR = Token.convert("ERR");
/* 531 */               errMessage[(i + 4)] = ERR[0];
/* 532 */               errMessage[(i + 5)] = ERR[1];
/* 533 */               System.arraycopy(bits, i, errMessage, i + 6, bits.length - i);
/* 534 */               PAR = Token.convert(")");
/* 535 */               errMessage[(bits.length + 6)] = PAR[0];
/* 536 */               errMessage[(bits.length + 7)] = PAR[1];
/*     */ 
/* 538 */               byte[] dm = createDiplomaticMessage(errMessage);
/*     */               try
/*     */               {
/* 541 */                 send(290, dm, 34);
/*     */               }
/*     */               catch (DisconnectedException e) {
/* 544 */                 e.printStackTrace();
/*     */               }
/*     */             } catch (UnknownTokenException localUnknownTokenException1) {
/*     */             }
/* 548 */             list.add("UNKNOWN");
/*     */           }
/*     */         }
/*     */       }
/*     */     }
/* 553 */     return (String[])list.toArray(new String[0]);
/*     */   }
/*     */ 
/*     */   byte[] convert(String[] tokens)
/*     */     throws UnknownTokenException
/*     */   {
/* 567 */     int numTokens = tokens.length;
/* 568 */     byte[] bytes = new byte[numTokens * 2];
/* 569 */     int t = 0;
/* 570 */     for (int i = 0; i < numTokens; i++) {
/* 571 */       String token = tokens[i];
/*     */       byte[] bits;
/* 573 */       if (token.startsWith("'")) {
/* 574 */         byte[] bits = Token.convertText(token);
/* 575 */         if (bits.length != 1) {
/* 576 */           int newSize = bytes.length + bits.length - 2;
/* 577 */           byte[] newArray = new byte[newSize];
/* 578 */           System.arraycopy(bytes, 0, newArray, 0, t);
/* 579 */           bytes = newArray;
/*     */         }
/*     */       } else {
/* 582 */         bits = Token.convert(token);
/*     */       }
/* 584 */       System.arraycopy(bits, 0, bytes, t, bits.length);
/* 585 */       t += bits.length;
/*     */     }
/* 587 */     return bytes;
/*     */   }
/*     */ 
/*     */   public class Consumer extends Thread
/*     */   {
/*     */     InputStream input;
/*     */ 
/*     */     public Consumer(InputStream in)
/*     */     {
/* 609 */       this.input = in;
/*     */     }
/*     */ 
/*     */     public void run()
/*     */     {
/* 623 */       while ((Connection.this.state & 0x800) == 0) {
/*     */         try {
/* 625 */           byte[] msgType = new byte[1];
/* 626 */           byte[] length_hi = new byte[1];
/* 627 */           byte[] length_lo = new byte[1];
/*     */ 
/* 629 */           while (this.input.read(msgType) == 0);
/* 631 */           while (this.input.skip(1L) == 0L);
/* 632 */           while (this.input.read(length_hi) == 0);
/* 633 */           while (this.input.read(length_lo) == 0);
/* 635 */           int length = length_hi[0] & 0xFF;
/* 636 */           length <<= 8;
/* 637 */           length += (length_lo[0] & 0xFF);
/*     */ 
/* 639 */           byte[] msg = new byte[length];
/* 640 */           int num = 0;
/*     */ 
/* 642 */           while (num != length) {
/* 643 */             int rd = this.input.read(msg, num, length - num);
/* 644 */             if (rd == -1) {
/* 645 */               System.err.println("EOF reading from server, disconnecting");
/*     */ 
/* 647 */               Connection.this.setState(2048);
/* 648 */               break;
/*     */             }
/* 650 */             num += rd;
/*     */           }
/*     */ 
/* 653 */           if ((Connection.this.state & 0x800) == 0)
/* 654 */             Connection.this.handleMessage(msgType[0], msg);
/*     */         }
/*     */         catch (IOException ioe) {
/* 657 */           ioe.printStackTrace();
/* 658 */           Connection.this.setState(2048);
/*     */         }
/*     */ 
/*     */       }
/*     */ 
/* 663 */       System.err.println("Disconnected from server, exiting");
/*     */     }
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.comm.Connection
 * JD-Core Version:    0.6.0
 */