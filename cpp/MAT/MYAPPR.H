#ifndef __MYAPPR_H
#define __MYAPPR_H

#include "matrics.h"
#include "lib/std/util.h"
#include "eigen_ma.h"
//#include "matlib.h"
//#include <alloc.h>

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

#endif
