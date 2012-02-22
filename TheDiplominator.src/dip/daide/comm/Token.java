/*     */ package dip.daide.comm;
/*     */ 
/*     */ import java.io.PrintStream;
/*     */ import java.io.UnsupportedEncodingException;
/*     */ import java.util.HashMap;
/*     */ import java.util.Iterator;
/*     */ import java.util.LinkedList;
/*     */ import java.util.Map;
/*     */ import java.util.Map.Entry;
/*     */ import java.util.Set;
/*     */ import java.util.StringTokenizer;
/*     */ 
/*     */ public final class Token
/*     */ {
/*     */   public static final byte MISC = 64;
/*     */   public static final byte POWERS = 65;
/*     */   public static final byte UNIT_TYPES = 66;
/*     */   public static final byte ORDERS = 67;
/*     */   public static final byte ORDER_NOTES = 68;
/*     */   public static final byte RESULTS = 69;
/*     */   public static final byte COASTS = 70;
/*     */   public static final byte PHASES = 71;
/*     */   public static final byte COMMANDS = 72;
/*     */   public static final byte PARAMETERS = 73;
/*     */   public static final byte PRESS = 74;
/*     */   public static final byte TEXT = 75;
/*     */   public static final byte INLAND_NON_SC = 80;
/*     */   public static final byte INLAND_SC = 81;
/*     */   public static final byte SEA_NON_SC = 82;
/*     */   public static final byte SEA_SC = 83;
/*     */   public static final byte COASTAL_NON_SC = 84;
/*     */   public static final byte COASTAL_SC = 85;
/*     */   public static final byte BI_COASTAL_NON_SC = 86;
/*     */   public static final byte BI_COASTAL_SC = 87;
/*  59 */   private static final Map tokenMap = createTokenMap();
/*  60 */   private static final Map bitsMap = new HashMap();
/*     */ 
/*  61 */   static { Iterator iter = tokenMap.entrySet().iterator();
/*     */ 
/*  63 */     while (iter.hasNext()) {
/*  64 */       Map.Entry entry = (Map.Entry)(Map.Entry)iter.next();
/*  65 */       byte[] bytes = (byte[])entry.getValue();
/*  66 */       bitsMap.put(new ByteArrayEntry(bytes[0], bytes[1]), 
/*  67 */         entry.getKey());
/*     */     }
/*  69 */     if (tokenMap.size() != bitsMap.size())
/*  70 */       System.err.println(Token.class.getName() + 
/*  71 */         ": tokenMap does not contain an 1-1 mapping!");
/*     */   }
/*     */ 
/*     */   public static void add(byte[] bits, String mnemonic)
/*     */   {
/*  94 */     ByteArrayEntry entry = new ByteArrayEntry(bits[0], bits[1]);
/*  95 */     bitsMap.put(entry, mnemonic);
/*  96 */     tokenMap.put(mnemonic, bits);
/*     */   }
/*     */ 
/*     */   public static String convert(byte[] bits)
/*     */     throws UnknownTokenException
/*     */   {
/* 114 */     if ((bits[0] & 0xC0) == 0) {
/* 115 */       int i = bits[0];
/* 116 */       if (bits[0] < 0) i += 256;
/* 117 */       i <<= 8;
/* 118 */       i += bits[1];
/* 119 */       if (bits[1] < 0) i += 256;
/* 120 */       return Integer.toString(i);
/*     */     }
/*     */ 
/* 123 */     String token = (String)bitsMap.get(
/* 124 */       new ByteArrayEntry(bits[0], 
/* 124 */       bits[1]));
/* 125 */     if (token == null) {
/* 126 */       throw new UnknownTokenException(bits);
/*     */     }
/* 128 */     return token;
/*     */   }
/*     */ 
/*     */   public static byte[] convert(String token)
/*     */     throws UnknownTokenException
/*     */   {
/* 146 */     byte[] bytes = (byte[])tokenMap.get(token);
/* 147 */     if (bytes == null) {
/* 148 */       throw new UnknownTokenException(token);
/*     */     }
/* 150 */     return bytes;
/*     */   }
/*     */ 
/*     */   public static byte[] convertText(String text)
/*     */   {
/*     */     try
/*     */     {
/* 163 */       byte[] bytes = text.getBytes("US-ASCII");
/* 164 */       if ((bytes.length < 2) || 
/* 165 */         (bytes[0] != 39) || 
/* 166 */         (bytes[(bytes.length - 1)] != 39)) {
/* 167 */         throw new IllegalArgumentException("String not surrounded by single quotes");
/*     */       }
/*     */ 
/* 171 */       byte[] res = new byte[(bytes.length - 2) * 2];
/* 172 */       for (int i = 0; i < res.length; i += 2) {
/* 173 */         res[i] = 75;
/* 174 */         res[(i + 1)] = bytes[(i / 2 + 1)];
/*     */       }
/* 176 */       return res;
/*     */     } catch (UnsupportedEncodingException uee) {
/* 178 */       uee.printStackTrace();
/* 179 */       System.exit(1);
/* 180 */     }return null;
/*     */   }
/*     */ 
/*     */   private static Map createTokenMap()
/*     */   {
/* 192 */     HashMap map = new HashMap();
/*     */ 
/* 194 */     map.put("(", new byte[] { 64 });
/* 195 */     map.put(")", new byte[] { 64, 1 });
/*     */ 
/* 197 */     map.put("AUS", new byte[] { 65 });
/* 198 */     map.put("ENG", new byte[] { 65, 1 });
/* 199 */     map.put("FRA", new byte[] { 65, 2 });
/* 200 */     map.put("GER", new byte[] { 65, 3 });
/* 201 */     map.put("ITA", new byte[] { 65, 4 });
/* 202 */     map.put("RUS", new byte[] { 65, 5 });
/* 203 */     map.put("TUR", new byte[] { 65, 6 });
/*     */ 
/* 205 */     map.put("AMY", new byte[] { 66 });
/* 206 */     map.put("FLT", new byte[] { 66, 1 });
/*     */ 
/* 208 */     map.put("CTO", new byte[] { 67, 32 });
/* 209 */     map.put("CVY", new byte[] { 67, 33 });
/* 210 */     map.put("HLD", new byte[] { 67, 34 });
/* 211 */     map.put("MTO", new byte[] { 67, 35 });
/* 212 */     map.put("SUP", new byte[] { 67, 36 });
/* 213 */     map.put("VIA", new byte[] { 67, 37 });
/* 214 */     map.put("DSB", new byte[] { 67, 64 });
/* 215 */     map.put("RTO", new byte[] { 67, 65 });
/* 216 */     map.put("BLD", new byte[] { 67, -128 });
/* 217 */     map.put("REM", new byte[] { 67, -127 });
/* 218 */     map.put("WVE", new byte[] { 67, -126 });
/*     */ 
/* 220 */     map.put("MBV", new byte[] { 68 });
/* 221 */     map.put("BPR", new byte[] { 68, 1 });
/* 222 */     map.put("CST", new byte[] { 68, 2 });
/* 223 */     map.put("ESC", new byte[] { 68, 3 });
/* 224 */     map.put("FAR", new byte[] { 68, 4 });
/* 225 */     map.put("HSC", new byte[] { 68, 5 });
/* 226 */     map.put("NAS", new byte[] { 68, 6 });
/* 227 */     map.put("NMB", new byte[] { 68, 7 });
/* 228 */     map.put("NMR", new byte[] { 68, 8 });
/* 229 */     map.put("NRN", new byte[] { 68, 9 });
/* 230 */     map.put("NRS", new byte[] { 68, 10 });
/* 231 */     map.put("NSA", new byte[] { 68, 11 });
/* 232 */     map.put("NSC", new byte[] { 68, 12 });
/* 233 */     map.put("NSF", new byte[] { 68, 13 });
/* 234 */     map.put("NSP", new byte[] { 68, 14 });
/* 235 */     map.put("NST", new byte[] { 68, 15 });
/* 236 */     map.put("NSU", new byte[] { 68, 16 });
/* 237 */     map.put("NVR", new byte[] { 68, 17 });
/* 238 */     map.put("NYU", new byte[] { 68, 18 });
/* 239 */     map.put("YSC", new byte[] { 68, 19 });
/*     */ 
/* 241 */     map.put("SUC", new byte[] { 69 });
/* 242 */     map.put("BNC", new byte[] { 69, 1 });
/* 243 */     map.put("CUT", new byte[] { 69, 2 });
/* 244 */     map.put("DSR", new byte[] { 69, 3 });
/* 245 */     map.put("FLD", new byte[] { 69, 4 });
/* 246 */     map.put("NSO", new byte[] { 69, 5 });
/* 247 */     map.put("RET", new byte[] { 69, 6 });
/*     */ 
/* 249 */     map.put("NCS", new byte[] { 70 });
/* 250 */     map.put("NEC", new byte[] { 70, 2 });
/* 251 */     map.put("ECS", new byte[] { 70, 4 });
/* 252 */     map.put("SEC", new byte[] { 70, 6 });
/* 253 */     map.put("SCS", new byte[] { 70, 8 });
/* 254 */     map.put("SWC", new byte[] { 70, 10 });
/* 255 */     map.put("WCS", new byte[] { 70, 12 });
/* 256 */     map.put("NWC", new byte[] { 70, 14 });
/*     */ 
/* 258 */     map.put("SPR", new byte[] { 71 });
/* 259 */     map.put("SUM", new byte[] { 71, 1 });
/* 260 */     map.put("FAL", new byte[] { 71, 2 });
/* 261 */     map.put("AUT", new byte[] { 71, 3 });
/* 262 */     map.put("WIN", new byte[] { 71, 4 });
/*     */ 
/* 264 */     map.put("CCD", new byte[] { 72 });
/* 265 */     map.put("DRW", new byte[] { 72, 1 });
/* 266 */     map.put("FRM", new byte[] { 72, 2 });
/* 267 */     map.put("GOF", new byte[] { 72, 3 });
/* 268 */     map.put("HLO", new byte[] { 72, 4 });
/* 269 */     map.put("HST", new byte[] { 72, 5 });
/* 270 */     map.put("HUH", new byte[] { 72, 6 });
/* 271 */     map.put("IAM", new byte[] { 72, 7 });
/* 272 */     map.put("LOD", new byte[] { 72, 8 });
/* 273 */     map.put("MAP", new byte[] { 72, 9 });
/* 274 */     map.put("MDF", new byte[] { 72, 10 });
/* 275 */     map.put("MIS", new byte[] { 72, 11 });
/* 276 */     map.put("NME", new byte[] { 72, 12 });
/* 277 */     map.put("NOT", new byte[] { 72, 13 });
/* 278 */     map.put("NOW", new byte[] { 72, 14 });
/* 279 */     map.put("OBS", new byte[] { 72, 15 });
/* 280 */     map.put("OFF", new byte[] { 72, 16 });
/* 281 */     map.put("ORD", new byte[] { 72, 17 });
/* 282 */     map.put("OUT", new byte[] { 72, 18 });
/* 283 */     map.put("PRN", new byte[] { 72, 19 });
/* 284 */     map.put("REJ", new byte[] { 72, 20 });
/* 285 */     map.put("SCO", new byte[] { 72, 21 });
/* 286 */     map.put("SLO", new byte[] { 72, 22 });
/* 287 */     map.put("SND", new byte[] { 72, 23 });
/* 288 */     map.put("SUB", new byte[] { 72, 24 });
/* 289 */     map.put("SVE", new byte[] { 72, 25 });
/* 290 */     map.put("THX", new byte[] { 72, 26 });
/* 291 */     map.put("TME", new byte[] { 72, 27 });
/* 292 */     map.put("YES", new byte[] { 72, 28 });
/* 293 */     map.put("ADM", new byte[] { 72, 29 });
/* 294 */     map.put("SMR", new byte[] { 72, 30 });
/*     */ 
/* 296 */     map.put("AOA", new byte[] { 73 });
/* 297 */     map.put("BTL", new byte[] { 73, 1 });
/* 298 */     map.put("ERR", new byte[] { 73, 2 });
/* 299 */     map.put("LVL", new byte[] { 73, 3 });
/* 300 */     map.put("MRT", new byte[] { 73, 4 });
/* 301 */     map.put("MTL", new byte[] { 73, 5 });
/* 302 */     map.put("NPB", new byte[] { 73, 6 });
/* 303 */     map.put("NPR", new byte[] { 73, 7 });
/* 304 */     map.put("PDA", new byte[] { 73, 8 });
/* 305 */     map.put("PTL", new byte[] { 73, 9 });
/* 306 */     map.put("RTL", new byte[] { 73, 10 });
/* 307 */     map.put("UNO", new byte[] { 73, 11 });
/* 308 */     map.put("DSD", new byte[] { 73, 13 });
/*     */ 
/* 310 */     map.put("ALY", new byte[] { 74 });
/* 311 */     map.put("AND", new byte[] { 74, 1 });
/* 312 */     map.put("BWX", new byte[] { 74, 2 });
/* 313 */     map.put("DMZ", new byte[] { 74, 3 });
/* 314 */     map.put("ELS", new byte[] { 74, 4 });
/* 315 */     map.put("EXP", new byte[] { 74, 5 });
/* 316 */     map.put("FWD", new byte[] { 74, 6 });
/* 317 */     map.put("FCT", new byte[] { 74, 7 });
/* 318 */     map.put("FOR", new byte[] { 74, 8 });
/* 319 */     map.put("HOW", new byte[] { 74, 9 });
/* 320 */     map.put("IDK", new byte[] { 74, 10 });
/* 321 */     map.put("IFF", new byte[] { 74, 11 });
/* 322 */     map.put("INS", new byte[] { 74, 12 });
/* 323 */     map.put("IOU", new byte[] { 74, 13 });
/* 324 */     map.put("OCC", new byte[] { 74, 14 });
/* 325 */     map.put("ORR", new byte[] { 74, 15 });
/* 326 */     map.put("PCE", new byte[] { 74, 16 });
/* 327 */     map.put("POB", new byte[] { 74, 17 });
/* 328 */     map.put("PPT", new byte[] { 74, 18 });
/* 329 */     map.put("PRP", new byte[] { 74, 19 });
/* 330 */     map.put("QRY", new byte[] { 74, 20 });
/* 331 */     map.put("SCD", new byte[] { 74, 21 });
/* 332 */     map.put("SRY", new byte[] { 74, 22 });
/* 333 */     map.put("SUG", new byte[] { 74, 23 });
/* 334 */     map.put("THK", new byte[] { 74, 24 });
/* 335 */     map.put("THN", new byte[] { 74, 25 });
/* 336 */     map.put("TRY", new byte[] { 74, 26 });
/* 337 */     map.put("UOM", new byte[] { 74, 27 });
/* 338 */     map.put("VSS", new byte[] { 74, 28 });
/* 339 */     map.put("WHT", new byte[] { 74, 29 });
/* 340 */     map.put("WHY", new byte[] { 74, 30 });
/* 341 */     map.put("XDO", new byte[] { 74, 31 });
/* 342 */     map.put("XOY", new byte[] { 74, 32 });
/* 343 */     map.put("YDO", new byte[] { 74, 33 });
/* 344 */     map.put("CHO", new byte[] { 74, 34 });
/* 345 */     map.put("BCC", new byte[] { 74, 35 });
/* 346 */     map.put("UNT", new byte[] { 74, 36 });
/*     */ 
/* 348 */     map.put("BOH", new byte[] { 80 });
/* 349 */     map.put("BUR", new byte[] { 80, 1 });
/* 350 */     map.put("GAL", new byte[] { 80, 2 });
/* 351 */     map.put("RUH", new byte[] { 80, 3 });
/* 352 */     map.put("SIL", new byte[] { 80, 4 });
/* 353 */     map.put("TYR", new byte[] { 80, 5 });
/* 354 */     map.put("UKR", new byte[] { 80, 6 });
/*     */ 
/* 356 */     map.put("BUD", new byte[] { 81, 7 });
/* 357 */     map.put("MOS", new byte[] { 81, 8 });
/* 358 */     map.put("MUN", new byte[] { 81, 9 });
/* 359 */     map.put("PAR", new byte[] { 81, 10 });
/* 360 */     map.put("SER", new byte[] { 81, 11 });
/* 361 */     map.put("VIE", new byte[] { 81, 12 });
/* 362 */     map.put("WAR", new byte[] { 81, 13 });
/*     */ 
/* 364 */     map.put("ADR", new byte[] { 82, 14 });
/* 365 */     map.put("AEG", new byte[] { 82, 15 });
/* 366 */     map.put("BAL", new byte[] { 82, 16 });
/* 367 */     map.put("BAR", new byte[] { 82, 17 });
/* 368 */     map.put("BLA", new byte[] { 82, 18 });
/* 369 */     map.put("EAS", new byte[] { 82, 19 });
/* 370 */     map.put("ECH", new byte[] { 82, 20 });
/* 371 */     map.put("GOB", new byte[] { 82, 21 });
/* 372 */     map.put("GOL", new byte[] { 82, 22 });
/* 373 */     map.put("HEL", new byte[] { 82, 23 });
/* 374 */     map.put("ION", new byte[] { 82, 24 });
/* 375 */     map.put("IRI", new byte[] { 82, 25 });
/* 376 */     map.put("MAO", new byte[] { 82, 26 });
/* 377 */     map.put("NAO", new byte[] { 82, 27 });
/* 378 */     map.put("NTH", new byte[] { 82, 28 });
/* 379 */     map.put("NWG", new byte[] { 82, 29 });
/* 380 */     map.put("SKA", new byte[] { 82, 30 });
/* 381 */     map.put("TYS", new byte[] { 82, 31 });
/* 382 */     map.put("WES", new byte[] { 82, 32 });
/*     */ 
/* 384 */     map.put("ALB", new byte[] { 84, 33 });
/* 385 */     map.put("APU", new byte[] { 84, 34 });
/* 386 */     map.put("ARM", new byte[] { 84, 35 });
/* 387 */     map.put("CLY", new byte[] { 84, 36 });
/* 388 */     map.put("FIN", new byte[] { 84, 37 });
/* 389 */     map.put("GAS", new byte[] { 84, 38 });
/* 390 */     map.put("LVN", new byte[] { 84, 39 });
/* 391 */     map.put("NAF", new byte[] { 84, 40 });
/* 392 */     map.put("PIC", new byte[] { 84, 41 });
/* 393 */     map.put("PIE", new byte[] { 84, 42 });
/* 394 */     map.put("PRU", new byte[] { 84, 43 });
/* 395 */     map.put("SYR", new byte[] { 84, 44 });
/* 396 */     map.put("TUS", new byte[] { 84, 45 });
/* 397 */     map.put("WAL", new byte[] { 84, 46 });
/* 398 */     map.put("YOR", new byte[] { 84, 47 });
/*     */ 
/* 400 */     map.put("ANK", new byte[] { 85, 48 });
/* 401 */     map.put("BEL", new byte[] { 85, 49 });
/* 402 */     map.put("BER", new byte[] { 85, 50 });
/* 403 */     map.put("BRE", new byte[] { 85, 51 });
/* 404 */     map.put("CON", new byte[] { 85, 52 });
/* 405 */     map.put("DEN", new byte[] { 85, 53 });
/* 406 */     map.put("EDI", new byte[] { 85, 54 });
/* 407 */     map.put("GRE", new byte[] { 85, 55 });
/* 408 */     map.put("HOL", new byte[] { 85, 56 });
/* 409 */     map.put("KIE", new byte[] { 85, 57 });
/* 410 */     map.put("LON", new byte[] { 85, 58 });
/* 411 */     map.put("LVP", new byte[] { 85, 59 });
/* 412 */     map.put("MAR", new byte[] { 85, 60 });
/* 413 */     map.put("NAP", new byte[] { 85, 61 });
/* 414 */     map.put("NWY", new byte[] { 85, 62 });
/* 415 */     map.put("POR", new byte[] { 85, 63 });
/* 416 */     map.put("ROM", new byte[] { 85, 64 });
/* 417 */     map.put("RUM", new byte[] { 85, 65 });
/* 418 */     map.put("SEV", new byte[] { 85, 66 });
/* 419 */     map.put("SMY", new byte[] { 85, 67 });
/* 420 */     map.put("SWE", new byte[] { 85, 68 });
/* 421 */     map.put("TRI", new byte[] { 85, 69 });
/* 422 */     map.put("TUN", new byte[] { 85, 70 });
/* 423 */     map.put("VEN", new byte[] { 85, 71 });
/*     */ 
/* 425 */     map.put("BUL", new byte[] { 87, 72 });
/* 426 */     map.put("SPA", new byte[] { 87, 73 });
/* 427 */     map.put("STP", new byte[] { 87, 74 });
/* 428 */     return map;
/*     */   }
/*     */ 
/*     */   public static void main(String[] args) {
/* 432 */     if (args.length != 2) {
/* 433 */       System.err.println("Usage:\n  Token -s <string>");
/*     */ 
/* 435 */       System.exit(1);
/*     */     }
/* 437 */     if (args[0].equals("-s"))
/*     */       try {
/* 439 */         LinkedList list = new LinkedList();
/* 440 */         StringTokenizer wsTok = new StringTokenizer(args[1], " ");
/* 441 */         while (wsTok.hasMoreTokens()) {
/* 442 */           String s = wsTok.nextToken();
/* 443 */           StringTokenizer parenTok = 
/* 444 */             new StringTokenizer(s, "()", true);
/* 445 */           while (parenTok.hasMoreTokens()) {
/* 446 */             list.add(parenTok.nextToken());
/*     */           }
/*     */         }
/* 449 */         int numTokens = list.size();
/* 450 */         Iterator iter = list.iterator();
/* 451 */         System.out.println("Converting " + numTokens + " tokens.");
/* 452 */         byte[] bytes = new byte[numTokens * 2];
/* 453 */         int t = 0;
/* 454 */         while (iter.hasNext()) {
/* 455 */           String token = (String)iter.next();
/*     */           byte[] bits;
/* 457 */           if (token.startsWith("'")) {
/* 458 */             byte[] bits = convertText(token);
/* 459 */             if (bits.length != 1) {
/* 460 */               int newSize = bytes.length + bits.length - 2;
/* 461 */               byte[] newArray = new byte[newSize];
/* 462 */               System.arraycopy(bytes, 0, newArray, 
/* 463 */                 0, t);
/* 464 */               bytes = newArray;
/*     */             }
/*     */           } else {
/* 467 */             bits = convert(token);
/*     */           }
/* 469 */           System.arraycopy(bits, 0, bytes, t, bits.length);
/* 470 */           t += bits.length;
/*     */         }
/* 472 */         for (int i = 0; i < bytes.length; i++) {
/* 473 */           System.out.print("0x" + Integer.toHexString(bytes[i]));
/* 474 */           if (i == bytes.length - 1)
/* 475 */             System.out.println();
/*     */           else
/* 477 */             System.out.print(", ");
/*     */         }
/*     */       }
/*     */       catch (UnknownTokenException ute) {
/* 481 */         System.out.println("Unknown token: " + ute.getToken());
/*     */       }
/*     */   }
/*     */ 
/*     */   static class ByteArrayEntry
/*     */   {
/*     */     byte high;
/*     */     byte low;
/*     */ 
/*     */     public ByteArrayEntry(byte high, byte low)
/*     */     {
/* 496 */       this.high = high;
/* 497 */       this.low = low;
/*     */     }
/*     */ 
/*     */     public int hashCode() {
/* 501 */       return this.high ^ this.low;
/*     */     }
/*     */ 
/*     */     public boolean equals(Object o) {
/* 505 */       if ((o instanceof ByteArrayEntry)) {
/* 506 */         ByteArrayEntry other = (ByteArrayEntry)o;
/* 507 */         return (this.high == other.high) && (this.low == other.low);
/*     */       }
/* 509 */       return false;
/*     */     }
/*     */   }
/*     */ }

/* Location:           C:\Users\lwy08\Desktop\DAIDE\2008\TheDiplominator.jar
 * Qualified Name:     dip.daide.comm.Token
 * JD-Core Version:    0.6.0
 */