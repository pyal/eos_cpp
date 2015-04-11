#include <stdio.h>
#include <stdlib.h>
#include <iomanip.h>
#include <fstream.h>
#include "util.h"
#include "anal.h"
//#define MaxOperation 100
#define MaxDifOper 100
#define MaxCol 512
extern unsigned _stklen=32000;
int UserNumCol;

int UserFuncEnter(istream &in,UserFuncPar &Par,int NumLines=0);
int UserFuncClc(UserFuncPar &Par,double *ResCol,double *InCol);
