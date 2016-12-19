#ifndef __MATT_HIS_H
#define __MATT_HIS_H

#include "matt_fac.h"
#include "hisch.h"
#include "mat/dat_mani.h"
#include "mat/mtquad.h"
#include "mat/zero.h"
//#include "matlib.h"
#include "mat/mtrung.h"



struct MatterHisch:MatterIOFind{

   double Denc;
   HischConst Hisch;
   MatterHisch():MatterIOFind("material.cfg","MatterHischBr4"),Denc(2.89),
	   Hisch(3.584,0.322,0.67,4.03,0,0.5,0.346,0.9,1,0.8,30){Hisch.Ed=Hisch.B0c*Hisch.V0c/(Hisch.M*Hisch.N);}
   double Pressure(double Dencity,double Energy)
     { return  Pres(Dencity,Energy,Hisch); };
   void Pressure(double *P,double *Dencity,double *Energy,int Num)
     { Pres(Dencity,Energy,P,Hisch,Num); };
   double DencityCold()                       {return Denc;};
   double DencityMax()                        {return Denc*10;};
   void ReadSubstancePar(istream &in){
       char tmp[256];in>>tmp>>Denc>>tmp>>Hisch.B0c>>tmp>>Hisch.V0c>>
        tmp>>Hisch.M>>tmp>>Hisch.N>>tmp>>Hisch.Gi>>
        tmp>>Hisch.V0>>tmp>>Hisch.G0c>>tmp>>Hisch.Sn>>
        tmp>>Hisch.Sm>>tmp>>Hisch.Ea;
      Hisch.Ed=Hisch.B0c*Hisch.V0c/(Hisch.M*Hisch.N);
     };
   void SaveSubstancePar(ostream &out){
       out<<" Denc "<<Denc<<" Hisch.B0c "<<Hisch.B0c<<" Hisch.V0c "<<Hisch.V0c
		   <<" Hisch.M "<<Hisch.M<<" Hisch.N "<<Hisch.N<<" Hisch.Gi "<<Hisch.Gi
		   <<" Hisch.V0 "<<Hisch.V0<<" Hisch.G0c "<<Hisch.G0c
		   <<" Hisch.Sn "<<Hisch.Sn<<" Hisch.Sm "<<Hisch.Sm
		   <<" Hisch.Ea "<<Hisch.Ea<<" ";
     };
};
//MatterHischBr1   Denc 2.89 B0c 3.244 V0c  0.332  m 0.3  n 6.2  Gi 0.5 V0 0.346 G0c 1.3  Sn 1 Sm 0.8  Ea 30
//MatterHischBr2   Denc 2.89 B0c 46.95 V0c  0.208  m 1    n 1.35 Gi 0.5 V0 0.21  G0c 1.17 Sn 1 Sm 0.8  Ea 30
//                 Denc      B0c       V0c         M      N      Gi     V0       G0c      Sn   Sm      Ea

struct MatterIVL:MatterIOFind{

   double Denc;
   IvlConst Ivl;
   MatterIVL():MatterIOFind("material.cfg","MatterIVLPolysterene"),Denc(1.05),
	   Ivl(0.954771,5.716068,2,.8,3.573227,0.45,.95,.2536666,.6666667,1.01371,2,.9025,28,-.3346744){};

   double Pressure(double Dencity,double Energy)
     { return  Pres(Dencity,Energy,Ivl); };
   void Pressure(double *P,double *Dencity,double *Energy,int Num)
     { Pres(Dencity,Energy,P,Ivl,Num); };
   double DencityCold()                       {return Denc;};
   double DencityMax()                        {return Denc*10;};
   void ReadSubstancePar(istream &in){
       char tmp[256];in>>tmp>>Denc>>tmp>>Ivl.V0>>tmp>>Ivl.DX
            >>tmp>>Ivl.CMN>>tmp>>Ivl.M>>tmp>>Ivl.N>>tmp>>Ivl.Ed
            >>tmp>>Ivl.E0>>tmp>>Ivl.GC>>tmp>>Ivl.GF>>tmp>>Ivl.GGI
            >>tmp>>Ivl.Gi>>tmp>>Ivl.QS>>tmp>>Ivl.Sm>>tmp>>Ivl.Ea;
     };
   void SaveSubstancePar(ostream &out){
       out<<" Denc "<<Denc<<" Ivl.V0 "<<Ivl.V0<<" Ivl.DX "<<Ivl.DX<<" Ivl.CMN "<<Ivl.CMN
		   <<" Ivl.M "<<Ivl.M<<" Ivl.N "<<Ivl.N<<" Ivl.Ed "<<Ivl.Ed
		   <<" Ivl.E0 "<<Ivl.E0<<" Ivl.GC "<<Ivl.GC<<" Ivl.GF "<<Ivl.GF<<" Ivl.GGI "<<Ivl.GGI
		   <<" Ivl.Gi "<<Ivl.Gi<<" Ivl.QS "<<Ivl.QS<<" Ivl.Sm "<<Ivl.Sm<<" Ivl.Ea "<<Ivl.Ea<<" ";
     };
  };


struct TempParStruct
  {
   TData<double> *data,*Grun_Denc,*Temp_Denc,*E_Denc,*Teta_Denc,*Cv_Teta;
   double Tmin;
   TempParStruct(){Grun_Denc=NULL;Temp_Denc=NULL;E_Denc=NULL;Teta_Denc=NULL;Cv_Teta=NULL;data=NULL;Tmin=0;}
   ~TempParStruct(){delete Grun_Denc;delete Temp_Denc;delete E_Denc;delete Teta_Denc;delete Cv_Teta;delete data;
                    Grun_Denc=NULL;Temp_Denc=NULL;E_Denc=NULL;Teta_Denc=NULL;Cv_Teta=NULL;data=NULL;}
  } ;

void GenerateTemp(X_func ShockU,X_func TempU,X_func GrunU,
                  double Misfit,double Denc,double Tmin,double Umin,double Umax,int NumPnt,TempParStruct *TempPar);
double GetTemp(double Dencity,double Energy,TempParStruct *TempPar);
double GetEnergy(double Dencity,double Temperature,TempParStruct *TempPar);

//====================================================================
//====================================================================
//====================================================================
struct MatterABuExpFit:MatterIOFind{
   double Denc;
   double A,B,EDist;
   double G0c,Sm,Sn,Ed,Ea,Gi;

   double Tmin,Umin,Umax;int NumPnt;
   double TA,TB;           // T=TA+TB*PHug
   double TL,TP,TT0;
   MatterHisch Hisch;
   TempParStruct TempPar;
   MatterABuExpFit():MatterIOFind("material.cfg","BromExpHghK5"),
		Denc(3.79),A(2.44768),B(1.39182),EDist(4),G0c(0.720703),Sm(0.738665),
		Sn(0.1),Ed(0),Ea(1e5),Gi(0.5),Tmin(0.001),Umin(0.2),Umax(16),NumPnt(100),TA(0),TB(1.13969),
		TL(3.03977),TP(1.34814),TT0(0.425789){};
   double DencityCold()                       {return Denc;};
   double DencityMax()                        {return 10*Denc;};
   double Pressure(double Dencity,double Energy)
     {
      if (Dencity<MathZer) 
        {cout<<" Dencity is negative. double MatterABu::Pressure.";return 0;}
//cout<<" Hisch Press \n";
      if (Dencity<Denc) 
        return Hisch.Pressure(Dencity,Energy);
      double X=1-Denc/Dencity;
      double Ph=Denc*sqr(A)*X/sqr(1-B*X);
      double Eh=0.5*Ph*X/Denc;
      double G=Grun(Dencity/Denc,Energy,Hisch.Hisch);
//cout<<Dencity<<"  "<<Energy<<"  "<<G<<"\n";
      return Ph+G*Dencity*(Energy-Eh);  // Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
     };
   static double S_A,S_B,S_TA,S_TB,S_Denc,S_TL,S_TP,S_TT0;
   static HischConst S_H;
   static double ShockU(double u) { return S_A+S_B*u;}
   static double GrunU(double u) { double D=S_A+S_B*u;return Grun(D/(D-u),0.5*sqr(u),S_H);}
//   static double TempU(double u) { double Ph=S_DencT*(S_A+S_B*u)*u;return S_TA+S_TB*Ph;}
//   static double TempU(double u) { return S_TA+S_TB*0.5*sqr(u);}
   static double CvU(double u) 
   {
    return (atan((u-S_TP)/S_TL)+M_PI/2.)/M_PI*(S_TB-S_TA)+S_TA;
//    if (u/S_DencT>10) return StndErr;
//cout<<" U/U0 "<<u/S_DencT<<" Cv "<<S_TB*(1-exp(-u/S_DencT))<<"\n";
//    return max<double>(StndErr,S_TB*(1-exp(-u/S_DencT)));
    }
   static double IntegUT(double u,double t) 
     { double D=S_A+S_B*u,D1=S_B,DM,G=Grun(D/(D-u),0.5*sqr(u),S_H),Cv=CvU(u);
       DM=D-u;
       return (sqr(u)*D1*DM+S_A*Cv*G*t)/(D*DM*Cv);
      }

   static double TempU(double u) 
     {
      double x,y,Stp=0.1;
      NotRungCurt(IntegUT,0,S_TT0,u,x,y,Stp);
      return y;
     }; 
//============= for the function:       d(y)=d(x)*f(x,y) ===========
//int NotRungCurt(XYZ_func func,double x_beg,double y_beg,double x_end,double &x,double &y,double &stp,double err=0,int MaxIter=1000,BrkXY_func Brk=NULL);
   void SetHisch() 
      {
//cout<<" SetHisch\n";ChRead();
       Hisch.Denc=Denc;
       Hisch.Hisch.V0c=1/Denc;Hisch.Hisch.V0=1/Denc;Hisch.Hisch.Ea=Ea;Hisch.Hisch.Gi=Gi;
       Hisch.Hisch.Sm=Sm;Hisch.Hisch.Sn=Sn;Hisch.Hisch.G0c=G0c;
       Hisch.Hisch.B0c=sqr(A)*Denc;
       SqPolynom(1,3-4*B,sqr(A)/EDist,Hisch.Hisch.N,Hisch.Hisch.M);
       if (Hisch.Hisch.M==0) { Hisch.Hisch.M=1; cout<<" Bad C0-Ed-B coef increase Ed\n";}
       if (Hisch.Hisch.N==0) { Hisch.Hisch.N=4*B-3-Hisch.Hisch.M; cout<<" Bad C0-Ed-B coef increase Ed\n";}
       Ed=Hisch.Hisch.B0c*Hisch.Hisch.V0c/(Hisch.Hisch.M*Hisch.Hisch.N);
       Hisch.Hisch.Ed=Ed;
                      
//cout<<"SQ polynom b "<<3-4*B<<" c "<<sqr(A)/EDist<<" sqrt() "<<sqr(3-4*B)-4*sqr(A)/EDist<<"\n";
//cout<<" V0c "<<Hisch.Hisch.V0c<<" Ea "<<Hisch.Hisch.Ea<<" Gi  "<<Hisch.Hisch.Gi<<"\n";
//cout<<" Sm  "<<Hisch.Hisch.Sm<<" Sn  "<<Hisch.Hisch.Sn<<" G0c "<<Hisch.Hisch.G0c<<"\n";
//cout<<" B0c "<<Hisch.Hisch.B0c<<" M  "<<Hisch.Hisch.M<<" N "<<Hisch.Hisch.N<<"\n";
//cout<<" Ed "<<Ed<<" EdBefore "<<EDist<<"\n";
       S_A=A;S_B=B; S_TA=TA;S_TB=TB;S_H=Hisch.Hisch;S_Denc=Denc;S_TL=TL;
       S_TP=TP;S_TT0=TT0;
//cout<<" Ptr "<<Temp.Grun_Denc<<" Generating\n";ChRead();
       GenerateTemp(ShockU,TempU,GrunU,
                  StndErr,Denc,Tmin,Umin,Umax,NumPnt,&TempPar);

      }; 
   double Temperature(double Dencity,double Energy)
     {return GetTemp(Dencity,Energy,&TempPar);}
   double Energy(double Dencity,double Temperature)
     {return GetEnergy(Dencity,Temperature,&TempPar);}
   void ReadSubstancePar(istream &in)
      {
       char tmp[256];
       in>>tmp>>Denc>>tmp>>A>>tmp>>B>>tmp>>EDist>>tmp>>G0c>>tmp>>Sm>>tmp>>Sn;
       in>>tmp>>Tmin>>tmp>>Umin>>tmp>>Umax>>tmp>>NumPnt>>tmp>>TA>>tmp>>TB>>
           tmp>>Gi>>tmp>>Ea>>tmp>>TL>>tmp>>TP>>tmp>>TT0;
//cout<<" Denc "<<Denc<<" A "<<A<<" B "<<B<<" EDist "<<EDist<<" G0c "<<G0c<<
//" \nSm "<<Sm<<" Sn "<<Sn<<" Tmin "<<Tmin<<" Umin "<<Umin<<" Umax "<<Umax<<" NumPnt "<<NumPnt<<
//" \nTa "<<TA<<" TB "<<TB<<" TL "<<TL<<" TP "<<TP<<" TT0 "<<TT0<<"\n";
       SetHisch();
      }; 
   void SaveSubstancePar(ostream &out){
	   out<<" Denc "<<Denc<<" A "<<A<<" B "<<B<<" EDist "<<EDist<<" G0c "<<G0c<<" Sm "<<Sm<<" Sn "<<Sn
		   <<" Tmin "<<Tmin<<" Umin "<<Umin<<" Umax "<<Umax<<" NumPnt "<<NumPnt<<" TA "<<TA<<" TB "<<TB
		   <<" Gi "<<Gi<<" Ea "<<Ea<<" TL "<<TL<<" TP "<<TP<<" TT0 "<<TT0<<" ";
	  }
  };

/*

struct MatterABuExpFit:virtual InterfaceFindSubstanceIO,virtual MatterIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}

//   TData<double> *data,*Ph_denc,*Eh_denc,*G_denc;
   double Denc;
   double A,B,EDist;
   double G0c,Sm,Sn,Ed,Ea,Gi;

   double Tmin,Umin,Umax;int NumPnt;
   double TA,TB;           // T=TA+TB*PHug
   double TL,TP,TT0;
   MatterHisch Hisch;
   TempParStruct TempPar;
//   MatterABuExpFit()
//     {
//cout<<" Set NULL; Ptr Temp.Grun_Denc "<<Temp.Grun_Denc<<"\n";//exit(0);
//      Temp.data=NULL;Temp.Grun_Denc=NULL;Temp.Temp_Denc=NULL;
//      Temp.E_Denc=NULL;Temp.Teta_Denc=NULL;Temp.Cv_Teta=NULL;}
   double DencityCold()                       {return Denc;};
   double DencityMax()                        {return 10*Denc;};
   double Pressure(double Dencity,double Energy)
     {
      if (Dencity<MathZer) 
        {cout<<" Dencity is negative. double MatterABu::Pressure.";return 0;}
//cout<<" Hisch Press \n";
      if (Dencity<Denc) 
        return Hisch.Pressure(Dencity,Energy);
      double X=1-Denc/Dencity;
      double Ph=Denc*sqr(A)*X/sqr(1-B*X);
      double Eh=0.5*Ph*X/Denc;
      double G=Grun(Dencity/Denc,Energy,Hisch.Hisch);
      return Ph+G*Dencity*(Energy-Eh);  // Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
     };
   static double S_A,S_B,S_TA,S_TB,S_Denc,S_TL,S_TP,S_TT0;
   static HischConst S_H;
   static double ShockU(double u) { return S_A+S_B*u;}
   static double GrunU(double u) { double D=S_A+S_B*u;return Grun(D/(D-u),0.5*sqr(u),S_H);}
   static double TempU(double u) { return max<double>(S_TA+S_TB*u+S_TP*sqr(u),StndErr);}
//============= for the function:       d(y)=d(x)*f(x,y) ===========
//int NotRungCurt(XYZ_func func,double x_beg,double y_beg,double x_end,double &x,double &y,double &stp,double err=0,int MaxIter=1000,BrkXY_func Brk=NULL);
   void SetHisch() 
      {
//cout<<" SetHisch\n";ChRead();
       Hisch.Denc=Denc;
       Hisch.Hisch.V0c=1/Denc;Hisch.Hisch.V0=1/Denc;Hisch.Hisch.Ea=Ea;Hisch.Hisch.Gi=Gi;
       Hisch.Hisch.Sm=Sm;Hisch.Hisch.Sn=Sn;Hisch.Hisch.G0c=G0c;
       Hisch.Hisch.B0c=sqr(A)*Denc;
       SqPolynom(1,3-4*B,sqr(A)/EDist,Hisch.Hisch.N,Hisch.Hisch.M);
       if (Hisch.Hisch.M==0) { Hisch.Hisch.M=1; cout<<" Bad C0-Ed-B coef increase Ed\n";}
       if (Hisch.Hisch.N==0) { Hisch.Hisch.N=4*B-3-Hisch.Hisch.M; cout<<" Bad C0-Ed-B coef increase Ed\n";}
       Ed=Hisch.Hisch.B0c*Hisch.Hisch.V0c/(Hisch.Hisch.M*Hisch.Hisch.N);
       Hisch.Hisch.Ed=Ed;
                      
cout<<"SQ polynom b "<<3-4*B<<" c "<<sqr(A)/EDist<<" sqrt() "<<sqr(3-4*B)-4*sqr(A)/EDist<<"\n";
cout<<" V0c "<<Hisch.Hisch.V0c<<" Ea "<<Hisch.Hisch.Ea<<" Gi  "<<Hisch.Hisch.Gi<<"\n";
cout<<" Sm  "<<Hisch.Hisch.Sm<<" Sn  "<<Hisch.Hisch.Sn<<" G0c "<<Hisch.Hisch.G0c<<"\n";
cout<<" B0c "<<Hisch.Hisch.B0c<<" M  "<<Hisch.Hisch.M<<" N "<<Hisch.Hisch.N<<"\n";
cout<<" Ed "<<Ed<<" EdBefore "<<EDist<<"\n";
       S_A=A;S_B=B; S_TA=TA;S_TB=TB;S_H=Hisch.Hisch;S_Denc=Denc;S_TL=TL;
       S_TP=TP;S_TT0=TT0;
//cout<<" Ptr "<<Temp.Grun_Denc<<" Generating\n";ChRead();
       GenerateTemp(ShockU,TempU,GrunU,
                  StndErr,Denc,Tmin,Umin,Umax,NumPnt,&TempPar);

      }; 
   double Temperature(double Dencity,double Energy)
     {return GetTemp(Dencity,Energy,&TempPar);}
   double Energy(double Dencity,double Temperature)
     {return GetEnergy(Dencity,Temperature,&TempPar);}
   void ReadSubstancePar(fstream &in)
      {
       char tmp[256];
       in>>tmp>>Denc>>tmp>>A>>tmp>>B>>tmp>>EDist>>tmp>>G0c>>tmp>>Sm>>tmp>>Sn;
       in>>tmp>>Tmin>>tmp>>Umin>>tmp>>Umax>>tmp>>NumPnt>>tmp>>TA>>tmp>>TB>>
           tmp>>Gi>>tmp>>Ea>>tmp>>TL>>tmp>>TP>>tmp>>TT0;
cout<<" Denc "<<Denc<<" A "<<A<<" B "<<B<<" EDist "<<EDist<<" G0c "<<G0c<<
" \nSm "<<Sm<<" Sn "<<Sn<<" Tmin "<<Tmin<<" Umin "<<Umin<<" Umax "<<Umax<<" NumPnt "<<NumPnt<<
" \nTa "<<TA<<" TB "<<TB<<" TL "<<TL<<" TP "<<TP<<" TT0 "<<TT0<<"\n";
       SetHisch();
      }; 
  };

*/
#endif


/*
   T_s->Generate(n,Mis,0,*data,0,4);n=NumX;Mis=Misfit;
   double integ;
   G_ss=G_s;
   for (k=0;k<data->I[0];k++) 
     {
      D=exp(data->D[0][k]);
      integ=RecInt(GrInt,MinV,1/D,Mis*0.01);
      T_s->Evaluate(&D,&T,1);T=exp(T)-AddT;
      data->D[1][k]=-integ/log(T/MinT);
//      data->D[1][k]=Teta;
     } 
   Teta0_s->Generate(n,Mis,0,*data,0,1);n=NumX;Mis=Misfit;
   Cv_s->Generate(n,Mis,0,*data,1,5);n=NumX;Mis=Misfit;




   CurveSpline *Ph_s,*Eh_s,*G_s,*T_s,*Teta0_s,*Cv_s;
   double AddE,AddP,AddT,MinT,MinV;
   double Denc;
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
      return ret; }; 

// data - Struct  -     0         1        2           3
//                   log(Denc)  log(Ph)  log(Eh)     Grun 
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
      if (!ZeroMullerIt(MisfitE,T0,StndErr,1000)) {cout<<" match in Temp. Mis "<<MisfitE(T0)<<" T0 "<<T0<<"\n";}
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

*/
