
/*********************************************************************
  Andre V.Kosteltsev (C) ZUBR Version - 3.0.0b. For Windows NT Console.
  Copyright (C) Andre V.Kosteltsev, 1995 - 1999. All rights reserved.

  This file prodused by ZUBR for used token definitions and
  union declaration.
 *********************************************************************/

#define ASSIGN 907
#define OROR 901
#define ANDAND 902
#define EQCOMPARE 906
#define ARITHCOMPARE 905
#define LSHIFT 910
#define RSHIFT 911
#define UNARY 908
#define PLUSPLUS 903
#define MINUSMINUS 904
#define HYPERUNARY 909
#define POINTSAT 912
#define TYPESPEC 800
#define TYPE_QUAL 802
#define IDENTIFIER 500
#define CONSTANT 502
#define VARIABLE 503
#define STRING 505
#define MTH_BUILTIN 600

typedef union
{
   unsigned long      itype;
   z_symbol          *sym;
   enum tetrade_code  code;
} ZUBR_STYPE;

extern ZUBR_STYPE zubr_lval;

/*************************** End of File *****************************/
