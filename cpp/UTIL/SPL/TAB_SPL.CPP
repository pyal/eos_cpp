#include <alloc.h>

#include "dat_fac.h"
#include "spl_fac.h"
#include "spl_reg.h"
#include "dat_reg.h"

//#include "matlib.h"

//  Constructs spline from  data(column) wich can be converted in data(table)
//  tab_spl file_data spl_build_cfg file_spl
//  spl_build_cfg - of the form 
//  NumX  40 NumY  40 Meth  0 Misf 1e-4 SplineName RegridSpline
double StndErr=1e-6;

extern unsigned _stklen=60000;



extern void OutputExamp();
void main( int argc, char *argv[] )
  {
cout<<" coreleft "<<coreleft()<<"\n";
   if (argc<2) { OutputExamp();exit(0);}
      
   fstream in(argv[1],ios::in);
   fstream scfg(argv[2],ios::in);
   fstream out(argv[3],ios::out);
			char tmp[256],name[256];
			int NumX,NumY,Meth;
			double Misf;
   scfg>>tmp>>NumX>>tmp>>NumY>>tmp>>Meth>>tmp>>Misf>>tmp>>name;
cout<<" SplineName "<<name<<"\n";                                                     
			seek_eoln(scfg);scfg.close();
   TData<real> *d=NULL;
   SplineXYIO *s=NULL;TData<real> *d1=NULL;DataRegister("TTable",d1);
   if ( !(DataRegister(in,d))  || (!SurfRegister(name,s)) )
                  {cout<<"could not register spline or data\n";exit(0);}
cout<<"Registered"<<" X "<<d->I[0] <<" y "<<d->I[1]<<" z "<<d->I[2]<<"\n";
   *d1=*d;
   fstream out1("tmp0",ios::out);DataStore(out1,d1);out1.close();
   s->SetLimits(*d1);
   int res=s->Generate(NumX,NumY,Misf,Meth,*d1);
cout<<"Generated res "<<res<<" Misf "<<Misf
    <<" NumX "<<NumX<<" NumY "<<NumY<<"\n";
   if (res<=0) SurfStore(out,s);
   out1.open("tmp1",ios::out);DataStore(out1,d1);
   s->Evaluate(*d1);
   out1.close();out1.open("tmp2",ios::out);DataStore(out1,d1);out1.close();
   out.close();
cout<<"Stored\n";
  };

void OutputExamp()
  {cout<<" usage: \ntab_spl data_file cfg_file out_SplFile\n"<<
         " data file  :   TColumn   NumX 3 NumY 10000\n"<<
         " cfg_file   : NumX  100 NumY  100  Meth   0   Misf 1e-6 SplineName RegridSpline\n"<<
         "                             Meth=0 std?                       \n"<<
         " out_SplFile: output Spline\n";
//   fstream examp("cell_dat",ios::out);
//   examp<<
//"BegTime 0   TimeStp 1e-10 TimeWrite   1.5e-7 PresDerivCoef 0.7 EndTime   1e-5  \n"<<
//"NumPnt   400  LftPnt   0  RgtPnt   10  NumPar 0                                \n"<<
//"LftBnd_Free 1 RgtBnd_Free 0 TimeStability 0.02                                 \n"<< 
//"Viscouse 0.7  SecondVisc 0                                                     \n"<<
//"NumMatters 3 Matter_Boundaries 100  200                                        \n"<<
//"MatterABu   Material_File material.cfg Substance Steel-Tern                    \n"<<
//"MatterABu   Material_File material.cfg Substance Pleks-Tern                    \n"<<
//"MatterABu   Material_File material.cfg Substance Cu-Tern                       \n"<<
//"bad                                                                            \n"<<
//"Parameters                                                                     \n"<<
//" P[1]   00  D[1] 0       E[1] 0    V[1]  5.2    Pos[2] 1.52                    \n"<<   
//" P[1]   00  D[1] 0       E[1] 0    V[1]  0  Pos[2] 4.09                        \n"<<  
//" P[1]   00  D[1] 0       E[1] 0    V[1]   0  Pos[2] 4.596                      \n";
//   examp.close();
  }
