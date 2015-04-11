// uses util;
#ifndef __RUNGCURT_H
#define __RUNGCURT_H

#include "const.h"
#include "util.h"
#include <math.h>
#include "deffunc.h"

//==================================================================
//==============   Rung - Curt method of the 4-th order  ===========
//==============   with control of the Error on the step ===========
//============= for the function:       d(y)=d(x)*f(x,y) ===========
//==================================================================
int NotRungCurt(XYZ_func func,double x_beg,double y_beg,double x_end,
                            double &x,double &y,
                            double &stp,double err=0,int MaxIter=1000,
                                                 BrkXY_func Brk=NULL);

#endif
