// uses util;
#ifndef __GRADAPPR_H
#define __GRADAPPR_H

#include "const.h"
#include "util.h"
#include <math.h>

//=====================================================================
//===============           GradAppr        ===========================
//=====================================================================


extern double *parptr,*dirptr,*tmpptr;
extern XAr_func hifunc;
extern double VecHiFunc(double x);

struct GradAppr
  {
   int NumIter;
   double StopErr;
   MIN_func minfunc;
   BrkXY_func brkfunc;

   GradAppr() { ResetAppr(MathZer*1000,0,NULL,NULL,NULL);};
   GradAppr(double Err,int Niter,XAr_func hi,MIN_func minf,BrkXY_func brkf)
     { ResetAppr(Err,Niter,hi,minf,brkf);};
  ~GradAppr(){};
   void ResetAppr(double Err,int Niter,XAr_func hi,MIN_func minf,BrkXY_func brkf)
     { StopErr=Err;NumIter=Niter;minfunc=minf;hifunc=hi;brkfunc=brkf; };
//==============================================================
   int Clc(double *param,double *stp,double hi=-1);
//==============================================================
   private:
   void ContStp(double *stp) {};
   void ClcGrad(double *param,double *stp,double *grad,double hi);
   double VecMulVec(double *vec1,double *vec2);
   double VecMulNum(double *vec1,double num);
   int ClcDir(double *grad,double *dir,double &NormGrad);
   void ClcMin(double *param,double *dir,double *stp,double &er,double &hi);
//   void PrepVecHiFunc(double *param,double *dir){};
//   void SetNewStp(double *stp,double MinStp,double MaxStp);
   double GetMaxStp(double *stp,double *dir);
   void   SetMaxStp(double *stp,double maxstp,double curstp);
  };

#endif



