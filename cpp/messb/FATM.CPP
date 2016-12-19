#include "fatm.h"

FAZerTM::FAZerTM()
  {
   name=strdup("Time");
   NumPar=1;
   namepar[1]=strdup("Ground   ");
  };
double FAZerTM::Clc(double *par,double Xval)
  {  return par[1]+RealTime(Xval); };
void   FAZerTM::Clc(double *par,double *Xval,double *Res)
  {
   for (int k=1;k<=Xval[0];k++) Res[k]=par[1]+RealTime(Xval[k]);
  };
     
void FAZerTM::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };

int FATMRegister(fstream &in,FAIO *&Func,char *name)
  {
   FAZer *SFunc=NULL;
   int ret=1;
   if (!Stricmp(name,"Time")) {SFunc=(FAZer*)new FAZerTM;}
                   else ret=0;
   if (!ret) return ret;
   if (Func!=NULL) delete(Func);
      FASingleIO *F=new FASingleIO;
      F->Func=SFunc;
      Func=(FAIO*)F;
   Func->Input(in);
//cout<<" Mem after FAMessbRegister  "<<coreleft()<<"\n";
   return ret;
  };
