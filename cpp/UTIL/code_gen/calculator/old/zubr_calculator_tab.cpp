
#include <stdio.h>  /* printf() used by parser   if set ZUBR_DEBUG */
#include <stdlib.h> /* getenv() used by parser for test ZUBR_DEBUG */

#define zubr_calculator_not_defined_zubr_sccsid 1

#define ZUBR_KOSTELTSEV_ZUBR 1
#ifndef zubr_calculator_not_defined_zubr_sccsid
static char zubr_sccsid[] = "@(#)zubrpar 3.0.0b Andre V.Kosteltsev 19/03/99";
#endif

int zubr_calculator_zubr_parse();

#line 7 "calculator.y"

#include "zubr_import.h"
#define code2(c1,c2) code(c1); code(c2)
#define code3(c1,c2,c3) code(c1); code(c2); code(c3)

#line 14 "calculator.y"
typedef union {
    SYMBOL sym;
    INST *inst;
} ZUBR_CALCULATOR_ZUBR_STYPE;
#line 27 "zubr_calculator_tab.cpp"

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

#define ZUBR_CALCULATOR_ZUBR_ERRCODE 256

int zubr_calculator_zubr_lhs[] = {
    -1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1,  1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4,
};

int zubr_calculator_zubr_len[] = {
    2, 0, 2, 3, 3, 3, 3, 1, 1, 1, 4, 4, 6, 3, 3,
    3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 1, 1, 3, 3, 2,
};

int zubr_calculator_zubr_defred[] = {
    1, 0, 0,  7, 0, 0, 0,  0, 0, 2, 0, 0, 0,  0, 6,  0, 0,  0, 0,
    0, 0, 26, 0, 9, 0, 0,  0, 0, 0, 0, 0, 0,  0, 4,  3, 5,  0, 0,
    0, 0, 0,  0, 0, 0, 18, 0, 0, 0, 0, 0, 11, 0, 28, 0, 10, 0, 12,
};

int zubr_calculator_zubr_dgoto[] = {
     1,    11,    23,    24,    13,
};

int zubr_calculator_zubr_sindex[] =
{
     0,   -10,    -7,     0,   -59,   -36,   -39,   -31,   -26,     0,
   -26,     1,     5,     6,     0,   -26,   -26,   -26,   -26,   -26,
   -26,     0,    14,     0,   -34,  -240,   -69,   -14,   -26,   -26,
   -26,   -26,   -26,     0,     0,     0,    14,    14,    14,    14,
    14,   -21,   -32,     4,     0,   -30,   -30,   -69,   -69,   -69,
     0,   -26,     0,    14,     0,     8,     0,
};

int zubr_calculator_zubr_rindex[] = {
    0,   0,   0,   0,  -5, 0,  0, 0,  0,   0,  0,  0,   20, 0, 0, 0,  0, 0,  0,
    0,   0,   0,   -3, 0,  24, 0, 62, 0,   0,  0,  0,   0,  0, 0, 0,  0, 50, 116,
    118, 120, 127, 0,  0,  0,  0, 72, 108, 80, 91, 101, 0,  0, 0, 22, 0, 0,  0,
};

int zubr_calculator_zubr_gindex[] = {
    0,
    68,
    46,
    0,
    0,
};

#define ZUBR_CALCULATOR_ZUBR_TABLESIZE 257
int zubr_calculator_zubr_table[] = {
    9,  10, 19, 14, 20, 8,  8,  25, 10, 25, 42, 33, 30, 8,  10, 34, 35, 31, 43, 8,
    50, 30, 28, 51, 29, 32, 31, 44, 30, 28, 10, 29, 27, 31, 29, 8,  8,  8,  8,  8,
    8,  25, 8,  30, 28, 54, 29, 12, 31, 56, 30, 28, 0,  29, 0,  31, 30, 28, 0,  29,
    21, 31, 9,  9,  32, 9,  27, 9,  0,  0,  0,  0,  19, 32, 22, 0,  26, 0,  27, 0,
    32, 0,  13, 36, 37, 38, 39, 40, 41, 8,  15, 21, 0,  0,  21, 32, 45, 46, 47, 48,
    49, 16, 32, 19, 19, 19, 19, 19, 32, 19, 53, 17, 0,  13, 9,  13, 13, 13, 14, 55,
    0,  15, 15, 15, 15, 15, 22, 15, 23, 0,  24, 0,  16, 16, 16, 16, 16, 20, 16, 0,
    0,  0,  17, 17, 17, 17, 17, 0,  17, 14, 0,  14, 14, 14, 0,  0,  0,  22, 0,  23,
    22, 24, 23, 0,  24, 0,  0,  0,  20, 0,  0,  20, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  15, 16, 17, 18, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4,
    5,  0,  21, 0,  0,  3,  4,  5,  7,  52, 0,  3,  4,  5,  0,  7,  0,  0,  0,  0,
    0,  7,  0,  0,  0,  0,  2,  3,  4,  5,  0,  0,  0,  0,  0,  0,  6,  7,
};

int zubr_calculator_zubr_check[] = {
    10,  40,  61,  10,  40,  10,  45,  10,  40,  40,  44,  10,  42,  45, 40,  10,  10,
    47,  258, 45,  41,  42,  43,  44,  45,  94,  47,  41,  42,  43,  40, 45,  10,  47,
    10,  45,  41,  42,  43,  44,  45,  44,  47,  42,  43,  41,  45,  1,  47,  41,  42,
    43,  -1,  45,  -1,  47,  42,  43,  -1,  45,  10,  47,  42,  43,  94, 45,  44,  47,
    -1,  -1,  -1,  -1,  10,  94,  6,   -1,  8,   -1,  10,  -1,  94,  -1, 10,  15,  16,
    17,  18,  19,  20,  94,  10,  41,  -1,  -1,  44,  94,  28,  29,  30, 31,  32,  10,
    94,  41,  42,  43,  44,  45,  94,  47,  42,  10,  -1,  41,  94,  43, 44,  45,  10,
    51,  -1,  41,  42,  43,  44,  45,  10,  47,  10,  -1,  10,  -1,  41, 42,  43,  44,
    45,  10,  47,  -1,  -1,  -1,  41,  42,  43,  44,  45,  -1,  47,  41, -1,  43,  44,
    45,  -1,  -1,  -1,  41,  -1,  41,  44,  41,  44,  -1,  44,  -1,  -1, -1,  41,  -1,
    -1,  44,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, -1,  -1,  262,
    263, 264, 265, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 257, 258, 259,
    -1,  261, -1,  -1,  257, 258, 259, 267, 261, -1,  257, 258, 259, -1, 267, -1,  -1,
    -1,  -1,  -1,  267, -1,  -1,  -1,  -1,  256, 257, 258, 259, -1,  -1, -1,  -1,  -1,
    -1,  266, 267,
};

#define ZUBR_CALCULATOR_ZUBR_FINAL 1
#ifndef ZUBR_CALCULATOR_ZUBR_DEBUG
#define ZUBR_CALCULATOR_ZUBR_DEBUG 0
#endif
#define ZUBR_CALCULATOR_ZUBR_MAXTOKEN 268
#if ZUBR_CALCULATOR_ZUBR_DEBUG
char *zubr_calculator_zubr_name[] =
{
"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,0,
0,0,0,0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,"NUMBER","VAR","BLTIN","UNDEF","STRING","ADDEQ","SUBEQ","MULEQ",
"DIVEQ","PRINT","READ","UNARYMINUS",
};

char *zubr_calculator_zubr_rule[] = {
    "$accept : list",
    "list :",
    "list : list '\\n'",
    "list : list assign '\\n'",
    "list : list expr '\\n'",
    "list : list stmt '\\n'",
    "list : list error '\\n'",
    "expr : NUMBER",
    "expr : VAR",
    "expr : assign",
    "expr : READ '(' VAR ')'",
    "expr : BLTIN '(' expr ')'",
    "expr : BLTIN '(' expr ',' expr ')'",
    "expr : expr '+' expr",
    "expr : expr '-' expr",
    "expr : expr '*' expr",
    "expr : expr '/' expr",
    "expr : expr '^' expr",
    "expr : '(' expr ')'",
    "expr : '-' expr",
    "assign : VAR '=' expr",
    "assign : VAR ADDEQ expr",
    "assign : VAR SUBEQ expr",
    "assign : VAR MULEQ expr",
    "assign : VAR DIVEQ expr",
    "prlist : expr",
    "prlist : STRING",
    "prlist : prlist ',' expr",
    "prlist : prlist ',' STRING",
    "stmt : PRINT prlist",
};
#endif

#define zubr_calculator_zubr_clearin (zubr_calculator_zubr_char = (-1))
#define zubr_calculator_zubr_errok (zubr_calculator_zubr_errflag = 0)
#ifdef ZUBR_CALCULATOR_ZUBR_STACKSIZE
#ifndef ZUBR_CALCULATOR_ZUBR_MAXDEPTH
#define ZUBR_CALCULATOR_ZUBR_MAXDEPTH ZUBR_CALCULATOR_ZUBR_STACKSIZE
#endif
#else
#ifdef ZUBR_CALCULATOR_ZUBR_MAXDEPTH
#define ZUBR_CALCULATOR_ZUBR_STACKSIZE ZUBR_CALCULATOR_ZUBR_MAXDEPTH
#else
#define ZUBR_CALCULATOR_ZUBR_STACKSIZE 500
#define ZUBR_CALCULATOR_ZUBR_MAXDEPTH 500
#endif
#endif

#if ZUBR_CALCULATOR_ZUBR_DEBUG
int zubr_calculator_zubr_debug;
#endif
int zubr_calculator_zubr_nerrs;
int zubr_calculator_zubr_errflag;
int zubr_calculator_zubr_char;
int *zubr_calculator_zubr_ssp;

ZUBR_CALCULATOR_ZUBR_STYPE *zubr_calculator_zubr_vsp;
ZUBR_CALCULATOR_ZUBR_STYPE zubr_calculator_zubr_val;
ZUBR_CALCULATOR_ZUBR_STYPE zubr_calculator_zubr_lval;

int zubr_calculator_zubr_ss[ZUBR_CALCULATOR_ZUBR_STACKSIZE];

ZUBR_CALCULATOR_ZUBR_STYPE zubr_calculator_zubr_vs[ZUBR_CALCULATOR_ZUBR_STACKSIZE];

#define zubr_calculator_zubr_stacksize ZUBR_CALCULATOR_ZUBR_STACKSIZE

#line 85 "calculator.y"

/********************  END OF GRAMMAR  ***********************/


#line 252 "zubr_calculator_tab.cpp"

#define ZUBR_CALCULATOR_ZUBR_ABORT     goto zubr_calculator_zubr_abort
#define ZUBR_CALCULATOR_ZUBR_ACCEPT    goto zubr_calculator_zubr_accept
#define ZUBR_CALCULATOR_ZUBR_NEWERROR  goto zubr_calculator_zubr_newerror
#define ZUBR_CALCULATOR_ZUBR_ERROR     goto zubr_calculator_zubr_errlab


int zubr_calculator_zubr_parse() {
    register int zubr_calculator_zubr_m, zubr_calculator_zubr_n,
        zubr_calculator_zubr_state;
#if ZUBR_CALCULATOR_ZUBR_DEBUG
    register char *zubr_calculator_zubr_s;

    if(zubr_calculator_zubr_s = getenv("ZUBR_DEBUG")) {
        zubr_calculator_zubr_n = *zubr_calculator_zubr_s;
        if(zubr_calculator_zubr_n >= '0' && zubr_calculator_zubr_n <= '9')
            zubr_calculator_zubr_debug = zubr_calculator_zubr_n - '0';
    }
#endif

    zubr_calculator_zubr_nerrs = 0;
    zubr_calculator_zubr_errflag = 0;
    zubr_calculator_zubr_char = (-1);

    zubr_calculator_zubr_ssp = zubr_calculator_zubr_ss;
    zubr_calculator_zubr_vsp = zubr_calculator_zubr_vs;
    *zubr_calculator_zubr_ssp = zubr_calculator_zubr_state = 0;

zubr_calculator_zubr_loop:
    zubr_calculator_zubr_n = zubr_calculator_zubr_defred[zubr_calculator_zubr_state];
    if(zubr_calculator_zubr_n != 0)
        goto zubr_calculator_zubr_reduce;
    if(zubr_calculator_zubr_char < 0) {
        if((zubr_calculator_zubr_char = zubr_calculator_zubr_lex()) < 0)
            zubr_calculator_zubr_char = 0;
#if ZUBR_CALCULATOR_ZUBR_DEBUG
        if(zubr_calculator_zubr_debug) {
            zubr_calculator_zubr_s = 0;
            if(zubr_calculator_zubr_char <= ZUBR_CALCULATOR_ZUBR_MAXTOKEN)
                zubr_calculator_zubr_s =
                    zubr_calculator_zubr_name[zubr_calculator_zubr_char];
            if(!zubr_calculator_zubr_s)
                zubr_calculator_zubr_s = "illegal-symbol";
            printf(
                "zubr_calculator_zubr_debug: state %d, reading %d (%s)\n",
                zubr_calculator_zubr_state,
                zubr_calculator_zubr_char,
                zubr_calculator_zubr_s);
        }
#endif
    } /*  End if( zubr_calculator_zubr_char < 0 )  */

    if((zubr_calculator_zubr_n =
            zubr_calculator_zubr_sindex[zubr_calculator_zubr_state]) &&
       (zubr_calculator_zubr_n += zubr_calculator_zubr_char) >= 0 &&
       zubr_calculator_zubr_n <= ZUBR_CALCULATOR_ZUBR_TABLESIZE &&
       zubr_calculator_zubr_check[zubr_calculator_zubr_n] == zubr_calculator_zubr_char) {
#if ZUBR_CALCULATOR_ZUBR_DEBUG
        if(zubr_calculator_zubr_debug)
            printf(
                "zubr_calculator_zubr_debug: state %d, shifting to state %d\n",
                zubr_calculator_zubr_state,
                zubr_calculator_zubr_table[zubr_calculator_zubr_n]);
#endif
        if(zubr_calculator_zubr_ssp >=
           zubr_calculator_zubr_ss + zubr_calculator_zubr_stacksize - 1) {
            goto zubr_calculator_zubr_overflow;
        }
        *++zubr_calculator_zubr_ssp = zubr_calculator_zubr_state =
            zubr_calculator_zubr_table[zubr_calculator_zubr_n];
        *++zubr_calculator_zubr_vsp = zubr_calculator_zubr_lval;
        zubr_calculator_zubr_char = (-1);
        if(zubr_calculator_zubr_errflag > 0)
            --zubr_calculator_zubr_errflag;
        goto zubr_calculator_zubr_loop;
    }

    if((zubr_calculator_zubr_n =
            zubr_calculator_zubr_rindex[zubr_calculator_zubr_state]) &&
       (zubr_calculator_zubr_n += zubr_calculator_zubr_char) >= 0 &&
       zubr_calculator_zubr_n <= ZUBR_CALCULATOR_ZUBR_TABLESIZE &&
       zubr_calculator_zubr_check[zubr_calculator_zubr_n] == zubr_calculator_zubr_char) {
        zubr_calculator_zubr_n = zubr_calculator_zubr_table[zubr_calculator_zubr_n];
        goto zubr_calculator_zubr_reduce;
    }
    if(zubr_calculator_zubr_errflag)
        goto zubr_calculator_zubr_inrecovery;
#ifdef zubr_calculator_not_defined_zubr_sccsid
    goto zubr_calculator_zubr_newerror;
#endif
zubr_calculator_zubr_newerror:
    zubr_calculator_zubr_error("syntax error");
#ifdef zubr_calculator_not_defined_zubr_sccsid
    goto zubr_calculator_zubr_errlab;
#endif
zubr_calculator_zubr_errlab:
    ++zubr_calculator_zubr_nerrs;
zubr_calculator_zubr_inrecovery:
    if(zubr_calculator_zubr_errflag < 3) {
        zubr_calculator_zubr_errflag = 3;
        for(;;) {
            if((zubr_calculator_zubr_n =
                    zubr_calculator_zubr_sindex[*zubr_calculator_zubr_ssp]) &&
               (zubr_calculator_zubr_n += ZUBR_CALCULATOR_ZUBR_ERRCODE) >= 0 &&
               zubr_calculator_zubr_n <= ZUBR_CALCULATOR_ZUBR_TABLESIZE &&
               zubr_calculator_zubr_check[zubr_calculator_zubr_n] ==
                   ZUBR_CALCULATOR_ZUBR_ERRCODE) {
#if ZUBR_CALCULATOR_ZUBR_DEBUG
                if(zubr_calculator_zubr_debug)
                    printf(
                        "zubr_calculator_zubr_debug: state %d, error recovery shifting\
 to state %d\n",
                        *zubr_calculator_zubr_ssp,
                        zubr_calculator_zubr_table[zubr_calculator_zubr_n]);
#endif
                if(zubr_calculator_zubr_ssp >=
                   zubr_calculator_zubr_ss + zubr_calculator_zubr_stacksize - 1) {
                    goto zubr_calculator_zubr_overflow;
                }
                *++zubr_calculator_zubr_ssp = zubr_calculator_zubr_state =
                    zubr_calculator_zubr_table[zubr_calculator_zubr_n];
                *++zubr_calculator_zubr_vsp = zubr_calculator_zubr_lval;
                goto zubr_calculator_zubr_loop;
            } else {
#if ZUBR_CALCULATOR_ZUBR_DEBUG
                if(zubr_calculator_zubr_debug)
                    printf(
                        "zubr_calculator_zubr_debug: error recovery discarding state %d\n",
                        *zubr_calculator_zubr_ssp);
#endif
                if(zubr_calculator_zubr_ssp <= zubr_calculator_zubr_ss)
                    goto zubr_calculator_zubr_abort;
                --zubr_calculator_zubr_ssp;
                --zubr_calculator_zubr_vsp;
            }
        } /*  End of for( ;; )  */
    }     /*  End if( zubr_calculator_zubr_errflag < 3 )  */
    else {
        if(zubr_calculator_zubr_char == 0)
            goto zubr_calculator_zubr_abort;
#if ZUBR_CALCULATOR_ZUBR_DEBUG
        if(zubr_calculator_zubr_debug) {
            zubr_calculator_zubr_s = 0;
            if(zubr_calculator_zubr_char <= ZUBR_CALCULATOR_ZUBR_MAXTOKEN)
                zubr_calculator_zubr_s =
                    zubr_calculator_zubr_name[zubr_calculator_zubr_char];
            if(!zubr_calculator_zubr_s)
                zubr_calculator_zubr_s = "illegal-symbol";
            printf(
                "zubr_calculator_zubr_debug: state %d, error recovery discards token %d (%s)\n",
                zubr_calculator_zubr_state,
                zubr_calculator_zubr_char,
                zubr_calculator_zubr_s);
        }
#endif
        zubr_calculator_zubr_char = (-1);
        goto zubr_calculator_zubr_loop;
    }
zubr_calculator_zubr_reduce:
#if ZUBR_CALCULATOR_ZUBR_DEBUG
    if(zubr_calculator_zubr_debug)
        printf(
            "zubr_calculator_zubr_debug: state %d, reducing by rule %d (%s)\n",
            zubr_calculator_zubr_state,
            zubr_calculator_zubr_n,
            zubr_calculator_zubr_rule[zubr_calculator_zubr_n]);
#endif
    zubr_calculator_zubr_m = zubr_calculator_zubr_len[zubr_calculator_zubr_n];
    zubr_calculator_zubr_val = zubr_calculator_zubr_vsp[1 - zubr_calculator_zubr_m];

    switch(zubr_calculator_zubr_n) {
    case 3:
#line 37 "calculator.y"
    {
        code2((INST)void_pop, STOP);
        return (1);
    } break;
    case 4:
#line 38 "calculator.y"
    {
        code2((INST)void_pop, STOP);
        return (1);
    } break;
    case 5:
#line 39 "calculator.y"
    {
        code(STOP);
        return (1);
    } break;
    case 6:
#line 41 "calculator.y"
    {
        zubr_calculator_zubr_error(NULL);
    } break;
    case 7:
#line 46 "calculator.y"
    {
        code2((INST)constpush, (INST)zubr_calculator_zubr_vsp[0].sym);
    } break;
    case 8:
#line 47 "calculator.y"
    {
        code3((INST)varpush, (INST)zubr_calculator_zubr_vsp[0].sym, (INST)eval);
    } break;
    case 9:
#line 48 "calculator.y"
    {
        code3((INST)varpush, (INST)zubr_calculator_zubr_vsp[0].inst, (INST)eval);
    } break;
    case 10:
#line 50 "calculator.y"
    {
        zubr_calculator_zubr_val.inst =
            code2((INST)varread, (INST)zubr_calculator_zubr_vsp[-1].sym);
    } break;
    case 11:
#line 52 "calculator.y"
    {
        code2((INST)bltin, (INST)zubr_calculator_zubr_vsp[-3].sym);
    } break;
    case 12:
#line 54 "calculator.y"
    {
        code2((INST)bltin2, (INST)zubr_calculator_zubr_vsp[-5].sym);
    } break;
    case 13:
#line 57 "calculator.y"
    {
        code((INST)Yadd);
    } break;
    case 14:
#line 58 "calculator.y"
    {
        code((INST)Ysub);
    } break;
    case 15:
#line 59 "calculator.y"
    {
        code((INST)Ymul);
    } break;
    case 16:
#line 61 "calculator.y"
    {
        code((INST)Ydiv);
    } break;
    case 17:
#line 63 "calculator.y"
    {
        code((INST)power);
    } break;
    case 18:
#line 64 "calculator.y"
    {
        zubr_calculator_zubr_val.inst = zubr_calculator_zubr_vsp[-1].inst;
    } break;
    case 19:
#line 65 "calculator.y"
    {
        code((INST)negate);
    } break;
    case 20:
#line 70 "calculator.y"
    {
        code3((INST)varpush, (INST)zubr_calculator_zubr_vsp[-2].sym, (INST)assign);
    } break;
    case 21:
#line 71 "calculator.y"
    {
        code3((INST)varpush, (INST)zubr_calculator_zubr_vsp[-2].sym, (INST)Yaddeq);
    } break;
    case 22:
#line 72 "calculator.y"
    {
        code3((INST)varpush, (INST)zubr_calculator_zubr_vsp[-2].sym, (INST)Ysubeq);
    } break;
    case 23:
#line 73 "calculator.y"
    {
        code3((INST)varpush, (INST)zubr_calculator_zubr_vsp[-2].sym, (INST)Ymuleq);
    } break;
    case 24:
#line 74 "calculator.y"
    {
        code3((INST)varpush, (INST)zubr_calculator_zubr_vsp[-2].sym, (INST)Ydiveq);
    } break;
    case 25:
#line 76 "calculator.y"
    {
        code((INST)prexpr);
    } break;
    case 26:
#line 77 "calculator.y"
    {
        zubr_calculator_zubr_val.inst =
            code2((INST)prstr, (INST)zubr_calculator_zubr_vsp[0].sym);
    } break;
    case 27:
#line 78 "calculator.y"
    {
        code((INST)prexpr);
    } break;
    case 28:
#line 79 "calculator.y"
    {
        zubr_calculator_zubr_val.inst =
            code2((INST)prstr, (INST)zubr_calculator_zubr_vsp[0].sym);
    } break;
    case 29:
#line 81 "calculator.y"
    {
        zubr_calculator_zubr_val.inst = zubr_calculator_zubr_vsp[0].inst;
    } break;
#line 508 "zubr_calculator_tab.cpp"

    } /*  End of switch( zubr_calculator_zubr_n )  */

    zubr_calculator_zubr_ssp -= zubr_calculator_zubr_m;
    zubr_calculator_zubr_state = *zubr_calculator_zubr_ssp;
    zubr_calculator_zubr_vsp -= zubr_calculator_zubr_m;
    zubr_calculator_zubr_m = zubr_calculator_zubr_lhs[zubr_calculator_zubr_n];

    if(zubr_calculator_zubr_state == 0 && zubr_calculator_zubr_m == 0) {
#if ZUBR_CALCULATOR_ZUBR_DEBUG
        if(zubr_calculator_zubr_debug)
            printf(
                "zubr_calculator_zubr_debug: after reduction, shifting from state 0 to\
 state %d\n",
                ZUBR_CALCULATOR_ZUBR_FINAL);
#endif
        zubr_calculator_zubr_state = ZUBR_CALCULATOR_ZUBR_FINAL;
        *++zubr_calculator_zubr_ssp = ZUBR_CALCULATOR_ZUBR_FINAL;
        *++zubr_calculator_zubr_vsp = zubr_calculator_zubr_val;
        if(zubr_calculator_zubr_char < 0) {
            if((zubr_calculator_zubr_char = zubr_calculator_zubr_lex()) < 0)
                zubr_calculator_zubr_char = 0;
#if ZUBR_CALCULATOR_ZUBR_DEBUG
            if(zubr_calculator_zubr_debug) {
                zubr_calculator_zubr_s = 0;
                if(zubr_calculator_zubr_char <= ZUBR_CALCULATOR_ZUBR_MAXTOKEN)
                    zubr_calculator_zubr_s =
                        zubr_calculator_zubr_name[zubr_calculator_zubr_char];
                if(!zubr_calculator_zubr_s)
                    zubr_calculator_zubr_s = "illegal-symbol";
                printf(
                    "zubr_calculator_zubr_debug: state %d, reading %d (%s)\n",
                    ZUBR_CALCULATOR_ZUBR_FINAL,
                    zubr_calculator_zubr_char,
                    zubr_calculator_zubr_s);
            }
#endif
        }
        if(zubr_calculator_zubr_char == 0)
            goto zubr_calculator_zubr_accept;
        goto zubr_calculator_zubr_loop;
    } /*  End if( zubr_calculator_zubr_state == 0 && zubr_calculator_zubr_m == 0 )  */

    if((zubr_calculator_zubr_n = zubr_calculator_zubr_gindex[zubr_calculator_zubr_m]) &&
       (zubr_calculator_zubr_n += zubr_calculator_zubr_state) >= 0 &&
       zubr_calculator_zubr_n <= ZUBR_CALCULATOR_ZUBR_TABLESIZE &&
       zubr_calculator_zubr_check[zubr_calculator_zubr_n] == zubr_calculator_zubr_state)
        zubr_calculator_zubr_state = zubr_calculator_zubr_table[zubr_calculator_zubr_n];
    else
        zubr_calculator_zubr_state = zubr_calculator_zubr_dgoto[zubr_calculator_zubr_m];
#if ZUBR_CALCULATOR_ZUBR_DEBUG
    if(zubr_calculator_zubr_debug)
        printf(
            "zubr_calculator_zubr_debug: after reduction, shifting from state %d \
to state %d\n",
            *zubr_calculator_zubr_ssp,
            zubr_calculator_zubr_state);
#endif
    if(zubr_calculator_zubr_ssp >=
       zubr_calculator_zubr_ss + zubr_calculator_zubr_stacksize - 1) {
        goto zubr_calculator_zubr_overflow;
    }
    *++zubr_calculator_zubr_ssp = zubr_calculator_zubr_state;
    *++zubr_calculator_zubr_vsp = zubr_calculator_zubr_val;
    goto zubr_calculator_zubr_loop;
zubr_calculator_zubr_overflow:
    zubr_calculator_zubr_error("zubr stack overflow");
zubr_calculator_zubr_abort:
    return (1);
zubr_calculator_zubr_accept:
    return (0);
}
