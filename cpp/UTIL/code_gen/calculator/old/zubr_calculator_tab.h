
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
#define READ 267
#define UNARYMINUS 268

typedef union {
    SYMBOL sym;
    INST *inst;
} ZUBR_CALCULATOR_ZUBR_STYPE;

extern ZUBR_CALCULATOR_ZUBR_STYPE zubr_calculator_zubr_lval;

/*************************** End of File *****************************/
