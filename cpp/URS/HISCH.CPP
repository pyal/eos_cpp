#include <lib\precompiled\eos.h>
#include "hisch.h"

#define Hisch H

double Ecold(double Sigma,HischConst &Hisch)
  {
   double ret,S,m=H.M,n=H.N;
   S=Sigma;
//   ret=H.B0c*H.V0c/(m-n)*(pow(S,m)/m-pow(S,n)/n)+H.Ed;
   ret=H.B0c*H.V0c/(m-n)*(pow(S,m)/m-pow(S,n)/n)+H.Ed;
   return ret;
  };
 
double Pcold(double Sigma,HischConst &Hisch)
  {
   double ret,S,m=H.M,n=H.N;
   S=Sigma;
   ret=H.B0c/(m-n)*(pow(S,m+1)-pow(S,n+1));
   return ret;
  };

double GrunC(double Sigma,HischConst &Hisch)
  {
   double ret,S,D23;
   S=Sigma;D23=2./3.;
   ret=D23+(H.G0c-D23)*(sqr(H.Sn)+sqr(log(H.Sm)))/(sqr(H.Sn)+sqr(log(S/H.Sm)));
   return ret;
  };

double Grun(double Sigma,double Energy,HischConst &Hisch)
  {
   double ret,S,D23,S1;
   S=Sigma;D23=2./3.;
   S1=S*H.V0/H.V0c;
   ret=H.Gi+(GrunC(S1,Hisch)-H.Gi)/( 1+pow(S,-D23)*(Energy-Ecold(S,Hisch))/H.Ea );
//cout<<" Denc "<<S/H.V0c<<" Sigma "<<S<<" Pc "<<Pcold(S,Hisch)<<
//      " Ec "<<Ecold(S,H)<<" G "<<ret<<"\n";
   return ret;
  } 

double Pres(double Dencity,double Energy,HischConst &Hisch)
  {
   double ret,S;
   S=H.V0c*Dencity;
   ret=Pcold(S,Hisch)+Grun(S,Energy,Hisch)*Dencity*(Energy-Ecold(S,Hisch));
//cout<<" Denc "<<S/H.V0c<<" Sigma "<<S<<" Pc "<<Pcold(S,Hisch)<<
//      " Ec "<<Ecold(S,H)<<" G "<<Grun(S,Energy,H)<<" Ed "<<H.Ed<<" P "<<ret<<"\n";
   return ret;
  } 

void Pres(double *Dencity,double *Energy,double *Pressure,HischConst &Hisch,int Num)
  {
   double S,Scoef=H.V0c,Pc,Ec,m=H.M,n=H.N,Ecoef,Pcoef,Ed=H.Ed,sm,sn;
   Ecoef=H.B0c*H.V0c/(m-n);Pcoef=H.B0c/(m-n);
   double D23=2./3.,Gc;
   double G0c=(H.G0c-D23)*(sqr(H.Sn)+sqr(log(H.Sm))),G0ca=sqr(H.Sn);
   double Sm=H.Sm,S1,S1coef=H.V0,Gi=H.Gi,G,D,E;
   for (int k=1;k<=Num;k++)
     {
      D=Dencity[k],E=Energy[k];
      S1=S1coef*D;
      S=Scoef*D;
      sm=pow(S,m);sn=pow(S,n);
      Ec=Ecoef*(sm/m-sn/n)+Ed;
      Pc=Pcoef*S*(sm-sn);
      Gc=D23+G0c/(G0ca+sqr(log(S1/Sm)));
      G=Gi+(Gc-Gi)/(1+pow(S,-D23)*(E-Ec)/H.Ea);
//cout<<" PcMiss "<<Pc-Pcold(S,Hisch)<<" EcMiss "<<Ec-Ecold(S,Hisch)<<" G_Miss "<<G-Grun(S,Energy[k],Hisch)<<"\n";
      Pressure[k]=Pc+G*D*(E-Ec);
     } 
  } 

// ==========================================================================
// =============  IVL EOS        ============================================
// ==========================================================================


//struct IvlConst
//  {  double DX,CMN,M,N,Ed,Gi,V0,GC,GGI,GF,QS,Sm,Ea; };
double Pres(double Dencity,double Energy,IvlConst &Ivl)
 {
  double E=Energy+Ivl.E0;
  double Sigma=Dencity*Ivl.V0;
  double SigmaCold=Ivl.DX*Sigma;
  double tmpM=pow(SigmaCold,Ivl.M),tmpN=pow(SigmaCold,Ivl.N);
  double Pc,Ec;
  Pc=Ivl.CMN*(tmpM-tmpN)*Dencity;
  Ec=Ivl.CMN*(tmpM/Ivl.M-tmpN/Ivl.N)+Ivl.Ed;

  double GrunCold=Ivl.GGI+Ivl.GC*Sigma/(pow(Sigma,Ivl.QS)+Ivl.Sm);
  double Grun= Ivl.Gi+Ivl.GF*(GrunCold-Ivl.Gi)/
                    ( 1+(E-Ec)/(Ivl.Ea*pow(SigmaCold,Ivl.GGI)) );
  return Pc+Grun*(E-Ec)/Dencity;
 }
   
void Pres(double *Dencity,double *Energy,double *Pressure,IvlConst &Ivl,int Num)
 {
  double E0=Ivl.E0,SigmaCoef=Ivl.V0,SigmaCoefCold=Ivl.DX,Ed=Ivl.Ed;
  double Ecoef=Ivl.CMN,m=Ivl.M,n=Ivl.N;
  double GcoldGGI=Ivl.GGI,GcoldGC=Ivl.GC,GcoldSm=Ivl.Sm,GcoldQS=Ivl.QS;
  double G_Gi=Ivl.Gi,G_GF=Ivl.GF,G_Ea=Ivl.Ea;
  double S,Sc,sm,sn,Ec,Pc,Gc,G,E,D;
   for (int k=1;k<=Num;k++)
     {
      D=Dencity[k],E=Energy[k]+E0;
      Sc=SigmaCoefCold*D;
      S=SigmaCoef*D;
      sm=pow(S,m);sn=pow(S,n);
      Ec=Ecoef*(sm/m-sn/n)+Ed;
      Pc=Ecoef*D*(sm-sn);
      Gc=GcoldGGI+GcoldGC*S/(pow(S,GcoldQS)+GcoldSm);
      G= G_Gi+G_GF*(Gc-G_Gi)/( 1+(E-Ec)/(G_Ea*pow(Sc,GcoldGGI)) );
      Pressure[k]=Pc+G*D*(E-Ec);
     } 

 }