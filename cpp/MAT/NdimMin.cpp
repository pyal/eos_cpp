#include <lib\precompiled\math.h>



#include "ndimmin.h"


int MinFind1DClass::CheckBound(double &x,double Stp)
{
  if (fabs(x)<MinDouble) return 1;
  //if (fabs(x)>fabs(Stp*StpOverGo)) x=x/fabs(x)*fabs(Stp*StpOverGo);
  return (x*Stp>0);
}
int MinFind1DClass::GoodVec(vector<IndexedVal> &vec)
{
  sort(vec.begin(),vec.end());
  if ( (vec[1].ValInd()<=vec[0].ValInd()) && (vec[1].ValInd()<=vec[2].ValInd()) ) return 1;
  return 0;
}
int MinFind1DClass::ConstVec(vector<IndexedVal> &vec)
{
  //double d1=(vec[1].ValInd());
  //  if ( fabs(d1-vec[0].ValInd())+
  //      fabs(d1-vec[2].ValInd()) <MinStep*max(fabs(d1),MinDouble) ) return 1;
  double d1=(vec[1].ValInd());
    if ( fabs(d1-vec[0].ValInd())+
//      fabs(d1-vec[2].ValInd()) <FuncError*max(fabs(d1),MinDouble) ) return 1;
      fabs(d1-vec[2].ValInd()) <FuncError) return 1;
  return 0;
}

double MinFind1DClass::CheckDifficult(double &StrtMisf,vector<IndexedVal> &vec)
{
//  double x4;
  vector<IndexedVal> vec1=vec;
  if (ZeroStp(vec[2].ValCmp(),vec[0].ValCmp()))
    {for (int k=0;k<(int)vec.size();k++) vec1[k]=IndexedVal(vec[k].ValCmp(),vec[k].ValInd());
    sort(vec1.begin(),vec1.end());StrtMisf=vec1[0].ValCmp();return vec1[0].ValInd();
    }
  double x4m=(vec[0].ValCmp()+vec[1].ValCmp())*0.5;
  double y4m=Func(x4m);
  vec1[2]=IndexedVal(y4m,x4m);
  if (GoodVec(vec1)) return ThreePointEst(StrtMisf,vec1);
  vec1=vec;
  double x4p=(vec[2].ValCmp()+vec[1].ValCmp())*0.5;
  double y4p=Func(x4p);
  vec1[0]=IndexedVal(y4p,x4p);
  if (GoodVec(vec1)) return ThreePointEst(StrtMisf,vec1);
  vec1[2]=IndexedVal(y4p,x4p);vec1[0]=IndexedVal(y4m,x4m);
  vec1[1]=vec[1];
  if (!GoodVec(vec1)) {cout<<"Very bad?\n";StrtMisf=vec[1].ValInd();return vec[1].ValCmp();}
  else return ThreePointEst(StrtMisf,vec1);
}
double MinFind1DClass::ThreePointEst(double &StrtMisf,vector<IndexedVal> &vec)
{
  double x4;
  if (ZeroStp(vec[2].ValCmp(),vec[0].ValCmp()))
  {StrtMisf=vec[1].ValInd();return vec[1].ValCmp();}
  x4=FixedMin(vec[0].ValCmp(),vec[1].ValCmp(),vec[2].ValCmp(),
              vec[0].ValInd(),vec[1].ValInd(),vec[2].ValInd());
  if ( (x4<vec[0].ValCmp()) || (x4>vec[2].ValCmp()) ) return CheckDifficult(StrtMisf,vec);
//    if (stp very small...
  double y4=Func(x4);
  if (y4>vec[1].ValInd()) return CheckDifficult(StrtMisf,vec);
  StrtMisf=y4;return x4;
}
double MinFind1DClass::LinMin(double StrtStpEst,double StrtPnt,double &StrtMisf)
{
  double Stp=(StrtStpEst),Pos=StrtPnt,Coef=LinearMulCoef;
  double y0=StrtMisf;
  vector<IndexedVal> vec(3);vec[0]=IndexedVal(y0,Pos);
  vec[1]=IndexedVal(Func(Pos+Stp),Pos+Stp);
  if (vec[1].ValInd()-vec[0].ValInd()>0) Pos=Pos-Stp;
  else Pos+=2*Stp;
  vec[2]=IndexedVal(Func(Pos),Pos);
  sort(vec.begin(),vec.end());
  double dir=1;
  int Ind=0;
  if (vec[1].ValInd()>vec[0].ValInd()) {dir=-1;Pos=vec[0].ValCmp();Ind=2;}
  else Pos=vec[2].ValCmp();
  for (int k=1;k<=MaxLinIter;k++)
  {
    if (GoodVec(vec)) return ThreePointEst(StrtMisf,vec);
    if (ConstVec(vec)) break;
    Stp=fabs(Stp*Coef)*dir;
    Pos+=Stp;
    vec[Ind]=IndexedVal(Func(Pos),Pos);
  }
  if (GoodVec(vec)) return ThreePointEst(StrtMisf,vec);
  if (Ind==0) Ind=2;else Ind=0;
  StrtMisf=vec[Ind].ValInd();
  return vec[Ind].ValCmp();
  
}

// Derivative is zero for the point x=
//                                              2                2               2
//                                 (-y3 + y2) x1  + (-y1 + y3) x2  + (y1 - y2) x3
//                             1/2 -----------------------------------------------
//                                  (-y3 + y2) x1 + (-y1 + y3) x2 + (y1 - y2) x3
double MinFind1DClass::FixedMin(double x1,double x2,double x3,double y1,double y2,double y3)
{
  double denom=(-y3 + y2)*x1 + (-y1 + y3)*x2 + (y1 - y2)*x3;
  if (fabs(denom)<MinDouble) denom=(denom<0)?-MinDouble:MinDouble;
  return 0.5*((-y3 + y2)*sqr(x1)  + (-y1 + y3)*sqr(x2)  + (y1 - y2)*sqr(x3) )/denom;
}


void Derive2DClass::Execute(VecCl &Par,VecCl &Stp,MatrCl &D2,VecCl &D1,double &D0)
{
  MatrCl F2(Par.Dim());
  VecCl F1(Par.Dim()),p=Par,s=Stp*0.01;
  D2.Dim(Par.Dim());D1.Dim(Par.Dim());D0=Execute(p);
  for (int k=1;k<=Par.Dim();k++)
  {
    p[k]+=s[k];F1[k]=Execute(p);
    for (int k1=k;k1<=Par.Dim();k1++)
    {
      p[k1]+=s[k1];F2(k,k1)=Execute(p);
      if (k!=k1) p[k1]=Par[k1];else p[k1]=Par[k1]+s[k1];
    }
    p[k]=Par[k];
    D1[k]=(F1[k]-D0)/s[k];
  }
  for (int k=1;k<=Par.Dim();k++)
  {
    for (int k1=k;k1<=Par.Dim();k1++)
    {D2(k1,k)=(F2(k,k1)-F1[k1]-F1[k]+D0)/(s[k]*s[k1]);D2(k,k1)=D2(k1,k);}
  }
};
void Derive2DClass::Execute(VecCl &Par,VecCl &Stp,VecCl &D1,double &D0)
{
  D0=Execute(Par);
  VecCl p;p=Par;
  for (int k=1;k<=Par[0];k++)
  {
    double dx=Stp[k]*0.01;
    p[k]+=dx;D1[k]=(Execute(p)-D0)/dx;
    p[k]=Par[k];
  }
};
void Derive2DHi2::Execute(VecCl &Par,VecCl &Stp,MatrCl &D2,VecCl &D1,double &D0)
{
  MatrCl Mat(Par.Dim(),Y0.Dim());
  D2.Dim(Par.Dim());D1.Dim(Par.Dim());
  VecCl Res,Y,p;p=Par;Func(p,Y);D0=Hi(Y);
  for (int k=1;k<=Par.Dim();k++)
  {
    p[k]+=Stp[k]*0.01;Func(p,Res);
    MatrCl::SetRow(Mat,k,Res);
    p[k]=Par[k];
  }
  for (int i=1;i<=Par.Dim();i++)
  {
    for (int j=1;j<i;j++)
    {
      double s=0;
      for (int k=1;k<=Y0.Dim();k++) 
        s+=Weight[k]*(Mat(i,k)-Y[k])*(Mat(j,k)-Y[k]);
      s*=20000/(Stp[i]*Stp[j]);
      D2(i,j)=s;D2(j,i)=s;
    }
    double s=0,sd=0,mul;
    for (int k=1;k<=Y0.Dim();k++) 
    {mul=Mat(i,k)-Y[k];sd+=mul*Weight[k]*(Y0[k]-Y[k]);s+=sqr(mul)*Weight[k];}
    s*=20000/sqr(Stp[i]);sd*=200/Stp[i];
    D2(i,i)=s;D1[i]=sd;
  }
};
double Derive2DHi2::Hi(VecCl &Res)
{
  double s=0;
  for (int k=1;k<=Res.Dim();k++) s+=Weight[k]*sqr(Y0[k]-Res[k]);
  return s;
}

VecCl GetParamsCorrelation::ParamsCorrD2(MatrCl &D2)
{
  int N=D2.Dim();
  double d;
  MatrCl StpDir(N),ND2;
  VecCl EigV(N),ParMis(N),Norm(N);ParMis=ParMis*0;
  double MatrZer=ZeroEig;
MatrZer=M_MinDouble2;
  for (int k=1;k<=N;k++) 
  {
    if ((d=D2(k,k))>MatrZer) Norm[k]=1/sqrt(d);else Norm[k]=0;
  }
  ND2=VectorNormalizeMatr(Norm,D2,Norm);
  ReduceLQ(ND2,StpDir,EigV.Ptr,ErrorLQ);
  int Sum=0;
  for (int k=1;k<=N;k++) if (EigV[k]>MatrZer) 
  {
    Sum++;
    VecCl tmp=MatrCl::GetRow(StpDir,k);
    double c=sqrt(1/EigV[k]);
    for (int k1=1;k1<=N;k1++) ParMis[k1]+=fabs(c*tmp[k1]*(Norm[k1]));
  }
  if (Sum>0) ParMis=ParMis*(1./Sum);
  return ParMis;
};
double GetParamsCorrelation::ParamsCorrelation(VecCl &Par,VecCl &Stp,VecCl &ParMis)
{
  VecCl StpV,D1,EigV(Par.Dim());StpV=Stp;ParMis=Par*0;
  StpV=Stp*0+MathEps05*100;
  double FoundMisf;
  if (Par.Dim()==0) {FoundMisf=Deriv2->Execute(Par);return 0;}
  MatrCl D2;Deriv2->Execute(Par,StpV,D2,D1,FoundMisf);
  ParMis=ParamsCorrD2(D2);
  return FoundMisf;
};

struct FormMinDirections
{
  MatrCl D2;
  VecCl D1;
VecCl StpGain;
  double D0,ErrorLQ,MathZer05,ZeroMatrVal;
  double Lambda;int Method;enum  MethodType {NoLambda,StdLambda,OptimLambda,UnitNorm} ;// Not used now
  FormMinDirections(MatrCl &d2,VecCl &d1,double d0,double errorlq,double mathzer05,
                    double zeromatrval) :D2(d2),D1(d1),D0(d0),Method(NoLambda),
            ErrorLQ(errorlq),MathZer05(mathzer05),ZeroMatrVal(zeromatrval),Lambda(0){};
  int Dim(){return D2.Dim();}
//  void SetLambda(double lambda){Lambda=lambda;};

// Gets StpDir - directions  StpEst - step estimations  
// StpOrder    - order in which to make steps
// NormVarC    - Normalizing coefs for variables ( for dirs and steps )
  void Execute(VecCl &StpEst,VecCl &NormVarC,double lambda,int Meth)
  {
    MatrCl StpDir;
    VecCl StpOrder;
    Lambda=lambda;Method=Meth;
    Execute(StpDir,StpEst,StpOrder,NormVarC);
    VecCl Res(Dim());Res=Res*0;
    for (int k=1;k<=Dim();k++) Res=Res+MatrCl::GetCol(StpDir,k)*StpEst[k];
    StpEst=Res;
    Method=NoLambda;Lambda=0;
  }
  void Execute(MatrCl &StpDir,VecCl &StpEst,VecCl &StpOrder,VecCl &NormVarC)
  {
    StpEst.Dim(Dim());StpOrder.Dim(Dim());
    VecCl EigV(Dim()),Edelta(Dim());
StpGain.Dim(Dim());
    NormalSolve_Unstable(StpDir,EigV,Edelta,NormVarC);
    //NormalSolve_Symm(D2,D1,StpDir,EigV,Edelta,ErrorLQ);
    vector<IndexedVal> vec;vec.reserve(Edelta.Dim());
    //vector<IndexedVal> vec(Edelta.Dim());
//cout<<"Edelta\n"<<Edelta<<"EigV\n"<<EigV<<"D0 "<<D0<<"\n";
    for (int k=1;k<=Dim();k++) 
    {
        double tmp = fabs(Edelta[k]*D0)>MathZer?fabs(sqr(Edelta[k])/(max(fabs(EigV[k]),MathZer05)*4*D0)):0;
      if ((tmp>MathZer05) && (fabs(EigV[k])>ErrorLQ)) 
                      {tmp=tmp*fabs(D0);StpEst[k]=Edelta[k]/EigV[k];}
      else {StpEst[k]=0;tmp=0;}
      vec.push_back(IndexedVal(k,-tmp));
StpGain[k]=tmp;
    }
    sort( vec.begin( ), vec.end( ) );
    for (int k=1;k<=Dim();k++) StpOrder[k]=vec[k-1].ValInd();
  };

private:
  void SetNormMatr(const MatrCl &D2l,const VecCl &D1l,VecCl &Norml,MatrCl &ND2l)
  {
    Norml.SetDim(Dim());
    double d;
    for (int k=1;k<=Dim();k++) 
      if ((d=fabs(D1l[k]))>MathZer05) Norml[k]=2/d;else Norml[k]=0;
    if (Method==UnitNorm) Norml=Norml*0+1;
    if (Method==StdLambda)
    {
      ND2l=D2l;for (int k=1;k<=Dim();k++) ND2l(k,k)*=(1+Lambda);
      ND2l=VectorNormalizeMatr(Norml,ND2l,Norml);
    }
    else
    {
      ND2l=VectorNormalizeMatr(Norml,D2l,Norml);
      if (Method==OptimLambda) {for (int k=1;k<=Dim();k++) ND2l(k,k)*=(1+Lambda);}
    }
    //{
    //  ND2l=VectorNormalizeMatr(Norml,D2l,Norml);
    //  if (Method!=NoLambda) {for (int k=1;k<=Dim();k++) ND2l(k,k)*=(1+Lambda);}
    //}
  }
// Try to make strict solution of the problem
  void NormalSolve_Unstable(MatrCl &StpDir,VecCl &EigV,VecCl &Edelta,VecCl &NormVarC)
  {
    MatrCl ND2;StpDir.Dim(Dim());EigV.Dim(Dim());
    VecCl Norm;
    SetNormMatr(D2,D1,Norm,ND2);

    VecCl ND1;ND1=Cvt2Coord(D1,Norm);
    if (!ReduceLQ(ND2,StpDir,EigV.Ptr,ErrorLQ)) 
          {NormalSolve_Symm(StpDir,EigV,Edelta,NormVarC);return;}
    MatrCl EigM;EigM=EigV;
    if (!GoodMatrAppr(StpDir*EigM*Transpon(StpDir),ND2,ZeroMatrVal))
          {NormalSolve_Symm(StpDir,EigV,Edelta,NormVarC);return;}
    Edelta=ND1*StpDir;
    NormVarC=Norm;
//cout<<" UnStable ";
  }
// Try to make stable but rough solution of the problem
  void NormalSolve_Symm(MatrCl &StpDir,VecCl &EigV,VecCl &Edelta,VecCl &NormVarC)
  {
//cout<<" Stable ";
    MatrCl ND2;StpDir.Dim(Dim());EigV.Dim(Dim());
    VecCl Norm;
    SetNormMatr(D2,D1,Norm,ND2);

    VecCl ND1;ND1=Cvt2Coord(D1,Norm);
    //if (!ReduceLQ(ND2,StpDir,EigV.Ptr,ErrorLQ)) 
    //    {cout<<" Cannot Calculate ReduceLQ. \n";return ;}
    //Edelta=ND1*StpDir;
    //NormVarC=Norm;

    VecCl Norm1(Norm),N1D1;//,Norm2(Norm);
    MatrCl N1D2;
    double V=1,d;//sqrt(MatrNorm(ND2));
//    double V=sqrt(1/MatrNorm(ND2));
    for (int k=1;k<=D2.Dim();k++) 
    {
      //if ((d=ND2(k,k))>sqr(10*MathZer05)) Norm1[k]=V/sqrt(d);else Norm1[k]=0;
      if ((d=ND2(k,k))>MathZer05) Norm1[k]=V/sqrt(d);else Norm1[k]=0;
      //if (fabs(d=Norm1[k])>MathZer05) Norm2[k]=1/(Norm1[k]);else Norm2[k]=0;
    }
    N1D2=VectorNormalizeMatr(Norm1,ND2,Norm1);
    N1D1=Cvt2Coord(ND1,Norm1);
    if (!ReduceLQ(N1D2,StpDir,EigV.Ptr,ErrorLQ)) 
        {cout<<" Cannot Calculate ReduceLQ. \n";return ;}
    Edelta=N1D1*StpDir;
    NormVarC=Cvt2Coord(Norm,Norm1);
  }

  VecCl Cvt2Coord(VecCl &Vec,VecCl &Cvt)
  {
    VecCl ret=Vec;
    for (int k=1;k<=Vec.Dim();k++) ret[k]*=Cvt[k];
    return ret;
  }
  int GoodMatrAppr(MatrCl &m1,MatrCl &m2,double GoodEstMisf)
  { 
//return 0;
return (GoodMatrAppr(m1,m2)>GoodEstMisf)?0:1; }

  double GoodMatrAppr(MatrCl &m1,MatrCl &m2)
  {
    int n=m1.Dim();
    double Mis=0,d;
    for (int k=1;k<=n;k++)
    {
      for (int k1=1;k1<=n;k1++) 
        if ((d=max(fabs(m1(k,k1)),fabs(m2(k,k1))))>M_Eps2)
        //if ((d=max(fabs(m1(k,k1)),fabs(m2(k,k1))))>sqrt(MathZer05))
          Mis=max(Mis,fabs(m1(k,k1)-m2(k,k1))/d);
        else Mis=max(Mis,fabs(m1(k,k1)-m2(k,k1)));
    }
    return Mis;
  }
};

#ifndef WCPP_NET
DescribedClass * Dummy_EigenVectMinim(){return new EigenVectMinim();}
static ClassDesc EigenVectMinim_ClassDesc(typeid(EigenVectMinim),"minimN_eigen",1,"MDMin",Dummy_EigenVectMinim);
#else
static ClassDesc EigenVectMinim_ClassDesc(typeid(EigenVectMinim),"minimN_eigen",1,"MDMin",create<EigenVectMinim>);
#endif
int EigenVectMinim::read_data_state(FilterIn&si){
  char tmp[256];
  si>>tmp>>ErrorLQ>>tmp>>MathEps05>>tmp>>ZeroEig>>tmp>>BreakIterStp;
  MathEps=sqr(MathEps05);
  BadWlk=0;
  return !(!si);
};
int EigenVectMinim::save_data_state( FilterOut&so){
  so<<" ErrorLQ "<<ErrorLQ<<" MathEps05 "<<MathEps05<<" ZeroEig "<<ZeroEig<<
    " BreakIterStp "<<BreakIterStp<<" ";
  return !(!so);
};

EigenVectMinim::EigenVectMinim():Deriv2(NULL),Min1D(NULL),ErrorLQ(1e-8),LambdaCoef(0.01),
                                 ZeroEig(1e-10),MathEps05(M_Eps2),MathEps(M_Eps),BreakIterStp(1.555),BadWlk(0){  };



void EigenVectMinim::FormStpNormDir(MatrCl &D2,VecCl &D1,double D0,MatrCl &StpDir,VecCl &StpEst,VecCl &StpOrder,VecCl &NormVarCoef)
{
//  FormMinDirections dir(D2,D1,D0,ErrorLQ,MathEps05,ZeroMatrVal);
  FormMinDirections dir(D2,D1,D0,ErrorLQ,sqrt(M_MinDouble2),ZeroEig);
  dir.Method=FormMinDirections::OptimLambda;dir.Lambda=LambdaCoef;
  dir.Execute(StpDir,StpEst,StpOrder,NormVarCoef);
StpGain=dir.StpGain;
}

int EigenVectMinim::MakeRndMotion(double &HiStrt,VecCl &ParStrt,MatrCl &StpDir,
                                   VecCl &StpEst,VecCl &NormVarCoef,int NumIter)
{
  //if (2*(int(BadWlk)/2)!=BadWlk) {BadWlk++;return;}
  //if (BadWlk!=0) NumIter=1;
  NumIter=int(double(NumIter)/(BadWlk+1.));
  double Tstrt=VecCl::ModMax(StpGain)/8,Tend=Tstrt/10,Astrt=0.5,Aend=0.1;
  double HiBest=HiStrt,Hi0=HiStrt;
  VecCl ParBest=ParStrt;
  int NumStp=50;
  Ref<RndIntegral> rndStp=new RndGaus(0.5,2);rndStp->Init(-4,4,100,1e-2);
  RandWalk wlk(Tstrt,Tend,Astrt,Aend,NumStp,Deriv2,rndStp);
  MatrCl StpM=StpDir;for (int k=1;k<=StpM.Dim();k++) MatrCl::SetCol(StpM,k,MatrCl::GetCol(StpDir,k)*StpEst[k]);
      for (int k=1;k<=StpM.Dim();k++) 
      {
        for (int k1=1;k1<=StpM.Dim();k1++) StpM(k,k1)*=NormVarCoef[k];
      }
  //double HiM=HiStrt;VecCl ParM=ParStrt;
//for (int Mean=1;Mean<5;Mean++)
//{

    for (int iter=1;iter<=NumIter;iter++)
    {
      VecCl Par=wlk.Execute(StpM,ParStrt);
      wlk.AStrt/=1.7;wlk.AEnd/=1.7;
      double Hi=Deriv2->Execute(Par);
      if (Hi<HiBest) {HiBest=Hi;ParBest=Par;}//else BadWlk=0;
  HiStrt=HiBest;ParStrt=ParBest;
    }
//}
//if 
  HiStrt=HiBest;ParStrt=ParBest;
//cout<<BadWlk<<" ";
  BadWlk++;
  //RandWalk(double tstrt,double tend,double astrt,double aend,int numstp,
  //  DeriveFuncClass* func,RndFunc* rndstp):TStrt(tstrt),TEnd(tend),AStrt(astrt),
//  VecCl Execute( MatrCl &StpDir, VecCl &Strt)
  return (HiStrt!=Hi0);
};
double EigenVectMinim::MakeRndStp(double &HiStrt,VecCl &Par,VecCl &Stp,double StpEst,double StpGet,
                VecCl &NormVarCoef,int NumIter)
{
  VecCl StpDo=Stp*StpEst;
  for (int k=1;k<=Stp.Dim();k++) StpDo[k]*=NormVarCoef[k];
  double T=StpGet/8;
  Ref<RndIntegral> rndStp=new RndGaus(1,2);rndStp->Init(-2,4,100,1e-2);
  double Xstp,Xres=0;
for (int o=1;o<=NumIter;o++)
{
  for (int k=1;k<=3;k++)
  {
    Xstp=rndStp->Rnd();
    double HiNew=Deriv2->Execute(Par+StpDo*Xstp);
    double Gain=HiNew-HiStrt;
    double Prob=exp(-Gain/T);
//    if ((Gain<0) || (double(rand())/RAND_MAX<Prob))
	if ((Gain<0) || (RandomClass::Rnd(1)<Prob))
      {Par=Par+StpDo*Xstp;HiStrt=HiNew;Xres+=Xstp;break;}
  }
}
  return Xres;
}
int EigenVectMinim::CheckModel(double &HiStrt,MatrCl &StpMatr,VecCl &StpEst,VecCl &Par,VecCl &NormVarCoef)
{
  VecCl Stp=Par*0;
  double Gain=0;for (int k=1;k<=StpGain.Dim();k++) Gain+=StpGain[k];
  for (int k=1;k<=StpGain.Dim();k++) Stp=Stp+MatrCl::GetCol(StpMatr,k)*StpEst[k];
  for (int k=1;k<=Stp.Dim();k++) Stp[k]*=NormVarCoef[k];
  double HiNew=Deriv2->Execute(Par+Stp);
  //double Hi0=Deriv2->Execute(Par);
//cout<<" Gain "<<Gain<<" Res "<<HiStrt-HiNew<<"\n";//<<" Par "<<Par<<" Stp "<<Stp<<" StpEst "<<StpEst<<"\n";
  if (HiStrt-HiNew<0.5*Gain) {LambdaCoef*=20;return 0;}
  HiStrt=HiNew;Par=Par+Stp;LambdaCoef/=2;
  return 1;
//      VecCl Norm=NormVarCoef*0+1;
//      MultiDim2One Func(Deriv2);
//      Min1D->SetFunc(MultiDim2One::Func1D);
//      Func.SetDir(Stp,Par+Stp,Norm);
//      Func.InitFunc();
//      double Stp1D=Min1D->Execute(0.5,HiNew);
//      Func.RestoreFunc();
//      Func.X2Vec(Stp1D,Par);
//cout<<" Step1D "<<Stp1D<<" Res "<<HiStrt-HiNew<<"\n";
//      HiStrt=HiNew;
//      LambdaCoef/=2;
}
void EigenVectMinim::Execute(double &HiStrt,VecCl &Par,VecCl &StpV,int NumIter,double ResErr)
{
  VecCl StpEst,StpOrder,NormVarCoef,D1;
  MatrCl StpDir,D2;
  Deriv2->Execute(Par,StpV,D2,D1,HiStrt);
  if (HiStrt<MathZer) 
      return;
//if (HiStrt>10){
//cout<<" Hi "<<HiStrt<<" D1 "<<D1<<" D2 \n"<<D2<<"\n"<<flush;
//}
  FormStpNormDir(D2,D1,HiStrt,StpDir,StpEst,StpOrder,NormVarCoef);//+SetNormalCoef
  if ( (NumIter!=-1)&& (CheckModel(HiStrt,StpDir,StpEst,Par,NormVarCoef)) ) return;
  MultiDim2One Func(Deriv2);
  Min1D->SetFunc(MultiDim2One::Func1D);
  //if (MakeRndMotion(HiStrt,Par,StpDir,StpEst,NormVarCoef,NumIter)) ;//return;
  double Hi0=HiStrt;
//return;
//cout<<" BreakIt "<<BreakIterStp;
//cout<<" gained ";double gain=0,Hi01;
  //for (int iter=1;iter<=NumIter;iter++)
  //{
//if (HiStrt>10){
//cout<<" Hi "<<HiStrt<<" StpeExt "<<StpEst<<"\n"<<flush;
//}
    for (int k=1;k<=D1.Dim();k++)
    {
      int Ind=StpOrder[k];
//cout<<Hi0-HiStrt<<"|"<<StpGain[Ind]<<"|"<<StpEst[Ind]<<">";Hi01=HiStrt;
//if (fabs(Hi0-HiStrt)>BreakIterStp*fabs(StpGain[Ind])) break;
if (BreakIterStp<k) break;
      //if (fabs(StpEst[Ind])<0.01*MathEps05) continue;
      //if (fabs(StpEst[Ind])<sqr(MathEps05)) continue;
      //if (fabs(StpEst[Ind])<min(0.01,ResErr)*ResErr) continue;
      if (fabs(StpEst[Ind])<0.1*ResErr) continue;
      //if (fabs(StpEst[Ind])<sqrt(M_MinDouble2)) continue;
      if (StpEst[Ind]==0) continue;
//      double Stp1D=MakeRndStp(HiStrt,Par,GetCol(StpDir,Ind),StpEst[Ind],StpGain[Ind],NormVarCoef,NumIter);

      Func.SetDir(MatrCl::GetCol(StpDir,Ind),Par,NormVarCoef);
      Func.InitFunc();

      double Stp1D=Min1D->Execute(StpEst[Ind],HiStrt);
      Func.RestoreFunc();
      Func.X2Vec(Stp1D,Par);

//cout<<"<"<<Hi01-HiStrt<<"|"<<Stp1D<<"  ";
//cout<<Stp1D/StpEst[Ind]<<"  ";
      if (fabs(Stp1D)>fabs(0.01*StpEst[Ind])) StpEst[Ind]=Stp1D;else 
        StpEst[Ind]=0.01*StpEst[Ind];
    }
//cout<<"\n";
//  }
}
int MinFindNDimClass::Execute(VecCl &Par,VecCl &Stp,int NumIter,double &FoundMisf)
{
  if (Par.Dim()==0)  {FoundMisf=Minim->GetDeriv2()->Execute(Par);return 0;}
  VecCl StpV,Par0;StpV=Stp;Par0=Par;
  double HiStrt;
  Minim->Execute(HiStrt,Par,StpV,NumIter,ResErr);
  int Continue=1;
//cout<<" StpMax "<<VecCl::ModMax(Par0-Par)<<" HIMisf "<<fabs(FoundMisf-HiStrt)<<" ResErr "<<ResErr<<"\n";
  if ((VecCl::ModMax(Par0-Par)<ResErr) || (fabs(FoundMisf-HiStrt)<0.1*ResErr)) 
    Continue=0;else Continue=1;
  FoundMisf=HiStrt;
    double MathEps05,MathEps,ErrorLQ,ZeroEig;
    Minim->GetConst(MathEps05,MathEps,ErrorLQ,ZeroEig);
  for (int k=1;k<=Stp.Dim();k++) Stp[k]=max(fabs(Par0[k]-Par[k])*10,100*MathEps05);
//      Stp=Stp*0+100*MathEps05;

  return Continue;
}


MultiDim2One* MultiDim2One::CurPtr;

ostream& operator<<(ostream& out,LambdaData &lam)
{out<<" { CoefStart "<<lam.CoefStart<<" DivideCoef "<<lam.DivideCoef
    <<" DivideVal "<<lam.DivideVal<<" } ";return out;}
istream& operator>>(istream& in,LambdaData &lam)
{
  char tmp[256];
  in>>tmp>>tmp;
  in>>lam.CurCoef>>tmp;lam.CoefStart=lam.CurCoef;
  in>>lam.DivideCoef>>tmp;
  in>>lam.DivideVal>>tmp;
  return in;
}



struct LambdaMin
{
  double ZerLambda;
  LambdaMin *Old;
  static LambdaMin *CurPtr;
  void SetZer(double zerlambda){ZerLambda=zerlambda;};
  void Init(){Old=CurPtr;CurPtr=this;}
  void Restore(){CurPtr=Old;}
  static double Func(double log_lambda)
  {VecCl tmp=CurPtr->Par;return CurPtr->Clc(log_lambda+CurPtr->ZerLambda,tmp);}
  double Clc(double log_lambda,VecCl &ParRes)
  {
    double HiStrt=Hi0;
    double Lambda=exp(log_lambda);
    VecCl StpEst,NormVarCoef;
    GetLambdaStp(D2,D1,HiStrt,StpEst,NormVarCoef,Lambda);
    MultiDim2One Func(Deriv2);
    Min1D->SetFunc(MultiDim2One::Func1D);
    Func.SetDir(StpEst,Par,NormVarCoef);
    Func.InitFunc();
    double Stp1D=Min1D->Execute(1,HiStrt);
    //Stp1D=min(1,Stp1D/(max(1,log_lambda+5)));
    //HiStrt=Func.Func1D(Stp1D);
    Func.RestoreFunc();
    Func.X2Vec(Stp1D,ParRes);
//cout<<" Lambda "<<Lambda<<" Stp1D "<<Stp1D<<" Hi "<<HiStrt<<"\n"<<flush;
    return HiStrt;
  }
  void SetPar(MatrCl &d2,VecCl &d1,VecCl &par,double hi0,
              Derive2DClass *deriv2,MinFind1DClass *min1d,LMMinim *lmmin)
  {D2=d2;D1=d1;Par=par;Hi0=hi0;Deriv2=deriv2;Min1D=min1d;LMmin=lmmin;};
private:
  MatrCl D2;
  VecCl  D1,Par;
  double Hi0;
  Ref<LMMinim> LMmin;
  Ref<Derive2DClass> Deriv2;
  Ref<MinFind1DClass> Min1D;
  void GetLambdaStp(MatrCl &D2l,VecCl &D1l,double HiStrt,VecCl &StpEst,VecCl &NormVarCoef,
               double Lambda)
  {
    MatrCl StpDir;
    double MathEps05,MathEps,ErrorLQ,ZeroEig;
    int Optimized=LMmin->Optimized;
    LMmin->GetConst(MathEps05,MathEps,ErrorLQ,ZeroEig);
    FormMinDirections dir(D2l,D1l,HiStrt,ErrorLQ,sqrt(M_MinDouble2),ZeroEig);
    dir.Execute(StpEst,NormVarCoef,Lambda,Optimized);
  }
};
LambdaMin *LambdaMin::CurPtr;

#ifndef WCPP_NET
DescribedClass * Dummy_LMMinim(){return new LMMinim();}
static ClassDesc LMMinim_ClassDesc(typeid(LMMinim),"minimN_LM",1,"MDMin",Dummy_LMMinim);
#else
static ClassDesc LMMinim_ClassDesc(typeid(LMMinim),"minimN_LM",1,"MDMin",create<LMMinim>);
#endif

LMMinim::LMMinim():EigenVectMinim(),Lambda(10,20,0.1),
                                    Optimized(FormMinDirections::OptimLambda){};
LMMinim::LMMinim(Derive2DClass *deriv2,MinFind1DClass *min1d,
        double errorlq,double zeroeig,double eps2,LambdaData lambda):
        EigenVectMinim(deriv2,min1d,errorlq,zeroeig,eps2,2),Lambda(lambda),
        Optimized(FormMinDirections::OptimLambda){};

        
int LMMinim::read_data_state(FilterIn&si){
  char tmp[256];
  EigenVectMinim::read_data_state(si);
  si>>tmp;
  si>>Lambda>>tmp;
  si>>Optimized;
  return !(!si);
};
int LMMinim::save_data_state( FilterOut&so){
    EigenVectMinim::save_data_state(so);
    so<<" Lambda "<<Lambda<<" Optimized "<<Optimized<<" ";
    return !(!so);
};


void LMMinim::Execute(double &HiStrt,VecCl &Par,VecCl &StpV,int NumIter,double ResErr)
{
  VecCl StpEst,StpOrder,NormVarCoef,D1;
  MatrCl StpDir,D2;
  Deriv2->Execute(Par,StpV,D2,D1,HiStrt);
  LambdaMin Min;
//  Ref<MinFind1DClass> MinLamb=new MinFind1DClass(NULL,0.5,HiStrt*0.001,3,2);
  Ref<MinFind1DClass> MinLamb=new MinFind1DClass(NULL,0.5,HiStrt*0.001,2,3,-1);
  Min.SetPar(D2,D1,Par,HiStrt,Deriv2,Min1D,this);
  MinLamb->SetFunc(LambdaMin::Func);
  double Log_Lam;
  //if (Lambda.CurCoef<Lambda.DivideVal) 
    Lambda.CurCoef/=Lambda.DivideCoef;
  //else  
  if (Lambda.CurCoef>Lambda.DivideVal) 
  {
    Min.Init();
    Min.SetZer(0);
    HiStrt=LambdaMin::Func(log(Lambda.CurCoef));
    Min.SetZer(log(Lambda.CurCoef));
    Log_Lam=MinLamb->Execute(-1,HiStrt);Min.Restore();
    Lambda.CurCoef=exp(Log_Lam+log(Lambda.CurCoef));
  }
  HiStrt=Min.Clc(log(Lambda.CurCoef),Par);
//cout<<"          Lam "<<FMT<<Lambda.CurCoef<<" ";//<<" ResHi "<<HiStrt<<"\n";
  if (Lambda.CurCoef<1e-15) Lambda.CurCoef=10*Lambda.DivideCoef;
};



//
//  FormStpNormDir(D2,D1,HiStrt,StpDir,StpEst,StpOrder,NormVarCoef);//+SetNormalCoef
//  MultiDim2One Func(Deriv2);
//  Min1D->SetFunc(MultiDim2One::Func1D);
//
//  //for (int iter=1;iter<=NumIter;iter++)
//  //{
//  //  for (int k=1;k<=D1.Dim();k++)
//  //  {
//      int Ind=StpOrder[k];
//      if (fabs(StpEst[Ind])<0.1*ResErr) continue;
//      if (StpEst[Ind]==0) continue;
//      Func.SetDir(GetCol(StpDir,Ind),Par,NormVarCoef);
//      Func.InitFunc();
//      double Stp1D=Min1D->Execute(StpEst[Ind],HiStrt);
//      Func.RestoreFunc();
//      Func.X2Vec(Stp1D,Par);
//      if (fabs(Stp1D)>fabs(0.01*StpEst[Ind])) StpEst[Ind]=Stp1D;else 
//        StpEst[Ind]=0.01*StpEst[Ind];
//  //  }
//  //}
//}
//
//



void MapVars::SetMap(VecCl &start,VecCl &coder)
{
  mapVar.clear();mapVar.reserve(coder.Dim());
  Start=start;
  for (int k=1;k<=coder.Dim();k++) if (coder[k]!=0) mapVar.push_back(k);

};
VecCl MapVars::CodeVec(VecCl &src)
{
  VecCl res((int)mapVar.size());
  for (int k=1;k<=(int)mapVar.size();k++) res[k]=src[mapVar[k-1]];
  return res;
}
VecCl MapVars::DeCodeVec(VecCl &src)
{
  VecCl res=Start;
  for (int k=1;k<=(int)mapVar.size();k++) res[mapVar[k-1]]=src[k];
  return res;
}


VecCl RandWalk::Execute( MatrCl &StpDir, VecCl &Strt)
{
  double Tcur,Acur,Ycur=Func->Execute(Strt),Y;
  VecCl ParCur=Strt;
  RndNumber Num(1,StpDir.Dim());
cout<<" RndWalk ";//<<NumStp<<" ";
cout<<Ycur<<" ";
  for (int k=1;k<=NumStp;k++)
  {
    ClcAT(k,Acur,Tcur);
    VecCl Stp=MatrCl::GetCol(StpDir,Num.Rnd());
    Stp=Stp*Acur*RndStp->Rnd();
    Y=Func->Execute(ParCur+Stp);
    if (GoodStp(Ycur-Y,Tcur)) {Ycur=Y;ParCur=ParCur+Stp;}
  }
cout<<Ycur<<"\n";
  return ParCur;
}
void RandWalk::ClcAT(int k,double &Acur,double &Tcur)
{
  Tcur=TStrt+(TEnd-TStrt)*(k-1)/NumStp;
  Acur=AStrt+(AEnd-AStrt)*(k-1)/NumStp;
}
int RandWalk::GoodStp(double Gain,double T)
{
  if (Gain>0) return 1;
  double Prob=exp(Gain/T);
//  return (double(rand())/RAND_MAX<Prob)?1:0;
  return (RandomClass::Rnd(1)<Prob)?1:0;
}


