#include <lib/precompiled/Messb.h>
#include "messb_mean_field.h"

//#define OnlyStat
//


MeanAngleConstructor_2D_3D *MeanAngleConstructor_2D_3D::Cur;
double MeanAngleConstructor_2D_3D::GetMeanCos(double Teta_Max) {
    int IntErr;
    double IntResMis;

    double Misf = 1e-5;
    int NumSubInt = 20, NumSpl = NumSubInt, MaxNumInt = 10000;
    Cur = this;
    IntegralAngle = M_PI - fabs(Teta_Max / 180 * M_PI);
    VecCl Phi = VecCl::MakeStepVector(NumSubInt, IntegralAngle, 0, 0), T(Phi[0]);

    T[1] = 0;
    for(int k = 2; k <= Phi[0]; k++)
        T[k] = T[k - 1] + Qags_IntSubInt(
                              IntegralPhiInt,
                              Phi[k],
                              Phi[k - 1],
                              Misf,
                              Misf,
                              MaxNumInt,
                              IntResMis,
                              IntErr);

    Phi_T_Spl = new CurveSpline;
    Phi_T_Spl->Generate(NumSpl, Misf, 0, &T[1], &Phi[1], Phi[0]);
    //Misf=1e-5;
    double ret =
        1 / T[Phi.Dim()] *
        Qags_IntSubInt(
            IntegralCosInt, 0, T[Phi.Dim()], Misf, Misf, MaxNumInt, IntResMis, IntErr);
    delete Phi_T_Spl;
    ret = (1 - Coef2D_3D) * ret + Coef2D_3D * cos(Teta_Max / 180 * M_PI);
    return ret;
}

void TetaAverageSolver::Solve(double *x, double *Res) {
    VecCl res(x[0]), cur(x[0]);
    res = res * 0;
    Res[0] = x[0];
    VecCl tmp = res;
    double Teta_H, Int_teta, H = Ham->H, SumInt = 0;
    if(OutPhi)
        cout << "   MeanTetaCos Int_Teta TetaCur\n";
//    int i = 0;
    TetaGen->ResetSerie();
    while(TetaGen->GetNextProb(Teta_H, Int_teta)) {
        SumInt += Int_teta;
        double MeanCos = MeanH->GetMeanCos(Teta_H);
        Ham->H = H * MeanCos;
        //if (OutPhi) cout<<MeanCos<<" "<<Int_teta<<" "<<Teta_H<<"\n";
        if(OutPhi)
            cout << MeanCos << " " << Int_teta << "\n";
        Solver_H_Z_poly(Ham).Solve(x, cur.Ptr);
        res = res + cur * Int_teta;
    }
    //cout<<" SumInt "<<SumInt;
    res.Copy2Double(Res);
    Ham->H = H;
    if(OutPhi)
        cout << " SumInt " << SumInt << "\n";
    cout.flush();
};


//========================================================================
// Really not rnd functions - just calculate for given Teta - probability
// Norm it afterwords...
//========================================================================


MagnonFieldStd::MagnonFieldStd(
    double js_const,
    double temp,
    double epow,
    double minen,
    double statistic,
    double qcoef1,
    double qcoef0,
    double tpow)
    : JS_const(js_const),
      Temp(temp),
      Epow(epow),
      MinEn(minen),
      Statistic(statistic),
      QCoef1(qcoef1),
      QCoef0(qcoef0),
      Tpow(tpow) {
    if(IsNan(QCoef1))
        QCoef1 = 0;
    if(IsNan(QCoef0))
        QCoef0 = M_Eps;
    if(IsNan(JS_const))
        JS_const = 1 / M_Eps2;
    if(!In_Lim(JS_const, M_Eps2, 1. / M_MinDouble2))
        JS_const = 1 / M_Eps2;
    if(!In_Lim(QCoef0, M_Eps2, 1. / M_MinDouble2))
        QCoef0 = M_Eps2;
    NormalizeCoef = 0;
    Clc(min(GetMaxX(), 89));
    NormalizeCoef = -ExpFactorMax;
    //cout<<" Max X "<<GetMaxX()<<" NormalizeCoef "<<NormalizeCoef<<"\n";
    //cout<<" QCoef1 "<<QCoef1<<" QCoef0 "<<QCoef0<<" Beta "<<Beta<<"\n";
    //cout.flush();
};

double MagnonFieldStd::Clc(double x) {
    double ret = (MinEn > 0) ? Clc1_Cos2(x) : ClcTeta4(x);
    //if (ret<1e-10) return 0;
    if((IsNan(ret)) || (!Finite(ret)) || (ret < 0))
        ret = 0;
    //cout<<ret<<" ";
    return ret;
}

double MagnonFieldStd::Clc1_Cos2(double x) {
    if(fabs(x) > 90)
        return 0;
    double Phi = x * M_PI / 180;
    double y0 = (1 - cos(Phi)), y = pow(y0, Epow);
    if(y < fabs(MinEn)) {
        ExpFactorMax = Beta() / Temp * QCoef0;
        return 0;
    }
    double ExpFactor = Beta() / Temp * (y + QCoef1 * sqrt(fabs(y)) + QCoef0);
    ExpFactorMax = ExpFactor;
    //   if ( r>40) return 0;
    //  return NormalizeCoef*y/(exp(ExpFactor)-1);
    //  double ret=NormalizeCoef*pow(y,1.5)/(exp(ExpFactor)+Statistic);
    double ret =
        pow(y, 1.5) / (exp(ExpFactor + NormalizeCoef) + Statistic * exp(NormalizeCoef));
    ret = fabs(sin(Phi)) * ret / y0;
    return ret;
}
double MagnonFieldStd::ClcTeta4(double x) {
    if(fabs(x) > 90)
        return 0;
    double Phi = x * M_PI / 180;
    double y0 = pow(Phi, 4);
    //  if (y0<fabs(MinEn)) return 0;
    if(y0 < fabs(MinEn)) {
        ExpFactorMax = Beta() / Temp * QCoef0;
        return 0;
    }
    double ExpFactor = Beta() / Temp * (y0 + QCoef1 * sqrt(y0) + QCoef0);
    ExpFactorMax = ExpFactor;
    //   if ( r>40) return 0;
    //  return NormalizeCoef*y/(exp(ExpFactor)-1);
    //  double ret=NormalizeCoef*y0*Phi/(exp(ExpFactor)+Statistic);
    double ret =
        y0 * Phi / (exp(ExpFactor + NormalizeCoef) + Statistic * exp(NormalizeCoef));
    return ret;
}

double MagnonFieldStd::GetMaxX() {
    double x = pow(5 * Temp / (4 * Beta()), 0.25);
    //double xp=pow(x,4);
    x = x * 180 / M_PI;
    //double old=NormalizeCoef;NormalizeCoef=1;
    //cout<<" Xmax = "<<x<<" ExpFactor "<<Beta/Temp*(xp+fabs(MinEn))<<" F(x) "<<Clc(x)<<"\n";NormalizeCoef=old;
    return x;
}

MagnonFieldAccustic *MagnonFieldAccustic::CurPtr;
MagnonFieldPaper *MagnonFieldPaper::CurPtr;
MagnonFieldPaperPlus *MagnonFieldPaperPlus::CurPtrPlus;
MagnonField_Paper9 *MagnonField_Paper9::CurPtr;


//double RndBoltsmanHamSol_Correlated::Clc(double x)
//{
//  //return RndBoltsmanHamSol::Clc(x+GetMaxX()/sqrt(NumNeib))+
//  //              RndBoltsmanHamSol::Clc(x-GetMaxX()/sqrt(NumNeib));
//  return Spl.Evaluate(fabs(x));
//}
//
//void RndBoltsmanHamSol_Correlated::Init()
//{
//  VecCl X=VecCl::MakeStepVector(360,0,180,1),Y;Y=X*0;
//  CurPtr=this;
//  double x_low=0,x_up=180,Misf=1e-6,IntResMis;
//  int IntNumInt=100,IntErr;
//  Norm=Qags_IntSubInt(InegralFuncSimple,x_low,x_up,Misf,Misf,IntNumInt,IntResMis,IntErr);
//  Norm=1/sqr(Norm);
//  double sigma=30/sqrt(Statistic);
//  for (int k=1;k<=Y.Dim();k++)
//  {
//    IntTeta=X[k];
////    if (sigma<0.1)
//      Y[k]=RndBoltsmanHamSol::Clc(X[k]);
////    else Y[k]=Qags_IntSubInt(InegralFunc,x_low,x_up,Misf,Misf,IntNumInt,IntResMis,IntErr);
//
//  }
//  int NumX=105;
//  Spl.Generate(NumX,Misf,0,&X[1],&Y[1],X[0]);
//};
//RndBoltsmanHamSol_Correlated *RndBoltsmanHamSol_Correlated::CurPtr;
//double RndBoltsmanHamSol_Correlated::InegralFunc(double x)
//{
//  //double teta=CurPtr->IntTeta;
//  //double sigma=CurPtr->GetMaxX()/sqrt(CurPtr->NumNeib);
//  //if (!In_Lim(teta-x,-180,180,0) ) return 0;
//  //return RndGaus(0,sigma).Clc(x)*
//  //       CurPtr->RndBoltsmanHamSol::Clc(teta-x);
//  double teta=CurPtr->IntTeta;
//  if (!In_Lim(teta-x,0,180,0) ) return 0;
//  if (!In_Lim(x*sqrt(CurPtr->Statistic),0,180,0) ) return 0;
//  return CurPtr->RndBoltsmanHamSol::Clc(x*sqrt(CurPtr->Statistic))*
//         CurPtr->RndBoltsmanHamSol::Clc(teta-x);
//}
//double RndBoltsmanHamSol_Correlated::InegralFuncSimple(double x)
//{
//  return CurPtr->RndBoltsmanHamSol::Clc(x);
//}


//========================================================================
// Really not rnd functions - just calculate for given Teta - probability
// Norm it afterwords...
//============================   END     =================================
//========================================================================


//
//void MessbMeanHTeta::GenerateTeta()
//{
//  Teta.SetDim(Num);
//  Formula.Init();
//  double Stp=1./Num,Start=-0.5*Stp;
//  for (int k=1;k<=Num;k++)  Teta[k]=Formula.X_from_Prob(Stp*k+Start);
//}
//
//void R01HQsInt(double H,double Qs,double Is,double Wid,
//               double Teta_Qs,double Phi_Qs,double Eta,
//           double Teta_H,double Phi_H,double *Xval,double *Res)
//{
//   CMatrCl Mat,AmpL,AmpR;
//   VecCl time(1);time[1]=0;
//   VecCl field(1);field[1]=H;
//   VecCl A_zx(1);A_zx[1]=Teta_H;
//   VecCl A_xy(1);A_xy[1]=Teta_H;
//   RnRelax(Mat, Teta_Qs, Phi_Qs, H, Qs, Eta, Is, Wid,
//           time,field,A_zx,A_xy);
//   RnHQsAmpVec(1,&time[0],AmpL,AmpR);
//   RIntTurn(Mat,AmpL,AmpR,Xval,Res,Wid);
//}
//
//double GlobalMaxAngle,GlobalFreq;
//double Xfunc(double x)
//{
//  return cos(GlobalMaxAngle*cos(GlobalFreq*x));
//  //return cos(GlobalMaxAngle);
//}
//double GetCosAngle_(MeanFieldStruct &Field,double Hint)
//{
//  GlobalMaxAngle=M_PI-fabs(Hint/180*M_PI);
//  GlobalFreq=Field.Frequency;
//  double Coef=2*GlobalFreq/M_PI;
//  double Misf=1e-5;
//
//  int IntErr,IntNumInt=10000;double IntResMis;
//  return Coef*Qags_IntSubInt(Xfunc,0,1/Coef,Misf,Misf,IntNumInt,IntResMis,IntErr);
//}
//double GetCosAngle(MeanFieldStruct &Field,double Hint);
//
//void MeanFieldHamInt(MeanFieldStruct &Field,
//                 double H,double Qs,double Is,double Wid,double Teta,double Phi,
//                 double Eta,
//                 double *Xval,double *Res)
// {
//  VecCl res(Xval[0]);res=res*0;Res[0]=Xval[0];
//  VecCl tmp=res;
//  MessbMeanHTeta mH(Field.NumHTetaInt,Field.formH);
////  MessbMeanHTeta mHmain(Field.NumHMainTetaInt,Field.formHmain);
//  Time_struct time;
//
////  for (int km=1;km<=Field.NumHMainTetaInt;km++)
//   {
////    double Hext=mHmain.Teta[km];
//    for (int k=1;k<=Field.NumHTetaInt;k++)
//    {
//      double Hint=mH.Teta[k];
//      //double Hzi=cos(Hint*M_PI/180),Hxi=sin(Hint*M_PI/180);
//      //double Hz=Hzi*Field.NumNeib+1,Hx=Hxi*Field.NumNeib;
//      //double mod=sqrt(sqr(Hz)+sqr(Hx));Hz/=mod;Hx/=mod;
//      //double CosAngle=(Hzi*Hz+Hxi*Hx);
//      double CosAngle=GetCosAngle(Field,Hint);
//      CosAngle=(1-Field.Coef2D_3D)*CosAngle+Field.Coef2D_3D*fabs(cos(Hint*M_PI/180));
////      double CosAngle=fabs(cos((fabs(Hint)-180)*M_PI/180/Field.NumNeib);
////      R01HQsInt( H*fabs(cos((Hext-Hint)*M_PI/180)), Qs,  Is, Wid, Teta, Phi, Eta,Hint, 0, Xval , &tmp[0]);
//      R01HQsInt( H*CosAngle, Qs,  Is, Wid, Teta, Phi, Eta,Hint, 0, Xval , &tmp[0]);
//      res=res+tmp;
//    }
//   }
//   res=res/Field.NumHTetaInt;
//  if (Field.OutPhi)
//  {
//    cout<<" Num UsedAngle Cos(UsedAngle) OscilatorsMeanCos(UsedAngle)\n";
//    for (int km=1;km<=Field.NumHTetaInt;km++) cout<<km<<" "<<mH.Teta[km]<<" "<<-cos(mH.Teta[km]*M_PI/180)<<" "<<GetCosAngle_(Field,mH.Teta[km])<<" "<<GetCosAngle(Field,mH.Teta[km])<<"\n";
//  }
//  MemoryMove(res.Ptr,Res,sizeof(double)*(Res[0]+1));
//
////cout<<" Time "<<time<<"\n";cout.flush();
// };
//
//double GlobalACoef;//,GlobalMaxAngle;
//CurveSpline *GlobalPhi_T_Spl;
//double GlobalPhiInt(double x)
//{
//  return GlobalACoef/sqrt(fabs(cos(GlobalMaxAngle)-cos(x)));
//}
//double GlobalCosInt(double x)
//{
//  return cos(GlobalPhi_T_Spl->Evaluate(x));
//}
//
//double GetCosAngle(MeanFieldStruct &Field,double Hint)
//{
//  double Misf=1e-5;
//  int IntErr,IntNumInt=10000;double IntResMis;
//  int Num=20;
//  VecCl Phi(Num),T(Num);
//
//  GlobalMaxAngle=M_PI-fabs(Hint/180*M_PI);
//  GlobalACoef=Field.Frequency;
//
//  Phi[1]=GlobalMaxAngle;T[1]=0;
//  for (int k=2;k<=Num;k++)
//  {
//    Phi[k]=Phi[k-1]-GlobalMaxAngle/(Num-1);
//    T[k]=T[k-1]+Qags_IntSubInt(GlobalPhiInt,Phi[k],Phi[k-1],Misf,Misf,IntNumInt,IntResMis,IntErr);
//  }
//
//  int NumSpl=Num;
////cout<<" T "<<T<<"\nPhi "<<Phi<<"\n";
//  GlobalPhi_T_Spl=new CurveSpline;
//  GlobalPhi_T_Spl->Generate(NumSpl,Misf,0,&T[1],&Phi[1],Num);
//  //Misf=1e-5;
//  double ret=1/T[Num]*Qags_IntSubInt(GlobalCosInt,0,T[Num],Misf,Misf,IntNumInt,IntResMis,IntErr);
//  delete GlobalPhi_T_Spl;GlobalPhi_T_Spl=NULL;
//  return ret;
//}
//
//
