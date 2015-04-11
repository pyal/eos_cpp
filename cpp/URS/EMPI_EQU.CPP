#include <lib\precompiled\eos.h>
#include "empi_equ.h"

EmpiParamStruct *EmpiColdParam;
#define CP (*EmpiColdParam)

//double DebyFuncInt(double x) 
//  {if (x==0) return 0;if (x>20) return 0;return x*x*x/(exp(x)-1);};
//double DebyFunc(double x) 
//  {return 3/(x*x*x)*RecInt(EmpiDebyFuncInt,0,x,StndErr);};

double EmpiPcold(double x)
  {
   if ( (CP.SingleCold) || (x>=1) )
     {
      double s=0;for (int k=1;k<=CP.Nparam;k++)
      { s+=CP.coef[k]*pow(x,k/3.+1);
//cout<<" CP.coef[k] "<<CP.coef[k]<<"\n";
      }
      return s;
     } 
   else 
     { 
      return CP.B0c/(CP.Mconst-CP.Nconst)*(pow(x,CP.Mconst+1)-pow(x,CP.Nconst+1));
     }                
  };
double EmpiGrun(double x)
  {
//  тогда грюнайзен    =-Sum(k*Ak*X^k) ?   =-Sum(k*Ak*X^(k-1))
   double l=-log(x*CP.TetaV*CP.Denc);
   double s=0;for(int k=0;k<CP.NTeta;k++) s-=k*CP.TetaPar[k+1]*pow(l,k-1);
   return s;
  };

double EmpiTeta(double x)
  {
//  Дебай - сумма ряда = Sum(exp(Ak*X^k)) X=log(V/Va)
   double l=-log(x*CP.TetaV*CP.Denc);
   double s=1;for(int k=0;k<CP.NTeta;k++) 
   {s*=exp(CP.TetaPar[k+1]*pow(l,k));
//cout<<" CP.TetaPar[k+1] "<<CP.TetaPar[k+1]<<" pow(l,k) "<<pow(l,k)<<" S "<<s<<"\n";
   }
   return s;
  };

double EmpiZparam(double x,double T)
  { 
      double k=CP.Lconst*CP.Rconst*T;
//cout<<" Z "<<CP.TetaZ0<<"\n";
      return k*CP.TetaZ0*sqr(pow(x,1./3.)/EmpiTeta(x));
  };


double EmpiEcold(double x)
  {
   if ( (CP.SingleCold) || (x>=1) )
     {
      double s=0;for (int k=1;k<=CP.Nparam;k++) s+=CP.coef[k]*(pow(x,k/3.)-1)/k;
      return s*3/CP.Denc;
     }
   else 
     { 
      return CP.Edist+CP.B0c/((CP.Mconst-CP.Nconst)*CP.Denc)*
                     (pow(x,CP.Mconst)/CP.Mconst-pow(x,CP.Nconst)/CP.Nconst);
     }                
  };

// ====================================================
// =============== End Cold Param  ====================
// ====================================================
///*
double EmpiFreeEMet(double Denc,double T)
    {
//                     B       A
//  F= Zero-R*T*ln(1- ---) - ----- - R*T*ln(V) - R*T*C *ln(T) - HiT*R*T
//                     V       V                      v
if ((T<MathZer) || (Denc<MathZer)) { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
     double Rs=pow(2.696/Denc,1./3.);
     double K_Ryd=6.3344e-6;
     double kT=T*K_Ryd,Ze=1,G14,F=0,e=-2.59,sige=-2.6;
     G14=pow(sqr(Ze)/(Rs*kT/2),1./4.);  // Ry= 2* 1 a.e.e
//     if (AddEG) 
       F+=2.21/sqr(Rs)-0.916/Rs-0.88/(Rs+7.8)-1.792/Rs;
//     if (AddLDA) 
       F+=(-0.11382+0.003554*Rs-0.012707*sqr(Rs));
//     if (AddOCP) F+=(4*(0.9528*G14-0.18907/G14)-0.81773*4*log(G14)-e )*kT;
     F+=sige;
//     if (AddDeb) 
//       {double TD=max((Denc-DebDenc0)*DebKoef,100);
//cout<<" D "<<Denc<<" T "<<T<<" TD "<<TD<<"\n";
//        F+=kT*(3*log(1-exp(-TD/T))-DebyFunc(TD/T));
//       }; 
     F*=1313;  // Conversion from Ry to (km/c)^2
     return F;
    };
double EmpiFLiq0(double Denc)
  {
   double x=Denc/CP.Denc;//,Vib;//,Te1;//,Deby;//AddT,
   double Ec=EmpiEcold(x),Te=EmpiTeta(x),r=CP.RLconst,T0=CP.T0const;
   double xl=Denc/CP.Denc0const,f=CP.Cconst+CP.Aconst/r*(pow(xl,r)-1);
   if (CP.BushmanMelt) f*=2*sqr(xl)/(sqr(xl)*xl+1);
   double F=Ec*CP.IncludeZeroE+CP.Rconst*(CP.IncludeNullE*9/8*Te+3*T0*f);
   return F;
  };
double EmpiFLiq00(double Denc)
  {
   double x=Denc/CP.Denc;
   double Ec=EmpiEcold(x),Te=EmpiTeta(x);
   double F=Ec*CP.IncludeZeroE+CP.Rconst*CP.IncludeNullE*9/8*Te;
   return F;
  };


   
double EmpiFDisDegree(double Denc,double T,EmpiParamStruct *Par)
  {
   EmpiColdParam=Par;                                                  
   if (!CP.IncludeDis) return EmpiFLiquid(Denc,T,Par);                 
   EmpiColdParam=Par;                                                  
   if (T<MathZer) T=StndErr;                                           
   double DeltaE;//Dis,
   double Fliq=EmpiFLiquid(Denc,T,Par),dr=StndErr*Denc;
   double Pressure=max(sqr(Denc)/dr*(EmpiFLiquid(Denc+dr,T,Par)-Fliq),0);
   double Dd=CP.DencityMet*exp(-Pressure/CP.PressureMet)*Denc;
   DeltaE=EmpiFreeEMet(Denc,1)-EmpiFLiq0(Denc+Dd)+CP.DisEnergy;
   double Z=CP.Rconst*T;                                               
   return exp(-DeltaE/Z)/(1+exp(-DeltaE/Z));
  }; 
// This is a paper version
double EmpiFDis(double Denc,double T,EmpiParamStruct *Par)
  {
   EmpiColdParam=Par;                                                  
   if (!CP.IncludeDis) return EmpiFLiquid(Denc,T,Par);                 
   EmpiColdParam=Par;                                                  
   if (T<MathZer) T=StndErr;                                           
   double DeltaE;//Dis,
   double Fliq=EmpiFLiquid(Denc,T,Par),dr=StndErr*Denc;
   double Pressure=max(sqr(Denc)/dr*(EmpiFLiquid(Denc+dr,T,Par)-Fliq),0);
//   DeltaE=EmpiFreeEMet(Denc,1)-EmpiFLiq0(Denc)+CP.DisEnergy;
   double Dd=CP.DencityMet*exp(-Pressure/CP.PressureMet)*Denc;
   DeltaE=EmpiFreeEMet(Denc,1)-EmpiFLiq0(Denc+Dd)+CP.DisEnergy;
   double Z=CP.Rconst*T;                                               
   return Fliq-Z*log(1+exp(-DeltaE/Z));             

  };

double EmpiFLiquid(double Denc,double T,EmpiParamStruct *Par)
  {
   EmpiColdParam=Par;
   double x=Denc/CP.Denc,AddT,Vib,Te1,Deby;
   double Ec=EmpiEcold(x),Te=EmpiTeta(x),z=EmpiZparam(x,T),b=CP.Bconst,
          T0=CP.T0const,TetaTurn=CP.T_Turn,r=CP.RLconst;
   double xl=Denc/CP.Denc0const,f=CP.Cconst+CP.Aconst/r*(pow(xl,r)-1);
   if (CP.BushmanMelt) f*=2*sqr(xl)/(sqr(xl)*xl+1);
   if (CP.IncludeTurnP!=0) TetaTurn*=pow(Denc/CP.DencH,2./3);
   if (CP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   Te1=CP.VibTeta/T*sqrt(1+z*CP.VibZCoef);//*Te/CP.VibTeta);
//   Te1=CP.VibTeta/T*sqrt(1+z*CP.VibZCoef*sqr(Te/CP.VibTeta));
//cout<<" Zcoef "<<CP.VibZCoef*min(1,sqrt(2)*Te/CP.VibTeta)<<"\n";
   if (CP.IncludeVibration==0) Vib=0;
   else if (Te1>10) Vib=0;else  Vib=log(1-exp(-Te1));
   Te1=Te/T*sqrt(1+z);Deby=DebyFunc(Te1);
   if (Te1>10) Deby=-Deby;else Deby= 3*log(1-exp(-Te1))-Deby;
   double F=Ec*CP.IncludeZeroE+CP.Rconst*
        (CP.IncludeNullE*9/8*Te+T*(Deby-b+Vib)+
                                3*T0*f-T*AddT);
   return F;
  };
//------------------------------------------------------------------
//    Consructing now, idea - minimum function, include  pressure.
//------------------------------------------------------------------

//------------------------------------------------------------------
double EmpiDenc,EmpiT,EmpiDenc0,EmpiDencCoef;
double EmpiTe,EmpiZ,EmpiEc,EmpiDelta;
double EmpiDeltaE(double DisX)
  { 
   EmpiDenc=(1+DisX*EmpiDencCoef)*EmpiDenc0;
   double x=EmpiDenc/CP.Denc;
   EmpiEc=EmpiEcold(x);EmpiZ=EmpiZparam(x,EmpiT);EmpiTe=EmpiTeta(x);
   return EmpiFreeEMet(EmpiDenc,1)-EmpiFLiq0(EmpiDenc)+CP.DisEnergy;
  }; 
double EmpiDisXExpr(double DisX)
  {
   DisX=sqrt(DisX);
   double Denc=EmpiDenc,T=EmpiT,x=Denc/CP.Denc,AddT,Vib,Te1,Deby;
   double Ec=EmpiEc,Te=EmpiTe*DisX,z=EmpiZ,b=CP.Bconst,
          T0=CP.T0const,TetaTurn=CP.T_Turn,r=CP.RLconst;
   double xl=Denc/CP.Denc0const,f=CP.Cconst+CP.Aconst/r*(pow(xl,r)-1);
   if (CP.BushmanMelt) f*=2*sqr(xl)/(sqr(xl)*xl+1);
   if (CP.IncludeTurnP!=0) TetaTurn*=pow(Denc/CP.DencH,2./3)*DisX;
   if (TetaTurn<StndErr) TetaTurn=StndErr;
   if (CP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   Te1=CP.VibTeta/T*sqrt(1+z*CP.VibZCoef)*DisX;//*Te/CP.VibTeta);
//   Te1=CP.VibTeta/T*sqrt(1+z*CP.VibZCoef*sqr(Te/CP.VibTeta));
//cout<<" Zcoef "<<CP.VibZCoef*min(1,sqrt(2)*Te/CP.VibTeta)<<"\n";
   if (CP.IncludeVibration==0) Vib=0;
   else if (Te1>10) Vib=0;else  if (Te1<StndErr) Vib=Te1;else Vib=log(1-exp(-Te1));
   Te1=Te/T*sqrt(1+z);
   if (Te1<StndErr) Deby=3*Te1;
   else 
     {
      Deby=DebyFunc(Te1);
      if (Te1>10) Deby=-Deby;
      else Deby= 3*log(1-exp(-Te1))-Deby;
     } 
   double F=Ec*CP.IncludeZeroE+CP.Rconst*
        (CP.IncludeNullE*9/8*Te+T*(Deby-b+Vib)+
                                3*T0*f-T*AddT);
   return F;
  };
double EmpiFindMinExpr(double DisX)
  {
   double DeltaE=EmpiDeltaE(DisX);
//   double DeltaE=EmpiDelta;
   return DisX*(EmpiDisXExpr(DisX)+DeltaE)+(1-DisX)*EmpiDisXExpr(1-DisX);
  }; 
//------------------------------------------------------------------
// This is a current version
double EmpiFDisPresDegree(double Denc,double T,EmpiParamStruct *Par)
  {   
   EmpiColdParam=Par;if (!CP.IncludeDis) return 0;
   if (T<MathZer) T=StndErr;
//   double x=Denc/CP.Denc;EmpiEc=EmpiEcold(x);EmpiZ=EmpiZparam(x,T);EmpiTe=EmpiTeta(x);
   EmpiT=T;EmpiDenc0=Denc;
   EmpiDencCoef=CP.IncludeDisPres*
                 EvalSpl(*(CP.Dis_Pnt),0,1,*(CP.Dis_Sp),Denc);
   double DisX;//=MinFunc(0,1,EmpiFindMinExpr,StndErr*0.1,30);
   double tmp;
   if (!NAGMin(EmpiFindMinExpr,StndErr*0.1,StndErr*0.1,0,1,1000,DisX,tmp))
      cout<<" Did not find Tmin. Try old version\n";
//cout<<" DisDegree "<<DisX<<" T "<<T<<" Denc "<<Denc<<"\n";
	return DisX;
  }; 


double EmpiFDisPres(double Denc,double T,EmpiParamStruct *Par)
  {
   EmpiColdParam=Par;if (!CP.IncludeDis) return EmpiFLiquid(Denc,T,Par);                 
   if (T<MathZer) T=StndErr;
//   double x=Denc/CP.Denc;EmpiEc=EmpiEcold(x);EmpiZ=EmpiZparam(x,T);EmpiTe=EmpiTeta(x);
   EmpiT=T;EmpiDenc0=Denc;
   EmpiDencCoef=CP.IncludeDisPres*
                 EvalSpl(*(CP.Dis_Pnt),0,1,*(CP.Dis_Sp),Denc);
   double DisX;//=MinFunc(0,1,EmpiFindMinExpr,StndErr*0.1,30),DeltaE=EmpiDeltaE(DisX);
   double tmp;
   if (!NAGMin(EmpiFindMinExpr,StndErr*0.1,StndErr*0.1,0,1,1000,DisX,tmp))
      cout<<" Did not find Tmin. Try old version\n";
   double DeltaE=EmpiDeltaE(DisX);
//        DelMinFunc();
//cout<<" DisX "<<DisX;
   return DisX*(EmpiDisXExpr(DisX)+DeltaE)+(1-DisX)*EmpiDisXExpr(1-DisX);
  };           
/*
double EmpiFLiquid(double Denc,double T,EmpiParamStruct *Par)
  {
   EmpiColdParam=Par;
   double x=Denc/CP.Denc,AddT,Vib,Te1;
   double Ec=EmpiEcold(x),Te=EmpiTeta(x),z=EmpiZparam(x,T),b=CP.Bconst,
          T0=CP.T0const,TetaTurn=CP.T_Turn,r=CP.RLconst;
   double f=CP.Cconst+CP.Aconst/r*(pow(Denc/CP.Denc0const,r)-1);

   if (CP.IncludeTurnP!=0) TetaTurn*=pow(Denc/CP.DencH,2./3);
   if (CP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   Te1=CP.VibTeta/T;
   if (CP.IncludeVibration==0) Vib=0;
   else if (Te1>10) Vib=0;else  Vib=log(1-exp(-Te1));
   double F=Ec*CP.IncludeZeroE+CP.Rconst*
        (CP.IncludeNullE*9/8*Te+3*T*(log(Te/T*sqrt(1+z))-b+Vib)+
                                3*T0*f-T*AddT);
   return F;
  };
double EmpiFLiquid(double Denc,double T,EmpiParamStruct *Par)
  {
   EmpiColdParam=Par;
   double x=Denc/CP.Denc,AddT,Vib;
   double Ec=EmpiEcold(x),Te0=EmpiTeta(x),TetaTurn=CP.T_Turn;
   double z=EmpiZparam(x,T),b=CP.Bconst,T0=CP.T0const,r=CP.RLconst;
   double f=CP.Cconst+CP.Aconst/r*(pow(Denc/CP.Denc0const,r)-1);
   double Te=Te0*sqrt(1+z);
   double Deby=EmpiDebyFunc(Te/T);
   if (CP.IncludeTurnP!=0) TetaTurn*=pow(Denc/CP.DencH,2./3);
   if (CP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   if (CP.IncludeVibration==0) Vib=0;
//   else Vib=log(1-exp(-CP.VibTeta/T))-EmpiDebyFunc(CP.VibTeta/T)/3.;
   else if (CP.VibTeta/T>10) Vib=0;else  Vib=log(1-exp(-CP.VibTeta/T));
   if (Te/T>10) Deby=-Deby;else Deby= 3*log(1-exp(-Te/T))-Deby;
   double F=Ec*CP.IncludeZeroE+CP.Rconst*
        (9/8*Te0*CP.IncludeNullE+T*(Deby-AddT-3*b+Vib)+3*T0*f);
//cout<<" FS "<<F<<"\n";
   return F;
  };
*/
double EmpiFSolid(double Denc,double T,EmpiParamStruct *Par)
  {
   EmpiColdParam=Par;
   double x=Denc/CP.Denc,AddT,Vib;
   double Ec=EmpiEcold(x),Te=EmpiTeta(x),TetaTurn=CP.T_Turn,
          Deby=DebyFunc(Te/T);
   if (CP.IncludeTurnP!=0) TetaTurn*=pow(Denc/CP.DencH,2./3);
   if (CP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   if (CP.IncludeVibration==0) Vib=0;
//   else Vib=log(1-exp(-CP.VibTeta/T))-EmpiDebyFunc(CP.VibTeta/T)/3.;
   else if (CP.VibTeta/T>10) Vib=0;else  Vib=log(1-exp(-CP.VibTeta/T));
   if (Te/T>10) Deby=-Deby;else Deby= 3*log(1-exp(-Te/T))-Deby;
   double F=Ec*CP.IncludeZeroE+CP.Rconst*
        (9/8*Te*CP.IncludeNullE+T*(Deby-AddT+Vib));
//cout<<" FS "<<F<<"\n";
   return F;
  };


void EmpiH2ReadPar(istream &in,EmpiParamStruct *Par)
  {
			char tmp[256];
   EmpiColdParam=Par;
   in>>tmp>>CP.Denc>>tmp>>CP.DencH>>tmp>>CP.GrunN>>tmp>>CP.Teta0;
			in>>tmp>>CP.Nparam;
			for (int k=1;k<=CP.Nparam;k++) in>>tmp>>CP.coef[k];
   in>>tmp>>CP.B0c>>tmp>>CP.Mconst>>tmp>>CP.Nconst>>tmp>>CP.Edist;
   in>>tmp>>CP.TetaV>>tmp>>CP.NTeta;
   for ( int k=1;k<=CP.NTeta;k++) in>>tmp>>CP.TetaPar[k];
   in>>tmp>>CP.Rconst>>tmp>>CP.VibTeta>>tmp>>CP.VibZCoef;
   in>>tmp>>CP.DisEnergy>>tmp>>CP.PressureMet>>tmp>>CP.DencityMet;

   in>>tmp>>CP.Aconst>>tmp>>CP.Bconst>>tmp>>CP.Cconst>>tmp>>CP.RLconst
      >>tmp>>CP.Lconst>>tmp>>CP.Denc0const>>tmp>>CP.T0const>>tmp>>CP.T_Turn
      >>tmp>>CP.IncludeTurnE>>tmp>>CP.IncludeNullE>>tmp
      >>CP.IncludeTurnP>>tmp>>CP.IncludeNullP>>tmp>>CP.IncludeZeroE
      >>tmp>>CP.SingleCold>>tmp>>CP.IncludeVibration>>tmp>>CP.BushmanMelt
      >>tmp>>CP.IncludeDis>>tmp>>CP.IncludeDisPres;
   in>>tmp>>CP.DataName;// DisPres Name
   if (CP.IncludeDisPres!=0)
     {
      InputTDataF(tmp,CP.Dis_Pnt);
      if (CP.Dis_Pnt->N!=2) { cout<<" Error in loading Dis file. exiting.\n";exit(1);}
      ConstructSpl(*(CP.Dis_Pnt),0,1,CP.Dis_Sp);
     } 
   double PrPl=EmpiPcold(1+1e-6),Pr=EmpiPcold(1);
//cout<<" REad OK  Pup "<<PrPl<<" Plow "<<Pr<<" CP.Teta0 "<<CP.Teta0<<"\n";
   CP.TetaZ0=sqr(CP.Teta0)*1e-6/(PrPl-Pr);
//cout<<" REad OK CP.TetaZ0 "<<CP.TetaZ0<<" CP.Teta0 "<<CP.Teta0<<"\n";
  };

void EmpiH2SavePar(ostream &out,EmpiParamStruct *Par)
  {
   EmpiColdParam=Par;
   out<<" CP.Denc "<<CP.Denc<<" CP.DencH "<<CP.DencH<<" CP.GrunN "<<CP.GrunN
       <<" CP.Teta0 "<<CP.Teta0<<" CP.Nparam "<<CP.Nparam;
	for (int k=1;k<=CP.Nparam;k++) out<<"  "<<CP.coef[k];
    out<<"\n";
    out<<" CP.B0c "<<CP.B0c<<" CP.Mconst "<<CP.Mconst<<" CP.Nconst "<<CP.Nconst
        <<" CP.Edist "<<CP.Edist<<" CP.TetaV "<<CP.TetaV <<" CP.NTeta "<<CP.NTeta; 
	for (int k=1;k<=CP.NTeta;k++) out<<"  "<<CP.TetaPar[k];
    out<<"\n"<<" CP.Rconst "<<CP.Rconst<<" CP.VibTeta "<<CP.VibTeta<<" CP.VibZCoef "<<CP.VibZCoef
        <<" CP.DisEnergy "<<CP.DisEnergy <<" CP.PressureMet "<<CP.PressureMet 
        <<" CP.DencityMet "<<CP.DencityMet <<" CP.Aconst "<<CP.Aconst 
        <<" CP.Bconst "<<CP.Bconst <<" CP.Cconst "<<CP.Cconst <<" CP.RLconst "<<CP.RLconst 
        <<" CP.Lconst "<<CP.Lconst <<" CP.Denc0const "<<CP.Denc0const <<" CP.T0const "<<CP.T0const 
        <<" CP.T_Turn "<<CP.T_Turn <<" CP.IncludeTurnE "<<CP.IncludeTurnE 
        <<" CP.IncludeNullE "<<CP.IncludeNullE <<" CP.IncludeTurnP "<<CP.IncludeTurnP 
        <<" CP.IncludeNullP "<<CP.IncludeNullP <<" CP.IncludeZeroE "<<CP.IncludeZeroE 
        <<" CP.IncludeDis "<<CP.IncludeDis <<" CP.IncludeDisPres "<<CP.IncludeDisPres 
        <<" DisPres Name ";
    fstream out_dat(CP.DataName.c_str(),ios::out);
    OutputTDataF(out_dat,*CP.Dis_Pnt);
  };






double EmpiGetH2Dencity(){return CP.Denc;};

