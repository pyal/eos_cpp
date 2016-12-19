#include <lib\precompiled\Messb.h>
// This program generates an array ph(2048) of random magnetic fields.
// r(250) is used in the random numbers generator
// JT is the physical ratio J/T
#include "kashub.h"
//DistribStruct Distrib;
//void SetGausDistrib()
// {
//  GausDistribZero=0;GausDistribSigma=1;
//  SetRndDistrib(GausDistrib,-4*GausDistribSigma,4*GausDistribSigma,100,1e-5,Distrib);
// }

void MC_StepOld(VecCl &ph,double Phi0,int N,double MainHarm,int FurieDiscr,
             double FullTime,double StrongShort)
 {
  double gZero=0,gSigma=1;RndGaus gaus(gZero,gSigma);
  gaus.Init(-4*gSigma,4*gSigma,100,1e-5);
  int NumJ=FurieDiscr;
  VecCl Phi_cos(NumJ),Phi_sin(NumJ);
  for (int k=1;k<=NumJ;k++) 
   {Phi_cos[k]=gaus.Rnd();Phi_sin[k]=gaus.Rnd();}
  double StrongCoef=StrongShort*sqr(2*M_PI/FullTime);
  MainHarm*=2/FullTime;
  Phi_cos=Phi_cos*MainHarm;Phi_sin=Phi_sin*MainHarm;
  for (int k=1;k<=N;k++) 
   {
    ph[k]=Phi0;
    for (int j=1;j<=NumJ;j++) 
      ph[k]+=(Phi_cos[j]*cos(M_PI*k*(j-1)/N)+Phi_sin[j]*sin(M_PI*k*(j-1)/N))/(sqrt(max<double>(j-1,0.25))*(1+sqr(j)*StrongCoef));
   }

 };
    
void MC_Step(VecCl &ph,double Phi0,int N,double MainHarm,int FurieDiscr,
             double FullTime,double StrongShort)
 {
  double gZero=0,gSigma=1;RndGaus gaus(gZero,gSigma);
  gaus.Init(-4*gSigma,4*gSigma,100,1e-5);

  int NumJ=FurieDiscr,j,k;
  VecCl Phi_cos(NumJ),Phi_sin(NumJ);
  for ( k=1;k<=NumJ;k++) 
   {Phi_cos[k]=gaus.Rnd();Phi_sin[k]=gaus.Rnd();}
  double StrongCoef=StrongShort*sqr(2*M_PI/FullTime);
  MainHarm*=2/FullTime;
  Phi_cos=Phi_cos*MainHarm;Phi_sin=Phi_sin*MainHarm;

  VecCl CosIJ(N),SinIJ(N);
  double coef=M_PI/N;
  for (k=1;k<=N;k++) 
     {CosIJ[k]=cos(coef*(k-1));SinIJ[k]=sin(coef*(k-1));}
  for (k=1;k<=N;k++) 
   {
    ph[k]=Phi0;
    for ( j=1;j<=NumJ;j++) 
     {
      int r=k*(j-1)/N;
      double SCCoef=1;if ((r/2)*2!=r) SCCoef=-1;
      r=k*(j-1)-r*N+1;//if (r==0) r=N;
      ph[k]+=(SCCoef*Phi_cos[j]*CosIJ[r]+SCCoef*Phi_sin[j]*SinIJ[r])/
                                         sqrt(max<double>(j-1,0.25)*(1+sqr(j)*StrongCoef));
     }
   }
 };




//
//
//RndAngleGenerator_Kashuba::RndAngleGenerator_Kashuba
//    (double phi0,int n,double mainharm,double furiediscr,
//    double fulltime,double strongshort):Phi0(phi0),MainHarm(mainharm),
//    FurieDiscr(furiediscr),FullTime(fulltime),StrongShort(strongshort),N(n){};
//
//void RndAngleGenerator_Kashuba::Rnd(VecCl &Rnd_Angle,VecCl &Rnd_Time)
//{
//   VecCl ph(N);
//  double gZero=0,gSigma=1;RndGaus gaus(gZero,gSigma);
//  gaus.Init(-4*gSigma,4*gSigma,100,1e-5);
//
//  int NumJ=FurieDiscr,j,k;
//  VecCl Phi_cos(NumJ),Phi_sin(NumJ);
//  for ( k=1;k<=NumJ;k++) 
//   {Phi_cos[k]=gaus.Rnd();Phi_sin[k]=gaus.Rnd();}
//  double StrongCoef=StrongShort*sqr(2*M_PI/FullTime);
//  MainHarm*=2/FullTime;
//  Phi_cos=Phi_cos*MainHarm;Phi_sin=Phi_sin*MainHarm;
//
//  VecCl CosIJ(N),SinIJ(N);
//  double coef=M_PI/N;
//  for (k=1;k<=N;k++) 
//     {CosIJ[k]=cos(coef*(k-1));SinIJ[k]=sin(coef*(k-1));}
//  for (k=1;k<=N;k++) 
//   {
//    ph[k]=Phi0;
//    for ( j=1;j<=NumJ;j++) 
//     {
//      int r=k*(j-1)/N;
//      double SCCoef=1;if ((r/2)*2!=r) SCCoef=-1;
//      r=k*(j-1)-r*N+1;//if (r==0) r=N;
//      ph[k]+=(SCCoef*Phi_cos[j]*CosIJ[r]+SCCoef*Phi_sin[j]*SinIJ[r])/
//                                         sqrt(max<double>(j-1,0.25)*(1+sqr(j)*StrongCoef));
//     }
//   }
//   Rnd_Angle=ph;Rnd_Time=VecCl::MakeStepVector(N,0,FullTime,0);
//
// };
//




















/*
  double ps,w,Pi=M_PI;
  int len=1024,len2=2048,i,j,k;
  MatrCl c(len2,len),s(len2,len)
  Pi*=2;
  for (i=1;i<=len2;i++)
   {
    for (j=1;j<=len;j++)
     {
      c(i,j)=cos( (i-0.5)*Pi/len2*(j-0.5) )
      s(i,j)=sin( (i-0.5)*Pi/len2*(j-0.5) )
     }
   }
  w=1/pow(2.,30);
  for (i=1;i<=len;i++)
   {
// !-------Generates random number ps according to the gaussian distribution
    ps=0;
    for (j=1;j<=2;j++)
     {
      for (k=1;k<=103;k++)
       {
        r[k]=IEOR(r[k],r[k+147]);
        ps+=r[k]*w-1;
       }
     }
    ps/=sqrt(500);
    phk_up=ps*sqrt(len2*3/(4*Pi*(i-0.5)*JT);  // !----random Fourier components
//!-------Generates random number ps according to the gaussian distribution
    ps=0;
    for (j=1;j<=2;j++)
     {
      for (k=1;k<=103;k++)
       {
        r[k]=IEOR(r[k],r[k+147]);
        ps+=r[k]*w-1;
       }
      for (k=104;k<=250;k++)
       {
        r[k]=IEOR(r[k],r[k-103]);
        ps+=r[k]*w-1;
       }
     }
    ps/=sqrt(500);
    phk_do=ps*sqrt(len2*3/(4*Pi*(i-0.5)*JT);  // !----random Fourier components
   }

  for (i=1;i<=len2;i++)
   {
    ph[i]=0;
    for (j=1;j<=len;j++)
      ph[i]+=phk_up[j]*c(i,j)+phk_do[j]*s(i,j);
    ph[i]*=sqrt(1/len);
   }
 };
*/
/*
Subroutine MC_Step(ph,JT,r)
Real*8 ph(2048),JT,phk(-2048:2048),ps,w,Pi
Real*8 c(2048,1024),s(2048,1024)
Integer i,j,len
Integer*4 r(250)
len=2048
Pi=datan(1.d0)*4.d0
Do i=1,len
Do j=1,len/2
c(i,j)=dcos((i-5.d-1)*Pi/len*(j-5.d-1))
s(i,j)=dsin((i-5.d-1)*Pi/len*(j-5.d-1))
Enddo
Enddo
w=1.d0/2.d0**30
Do i=1,len/2
ps=0.d0
Do j=1,2
Do k=1,103
r(k)=ieor(r(k),r(k+147))
ps=ps+r(k)*w-1.d0
Enddo
Do k=104,250
r(k)=ieor(r(k),r(k-103))
ps=ps+r(k)*w-1.d0
Enddo
Enddo
ps=ps/dsqrt(5.d2)
!-------------End
phk(i)=ps*dsqrt(len*3.d0/(4.d0*Pi*(i-5.d-1)*JT)) !----random Fourier components
!-------Generates random number ps according to the gaussian distribution
ps=0.d0
Do j=1,2
Do k=1,103
r(k)=ieor(r(k),r(k+147))
ps=ps+r(k)*w-1.d0
Enddo
Do k=104,250
r(k)=ieor(r(k),r(k-103))
ps=ps+r(k)*w-1.d0
Enddo
Enddo
ps=ps/dsqrt(5.d2)
!------------End
phk(-i)=ps*dsqrt(len*3.d0/(4.d0*Pi*(i-5.d-1)*JT)) !----random Fourier components 
Enddo

Do i=1,len
ph(i)=0.d0
Do j=1,len/2
ph(i)=ph(i)+phk(j)*c(i,j)+phk(-j)*s(i,j)
Enddo
ph(i)=ph(i)*dsqrt(2.d0/len)
Enddo
Return
End
*/