#include "myappr.h"
// ===================================================================
// ========================= Begin ===================================
// ===================================================================
#define DERIVESTP 0.01

void InitTruePar(double *Par,double *Stp)
  {
   int Dimen=Par[0],NewDim=0;
   double *tmp=new double[Dimen+1];tmp[0]=Dimen;
   for (int k=1;k<=Dimen;k++)
     {
      if (fabs(Stp[k])<MathZer) tmp[k]=Par[k];
      else
        {tmp[k]=NullNumber;NewDim++;
         Stp[NewDim]=Stp[k];Par[NewDim]=Par[k];}
     }
   Stp[0]=NewDim;Par[0]=NewDim;
   (*MAPVar).RecalcPar=tmp;
  };
void GetTruePar(double *Par,double *Stp)
// Also deleting ( only 1 use after init )
  {
   int Dimen=(*MAPVar).RecalcPar[0],k,NewDim=0;
   double *tmpstp=new double[Dimen+1];tmpstp[0]=Dimen;
   double *tmppar =(*MAPVar).RecalcPar;
   for (k=1;k<=Dimen;k++)
     {
      if (tmppar[k]==NullNumber)
        { NewDim++;tmppar[k]=Par[NewDim];
          tmpstp[k]=Stp[NewDim];}
      else tmpstp[k]=0;
     }
   for (k=0;k<=Dimen;k++) { Par[k]=tmppar[k];Stp[k]=tmpstp[k];}
   delete tmpstp;
   delete tmppar;
  };
double True_Hi(double *Par)
  {
   int Dimen=(*MAPVar).RecalcPar[0];
   double *zerpar =(*MAPVar).RecalcPar,*tmp=new double[Dimen+1];
   for (int k=0;k<=Dimen;k++)
     {
      if (zerpar[k]==NullNumber) tmp[k]=Par[k];
      else tmp[k]=zerpar[k];
     }
   double ret=(*MAPVar).Hi_Func(tmp);
   delete tmp;
   return ret;
  };
void True_Gr(double *Par,double *GrArr)
  {
   int Dimen=(*MAPVar).RecalcPar[0];
   double *zerpar =(*MAPVar).RecalcPar,*tmp=new double[Dimen+1];
   for (int k=0;k<=Dimen;k++)
     {
      if (zerpar[k]==NullNumber) tmp[k]=Par[k];
      else tmp[k]=zerpar[k];
     }
   (*MAPVar).Gr_Func(tmp,GrArr);
   delete tmp;
  };
double Correl_Func(int Dim,double *Exper1,double *Exper2)
  {
   double s=0;
   for (int k=1;k<=Dim;k++) {s+=(Exper1[k]*Exper2[k]);}
   return s;
  };  
//typedef double* FOOL;
void MAPGetDir(double *Param,double *MaxStep,MatrCl &DirMat,double ErrorMatr) 
  {
cout<<"   MAPGetDir  Begin  \n";
   int Dimen=Param[0],DimExp=(*MAPVar).ExperPoint[0];
   if ((Dimen>0) && (DimExp>0))
     {
      int k,k1;
      int Iter,Err;
      VecCl TmpGr(DimExp),CurGr(DimExp);
      VecCl HiGrad(Dimen);
      MatrCl WMat1(Dimen),WMat2(Dimen);
      double TmpHi,CurHi=True_Hi(Param); 
cout<<" Coreleft MAPDir "<<coreleft()<<"\n";
      double *TmpPar=new double[Dimen+1];
      double *NormDiag=new double[Dimen+1];
      VecCl *GrDat=new VecCl[Dimen+1];
      for (k=1;k<=Dimen;k++) GrDat[k].SetDim(DimExp);

      True_Gr(Param,CurGr.Ptr);
// begining of calculation of d(hi)/dx[k] in HiGrad[k]
cout<<" Coreleft 90     "<<coreleft()<<"\n";
      for (k=1;k<=Dimen;k++)
        {
         movmem(Param,TmpPar,sizeof(double)*(Dimen+1));
         TmpPar[k]+=DERIVESTP*MaxStep[k];
         True_Gr(TmpPar,TmpGr.Ptr);
         for (k1=1;k1<=DimExp;k1++) 
           GrDat[k][k1]=(TmpGr[k1]-CurGr[k1])/(DERIVESTP*MaxStep[k]);
         TmpHi=True_Hi(TmpPar);
         HiGrad[k]=(CurHi-TmpHi)/(DERIVESTP*MaxStep[k]);
        }
//cout<<" Coreleft 101==90"<<coreleft()<<"\n";
cout<<" Hi Grad "<<HiGrad<<"Cur_Hi "<<CurHi<<"\nMax Step ";
      for (k=1;k<=Dimen;k++) cout<<MaxStep[k]<<"  ";cout<<"\n";
ChRead();
//
//     end of calculation of d(hi)/dx[k] in HiGrad[k]}
//
//{ begining of calculation d(integral(dx[i]))/dx[k]=hmat[i,k]
//                                       D'[k]=correl[k]               }
      for (k=1;k<=Dimen;k++)
        {
         for (k1=k;k1<=Dimen;k1++) 
            WMat1(k,k1)=Correl_Func(DimExp,GrDat[k].Ptr,GrDat[k1].Ptr);
         NormDiag[k]=1/sqrt(WMat1(k,k));
        }
cout<<" dxi/dxk \n"<<WMat1;
      for ( k=1;k<=Dimen;k++)
       { 
        for (k1=k;k1<=Dimen;k1++)
          {
           WMat1(k,k1)=WMat1(k,k1)*NormDiag[k]*NormDiag[k1];
           WMat1(k1,k)=WMat1(k,k1);
          }
       } 
cout<<" dxi/dxk normalized\n"<<WMat1;
//  fwritem(nomparam,addr(hmat),1,1);k0:=ermes('first converted',-1);
//{ end      of calculation d(integral(dx[i]))/dx[k]=hmat[i,k]
//                                       D'[k,k]=correl[k]               }
//cout<<" Coreleft 129    "<<coreleft()<<"\n";
      if ((Err=Jacobi(WMat1,1000,ErrorMatr,TmpPar,WMat1,Iter))!=0) 
        {cout<<" Cannot Calculate Jacobi. Error ="<<Err<<" \n";exit(1);}
//cout<<" Coreleft 132=129"<<coreleft()<<"\n";
cout<<" Eigen Vectors\n"<<WMat1<<" Eigen Val \n";
ChRead();
for ( k=1;k<=Dimen;k++) cout<<TmpPar[k]<<"   ";cout<<"\n";
      WMat2=WMat1;
//cout<<" Coreleft 137    "<<coreleft()<<"\n";
      Inverse(WMat1);
//cout<<" Coreleft 139=137"<<coreleft()<<"\n";
      for ( k=1;k<=Dimen;k++)
       { 
        for ( k1=1;k1<=Dimen;k1++)
           WMat2(k,k1)=WMat2(k,k1)/sqrt(TmpPar[k1]);
       } 
      WMat1=WMat2*WMat1;
cout<< " Result Matr After Mull\n"<<WMat1;
      for ( k=1;k<=Dimen;k++)
       { 
        for ( k1=1;k1<=Dimen;k1++)
           WMat1(k,k1)=WMat1(k,k1)*NormDiag[k];
       } 
      for ( k=1;k<=Dimen;k++)
       { 
        double s=0;
        for (int k1=1;k1<=Dimen;k1++)  s+=HiGrad[k1]*WMat1(k1,k);
        MaxStep[k]=s;
       } 
cout<<" Coreleft 158=90    "<<coreleft()<<"\n";
      DirMat=WMat1;
cout<<" Coreleft 160 "<<coreleft()<<"\n";
cout<< " Dir  Matr \n"<<DirMat;
cout<<"MaxStep ";for (k=1;k<=Dimen;k++) cout<<MaxStep[k]<<"  ";cout<<"\n";
  delete TmpPar;
  delete NormDiag;
      for ( k=0;k<=Dimen;k++)  delete GrDat[k].Ptr;
  delete GrDat;
cout<< "End  MAPGetDir\n";
cout<<" Coreleft MAPDir "<<coreleft()<<"\n";
    }
 };
void SetMaxStp(double *stp,double maxstp,double curstp)
  {
   double coe=1.5*sqrt(fabs(curstp/maxstp));
   for (int k=1;k<=stp[0];k++) stp[k]=stp[k]*coe;
  };
double MAPHiFunc(double x)
  {
    int Dimen=(*MAPVar).CurParam[0];
cout<<" Dime= "<<Dimen;
    double *tmpptr=new double[Dimen+1];
    for (int k=1;k<=Dimen;k++) 
      tmpptr[k]=(*MAPVar).CurParam[k]+x*(*MAPVar).StepDir[k];
    double ret=True_Hi(tmpptr);
cout<<" MAPHi =  "<<ret<<"\n";
    delete tmpptr;
    return ret;
  };


void MAPIter(double *Param,double *MaxStep,MatrCl DirMat)
  {
   int Dimen=DirMat.Dim(),k;Param[0]=Dimen;
   ClcMinStrc MinObj;
cout<<" Coreleft MAPIter "<<coreleft()<<"\n";
   double *Step=new double[Dimen+1]; 
   (*MAPVar).StepDir=&Step[0];
   (*MAPVar).CurParam=Param;
//   Transpon(DirMat);NormalizeEigenvectors(DirMat);Transpon(DirMat);
cout<<" Dir Matr "<<DirMat<<" MaxStep:   ";
for ( k=1;k<=Dimen;k++) cout<<MaxStep[k]<<"   ";cout<<"\n";ChRead();
   for (int k0=1;k0<=Dimen;k0++)
     {
      
      for ( k=1;k<=Dimen;k++)  Step[k]=MaxStep[k]*DirMat(k,k0);
cout<<" Step It:  ";
for (int  k2=1;k2<=Dimen;k2++) cout<<Step[k2]<<"   ";cout<<"\n";ChRead();
      double GetStp=MinObj.MinGoldSqr(0,2,&(MAPHiFunc),1e-2);
cout<<"GetSpt "<<GetStp<<"\n";
      for ( k=1;k<=Dimen;k++) Param[k]+=Step[k]*GetStp;
      SetMaxStp(MaxStep,1,GetStp);
     }        
   delete Step;
cout<<" Coreleft MAPIter "<<coreleft()<<"\n";
  };
// ============================================================
// ====================== Main Procedure ======================
// ============================================================
int MAPClc(double *Param,double *MaxStep,int Iter,
           XAr_func HiF,GR_XAr_func GrF,double *Exper,double Misfit)
  {
   MatrCl DirMat;
cout<<" Coreleft MAPClc  "<<coreleft()<<"\n";
cout<<"  MAPClc  Begin \n";
   MAPVar =new MAPVarStrc; 
   (*MAPVar).Hi_Func=HiF;(*MAPVar).Gr_Func=GrF;
   (*MAPVar).ExperPoint=Exper;
   InitTruePar(Param,MaxStep);
   MAPGetDir(Param,MaxStep,DirMat,Misfit);
   for (int k=1;k<=Iter;k++) MAPIter(Param,MaxStep,DirMat);
   GetTruePar(Param,MaxStep);                                         
   delete MAPVar;
cout<<"  MAPClc  End   \n";
cout<<" Coreleft MAPClc  "<<coreleft()<<"\n";
   return 1;
  };



