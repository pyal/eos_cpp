//  Calculation of liquid parameters a,b,c

#include "util.h"
#include "matt_reg.h"

double StndErr=1e-6;


//double FPres(double Dencity,double Temperature,InterfaceFreeEIO *F)
//  {double dr=StndErr*Dencity;
//cout<<" dr "<<dr<<"\n";
//   return sqr(Dencity)/dr*
//             (F->FreeE(Dencity+dr,Temperature)-F->FreeE(Dencity,Temperature));
//  };

double FPres(double Dencity,double Temperature,InterfaceFreeEIO *F)
  {
//   cout<<" pres "<<F->Pressure(Dencity,Temperature)<<"\n";
   return F->Pressure(Dencity,Temperature);};
   
double FFree(double Dencity,double Temperature,InterfaceFreeEIO *F)
  { 
//   cout<<" FreeE "<<F->FreeE(Dencity,Temperature)<<"\n";
   return F->FreeE(Dencity,Temperature);};
   
double FEntr(double Dencity,double Temperature,InterfaceFreeEIO *F)
  {double dr=StndErr*Temperature;
   return 
     (F->FreeE(Dencity,Temperature)-F->FreeE(Dencity,Temperature+dr))/dr;
  };


void main(int argc,char *argv[])
  {
   StndErr=1e-6;
   fstream in(argv[1],ios::in);

   FreeEEmpiSolid FSolid; 
   FreeEEmpiLiquid FLiquid;
   InterfaceFreeEIO *FSol=(InterfaceFreeEIO *)&FSolid,
                    *FLiq=(InterfaceFreeEIO *)&FLiquid;
   char tmp[256];
   in>>tmp;FSolid.ReadIni(in);
   in>>tmp;FLiquid.ReadIni(in);
   FLiquid.Empi.Aconst=0;FLiquid.Empi.Bconst=0;FLiquid.Empi.Cconst=0;
//   FLiquid.Empi.RLconst=0;
   double r=FLiquid.Empi.Rconst;
//cout<<" r "<<r<<"\n";
   double DencL0,DencS0,T0,P0,dS;
   in>>tmp>>DencL0>>tmp>>DencS0>>tmp>>T0>>tmp>>P0>>tmp>>dS;
   double a= (FPres(DencL0,T0,FLiq)-FPres(DencS0,T0,FSol))/DencL0;
//cout<<" a "<<a<<"\n";
   double b=-(FEntr(DencL0,T0,FLiq)-FEntr(DencS0,T0,FSol)+dS)
                                      /(3*r);
//cout<<" b "<<b<<"\n";
   double c_b=-(FFree(DencL0,T0,FLiq)-FFree(DencS0,T0,FSol)-
               P0*(1/DencL0-1/DencS0))/(3*r*T0);
cout<<" Eval a "<<a<<" b "<<b<<" c "<<c_b+b<<" c-b "<<c_b<<"\n";
  }; 
