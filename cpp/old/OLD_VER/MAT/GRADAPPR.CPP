#include "gradappr.h"

double *parptr,*dirptr,*tmpptr;
XAr_func hifunc;
double VecHiFunc(double x);

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
      if (fabs(stp[k])<MathZer) cout<<"in ClcGrad stp["<<k<<"]=0\n";
      else
	{
	 movmem(param,curpar,sizeof(double)*(param[0]+1));
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

















