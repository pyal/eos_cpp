#ifndef __MULTIMIN_H
#define __MULTIMIN_H

#include "myappr.h"
/*
double UncMin(int &Error,int N,double *x_start,double *x_res,double est_sol,
              double (*FuncClc)(double *x,int n) , double *GradStp=NULL,
              void (*GradClc)(double *x,double *grad,int n,double &func)=NULL,
              double FunAccur=1e-9,double ResErr=1e-4,int MaxFun=150,int MaxIter=5,
              double *BoundMin=NULL,double *BoundMax=NULL,
              double MaxLineSearchStp=10,
              double LineSearchEta=0.25);
*/
double MultiMin(int &Error,int N,double *x,double (*FuncClc)(double *x,int n) ,
                void (*SecDerivClc)(VecCl &x,VecCl &deriv,MatrCl &deriv2),
                double XMisf,int MaxIterStep,int MaxIter,double &ResErr,
                double *GradStp,double *BoundMin,double *BoundMax,
                double *MaxLineSearchStp,
                int LineSearchMeth);

extern int NumGradCalls;
#endif


/*
#define MaxFunctionParam 100
typedef double (*XAr_func) (double *par);
typedef void (*Correlation_func) (MatrCl &DatM,VecCl &MisV,double &CurHi,
                                  VecCl &Par,VecCl &MaxStep);
typedef void (*GR_XAr_func) (double *par,double *Fval);
                  

struct MAPVarStrc
  {
   GR_XAr_func Gr_Func;
   VecCl ExperPoint;
   double *HiPntCoef;

   XAr_func Hi_Func ;
   Correlation_func SetCorrelation;

   VecCl LongPar;
   int LongNum[MaxFunctionParam];
   int Iter;
  };
static MAPVarStrc (*MAPVar);


void CorrelationHi(MatrCl &DatM,VecCl &MisV,double &CurHi,VecCl &Par,
                                                          VecCl &MaxStep);
void CorrelationMin(MatrCl &DatM,VecCl &MisV,double &CurHi,VecCl &Par,
                                                          VecCl &MaxStep);
int MAPClc(double *Param,double *MaxStep,int Iter,
    XAr_func HiF,double ErrMisfit,int MinHi,
    GR_XAr_func GrF=NULL,double *Exper=NULL,double *HiPnt=NULL);

// HiPnt - Coef to multiple to obtain real hi from point
*/
