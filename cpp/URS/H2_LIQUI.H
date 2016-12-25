//  Григорьев, Кормер ЖЭТФ Т 65, Вып. 2 (8)  1975, стр 743

#ifndef __H2_LIQUI_H
#define __H2_LIQUI_H 

#include "mat/mtquad.h"
#include "mat/zero.h"
#include "mat/mtfunc.h"

//   x = r/r0
//
//  units for pressure : (gram*km^2)/(cm^3*c^2)  GPa
//  units for energy   : (km/c)^2
//  units for dencity  :  gram/cm^3
//  units for velocity :  km/c   
//

//double DebyFunc(double x);


double Pcold(double x);

//double PNull(double x);
double Zparam(double x,double T);

//double EcoldInt(double V);//{return Pcold(1/(V*ColdParam.Denc));}?????????
double Ecold(double x);
double Esolid(double x,double T);
//double Ecold_Ehug(double x);

double Psolid(double x,double T);
double Esolid(double x,double T);
double Pliquid(double x,double T);
double Eliquid(double x,double T);
double FindTemp(double x,double DeltaE);

double GetH2Dencity();
//void SetH2Temperature(double T);

double TemperatureS(double r,double e);
double PressureS(double r,double e);
double PressureL(double r,double e);
double TemperatureL(double r,double e);

double Grun(double x);
double Ccold2(double x);
double Teta(double x);

void SetConstants();

struct ColdParamStruct
  {int Nparam;
   double coef[7],Denc,DencH,GrunN,Teta0,Rconst; 
   double B0c,Mconst,Nconst,Edist;};

struct LiquidParamStruct
//  { double Aconst,Cconst,Rconst,Lconst,Teta0const,T0const;};
  { double Aconst,Bconst,Cconst,Rconst,Lconst,Denc0const,T0const,T_Turn;
    double B0c,Mconst,Nconst,Edist;
    double IncludeTurnE,IncludeNullE,IncludeTurnP,IncludeNullP,IncludeZeroE;
    double SingleCold;};

double FLiquid(double Denc,double T);
double FSolid(double Denc,double T);
void H2ReadPar(istream &in);
void H2SavePar(ostream &out);


extern ColdParamStruct ColdParam;
extern LiquidParamStruct LiquidParam;

#endif
