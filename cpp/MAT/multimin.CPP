#include <lib\precompiled\math.h>
#include "multimin.h"

extern int NumGradCalls;
void SetVec(VecCl &vec,double *pnt,int SetVec)
 {if (SetVec) MemoryMove(pnt,&vec.Ptr[1],sizeof(double)*vec.Dim());
else MemoryMove(&vec.Ptr[1],pnt,sizeof(double)*vec.Dim());}
/*
double (*StaticUncMinFuncClcMySubst)(double *x,int n);
double StaticUncMinFuncClcMy(double *par)
 {
  double func=StaticUncMinFuncClcMySubst(&par[1],par[0]);
//NumGradCalls++;
//cout<<" NumGradCalls "<<NumGradCalls<<" x1 "<<par[1]<<" x2 "<<par[2]<<" func_est "<<func<<"\n"<<flush;

  return func;
 }

double UncMin(int &Error,int N,double *x_start,double *x_res,double est_sol,
              double (*FuncClc)(double *x,int n) , double *GradStp,
              void (*GradClc)(double *x,double *grad,int n,double &func),
              double FunAccur,double ResErr,int MaxFun,int MaxIter,
              double *BoundMin,double *BoundMax,
              double MaxLineSearchStp,
              double LineSearchEta)
 {
//int MAPClc(double *Param,double *MaxStep,int Iter,
//    XAr_func HiF,double ErrMisfit,int MinHi,
//    GR_XAr_func GrF=NULL,double *Exper=NULL,double *HiPnt=NULL);
  VecCl Param(N),MaxStep(N),LowBnd(N),UpBnd(N);
  SetVec(Param,x_start,1);
  if ((BoundMin!=NULL)&&(BoundMax!=NULL))
   {SetVec(LowBnd,BoundMin,1);SetVec(UpBnd,BoundMax,1);
    MaxStep=(UpBnd-LowBnd)*0.3;}
  else MaxStep=MaxStep*0+10.;
//  double (*StaticUncMinFuncClcMySubst)(double *x,int n)  =FuncClc;
  StaticUncMinFuncClcMySubst  =FuncClc;

  double ret=1e-6;
//  MaxFun=3;
  for (int k=0;k<MaxIter;k++)
   {
//cout<<"\n Start MAPClc\n";NumGradCalls=0;
    MAPClc(Param.Ptr,MaxStep.Ptr,MaxFun,StaticUncMinFuncClcMy,ret,0);
   }
  SetVec(Param,x_start,0);
//  if ((BoundMin!=NULL)&&(BoundMax!=NULL))
//   {
//    for (k=1;k<=Param[0];k++) 
//     Param[k]=min(UpBnd[k],max(LowBnd[k],Param[k]));
//   }
  ret=StaticUncMinFuncClcMy(Param.Ptr);
  ResErr=0;
  return ret;
 }
*/


















//*
struct MultiMinParam
 {
  VecCl x_cur,SearchDir,MaxStep,GradStp;
//  double MaxInDir,MinInDir;
  double y_cur;
  int N,y_setted;
  int SearchMeth;
  VecCl BndMin,BndMax;
  double (*FuncClc)(double *x,int n);
  void (*SecDerivClc)(VecCl &x,VecCl &deriv,MatrCl &deriv2);
  static double DirClc(double x);
  int SetParam(int n,double *x,double (*Func)(double *x,int n),
               void (*SecDerivClc)(VecCl &x,VecCl &deriv,MatrCl &deriv2),
               double *grad_stp,
               double *Bmin,double *Bmax,double *max_stp,int Meth);
 };

MultiMinParam *Static_Clc_Param;
#define CP Static_Clc_Param 

int CheckStp(VecCl &x0,VecCl &stp,int InvertDir,VecCl &BndMin,VecCl &BndMax)
 { 
  int N=x0.Dim(),k; 
  double lo,up;
  VecCl x1;x1=x0+stp;
  for (k=1;k<=N;k++) 
   {
    lo=BndMin[k];up=BndMax[k];
    if (lo-up<sqrt(MathZer)) continue;
    if (!In_Lim(x0[k],lo,up,1)) return 0;
    if (In_Lim(x1[k],lo,up)) continue;
    if (InvertDir)
     {
     if (!In_Lim(x0[k]-stp[k],lo,up)) {stp[k]=-stp[k];continue;}
     stp[k]=(up-x0[k]>x0[k]-lo)?up-x0[k]:lo-x0[k];
     }
    else stp[k]=(stp[k]>0)?up-x0[k]:lo-x0[k];
   }
  return 1;
 };
void ResetVectToBound(VecCl &x,VecCl &BndMin,VecCl &BndMax)
 {
  int N=x.Dim(),k; 
  double lo,up;
  for (k=1;k<=N;k++) 
   { lo=BndMin[k];up=BndMax[k];if (x[k]<lo) x[k]=lo;if (x[k]>up) x[k]=up;}
 }

int SetBounds(VecCl &x,VecCl &stp,VecCl &BndMin,VecCl &BndMax,double &from,double &to_)
 {
  int N=x.Dim(),k; 
  double lo,up,minfr=1e150,minto=1e150,t;
  for (k=1;k<=N;k++) 
   {
    lo=BndMin[k];up=BndMax[k];
    if (!In_Lim(x[k],lo,up,1)) 
     {
cout<<" Error not in lim x "<<x[k]<<" low, up "<<lo<<" "<<up<<"\n";
x[k]=(x[k]<lo)?lo:up;
cout<<" x[k] setted "<<x[k]<<"\n";
//     return 0;
     }
    if (fabs(stp[k])<sqrt(MathZer)) continue;
    if (stp[k]>0)
     {if ((t=(x[k]-lo)/stp[k])<minfr) minfr=t;
      if ((t=(up-x[k])/stp[k])<minto) minto=t;}
    else
     {if ((t=(x[k]-lo)/-stp[k])<minto) minto=t;
      if ((t=(up-x[k])/-stp[k])<minfr) minfr=t;}
   }
  from=-minfr;to_=minto;return 1;
 }
int CheckStp(VecCl &x0,VecCl &stp,MatrCl &DirMat,VecCl &BndMin,VecCl &BndMax,int k)
 {
  int Dim=x0.Dim();//,k;
  double lo,up;
  VecCl Del(Dim);
//  for (k=1;k<=Dim;k++)
   {
    Del=MatrCl::GetCol(DirMat,k);
    if (!SetBounds(x0,Del,BndMin,BndMax,lo,up)) return 0;
    if (!In_Lim(stp[k],lo,up)) stp[k]=(stp[k]<=lo)?lo:up;
   }
  return 1;
 }

void DerivMatrClc(VecCl &x,VecCl &deriv,MatrCl &deriv2)//,VecCl &GradStp,double &CurVal)//VecCl &MisV,double &CurHi,VecCl &Par,VecCl &MaxStep)
  {
   int Dim=CP->N,k,k1;
   VecCl Stp,Fun(Dim);Stp=CP->GradStp*2;CheckStp(x,Stp,1,CP->BndMin,CP->BndMax);Stp=Stp*0.5;
   if (VecCl::ModMax(CP->x_cur-x)>sqrt(MathZer)) {CP->x_cur=x;CP->y_setted=0;}
//   if (!CP->y_setted)
    {CP->y_setted=1;CP->y_cur=CP->FuncClc(&x.Ptr[1],Dim);}
   MatrCl ResM(Dim);
   VecCl x_plus;
   double CurHi=CP->y_cur,CurVal=CurHi;
   for (k=1;k<=Dim;k++) {x_plus=x;x_plus[k]+=Stp[k];Fun[k]=CP->FuncClc(&x_plus.Ptr[1],Dim);}
   for (k=1;k<=Dim;k++)
     {
      for (k1=k;k1<=Dim;k1++)
        {
         x_plus=x;x_plus[k]+=Stp[k];x_plus[k1]+=Stp[k1];
         ResM(k,k1)=(CP->FuncClc(&x_plus.Ptr[1],Dim)+CurHi-Fun[k]-Fun[k1])/(Stp[k]*Stp[k1]);
         ResM(k1,k)=ResM(k,k1);
        }
      deriv[k]=(Fun[k]-CurHi)/Stp[k];
     };
   deriv2=ResM;
//cout<<"\n Correlation \n"<<DatM<<"\n Step "<<MisV<<"\n";//<<" Deriv "<<Fun<<" MaxStep "<<MaxStep<<"\n";
  };
void NormDeriv(MatrCl &deriv2,VecCl &deriv)
 {
   int Dim=deriv.Dim(),k,k1;
   deriv=deriv*0.5;
   deriv2=deriv2*0.5;
   for (k=1;k<=Dim;k++) 
    {
     if (deriv2(k,k)<0) 
      {deriv[k]=deriv[k]*0.5;
       for (k1=1;k1<=Dim;k1++) if (k!=k1) deriv2(k1,k)=deriv2(k,k1)=0;deriv2(k,k)=fabs(deriv2(k,k));
      }
    }
 }

double MultiMinParam::DirClc(double p)
 {
  VecCl x;x=Static_Clc_Param->x_cur+Static_Clc_Param->SearchDir*p;
  double ret=Static_Clc_Param->FuncClc(&x.Ptr[1],x.Dim());
NumGradCalls++;
cout<<" NumGradCalls "<<NumGradCalls<<" x1 "<<x[1]<<" x2 "<<x[2]<<" func_est "<<ret<<"\n"<<flush;
cout<<" "<<ret<<" "<<flush;
  return ret;
 }


int MultiMinParam::SetParam(int n,double *x,double (*Func)(double *x,int n),
                            void (*SecDerivClc_)(VecCl &x,VecCl &deriv,MatrCl &deriv2),
                            double *grad_stp,
                            double *Bmin,double *Bmax,double *max_stp,int Meth)
 {
  N=n;
  x_cur.SetDim(N);BndMin.SetDim(N);BndMax.SetDim(N);MaxStep.SetDim(N);GradStp.SetDim(N);
  SetVec(x_cur,x,1);
  if ((Bmin!=NULL)&&(Bmax!=NULL)) {SetVec(BndMin,Bmin,1);SetVec(BndMax,Bmax,1);}
  else {BndMin=BndMin*0;BndMax=BndMin+(-1);}
  if (max_stp!=NULL) SetVec(MaxStep,max_stp,1);
  else MaxStep=MaxStep*0+(-1);
  if (grad_stp!=NULL) SetVec(GradStp,grad_stp,1);
  else 
   {for (int k=1;k<=N;k++) if (MaxStep[k]>sqrt(MathZer)) GradStp[k]=MaxStep[k]*0.01;else GradStp[k]=StndErr;}
  FuncClc=Func;SearchMeth=Meth;
  SecDerivClc=SecDerivClc_;
  ResetVectToBound(x_cur,BndMin,BndMax);
//cout<<" Checking "<<x_cur<<" "<<BndMin<<" "<<BndMax<<"\n";
  return CheckStp(x_cur,GradStp,1,BndMin,BndMax);
 }


int GetDir(MatrCl &DirMat,double ErrorMatr,VecCl &EstStp,VecCl &Par) 
  {
   
   int Dim=Static_Clc_Param->N,DimGr=1;EstStp.SetDim(Dim);
   if ((Dim>0) && (DimGr>0))
     {
      int k;//,k1;
      double d;
      MatrCl DatM(Dim);
      VecCl  MisV(Dim),Norm(Dim),EigV(Dim);
// MisV - Derivativ of the function
      CP->SecDerivClc(Par,MisV,DatM);//,CurHi,,MaxStep);
//cout<<" X "<<Par<<"\nDeriv "<<MisV<<"\n2Deriv "<<DatM<<"\n";
      NormDeriv(DatM,MisV);
//cout<<" X "<<Par<<"\nDeriv "<<MisV<<"\n2Deriv\n "<<DatM<<"\n"<<flush;
      for (k=1;k<=Dim;k++) if ((d=DatM(k,k))>MathZer) Norm[k]=1/sqrt(d);else Norm[k]=0;
      DatM=VectorNormalizeMatr(Norm,DatM,Norm);
      if (!ReduceLQ(DatM,DatM,EigV.Ptr,ErrorMatr)) 
        {cout<<" Cannot Calculate ReduceLQ. \n";return 0;}
      DatM=Transpon(DatM);      
      for (k=1;k<=Dim;k++) 
        if ( (fabs(d=EigV[k]))>MathZer) EigV[k]=1/d;else EigV[k]=0;
      EstStp=EstStp*0-VectorNormalizeMatr(EigV,DatM,Norm)*MisV;
//cout<<" EstStp "<<EstStp<<"\n"<<flush;
      DirMat=VectorNormalizeMatr(Norm,Transpon(DatM),Norm*0+1);
//cout<<" Dir\n "<<DirMat<<"\n";
   }
  return 1;
 };

#include "quad.h"
int Iter(MatrCl &DirMat,double XMisf,VecCl &EstStp)
  {
//cout<<" StartIter\n"<<flush;
//cout<<" EstStp "<<EstStp<<"\n"<<flush;
//cout<<" Iter min "<<CP->BndMin<<"\nmax "<<CP->BndMax<<"\n"<<flush;
   int Dim=DirMat.Dim(),k,i,k1;//,u
   double d;
   VecCl Step(Dim),Tmp,x_last,NewStp;Tmp=EstStp;x_last=CP->x_cur;NewStp=EstStp;
   for (int k0=1;k0<=Dim;k0++)
     {
      d=0;i=k0;
      for (k1=1;k1<=Dim;k1++) {if (fabs(Tmp[k1])>d) {i=k1;d=fabs(Tmp[k1]);}}
      k=i;
       if (!CheckStp(CP->x_cur,EstStp,DirMat,CP->BndMin,CP->BndMax,k)) return 0;
       if (fabs(EstStp[k]*VecCl::ModMax(MatrCl::GetCol(DirMat,k)))>2*sqrt(MathZer))
        {
         Step=MatrCl::GetCol(DirMat,k)*EstStp[k];
//cout<<" x "<<CP->x_cur<<" Step "<<EstStp[k]<<" Dir "<<Step<<" x_after step "<<CP->x_cur+Step<<flush;
         CP->SearchDir=Step;
         double up,low,stp=1;
         if (!SetBounds(CP->x_cur,Step,CP->BndMin,CP->BndMax,low,up)) return 0;;
//cout<<"found lims lo,up "<<low<<" "<<up<<"\n clc_lim are up "<<CP->x_cur+Step*low<<"        and low "<<CP->x_cur+Step*up<<"\n"<<flush;
         stp=Fmin(CP->DirClc,low,up,stp,XMisf,CP->y_cur);
         CP->x_cur=CP->x_cur+Step*stp;
         NewStp[k]=stp;
         ResetVectToBound(CP->x_cur,CP->BndMin,CP->BndMax);
        }
      Tmp[k]=0;
     }
//   CP->y_cur=y_cur;
   Tmp=(CP->x_cur-x_last);
   d=0;for (k=1;k<=Dim;k++) 
        {d+=fabs(Tmp[k]);if (fabs(Tmp[k])<0.5*fabs(EstStp[k])) EstStp[k]*=0.5;
                         else if (fabs(Tmp[k])>2*fabs(EstStp[k])) EstStp[k]*=2;}
   EstStp=NewStp;
   if (d/Dim<XMisf) return 0;
   return 1;
  };

//  MatrCl GradMatr;
double MultiMin(int &Error,int N,double *x,double (*FuncClc)(double *x,int n) ,
                void (*SecDerivClc)(VecCl &x,VecCl &deriv,MatrCl &deriv2),
                double XMisf,int MaxIterStep,int MaxIter,double &ResErr,
                double *GradStp,double *BoundMin,double *BoundMax,
                double *MaxLineSearchStp,
                int LineSearchMeth)
 {
//cout<<"\n New MultiMinClc\n\n\n";
 NumGradCalls=0;
   MultiMinParam Par;Static_Clc_Param=&Par;
   MatrCl DirMat(N);
   VecCl EstStp;
   if (SecDerivClc==NULL) SecDerivClc=DerivMatrClc;
   int cont=Par.SetParam(N,x,FuncClc,SecDerivClc,GradStp,BoundMin,BoundMax,MaxLineSearchStp,LineSearchMeth);
   int cur_it=0,cur_it_step=0;
   while ((cont) && (cur_it++<MaxIter))
    {
cout<<"\n Start MultiMin Iter\n";//NumGradCalls=0;
     ResetVectToBound(CP->x_cur,CP->BndMin,CP->BndMax);
     cont=GetDir(DirMat,1e-6,EstStp,CP->x_cur);
     cur_it_step=0;
     while ((cont) && (cur_it_step++<MaxIterStep))cont=Iter(DirMat,XMisf,EstStp);
    }
cout<<"\n";
   Error=0;SetVec(CP->x_cur,x,0);
   return CP->y_cur;
 };


 /*

void IterSimple(VecCl &Par,VecCl &MaxStep,MatrCl &DirMat,double &CurHi)
  {
   
   int Dim=DirMat.Dim(),k,i,k1,u;
   double Pos[4]={0,0.5,1,0};
   double Hi[4],d;Hi[0]=CurHi;
//double x=MAPHiFunc(Par);cout<<" External "<<x<<" Internal "<<CurHi<<" Dim "<<Dim<<"\n";
   VecCl Step(Dim),Tmp(Dim);Tmp=MaxStep;
   for (int k0=1;k0<=Dim;k0++)
     {
      d=0;i=k0;
      for (k1=1;k1<=Dim;k1++) {if (fabs(Tmp[k1])>d) {i=k1;d=fabs(Tmp[k1]);}}
      k=i;
      if (fabs(MaxStep[k])>MathZer)
        {
         Step=GetCol(DirMat,k)*MaxStep[k];
         Hi[2]=MAPHiFunc(Par+Step);
         u=0;
         while ((Hi[2]>Hi[0]) && (u<3)) 
           {Step=Step*0.5;Hi[2]=MAPHiFunc(Par+Step);u++;}
         Hi[1]=MAPHiFunc(Par+Step*0.5);
         d=Hi[2]+Hi[0]-2*Hi[1];
         if (fabs(d)>MathZer) d=1/d;else d=0;
         Pos[3]=((Hi[2]+3* Hi[0]-4*Hi[1])/4)*d;
         Hi[3]=MAPHiFunc(Par+Step*Pos[3]);
         i=0;d=Hi[0];
         for (int k1=1;k1<=3;k1++) if (Hi[k1]<d) {i=k1;d=Hi[k1];}
         Par=Par+Step*Pos[i];
         if ((fabs(Pos[i])<0.5) || (u!=0)) MaxStep[k]*=0.5;
         else MaxStep[k]*=Pos[i];
//cout<<" Posi "<<Pos[i]<<" k "<<k;
//cout<<" dHi   "<<Hi[0]-d<<" Hi "<<d<<"\n";
         Hi[0]=d;               
        }
      Tmp[k]=0;
     }
//cout<<"\n";
//   MaxStep=MaxStep*0.5; 
   CurHi=Hi[0];
  };




  */