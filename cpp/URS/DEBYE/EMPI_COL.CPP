//  Calculation of liquid parameters a,b,c

#include "util.h"
#include "matt_reg.h"

double StndErr=1e-6;

extern EmpiParamStruct *EmpiColdParam;


void main(int argc,char *argv[])
  {
   StndErr=1e-6;
   fstream in(argv[1],ios::in);

   FreeEEmpiLiquidPres FLiquid;
   InterfaceFreeEIO *FLiq=(InterfaceFreeEIO *)&FLiquid;
   char tmp[256];
   in>>tmp;FLiquid.ReadIni(in);
   fstream pnt(argv[2],ios::in);
   fstream out(argv[3],ios::out);
   double d,ar[8];
   while (!seek_eof(pnt))
     {
      pnt>>d;ar[1]=d;
     double dr=StndErr*d;
     ar[2]=sqr(d)/dr*(EmpiFLiq0(d+dr)-EmpiFLiq0(d));
     ar[3]=sqr(d)/dr*(EmpiFLiq00(d+dr)-EmpiFLiq00(d));
     ar[4]=sqr(d)/dr*(EmpiFreeEMet(d+dr,1)-EmpiFreeEMet(d,1));
     ar[5]=EmpiFLiq0(d);ar[6]=EmpiFLiq00(d);
     ar[7]=EmpiFreeEMet(d,1)+EmpiColdParam->DisEnergy;
     WriteStr(out,&ar[0],7);
     }
//cout<<EmpiColdParam->DisEnergy<<"\n";
   out.close();
   delete EmpiColdParam;
  }; 
