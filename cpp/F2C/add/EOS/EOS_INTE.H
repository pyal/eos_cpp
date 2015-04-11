#ifndef __EOS_INTE_H
#define __EOS_INTE_H

#include "eos_std.h"
#include "eos_spl.h"
#include "eos_def.h"

#define MaxNumEos 100
struct EOSstruct
  {
   void *IntPar;
   char name[50];
   PointFunc Pressure;
   PointFunc Sound;
   PointFunc Temperature;

   ReadFunc ReadIni;
   VoidFunc DeleteIni;
  };

int EOSRead(int N,FILE *hand);
void EOSDelete();
double EOSPressure(double D,double E,int N);
double EOSTemperature(double D,double E,int N);
double EOSSound(double D,double E,int N);


#endif