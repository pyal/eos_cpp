
/**************************************************************
  INIT.C
 **************************************************************/

#include <stdio.h>

#include "init.h"
#include "symtab.h"
#include "mathf.h"
#include "code.h"
#include "main.h"

#include "z_tab.h"


/*****************  initial data structures  ******************/

static struct
{
   char *name;
   int   kval;
}
keywords[] =
{
   "proc",   PROC,
   "func",   FUNC,
   "return", RETURN,
   "if",     IF,
   "else",   ELSE,
   "while",  WHILE,
   "print",  PRINT,
   "read",   READ,
   0,        0
};

static struct
{
   char   *name;
   double  cval;
}
consts[] =
{
   "PI",    3.14159265358979323846,
   "E",     2.71828182845904523536,
   "GAMMA", 0.57721566490153286060,
   "DEG",   57.2957795130823208768,
   "PHI",   1.61803398874989484820,
   0,       0
};

static struct
{
   char *name;
   double (*func)();
}
builtins[] =
{
   "acos",  ZAcos,
   "asin",  ZAsin,
   "atan",  ZAtan,
   "atan2", ZAtan2,
   "cos",   ZCos,
   "cosh",  ZCosh,
   "exp",   ZExp,
   "log",   ZLog,
   "log10", ZLog10,
   "sin",   ZSin,
   "sinh",  ZSinh,
   "sqrt",  ZSqrt,
   "tan",   ZTan,
   "tanh",  ZTanh,
   "ceil",  ZCeil,
   "fabs",  ZFabs,
   "floor", ZFloor,
   0,       0
};

void init( void )
{
   int     i;
   SYMBOL *s;

   for( i = 0; keywords[i].name; i++ )
      install( keywords[i].name, keywords[i].kval, 0.0 );

   for( i = 0; consts[i].name; i++ )
      install( consts[i].name, VAR, consts[i].cval );

   for( i = 0; builtins[i].name; i++ )
   {
      s = install( builtins[i].name, BLTIN, 0.0 );
      s->u.ptr = builtins[i].func;
   }

}

/*******************  END OF FILE INIT.C  *********************/
