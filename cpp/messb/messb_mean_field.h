#ifndef __Messb_Mean_Field_H
#define __Messb_Mean_Field_H

//#include "matrics.h"
#include "lib/std/ex_out.h"
#include "messb.h"
#include "mat\mtrnd.h"
#include "famessb.h"
#include "mat\quad.h"


struct MagnonMeanField : RndIntegral {
    virtual double GetExpFactor() = 0;
    virtual double GetDisorderedInt() = 0;
    virtual double GetMaxOrderedQ() {
        return 0;
    };
    virtual double GetMinOrderedQ() {
        return 0;
    };
};


struct TetaGenerator : RefCount {
    TetaGenerator(
        MagnonMeanField *rndgenerator,
        int numhtetaint,
        double normcoef = 1,
        int clcstartparams = 1)
        : RndGenerator(rndgenerator), NumHTetaInt(numhtetaint), NormCoef(normcoef) {
        TetaStp = VecCl::MakeStepVector(NumHTetaInt, 0, 90, 1);
        TetaProb = TetaStp;
        ResetSerie();
        if(!clcstartparams)
            return;
        for(int k = 1; k <= TetaStp.Dim(); k++)
            TetaProb[k] = RndGenerator->Clc(TetaStp[k]);
        if(NormCoef >= 0)
            TetaProb =
                TetaProb *
                (NormCoef / max<double>(TetaProb * (TetaProb * 0 + 1), M_MinDouble2));
    }
    int GetNextProb(double &Teta_H, double &Int_Teta) {
        if(CurNum >= NumHTetaInt) {
            ResetSerie();
            return 0;
        }
        Teta_H = TetaStp[++CurNum];
        //    Int_Teta=RndGenerator->Clc(Teta_H)*RndGenerator->Norm;
        //    Int_Teta=RndGenerator->Clc(Teta_H);
        Int_Teta = TetaProb[CurNum];
        return 1;
    }
    double GetDisorderedInt() {
        return NormCoef * RndGenerator->GetDisorderedInt();
    }
    void ResetSerie() {
        CurNum = 0;
    }
    int GetNumInt() {
        return NumHTetaInt;
    }

protected:
    int NumHTetaInt, CurNum;
    Ref<MagnonMeanField> RndGenerator;
    VecCl TetaProb, TetaStp;
    double NormCoef;
};
struct TetaGenerator_Paper9 : TetaGenerator {
    TetaGenerator_Paper9(
        MagnonMeanField *rndgenerator,
        int numhtetaint,
        double normcoef = 1)
        : TetaGenerator(rndgenerator, numhtetaint, normcoef, 0) {
        VecCl Q = VecCl::MakeStepVector(
            NumHTetaInt,
            RndGenerator->GetMinOrderedQ(),
            RndGenerator->GetMaxOrderedQ(),
            1);
        TetaStp = Q;
        TetaProb = Q;
        for(int k = 1; k <= Q.Dim(); k++) {
            TetaProb[k] = RndGenerator->Clc(TetaStp[k]);
        }
        if(NormCoef >= 0)
            TetaProb =
                TetaProb *
                (NormCoef / max<double>(TetaProb * (TetaProb * 0 + 1), M_MinDouble2));
    }
};


struct MeanAngleConstructor : RefCount {
    //MeanAngleConstructor(){};
    virtual double GetMeanCos(double Teta_Max) {
        return cos(M_PI * Teta_Max / 180);
    }
};

struct MeanAngleConstructor_2D_3D : MeanAngleConstructor {
    double Frequency, Coef2D_3D;
    static MeanAngleConstructor_2D_3D *Cur;
    double IntegralAngle;
    CurveSpline *Phi_T_Spl;
    MeanAngleConstructor_2D_3D(double frequency, double coef2d_3d)
        : Frequency(frequency), Coef2D_3D(coef2d_3d){};
    static double IntegralPhiInt(double x) {
        return Cur->Frequency / sqrt(fabs(cos(Cur->IntegralAngle) - cos(x)));
    };
    static double IntegralCosInt(double x) {
        return cos(Cur->Phi_T_Spl->Evaluate(x));
    }

    double GetMeanCos(double Teta_Max);
};
struct MeanAngleConstructor_Paper9 : MeanAngleConstructor {
    MeanAngleConstructor_Paper9(double rt) : MeanAngleConstructor(), Rt(rt){};
    virtual double GetMeanCos(double q) {
        double ch = 1;
        if(Rt * q > 0.01) {
            double e = exp(2 / (Rt * q));
            ch = (1 + e) / (e - 1);
        }
        return ch - Rt * q;
    }

private:
    double Rt;
};


struct TetaAverageSolver : Solver {
    Ref<MeanAngleConstructor> MeanH;
    Ref<HamData> Ham;
    int OutPhi;
    Ref<TetaGenerator> TetaGen;
    TetaAverageSolver(
        HamData *ham,
        TetaGenerator *tetagen,
        MeanAngleConstructor *meanh,
        int outphi)
        : Ham(ham), TetaGen(tetagen), OutPhi(outphi), MeanH(meanh){};
    void Solve(double *x, double *Res);
};
struct TetaAverageSolver_Advanced : TetaAverageSolver {
    Ref<HamData> HamDisord;
    double RelDisordInt;
    TetaAverageSolver_Advanced(
        HamData *ham,
        TetaGenerator *tetagen,
        MeanAngleConstructor *meanh,
        int outphi,
        HamData *hamdisord,
        double reldisordint)
        : TetaAverageSolver(ham, tetagen, meanh, outphi),
          HamDisord(hamdisord),
          RelDisordInt(reldisordint){};
    void Solve(double *x, double *Res) {
        TetaAverageSolver::Solve(x, Res);
        Res[0] = x[0];
        VecCl tmp(x[0]), cur(x[0]);
        tmp = Res;
        Solver_H_Z_poly(HamDisord).Solve(x, cur.Ptr);
        double DisordCoef = TetaGen->GetDisorderedInt();
        //    double OrdInt=tmp*(tmp*0+1);
        tmp = tmp + cur * DisordCoef * RelDisordInt;
        tmp.Copy2Double(Res);
    }
};


//struct BoltzmanTetaAverage_2D3D_Solver:Solver
//{
//  BoltzmanTetaAverage_2D3D_Solver(HamData *ham,TetaGenerator *tetagen,
//    MeanAngleConstructor *meanh,int outphi):Ham(ham),TetaGen(tetagen),MeanH(meanh),
//                                            OutPhi(outphi){};
//  void Solve(double *x,double *Res);
//  //Ref<HamData> Ham;
//  //int OutPhi;
//  //Ref<TetaGenerator> TetaGen;
//};


//========================================================================
// Really not rnd functions - just calculate for given Teta - probability
// Norm it afterwords...
//========================================================================


// Prob(E)=N(E)*Exp(-Beta*E/T)
// N(E)=A*E^Epow
// Prob(E)=Exp[ -Beta*E/T+Epow*log(E) ]
// E=Beta*0.5(cos(x)+1)
//
// Maximum of the function cos(teta)=1-2*T*Epow/Beta
struct FieldBoltsmanSinPower : MagnonMeanField {
    RndBoltsmanSinPower Bolt;
    FieldBoltsmanSinPower(double BoltCoef = 0, double SinPower = 1, double BoltCoef2 = 1)
        : Bolt(BoltCoef, SinPower, BoltCoef2){};
    virtual double GetExpFactor() {
        return 0;
    }
    double Clc(double x) {
        return Bolt.Clc(x);
    }
    virtual double GetDisorderedInt() {
        return 0;
    };
};
struct MagnonFieldStd : MagnonMeanField {
    MagnonFieldStd(
        double js_const,
        double temp,
        double epow,
        double minen,
        double numneib,
        double qcoef1,
        double qcoef0,
        double tpow);
    virtual double GetExpFactor() {
        return NormalizeCoef;
    }
    virtual double GetDisorderedInt() {
        return 0;
    };
    virtual double Clc(double x);
    virtual double Clc1_Cos2(double x);
    virtual double ClcTeta4(double x);

    //  double Energy(double x) { return Beta*NormalEnergy(x);}
    //// For Exp - norm E [1..0]
    //  double NormalEnergy(double x);
    ////  zero is zero angle - so add 180 to x
    //  double GetShiftedX(double x);
    double GetMaxX();
    double Beta() {
        return JS_const / pow(Temp, 2 * Tpow);
    };
    double NormalizeCoef;

private:
    double MinEn, Statistic, QCoef1, QCoef0, Tpow;
    double JS_const, Temp, Epow;
    double ExpFactorMax;
};

struct MagnonFieldOptic : MagnonMeanField {
    MagnonFieldOptic(
        double js_const,
        double temp,
        double statistic,
        double t_pow,
        double q_a0,
        double q_a1,
        double e_a0,
        double e_a1)
        : NumInt2EstMax(100),
          JS_const(js_const),
          Temp(temp),
          Statistic(statistic),
          T_pow(t_pow),
          E_a0(q_a0),
          E_a1(q_a1),
          Q_a0(q_a0),
          Q_a1(q_a1) {
        GetMaxX();
    }
    virtual double GetDisorderedInt() {
        return 0;
    };
    double Clc(double x) {
        if(!GoodPar(x))
            return 0;
        return Distrib(x);
    }
    double GetExpFactor() {
        return ExpFactor;
    }
    double Energy(double x) {
        double Phi = x * M_PI / 180, CosX = cos(Phi);
        return sqr(((1 - CosX) + E_a0 * pow(1 - CosX, E_a1)) / pow(Temp, T_pow));
    }
    double q(double x) {
        return 0.5 * (-Q_a1 + q_sqrt(x));
    }
    double q_sqrt(double x) {
        return sqrt(4 * (Energy(x) - Q_a0) + sqr(Q_a1));
    }
    int GoodPar(double x) {
        return (Energy(x) - Q_a0 < 0) ? 0 : 1;
    }
    double dq(double x) {
        double Phi = x * M_PI / 180, CosX = cos(Phi), SinX = fabs(sin(Phi)),
               E2 = sqrt(Energy(x)), Denom = q_sqrt(x);
        return SinX * E2 * (1 + E_a0 * E_a1 * pow(1 - CosX, E_a1 - 1)) /
               max<double>(Denom, M_MinDouble2);
    }
    double Distrib(double x) {
        return sqr(q(x)) * dq(x) / (exp(JS_const / Temp * Energy(x) + ExpFactor) +
                                    Statistic * exp(ExpFactor));
    }
    double GetMaxX()   // + Set ExpFactor
    {
        VecCl Est = VecCl::MakeStepVector(NumInt2EstMax, 0, 90, 1);
        ExpFactor = 0;
        double max_val = -1. / M_MinDouble2;
        int k_max = 1;
        for(int k = 1; k <= Est.Dim(); k++) {
            if(!GoodPar(Est[k]))
                continue;
            double x = Est[k], tmp = log(sqr(q(x)) * dq(x)) - JS_const / Temp * Energy(x);
            if(tmp > max_val) {
                max_val = tmp;
                k_max = k;
            }
        }
        ExpFactor = -JS_const / Temp * Energy(Est[k_max]);
        return Est[k_max];
    }

private:
    int NumInt2EstMax;
    double ExpFactor;   //is -ExpFactorMax - to be added under exp
    double JS_const, Temp, Statistic;
    double Q_a0, Q_a1, E_a0, E_a1, T_pow;
};
struct MagnonFieldAccustic : MagnonMeanField {
    MagnonFieldAccustic(
        double js_const,
        double temp,
        double statistic,
        double t_pow,
        double q_a0,
        double q_a1,
        double e_a0,
        double e_a1,
        double e_max,
        double q_max,
        double q_min,
        double dcosmax)
        : NumInt2EstMax(100),
          JS_const(js_const),
          Temp(temp),
          Statistic(statistic),
          T_pow(t_pow),
          E_a0(e_a0),
          E_a1(e_a1),
          Q_a0(q_a0),
          Q_a1(q_a1),
          E_max(e_max),
          Q_max(q_max),
          Q_min(q_min),
          dCosMax(dcosmax) {
        Temp = fabs(Temp);
        E_max = fabs(E_max);
        Q_max = fabs(Q_max);
        Q_min = fabs(Q_min);
        GetMaxX();
    }
    virtual double GetDisorderedInt() {
        double Qbnd = q_bound();
        if(Qbnd > Q_max)
            return 0;
        CurPtr = this;

        //  YNorm=100/RecInt(RndIntegral::xFunc,x_low,x_up,Misf);Misfit=StndErr*0.1;
        int IntErr, IntNumInt = 10000;
        double IntResMis;
        double Misf = 1e-15;
        double IntOrdered = Qags_IntSubInt(
            MagnonFieldAccustic::IntegralFunc,
            Q_min,
            Qbnd,
            Misf,
            Misf,
            IntNumInt,
            IntResMis,
            IntErr);
        //double IntDisOrdered=Qags_IntSubInt(MagnonFieldAccustic::IntegralFunc,
        //                                  Qbnd,Q_max,Misf,Misf,IntNumInt,IntResMis,IntErr);
        double IntDisOrdered = Qags_IntSubInt(
                                   MagnonFieldAccustic::IntegralFunc,
                                   Qbnd,
                                   min(2 * Qbnd, Q_max),
                                   Misf,
                                   Misf,
                                   IntNumInt,
                                   IntResMis,
                                   IntErr) +
                               Qags_IntSubInt(
                                   MagnonFieldAccustic::IntegralFunc,
                                   min(Q_max, 2 * Qbnd),
                                   Q_max,
                                   Misf,
                                   Misf,
                                   IntNumInt,
                                   IntResMis,
                                   IntErr);
        double Coef = (IntOrdered < M_MinDouble2) ? 1 : IntDisOrdered / IntOrdered;

        double ret = Coef;   ///(pow(Q_max/Qbnd,3)-1);
        //cout<<" IntOrdered "<<IntOrdered<<" IntDisOrdered "<<IntDisOrdered;
        //cout<<" Q_min "<<Q_min<<" Qbnd "<<Qbnd<<" Q_max "<<Q_max<<" Coef "<<ret<<"\n";
        return ret;
    };

    double Clc(double x) {
        if(!GoodPar(x))
            return 0;
        return Distrib(x);
    }
    double GetExpFactor() {
        return ExpFactor;
    }
    double Energy(double x) {
        //double Phi=x*M_PI/180,CosX=cos(Phi);
        //return sqr(( (1-CosX)+E_a0*pow(1-CosX,E_a1) )/pow(Temp,T_pow));
        return Energy_Q(q(x));
    }
    double Energy_Q(double x) {
        return sqr(x) + Q_a1 * x + Q_a0;
    }
    double q(double x) {
        double Phi = x * M_PI / 180, CosX = cos(Phi);
        double ret = ((1 - CosX) + E_a0 * pow(1 - CosX, E_a1));
        if(ret < 0)
            cout << " very bad q<0\n";
        //if (T_pow>0) return fabs(ret)*E_max*pow(Temp,-T_pow);
        return fabs(ret) * E_max * ((T_pow > 0) ? pow(Temp, -T_pow) : 1);
    }
    double q_bound() {
        return dCosMax * E_max * ((T_pow > 0) ? pow(Temp, -T_pow) : 1);
    }
    int GoodPar(double x) {
        return ((q(x) < min(Q_max, q_bound()) && (q(x) > Q_min))) ? 1 : 0;
    }
    double dq(double x) {
        double Phi = x * M_PI / 180, CosX = cos(Phi), SinX = fabs(sin(Phi));
        if(E_a1 != 0)
            return SinX * (1 + E_a0 * E_a1 * pow(1 - CosX, E_a1 - 1));
        return SinX;
    }
    double Distrib(double x) {
        return sqr(q(x)) * dq(x) / (exp(JS_const / Temp * Energy(x) + ExpFactor) +
                                    Statistic * exp(ExpFactor));
    }
    double GetMaxX()   // + Set ExpFactor
    {
        VecCl Est = VecCl::MakeStepVector(NumInt2EstMax, 0, 90, 1);
        ExpFactor = 0;
        double max_val = -1. / M_MinDouble2;
        int k_max = 1;
        for(int k = 1; k <= Est.Dim(); k++) {
            if(!GoodPar(Est[k]))
                continue;
            double x = Est[k], tmp = log(sqr(q(x)) * dq(x)) - JS_const / Temp * Energy(x);
            if(tmp > max_val) {
                max_val = tmp;
                k_max = k;
            }
        }
        ExpFactor = -JS_const / Temp * Energy(Est[k_max]);
        return Est[k_max];
    }
    double StDens_Q(double x) {
        //    x=x*E_max*((T_pow>0)?pow(Temp,-T_pow):1);
        return sqr(x) / (exp(JS_const / Temp * Energy_Q(x) + ExpFactor) +
                         Statistic * exp(ExpFactor));
    }
    static double IntegralFunc(double x)   // integrating by q
    {
        return CurPtr->StDens_Q(x);
    }
    static MagnonFieldAccustic *CurPtr;

private:
    int NumInt2EstMax;
    double ExpFactor;   //is -ExpFactorMax - to be added under exp
    double JS_const, Temp, Statistic;
    double Q_a0, Q_a1, E_a0, E_a1, T_pow;
    double Q_min, Q_max, E_max, dCosMax;
};
struct MagnonFieldPaper : MagnonMeanField {
    MagnonFieldPaper(
        double js_const,
        double temp,
        double statistic,
        //      double t_pow,
        double q_a0,
        double q_a1,
        //      double e_a0,double e_a1,
        double Rconst,
        double q_max,
        double q_min,
        double dcosmax)
        : NumInt2EstMax(100),
          JS_const(js_const),
          Temp(temp),
          Statistic(statistic),
          Q_a0(q_a0),
          Q_a1(q_a1),
          R(Rconst),
          Q_max(q_max),
          Q_min(q_min),
          dCosMax(dcosmax) {
        Temp = fabs(Temp);
        R = fabs(R);
        Q_max = fabs(Q_max);
        Q_min = fabs(Q_min);
        GetMaxX();
    }
    virtual double GetDisorderedInt() {
        //    return 0;
        double Qbnd = q_bound();
        if(Qbnd > Q_max)
            return 0;
        CurPtr = this;

        //  YNorm=100/RecInt(RndIntegral::xFunc,x_low,x_up,Misf);Misfit=StndErr*0.1;
        int IntErr, IntNumInt = 10000;
        double IntResMis;
        double Misf = 1e-15;
        double IntOrdered = Qags_IntSubInt(
            MagnonFieldPaper::IntegralFunc,
            Q_min,
            Qbnd,
            Misf,
            Misf,
            IntNumInt,
            IntResMis,
            IntErr);
        //double IntDisOrdered=Qags_IntSubInt(MagnonFieldAccustic::IntegralFunc,
        //                                  Qbnd,Q_max,Misf,Misf,IntNumInt,IntResMis,IntErr);
        double IntDisOrdered = Qags_IntSubInt(
                                   MagnonFieldPaper::IntegralFunc,
                                   Qbnd,
                                   min(2 * Qbnd, Q_max),
                                   Misf,
                                   Misf,
                                   IntNumInt,
                                   IntResMis,
                                   IntErr) +
                               Qags_IntSubInt(
                                   MagnonFieldPaper::IntegralFunc,
                                   min(Q_max, 2 * Qbnd),
                                   Q_max,
                                   Misf,
                                   Misf,
                                   IntNumInt,
                                   IntResMis,
                                   IntErr);
        double Coef = (IntOrdered < M_MinDouble2) ? 1 : IntDisOrdered / IntOrdered;

        double ret = Coef;   ///(pow(Q_max/Qbnd,3)-1);
        //cout<<" IntOrdered "<<IntOrdered<<" IntDisOrdered "<<IntDisOrdered;
        //cout<<" Q_min "<<Q_min<<" Qbnd "<<Qbnd<<" Q_max "<<Q_max<<" Coef "<<ret<<"\n";
        return ret;
    };

    double Clc(double x) {
        if(!GoodPar(x))
            return 0;
        return Distrib(x);
    }
    double GetExpFactor() {
        return ExpFactor;
    }
    double Energy_Q(double x) {
        return sqr(x) + Q_a1 * x + Q_a0;
    }
    virtual double q(double x) {
        double Phi = x * M_PI / 180, CosX = cos(Phi);
        FindCos = CosX;
        CurPtr = this;
        double From = 0, To = 10, Guess = (1 - FindCos) / (R * Temp);
        if(!Fzero(MagnonFieldPaper::ZeroFunc, From, To, Guess, 1e-8, 1e-8, 1000))
            cout << " Fzero error\n";
        return Guess;
    }
    double FindCos;
    double MisfQ(double x) {
        double R1 = R * Temp * x;
        double Coef = (R1 < 0.001) ? 1 : (1 + exp(2 / R1)) / (exp(2 / R1) - 1);
        Coef = 1;
        return -R1 + Coef - FindCos;
    }
    virtual double q_bound() {
        return dCosMax / (R * Temp);
    }
    virtual double dq(double qu, double tet) {
        double Phi = tet * M_PI / 180, CosX = cos(Phi), SinX = fabs(sin(Phi));
        return -SinX * Temp * R * qu * qu / (2 * Temp * R * qu * CosX - SinX * SinX);
        //double R1 = R*Temp*qu;
        //double Coef = (R1<0.001)?0:2*exp(1/R1)/(exp(2/R1)-1);
        //double Der = -R*Temp+sqr(Coef)/(qu*R1);
        //return -SinX/Der;
    }
    double Denom(double qu) {
        return 1. / (exp(JS_const * Energy_Q(qu) / Temp + ExpFactor) +
                     Statistic * exp(ExpFactor));
    }
    double Distrib(double x) {
        double qu = q(x);
        double ret = sqr(qu) * dq(qu, x) * Denom(qu);
        cout << x << " " << qu << " " << dq(qu, x) << " " << ret << "\n";
        //      cout<<exp(JS_const*Energy_Q(qu)/Temp+ExpFactor)<<" "<<Statistic*exp(ExpFactor)<<"\n";
        return ret;
    }
    double StDens_Q(double x) {
        return sqr(x) * Denom(x);
    }
    static double IntegralFunc(double x) {
        return CurPtr->StDens_Q(x);
    }   // integrating by q
    static double ZeroFunc(double x) {
        return CurPtr->MisfQ(x);
    }   // integrating by q
    static MagnonFieldPaper *CurPtr;

    int GoodPar(double x) {
        double qu = q(x);
        return ((qu < min(Q_max, q_bound()) && (qu > Q_min))) ? 1 : 0;
    }
    double GetMaxX() {
        ExpFactor = -JS_const / Temp * Energy_Q(1.04681 * sqrt(Temp / JS_const));
        //cout<<" ExpFactor "<<ExpFactor<<"\n";
        return 0.1;
    }
    double GetMaxXOld()   // + Set ExpFactor
    {
        VecCl Est = VecCl::MakeStepVector(NumInt2EstMax, 0, 90, 1);
        ExpFactor = 0;
        double max_val = -1. / M_MinDouble2;
        int k_max = 1;
        for(int k = 1; k <= Est.Dim(); k++) {
            if(!GoodPar(Est[k]))
                continue;
            double x = Est[k], qu = q(x);
            double tmp = log(sqr(qu) * dq(qu, x)) - JS_const / Temp * Energy_Q(qu);
            if(tmp > max_val) {
                max_val = tmp;
                k_max = k;
            }
        }
        ExpFactor = -JS_const / Temp * Energy_Q(q(Est[k_max]));
        //ExpFactor = 0;
        //cout<<" ExpFactor "<<ExpFactor<<"\n";
        return Est[k_max];
    }

protected:
    int NumInt2EstMax;
    double ExpFactor;   //is -ExpFactorMax - to be added under exp
    double JS_const, Temp, Statistic;
    double Q_a0, Q_a1;
    double Q_min, Q_max, R, dCosMax;
};
struct MagnonFieldPaperPlus : MagnonFieldPaper {
    double AlphaPar;
    MagnonFieldPaperPlus(
        double js_const,
        double temp,
        double statistic,
        double q_a0,
        double q_a1,
        double Rconst,
        double q_max,
        double q_min,
        double dcosmax,
        double alpha)
        : MagnonFieldPaper(
              js_const,
              temp,
              statistic,
              q_a0,
              q_a1,
              Rconst,
              q_max,
              q_min,
              dcosmax) {
        AlphaPar = max<double>(alpha, StndErr);
        //VecCl tmp=VecCl::MakeStepVector(100,0.1,100,1);
        //for (int k = 1;k<=tmp[0];k++) {
        //    Temp = tmp[k];
        //    cout<<tmp[k]<<" "<<1-GetDisorderedInt()<<"\n";
        //}
        //Temp = temp;
    };

    static MagnonFieldPaperPlus *CurPtrPlus;
    static double IntegralFuncPlus(double x) {
        return CurPtrPlus->ClcIntegralInt(x);
    }
    double ClcIntegralInt(double q) {
        double qrt = q * R * Temp;
        double Coef;
        if(1 / qrt < 10) {
            double e = exp(1 / qrt);
            Coef = e * (e - exp(AlphaPar)) / (sqr(e) - 1);
        } else {
            Coef = 1;
        }
        double mul = sqr(q) / (exp(JS_const * sqr(q) / Temp) - 1);
        //      double mul = StDens_Q(q)*exp(ExpFactor);
        return Coef * mul;
    }
    double MisfQPlus(double q) {
        double qrt = R * Temp * q;
        double Add;
        if(1 / qrt < 10) {
            double e = exp(1 / qrt);
            Add = e * (1 - qrt * AlphaPar) / (e - exp(AlphaPar));
        } else {
            Add = 1 - qrt * AlphaPar;
        }
        //cout<<q<<" "<<Add+qrt*(AlphaPar-1)-FindCos<<"\n";
        return Add + qrt * (AlphaPar - 1) - FindCos;
    }
    static double ZeroFuncPlus(double x) {
        return CurPtrPlus->MisfQPlus(x);
    }   // integrating by q

    virtual double q_bound() {
        return 1 / (R * Temp * AlphaPar);
    }
    virtual double q(double x) {
        double Phi = x * M_PI / 180, CosX = cos(Phi);
        FindCos = CosX;
        CurPtrPlus = this;
        double From = 0, To = min(q_bound(), Q_max), Guess = (1 - FindCos) / (R * Temp);
        if(!Fzero(MagnonFieldPaperPlus::ZeroFuncPlus, From, To, Guess, 1e-8, 1e-8, 1000))
            cout << " Fzero error\n";
        return Guess;
    }
    virtual double dq(double q, double tet) {
        double qrt = R * Temp * q;
        double der;
        if(1 / qrt < 10) {
            double e = exp(1 / qrt);
            double a = exp(AlphaPar);
            double up = -sqr(e * qrt) + sqr(a * qrt) * (AlphaPar - 1) +
                        e * a * (1 - qrt * (qrt * (AlphaPar - 2) + AlphaPar));
            double down = sqr(e - a) * qrt * q;
            der = up / down;
        } else {
            der = -R * Temp;
        }
        double Phi = tet * M_PI / 180, CosX = cos(Phi), SinX = fabs(sin(Phi));
        if(fabs(der) < M_Eps2)
            der = M_Eps2;
        //cout<<" der "<<der<<" "<<" SinX "<<SinX<<"\n"; cout.flush();
        //      return -SinX/der;
        return max<double>(-SinX / der, 0);
    }
    virtual double GetDisorderedInt() {
        double Coef = 0.8638721596138914 * pow(JS_const / Temp, 1.5);
        CurPtrPlus = this;
        int IntErr, IntNumInt = 10000;
        double IntResMis;
        double Qbnd = 1 / (R * Temp * AlphaPar);
        double Misf = 1e-15;
        double IntOrdered = Coef * Qags_IntSubInt(
                                       MagnonFieldPaperPlus::IntegralFuncPlus,
                                       Q_min,
                                       Qbnd,
                                       Misf,
                                       Misf,
                                       IntNumInt,
                                       IntResMis,
                                       IntErr);
        return (1 - IntOrdered);
    }
};


// ========================= Magnon_Updated ====================================

struct MagnonField_Paper9 : MagnonMeanField {

    MagnonField_Paper9(
        double temp,
        double statistic,
        double q_a0,
        double q_a1,
        double q_a2,
        double Rconst,
        double q_max,
        double q_min,
        double dcosmax)
        : Temp(temp),
          Statistic(statistic),
          Q_a0(q_a0),
          Q_a1(q_a1),
          Q_a2(q_a2),
          R(Rconst),
          Q_max(q_max),
          Q_min(q_min),
          dCosMax(dcosmax) {
        Temp = fabs(Temp);
        R = fabs(R);
        Q_max = fabs(Q_max);
        Q_min = fabs(Q_min);
        ExpFactor = 0;
    }
    virtual double GetDisorderedInt() {
        double Qbnd = q_bound();
        if(Qbnd > Q_max)
            return 0;
        CurPtr = this;
        int IntErr, IntNumInt = 10000;
        double IntResMis;
        double Misf = 1e-15;
        double IntFull = Qags_IntSubInt(
            MagnonField_Paper9::IntegralFunc,
            0,
            Q_max,
            Misf,
            Misf,
            IntNumInt,
            IntResMis,
            IntErr);
        double IntDisOrdered = Qags_IntSubInt(
            MagnonField_Paper9::IntegralFunc,
            Qbnd,
            Q_max,
            Misf,
            Misf,
            IntNumInt,
            IntResMis,
            IntErr);
        if(IntFull < MathZer)
            return 1;
        return IntDisOrdered / IntFull;
    };
    virtual double q_bound() {
        return dCosMax / (R * Temp);
    }

    double GetExpFactor() {
        return ExpFactor;
    }
    double Clc(double qu) {
        return fabs(sqr(qu) * dq(qu) * Denom(qu));
    }
    double Energy_Q(double qu) {
        return Q_a2 * sqr(qu) + Q_a1 * qu + Q_a0;
    }
    double Denom(double qu) {
        return 1. / (exp(Energy_Q(qu) / Temp + ExpFactor) + Statistic * exp(ExpFactor));
    }
    virtual double dq(double qu) {
        double d = qu * R * Temp;
        double ep = exp(1 / d), em = exp(-1 / d);
        double den = sqr(ep - em) * qu * d;
        return 4 / den - R * Temp;
    }


    virtual double GetMaxOrderedQ() {
        return min(Q_max, q_bound());
    };
    virtual double GetMinOrderedQ() {
        return Q_min;
    };


    static MagnonField_Paper9 *CurPtr;
    static double IntegralFunc(double x) {
        return CurPtr->ClcIntegral(x);
    }
    double ClcIntegral(double q) {
        return fabs(sqr(q) * Denom(q));
    }

protected:
    double ExpFactor;   //is -ExpFactorMax - to be added under exp
    double Temp, Statistic;
    double Q_a0, Q_a1, Q_a2;
    double Q_min, Q_max, R, dCosMax;
    //private:
    //// Integral calculation only
    //    double
};


//struct MagnonField_AccusOptic:MagnonMeanField
//{
//  Ref<MagnonMeanField> Optic,Accustic;
//  MagnonField_AccusOptic(Ref<MagnonMeanField> accustic,Ref<MagnonMeanField> optic,
//    double coefoptic): Optic(optic),Accustic(accustic),CoefOptic(coefoptic)
//  {
//    double Eo=-Optic->GetExpFactor();
//    double Ea=-Accustic->GetExpFactor();
//    ExpFactor=max<double>(Eo,Ea);
//    Eo-=ExpFactor;Ea-=ExpFactor;
//    NormOptic=SetExp(Eo);NormAccus=SetExp(Ea);
//  }
//  double SetExp(double e) { if (e<-40) return 0;return exp(e); }
//  double GetExpFactor()   { return ExpFactor;}
//  double Clc(double x)
//  {
//    double IntA=Accustic->Clc(x)*NormAccus;
//    double IntO=Optic->Clc(x)*NormOptic*CoefOptic;
//    return IntA+IntO;
//  }
//
//private:
//  double CoefOptic;
//  double NormAccus,NormOptic,ExpFactor;
//};

//========================================================================
// Really not rnd functions - just calculate for given Teta - probability
// Norm it afterwords...
//========================================================================


#endif

//struct MeanFieldFormSinCos
//{
//  RndBoltsmanSinPower AnisGenerator;
//  double AnisTeta,AnisPower;
//  MeanFieldFormSinCos(double anisteta=-1,double anispower=-1)
//  {Set( anisteta, anispower);}
//  void Set(double anisteta,double anispower) {AnisTeta=anisteta;AnisPower=anispower;}
//  void Init()
//  {
//    //if (fabs(AnisTeta)<30)
//    {
//	  AnisGenerator.Set(AnisTeta,AnisPower);
//	  AnisGenerator.Init(-180,180,800,1e-5);
//    }
//  };
//  double X_from_Prob(double prob){return AnisGenerator.X_from_Prob(prob);}
////    {return ((fabs(AnisTeta)<30)? AnisGenerator.X_from_Prob(prob): 180);}
//};
//
//struct MessbMeanHTeta
//{
//  int Num;
//  MeanFieldFormSinCos Formula;
//  VecCl Teta;
//  MessbMeanHTeta(int num,MeanFieldFormSinCos &form)
//  {Num=num;Formula.Set(form.AnisTeta,form.AnisPower);GenerateTeta();}
//  void GenerateTeta();
//
//};
//
//struct MeanFieldStruct
//{
//  int OutPhi,NumHTetaInt;
//  MeanFieldFormSinCos formH;
//  double Frequency,Coef2D_3D;
//
//  MeanFieldStruct(int outphi,int numhtetaint,int coef2d_3d,
//                  double frequency,double anisteta,double anispower)
//                  :formH(anisteta,anispower)
//
//  {
//    OutPhi=outphi;NumHTetaInt=numhtetaint;Coef2D_3D=coef2d_3d;
//    Frequency=frequency;
//  }
//};
//
//void MeanFieldHamInt(MeanFieldStruct &Field,
//                 double H,double Qs,double Is,double Wid,double Teta,double Phi,
//                 double Eta,
//                 double *Xval,double *Res);
//
//
//
//
