
#include <stdio.h>  /* printf() used by parser   if set ZUBR_DEBUG */
#include <stdlib.h> /* getenv() used by parser for test ZUBR_DEBUG */

#define zubr_calculator_not_defined_zubr_sccsid   1

#define ZUBR_KOSTELTSEV_ZUBR 1
#ifndef zubr_calculator_not_defined_zubr_sccsid
static char zubr_sccsid[] = "@(#)zubrpar 3.0.0b Andre V.Kosteltsev 19/03/99";
#endif

int zubr_calculator_zubr_parse();

#line 7 "test.y"
#include HEAD
#line 10 "test.y"
typedef union
{
   SYMBOL *sym;
   INST   *inst;
} ZUBR_CALCULATOR_ZUBR_STYPE;
#line 23 "zubr_calculator_tab.c"

#define NUMBER 1024
#define VAR 257
#define BLTIN 258
#define UNDEF 259
#define STRING 260

#define ZUBR_CALCULATOR_ZUBR_ERRCODE 256

int zubr_calculator_zubr_lhs[] =
{
    -1,     0,     0,     0,     0,     1,     1,     1,     1,     1,
     1,     1,     1,     2,
};

int zubr_calculator_zubr_len[] =
{
     2,     0,     2,     3,     3,     1,     1,     1,     3,     3,
     3,     3,     3,     3,
};

int zubr_calculator_zubr_defred[] =
{
     1,     0,     0,     0,     2,     0,     4,     0,     3,     5,
     0,     0,     7,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,
};

int zubr_calculator_zubr_dgoto[] =
{
     1,    11,    12,
};

int zubr_calculator_zubr_sindex[] =
{
     0,    -9,    -3,   -50,     0,     2,     0,  -257,     0,     0,
   -50,   -39,     0,  -257,  -257,  -257,  -257,  -257,   -37,   -37,
   -81,   -81,   -81,
};

int zubr_calculator_zubr_rindex[] =
{
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -8,     4,     0,     0,     0,     0,     0,     0,     6,    16,
    -1,     5,    11,
};

int zubr_calculator_zubr_gindex[] =
{
     0,    14,    17,
};

#define ZUBR_CALCULATOR_ZUBR_TABLESIZE 767
int zubr_calculator_zubr_table[] =
{
    10,     4,     6,    15,    13,    15,    14,     6,    16,    10,
    16,     7,     8,    17,    13,    11,     8,     0,     5,     0,
     0,    12,     0,     0,     0,     0,     9,    18,    19,    20,
    21,    22,     0,     0,     6,     6,     0,     6,     0,     6,
     0,    10,    10,     0,    10,     0,    10,    11,    11,     8,
    11,     8,    11,    12,    12,    17,    12,    17,    12,     9,
     0,     9,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     6,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     2,     3,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     9,
};

int zubr_calculator_zubr_check[] =
{
   257,    10,    10,    42,    43,    42,    45,    10,    47,    10,
    47,    61,    10,    94,    10,    10,    10,    -1,     1,    -1,
    -1,    10,    -1,    -1,    -1,    -1,    10,    13,    14,    15,
    16,    17,    -1,    -1,    42,    43,    -1,    45,    -1,    47,
    -1,    42,    43,    -1,    45,    -1,    47,    42,    43,    43,
    45,    45,    47,    42,    43,    94,    45,    94,    47,    43,
    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   256,   257,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1024,
};

#define ZUBR_CALCULATOR_ZUBR_FINAL 1
#ifndef ZUBR_CALCULATOR_ZUBR_DEBUG
#define ZUBR_CALCULATOR_ZUBR_DEBUG 0
#endif
#define ZUBR_CALCULATOR_ZUBR_MAXTOKEN 1024
#if ZUBR_CALCULATOR_ZUBR_DEBUG
char *zubr_calculator_zubr_name[] =
{
"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,"'*'","'+'",0,"'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,0,0,
"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'^'",0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"VAR","BLTIN","UNDEF","STRING",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"NUMBER",
};

char *zubr_calculator_zubr_rule[] =
{
"$accept : list",
"list :",
"list : list '\\n'",
"list : list assign '\\n'",
"list : list error '\\n'",
"expr : NUMBER",
"expr : VAR",
"expr : assign",
"expr : expr '+' expr",
"expr : expr '-' expr",
"expr : expr '*' expr",
"expr : expr '/' expr",
"expr : expr '^' expr",
"assign : VAR '=' expr",
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

#line 49 "test.y"

/********************  END OF GRAMMAR  ***********************/

Program
#line 333 "zubr_calculator_tab.c"

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
   if( zubr_calculator_zubr_n = zubr_calculator_zubr_defred[zubr_calculator_zubr_state] ) goto zubr_calculator_zubr_reduce;
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
#line 27 "test.y"
         { code2( (INST)void_pop, STOP ); return( 1 ); }
         break;
      case 4:
#line 29 "test.y"
         {
            zubr_errok;
         }
         break;
      case 5:
#line 34 "test.y"
         { code2( (INST)constpush, (INST)zubr_calculator_zubr_vsp[0].sym ); }
         break;
      case 6:
#line 35 "test.y"
         { code3( (INST)varpush, (INST)zubr_calculator_zubr_vsp[0].sym, (INST)eval ); }
         break;
      case 7:
#line 36 "test.y"
         { code3( (INST)varpush, (INST)zubr_calculator_zubr_vsp[0].inst, (INST)eval ); }
         break;
      case 8:
#line 37 "test.y"
         { code( (INST)Yadd );   }
         break;
      case 9:
#line 38 "test.y"
         { code( (INST)Ysub );   }
         break;
      case 10:
#line 39 "test.y"
         { code( (INST)Ymul );   }
         break;
      case 11:
#line 41 "test.y"
         { code( (INST)Ydiv );   }
         break;
      case 12:
#line 43 "test.y"
         { code( (INST)power );  }
         break;
      case 13:
#line 46 "test.y"
         { code3( (INST)varpush, (INST)zubr_calculator_zubr_vsp[-2].sym, (INST)assign ); }
         break;
#line 523 "zubr_calculator_tab.c"

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
