#ifndef __DEFFUNC_H
#define __DEFFUNC_H

#include "const.h"
#include "util.h"
#include <math.h>

typedef double (*XYZ_func)   (double x,double y);
typedef int    (*BrkXY_func) (double x,double y);

typedef double (*XAr_func)   (double *par);
typedef double (*X_func)     (double par);
typedef double (*MIN_func)   (double x,double y,X_func Func,double Tol);
typedef XAr_func* Ar_Mat_func;


#endif
