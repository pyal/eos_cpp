#include <lib/precompiled/math.h>
#include "mtrung.h"

double *parptr, *dirptr, *tmpptr;
XAr_func hifunc;

double VecHiFunc(double x);
// Rung - Curt method with control of the Error on the step
// for the function:       d(y)=d(x)*f(x,y)
int goodstp(double r, double &hc, double err) {
    r = fabs(r);
    if(r < err / 30)
        r = err / 30;
    //cout<<" r "<<r<<" err "<<err<<"\n";
    if(r > err)
        hc = err / r;
    else
        hc = log(err / r);
    //cout<<" r "<<r<<" err "<<err<<" hc "<<hc<<"\n";
    if(r > 3 * err)
        return 0;
    return 1;
};
int stop(
    double x,
    double y,
    int iter,
    int MaxIter,
    double x_beg,
    double x_end,
    BrkXY_func Brk) {
    int ret = 0;
    if(iter++ > MaxIter)
        return 1;
    if(!In_Lim(x, x_beg, x_end, 1))
        return -1;
    if(Brk != NULL)
        ret = Brk(x, y);
    return ret;
};
void SetStp(double &err, double &stp, double x_beg, double x_end) {
    if(fabs(stp) > fabs(x_end - x_beg) / 10.)
        stp = fabs(x_end - x_beg) / 10.;
    if(fabs(stp) < MathZer)
        stp = StndErr;
    if(!In_Lim(x_beg + stp, x_beg, x_end))
        stp = -stp;
    if(err <= 0)
        err = StndErr;
};
int NotRungCurt(
    XYZ_func func,
    double x_beg,
    double y_beg,
    double x_end,
    double &x,
    double &y,
    double &stp,
    double err,
    int MaxIter,
    BrkXY_func Brk) {
    SetStp(err, stp, x_beg, x_end);
    x = x_beg;
    y = y_beg;
    double &h = stp, k1, k2, k3, k4, k5, k6, r, hc;
    int iter = 0, ret;
    while(!(ret = stop(x, y, iter, MaxIter, x_beg, x_end, Brk))) {
        k1 = h * (*func)(x, y);
        k2 = h * (*func)(x + h / 2, y + k1 / 2);
        k3 = h * (*func)(x + h / 2, y + (k1 + k2) / 4);
        k4 = h * (*func)(x + h, y - k2 + 2 * k3);
        k5 = h * (*func)(x + 2 * h / 3, y + (7 * k1 + 10 * k2 + k4) / 27);
        k6 = h * (*func)(
                     x + h / 5,
                     y + (28 * k1 - 125 * k2 + 546 * k3 + 54 * k4 - 378 * k5) / 625);

        r = (42 * k1 + 224 * k3 + 21 * k4 - 162 * k5 - 125 * k6) / 336;
        //cout<<" x "<<x<<" y "<<y<<" h "<<h<<" r "<<r<<"\n";
        if(goodstp(r, hc, err)) {
            y += (k1 + 4 * k3 + k4) / 6 + r;
            x += h;
        }
        h *= hc;
        if((x + h - x_end) * (x - x_end) < 0)
            h = x_end - x;
    }
    h = x_end - x;
    k1 = h * (*func)(x, y);
    k2 = h * (*func)(x + h / 2, y + k1 / 2);
    k3 = h * (*func)(x + h / 2, y + (k1 + k2) / 4);
    k4 = h * (*func)(x + h, y - k2 + 2 * k3);
    k5 = h * (*func)(x + 2 * h / 3, y + (7 * k1 + 10 * k2 + k4) / 27);
    k6 = h *
         (*func)(
             x + h / 5, y + (28 * k1 - 125 * k2 + 546 * k3 + 54 * k4 - 378 * k5) / 625);
    r = (42 * k1 + 224 * k3 + 21 * k4 - 162 * k5 - 125 * k6) / 336;
    y += (k1 + 4 * k3 + k4) / 6 + r;
    x += h;

    //cout<<" x_end "<<x_end<<" x "<<x<<"\n";
    if(ret == -1)
        ret = 0;
    return ret;
};
/*
//=====================================================================
//===============         MinFunc           ===========================
//=====================================================================
   
//=====================================================================
//================       Simple functions   ===========================
//=====================================================================

double MinFunc(double x,double y,X_func Func,double Tol,int MaxIter)
  {
   if (MinStrc==NULL) MinStrc=new ClcMinStrc;
   return MinStrc->MinGoldSqr(x,y,Func,Tol,MaxIter);
  };
double MinGold(double x,double y,X_func Func,double Tol)
  {
   if (MinStrc==NULL) MinStrc=new ClcMinStrc;
   return MinStrc->MinGold(x,y,Func,Tol);
  };
double MinSqr(double x,double y,X_func Func,double Tol)
  {
   if (MinStrc==NULL) MinStrc=new ClcMinStrc;
   return MinStrc->MinSqr(x,y,Func,Tol);
  };
void DelMinFunc() { if (MinStrc!=NULL) delete MinStrc;};
//=====================================================================
//================       Object module      ===========================
//=====================================================================

   ClcMinStrc::ClcMinStrc()
     {clcx=new double[3];Fclcx=new double[3];clcx[0]=2;Fclcx[0]=2;
      MachZer=1;while ( (MachZer+1)!=1 ) MachZer=MachZer/2;
      MachZer=sqrt(MachZer);
      GoldCoef=0.5*(3-sqrt(5));
     };

   double ClcMinStrc::CmpDelMin(double mainx,double newx,double Zer)
     {
      if (fabs(mainx-newx)>Zer) return newx;
      if (mainx>newx) return mainx-Zer;
      return mainx+Zer;
     };
   double ClcMinStrc::MinSqrAppr(double x1,double x2,double x3,
		     double y1,double y2,double y3)
     {
      double a,b,temp;
      temp=(y2-y1)/(x2-x1);
      a=((y3-y1)/(x3-x1)-temp)/(x3-x2);
      b=temp-(x2+x1)*a;
      if (fabs(a)<MachZer) return MathInf;
      return (-b/(2*a));
     };
   void ClcMinStrc::SetBnd(double x1,double x2,double x3,
		       double y1,double y2,double y3)
     { lft=x1;clcx[1]=x2;rgt=x3; Flft=y1;Fclcx[1]=y2;Frgt=y3;  };
   void ClcMinStrc::NextBound()
     {
      if (clcx[1]>clcx[2])
        {ExchVar(&clcx[1],&clcx[2],sizeof(clcx[1]));
         ExchVar(&Fclcx[1],&Fclcx[2],sizeof(Fclcx[1]));}
      double minBl=min(Flft,Fclcx[1]);
      double minBr=min(Frgt,Fclcx[2]);
      if (minBl<minBr)
        {SetBnd(lft,clcx[1],clcx[2],Flft,Fclcx[1],Fclcx[2]);MinCurr=minBl;}
      else {SetBnd(clcx[1],clcx[2],rgt,Fclcx[1],Fclcx[2],Frgt);MinCurr=minBr;}
       
     };

   void ClcMinStrc::GoldStp()
     {
//cout<<" GoldStp\n";
      double delt=(rgt-lft);
      if (clcx[1]<lft+delt/2) clcx[2]=rgt-delt*GoldCoef;
      else                    clcx[2]=lft+delt*GoldCoef;
      clcx[2]=CmpDelMin(clcx[1],clcx[2],ZerStp);
      Fclcx[2]=func(clcx[2]);
      NextBound();
     };
   void ClcMinStrc::SqrStp()
     {
      clcx[2]=MinSqrAppr(lft,clcx[1],rgt,Flft,Fclcx[1],Frgt);
//cout<<" SgrStp Lft "<<lft<<" clcx[1] "<<clcx[1]<<" clcx[2] "<<clcx[2]
//    <<" rgt "<<rgt<<"\n";
      if ( (!In_Lim(clcx[2],lft,rgt)) ||
           ( fabs(clcx[1]-clcx[2]) <ZerStp ) ||
           ( fabs(lft-clcx[2]) <ZerStp ) ||
           ( fabs(rgt-clcx[2]) <ZerStp ))
		                                   GoldStp();
      else
	{
	 Fclcx[2]=func(clcx[2]);
	 NextBound();
	}
     };
   void ClcMinStrc::SetBeg(double A,double B,X_func yfunc,double Tol)
     {
      func=yfunc;
      lft=A;rgt=B;Flft=func(lft);Frgt=func(rgt);
      ClcZer=Tol;
      clcx[1]=lft+(rgt-lft)*GoldCoef;Fclcx[1]=func(clcx[1]);
      MinCurr=MathInf;
     };

   double ClcMinStrc::MinGoldSqr(double A,double B,X_func yfunc,double Tol,int MaxIter)
     {
//      double minold=MathInf;
      SetBeg(A,B,yfunc,Tol);
//	  int MaxIter=30;
      while ( (rgt-lft)>2*ClcZer )
	{
	 if (!(--MaxIter)) break;
//	 minold=MinCurr;
	 ZerStp=MachZer*fabs(clcx[1])+Tol/3;
	 if ( ((clcx[1]-lft)>ZerStp) && ((rgt-clcx[1])>ZerStp) ) SqrStp();
	 else GoldStp();
	};
	  if (!MaxIter) 
	  {fstream out("error.log",ios::ate);
		  out<<" bad ClcMinStrc::MinGoldSqr MaxIter; Tol"<<Tol
			  <<" rgt "<<rgt<<" lft "<<lft<<"\n";out.close();}
      return clcx[1];
     };
   double ClcMinStrc::MinNumGoldSqr(double A,double B,X_func yfunc,double Tol)
     {
//      double minold=MathInf;
      int NumIter=Tol;Tol=(B-A)*1e-6;
      SetBeg(A,B,yfunc,Tol);
      for (int k=1;k<=NumIter;k++)
	{
//	 minold=MinCurr;
	 ZerStp=MachZer*fabs(clcx[1])+Tol/3;
	 if ( ((clcx[1]-lft)>ZerStp) && ((rgt-clcx[1])>ZerStp) ) SqrStp();
	 else GoldStp();
	};
      return clcx[1];
     };
   double ClcMinStrc::MinGold(double A,double B,X_func yfunc,double Tol)
     {
      SetBeg(A,B,yfunc,Tol);
      while ( (rgt-lft)>2*ClcZer )
	{ ZerStp=MachZer*fabs(clcx[1])+Tol/3;SqrStp();};
      return clcx[1];
     };
   double ClcMinStrc::MinSqr(double A,double B,X_func yfunc,double Tol)
     {
      SetBeg(A,B,yfunc,Tol);
      while ( (rgt-lft)>2*ClcZer )
	{ ZerStp=MachZer*fabs(clcx[1])+Tol/3;SqrStp();  };
      return clcx[1];
     };


//=====================================================================
//===============           GradAppr        ===========================
//=====================================================================

double VecHiFunc(double x)
  {
    for (int k=1;k<=parptr[0];k++) tmpptr[k]=parptr[k]+x*dirptr[k];
    return hifunc(tmpptr);
  };
double GradAppr::VecMulVec(double *vec1,double *vec2)
  { double ret=0;for (int k=1;k<=vec1[0];k++) ret+=vec1[k]*vec2[k];
    return ret;};
double GradAppr::VecMulNum(double *vec1,double num)
  { double ret=0;for (int k=1;k<=vec1[0];k++) ret+=vec1[k]*num;
    return ret;};
//void GradAppr::PrepVecHiFunc(double *param,double *dir)
//  {  parptr=param;dirptr=dir;};


double GradAppr::GetMaxStp(double *stp,double *dir)
  {
   double ret=MathInf,ret1;
   for (int k=1;k<=dir[0];k++)
     {
      if ( (fabs(dir[k])>MathZer) && ((ret1=fabs(stp[k]/dir[k]))<ret) )
	 ret=ret1;
     }
   return ret;
  };
void GradAppr::SetMaxStp(double *stp,double maxstp,double curstp)
  {
   double coe=1.5*curstp/maxstp;
   for (int k=1;k<=stp[0];k++) stp[k]=stp[k]*coe;
  };

void GradAppr::ClcMin(double *param,double *dir,double *stp,
						  double &er,double &hi)
  {
   double MaxStp=GetMaxStp(stp,dir);
//   PrepVecHiFunc(param,dir);
//   double MinStp=minfunc(-MaxStp,0,&(::VecHiFunc),er);
   double MinStp=minfunc(0,MaxStp,&(::VecHiFunc),MaxStp/50);
   double hi1=VecHiFunc(MinStp);
   er=hi-hi1;hi=hi1;
//   SetNewStp(stp,MinStp,MaxStp);
   for (int k=1;k<=param[0];k++) param[k]+=dir[k]*MinStp;
   SetMaxStp(stp,MaxStp,MinStp);
  };

int GradAppr::ClcDir(double *grad,double *dir,double &NormGrad)
  {
   if (fabs(NormGrad)<MathZer) return 0;
   double NewNorm=VecMulVec(grad,grad);
   double Coef=(NewNorm/NormGrad);
   for (int k=1;k<=dir[0];k++) dir[k]=grad[k]+Coef*dir[k];
   NormGrad=NewNorm;
   return 1;
  }
void GradAppr::ClcGrad(double *param,double *stp,double *grad,double hi)
  {
   double *curpar=new double[param[0]+1];
   for (int k=1;k<=param[0];k++)
     {
      if (fabs(stp[k])<MathZer) grad[k]=0;// cout<<"in ClcGrad stp["<<k<<"]=0\n";
      else
	{
	 MemoryMove(param,curpar,sizeof(double)*(param[0]+1));
	 curpar[k]+=stp[k]/100+MathZer;
	 grad[k]=(hi-hifunc(curpar))*(100/(stp[k]+100*MathZer));
	}
     }
   delete curpar;
  };
int GradAppr::Clc(double *param,double *stp,double hi)
  {
   ContStp(stp);
   int num=0,GoodDir=1;
  double er=MathInf,NormDir=1;
   double *grad=new double[param[0]+1];grad[0]=param[0];
   double *dir=new double[param[0]+1];dir[0]=param[0];
   for (int k=1;k<=dir[0];k++) dir[k]=0;
   if (hi<0) hi=hifunc(param);

   tmpptr=new double[param[0]+1];tmpptr[0]=param[0];
   parptr=param;dirptr=dir;

   while ( (er>StopErr) && (num++<NumIter) && (GoodDir) &&
	  (!( (brkfunc!=NULL) && (brkfunc(hi,er)) )) )
     {
      ClcGrad(param,stp,grad,hi);
      if ( (GoodDir=ClcDir(grad,dir,NormDir)) )
						ClcMin(param,dir,stp,er,hi);
     }
   delete grad;delete dir;delete tmpptr;
   return 1;
  };
















*/
