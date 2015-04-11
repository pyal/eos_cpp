#ifndef __UIL_CLC_H
#define __UIL_CLC_H

#include "matt_reg.h"
#include "uil_bod.h"
#include <malloc.h>

void ClcMassInBody(Body &WorkBody);

void CellFixed(Body &WorkBody);//,double &PresDeriv);


void ClcAddU(Body &WorkBody,double *AddU);
//void CalcPos(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability);
void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability);
void CalcPosE(Body &WorkBody,double TimeStp);


#endif