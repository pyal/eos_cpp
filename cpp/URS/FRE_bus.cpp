#include <lib/precompiled/eos.h>
#include "fre_bus.h"

static ClassDesc FreeEBushman_ClassDesc(typeid(FreeEBushman),"FreeEBushman",1,"MatterFreeE_category",create<FreeEBushman>);


static ClassDesc FreeCold_V_ClassDesc(typeid(FreeCold_V),"FreeCold_V",1,"OneVarFunction_category",create<FreeCold_V>);
static ClassDesc FreeColdR_V_ClassDesc(typeid(FreeColdR_V),"FreeColdR_V",1,"OneVarFunction_category",create<FreeColdR_V>);
static ClassDesc TetaExp_Sigma_ClassDesc(typeid(TetaExp_Sigma),"TetaExp_Sigma",1,"OneVarFunction_category",create<TetaExp_Sigma>);
static ClassDesc BfuncE_T_ClassDesc(typeid(BfuncE_T),"BfuncE_T",1,"OneVarFunction_category",create<BfuncE_T>);


static ClassDesc HeatCapacity_SigmaT_ClassDesc(typeid(HeatCapacity_SigmaT),"HeatCapacity_SigmaT",1,"TwoVarFunction_category",create<HeatCapacity_SigmaT>);
//static ClassDesc TetaL_SigmaLT_ClassDesc(typeid(TetaL_SigmaLT),"TetaL_SigmaLT",1,"TwoVarFunction_category",create<TetaL_SigmaLT>);
static ClassDesc TetaL_SigmaLT_Short_ClassDesc(typeid(TetaL_SigmaLT_Short),"TetaL_SigmaLT_Short",1,"TwoVarFunction_category",create<TetaL_SigmaLT_Short>);
static ClassDesc FreeM_SigmaMT_ClassDesc(typeid(FreeM_SigmaMT),"FreeM_SigmaMT",1,"TwoVarFunction_category",create<FreeM_SigmaMT>);
static ClassDesc HeatCapacityE_SigmaT_ClassDesc(typeid(HeatCapacityE_SigmaT),"HeatCapacityE_SigmaT",1,"TwoVarFunction_category",create<HeatCapacityE_SigmaT>);
static ClassDesc GammaE_SigmaT_ClassDesc(typeid(GammaE_SigmaT),"GammaE_SigmaT",1,"TwoVarFunction_category",create<GammaE_SigmaT>);
//static ClassDesc _ClassDesc(typeid(HeatCapacity_SigmaT),"HeatCapacity_SigmaT",1,"VarFunction_category",create<HeatCapacity_SigmaT>);
//static ClassDesc _ClassDesc(typeid(HeatCapacity_SigmaT),"HeatCapacity_SigmaT",1,"VarFunction_category",create<HeatCapacity_SigmaT>);



/*
double FreeEBushman::FreeE(double Denc,double T)
  {
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
   double Cei=3*Rconst*Zconst/2;
   Fe=-Rconst*CvE_VT->Calculate(SigmaE,T)*T*
         log(1+Be->Calculate(T)*T/(2*Cei)*pow(SigmaE,-Ge_VT->Calculate(SigmaE,T)));
//Fe=0;
   Fl=Ft*IncludeFt+Fm*IncludeFm;
//cout<<Denc<<" "<<T<<" "<<Fm<<"\n";
//     return Fe;
   return Fl+Fc*IncludeFc+Fe*IncludeFe;
   };
*/

FreeEBushman::FreeEBushman():FreeEIOFind("material.cfg","Ni_Bushman"),
                             Denc0c(9.03342), Denc0(8.92857), Denc0m(7.83), Zconst(28), AtomVeight(58.71), Denc0e(8.92857),
        IncludeFcold(1), IncludeFelectron(1), IncludeFliquid(1), IncludeFsolid(1)
{
	ColdComp=new FreeCold_V();
	ColdRelease=new FreeColdR_V();
	TetaS=new TetaExp_Sigma();
	Be=new BfuncE_T();
    Cv_VT=new HeatCapacity_SigmaT();
	TetaL=new TetaL_SigmaLT_Short();
	FreeM=new FreeM_SigmaMT();
	Ge_VT=new GammaE_SigmaT();
	CvE_VT=new HeatCapacityE_SigmaT();
}



//    ========================================================================
//    ====================== FreeCold_V ===================================
//    ========================================================================

double FreeCold_V::Calculate(double sigma)
   { double ret=0;for (int k=1;k<=Num;k++) ret+=ColdCoef[k]*(pow(sigma,k/3.)-1)/k;return 3*ret;}

int FreeCold_V::read_data_state(FilterIn& input)
  { 
   char tmp[256];input>>tmp>>Num;
   if (Num>20) 
     {
      cout<<" Error reading FreeCold_V. Read Num pow coef ("<<Num<<") is more then 20.\n";
      return 0;
     }
   for (int k=1;k<=Num;k++) input>>tmp>>ColdCoef[k];
//   double s=0,B0c=0,B10c=0;
//for (int k=1;k<=Num;k++) {s+=ColdCoef[k];B0c+=ColdCoef[k]*k/3.;B10c+=ColdCoef[k]*sqr(k/3.);}
//cout<<"FreeCold_V Sum=0?:"<<s<<" B0c="<<B0c<<" B'0c="<<B10c<<"\n";
   return 1;
  }
int FreeCold_V::save_data_state(FilterOut& output)
  { 
    output<<" NumCoef "<<Num;
    for (int k=1;k<=Num;k++) output<<" Coef_i "<<ColdCoef[k];
	return 1;
  };

//    ========================================================================
//    ====================== FreeColdR_V ===================================
//    ========================================================================

//   double Ac,Bc,Cc,M_,N_,L_,Esub;
double FreeColdR_V::Calculate(double sigma)
   { 
     return (Ac*pow(sigma,M_)/M_+Bc*pow(sigma,N_)/N_+Cc*pow(sigma,L_)/L_)+Esub_Mul_Denc0c;
}

int FreeColdR_V::read_data_state(FilterIn& input)
  { 
   char tmp[256];input>>tmp>>Ac>>tmp>>Bc>>tmp>>M_>>tmp>>N_>>tmp>>Esub_Mul_Denc0c;
   Cc=-Ac-Bc;
   L_=-Cc/(Ac/M_+Bc/N_+Esub_Mul_Denc0c);
//cout<<" readParams found L = "<<L_<<" M_ "<<M_<<" N_ "<<N_<<" Ac "<<Ac<<" Bc "<<Bc<<" Cc "<<Cc<<"\n";
//double B0c=M_*Ac+N_*Bc+L_*Cc;cout<<" FreeColdR_V: B0c="<<B0c<<" B'0c="<<(sqr(M_)*Ac+sqr(N_)*Bc+sqr(L_)*Cc)<<"\n";
   return 1;
  }
int FreeColdR_V::save_data_state(FilterOut& output)
  { 
   output<<" Ac "<<Ac<<" Bc "<<Bc<<" m "<<M_<<" n "<<N_<<" Esub_Mul_Denc0c "<<Esub_Mul_Denc0c;return 1;
  };

//    ========================================================================
//    ====================== TetaExp_Sigma ===================================
//    ========================================================================

double TetaExp_Sigma::Calculate(double x)
  {
   double tmp=log(max<double>(MathZer,x));
   double ret=Teta0*pow(x,2./3)*exp( (Gamma0-2./3)*(sqr(Bcoef)+sqr(Dcoef))/Bcoef
                                    *atan(tmp*Bcoef/(sqr(Bcoef)+Dcoef*(tmp+Dcoef))) );
//for (double u=-1000;u<=1000;u+=1) cout<<u<<" "<<atan(u)<<"\n";
   return ret;
  }
int TetaExp_Sigma::read_data_state(FilterIn& input) 
  { char tmp[256];input>>tmp>>Teta0>>tmp>>Gamma0>>tmp>>Bcoef>>tmp>>Dcoef;return 1;}
int TetaExp_Sigma::save_data_state(FilterOut& output) 
  { output<<" Teta0 "<<Teta0<<" Gamma0 "<<Gamma0<<" Bcoef "<<Bcoef<<" Dcoef "<<Dcoef;return 1;}

//    ========================================================================
//    ====================== HeatCapacity_SigmaT ===================================
//    ========================================================================

double HeatCapacity_SigmaT::Calculate(double sig,double temp)
   { 
     double ret=3./2*(1+sig*Temp_a/(sig+Sigma_a)/(temp+Temp_a));
     return ret; //*R_const;
   }
int HeatCapacity_SigmaT::read_data_state(FilterIn& input)
   { char tmp[256];input>>tmp>>Sigma_a>>tmp>>Temp_a;return 1;}
int HeatCapacity_SigmaT::save_data_state(FilterOut& output)
    { output<<" Sigma_a "<<Sigma_a<<" Temp_a "<<Temp_a;return 1;}

//    ========================================================================
//    ====================== TetaL_SigmaLT ===================================
//    ========================================================================

double TetaL_SigmaLT::Teta(double sigL)
  {
   double x=log(max<double>(MathZer,sigL));
   double Coef=1;
// double Sub=PowX0/PowRelease,Div=1/PowRelease,F0=atan(-Sub),
//        F1=atan(Div-Sub);
//   if (sigL<1) Coef=( (1-PowCoef)*atan(sigL*Div-Sub)+PowCoef*F1-F0 )/(F1-F0) ;
//   PowX0=-1/(PowRelease-1);
//   double A=(PowX0-1)/(PowRelease-1);
//   if (sigL<1) Coef=A*pow(sigL,PowRelease)-sigL*A*PowRelease+1-A+A*PowRelease ;
//   double A1=PowRelease,A2=PowX0,A3=PowCoef;
//   double B2=(1-A3)*A1/(A1-A2),B1=-A2*B2/A1;
//   if (sigL<1) Coef=B1*pow(sigL,A1)+B2*pow(sigL,A2)+A3;

   double ret=Teta0*exp( Coef*(Gamma0-2./3)*(sqr(Bcoef)+sqr(Dcoef))/Bcoef
                                    *atan(x*Bcoef/(sqr(Bcoef)+Dcoef*(x+Dcoef))) );
   return ret;
  }
double TetaL_SigmaLT::Calculate(double sigL,double temp)
   { 
     
   double Coef=1;
   double A1=PowRelease,A2=PowX0,A3=PowCoef;
   double B2=(1-A3)*A1/(A1-A2),B1=-A2*B2/A1;
   if (sigL<1) Coef=B1*pow(sigL,A1)+B2*pow(sigL,A2)+A3;
     double ret=Temp_sa*(Temp_ca*Teta(sigL)+temp)
                                                *pow(sigL_sigC*sigL,2./3*Coef)/(Temp_ca+temp);
     return ret;
   };
int TetaL_SigmaLT::read_data_state(FilterIn& input)
    { char tmp[256];input>>tmp>>Temp_sa>>tmp>>Temp_ca>>tmp>>sigL_sigC
                         >>tmp>>PowRelease>>tmp>>PowX0>>tmp>>PowCoef>>tmp>>PowPow
                         >>tmp>>Teta0>>tmp>>Gamma0>>tmp>>Bcoef>>tmp>>Dcoef;
      return 1;
    }
int TetaL_SigmaLT::save_data_state(FilterOut& output)
    { output<<" Temp_sa "<<Temp_sa<<" Temp_ca "<<Temp_ca<<" sigL_sigC "<<sigL_sigC
      <<" PowRelease "<<PowRelease<<" PowX0 "<<PowX0<<" PowCoef "<<PowCoef<<" PowPow "<<PowPow
      <<" Teta0 "<<Teta0<<" Gamma0 "<<Gamma0
      <<" Bcoef "<<Bcoef<<" Dcoef "<<Dcoef;
	  return 1;
    }

//    ========================================================================
//    ====================== TetaL_SigmaLT_Short =============================
//    ========================================================================

double TetaL_SigmaLT_Short::Teta(double sigL)
  {
   double x=log(max<double>(MathZer,sigL));
   double ret=Teta0*exp( (Gamma0-2./3)*(sqr(Bcoef)+sqr(Dcoef))/Bcoef
                                    *atan(x*Bcoef/(sqr(Bcoef)+Dcoef*(x+Dcoef))) );
   return ret;
  }
double TetaL_SigmaLT_Short::Calculate(double sigL,double temp)
   { 
//     double Coef=1;
     double ret=Temp_sa*(Temp_ca*Teta(sigL)+temp)*pow(sigL,2./3)/(Temp_ca+temp);
     return ret;
   };
int TetaL_SigmaLT_Short::read_data_state(FilterIn& input)
    { char tmp[256];input>>tmp>>Temp_sa>>tmp>>Temp_ca>>
                           tmp>>Teta0>>tmp>>Gamma0>>tmp>>Bcoef>>tmp>>Dcoef;
      return 1;
    }
int TetaL_SigmaLT_Short::save_data_state(FilterOut& output)
    { output<<" Temp_sa "<<Temp_sa<<" Temp_ca "<<Temp_ca
      <<" Teta0 "<<Teta0<<" Gamma0 "<<Gamma0<<" Bcoef "<<Bcoef<<" Dcoef "<<Dcoef;
	  return 1;
    }

//    ========================================================================
//    ====================== FreeM_SigmaMT ===================================
//    ========================================================================

double FreeM_SigmaMT::Calculate(double sigM,double temp)
{ 
//cout<<sigM<<" ";
//  return 3*( 2*sqr(sigM)*Temp_m0/(1+pow(sigM,3))*(Cm+2.*Am/5*(pow(sigM,5./3)-1)+(Bm-Cm)*temp) );}
  double sigM2=sqr(sigM),sigM3=sigM2*sigM;
  double MulCoef=2*sigM2*Temp_m0/(1+sigM3);
  return 3*MulCoef*(Cm+2.*Am/5*(pow(sigM,PowAm)-1))+3*(Bm-Cm)*temp;
}
int FreeM_SigmaMT::read_data_state(FilterIn& input)
  { char tmp[256];input>>tmp>>Temp_m0>>tmp>>Am>>tmp>>Bm>>tmp>>Cm>>tmp>>PowAm;
    return 1;}
int FreeM_SigmaMT::save_data_state(FilterOut& output)
  { output<<" Temp_m0 "<<Temp_m0<<" Am "<<Am<<" Bm "<<Bm<<" Cm "<<Cm<<" PowAm "<<PowAm;return 1;}
  
