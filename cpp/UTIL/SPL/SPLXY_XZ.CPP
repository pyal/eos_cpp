#include <alloc.h>

#include "dat_fac.h"
#include "spl_fac.h"
#include "spl_reg.h"
#include "dat_reg.h"

#include "matlib.h"
#include "zero.h"

// From xy_z spline build xz_y spline
//  spl_file file_data file_spl_reg spl_build_cfg file_spl
//  spl_build_cfg - of the form 
//  NumX  10 NumY  10 Meth  1 Misf 1e-4

double StndErr=1e-6;

extern unsigned _stklen=60000;


void Reconstruct_d1(TData<real> *d1,double &MinY,double &MaxY)
  {
			int k;
			double minz=MathInf,maxz=-MathInf,cury,stpy,tmp;
			MinY=min(d1->D[1][0],d1->D[1][d1->I[1]-1]);
			MaxY=max(d1->D[1][0],d1->D[1][d1->I[1]-1]);
   for (k=0;k<d1->I[2];k++) 
			  {tmp=d1->D[2][k];minz=min(minz,tmp);maxz=max(maxz,tmp);}
			cury=minz;stpy=(maxz-minz)/(d1->I[1]-1);
   for (k=0;k<d1->I[1];k++) {d1->D[1][k]=cury;cury+=stpy;}
		};


double WorkZ;
double WorkX;
SplineXYIO *WorkSpline;
int WorkCol;

double Delta(double pnt)
  {
   double x=WorkX,y=pnt,z;
   WorkSpline->Evaluate (&x,&y,&z,1);
   return z-WorkZ;
  };

void FormData(TData<real> *dat,SplineXYIO *s,double MinY,double MaxY)
  {
   WorkSpline=s;                           
   int NX=dat->I[0],NY=dat->I[1];
   int kx,ky,n=-1;
   real T;
   for (kx=0;kx<NX;kx++)
     {
      for (ky=0;ky<NY;ky++)
        {
         n++;
									WorkX=dat->D[0][kx];WorkZ=dat->D[1][ky];
         dat->D[2][n]=ZeroIn(MinY,MaxY,Delta,StndErr);
        };
     };
  };                                                     
			


void main( int argc, char *argv[] )
  {
// argv1 - Tab
// argv2 - Spl_reg
// argv3 - Spl_cfg
// argv4 - Spl_out
cout<<" coreleft "<<coreleft()<<"\n";
   
   fstream in(argv[1],ios::in);
   fstream in1(argv[2],ios::in);
   fstream scfg(argv[3],ios::in);
			char tmp[256],name[256];
			int NumX,NumY,Meth;
			double Misf;
   scfg>>tmp>>NumX>>tmp>>NumY>>tmp>>Meth>>tmp>>Misf>>tmp>>name;
			seek_eoln(scfg);scfg.close();

   fstream out(argv[4],ios::out);
   SplineXYIO *s=NULL;TData<real> *d1=NULL,*d=NULL;DataRegister("TTable",d1);
   if ( !(DataRegister(in,d))   || (!SurfRegister(in1,s)) ) 
                  {cout<<"could not register spline or data\n";exit(0);}
			*d1=*d;delete d;
   s->Evaluate(*d1);
			double MinY,MaxY;
			Reconstruct_d1(d1,MinY,MaxY);FormData(d1,s,MinY,MaxY);
			s->SetLimits(*d1);
   int res=s->Generate(NumX,NumY,Misf,Meth,*d1);
cout<<"Generated res "<<res<<" Misf "<<Misf
    <<" NumX "<<NumX<<" NumY "<<NumY<<"\n";
   SurfStore(out,s);
  };
 
