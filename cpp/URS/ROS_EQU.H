#ifndef __ROS_EQU_H
#define __ROS_EQU_H 


#include "lib/std/util.h"
//#include "mtquad.h"
//#include "Quad.h"
//#include "mtmin.h"
//#include "zero.h"
#include "mat/mtfunc.h"
#include "lib/std/stdexception.h"
#include "lib/ref/ref.h"
//#include "spl_reg.h"
struct Exp6Param:RefCount
  {
	Exp6Param():Alpha(36.4),Epsilon(11.1),Radius0(3.43),RadiusExp(1.45855),CutDiameter_Niu(0.74),IncludePow6(1){};
enum RosIntegralMethods{ROS_INTEGRAL_STRICT,ROS_INTEGRAL_SMOOTH,ROS_INTEGRAL_NOTSMOOTH,ROS_INTEGRAL_SMOOTH_PLUSNIU} ;
   double Alpha,Epsilon,Radius0,RadiusExp,MulExp,ArgExp;
   double CutDiameter_Niu;
// For MainIntegral
   double P6,Ec0,Ec1,E0,E1,X;
   double Niu;
// For CutParam
   double Amp[3],Freq[3];
// Exclude Pow6
   double IncludePow6;
// For MinCalc
   double L_Denc,L_Int,L_T,L_Niu;
   static double Pow6Clc(double r, const Exp6Param &par);
   static double Pow6ClcDeriv(double r, const Exp6Param &par);
// This is simplified deriv only
   static double Pow6ClcDeriv2(double r);
// Calcualtes RadiusExp - and for it low exponent params : ArgExp MulExp
   static int FindRexp(Exp6Param &Rexp);
   static void Clc_ArgExpMulExp(Exp6Param &Rexp);
  private:
   static Exp6Param *Current;
  }; 
extern double SmoothLimitUp;
extern double SmoothLimitLo;
extern int IntExp6Correlation_IntMethod;


double IntExp6Correlation(Exp6Param &Par,double niu,double Diameter);
double HS_Correction(double niu);
double Ross6Config(Exp6Param &Par6,double MolVeight,double Denc,double T);
double DissEnergy(double E1,double E2,double MolVeight,double T);
double IonizeEnergy(double E1,double E2,double MolVeight,double T,
					double Mul_Degeneration,double V);
int Exp6Read(istream &in,Exp6Param *&par);
int Exp6Read(istream &in,Exp6Param &par);
void Exp6Save(ostream &out,Exp6Param &par);

double FreeRCheck(Exp6Param *par,double Denc,double T,double niu,double MolVeight,double &Niu_Res,double &F_Res);


struct MinConfigParam
  {  double S_DensDia,S_Temp,S_Rconst; };
//void SetConfigParam(MinConfigParam &Par,double Denc,double T);
double RossElectronN2_E(double T);
double RossElectronN_E(double T,int MetalE);
// return DensOfFreeE to add
double FreeESemiConductor(double ZoneWidth,double LowDegener,double UpDegener, // in eV
						  double MolVeight,double Dens,double T);


extern double DisDegree;
//#define IntegralInfinity 200
extern double IntegralInfinity;// 200
double DoubleIntegral(double q);
//double PYfunc(double q);
//double MainIntegral(double q);
  
//extern Exp6Param *CalculateExp6Struct;
 
#endif




/*

struct RossParam
  {
   double Rconst;
   double MolVeight,Zero,CvId,HiT,ParInteract[8],Tvib;
   double NiuL,TL,DL;
   double P6,Ec0,Ec1,E0,E1,X;
   SplineXYIO *PercYerv;
   double S_Niu,S_Diameter,S_DensDia,S_Temp,S_MaxR_PY;
   RossParam(){Rconst=M_Rconst;PercYerv=NULL;};
   ~RossParam(){delete PercYerv;PercYerv=NULL;NiuL=0;DL=0;TL=0;}
  };

double FreeSSVT(RossParam *par,double Denc,double T);
int FreeERossRead(fstream &in,RossParam *&par);
double FreeRCheck(RossParam *par,double Denc,double T,double niu);
*/
