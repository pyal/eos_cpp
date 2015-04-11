#ifndef __MTMIN_H
#define __MTMIN_H

#include "lib\std\deffunc.h"


int NAGMin(X_func F,double relat_err,double abs_err,double MinB,
              double MaxB,int MaxCal,double &X_res,double  &F_res);



#endif
