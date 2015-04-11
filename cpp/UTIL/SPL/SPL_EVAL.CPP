#include <alloc.h>

#include "dat_fac.h"
#include "spl_fac.h"
#include "spl_reg.h"
#include "dat_reg.h"

#include "matlib.h"

//  Evaluates spline from  data(column) 
//  spl_eval file_data file_spl res_dat
double StndErr=1e-6;

extern unsigned _stklen=60000;



void main( int argc, char *argv[] )
  {
cout<<" coreleft "<<coreleft()<<"\n";
   
   fstream da(argv[1],ios::in);
   fstream sp(argv[2],ios::in);
   fstream out(argv[3],ios::out);
   TData<real> *d=NULL;
   SplineXYIO *s=NULL;TData<real> *d1=NULL;DataRegister("TColumn",d1);
   if ( !(DataRegister(da,d))  || (!SurfRegister(sp,s)) )
                  {cout<<"could not register spline or data\n";exit(0);}
   s->Evaluate(*d);*d1=*d;
   DataStore(out,d1);out.close();
  };

