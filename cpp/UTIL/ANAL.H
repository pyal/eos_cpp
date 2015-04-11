
#ifndef __ANAL_H
#define __ANAL_H
#define MaxOperation 1256

#include <string.h>

#include <math.h>
//#include "mem.h"
#include "lib/std/util.h"
//#include <iostream.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <ctype.h>
struct Action
  {
   int Type;
   double Val;
   int Num,Imp,act;
  };

struct CellAct
  {
   Action Do,Var;
   friend ostream& operator<<(ostream &output,CellAct &act);
  };    

int DefFunc(char *(&input),int &what);
int GetRow(char *&input, int &col);
int parsestr(char *s,Action *act,int &k);
int PackAct(Action *in);
void EvalAct(Action *in,double *Tab);

int IsInt(char *(&input),int &ans,int MayBeNag=1);
int StrBeg(char *&input,char *s,int Trim=1);
int IsDouble(char *(&input),double &ans,int MayBeNeg=1);

#endif
