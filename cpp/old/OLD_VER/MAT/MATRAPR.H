#ifndef __MATRAPR_H
#define __MATRAPR_H

#include "deffunc.h"
//=====================================================================
//===============           MatrAppr        ===========================
//=====================================================================


extern double *parptr,*dirptr,*tmpptr;
extern XAr_func hifunc;
extern double VecHiFunc(double x);

struct MatrAppr
  {
   int MatDim;
   int NumIter;
   double StopErr;
   Ar_Mat_func Ar_Func;
   double** DeriveAr;
   BrkXY_func brkfunc;

   MatrAppr() { ResetAppr(MathZer*1000,0,0,NULL,NULL);};
   MatrAppr(double Err,int Niter,int Dim,Ar_Mat_func ArFunc,BrkXY_func brkf)
     { ResetAppr(Err,Niter,Dim,ArFunc,brkf);};
  ~MatrAppr(){};
   void ResetAppr(double Err,int Niter,int Dim,Ar_Mat_func ArFunc,
                  BrkXY_func brkf)
     { StopErr=Err;NumIter=Niter;MatDim=Dim;
       Ar_Func=ArFunc;brkfunc=brkf; 
     };
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
