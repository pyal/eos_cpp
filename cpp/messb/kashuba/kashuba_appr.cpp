#include <lib/precompiled/Messb.h>

#include "kashuba_appr.h"


RndAngleGenerator_Kashuba::RndAngleGenerator_Kashuba(
    double phi0,
    int n,
    double mainharm,
    int furiediscr,
    double fulltime,
    double strongshort)
    : Phi0(phi0),
      MainHarm(mainharm),
      FurieDiscr(furiediscr),
      FullTime(fulltime),
      StrongShort(strongshort),
      N(n){};

void RndAngleGenerator_Kashuba::Rnd(VecCl &Rnd_Angle, VecCl &Rnd_Time) {
    VecCl ph(N);
    double gZero = 0, gSigma = 1;
    RndGaus gaus(gZero, gSigma);
    gaus.Init(-4 * gSigma, 4 * gSigma, 100, 1e-5);

    int NumJ = FurieDiscr, j, k;
    VecCl Phi_cos(NumJ), Phi_sin(NumJ);
    for(k = 1; k <= NumJ; k++) {
        Phi_cos[k] = gaus.Rnd();
        Phi_sin[k] = gaus.Rnd();
    }
    double StrongCoef = StrongShort * sqr(2 * M_PI / FullTime);
    double MainHarm_ = MainHarm * 2 / FullTime;
    Phi_cos = Phi_cos * MainHarm_;
    Phi_sin = Phi_sin * MainHarm_;

    VecCl CosIJ(N), SinIJ(N);
    double coef = M_PI / N;
    for(k = 1; k <= N; k++) {
        CosIJ[k] = cos(coef * (k - 1));
        SinIJ[k] = sin(coef * (k - 1));
    }
    for(k = 1; k <= N; k++) {
        ph[k] = Phi0;
        for(j = 1; j <= NumJ; j++) {
            int r = k * (j - 1) / N;
            double SCCoef = 1;
            if((r / 2) * 2 != r)
                SCCoef = -1;
            r = k * (j - 1) - r * N + 1;   //if (r==0) r=N;
            ph[k] += (SCCoef * Phi_cos[j] * CosIJ[r] + SCCoef * Phi_sin[j] * SinIJ[r]) /
                     sqrt(max<double>(j - 1, 0.25) * (1 + sqr(j) * StrongCoef));
        }
    }
    Rnd_Angle = ph;
    Rnd_Time = VecCl::MakeStepVector(N, 0, FullTime, 0);
};


//
////  =======================================================
////  ============= Polycristall case rotating lines ========
////  =================   Kashubas Variant  =================
////  =======================================================
//double HQsInt_Rotate(double *Intencity,double *Position,double Wid,
//					 double T,double Tbkt,double A,double W_coef,double x)
//  {
//   double ZerPos=0;for (int k=1;k<=8;k++) ZerPos+=Position[k];ZerPos/=8;
//   double Res=0;
//   double K=Wid/(2*M_PI);
//   for (k=1;k<=8;k++)
//      {
//	   double Sw=x-Position[k];
//	   double Mul=1;if (ZerPos-Position[k]>0) Mul=-1;
//	   double Wc=Wid*0.5+W_coef*fabs(ZerPos-Position[k])*atan(M_PI*T/(2*(8*Tbkt-T)));
//	   Res+=Intencity[k]*(1-Sw*A*Mul/Wid)/(sqr(Wc)+sqr(Sw));
//	  }
//
//   return Res*K;
//  };
//
//void HQsInt_Rotate(double *Intencity,double *Position,double Wid,
//				   double T,double Tbkt,double A,double W_coef,double *x,double *y)
//  {
//   double ZerPos=0;for (int k=1;k<=8;k++) ZerPos+=Position[k];ZerPos/=8;
//   double s,p,W=sqr(Wid/2);
//   double K=Wid/(2*M_PI);
////VecCl Tmp;Tmp.Ptr=Position;cout<<" Pos "<<Tmp;Tmp.Ptr=Intencity;cout<<"\nInt "<<Tmp<<"\n";Tmp.Ptr=NULL;
//   for (k=1;k<=x[0];k++)
//     {
//      p=x[k];s=0;
//      for (int k1=1;k1<=8;k1++) //s+=Intencity[k1]/(W+sqr(Position[k1]-p));
//      {
//	   double Sw=p-Position[k1];
//	   double Mul=1;if (ZerPos-Position[k1]>0) Mul=-1;
//	   double Wc=Wid*0.5+W_coef*fabs(ZerPos-Position[k1])*atan(M_PI*T/(2*(8*Tbkt-T)));
//	   s+=Intencity[k1]*(1-Sw*A*Mul/Wid)/(sqr(Wc)+sqr(Sw));
//	  }
//      y[k]=s*K;
//     }
//  };
//
