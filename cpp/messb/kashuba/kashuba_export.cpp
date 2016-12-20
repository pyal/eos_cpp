#include <lib/precompiled/Messb.h>

#include "kashuba_export.h"

#include "kashuba_appr.h"

//  =======================================================
//  ============= Polycristall case rotating lines ========
//  =======================================================
// Kashubas calculation of line width
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerHQs_Rotate(){return new FAZerHQs_Rotate();}
static ClassDesc FAZerHQs_Rotate_ClassDesc(typeid(FAZerHQs_Rotate),"HQ_poly_kashuba_est",1,"FAZer",Dummy_FAZerHQs_Rotate);
#else
static ClassDesc FAZerHQs_Rotate_ClassDesc(typeid(FAZerHQs_Rotate),"HQ_poly_kashuba_est",1,"FAZer",create<FAZerHQs_Rotate>);
#endif

FAZerHQs_Rotate::FAZerHQs_Rotate()
  {
   name=strdup("HQ_poly_kashuba_est");
   NumPar=13;
   helppar[0 ]=strdup("Calculation of spectra of polycrystall Fe with rotating H (Kashuba's corrections for width of lines). Main axis - Fix, H//Fix_z, QS - is turned (Phi,Teta params). Calculated relaxation correction by formula: ZeroPos = Sum(LinePosition[k])/8 ; Sw=x-Position[k]; Mul=sign(ZerPos-Position[k]) ;  Wc=Wid*0.5+W_coef*fabs(ZerPos-LinePosition[k])*atan(M_PI*T/(2*(8*Tbkt-T))) ; Res+=Intencity[k]*(1-Sw*A*Mul/Wid)/(sqr(Wc)+sqr(Sw));  Direct calculation of spectra in the relaxation model sugested by Kashuba (2D magnetic relaxation model) does not give such results... Unfortunately... So - not to be used for real spectra estimations.Dummy... Test purpose only.");
   namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z  Teta - rotation around  Y ( after Phi rotation )   ");
   namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle -rotation around Fix_z is done first - final QS_axis - result of two operations under its axises - turn on phi around Fix_z plus turn by teta around Y. Eiler modified angles ( second turn around Y not Y'). ");
   namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry) Vxx-Vyy  ");

   namepar[10]=strdup("   T      ");valpar[10]=0;	helppar[10]=strdup("Kashuba's koef  additional width formula parameters (works under atan()) - small influence");
   namepar[11]=strdup("   Tbkt   ");valpar[11]=0;	helppar[11]=strdup("Kashuba's koef  additional width formula parameters (works under atan()) - small influence ");
   namepar[12]=strdup("    A     ");valpar[12]=0;	helppar[12]=strdup("Kashuba's koef  additional intencity multiplier ");
   namepar[13]=strdup("  W_coef  ");valpar[13]=0;	helppar[13]=strdup("Kashuba's koef  additional width multiplier ");
  };
void   FAZerHQs_Rotate::ClcBase(double *par,double *Xval,double *Res)
{
   double Teta=par[7],Phi=par[8],H=par[3],Qs=par[4],Eta=par[9],IS=par[5],W=par[6];
   double Gr=par[1],Amp=par[2],T=par[10],Tb=par[11],A=par[12],Wc=par[13];
   Solver_H_Z_poly_kashuba(new Ham_Qs_Eiler(Teta,Phi,H,Qs,Eta,IS,W),T,Tb,A,Wc).Solve(Xval,Res);
}



//  ================================================
//  ================ Relaxation Kashuba Function ===
//  ================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerQsHRelaxKashuba(){return new FAZerQsHRelaxKashuba();}
static ClassDesc FAZerQsHRelaxKashuba_ClassDesc(typeid(FAZerQsHRelaxKashuba),"qh_relax_kashuba",1,"FAZer",Dummy_FAZerQsHRelaxKashuba);
#else
static ClassDesc FAZerQsHRelaxKashuba_ClassDesc(typeid(FAZerQsHRelaxKashuba),"qh_relax_kashuba",1,"FAZer",create<FAZerQsHRelaxKashuba>);
#endif
FAZerQsHRelaxKashuba::FAZerQsHRelaxKashuba()
  {
   name=strdup("qh_relax_kashuba");
   NumPar=23;
													helppar[0 ]=strdup("Calculation of spectra of Fe under H relaxation with H(t) given by formulas of Kashuba. Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta params), H(t)=H(H0,Teta0,Phi(t)+Teta0)  ");
   namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");

   namepar[10]=strdup("  OutPhi ");valpar[10]=1;	helppar[10]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[11]=strdup("  Time   ");valpar[11]=30;	helppar[11]=strdup("Time to make calcultaion of relaxation process. Line intencity is calculated by formula Int=exp(-CurTime*Width)  so final line intencity Int_fin=exp(-Time*Width) - all other process is not treated ");
   namepar[12]=strdup("NumToAver");valpar[12]=1;	helppar[12]=strdup("Number of times to recalculte relaxating H angles  ( to calculate mean line intencity value by averaging of all of them )");
   namepar[13]=strdup("NTimeAver");valpar[13]=1;	helppar[13]=strdup("Number of times to calculate intencity spectra for one given H angle serie ( modify start integration time Ts for serie from 0 to EndTAver (given by next parameter) I=Sum(Int(Ts[i],Tfinal)) )");
   namepar[14]=strdup("EndTAver ");valpar[14]=1;	helppar[14]=strdup("Final Average intgration time (look NTimeAver parameter for explanation) ");

   namepar[15]=strdup("NumTimes ");valpar[15]=100;	helppar[15]=strdup("Num different H angles to generate in one serie ( Full Clc time will be devided in this number parts, taken that for each part - field is static )");
   namepar[16]=strdup(" RotTime ");valpar[16]=50;	helppar[16]=strdup(" Амплитуда гармоник - чем больше тем больше флуктуации поля для заданного шага по времени  ");
   namepar[17]=strdup("FurieDisc");valpar[17]=10;	helppar[17]=strdup("Число случайных гармоник расчитываемых в спектре.");
   namepar[18]=strdup("StrongSho");valpar[18]=10;	helppar[18]=strdup("Коэффициент при квадратичном члене, предложенным Сашей для нормализации спектра при высоких частотах определяет на сколько сильно подавляются высокие гармоники.");
   namepar[19]=strdup("  Phi0   ");valpar[19]=0;	helppar[19]=strdup("  Start serie from H - Phi0  (not used now?) ");
   namepar[20]=strdup("  Teta0  ");valpar[20]=90;	helppar[20]=strdup("  In serie will be used given fixed H - Teta0 angle ");

   namepar[21]=strdup("EigenAcc "); valpar[21]=0;	  helppar[21]=strdup("  What method will be used for integration: 1 - will be used eigen acclerated method, 0 - will be used direct integration. For method 1 - parameters used: NumToAver - num of different series of ham will be generated; NTimeAver - number of times each generated serie will be shifted by initial time; EndTAver - maximum time shift for serie;; For method 0 : parameters are used - : NumToAver - num of different series of ham will be generated; OutMeanAmp - to output mean amplitude generated (spectr - is fourie transform of this amplitude); DiscrNum - number of discretization time of Ham calculation - have to be of the order - 10000 - the higher value, the more precies result ( to some extent )");
   namepar[22]=strdup("OutMeanAmp");valpar[22]=0;	  helppar[22]=strdup("  1 - to output the result mean amplitude, before fourie transformation ");
   namepar[23]=strdup("DiscrNum "); valpar[23]=1e4;	helppar[23]=strdup("  Number of time steps to make for Amplitude construction, have to be of the order 10000 - the higher - the better ( but in some moment calculation can become unstable ");

  };
#include "..\messb_in.h"

void   FAZerQsHRelaxKashuba::ClcBase(double *par,double *Xval,double *Res)
{
    double Ground=par[1],Intencity=par[2],H=par[3],QS=par[4],IS=par[5],W=par[6],
           Teta=par[7],Phi=par[8],Eta=par[9],OutPhi=par[10],Time=par[11],
           NumToAver=par[12],NTimeAver=par[13],EndTAver=par[14];

    double NumTimes=par[15],RotTime=par[16],FurieDisc=par[17],StrongSho=par[18],
           Phi0=par[19],Teta0=par[20],EigenMethod=par[21],OutAmp=par[22],HamDiscr=par[23];

   Ref<RndAngleGenerator_Base> ang_t=
     new RndAngleGenerator_Kashuba(Phi0,(int)NumTimes,RotTime,(int)FurieDisc,Time,StrongSho);
   Ref<Rnd2AngleGenerator_Base> ang=new Rnd2AngleGenerator_FixPhi(ang_t,Teta0);
   Ref<HamData> Ham=new Ham_Qs_Eiler(Teta,Phi,H,QS,Eta,IS,W);
   Ref<ContiniousRelaxationStepSolver_Base> solv=new ContiniousRelaxationStepSolver();
   Ref<HamTimeAverageRndWalk> hams=new HamTimeAverageRndWalk(ang,Ham,0,0,(int)OutPhi);
   if (EigenMethod)
   {
    ContiniousRelaxationAverageSolver(
      new ContiniousRelaxationSolver(
          NULL,new ContiniousRelaxationStepMultiSolver(
            new TimeShiftGenerator((int)NTimeAver,EndTAver),solv)),
            hams,(int)NumToAver).Solve(Xval,Res);
   }
   else
   ContiniousRelaxationAverageSolver_DirectMatrMul(hams,(int)NumToAver,(int)HamDiscr,(int)OutAmp)
      .Solve(Xval,Res);

}

