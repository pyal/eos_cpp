
/*********************************************************************
  Andre V.Kosteltsev (C) ZUBR Version - 3.0.0b. For Windows NT Console.
  Copyright (C) Andre V.Kosteltsev, 1995 - 1999. All rights reserved.

  This file prodused by ZUBR for used token definitions and
  union declaration.
 *********************************************************************/

#define NUMBER 1024
#define VAR 257
#define BLTIN 258
#define UNDEF 259
#define STRING 260

typedef union
{
   SYMBOL *sym;
   INST   *inst;
} ZUBR_CALCULATOR_ZUBR_STYPE;

extern ZUBR_CALCULATOR_ZUBR_STYPE zubr_calculator_zubr_lval;

/*************************** End of File *****************************/
