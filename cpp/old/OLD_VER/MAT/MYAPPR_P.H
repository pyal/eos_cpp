#ifndef __MYAPPR_H
#define __MYAPPR_H

#include "matrics.h"
#include "util.h"
#include "eigen_ma.h"
#include "matlib.h"
#include <alloc.h>

typedef double (*XAr_func) (double *par);
typedef void (*GR_XAr_func) (double *par,double *Fval);
struct MAPVarStrc
  {
   XAr_func Hi_Func ;
   GR_XAr_func Gr_Func;
   double *ExperPoint;
   double *HiPntCoef;
   double *RecalcPar; // Used in InitTruePar
   double *StepDir;  // Iterations along dir
   double *CurParam; // Iterations along dir
   int TmpIter;
  };
static MAPVarStrc (*MAPVar);

int MAPClc(double *Param,double *MaxStep,int Iter,
    XAr_func HiF,GR_XAr_func GrF,double *Exper,double Misfit,double *HiPnt);

// HiPnt - Coef to multiple to obtain real hi from point

int MAPClc(double *Param,double *MaxStep,int Iter,
    XAr_func HiF,GR_XAr_func GrF,double *Exper,double Misfit);

#endif
