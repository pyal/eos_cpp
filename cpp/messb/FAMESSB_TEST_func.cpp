#include <lib/precompiled/Messb.h>

#include "famessb_test_Func.h"
#include "messb_av.h"

//  =======================================================
//  === Polycristall case  Teta_Phi(const)_Gamma angles ===
//  =============  No phi dependence so it is fixed =======
//  =======================================================

#ifndef WCPP_NET
DescribedClass * Dummy_FAZerHQs_Gamma(){return new FAZerHQs_Gamma();}
static ClassDesc FAZerHQs_Gamma_ClassDesc(typeid(FAZerHQs_Gamma),"HQ_Gamma_poly",1,"FAZer_Test",Dummy_FAZerHQs_Gamma);
#else
static ClassDesc FAZerHQs_Gamma_ClassDesc(typeid(FAZerHQs_Gamma),"HQ_Gamma_poly",1,"FAZer_Test",create<FAZerHQs_Gamma>);
#endif

FAZerHQs_Gamma::FAZerHQs_Gamma()
  {
   name=strdup("HQ_Gamma_poly");
   NumPar=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);
	 helppar[0 ]=strdup("Calculation of spectra of poycristal Fe. Main axis - Fix, H//Fix_z, QS - is turned (Gamma,Teta params) ");
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z  Teta rotation around Fix_y ");
   //namepar[8 ]=strdup("  Gamma  ");valpar[8 ]=0;	helppar[8 ]=strdup("Gamma Angle - rotation around new Z' -so final QS_axis - result of two operations under its axises - turn by teta around Fix_y  plus turn by Gamma around new Z'. Rotation Phi ( around Fix_z axis so that new Qs_z will leave Z_Y plane ) is not used here because for axial Field ( H//Z ) result does not depend on Phi for any angles Teta, values Eta.  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry) (Vxx-Vyy)/Vzz  ");
   //namepar[10]=strdup("  Phi    ");valpar[10]=0;	helppar[10]=strdup("Phi Angle - rotation of turned by Teta Qs on angle Phi around Fix_z axis  - so final QS_axis - result of three operations under its axises - turn by teta around Fix_x   turn on phi around Fix_z  plus turn by Gamma around new Z'. Result have not to depend on Phi at all for axial symmetric field H  ");
  };
void   FAZerHQs_Gamma::ClcBase(double *par,double *Xval,double *Res)
  {
//   VecCl Int(8),Pos(8);
//   CMatrCl Mat;
   //HQsGamma_Mat(Mat, Teta, Phi, H, Qs, Eta,Gamma);
   //HQsMatClc(Mat,H,IS,Int.Ptr,Pos.Ptr);
   //HQsInt(Int.Ptr,Pos.Ptr,W,Xval,Res);
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10];
   Solver_H_Z_poly(new Ham_Qs_Gamma(HamData(Teta,Phi,H,Qs,Eta,IS,W),Gamma)).Solve(Xval,Res);


//   for (int k=1;k<=Xval[0];k++) Res[k]=Gr-Amp*Res[k];
  };
     


//  =======================================================
//  === Polycristall case  Teta_Phi  angles of H rotation==
//  =============  in QS bases                      =======
//  =======================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerQsH(){return new FAZerQsH();}
static ClassDesc FAZerQsH_ClassDesc(typeid(FAZerQsH),"QsH_poly",1,"FAZer_Test",Dummy_FAZerQsH);
#else
static ClassDesc FAZerQsH_ClassDesc(typeid(FAZerQsH),"QsH_poly",1,"FAZer_Test",create<FAZerQsH>);
#endif

FAZerQsH::FAZerQsH()
  {
   name=strdup("QsH_poly");
   NumPar=9;
	 helppar[0 ]=strdup("Calculation of spectra of poycristal Fe. Main axis - Fix in Qs basis ( Fix ), H - is turned (H_Teta H_Phi) ");
   namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   namepar[3 ]=strdup("   H     ");valpar[3 ]=200;helppar[3 ]=strdup("Internal magnetic field     ");
   namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;helppar[6 ]=strdup("Width of line     ");
   namepar[7 ]=strdup("  Eta    ");valpar[7 ]=0;	helppar[7 ]=strdup("Qs Eta (xy assimetry) (Vxx-Vyy)/Vzz  ");
   namepar[8 ]=strdup("  H_Teta ");valpar[8 ]=0;	helppar[8 ]=strdup("Teta Angle between QS_Z and H  Teta rotation around Fix_y ");
   namepar[9 ]=strdup("  H_Phi  ");valpar[9 ]=0;	helppar[9 ]=strdup("Phi Angle - rotation around QS_Z -so final H - result of two operations - turn by teta around Fix_y  plus turn by Phi around Fix_Z.  ");
  };

#include "messb_in.h"
void   FAZerQsH::ClcBase(double *par,double *Xval,double *Res)
  {
   //VecCl Int(8),Pos(8);
   //CMatrCl Mat;
   //double H_Teta=par[8],H_Phi=par[9],H=par[3],Qs=par[4],Eta=par[7],
   //       IS=par[5],W=par[6],Gr=par[1],Amp=par[2];
   //QsHMat(Mat, H_Teta, H_Phi, H, Qs, Eta,IS);
   //my_comp sh=my_comp(0,-W/2);Mat=Mat+sh;
   //CMatrCl AmpL=R0HQsAmpVec_(),AmpR=AmpL;
   //RIntTurn(Mat,AmpL,AmpR,Xval,Res,W);

   //for (int k=1;k<=Xval[0];k++) Res[k]=Gr-Amp*Res[k];
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Eta=par[7],
          H_Teta=par[8],H_Phi=par[9];
   //Ref<HamData>  ham=new HamHTurned_Z_Qs(Ham_Qs_Eiler(0,0,H,Qs,Eta,IS,W),H_Teta,H_Phi);
   //Solver_H_Free_poly(ham).Solve(Xval,Res);

//HamHTurned_Z_Qs tmp(*ham,H_Teta,H_Phi);
//CMatrCl T=TurnSpinors::FormTurn4_2Ham(H_Teta,H_Phi),iT=Conj(Transpon(T)),
//Ht=tmp.Make8Ham();
////Ht=FormHHam(H_Teta,H_Phi,tmp.mu_g/tmp.mu_e);
//
//cout<<ham->Make8Ham()<<"\n"<<Ht<<"\n"<<T*Ht*iT<<"\n"<<ham->Make8Ham()-T*Ht*iT<<"\n";

   //Ref<HamData>  ham=new Ham_Z_Turned_Qs_Gamma(Ham_Qs_Gamma(HamData(0,0,H,Qs,Eta,IS,W),0),H_Teta,H_Phi);
   //Solver_H_Z_poly(ham).Solve(Xval,Res);



   Ref<HamData>  ham=new HamHTurned_Z_Qs(Ham_Qs_Eiler(0,0,H,Qs,Eta,IS,W),H_Teta,H_Phi);
//   Solver_Free2_4(ham).Solve(Xval,Res);
//   ContiniousRelaxationStepSolver(ham).Solve(Xval,Res);

   //ContiniousRelaxationSolver(new HamTimeGenerator(new HamTimeStepData(ham,0,100)),
   //  new ContiniousRelaxationStepMultiSolver(new TimeShiftGenerator(1,0,0))).Solve(Xval,Res);
   ContiniousRelaxationSolver(new HamTimeGenerator(ham,10,100),
     new ContiniousRelaxationStepMultiSolver(new TimeShiftGenerator(10,20),
        new ContiniousRelaxationStepSolver())).Solve(Xval,Res);
  };
     




//  ================================================
//  ================ Average int          ==========
//  ================================================
// Help is Not done yet
//  Monocristal in the external field
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerHQsAver(){return new FAZerHQsAver();}
static ClassDesc FAZerHQsAver_ClassDesc(typeid(FAZerHQsAver),"hq_aver",1,"FAZer_Test",Dummy_FAZerHQsAver);
#else
static ClassDesc FAZerHQsAver_ClassDesc(typeid(FAZerHQsAver),"hq_aver",1,"FAZer_Test",create<FAZerHQsAver>);
#endif
FAZerHQsAver::FAZerHQsAver()
  {
   name=strdup("hq_aver");
   NumPar=17;
													helppar[0 ]=strdup("Calculation of spectra of Fe under external magnetic field H. In the model used tensor of anisotropy of H ( internal magnetic field do not follow the direction of external field directly, for details look description of Add parameter ). Its main axis (anisotropy tensor's) are used as the main axis of fixed system. QS - is turned (Phi,Teta params)  in this system. Hc - module of the internal \"electron\" field, it is turned by the Hexternal according to the formula. Teta_V - defines angle between external field and viewing direction. Do not work any more. Test purpose only....");
   int i=StandardParams::H_Qs_eiler(namepar,valpar,helppar,1);

   //namepar[1 ]=strdup("Ground    ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity ");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("Hexternal ");valpar[3 ]=500;	helppar[3 ]=strdup("External magnetic field");
   //namepar[4 ]=strdup("   QS     ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS     ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W      ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta    ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi     ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta     ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");
   delete namepar[3];delete helppar[3];
   namepar[3 ]=strdup("Hexternal ");valpar[3 ]=500;	helppar[3 ]=strdup("External magnetic field");

   namepar[i+1]=strdup("  I_Hx_c  ");valpar[i+1]=0;	helppar[i+1]=strdup(" H magnetic anisotropy tenzor main Hx amplitude, direction coinced with Fixed axis system");
   namepar[i+2]=strdup("  I_Hy_c  ");valpar[i+2]=0;	helppar[i+2]=strdup(" H magnetic anisotropy tenzor main Hy amplitude, direction coinced with Fixed axis system");
   namepar[i+3]=strdup("  I_Hz_c  ");valpar[i+3]=0;	helppar[i+3]=strdup(" H magnetic anisotropy tenzor main Hz amplitude, direction coinced with Fixed axis system");
   namepar[i+4]=strdup("   Hc     ");valpar[i+4]=0;	helppar[i+4]=strdup(" Internal magnetic field amplitude");
   namepar[i+5]=strdup(" TetaV_H  ");valpar[i+5]=0;	helppar[i+5]=strdup(" Angle between Hext and viewing direction");
   namepar[i+6]=strdup(" Add_H    ");valpar[i+6]=0;	helppar[i+6]=strdup("If Add=1 - Hmag on the nucleus is calculated according to the formula: Hext - random direction of external field, Hc - amplitude of internal field, then: Hi[i]=I_H[i]_c  Hi=Hi+Hext  Hmod=sqrt(Hi*Hi)   Hi=Hi*(Hc/Hmod)+Hext - it means that internal magnetic field is turning to the direction of vector Hext+I_H . \t\tIf Add=0 then  Hi[i]=H_I[i]_c*Hext[i]   Hmod=sqrt(Hi*Hi)   Hi=Hi*(Hc/Hmod)+Hext  (defined in FormHSum procedure ). <internal magnetic field is turned to direction  H_I*Hext, where H_I - tensor, Hext - vector>");
   namepar[i+7]=strdup("FineAver  ");valpar[i+7]=0;	helppar[i+7]=strdup("Number of angles used in summation  is FineAver^2/2 ( aproximately ) (in one sector) ");
   namepar[i+8]=strdup(" Sum8     ");valpar[i+8]=0;	helppar[i+8]=strdup("Sum8=0 - make random angles only for one sphere sector. Sum8=1 - makes random angles for in all 8 sphere sectors ( 8 times more calculations )");
   NumPar=i+8;
  };


void   FAZerHQsAver::ClcBase(double *par,double *Xval,double *Res)
  {
   double Teta=par[7],Phi=par[10],H=par[3],Qs=par[4],Eta=par[9],Gamma=par[8],
          IS=par[5],W=par[6],Gr=par[1],Amp=par[2],Teta_V=par[14],Add_H=par[15],
          I_Hx=par[10],I_Hy=par[11],I_Hz=par[12],Hc=par[13],Fine=par[16],Sum8=par[17];

   Res[0]=Xval[0];
   (VecCl(Xval[0])*0).Copy2Double(Res);
   //HQsAvInt(Teta,Phi,H,Qs,Eta,IS,W,I_Hx,I_Hy,I_Hz,Hc,
   //                           Teta_V,Add_H,Fine,Sum8,Xval,Res);
//   for (int k=1;k<=Xval[0];k++) Res[k]=Gr-Amp*Res[k];
  };



/*

//  =======================================================
//  ================ Polycristall case  Eiler angles ======
//  =======================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerHQs_Eiler(){return new FAZerHQs_Eiler();}
static ClassDesc FAZerHQs_Eiler_ClassDesc(typeid(FAZerHQs_Eiler),"HQ_Eiler_poly",1,"FAZer_Test",Dummy_FAZerHQs_Eiler);
#else
static ClassDesc FAZerHQs_Eiler_ClassDesc(typeid(FAZerHQs_Eiler),"HQ_Eiler_poly",1,"FAZer_Test",create<FAZerHQs_Eiler>);
#endif

FAZerHQs_Eiler::FAZerHQs_Eiler()
  {
   name=strdup("HQ_Eiler_poly");
   NumPar=9;
													helppar[0 ]=strdup("Calculation of spectra of poycristal Fe. Main axis - Fix, H//Fix_z, QS - is turned (Phi,Teta params) Eiler angles. ");
   namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z  Teta - rotation around new Y' ( after Phi rotation )   ");
   namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle -rotation around Fix_z is done first - final QS_axis - result of two operations under its axises - turn on phi around Fix_z plus turn by teta around new Y'. Eiler angles. Angles can be treated as rotation by teta around Fix_y plus rotation by Phi around Z'");
   namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry) (Vxx-Vyy)/Vzz  ");
  };
double FAZerHQs_Eiler::Clc(double *par,double Xval)
  {
   VecCl Int(8),Pos(8);
   CMatrCl Mat;
   double Teta=par[7],Phi=par[8],H=par[3],Qs=par[4],Eta=par[9],IS=par[5],W=par[6];
   double Gr=par[1],Amp=par[2];
   HQsEiler_Mat(Mat, Teta, Phi, H, Qs, Eta);
   HQsMatClc(Mat,H,IS,Int.Ptr,Pos.Ptr);
   double ret=Gr-Amp*HQsInt(Int.Ptr,Pos.Ptr,W,Xval);
   return ret;
  };
void   FAZerHQs_Eiler::Clc(double *par,double *Xval,double *Res)
  {
   VecCl Int(8),Pos(8);
   CMatrCl Mat;
   double Teta=par[7],Phi=par[8],H=par[3],Qs=par[4],Eta=par[9],IS=par[5],W=par[6];
   double Gr=par[1],Amp=par[2];

   HQsEiler_Mat(Mat, Teta, Phi, H, Qs, Eta);
   HQsMatClc(Mat,H,IS,Int.Ptr,Pos.Ptr);
   HQsInt(Int.Ptr,Pos.Ptr,W,Xval,Res);

   for (int k=1;k<=Xval[0];k++) Res[k]=Gr-Amp*Res[k];
  };
     
void FAZerHQs_Eiler::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };

*/
