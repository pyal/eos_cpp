
#ifndef __MATT_ADI_H
#define __MATT_ADI_H


#include "lib\std\util.h"
#include "mat\zero.h"
#include "lib\std\ex_out.h"
#include "mat\mtrung.h"
#include "matt_fac.h"
//#include "matt_reg.h"
#include "mat\Quad.h"
#define MaxDencity 100
//#define NEWTON_HUGONIO_SEARCH
#define FZERO_HUGONIO_SEARCH

void SetIntPar(double r,double e);

double Sound(double r,double e);

int InputPar(fstream &in,double &from,double &min,double &max,
                                                 double &stp,char *write);
void HugDenc(double u1,double r1,double p1,double e1,
             double &u2,double r2,double &p2,double &e2,
             double &D);
int HugPres(double u1,double r1,double p1,double e1,
             double &u2,double &r2,double p2,double &e2,
             double &D);


void ShowHugPres(fstream &in,double u1,double r1,double p1,double e1,
             double u2,double r2,double p2,double e2,double D);
void ShowHugDenc(fstream &in,double u1,double r1,double p1,double e1,
             double u2,double r2,double p2,double e2,double D);
//int InputPar(fstream &in,double &from,double &min,double &max,
//                                                 double &stp,char *write);


#ifdef MatterH2Liq_Register
void ShowColdH2(fstream &in);
void ShowColdEmpi(fstream &in);
#endif

void ShowIsoTherm(fstream &in);
void ShowIsoEntrope(fstream &in);
void ShowTemperature(fstream &in);
void ShowEnergy(fstream &in);

//#ifdef MatterSpline_Register
void FormSplineDat(fstream &in);
void FormTwoPhaseBnd(fstream &in);
//#endif
#ifdef RossDiameter
void ShowRossDiameter(fstream &in);
#endif
extern MatterIO *MatterAdiabat;
double PressurePorous(double r,double e);
//extern XYZ_func PressurePtr;
extern double* ConstPar;


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

void IsentropePres(double u1,double r1,double p1,double e1,
             double &u2,double &r2,double p2,double &e2,
             double &D);

void ShowHugMis(fstream &in,double u1,double r1,double p1,double e1,
                           double u2,double r2,double p2,double e2,double D,
                           double porous,double Rcritical);
void ShowDHugMis(fstream &in,double u1,double r1,double p1,double e1,
                            double u2,double r2,double p2,double e2,double D,
                            double porous,double Rcritical);
void ShowDHug(fstream &in,double u1,double r1,double p1,double e1,
                        double u2,double r2,double p2,double e2,double D,
                        double porous,double Rcritical);

void FormSolidLiquidDat(fstream &in);
void FindBushCoefABC_Mat(fstream &in);
void FormSolLiqBnd(fstream &in);


#endif
