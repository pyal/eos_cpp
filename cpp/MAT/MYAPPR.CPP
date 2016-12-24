#include <lib/precompiled/math.h>
#include "myappr.h"
// ===================================================================
// ========================= Begin ===================================
// ===================================================================
#define DERIVESTP 0.01

VecCl SetParLong(VecCl Par)
  {
   VecCl Ret=MAPVar->LongPar;
   int *pos=&MAPVar->LongNum[0];
   for (int k=1;k<=Par[0];k++) Ret[pos[k]]=Par[k];
   return Ret;
  }; 

double MAPHiFunc(VecCl Par)
  {
   VecCl Lpar=SetParLong(Par);
   double ret=MAPVar->Hi_Func(Lpar.Ptr);
   return ret;
  };


int MAPGetDir(VecCl &Par,VecCl &MaxStep,MatrCl &DirMat,double ErrorMatr,
              double &CurHi) 
  {
   int Dim=Par.Dim(),DimGr=1;
   if (MAPVar->ExperPoint.Ptr!=NULL) DimGr=MAPVar->ExperPoint.Dim();
   if ((Dim>0) && (DimGr>0))
     {
      int k;//,k1;
      double d;
      MatrCl DatM;
      VecCl  MisV(Dim),Norm(Dim),EigV(Dim);
//      SetGrFunc(DatM,MisV,Par,MaxStep);
//      SetCorelFunc(DatM,MisV,CurHi); // now DatGr(Dim,Dim) correlations only
// MisV - Derivativ of the function
      MAPVar->SetCorrelation(DatM,MisV,CurHi,Par,MaxStep);
      for (k=1;k<=Dim;k++) 
        if ((d=DatM(k,k))>MathZer) Norm[k]=1/sqrt(d);else Norm[k]=0;
      DatM=VectorNormalizeMatr(Norm,DatM,Norm);
      if (!ReduceLQ(DatM,DatM,EigV.Ptr,ErrorMatr)) 
        {cout<<" Cannot Calculate ReduceLQ. \n";return 0;}
      DatM=Transpon(DatM);      
      for (k=1;k<=Dim;k++) 
        if ( (fabs(d=EigV[k]))>MathZer) EigV[k]=1/d;else EigV[k]=0;
      MaxStep=MaxStep*0-VectorNormalizeMatr(EigV,DatM,Norm)*MisV;
//cout<<" MaxStep "<<MaxStep<<"\n";
      DirMat=VectorNormalizeMatr(Norm,Transpon(DatM),Norm*0+1);
   }
  return 1;
 };

void MAPIter(VecCl &Par,VecCl &MaxStep,MatrCl &DirMat,double &CurHi)
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
         Step=MatrCl::GetCol(DirMat,k)*MaxStep[k];
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
// ============================================================
// ====================== Main Procedure ======================
// ============================================================


//int MAPClc(double *Param,double *MaxStep,int Iter,
//           XAr_func HiF,GR_XAr_func GrF,double *Exper,double Misfit,double *HiPnt)
int MAPClc(double *Param,double *MaxStep,int Iter,
    XAr_func HiF,double ErrMisfit,int MinHi,
    GR_XAr_func GrF,double *Exper,double *HiPnt)
  {

   MAPVar =new MAPVarStrc; 
   MAPVar->Hi_Func=HiF;
   MAPVar->HiPntCoef=HiPnt;MAPVar->LongPar=Param;MAPVar->Iter=0;
   if (MinHi)
     {MAPVar->Gr_Func=GrF;MAPVar->ExperPoint=Exper;MAPVar->SetCorrelation=CorrelationHi;}
   else MAPVar->SetCorrelation=CorrelationMin;

   int cur=0,k;
   for (k=1;k<=Param[0];k++) 
     if (fabs(MaxStep[k])>MathZer) {cur++;MAPVar->LongNum[cur]=k;}
   VecCl Step(cur),Par(cur);
   for (k=1;k<=cur;k++) 
     {
      Step[k]=MaxStep[MAPVar->LongNum[k]];
      Par[k]=Param[MAPVar->LongNum[k]];
     }
      
   MatrCl DirMat(cur);
   double Hi;
   if ( (Iter>0) && (!MAPGetDir(Par,Step,DirMat,ErrMisfit,Hi)) ) 
     {delete MAPVar;return 0;};
   for (k=1;k<=Iter;k++) MAPIter(Par,Step,DirMat,Hi);
   
   for (k=1;k<=cur;k++) 
     {
      MaxStep[MAPVar->LongNum[k]]=Step[k];
      Param[MAPVar->LongNum[k]]=Par[k];
     }
//cout<<"\n N iter "<<MAPVar->Iter<<"\n";
   delete MAPVar;
   return 1;
  };
//#include "gaussolv.h"
void CorrelationMin(MatrCl &DatM,VecCl &MisV,double &CurHi,VecCl &Par,
                                                          VecCl &MaxStep)
  {
   int Dim=Par.Dim(),k,k1;
   VecCl Stp(Dim),Fun(Dim);Stp=MaxStep*DERIVESTP;
   MatrCl ResM(Dim);
   CurHi=MAPHiFunc(Par);
   for (k=1;k<=Dim;k++) {MisV=Par;MisV[k]+=Stp[k];Fun[k]=MAPHiFunc(MisV);}
   for (k=1;k<=Dim;k++)
     {
      for (k1=k;k1<=Dim;k1++)
        {
         MisV=Par;MisV[k]+=Stp[k];MisV[k1]+=Stp[k1];
         ResM(k,k1)=(MAPHiFunc(MisV)+CurHi-Fun[k]-Fun[k1])/(Stp[k]*Stp[k1]);
         ResM(k1,k)=ResM(k,k1);
        }
     };
   MisV=MaxStep;DatM=ResM*0.5;

//MisV=MaxStep;
for (k=1;k<=Dim;k++) Fun[k]=(Fun[k]-CurHi)/Stp[k];
MisV=Fun*0.5;
for (k=1;k<=Dim;k++) {if (DatM(k,k)<0) 
{MisV=MisV*0.5;for (k1=1;k1<=Dim;k1++) if (k!=k1) DatM(k1,k)=DatM(k,k1)=0;DatM(k,k)=fabs(DatM(k,k));}}
//cout<<"\n Correlation \n"<<DatM<<"\n Step "<<MisV<<"\n";//<<" Deriv "<<Fun<<" MaxStep "<<MaxStep<<"\n";
  };
   
   


void SetGrFunc(MatrCl &DatM,VecCl &MisV,VecCl &Par,VecCl &Step)
  {
   int Dim=Par.Dim(),DimGr=MAPVar->ExperPoint.Dim();
   DatM.SetMatr(Dim,DimGr);MisV.Dim(DimGr);
   VecCl tmp(DimGr);
   VecCl ParL=SetParLong(Par),TmpL;
   MAPVar->Gr_Func(ParL.Ptr,MisV.Ptr);
   for (int k=1;k<=Dim;k++)
     {
      double d=DERIVESTP*Step[k];
      int i=MAPVar->LongNum[k];TmpL=ParL;TmpL[i]=TmpL[i]+d;
      MAPVar->Gr_Func(TmpL.Ptr,tmp.Ptr);
      MatrCl::SetRow(DatM,k,(tmp-MisV)/d);
     }
   MisV=(MisV-MAPVar->ExperPoint);
  };    

void SetCorelFunc(MatrCl &DatM,VecCl &MisV,double &CurHi)
  {
   int Dim,DimGr,k,k1;
   DatM.GetDimMN(Dim,DimGr);
   VecCl tmp(DimGr),ResV(Dim);
   MatrCl ResM(Dim);
   double *ptr=MAPVar->HiPntCoef;//d,
   for (k=1;k<=Dim;k++)
     { 
      tmp=MatrCl::GetRow(DatM,k);
      if (ptr!=NULL) {for (k1=1;k1<=DimGr;k1++) tmp[k1]*=ptr[k1];}
      for (k1=1;k1<k;k1++) 
        {ResM(k1,k)=ResM(k,k1)=tmp*MatrCl::GetRow(DatM,k1);}
      ResM(k,k)=tmp*MatrCl::GetRow(DatM,k);
      ResV[k]=tmp*MisV;
     };
   tmp=MisV;if (ptr!=NULL) {for (k1=1;k1<=DimGr;k1++) tmp[k1]*=ptr[k1];}
   CurHi=(tmp*MisV);

   DatM=ResM;MisV=ResV;
//cout<<"\n Correlation \n"<<DatM<<"\n Step "<<MisV<<"\n";
  };



void CorrelationHi(MatrCl &DatM,VecCl &MisV,double &CurHi,VecCl &Par,
                                                          VecCl &MaxStep)
  {
   SetGrFunc(DatM,MisV,Par,MaxStep);
   SetCorelFunc(DatM,MisV,CurHi);
  };  // now DatGr(Dim,Dim) correlations only





// =========================================================
// ==========   END    =====================================
// =========================================================

/*
VecCl Stat_Par,Stat_StpDir;
double MAPMinFun(double stp)
{
	double ret=MAPHiFunc(Stat_Par+Stat_StpDir*stp);
	return ret;
};
#include "mtmin.h"
void MAPIter1(VecCl &Par,VecCl &MaxStep,MatrCl &DirMat,double &CurHi)
  {
   int Dim=DirMat.Dim(),k,i,k1,u;
   VecCl Tmp(Dim);Tmp=MaxStep;
   double d;
   for (int k0=1;k0<=Dim;k0++)
     {
      d=0;i=k0;
      for (k1=1;k1<=Dim;k1++) {if (fabs(Tmp[k1])>d) {i=k1;d=fabs(Tmp[k1]);}}
      k=i;
      if (fabs(MaxStep[k])>MathZer)
        {
		 Stat_Par=Par;Stat_StpDir=GetCol(DirMat,k);
//		 double stp=MinFunc(-fabs(MaxStep[k]*3),fabs(MaxStep[k]*3),MAPMinFun,StndErr*max<double>(1,fabs(MaxStep[k]*6)));
//double NAGMin(X_func F,double relat_err,double abs_err,double MinB,
//              double MaxB,int MaxCal,double F_res,int Fail);
		 int fail;
		 double stp=NAGMin(MAPMinFun,StndErr,StndErr,-fabs(MaxStep[k]*3),fabs(MaxStep[k]*3),1000,CurHi,fail);
		 if (fail==0) Par=Par+Stat_StpDir*stp;
CurHi=MAPHiFunc(Par);

         if (fabs(stp)<0.3) MaxStep[k]*=0.3;
         else MaxStep[k]=stp;

        }
      Tmp[k]=0;
     }
//   CurHi=MAPHiFunc(Par);
  };
*/