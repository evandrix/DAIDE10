/*    */ package dip.daide.us;
/*    */ 
/*    */ import java.io.PrintStream;
/*    */ import java.util.Scanner;
/*    */ 
/*    */ public class TestMap
/*    */ {
/*    */   public static void main(String[] args)
/*    */   {
/*  7 */     Scanner in = new Scanner(System.in);
/*    */ 
/*  9 */     String[] message = new String[10000];
/* 10 */     int i = 0;
/* 11 */     boolean loop = true;
/* 12 */     while ((in.hasNext()) && (loop)) {
/* 13 */       message[i] = in.next();
/* 14 */       if (message[i].equals("EOF")) loop = false;
/* 15 */       i++;
/*    */     }
/* 17 */     System.out.println("reached");
/* 18 */     for (int j = 0; j < message.length; j++) {
/* 19 */       System.out.print(" " + message[j]);
/*    */     }
/*    */ 
/* 22 */     Map hat = new Map(message);
/*    */   }
/*    */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.us.TestMap
 * JD-Core Version:    0.6.0
 */