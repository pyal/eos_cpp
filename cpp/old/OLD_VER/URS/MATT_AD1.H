
#ifndef __MATT_ADI_H
#define __MATT_ADI_H


#include "util.h"
#include "zero.h"
#include "matlib.h"
#include "matt_fac.h"
#include "matt_reg.h"
#define MaxDencity 100
//#define FMT "      "<<setw(13)<<setprecision(6)
//#define FM  setw(13)<<setprecision(9)

// Calculates from D hugoniot. Do not forget about
// the lowest possible D.
void DHug(double u1,double r1,double p1,double e1,
          double &u2,double &r2,double &p2,double &e2,
          double D,double porous=1,double Rcritical=MaxDencity);

// Calculates from velocites hugoniot. D - calculates
//void HugO(double u1,double &r1,double &p1,double &e1,
//          double u2,double r2,double p2,double e2,double &D,double porous=1);
void Hug(double u1,double r1,double p1,double e1,
          double u2,double &r2,double &p2,double &e2,double &D,
          double porous=1,double Rcritical=MaxDencity);

//double MisfitHug(double r);
//double MisfitDHug(double r1);

double Sound(double r,double e);

int InputPar(fstream &in,double &from,double &min,double &max,
                                                 double &stp,char *write);

void ShowHugMis(fstream &in,double u1,double r1,double p1,double e1,
                           double u2,double r2,double p2,double e2,double D,
                           double porous,double Rcritical);
void ShowDHugMis(fstream &in,double u1,double r1,double p1,double e1,
                            double u2,double r2,double p2,double e2,double D,
                            double porous,double Rcritical);
void ShowHug(fstream &in,double u1,double r1,double p1,double e1,
                        double u2,double r2,double p2,double e2,double D,
                        double porous,double Rcritical);
void ShowDHug(fstream &in,double u1,double r1,double p1,double e1,
                        double u2,double r2,double p2,double e2,double D,
                        double porous,double Rcritical);
int InputPar(fstream &in,double &from,double &min,double &max,
                                                 double &stp,char *write);


#ifdef MatterH2Liq_Register
void ShowColdH2(fstream &in);
#endif

void ShowIsoTherm(fstream &in);
void ShowIsoEntrope(fstream &in);
void ShowTemp(fstream &in);

#ifdef MatterSpline_Register
void FormSplineDat(fstream &in);
#endif

extern MatterIO *MatterAdiabat;
//extern XYZ_func PressurePtr;
extern double* ConstPar;

#endif
