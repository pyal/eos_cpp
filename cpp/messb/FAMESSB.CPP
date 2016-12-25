#include <lib/precompiled/Messb.h>


#include "famessb.h"
#include "messb_av.h"
#include "messb_mean_field.h"
#include "messb_in.h"




//#include "malloc.h"

//double DebyFuncInt(double x) {if (x==0) return 0;return x*x*x/(exp(x)-1);};
//double DebyFunc(double x) 
//  {
//   double ResMis;int Err;
//   return 3/(x*x*x)*Qags_IntSubInt(DebyFuncInt,0,x,StndErr,StndErr,1000,ResMis,Err);};
////  RecInt(DebyFuncInt,0,x,StndErr);};
   

int StandardParams::H_Qs_eiler_short(char **namepar,double *valpar,char **helppar,int StartNum)
{
  int i=StartNum-1;
   namepar[1+i ]=strdup("   H     ");valpar[1+i ]=200;	helppar[1+i ]=strdup("Internal magnetic field     ");
   namepar[2+i ]=strdup("   QS    ");valpar[2+i ]=2;	  helppar[2+i ]=strdup("Internal QS    ");
   namepar[3+i ]=strdup("   IS    ");valpar[3+i ]=0;	  helppar[3+i ]=strdup("Isomer shift    ");
   namepar[4+i ]=strdup("   W     ");valpar[4+i ]=0.4;	helppar[4+i ]=strdup("Width of line     ");
   namepar[5+i ]=strdup("  Teta   ");valpar[5+i ]=0;	  helppar[5+i ]=strdup("Teta Angle between Fix_Z and QS_Z  Teta - rotation around Fix_Y ( after Phi rotation )   ");
   namepar[6+i ]=strdup("  Phi    ");valpar[6+i ]=0;	  helppar[6+i ]=strdup("Phi Angle -rotation around Fix_z is done first - final QS_axis - result of two operations under its axises - turn on phi around Fix_z plus turn by teta around Fix_Y. Not totally correct Eiler angles. ");
   namepar[7+i ]=strdup("  Eta    ");valpar[7+i ]=0;	  helppar[7+i ]=strdup("Qs Eta (xy assimetry) Vxx-Vyy  ");
   return 7;
}

int StandardParams::H_Qs_eiler(char **namepar,double *valpar,char **helppar,int StartNum)
{
   int i=StartNum-1;
   namepar[1+i ]=strdup("Ground   ");valpar[1+i ]=0;	  helppar[1+i ]=strdup("Added Ground int");
   namepar[2+i ]=strdup("Intencity");valpar[2+i ]=10;	  helppar[2+i ]=strdup("Spectr multiplied by ");
   return StandardParams::H_Qs_eiler_short(namepar,valpar,helppar,3+i)+2;
}
int StandardParams::H_Qs_gamma(char **namepar,double *valpar,char **helppar,int StartNum)
{
   int i=StartNum-1;
   namepar[1+i ]=strdup("Ground    ");valpar[1+i ]=0;	  helppar[1+i ]=strdup("Added Ground int");
   namepar[2+i ]=strdup("Intencity ");valpar[2+i ]=10;	helppar[2+i ]=strdup("Spectr multiplied by ");
   namepar[3+i ]=strdup("   H      ");valpar[3+i ]=200;	helppar[3+i ]=strdup("Internal magnetic field     ");
   namepar[4+i ]=strdup("   QS     ");valpar[4+i ]=2;	  helppar[4+i ]=strdup("Internal QS    ");
   namepar[5+i ]=strdup("   IS     ");valpar[5+i ]=0;	  helppar[5+i ]=strdup("Isomer shift    ");
   namepar[6+i ]=strdup("   W      ");valpar[6+i ]=0.4;	helppar[6+i ]=strdup("Width of line     ");
   namepar[7+i ]=strdup("  Teta   "); valpar[7+i ]=0;	  helppar[7+i ]=strdup("Teta Angle between Fix_Z and QS_Z  Teta rotation around Fix_y ");
   namepar[8+i ]=strdup("  Gamma  "); valpar[8+i ]=0;	  helppar[8+i ]=strdup("Gamma Angle - rotation around Fix_Z done first. Final QS_axis - result of three operations under its axises - turn by Gamma around Fix_Z turn by teta around Fix_y  plus turn by Phi around Fix_Z. for axial Field ( H//Z ) result does not depend on Phi for any angles Teta,Gamma values of Eta.  ");
   namepar[9+i ]=strdup("  Eta    "); valpar[9+i ]=0;	  helppar[9+i ]=strdup("Qs Eta (xy assimetry) (Vxx-Vyy)/Vzz  ");
   namepar[10+i]=strdup("  Phi    "); valpar[10+i]=0;	  helppar[10+i]=strdup("Phi Angle - rotation of turned by Gamma and Teta Qs on angle Phi around Fix_z axis  ");
   return 10;
}


//  =======================================================
//  ================ Polycristall case   Eiler angles =====
//  =======================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerHQs(){return new FAZerHQs();}
static ClassDesc FAZerHQs_ClassDesc(typeid(FAZerHQs),"HQ_poly",1,"FAZer",Dummy_FAZerHQs);
#else
static ClassDesc FAZerHQs_ClassDesc(typeid(FAZerHQs),"HQ_poly",1,"FAZer",create<FAZerHQs>);
#endif

FAZerHQs::FAZerHQs()
  {
   name=strdup("HQ_poly");

													helppar[0 ]=strdup("Calculation of spectra of poycristal Fe. Main axis - Fix, H//Fix_z, QS - is turned (Phi,Teta params). Eiler angles.");
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z  Teta - rotation around new Y' ( after Phi rotation )   ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle -rotation around Fix_z is done first - final QS_axis - result of two operations under its axises - turn on phi around Fix_z plus turn by teta around new Y'. Eiler angles. Angles can be treated as rotation by teta around Fix_y plus rotation by Phi around Z'");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry) Vxx-Vyy  ");
   NumPar=StandardParams::H_Qs_eiler(namepar,valpar,helppar,1);
  };
void   FAZerHQs::ClcBase(double *par,double *Xval,double *Res)
  {
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Phi=par[8],
          Eta=par[9];

   Solver_H_Z_poly(new Ham_Qs_Eiler(Teta,Phi,H,Qs,Eta,IS,W)).Solve(Xval,Res);

  };
     



//  =======================================================
//  ================ Monocristall case  ===================
//  =======================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerHQsM(){return new FAZerHQsM();}
static ClassDesc FAZerHQsM_ClassDesc(typeid(FAZerHQsM),"HQ_mono",1,"FAZer",Dummy_FAZerHQsM);
#else
static ClassDesc FAZerHQsM_ClassDesc(typeid(FAZerHQsM),"HQ_mono",1,"FAZer",create<FAZerHQsM>);
#endif

FAZerHQsM::FAZerHQsM()
  {
   name=strdup("HQ_mono");
   int i=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);
													helppar[0 ]=strdup("Calculation of spectra of monocrystall Fe. Main axis - Fix, H//Fix_z, QS - is turned (Phi,Teta Gamma params), Look along Dir(TetaH_View,TetaH_View) angles of Fix system coordinats.");
   //namepar[1 ]=strdup("Ground    ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity ");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H      ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS     ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS     ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W      ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z  Teta rotation around Fix_y ");
   //namepar[8 ]=strdup("  Gamma  ");valpar[8 ]=0;	helppar[8 ]=strdup("Gamma Angle - rotation around new Z' -so final QS_axis - result of two operations under its axises - turn by teta around Fix_y  plus turn by Gamma around new Z'. Rotation Phi ( around Fix_z axis so that new Qs_z will leave Z_Y plane ) is not used here because for axial Field ( H//Z ) result does not depend on Phi for any angles Teta, values Eta.  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry) (Vxx-Vyy)/Vzz  ");
   //namepar[10]=strdup("  Phi    ");valpar[10]=0;	helppar[10]=strdup("Phi Angle - rotation of turned by Teta Qs on angle Phi around Fix_z axis  - so final QS_axis - result of three operations under its axises - turn by teta around Fix_x   turn on phi around Fix_z  plus turn by Gamma around new Z'. Result have not to depend on Phi at all for axial symmetric field H  ");
   namepar[i+1]=strdup("TetaH_View");valpar[i+1]=0;	helppar[i+1]=strdup("Look along Dir(TetaH_View,TetaH_View) angles of Fix system coordinats.   ");
   namepar[i+2]=strdup("PhiH_View ");valpar[i+2]=0;	helppar[i+2]=strdup("Look along Dir(TetaH_View,TetaH_View) angles of Fix system coordinats.   ");
   NumPar=i+2;
  };
void   FAZerHQsM::ClcBase(double *par,double *Xval,double *Res)
  {
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10],
          VTeta=par[11],VPhi=par[12];

   Solver_H_Z_mono(new Ham_Qs_Gamma(HamData(Teta,Phi,H,Qs,Eta,IS,W),Gamma),VTeta,VPhi).Solve(Xval,Res);

  };




//  ================================================
//  ================ Relax 2 case Free  ============
//  ================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerFreeR(){return new FAZerFreeR();}
static ClassDesc FAZerFreeR_ClassDesc(typeid(FAZerFreeR),"F_relax2",1,"FAZer",Dummy_FAZerFreeR);
#else
static ClassDesc FAZerFreeR_ClassDesc(typeid(FAZerFreeR),"F_relax2",1,"FAZer",create<FAZerFreeR>);
#endif
FAZerFreeR::FAZerFreeR()
  {
   name=strdup("F_relax2");
//   NumPar=22;
													helppar[0 ]=strdup("Calculation of spectra of Fe under relaxation, 2- state system, Blumes calculations. Treated two states - from state to state changes not only H, but also QS. There is a fixed axis system (Fix). According to it QS1, QS2 - is turned (Phi1,2,Teta1,2 params) . Magnetic field also is turned - AlphH_zx1,2 (Teta) AlphH_xy1,2 - (Phi) angle, H,QS - have arbitrary value in both states. T1 T2 - Blumes inverce times for state 1,2");
   int i=StandardParams::H_Qs_eiler(namepar,valpar,helppar,1);

   //namepar[1 ]=strdup("Ground    ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity ");valpar[2 ]=10;	helppar[2 ]=strdup("Spectras multiplied by - the same for both spectras");
   //namepar[3 ]=strdup("   H1     ");valpar[3 ]=500;	helppar[3 ]=strdup("Internal magnetic field -     state 1 ");
   //namepar[4 ]=strdup("   QS1    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS                   state 1 ");
   //namepar[5 ]=strdup("   IS1    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift                  state 1");
   //namepar[6 ]=strdup("   W1     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line                 state 1");
   //namepar[7 ]=strdup("  Teta1   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z  state 1  ");
   //namepar[8 ]=strdup("  Phi1    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta1    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)         state 1");

   namepar[i+1]=strdup("AlphH_zx1 ");valpar[i+1]=0;	helppar[i+1]=strdup("Teta angle for magnetic field state 1");
   namepar[i+2]=strdup("AlphH_xy1 ");valpar[i+2]=0;	helppar[i+2]=strdup("Phi angle for magnetic field  state 1");

   i+=2+StandardParams::H_Qs_eiler_short(namepar,valpar,helppar,i+3);
   //namepar[12]=strdup("   H2     ");valpar[12]=500;	helppar[12]=strdup("Internal magnetic field -     state 2 ");
   //namepar[13]=strdup("   QS2    ");valpar[13]=2;	helppar[13]=strdup("Internal QS                   state 2 ");
   //namepar[14]=strdup("   IS2    ");valpar[14]=0;	helppar[14]=strdup("Isomer shift                  state 2");
   //namepar[15]=strdup("   W2     ");valpar[15]=0.4;	helppar[15]=strdup("Width of line                 state 2");
   //namepar[16]=strdup("  Teta2   ");valpar[16]=0;	helppar[16]=strdup("Teta Angle between Fix_Z and QS_Z  state 2  ");
   //namepar[17]=strdup("  Phi2    ");valpar[17]=0;	helppar[17]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[18]=strdup("  Eta2    ");valpar[18]=0;	helppar[18]=strdup("Qs Eta (xy assimetry)         state 2");
   namepar[i+1]=strdup("AlphH_zx2 ");valpar[i+1]=57.4;helppar[i+1]=strdup("Teta angle for magnetic field state 2");
   namepar[i+2]=strdup("AlphH_xy2 ");valpar[i+2]=0;	  helppar[i+2]=strdup("Phi angle for magnetic field  state 2");

   namepar[i+3]=strdup("  T1      ");valpar[i+3]=10;	helppar[i+3]=strdup("Blumes time for state 1 (inverce time, in units of width, after which, relaxation occur, with the same probability to any other state )");
   namepar[i+4]=strdup("  T2      ");valpar[i+4]=10;	helppar[i+4]=strdup("Blumes time for state 2 (inverce time, in units of width, after which, relaxation occur, with the same probability to any other state )");

   NumPar=i+4;

  };

void   FAZerFreeR::ClcBase(double *par,double *Xval,double *Res)
  {
   double 
          H1=par[3],Qs1=par[4],IS1=par[5],W1=par[6],
          Teta1=par[7],Phi1=par[8],Eta1=par[9],
          AlphH_zx1=par[10],AlphH_xy1=par[11],
          H2=par[12],Qs2=par[13],IS2=par[14],W2=par[15],
          Teta2=par[16],Phi2=par[17],Eta2=par[18],
          AlphH_zx2=par[19],AlphH_xy2=par[20],
          T1=par[21],T2=par[22];
//   Teta1=Phi1=Teta2=Phi2=0;
   vector<Ref<HamData> > ham(2);
   ham[0]=new HamHTurned_Z_Free_Qs_Eiler(Ham_Qs_Eiler(Teta1,Phi1,H1,Qs1,Eta1,IS1,W1),AlphH_zx1,AlphH_xy1);
   ham[1]=new HamHTurned_Z_Free_Qs_Eiler(Ham_Qs_Eiler(Teta2,Phi2,H2,Qs2,Eta2,IS2,W2),AlphH_zx2,AlphH_xy2);
   VecCl t(2);t[1]=T1;t[2]=T2;
   Solver_Blume_Nax_poly(ham,t).Solve(Xval,Res);


  };



//  ================================================
//  ================ Average HQs  case    ==========
//  ================================================
//  Monocristal in the external field rotated base
#include "messb_av.h"
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerHQsExternalField(){return new FAZerHQsExternalField();}
static ClassDesc FAZerHQsExternalField_ClassDesc(typeid(FAZerHQsExternalField),"QsH_externalH",1,"FAZer",Dummy_FAZerHQsExternalField);
#else
static ClassDesc FAZerHQsExternalField_ClassDesc(typeid(FAZerHQsExternalField),"QsH_externalH",1,"FAZer",create<FAZerHQsExternalField>);
#endif

FAZerHQsExternalField::FAZerHQsExternalField()
{
   name=strdup("QsH_externalH");
//   NumPar=21;
													helppar[0 ]=strdup("Calculation of spectra of Fe under external magnetic field H. In the model used tensor of anisotropy of H ( internal magnetic field do not follow the direction of external field directly, for details look html file ). Its main axis (anisotropy tensor's) are used as the main axis of fixed system. QS - is turned, constructed in Qs_gamma model (Phi,Teta,Gamma params)  according to fix system. Hc - module of the internal \"electron\" field, it is turned by the Hexternal according to the formula ( view HTML ). Teta_V - defines angle between external field and viewing direction");
   int i=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);

   //namepar[1 ]=strdup("Ground    ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity ");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("Hexternal ");valpar[3 ]=500;	helppar[3 ]=strdup("External magnetic field");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z  Teta rotation around Fix_y ");
   //namepar[8 ]=strdup("  Gamma  ");valpar[8 ]=0;	helppar[8 ]=strdup("Gamma Angle - rotation around new Z' -so final QS_axis - result of two operations under its axises - turn by teta around Fix_y  plus turn by Gamma around new Z'. Rotation Phi ( around Fix_z axis so that new Qs_z will leave Z_Y plane ) is not used here because for axial Field ( H//Z ) result does not depend on Phi for any angles Teta, values Eta.  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry) (Vxx-Vyy)/Vzz  ");
   //namepar[10]=strdup("  Phi    ");valpar[10]=0;	helppar[10]=strdup("Phi Angle - rotation of turned by Teta Qs on angle Phi around Fix_z axis  - so final QS_axis - result of three operations under its axises - turn by teta around Fix_x   turn on phi around Fix_z  plus turn by Gamma around new Z'. Result have not to depend on Phi at all for axial symmetric field H  ");
   delete namepar[3];delete helppar[3];
   namepar[3 ]=strdup("Hexternal ");valpar[3 ]=500;	helppar[3 ]=strdup("External magnetic field");

   namepar[i+1 ]=strdup("  I_Hx_c  ");valpar[i+1 ]=0;	helppar[i+1 ]=strdup(" H magnetic anisotropy tenzor main Hx amplitude, direction coinced with Fixed axis system");
   namepar[i+2 ]=strdup("  I_Hy_c  ");valpar[i+2 ]=0;	helppar[i+2 ]=strdup(" The same as I_Hx_c, Y direction");
   namepar[i+3 ]=strdup("  I_Hz_c  ");valpar[i+3 ]=0;	helppar[i+3 ]=strdup(" The same as I_Hx_c, Z direction");
   namepar[i+4 ]=strdup("    Vx_c  ");valpar[i+4 ]=0;	helppar[i+4 ]=strdup(" H magnetic anisotropy vector, first order correction to Ham, direction is calculated in Fixed axis system");
   namepar[i+5 ]=strdup("    Vy_c  ");valpar[i+5 ]=0;	helppar[i+5 ]=strdup("The same as Vx_c - Y component");
   namepar[i+6 ]=strdup("    Vz_c  ");valpar[i+6 ]=0;	helppar[i+6 ]=strdup("The same as Vx_c - Z component");
   namepar[i+7 ]=strdup("   Hc     ");valpar[i+7 ]=0;	helppar[i+7 ]=strdup(" Internal magnetic field amplitude");
   namepar[i+8 ]=strdup(" TetaV_H  ");valpar[i+8 ]=0;	helppar[i+8 ]=strdup(" Angle between Hext and viewing direction");
   namepar[i+9 ]=strdup(" NumRotV  ");valpar[i+9 ]=0;	helppar[i+9 ]=strdup("Number of View angles used in summation (if TetaV=0 - use NumRotV=0, )");
   namepar[i+10]=strdup("NumRndH   ");valpar[i+10]=0;	helppar[i+10]=strdup("Number of Hexternal angles used in summation  is NumRndH^2/2 ( aproximately ) (for one sector) ");
   namepar[i+11]=strdup(" NotSum8  ");valpar[i+11]=0;	helppar[i+11]=strdup("NotSum8=1 - make random angles only for one sphere sector. NotSum8=0 - makes random angles for in all 8 sphere sectors ( 8 times more calculations )");
   NumPar=i+11;
};
void   FAZerHQsExternalField::ClcBase(double *par,double *Xval,double *Res)
{
   double Hext=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10],
          I_Hx_c=par[11],I_Hy_c=par[12],I_Hz_c=par[13],
          Vx_c=par[14],Vy_c=par[15],Vz_c=par[16],
          Hc=par[17],TetaV_H=par[18],NumRotV=par[19],NumRndH=par[20],NotSum8=par[21];


   PolyCrystallAverage(new SphereAnglesGenerator(NumRndH,NumRotV,NotSum8,TetaV_H),
     new AnisotropeGenerator(Hc,V3D_Angles(V3D_XYZ(Vx_c,Vy_c,Vz_c)),
     V3D_XYZ(I_Hx_c,I_Hy_c,I_Hz_c)),
     new Ham_Qs_Gamma(HamData(Teta,Phi,0,Qs,Eta,IS,W),Gamma),Hext).Solve(Xval,Res);
};



//  ================================================
//  ======== Relaxation n _H angle Function  =======
//  ================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerQsH_R_Hn(){return new FAZerQsH_R_Hn();}
static ClassDesc FAZerQsH_R_Hn_ClassDesc(typeid(FAZerQsH_R_Hn),"qh_relax_Hn",1,"FAZer",Dummy_FAZerQsH_R_Hn);
#else
static ClassDesc FAZerQsH_R_Hn_ClassDesc(typeid(FAZerQsH_R_Hn),"qh_relax_Hn",1,"FAZer",create<FAZerQsH_R_Hn>);
#endif

FAZerQsH_R_Hn::FAZerQsH_R_Hn()
  {
   name=strdup("qh_relax_Hn");
//   NumPar=42;
   int i=StandardParams::H_Qs_eiler(namepar,valpar,helppar,1);

													helppar[0 ]=strdup("Calculation of spectra of Fe under H relaxation - Blumes treatment. H(t) makes random motion from ona axis to another ( up to 8). Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta params). For each axis is given H=H(H[i],A_zx,A_xy) A_zx - Teta angle, A_xy - Phi angle ( see description of Teta,Phi params for QS). Also is given T[i] - mean !!!inverse time (calculated in width) after which H will try to jump to some other axis");
   //namepar[1 ]=strdup("Ground    ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity ");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H      ");valpar[3 ]=500;	helppar[3 ]=strdup("Internal magnetic field  is not used here, ups");
   //namepar[4 ]=strdup("   QS     ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS     ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W      ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta    ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi     ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta     ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");
   delete helppar[3];helppar[3 ]=strdup("Internal magnetic field  is not used here, ups");
   namepar[i+1 ]=strdup("  numH    ");valpar[i+1 ]=2;	  helppar[i+1 ]=strdup(" Number of axis to jump in between ");

   namepar[i+2 ]=strdup("  T1      ");valpar[i+2 ]=1;	  helppar[i+2 ]=strdup(" Blumes time ( inverse ) for axis 1 ");
   namepar[i+3 ]=strdup("  H1      ");valpar[i+3 ]=500;	helppar[i+3 ]=strdup(" Amplitude of magnetic field for axis 1");
   namepar[i+4 ]=strdup(" Alph1_zx ");valpar[i+4 ]=0;	  helppar[i+4 ]=strdup(" Teta angle of magnetic field for axis  1 (Measured in Fixed axis system)");
   namepar[i+5 ]=strdup(" Alph1_xy ");valpar[i+5 ]=0;	  helppar[i+5 ]=strdup(" Phi angle of magnetic field for axis  1 (Measured in Fixed axis system)");
   namepar[i+6 ]=strdup("  T2      ");valpar[i+6 ]=10;	helppar[i+6 ]=strdup(" The same for axis 2 ");
   namepar[i+7 ]=strdup("  H2      ");valpar[i+7 ]=500;	helppar[i+7 ]=strdup("");
   namepar[i+8 ]=strdup(" Alph2_zx ");valpar[i+8 ]=57.4;helppar[i+8 ]=strdup("");
   namepar[i+9 ]=strdup(" Alph2_xy ");valpar[i+9 ]=0;	  helppar[i+9 ]=strdup("");
   namepar[i+10]=strdup("  T3      ");valpar[i+10]=0;	  helppar[i+10]=strdup("");
   namepar[i+11]=strdup("  H3      ");valpar[i+11]=0;	  helppar[i+11]=strdup("");
   namepar[i+12]=strdup(" Alph3_zx ");valpar[i+12]=0;	  helppar[i+12]=strdup("");
   namepar[i+13]=strdup(" Alph3_xy ");valpar[i+13]=0;	  helppar[i+13]=strdup("");
   namepar[i+14]=strdup("  T4      ");valpar[i+14]=0;	  helppar[i+14]=strdup("");
   namepar[i+15]=strdup("  H4      ");valpar[i+15]=0;	  helppar[i+15]=strdup("");
   namepar[i+16]=strdup(" Alph4_zx ");valpar[i+16]=0;	  helppar[i+16]=strdup("");
   namepar[i+17]=strdup(" Alph4_xy ");valpar[i+17]=0;	  helppar[i+17]=strdup("");
   namepar[i+18]=strdup("  T5      ");valpar[i+18]=0;	  helppar[i+18]=strdup("");
   namepar[i+19]=strdup("  H5      ");valpar[i+19]=0;	  helppar[i+19]=strdup("");
   namepar[i+20]=strdup(" Alph5_zx ");valpar[i+20]=0;	  helppar[i+20]=strdup("");
   namepar[i+21]=strdup(" Alph5_xy ");valpar[i+21]=0;	  helppar[i+21]=strdup("");
   namepar[i+22]=strdup("  T6      ");valpar[i+22]=0;	  helppar[i+22]=strdup("");
   namepar[i+23]=strdup("  H6      ");valpar[i+23]=0;	  helppar[i+23]=strdup("");
   namepar[i+24]=strdup(" Alph6_zx ");valpar[i+24]=0;	  helppar[i+24]=strdup("");
   namepar[i+25]=strdup(" Alph6_xy ");valpar[i+25]=0;	  helppar[i+25]=strdup("");
   namepar[i+26]=strdup("  T7      ");valpar[i+26]=0;	  helppar[i+26]=strdup("");
   namepar[i+27]=strdup("  H7      ");valpar[i+27]=0;	  helppar[i+27]=strdup("");
   namepar[i+28]=strdup(" Alph7_zx ");valpar[i+28]=0;	  helppar[i+28]=strdup("");
   namepar[i+29]=strdup(" Alph7_xy ");valpar[i+29]=0;	  helppar[i+29]=strdup("");
   namepar[i+30]=strdup("  T8      ");valpar[i+30]=0;	  helppar[i+30]=strdup("");
   namepar[i+31]=strdup("  H8      ");valpar[i+31]=0;	  helppar[i+31]=strdup("");
   namepar[i+32]=strdup(" Alph8_zx ");valpar[i+32]=0;	  helppar[i+32]=strdup("");
   namepar[i+33]=strdup(" Alph8_xy ");valpar[i+33]=0;	  helppar[i+33]=strdup("");

   NumPar=i+33;
  };
void   FAZerQsH_R_Hn::ClcBase(double *par,double *Xval,double *Res)
  {

   double 
          Qs=par[4],IS=par[5],W=par[6],
          Teta=par[7],Phi=par[8],Eta=par[9],NumH=par[10]; //H=par[3],
   vector<Ref<HamData> > ham(NumH);
   VecCl t(NumH);
   double h,zx,xy;
   for (int k=1;k<=NumH;k++)
   {
	   int Sh=10+(k-1)*4;
     h=par[Sh+2];zx=par[Sh+3];xy=par[Sh+4];
	   t[k]=par[Sh+1];
     ham[k-1]=new HamHTurned_Z_Free_Qs_Eiler(Ham_Qs_Eiler(Teta,Phi,h,Qs,Eta,IS,W),zx,xy);
   }
   Solver_Blume_Nax_poly(ham,t).Solve(Xval,Res);
  };

//  ================================================
//  ================ Relaxation Random Walk  =======
//  ================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerQsHRelaxRandWalk(){return new FAZerQsHRelaxRandWalk();}
static ClassDesc FAZerQsHRelaxRandWalk_ClassDesc(typeid(FAZerQsHRelaxRandWalk),"qh_relax_rand_walk",1,"FAZer",Dummy_FAZerQsHRelaxRandWalk);
#else
static ClassDesc FAZerQsHRelaxRandWalk_ClassDesc(typeid(FAZerQsHRelaxRandWalk),"qh_relax_rand_walk",1,"FAZer",create<FAZerQsHRelaxRandWalk>);
#endif
FAZerQsHRelaxRandWalk::FAZerQsHRelaxRandWalk()
  {
   name=strdup("qh_relax_rand_walk");
   //NumPar=27;
   helppar[0 ]=strdup("Calculation of spectra of Fe under H relaxation . H(t) - supposed to make in gaus distributed time (TimeTeta(Phi),SigTimeTeta(Phi)) random steps by gaus distributed angles (AngTeta(Phi),SigAngTeta(Phi)). Probability to make +- step is calculated according anisotropy model Pplus/Pminus=exp(-sin^2(plus))/exp(-sin^2(minus) Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta params). Model gives very good results for field - collapsing process (unlike multiaxis relaxation models) put Angle=5 and decrease Time(Teta,Phi) to 1e-3 - 1e-5./n Remarks: Teta Phi params do not work  ");
   int i=StandardParams::H_Qs_eiler(namepar,valpar,helppar,1);

   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Currnetly - do not work Qs_Z//Z Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Currnetly - do not work Qs_Z//Z Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");

   namepar[i+1 ]=strdup("  OutPhi "); valpar[i+1 ]=1;	  helppar[i+1 ]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2 ]=strdup("  Time   "); valpar[i+2 ]=30;	helppar[i+2 ]=strdup("Time to make calcultaion of relaxation process. Line intencity is calculated by formula Int=exp(-CurTime*Width)  so final line intencity Int_fin=exp(-Time*Width) - all other process is not treated ");
   namepar[i+3 ]=strdup("NumToAver"); valpar[i+3 ]=1;	  helppar[i+3 ]=strdup("Number of times to recalculte relaxating H angles  ( to calculate mean line intencity value by averaging of all of them )");
   namepar[i+4 ]=strdup("NTimeAver"); valpar[i+4 ]=1;	  helppar[i+4 ]=strdup("Number of times to calculate intencity spectra for one given H angle serie ( modify start integration time Ts for serie from 0 to EndTAver (given by next parameter) I=Sum(Int(Ts[i],Tfinal)) )");
   namepar[i+5 ]=strdup("EndTAver "); valpar[i+5 ]=1;	  helppar[i+5 ]=strdup("Final Average intgration time (look NTimeAver parameter for explanation) ");

   namepar[i+6 ]=strdup(" TimeTeta  ");valpar[i+6 ]=1;	helppar[i+6 ]=strdup("Average time between Teta angle jumps");
   namepar[i+7 ]=strdup("SigTimeTeta");valpar[i+7 ]=0.1;helppar[i+7 ]=strdup("Dispertion of average time between Teta angle jumps");
   namepar[i+8 ]=strdup(" AngTeta   ");valpar[i+8 ]=10;	helppar[i+8 ]=strdup("Average jump Teta angle value");
   namepar[i+9 ]=strdup(" SigAngTeta");valpar[i+9 ]=5;	helppar[i+9 ]=strdup("Dispertion of average jump Teta angle value");
   namepar[i+10]=strdup("  AnisTeta ");valpar[i+10]=5;	helppar[i+10]=strdup("Anisotropy coef for teta angle prob calculations (look param description for AnisPower)");
   namepar[i+11]=strdup(" TimePhi   ");valpar[i+11]=1;	helppar[i+11]=strdup("Average time between Phi angle jumps");
   namepar[i+12]=strdup(" SigTimePhi");valpar[i+12]=0.1;helppar[i+12]=strdup(" ");
   namepar[i+13]=strdup(" AngPhi    ");valpar[i+13]=10;	helppar[i+13]=strdup("");
   namepar[i+14]=strdup(" SigAngPhi ");valpar[i+14]=5;	helppar[i+14]=strdup(" ");
   namepar[i+15]=strdup("  AnisPhi  ");valpar[i+15]=5; 	helppar[i+15]=strdup(" ");
   namepar[i+16]=strdup("  NoPhi    ");valpar[i+16]=0;	helppar[i+16]=strdup(" Only teta angle works, phi is fixed; Phi=0; ");
   namepar[i+17]=strdup("  HmulCos  ");valpar[i+17]=0; 	helppar[i+17]=strdup(" If NoPhi && HmulCos - mod[H(Teta)]=H*cos(Teta) - model of very fast Phi relaxation.Nucleu sees only mean field value");
   namepar[i+18]=strdup(" AnisPower ");valpar[i+18]=1; 	helppar[i+18]=strdup(" AnisPower used for Teta-Phi relaxation calculation. If AnisPower>0 P(teta)=AnisTeta*exp(-(sin(teta))^AnisPower) else P(teta)=AnisTeta*exp(-(cos(teta))^-AnisPower). If AnisPower is not integer - is done fabs(sin) or fabs(cos) before power.");

   namepar[i+19]=strdup("EigenAcc "); valpar[i+19]=0;	 helppar[i+19]=strdup("  What method will be used for integration: 1 - will be used eigen acclerated method, 0 - will be used direct integration. For method 1 - parameters used: NumToAver - num of different series of ham will be generated; NTimeAver - number of times each generated serie will be shifted by initial time; EndTAver - maximum time shift for serie;; For method 0 : parameters are used - : NumToAver - num of different series of ham will be generated; OutMeanAmp - to output mean amplitude generated (spectr - is fourie transform of this amplitude); DiscrNum - number of discretization time of Ham calculation - have to be of the order - 10000 - the higher value, the more precies result ( to some extent )");
   namepar[i+20]=strdup("OutMeanAmp");valpar[i+20]=0;	 helppar[i+20]=strdup("  1 - to output the result mean amplitude, before fourie transformation ");
   namepar[i+21]=strdup("DiscrNum "); valpar[i+21]=1e4;helppar[i+21]=strdup("  Number of time steps to make for Amplitude construction, have to be of the order 10000 - the higher - the better ( but in some moment calculation can become unstable ");

   NumPar=i+21;
  };
void   FAZerQsHRelaxRandWalk::ClcBase(double *par,double *Xval,double *Res)
  {
//   RelaxAverageStruct Aver;
    double H=par[3],QS=par[4],IS=par[5],W=par[6],
           Teta=par[7],Phi=par[8],Eta=par[9],OutPhi=par[10],Time=par[11],
           NumToAver=par[12],NTimeAver=par[13],EndTAver=par[14],TimeTeta=par[15],
           SigTimeTeta=par[16],AngTeta=par[17],SigAngTeta=par[18],AnisTeta=par[19],
           TimePhi=par[20],SigTimePhi=par[21],AngPhi=par[22],SigAngPhi=par[23],
           AnisPhi=par[24],NoPhi=par[25],HmulCos=par[26],AnisPower=par[27],
           EigenMethod=par[28],OutAmp=par[29],HamDiscr=par[30]; //Ground=par[1],Intencity=par[2],

   Ref<RndAngleGenerator> ang_t=
     new RndAngleGenerator(
        new RndBoltsman_GausWalk(
          (RndGaus*)(new RndGaus(AngTeta,SigAngTeta))->Init(AngTeta-4*SigAngTeta,AngTeta+4*SigAngTeta,100,1e-5),
          (RndBoltsmanSinPower*)(new RndBoltsmanSinPower(AnisTeta,AnisPower))->Init(-180,180,360,1e-5)),
        new RndTimeGenerator((new RndGaus(TimeTeta,SigTimeTeta))->Init(max<double>(TimeTeta-4*SigTimeTeta,0),TimeTeta+4*SigTimeTeta,100,1e-5),0,Time));
   Ref<Rnd2AngleGenerator_Base> ang;
   if (!NoPhi) 
     ang=new Rnd2AngleGenerator(ang_t,
      new RndAngleGenerator(
        new RndBoltsman_GausWalk(
          (RndGaus*)(new RndGaus(AngPhi,SigAngPhi))->Init(AngPhi-4*SigAngPhi,AngPhi+4*SigAngPhi,100,1e-5),
          (RndBoltsmanSinPower*)(new RndBoltsmanSinPower(AnisPhi,AnisPower))->Init(-180,180,360,1e-5)),
        new RndTimeGenerator((new RndGaus(TimePhi,SigTimePhi))->Init(max<double>(TimePhi-4*SigTimePhi,0),TimePhi+4*SigTimePhi,100,1e-5),0,Time)));
   else 
     ang=new Rnd2AngleGenerator_FixPhi(ang_t,0);

   Ref<HamTimeAverageRndWalk> hams=new HamTimeAverageRndWalk(
    
    ang,new Ham_Qs_Eiler(Teta,Phi,H,QS,Eta,IS,W),0,HmulCos,OutPhi);

   if (EigenMethod)
   {

      ContiniousRelaxationAverageSolver(
        new ContiniousRelaxationSolver(
            NULL,new ContiniousRelaxationStepMultiSolver(
              new TimeShiftGenerator(NTimeAver,EndTAver),new ContiniousRelaxationStepSolver())),
              hams,NumToAver).Solve(Xval,Res);
   }
   else
     ContiniousRelaxationAverageSolver_DirectMatrMul(hams,NumToAver,HamDiscr,OutAmp)
      .Solve(Xval,Res);
  };

//  ================================================
//  ================ Relaxation Random Jump  =======
//  ================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerQsHRelaxRandJump(){return new FAZerQsHRelaxRandJump();}
static ClassDesc FAZerQsHRelaxRandJump_ClassDesc(typeid(FAZerQsHRelaxRandJump),"qh_relax_rand_Jump",1,"FAZer",Dummy_FAZerQsHRelaxRandJump);
#else
static ClassDesc FAZerQsHRelaxRandJump_ClassDesc(typeid(FAZerQsHRelaxRandJump),"qh_relax_rand_Jump",1,"FAZer",create<FAZerQsHRelaxRandJump>);
#endif
FAZerQsHRelaxRandJump::FAZerQsHRelaxRandJump()
  {
   name=strdup("qh_relax_rand_Jump");
   //NumPar=49;

   helppar[0 ]=strdup("Calculation of spectra of Fe under H relaxation. H(t) - random ( boltsman  distrib Prob(alpha)=exp(-Anis_alpha*sqr(sin(alpha*M_PI/180))) for angles (Teta,Phi)),with random (gaus distrib) time step(params TimeTeta(Phi) SigTimeTeta(Phi)). Plus Added several (axes) between which H jumps(AnisJump+AxesJump is treated) Prob[i] for i-th axes have the same meaning like Blumes coefficients (look description for TimeDisp parameter)(can be checked with Blume relaxation ( set anis_alpha>1e4 - no boltzman random turns)- have to give the same result.) Added parameter TimeDisp - Prob[i] gives only times, TimeDisp - relative dispertion of this times(Blume says that result have to be the same for any dispertion?). Supposed that we have fixed Fix_axis - coordinat system , in this system QS_Z//Z - is fixed and not turned (Phi,Teta params are not used), H- relaxing.\n\n Problems: Teta Phi - does not work.   ");
   int i=StandardParams::H_Qs_eiler(namepar,valpar,helppar,1);
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift  - currently does not work  ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Not used Qs_Z//Z currently (have to be: Teta Angle between Fix_Z and QS_Z)    ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Not used Qs_Z//Z currently  (have to be: Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z ) ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (Qxy assimetry)   ");

   namepar[i+1 ]=strdup("  OutPhi ");valpar[i+1 ]=1;	helppar[i+1 ]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2 ]=strdup("  Time   ");valpar[i+2 ]=30;	helppar[i+2 ]=strdup("Time to make calcultaion of relaxation process. Line intencity is calculated by formula Int=exp(-CurTime*Width)  so final line intencity Int_fin=exp(-Time*Width) - all other process is not treated ");
   namepar[i+3 ]=strdup("NumToAver");valpar[i+3 ]=1;	helppar[i+3 ]=strdup("Number of times to recalculte relaxating H angles  ( to calculate mean line intencity value by averaging of all of them )");
   namepar[i+4 ]=strdup("NTimeAver");valpar[i+4 ]=1;	helppar[i+4 ]=strdup("Number of times to calculate intencity spectra for one given H angle serie ( modify start integration time Ts for serie from 0 to EndTAver (given by next parameter) I=Sum(Int(Ts[i],Tfinal)) )");
   namepar[i+5 ]=strdup("EndTAver ");valpar[i+5 ]=1;	helppar[i+5 ]=strdup("Final Average intgration time (look NTimeAver parameter for explanation) ");

   namepar[i+6 ]=strdup(" TimeTeta  ");valpar[i+6 ]=1;	helppar[i+6 ]=strdup("Average time between Teta angle jumps");
   namepar[i+7 ]=strdup("SigTimeTeta");valpar[i+7 ]=0.1;helppar[i+7 ]=strdup("Dispertion of average time between Teta angle jumps");
   namepar[i+8 ]=strdup("  AnisTeta ");valpar[i+8 ]=5;	helppar[i+8 ]=strdup("Anisotropy coef for teta angle prob calculations (look param description for AnisPower)");
   namepar[i+9 ]=strdup(" TimePhi   ");valpar[i+9 ]=1;	helppar[i+9 ]=strdup("Average time between Phi angle jumps");
   namepar[i+10]=strdup(" SigTimePhi");valpar[i+10]=0.1;helppar[i+10]=strdup(" ");
   namepar[i+11]=strdup("  AnisPhi  ");valpar[i+11]=5; 	helppar[i+11]=strdup(" ");
   namepar[i+12]=strdup("  NoPhi    ");valpar[i+12]=0;	helppar[i+12]=strdup(" Only teta angle works, phi is fixed; Phi=0; (relaxation only in one plane)");
   namepar[i+13]=strdup("  HmulCos  ");valpar[i+13]=0; 	helppar[i+13]=strdup(" If NoPhi && HmulCos - mod[H(Teta)]=H*cos(Teta) (model of fast relaxation Phi angles - so that nucleus fills only mean value");
   namepar[i+14]=strdup(" AnisPower ");valpar[i+14]=1; 	helppar[i+14]=strdup(" AnisPower used for Teta-Phi  relaxation calculation If AnisPower>0 P(teta)=AnisTeta*exp(-(sin(teta))^AnisPower) else P(teta)=AnisTeta*exp(-(cos(teta))^-AnisPower). If AnisPower is not integer - is done fabs(sin) or fabs(cos) before power");

   namepar[i+15]=strdup(" NumAx     ");valpar[i+15]=5;	helppar[i+15]=strdup("Number of axes to jump between (max 8)");
   namepar[i+16]=strdup(" TimeDisp  ");valpar[i+16]=5; 	helppar[i+16]=strdup("TimeDispertion multiplier of RelaxTime. Treated a model: calculated gaus distrib function: GausDistrib(1 : zero pos, TimeDisp : dispertion). For arbitrary axis after time T=GausDistib()*0.5/Prob[i] - will occur jump. New direction - any one in the serie, with the same probability. Tested with Blume model (qh_relax_Hn) - gives the same results when TimeDisp=1");
   namepar[i+17]=strdup(" Teta1     ");valpar[i+17]=5; 	helppar[i+17]=strdup("Teta Angle of 1 axes ");
   namepar[i+18]=strdup(" Phi1      ");valpar[i+18]=0;	helppar[i+18]=strdup("Phi  Angle of 1 axes ");
   namepar[i+19]=strdup(" Prob1     ");valpar[i+19]=0; 	helppar[i+19]=strdup("Probability of axis in Blume treatment ");
   namepar[i+20]=strdup(" Teta2     ");valpar[i+20]=5; 	helppar[i+20]=strdup("");
   namepar[i+21]=strdup(" Phi2      ");valpar[i+21]=0;	helppar[i+21]=strdup("");
   namepar[i+22]=strdup(" Prob2     ");valpar[i+22]=0; 	helppar[i+22]=strdup("");
   namepar[i+23]=strdup(" Teta3     ");valpar[i+23]=5; 	helppar[i+23]=strdup("");
   namepar[i+24]=strdup(" Phi3      ");valpar[i+24]=0;	helppar[i+24]=strdup("");
   namepar[i+25]=strdup(" Prob3     ");valpar[i+25]=0; 	helppar[i+25]=strdup("");
   namepar[i+26]=strdup(" Teta4     ");valpar[i+26]=5; 	helppar[i+26]=strdup("");
   namepar[i+27]=strdup(" Phi4      ");valpar[i+27]=0;	helppar[i+27]=strdup("");
   namepar[i+28]=strdup(" Prob4     ");valpar[i+28]=0; 	helppar[i+28]=strdup("");
   namepar[i+29]=strdup(" Teta5     ");valpar[i+29]=5; 	helppar[i+29]=strdup("");
   namepar[i+30]=strdup(" Phi5      ");valpar[i+30]=0;	helppar[i+30]=strdup("");
   namepar[i+31]=strdup(" Prob5     ");valpar[i+31]=0; 	helppar[i+31]=strdup("");
   namepar[i+32]=strdup(" Teta6     ");valpar[i+32]=5; 	helppar[i+32]=strdup("");
   namepar[i+33]=strdup(" Phi6      ");valpar[i+33]=0;	helppar[i+33]=strdup("");
   namepar[i+34]=strdup(" Prob6     ");valpar[i+34]=0; 	helppar[i+34]=strdup("");
   namepar[i+35]=strdup(" Teta7     ");valpar[i+35]=5; 	helppar[i+35]=strdup("");
   namepar[i+36]=strdup(" Phi7      ");valpar[i+36]=0;	helppar[i+36]=strdup("");
   namepar[i+37]=strdup(" Prob7     ");valpar[i+37]=0; 	helppar[i+37]=strdup("");
   namepar[i+38]=strdup(" Teta8     ");valpar[i+38]=5; 	helppar[i+38]=strdup("");
   namepar[i+39]=strdup(" Phi8      ");valpar[i+39]=0;	helppar[i+39]=strdup("");
   namepar[i+40]=strdup(" Prob8     ");valpar[i+40]=0; 	helppar[i+40]=strdup("");

   namepar[i+41]=strdup("EigenAcc "); valpar[i+41]=0;	 helppar[i+41]=strdup("  What method will be used for integration: 1 - will be used eigen acclerated method, 0 - will be used direct integration. For method 1 - parameters used: NumToAver - num of different series of ham will be generated; NTimeAver - number of times each generated serie will be shifted by initial time; EndTAver - maximum time shift for serie;; For method 0 : parameters are used - : NumToAver - num of different series of ham will be generated; OutMeanAmp - to output mean amplitude generated (spectr - is fourie transform of this amplitude); DiscrNum - number of discretization time of Ham calculation - have to be of the order - 10000 - the higher value, the more precies result ( to some extent )");
   namepar[i+42]=strdup("OutMeanAmp");valpar[i+42]=0;	 helppar[i+42]=strdup("  1 - to output the result mean amplitude, before fourie transformation ");
   namepar[i+43]=strdup("DiscrNum "); valpar[i+43]=1e4;helppar[i+43]=strdup("  Number of time steps to make for Amplitude construction, have to be of the order 10000 - the higher - the better ( but in some moment calculation can become unstable ");

   NumPar=i+43;
  };
void   FAZerQsHRelaxRandJump::ClcBase(double *par,double *Xval,double *Res)
  {
//   RelaxAverageStruct Aver;
   int N=par[24],k;
   VecCl axPhi(N),axTeta(N),axProb(N);
   for (k=1;k<=N;k++) {axTeta[k]=par[26+(k-1)*3];axPhi[k]=par[27+(k-1)*3];axProb[k]=par[28+(k-1)*3];}

    double H=par[3],QS=par[4],IS=par[5],W=par[6],
           Teta=par[7],Phi=par[8],Eta=par[9],OutPhi=par[10],Time=par[11],
           NumToAver=par[12],NTimeAver=par[13],EndTAver=par[14],TimeTeta=par[15],
           SigTimeTeta=par[16],AnisTeta=par[17],
           TimePhi=par[18],SigTimePhi=par[19],
           AnisPhi=par[20],NoPhi=par[21],HmulCos=par[22],AnisPower=par[23]; //Ground=par[1],Intencity=par[2],
    double timeDisp=par[25];
    double EigenMethod=par[50],OutAmp=par[51],HamDiscr=par[52];


   Ref<RndAngleGenerator> ang_t=
     new RndAngleGenerator(
//          (new RndBoltsmanSinPower(AnisTeta,AnisPower))->Init(-180,180,360,1e-5),
          (new RndBoltsmanSinPower(AnisTeta,AnisPower))->Init(-90,90,180,1e-5),
        new RndTimeGenerator((new RndGaus(TimeTeta,SigTimeTeta))->Init(max<double>(TimeTeta-4*SigTimeTeta,0),TimeTeta+4*SigTimeTeta,100,1e-5),0,Time));
   Ref<Rnd2AngleGenerator_Base> ang;
   if (!NoPhi) 
     ang=new Rnd2AngleGenerator(ang_t,
      new RndAngleGenerator(
//          (new RndBoltsmanSinPower(AnisTeta,AnisPower))->Init(-180,180,360,1e-5),
          (new RndBoltsmanSinPower(AnisPhi,AnisPower))->Init(-90,90,180,1e-5),
        new RndTimeGenerator((new RndGaus(TimePhi,SigTimePhi))->Init(max<double>(TimePhi-4*SigTimePhi,0),TimePhi+4*SigTimePhi,100,1e-5),0,Time)));
   else 
     ang=new Rnd2AngleGenerator_FixPhi(ang_t,0);

   Ref<Rnd2AngleAxGenerator> ang_ax=
     new Rnd2AngleAxGenerator(
      new RndBlumeNumGenerator(axProb,timeDisp,0,Time),
      ang,axTeta,axPhi);


   Ref<HamTimeAverageRndWalk> hams=new HamTimeAverageRndWalk(
    
    ang_ax,new Ham_Qs_Eiler(Teta,Phi,H,QS,Eta,IS,W),0,HmulCos,OutPhi);
   if (EigenMethod)
   {
    ContiniousRelaxationAverageSolver(
      new ContiniousRelaxationSolver(
          NULL,new ContiniousRelaxationStepMultiSolver(
            new TimeShiftGenerator(NTimeAver,EndTAver),
            new ContiniousRelaxationStepSolver())),
            hams,NumToAver).Solve(Xval,Res);
   }
   else
     ContiniousRelaxationAverageSolver_DirectMatrMul(hams,NumToAver,HamDiscr,OutAmp)
      .Solve(Xval,Res);



  };




//  ================================================
//  ================ Relaxation Mean Field   =======
//  ================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerQsHRelaxMeanField(){return new FAZerQsHRelaxMeanField();}
static ClassDesc FAZerQsHRelaxMeanField_ClassDesc(typeid(FAZerQsHRelaxMeanField),"qh_relax_mean_field",1,"FAZer",Dummy_FAZerQsHRelaxMeanField);
#else
static ClassDesc FAZerQsHRelaxMeanField_ClassDesc(typeid(FAZerQsHRelaxMeanField),"qh_relax_mean_field",1,"FAZer",create<FAZerQsHRelaxMeanField>);
#endif
FAZerQsHRelaxMeanField::FAZerQsHRelaxMeanField()
  {
   name=strdup("qh_relax_mean_field");
   //NumPar=15;
   int i=StandardParams::H_Qs_eiler(namepar,valpar,helppar,1);

	 helppar[0 ]=strdup("For 2-D very fast relaxation oscillators over Z axis. Calculation of spectra of Fe under H relaxation . Spectra calulated as a mean spectra averaged over different angels H_Teta. H(teta)=mean(cos(teta))*H0. H_Prob(teta)= exp(-AnisCoef*(func(teta))^AnisPower) ; func1(x)=sin(x) if AnisPower>0 func1(x)=cos(x) if AnisPower<0. If AnisPower==(int)AnisPower then func(x)=func1(x) else func(x)=fabs(func(x)). mean(cos(teta)) - is calculated under assumption that we have 2-D relaxation from teta to -teta angle by harmonic oscillator ( rough estimation works for small angles only ). Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta params).   ");
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");

   namepar[i+1]=strdup("    OutPhi     ");  valpar[i+1]=1;	helppar[i+1]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2]=strdup("   NumHTetaInt ");  valpar[i+2]=30;	helppar[i+2]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");
   namepar[i+3]=strdup("  Coef2D_3D    ");  valpar[i+3]=1;	helppar[i+3]=strdup("Coef determines if 2D Mean_Oscill_Cos(teta) to use or simple 3D cos(teta); X=(1-Coef2D_3D)*2D_Cos(teta)+Coef2D_3D*3D_Cos(teta). 0 - 2D model, 1 - 3D model");
   namepar[i+4]=strdup("   Frequency     ");valpar[i+4]=1;	helppar[i+4]=strdup("Now determines frequency of oscillations - does not have any effect, still leave it at 1.");
   namepar[i+5]=strdup("   AnisTeta    ");  valpar[i+5]=5;	helppar[i+5]=strdup("Anisotropy coef for teta angle prob calculations (look param description for AnisPower)");
   namepar[i+6]=strdup("   AnisPower   ");  valpar[i+6]=-1; helppar[i+6]=strdup(" AnisPower used for Teta-Phi relaxation calculation. H_Prob(teta)= exp(-AnisCoef*(func(teta+AnisTeta2))^AnisPower) ; func1(x)=sin(x) if AnisPower>0 func1(x)=cos(x) if AnisPower<0. If AnisPower==(int)AnisPower then func(x)=func1(x) else func(x)=fabs(func(x)). For 2-d magnetic have to use [-1].");
   namepar[i+7]=strdup("   AnisTeta2   ");  valpar[i+7]=-1; helppar[i+7]=strdup("Anisotropy coef for teta angle prob calculations  ");
   NumPar=i+7;

  };
void   FAZerQsHRelaxMeanField::ClcBase(double *par,double *Xval,double *Res)
  {
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Phi=par[8],Eta=par[9],
          OutPhi=par[10],NumHTetaInt=par[11],Coef2D_3D=par[12],Frequency=par[13],
          AnisTeta=par[14],AnisPower=par[15],AnisTeta2=par[16];
   //BoltzmanTetaAverage_2D3D_Solver(new Ham_Qs_Eiler(Teta,Phi,H,Qs,Eta,IS,W),
   //  new TetaGenerator(new RndBoltsmanSinPower(AnisTeta,AnisPower),NumHTetaInt),
   //  new MeanAngleConstructor(Frequency,Coef2D_3D),OutPhi).Solve(Xval,Res);
   TetaAverageSolver(new Ham_Qs_Eiler(Teta,Phi,H,Qs,Eta,IS,W),
     new TetaGenerator(new FieldBoltsmanSinPower(AnisTeta,AnisPower,AnisTeta2),NumHTetaInt),
     new MeanAngleConstructor_2D_3D(Frequency,Coef2D_3D),OutPhi).Solve(Xval,Res);
   //BoltzmanTetaAverage_2D3D_Solver(new Ham_Qs_Eiler(Teta,Phi,H,Qs,Eta,IS,W),
   //  new TetaGenerator(new RndGaus(AnisTeta2,AnisPower),NumHTetaInt),
   //  new MeanAngleConstructor(Frequency,Coef2D_3D),OutPhi).Solve(Xval,Res);


   //double gr=par[1],in=par[2];
   //for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
  };


//  =======================================================
//  ================    Magnon Mean Field  Std      =======
//  =======================================================
//#ifndef WCPP_NET
//DescribedClass * Dummy_FAZerHQsMagnonMeanFieldStd(){return new FAZerHQsMagnonMeanFieldStd();}
//static ClassDesc FAZerHQsMagnonMeanFieldStd_ClassDesc(typeid(FAZerHQsMagnonMeanFieldStd),"hq_magnon_mean_field_std",1,"FAZer",FAZerHQsMagnonMeanFieldStd);
//#else
static ClassDesc FAZerHQsMagnonMeanFieldStd_ClassDesc(typeid(FAZerHQsMagnonMeanFieldStd),"hq_magnon_mean_field_std",1,"FAZer",create<FAZerHQsMagnonMeanFieldStd>);
//#endif
FAZerHQsMagnonMeanFieldStd::FAZerHQsMagnonMeanFieldStd()
  {
   name=strdup("hq_magnon_mean_field_std");
   //NumPar=15;
   int i=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);

	 helppar[0 ]=strdup(" Calculation of spectra of Fe under H relaxation, a magnon model . Spectra calulated as a mean spectra averaged over different angels H_Teta. H(teta)=mean(cos(teta))*H0. H_Prob(teta)= func(teta)^(1.5*Epow-1)*fabs(sin(teta))/(exp(Beta/Temp*(func(teta))^Epow)+Statistic) ; func(x)=1-cos(x) . This is when MinEn>0; If MeanEn<0 - H_Prob(teta)= teta^5/(exp(Beta/Temp*teta^4)+Statistic). Mean(cos(teta)) - is calculated under assumption that we have fast oscilating, long live magnons. Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta,Gamma params). ");
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");

   namepar[i+1]=strdup("    OutPhi     ");  valpar[i+1]=1;	helppar[i+1]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2]=strdup("   NumHTetaInt ");  valpar[i+2]=30;	helppar[i+2]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");

   namepar[i+3]=strdup("  JS_const ");valpar[i+3]=1000;	helppar[i+3]=strdup(" In the formulas named A. Coef in the exponent, divided on the temperature.");
   namepar[i+4]=strdup("  Temp     ");valpar[i+4]=4;	  helppar[i+4]=strdup(" Temperature");
   namepar[i+5]=strdup("  Epow     ");valpar[i+5]=2;	  helppar[i+5]=strdup(" In the formulas named as Alpha, According to the formulas have to be fixed = 2; When MinEn is negative - parameter is not used");
   namepar[i+6]=strdup("  MinEn    ");valpar[i+6]=1e-10;helppar[i+6]=strdup(" For lower angles - used estimation 0; If it is negative - used formula hw=q^2=Teta^4 else hw=q^2=(1-cos(Teta))^2 ");
   namepar[i+7]=strdup(" Statistic ");valpar[i+7]=-1;   helppar[i+7]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   namepar[i+8]=strdup(" QCoef0    ");valpar[i+8]= 0;   helppar[i+8]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+9]=strdup(" QCoef1    ");valpar[i+9]= 0;   helppar[i+9]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+10]=strdup("  T_pow    ");valpar[i+10]= 0.5;   helppar[i+10]=strdup("Fluctuating energy E(teta)~k*T^Tpow  Tpow=0.5 ( from landau? for fluctuations ?) from the experiments with isotrope samples ");
   NumPar=i+10;

  };
void   FAZerHQsMagnonMeanFieldStd::ClcBase(double *par,double *Xval,double *Res)
  {
   //double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Phi=par[8],Eta=par[9],
   //       OutPhi=par[10],NumHTetaInt=par[11],Beta=par[12],Temp=par[13],
   //       Epow=par[14],MinEn=par[15],Statistic=par[16];
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10],
          OutPhi=par[11],NumHTetaInt=par[12],Beta=par[13],Temp=par[14],
          Epow=par[15],MinEn=par[16],Statistic=par[17],QCoef0=par[18],QCoef1=par[19],
          Tpow=par[20];

//   BoltzmanTetaAverage_2D3D_Solver(new Ham_Qs_Eiler(Teta,Phi,H,Qs,Eta,IS,W),
   TetaAverageSolver(new Ham_Qs_Gamma(HamData(Teta,Phi,H,Qs,Eta,IS,W),Gamma),
//     new TetaGenerator(new RndBoltsmanHamSol_Correlated(Beta,Temp,Epow,MinEn,NumNeib),NumHTetaInt),
     new TetaGenerator(new MagnonFieldStd(Beta,Temp,Epow,MinEn,Statistic,QCoef1,QCoef0,Tpow),NumHTetaInt),
     new MeanAngleConstructor,OutPhi).Solve(Xval,Res);
  };

//  =======================================================
//  ================ Magnon Mean Field  Optic       =======
//  =======================================================
//#ifndef WCPP_NET
//DescribedClass * Dummy_FAZerHQsMagnonMeanFieldOptic(){return new FAZerHQsMagnonMeanFieldOptic();}
//static ClassDesc FAZerHQsMagnonMeanFieldOptic_ClassDesc(typeid(FAZerHQsMagnonMeanFieldOptic),"hq_magnon_mean_field_optic",1,"FAZer",FAZerHQsMagnonMeanFieldOptic);
//#else
static ClassDesc FAZerHQsMagnonMeanFieldOptic_ClassDesc(typeid(FAZerHQsMagnonMeanFieldOptic),"hq_magnon_mean_field_optic",1,"FAZer",create<FAZerHQsMagnonMeanFieldOptic>);
//#endif
FAZerHQsMagnonMeanFieldOptic::FAZerHQsMagnonMeanFieldOptic()
  {
   name=strdup("hq_magnon_mean_field_optic");
   //NumPar=15;
   int i=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);

	 helppar[0 ]=strdup(" Calculation of spectra of Fe under H relaxation, a magnon model . Spectra calulated as a mean spectra averaged over different angels H_Teta. H(teta)=mean(cos(teta))*H0. H_Prob(teta)= func(teta)^(1.5*Epow-1)*fabs(sin(teta))/(exp(Beta/Temp*(func(teta))^Epow)+Statistic) ; func(x)=1-cos(x) . This is when MinEn>0; If MeanEn<0 - H_Prob(teta)= teta^5/(exp(Beta/Temp*teta^4)+Statistic). Mean(cos(teta)) - is calculated under assumption that we have fast oscilating, long live magnons. Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta,Gamma params). ");
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");

   namepar[i+1]=strdup("    OutPhi     ");  valpar[i+1]=1;	helppar[i+1]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2]=strdup("   NumHTetaInt ");  valpar[i+2]=30;	helppar[i+2]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");

   namepar[i+3]=strdup("  JS_const ");valpar[i+3]=1000;	helppar[i+3]=strdup(" In the formulas named A. Coef in the exponent, divided on the temperature.");
   namepar[i+4]=strdup("  Temp     ");valpar[i+4]=4;	  helppar[i+4]=strdup(" Temperature");
   namepar[i+5]=strdup("  Tpow     ");valpar[i+5]=0.5;  helppar[i+5]=strdup(" ");
   namepar[i+6]=strdup("  NormCoef ");valpar[i+6]=1;    helppar[i+6]=strdup(" For lower angles - used estimation 0; If it is negative - used formula hw=q^2=Teta^4 else hw=q^2=(1-cos(Teta))^2 ");
   namepar[i+7]=strdup(" Statistic ");valpar[i+7]=-1;   helppar[i+7]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   namepar[i+8]=strdup(" QCoef0    ");valpar[i+8]= 0;   helppar[i+8]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+9]=strdup(" QCoef1    ");valpar[i+9]= 0;   helppar[i+9]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+10]=strdup(" Eanis0    ");valpar[i+10]= 0;   helppar[i+10]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+11]=strdup(" Eanis1    ");valpar[i+11]= 0;   helppar[i+11]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   NumPar=i+11;

  };
void   FAZerHQsMagnonMeanFieldOptic::ClcBase(double *par,double *Xval,double *Res)
  {
   //double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Phi=par[8],Eta=par[9],
   //       OutPhi=par[10],NumHTetaInt=par[11],Beta=par[12],Temp=par[13],
   //       Epow=par[14],MinEn=par[15],Statistic=par[16];
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10],
          OutPhi=par[11],NumHTetaInt=par[12];
   double JS_const=par[13],Temp=par[14],Tpow=par[15],NormCoef=par[16],Statistic=par[17],
          QCoef0=par[18],QCoef1=par[19],ECoef0=par[20],ECoef1=par[21];

   TetaAverageSolver(new Ham_Qs_Gamma(HamData(Teta,Phi,H,Qs,Eta,IS,W),Gamma),
     new TetaGenerator(
       new MagnonFieldOptic(JS_const,Temp,Statistic,Tpow,QCoef0,QCoef1,ECoef0,ECoef1),NumHTetaInt,NormCoef),
     new MeanAngleConstructor,OutPhi).Solve(Xval,Res);
  };

//  =======================================================
//  ================    Magnon Mean Field    Accust     ===
//  =======================================================
//#ifndef WCPP_NET
//DescribedClass * Dummy_FAZerHQsMagnonMeanFieldAccus(){return new FAZerHQsMagnonMeanFieldAccus();}
//static ClassDesc FAZerHQsMagnonMeanFieldAccus_ClassDesc(typeid(FAZerHQsMagnonMeanFieldAccus),"hq_magnon_mean_field_accus",1,"FAZer",FAZerHQsMagnonMeanFieldAccus);
//#else
static ClassDesc FAZerHQsMagnonMeanFieldAccus_ClassDesc(typeid(FAZerHQsMagnonMeanFieldAccus),"hq_magnon_mean_field_accus",1,"FAZer",create<FAZerHQsMagnonMeanFieldAccus>);
//#endif
FAZerHQsMagnonMeanFieldAccus::FAZerHQsMagnonMeanFieldAccus()
  {
   name=strdup("hq_magnon_mean_field_accus");
   //NumPar=15;
   int i=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);

	 helppar[0 ]=strdup(" Calculation of spectra of Fe under H relaxation, a magnon model . Spectra calulated as a mean spectra averaged over different angels H_Teta. H(teta)=mean(cos(teta))*H0. H_Prob(teta)= func(teta)^(1.5*Epow-1)*fabs(sin(teta))/(exp(Beta/Temp*(func(teta))^Epow)+Statistic) ; func(x)=1-cos(x) . This is when MinEn>0; If MeanEn<0 - H_Prob(teta)= teta^5/(exp(Beta/Temp*teta^4)+Statistic). Mean(cos(teta)) - is calculated under assumption that we have fast oscilating, long live magnons. Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta,Gamma params). ");
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");
//[10]

   namepar[i+1]=strdup("    OutPhi     ");  valpar[i+1]=1;	helppar[i+1]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2]=strdup("   NumHTetaInt ");  valpar[i+2]=30;	helppar[i+2]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");

   //namepar[i+3 ]=strdup("IntOpticCoef");  valpar[i+3 ]=1;     helppar[i+3 ]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");

   namepar[i+3 ]=strdup("  JS_const  ");  valpar[i+3 ]=1000;  helppar[i+3 ]=strdup(" In the formulas named A. Coef in the exponent, divided on the temperature.");
   namepar[i+4 ]=strdup("   Temp     ");  valpar[i+4 ]=4;	    helppar[i+4 ]=strdup(" Temperature");
   namepar[i+5 ]=strdup("   Tpow     ");  valpar[i+5 ]=0.5;   helppar[i+5 ]=strdup(" ");
   namepar[i+6 ]=strdup(" DisordCoef ");  valpar[i+6 ]=1;     helppar[i+6 ]=strdup(" For lower angles - used estimation 0; If it is negative - used formula hw=q^2=Teta^4 else hw=q^2=(1-cos(Teta))^2 ");
   namepar[i+7 ]=strdup("  Statistic ");  valpar[i+7 ]=-1;    helppar[i+7 ]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   namepar[i+8 ]=strdup("  QCoef0    ");  valpar[i+8 ]= 0;    helppar[i+8 ]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+9 ]=strdup("  QCoef1    ");  valpar[i+9 ]= 0;    helppar[i+9 ]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+10]=strdup("  Eanis0    ");  valpar[i+10]= 0;    helppar[i+10]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+11]=strdup("  Eanis1    ");  valpar[i+11]= 0;    helppar[i+11]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+12]=strdup("  EMax      ");  valpar[i+12]=100;   helppar[i+12]=strdup(" In the formulas named A. Coef in the exponent, divided on the temperature.");
   namepar[i+13]=strdup("  QMax      ");  valpar[i+13]=0.5;   helppar[i+13]=strdup(" ");
   namepar[i+14]=strdup("  QMin      ");  valpar[i+14]=0.5;   helppar[i+14]=strdup(" ");
   namepar[i+15]=strdup("  dCosMax   ");  valpar[i+15]=1;     helppar[i+15]=strdup(" For lower angles - used estimation 0; If it is negative - used formula hw=q^2=Teta^4 else hw=q^2=(1-cos(Teta))^2 ");
   //namepar[i+16]=strdup(" OStatistic ");  valpar[i+16]=-1;    helppar[i+16]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   //namepar[i+17]=strdup(" OQCoef0    ");  valpar[i+17]= 0;    helppar[i+17]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+18]=strdup(" OQCoef1    ");  valpar[i+18]= 0;    helppar[i+18]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+19]=strdup(" OEanis0    ");  valpar[i+19]= 0;    helppar[i+19]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+20]=strdup(" OEanis1    ");  valpar[i+20]= 0;    helppar[i+20]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   NumPar=i+15;

  };
void   FAZerHQsMagnonMeanFieldAccus::ClcBase(double *par,double *Xval,double *Res)
  {
   //double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Phi=par[8],Eta=par[9],
   //       OutPhi=par[10],NumHTetaInt=par[11],Beta=par[12],Temp=par[13],
   //       Epow=par[14],MinEn=par[15],Statistic=par[16];
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10],
          OutPhi=par[11],NumHTetaInt=par[12];
   //double IntOpticCoef=par[13];
   double JS_const=par[13],Temp=par[14],Tpow=par[15],DisordCoef=par[16],Statistic=par[17],
          QCoef0=par[18],QCoef1=par[19],ECoef0=par[20],ECoef1=par[21];
   double EMax=par[22],QMax=par[23],QMin=par[24],dCosMax=par[25];
   //double OJS_const=par[23],OTpow=par[24],OMinEn=par[25],OStatistic=par[26],
   //       OQCoef0=par[27],OQCoef1=par[28],OECoef0=par[29],OECoef1=par[30];

   Ref<MagnonMeanField> field=new MagnonFieldAccustic(JS_const,Temp,Statistic,
     Tpow,QCoef0,QCoef1,ECoef0,ECoef1,EMax,QMax,QMin,dCosMax);
   Ref<HamData> HamOrd=   new Ham_Qs_Gamma(HamData(Teta,Phi,H,Qs,Eta,IS,W),Gamma);
   Ref<HamData> HamDisOrd=new Ham_Qs_Gamma(HamData(0,0,0,Qs,0,IS,W),0);
   
   TetaAverageSolver_Advanced(HamOrd,
     new TetaGenerator(field,NumHTetaInt,1),
     new MeanAngleConstructor(),OutPhi,HamDisOrd,DisordCoef).Solve(Xval,Res);
   //TetaAverageSolver(HamOrd,
   //  new TetaGenerator(field,NumHTetaInt,NormOrdCoef),
   //  new MeanAngleConstructor(),OutPhi).Solve(Xval,Res);
  };


//  =======================================================
//  ================    Magnon Mean Field    Paper     ===
//  =======================================================
//#ifndef WCPP_NET
//DescribedClass * Dummy_FAZerHQsMagnonMeanFieldPaper(){return new FAZerHQsMagnonMeanFieldPaper();}
//static ClassDesc FAZerHQsMagnonMeanFieldPaper_ClassDesc(typeid(FAZerHQsMagnonMeanFieldPaper),"hq_magnon_mean_field_paper",1,"FAZer",FAZerHQsMagnonMeanFieldPaper);
//#else
static ClassDesc FAZerHQsMagnonMeanFieldPaper_ClassDesc(typeid(FAZerHQsMagnonMeanFieldPaper),"hq_magnon_mean_field_paper",1,"FAZer",create<FAZerHQsMagnonMeanFieldPaper>);
//#endif
FAZerHQsMagnonMeanFieldPaper::FAZerHQsMagnonMeanFieldPaper()
  {
   name=strdup("hq_magnon_mean_field_paper");
   //NumPar=15;
   int i=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);

	 helppar[0 ]=strdup(" Calculation of spectra of Fe under H relaxation, a magnon model . Spectra calulated as a mean spectra averaged over different angels H_Teta. H(teta)=mean(cos(teta))*H0. H_Prob(teta)= func(teta)^(1.5*Epow-1)*fabs(sin(teta))/(exp(Beta/Temp*(func(teta))^Epow)+Statistic) ; func(x)=1-cos(x) . This is when MinEn>0; If MeanEn<0 - H_Prob(teta)= teta^5/(exp(Beta/Temp*teta^4)+Statistic). Mean(cos(teta)) - is calculated under assumption that we have fast oscilating, long live magnons. Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta,Gamma params). ");
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");
//[10]

   namepar[i+1]=strdup("    OutPhi     ");  valpar[i+1]=1;	helppar[i+1]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2]=strdup("   NumHTetaInt ");  valpar[i+2]=30;	helppar[i+2]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");

   //namepar[i+3 ]=strdup("IntOpticCoef");  valpar[i+3 ]=1;     helppar[i+3 ]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");

   namepar[i+3 ]=strdup("  JS_const  ");  valpar[i+3 ]=1000;  helppar[i+3 ]=strdup(" In the formulas named A. Coef in the exponent, divided on the temperature.");
   namepar[i+4 ]=strdup("   Temp     ");  valpar[i+4 ]=4;	    helppar[i+4 ]=strdup(" Temperature");
   namepar[i+5 ]=strdup("   Tpow     ");  valpar[i+5 ]=0.5;   helppar[i+5 ]=strdup(" Not used ");
   namepar[i+6 ]=strdup(" DisordCoef ");  valpar[i+6 ]=1;     helppar[i+6 ]=strdup(" Not used ");
   namepar[i+7 ]=strdup("  Statistic ");  valpar[i+7 ]=-1;    helppar[i+7 ]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   namepar[i+8 ]=strdup("  QCoef0    ");  valpar[i+8 ]= 0;    helppar[i+8 ]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+9 ]=strdup("  QCoef1    ");  valpar[i+9 ]= 0;    helppar[i+9 ]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+10]=strdup("  Eanis0    ");  valpar[i+10]= 0;    helppar[i+10]=strdup("Not used");
   namepar[i+11]=strdup("  Eanis1    ");  valpar[i+11]= 0;    helppar[i+11]=strdup("Not used");
   namepar[i+12]=strdup("  Rconst    ");  valpar[i+12]=100;   helppar[i+12]=strdup(" In the formulas named R. Coef in the exponent, muled on the temperature.");
   namepar[i+13]=strdup("  QMax      ");  valpar[i+13]=0.5;   helppar[i+13]=strdup(" ");
   namepar[i+14]=strdup("  QMin      ");  valpar[i+14]=0.5;   helppar[i+14]=strdup(" ");
   namepar[i+15]=strdup("  dCosMax   ");  valpar[i+15]=1;     helppar[i+15]=strdup(" Limit order - disorder spectra. Qmin<q<Qbnd=dCosMax/(Rconst*Temp)- ordered spectra Qbnd<q<Qmax - disordered spectra ");
   //namepar[i+16]=strdup(" OStatistic ");  valpar[i+16]=-1;    helppar[i+16]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   //namepar[i+17]=strdup(" OQCoef0    ");  valpar[i+17]= 0;    helppar[i+17]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+18]=strdup(" OQCoef1    ");  valpar[i+18]= 0;    helppar[i+18]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+19]=strdup(" OEanis0    ");  valpar[i+19]= 0;    helppar[i+19]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+20]=strdup(" OEanis1    ");  valpar[i+20]= 0;    helppar[i+20]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   NumPar=i+15;

  };
void   FAZerHQsMagnonMeanFieldPaper::ClcBase(double *par,double *Xval,double *Res)
  {
   //double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Phi=par[8],Eta=par[9],
   //       OutPhi=par[10],NumHTetaInt=par[11],Beta=par[12],Temp=par[13],
   //       Epow=par[14],MinEn=par[15],Statistic=par[16];
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10],
          OutPhi=par[11],NumHTetaInt=par[12];
   //double IntOpticCoef=par[13];
   double JS_const=par[13],Temp=par[14],DisordCoef=par[16],Statistic=par[17],
          QCoef0=par[18],QCoef1=par[19];//,ECoef0=par[20],ECoef1=par[21], Tpow=par[15],
   double Rconst=par[22],QMax=par[23],QMin=par[24],dCosMax=par[25];
   //double OJS_const=par[23],OTpow=par[24],OMinEn=par[25],OStatistic=par[26],
   //       OQCoef0=par[27],OQCoef1=par[28],OECoef0=par[29],OECoef1=par[30];

   Ref<MagnonMeanField> field=new MagnonFieldPaper(JS_const,Temp,Statistic,
     QCoef0,QCoef1,Rconst,QMax,QMin,dCosMax);
   Ref<HamData> HamOrd=   new Ham_Qs_Gamma(HamData(Teta,Phi,H,Qs,Eta,IS,W),Gamma);
   Ref<HamData> HamDisOrd=new Ham_Qs_Gamma(HamData(0,0,0,Qs,0,IS,W),0);
   
   TetaAverageSolver_Advanced(HamOrd,
     new TetaGenerator(field,NumHTetaInt,1),
     new MeanAngleConstructor(),OutPhi,HamDisOrd,DisordCoef).Solve(Xval,Res);
   //TetaAverageSolver(HamOrd,
   //  new TetaGenerator(field,NumHTetaInt,NormOrdCoef),
   //  new MeanAngleConstructor(),OutPhi).Solve(Xval,Res);
  };
//  =======================================================
//  ================    Magnon Mean Field    PaperPlus  ===
//  =======================================================
//#ifndef WCPP_NET
//DescribedClass * Dummy_FAZerHQsMagnonMeanFieldPaperPlus(){return new FAZerHQsMagnonMeanFieldPaperPlus();}
//static ClassDesc FAZerHQsMagnonMeanFieldPaperPlus_ClassDesc(typeid(FAZerHQsMagnonMeanFieldPaperPlus),"hq_magnon_mean_field_paper_plus",1,"FAZer",FAZerHQsMagnonMeanFieldPaperPlus);
//#else
static ClassDesc FAZerHQsMagnonMeanFieldPaperPlus_ClassDesc(typeid(FAZerHQsMagnonMeanFieldPaperPlus),"hq_magnon_mean_field_paper_plus",1,"FAZer",create<FAZerHQsMagnonMeanFieldPaperPlus>);
//#endif
FAZerHQsMagnonMeanFieldPaperPlus::FAZerHQsMagnonMeanFieldPaperPlus()
  {
   name=strdup("hq_magnon_mean_field_paper_plus");
   //NumPar=15;
   int i=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);

	 helppar[0 ]=strdup(" Calculation of spectra of Fe under H relaxation, a magnon model . Spectra calulated as a mean spectra averaged over different angels H_Teta. H(teta)=mean(cos(teta))*H0. H_Prob(teta)= func(teta)^(1.5*Epow-1)*fabs(sin(teta))/(exp(Beta/Temp*(func(teta))^Epow)+Statistic) ; func(x)=1-cos(x) . This is when MinEn>0; If MeanEn<0 - H_Prob(teta)= teta^5/(exp(Beta/Temp*teta^4)+Statistic). Mean(cos(teta)) - is calculated under assumption that we have fast oscilating, long live magnons. Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta,Gamma params). ");
   //namepar[1 ]=strdup("Ground   ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   //namepar[2 ]=strdup("Intencity");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   //namepar[3 ]=strdup("   H     ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   //namepar[4 ]=strdup("   QS    ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   //namepar[5 ]=strdup("   IS    ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   //namepar[6 ]=strdup("   W     ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   //namepar[7 ]=strdup("  Teta   ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   //namepar[8 ]=strdup("  Phi    ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (Fix_X,Fix_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   //namepar[9 ]=strdup("  Eta    ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");
//[10]

   namepar[i+1]=strdup("    OutPhi     ");  valpar[i+1]=1;	helppar[i+1]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2]=strdup("   NumHTetaInt ");  valpar[i+2]=30;	helppar[i+2]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");

   //namepar[i+3 ]=strdup("IntOpticCoef");  valpar[i+3 ]=1;     helppar[i+3 ]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");

   namepar[i+3 ]=strdup("  JS_const  ");  valpar[i+3 ]=1000;  helppar[i+3 ]=strdup(" In the formulas named A. Coef in the exponent, divided on the temperature.");
   namepar[i+4 ]=strdup("   Temp     ");  valpar[i+4 ]=4;	    helppar[i+4 ]=strdup(" Temperature");
   namepar[i+5 ]=strdup("   Tpow     ");  valpar[i+5 ]=0.5;   helppar[i+5 ]=strdup(" Not used ");
   namepar[i+6 ]=strdup(" DisordCoef ");  valpar[i+6 ]=1;     helppar[i+6 ]=strdup(" Not used ");
   namepar[i+7 ]=strdup("  Statistic ");  valpar[i+7 ]=-1;    helppar[i+7 ]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   namepar[i+8 ]=strdup("  QCoef0    ");  valpar[i+8 ]= 0;    helppar[i+8 ]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+9 ]=strdup("  QCoef1    ");  valpar[i+9 ]= 0;    helppar[i+9 ]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+10]=strdup("  Alpha     ");  valpar[i+10]= 0;    helppar[i+10]=strdup("Coef when disorder occur cos(teta_max)=qRTempAlpha");
   namepar[i+11]=strdup("  Eanis1    ");  valpar[i+11]= 0;    helppar[i+11]=strdup("Not used");
   namepar[i+12]=strdup("  Rconst    ");  valpar[i+12]=100;   helppar[i+12]=strdup(" In the formulas named R. Coef in the exponent, muled on the temperature.");
   namepar[i+13]=strdup("  QMax      ");  valpar[i+13]=0.5;   helppar[i+13]=strdup(" ");
   namepar[i+14]=strdup("  QMin      ");  valpar[i+14]=0.5;   helppar[i+14]=strdup(" ");
   namepar[i+15]=strdup("  dCosMax   ");  valpar[i+15]=1;     helppar[i+15]=strdup(" Limit order - disorder spectra. Qmin<q<Qbnd=dCosMax/(Rconst*Temp)- ordered spectra Qbnd<q<Qmax - disordered spectra ");
   //namepar[i+16]=strdup(" OStatistic ");  valpar[i+16]=-1;    helppar[i+16]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   //namepar[i+17]=strdup(" OQCoef0    ");  valpar[i+17]= 0;    helppar[i+17]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+18]=strdup(" OQCoef1    ");  valpar[i+18]= 0;    helppar[i+18]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+19]=strdup(" OEanis0    ");  valpar[i+19]= 0;    helppar[i+19]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+20]=strdup(" OEanis1    ");  valpar[i+20]= 0;    helppar[i+20]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   NumPar=i+15;

  };
void   FAZerHQsMagnonMeanFieldPaperPlus::ClcBase(double *par,double *Xval,double *Res)
  {
   //double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Phi=par[8],Eta=par[9],
   //       OutPhi=par[10],NumHTetaInt=par[11],Beta=par[12],Temp=par[13],
   //       Epow=par[14],MinEn=par[15],Statistic=par[16];
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10],
          OutPhi=par[11],NumHTetaInt=par[12];
   //double IntOpticCoef=par[13];
   double JS_const=par[13],Temp=par[14],DisordCoef=par[16],Statistic=par[17],
          QCoef0=par[18],QCoef1=par[19],Alph=par[20];//,ECoef1=par[21],Tpow=par[15]
   double Rconst=par[22],QMax=par[23],QMin=par[24],dCosMax=par[25];
   //double OJS_const=par[23],OTpow=par[24],OMinEn=par[25],OStatistic=par[26],
   //       OQCoef0=par[27],OQCoef1=par[28],OECoef0=par[29],OECoef1=par[30];

   Ref<MagnonMeanField> field=new MagnonFieldPaperPlus(JS_const,Temp,Statistic,
     QCoef0,QCoef1,Rconst,QMax,QMin,dCosMax,Alph);
   Ref<HamData> HamOrd=   new Ham_Qs_Gamma(HamData(Teta,Phi,H,Qs,Eta,IS,W),Gamma);
   Ref<HamData> HamDisOrd=new Ham_Qs_Gamma(HamData(0,0,0,Qs,0,IS,W),0);
   
   TetaAverageSolver_Advanced(HamOrd,
     new TetaGenerator(field,NumHTetaInt,1),
     new MeanAngleConstructor(),OutPhi,HamDisOrd,DisordCoef).Solve(Xval,Res);
   //TetaAverageSolver(HamOrd,
   //  new TetaGenerator(field,NumHTetaInt,NormOrdCoef),
   //  new MeanAngleConstructor(),OutPhi).Solve(Xval,Res);
  };

//  =======================================================
//  ================    Magnon Mean Field    Paper9     ===
//  =======================================================
//#ifndef WCPP_NET
//DescribedClass * Dummy_FAZerHQsMagnonMeanFieldPaper9(){return new FAZerHQsMagnonMeanFieldPaper9();}
//static ClassDesc FAZerHQsMagnonMeanFieldPaper9_ClassDesc(typeid(FAZerHQsMagnonMeanFieldPaper9),"hq_magnon_mean_field_paper9",1,"FAZer",FAZerHQsMagnonMeanFieldPaper9);
//#else
static ClassDesc FAZerHQsMagnonMeanFieldPaper9_ClassDesc(typeid(FAZerHQsMagnonMeanFieldPaper9),"hq_magnon_mean_field_paper9",1,"FAZer",create<FAZerHQsMagnonMeanFieldPaper9>);
//#endif
FAZerHQsMagnonMeanFieldPaper9::FAZerHQsMagnonMeanFieldPaper9()
  {
   name=strdup("hq_magnon_mean_field_paper9");
   //NumPar=15;
   int i=StandardParams::H_Qs_gamma(namepar,valpar,helppar,1);


	 helppar[0 ]=strdup(" Calculation of spectra of Fe under H relaxation, a magnon model . Spectra calulated as a mean spectra averaged over different angels H_Teta. H(teta)=mean(cos(teta))*H0. H_Prob(cos(teta))= func(teta)^(1.5*Epow-1)*fabs(sin(teta))/(exp(Beta/Temp*(func(teta))^Epow)+Statistic) ; func(x)=1-cos(x) . This is when MinEn>0; If MeanEn<0 - H_Prob(teta)= teta^5/(exp(Beta/Temp*teta^4)+Statistic). Mean(cos(teta)) - is calculated under assumption that we have fast oscilating, long live magnons. Supposed that we have fixed Fix_axis - system coordinat, in this system QS - is turned (Phi,Teta,Gamma params). ");

   namepar[i+1]=strdup("    OutPhi     ");  valpar[i+1]=1;	helppar[i+1]=strdup("Program will make output for all generated Hmag angles (calculated in Fix - axis system) to stdout. Check -  how H is relaxing in model");
   namepar[i+2]=strdup("   NumHTetaInt ");  valpar[i+2]=30;	helppar[i+2]=strdup("Number of H_Teta used in mean spectra calculation ( for averaging ). From the probability of Teta angle is constructed integral probability - Prob(Teta<Teta0). After that for the same Prob_step are calculated angle steps. Spectras are calculated for this angles.");


   namepar[i+3 ]=strdup("  JS_const  ");  valpar[i+3 ]=1000;  helppar[i+3 ]=strdup(" In the formulas named A. Coef in the exponent, divided on the temperature.");
   namepar[i+4 ]=strdup("   Temp     ");  valpar[i+4 ]=4;	    helppar[i+4 ]=strdup(" Temperature");
   namepar[i+5 ]=strdup("   Tpow     ");  valpar[i+5 ]=0.5;   helppar[i+5 ]=strdup(" Not used ");


   namepar[i+6 ]=strdup("  Statistic ");  valpar[i+6 ]=-1;    helppar[i+6 ]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   namepar[i+7 ]=strdup("  QCoef0    ");  valpar[i+7 ]= 0;    helppar[i+7 ]=strdup("Shift of the magnon spectra E_magnon=QCoef2*q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+8 ]=strdup("  QCoef1    ");  valpar[i+8 ]= 0;    helppar[i+8 ]=strdup("Shift of the magnon spectra E_magnon=QCoef2*q^2+QCoef1*fabs(q)+QCoef0 ");
   namepar[i+9 ]=strdup("  QCoef2    ");  valpar[i+9 ]= 0;    helppar[i+9 ]=strdup("Shift of the magnon spectra E_magnon=QCoef2*q^2+QCoef1*fabs(q)+QCoef0 ");

   namepar[i+10]=strdup(" QCoef2_R2  ");  valpar[i+10]= 0;    helppar[i+10]=strdup("QCoef2/R^2 coef");

   namepar[i+11]=strdup("  DisorC    ");  valpar[i+11]= 0;    helppar[i+11]=strdup("Coef to mul disordered int part");

   namepar[i+12]=strdup("  Rconst    ");  valpar[i+12]=100;   helppar[i+12]=strdup(" In the formulas named R. R~1/(2piTcrit)");
   namepar[i+13]=strdup("  QMax      ");  valpar[i+13]=0.5;   helppar[i+13]=strdup(" Have to be 2Pi");
   namepar[i+14]=strdup("  QMin      ");  valpar[i+14]=0.5;   helppar[i+14]=strdup(" ");
   namepar[i+15]=strdup("  dCosMax   ");  valpar[i+15]=1;     helppar[i+15]=strdup(" Limit order - disorder spectra. Qmin<q<Qbnd=dCosMax/(Rconst*Temp)- ordered spectra Qbnd<q<Qmax - disordered spectra ");
   //namepar[i+16]=strdup(" OStatistic ");  valpar[i+16]=-1;    helppar[i+16]=strdup("Statistic to be used -1 - Bose, 0 - Boltsman, 1 - Fermi");
   //namepar[i+17]=strdup(" OQCoef0    ");  valpar[i+17]= 0;    helppar[i+17]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+18]=strdup(" OQCoef1    ");  valpar[i+18]= 0;    helppar[i+18]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+19]=strdup(" OEanis0    ");  valpar[i+19]= 0;    helppar[i+19]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   //namepar[i+20]=strdup(" OEanis1    ");  valpar[i+20]= 0;    helppar[i+20]=strdup("Shift of the magnon spectra E_magnon=q^2+QCoef1*fabs(q)+QCoef0 ");
   NumPar=i+15;

  };
void   FAZerHQsMagnonMeanFieldPaper9::ClcBase(double *par,double *Xval,double *Res)
  {
   //double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Phi=par[8],Eta=par[9],
   //       OutPhi=par[10],NumHTetaInt=par[11],Beta=par[12],Temp=par[13],
   //       Epow=par[14],MinEn=par[15],Statistic=par[16];
   double H=par[3],Qs=par[4],IS=par[5],W=par[6],Teta=par[7],Gamma=par[8],
          Eta=par[9],Phi=par[10],
          OutPhi=par[11],NumHTetaInt=par[12];
   //double IntOpticCoef=par[13];
   double Temp=par[14];//JS_const=par[13],Tpow=par[15],
   double Statistic=par[16], QCoef0=par[17], QCoef1=par[18], QCoef2=par[19], QCoef2_R2=par[20],
          DisordCoef=par[21];
   double Rconst=par[22],QMax=par[23],QMin=par[24],dCosMax=par[25];
   QCoef2 = QCoef2_R2*sqr(Rconst);
   par[19] = QCoef2;
cout<<" Rconst " <<Rconst<<" Qcoef2 "<<QCoef2<<"\n";

   Ref<MagnonMeanField> field=new MagnonField_Paper9(Temp,Statistic,
     QCoef0, QCoef1, QCoef2, Rconst, QMax, QMin, dCosMax);
   Ref<HamData> HamOrd=   new Ham_Qs_Gamma(HamData(Teta,Phi,H,Qs,Eta,IS,W),Gamma);
   Ref<HamData> HamDisOrd=new Ham_Qs_Gamma(HamData(0,0,0,Qs,0,IS,W),0);
   
   TetaAverageSolver_Advanced(HamOrd,
     new TetaGenerator_Paper9(field,NumHTetaInt,1),
     new MeanAngleConstructor_Paper9(Rconst*Temp),OutPhi,HamDisOrd,DisordCoef).Solve(Xval,Res);

  };


//  ================================================
//  ================ QS(t) approcsimaton  ==========
//  ===== sum of two probable values for QS ========
//  ======       T=0 Qs=max;  T=inf QS=0;   ========
//  ================================================

#ifndef WCPP_NET
DescribedClass * Dummy_FAZerQs_T(){return new FAZerQs_T();}
static ClassDesc FAZerQs_T_ClassDesc(typeid(FAZerQs_T),"Qs_T",1,"FAZer",Dummy_FAZerQs_T);
#else
static ClassDesc FAZerQs_T_ClassDesc(typeid(FAZerQs_T),"Qs_T",1,"FAZer",create<FAZerQs_T>);
#endif

FAZerQs_T::FAZerQs_T()
  {
   name=strdup("Qs_T");
   NumPar=3;
   helppar[0 ]=strdup("Calculation of Qs(Temperature), X - temperature, Y - Qs. Calculation done by formula: T_to_Cm_mu1=0.69457 ; T=Xval*T_to_Cm_mu1 ; d1=Fst_split ; d2=Fst_split*Sec_split_coef ; QS(Xval[K]) = -0.5*QS_max*(-2+exp(-d1/T)+exp(-d2/T))/(1+exp(-d1/T)+exp(-d2/T));  ");
   namepar[1 ]=strdup("   QS_max     ");valpar[1 ]=2;	helppar[1 ]=strdup("Maximum possible QS(0)");
   namepar[2 ]=strdup("   Fst_split  ");valpar[2 ]=100;	helppar[2 ]=strdup("Energy of splitting of Qs (Cubic distortion?, of cause not correct )");
   namepar[3 ]=strdup("Sec_split_coef");valpar[3 ]=30;	helppar[3 ]=strdup("Energy of splitting of splitted level of Qs (Trigonal distortion?, of cause not correct )");
  };

double FAZerQs_T::Clc(double *par,double Xval)
  {
   double T_to_Cm_mu1=0.69457,T=Xval*T_to_Cm_mu1,d1=par[2],d2=par[2]*par[3];
   return -0.5*par[1]*(-2+exp(-d1/T)+exp(-d2/T))/(1+exp(-d1/T)+exp(-d2/T));
  };
void   FAZerQs_T::Clc(double *par,double *Xval,double *Res)
  {
   double T_to_Cm_mu1=0.69457,T,d1=par[2],d2=par[2]*par[3],Amp=-0.5*par[1];
   for (int k=1;k<=Xval[0];k++) 
     {
      T=Xval[k]*T_to_Cm_mu1;
      Res[k]=Amp*(-2+exp(-d1/T)+exp(-d2/T))/(1+exp(-d1/T)+exp(-d2/T));
     }
//cout<<" Arr\n";
  };
     
void FAZerQs_T::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) {
        if (fabs(par[k]) <= MathZer) stp[k] = 0.1;
        else stp[k] = par[k] * 0.01;
    }
 };


//  ================================================
//  ================ Int(t) in the  ================
//  ==============  deby approcsimaton  ============
//  ================================================
// from mineint1.pas

#ifndef WCPP_NET
DescribedClass * Dummy_FAZerInt_T(){return new FAZerInt_T();}
static ClassDesc FAZerInt_T_ClassDesc(typeid(FAZerInt_T),"Int_T",1,"FAZer",Dummy_FAZerInt_T);
#else
static ClassDesc FAZerInt_T_ClassDesc(typeid(FAZerInt_T),"Int_T",1,"FAZer",create<FAZerInt_T>);
#endif

FAZerInt_T::FAZerInt_T()
  {
   name=strdup("Int_T");
   NumPar=3;
   helppar[0 ]=strdup("Calculation of Int(Temperature) in Deby model, X - temperature, Y - Int. Calculation done by formula: Int(Xval[k])=Ground+Intencity*128/T_deby*( 0.25+sqr(Xval/T_deby)*Integral(Func=[x/(exp(x)-1)],From=[0],To=[T_deby/Xval]) );  ");
   namepar[1 ]=strdup("   Ground     ");valpar[1 ]=1e4;	helppar[1 ]=strdup("Zero Int(0)");
   namepar[2 ]=strdup("   Intencity  ");valpar[2 ]=1e3;	helppar[2 ]=strdup("Amplitude of Deby temperature correction ");
   namepar[3 ]=strdup("   T_deby     ");valpar[3 ]=200;	helppar[3 ]=strdup("Deby temperature");
  };

double DebyFuncInt_Is(double x) {if (x==0) return 1;return x/(exp(x)-1);};
double FAZerInt_T::Clc(double *par,double Xval)
  {
   double ResMis;int Err;
   return par[1]+par[2]*128/par[3]*(0.25+sqr(Xval/par[3])*
            Qags_IntSubInt(DebyFuncInt_Is,0,par[3]/Xval,StndErr,StndErr,1000,ResMis,Err));
//                 RecInt(DebyFuncInt_Is,0,par[3]/Xval,StndErr));
  };
void   FAZerInt_T::Clc(double *par,double *Xval,double *Res)
  {
    double ResMis;int Err;
   for (int k=1;k<=Xval[0];k++) 
      Res[k]=par[1]+par[2]*128/par[3]*(0.25+sqr(Xval[k]/par[3])*
Qags_IntSubInt(DebyFuncInt_Is,0,par[3]/Xval[k],StndErr,StndErr,1000,ResMis,Err));                 
//   RecInt(DebyFuncInt_Is,0,par[3]/Xval[k],StndErr));
  };
     
void FAZerInt_T::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) {
        if (fabs(par[k]) <= MathZer) stp[k] = 0.1;
        else stp[k] = par[k] * 0.01;
    }
 };



//  ================================================
//  ================ Is(t) in the  ================
//  ==============  deby approcsimaton  ============
//  ================================================

#ifndef WCPP_NET
DescribedClass * Dummy_FAZerIs_T(){return new FAZerIs_T();}
static ClassDesc FAZerIs_T_ClassDesc(typeid(FAZerIs_T),"Is_T",1,"FAZer",Dummy_FAZerIs_T);
#else
static ClassDesc FAZerIs_T_ClassDesc(typeid(FAZerIs_T),"Is_T",1,"FAZer",create<FAZerIs_T>);
#endif
FAZerIs_T::FAZerIs_T()
  {
   name=strdup("Is_T");
   NumPar=2;
   helppar[0 ]=strdup("Calculation of IS(Temperature) in Deby model, X - temperature, Y - IS. Calculation done by formula: DebyFunc(x)=3/(x*x*x)*Integral(Func=[x*x*x/(exp(x)-1)],From=[0],To=[x]) ; IS(Xval[k])=Ground+7.23*(1e-4)*Xval*DebyFunc(T_deby/Xval) ;  ");
   namepar[1 ]=strdup("   Ground     ");valpar[1 ]=2;	helppar[1 ]=strdup("Zero IS(0)");
   namepar[2 ]=strdup("   T_deby     ");valpar[2 ]=200;	helppar[2 ]=strdup("Deby temperature");
  };

double FAZerIs_T::Clc(double *par,double Xval)
  {
   return par[1]+7.23*(1e-4)*Xval*DebyFunc(par[2]/Xval);
  };
void   FAZerIs_T::Clc(double *par,double *Xval,double *Res)
  {
   for (int k=1;k<=Xval[0];k++) 
     Res[k]=par[1]+7.23*(1e-4)*Xval[k]*DebyFunc(par[2]/Xval[k]);
  };
     
void FAZerIs_T::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) {
        if (fabs(par[k]) <= MathZer) stp[k] = 0.1;
        else stp[k] = par[k] * 0.01;
    }
 };






/*
int FAMessbRegister(fstream &in,FAIO *&Func,char *name)
  {
   FAZer *SFunc=NULL;
   int ret=1;
   if (!Stricmp(name,"hq_poly")) {SFunc=(FAZer*)new FAZerHQs;}
   else if (!Stricmp(name,"hq_mono")) {SFunc=(FAZer*)new FAZerHQsM;}
//   else if (!Stricmp(name,"qh_poly")) {SFunc=(FAZer*)new FAZerQsHR0;}
//   else if (!Stricmp(name,"hq_relax0")) {SFunc=(FAZer*)new FAZerHQsR0;}
   else if (!Stricmp(name,"hq_poly_rotate")) {SFunc=(FAZer*)new FAZerHQs_Rotate;}
   //else if (!Stricmp(name,"hq_relax")) {SFunc=(FAZer*)new FAZerHQsR;}
   //else if (!Stricmp(name,"hq_relax3")) {SFunc=(FAZer*)new FAZerHQsR3;}
   //else if (!Stricmp(name,"qh_relax6")) {SFunc=(FAZer*)new FAZerQsHR6;}
   //else if (!Stricmp(name,"hq_relax0h")) {SFunc=(FAZer*)new FAZerHQs0QsR;}
   else if (!Stricmp(name,"F_relax2")) {SFunc=(FAZer*)new FAZerFreeR;}
   else if (!Stricmp(name,"hq_aver")) {SFunc=(FAZer*)new FAZerHQsAver;}
   else if (!Stricmp(name,"qh_relax_Hn")) {SFunc=(FAZer*)new FAZerQsH_R_Hn;}
   else if (!Stricmp(name,"qh_relax_kashuba")) {SFunc=(FAZer*)new FAZerQsHRelaxKashuba;}
   else if (!Stricmp(name,"qh_relax_rand_walk")) {SFunc=(FAZer*)new FAZerQsHRelaxRandWalk;}

//   else if (!Stricmp(name,"HQ_relax_circular")) {SFunc=(FAZer*)new FAZerHQsRC;}
   
   else if (!Stricmp(name,"Is_T")) {SFunc=(FAZer*)new FAZerIs_T;}
   else if (!Stricmp(name,"Int_T")) {SFunc=(FAZer*)new FAZerInt_T;}
   else if (!Stricmp(name,"Qs_T")) {SFunc=(FAZer*)new FAZerQs_T;}
                   else ret=0;
   if (!ret) return ret;
   if (Func!=NULL) delete(Func);
      FASingleIO *F=new FASingleIO;
      F->Func=SFunc;
      Func=(FAIO*)F;
   Func->Input(in);
//cout<<" Mem after FAMessbRegister  "<<coreleft()<<"\n";
   return ret;
  };




*/










/*
//      Do not work for Phi
//  ================================================
//  ================ QH poly     ===================
//  ================================================

FAZerQsHR0::FAZerQsHR0()
  {
   name=strdup("QH_poly");
   NumPar=9;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
  };

double FAZerQsHR0::Clc(double *par,double Xval)
  {
   CMatrCl Mat(8);
   QsHMat(Mat,par[7],par[8],par[3],par[4],par[9],par[5]);
   Mat=Mat-my_comp(0,par[6]/2);
   double ret=par[1]-par[2]*QsHInt(Mat,par[7],par[8],Xval);
//cout<<" One\n";
   return ret;
  };
void   FAZerQsHR0::Clc(double *par,double *Xval,double *Res)
  {
   CMatrCl Mat(8);
   QsHMat(Mat,par[7],par[8],par[3],par[4],par[9],par[5]);
   Mat=Mat-my_comp(0,par[6]/2);
//      Do not work for Phi
   QsHInt(Mat,par[7],par[8],Xval,Res);
   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
//cout<<" Arr\n";
  };
     
void FAZerQsHR0::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };


//  ================================================
//  ================ No Relax case  ================
//  ================ calculated as relax ===========
//  ================================================

FAZerHQsR0::FAZerHQsR0()
  {
   name=strdup("HQ_relax0");
   NumPar=11;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
   namepar[10]=strdup("Teta_H   ");
   namepar[11]=strdup("Phi_H    ");
  };

double FAZerHQsR0::Clc(double *par,double Xval)
  {
   CMatrCl Mat(8);
   R01HQs(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],par[10],par[11]);
   double ret=par[1]-par[2]*QsHInt(Mat,par[10],par[11],Xval);
//   double ret=par[1]-par[2]*R0HQsInt(Mat,Xval);
//cout<<" One\n";
   return ret;
  };
void   FAZerHQsR0::Clc(double *par,double *Xval,double *Res)
  {
   CMatrCl Mat(8);
   R01HQs(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],par[10],par[11]);
   QsHInt(Mat,par[10],par[11],Xval,Res);
   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
//cout<<" Arr\n";
  };
     
void FAZerHQsR0::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };


*/



/*
//  ================================================
//  ================ Relax 2 case  =================
//  ================================================


FAZerHQsR::FAZerHQsR()
  {
   name=strdup("HQ_relax");
   NumPar=11;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
   namepar[10]=strdup("  Tup    ");
   namepar[11]=strdup("  Tdown  ");
  };

double FAZerHQsR::Clc(double *par,double Xval)
  {
   CMatrCl Mat(16);
   R2HUpHDown(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],par[10],par[11]);
   CMatrCl AmpL,AmpR;
   R2HQsAmpVec(par[10],par[11],AmpL,AmpR);
//   double ret=par[1]-par[2]*RInt(Mat,AmpL,AmpR,Xval,par[6]);
   double ret=par[1]-par[2]*RIntTurn(Mat,AmpL,AmpR,Xval,par[6]);
   return ret;
  };
void   FAZerHQsR::Clc(double *par,double *Xval,double *Res)
  {
   CMatrCl Mat(16);
   R2HUpHDown(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],par[10],par[11]);
   CMatrCl AmpL,AmpR;
   R2HQsAmpVec(par[10],par[11],AmpL,AmpR);
//   RInt(Mat,AmpL,AmpR,Xval,Res,par[6]);
   RIntTurn(Mat,AmpL,AmpR,Xval,Res,par[6]);
   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
//cout<<" Arr\n";
  };
void FAZerHQsR::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };

//  ================================================
//  ================ Relax 2 case  =================
//  ================ Field no field ================
//  ================================================


FAZerHQs0QsR::FAZerHQs0QsR()
  {
   name=strdup("HQ_relax0h");
   NumPar=11;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
   namepar[10]=strdup("  TField ");
   namepar[11]=strdup("  TQs    ");
  };

double FAZerHQs0QsR::Clc(double *par,double Xval)
  {
   CMatrCl Mat(16);
   R2HUpHZer(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],par[10],par[11]);
   CMatrCl AmpL,AmpR;
   R2HQsAmpVec(par[10],par[11],AmpL,AmpR);
//   double ret=par[1]-par[2]*RInt(Mat,AmpL,AmpR,Xval,par[6]);
   double ret=par[1]-par[2]*RIntTurn(Mat,AmpL,AmpR,Xval,par[6]);
//cout<<" One\n";
   return ret;
  };
void   FAZerHQs0QsR::Clc(double *par,double *Xval,double *Res)
  {
   CMatrCl Mat(16);
   R2HUpHZer(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],par[10],par[11]);
   CMatrCl AmpL,AmpR;
   R2HQsAmpVec(par[10],par[11],AmpL,AmpR);
//   RInt(Mat,AmpL,AmpR,Xval,Res,par[6]);
   RIntTurn(Mat,AmpL,AmpR,Xval,Res,par[6]);
   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
//cout<<" Arr\n";
  };
     

void FAZerHQs0QsR::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
//  ================================================
//  ================ Relax 3 case  =================
//  ================================================


FAZerHQsR3::FAZerHQsR3()
  {
   name=strdup("HQ_relax3");
   NumPar=14;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
   namepar[10]=strdup("  T1     ");
   namepar[11]=strdup("  T2     ");
   namepar[12]=strdup("  T3     ");
   namepar[13]=strdup(" Alph_zx ");
   namepar[14]=strdup(" Alph_xy ");
  };

double FAZerHQsR3::Clc(double *par,double Xval)
  {
   CMatrCl Mat(24);
   R3Turn120(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],par[10],par[11],par[12],par[13],par[14]);
   CMatrCl AmpL(24),AmpR(24);
   R3HQsAmpVec(par[13],par[14],par[13]+120,par[14],
               par[13]-120,par[14],par[10],par[11],par[12],AmpL,AmpR);
   double ret=par[1]-par[2]*RIntTurn(Mat,AmpL,AmpR,Xval,par[6]);
//cout<<" One\n";
   return ret;
  };
void   FAZerHQsR3::Clc(double *par,double *Xval,double *Res)
  {
   CMatrCl Mat(24);
   R3Turn120(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],par[10],par[11],par[12],par[13],par[14]);
   CMatrCl AmpL(24),AmpR(24);
   R3HQsAmpVec(par[13],par[14],par[13]+120,par[14],
               par[13]-120,par[14],par[10],par[11],par[12],AmpL,AmpR);
   RIntTurn(Mat,AmpL,AmpR,Xval,Res,par[6]);
   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
//cout<<" Arr\n";
  };
     
void FAZerHQsR3::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
//  ================================================
//  ======== Relaxation 6-phi angle Function =======
//  ================================================

FAZerQsHR6::FAZerQsHR6()
  {
   name=strdup("qh_relax6");
   NumPar=15;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
   namepar[10]=strdup("  T0     ");
   namepar[11]=strdup("  T60    ");
   namepar[12]=strdup("  T120   ");
   namepar[13]=strdup("  T180   ");
   namepar[14]=strdup("  T240   ");
   namepar[15]=strdup("  T300   ");
  };
double FAZerQsHR6::Clc(double *par,double Xval)
  {
//   double ret=par[1]-
//            par[2]*HQsAvInt(par[7],par[8],par[3],par[4],par[9],
//                            par[5],par[6],par[10],par[11],par[12],par[13],par[14],par[15],par[16],par[17],Xval);
   return 0;
  };
void   FAZerQsHR6::Clc(double *par,double *Xval,double *Res)
  {

   CMatrCl Mat,AmpL,AmpR;
   double time[7]={6,par[10],par[11],par[12],par[13],par[14],par[15]};

   RnRelax(Mat,par[7],par[8],par[3],par[4],par[9],
             par[5],par[6],&time[0],6);
   RnHQsAmpVec(6,&time[0],AmpL,AmpR);
   RIntTurn(Mat,AmpL,AmpR,Xval,Res,par[6]);
   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
   
//cout<<" Arr\n";
  };
void FAZerQsHR6::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };

//  ================================================
//  ================ Relax circular case  ==========
//  ================================================

FAZerHQsRC::FAZerHQsRC()
  {
   name=strdup("HQ_relax_circular");
   NumPar=12;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
   namepar[10]=strdup("Tcircule ");
   namepar[11]=strdup(" Alph_zx ");
   namepar[12]=strdup(" Alph_xy ");
  };

double FAZerHQsRC::Clc(double *par,double Xval)
  {
   double x[2],y[2];x[0]=1;x[1]=Xval;y[0]=1;
   R2CircleInt(par[ 7],par[ 8],par[ 3],par[ 4],par[ 9],par[ 5],
               par[ 6],par[11],par[12],x,y,par[10]);
   double gr=par[1],in=par[2];
   return gr-in*y[1];
  };
void   FAZerHQsRC::Clc(double *par,double *Xval,double *Res)
  {
   R2CircleInt(par[ 7],par[ 8],par[ 3],par[ 4],par[ 9],par[ 5],
               par[ 6],par[11],par[12],Xval,Res,par[10]);
   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
//cout<<" Arr\n";
  };
     
void FAZerHQsRC::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
*/

//  ================================================
//  =============      to be deleted   =============
//  ================================================


/*
//  ================================================
//  ================ Relaxation Any Function =======
//  ================================================

FAZerQsHRelaxAver::FAZerQsHRelaxAver()
  {
   name=strdup("qh_relax_aver");
   NumPar=22;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
   namepar[10]=strdup(" RotTime ");
   namepar[11]=strdup("NumTimes ");
   namepar[12]=strdup("NumToAver");
   namepar[13]=strdup("FurieDisc");
   namepar[14]=strdup("  Time   ");
   namepar[15]=strdup("  OutPhi ");
   namepar[16]=strdup("StrongSho");
   namepar[17]=strdup("SingleRot");
   namepar[18]=strdup("NTimeAver");
   namepar[19]=strdup("EndTAver ");
   namepar[20]=strdup("   Pup   ");
   namepar[21]=strdup("  Pdown  ");
   namepar[22]=strdup("  AnisK  ");

  };
double FAZerQsHRelaxAver::Clc(double *par,double Xval)
  {
//   double ret=par[1]-
//            par[2]*HQsAvInt(par[7],par[8],par[3],par[4],par[9],
//                            par[5],par[6],par[10],par[11],par[12],par[13],par[14],par[15],par[16],par[17],Xval);
   return 0;
  };
void   FAZerQsHRelaxAver::Clc(double *par,double *Xval,double *Res)
  {
   RelaxAverageStruct Aver;
   Aver.Teta0=par[7];Aver.Phi0=par[8];
   Aver.NumTimes=par[11];Aver.FurieDiscr=par[13];
   Aver.EndTime=par[14];Aver.MeanRotTime=par[10];Aver.NumToAverage=par[12];
   Aver.StrongShort=par[16];Aver.SingleRotation=par[17];Aver.OutPhi=par[15];
   Aver.NumTimeAver=par[18];Aver.EndTimeAver=par[19];
   Aver.Pup=par[20];Aver.Pdown=par[21];Aver.AnisotrpCoef=par[22];
   RelaxHamInt(Aver,  par[3],par[4],par[9],par[5],par[6],Xval,Res);

   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
  };
void FAZerQsHRelaxAver::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };


//  ================================================
//  ================ Relaxation Random Walk  =======
//  ================================================

FAZerQsHRelaxRandWalk::FAZerQsHRelaxRandWalk()
  {
   name=strdup("qh_relax_aver");
   NumPar=28;
   namepar[1 ]=strdup("Ground   ");
   namepar[2 ]=strdup("Intencity");
   namepar[3 ]=strdup("   H     ");
   namepar[4 ]=strdup("   QS    ");
   namepar[5 ]=strdup("   IS    ");
   namepar[6 ]=strdup("   W     ");
   namepar[7 ]=strdup("  Teta   ");
   namepar[8 ]=strdup("  Phi    ");
   namepar[9 ]=strdup("  Eta    ");
   namepar[10]=strdup("  OutPhi ");
   namepar[11]=strdup("RelaxType");
   namepar[12]=strdup("NumToAver");
   namepar[13]=strdup("NTimeAver");
   namepar[14]=strdup("EndTAver ");
   namepar[15]=strdup("IntegTime");

   namepar[16]=strdup("JumpTimeTet");
   namepar[17]=strdup("JumpTimeTetS");
   namepar[18]=strdup("JumpTimePhi");
   namepar[19]=strdup("JumpTimePhiS");
   namepar[20]=strdup("JumpAngTet");
   namepar[21]=strdup("JumpAngTetS");
   namepar[22]=strdup("JumpAngPhi");
   namepar[23]=strdup("JumpAngPhiS");
   namepar[24]=strdup("AnisTeta ");
   namepar[25]=strdup("AnisPhi  ");
   namepar[26]=strdup("PhiRelaxInf");
   namepar[27]=strdup("  Pup    ");
   namepar[28]=strdup("  Pdown  ");

  };
double FAZerQsHRelaxRandWalk::Clc(double *par,double Xval)
  {
//   double ret=par[1]-
//            par[2]*HQsAvInt(par[7],par[8],par[3],par[4],par[9],
//                            par[5],par[6],par[10],par[11],par[12],par[13],par[14],par[15],par[16],par[17],Xval);
   return 0;
  };
void   FAZerQsHRelaxRandWalk::Clc(double *par,double *Xval,double *Res)
  {
   RelaxAverageStruct Aver;
   Aver.NumTimes=1;Aver.FurieDiscr=1;Aver.MeanRotTime=1;
   Aver.EndTime=par[15];Aver.NumToAverage=par[12];Aver.OutPhi=par[10];
   Aver.SingleRotation=par[11];Aver.NumTimeAver=par[13];Aver.EndTimeAver=par[14];
   Aver.StrongShort=par[26];
   Aver.Pup=par[27];Aver.Pdown=par[28];
   Aver.AnisotrpCoef=par[24];Aver.AnisotrpCoef1=par[25];

   Aver.TimeTeta0=par[16];Aver.TimeTeta1=par[17];
   Aver.TimePhi0=par[18];Aver.TimePhi1=par[19];
   Aver.Teta0=par[20];Aver.Teta1=par[21];
   Aver.Phi0=par[22];Aver.Phi1=par[23];

   RelaxHamInt(Aver,  par[3],par[4],par[9],par[5],par[6],Xval,Res);

   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
  };
void FAZerQsHRelaxRandWalk::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };






//  =======================================================
//  ================ Monocristall case  ===================
//  =======================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerHQsM(){return new FAZerHQsM();}
static ClassDesc FAZerHQsM_ClassDesc(typeid(FAZerHQsM),"HQ_mono",1,"FAZer",Dummy_FAZerHQsM);
#else
static ClassDesc FAZerHQsM_ClassDesc(typeid(FAZerHQsM),"HQ_mono",1,"FAZer",create<FAZerHQsM>);
#endif

FAZerHQsM::FAZerHQsM()
  {
   name=strdup("HQ_mono");
   NumPar=11;
													helppar[0 ]=strdup("Calculation of spectra of monocrystall Fe. Main axis - Fix, H//Fix_z, QS - is turned (Phi,Teta params), Look along Dir(TetaH_View,TetaH_View) angles of Fix system coordinats.");
   namepar[1 ]=strdup("Ground    ");valpar[1 ]=0;	helppar[1 ]=strdup("Added Ground int");
   namepar[2 ]=strdup("Intencity ");valpar[2 ]=10;	helppar[2 ]=strdup("Spectr multiplied by ");
   namepar[3 ]=strdup("   H      ");valpar[3 ]=200;	helppar[3 ]=strdup("Internal magnetic field     ");
   namepar[4 ]=strdup("   QS     ");valpar[4 ]=2;	helppar[4 ]=strdup("Internal QS    ");
   namepar[5 ]=strdup("   IS     ");valpar[5 ]=0;	helppar[5 ]=strdup("Isomer shift    ");
   namepar[6 ]=strdup("   W      ");valpar[6 ]=0.4;	helppar[6 ]=strdup("Width of line     ");
   namepar[7 ]=strdup("  Teta    ");valpar[7 ]=0;	helppar[7 ]=strdup("Teta Angle between Fix_Z and QS_Z    ");
   namepar[8 ]=strdup("  Phi     ");valpar[8 ]=0;	helppar[8 ]=strdup("Phi Angle between Fix_X and QS_Z - proection in (QS_X,QS_Y) plane -so final QS_axis - result of two operations under its axises - turn by teta around Fix_x plus turn on phi around Fix_z  ");
   namepar[9 ]=strdup("  Eta     ");valpar[9 ]=0;	helppar[9 ]=strdup("Qs Eta (xy assimetry)   ");
   namepar[10]=strdup("TetaH_View");valpar[10]=0;	helppar[10]=strdup("Look along Dir(TetaH_View,TetaH_View) angles of Fix system coordinats.   ");
   namepar[11]=strdup("PhiH_View ");valpar[11]=0;	helppar[11]=strdup("Look along Dir(TetaH_View,TetaH_View) angles of Fix system coordinats.   ");
  };
double FAZerHQsM::Clc(double *par,double Xval)
  {
   double *Int=new double[9];Int[0]=8;
   double *Pos=new double[9];Pos[0]=8;
   HQsMatMonoClc(par[7],par[8],par[3],par[4],par[5],par[9],par[10],par[11],
                                                                  Int,Pos);
   double ret=par[1]-par[2]*HQsInt(Int,Pos,par[6],Xval);
//cout<<" One\n";
   delete Int;delete Pos;
   return ret;
  };
void   FAZerHQsM::Clc(double *par,double *Xval,double *Res)
  {
   double *Int=new double[9];Int[0]=8;
   double *Pos=new double[9];Pos[0]=8;
   HQsMatMonoClc(par[7],par[8],par[3],par[4],par[5],par[9],par[10],par[11],
                                                                   Int,Pos);
   HQsInt(Int,Pos,par[6],Xval,Res);
   double gr=par[1],in=par[2];
   for (int k=1;k<=Xval[0];k++) Res[k]=gr-in*Res[k];
//cout<<" Arr\n";
   delete Int;delete Pos;
  };
     
void FAZerHQsM::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };



*/
//double FAZerHQs::Clc(double *par,double Xval)
//{
//   VecCl Int(8),Pos(8);
//   CMatrCl Mat;
//   double Teta=par[7],Phi=par[8],H=par[3],Qs=par[4],Eta=par[9],IS=par[5],W=par[6];
//   double Gr=par[1],Amp=par[2];
//   HQsEiler_Mat(Mat, Teta, Phi, H, Qs, Eta);
//   HQsMatClc(Mat,H,IS,Int.Ptr,Pos.Ptr);
//   double ret=Gr-Amp*HQsInt(Int.Ptr,Pos.Ptr,W,Xval);
//   return ret;
//  };
//void FAZerHQs::ParStp(double *par,double *stp)
// {
//  for (int k=1;k<=NumPar;k++)
//    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
//                   else stp[k]=par[k]*0.01;
// };
//
//

//double FAZerHQsM::Clc(double *par,double Xval)
//  {
//   VecCl Int(8),Pos(8);
//   CMatrCl Mat;
//   double Teta=par[7],Phi=par[10],H=par[3],Qs=par[4],Eta=par[9],Gamma=par[8],
//          IS=par[5],W=par[6],Gr=par[1],Amp=par[2],HTeta=par[11],HPhi=par[12];
//
//   HQsMatMono3Clc(Teta,Gamma,Phi,H,Qs,IS,Eta,HTeta,HPhi, Int.Ptr,Pos.Ptr);
//   double ret=Gr-Amp*HQsInt(Int.Ptr,Pos.Ptr,W,Xval);
//   return ret;
//  };
//
//   
//void FAZerHQsM::ParStp(double *par,double *stp)
// {
//  for (int k=1;k<=NumPar;k++)
//    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
//                   else stp[k]=par[k]*0.01;
// };
//
//double FAZerFreeR::Clc(double *par,double Xval)
//  {
//   CMatrCl Mat(16);
//   R2Free(Mat,par[7],par[8],par[3],par[4],par[9],par[5],par[6],par[10],
//         par[11],par[16],par[17],par[12],par[13],par[18],par[14],par[15],
//         par[19],par[20],par[21],par[22]);
//   CMatrCl AmpL,AmpR;
////   R2HQsAmpVec(par[10],par[11],par[19],par[20],par[21],par[22],AmpL,AmpR);
//   R2HQsAmpVec(par[21],par[22],AmpL,AmpR);
//   double ret=par[1]-par[2]*RIntTurn(Mat,AmpL,AmpR,Xval,par[6]); 
////cout<<" One\n";
//   return ret;
//  };
//
//void FAZerFreeR::ParStp(double *par,double *stp)
// {
//  for (int k=1;k<=NumPar;k++)
//    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
//                   else stp[k]=par[k]*0.01;
// };
//
