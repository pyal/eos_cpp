#ifndef __MATT_EXP_H
#define __MATT_EXP_H

#include "matt_fac.h"
#include "mat/dat_mani.h"
#include "mat/spl_fac.h"
#include "mat/zero.h"
#include "mat/mtquad.h"
#include "matt_his.h"


struct MatterABuAdd:MatterIOFind{

	  MatterABuAdd():MatterIOFind("material.cfg","BromoformMy"),Denc(2.87),A(1.5),
		  B(1.38),G0(-9.538937),G1(8.001208),G2(-1.309827),G3(0){};
   double Denc,A,B,G0,G1,G2,G3;
   double DencityCold()                       {return Denc;};
   double DencityMax()                        {return Denc*B/(fabs(B-1)+StndErr);};
   double Pressure(double Dencity,double Energy)
     {
      if (Dencity<MathZer) 
        {cout<<" Dencity is negative. double MatterABu::Pressure.";return 0;}
      double X=1-Denc/Dencity,Ph=Denc*sqr(A)*X/sqr(1-B*X),Eh=0.5*Ph*X/Denc;
// Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
// c0+c1*arctan(Gw/(Dm-D0)-Gz)=G0
// c0+c1*Pi/2=GM  c1=(G0-GM)/(arctan(Gw/(Dm-D0)-Gz)-Pi/2) c0=GM+c1*Pi/2
      double D0=DencityCold(),t=Dencity/D0,
             Grun=G0+G1*t+(G2+G3*t)*sqr(t);
      return Ph+Grun*Denc*(Energy-Eh);  // Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
//      return Grun;
     }; 
//   double Sound(double Dencity,double Energy);
   double Temperature(double Dencity,double Energy)
     {
      double D0=DencityCold(),t=Dencity/D0,
             Grun=G0+G1*t+(G2+G3*t)*sqr(t);
      return Grun;
     }; 
//   void Pressure(double *P,double *Dencity,double *Energy,int Num);
//   void Sound(double *S,double *Dencity,double *Energy,int Num);
	void ReadSubstancePar(istream &in){
        char tmp[256];in>>tmp>>tmp>>Denc>>tmp>>A>>tmp>>B>>tmp>>G0
            >>tmp>>G1>>tmp>>G2>>tmp>>G3;
	};
	void SaveSubstancePar(ostream &out){
		out<<Finder.Substance<<" \tDenc "<<Denc<<" A "<<A<<" B "<<B<<" G0 "<<G0<<" G1 "<<G1<<" G2 "<<G2<<" G3 "<<G3<<" ";
	};
  };


struct MatterABuExp:MatterIOFind{
   CurveSpline *Ph_s,*Eh_s,*G_s,*T_s,*Teta0_s,*Cv_s;
   double Denc;
   int NumX;
   double Misfit;
   double AddP,AddE,AddT,MinT,MinV;
   double DencHisch;

   MatterHisch Hisch;

   static double SubT,DivT,Teta0;
   static CurveSpline *Cv_ss;
   static double CvFunc(double T)
     {
//      double teta=Teta0+log(T/Th0),ret;Cv_ss->Evaluate(&teta,&ret,1);
      double teta=Teta0*(log(T)-SubT)/DivT,ret;Cv_ss->Evaluate(&teta,&ret,1);
      return ret; }; 

   static double Denc0,Energy0,Eh0,Th0;
   static double MisfitE(double T)
     {
      if (T<sqrt(2*StndErr)) return 0;
      double ret;ret=RecInt(CvFunc,Th0,T,StndErr);
      return ret+Eh0-Energy0; }; 
   
   static CurveSpline *G_ss;
   static double GrInt(double V)
     {
      double D=1/V;
      double Grun;G_ss->Evaluate(&D,&Grun,1);
      double ret;ret=Grun*D;
//cout<<" GrInt "<<ret<<" V "<<V<<"\n";
      return ret; }; 

// data - Struct  -     0         1        2           3
//                   log(Denc)  log(Ph)  log(Eh)     Grun 
   double DencityCold()                       {return Denc;};
   double DencityMax()                        {return 10*Denc;};
   double Pressure(double Dencity,double Energy)
     {
      if (Dencity<MathZer) 
        {cout<<" Dencity is negative. double MatterABu::Pressure.";return 0;}
      if (Dencity<DencHisch) return Hisch.Pressure(Dencity,Energy);
      double D=log(Dencity);
      double Ph;Ph_s->Evaluate(&D,&Ph,1);Ph=exp(Ph)-AddP;
      double Eh;Eh_s->Evaluate(&D,&Eh,1);Eh=exp(Eh)-AddE;
      double Grun;G_s->Evaluate(&D,&Grun,1);

      return Ph+Grun*Dencity*(Energy-Eh);  // Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
     }; 
//   double Sound(double Dencity,double Energy);
   double Temperature(double Dencity,double Energy) 
     {
      double T0,D=log(Dencity);T_s->Evaluate(&D,&T0,1);T0=exp(T0)-AddT;
      double Eh;Eh_s->Evaluate(&D,&Eh,1);Eh0=exp(Eh)-AddE;
cout<<" Fst Est "<<T0+(Energy-Eh0);Eh=T0+(Energy-Eh0);
      Denc0=Dencity;Energy0=Energy;Th0=T0;
      double teta;Teta0_s->Evaluate(&D,&teta,1);
      SubT=log(MinT);DivT=log(T0)-SubT;Teta0=teta;
      Cv_ss=Cv_s;
      T0=max<double>(Eh,sqrt(4*StndErr));
      if (!ZeroMullerIt(MisfitE,T0,StndErr,1000))
        {cout<<" match in Temp. Mis "<<MisfitE(T0)<<" T0 "<<T0<<"\n";}
cout<<" Lst "<<T0<<" Mis "<<T0-Eh<<"\n";
      return T0;
     };
   double Energy(double Dencity,double Temperature)
     {
      double T0,D=log(Dencity);T_s->Evaluate(&D,&T0,1);T0=exp(T0)-AddT;
      double teta;Teta0_s->Evaluate(&D,&teta,1);
      SubT=log(MinT);DivT=log(T0)-SubT;Teta0=teta;
      Cv_ss=Cv_s;
      double Eh;Eh_s->Evaluate(&D,&Eh,1);Eh=exp(Eh)-AddE;
      double ret;ret=Eh+RecInt(CvFunc,T0,Temperature,StndErr);
      return ret;
     };
	void ReadSubstancePar(istream &in);
	void SaveSubstancePar(ostream &out);
	Stroka DataFile;
	MatterABuExp():MatterIOFind("material.cfg","BromHiExp"),
		Denc(2.89),NumX(100),Misfit(1e-5),AddP(1),AddE(1),AddT(0),MinT(0.29),MinV(0.35),
		DencHisch(4.67),DataFile("urs_exbr.vah")
   {Eh_s=NULL;Ph_s=NULL;G_s=NULL;T_s=NULL;Teta0_s=NULL;Cv_s=NULL;}
// BromHiExp         29  Denc   2.89  NumX 100 Misfit 1e-5 AddP 1 AddE 1 AddT 0 MinT 0.290 MaxV 0.35 
//   DencHisch 4.67  name_data urs_exbr.vah br_ex.cfg
   ~MatterABuExp(){delete Ph_s;delete Eh_s;delete G_s;delete T_s;
                   delete Teta0_s;delete Cv_s;};
  };



#endif


/*
struct MatterABuExp:virtual InterfaceFindSubstanceIO,virtual MatterIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}

//   TData<double> *data,*Ph_denc,*Eh_denc,*G_denc;
   CurveSpline *Ph_s,*Eh_s,*G_s,*T_s,*Teta0_s,*Cv_s;
   double AddE,AddP,AddT,MinT,MinV;
   double Denc;
   double DencHisch;
   MatterHisch Hisch;

   static double SubT,DivT,Teta0;
   static CurveSpline *Cv_ss;
   static double CvFunc(double T)
     {double teta=Teta0*(log(T)-SubT)/DivT,ret;Cv_ss->Evaluate(&teta,&ret,1);
      return ret; }; 

   static double Denc0,Energy0,Eh0,Th0;
   static double MisfitE(double T)
     {
      if (T<sqrt(2*StndErr)) return 0;
      double ret;ret=RecInt(CvFunc,Th0,T,StndErr);
      return ret+Eh0-Energy0; }; 
   
   static CurveSpline *G_ss;
   static double GrInt(double V)
     {
      double D=1/V;
      double Grun;G_ss->Evaluate(&D,&Grun,1);
      double ret;ret=Grun*D;
//cout<<" GrInt "<<ret<<" V "<<V<<"\n";
      return ret; }; 

// data - Struct  -     0         1        2           3
//                   log(Denc)  log(Ph)  log(Eh)     Grun 
   double DencityCold()                       {return Denc;};
   double DencityMax()                        {return 10*Denc;};
   double Pressure(double Dencity,double Energy)
     {
      if (Dencity<MathZer) 
        {cout<<" Dencity is negative. double MatterABu::Pressure.";return 0;}
      if (Dencity<DencHisch) return Hisch.Pressure(Dencity,Energy);
      double D=log(Dencity);
//      double D=Dencity;

//      double Ph=EvalSpl(*data,0,1,*Ph_denc,D);Ph=exp(Ph)-1;
//      double Eh=EvalSpl(*data,0,2,*Eh_denc,D);Eh=exp(Eh)-1;
//      double Grun=EvalSpl(*data,0,3,*G_denc,D);
      double Ph;Ph_s->Evaluate(&D,&Ph,1);Ph=exp(Ph)-AddP;
      double Eh;Eh_s->Evaluate(&D,&Eh,1);Eh=exp(Eh)-AddE;
      double Grun;G_s->Evaluate(&D,&Grun,1);
//      double T;T_s->Evaluate(&D,&T,1);

      return Ph+Grun*Dencity*(Energy-Eh);  // Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
     }; 
//   double Sound(double Dencity,double Energy);
   double Temperature(double Dencity,double Energy) 
     {
      double T0,D=log(Dencity);T_s->Evaluate(&D,&T0,1);T0=exp(T0)-AddT;
      double Eh;Eh_s->Evaluate(&D,&Eh,1);Eh0=exp(Eh)-AddE;
cout<<" Fst Est "<<T0+(Energy-Eh0);Eh=T0+(Energy-Eh0);
      Denc0=Dencity;Energy0=Energy;Th0=T0;
      double teta;Teta0_s->Evaluate(&D,&teta,1);
      SubT=log(MinT);DivT=log(T0)-SubT;Teta0=teta;
      Cv_ss=Cv_s;
//      T0/=2;
      T0=max<double>(Eh,sqrt(4*StndErr));
//double Mist=StndErr;
//      if (!ZeroNewtonIt(MisfitE,T0,StndErr,1000,1/StndErr,sqrt(StndErr),0.1*max<double>(T0,0.1)))
      if (!ZeroMullerIt(MisfitE,T0,StndErr,1000))
        {cout<<" match in Temp. Mis "<<MisfitE(T0)<<" T0 "<<T0<<"\n";}
cout<<" Lst "<<T0<<" Mis "<<T0-Eh<<"\n";
      return T0;
     };
   double Energy(double Dencity,double Temperature)
     {
//      Temperature/=1000;
      double T0,D=log(Dencity);T_s->Evaluate(&D,&T0,1);T0=exp(T0)-AddT;
      double teta;Teta0_s->Evaluate(&D,&teta,1);
      SubT=log(MinT);DivT=log(T0)-SubT;Teta0=teta;
      Cv_ss=Cv_s;
      double Eh;Eh_s->Evaluate(&D,&Eh,1);Eh=exp(Eh)-AddE;
      double ret;ret=Eh+RecInt(CvFunc,T0,Temperature,StndErr);
      return ret;
     };
//   void Pressure(double *P,double *Dencity,double *Energy,int Num);
//   void Sound(double *S,double *Dencity,double *Energy,int Num);
   void ReadSubstancePar(fstream &in);
   MatterABuExp(){Eh_s=NULL;Ph_s=NULL;G_s=NULL;T_s=NULL;Teta0_s=NULL;Cv_s=NULL;}
   ~MatterABuExp(){delete Ph_s;delete Eh_s;delete G_s;delete T_s;
                   delete Teta0_s;delete Cv_s;};
  };
*/
