#ifndef __MTZERO_H
#define __MTZERO_H

#include "lib\std\deffunc.h"
#include "lib\std\const.h"


int FindZeroAB(X_func f,double A,double B,double &X_res,double &F_res,
               double Tol,int MaxCount=1000,int TypeEnd=0);
int FindZeroPos(X_func f,double &X_res,double &F_res,double Tol,
                double Deriv_scale,int MaxCount=1000,int TypeEnd=0);




#endif
