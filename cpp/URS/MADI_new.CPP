#include "matt_reg.h"
//#include "matt_h2.h"
//#include <alloc.h>
#include "fre_ros.h"

double StndErr;

extern unsigned _stklen=64000;
//extern int NumHugIteration=5;

void main(int argc,char *argv[])
  {
   Time_struct Time;
   fstream in(argv[1],ios::in);
   char tmp[512];
   StndErr=1e-6;
   double T,Sub_liq;
   in>>tmp>>T>>Sub_liq;
//   MatterIO *Matter;
   FreeERossH2 Ross;Ross.ReadIni(in);
   double Denc,V;
 //  double T=100,Fmet,Fmol;
   while (!seek_eof(in))
	  {in>>V;Denc=2/V;
       double AddMet=2975+Sub_liq;AddMet=3075-50;AddMet/=2;//new 3040
       double Fmet=Ross.Fmet->FreeE(Denc,T);Fmet=Fmet+AddMet;
       double Ros=Ross6Config(*(Ross.par),Ross.Fid->MolVeight,Denc,T);
	   double Id=Ross.Fid->FreeE(Denc,T);
       double Fmol=Id+Ros;
//	   double Fmix=Ross.FreeE(Denc,T);
       cout<<V<<" "<<2*(Fmet-AddMet)/1313<<" "<<2*(Fmol-AddMet-Sub_liq)/1313
		   <<" "<<2*(Fmet-Fmol-Sub_liq)/1313<<"\n";
	  }
cout<<" Coreleft() "<<Coreleft()<<" Stack "<<StackAvail()<<Time<<"\n";

//delete par;
  };
/*
// ===================== Control Ross Correlation block ====
void main(int argc,char *argv[])
  {
   Time_struct Time;
   fstream in(argv[1],ios::in);
   char tmp[512];
   StndErr=1e-6;
//   MatterIO *Matter;
//   FreeERoss Matter;Matter.ReadIni(in);
	Exp6Param *par=NULL;Exp6Read(in,par);
    double Niu_min,F_min;
	double Dens,Temp,Pack;
   while (!seek_eof(in))
   {in>>Dens>>Temp>>Pack;
       cout<<Dens<<" "<<Temp<<" "<<Pack<<" "<<FreeRCheck(par,Dens,Temp,Pack,2,Niu_min,F_min);
	   cout<<" Niu "<<Niu_min<<" F "<<F_min<<"\n";
   };
cout<<" Coreleft() "<<Coreleft()<<" Stack "<<StackAvail()<<Time<<"\n";

delete par;
  };
/*
// Hauke Calculation for H2
# n in 1/A^3   T in K       F in 1       d in A       eta in 1
1.806551e-02 1.000000e+03 8.533399e+01 2.181457e+00 9.819480e-02
1.806551e-02 4.000000e+03 5.276702e+02 1.793881e+00 5.460463e-02
1.806551e-02 7.000000e+03 8.795723e+02 1.616890e+00 3.998439e-02
1.385022e-01 1.000000e+03 7.930592e+03 1.807741e+00 4.284140e-01
1.385022e-01 4.000000e+03 1.208354e+04 1.522100e+00 2.557320e-01
1.385022e-01 7.000000e+03 1.496683e+04 1.361978e+00 1.832168e-01
2.586793e-01 1.000000e+03 2.744291e+04 1.582201e+00 5.364695e-01
2.589389e-01 4.000000e+03 3.493386e+04 1.300634e+00 2.983055e-01
2.589389e-01 7.000000e+03 3.813378e+04 1.190453e+00 2.287351e-01

// Configuration file for calculation
Exp_6_en 36.4 Exp_6_alph 11.1 Exp_6_r 3.43  Exp_6_r0 1.45855   
5.999701e-02   1.000000e+03  9.819480e-02   
5.999701e-02   4.000000e+03  5.460463e-02   
5.999701e-02   7.000000e+03  3.998439e-02   
4.599769e-01   1.000000e+03  4.284140e-01   
4.599769e-01   4.000000e+03  2.557320e-01   
4.599769e-01   7.000000e+03  1.832168e-01   
8.590947e-01   1.000000e+03  5.364695e-01   
8.599569e-01   4.000000e+03  2.983055e-01   
8.599569e-01   7.000000e+03  2.287351e-01   
*/
/*
// ===================== Control Ross Correlation block ====
*/










