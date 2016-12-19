#include "h2_liqui.h"

ColdParamStruct ColdParam;
LiquidParamStruct LiquidParam;
#define CP ColdParam
#define LP LiquidParam

double DebyFuncInt(double x) {if (x==0) return 0;return x*x*x/(exp(x)-1);};
double DebyFunc(double x) 
  {return 3/(x*x*x)*RecInt(DebyFuncInt,0,x,StndErr);};

double Pcold(double x)
  {
   double s=0;
   for (int k=1;k<=ColdParam.Nparam;k++) 
     s+=ColdParam.coef[k]*pow(x,k/3.+1);
   return s;
  };
double Grun(double x)
  {
   double s1=0,s2=0,Add=3-2*ColdParam.GrunN,cur;
   for (int k=1;k<=ColdParam.Nparam;k++) 
     {cur=ColdParam.coef[k]*pow(x,k/3.+1)*(k+Add);
      s1+=cur*k;s2+=cur;}
   double ret=1./3.+s1/(6*s2);
//cout<<x<<" "<<ret<<" "<<x/Teta(x)*(Teta(x+StndErr)-Teta(x))/StndErr<<"\n";
//   ret=x/Teta(x)*(Teta(x+StndErr)-Teta(x))/StndErr;
   return ret;
  };

double Teta(double x)
  {
   double s1=0,s2=0,Add=3-2*ColdParam.GrunN;
   for (int k=1;k<=ColdParam.Nparam;k++) 
     {
      s1+=ColdParam.coef[k]*pow(x,k/3.+1)*(k+Add);
      s2+=ColdParam.coef[k]*(k+3);
     }
   if ( (fabs(s1)<MathZer) || (s1*s2<MathZer) )    return 0;
   return ColdParam.Teta0*pow(x,-1./6.)*sqrt(s1/s2);
//   return ColdParam.Teta0*sqrt(s1/s2)*pow(x,1./3.); // Bad !!!!!!!!!!!!! Good Previous.
  };

double Zparam(double x,double T)
  { 
//   return 0;
   double s=0,Add=3-2.*ColdParam.GrunN;
   for (int k=1;k<=ColdParam.Nparam;k++) 
      s+=ColdParam.coef[k]*pow(x,k/3.+1)*(k+Add); 
   s=s/(3*x*ColdParam.Denc);
   return LiquidParam.Lconst*ColdParam.Rconst*T/s; 
  };


double Ecold(double x)
  {
   double s1=0;
   for (int k=1;k<=ColdParam.Nparam;k++) 
      s1+=ColdParam.coef[k]*(pow(x,k/3.)-1)/k;
   return s1*3/ColdParam.Denc;
  };

double Esolid(double x,double T)  
  {
   double R=CP.Rconst,teta=Teta(x);
   return Ecold(x)+R*(LP.IncludeNullE*teta*9/8+
                          3*T*DebyFunc(teta/T))+
                          LP.IncludeTurnE*R*T;
  };                                            
   
double Psolid(double x,double T)
  {
   double G=Grun(x),R=CP.Rconst,D=x*CP.Denc,teta=Teta(x);
   return Pcold(x)+R*D*G*(LP.IncludeNullP*teta*9/8+
                          3*T*DebyFunc(teta/T))+
                          LP.IncludeTurnP*R*D*T*2/3;
  };                                            

double CCCa,CCCteta;
double MisT(double T)
  {return CCCa-T*CP.Rconst*(3*DebyFunc(CCCteta/max<double>(T,StndErr))+LP.IncludeTurnE);}
double TemperatureS(double r,double e)
  {
   double x=r/ColdParam.Denc;
   double teta=Teta(x),T=1000;
   double de=e-Ecold(x)-LP.IncludeNullE*ColdParam.Rconst*9*teta/8;
   CCCa=de;CCCteta=teta;
   if (!ZeroNewtonIt(MisT,T,StndErr*1e-3,10000,1e5,MathZer,T/2))
        {cout<<" Bad TemperatureS in H2Solid Not Zero"<<MisT(T)<<"\n";}
   return T;
  };
double PressureS(double r,double e)
  {  return Psolid(r/CP.Denc,TemperatureS(r,e)); };







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
   double x=Denc/CP.Denc,AddT;
   double Ec=Ecold(x),Te=Teta(x),z=Zparam(x,T),b=LP.Bconst,
          T0=LP.T0const,TetaTurn=LP.T_Turn,r=LP.Rconst;
   double f=LP.Cconst+LP.Aconst/r*(pow(Denc/LP.Denc0const,r)-1);

   if (LP.IncludeTurnP!=0) TetaTurn*=pow(Denc/CP.DencH,2./3);
   if (LP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   double F=Ec*LP.IncludeZeroE+CP.Rconst*
        (LP.IncludeNullE*9/8*Te+3*T*(log(Te/T*sqrt(1+z))-b)+
                                3*T0*f-T*AddT);
   return F;
  };

double FSolid(double Denc,double T)
  {
   double x=Denc/CP.Denc,AddT;
   double Ec=Ecold(x),Te=Teta(x),TetaTurn=LP.T_Turn,
          Deby=DebyFunc(Te/T);
   if (LP.IncludeTurnP!=0) TetaTurn*=pow(Denc/CP.DencH,2./3);
   if (LP.IncludeTurnE==0) AddT=0;
   else AddT=log(T*0.5/TetaTurn);
   double F=Ec*LP.IncludeZeroE+CP.Rconst*
        (9/8*Te*LP.IncludeNullE+T*(3*log(1-exp(-Te/T))-Deby-AddT));
   return F;
  };


void H2ReadPar(fstream &in)
  {
			char tmp[256];
   in>>tmp>>CP.Denc>>tmp>>CP.DencH>>tmp>>CP.GrunN>>tmp>>CP.Teta0;
			in>>tmp>>CP.Nparam;
			for (int k=1;k<=CP.Nparam;k++) in>>tmp>>CP.coef[k];
			in>>tmp>>LP.Aconst>>tmp>>LP.Bconst>>tmp>>LP.Cconst>>tmp>>LP.Rconst
      >>tmp>>LP.Lconst>>tmp>>LP.Denc0const>>tmp>>LP.T0const>>tmp>>LP.T_Turn
						>>tmp>>LP.IncludeTurnE>>tmp>>LP.IncludeNullE>>tmp
      >>LP.IncludeTurnP>>tmp>>LP.IncludeNullP>>tmp>>LP.IncludeZeroE;
  };



/*
double PressureL(double r,double e)
  {
   double x=r/ColdParam.Denc;
   if (x<=0) cout<<" Error x in pressureL\n";
   double DeltaE=e-Ecold(x);
//cout<<" X "<<x<<" DeltaE "<<DeltaE<<"  ";
   return Pliquid( x,FindTemp(x,DeltaE) );
  };
*/
/*
// / *
double Ecold_Ehug(double x)
  {
   return Ecold(x)-0.5*Pcold(x)*(1-1/x)/ColdParam.Denc;
  };
double Ccold2(double x)
  {
   double s=0;
   for (double k=1;k<=ColdParam.Nparam;k++) 
     s+=ColdParam.coef[k]*pow(x,k/3.+1)*(k+3);
   s=s/(3*x*ColdParam.Denc);
//cout<<" C2 = "<<ret<<" x param "<<x<<"\n";
   return  s;
  };
double PNull(double x)
  {  return Pcold(x)+9/8*ColdParam.Rconst*x*ColdParam.Denc*Grun(x)*Teta(x); };
// * /
   
double Pliquid(double x,double T)
  {
//   double r=x*ColdParam.Denc;
   double z=Zparam(x,T);
   double D=x*ColdParam.Denc;
   return Pcold(x)+3*ColdParam.Rconst*D*
             (T*(Grun(x)+z/3.)/(1+z)+
                LiquidParam.T0const*LiquidParam.Aconst*
//                       pow(x/LiquidParam.Teta0const,LiquidParam.Rconst));
                       pow(D/LiquidParam.Denc0const,LiquidParam.Rconst));
  };

double Eliquid(double x,double T)
  {
//   double r=x*ColdParam.Denc;
   double z=Zparam(x,T);
   double D=x*ColdParam.Denc;
   return Ecold(x)+3*ColdParam.Rconst*(T*(1+z/2)/(1+z)+
          LiquidParam.T0const*(LiquidParam.Cconst+
                LiquidParam.Aconst/LiquidParam.Rconst*
//                  (pow(x/LiquidParam.Teta0const,LiquidParam.Rconst)-1)));
                  (pow(D/LiquidParam.Denc0const,LiquidParam.Rconst)-1)));
  };
double FindTemp(double x,double DeltaE)
  {
   double Gamma=Zparam(x,1);
   double D=x*ColdParam.Denc;
   double Betta=3*ColdParam.Rconst*
                LiquidParam.T0const*(LiquidParam.Cconst+
                LiquidParam.Aconst/LiquidParam.Rconst*
//                (pow(x/LiquidParam.Teta0const,LiquidParam.Rconst)-1));
                (pow(D/LiquidParam.Denc0const,LiquidParam.Rconst)-1));
   
   double a=(DeltaE-Betta)/(3*ColdParam.Rconst);
   double T1,T2;
   if ( !SqPolynom(Gamma/2,(1-Gamma*a),-a,T1,T2) ) 
       cout<<"Could not Find Temp\n" ;
//cout<<" FindTemp  x "<<x<<" Tf  "<<T1<<" Tw "<<T2<<"\n";
//   return T1;
   return max<double>(T1,T2);
  };
*/
/*
struct PorousParamStruct
  {
   double M,N,B0c,Esub;
  } ;
extern PorousParamStruct PorousParam;
  PorousParam.M=3;
  PorousParam.N=1;
  PorousParam.B0c=1;
  PorousParam.Esub=1;
double EcoldPorous(double x)
  {
   double M=PorousParam.M,N=PorousParam.N;
   return PorousParam.B0c/(ColdParam.Denc*(M-N))*(pow(x,M)/M-pow(x,N)/N)
                                         +PorousParam.Esub;
  }
double PcoldPorous(double x)
  {
   double M=PorousParam.M,N=PorousParam.N;
   return PorousParam.B0c/(M-N)*(pow(x,M+1)-pow(x,N+1));
  };                           
                                              
double Ccold2Porous(double x)
  {
   double M=PorousParam.M,N=PorousParam.N;
   return PorousParam.B0c/(ColdParam.Denc*(M-N))*
          ((M+1)*pow(x,M+1)-(N+1)*pow(x,N+1));
  };

double GrunPorous(double x)
  {
   double M=PorousParam.M,N=PorousParam.N;
   return 1./3.+0.5*(M*pow(x,M+1)-N*pow(x,N+1))/(pow(x,M+1)-pow(x,N+1));
  };

double TetaPorous(double x)
  {
   double M=PorousParam.M,N=PorousParam.N,Add=-2.*ColdParam.GrunN/3;
   double s1=(M+1-Add)*pow(x,M+1)-(N+1-Add)*pow(x,N+1);
   double s2=(M+1)*pow(x,M+1)-(N+1)*pow(x,N+1);
   if ( (fabs(s1)<MathZer) || (s1*s2<MathZer) )
     {
cout<<"TetaPorous Func is Infinity!!!!!!! s1 "<<s1<<" s2 "<<s2<<"\n";
//      for (k=1;k<=ColdParam.Nparam;k++) 
//         cout<<" ColdParPorous "<<ColdParam.coef[k];
//					 cout<<"  ColdParam.GrunN "<<ColdParam.GrunN<<"\n";
						return 0;}
   return ColdParam.Teta0*pow(x,1./3.)*sqrt(s1/s2);
  };

double ZparamPorous(double x,double T)
  { 
   double M=PorousParam.M,N=PorousParam.N,Add=-2*ColdParam.GrunN/3;
   double s=((M+1-Add)*pow(x,M+1)-(N+1-Add)*pow(x,N+1))/(x*ColdParam.Denc);
   return LiquidParam.Lconst*ColdParam.Rconst*T*PorousParam.B0c/(M-N)/s; 
  };

double InFunc1(double x)
  {
   return Ccold2(x)-ColdParam.GrunN*2*Pcold(x)*ColdParam.Denc/(3*x);
  };
double Grun1(double x)
  {
   double Cfunc=InFunc1(x),XStp=0.01*x;
   double Derive=(InFunc1(x+XStp)-Cfunc)/(XStp*Cfunc);
   return 1./3+0.5*Derive;
  };

double InFunc(double x)
  {
   double s=0,s1=0,b;
   for (int k=1;k<=ColdParam.Nparam;k++) 
     {
      b=ColdParam.coef[k]*(k+3-2*ColdParam.GrunN)*pow(x,(double)k/3);
      s+=b*k;s1+=b;
     }
   if (fabs(s1)<MathZer) {cout<<"InFunc is Infinity!!!!!!!\n";return 1;}
   b=s/(s1*3);
   return b;
  };

double Grun(double x)
  {
   return 1./3+0.5*InFunc(x);
  };

double Teta1(double x)
  {
   return ColdParam.Teta0*sqrt(InFunc1(x))*pow(x,(double)1/3)/sqrt(Ccold2(1));
  };
double TetaOld(double x)
  {
   double s=0,s1=0,b;
   for (int k=1;k<=ColdParam.Nparam;k++) 
     {
      s+=ColdParam.coef[k]*(k+3-2*ColdParam.GrunN)*pow(x,(double)k/3);
      s1+=ColdParam.coef[k]*k;
     }
   if ((fabs(s1)<MathZer) || (s*s1<0)) 
     {cout<<"Teta Func is Infinity!!!!!!!\n";return 1;}
   b=ColdParam.Teta0*pow(x,(double)1/2)*sqrt(s/s1);
   return b;
  };
double Teta(double x)
  {
   double s=0,s1=0,b;
   for (int k=1;k<=ColdParam.Nparam;k++) 
     {
      s+=ColdParam.coef[k]*(k+3-2*ColdParam.GrunN)*pow(x,(double)k/3);
      s1+=ColdParam.coef[k]*(k+3                  )*pow(x,(double)k/3);
     }
   if ((fabs(s1)<MathZer) || (s*s1<0)) 
     {cout<<"Teta Func is Infinity!!!!!!!\n";return 1;}
   b=ColdParam.Teta0*pow(x,(double)1/3)*sqrt(s/s1);
   return b;
  };
*/
