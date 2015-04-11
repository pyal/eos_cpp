#include <stdio.h>
#include <stdlib.h>
#include <iomanip.h>
#include <fstream.h>
#include <ctype.h>


#include "util.h"
//#include "..\urs\ros_equ.h"
//extern unsigned _stklen=32000;
double StndErr;

//#include <stdio.h>
//#include <windows.h>
//#include <process.h>

#include <iostream.h>
#include <fstream>
#include <math.h>
#include <const.h>

#define FMT "    "


#include "quad.h"



static int IterCounter=0;
void PrintIter(int n,double *x,double f,double *grad=NULL)
 {
  int k;
  cout<<"\n Iter "<<IterCounter++<<" Cur min "<<f<<"\n";
  cout<<"Cur vect ";
  for (k=0;k<n;k++) cout<<x[k]<<" ";cout<<"\n";
  if (grad!=NULL)
   {
    cout<<"Cur grad ";
    for (k=0;k<n;k++) cout<<grad[k]<<" ";cout<<"\n";
   }
 }

void sfun_(double *x, double *g,int n,  double &f)
{
 f=0;
 for (int k=0;k<n;k++)
  {
   double t=x[k]-k-1;
   f+=sqr(t);
   double add=(1+(rand()/(double)RAND_MAX-0.5)*0.00001);
   g[k]=2*t*add;
  }
  PrintIter(n,x,f,g);
 }

double fun1(double *x, int n)
{
 double f=0;
 for (int k=0;k<n;k++)
  {
   double t=x[k]-k-1;
   double add=(1+(rand()/(double)RAND_MAX-0.5)*0.000003);
//cout<<add<<"\n";
   f+=sqr(t)*add;
//   g[k]=2*t;
  }
//  PrintIter(n,x,f);
  return f;
 }

void main( int argc, char *argv[] )
 {
  srand( (unsigned)time( NULL ) );

cout<<NashMin_Mach_Epsilon<<"\n";
  NashMin_Mach_Epsilon=1e-5;
  StndErr=1e-6;
  int err,N=10,k;
  double x_start[20],est_sol=1,stp[20],low[20],up[20];
  for (k=0;k<N;k++) {x_start[k]=(k+1.)/(N+1);low[k]=0;up[k]=6;}
  for (k=0;k<N;k++) stp[k]=0.001;

  SetLeakTest();

//  double ret=UncMin(err,10,&x_start[0],&x_start[0],est_sol,NULL , NULL,sfun_,low,up);
  double ret=UncMin(err,10,&x_start[0],&x_start[0],est_sol,fun1,&stp[0],NULL,low,up );
  LeakTest();

 }


/*

void sfun_(double *x, double *g,int n,  double &f)
{
 f=0;
 for (int k=0;k<n;k++)
  {
   double t=x[k]-k-1;
   f+=sqr(t);
   g[k]=2*t;
  }
 }

double fun1(double *x, int n)
{
 double f=0;
 for (int k=0;k<n;k++)
  {
   double t=x[k]-k-1;
   f+=sqr(t);
//   g[k]=2*t;
  }
 return f;
 }

void main( int argc, char *argv[] )
 {
  StndErr=1e-6;
  int err,N=10,k;
  double x_start[20],est_sol=1,stp[20];
  for (k=0;k<N;k++) x_start[k]=(k+1.)/(N+1);
  for (k=0;k<N;k++) stp[k]=0.0000001;

  SetLeakTest();

//  double ret=UncMin(err,10,&x_start[0],&x_start[0],est_sol,NULL , NULL,sfun_);
  double ret=UncMin(err,10,&x_start[0],&x_start[0],est_sol,fun1,&stp[0] );
  LeakTest();

 }


*/

/*
double *SetArray(int NumPnt)
{ double *tmp=new double[NumPnt+1];tmp[0]=NumPnt;return tmp;}
void GenerateXY(double *&x,double *&y,double from,double upto,int NumPnt)
 {
  x=SetArray(NumPnt);y=SetArray(NumPnt);
  double Stp=(upto-from)/(NumPnt-1),Cur=from;
  for (int k=1;k<=NumPnt;k++)  {x[k]=Cur;y[k]=log(Cur);Cur+=Stp;}
 };
void GenerateCoef(double *x,double *y,double *&SinCoef,double *&CosCoef,double &BaseVal)
 {
  int NumPnt=x[0];
  SinCoef=SetArray(NumPnt/2); CosCoef=SetArray(NumPnt/2);
//  double MainHarm=2*M_PI/(x[NumPnt]-x[1]);
  double MainHarm=2*M_PI/NumPnt;
  for (int j=1;j<=NumPnt/2;j++)
   {
    CosCoef[j]=0;
    SinCoef[j]=0;
    for (int k=1;k<=NumPnt;k++)
     {
      CosCoef[j]+=y[k]*cos((j-1)*MainHarm*k);
      SinCoef[j]+=y[k]*sin((j-1)*MainHarm*k);
     }
    CosCoef[j]*=2./NumPnt;
    SinCoef[j]*=2./NumPnt;
//    if (j==1) { CosCoef[j]=10;SinCoef[j]=0;continue;}
//    CosCoef[j]=-0.2/pow(j,0.1);
//    SinCoef[j]=-0.1/pow(j,0.1);
   }
  CosCoef[NumPnt/2]*=0.5;
  BaseVal=0;for (int k=1;k<=NumPnt;k++) BaseVal+=y[k];BaseVal/=NumPnt;
 };
void Reconstruct(double *SinCoef,double *CosCoef,double BaseVal,double *x,double *&Restore)
 {
  int NumPnt=x[0];
  Restore=SetArray(NumPnt);
  double MainHarm=2*M_PI/NumPnt;
  for (int k=1;k<=NumPnt;k++)
   {
    Restore[k]=-BaseVal;
    for (int j=1;j<=NumPnt/2;j++)
     {
      Restore[k]+=CosCoef[j]*cos((j-1)*MainHarm*k)+
                      SinCoef[j]*sin((j-1)*MainHarm*k);
     }
   }
 };  


int main(int argc, char *argv[])
{
  fstream in(argv[1],ios::in);
  fstream out(argv[2],ios::out);
  double from,upto;
  int NumPnt,k;
  char tmp[256];
  in>>tmp>>from>>tmp>>upto>>tmp>>NumPnt;
  double *SinCoef,*CosCoef,*x,*y,BaseVal,*Restore;
  GenerateXY(x,y,from,upto,NumPnt);
  GenerateCoef(x,y,SinCoef,CosCoef,BaseVal);
  Reconstruct(SinCoef,CosCoef,BaseVal,x,Restore);

  for (k=1;k<=SinCoef[0];k++) {SinCoef[k]=SinCoef[k];CosCoef[k]=CosCoef[k]*sqrt(1./k);}

//  for (k=1;k<=SinCoef[0];k++) out<<1000/x[k]<<FMT<<y[k]<<FMT<<Restore[k]<<FMT<<SinCoef[k]<<FMT<<CosCoef[k]<<"\n";
  for (k=2;k<=SinCoef[0];k++) out<<1000/x[k]<<FMT<<SinCoef[k]<<FMT<<CosCoef[k]<<"\n";
  out.close();in.close();
  delete x;delete y;delete CosCoef;delete SinCoef;delete Restore;
}

*/

/*
void main( int argc, char *argv[] )
  {
   StndErr=1e-6;
   if (argc<=2) 
     {cout<<" use :  \n1.exe get_file put_file calc_mtd_file [options]\n"<<
            "options:\n   /r - number of rows to output \n"<<
		      "       (use for calculation in the input only first row)\n"<<
                      "   /n - no header information\n"<<
                      "   /c - number of columns to output\n";exit(1);}

   fstream in(argv[1],ios::in);
   fstream out(argv[2],ios::out);
//   facet fac=use_facet(in);
   cout<<hex;
   in.fill('.');
for (int k=0;k<=256;k++) out<<hex<<k<<" "<<dec<<k<<" = "<<oct<<_pctype[k]<<" "<<dec<<_pctype[k]<<" "<<char(k)<<"\n";	   
_pctype[32]=16;_pctype[46]=72;
   char tmp[256];
   while ( !seek_eof(in) )
   {
	   in>>tmp;out<<tmp<<"\n";

   };
  };
*/
/*
void main( int argc, char *argv[] )
  {
   StndErr=1e-6;
   if (argc<=2) 
     {cout<<" use :  \n1.exe get_file put_file calc_mtd_file [options]\n"<<
            "options:\n   /r - number of rows to output \n"<<
		      "       (use for calculation in the input only first row)\n"<<
                      "   /n - no header information\n"<<
                      "   /c - number of columns to output\n";exit(1);}

   fstream in(argv[1],ios::in);
   fstream out(argv[2],ios::out);
   out<<setiosflags(ios::showpoint|ios::scientific|ios::left);
   double ZoneWidth,LowDegener,UpDegener,T,Dens,MolVeight,FreeE,Ion;
   char tmp[256];
   while ( !seek_eof(in) )
   {
	   in>>tmp>>ZoneWidth>>tmp>>LowDegener>>tmp>>UpDegener
		 >>tmp>>T>>tmp>>Dens>>tmp>>MolVeight;
	   FreeE=FreeESemiConductor(ZoneWidth,LowDegener,UpDegener,MolVeight,Dens,T);
       Ion=IonizeEnergy(ZoneWidth*M_eV_K*M_Rconst/MolVeight,0,MolVeight,
		   T,LowDegener*UpDegener,1/(Dens*MolVeight));
	   out<<" T "<<T<<" Dens "<<Dens<<" FreeE "<<FreeE
		  <<" Ion "<<Ion<<"\n";

   };
  };

*/