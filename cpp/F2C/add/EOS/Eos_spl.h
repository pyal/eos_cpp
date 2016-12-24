#ifndef __EOS_SPL_H
#define __EOS_SPL_H

#include <stdio.h>
#include <stdlib.h>
#include "eos_def.h"
#include <string.h>
#include <math.h>


#include "f2c.h"

struct DatSpline {
    int Num_SX, Num_SY, Num_SZ;
    real *D_SX, *D_SY, *D_SZ;
};
struct EOSSpline_struct {
    DatSpline PSpl, TSpl;
    double DencCold, DencMax;
    double AddE, AddP;
    int TempSpl;
    char spl_name[256];
};
void DeleteSpl(void **IntPar);
int ReadSpl(FILE *hand, void **IntPar);
double PressureSpl(double Dencity, double Energy, void *IntPar);
double SoundSpl(double Dencity, double Energy, void *IntPar);
double TemperatureSpl(double Dencity, double Energy, void *IntPar);

#endif