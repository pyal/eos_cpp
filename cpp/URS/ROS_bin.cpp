#include <lib/precompiled/eos.h>
#include "ros_bin.h"
//#include "Quad.h"


void ClcYHs(MixtureComposition &mixt, double *y, double &niu) {
    double o3 = 1. / 3;
    double niu1, niu2, alpha;
    mixt.get_Niu(niu1, niu2);
    alpha = mixt.get_Alpha();
    double n1_ = (1 - alpha) * niu1, n2_ = alpha * niu2;
    double arg1 = n1_ * n2_ * n2_, arg2 = n1_ * n1_ * n2_;
    double niu12_ = 0, niu21_ = 0;
    if(arg1 > sqr(StndErr * StndErr)) {
        niu12_ = pow((double)arg1, (double)o3);
        //       niu12_=pow(arg1,o3);
    }
    if(arg2 > sqr(StndErr * StndErr)) {
        niu21_ = pow((double)arg2, (double)o3);
        //     niu21_=pow(arg2,o3);
    }

    niu = niu1 + niu2;
    y[1] = (n1_ + n2_ - niu12_ - niu21_) / niu;
    y[2] = (niu1 * niu2 + niu21_ * (niu1 - 2 * niu2) + niu12_ * (niu2 - 2 * niu1)) /
           sqr(niu);
    y[3] =
        pow(pow(alpha * sqr(niu1), o3) + pow((1 - alpha) * sqr(niu2), o3), 3) / sqr(niu);
}
// for unity mass
double BinHS_Corr(MixtureComposition &mixt) {
    double y[4], niu;
    ClcYHs(mixt, &y[0], niu);
    //  double kt=M_Rconst/MolVeight*T;
    //  double a=mixt.get_Alpha(),m1,m2;mixt.get_MolVeight(m1,m2);
    //  double kt=M_Rconst*T/mixt.get_MeanMolVeight();
    //  return kt*
    return (
        1.5 * (1 + y[1] - y[2] - y[3]) + (3 * y[2] + 2 * y[3]) / (1 - niu) +
        (y[3] - 1) * log(1 - niu) + 1.5 * (1 - y[1] - y[2] - y[3] / 3) / sqr(1 - niu) -
        3);
}

static BinMinConfigParam *BinRoss6Min;

double CorBinMinFunc(double *niu_get, int n_nop) {
    double niu[2];
    niu[0] = niu_get[0];
    niu[1] = niu_get[1];
    niu[0] = exp(niu[0]);
    niu[1] = exp(niu[1]);
    if((niu[0] < 0) || (niu[1] < 0) || (niu[0] + niu[1] >= 1 - StndErr))
        return (fabs(niu[0]) + fabs(niu[1]) + 1) * 1e6;
    MixtureComposition *mixt = BinRoss6Min->mixt;
    mixt->set_Niu(niu[0], niu[1]);
    double alpha = mixt->get_Alpha();
    double F_Corr = BinHS_Corr(*mixt);
    double C = M_Na / 1e24;
    //   SmoothLimitUp=StndErr;SmoothLimitLo=StndErr*2;

    double Int1 = 0, Int2 = 0, Int12 = 0, Diameter1 = 0, Diameter2 = 0;
    if(alpha > sqr(StndErr))
        Diameter1 =
            pow(6 * niu[0] / (BinRoss6Min->S_DensDia * alpha * M_PI * C),
                1. / 3);   //*2.238515;// To obtain S_Diameter in Bor radius
    if(1 - alpha > sqr(StndErr))
        Diameter2 =
            pow(6 * niu[1] / (BinRoss6Min->S_DensDia * (1 - alpha) * M_PI * C),
                1. / 3);   //*2.238515;// To obtain S_Diameter in Bor radius
    if(Diameter1 > sqr(StndErr))
        Int1 = IntExp6Correlation(mixt->Exp6[0], niu[0], Diameter1);

    //   Diameter1=pow(6*1e-7/(BinRoss6Min->S_DensDia*alpha*M_PI*C),1./3);//*2.238515;// To obtain S_Diameter in Bor radius
    //   if (Diameter1>sqr(StndErr))  Int1=IntExp6Correlation(mixt->Exp6[0],1e-7,Diameter1);


    if(Diameter2 > sqr(StndErr))
        Int2 = IntExp6Correlation(mixt->Exp6[1], niu[1], Diameter2);

    double niu_ = pow(pow(niu[0], 1. / 3) + pow(niu[1], 1. / 3), 3) / 8;
    double niu_mix = pow(pow(niu[0] / max<double>(alpha, StndErr), 1. / 3) +
                             pow(niu[1] / max<double>(1 - alpha, StndErr), 1. / 3),
                         3) /
                     8;
    double Diameter_ =
        pow(6 * niu_mix / (BinRoss6Min->S_DensDia * M_PI * C),
            1. / 3);   //*2.238515;// To obtain S_Diameter in Bor radius

    if((Diameter1 > sqr(StndErr)) && (Diameter2 > sqr(StndErr)))
        Int12 = IntExp6Correlation(mixt->Exp6[2], niu_, Diameter_) * niu_mix / niu_;

    double Int = alpha * Int1 + 2 * alpha * (1 - alpha) * Int12 + (1 - alpha) * Int2;

    double ret = (BinRoss6Min->S_Temp * F_Corr + BinRoss6Min->S_Rconst * Int);
    //   double ret=(BinRoss6Min->S_Temp*F_Corr);
    //cout<<" Diameter1 "<<Diameter1<<" Diameter2 "<<Diameter2<<"\n";
    //cout<<" niu "<<niu[0]<<" "<<niu[1]<<" FC "<<F_Corr<<" I1 "<<Int1<<" I2 "<<Int2<<" I12 "<<Int12<<" ret "<<ret<<"\n";cout.flush();
    //cout<<" niu "<<niu[0]<<" "<<niu[1]<<" ret "<<ret<<"\n";cout.flush();
    return ret;
};

int MinimizePar;
double FixedPar;
double MinimizeFunc(double par) {
    double niu[2];
    niu[MinimizePar] = par;
    niu[(MinimizePar == 0) ? 1 : 0] = FixedPar;
    return CorBinMinFunc(&niu[0], 2);
}

#include "mat/NdimMin.h"
double CheckInLim(double X, double From, double To, int IncBounds) {
    //   double ret=0;
    if(In_Lim(X, From, To, IncBounds))
        return 0;
    //   cout<<" X "<<X<<" From "<<From<<" To "<<To<<"\n";
    return 1e5 * sqr(min(fabs(From * 1.1 - X), fabs(To * 0.9 - X))) + 1e5;
}
struct MinFuncClass : DeriveFuncClass {
    double *MinV, *MaxV, CorDeltaCoef;
    MinFuncClass(double *minv, double *maxv, double cordeltacoef)
        : MinV(minv), MaxV(maxv), CorDeltaCoef(cordeltacoef){};
    using DeriveFuncClass::Execute;
    double Execute(VecCl &par) {
        //if (!In_Lim(par[1],MinV[0],MaxV[0],0)) return 1e50;
        //if (!In_Lim(par[2],MinV[1],MaxV[1],0)) return 1e50;
        double Add = 0;
        Add += CheckInLim(par[1], MinV[0], MaxV[0], 1);
        Add += CheckInLim(par[2], MinV[1], MaxV[1], 1);
        if(Add != 0)
            return Add;
        return CorBinMinFunc(&par[1], 1) + CorDeltaCoef * sqr(par[1] - par[2]);
    }
    double CorCoef(double cordeltacoef = -1) {
        if(cordeltacoef != -1)
            CorDeltaCoef = cordeltacoef;
        return CorDeltaCoef;
    }
};
#include "mat/multimin.h"
#include "mat/quad.h"
int LargeNiuStep(double *niu)
//{return 0;}
{
    return ((niu[0] == -5) && (niu[1] == -5)) ? 1 : 0;
}
void SetLargeNiuStep(double *niu) {
    niu[0] = -5;
    niu[1] = -5;
}
void ClcMinNiu_New(double *niu, double *low, double *up, double &MinE, double FuncAccur) {
    // To be updated
    //MinFindNDimClass
    //  double MaxNiuStp=1;
    Ref<MinFuncClass> minFunc = new MinFuncClass(low, up, 0);
    Ref<Derive2DStd> D2 = new Derive2DStd(minFunc);
    double MinError = FuncAccur;

    double ResError = MinError, FuncError = MinError, Min1D_Coef = 1.2;
    int Min1D_MaxLinSrch = 5, Min1D_MaxRetry = 5;
    Ref<MinFind1DClass> min1d = new MinFind1DClass(
        NULL, ResError, FuncError, Min1D_Coef, Min1D_MaxLinSrch, Min1D_MaxRetry);
    Ref<MultiDimMinimizer> Minimizer =
        new EigenVectMinim(D2, min1d, 1e-8, 1e-15, FuncError, 2);
    MinFindNDimClass MinNd(Minimizer, ResError);
    //MinFind1DClass *min1d=new MinFind1DClass(1,NULL);
    //MinFindNDimClass MinNd(D2,min1d,1e-10,1e-8,MinError,MinError);
    VecCl TmpPar(2), TmpStp(2);
    TmpPar[1] = niu[0];
    TmpPar[2] = niu[1];
    TmpStp[1] = min(fabs(niu[0] - low[0]), fabs(niu[0] - up[0]));
    TmpStp[1] = 0.5;
    TmpStp[2] = min(fabs(niu[1] - low[1]), fabs(niu[1] - up[1]));
    TmpStp[2] = 0.5;
    int Cont = 1, MaxIt = 5, NumT = MaxIt, k;
    if(LargeNiuStep(niu))
        minFunc->CorCoef(0.4);
    else
        minFunc->CorCoef(0.02);
    for(k = 1; ((k <= NumT) && (Cont)); k++)
        Cont = MinNd.Execute(TmpPar, TmpStp, -1, MinE);
    //cout<<" NumIter "<<k<<"  "<<" Tmp "<<TmpPar[1]<<" "<<TmpPar[2]<<"\n";;
    //  if ((k>MaxIt)&& (!LargeNiuStep(niu)))
    //  {SetLargeNiuStep(niu);ClcMinNiu_New(niu,low,up,MinE,FuncAccur);return;}
    if((LargeNiuStep(niu))) {
        niu[0] = TmpPar[1] * 0.948340978;
        niu[1] = TmpPar[2] * 1.0579653;
        ClcMinNiu_New(niu, low, up, MinE, FuncAccur);
        return;
    }
    //  {niu[0]=TmpPar[1]*1.0579653;niu[1]=TmpPar[2]*0.952340978;ClcMinNiu_New(niu,low,up,MinE,FuncAccur);return;}

    if((k > 0)) {
        minFunc->CorCoef(0.0);
        TmpStp = TmpStp * 0 + 0.2;
        Cont = 1;
        TmpPar[1] = TmpPar[1] * 0.998;
        TmpPar[2] = TmpPar[2] * 1.002;
        for(k = 1; ((k <= NumT) && (Cont)); k++)
            Cont = MinNd.Execute(TmpPar, TmpStp, -1, MinE);
        if(k > 0) {
            //      TmpPar=TmpPar*1.02;
            //cout<<" NumIter "<<k<<"  ";
            int s = k;
            minFunc->CorCoef(0.0);
            TmpPar[1] = TmpPar[1] * 1.001;
            TmpPar[2] = TmpPar[2] * 0.999;
            TmpStp = TmpStp * 0 + 0.1;
            Cont = 1;
            for(k = 1; ((k <= NumT) && (Cont)); k++)
                Cont = MinNd.Execute(TmpPar, TmpStp, -1, MinE);
            k += s;

            //        minFunc->CorCoef(0.0);
            //      TmpPar[1]=TmpPar[1]*1.001;
            //      TmpPar[2]=TmpPar[2]*0.999;
            //      TmpStp=TmpStp*0+0.1;Cont=1;
            //      for (k=1;((k<=NumT)&&(Cont));k++) Cont=MinNd.Execute(TmpPar,TmpStp,-1,MinE);
        }
    } else
        k = 0;
    niu[0] = TmpPar[1];
    niu[1] = TmpPar[2];
    //cout<<" NumIter "<<k<<" niu "<<niu[0]<<" "<<niu[1]<<" MinE "<<MinE<<"\n";
}
void ClcMinNiu(double *niu, double *low, double *up, double &MinE, double FuncAccur) {
    double ResFuncVal;
    FixedPar = niu[1];
    MinimizePar = 0;
    niu[0] = Fmin(MinimizeFunc, low[0], up[0], niu[0], 1e-2, ResFuncVal);
    FixedPar = niu[0];
    MinimizePar = 1;
    niu[1] = Fmin(MinimizeFunc, low[1], up[1], niu[1], 1e-3, ResFuncVal);
    FixedPar = niu[1];
    MinimizePar = 0;
    niu[0] = Fmin(MinimizeFunc, low[0], up[0], niu[0], 1e-4, ResFuncVal);
    FixedPar = niu[0];
    MinimizePar = 1;
    niu[1] = Fmin(MinimizeFunc, low[1], up[1], niu[1], 1e-4, ResFuncVal);
    int err;
    double ResErr = sqrt(FuncAccur), MaxFun = 2, MaxIter = 2;
    double stp[2];
    stp[0] = stp[1] = FuncAccur * 10;
    ResErr = 0.0002;
    NashMin_Mach_Epsilon = FuncAccur;
    MinE = UncMin(
        err,
        2,
        &niu[0],
        &niu[0],
        MinE,
        CorBinMinFunc,
        &stp[0],
        NULL,
        FuncAccur,
        ResErr,
        MaxFun,
        MaxIter,
        low,
        up,
        10,
        0.25);
}
int CheckInLim(
    MixtureComposition &mixt,
    double Denc,
    double T,
    double AccurDA,
    double AccurT) {
    //return 0;
    if((fabs(mixt.L_Denc - Denc) < AccurDA * Denc) && (fabs(mixt.L_T - T) < AccurT) &&
       (fabs(mixt.L_alpha - mixt.alpha) < AccurDA))
        return 1;
    return 0;
}
double FreeBinaryCor(MixtureComposition &mixt, double Denc, double T) {
    if(mixt.ClcInteractPart == 0)
        return 0;
    if(CheckInLim(mixt, Denc, T, 0.001 * StndErr, 0.5))
        return mixt.L_FreeE;
    BinMinConfigParam MinPar;
    BinRoss6Min = &MinPar;
    MinPar.mixt = &mixt;
    double MolVeight = mixt.get_MeanMolVeight();
    MinPar.S_Temp = M_Rconst / MolVeight * T;
    MinPar.S_Rconst = M_Rconst / MolVeight;
    MinPar.S_DensDia = Denc / MolVeight;


    double MinE = mixt.L_FreeE, FuncAccur;
    double niu[2], low[2], up[2];
    low[0] = low[1] = 1e-5;   //stp[2]
    up[0] = mixt.Exp6[0].CutDiameter_Niu;
    up[1] = mixt.Exp6[1].CutDiameter_Niu;

    IntExp6Correlation_IntMethod = Exp6Param::ROS_INTEGRAL_SMOOTH_PLUSNIU;
    SmoothLimitUp = low[0] * 20;
    SmoothLimitLo = low[0];
    //   IntExp6Correlation_IntMethod=Exp6Param::ROS_INTEGRAL_SMOOTH;
    up[0] *= mixt.alpha;
    up[1] *= (1 - mixt.alpha);
    up[0] = max<double>(up[0], 0.1 * sqr(StndErr));
    up[1] = max<double>(up[1], 0.1 * sqr(StndErr));
    mixt.get_Niu(niu[0], niu[1]);

    up[1] = log(up[1]);
    up[0] = log(up[0]);
    low[1] = log(low[1]);
    low[0] = log(low[0]);
    niu[0] = log(niu[0]);
    niu[1] = log(niu[1]);
    if(!CheckInLim(mixt, Denc, T, 0.1, max<double>(500, T * 0.05)))
        SetLargeNiuStep(niu);
    //   FuncAccur=1e-6;
    FuncAccur = StndErr;
    ClcMinNiu_New(niu, low, up, MinE, FuncAccur);

    niu[0] = exp(niu[0]);
    niu[1] = exp(niu[1]);


    mixt.L_Denc = Denc;
    mixt.L_T = T;
    mixt.L_FreeE = MinE;
    mixt.L_alpha = mixt.alpha;
    mixt.set_Niu(niu[0], niu[1]);
    //cout<<" \n====================================\nFound Min "<<MinE<<" niu "<<niu[0]<<" "<<niu[1]<<" Denc "<<Denc<<" T "<<T<<" NumCalls "<<NumGradCalls<<"\n====================================\n";cout.flush();
    return MinE;
};


/*
double FreeBinaryCor(MixtureComposition &mixt,double Denc,double T)
{
   if (mixt.ClcInteractPart==0) return 0;
   if ( (fabs(mixt.L_Denc-Denc)<0.001*StndErr*Denc) && (fabs(mixt.L_T-T)<0.5) 
         && (fabs(mixt.L_alpha-mixt.alpha)<0.001*StndErr) ) return mixt.L_FreeE;
   BinMinConfigParam MinPar;BinRoss6Min=&MinPar;MinPar.mixt=&mixt;
   double MolVeight=mixt.get_MeanMolVeight();
   MinPar.S_Temp=M_Rconst/MolVeight*T;
   MinPar.S_Rconst=M_Rconst/MolVeight;
   MinPar.S_DensDia=Denc/MolVeight;



   double MinE=mixt.L_FreeE,Min_E=MinE;
   double FunAccur,ResErr,MaxFun,MaxIter;
   double niu[2],low[2],up[2],stp[2];low[0]=low[1]=1e-5;
   up[0]=mixt.Exp6[0].CutDiameter_Niu;up[1]=mixt.Exp6[1].CutDiameter_Niu;

   IntExp6Correlation_IntMethod=Exp6Param::ROS_INTEGRAL_SMOOTH_PLUSNIU;SmoothLimitUp=low[0]*20;SmoothLimitLo=low[0];
//   IntExp6Correlation_IntMethod=Exp6Param::ROS_INTEGRAL_SMOOTH;
   up[0]*=mixt.alpha;up[1]*=(1-mixt.alpha);
   up[0]=max<double>(up[0],0.1*sqr(StndErr));up[1]=max<double>(up[1],0.1*sqr(StndErr));

up[1]=log(up[1]);up[0]=log(up[0]);low[1]=log(low[1]);low[0]=log(low[0]);
   mixt.get_Niu(niu[0],niu[1]);
   niu[0]=log(niu[0]);niu[1]=log(niu[1]);

   FixedPar=niu[1];MinimizePar=0;niu[0]=Fmin(MinimizeFunc,low[0],up[0],niu[0],1e-2,Min_E);
   FixedPar=niu[0];MinimizePar=1;niu[1]=Fmin(MinimizeFunc,low[1],up[1],niu[1],1e-3,Min_E);
   FixedPar=niu[1];MinimizePar=0;niu[0]=Fmin(MinimizeFunc,low[0],up[0],niu[0],1e-4,Min_E);
   FixedPar=niu[0];MinimizePar=1;niu[1]=Fmin(MinimizeFunc,low[1],up[1],niu[1],1e-4,Min_E);
   int err;

//   niu[0]=max<double>(0.001,niu[0]);niu[1]=max<double>(0.001,niu[1]);
//   FunAccur=1e-6;ResErr=sqrt(FunAccur);MaxFun=2;MaxIter=2;
   FunAccur=1e-6;ResErr=sqrt(FunAccur);MaxFun=2;MaxIter=2;
   stp[0]=stp[1]=FunAccur*10;
//cout<<" FreeBinaryCor Rough\n";
//   IntExp6Correlation_IntMethod=Exp6Param::ROS_INTEGRAL_SMOOTH;CorBinMinFunc(&low[0],2);// To reset old values of params
ResErr=0.0002;
//low[0]=max<double>(niu[0]-ResErr*10,low[0]);
//low[1]=max<double>(niu[1]-ResErr*10,low[1]);
//up[0]=min(niu[0]+ResErr*10,up[0]);
//up[1]=min(niu[1]+ResErr*10,up[1]);
//double MultiMin(int &Error,int N,double *x,double (*FuncClc)(double *x,int n) ,
//                void (*SecDerivClc)(VecCl &x,VecCl &deriv,MatrCl &deriv2),
//                double XMisf,int MaxIterStep,int MaxIter,double &ResErr,
//                double *GradStp,double *BoundMin,double *BoundMax,
//                double *MaxLineSearchStp,
//                int LineSearchMeth);
//    MinE=MultiMin(err,2,&niu[0],CorBinMinFunc,NULL,ResErr,3,3,ResErr,&stp[0],
//                  low,up,NULL,1 );
   NashMin_Mach_Epsilon=FunAccur;
    MinE=UncMin(err,2,&niu[0],&niu[0],MinE,
               CorBinMinFunc,&stp[0],NULL,
               FunAccur,ResErr,MaxFun,MaxIter,
               low,up,10,0.25 );
//    MinE=UncMin(err,2,&niu[0],&niu[0],MinE,
//               CorBinMinFunc,&stp[0],NULL,
//               FunAccur,ResErr,MaxFun,MaxIter,
//               low,up,10,0.25 );


//    up[0]=mixt.Exp6[0].CutDiameter_Niu;up[1]=mixt.Exp6[1].CutDiameter_Niu;
//    up[0]*=mixt.alpha;up[1]*=(1-mixt.alpha);
//    up[0]=max<double>(up[0],0.1*sqr(StndErr));up[1]=max<double>(up[1],0.1*sqr(StndErr));
//    int badrestr=0,k;
//    for (k=0;k<2;k++) if (niu[k]>up[k]) {badrestr=1;niu[k]=up[k];}
//    if (badrestr) MinE=CorBinMinFunc(niu,2);

niu[0]=exp(niu[0]);niu[1]=exp(niu[1]);

   mixt.L_Denc=Denc;mixt.L_T=T;mixt.L_FreeE=MinE;mixt.L_alpha=mixt.alpha;
   mixt.set_Niu(niu[0],niu[1]);
//cout<<" \n====================================\nFound Min "<<MinE<<" niu "<<niu[0]<<" "<<niu[1]<<" Denc "<<Denc<<" T "<<T<<" NumCalls "<<NumGradCalls<<"\n====================================\n";cout.flush();
   return MinE;
};
*/


/*
double FreeBinaryCor(MixtureComposition &mixt,double Denc,double T)
{
   if (mixt.ClcInteractPart==0) return 0;
   if ( (fabs(mixt.L_Denc-Denc)<0.001*StndErr*Denc) && (fabs(mixt.L_T-T)<0.5) 
         && (fabs(mixt.L_alpha-mixt.alpha)<0.001*StndErr) ) return mixt.L_FreeE;
   BinMinConfigParam MinPar;BinRoss6Min=&MinPar;MinPar.mixt=&mixt;
   double MolVeight=mixt.get_MeanMolVeight();
   MinPar.S_Temp=M_Rconst/MolVeight*T;
   MinPar.S_Rconst=M_Rconst/MolVeight;
   MinPar.S_DensDia=Denc/MolVeight;

   double FunAccur=1e-4,ResErr=1e-2,MaxFun=100,MaxIter=1;
   double niu[2],low[2],up[2],stp[2];low[0]=low[1]=3e-5;
   up[0]=mixt.Exp6[0].CutDiameter_Niu;up[1]=mixt.Exp6[1].CutDiameter_Niu;
   up[0]*=mixt.alpha;up[1]*=(1-mixt.alpha);
   up[0]=max<double>(up[0],0.1*sqr(StndErr));up[1]=max<double>(up[1],0.1*sqr(StndErr));
//   stp[0]=up[0]*StndErr*100;stp[1]=up[1]*StndErr*100;
stp[0]=stp[1]=1e-4;
   mixt.get_Niu(niu[0],niu[1]);
   int err;
//   NashMin_Mach_Epsilon=2e-16;IntegralInfinity=200;
   FunAccur=1e-2;ResErr=sqrt(FunAccur);MaxFun=3;MaxIter=3;
//   Min_niu=Fmin(Ross6MinFunc,StndErr,CE->CutDiameter_Niu,CE->L_Niu,StndErr,Min_E);
//   MinE=Fmin(Ross6MinFunc,StndErr,CE->CutDiameter_Niu,CE->L_Niu,StndErr,Min_E);
//   NashMin_Mach_Epsilon=FunAccur;
   double MinE=mixt.L_FreeE;
cout<<" FreeBinaryCor Rough\n";
//   IntExp6Correlation_IntMethod=Exp6Param::ROS_INTEGRAL_SMOOTH;CorBinMinFunc(&low[0],2);// To reset old values of params
    MinE=UncMin(err,2,&niu[0],&niu[0],MinE,
               CorBinMinFunc,&stp[0],NULL,
               FunAccur,ResErr,MaxFun,MaxIter,
               low,up,10,0.25 );
//   IntExp6Correlation_IntMethod=Exp6Param::ROS_INTEGRAL_STRICT;CorBinMinFunc(&low[0],2);// To reset old values of params
ResErr=0.01;
low[0]=max<double>(niu[0]-ResErr*10,low[0]);
low[1]=max<double>(niu[1]-ResErr*10,low[1]);
up[0]=min(niu[0]+ResErr*10,up[0]);
up[1]=min(niu[1]+ResErr*10,up[1]);
stp[0]=stp[1]=1e-5;
//   NashMin_Mach_Epsilon=2e-16;IntegralInfinity=200;
   FunAccur=1e-5;ResErr=sqrt(FunAccur);MaxFun=2;MaxIter=2;
//   NashMin_Mach_Epsilon=FunAccur;
cout<<" FreeBinaryCor Strict\n";
    MinE=UncMin(err,2,&niu[0],&niu[0],MinE,
               CorBinMinFunc,&stp[0],NULL,
               FunAccur,ResErr,MaxFun,MaxIter,
               low,up,0.3,0.9 );
//    }
//if (err!=0) cout<<" Error "<<err<<"\n";

   mixt.L_Denc=Denc;mixt.L_T=T;mixt.L_FreeE=MinE;mixt.L_alpha=mixt.alpha;
   mixt.set_Niu(niu[0],niu[1]);
cout<<" \n====================================\nFound Min "<<MinE<<" niu "<<niu[0]<<" "<<niu[1]<<" Denc "<<Denc<<" T "<<T<<"\n====================================\n";cout.flush();
   return MinE;
};

*/