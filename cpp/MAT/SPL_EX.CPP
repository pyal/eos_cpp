//#include "dat_fac.h"
//#include "spl_fac.h"
//#include "spl_reg.h"
////#include <alloc.h>
//#include "dat_reg.h"
//#include "lib/std/f2c.h"
//
//extern unsigned _stklen=63000;
//
//void main( int argc, char *argv[] )
//  {
//   fstream in(argv[1],ios::in);
//   fstream in1(argv[2],ios::in);
//   fstream out(argv[3],ios::out);
//   fstream out1(argv[4],ios::out);
//cout<<" coreleft "<<coreleft()<<"\n";
//   int NumX,NumY,Meth,LogZ;
//   double Misf;
//char tmp[256];in1>>tmp>>NumX>>tmp>>NumY>>tmp>>Meth>>tmp>>Misf>>tmp>>LogZ;
//   TData<real> *d=NULL;
////   TData<double> *d1=NULL;DataRegister("TColumn",d1);
//   SplineXYIO *s=NULL;
////cout<<" Dat Reg \n";
////   DataRegister(in,d);
//   if ( !(DataRegister(in,d))  || (!SurfRegister(in1,s)) )
//                  {cout<<"could not register spline\n";exit(0);}
//cout<<" Dat Spl Reg \n";
//
//   TData<real> *d1=NULL;DataRegister("TColumn",d1);
//   if (LogZ)  for (int k=0;k<d->I[2];k++) d->D[2][k]=log(d->D[2][k]);
//   *d1=*d;
//   DataStore(out1,d1);
//   s->Generate(NumX,NumY,Misf,Meth,*d);
////   *d1=*d;
////   SetDifTData(*d1,*d);
//   SurfStore(out,s);
////   DataStore(out,d1);
//   s->Evaluate(*d1);
//   DataStore(out1,d1);
//   delete d;delete d1;delete s;
//cout<<" coreleft "<<coreleft()<<"\n";
//  };
//
////   DataRegister(in1,d1);
////   DataStore(out,&(*d=*d1));
////   TData<double> *dd=NULL;DataRegister(in1,dd);
////   SetDifTData(*dd,*d);
//
