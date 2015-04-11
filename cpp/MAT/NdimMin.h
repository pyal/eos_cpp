#ifndef __NDIMMIN_H
#define __NDIMMIN_H

#include "eigen_ma.h"
#include "matrics.h"
#include "lib\ref\class.h"
#include "lib\ref\class_sav.h"
#include <algorithm>
#include <vector>



struct IndexedVal:RefCount
{
  double ValInd()const{return ValueInd;}
  double ValCmp()const{return ValueCmp;}
  double ValInd(){return ValueInd;}
  double ValCmp(){return ValueCmp;}
  IndexedVal():ValueInd(0),ValueCmp(0){};
  IndexedVal(double valueind,double valuecmp):ValueInd(valueind),ValueCmp(valuecmp){};
  int operator<(const IndexedVal &sec)
  {
    int ret=(ValCmp()<sec.ValCmp());
    return ret;
  }
private:
  double ValueInd;
  double ValueCmp;
};

struct MinFind1DClass:RefCount
{
  double (*Func)(double);
  //double StpOverGo;// Not used now
  double MinStep,FuncError,LinearMulCoef;
  int MaxLinIter,MaxSearchIter,CurSearchIter;
  const double MinDouble;
  //MinFind1DClass(double stpovergo,double (*func)(double)):
  //                        StpOverGo(stpovergo),Func(func){SetConst(0,0,5,3);};
  void SetConst(double minstep,double funcerror,double linearmulcoef,int maxliniter,int maxsearchiter)
  {
    MinStep=max(minstep,M_Eps2);FuncError=max(funcerror,M_Eps2);
    MaxLinIter=maxliniter;MaxSearchIter=maxsearchiter;
    LinearMulCoef=linearmulcoef;
  };
  MinFind1DClass(double (*func)(double),double minstep,double mindouble,
                 double linearmulcoef,int maxliniter,int maxsearchiter):
                 Func(func),MinDouble(M_MinDouble2)
                 {SetConst(minstep,mindouble,linearmulcoef,maxliniter,maxsearchiter);};
  MinFind1DClass(double minstep):Func(NULL),MinDouble(M_MinDouble2){
	  SetConst(minstep,minstep,1.2,5,5);
  }
  void SetFunc( double (*f)(double) ){Func=f;};

  double Execute(double StrtStpEst,double &StrtMisf)
  {
   CurSearchIter=0;
   double ret=LinMin(StrtStpEst,0,StrtMisf);
//cout<<" FoundStp "<<ret<<" FoundMisf "<<StrtMisf<<" MinDouble "<<MinDouble<<"\n";
    return ret;
  }
// For the three points estimate x with minimum value
  double FixedMin(double x1,double x2,double x3,double y1,double y2,double y3);


private:
  int CheckBound(double &x,double Stp);
  int GoodVec(vector<IndexedVal> &vec);
  int ConstVec(vector<IndexedVal> &vec);
  double CheckDifficult(double &StrtMisf,vector<IndexedVal> &vec);
  double ThreePointEst(double &StrtMisf,vector<IndexedVal> &vec);
  double LinMin(double StrtStpEst,double StrtPnt,double &StrtMisf);
  int ZeroStp(double x1,double x2)
  {return ((CurSearchIter++>MaxSearchIter)||(fabs(x1-x2)<2*(MinStep)))?1:0;}
};
struct DeriveFuncClass:RefCount
{
  virtual double Execute(VecCl &Par){return 0;};
};
struct DeriveStaticFunc:DeriveFuncClass
{
  double (*Func)(VecCl&);
  DeriveStaticFunc(double (*f)(VecCl&)):Func(f){};
  virtual double Execute(VecCl &Par){return Func(Par);};
};
struct Derive2DClass:DeriveFuncClass
{
  virtual void   Execute(VecCl &Par,VecCl &Stp,MatrCl &D2,VecCl &D1,double &D0);
  virtual void   Execute(VecCl &Par,VecCl &Stp,VecCl &D1,double &D0);
  double Execute(VecCl &Par){return DeriveFuncClass::Execute(Par);};
};
struct Derive2DStd:Derive2DClass
{
  Ref<DeriveFuncClass> Func;
  Derive2DStd(DeriveFuncClass *func):Func(func){};
  double Execute(VecCl &Par)  {return Func->Execute(Par);}
};
// Calculate 2Deriv appr from 1Deriv for Hi square misfit function
// Estimation of the derivative is done by formulas
//                    Length
//                    -----
//                     \                                  2
//         Hi := x ->   )    W[i] (y0[i] - Func(x, x0[i]))
//                     /
//                    -----
//                    i = 1
//  Length
//  -----
//   \     /                                 /d                \\
//    )    |-2 W[i] (y0[i] - Func(x, x0[i])) |-- Func(x, x0[i])||
//   /     \                                 \dx               //
//  -----
//  i = 1
//Length
//-----  /                                                       / 2              \\
// \     |       /d               \2                             |d               ||
//  )    |2*W[i]*|--Func(x, x0[i])| -2*W[i]*(y0[i]-Func(x,x0[i]))|---Func(x,x0[i])||
// /     |       \dx              /                              |  2             ||
//-----  \                                                       \dx              //
//i = 1
struct Derive2DHi2:Derive2DClass
{
  VecCl Weight,Y0;
  void (*Func)(VecCl &Par,VecCl &Res);
  Derive2DHi2(const VecCl &weight,const VecCl &y0,void (*func)(VecCl &Par,VecCl &Res)):
                Weight(weight),Y0(y0),Func(func){};
  void   Execute(VecCl &Par,VecCl &Stp,MatrCl &D2,VecCl &D1,double &D0);
  double Execute(VecCl &Par)  {
      VecCl Res;
      Func(Par,Res);
      if (Res.CheckInfinity(1/MathZer))
          throw info_except("Got infinity... Paraqmeters are : %s",Par.ToString().c_str());
//cout<<" Par "<<Par<<" Res "<<Res<<" Hi(Res) "<<Hi(Res)<<"\n"<<flush;
      return Hi(Res);
  }
private:
  double Hi(VecCl &Res);
};

struct GetParamsCorrelation
{
  Ref<Derive2DClass> Deriv2;
  double ErrorLQ,ZeroEig,MathEps05;
  GetParamsCorrelation(Derive2DClass *deriv2,double matheps05=0,double errorlq=0,
                       double zeroeig=0):Deriv2(deriv2)
  {
    if (matheps05>0) MathEps05=matheps05;else MathEps05=M_Eps2;
    if (errorlq>0) ErrorLQ=errorlq;else ErrorLQ=MathEps05;
    if (zeroeig>0) ZeroEig=zeroeig;else ZeroEig=M_Eps;
  }
  VecCl ParamsCorrD2(MatrCl &D2);
  double ParamsCorrelation(VecCl &Par,VecCl &Stp,VecCl &ParMis);

};

struct LambdaData
{
  double CoefStart;
  double DivideCoef;
  double CurCoef;
  double DivideVal;
  LambdaData(double curcoef,double dividecoef,double divideval):
             CurCoef(curcoef),CoefStart(curcoef),DivideCoef(dividecoef),
             DivideVal(divideval){}
  friend ostream& operator<<(ostream& out,LambdaData &lam);
  friend istream& operator>>(istream& in,LambdaData &lam);
};

struct MultiDim2One
{
  //Ref<MinFind1DClass> Min1D;
  MultiDim2One(DeriveFuncClass *func):Func(func){};
  Ref<DeriveFuncClass> Func;
  VecCl Direction,StartPnt,Cvt;
  void SetDir(VecCl &direction,VecCl &startpnt,VecCl &cvt)
    {Direction=direction;StartPnt=startpnt;Cvt=cvt;};
  void ChangeCoord(VecCl &cvt){Cvt=cvt;}
  void X2Vec(double x,VecCl &Par)
//  { Par=StartPnt+Direction*x; }
  { 
      Par=StartPnt;
      for (int k=1;k<=Par.Dim();k++) 
          Par[k]+=Direction[k]*Cvt[k]*x; 
//cout<<" Par "<<Par<<" Direction "<< Direction<<" Cvt "<<Cvt<<" X "<<x<<"\n"<<flush;
  }
  MultiDim2One *Stored;

  void InitFunc(){Stored=CurPtr;CurPtr=this;}
  void RestoreFunc(){CurPtr=Stored;}

  static MultiDim2One* CurPtr;
  static double Func1D(double x){
      VecCl Par;
      CurPtr->X2Vec(x,Par);
      return CurPtr->Func->Execute(Par);
  }
};

//struct MultiDimMinimizer:RefCount
struct MultiDimMinimizer:SavableClass
{
//  MultiDimMinimizer(Derive2DClass *deriv2){}
  virtual Derive2DClass *GetDeriv2()=0;//{return NULL;};
  virtual void Execute(double &HiStrt,VecCl &Par,VecCl &StpV,int NumIter,double ResError)=0;//{};
  virtual void GetConst(double &matheps05,double &matheps,double &errorlq,double &zeroeig)=0;//{};
  virtual double ParamsCorrelation(VecCl &Par,VecCl &Stp,VecCl &ParMis)
  { 
    double MathEps05,MathEps,ErrorLQ,ZeroEig;
    GetConst(MathEps05,MathEps,ErrorLQ,ZeroEig);
    return GetParamsCorrelation(GetDeriv2(),MathEps05,ErrorLQ,ZeroEig).
                               ParamsCorrelation(Par,Stp,ParMis);
  }
// For inout
  virtual void SetFunc(MinFind1DClass *min1d,Derive2DClass *deriv2)=0;
  //virtual void Input(istream &in) = 0;
  //virtual void Output(ostream &out){) = 0;
    //virtual int save_data_state( FilterOut&so) {
    //    Output(so);
    //};
    //virtual int read_data_state(FilterIn&si){ 
    //    Input(si);
    //};

};
struct EigenVectMinim:MultiDimMinimizer
{
  double ErrorLQ,MathEps05,MathEps,ZeroEig,BreakIterStp;
  double LambdaCoef;
  int BadWlk;
  Ref<MinFind1DClass> Min1D;
  Ref<Derive2DClass> Deriv2;
  void SetFunc(MinFind1DClass *min1d,Derive2DClass *deriv2){Min1D=min1d;Deriv2=deriv2;}
  EigenVectMinim();
  EigenVectMinim(Derive2DClass *deriv2,MinFind1DClass *min1d,
                   double errorlq,double zeroeig,double eps2,double breakiterstp):
                   Deriv2(deriv2),Min1D(min1d),LambdaCoef(0.01),
                   ErrorLQ(errorlq),ZeroEig(zeroeig),BreakIterStp(breakiterstp),BadWlk(0)
                   {if (eps2>M_Eps2){MathEps05=eps2;MathEps=sqr(eps2);}
                    else {MathEps05=M_Eps2;MathEps=M_Eps;}
                   }
  void GetConst(double &matheps05,double &matheps,double &errorlq,double &zeroeig)
      {matheps05=MathEps05;matheps=MathEps;errorlq=ErrorLQ;zeroeig=ZeroEig;}
  Derive2DClass *GetDeriv2(){return Deriv2;};
// If NumIter==-1 - force lin search
  void Execute(double &HiStrt,VecCl &Par,VecCl &StpV,int NumIter,double ResError);

  //virtual void Input(istream &in);
  //virtual void Output(ostream &out);
    virtual int save_data_state( FilterOut&so);
    virtual int read_data_state(FilterIn&si);

protected:
  void FormStpNormDir(MatrCl &D2,VecCl &D1,double D0,MatrCl &StpDir,VecCl &StpEst,VecCl &StpOrder,VecCl &NormVarCoef);
  int MakeRndMotion(double &HiStrt,VecCl &ParStrt,MatrCl &StpDir,VecCl &StpEst,VecCl &NormVarCoef,int NumIter);
  double MakeRndStp(double &HiStrt,VecCl &Par,VecCl &Stp,double StpEst,double StpGet,
                VecCl &NormVarCoef,int NumIter);
  int CheckModel(double &HiStrt,MatrCl &StpMatr,VecCl &StpEst,VecCl &Par,VecCl &NormVarCoef);

VecCl StpGain;
};
//Levenberg-Marquardt
struct LMMinim:EigenVectMinim
{
  LambdaData Lambda;
  int Optimized;
  LMMinim();
  LMMinim(Derive2DClass *deriv2,MinFind1DClass *min1d,
                   double errorlq,double zeroeig,double eps2,LambdaData lambda);

  void Execute(double &HiStrt,VecCl &Par,VecCl &StpV,int NumIter,double ResError);

  //virtual void Input(istream &in);
  //virtual void Output(ostream &out);
    virtual int save_data_state( FilterOut&so);
    virtual int read_data_state(FilterIn&si);

};
struct MinFindNDimClass:RefCount
{
  double ResErr;//,ZeroEig;
  Ref<MultiDimMinimizer> Minim;
  MinFindNDimClass(MultiDimMinimizer * minim,double reserr):
                   Minim(minim),ResErr(reserr) {if (ResErr<M_Eps) ResErr=M_Eps;}


  int Execute(VecCl &Par,VecCl &Stp,int NumIter,double &FoundMisf);
protected:
  //void MinimizeIter(double &HiStrt,VecCl &Par,VecCl &StpV,int NumIter);

  //void FormStpNormDir(MatrCl &D2,VecCl &D1,double D0,MatrCl &StpDir,VecCl &StpEst,VecCl &StpOrder,VecCl &NormVarCoef);
};

struct MapVars
{
  vector<int> mapVar;
  VecCl Start,Norm;
  void SetMap(VecCl &start,VecCl &coder);
  VecCl CodeVec(VecCl &src);
  VecCl DeCodeVec(VecCl &src);
};

#include "mtrnd.h"

struct RandWalk
{
  double TStrt,TEnd,AStrt,AEnd;
  int NumStp;
  Ref<DeriveFuncClass> Func;
  Ref<RndFunc> RndStp;
  RandWalk(double tstrt,double tend,double astrt,double aend,int numstp,
    DeriveFuncClass* func,RndFunc* rndstp):TStrt(tstrt),TEnd(tend),AStrt(astrt),
    AEnd(aend),NumStp(numstp),Func(func),RndStp(rndstp){};
  VecCl Execute( MatrCl &StpDir, VecCl &Strt);
  void ClcAT(int k,double &Acur,double &Tcur);
  int GoodStp(double Gain,double T);
};




#endif
