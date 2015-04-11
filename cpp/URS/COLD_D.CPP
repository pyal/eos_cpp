#include "util.h"
#include "mtquad.h"
#include <iostream.h>


double StndErr;

double DebyFuncInt(double x) {if (x==0) return 0;return x*x*x/(exp(x)-1);};
double DebyFunc(double x) 
  {return 3/(x*x*x)*RecInt(DebyFuncInt,0,x,StndErr);};

double teta;

double Cv(double T)
  {
   if (T<=MathZer) return 0;
   return (DebyFunc(teta/T)
          -teta/T*(DebyFunc(teta/T+StndErr)-DebyFunc(teta/T))/StndErr);
  };        
double IntE(double T)
  {return RecInt(Cv,0,T,StndErr);};
   
void main(int argc,char *argv[])
  {
   StndErr=1e-8;
   char buf[512];
   fstream in(argv[1],ios::in);
   double r,T,alpha,gamma,MolVeight;
   int Nmol;
   for (int k=1;k<=5;k++) in.getline(buf,512);
//R 0.089 T 13.96 TetaD 122 Gamma 0 alpha 4.8e-3 Nmol 1 MolVeight 2
   in>>buf>>r>>buf>>T>>buf>>teta>>buf>>alpha>>buf>>gamma;
//Rc=R0*(1+a*E/Cv) a/Cv=const E=Int(T0,0)(Cv*dT)
// Cv=3*R* ( D(t/T)-t/T*D'(t/T) )
cout<<" Rc "<<r*(1+alpha/Cv(T)*IntE(T))<<"\n";
//for (k=1;k<=100;k++)
//   cout<<k/10.<<" "<<DebyFunc(k/10.)<<" "<<Cv(k/10.*teta)<<"\n";
  };
