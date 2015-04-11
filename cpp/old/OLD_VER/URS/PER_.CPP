#include "util.h"
#include <conio.h>


//#include <complex.h>
#include "my_comp.h"

double StndErr;
my_comp PercusL(my_comp t,double niu)
  { return 12*niu*(t*(1+0.5*niu)+1+2*niu);}
my_comp PercusS(my_comp t,double niu)
  { return sqr(1-niu)*t*t*t+6*niu*(1-niu)*t*t+18*sqr(niu)*t-12*niu*(1+2*niu);}
double PercusN(double x,double niu,int Order)
  {
   double f=(3+3*niu-sqr(niu))/(4*sqr(niu));
   my_comp xp=pow(my_comp(f+sqrt(sqr(f)+1./8),0),1./3);
   my_comp xm;
   xm=-1./(2*xp);
   double alpha=2*niu/(1-niu);
   my_comp t0=alpha*(-1+xp+xm);
   my_comp t1=alpha*(-1-(xp+xm)/2+my_comp(0,0.5)*sqrt(3)*(xp-xm));
   my_comp t2=alpha*(-1-(xp+xm)/2-my_comp(0,0.5)*sqrt(3)*(xp-xm));
   if (norm(PercusS(t0,niu))+norm(PercusS(t1,niu))+norm(PercusS(t2,niu))>1e-6)
     cout<<" Not zero points?\n";
   my_comp ret=1./(12*niu*x)*(
           exp(t0*(x-Order))*pow(PercusL(t0,niu)/((t0-t1)*(t0-t2)),Order)*t0+
           exp(t1*(x-Order))*pow(PercusL(t1,niu)/((t1-t0)*(t1-t2)),Order)*t1+
           exp(t2*(x-Order))*pow(PercusL(t2,niu)/((t2-t1)*(t2-t0)),Order)*t2);
cout<<" x "<<x<<" res "<<ret<<"\n";
   return real(ret);
  }; 
double Percus(double x,double niu)
  {
   double s=0;
//   for (int k=1;k<x;k++) s+=PercusN(x,niu,k);
   return PercusN(x,niu,1);
   return s;
  };

void main()
  {
   fstream out("per_.dat",ios::out);
   double MinR=1,MaxR=6,MinNiu=0,MaxNiu=1;StndErr=1e-5;
   int NumR=100,NumNiu=100;
//   NumR=100;NumNiu=2;
   MaxNiu=0.7;MaxR=6;
   double StpR=(MaxR-MinR)/NumR,StpNiu=(MaxNiu-MinNiu)/NumNiu;
   double CurR=MinR+0.5*StpR,CurNiu=MinNiu+0.5*StpNiu;
   while (CurR<2)
     {
      CurNiu=MinNiu+0.5*StpNiu;
      while (CurNiu<MaxNiu)
        {
         out<<CurR<<" "<<CurNiu<<" "<<Percus(CurR,CurNiu)<<"\n";
         CurNiu+=StpNiu;
//cout<<"I";
        } 
      CurR+=StpR;
//cout<<" CurR "<<CurR<<" "<<Coreleft()<<"\n";
     }
   out.close();
  } 



/*
#include "mtquad.h"
#define ESC 283
fstream out_it("per_it.dat",ios::out);
double niu,r;
double StndErr;
int NotShowIt;
double IntegFunc(double q)
  {
//cout<<" q "<<q<<" niu "<<niu<<" r "<<r<<"\n";
   double q2=sqr(q),sq=sin(q),cq=cos(q),niu2=sqr(niu-1);
   double nc=-1/(pow(q,6)*sqr(niu2))*24*niu*
             (sqr(2*niu+1)*(sq-q*cq)*q2*q+
              1.5*niu*sqr(niu+2)*((q2-2)*cq-2*q*sq+2)*q2-
              0.5*niu*sqr(2*niu+1)*((sqr(q2)-12*q2+24)*cos(q)-
              4*(q*(q2-6)*sq+6)));
//cout<<"    "<<q<<"    "<<nc<<"     "<<q*sin(q*r)*(1/(1-nc)-1)<<"\n";
   return q*sin(q*r)*(1/(1-nc)-1);
  };
double PY(double r_,double niu_)
  {
   r=r_;niu=niu_;
   int Err;
//double InFinQunc10(X_func Func,double A,double AbsErr,double InFinZero,
//                   int NumZero,long MaxFunc,int &Flag);
   double Int,Add,Low=0,Up,Tim=2*M_PI/r; ;
   for (int k=1;k<=1000;k++) 
     if (fabs(int(k*r*(1+0.001*StndErr))-k*r)<StndErr) break;
//     if (fmod(k,r)<StndErr) break;
cout<<" k "<<k<<"\n";
   Tim*=k*r;Up=Tim;
//   Tim*=k;Up=Tim;
cout<<" Tim "<<Tim;
out_it<<" r "<<r<<" niu "<<niu<<"\n";
   long int l=0,lz=0,MaxIter=200;
   Add=RecInt(IntegFunc,sqrt(MathZer),Tim/M_PI,sqr(StndErr));
   Add+=RecInt(IntegFunc,Tim/M_PI,Tim,sqr(StndErr));
   Int=Add;
//   while ((l++<MaxIter) && (lz<200))
   while ((l++<MaxIter) && (lz<30))
     {
      Low+=Tim;
      Add=RecInt(IntegFunc,Low,Low+Tim/M_PI,StndErr*0.01*max(fabs(Int),1000*StndErr));
      Add+=RecInt(IntegFunc,Low+Tim/M_PI,Low+Tim,StndErr*0.01*max(fabs(Int),1000*StndErr));
      Int+=Add;
      if (fabs(Add)<max(fabs(Int),0.1)*StndErr) lz++;
      else lz=0;
//cout<<l<<" "<<Int<<" "<<stackavail()<<" "<<Coreleft()<<"\n";
if (!NotShowIt) out_it<<l<<" "<<Int<<"\n";
   int BreakSignal;
   while ((BreakSignal=bioskey(1))!=0) 
     if (BreakSignal==ESC) exit(0);else {cout<<" "<<BreakSignal;bioskey(0);}
     } 
//   Int=InFinQulb9(IntegFunc,StndErr,StndErr,0.01*StndErr,100,200000,Err);
//cout<<Low<<" "<<Add<<"\n";
   if (l>=MaxIter) { cout<<" Bad L\n";}
   double ret=1+1/(12*M_PI*niu*r)*Int;
if (!NotShowIt) NotShowIt=27;else NotShowIt--;

//   if (Err!=0) cout<<" Err= "<<Err<<" ";
   return ret;
  };
///*
void main()
  {
   fstream out("percus.dat",ios::out);

   double MinR=1,MaxR=6,MinNiu=0,MaxNiu=1;StndErr=1e-5;
   int NumR=100,NumNiu=100;
//   NumR=100;NumNiu=2;
NotShowIt=2;
//   MaxNiu=0.95;MaxR=11;
//   while (Cur<5)  { out<<Cur<<" "<<PY(Cur,niu)<<"\n";Cur+=Stp;cout<<" Cur "<<Cur<<"\n";}
   double StpR=(MaxR-MinR)/NumR,StpNiu=(MaxNiu-MinNiu)/NumNiu;
   double CurR=MinR+0.5*StpR,CurNiu=MinNiu+0.5*StpNiu;
   while (CurR<MaxR)
     {
      CurNiu=MinNiu+0.5*StpNiu;
//CurNiu=0.5;
      while (CurNiu<MaxNiu)
        {
         out<<CurR<<" "<<CurNiu<<" "<<PY(CurR,CurNiu)<<"\n";
         CurNiu+=StpNiu;
//cout<<"I";
        } 
      CurR+=StpR;
//cout<<" CurR "<<CurR<<" "<<Coreleft()<<"\n";
     }
   out.close();out_it.close();
  } 
*/




//*/
