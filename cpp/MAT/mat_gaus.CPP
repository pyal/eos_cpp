#include <lib/precompiled/math.h>
#include "mat_gaus.h"

int GauseSolve::NotLUSolve(MatrCl &Matr,VecCl &ConstVec,VecCl &ResultVec)
  {
//cout<<" NotLUSolve \n";ChRead();
   int error=0;
   VecCl *ConstVecPtr,*ResVecPtr;

   ConstVecPtr=new VecCl(ConstVec);
   ResVecPtr=&ResultVec;

   if (error==Initialize(Matr)) goto exi;
   if (error==LU_Decompose()) goto exi;

   error=LU_Solve(*ConstVecPtr,*ResVecPtr);
   exi:
   delete ConstVecPtr;
   return error;
  };

//    Init PermuteRes; Low,Up,Matr
int GauseSolve::Initialize(MatrCl &Matr)
  {
//cout<<" Initialize \n";ChRead();
   DisposeOld();
    
   Dimenshion=Matr.Dim();

   PermuteRes=new int[Dimenshion+1];PermuteRes[0]=Dimenshion;
   for (int k=1;k<=Dimenshion;k++) PermuteRes[k]=k;

   Matrics=Matr;

   Low=Matrics*0;Up=Low;

   return 0;
  };

int GauseSolve::LU_Decompose()
 {
  double coef; 
  int error=1;
  for (int k=1;k<=Dimenshion;k++)
    {
     coef=PivotEl(k);
     if (fabs(coef)<MathZer) goto exi;
     SetLU(coef,k);
    }
//cout<<" Low\n"<<Low<<"Up\n"<<Up<<
//cout<<"Multiply\n"<<Low*Up<<"\n";ChRead();
  error=0;
  exi:
  return error;
};
void GauseSolve::SetLU(double coef,int k)
  {
   Low(k,k)=coef;Up(k,k)=1;
   coef=1/coef;
   double s;
   for (int i=k+1;i<=Dimenshion;i++)
    {
//cout<<" SetLu "<<i<<" UP \n"<<Up<<"\n";ChRead();
     s=Matrics(k,i);
     for (int m=1;m<k;m++) s-=Low(k,m)*Up(m,i);
     Up(k,i)=s*coef;
     s=Matrics(i,k);
     for (int m=1;m<k;m++) s-=Low(i,m)*Up(m,k);
     Low(i,k)=s;
    }
  };
double GauseSolve::PivotEl(int k)
  {
   int Leader=0;
   double val=0,s;
//   for (int i=k;i<=k;i++)
   for (int i=k;i<=Dimenshion;i++)
     {
      s=Matrics(i,k);
      for (int m=1;m<k;m++) s-=Low(i,m)*Up(m,k);
      if ( fabs(val)<fabs(s)) {val=s;Leader=i;}
     }
//cout<<"Leader["<<Leader<<"]="<<val<<"\n";
   if ( (Leader!=0) && (Leader!=k) )
     {
      Change(Matrics,k,Leader);   
      Change(Low,k,Leader);   
      ExchVar(&PermuteRes[k],&PermuteRes[Leader],sizeof(int));
     }
   return val;
  };
void GauseSolve::Change(MatrCl &matr,int i,int j)
  {
//cout<<" Before Matr \n"<<matr<<"\n";
   VecCl get_i=MatrCl::GetRow(matr,i);
   VecCl get_j=MatrCl::GetRow(matr,j);
//cout<<" row i "<<i<<"  "<<get_i<<"\n";
//cout<<" row j "<<j<<"  "<<get_j<<"\n";
   MatrCl::SetRow(matr,i,get_j); MatrCl::SetRow(matr,j,get_i);
//cout<<" After Matr \n"<<matr<<"\n";ChRead();
  };
int GauseSolve::LU_Solve(VecCl& ConstVec,VecCl& ResVec)
  {
   int error=1;
//   MatrCl TmpM(Dimenshion);
//   TmpM=Low*Up;
   PermuteConstants(ConstVec);
   VecCl PartialSolution(Dimenshion);
//for (int i=1;i<=Dimenshion;i++) { cout<<Constants[i]<<"  ";}; cout<<"\n";
//cout<<"Low matr:\n "<<Low;
//cout<<"\nUp Matr:\n "<<Up;
//cout<<"\nThere mult\n"<<TmpM<<"\n";
   double s,tmp;
   int n,k;
   for (n=1;n<=Dimenshion;n++)
     {  
      s=0;
      for (k=1;k<n;k++) s+=PartialSolution[k]*Low(n,k);
      tmp=Low(k,k);
      if (fabs(tmp)<MathZer) goto exi;
      PartialSolution[n]=(ConstVec[n]-s)/tmp;
     }
   for (n=Dimenshion;n>=1;n--)
     {
      s=0;
      for (k=n+1;k<=Dimenshion;k++) s+=ResVec[k]*Up(n,k);
      ResVec[n]=(PartialSolution[n]-s);
     }
   error=0;
   exi:
   return error;
  };
void GauseSolve::PermuteConstants(VecCl& ConstVec)
  {
   VecCl TmpConstants(Dimenshion);
   for (int i=1;i<=Dimenshion;i++) TmpConstants[i]=ConstVec[PermuteRes[i]];
   ConstVec=TmpConstants;
  };
void GauseSolve::DisposeOld()
  { if (Dimenshion!=-1) delete PermuteRes;
    Dimenshion=-1;
  };











/*
 * 
 * 
int GauseSolve::NotIterLU(MatrCl &matr,double *res,double* vec_var,
                                            double Mist)
  {
   double *Sol1,*TrueConst,*Sol=new double[Dimenshion+1];
   double *dist=new double[Dimenshion+1];
   int Good=0,First=1,error;
   double NormRes, NormDist,NormDistL;

   if (error=Initialize(matr,res,Sol1)) goto exi;
   if (error=LU_Decompose()) goto exi;
   if (error=LU_Solve()) goto exi;
   movmem(Sol1,Sol,sizeof(double)*(Dimenshion+1));
   TrueConst=Constants;

   GetDist(TrueConst,Sol,dist);
   NormRes=Norm(Sol);NormDist=Norm(dist);
   while ((!Good) && (!error))
     {
      Constants=dist;
      if (error=LU_Solve()) {break;}
      for (int k=1;k<=Dimenshion;k++)
        { Sol[k]-=Results[k];}
      GetDist(TrueConst,Sol,dist);
      NormDistL=NormDist;NormDist=Norm(dist);
      if (First) {First=0;if (NormDist>0.5*NormDistL) Good=3;}
      if (NormDist>0.5*NormDistL) Good=2;
      if ( NormRes*Mist>NormDist ) Good=1;
cout<<" Norm Disc = "<<NormDist<<"\n";
     }
   Constants=TrueConst;
   exi:
   vec_var=Sol;
   delete Sol1;
   delete dist;
cout<<" Norm Disc = "<<NormDist<<"\n";
   DisposeInter();
   if (error) return 4;
   else return Good-1;
  };
void GauseSolve::GetDist(double *TrueConst,double *Sol1,double *dist)
  {
   double *answer=MatMulVec(Matrics,Sol1);
//cout<<" Dist : \n";
   for (int k=1;k<=Dimenshion;k++) {answer[k]-=TrueConst[k];}
//cout<<answer[k]<<"  ";}
//cout<<"\nTrue Const :\n";
//   for (k=1;k<=Dimenshion;k++) {cout<<TrueConst[k]<<"  ";}
//cout<<"\nSol Full:\n";
//   for (k=1;k<=Dimenshion;k++) {cout<<Sol1[k]<<"  ";}
//cout<<"\n";
   movmem(answer,dist,sizeof(double)*(Dimenshion+1));
   delete answer;
  };
double GauseSolve::Norm(double *Sol1)
  {
   double ret=0;
   for (int k=1;k<=Dimenshion;k++) ret+=fabs(Sol1[k]);
   return ret;
  };    





// ---------------------------------------------------------------      
// -------------------    QRSolution   ---------------------------      
// ---------------------------------------------------------------      
int QRSolve::NotQRSolve(MatrCl &matr,double *res,double* (&vec_var))
  {
   int error=0;
   if (error=Initialize(matr,res,vec_var)) goto exi;
   if (error=QR_Decompose()) goto exi;
   error=QR_Solution();
   exi:
   DisposeInter();
   return error;
  };
int QRSolve::Initialize(MatrCl &matr,double *res,double* (&vec_var))
  {
   
   Dimenshion=res[0];

   Constants=new double[Dimenshion+1];
   movmem(res,Constants,sizeof(double)*(Dimenshion+1));
   Results=new double[Dimenshion+1];Results[0]=Dimenshion;
   vec_var=Results;
   PermuteRes=new int[Dimenshion+1];PermuteRes[0]=Dimenshion;

   for (int k=1;k<=Dimenshion;k++) PermuteRes[k]=k;

   Matrics=matr;
   Up=Low=Matrics*0;

   return 0;
  };
int QRSolve::QR_Decompose()
 {
  int sing=0,k,i;
  double MaxEl=0,Tmp,Sum;
  for (k=1;k<Dimenshion;k++)
    {
     if (MaxEl=MaxColEl(k,k,Dimenshion))<MathZer) 
        { MDiag[k]=0;QDiag[k]=0;sing=1;goto exi; }
     for (i=k;i<=Dimenshion;i++) Matrics(i,k)/=MaxEl;
     Tmp=sign(Matrics(k,k)*NormCol(k,k,Dimenshion);
     Matrics(k,k)+=Tmp;
     QDiag[k]=Tmp*Matrics(k,k);
     MDiag[k]=-MaxEl*Tmp;
     for (i=k+1;i<=Dimenshion;i++)
       {
        Sum=0;
        for (int m=k;i<=Dimenshion;i++) Sum+=Matrics(m,k)*Matrics(m,i);
        Sum/=QDiag[k];
        for (int m=k;i<=Dimenshion;i++) Matrics(m,i)-=Sum*Matrics(m,k);
       }
    }
  if (Matrics(Dimenshion,Dimenshion)<MathZer) sing=1;
  exi:
  MDiag[Dimenshion]=Matrics(Dimenshion,Dimenshion);
  return sing;
 };
double QRSolve::MaxColEl(int Col,int Down,int Up)
  { double Max=0,Tmp; 
    for (int i=Down;i<=Up;i++) 
      if ( (Tmp=fabs( Matrics(i,Col) )) > Max ) Max=Tmp;
    return Max;
  };
double QRSolve::NormCol(int k,int k,int Dimenshion);
  { double Sum=0; 
    for (int i=Down;i<=Up;i++) Sum+=sqr( Matrics(i,Col) );
    return sqrt(Sum);
  };
double QRSolve::QRSolution();
  { double Sum=0; 
    for (int i=Down;i<=Up;i++) Sum+=sqr( Matrics(i,Col) );
    return sqrt(Sum);
  };




void QRSolve::QRResult()
  {
   Low(k,k)=coef;Up(k,k)=1;
   coef=1/coef;
   double s;
   for (int i=k+1;i<=Dimenshion;i++)
    {
     s=Matrics(k,i);
     for (int m=1;m<k;m++) s-=Low(k,m)*Up(m,i);
     Up(k,i)=s*coef;
     s=Matrics(i,k);
     for (m=1;m<k;m++) s-=Low(i,m)*Up(m,k);
     Low(i,k)=s;
    }
  };
double QRSolve::PivotEl(int k)
  {
   int Leader=0;
   double val=0,s;
//   for (int i=k;i<=k;i++)
   for (int i=k;i<=Dimenshion;i++)
     {
      s=Matrics(i,k);
      for (int m=1;m<k;m++) s-=Low(i,m)*Up(m,k);
      if ( fabs(val)<fabs(s)) {val=s;Leader=i;}
     }
//cout<<"Leader["<<Leader<<"]="<<val<<"\n";
   if ( (Leader!=0) && (Leader!=k) )
     {
      Change(Matrics,k,Leader);   
      Change(Low,k,Leader);   
      ExchVar(&PermuteRes[k],&PermuteRes[Leader],sizeof(int));
     }
   return val;
  };
void QRSolve::Change(MatrCl &matr,int i,int j)
  {
   double *get_i=matr.GetRow(i);
   double *get_j=matr.GetRow(j);

   matr.SetRow(i,get_j); matr.SetRow(j,get_i);
   delete get_i;delete get_j;
  };
int QRSolve::LU_Solve()
  {
   int error=1;
   MatrCl TmpM(Dimenshion);
   TmpM=Low*Up;
   PermuteConstants();
   double *PartialSolution=new double[Dimenshion+1];
   PartialSolution[0]=Dimenshion;
//for (int i=1;i<=Dimenshion;i++) { cout<<Constants[i]<<"  ";}; cout<<"\n";
//cout<<"Low matr:\n "<<Low;
//cout<<"\nUp Matr:\n "<<Up;
//cout<<"\nThere mult\n"<<TmpM<<"\n";
   double s,tmp;
   int n,k;
   for (n=1;n<=Dimenshion;n++)
     {  
      s=0; 
      for (int k=1;k<n;k++) s+=PartialSolution[k]*Low(n,k);
      tmp=Low(k,k);
      if (fabs(tmp)<MathZer) goto exi;
      PartialSolution[n]=(Constants[n]-s)/tmp;
     }
   for (n=Dimenshion;n>=1;n--)
     {
      s=0;
      for (k=n+1;k<=Dimenshion;k++) s+=Results[k]*Up(n,k);
      Results[n]=(PartialSolution[n]-s);
     }
   error=0;
   exi:
   delete PartialSolution;
   return error;
  };
void QRSolve::PermuteConstants()
  {
   double *TmpConstants=new double[Dimenshion+1];TmpConstants[0]=Dimenshion;
   for (int i=1;i<=Dimenshion;i++) TmpConstants[i]=Constants[PermuteRes[i]];
   movmem(TmpConstants,Constants,sizeof(double)*(Dimenshion+1));
   delete TmpConstants;
  };
void QRSolve::DisposeInter()
  { delete Constants;delete PermuteRes;};









*/
