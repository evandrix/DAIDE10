/*    */ package dip.daide.comm;
/*    */ 
/*    */ public class UnknownTokenException extends Exception
/*    */ {
/*    */   Object token;
/*    */ 
/*    */   public UnknownTokenException(Object token)
/*    */   {
/* 25 */     this.token = token;
/*    */   }
/*    */ 
/*    */   public Object getToken()
/*    */   {
/* 37 */     return this.token;
/*    */   }
/*    */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.comm.UnknownTokenException
 * JD-Core Version:    0.6.0
 */