#include <lib/precompiled/math.h>


#include "mtrnd.h"
//#include "mtquad.h"
#include "dat_mani.h"


RndIntegral *RndIntegral::CurRndStruct=NULL;

RndIntegral* RndIntegral::Init(double x_low,double x_up,int NumInt,double Misf)
 {

  if (x_up<x_low) {double tmp=x_low;x_low=x_up;x_up=tmp;}
  YNorm=1;
  RndIntegral *oldRnd=CurRndStruct;CurRndStruct=this;

//  YNorm=100/RecInt(RndIntegral::xFunc,x_low,x_up,Misf);Misfit=StndErr*0.1;
  int IntErr,IntNumInt=10000;double IntResMis;
  YNorm=100/Qags_IntSubInt(RndIntegral::xFunc,x_low,x_up,Misf,Misf,IntNumInt,IntResMis,IntErr);
  Misfit=StndErr*0.1;
//double Qags_IntSubInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
//                  int NumSubInt,double &ResErr,int &Err);

  TDataF<real> dat;
  int i[2]={NumInt+1,NumInt+1};dat.SetDim(2,&i[0]);
  double Stp2=(x_up-x_low)/NumInt/2,Cur=x_low+Stp2,S=0;
  dat.D[0][0]=Cur;dat.D[1][0]=0;
  for (int k=1;k<dat.I[0];k++)
   {
     double tmp=Qags_IntSubInt(RndIntegral::xFunc,Cur-Stp2,Cur+Stp2,Misf/(NumInt+5),Misf/(NumInt+5),IntNumInt,IntResMis,IntErr);
//	  S+=max<double>(RecInt(RndIntegral::xFunc,Cur-Stp2,Cur+Stp2,Misf/(NumInt+5)),Misf/(NumInt+5));
	  S+=max<double>(tmp,Misf/(NumInt+5));
    dat.D[0][k]=Cur;
    dat.D[1][k]=S;
    Cur+=Stp2*2;
   }
  int NumSpl=NumInt;
  if (Int.Generate(NumSpl,Misf,0,dat,1,0)==10) 
  {
	  cout<<" RndFunc initial Spl data \n";
	  double *tmp=dat.D[0];dat.D[0]=dat.D[1];dat.D[1]=tmp;
      OutputTDataF<real>(cout,dat);cout.flush();
  }
//  Norm=S-Stp2*0.01;
  Norm=S;//-Stp2*0.01;
//for (double cx=1e-5;cx<Norm;cx+=Norm/300) cout<<cx<<" "<<Int.Evaluate(cx)<<"\n";

//  srand( (unsigned)time( NULL ) );
  CurRndStruct=oldRnd;
  return this;
 }

/*
//static double GausDistribSigma=1;
double GausDistribSigma=1;
double GausDistribZero=0;
double AllNormCoef=1;

double GausDistrib(double x)
 {
//cout<<x<<FMT<<exp(-sqr((x-GausDistribZero)/GausDistribSigma)/2)/sqrt(2*M_PI)/GausDistribSigma<<"\n";
  return AllNormCoef*exp(-sqr((x-GausDistribZero)/GausDistribSigma)/2)/sqrt(2*M_PI)/GausDistribSigma;
 };

// Probability  of state exp(-H/T)=exp(-BoltsmanDistribCoef*sin(x)^2)
double BoltsmanDistribCoef=1;
double BoltsmanDistrib(double x)
 {
  double r;
  if ( (r=BoltsmanDistribCoef*sqr(sin(x*M_PI/180)))>40) return 0;
  return AllNormCoef*exp(-r);
 };



void SetRndDistrib(X_func Func,double x_low,double x_up,int NumInt,double Misf,
                   DistribStruct &Distrib)
 {

  if (x_up<x_low) {double tmp=x_low;x_low=x_up;x_up=tmp;}
  AllNormCoef=1;
  AllNormCoef=100/RecInt(Func,x_low,x_up,Misf);Distrib.YNorm=AllNormCoef;Misf=StndErr*0.1;
  TDataF<real> dat;
  int i[2]={NumInt+1,NumInt+1};dat.SetDim(2,&i[0]);
  double Stp2=(x_up-x_low)/NumInt/2,Cur=x_low+Stp2,S=0;
//  dat.D[0][1]=Cur;dat.D[1][1]=0;
  dat.D[0][0]=Cur;dat.D[1][0]=0;
//  for (int k=2;k<=dat.I[0];k++)
  for (int k=1;k<dat.I[0];k++)
   {
//    S+=max<double>(RecInt(Func,Cur,Cur+Stp,Misf/(NumInt+5)),1e-7);
//    S+=max<double>(RecInt(Func,Cur,Cur+Stp,Misf/(NumInt+5)),sqrt(MathZer));
    S+=max<double>(RecInt(Func,Cur-Stp2,Cur+Stp2,Misf/(NumInt+5)),Misf*0.0001);
//    S+=RecInt(Func,Cur,Cur+Stp,Misf/(NumInt+5)); //WWRROONNGG  NNOOWW
    dat.D[0][k]=Cur;
    dat.D[1][k]=S;
//cout<<dat.D[1][k]<<"  "<<dat.D[0][k]<<"\n";
    Cur+=Stp2*2;
   }
//  delete Distrib;Distrib=new CurveSpline;
//dat.SaveIni(cout);
int NumSpl=NumInt;
  Distrib.Int.Generate(NumSpl,Misf,0,dat,1,0);
  Distrib.Norm=S-Stp2*0.01;

  srand( (unsigned)time( NULL ) );
//  delete dat;
//for (k=1;k<200;k++) cout<<FM<<k*S/200<<FMT<<Distrib.Int.Evaluate(k*S/200)<<"\n";
 }
//static double CurRnd=0;
double FindRndVal(DistribStruct &Distrib)
 {
  double ret=double(rand())/RAND_MAX*Distrib.Norm;
//  ret=CurRnd;CurRnd+=0.001;
//cout<<ret<<" ";
  ret=Distrib.Int.Evaluate(ret);///Distrib.YNorm;
  
//cout<<ret<<"\n";

  return ret;
 };

*/