
/*********************************************************************
  Andre V.Kosteltsev (C) ZUBR Version - 3.0.0b. For Windows NT Console.
  Copyright (C) Andre V.Kosteltsev, 1995 - 1999. All rights reserved.

  This file prodused by ZUBR for used token definitions and
  union declaration.
 *********************************************************************/

#define IDENT 257
#define NUMBER 258
#define STRING 259
#define VAR 260
#define TYPE 261
#define UNARYMINUS 262

typedef union
{
   SYMBOL sym;
} ZUBR_CALCULATOR_ZUBR_STYPE;

extern ZUBR_CALCULATOR_ZUBR_STYPE zubr_calculator_zubr_lval;

/*************************** End of File *****************************/
