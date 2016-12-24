#ifndef __EOS_STD_H
#define __EOS_STD_H

#include <stdio.h>
#include <stdlib.h>
#include "eos_def.h"
#include <string.h>
#include <math.h>


double sqr(double a);
double max(double a, double b);
double min(double a, double b);


int ReadStd(FILE *hand, void **IntPar, int NumPar, int Skip = 0);
double SoundStd(double Dencity, double Energy, void *IntPar, PointFunc Pressure);


double TemperatureABu(double Dencity, double Energy, void *IntPar);
double PressureABu(double Dencity, double Energy, void *IntPar);
double SoundABu(double Dencity, double Energy, void *IntPar);
int ReadABu(FILE *hand, void **IntPar);
void DeleteABu(void **IntPar);

double TemperatureGas(double Dencity, double Energy, void *IntPar);
double PressureGas(double Dencity, double Energy, void *IntPar);
double SoundGas(double Dencity, double Energy, void *IntPar);
void DeleteGas(void **IntPar);
int ReadGas(FILE *hand, void **IntPar);


#endif