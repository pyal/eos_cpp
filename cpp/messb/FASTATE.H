
#ifndef __FAState_H
#define __FAState_H

#include "faobj.h"
//#include "urs/matt_reg.h"
#include "urs/empi_equ.h"
#include "urs/fre_fac.h"
//#include "matt_cfg.h"
#include "urs/matt_adi.h"
#include "urs/matt_h2.h"
#include "urs/matt_his.h"
//#include "h2_liqui.h"
//#include "faregist.h"

int FAStateRegister(fstream &in,FAIO *&Func,char *name);
//extern int FARegister(fstream &in,FAIO *&Func);



//  =======================================================
//  ================                    ===================
//  =======================================================
/*
struct FAZerABu_Grun:FAZer
  {
   MatterABuAdd Mat;
   FAZerABu_Grun();
   void SetABuConst(double *par);
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
  };
*/
//  =======================================================
//  ================ H2 Cold case of Zero T  log scale ====
//  =======================================================
struct FAZerABu_H2cold:FAZer
  {
   MatterH2Sol H2Mat;
   FAZerABu_H2cold();
   void SetABuConst(double *par);
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
  };

struct FAZerABu_EmpiCold:FAZer
  {
   MatterIO *Matt;
   FAZerABu_EmpiCold();
   void SetABuConst(double *par);
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
   ~FAZerABu_EmpiCold(){delete Matt;}
  };

struct FAZerABu_EmpiMelt:FAZer
  {
   FAZerABu_EmpiMelt();
   int NumVal;
   double *PTarr[4];
   void SetABuConst(double *par);
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
   ~FAZerABu_EmpiMelt();
  };

struct FAZerABu_EmpiABu:FAZer
  {
   MatterABuExpFit ExpMat;
   void SetABuConst(double *par);
   FAZerABu_EmpiABu();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
   ~FAZerABu_EmpiABu();
  };


#endif 



