#include <lib/precompiled/eos.h>
#include "url_equ.h"

UrlParamStruct *UrlPar;
#define UP (*UrlPar)

//double UrlDebyFuncInt(double x) {if (x==0) return 0;return x*x*x/(exp(x)-1);};
//double UrlDebyFunc(double x) 
//  {return 3/(x*x*x)*RecInt(UrlDebyFuncInt,0,x,StndErr);};

double UrlPcold(double x)
  {
      double s=0;
      for (int k=1;k<=UP.Nparam;k++) 
        s+=UP.coef[k]*pow(x,k/3.+1);
      return s;
  };

double UrlTeta(double x)
  {
//    double s1=0,s2=0,Add=3-2*UP.GrunN;
//    for (int k=1;k<=UP.Nparam;k++) 
//      {
//       s1+=UP.coef[k]*pow(x,k/3.+1)*(k+Add);
//       s2+=UP.coef[k]*(k+3);
//      }
//    if ( (fabs(s1)<MathZer) || (s1*s2<MathZer) )    return 0;
//    return UP.Teta0*pow(x,-1./6.)*sqrt(s1/s2);
   double pc=UrlPcold(x);
   double c2=(UrlPcold(x+StndErr)-pc)/(StndErr*UP.Denc);
   return UP.Teta0*pow(x,1./3)*sqrt(max<double>(c2-UP.GrunN*2*pc/(3*UP.Denc*x),StndErr));
  };

double UrlZparam(double x,double T)
  { 
//    double s=0,Add=3-2.*UP.GrunN;
//    for (int k=1;k<=UP.Nparam;k++) 
//       s+=UP.coef[k]*pow(x,k/3.+1)*(k+Add); 
//    s=s/(3*x*UP.Denc);
//    return UP.Lconst*UP.Rstat*T/s; 
   double pc=UrlPcold(x);
   double c2=(UrlPcold(x+StndErr)-pc)/(StndErr*UP.Denc);
   return UP.Lconst*UP.Rstat*T/(c2-UP.GrunN*2*pc/(3*UP.Denc*x));
  };


double UrlEcold(double x)
  {
      double s1=0;
      for (int k=1;k<=UP.Nparam;k++) 
         s1+=UP.coef[k]*(pow(x,k/3.)-1)/k;
      return s1*3/UP.Denc;
  };
double UrlFel(double Denc,double T) 
  {
   if (!UP.IncludeElektron) return 0;
   else 
     {
      double R=UP.Rstat;
      double x=Denc/UP.Denc,ret;
      double Na=6.023e23;
      if (UP.ElektronAr)
        ret=-4*R*T/Denc*pow(UP.PowCoef*T*pow(x,2./3-UP.Gconst),3./2)
                       *exp(-UP.Wconst*pow(x,-UP.Gconst)*0.5/T)
                       *UP.MolWeight/Na ;
      else 
        ret=-4*R*T/Denc*pow(UP.PowCoef*T*pow(x,2./3-UP.Gconst),3./2)
                       *exp(-UP.Wconst*log(UP.DencMet/Denc)*0.5/T)
                       *UP.MolWeight
                       *sqrt(UP.Degeneration)/Na;
      return ret;
     };
  };               
double FUrlLiquid(double Denc,double T,UrlParamStruct *Url) 
  {
   UrlPar=Url;
   double x=Denc/UP.Denc,AddT;
   double Ec=UrlEcold(x),Te=UrlTeta(x),z=UrlZparam(x,T),b=UP.Bconst,
          T0=UP.T0const,TetaTurn=UP.T_Turn,r=UP.Rconst;
   double f;
   if (UP.H2Liquid)
     f=UP.Cconst+UP.Aconst/r*(pow(Denc/UP.Denc0const,r)-1);
   else 
     f=UP.Cconst+UP.Aconst*pow(Denc/UP.Denc0const,r)+UP.BLarge*log(Denc/UP.Denc0const);
//     f=UP.Cconst+UP.Aconst*pow(x,r)+UP.BLarge*log(x);
   if (UP.IncludeTurnP!=0) TetaTurn*=pow(Denc/UP.DencH,2./3);
   if (UP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   double F=Ec*UP.IncludeZeroE+UP.Rstat*
        (UP.IncludeNullE*9/8*Te+3*T*(log(Te/T*sqrt(max<double>(1+z,StndErr)))-b)+
                                3*T0*f-T*AddT)+UrlFel(Denc,T);
   return F;
  };
double FUrlSolid(double Denc,double T,UrlParamStruct *Url) 
  {
   UrlPar=Url;
   double x=Denc/UP.Denc,AddT;
   double Ec=UrlEcold(x),Te=UrlTeta(x),TetaTurn=UP.T_Turn,
          Deby=DebyFunc(Te/T);
   if (UP.IncludeTurnP!=0) TetaTurn*=pow(Denc/UP.DencH,2./3);
   if (UP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   if (Te/T>10) Deby=-Deby;else Deby= 3*log(1-exp(-Te/T))-Deby;
cout<<" ZE "<<Ec*UP.IncludeZeroE<<" D "<<Denc<<" x "<<x<<" UP.Denc "<<UP.Denc<<"\n";
   double F=Ec*UP.IncludeZeroE+UP.Rstat*
        (9/8*Te*UP.IncludeNullE+T*(Deby-AddT))+UrlFel(Denc,T);
   return F;
  };


void UrlReadPar(istream &in,UrlParamStruct *Url) 
  {
   UrlPar=Url;
			char tmp[256];
   in>>tmp>>UP.Denc>>tmp>>UP.DencH>>tmp>>UP.GrunN>>tmp>>UP.Teta0;
   in>>tmp>>UP.Nparam;
   for (int k=1;k<=UP.Nparam;k++) in>>tmp>>UP.coef[k];
   in>>tmp>>UP.B0c>>tmp>>UP.Mconst>>tmp>>UP.Nconst>>tmp>>UP.Edist;
   in>>tmp>>UP.Aconst>>tmp>>UP.Bconst>>tmp>>UP.Cconst>>tmp>>UP.Rconst
      >>tmp>>UP.Lconst>>tmp>>UP.BLarge
      >>tmp>>UP.MolWeight>>tmp>>UP.Wconst>>tmp>>UP.Gconst>>tmp>>UP.PowCoef
      >>tmp>>UP.DencMet>>tmp>>UP.Degeneration
      >>tmp>>UP.Denc0const>>tmp>>UP.T0const>>tmp>>UP.T_Turn
      >>tmp>>UP.IncludeTurnE>>tmp>>UP.IncludeNullE>>tmp
      >>UP.IncludeTurnP>>tmp>>UP.IncludeNullP>>tmp>>UP.IncludeZeroE
      >>tmp>>UP.IncludeElektron>>tmp>>UP.ElektronAr>>tmp>>UP.H2Liquid
      >>tmp>>UP.SingleCold;
   UP.Rstat=8.31441e-3/UP.MolWeight;  // Free energy per gramm
   UP.Denc0const*=UP.Denc;            // Given sigma
   UP.Wconst*=1.1605e4;               // Given energy in ev to Kelvin
   UP.PowCoef=2*M_PI*9.109e-28*1.38e-16/sqr(6.625e-27);  // unit cm/K
  };
void UrlSavePar(ostream &out,UrlParamStruct *Url) {
   UrlPar=Url;
   out<<" UP.Denc "<<UP.Denc <<" UP.DencH "<<UP.DencH <<" UP.GrunN "<<UP.GrunN 
       <<" UP.Teta0 "<<UP.Teta0 <<" UP.Nparam "<<UP.Nparam;
   for (int k=1;k<=UP.Nparam;k++) out<<" par "<<UP.coef[k];out<<"\n";
   out<<" UP.B0c "<<UP.B0c <<" UP.Mconst "<<UP.Mconst <<" UP.Nconst "<<UP.Nconst 
       <<" UP.Edist "<<UP.Edist <<" UP.Aconst "<<UP.Aconst <<" UP.Bconst "<<UP.Bconst
       <<" UP.Cconst "<<UP.Cconst <<" UP.Rconst "<<UP.Rconst <<" UP.Lconst "<<UP.Lconst
       <<" UP.BLarge "<<UP.BLarge <<" UP.MolWeight "<<UP.MolWeight <<" UP.Wconst "<<UP.Wconst
       <<" UP.Gconst "<<UP.Gconst <<" UP.PowCoef "<<UP.PowCoef <<" UP.DencMet "<<UP.DencMet
       <<" UP.Degeneration "<<UP.Degeneration <<" UP.Denc0const "<<UP.Denc0const 
       <<" UP.T0const "<<UP.T0const <<" UP.T_Turn "<<UP.T_Turn <<" UP.IncludeTurnE "<<UP.IncludeTurnE
       <<" UP.IncludeNullE "<<UP.IncludeNullE <<" UP.IncludeTurnP "<<UP.IncludeTurnP
       <<" UP.IncludeNullP "<<UP.IncludeNullP <<" UP.IncludeZeroE "<<UP.IncludeZeroE
       <<" UP.IncludeElektron "<<UP.IncludeElektron <<" UP.ElektronAr "<<UP.ElektronAr
       <<" UP.H2Liquid "<<UP.H2Liquid <<" UP.SingleCold "<<UP.SingleCold<<" ";
  };



/*



// ====================================================
// =============== End Cold Param  ====================
// ====================================================

double Esolid(double x,double T)  
  {
   double R=UP.Rconst,teta=Teta(x);
   return Ecold(x)+R*(UP.IncludeNullE*teta*9/8+
                          3*T*DebyFunc(teta/T))+
                          UP.IncludeTurnE*R*T;
  };                                            
   
double Psolid(double x,double T)
  {
   double G=Grun(x),R=UP.Rconst,D=x*UP.Denc,teta=Teta(x);
   return Pcold(x)+R*D*G*(UP.IncludeNullP*teta*9/8+
                          3*T*DebyFunc(teta/T))+
                          UP.IncludeTurnP*R*D*T*2/3;
  };                                            

double CCCa,CCCteta;
double MisT(double T)
  {return CCCa-T*UP.Rconst*(3*DebyFunc(CCCteta/max<double>(T,StndErr))+UP.IncludeTurnE);}
double TemperatureS(double r,double e)
  {
   double x=r/ColdParam.Denc;
   double teta=Teta(x),T=1000;
   double de=e-Ecold(x)-UP.IncludeNullE*ColdParam.Rconst*9*teta/8;
   CCCa=de;CCCteta=teta;
   if (!ZeroNewtonIt(MisT,T,StndErr*1e-3,10000,1e5,MathZer,T/2))
        {cout<<" Bad TemperatureS in H2Solid Not Zero"<<MisT(T)<<"\n";}
   return T;
  };
double PressureS(double r,double e)
  {  return Psolid(r/UP.Denc,TemperatureS(r,e)); };







double Pliquid(double x,double T)
  {
   double G=Grun(x),z=Zparam(x,T),R=ColdParam.Rconst,D=x*ColdParam.Denc,teta=0;
			if (LiquidParam.IncludeNullP!=0) teta=Teta(x);
   return Pcold(x)+3*R*D*(T*(G+z/3.)/(1+z)+
                       LiquidParam.T0const*LiquidParam.Aconst*
                       pow(D/LiquidParam.Denc0const,LiquidParam.Rconst))+
																							LiquidParam.IncludeNullP*G*teta*9/8*R*D+
																							LiquidParam.IncludeTurnP*2/3*R*D*T;
  };

double Eliquid(double x,double T)
  {
   double z=Zparam(x,T),D=x*ColdParam.Denc,R=ColdParam.Rconst,teta=0;
			if (LiquidParam.IncludeNullE!=0) teta=Teta(x);
   return Ecold(x)+3*R*(T*(1+z/2)/(1+z)+
          LiquidParam.T0const*(LiquidParam.Cconst+
                LiquidParam.Aconst/LiquidParam.Rconst*
                  (pow(D/LiquidParam.Denc0const,LiquidParam.Rconst)-1)))+
																							LiquidParam.IncludeNullE*teta*R*9/8+
																							LiquidParam.IncludeTurnE*R*T;
  };
double FindTemp(double x,double DeltaE)
  {
   double D=x*ColdParam.Denc;
   double teta=0;
			if (LiquidParam.IncludeNullE!=0) teta=Teta(x);
			double a=Zparam(x,1),R=ColdParam.Rconst;
			double f=3*R,b=R*LiquidParam.IncludeTurnE,d=R*9*teta/8*LiquidParam.IncludeNullE,
          c=3*R*LiquidParam.T0const*(LiquidParam.Cconst+
                LiquidParam.Aconst/LiquidParam.Rconst*
                (pow(D/LiquidParam.Denc0const,LiquidParam.Rconst)-1));

   double T1,T2,u=c-DeltaE+d;
   if ( !SqPolynom(a*(0.5*f+b),f+b+a*u,u,T1,T2) ) 
       cout<<"Could not Find Temp\n" ;
//cout<<" FindTemp  x "<<x<<" Tf  "<<T1<<" Tw "<<T2<<"\n";
//   return T1;
   return max<double>(T1,T2);

  };
double PressureL(double r,double e)
  {
   double x=r/ColdParam.Denc;
   if (x<=0) { cout<<" Error x in pressureL\n";return 0;}
//   if (x>1)
     {
      double DeltaE=e-Ecold(x);
//cout<<" X "<<x<<" DeltaE "<<DeltaE<<"  ";
      return Pliquid( x,FindTemp(x,DeltaE) );
     }
  };
double TemperatureL(double r,double e)
  {
   double x=r/ColdParam.Denc;
   return FindTemp(x,e-Ecold(x));
  }

double GetH2Dencity(){return ColdParam.Denc;};

void SetConstants()
  {
//   ColdParam.Rconst=8.31441e-3;
   ColdParam.Rconst=4.157e-3; // True one
//   ColdParam.Rconst=4.12664e-3;      // To get Urlin
//   ColdParam.Rconst=2.0765e-3;
   ColdParam.Nparam=4;

//   Grigoriev - current paper param
     ColdParam.coef[1]=-0.557;ColdParam.coef[2]=2.871;
     ColdParam.coef[3]=-7.071;ColdParam.coef[4]=4.557; // P in GPa
     ColdParam.Denc=0.1335;                            // in g/cm^3
     ColdParam.DencH=0.89;  // For Turn F              // in g/cm^3
//     ColdParam.Denc=0.098;                               // in g/cm^3
     ColdParam.Teta0=115;                              // in K from Trubicin?
//   Grigoriev - current paper param


   ColdParam.GrunN=1;

  LiquidParam.Aconst=9.247;
  LiquidParam.Bconst=1.8663;  //            ? Don't used in formulas.
  LiquidParam.Cconst=1.2593;
  LiquidParam.Rconst=1;
  LiquidParam.Lconst=21;
//  LiquidParam.Teta0const=0.579;  LiquidParam.Denc0const=0.579*0.1335=0.0772965;
  LiquidParam.Denc0const=0.0772965;
  LiquidParam.T0const=14;
  LiquidParam.T_Turn=85.4;
//  t(krit)=33.23  p(krit)=13.16  v(krit)=63.8  Esub=0.7 kDj/g
  };
double FLiquid(double Denc,double T)
  {
   double x=Denc/UP.Denc;
   double Fsol=FSolid(Denc,T);
   double z=Zparam(x,T),b=UP.Bconst,T0=UP.T0const,r=UP.Rconst;
   double f=UP.Cconst+UP.Aconst/r*(pow(Denc/UP.Denc0const,r)-1);

   return Fsol-3*r*T*(-0.5*log(1+z)+b-f*T0/T);
  };
*/
