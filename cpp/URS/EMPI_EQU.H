//  Григорьев, Кормер ЖЭТФ Т 65, Вып. 2 (8)  1975, стр 743

#ifndef __EMPI_EQU_H
#define __EMPI_EQU_H 

#include "mat\mtquad.h"
#include "mat\zero.h"
//#include "matlib.h"
#include "mat\mtmin.h"
#include "mat\dat_mani.h"
#include "mat\mtfunc.h"
#include "lib\ref\stroka.h"
//   x = r/r0
//
//  units for pressure : (gram*km^2)/(cm^3*c^2)  GPa
//  units for energy   : (km/c)^2
//  units for dencity  :  gram/cm^3
//  units for velocity :  km/c   
//

//double EmpiDebyFunc(double x);


double EmpiPcold(double x);
double EmpiEcold(double x);
double EmpiGrun(double x);
double EmpiTeta(double x);
double EmpiZparam(double x,double T);


struct EmpiParamStruct
  {int Nparam;
   double coef[7],Denc,DencH,GrunN,Teta0,Rconst; 
   double B0c,Mconst,Nconst,Edist;
   int NTeta;
   double TetaPar[7],TetaV,TetaZ0; // TetaZ0=sqr(Teta0)/sqr(SoundCold(x=1))
   double VibTeta,VibZCoef,DisEnergy,PressureMet,DencityMet;
   double Aconst,Bconst,Cconst,RLconst,Lconst,Denc0const,T0const,T_Turn;
   double IncludeTurnE,IncludeNullE,IncludeTurnP,IncludeNullP,IncludeZeroE;
   double SingleCold,IncludeVibration,BushmanMelt,IncludeDis,IncludeDisPres;
   TData<double> *Dis_Sp,*Dis_Pnt;
   Stroka DataName;
   EmpiParamStruct():DataName("##tmp.test.data"),
		SingleCold(0),IncludeVibration(0),BushmanMelt(0),IncludeDis(0),IncludeDisPres(0),
		IncludeTurnE(0),IncludeNullE(0),IncludeTurnP(0),IncludeNullP(0),IncludeZeroE(0),
		Aconst(0),Bconst(0),Cconst(0),RLconst(0),Lconst(0),Denc0const(0),T0const(0),T_Turn(0),
		VibTeta(0),VibZCoef(0),DisEnergy(0),PressureMet(0),DencityMet(0),
		TetaV(0),TetaZ0(0),NTeta(1),
		B0c(0),Mconst(0),Nconst(0),Edist(0),
		Denc(0),DencH(0),GrunN(0),Teta0(0),Rconst(0),Nparam(1){
			TetaPar[0]=0;TetaPar[1]=1;coef[0]=0;coef[1]=1;
			Dis_Sp=NULL,Dis_Pnt=new TColumn<double>();
		}
   ~EmpiParamStruct(){delete Dis_Sp;delete Dis_Pnt;Dis_Sp=NULL,Dis_Pnt=NULL;}
  };

   double EmpiFLiquid(double Denc,double T,EmpiParamStruct *Par);
   double EmpiFSolid(double Denc,double T,EmpiParamStruct *Par);

   double EmpiFDis(double Denc,double T,EmpiParamStruct *Par);
   double EmpiFDisDegree(double Denc,double T,EmpiParamStruct *Par);

   double EmpiFDisPres(double Denc,double T,EmpiParamStruct *Par);
   double EmpiFDisPresDegree(double Denc,double T,EmpiParamStruct *Par);

   void EmpiH2ReadPar(istream &in,EmpiParamStruct *Par);
   void EmpiH2SavePar(ostream &out,EmpiParamStruct *Par);
   double EmpiGetH2Dencity();

// This is for deby construction only
double EmpiFreeEMet(double Denc,double T);
double EmpiFLiq00(double Denc);
double EmpiFLiq0(double Denc);
//extern EmpiParamStruct EmpiColdParam;

#endif
