#ifndef __MTMIN_H
#define __MTMIN_H

#include "util.h"


double NAGMin(X_func F,double relat_err,double abs_err,double MinB,
              double MaxB,int MaxCal,double F_res,int Fail);



#endif
