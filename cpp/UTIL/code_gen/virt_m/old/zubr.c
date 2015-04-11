
#include <stdio.h>  /* printf() used by parser   if set ZUBR_DEBUG */
#include <stdlib.h> /* getenv() used by parser for test ZUBR_DEBUG */

#define not_defined_zubr_sccsid   1

#define ZUBR_KOSTELTSEV_ZUBR 1
#ifndef not_defined_zubr_sccsid
static char zubr_sccsid[] = "@(#)zubrpar 3.0.0b Andre V.Kosteltsev 19/03/99";
#endif

int zubr_parse();

#line 3 "zubr.y"
#include "zubr_use.h"



char *progname;

FILE *input_file;
char *input_file_name;

int lineno;

program *current_prog;

static unsigned long current_decl_flag;
static unsigned long current_decl_type;

#line 21 "zubr.y"
typedef union
{
   unsigned long      itype;
   z_symbol          *sym;
   enum tetrade_code  code;
} ZUBR_STYPE;
#line 39 "zubr.c"

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

#define ZUBR_ERRCODE 256

int zubr_lhs[] =
{
    -1,     0,     0,    14,    14,    15,    16,    16,    16,    16,
    16,    16,     2,     3,    17,    17,    18,    12,    12,    13,
    13,    13,    13,    11,     1,     1,     1,     1,     1,     1,
     1,     4,     5,     5,     9,     9,     9,     7,     6,     6,
     6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
     6,     6,     6,     6,     6,     6,     6,     8,     8,     8,
     8,     8,     8,     8,    10,    10,
};

int zubr_len[] =
{
     2,     0,     1,     1,     2,     1,     2,     4,     3,     2,
     2,     1,     1,     1,     1,     3,     0,     4,     1,     2,
     4,     3,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     3,     1,     2,     2,     1,     1,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     4,     6,     1,     1,     1,
     3,     4,     2,     2,     1,     2,
};

int zubr_defred[] =
{
     0,     0,     0,    13,    12,    22,    11,     0,     0,     0,
    14,     0,     0,     3,     5,     0,    10,     0,     9,     0,
     0,    16,     0,     4,     6,     0,     0,     8,     0,    24,
    26,    25,     0,    27,    28,     0,    58,    57,    64,     0,
    21,    29,    30,     0,     0,     0,     0,    38,     0,    37,
     0,    15,     7,     0,    17,    35,     0,     0,    36,    20,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    62,    63,     0,
    65,    60,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    42,    41,    43,
     0,     0,    55,    61,     0,    56,
};

int zubr_dgoto[] =
{
     7,    43,     8,     9,    44,    45,    46,    47,    48,    49,
    50,    54,    10,    11,    12,    13,    14,    15,    28,
};

int zubr_sindex[] =
{
   -28,   -46,   -33,     0,     0,     0,     0,     0,   -56,   -33,
     0,   -13,   -28,     0,     0,    36,     0,   -64,     0,   -33,
    50,     0,   158,     0,     0,   -33,    73,     0,   169,     0,
     0,     0,   180,     0,     0,   169,     0,     0,     0,   -11,
     0,     0,     0,   180,   -41,    14,    39,     0,   -89,     0,
  -442,     0,     0,    39,     0,     0,    31,   169,     0,     0,
   169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
   169,   169,   169,   169,   169,   169,   169,     0,     0,   169,
     0,     0,    -1,    39,    39,    39,    54,    65,    82,    93,
   -17,     8,   100,   268,   268,   104,   104,     0,     0,     0,
   -19,   169,     0,     0,    28,     0,
};

int zubr_rindex[] =
{
    85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    90,    88,     0,     0,     0,     0,   -12,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    -3,    18,     0,   -26,     0,
   -37,     0,     0,    95,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    77,    20,   204,   260,   230,   221,   209,
   237,   200,   193,   136,   149,   112,   123,     0,     0,     0,
     0,     0,     0,     0,     0,     0,
};

int zubr_gindex[] =
{
     0,     0,     0,    98,     4,     0,   306,    -9,     0,     0,
     0,     0,    79,   114,     0,   102,     0,    96,     0,
};

#define ZUBR_TABLESIZE 1167
int zubr_table[] =
{
    59,    59,    79,    18,    59,    59,    59,    59,    59,     2,
    59,    34,    34,    16,     2,    34,    34,    34,    34,    34,
    76,    34,    59,    55,    59,    74,    72,    22,    73,    57,
    75,     6,    19,    34,    58,    34,    76,    67,    31,    56,
   102,    74,    72,   101,    73,    76,    75,    19,    21,    19,
    74,    72,    59,    73,    59,    75,    59,    59,    60,    32,
    62,    54,    32,    80,    54,    76,    67,    34,    34,   105,
    74,    72,    81,    73,   103,    75,    76,    67,    22,    54,
    25,    74,    72,   100,    73,     1,    75,    59,     2,    62,
    31,    76,    67,    66,    25,    24,    74,    72,    34,    73,
    62,    75,    76,    67,    51,    20,    19,    74,    72,    27,
    73,    32,    75,    54,    23,    26,    17,    25,    33,    76,
    67,    33,    66,    65,    74,    72,     0,    73,     0,    75,
    76,    67,    52,    66,    18,    74,    72,    76,    73,    23,
    75,    76,    74,    72,     0,    73,    74,    75,    66,    18,
    39,    75,    65,    39,    23,    39,    39,    39,     0,    66,
     0,    40,     0,    65,    40,     0,    40,    40,    40,     0,
    33,    39,     0,    39,    47,     0,    66,    47,    65,     0,
    47,     0,    40,     0,    40,     0,     0,    48,     0,    65,
    48,    42,     0,    48,     0,    47,    29,    47,    35,     0,
    32,    30,    42,    31,     0,    39,    39,    29,    48,    35,
    48,    32,    30,    42,    31,     0,    40,    40,    29,     0,
    35,     0,    32,    30,     0,    31,     0,     0,     1,    47,
    47,    49,     0,     0,    49,     0,    39,    49,    50,     0,
     0,    50,    48,    48,    50,    53,     0,    40,    53,     0,
    46,    40,    49,    46,    49,     0,     0,     0,     0,    50,
    47,    50,    45,    53,     0,    45,     0,     0,    46,     0,
    46,    51,     0,    48,    51,    44,     0,     0,    44,     0,
    45,    44,    45,     0,    41,     0,    49,    49,     0,    51,
     0,    51,     0,    50,    50,    41,    44,    53,    44,     0,
     0,    52,    46,    46,    52,    76,    41,     0,     0,     0,
    74,    72,     0,    73,    45,    75,     0,    49,     0,    52,
     0,    52,     0,    51,    50,     0,     0,     0,     0,     0,
    44,    44,     0,    46,    53,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    45,     0,     0,     0,     0,
     0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
     0,    44,     0,    82,     0,     0,    83,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     5,     0,     0,
     0,     0,     5,     0,     0,     0,     0,     0,     0,     0,
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
    36,    37,     0,    38,     0,     0,     0,     0,     0,     0,
     0,    36,    37,     0,    38,     0,     0,     0,     0,     0,
     0,     0,    36,    37,     0,    38,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     3,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    39,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
     0,     0,     3,     0,     4,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    77,    78,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    59,    59,    59,    59,    59,    59,
    59,     0,     0,    59,    59,    34,    34,     0,     0,    34,
    34,    34,     0,     0,    34,    34,     0,     0,    69,    68,
     0,     0,     0,    70,    71,     0,     0,     0,     0,     0,
    63,    64,     0,     0,    69,    68,    61,     0,     0,    70,
    71,     0,     0,    69,     0,     0,     0,     0,    70,    71,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
    64,     0,     0,    69,    68,    61,     0,     0,    70,    71,
    63,    64,     0,     0,    69,    68,    61,     0,     0,    70,
    71,     0,     0,     0,     0,     0,    64,     0,     0,    69,
    68,     0,     0,     0,    70,    71,     0,     0,     0,     0,
    69,    68,     0,     0,     0,    70,    71,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    69,    68,     0,
     0,     0,    70,    71,     0,     0,     0,     0,    69,    68,
     0,     0,     0,    70,    71,     0,     0,     0,     0,     0,
    70,    71,     0,    39,    39,     0,     0,    39,    39,    39,
     0,     0,    39,    39,    40,    40,     0,     0,    40,    40,
    40,     0,     0,    40,    40,     0,     0,    47,    47,     0,
     0,    47,    47,    47,     0,     0,    47,    47,     0,     0,
    48,    48,     0,     0,    48,    48,    48,     0,     0,    48,
    48,    33,    34,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    33,    34,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    33,    34,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    49,    49,     0,     0,    49,    49,
    49,    50,    50,     0,     0,     0,    50,    50,     0,     0,
    46,    46,     0,     0,     0,     0,    46,     0,     0,     0,
     0,     0,    45,    45,     0,     0,     0,     0,    45,     0,
     0,    51,    51,     0,     0,     0,     0,    51,    44,    44,
     0,     0,     0,     0,    44,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    52,     0,     0,     0,     0,     0,    52,
};

int zubr_check[] =
{
    37,    38,    91,    59,    41,    42,    43,    44,    45,    42,
    47,    37,    38,    59,    42,    41,    42,    43,    44,    45,
    37,    47,    59,    32,    61,    42,    43,    91,    45,    40,
    47,    59,    44,    59,    43,    61,    37,    38,    41,    35,
    41,    42,    43,    44,    45,    37,    47,    59,    61,    61,
    42,    43,    93,    45,    91,    47,    93,    94,    44,    41,
    61,    41,    44,   505,    44,    37,    38,    93,    94,    41,
    42,    43,    41,    45,    93,    47,    37,    38,    91,    59,
    44,    42,    43,    79,    45,     0,    47,   124,     0,    61,
    93,    37,    38,    94,    44,    59,    42,    43,   124,    45,
    61,    47,    37,    38,    25,     9,     8,    42,    43,    59,
    45,    93,    47,    93,    12,    19,     2,    44,    41,    37,
    38,    44,    94,   124,    42,    43,    -1,    45,    -1,    47,
    37,    38,    59,    94,    44,    42,    43,    37,    45,    44,
    47,    37,    42,    43,    -1,    45,    42,    47,    94,    59,
    38,    47,   124,    41,    59,    43,    44,    45,    -1,    94,
    -1,    38,    -1,   124,    41,    -1,    43,    44,    45,    -1,
    93,    59,    -1,    61,    38,    -1,    94,    41,   124,    -1,
    44,    -1,    59,    -1,    61,    -1,    -1,    38,    -1,   124,
    41,    33,    -1,    44,    -1,    59,    38,    61,    40,    -1,
    42,    43,    33,    45,    -1,    93,    94,    38,    59,    40,
    61,    42,    43,    33,    45,    -1,    93,    94,    38,    -1,
    40,    -1,    42,    43,    -1,    45,    -1,    -1,   256,    93,
    94,    38,    -1,    -1,    41,    -1,   124,    44,    38,    -1,
    -1,    41,    93,    94,    44,    41,    -1,   124,    44,    -1,
    41,    93,    59,    44,    61,    -1,    -1,    -1,    -1,    59,
   124,    61,    41,    59,    -1,    44,    -1,    -1,    59,    -1,
    61,    41,    -1,   124,    44,    38,    -1,    -1,    41,    -1,
    59,    44,    61,    -1,   126,    -1,    93,    94,    -1,    59,
    -1,    61,    -1,    93,    94,   126,    59,    93,    61,    -1,
    -1,    41,    93,    94,    44,    37,   126,    -1,    -1,    -1,
    42,    43,    -1,    45,    93,    47,    -1,   124,    -1,    59,
    -1,    61,    -1,    93,   124,    -1,    -1,    -1,    -1,    -1,
    93,    94,    -1,   124,    28,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   124,    -1,    57,    -1,    -1,    60,    61,    62,    63,
    64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   500,    -1,    -1,
    -1,    -1,   500,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
   502,   503,    -1,   505,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   502,   503,    -1,   505,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   502,   503,    -1,   505,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   800,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   600,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   600,
    -1,    -1,   800,    -1,   802,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   903,   904,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   901,   902,   903,   904,   905,   906,
   907,    -1,    -1,   910,   911,   901,   902,    -1,    -1,   905,
   906,   907,    -1,    -1,   910,   911,    -1,    -1,   905,   906,
    -1,    -1,    -1,   910,   911,    -1,    -1,    -1,    -1,    -1,
   901,   902,    -1,    -1,   905,   906,   907,    -1,    -1,   910,
   911,    -1,    -1,   905,    -1,    -1,    -1,    -1,   910,   911,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   901,
   902,    -1,    -1,   905,   906,   907,    -1,    -1,   910,   911,
   901,   902,    -1,    -1,   905,   906,   907,    -1,    -1,   910,
   911,    -1,    -1,    -1,    -1,    -1,   902,    -1,    -1,   905,
   906,    -1,    -1,    -1,   910,   911,    -1,    -1,    -1,    -1,
   905,   906,    -1,    -1,    -1,   910,   911,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   905,   906,    -1,
    -1,    -1,   910,   911,    -1,    -1,    -1,    -1,   905,   906,
    -1,    -1,    -1,   910,   911,    -1,    -1,    -1,    -1,    -1,
   910,   911,    -1,   901,   902,    -1,    -1,   905,   906,   907,
    -1,    -1,   910,   911,   901,   902,    -1,    -1,   905,   906,
   907,    -1,    -1,   910,   911,    -1,    -1,   901,   902,    -1,
    -1,   905,   906,   907,    -1,    -1,   910,   911,    -1,    -1,
   901,   902,    -1,    -1,   905,   906,   907,    -1,    -1,   910,
   911,   903,   904,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   903,   904,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   903,   904,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   901,   902,    -1,    -1,   905,   906,
   907,   901,   902,    -1,    -1,    -1,   906,   907,    -1,    -1,
   901,   902,    -1,    -1,    -1,    -1,   907,    -1,    -1,    -1,
    -1,    -1,   901,   902,    -1,    -1,    -1,    -1,   907,    -1,
    -1,   901,   902,    -1,    -1,    -1,    -1,   907,   901,   902,
    -1,    -1,    -1,    -1,   907,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   901,    -1,    -1,    -1,    -1,    -1,   907,
};

#define ZUBR_FINAL 7
#ifndef ZUBR_DEBUG
#define ZUBR_DEBUG 0
#endif
#define ZUBR_MAXTOKEN 912
#if ZUBR_DEBUG
char *zubr_name[] =
{
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,"'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,
0,0,0,0,0,0,0,"':'","';'",0,"'='",0,"'?'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,"'['",0,"']'","'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,"'|'",0,"'~'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"IDENTIFIER",0,"CONSTANT",
"VARIABLE",0,"STRING",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"MTH_BUILTIN",0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"TYPESPEC",0,
"TYPE_QUAL",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"OROR","ANDAND","PLUSPLUS",
"MINUSMINUS","ARITHCOMPARE","EQCOMPARE","ASSIGN","UNARY","HYPERUNARY","LSHIFT",
"RSHIFT","POINTSAT",
};

char *zubr_rule[] =
{
"$accept : program",
"program :",
"program : extdefs",
"extdefs : extdef",
"extdefs : extdefs extdef",
"extdef : datadef",
"datadef : initdecls ';'",
"datadef : decl_mods type_spec initdecls ';'",
"datadef : type_spec initdecls ';'",
"datadef : decl_mods ';'",
"datadef : error ';'",
"datadef : ';'",
"decl_mods : TYPE_QUAL",
"type_spec : TYPESPEC",
"initdecls : initdcl",
"initdecls : initdecls ',' initdcl",
"$$1 :",
"initdcl : declarator '=' $$1 init",
"initdcl : declarator",
"declarator : '*' declarator",
"declarator : declarator '[' expr ']'",
"declarator : declarator '[' ']'",
"declarator : IDENTIFIER",
"init : expr_no_commas",
"unop : '&'",
"unop : '-'",
"unop : '+'",
"unop : PLUSPLUS",
"unop : MINUSMINUS",
"unop : '~'",
"unop : '!'",
"expr : nonull_exprlist",
"nonull_exprlist : expr_no_commas",
"nonull_exprlist : nonull_exprlist ',' expr_no_commas",
"unary_expr : primary",
"unary_expr : '*' cast_expr",
"unary_expr : unop cast_expr",
"cast_expr : unary_expr",
"expr_no_commas : cast_expr",
"expr_no_commas : expr_no_commas '+' expr_no_commas",
"expr_no_commas : expr_no_commas '-' expr_no_commas",
"expr_no_commas : expr_no_commas '/' expr_no_commas",
"expr_no_commas : expr_no_commas '*' expr_no_commas",
"expr_no_commas : expr_no_commas '%' expr_no_commas",
"expr_no_commas : expr_no_commas '&' expr_no_commas",
"expr_no_commas : expr_no_commas '|' expr_no_commas",
"expr_no_commas : expr_no_commas '^' expr_no_commas",
"expr_no_commas : expr_no_commas LSHIFT expr_no_commas",
"expr_no_commas : expr_no_commas RSHIFT expr_no_commas",
"expr_no_commas : expr_no_commas ARITHCOMPARE expr_no_commas",
"expr_no_commas : expr_no_commas EQCOMPARE expr_no_commas",
"expr_no_commas : expr_no_commas ANDAND expr_no_commas",
"expr_no_commas : expr_no_commas OROR expr_no_commas",
"expr_no_commas : expr_no_commas '=' expr_no_commas",
"expr_no_commas : expr_no_commas ASSIGN expr_no_commas",
"expr_no_commas : MTH_BUILTIN '(' expr_no_commas ')'",
"expr_no_commas : MTH_BUILTIN '(' expr_no_commas ',' expr_no_commas ')'",
"primary : VARIABLE",
"primary : CONSTANT",
"primary : string",
"primary : '(' expr ')'",
"primary : primary '[' expr ']'",
"primary : primary PLUSPLUS",
"primary : primary MINUSMINUS",
"string : STRING",
"string : string STRING",
};
#endif

#define     zubr_clearin    (zubr_char=(-1))
#define     zubr_errok      (zubr_errflag=0)
#ifdef  ZUBR_STACKSIZE
#ifndef ZUBR_MAXDEPTH
#define     ZUBR_MAXDEPTH   ZUBR_STACKSIZE
#endif
#else
#ifdef  ZUBR_MAXDEPTH
#define     ZUBR_STACKSIZE  ZUBR_MAXDEPTH
#else
#define     ZUBR_STACKSIZE  500
#define     ZUBR_MAXDEPTH   500
#endif
#endif

#if ZUBR_DEBUG
int            zubr_debug;
#endif
int            zubr_nerrs;
int            zubr_errflag;
int            zubr_char;
int           *zubr_ssp;

ZUBR_STYPE    *zubr_vsp;
ZUBR_STYPE     zubr_val;
ZUBR_STYPE     zubr_lval;

int            zubr_ss[ZUBR_STACKSIZE];

ZUBR_STYPE     zubr_vs[ZUBR_STACKSIZE];

#define        zubr_stacksize  ZUBR_STACKSIZE

#line 277 "zubr.y"
/*********************  END OF GRAMMAR  ***********************/


void zubr_error( char *s )
{
   printf( "%s\n" );
}

int main_zubr( int argc, char *argv[] )
{
   int c;
//   bucket b, *fn, *rc, *op1, *op2;


   progname = argv[0];
   if (argc < 2)
   {
      fprintf( stderr, "usage: %s file.name\n", progname );
      exit( 1 );
   }

   input_file_name = argv[1];

   if( (input_file = fopen( input_file_name, "rb" )) == NULL )
   {
      printf( "Cannot open file: %s\n", input_file_name );
      exit( 1 );
   }

   init();

   zubr_parse();

   if( input_file ) fclose( input_file );


   execute_program( current_prog );


   free_program( current_prog );
   free_symbol_table();


   return( 0 );
}

/**********************  END OF MAIN.Y  ***********************/

#line 575 "zubr.c"

#define ZUBR_ABORT     goto zubr_abort
#define ZUBR_ACCEPT    goto zubr_accept
#define ZUBR_NEWERROR  goto zubr_newerror
#define ZUBR_ERROR     goto zubr_errlab


int zubr_parse()
{
   register int zubr_m, zubr_n, zubr_state;
#if ZUBR_DEBUG
   register char *zubr_s;

   if( zubr_s = getenv("ZUBR_DEBUG") )
   {
      zubr_n = *zubr_s;
      if( zubr_n >= '0' && zubr_n <= '9' ) zubr_debug = zubr_n - '0';
   }
#endif

   zubr_nerrs   = 0;
   zubr_errflag = 0;
   zubr_char    = (-1);

   zubr_ssp     = zubr_ss;
   zubr_vsp     = zubr_vs;
  *zubr_ssp     = zubr_state = 0;

zubr_loop:
   if( zubr_n = zubr_defred[zubr_state] ) goto zubr_reduce;
   if( zubr_char < 0 )
   {
      if( (zubr_char = zubr_lex()) < 0 ) zubr_char = 0;
#if ZUBR_DEBUG
      if( zubr_debug )
      {
         zubr_s = 0;
         if( zubr_char <= ZUBR_MAXTOKEN ) zubr_s = zubr_name[zubr_char];
         if( !zubr_s ) zubr_s = "illegal-symbol";
         printf( "zubr_debug: state %d, reading %d (%s)\n",
                 zubr_state, zubr_char, zubr_s );
      }
#endif
   } /*  End if( zubr_char < 0 )  */

   if( (zubr_n = zubr_sindex[zubr_state]) && (zubr_n += zubr_char) >= 0 &&
       zubr_n <= ZUBR_TABLESIZE && zubr_check[zubr_n] == zubr_char )
   {
#if ZUBR_DEBUG
      if( zubr_debug )
         printf( "zubr_debug: state %d, shifting to state %d\n",
                 zubr_state, zubr_table[zubr_n] );
#endif
      if( zubr_ssp >= zubr_ss + zubr_stacksize - 1 )
      {
         goto zubr_overflow;
      }
      *++zubr_ssp = zubr_state = zubr_table[zubr_n];
      *++zubr_vsp = zubr_lval;
      zubr_char = (-1);
      if( zubr_errflag > 0 ) --zubr_errflag;
      goto zubr_loop;
   }

   if( (zubr_n = zubr_rindex[zubr_state]) && (zubr_n += zubr_char) >= 0 &&
       zubr_n <= ZUBR_TABLESIZE && zubr_check[zubr_n] == zubr_char )
   {
      zubr_n = zubr_table[zubr_n];
      goto zubr_reduce;
   }
   if (zubr_errflag) goto zubr_inrecovery;
#ifdef not_defined_zubr_sccsid
   goto zubr_newerror;
#endif
zubr_newerror:
   zubr_error( "syntax error" );
#ifdef not_defined_zubr_sccsid
   goto zubr_errlab;
#endif
zubr_errlab:
   ++zubr_nerrs;
zubr_inrecovery:
   if( zubr_errflag < 3 )
   {
      zubr_errflag = 3;
      for( ;; )
      {
         if( (zubr_n = zubr_sindex[*zubr_ssp]) && (zubr_n += ZUBR_ERRCODE) >= 0 &&
             zubr_n <= ZUBR_TABLESIZE && zubr_check[zubr_n] == ZUBR_ERRCODE )
         {
#if ZUBR_DEBUG
            if( zubr_debug )
               printf( "zubr_debug: state %d, error recovery shifting\
 to state %d\n", *zubr_ssp, zubr_table[zubr_n] );
#endif
            if( zubr_ssp >= zubr_ss + zubr_stacksize - 1 )
            {
               goto zubr_overflow;
            }
            *++zubr_ssp = zubr_state = zubr_table[zubr_n];
            *++zubr_vsp = zubr_lval;
            goto zubr_loop;
         }
         else
         {
#if ZUBR_DEBUG
            if( zubr_debug )
               printf( "zubr_debug: error recovery discarding state %d\n",
                       *zubr_ssp );
#endif
            if( zubr_ssp <= zubr_ss ) goto zubr_abort;
            --zubr_ssp;
            --zubr_vsp;
         }
      } /*  End of for( ;; )  */
   } /*  End if( zubr_errflag < 3 )  */
   else
   {
      if( zubr_char == 0 ) goto zubr_abort;
#if ZUBR_DEBUG
      if( zubr_debug )
      {
         zubr_s = 0;
         if( zubr_char <= ZUBR_MAXTOKEN ) zubr_s = zubr_name[zubr_char];
         if( !zubr_s ) zubr_s = "illegal-symbol";
printf( "zubr_debug: state %d, error recovery discards token %d (%s)\n",
        zubr_state, zubr_char, zubr_s );
      }
#endif
      zubr_char = (-1);
      goto zubr_loop;
   }
zubr_reduce:
#if ZUBR_DEBUG
   if( zubr_debug )
      printf( "zubr_debug: state %d, reducing by rule %d (%s)\n",
              zubr_state, zubr_n, zubr_rule[zubr_n] );
#endif
   zubr_m   = zubr_len[zubr_n];
   zubr_val = zubr_vsp[1-zubr_m];

   switch( zubr_n )
   {
      case 1:
#line 65 "zubr.y"
         { ; /* finish file */ }
         break;
      case 6:
#line 79 "zubr.y"
         {
            current_decl_flag = 0;
            fprintf( stderr,
            "%s: %s:%d: WARNING: data definition with no type or storage class\n",
                     progname, input_file_name, lineno );
         }
         break;
      case 7:
#line 86 "zubr.y"
         {
            current_decl_type = 0;
            current_decl_flag = 0;
         }
         break;
      case 8:
#line 91 "zubr.y"
         {
            current_decl_type = 0;
            current_decl_flag = 0;
         }
         break;
      case 9:
#line 96 "zubr.y"
         {
            current_decl_flag = 0;
            current_decl_type = 0;
            fprintf( stderr,
            "%s: %s:%d: WARNING: empty declaration\n",
                     progname, input_file_name, lineno );
         }
         break;
      case 10:
#line 103 "zubr.y"
         { current_decl_type = 0;current_decl_flag=0; zubr_errok; }
         break;
      case 11:
#line 105 "zubr.y"
         {
            current_decl_type = 0;
            current_decl_flag = 0;
            fprintf( stderr,
            "%s: %s:%d: WARNING: not allow extra ';' outside of a function\n",
                     progname, input_file_name, lineno );
         }
         break;
      case 12:
#line 116 "zubr.y"
         { zubr_val.itype = (current_decl_flag |= zubr_vsp[0].itype); }
         break;
      case 13:
#line 121 "zubr.y"
         { zubr_val.itype = (current_decl_type = zubr_vsp[0].itype); }
         break;
      case 16:
#line 133 "zubr.y"
         {
            init_decl_prog(); /* set current_prog to init_declaration_prog */
         }
         break;
      case 17:
#line 137 "zubr.y"
         {
            zubr_val.sym = build_modify_expr( zubr_vsp[-3].sym, NOP_EXPR, zubr_vsp[0].sym );
            execute_program( current_prog ); /* exec init_declaration_prog */
            /* DELETE TMP VARIABLE & CONSTANT */
            free_program( current_prog ); /* FREE init_declaration_prog */
            goto_text_prog(); /* RETURN TO GENERATION TEXT PROGRAM */
         }
         break;
      case 19:
#line 149 "zubr.y"
         {
            zubr_val.sym = make_pointer_declarator( zubr_vsp[0].sym );
         }
         break;
      case 20:
#line 153 "zubr.y"
         {
            zubr_val.sym = build_array( zubr_vsp[-3].sym, zubr_vsp[-1].sym );
         }
         break;
      case 21:
#line 157 "zubr.y"
         {
            zubr_val.sym = build_array( zubr_vsp[-2].sym, (z_symbol *)0 );
         }
         break;
      case 22:
#line 161 "zubr.y"
         {
			zubr_val.sym = init_identifier(zubr_vsp[0].sym,current_decl_type,current_decl_flag);
         }
         break;
      case 23:
#line 169 "zubr.y"
         { zubr_val.sym = zubr_vsp[0].sym;}
         break;
      case 24:
#line 176 "zubr.y"
         { zubr_val.code = ADDR_EXPR;         }
         break;
      case 25:
#line 177 "zubr.y"
         { zubr_val.code = NEGATE_EXPR;       }
         break;
      case 26:
#line 178 "zubr.y"
         { zubr_val.code = CONVERT_EXPR;      }
         break;
      case 27:
#line 179 "zubr.y"
         { zubr_val.code = PREINCREMENT_EXPR; }
         break;
      case 28:
#line 180 "zubr.y"
         { zubr_val.code = PREDECREMENT_EXPR; }
         break;
      case 29:
#line 181 "zubr.y"
         { zubr_val.code = BIT_NOT_EXPR;      }
         break;
      case 30:
#line 182 "zubr.y"
         { zubr_val.code = TRUTH_NOT_EXPR;    }
         break;
      case 33:
#line 192 "zubr.y"
         { zubr_val.sym = zubr_vsp[0].sym; }
         break;
      case 35:
#line 198 "zubr.y"
         { zubr_val.sym = build_indirect_ref( zubr_vsp[0].sym, "unary *" ); }
         break;
      case 36:
#line 200 "zubr.y"
         { zubr_val.sym = build_unary_op( zubr_vsp[-1].code, zubr_vsp[0].sym ); }
         break;
      case 39:
#line 210 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 40:
#line 212 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 41:
#line 214 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 42:
#line 216 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 43:
#line 218 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 44:
#line 220 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 45:
#line 222 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 46:
#line 224 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 47:
#line 226 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 48:
#line 228 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 49:
#line 230 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 50:
#line 232 "zubr.y"
         { zubr_val.sym = build_binary_op( zubr_vsp[-1].code, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 51:
#line 234 "zubr.y"
         { zubr_val.sym = build_binary_op( ANDIF_EXPR, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 52:
#line 236 "zubr.y"
         { zubr_val.sym = build_binary_op( ORIF_EXPR, zubr_vsp[-2].sym, zubr_vsp[0].sym ); }
         break;
      case 53:
#line 238 "zubr.y"
         { zubr_val.sym = build_modify_expr( zubr_vsp[-2].sym, NOP_EXPR, zubr_vsp[0].sym ); }
         break;
      case 54:
#line 240 "zubr.y"
         { zubr_val.sym = build_modify_expr( zubr_vsp[-2].sym, zubr_vsp[-1].code, zubr_vsp[0].sym ); }
         break;
      case 55:
#line 242 "zubr.y"
         {
            zubr_val.sym = build_mth_builtin1_fn( zubr_vsp[-3].sym, zubr_vsp[-1].sym );
         }
         break;
      case 56:
#line 246 "zubr.y"
         {
            zubr_val.sym = build_mth_builtin2_fn( zubr_vsp[-5].sym, zubr_vsp[-3].sym, zubr_vsp[-1].sym );
         }
         break;
      case 60:
#line 255 "zubr.y"
         { zubr_val.sym = zubr_vsp[-1].sym; }
         break;
      case 61:
#line 257 "zubr.y"
         {
            zubr_val.sym = build_array_ref( zubr_vsp[-3].sym, zubr_vsp[-1].sym );
         }
         break;
      case 62:
#line 261 "zubr.y"
         { zubr_val.sym = build_unary_op( POSTINCREMENT_EXPR, zubr_vsp[-1].sym ); }
         break;
      case 63:
#line 263 "zubr.y"
         { zubr_val.sym = build_unary_op( POSTDECREMENT_EXPR, zubr_vsp[-1].sym ); }
         break;
      case 65:
#line 269 "zubr.y"
         { zubr_val.sym = build_chainon_string( zubr_vsp[-1].sym, zubr_vsp[0].sym ); }
         break;
#line 960 "zubr.c"

   } /*  End of switch( zubr_n )  */

   zubr_ssp   -=  zubr_m;
   zubr_state  = *zubr_ssp;
   zubr_vsp   -=  zubr_m;
   zubr_m      =  zubr_lhs[zubr_n];

   if( zubr_state == 0 && zubr_m == 0 )
   {
#if ZUBR_DEBUG
      if( zubr_debug )
         printf( "zubr_debug: after reduction, shifting from state 0 to\
 state %d\n", ZUBR_FINAL );
#endif
      zubr_state  = ZUBR_FINAL;
      *++zubr_ssp = ZUBR_FINAL;
      *++zubr_vsp = zubr_val;
      if( zubr_char < 0 )
      {
         if( (zubr_char = zubr_lex()) < 0 ) zubr_char = 0;
#if ZUBR_DEBUG
         if( zubr_debug )
         {
            zubr_s = 0;
            if( zubr_char <= ZUBR_MAXTOKEN) zubr_s = zubr_name[zubr_char];
            if( !zubr_s ) zubr_s = "illegal-symbol";
            printf( "zubr_debug: state %d, reading %d (%s)\n",
                    ZUBR_FINAL, zubr_char, zubr_s);
         }
#endif
      }
      if( zubr_char == 0 ) goto zubr_accept;
      goto zubr_loop;
   } /*  End if( zubr_state == 0 && zubr_m == 0 )  */

   if( (zubr_n = zubr_gindex[zubr_m]) && (zubr_n += zubr_state) >= 0 &&
       zubr_n <= ZUBR_TABLESIZE && zubr_check[zubr_n] == zubr_state )
      zubr_state = zubr_table[zubr_n];
   else
      zubr_state = zubr_dgoto[zubr_m];
#if ZUBR_DEBUG
   if( zubr_debug )
      printf( "zubr_debug: after reduction, shifting from state %d \
to state %d\n", *zubr_ssp, zubr_state );
#endif
   if( zubr_ssp >= zubr_ss + zubr_stacksize - 1 )
   {
      goto zubr_overflow;
   }
   *++zubr_ssp = zubr_state;
   *++zubr_vsp = zubr_val;
   goto zubr_loop;
zubr_overflow:
   zubr_error( "zubr stack overflow" );
zubr_abort:
   return( 1 );
zubr_accept:
   return( 0 );
}
