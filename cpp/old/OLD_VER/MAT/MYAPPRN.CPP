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
//cout<<" LPar "<<Lpar;
   double ret=MAPVar->Hi_Func(Lpar.Ptr);
   return ret;
  };

void SetGrFunc(MatrCl &DatM,VecCl &MisV,VecCl &Par,VecCl &Step)
  {
   int Dim=Par[0],DimGr=MAPVar->ExperPoint[0];
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
   double d,*ptr=MAPVar->HiPntCoef;
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
  };

int MAPGetDir(VecCl &Par,VecCl &MaxStep,MatrCl &DirMat,double ErrorMatr,
              double &CurHi) 
  {
   int Dim=Par[0],DimGr=MAPVar->ExperPoint[0];
   if ((Dim>0) && (DimGr>0))
     {
      int k,k1;
      double d;
      MatrCl DatM;
      VecCl  MisV(Dim),Norm(Dim),EigV(Dim);
      SetGrFunc(DatM,MisV,Par,MaxStep);
      SetCorelFunc(DatM,MisV,CurHi); // now DatGr(Dim,Dim) correlations only
      for (k=1;k<=Dim;k++) 
        if ((d=DatM(k,k))>MathZer) Norm[k]=1/sqrt(d);else Norm[k]=0;
//        Norm[k]=1;
      DatM=NormMatr(Norm,DatM,Norm);
      if (!ReduceLQ(DatM,DatM,EigV.Ptr,ErrorMatr)) 
        {cout<<" Cannot Calculate ReduceLQ. \n";return 0;}
      
      DirMat=NormMatr(Norm,Transpon(DatM),Norm*0+1);

//cout<<" EigV \n"<<EigV;
      for (k=1;k<=Dim;k++) 
        if ( (fabs(d=EigV[k]))>MathZer) EigV[k]=1/d;else EigV[k]=0;
      MaxStep=MaxStep*0-NormMatr(EigV,DatM,Norm)*MisV;
//cout<<" MaxStep "<<MaxStep<<"\n";
//cout<<" DirMat  \n"<<DirMat;
//ChRead();
   }
  return 1;
 };

void MAPIter(VecCl &Par,VecCl &MaxStep,MatrCl &DirMat,double &CurHi)
  {
   
   int Dim=DirMat.Dim(),k,i,k1;
   double Pos[4]={0,0.5,1,0};
   double Hi[4],d;Hi[0]=CurHi;
//double x=MAPHiFunc(Par);cout<<" External "<<x<<" Internal "<<CurHi<<" Dim "<<Dim<<"\n";
   VecCl Step(Dim),Tmp(Dim);Tmp=MaxStep;
   for (int k0=1;k0<=Dim;k0++)
     {
      d=0;i=1;
      for (k1=1;k1<=Dim;k1++) {if (fabs(Tmp[k1])>d) {i=k1;d=fabs(Tmp[k1]);}}
      k=i;
      if (fabs(MaxStep[k])>MathZer)
        {
         Step=GetCol(DirMat,k)*MaxStep[k];
         Hi[1]=MAPHiFunc(Par+Step*0.5);Hi[2]=MAPHiFunc(Par+Step);
         d=Hi[2]+Hi[0]-2*Hi[1];
         if (fabs(d)>MathZer) d=1/d;else d=0;
         Pos[3]=((Hi[2]+3* Hi[0]-4*Hi[1])/4)*d;
         Hi[3]=MAPHiFunc(Par+Step*Pos[3]);
         i=0;d=Hi[0];
         for (int k1=1;k1<=3;k1++) if (Hi[k1]<d) {i=k1;d=Hi[k1];}
         Par=Par+Step*Pos[i];
//         MaxStep[k]*=Pos[i];
//cout<<" Posi "<<Pos[i];           
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


int MAPClc(double *Param,double *MaxStep,int Iter,
           XAr_func HiF,GR_XAr_func GrF,double *Exper,double Misfit,double *HiPnt)
  {

   MAPVar =new MAPVarStrc; 
//   MAPVar->Hi_Func=HiF;
//cout<<"\n";
   MAPVar->Gr_Func=GrF;MAPVar->ExperPoint=Exper;MAPVar->Hi_Func=HiF;
   MAPVar->HiPntCoef=HiPnt;MAPVar->LongPar=Param;
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
   if ( (Iter>0) && (!MAPGetDir(Par,Step,DirMat,Misfit,Hi)) ) 
     {delete MAPVar;return 0;};
   for (k=1;k<=Iter;k++) MAPIter(Par,Step,DirMat,Hi);
   
   for (k=1;k<=cur;k++) 
     {
      MaxStep[MAPVar->LongNum[k]]=Step[k];
      Param[MAPVar->LongNum[k]]=Par[k];
     }
   delete MAPVar;
   return 1;
  };


