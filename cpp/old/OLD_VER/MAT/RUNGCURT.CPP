#include "rungcurt.h"

// Rung - Curt method with control of the Error on the step
// for the function:       d(y)=d(x)*f(x,y)
  int goodstp(double r,double &hc,double err)
    {
     if (fabs(r)<MathZer) r=err/30; 
     hc=log(fabs(err/r));
     if (r>3*err)  return 0;
     return 1;
    };
  int stop(double x,double y,int iter,int MaxIter,double x_beg,
                                              double x_end,BrkXY_func Brk)
    {
     int ret=0; 
     if (iter++>MaxIter) return 1;
     if ( !In_Lim(x,x_beg,x_end) ) return -1;
     if (Brk!=NULL) ret=Brk(x,y);
     return ret;
    };
  void SetStp(double &stp,double &err,double x_beg,double x_end)
    {
     if (stp<=0) stp=(x_end-x_beg)/10; 
     if (err<=0) err=StndErr;
    };
int NotRungCurt(XYZ_func func,double x_beg,double y_beg,double x_end,
                            double &x,double &y,
                            double &stp,double err,int MaxIter,
                                                 BrkXY_func Brk)
  {
   SetStp(err,stp,x_beg,x_end);
   x=x_beg;y=y_beg;
   double &h=stp,k1,k2,k3,k4,k5,k6,r,hc;
   int iter=0,ret;
   while ( !(ret=stop(x,y,iter,MaxIter,x_beg,x_end,Brk)) )
     {
      k1=h*(*func)(x,y);k2=h*(*func)(x+h/2,y+k1/2);
      k3=h*(*func)(x+h/2,y+(k1+k2)/4);
      k4=h*(*func)(x+h,y-k2+2*k3);
      k5=h*(*func)(x+2*h/3,y+(7*k1+10*k2+k4)/27);
      k6=h*(*func)(x+h/5,y+(28*k1-125*k2+546*k3+54*k4-378*k5)/625);

      r=(42*k1+224*k3+21*k4-162*k5-125*k6)/336;
      if ( goodstp(r,hc,err) ) { y+=(k1+4*k3+k4)/6+r;x+=h; }
      h*=hc;
     }
   if (ret==-1) ret=0;
   return ret;
  };

