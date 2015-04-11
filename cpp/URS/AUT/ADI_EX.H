
#ifndef __ADI_EX_H
#define __ADI_EX_H  


#include "util.h"
#include "zero.h"
#include "matlib.h"
#include "ex_out.h"
#include "../matt_fac.h"
#include "../matt_reg.h"
#define MaxDencity 100

extern MatterIO *MatterAdiabat;
double PressurePorous(double r,double e);
extern double* ConstPar;
void HugDenc(double u1,double r1,double p1,double e1,
             double &u2,double r2,double &p2,double &e2,
             double &D);
void HugPres(double u1,double r1,double p1,double e1,
             double &u2,double &r2,double p2,double &e2,
             double &D);

#endif

