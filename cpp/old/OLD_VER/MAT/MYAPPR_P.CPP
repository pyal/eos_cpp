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
//cout<<"True_Hi  ";
   int Dimen=(*MAPVar).RecalcPar[0],NewDim=0;
   double *zerpar =(*MAPVar).RecalcPar,*tmp=new double[Dimen+1];
   for (int k=0;k<=Dimen;k++)
     {
      if (zerpar[k]==NullNumber) {NewDim++;tmp[k]=Par[NewDim];}
      else tmp[k]=zerpar[k];
//cout<<tmp[k]<<"   ";
     }
//cout<<"\n";
   double ret=(*MAPVar).Hi_Func(tmp);
//cout<<" O Key\n";
   delete tmp;
   return ret;
  };
void True_Gr(double *Par,double *GrArr)
  {
   int Dimen=(*MAPVar).RecalcPar[0],NewDim=0;
   double *zerpar =(*MAPVar).RecalcPar,*tmp=new double[Dimen+1];
//cout<<"Par Gr "<<"\n";
   for (int k=0;k<=Dimen;k++)
     {
      if (zerpar[k]==NullNumber) {NewDim++; tmp[k]=Par[NewDim];}
      else tmp[k]=zerpar[k];
//cout<<tmp[k]<<"   ";
     }
//cout<<"\n";
   (*MAPVar).Gr_Func(tmp,GrArr);
   delete tmp;
  };
double Correl_Func(int Dim,double *Exper1,double *Exper2)
  {
   double s=0;
   if (MAPVar->HiPntCoef!=NULL)
     for (int k=1;k<=Dim;k++) {s+=Exper1[k]*Exper2[k]*MAPVar->HiPntCoef[k];}
   else for (int k=1;k<=Dim;k++) {s+=Exper1[k]*Exper2[k];}
   return s;
  };  
//typedef double* FOOL;
int MAPGetDir(double *Param,double *MaxStep,MatrCl &DirMat,double ErrorMatr) 
  {
//cout<<"   MAPGetDir  Begin  \n";
   int Dimen=Param[0],DimExp=(*MAPVar).ExperPoint[0],Ret=1;
   if ((Dimen>0) && (DimExp>0))
     {
      int k,k1;
      int Iter;
      VecCl TmpGr(DimExp),CurGr(DimExp);
      VecCl HiGrad(Dimen);
      MatrCl WMat1(Dimen),WMat2(Dimen);
      double TmpHi,CurHi=True_Hi(Param); 
//cout<<" Coreleft MAPDir "<<coreleft()<<"\n";
      double *TmpPar=new double[Dimen+1];
      double *NormDiag=new double[Dimen+1];
      VecCl *GrDat=new VecCl[Dimen+1];
      for (k=1;k<=Dimen;k++) GrDat[k].SetDim(DimExp);

      True_Gr(Param,CurGr.Ptr);
// begining of calculation of d(hi)/dx[k] in HiGrad[k]
//cout<<" Coreleft 90     "<<coreleft()<<"\n";
      for (k=1;k<=Dimen;k++)
        {
if (fabs(MaxStep[k])<MathZer) cout<<" MAPGetDir MaxStep k "<<MaxStep[k]<<"  "<<k<<"\n";
         movmem(Param,TmpPar,sizeof(double)*(Dimen+1));
         TmpPar[k]+=DERIVESTP*MaxStep[k];
         True_Gr(TmpPar,TmpGr.Ptr);
         for (k1=1;k1<=DimExp;k1++) 
           GrDat[k][k1]=(TmpGr[k1]-CurGr[k1])/(DERIVESTP*MaxStep[k]);
         TmpHi=True_Hi(TmpPar);
         HiGrad[k]=(CurHi-TmpHi)/(DERIVESTP*MaxStep[k]);
        }
//cout<<" Hi Grad "<<HiGrad<<"Cur_Hi "<<CurHi<<"\nMax Step ";
//      for (k=1;k<=Dimen;k++) cout<<MaxStep[k]<<"  ";cout<<"\n";
//ChRead();
//
//     end of calculation of d(hi)/dx[k] in HiGrad[k]}
//
//{ begining of calculation d(integral(dx[i]))/dx[k]=hmat[i,k]
//                                       D'[k]=correl[k]               }
      for (k=1;k<=Dimen;k++)
        {
         for (k1=k;k1<=Dimen;k1++) 
            WMat1(k,k1)=Correl_Func(DimExp,GrDat[k].Ptr,GrDat[k1].Ptr);
         if (WMat1(k,k)<MathZer) 
           {cout<<" Function does not depend from parameter "<<k<<". Exiting\n";
            Ret=0;goto exi;
           }
         double d=sqrt(WMat1(k,k));
if (d<MathZer) cout<<" MAPGetDir sqrt(WMat1(k,k)) k "<<d<<"  "<<k<<"\n";
         NormDiag[k]=1/d;//sqrt(WMat1(k,k));
        }
//cout<<" dxi/dxk \n"<<WMat1;
      for ( k=1;k<=Dimen;k++)
       { 
        for (k1=k;k1<=Dimen;k1++)
          {
           WMat1(k,k1)=WMat1(k,k1)*NormDiag[k]*NormDiag[k1];
           WMat1(k1,k)=WMat1(k,k1);
          }
       } 
//cout<<" dxi/dxk normalized\n"<<WMat1;
//  fwritem(nomparam,addr(hmat),1,1);k0:=ermes('first converted',-1);
//{ end      of calculation d(integral(dx[i]))/dx[k]=hmat[i,k]
//                                       D'[k,k]=correl[k]               }
//cout<<" Coreleft 129    "<<coreleft()<<"\n";

//      if ((Err=Jacobi(WMat1,1000,ErrorMatr,TmpPar,WMat1,Iter))!=0) 
//        {cout<<" Cannot Calculate Jacobi. Error ="<<Err<<" \n";exit(1);}
      if (!ReduceLQ(WMat1,WMat1,TmpPar,ErrorMatr)) 
        {cout<<" Cannot Calculate ReduceLQ. \n";Ret=0;goto exi;}
//cout<<" Coreleft 132=129"<<coreleft()<<"\n";
//cout<<" Eigen Vectors\n"<<WMat1<<" Eigen Val \n";ChRead();
//for ( k=1;k<=Dimen;k++) cout<<TmpPar[k]<<"   ";cout<<"\n";
      
//      Transpon(WMat1);  //Because ReduceLQ we have not to transpon
      WMat2=WMat1;
//cout<<" Coreleft 137    "<<coreleft()<<"\n";
//cout<<"Inverse ";
      Inverse(WMat1);
//cout<<" O Key \n";
//cout<<" Coreleft 139=137"<<coreleft()<<"\n";
      for ( k=1;k<=Dimen;k++)
       { 
        for ( k1=1;k1<=Dimen;k1++)
          {
         double d=sqrt(fabs(TmpPar[k1]));
if (d<MathZer) cout<<" MAPGetDir sqrt(fabs(TmpPar[k1])) k1 "<<d<<"  "<<k1<<"\n";
           WMat2(k,k1)=WMat2(k,k1)/d;//sqrt(fabs(TmpPar[k1]));
          } 
       } 
      WMat1=WMat2*WMat1;
//cout<< " Result Matr After Mull\n"<<WMat1;
      for ( k=1;k<=Dimen;k++)
       { 
        for ( k1=1;k1<=Dimen;k1++)
           WMat1(k,k1)=WMat1(k,k1)*NormDiag[k];
       } 
//cout<<" MaxStep: ";
      for ( k=1;k<=Dimen;k++)
       { 
        double s=0;
        for (int k1=1;k1<=Dimen;k1++)  s+=HiGrad[k1]*WMat1(k1,k);
        MaxStep[k]=s;
//cout<<MaxStep[k]<<"  ";
       } 
//cout<<"\n";
//ChRead();
//cout<<" Coreleft 158=90    "<<coreleft()<<"\n";
      DirMat=WMat1;
//cout<<" Coreleft 160 "<<coreleft()<<"\n";
//cout<< " Dir  Matr \n"<<DirMat;
//cout<<"MaxStep ";for (k=1;k<=Dimen;k++) cout<<MaxStep[k]<<"  ";cout<<"\n";
  exi:
    delete TmpPar;
    delete NormDiag;
    for ( k=0;k<=Dimen;k++)  {delete GrDat[k].Ptr;GrDat[k].Ptr=NULL;}
    delete GrDat;
//cout<< "End  MAPGetDir\n";
//cout<<" Coreleft MAPDir "<<coreleft()<<"\n";
   }
  return Ret;
 };


void SetMaxStp(double *stp,double coe)
  {
   for (int k=1;k<=stp[0];k++) stp[k]*=coe;
  };
double MAPHiFunc(double x)
  {
    int Dimen=(*MAPVar).CurParam[0];
//cout<<" Dimen= "<<Dimen;
    double *tmpptr=new double[Dimen+1];
    for (int k=1;k<=Dimen;k++) 
      tmpptr[k]=(*MAPVar).CurParam[k]+x*(*MAPVar).StepDir[k];
    double ret=True_Hi(tmpptr);
//cout<<" MAPHi =  "<<ret<<"\n";
    (*MAPVar).TmpIter++;
    delete tmpptr;
    return ret;
  };


void MAPIter(double *Param,double *MaxStep,MatrCl DirMat)
  {
   int Dimen=DirMat.Dim(),k;Param[0]=Dimen;
   double DeltaHi=True_Hi(Param);
   double BegHi=DeltaHi,EndHi=DeltaHi;//,FirstHi=DeltaHi;
   double Hi[4];Hi[0]=BegHi;
//   ClcMinStrc MinObj;
//cout<<" Coreleft MAPIter "<<coreleft()<<"\n";
   double *Step=new double[Dimen+1]; 
   (*MAPVar).StepDir=&Step[0];
   (*MAPVar).CurParam=Param;
//   double NewStpCoef=0;
//cout<<"\n Dir Matr \n"<<DirMat<<" MaxStep:   ";
//for ( k=1;k<=Dimen;k++) cout<<MaxStep[k]<<"   "<<Param[k]<<"   ";cout<<"\n";ChRead();
   for (int k0=1;k0<=Dimen;k0++)
     {
      BegHi=EndHi;
//cout<<" Step ";
      for ( k=1;k<=Dimen;k++)  {Step[k]=MaxStep[k0]*DirMat(k,k0);
//cout<<Step[k]<<"   ";
                               }
//cout<<"\n";
      double s1,s;
//      Hi[0]=BegHi;
//      Hi[2]=MAPHiFunc(1);

//      Hi[1]=Hi[0];
//      Hi[0]=MAPHiFunc(-0.5);
//      Hi[2]=MAPHiFunc(0.5);
      Hi[1]=MAPHiFunc(0.5);
      Hi[2]=MAPHiFunc(1);
      double d_tmp=Hi[2]+Hi[0]-2*Hi[1];
//cout<<" Hi "<<Hi[0]<<"   "<<Hi[1]<<"  "<<Hi[2]<<"  "<<d_tmp<<"   ";
     
      if (fabs(d_tmp)>MathZer) s1=1/d_tmp;
      else s1=0;
//cout<<"MAPIter  s1 = "<<s1<<"   "<<1/MathZer<<"   ";
//      s1=((Hi[2]+3* Hi[0]-4*Hi[1])/4*s1)-0.5;
      s1=((Hi[2]+3* Hi[0]-4*Hi[1])/4)*s1;
//cout<<" MAPIter  s1 = "<<s1<<"\n";
      if (fabs(s1)>1) s1=s1/fabs(s1);

//s0=s1;
      Hi[3]=MAPHiFunc(s1);
      s=Hi[0];
      int k1=0;
      for ( k=1;k<=3;k++) if (Hi[k]<s) {k1=k;s=Hi[k];}
      if (k1==0) s1=0;else if (k1==1) s1=0.5;else if (k1==2) s1=1;
//      if (k1==0) s1=-0.5;else if (k1==1) s1=0;else if (k1==2) s1=0.5;

      for ( k=1;k<=Dimen;k++) Param[k]+=Step[k]*s1;
//      BegHi=Hi[0];
//cout<<"   GetSpt "<<s1<<" NewHi "<<s<<"Clc stp"<<s0<<" hi0 "<<Hi[0]<<" hi1 "<<Hi[1]<<" hi2  "<<Hi[2]<<" hi3 "<<Hi[3]<<"\n";
//cout<<"       MakeStep ";
//for ( k=1;k<=Dimen;k++)  {cout<<Step[k]*s1<<"   ";}
//cout<<"\n";
     Hi[0]=s;
/*
//             Old Version
//cout<<" Step It: ";
//for (int  k2=1;k2<=Dimen;k2++) cout<<Step[k2]<<"   ";cout<<"\n";//ChRead();
      double GetStp=MinObj.MinGoldSqr(-1,1,&(MAPHiFunc),DERIVESTP);
cout<<"GetSpt "<<GetStp;
      EndHi=MAPHiFunc(GetStp);
cout<<" Cur Hi "<<EndHi;                                              
      if (EndHi>BegHi) 
           { cout<<" Error BegHi<EndHi  ";
             EndHi=BegHi;GetStp=0;}
cout<<"\n";
      for ( k=1;k<=Dimen;k++) Param[k]+=Step[k]*GetStp;
//      DeltaHi=(DeltaHi+fabs(BegHi-EndHi))*k0/(k0+1);
//      SetMaxStp(MaxStep[k0],FirstHi,DeltaHi);
//cout<<"Coef to step Hi "<<DeltaHi/FirstHi<<"\n";

*/
     }        
//cout<<"Coef to step Hi "<<0.1*((FirstHi-EndHi)/FirstHi)+DERIVESTP<<"\n";
   SetMaxStp(MaxStep,0.5); 
//cout<<" MAPIter After one iter Hi= "<<Hi[0]<<"\n";
   delete Step;
//cout<<" Coreleft MAPIter "<<coreleft()<<"\n";
  };
// ============================================================
// ====================== Main Procedure ======================
// ============================================================
int MAPClc(double *Param,double *MaxStep,int Iter,
           XAr_func HiF,GR_XAr_func GrF,double *Exper,double Misfit);

int MAPClc(double *Param,double *MaxStep,int Iter,
           XAr_func HiF,GR_XAr_func GrF,double *Exper,double Misfit,double *HiPnt)
  {
   MatrCl DirMat;

//cout<<" Coreleft MAPClc  "<<coreleft()<<"\n";
//cout<<"  MAPClc  Begin \n";
   MAPVar =new MAPVarStrc; 
   (*MAPVar).Hi_Func=HiF;(*MAPVar).Gr_Func=GrF;
   (*MAPVar).ExperPoint=Exper;
   (*MAPVar).TmpIter=0;
   (*MAPVar).HiPntCoef=HiPnt;
   InitTruePar(Param,MaxStep);
//cout<<"MAPClc Par "<<Param[0]<<"\n";
//for (int k1=1;k1<=Param[0];cout<<Param[k1++]<<"   ");cout<<"\n";
//   VecCl HiGrad(Param[0]);
   if (!MAPGetDir(Param,MaxStep,DirMat,Misfit)) {delete MAPVar;return 0;};
   for (int k=1;k<=Iter;k++) MAPIter(Param,MaxStep,DirMat);
   GetTruePar(Param,MaxStep);                                         
//cout<<" Full Iter "<<(*MAPVar).TmpIter<<"\n";//ChRead();
   delete MAPVar;

//cout<<"  MAPClc  End   \n";
//cout<<" Coreleft MAPClc  "<<coreleft()<<"\n";
   return 1;
  };

int MAPClc(double *Param,double *MaxStep,int Iter,
    XAr_func HiF,GR_XAr_func GrF,double *Exper,double Misfit)
  {  return MAPClc(Param,MaxStep,Iter,HiF,GrF,Exper,Misfit,NULL);  };

