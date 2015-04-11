//#include <alloc.h>

#include "dat_fac.h"
#include "spl_fac.h"
#include "spl_reg.h"
#include "dat_reg.h"

//#include "matlib.h"

//  converts from file data(xyz) to file data(yxz) 
//  datxy_yx file_data file_spl res_dat
double StndErr=1e-6;

extern unsigned _stklen=60000;



void main( int argc, char *argv[] )
  {
cout<<" coreleft "<<Coreleft()<<"\n";
   
   fstream da(argv[1],ios::in);
   fstream out(argv[2],ios::out);
   TData<real> *d=NULL;
   TData<real> *d1=NULL;DataRegister("TTable",d1);
   if ( !(DataRegister(da,d)))  {cout<<"could not register data\n";exit(0);}
   *d1=*d;
   real *buf=new real[d1->I[2]];
   for (int k=0;k<d1->I[0];k++)
    { for (int k1=0;k1<d1->I[1];k1++)  buf[k1*d1->I[1]+k]=d1->D[2][k*d1->I[0]+k1];}

   real *tmp=d1->D[0];d1->D[0]=d1->D[1];d1->D[1]=tmp;delete d1->D[2];d1->D[2]=buf;

   int x=d1->I[0];d1->I[0]=d1->I[1];d1->I[1]=x;
   *d=*d1;
   DataStore(out,d);out.close();
  };

