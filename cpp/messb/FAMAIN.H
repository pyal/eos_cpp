#ifndef __APPR_BOD_H
#define __APPR_BOD_H



#include "lib/std/util.h"
#include "faobj.h"
#include "fawobj.h"
#include "faregist.h"
#include "lib/std/ex_out.h"
//#include "ex_out.h"
#include "messb_forcelnk.h"
#include "kashuba\kashuba_forcelnk.h"
#include "lib/std/stdexception.h"
#include "urs\curve_forcelnk.h"
#include "urs\urs_forcelnk.h"

//#define MaxFunctions   50

void  Show(char *arguments[],int NumToShow);


FAWZer *WorkFunc;

struct OutParStrc
  {
   int ColFr,ColTo,NumRow,FirstRow,HiCol;
   char nameout[256],namein[256];
   double FrVel,ToVel;
  };

void WriteGr(fstream &in,OutParStrc &OutPar,const char *name);
        
#endif
