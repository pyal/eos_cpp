#ifndef __MATT_BUS_H
#define __MATT_BUS_H

#include "mat/fun_fac.h"
#include "fre_fac.h"

struct FreeEBushman:FreeEIOFind {
   Ref<OneVarFunction> ColdComp,ColdRelease,TetaS,   Be;
   Ref<TwoVarFunction> Cv_VT, TetaL, FreeM,            Ge_VT, CvE_VT;
   double Denc0c,Denc0,Denc0m,Rconst,Zconst,AtomVeight,Denc0e;
   double IncludeFcold,IncludeFelectron,IncludeFliquid,IncludeFsolid;

   FreeEBushman();
   ~FreeEBushman() {DeleteFreeEBushman();}
   void DeleteFreeEBushman(){};
    //{delete ColdComp;delete ColdRelease;delete TetaS;delete Be;
    // ColdComp=NULL;ColdRelease=NULL;TetaS=NULL;Be=NULL;
    // delete Cv_VT;delete TetaL;delete FreeM;delete Ge_VT;delete CvE_VT;
    // Cv_VT=NULL;TetaL=NULL;FreeM=NULL;Ge_VT=NULL;CvE_VT=NULL;}

   double FreeE(double Denc,double T)
    {
        if (Denc<MathZer)
            return 0;
     T*=1e-3;
     double SigmaC=Denc/Denc0c;
     double Sigma=Denc/Denc0;
     double SigmaM=Denc/Denc0m;
     double SigmaE=Denc/Denc0e;
     double Fc,Fs,Fl,Ft,Fm,Fe;

     if (SigmaC>1) Fc=ColdComp->Calculate(SigmaC)/Denc0c;
     else 
       Fc=ColdRelease->Calculate(SigmaC)/Denc0c;

     Fs=3*Rconst*T*log(TetaS->Calculate(Sigma)/T);
     Ft=Rconst*Cv_VT->Calculate(Sigma,T)*T*log(TetaL->Calculate(Sigma,T)/T);
//Ft=Fs;
     Fm=Rconst*FreeM->Calculate(SigmaM,T);
     //Fm=Rconst*FreeM->Calculate(Sigma,T);
     double Cei=3*Rconst*Zconst/2;
     double CvE = CvE_VT->Calculate(SigmaE,T);
     double BE = Be->Calculate(T);
     double GE = Ge_VT->Calculate(SigmaE,T);
     Fe=-Rconst*CvE*T*log(1+BE*T/(2*Cei)*pow(SigmaE,-GE));
     //Fe=-Rconst*CvE_VT->Calculate(SigmaE,T)*T*
     //    log(1+Be->Calculate(T)*T/(2*Cei)*pow(SigmaE,-Ge_VT->Calculate(SigmaE,T)));
//Fe=0;
     Fl=Ft+Fm;
//cout<<Denc<<" "<<T<<" "<<Fm<<"\n";
//     return Fe;
     return Fl*IncludeFliquid+Fs*IncludeFsolid+Fc*IncludeFcold+Fe*IncludeFelectron;
   };

   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e4;};
   void ReadSubstancePar(istream &in) { 
     char tmp[256];
     in>>tmp>>AtomVeight;
     in>>tmp>>Zconst;
     in>>tmp>>Denc0>>tmp>>Denc0c>>tmp>>Denc0m>>tmp>>Denc0e;
     in>>tmp>>IncludeFcold>>tmp>>IncludeFelectron>>tmp>>IncludeFliquid>>tmp>>IncludeFsolid;


	 in>>tmp;ColdComp << SavableClass::Read(in);
	 in>>tmp;ColdRelease << SavableClass::Read(in);
	 in>>tmp;TetaS << SavableClass::Read(in);

	 in>>tmp;Cv_VT << SavableClass::Read(in);
	 in>>tmp;TetaL << SavableClass::Read(in);
	 in>>tmp;FreeM << SavableClass::Read(in);

	 in>>tmp;Be << SavableClass::Read(in);
	 in>>tmp;CvE_VT << SavableClass::Read(in);
	 in>>tmp;Ge_VT << SavableClass::Read(in);

	 //in>>tmp;OneVarFuncRegister(in,ColdComp);
  //   in>>tmp;OneVarFuncRegister(in,ColdRelease);
  //   in>>tmp;OneVarFuncRegister(in,TetaS);
  //   in>>tmp;TwoVarFuncRegister(in,Cv_VT);
  //   in>>tmp;TwoVarFuncRegister(in,TetaL);
  //   in>>tmp;TwoVarFuncRegister(in,FreeM);

  //   in>>tmp;OneVarFuncRegister(in,Be);
  //   in>>tmp;TwoVarFuncRegister(in,CvE_VT);
  //   in>>tmp;TwoVarFuncRegister(in,Ge_VT);
     Rconst=M_Rconst/AtomVeight*1e3;
//     FreeEBushman::ReadIni(in); 
   }; 
   void SaveSubstancePar(ostream &out) { 
       out<<" AtomVeight "<<AtomVeight <<" Zconst "<<Zconst <<" Denc0 "<<Denc0 
           <<" Denc0c "<<Denc0c <<" Denc0m "<<Denc0m <<" Denc0e "<<Denc0e 
           <<" IncludeFcold "<<IncludeFcold <<" IncludeFelectron "<<IncludeFelectron 
           <<" IncludeFliquid "<<IncludeFliquid<<" IncludeFsolid "<<IncludeFsolid;
	   out<<SavableClass::EOLN()<<"ColdComp "<<ColdComp<<" ColdRelease "<<ColdRelease<<" TetaS "<<TetaS
           <<" Cv_VT "<<Cv_VT<<" TetaL "<<TetaL<<" FreeM "<<FreeM<<" Be "<<Be<<" CvE_VT "<<CvE_VT<<" Ge_VT "<<Ge_VT;
    //   out<<"\nColdComp ";SavableClass::Save(out, ColdComp);
    //   out<<"\nColdRelease ";SavableClass::Save(out, ColdRelease);
    //   out<<"\nTetaS ";SavableClass::Save(out, TetaS);
    //   out<<"\nCv_VT ";SavableClass::Save(out, Cv_VT);
    //   out<<"\nTetaL ";SavableClass::Save(out, TetaL);
    //   out<<"\nFreeM ";SavableClass::Save(out, FreeM);
    //   out<<"\nBe ";SavableClass::Save(out, Be);
    //   out<<"\nCvE_VT ";SavableClass::Save(out, CvE_VT);
    //   out<<"\nGe_VT ";SavableClass::Save(out, Ge_VT);
	   //out<<"\n";
   }

  };


struct FreeCold_V:OneVarFunction{
	  FreeCold_V():Num(1){ColdCoef[1]=1;}
   int Num;
   double ColdCoef[21];
   virtual double Calculate(double sigma);
   virtual int save_data_state(FilterOut&so);
   virtual int read_data_state(FilterIn&si);
  };

struct FreeColdR_V:OneVarFunction{
	FreeColdR_V():Ac(-0.67), Bc(67.02), Cc(-66.35), M_(20), N_(3.87), Esub_Mul_Denc0c(65.673){}
// Esub = Esub_IVL/V0c = Esub_IVL*Denc0c
// L_ = -Cc/(Ac/M_+Bc/N_+Esub) 
   double Ac,Bc,Cc,M_,N_,L_,Esub_Mul_Denc0c;

//   double Denc0c;
   virtual double Calculate(double sigma);
   virtual int save_data_state(FilterOut&so);
   virtual int read_data_state(FilterIn&si);
  };

struct TetaExp_Sigma:OneVarFunction{
	TetaExp_Sigma():Teta0(0.51), Gamma0(1.93), Bcoef(0.8), Dcoef(0.51){}
   double Teta0,Gamma0,Bcoef,Dcoef;
   virtual double Calculate(double x);
   virtual int save_data_state(FilterOut&so);
   virtual int read_data_state(FilterIn&si);
  };

struct HeatCapacity_SigmaT:TwoVarFunction{
	HeatCapacity_SigmaT():Sigma_a(0.03), Temp_a(60){}
   double Sigma_a,Temp_a;
   virtual double Calculate(double sig,double temp);
   virtual int save_data_state(FilterOut&so);
   virtual int read_data_state(FilterIn&si);
  };

struct TetaL_SigmaLT:TwoVarFunction{
	TetaL_SigmaLT():Temp_sa(14), Temp_ca(27), sigL_sigC(0.98839),
		 Teta0(226), Gamma0(2.6), Bcoef(0.7), Dcoef(0.),
		PowRelease(0.8), PowX0(0.7), PowCoef(0.35), PowPow(0){}
   double Temp_sa,Temp_ca,sigL_sigC;
   double Teta0,Gamma0,Bcoef,Dcoef;
   double PowRelease,PowX0,PowCoef,PowPow;
//   TetaExp_Sigma TetaL;
   double Teta(double sigL);
   virtual double Calculate(double sigL,double temp);
   virtual int save_data_state(FilterOut&so);
   virtual int read_data_state(FilterIn&si);
  };
struct TetaL_SigmaLT_Short:TwoVarFunction{
	TetaL_SigmaLT_Short():Temp_sa(14), Temp_ca(27), 
		 Teta0(226), Gamma0(2.6), Bcoef(0.7), Dcoef(0.){}
   double Temp_sa,Temp_ca;
   double Teta0,Gamma0,Bcoef,Dcoef;
   double Teta(double sigL);
   virtual double Calculate(double sigL,double temp);
   virtual int save_data_state(FilterOut&so);
   virtual int read_data_state(FilterIn&si);
  };
struct FreeM_SigmaMT:TwoVarFunction{
	FreeM_SigmaMT():Temp_m0(1.73), Am(0.97009), Bm(0.), Cm(-0.2), PowAm(1.66666666){}
   double Temp_m0,Am,Bm,Cm, PowAm;
   TetaExp_Sigma TetaL;
   virtual double Calculate(double sigM,double temp);
   virtual int save_data_state(FilterOut&so);
   virtual int read_data_state(FilterIn&si);
  };

struct HeatCapacityE_SigmaT:TwoVarFunction{
   HeatCapacityE_SigmaT():Zconst(28), SigmaZ(0.7), SigmaI(0.35), Tz(500), Ti(27), PowVE(-1){}
   double Zconst,SigmaZ,SigmaI,Tz,Ti,PowVE;
   virtual double Calculate(double sigma,double temp)
     {
         double ExpFactor = 1;
         if (SigmaI/sigma<200){
             double arg = -Ti*exp(-SigmaI/sigma)/temp;
             if (arg<-200)
                 ExpFactor = 0;
             else
                 if (arg>200)
                     ExpFactor = 1. / M_MinDouble2;
                 else
                     ExpFactor = exp(arg);
         }
         return 3./2*( Zconst+pow((sigma+SigmaZ)/SigmaZ,PowVE)*sqr(Tz)*(1-Zconst)/(sqr(temp)+sqr(Tz)) )*
                    ExpFactor;
         //return 3./2*( Zconst+pow((sigma+SigmaZ)/SigmaZ,PowVE)*sqr(Tz)*(1-Zconst)/(sqr(temp)+sqr(Tz)) )*
         //           exp(-Ti*exp(-SigmaI/sigma)/temp);
   }
   virtual int read_data_state(FilterIn& input)
    {char tmp[256];input>>tmp>>Zconst>>tmp>>SigmaZ>>tmp>>SigmaI>>tmp>>Tz>>tmp
                        >>Ti>>tmp>>PowVE;return 1;}
   virtual int save_data_state(FilterOut&output)
    {output<<" Zconst "<<Zconst<<" SigmaZ "<<SigmaZ<<" SigmaI "<<SigmaI<<" Tz "<<Tz
           <<" Ti "<<Ti<<" PowVE "<<PowVE;return 1;}
  };

struct GammaE_SigmaT:TwoVarFunction{
	GammaE_SigmaT():Ge0(1.07), Gei(0.66666), Gm(0), Tg(110), SigmaE(1), SigmaD(1e50){}
   double Ge0,Gei,Gm,Tg,SigmaE,SigmaD;
   virtual double Calculate(double sigma,double temp)
     {return Gei+( Ge0-Gei+Gm*temp/Tg )*exp(-temp/Tg-sqr(sigma-SigmaE)/sigma/SigmaD);}
   virtual int read_data_state(FilterIn& input)
    {char tmp[256];input>>tmp>>Ge0>>tmp>>Gei>>tmp>>Gm>>tmp>>Tg>>tmp>>SigmaE
                        >>tmp>>SigmaD;return 1;}
   virtual int save_data_state(FilterOut&output)
    {output<<" Ge0 "<<Ge0<<" Gei "<<Gei<<" Gm "<<Gm<<" Tg "<<Tg
           <<" SigmaE "<<SigmaE<<" SigmaD "<<SigmaD;return 1;}
  };

struct BfuncE_T:OneVarFunction{
	BfuncE_T():B0(0.02), Bi(0.01), Bm(0), Tb(6){}
	double B0,Bi,Bm,Tb;
   virtual double Calculate(double temp)
     {return 2/sqr(temp)*( 0.5*Bi*sqr(temp)+(Bm+B0-Bi)*Tb*temp+sqr(Tb)*(Bi-B0-2*Bm)+
                           Tb*exp(-temp/Tb)*(Tb*(2*Bm+B0-Bi)+Bm*temp) );}
   virtual int read_data_state(FilterIn& input)
    {char tmp[256];input>>tmp>>B0>>tmp>>Bi>>tmp>>Bm>>tmp>>Tb;return 1;}
   virtual int save_data_state(FilterOut&output)
    {output<<" B0 "<<B0<<" Bi "<<Bi<<" Bm "<<Bm<<" Tb "<<Tb;return 1;}
  };



#endif  //  __MATT_BUS_H
