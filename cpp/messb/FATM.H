#ifndef __FATM_H
#define __FATM_H

#include "faobj.h"
//#include "faregist.h"
//#include "util/tmstd.h"


double RealTime(double Wait);

int FATMRegister(fstream &in,FAIO *&Func,char *name);
extern int FARegister(fstream &in,FAIO *&Func);
struct FAZerTM:FAZer
  {
   FAZerTM();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
//   ~FAZerExp();
  };

#endif 

