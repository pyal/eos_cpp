#include "user_fun.h"
#define MaxUserOperator 100
struct UserFuncPar
  {
   Action *act[MaxUserOperator];
   int NumAct;
   UserFuncPar(){NumAct=0;}
   ~UserFuncPar()
     {
      for (int k=0;k<NumAct;k++) {delete act[k];act[k]=NULL;}
      NumAct=0;
     };
  }; 
int UserFuncEnter(istream &in,UserFuncPar &Par,int NumOperator=0)
  {
   NumOperator=min(MaxUserOperator,NumOperator);
   if (NumOperator==0) NumOperator=MaxUserOperator;
   RandomInit();
   char s[512];
   int c,k=0;
   while ( (k<NumOperator) && (!seek_eof(in)))
     {
      do {if (seek_eof(in)) goto exit_proc;in.getline(s,511);}
      while (strncmp(s,"//",2)==0) ;
      if (Stricmp(s,"EndFunc",7)==0) goto exit_proc;
      Par.act[k]=new Action[MaxOperation];
      parsestr(s,Par.act[k],c);
      PackAct(Par.act[k]);
      k++
     }
  exit_proc:
   Par.NumAct=k;
  }
 
int UserFuncClc(UserFuncPar &Par,double *ResCol,double *InCol,int NumCol);
  {
   if (Par.NumAct==0) return 0;
   MemoryMove(InCol,ResCol,sizeof(double)*NumCol);
   for (int k=0;k<Par.NumAct;k++) EvalAct(act[k],InCol);
   return 1
  }; 

