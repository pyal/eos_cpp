#ifndef __MTFUNC_H
#define __MTFUNC_H

#include "lib\std\const.h"
#include "quad.h"

#include <math.h>
#ifdef WCPP_NET
#include <fstream>
#else
#include <fstream.h>
#endif
double MTSign(double x);
double MTSinIntegral(double x);
double DebyFuncInt(double x);// {if (x==0) return 0;return x*x*x/(exp(x)-1);};
double DebyFunc(double x) ;
  //{
  // double ResMis;int Err;
  // return 3/(x*x*x)*Qags_IntSubInt(DebyFuncInt,0,x,StndErr,StndErr,1000,ResMis,Err);};

#endif
