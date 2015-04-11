#include <lib\precompiled\math.h>
#include "zero.h"    
double ZeroIn(double AX,double BX,X_func F,double Tol) 
   {
    double A,B,C,D,E,Eps,FA,FB,FC,Tol1,XM,P,Q,R,S;
   Eps=1;
   while (1+Eps/2 != 1) Eps=Eps/2;
   A=AX;B=BX;
   FA=F(A);FB=F(B);
   do
    {  
    C=A;FC=FA;
    D=B-A;
    E=D;
 Start: FC=FC;
    if (fabs(FC) < fabs(FB)) 
       {
         A=B;B=C;C=A;
         FA=FB;FB=FC;FC=FA;
       }
    Tol1=2*Eps*fabs(B)+Tol/2;
    XM=(C-B)/2;
    if ((fabs(XM)<=Tol1) || (FB==0))  return B;
    if ((fabs(E) < Tol1) || (fabs(FA) <= fabs(FB))) goto L70;
    else
      {  
        if (A==C) 
          {
            S=FB/FA;
            P=2*XM*S;
            Q=1-S;
          }       
        else
          {
            Q=FA/FC;
            R=FB/FC;
            S=FB/FA;
            P=S*(2*XM*Q*(Q-R)-(B-A)*(R-1));
            Q=(Q-1)*(R-1)*(S-1);
          }
        if (P>0) Q=-Q;
        P=fabs(P);
      }
    if (((2*P) < (3*XM*Q-fabs(Tol1)*Q)) && (P < fabs(E*Q/2)))
       {    
        E=D;
        D=P/Q;
       }
    else
      {  
  L70:  D=XM;
        E=D;
      }
    A=B;
    FA=FB;
    if (fabs(D)>Tol1)  B=B+D;
    else if (XM<0) B-=fabs(Tol1); else B+=fabs(Tol1);
    FB=F(B);
    if ((FB*FC/fabs(FC)) <= 0) goto Start;
    }
   while (1);
  };

int ZeroMullerIt(X_func Func,double &CurPnt,double Tol,int MaxIter) 
  {
//cout<<" Beg ZeroMullerIt\n";
   int CurIter=0;//,k,i;
   double f,r,x0,x1,x2,p0,p1,p2,d,d1,u,t,h;
   p0=CurPnt-Tol;
   p1=CurPnt;
   p2=CurPnt+Tol;
   h=p2-p1;
   r=p0;
Start:
    CurIter++;
    f=Func(r);
//cout<<" Zero Iteration r= "<<r<<" f= "<<f<<" curiter "<<CurIter<<"\n";
    if ((CurIter>=MaxIter) || (fabs(f)<Tol))  
      {
       CurPnt=r;
       return !(CurIter>=MaxIter);
      }
    else
      {
       switch (CurIter)
         {
          case 1:
               r=p1;
               x0=f;
               goto Start;
          case 2:
               r=p2;
               x1=f;
               goto Start;
          case 3: 
               x2=f;
               d=(p2-p1)/(p1-p0);
               break;
          default:
                 x0=x1;
                 x1=x2;
                 x2=f;
                 d=t;
         }         
       d1=d+1.0;
       t=x0*sqr(d)-x1*sqr(d1)+x2*(d+d1);
       u=sqr(t)-4.0*x2*d*d1*(x0*d-x1*d1+x2);
       if (u>0) u=sqrt(u); else u=0;
       u=u+fabs(t);
       if (t<0) u=-u;
       if (u==0) u=1.0;
       t=-2.0*x2*d1/u;
       h=t*h;
       r=r+h;
       goto Start;
      }
  };   

int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter) 
  {
   int CurIter=0;
   double f,f1,f2,x,x1,x2;
   x=CurPnt-Tol;
   x1=CurPnt+Tol;
   f=Func(x);
   f1=Func(x1);
   while ((CurIter++<MaxIter) && (fabs(f)>Tol))  
     {
      x2=x1;x1=x;f2=f1;f1=f;
      x=x1-f1/(f2-f1)*(x2-x1);
      f=Func(x);
//cout<<" Zero Iteration r= "<<x<<" f= "<<f<<" curiter "<<CurIter<<"\n";
     }
   CurPnt=x;
//cout<<" Zero Iteration r= "<<x<<" f= "<<f<<" curiter "<<CurIter<<"\n";
   return !(CurIter>=MaxIter);
  };   
   
int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
                             double Up,double Down,double MaxStp,double BegStp) 
  {
   int CurIter=0,IntIter;
   double f,f1,f2,x,x1,x2,MinF;
   MaxStp=fabs(MaxStp);

   x=CurPnt-BegStp*MaxStp;
   x1=CurPnt+BegStp*MaxStp;
//cout<<"ZPnt "<<x<<"  "<<x1<<"\n";
   f=Func(x);
   f1=Func(x1);
   CurPnt=x,MinF=fabs(f);
//cout<<" F "<<f<<" F1 "<<f1<<"\n";
   while ((CurIter++<MaxIter) && (fabs(f)>Tol) && (MaxStp>Tol))  
     {
      if (fabs(f)<MinF) {CurPnt=x;MinF=fabs(f);}
      if (fabs(f1)<MinF) {CurPnt=x1;MinF=fabs(f1);}
      x2=x1;x1=x;f2=f1;f1=f;
      IntIter=0;
      while ( (fabs(f2-f1)<100*MathZer) && (IntIter++<100) ) {x2+=MaxStp;f2=Func(x2);}
      if (IntIter>=100) { cout<<" \nFunctions are constant.\n MaxStp "
         <<MaxStp<<" Func "<<f2<<" X1 "<<x1<<" X2 "<<x2<<"\n";CurIter=MaxIter;break;}
      x=x1-f1/(f2-f1)*(x2-x1);
      if (x-x1>MaxStp) x=x1+MaxStp;
      if (x1-x>MaxStp) x=x1-MaxStp;
      if (x>=Up) {x=Up-10*Tol;MaxStp*=0.5;}
      if (x<=Down) {x=Down+10*Tol;MaxStp*=0.5;}
      f=Func(x);
      IntIter=0;
      while ((fabs(f)>fabs(f1)) && (IntIter++<10)) { x=x1+(x-x1)/2;f=Func(x);}
	  if (IntIter!=0) MaxStp=2*fabs(x-x1);
      if (IntIter>=10) 
        { cout<<" \nBad Functions evaluation.Exit.\n "; CurIter=MaxIter;break;}
//cout<<" Zero Iteration r= "<<x<<" f= "<<f<<" curiter "<<CurIter<<"\n";
     }
   if (fabs(f)<MinF) {CurPnt=x;MinF=fabs(f);}
   if (fabs(f1)<MinF) {CurPnt=x1;MinF=fabs(f1);}
//cout<<" Zero Iteration r= "<<CurPnt<<" f= "<<MinF<<" curiter "<<CurIter<<"\n";
   return !((CurIter>=MaxIter) || (fabs(MaxStp)<=Tol));
  };   

