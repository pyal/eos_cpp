#ifndef __UIL_CLC_H
#define __UIL_CLC_H

#include "matt_reg.h"
#include "eno_bod.h"
#include <malloc.h>

#include "matrics.h"
#include "vec_fre.h"


//  UIL CLC

void ClcMassInBody(Body &WorkBody);

void CellFixed(Body &WorkBody);   //,double &PresDeriv);


void ClcAddU(Body &WorkBody, double *AddU);
void CalcTStpVisc(
    Body &WorkBody,
    double TimeStp,
    double &MinTimeStp,
    double TimeStability);
void CalcPosE(Body &WorkBody, double TimeStp);


#endif