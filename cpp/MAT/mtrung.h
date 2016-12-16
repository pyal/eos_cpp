// uses util;
#ifndef __MTRUNG_H
#define __MTRUNG_H

#include "lib/std/const.h"
#include "lib/std/deffunc.h"
//#include "util.h"
//#include <math.h>

//typedef double (*XYZ_func)   (double x,double y);
//typedef int    (*BrkXY_func) (double x,double y);

//typedef double (*XAr_func)   (double *par);
//typedef double (*X_func)     (double par);
//typedef double (*MIN_func)   (double x,double y,X_func Func,double Tol);

//==================================================================
//==============   Rung - Curt method of the 4-th order  ===========
//==============   with control of the Error on the step ===========
//============= for the function:       d(y)=d(x)*f(x,y) ===========
//==================================================================
int NotRungCurt(XYZ_func func,double x_beg,double y_beg,double x_end,
                            double &x,double &y,
                            double &stp,double err=0,int MaxIter=1000,
                                                 BrkXY_func Brk=NULL);

//=====================================================================
//===============         MinFunc           ===========================
//=====================================================================
/*
struct ClcMinStrc
  {
   double lft,rgt,*clcx,Flft,Frgt,*Fclcx;
   double GoldCoef,MachZer,ClcZer;
   double ZerStp;
   double MinCurr;
   X_func func;
   ClcMinStrc();
   ~ClcMinStrc(){delete clcx;delete Fclcx;};
   double MinGoldSqr(double A,double B,X_func yfunc,double Tol,int MaxIter=30000);
   double MinNumGoldSqr(double A,double B,X_func yfunc,double Tol);
   double MinSqr(double A,double B,X_func yfunc,double Tol);
   double MinGold(double A,double B,X_func yfunc,double Tol);
   private:
   void SetBeg(double A,double B,X_func yfunc,double Tol);
   void GoldStp();
   void SqrStp();
   double CmpDelMin(double mainx,double newx,double Zer);
   double MinSqrAppr(double x1,double x2,double x3,
		     double y1,double y2,double y3);
   void NextBound();
   void SetBnd(double x1,double x2,double x3,double y1,double y2,double y3);
  };
static ClcMinStrc *MinStrc=NULL;

double MinFunc(double x,double y,X_func Func,double Tol,int MaxIter=30000);
double MinGold(double x,double y,X_func Func,double Tol);
double MinSqr(double x,double y,X_func Func,double Tol);
void DelMinFunc();
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
*/

#endif


