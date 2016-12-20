#include <lib/precompiled/Messb.h>


#include "fastate.h"

/*
//  =======================================================
//  ================ Grun calc?             ===============
//  =======================================================


FAZerABu_Grun::FAZerABu_Grun()
  {
   name=strdup("ABu_Grun");
   NumPar=8;
   namepar[1]=strdup("Ground       ");
   namepar[2]=strdup("Denc         ");
   namepar[3]=strdup("A            ");
   namepar[4]=strdup("B            ");
   namepar[5]=strdup("G0           ");
   namepar[6]=strdup("G1           ");
   namepar[7]=strdup("G2           ");
   namepar[8]=strdup("G3           ");
  };

void FAZerABu_Grun::SetABuConst(double *par)
  {
   Mat.Denc=par[2];Mat.A=par[3];Mat.B=par[4];
   Mat.G0=par[5];Mat.G1=par[6];Mat.G2=par[7];Mat.G3=par[8];
   MatterAdiabat=&Mat;
  };

double FAZerABu_Grun::Clc(double *par,double Xval)
  {
   SetABuConst(par);
   double u1=Xval,r1=Mat.DencityCold(),p1=0,e1=0,u2=0,r2,p2,e2,D;
   Hug(u1,r1,p1,e1,u2,r2,p2,e2,D);
   return Mat.Sound(r2,e2);
  }; 

void   FAZerABu_Grun::Clc(double *par,double *Xval,double *Res)
  {
   SetABuConst(par);
   double u1=0,r1=Mat.DencityCold(),p1=0,e1=0,u2=0,r2,p2,e2,D;
   for (int k=1;k<=Xval[0];k++)
     {
      Hug(Xval[k],r1,p1,e1,u2,r2,p2,e2,D);
      Res[k]=Mat.Sound(r2,e2);
     } 
  };

void FAZerABu_Grun::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
*/
//  =======================================================
//  ================ H2 Cold case of Zero T ===============
//  =======================================================


FAZerABu_H2cold::FAZerABu_H2cold()
  {
   name=strdup("ABu_H2cold");
   fstream in("h2_cold.inp",ios::in);
//   H2Mat.InterfaceFindSubstanceIO::ReadIni(in);
   H2Mat.ReadSubstancePar(in);
   NumPar=14;
   namepar[1]=strdup("Ground       ");
   namepar[2]=strdup(" ExpAdd      ");
   namepar[3]=strdup(" a1          ");
   namepar[4]=strdup(" a2          ");
   namepar[5]=strdup(" a3          ");
   namepar[6]=strdup(" a4          ");
   namepar[7]=strdup(" a5          ");
   namepar[8]=strdup(" a6          ");
   namepar[9]=strdup(" Temp        ");
   namepar[10]=strdup(" B0c         ");
   namepar[11]=strdup(" M           ");
   namepar[12]=strdup(" N           ");
   namepar[13]=strdup(" Edist       ");
   namepar[14]=strdup(" Denc        ");
  };

void FAZerABu_H2cold::SetABuConst(double *par)
  {
   ColdParam.Nparam=6;
   ColdParam.Denc=par[14];
   double s=0;
   int o;
   for (o=4;o<=8;o++) s+=par[o];par[3]=-s;
   s=0;for (o=3;o<=8;o++) s+=par[o]*((o-2)/3.+1);par[10]=s;
   double s1=0;for (o=3;o<=8;o++) s1+=par[o]*sqr((o-2)/3.+1);
   par[12]=s1/s-par[11]-2;
   par[13]=par[10]/(ColdParam.Denc*par[11]*par[12]);
   ColdParam.B0c=par[10];ColdParam.Mconst=par[11]; ColdParam.Nconst=par[12];
   ColdParam.Edist=par[13];
cout<<" B "<<par[10]<<" M "<<par[11]<<" N "<<par[12]<<" Ed "<<par[13]<<"\n";
   for (int k=1;k<=6;k++) ColdParam.coef[k]=par[k+2];
  };

double FAZerABu_H2cold::Clc(double *par,double Xval)
  {
   double x=Xval/ColdParam.Denc;SetABuConst(par);
   double Temp=par[9];
   return log(Psolid(x,Temp)+par[2])+par[1];
  }; 

void   FAZerABu_H2cold::Clc(double *par,double *Xval,double *Res)
  {
   SetABuConst(par);
   double x,Temp=par[9];
   for (int k=1;k<=Xval[0];k++)
     {
      x=Xval[k]/ColdParam.Denc;
// Limitation sum=0; do not vary 1
      Res[k]=log(Psolid(x,Temp)+par[2])+par[1];
     } 
  };

void FAZerABu_H2cold::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };

//  =======================================================
//  ================ Empi Cold case of Zero T =============
//  =======================================================

extern EmpiParamStruct *EmpiColdParam;

FAZerABu_EmpiCold::FAZerABu_EmpiCold()
  {
   name=strdup("ABu_EmpiCold");
   fstream in("empi.inp",ios::in);
   Matt=dynamic_cast<MatterIO*>(SavableClass::Read(in));
   NumPar=14;
   namepar[1]=strdup("Ground       ");
   namepar[2]=strdup(" ExpAdd      ");
   namepar[3]=strdup(" a1          ");
   namepar[4]=strdup(" a2          ");
   namepar[5]=strdup(" a3          ");
   namepar[6]=strdup(" a4          ");
   namepar[7]=strdup(" a5          ");
   namepar[8]=strdup(" a6          ");
   namepar[9]=strdup(" Temp        ");
   namepar[10]=strdup(" B0c         ");
   namepar[11]=strdup(" M           ");
   namepar[12]=strdup(" N           ");
   namepar[13]=strdup(" Edist       ");
   namepar[14]=strdup(" Denc        ");
  };

void FAZerABu_EmpiCold::SetABuConst(double *par)
  {
   EmpiColdParam->Nparam=6;
   EmpiColdParam->Denc=par[14];
   double s=0;for (int o=4;o<=8;o++) s+=par[o];par[3]=-s;
   s=0;for (int o=3;o<=8;o++) s+=par[o]*((o-2)/3.+1);par[10]=s;
   double s1=0;for (int o=3;o<=8;o++) s1+=par[o]*sqr((o-2)/3.+1);
   par[12]=s1/s-par[11]-2;
   par[13]=par[10]/(EmpiColdParam->Denc*par[11]*par[12]);
   EmpiColdParam->B0c=par[10];EmpiColdParam->Mconst=par[11]; EmpiColdParam->Nconst=par[12];
   EmpiColdParam->Edist=par[13];
cout<<" B "<<par[10]<<" M "<<par[11]<<" N "<<par[12]<<" Ed "<<par[13]<<"\n";
   for (int k=1;k<=6;k++) EmpiColdParam->coef[k]=par[k+2];
  };

double FAZerABu_EmpiCold::Clc(double *par,double Xval)
  {
   SetABuConst(par);double Temp=par[9];
   double x=Matt->Energy(Xval,Temp);
   return log(Matt->Pressure(Xval,x)+par[2])+par[1];
  }; 

void   FAZerABu_EmpiCold::Clc(double *par,double *Xval,double *Res)
  {
   SetABuConst(par);
   double x,Temp=par[9];
   for (int k=1;k<=Xval[0];k++)
     {
      x=Matt->Energy(Xval[k],Temp);
      Res[k]=log(Matt->Pressure(Xval[k],x)+par[2])+par[1];
     } 
  };

void FAZerABu_EmpiCold::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
/*
//  =======================================================
//  ================ Empi Hugoniot 2-param?   =============
//  =======================================================


FAZerABu_EmpiHug::FAZerABu_EmpiHug()
  {
   name=strdup("ABu_EmpiHug");
   fstream in("empi.inp",ios::in);
   Matt=NULL;MatterRegister(in,Matt);
   NumPar=4;
   namepar[1]=strdup("Ground       ");
   namepar[2]=strdup(" ExpAdd      ");
   namepar[3]=strdup(" Lconst      ");
   namepar[4]=strdup(" VibCoef     ");
  };

void FAZerABu_EmpiHug::SetABuConst(double *par)
  {
   EmpiColdParam->Nparam=6;
   EmpiColdParam->Denc=par[14];
   double s=0;for (int o=4;o<=8;o++) s+=par[o];par[3]=-s;
   s=0;for (o=3;o<=8;o++) s+=par[o]*((o-2)/3.+1);par[10]=s;
   double s1=0;for (o=3;o<=8;o++) s1+=par[o]*sqr((o-2)/3.+1);
   par[12]=s1/s-par[11]-2;
   par[13]=par[10]/(EmpiColdParam->Denc*par[11]*par[12]);
   EmpiColdParam->B0c=par[10];EmpiColdParam->Mconst=par[11]; EmpiColdParam->Nconst=par[12];
   EmpiColdParam->Edist=par[13];
cout<<" B "<<par[10]<<" M "<<par[11]<<" N "<<par[12]<<" Ed "<<par[13]<<"\n";
   for (int k=1;k<=6;k++) EmpiColdParam->coef[k]=par[k+2];
  };

double FAZerABu_EmpiHug::Clc(double *par,double Xval)
  {
   SetABuConst(par);double Temp=par[9];
   double x=Matt->Energy(Xval,Temp);
   return log(Matt->Pressure(Xval,x)+par[2])+par[1];
  }; 

void   FAZerABu_EmpiHug::Clc(double *par,double *Xval,double *Res)
  {
   SetABuConst(par);
   double x,Temp=par[9];
   for (int k=1;k<=Xval[0];k++)
     {
      x=Matt->Energy(Xval[k],Temp);
      Res[k]=log(Matt->Pressure(Xval[k],x)+par[2])+par[1];
     } 
  };

void FAZerABu_EmpiHug::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
*/
//  =======================================================
//  ================ H2 Solid Liquid        ===============
//  =======================================================
// empimelt.inp - structure
//FreeEEmpiSolid   FileParam h2_param.cfg  NameSubstance Empi_My1
//FreeEEmpiLiquid  FileParam h2_param.cfg  NameSubstance Empi_My1
//Num 18                         
//4.733000e-01   7.504000e+01   1.368925e-01   0.0060881        
// Pressure (GPa )  Temperature   DencSolid     DencSolid-DencLiquid
// Parameter Structure Num - Temp,DensCold,DeltaDens
//
static InterfaceFreeEIO *F1,*F2,*F;
double CurPres,CurTemp,CurDenc1,CurDenc2;
double DeltaPres(double Denc)
  { 
//cout<<" CurP "<<CurPres<<" Delta "<<CurPres-F->Pressure(Denc,CurTemp)<<"\n";
   return CurPres-F->Pressure(Denc,CurTemp);  }
void FindDenc(double &Denc1,double &Denc2,double Temp)
  { 
   CurTemp=Temp;
   Denc1=CurDenc1;F=F1;
   if (!ZeroNewtonIt(DeltaPres,Denc1,StndErr*0.01,1000,1e6,MathZer,0.01*Denc1,0.1))
      cout<<" Bad ZeroNewtonIt in FindDenc1 Not Zero\nfound Denc "<<Denc1<<" Mis "<<DeltaPres(Denc1)<<"\n";
   Denc2=CurDenc2;F=F2;
   if (!ZeroNewtonIt(DeltaPres,Denc2,StndErr*0.01,1000,1e6,MathZer,0.01*Denc2,0.1))
      cout<<" Bad ZeroNewtonIt in FindDenc2 Not Zero\nfound Denc "<<Denc2<<" Mis "<<DeltaPres(Denc2)<<"\n";
   CurDenc1=Denc1;CurDenc2=Denc2;
  }  
   
double DeltaFree(double Temp)
  {
   double Denc1,Denc2;
   FindDenc(Denc1,Denc2,Temp);
   return F1->FreeE(Denc1,Temp)-F2->FreeE(Denc2,Temp) 
                   +CurPres*(1/Denc1-1/Denc2);
  };

FAZerABu_EmpiMelt::FAZerABu_EmpiMelt()
  {
   char tmp[256];
   name=strdup("ABu_EmpiMelt");
   fstream in("empimelt.inp",ios::in);
   F1=NULL;F2=NULL;
   if ( (!FreeERegister(in,F1)) || (!FreeERegister(in,F2)) )
     { cout<<" Cold not register FreeERegister(in,F1) in FAZerABu_EmpiMelt::FAZerABu_EmpiMelt()\n";exit(0);}
// F2 - Liquid - then OK!.
   in>>tmp>>NumVal;
//cout<<" NumVal "<<NumVal<<"\n";
   for (int k=0;k<4;k++) PTarr[k]=new double[NumVal+1];
   for (int k0=1;k0<=NumVal;k0++) 
     { for (int k=0;k<4;k++) in>>PTarr[k][k0];
       PTarr[3][k0]+=PTarr[2][k0];}
//cout<<" Entered Points\n";
   NumPar=6;
   namepar[1]=strdup("Ground       ");
   namepar[2]=strdup(" a           ");
   namepar[3]=strdup(" b           ");
   namepar[4]=strdup(" c           ");
   namepar[5]=strdup(" r           ");
   namepar[6]=strdup(" DensL       ");
  };

void FAZerABu_EmpiMelt::SetABuConst(double *par)
  {
   F2->FreeE(PTarr[1][3],PTarr[1][1]); // EmpiColdParam - pointng to the F2
//cout<<" Set Const ";
   EmpiColdParam->Aconst=par[2];
   EmpiColdParam->Bconst=par[3];
   EmpiColdParam->Cconst=par[4];
   EmpiColdParam->RLconst=par[5];
   EmpiColdParam->Denc0const=par[6];
//cout<<" OK \n";
  };

double FAZerABu_EmpiMelt::Clc(double *par,double Xval)
  {
   SetABuConst(par);
   return 0;
  }; 

void   FAZerABu_EmpiMelt::Clc(double *par,double *Xval,double *Res)
  {
   SetABuConst(par);
   double T0;
   
   if (Xval[0]!=NumVal*3) return ;
   for (int k=1;k<=NumVal;k++)
     {
      CurPres=PTarr[0][k];CurTemp=PTarr[1][k];CurDenc1=PTarr[2][k];CurDenc2=PTarr[3][k];
      T0=CurTemp;
//cout<<" Begin  ZeroNewtonIt(DeltaFree,T0,StndErr*0.01,1000,1e6,MathZer,0.3*T0,0.1))";
//ChRead();
      if (!ZeroNewtonIt(DeltaFree,T0,StndErr*0.01,1000,1e6,MathZer,0.3*T0,0.1))
          {cout<<" Bad ZeroNewtonFreeE in H2Melt Mis Not Zero\nfound T "
                    <<T0<<" Mis "<<DeltaFree(T0)<<"\n";}
//cout<<" OK\n";
      Res[k]=T0;Res[k+NumVal]=CurDenc1;Res[k+2*NumVal]=CurDenc1-CurDenc2;
     } 
  };

void FAZerABu_EmpiMelt::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
FAZerABu_EmpiMelt::~FAZerABu_EmpiMelt()
  {delete F1;delete F2;for (int k=0;k<4;k++) delete PTarr[k]; };

//  =======================================================
//  ================ Empi ABu fit           ===============
//  =======================================================
//MatterIO *MatterAdiabat;

FAZerABu_EmpiABu::FAZerABu_EmpiABu()
  {
   name=strdup("ABu_EmpiABu");
//   Matt=NULL;MatterRegister(in,Matt);
   NumPar=22;
   namepar[1]=strdup(" Ground      ");
   namepar[2]=strdup(" DencHug     ");
   namepar[3]=strdup(" EHug        ");
   namepar[4]=strdup(" Denc0       ");
   namepar[5]=strdup(" A           ");
   namepar[6]=strdup(" B           ");
   namepar[7]=strdup(" EDist       ");
   namepar[8]=strdup(" G0c         ");
   namepar[9]=strdup(" Sm          ");
   namepar[10]=strdup(" Sn          ");
   namepar[11]=strdup(" Tmin        ");
   namepar[12]=strdup(" Umin        ");
   namepar[13]=strdup(" Umax        ");
   namepar[14]=strdup(" NumPnt      ");
   namepar[15]=strdup(" TA          ");
   namepar[16]=strdup(" TB          ");
   namepar[17]=strdup(" Draw_Sound  ");
   namepar[18]=strdup(" Gi          ");
   namepar[19]=strdup(" Ea          ");
   namepar[20]=strdup(" TL          ");
   namepar[21]=strdup(" TP          ");
   namepar[22]=strdup(" TT0         ");
  };

void FAZerABu_EmpiABu::SetABuConst(double *par)
  {
   MatterAdiabat=(MatterIO *)(&ExpMat);
   ExpMat.Denc=par[4];
   ExpMat.A=par[5]; ExpMat.B=par[6]; ExpMat.EDist=par[7]; 
   ExpMat.G0c=fabs(par[8]); ExpMat.Sm=par[9]; ExpMat.Sn=par[10];
   ExpMat.Tmin=par[11];ExpMat.Umin=par[12];ExpMat.Umax=par[13];
   ExpMat.NumPnt=par[14];ExpMat.TA=par[15];ExpMat.TB=par[16];
   ExpMat.Gi=par[18];ExpMat.Ea=par[19];ExpMat.TL=par[20];
   ExpMat.TP=par[21]; ExpMat.TT0=par[22];
   ExpMat.SetHisch();
  }

double FAZerABu_EmpiABu::Clc(double *par,double Xval)
  {
   double DencRes,ERes,PRes,DRes,DencHug=par[2],EHug=par[3];
   SetABuConst(par);//MatterAdiabat=(MatterIO *)(&ExpMat);
   double PHug;PHug=PressurePorous(DencHug,EHug);
   double u=Xval;if (Xval>100) u-=100;
   if ((par[17]<0) && (Xval<0)) u=-u;
   Hug(fabs(u),DencHug,PHug,EHug,0,DencRes,PRes,ERes,DRes);
   if (par[17]>=0){ if (par[17]) return ExpMat.Sound(DencRes,ERes); else return DRes;}
   else {if (Xval<0) return ExpMat.Sound(DencRes,ERes); 
         else if (Xval<100) return DRes;else return ExpMat.Temperature(DencRes,ERes);}
  };

void   FAZerABu_EmpiABu::Clc(double *par,double *Xval,double *Res)
  {
   double DencRes,ERes,PRes,DRes,DencHug=par[2],EHug=par[3];
   SetABuConst(par);//MatterAdiabat=(MatterIO *)(&ExpMat);
   double PHug;PHug=PressurePorous(DencHug,EHug);
cout<<" PHug "<<PHug<<"\n";
   for (int k=1;k<=Xval[0];k++)
     {
      double u=Xval[k];if (Xval[k]>100) u-=100;
      if ((par[17]<0) && (Xval[k]<0)) u=-u;
      Hug(fabs(u),DencHug,PHug,EHug,0,DencRes,PRes,ERes,DRes);
      if (par[17]>=0){ if (par[17])  Res[k]=ExpMat.Sound(DencRes,ERes); else Res[k]=DRes;}
      else {if (Xval[k]<0) Res[k]=ExpMat.Sound(DencRes,ERes); 
           else if (Xval[k]<100) Res[k]=DRes;else 
Res[k]=ExpMat.Temperature(DencRes,ERes);}
//Res[k]=ERes;}
     } 
  };

void FAZerABu_EmpiABu::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
  if (stp[6]!=0) stp[6]*=0.1;
 };

//===========================================================
//======================= REGISTER    =======================
//===========================================================

int FAStateRegister(fstream &in,FAIO *&Func,char *name)
  {
   FAZer *SFunc=NULL;
   int ret=1;
//   if (!Stricmp(name,"h2cold")) {SFunc=(FAZer*)new FAZerH2cold;}
//                   else 
//   if (!Stricmp(name,"ABu_Grun")) {SFunc=(FAZer*)new FAZerABu_Grun;}
//                   else 
   if (!Stricmp(name,"ABu_H2cold")) {SFunc=(FAZer*)new FAZerABu_H2cold;}
                   else 
   if (!Stricmp(name,"ABu_Empicold")) {SFunc=(FAZer*)new FAZerABu_EmpiCold;}
                   else 
   if (!Stricmp(name,"ABu_EmpiMelt")) {SFunc=(FAZer*)new FAZerABu_EmpiMelt;}
                   else 
   if (!Stricmp(name,"ABu_EmpiABu")) {SFunc=(FAZer*)new FAZerABu_EmpiABu;}
                   else 
//   if (!Stricmp(name,"ABu_EmpiHug")) {SFunc=(FAZer*)new FAZerABu_EmpiHug;}
//                   else 
                   ret=0;
   if (!ret) return ret;
   if (Func!=NULL) delete(Func);
      FASingleIO *F=new FASingleIO;
      F->Func=SFunc;
      Func=(FAIO*)F;
   Func->Input(in);
//cout<<" Mem after FAStateRegister  "<<coreleft()<<"\n";
   return ret;
  };

