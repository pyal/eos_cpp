#include <lib/precompiled/calc_lib.h>

#include <stdio.h>  /* printf() used by parser   if set ZUBR_DEBUG */
#include <stdlib.h> /* getenv() used by parser for test ZUBR_DEBUG */

#define zubr_calculator_not_defined_zubr_sccsid   1

#define ZUBR_KOSTELTSEV_ZUBR 1
#ifndef zubr_calculator_not_defined_zubr_sccsid
static char zubr_sccsid[] = "@(#)zubrpar 3.0.0b Andre V.Kosteltsev 19/03/99";
#endif

int zubr_calculator_zubr_parse();

#line 7 "calculator.y"

#include "zubr_import.h"

#line 12 "calculator.y"
typedef union
{
   SYMBOL sym;
} ZUBR_CALCULATOR_ZUBR_STYPE;
#line 24 "zubr_calculator_tab.cpp"

#define IDENT 257
#define NUMBER 258
#define STRING 259
#define VAR 260
#define TYPE 261
#define UNARYMINUS 262

#define ZUBR_CALCULATOR_ZUBR_ERRCODE 256

int zubr_calculator_zubr_lhs[] =
{
    -1,     0,     0,     0,     0,     0,     0,     0,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     2,     2,     5,     5,     4,     4,     4,     4,     6,
     6,     7,     7,     3,     3,
};

int zubr_calculator_zubr_len[] =
{
     2,     0,     2,     3,     3,     3,     3,     3,     1,     1,
     1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
     2,     3,     3,     1,     3,     4,     3,     4,     3,     4,
     4,     1,     3,     2,     2,
};

int zubr_calculator_zubr_defred[] =
{
     1,     0,     0,     0,     8,    10,     0,     0,     0,     2,
     0,     0,     0,     0,     0,     0,     5,     0,     0,     0,
     0,    31,     0,     0,    13,     0,    12,     0,     0,     0,
     0,     0,     0,     6,     3,     0,     7,     4,     0,    26,
     0,     0,     0,    28,     0,     0,     0,    19,     0,     0,
     0,     0,     0,     0,     0,    25,     0,    27,    29,    32,
    30,     0,
};

int zubr_calculator_zubr_dgoto[] =
{
     1,    40,    24,    25,    26,    41,    15,    22,
};

int zubr_calculator_zubr_sindex[] =
{
     0,   -10,     1,   -30,     0,     0,   -37,  -242,   -27,     0,
   -27,    10,     6,     9,     7,   -70,     0,   -39,   -27,   -33,
   -27,     0,   -21,   -61,     0,   -18,     0,   100,   -27,   -27,
   -27,   -27,   -27,     0,     0,   -27,     0,     0,   -27,     0,
   122,   -19,   122,     0,   -13,    60,  -213,     0,   -38,   -38,
   -61,   -61,   -61,   122,   114,     0,   -27,     0,     0,     0,
     0,   122,
};

int zubr_calculator_zubr_rindex[] =
{
     0,     0,     0,     0,     0,     0,    -5,     0,     0,     0,
     0,     0,   135,     0,   143,     4,     0,     0,     0,     0,
     0,     0,    16,    17,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   -12,     0,    90,     0,     0,     0,     0,     0,    70,    82,
    24,    31,    40,    91,     0,     0,     0,     0,     0,     0,
     0,    49,
};

int zubr_calculator_zubr_gindex[] =
{
     0,   120,    62,    78,    85,    72,     0,     0,
};

#define ZUBR_CALCULATOR_ZUBR_TABLESIZE 251
int zubr_calculator_zubr_table[] =
{
     9,    10,    39,    19,    30,     9,     8,    10,    43,    31,
    17,    16,     8,    10,    11,    21,    34,    37,     8,    36,
    33,    38,    55,    46,    18,    56,    34,    20,    57,    23,
    10,    56,    23,    32,    16,     8,     9,     9,     9,     9,
     9,    17,     9,    35,    59,    11,    11,    11,    11,    11,
    18,    11,    30,    28,    20,    29,    32,    31,    20,    20,
    20,    20,    20,    12,    20,    16,    16,    16,    16,    16,
    35,    16,    17,    17,    17,    17,    17,    34,    17,    13,
    14,    18,    18,    18,    18,    18,    14,    18,     9,     9,
    24,    44,    15,    24,     0,     0,     0,    11,    11,     0,
    21,    22,    30,    28,    32,    29,     0,    31,     0,     0,
    20,    14,     0,    14,    14,    14,     0,    16,     0,     0,
     0,    11,     0,    15,    17,    15,    15,    15,    23,     0,
    27,    21,    22,    18,    21,    22,     0,     0,    42,     0,
    45,    47,    30,    28,     0,    29,     0,    31,    48,    49,
    50,    51,    52,    58,    32,    53,    30,    28,    54,    29,
     0,    31,     0,    14,    30,    28,     0,    29,     0,    31,
     0,     0,     0,     0,     0,    15,    61,    13,    13,     0,
    13,     0,    13,    21,    22,    12,    12,     0,    12,     0,
    12,     0,     0,     0,    32,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    60,    32,     0,
     0,     0,     0,     0,     0,     0,    32,     0,     3,     4,
     5,     6,     7,     0,     3,     4,     5,     6,     7,    13,
     3,     4,     5,     6,     7,     0,     0,    12,     0,     0,
     0,     0,     0,     0,     0,     0,     2,     3,     4,     5,
     6,     7,
};

int zubr_calculator_zubr_check[] =
{
    10,    40,    41,    40,    42,    10,    45,    40,    41,    47,
    40,    10,    45,    40,    10,   257,    10,    10,    45,    10,
    10,    91,    41,    44,    61,    44,    10,    10,    41,    41,
    40,    44,    44,    94,    10,    45,    41,    42,    43,    44,
    45,    10,    47,    61,   257,    41,    42,    43,    44,    45,
    10,    47,    42,    43,    91,    45,    94,    47,    41,    42,
    43,    44,    45,     1,    47,    41,    42,    43,    44,    45,
    61,    47,    41,    42,    43,    44,    45,    61,    47,     1,
    10,    41,    42,    43,    44,    45,     1,    47,    93,    94,
    41,    19,    10,    44,    -1,    -1,    -1,    93,    94,    -1,
    10,    10,    42,    43,    94,    45,    -1,    47,    -1,    -1,
    93,    41,    -1,    43,    44,    45,    -1,    93,    -1,    -1,
    -1,     1,    -1,    41,    93,    43,    44,    45,     8,    -1,
    10,    41,    41,    93,    44,    44,    -1,    -1,    18,    -1,
    20,    41,    42,    43,    -1,    45,    -1,    47,    28,    29,
    30,    31,    32,    93,    94,    35,    42,    43,    38,    45,
    -1,    47,    -1,    93,    42,    43,    -1,    45,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    93,    56,    42,    43,    -1,
    45,    -1,    47,    93,    93,    42,    43,    -1,    45,    -1,
    47,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,   257,   258,
   259,   260,   261,    -1,   257,   258,   259,   260,   261,    94,
   257,   258,   259,   260,   261,    -1,    -1,    94,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   256,   257,   258,   259,
   260,   261,
};

#define ZUBR_CALCULATOR_ZUBR_FINAL 1
#ifndef ZUBR_CALCULATOR_ZUBR_DEBUG
#define ZUBR_CALCULATOR_ZUBR_DEBUG 0
#endif
#define ZUBR_CALCULATOR_ZUBR_MAXTOKEN 262
#if ZUBR_CALCULATOR_ZUBR_DEBUG
char *zubr_calculator_zubr_name[] =
{
"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,0,
0,0,0,0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,
"']'","'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,"IDENT","NUMBER","STRING","VAR","TYPE","UNARYMINUS",
};

char *zubr_calculator_zubr_rule[] =
{
"$accept : list",
"list :",
"list : list '\\n'",
"list : list assign '\\n'",
"list : list func '\\n'",
"list : list error '\\n'",
"list : list expr '\\n'",
"list : list declare '\\n'",
"expr : NUMBER",
"expr : VAR",
"expr : STRING",
"expr : arr",
"expr : func",
"expr : assign",
"expr : expr '+' expr",
"expr : expr '-' expr",
"expr : expr '*' expr",
"expr : expr '/' expr",
"expr : expr '^' expr",
"expr : '(' expr ')'",
"expr : '-' expr",
"assign : VAR '=' expr",
"assign : declare '=' expr",
"args : expr",
"args : args ',' expr",
"func : IDENT '(' args ')'",
"func : IDENT '(' ')'",
"func : VAR '(' args ')'",
"func : VAR '(' ')'",
"arr : VAR '[' expr ']'",
"arr : arr '[' expr ']'",
"decl_lst : IDENT",
"decl_lst : decl_lst ',' IDENT",
"declare : TYPE IDENT",
"declare : TYPE decl_lst",
};
#endif

#define     zubr_calculator_zubr_clearin    (zubr_calculator_zubr_char=(-1))
#define     zubr_calculator_zubr_errok      (zubr_calculator_zubr_errflag=0)
#ifdef  ZUBR_CALCULATOR_ZUBR_STACKSIZE
#ifndef ZUBR_CALCULATOR_ZUBR_MAXDEPTH
#define     ZUBR_CALCULATOR_ZUBR_MAXDEPTH   ZUBR_CALCULATOR_ZUBR_STACKSIZE
#endif
#else
#ifdef  ZUBR_CALCULATOR_ZUBR_MAXDEPTH
#define     ZUBR_CALCULATOR_ZUBR_STACKSIZE  ZUBR_CALCULATOR_ZUBR_MAXDEPTH
#else
#define     ZUBR_CALCULATOR_ZUBR_STACKSIZE  500
#define     ZUBR_CALCULATOR_ZUBR_MAXDEPTH   500
#endif
#endif

#if ZUBR_CALCULATOR_ZUBR_DEBUG
int            zubr_calculator_zubr_debug;
#endif
int            zubr_calculator_zubr_nerrs;
int            zubr_calculator_zubr_errflag;
int            zubr_calculator_zubr_char;
int           *zubr_calculator_zubr_ssp;

ZUBR_CALCULATOR_ZUBR_STYPE    *zubr_calculator_zubr_vsp;
ZUBR_CALCULATOR_ZUBR_STYPE     zubr_calculator_zubr_val;
ZUBR_CALCULATOR_ZUBR_STYPE     zubr_calculator_zubr_lval;

int            zubr_calculator_zubr_ss[ZUBR_CALCULATOR_ZUBR_STACKSIZE];

ZUBR_CALCULATOR_ZUBR_STYPE     zubr_calculator_zubr_vs[ZUBR_CALCULATOR_ZUBR_STACKSIZE];

#define        zubr_calculator_zubr_stacksize  ZUBR_CALCULATOR_ZUBR_STACKSIZE

#line 88 "calculator.y"

/********************  END OF GRAMMAR  ***********************/


 
#line 252 "zubr_calculator_tab.cpp"

#define ZUBR_CALCULATOR_ZUBR_ABORT     goto zubr_calculator_zubr_abort
#define ZUBR_CALCULATOR_ZUBR_ACCEPT    goto zubr_calculator_zubr_accept
#define ZUBR_CALCULATOR_ZUBR_NEWERROR  goto zubr_calculator_zubr_newerror
#define ZUBR_CALCULATOR_ZUBR_ERROR     goto zubr_calculator_zubr_errlab


int zubr_calculator_zubr_parse()
{
   register int zubr_calculator_zubr_m, zubr_calculator_zubr_n, zubr_calculator_zubr_state;
#if ZUBR_CALCULATOR_ZUBR_DEBUG
   register char *zubr_calculator_zubr_s;

   if( zubr_calculator_zubr_s = getenv("ZUBR_DEBUG") )
   {
      zubr_calculator_zubr_n = *zubr_calculator_zubr_s;
      if( zubr_calculator_zubr_n >= '0' && zubr_calculator_zubr_n <= '9' ) zubr_calculator_zubr_debug = zubr_calculator_zubr_n - '0';
   }
#endif

   zubr_calculator_zubr_nerrs   = 0;
   zubr_calculator_zubr_errflag = 0;
   zubr_calculator_zubr_char    = (-1);

   zubr_calculator_zubr_ssp     = zubr_calculator_zubr_ss;
   zubr_calculator_zubr_vsp     = zubr_calculator_zubr_vs;
  *zubr_calculator_zubr_ssp     = zubr_calculator_zubr_state = 0;

zubr_calculator_zubr_loop:
   if( (zubr_calculator_zubr_n = zubr_calculator_zubr_defred[zubr_calculator_zubr_state]) ) goto zubr_calculator_zubr_reduce;
   if( zubr_calculator_zubr_char < 0 )
   {
      if( (zubr_calculator_zubr_char = zubr_calculator_zubr_lex()) < 0 ) zubr_calculator_zubr_char = 0;
#if ZUBR_CALCULATOR_ZUBR_DEBUG
      if( zubr_calculator_zubr_debug )
      {
         zubr_calculator_zubr_s = 0;
         if( zubr_calculator_zubr_char <= ZUBR_CALCULATOR_ZUBR_MAXTOKEN ) zubr_calculator_zubr_s = zubr_calculator_zubr_name[zubr_calculator_zubr_char];
         if( !zubr_calculator_zubr_s ) zubr_calculator_zubr_s = "illegal-symbol";
         printf( "zubr_calculator_zubr_debug: state %d, reading %d (%s)\n",
                 zubr_calculator_zubr_state, zubr_calculator_zubr_char, zubr_calculator_zubr_s );
      }
#endif
   } /*  End if( zubr_calculator_zubr_char < 0 )  */

   if( (zubr_calculator_zubr_n = zubr_calculator_zubr_sindex[zubr_calculator_zubr_state]) && (zubr_calculator_zubr_n += zubr_calculator_zubr_char) >= 0 &&
       zubr_calculator_zubr_n <= ZUBR_CALCULATOR_ZUBR_TABLESIZE && zubr_calculator_zubr_check[zubr_calculator_zubr_n] == zubr_calculator_zubr_char )
   {
#if ZUBR_CALCULATOR_ZUBR_DEBUG
      if( zubr_calculator_zubr_debug )
         printf( "zubr_calculator_zubr_debug: state %d, shifting to state %d\n",
                 zubr_calculator_zubr_state, zubr_calculator_zubr_table[zubr_calculator_zubr_n] );
#endif
      if( zubr_calculator_zubr_ssp >= zubr_calculator_zubr_ss + zubr_calculator_zubr_stacksize - 1 )
      {
         goto zubr_calculator_zubr_overflow;
      }
      *++zubr_calculator_zubr_ssp = zubr_calculator_zubr_state = zubr_calculator_zubr_table[zubr_calculator_zubr_n];
      *++zubr_calculator_zubr_vsp = zubr_calculator_zubr_lval;
      zubr_calculator_zubr_char = (-1);
      if( zubr_calculator_zubr_errflag > 0 ) --zubr_calculator_zubr_errflag;
      goto zubr_calculator_zubr_loop;
   }

   if( (zubr_calculator_zubr_n = zubr_calculator_zubr_rindex[zubr_calculator_zubr_state]) && (zubr_calculator_zubr_n += zubr_calculator_zubr_char) >= 0 &&
       zubr_calculator_zubr_n <= ZUBR_CALCULATOR_ZUBR_TABLESIZE && zubr_calculator_zubr_check[zubr_calculator_zubr_n] == zubr_calculator_zubr_char )
   {
      zubr_calculator_zubr_n = zubr_calculator_zubr_table[zubr_calculator_zubr_n];
      goto zubr_calculator_zubr_reduce;
   }
   if (zubr_calculator_zubr_errflag) goto zubr_calculator_zubr_inrecovery;
#ifdef zubr_calculator_not_defined_zubr_sccsid
   goto zubr_calculator_zubr_newerror;
#endif
zubr_calculator_zubr_newerror:
   zubr_calculator_zubr_error( "syntax error" );
#ifdef zubr_calculator_not_defined_zubr_sccsid
   goto zubr_calculator_zubr_errlab;
#endif
zubr_calculator_zubr_errlab:
   ++zubr_calculator_zubr_nerrs;
zubr_calculator_zubr_inrecovery:
   if( zubr_calculator_zubr_errflag < 3 )
   {
      zubr_calculator_zubr_errflag = 3;
      for( ;; )
      {
         if( (zubr_calculator_zubr_n = zubr_calculator_zubr_sindex[*zubr_calculator_zubr_ssp]) && (zubr_calculator_zubr_n += ZUBR_CALCULATOR_ZUBR_ERRCODE) >= 0 &&
             zubr_calculator_zubr_n <= ZUBR_CALCULATOR_ZUBR_TABLESIZE && zubr_calculator_zubr_check[zubr_calculator_zubr_n] == ZUBR_CALCULATOR_ZUBR_ERRCODE )
         {
#if ZUBR_CALCULATOR_ZUBR_DEBUG
            if( zubr_calculator_zubr_debug )
               printf( "zubr_calculator_zubr_debug: state %d, error recovery shifting\
 to state %d\n", *zubr_calculator_zubr_ssp, zubr_calculator_zubr_table[zubr_calculator_zubr_n] );
#endif
            if( zubr_calculator_zubr_ssp >= zubr_calculator_zubr_ss + zubr_calculator_zubr_stacksize - 1 )
            {
               goto zubr_calculator_zubr_overflow;
            }
            *++zubr_calculator_zubr_ssp = zubr_calculator_zubr_state = zubr_calculator_zubr_table[zubr_calculator_zubr_n];
            *++zubr_calculator_zubr_vsp = zubr_calculator_zubr_lval;
            goto zubr_calculator_zubr_loop;
         }
         else
         {
#if ZUBR_CALCULATOR_ZUBR_DEBUG
            if( zubr_calculator_zubr_debug )
               printf( "zubr_calculator_zubr_debug: error recovery discarding state %d\n",
                       *zubr_calculator_zubr_ssp );
#endif
            if( zubr_calculator_zubr_ssp <= zubr_calculator_zubr_ss ) goto zubr_calculator_zubr_abort;
            --zubr_calculator_zubr_ssp;
            --zubr_calculator_zubr_vsp;
         }
      } /*  End of for( ;; )  */
   } /*  End if( zubr_calculator_zubr_errflag < 3 )  */
   else
   {
      if( zubr_calculator_zubr_char == 0 ) goto zubr_calculator_zubr_abort;
#if ZUBR_CALCULATOR_ZUBR_DEBUG
      if( zubr_calculator_zubr_debug )
      {
         zubr_calculator_zubr_s = 0;
         if( zubr_calculator_zubr_char <= ZUBR_CALCULATOR_ZUBR_MAXTOKEN ) zubr_calculator_zubr_s = zubr_calculator_zubr_name[zubr_calculator_zubr_char];
         if( !zubr_calculator_zubr_s ) zubr_calculator_zubr_s = "illegal-symbol";
printf( "zubr_calculator_zubr_debug: state %d, error recovery discards token %d (%s)\n",
        zubr_calculator_zubr_state, zubr_calculator_zubr_char, zubr_calculator_zubr_s );
      }
#endif
      zubr_calculator_zubr_char = (-1);
      goto zubr_calculator_zubr_loop;
   }
zubr_calculator_zubr_reduce:
#if ZUBR_CALCULATOR_ZUBR_DEBUG
   if( zubr_calculator_zubr_debug )
      printf( "zubr_calculator_zubr_debug: state %d, reducing by rule %d (%s)\n",
              zubr_calculator_zubr_state, zubr_calculator_zubr_n, zubr_calculator_zubr_rule[zubr_calculator_zubr_n] );
#endif
   zubr_calculator_zubr_m   = zubr_calculator_zubr_len[zubr_calculator_zubr_n];
   zubr_calculator_zubr_val = zubr_calculator_zubr_vsp[1-zubr_calculator_zubr_m];

   switch( zubr_calculator_zubr_n )
   {
      case 3:
#line 33 "calculator.y"
         { EndStmt(zubr_calculator_zubr_vsp[-1].sym);    }
         break;
      case 4:
#line 34 "calculator.y"
         { EndStmt(zubr_calculator_zubr_vsp[-1].sym);    }
         break;
      case 5:
#line 35 "calculator.y"
         { zubr_calculator_zubr_error(NULL); }
         break;
      case 6:
#line 36 "calculator.y"
         { EndStmt(zubr_calculator_zubr_vsp[-1].sym);    }
         break;
      case 7:
#line 37 "calculator.y"
         {EndStmt(zubr_calculator_zubr_vsp[-1].sym); }
         break;
      case 14:
#line 49 "calculator.y"
         { zubr_calculator_zubr_val.sym = AddOper(zubr_calculator_zubr_vsp[-2].sym,zubr_calculator_zubr_vsp[0].sym,'+');   }
         break;
      case 15:
#line 50 "calculator.y"
         { zubr_calculator_zubr_val.sym = AddOper(zubr_calculator_zubr_vsp[-2].sym,zubr_calculator_zubr_vsp[0].sym,'-');   }
         break;
      case 16:
#line 51 "calculator.y"
         { zubr_calculator_zubr_val.sym = AddOper(zubr_calculator_zubr_vsp[-2].sym,zubr_calculator_zubr_vsp[0].sym,'*');   }
         break;
      case 17:
#line 52 "calculator.y"
         { zubr_calculator_zubr_val.sym = AddOper(zubr_calculator_zubr_vsp[-2].sym,zubr_calculator_zubr_vsp[0].sym,'/');   }
         break;
      case 18:
#line 53 "calculator.y"
         { zubr_calculator_zubr_val.sym = AddOper(zubr_calculator_zubr_vsp[-2].sym,zubr_calculator_zubr_vsp[0].sym,'^');   }
         break;
      case 19:
#line 54 "calculator.y"
         { zubr_calculator_zubr_val.sym =  zubr_calculator_zubr_vsp[-1].sym; }
         break;
      case 20:
#line 55 "calculator.y"
         { zubr_calculator_zubr_val.sym = AddOper(zubr_calculator_zubr_vsp[0].sym,NULL,'-');   }
         break;
      case 21:
#line 59 "calculator.y"
         { zubr_calculator_zubr_val.sym = AddOper(zubr_calculator_zubr_vsp[-2].sym,zubr_calculator_zubr_vsp[0].sym,'=');   }
         break;
      case 22:
#line 60 "calculator.y"
         { zubr_calculator_zubr_val.sym = AddOper(zubr_calculator_zubr_vsp[-2].sym,zubr_calculator_zubr_vsp[0].sym,'=');   }
         break;
      case 23:
#line 63 "calculator.y"
         {ListStart();ListAdd(zubr_calculator_zubr_vsp[0].sym);}
         break;
      case 24:
#line 64 "calculator.y"
         {ListAdd(zubr_calculator_zubr_vsp[0].sym);}
         break;
      case 25:
#line 68 "calculator.y"
         {zubr_calculator_zubr_val.sym = AddFunc(zubr_calculator_zubr_vsp[-3].sym);}
         break;
      case 26:
#line 69 "calculator.y"
         {ListStart();zubr_calculator_zubr_val.sym = AddFunc(zubr_calculator_zubr_vsp[-2].sym);}
         break;
      case 27:
#line 70 "calculator.y"
         {zubr_calculator_zubr_val.sym = AddFunc(zubr_calculator_zubr_vsp[-3].sym);}
         break;
      case 28:
#line 71 "calculator.y"
         {ListStart();zubr_calculator_zubr_val.sym = AddFunc(zubr_calculator_zubr_vsp[-2].sym);}
         break;
      case 29:
#line 75 "calculator.y"
         {zubr_calculator_zubr_val.sym = GetEl(zubr_calculator_zubr_vsp[-3].sym,zubr_calculator_zubr_vsp[-1].sym);}
         break;
      case 30:
#line 76 "calculator.y"
         {zubr_calculator_zubr_val.sym = GetEl(zubr_calculator_zubr_vsp[-3].sym,zubr_calculator_zubr_vsp[-1].sym);}
         break;
      case 31:
#line 79 "calculator.y"
         {ListStart();ListAdd(zubr_calculator_zubr_vsp[0].sym);}
         break;
      case 32:
#line 80 "calculator.y"
         {ListAdd(zubr_calculator_zubr_vsp[0].sym);}
         break;
      case 33:
#line 83 "calculator.y"
         {zubr_calculator_zubr_val.sym = DefineVar(zubr_calculator_zubr_vsp[-1].sym, zubr_calculator_zubr_vsp[0].sym);}
         break;
      case 34:
#line 84 "calculator.y"
         {zubr_calculator_zubr_val.sym = DefineVar(zubr_calculator_zubr_vsp[-1].sym);}
         break;
#line 500 "zubr_calculator_tab.cpp"

   } /*  End of switch( zubr_calculator_zubr_n )  */

   zubr_calculator_zubr_ssp   -=  zubr_calculator_zubr_m;
   zubr_calculator_zubr_state  = *zubr_calculator_zubr_ssp;
   zubr_calculator_zubr_vsp   -=  zubr_calculator_zubr_m;
   zubr_calculator_zubr_m      =  zubr_calculator_zubr_lhs[zubr_calculator_zubr_n];

   if( zubr_calculator_zubr_state == 0 && zubr_calculator_zubr_m == 0 )
   {
#if ZUBR_CALCULATOR_ZUBR_DEBUG
      if( zubr_calculator_zubr_debug )
         printf( "zubr_calculator_zubr_debug: after reduction, shifting from state 0 to\
 state %d\n", ZUBR_CALCULATOR_ZUBR_FINAL );
#endif
      zubr_calculator_zubr_state  = ZUBR_CALCULATOR_ZUBR_FINAL;
      *++zubr_calculator_zubr_ssp = ZUBR_CALCULATOR_ZUBR_FINAL;
      *++zubr_calculator_zubr_vsp = zubr_calculator_zubr_val;
      if( zubr_calculator_zubr_char < 0 )
      {
         if( (zubr_calculator_zubr_char = zubr_calculator_zubr_lex()) < 0 ) zubr_calculator_zubr_char = 0;
#if ZUBR_CALCULATOR_ZUBR_DEBUG
         if( zubr_calculator_zubr_debug )
         {
            zubr_calculator_zubr_s = 0;
            if( zubr_calculator_zubr_char <= ZUBR_CALCULATOR_ZUBR_MAXTOKEN) zubr_calculator_zubr_s = zubr_calculator_zubr_name[zubr_calculator_zubr_char];
            if( !zubr_calculator_zubr_s ) zubr_calculator_zubr_s = "illegal-symbol";
            printf( "zubr_calculator_zubr_debug: state %d, reading %d (%s)\n",
                    ZUBR_CALCULATOR_ZUBR_FINAL, zubr_calculator_zubr_char, zubr_calculator_zubr_s);
         }
#endif
      }
      if( zubr_calculator_zubr_char == 0 ) goto zubr_calculator_zubr_accept;
      goto zubr_calculator_zubr_loop;
   } /*  End if( zubr_calculator_zubr_state == 0 && zubr_calculator_zubr_m == 0 )  */

   if( (zubr_calculator_zubr_n = zubr_calculator_zubr_gindex[zubr_calculator_zubr_m]) && (zubr_calculator_zubr_n += zubr_calculator_zubr_state) >= 0 &&
       zubr_calculator_zubr_n <= ZUBR_CALCULATOR_ZUBR_TABLESIZE && zubr_calculator_zubr_check[zubr_calculator_zubr_n] == zubr_calculator_zubr_state )
      zubr_calculator_zubr_state = zubr_calculator_zubr_table[zubr_calculator_zubr_n];
   else
      zubr_calculator_zubr_state = zubr_calculator_zubr_dgoto[zubr_calculator_zubr_m];
#if ZUBR_CALCULATOR_ZUBR_DEBUG
   if( zubr_calculator_zubr_debug )
      printf( "zubr_calculator_zubr_debug: after reduction, shifting from state %d \
to state %d\n", *zubr_calculator_zubr_ssp, zubr_calculator_zubr_state );
#endif
   if( zubr_calculator_zubr_ssp >= zubr_calculator_zubr_ss + zubr_calculator_zubr_stacksize - 1 )
   {
      goto zubr_calculator_zubr_overflow;
   }
   *++zubr_calculator_zubr_ssp = zubr_calculator_zubr_state;
   *++zubr_calculator_zubr_vsp = zubr_calculator_zubr_val;
   goto zubr_calculator_zubr_loop;
zubr_calculator_zubr_overflow:
   zubr_calculator_zubr_error( "zubr stack overflow" );
zubr_calculator_zubr_abort:
   return( 1 );
zubr_calculator_zubr_accept:
   return( 0 );
}
