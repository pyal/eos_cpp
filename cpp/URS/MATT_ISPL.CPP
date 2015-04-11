#include <lib\precompiled\eos.h>
#include "matt_ispl.h"   
//#include "empi_equ.h"
//#include "matt_fac.h"


static ClassDesc TMatterSpl_ClassDesc(typeid(TMatterSpl),"TMatterSpl",1,"Matter_category",create<TMatterSpl>);

TMatterSpl* TMatterSpl::Current;

TMatterSpl::TMatterSpl()
    :SplFile("h2e_ros1.ispl")
    , SplineDesciption("sample spline")
    , DencCold(1e-6)
    , DencMax(100)
    , E2FT_Converter(new TExpConverter(1, 1, 150))
    , SoundStep(StndErr * 10)
    , E2FTmin(0)
    , E2FTmax(100)
    , E2FTzero(5)
    , E2FTeps(StndErr * 0.01) {
}


//void MatterSpl::Pressure(double *P,double *Dencity,double *Energy,int Num)
//  {
//   double *E=new double [Num+1],e,d,*D=new double [Num+1];
//   for (int k=1;k<=Num;k++) if ((e=Energy[k]+AddE)>StndErr) E[k]=log(e); else E[k]=-6;
//   for (int k=1;k<=Num;k++) if ((d=Dencity[k])>StndErr) D[k]=log(d); else D[k]=-9;
//   SPre->Evaluate(&E[1],&D[1],&P[1],Num);
//   for (int k=1;k<=Num;k++) P[k]=exp(P[k])-AddP;
//   delete E;delete D;
//  }; 
//void MatterSpl::Temperature(double *T,double *Dencity,double *Energy,int Num)
//  {
//   int k;
//   if (SThe!=NULL) 
//     {
//      double *E=new double [Num+1],e,d,*D=new double [Num+1];
//      for (k=1;k<=Num;k++) if ((e=Energy[k]+AddE)>StndErr) E[k]=log(e); else E[k]=-6;
//      for (k=1;k<=Num;k++) if ((d=Dencity[k])>StndErr) D[k]=log(d); else D[k]=-9;
//      SThe->Evaluate(&E[1],&D[1],&T[1],Num);
//      for (k=1;k<=Num;k++) T[k]=exp(T[k]);
//      delete E;delete D;
//     } 
//   else {for (k=1;k<=Num;k++) T[k]=0;}
//  }; 


