#ifndef __UIL_CLC_H
#define __UIL_CLC_H

#include "matt_reg.h"
#include "eno_bod.h"
#include <malloc.h>

#include "matrics.h"
#include "vec_fre.h"



void ClcMassInBody(Body &WorkBody,int Order);
void CellFixed(Body &WorkBody);//,double &PresDeriv);
void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability);
void CalcPosE(Body &WorkBody,double TimeStp);


//  UIL CLC

//void ClcMassInBody(Body &WorkBody);
//void CellFixed(Body &WorkBody);//,double &PresDeriv);
//void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability);
//void CalcPosE(Body &WorkBody,double TimeStp);

//void ClcAddU(Body &WorkBody,double *AddU);


#endif