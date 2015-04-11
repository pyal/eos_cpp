//  Григорьев, Кормер ЖЭТФ Т 65, Вып. 2 (8)  1975, стр 743

#ifndef __URL_EQU_H
#define __URL_EQU_H 

#include "mat\mtquad.h"
#include "mat\zero.h"
#include "mat\mtfunc.h"

//   x = r/r0
//
//  units for pressure : (gram*km^2)/(cm^3*c^2)  GPa
//  units for energy   : (km/c)^2
//  units for dencity  :  gram/cm^3
//  units for velocity :  km/c   
//


struct UrlParamStruct{
	UrlParamStruct():Nparam(1),
		Denc(0),DencH(0),GrunN(0),Teta0(0),Rstat(0),MolWeight(0),
		B0c(0),Mconst(0),Nconst(0),Edist(0),
		Aconst(0),Bconst(0),Cconst(0),Rconst(0),Lconst(0),Denc0const(0),T0const(0),T_Turn(0),BLarge(0),
		IncludeTurnE(0),IncludeNullE(0),IncludeTurnP(0),IncludeNullP(0),IncludeZeroE(0),
		IncludeElektron(0),ElektronAr(0),H2Liquid(0),
		SingleCold(0),Wconst(0),Gconst(0),PowCoef(0),DencMet(0),Degeneration(0){coef[0]=coef[1]=1;};
	int Nparam;
	double coef[9],Denc,DencH,GrunN,Teta0,Rstat,MolWeight; 
	double B0c,Mconst,Nconst,Edist;

	double Aconst,Bconst,Cconst,Rconst,Lconst,Denc0const,T0const,T_Turn,BLarge;

	double IncludeTurnE,IncludeNullE,IncludeTurnP,IncludeNullP,IncludeZeroE;
	double IncludeElektron,ElektronAr,H2Liquid;
	double SingleCold;

	double Wconst,Gconst,PowCoef,DencMet,Degeneration;
  };

double FUrlLiquid(double Denc,double T,UrlParamStruct *Url);
double FUrlSolid(double Denc,double T,UrlParamStruct *Url);
void UrlReadPar(istream &in,UrlParamStruct *Url);
void UrlSavePar(ostream &out,UrlParamStruct *Url);




#endif



/*
double DebyFunc(double x);


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

*/
