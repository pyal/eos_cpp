
/*********************************************************************
  Andre V.Kosteltsev (C) ZUBR Version - 3.0.0b. For Windows NT Console.
  Copyright (C) Andre V.Kosteltsev, 1995 - 1999. All rights reserved.

  This file prodused by ZUBR for used token definitions and
  union declaration.
 *********************************************************************/

#define NUMBER 257
#define VAR 258
#define BLTIN 259
#define UNDEF 260
#define STRING 261
#define ADDEQ 262
#define SUBEQ 263
#define MULEQ 264
#define DIVEQ 265
#define PRINT 266
#define WHILE 267
#define IF 268
#define ELSE 269
#define FUNCTION 270
#define PROCEDURE 271
#define RETURN 272
#define FUNC 273
#define PROC 274
#define READ 275
#define ARG 276
#define OR 277
#define AND 278
#define GT 279
#define GE 280
#define LT 281
#define LE 282
#define EQ 283
#define NE 284
#define UNARYMINUS 285
#define NOT 286

typedef union {
    SYMBOL *sym;
    INST *inst;
    int narg;
} ZUBR_STYPE;

extern ZUBR_STYPE zubr_lval;

/*************************** End of File *****************************/
