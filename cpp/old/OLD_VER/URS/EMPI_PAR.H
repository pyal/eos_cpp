#ifndef __FRE_FAC_H
#define __FRE_FAC_H 


#include "util.h"
#include "matt_fac.h"
#include "h2_liqui.h"
#include "empi_equ.h"

//#include "matt_reg.h"

struct InterfaceFreeEIO:virtual InterfaceIO
  {
   virtual double FreeE(double Dencity,double Temperature)=0;
   virtual double Pressure(double Dencity,double Temperature)=0;
   virtual double Energy(double Dencity,double Temperature)=0;
   friend int FreeERegister(fstream &in,InterfaceFreeEIO *&Func);
   friend int FreeEStore(fstream &out,InterfaceFreeEIO *&Func);
  };

struct FreeEIO:InterfaceFreeEIO
  {
   double Pressure(double Dencity,double Temperature)
     {double dr=StndErr*Dencity;
      return sqr(Dencity)/dr*
             (FreeE(Dencity+dr,Temperature)-FreeE(Dencity,Temperature));};
   double Energy(double Dencity,double Temperature)
     {double dr=StndErr*Temperature;
      return -sqr(Temperature)/dr*
             (FreeE(Dencity,Temperature+dr)/(Temperature+dr)-
                FreeE(Dencity,Temperature)/Temperature);};
  };

// ========================================================
// ========================================================
// ========================================================

struct MatterFreeE:MatterIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}
   InterfaceFreeEIO *FreeEPtr;
			double E,D;
			static MatterFreeE* Current;
			static double DeltaFree(double T)
			  {
//double dE=Current->FreeEPtr->Energy(Current->D,T)-Current->E;
//cout<<" DF T "<<T<<" D "<<Current->D<<" E "<<Current->E<<" dE "<<dE<<"\n";
      return Current->FreeEPtr->Energy(Current->D,T)-Current->E;
					}
   double Pressure(double Dencity,double Energy)
			  {
						double T=Temperature(Dencity,Energy);
      return FreeEPtr->Pressure(Dencity,T);
					};	
//   double Sound(double Dencity,double Energy);
   double Temperature(double Dencity,double Energy)
			  {
      double T0=1000;
						E=Energy;D=Dencity;
						Current=this;
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp=MathInf) ;
      if (!ZeroNewtonIt(DeltaFree,T0,StndErr*0.01,1000,1e6,MathZer,0.3*T0,0.1))
        {cout<<" Bad ZeroNewtonFreeE in MatterFreeE::Temperature Not Zero\nfound T "
             <<T0<<" Mis "<<DeltaFree(T0)<<"\n";}
//      if (!ZeroMullerIt(DeltaFree,T0,StndErr*1e-2,10000))
//        {cout<<" Bad ZeroMullerIt in MatterFreeE::Temperature Not Zero\n";}
//      T0=ZeroIn(MathZer,100000,DeltaFree,StndErr*1e-2);
						return T0;
					}	                                         
   double Energy(double Dencity,double Temperature)
    {return FreeEPtr->Energy(Dencity,Temperature);};
   double DencityCold(){return 1e-4;}
   double DencityMax(){return 100;}
   
//   char *FileName,*Substance;
   MatterFreeE(){FreeEPtr=NULL;};
   virtual void SaveIni(fstream &output) { FreeEStore(output,FreeEPtr); };
   virtual int ReadIni(fstream &input)   { return FreeERegister(input,FreeEPtr); };
   ~MatterFreeE(){delete FreeEPtr;FreeEPtr=NULL;};
		};	

struct FreeEVanDerVals:InterfaceFindSubstanceIO,FreeEIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName() {return name;}
   static const double Rconst;
   double A,B,MolVeight,Zero,CvId,HiT;
   double FreeE(double Denc,double T)
    {
//                     B       A
//  F= Zero-R*T*ln(1- ---) - ----- - R*T*ln(V) - R*T*C *ln(T) - HiT*R*T
//                     V       V                      v
if ((T<MathZer) || (Denc<MathZer)) { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
     double F=-A*Denc-Zero-(Rconst/MolVeight*T)*
               (log(max<double>(1/Denc-B,MathZer))+CvId*log(T)+HiT);
     return F;
    };
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
			void ReadSubstancePar(fstream &in)
     {char tmp[256];in>>tmp>>A>>tmp>>B>>tmp>>CvId>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT;in.getline(tmp,256);
cout<<" Pkr(GPa) "<<A/27/sqr(B)<<" Tkr(K) "<<8*A*MolVeight /(27*Rconst*B)
    <<" Rkr(g/cm3) "<<1/(3*B)<<"\n";
      };
  }; 

struct FreeEH2Liquid:InterfaceFindSubstanceIO,FreeEIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}

   FreeEH2Liquid(){ SetConstants(); };
   double FreeE(double Denc,double T) {return FLiquid( Denc, T);  };
   void ReadSubstancePar(fstream &in) {H2ReadPar(in);}
  }; 
struct FreeEH2Solid:InterfaceFindSubstanceIO,FreeEIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}

   FreeEH2Solid(){ SetConstants(); };
   double FreeE(double Denc,double T) {return FSolid(Denc, T); };
   void ReadSubstancePar(fstream &in) {H2ReadPar(in);}
  }; 

struct FreeEEmpiLiquid:InterfaceFindSubstanceIO,FreeEIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}
   EmpiParamStruct Empi;
   double FreeE(double Denc,double T) {return EmpiFLiquid( Denc, T,&Empi);};
   void ReadSubstancePar(fstream &in) {EmpiH2ReadPar(in,&Empi);}
  }; 
struct FreeEEmpiSolid:InterfaceFindSubstanceIO,FreeEIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}
   EmpiParamStruct Empi;
   double FreeE(double Denc,double T) {return EmpiFSolid(Denc, T, &Empi); };
   void ReadSubstancePar(fstream &in) {EmpiH2ReadPar(in,&Empi);}
  }; 

struct FreeEMetHCold:InterfaceFindSubstanceIO,FreeEIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName() {return name;}
//   static const double Rconst;
//   double A,B,MolVeight,Zero,CvId,HiT;
   double DebDenc0,DebKoef;
   int AddEG,AddLDA,AddOCP,AddDeb;
   double FreeE(double Denc,double T)
    {
//                     B       A
//  F= Zero-R*T*ln(1- ---) - ----- - R*T*ln(V) - R*T*C *ln(T) - HiT*R*T
//                     V       V                      v
if ((T<MathZer) || (Denc<MathZer)) { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
     double Rs=pow(2.696/Denc,1./3.);
     double K_Ryd=6.3344e-6;
     double kT=T*K_Ryd,Ze=1,G14,F=0,e=-2.59,sige=-2.6;
     G14=pow(sqr(Ze)/(Rs*kT/2),1./4.);  // Ry= 2* 1 a.e.e
     if (AddEG) F+=2.21/sqr(Rs)-0.916/Rs-0.88/(Rs+7.8)-1.792/Rs;
     if (AddLDA) F+=(-0.11382+0.003554*Rs-0.012707*sqr(Rs));
     if (AddOCP) F+=(4*(0.9528*G14-0.18907/G14)-0.81773*4*log(G14)-e )*kT;
     F+=sige;
     if (AddDeb) 
       {double TD=max((Denc-DebDenc0)*DebKoef,100);
cout<<" D "<<Denc<<" T "<<T<<" TD "<<TD<<"\n";
        F+=kT*(3*log(1-exp(-TD/T))-DebyFunc(TD/T));
       }; 
     F*=1313;  // Conversion from Ry to (km/c)^2
     return F;
    };
//   double Energy(double Dencity,double Temperature)
//     {double dr=StndErr*Temperature;
//      double ret=-sqr(Temperature)/dr*
//             (FreeE(Dencity,Temperature+dr)/(Temperature+dr)-
//                FreeE(Dencity,Temperature)/Temperature);
//cout<<" D "<<Dencity<<" T "<<Temperature<<" E "<<ret<<"\n";
//      return ret;};
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e5;};
			void ReadSubstancePar(fstream &in)
     {char tmp[256];in>>tmp>>AddEG>>tmp>>AddLDA>>tmp>>AddOCP
                      >>tmp>>AddDeb>>tmp>>DebDenc0>>tmp>>DebKoef;in.getline(tmp,256);
      };
  }; 


#endif

//    For H2
//  t(krit)=33.23
//  p(krit)=13.16
//  v(krit)=63.8
//  Esub=0.7 kDj/g

//  VanDerVals
//                    N*b    N*N*a
//  F=Fid - N*T*ln(1- ---) - -----
//                     V       V
// (P+sqr(N/V)*a)*(V-N*b)=NT
// E=Eid-N*N*a/V
// for crit point: my approcs
// Pc*b+R*Tc=sqrt(3*Pc*a)
// (Pc*b+R*Tc)/(3*Pc)=Vc
// Landau crit point
// Tкр=(8/27*A/B)     /R?
// Vкр=3B
// Pкр=A/(27*B*B)

/*
double EVanDerVals(double Denc,double T) // With turn Cv=5/2
  {
   return ColdParam.Rconst*(T-ValsParam.A*Denc);
  };
double PVanDerVals(double Denc,double T) // With turn Cv=5/2
  {
   return ColdParam.Rconst*T/(1/Denc-ValsParam.B)-ValsParam.A*sqr(Denc);
  };
double FVanDerVals(double Denc,double T)
  {
//                    N*b    N*N*a
//  F=Fid - N*T*ln(1- ---) - -----
//                     V       V
   double A=ValsParam.A,B=ValsParam.B,dE=ValsParam.FreeDelta;
   double F=ColdParam.Rconst*(2.5*T-log(1-B*Denc)-A*Denc)-dE;
   return F;
  };
double FLiquid(double Denc,double T)
  {
   double Ecold=,Teta=,z=,b=,T0=,TetaTurn=;
   double F=Ecold+ColdParam.Rconst*
        (9/8*Teta+3*T*(log(Teta/T*sqrt(1+z))-b)+3*T0*f-T*log(T*0.5/TetaTurn));
   return F;
  };

double FSolid(double Denc,double T)
  {
   double Ecold=,Teta=,TetaTurn=,Deby=;
   double F=Ecold+ColdParam.Rconst*
        (9/8*Teta+T*(3*log(1-exp(Teta/T))-Deby-log(T*0.5/TetaTurn)));
   return F;
  };

int CurState(double r,double e)
  { return 2;};  // 1 - Gas, 2 - Liquid, 3 - Solid
*/


