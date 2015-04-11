#include <lib\precompiled\eos.h>
#include "matt_2phase.h"



MatterStable* MatterStable::Current=NULL;


static ClassDesc MatterStable_ClassDesc(typeid(MatterStable),"MatterStable",1,"Matter_category",create<MatterStable>);
static ClassDesc MatterLiqSol_ClassDesc(typeid(MatterLiqSol),"MatterLiqSol",1,"Matter_category",create<MatterLiqSol>);
//#ifdef InternalPointsUrs 
static ClassDesc MatterBinodalSpinodal_ClassDesc(typeid(MatterBinodalSpinodal),"MatterBinodalSpinodal",1,"Matter_category",create<MatterBinodalSpinodal>);
//#endif


static ClassDesc TwoPhaseBoundary_ClassDesc(typeid(MatterStable::TwoPhaseBoundary),"TwoPhaseBoundary",1,"MatterBnd_category",create<MatterStable::TwoPhaseBoundary>);
static ClassDesc LiquidSolidBnd_ClassDesc(typeid(MatterLiqSol::LiquidSolidBnd),"LiquidSolidBnd",1,"MatterBnd_category",create<MatterLiqSol::LiquidSolidBnd>);

static ClassDesc FreeELiqSol_ClassDesc(typeid(FreeELiqSol),"FreeELiqSol",1,"MatterFreeE_category",create<FreeELiqSol>);
static ClassDesc FreeELiqSol_Bnd_ClassDesc(typeid(FreeELiqSol_Bnd),"FreeELiqSol_Bnd",1,"",create<FreeELiqSol_Bnd>);











MatterStable::TwoPhaseBoundary* MatterStable::TwoPhaseBoundary::TwoPhaseBoundaryStaticPtr = NULL;

double MatterStable::TwoPhaseBoundary::IntegralFunc_PV_Int(double v) { 
  double res=IntegralSpline_P_V.Evaluate(v);
  if (IntegralFunc_Negative) res= -res;//+IntegralFunc_Add;
  res+=IntegralFunc_Add;
//cout<<v<<"   "<<res<<"\n";
  return res;
};
double MatterStable::TwoPhaseBoundary::ZeroFunc_PV_Int(double pres) { 
  double Vmin,Vmax,Smin=ZeroFunc_Spl_Vmin,Smax=ZeroFunc_Spl_Vmax,Ds=Smax-Smin;//,PresInt,ResErr
  Vmin=Smin-Ds/2;Vmax=Smax+Ds/2;
  IntegralFunc_Negative=0;IntegralFunc_Add=-pres;

  //cout<<" pres "<<pres<<" Vmin "<<1/Dmax<<" pmin "<<Mat->Pressure(Dmax,T)<<" spl "<<IntegralSpline_P_V.Evaluate(1/Dmax)<<" vmax "<<Smin<<" pmax "<<Mat->Pressure(1/Smin,T)<<"\n";

  Fzero(IntegralFunc_PV_Int_Static,1/Dmax,Smin,Vmin,ZeroFunc_Misf*0.1,ZeroFunc_Misf*0.1,100);
  Fzero(IntegralFunc_PV_Int_Static,Smax,1/Dmin,Vmax,ZeroFunc_Misf*0.1,ZeroFunc_Misf*0.1,100);
  //Fzero(IntegralFunc_PV_Int_Static,Smin-Ds,Smin,Vmin,ZeroFunc_Misf*0.1,ZeroFunc_Misf*0.1,100);
  //Fzero(IntegralFunc_PV_Int_Static,Smax,Smax+10000*Ds,Vmax,ZeroFunc_Misf*0.1,ZeroFunc_Misf*0.1,100);
  ZeroFunc_SplRes_Bmin=Vmin;ZeroFunc_SplRes_Bmax=Vmax;
//double MisPmin=fabs(Mat->Pressure(1/Vmin,T)-pres),
//       MisPmax=fabs(Mat->Pressure(1/Vmax,T)-pres);
//if (MisPmin+MisPmax>ZeroFunc_Misf) 
//cout<<" P "<<pres<<"\nmax V "<<Vmax<<" P "<<Mat->Pressure(1/Vmax,T)<<" spl "<<IntegralSpline_P_V.Evaluate(Vmax)<<
//"\nmin V " <<Vmin<<" Pmin "<<Mat->Pressure(1/Vmin,T)<<" spl "<<IntegralSpline_P_V.Evaluate(Vmin)<<"\n";

return Mat->FreeE(1/Vmin,T)-Mat->FreeE(1/Vmax,T)+ pres*(Vmin-Vmax);
}


int MatterStable::TwoPhaseBoundary::SetDP(VecCl &V, VecCl &D, VecCl &P, int &SpinIndMin, int &SpinIndMax){
    //int SpinIndMin, SpinIndMax;
    D = VecCl::MakeStepVector(NumPnt, log(Dmin), log(Dmax), 0);
    V=D;P=D;
    for(int k = 1;k<=NumPnt;k++){
        D[k] = exp(D[k]);
        V[k] = 1/D[k];
        P[k] = Mat->Pressure(D[k], T);
    }
    int  k = 2;
    while(P[k]>=P[k-1] && k<=NumPnt)
        k++;
    if (k>=NumPnt)
        return 0;
    SpinIndMin = k-1;
    while(P[k]<=P[k-1] && k<=NumPnt)
        k++;
    if (k>=NumPnt)
        return 0;
    SpinIndMax = k;
    return 1;
}
void MatterStable::TwoPhaseBoundary::PrepareStatic(VecCl &P, VecCl &V){
    TwoPhaseBoundaryStaticPtr = this;
    MakeSpline(IntegralSpline_P_V,NumSplPnt,Mis*0.0001,&P[1],&V[1],NumPnt," GenerateAllBnd P_V");
    //IntegralFunc_Negative=0;
    IntegralFunc_Add=0;
    ZeroFunc_Misf=Mis*0.1;
}
void MatterStable::TwoPhaseBoundary::FindMinMax(VecCl &V, int CentInd, double &ResV, double &ResP, int Min){
    IntegralFunc_Add=0;
    if (Min)
        IntegralFunc_Negative=0;
    else
        IntegralFunc_Negative=1;
    ResV = Fmin(IntegralFunc_PV_Int_Static,V[min(CentInd+2,NumPnt)], V[max(1,CentInd-2)],V[CentInd],Mis*0.1,ResP);
    if (!Min)
        ResP = -ResP;
}
map<Stroka, double> MatterStable::TwoPhaseBoundary::MakeBndPnt(){
    VecCl V, D, P;
    int SpinIndMin, SpinIndMax;
    map<Stroka, double> ret;
    ret["T"] = T;ret["DSr_T"] = 0;ret["DSl_T"] = 0;ret["Dr_T"] = 0;ret["Dl_T"] = 0;
    ret["P_T"] = 0;ret["E_T"] = 0;ret["dPdT_T"] = 0;
    if (!SetDP(V, D, P, SpinIndMin, SpinIndMax))
        return ret;
    ret["T"] = T;
    TwoPhaseBoundary *OldPtr = TwoPhaseBoundaryStaticPtr;
    PrepareStatic(P, V);

    double PT_From, PT_To;
    FindMinMax(V, SpinIndMax, ZeroFunc_Spl_Vmin, PT_From, 1);
    ret["DSr_T"] = 1/ZeroFunc_Spl_Vmin;
    FindMinMax(V, SpinIndMin, ZeroFunc_Spl_Vmax, PT_To, 0);
    ret["DSl_T"] = 1/ZeroFunc_Spl_Vmax;
    IntegralFunc_Negative=0;
    double PT_est=IntegralFunc_PV_Int_Static( (ZeroFunc_Spl_Vmin+ZeroFunc_Spl_Vmax)*0.5 );
    Fzero(ZeroFunc_PV_Int_Static, max(PT_From, sqrt(MathZer)), max(PT_To, sqrt(MathZer)), PT_est, Mis*0.1, Mis*0.1, 100);
    //Fzero(ZeroFunc_PV_Int_Static, PT_From, PT_To, PT_est, Mis*0.1, Mis*0.1, 100);
    ZeroFunc_PV_Int_Static(PT_est);
    //ret["P_T"] = PT_est;
    ret["Dl_T"] = 1/ZeroFunc_SplRes_Bmax;
    ret["Dr_T"] = 1/ZeroFunc_SplRes_Bmin;
    ret["P_T"] = 0.5*(Mat->Pressure(ret["Dr_T"], T) + Mat->Pressure(ret["Dl_T"], T) );
    ret["E_T"] = Mat->Energy(ret["Dr_T"], T);

    double E0=ret["E_T"];//,P0=Mat->Pressure(ret["Dr_T"],T);
    double E1=Mat->Energy(ret["Dl_T"],T);
    ret["dPdT_T"] = -(E0-E1)/(1/ret["Dr_T"]-1/ret["Dl_T"]);

ret["Pr_T"] = Mat->Pressure(ret["Dr_T"], T);
ret["Pl_T"] = Mat->Pressure(ret["Dl_T"], T);

ret["PSr_T"] = Mat->Pressure(ret["DSr_T"], T);
ret["PSl_T"] = Mat->Pressure(ret["DSl_T"], T);
//if (fabs(ret["Pr_T"]-ret["Pl_T"])>Mis) cout<<" BinodalBndPres l "<<ret["PSl_T"]<<" r "<<ret["PSr_T"]<<"\n";

    TwoPhaseBoundaryStaticPtr = OldPtr;
    return ret;
}


map<Stroka, double> MatterStable::TwoPhaseBoundary::ClcBnd(double Temp, double Dmin, double Dmax, double NumPnt, double NumSplPnt, FreeEIO *Mat, double Misfit){
    TwoPhaseBoundary Bnd;
    Bnd.T = Temp;Bnd.Mat = Mat;Bnd.Dmin = Dmin;Bnd.Dmax = Dmax;Bnd.Mis = Misfit;Bnd.NumPnt = NumPnt;Bnd.NumSplPnt = NumSplPnt;
    return Bnd.MakeBndPnt();
};
Stroka MatterStable::TwoPhaseBoundary::MakeSpline(CurveSpline &spl,int Nspl,double Misf,double *y,double *x,int NumX, const char *err_name){
    Stroka res;
    if (!spl.Generate(Nspl,Misf,0,x,y,NumX)) 
        throw info_except("GenerateTwoPhaseBnd SaveBound error Generating %s spline\n", err_name);
    res += Stroka(" MakeSpline Control ") +  err_name + Stroka("\n");
    for (int k=0;k<NumX-1;k++) { 
      double tmp=(x[k]+x[k+1])/2;
      //char a[256];
      //res<<x[k]<<" "<<y[k]<<" "<<spl.Evaluate(x[k])<<"\n";
      //res<<(y[k]+y[k+1])/2<<" "<<spl.Evaluate(tmp)<<"\n";
    }
    return res;
}

Stroka MatterStable::TwoPhaseBoundary::SetBnd(map<Stroka, vector<double> > &data, const Stroka &res_file_name, double SplMisfit, double NumSplPnt){
    double Dmin = 1e10, Dmax = -1, Tmin = 1e10, Tmax = -1, Num = data["T"].size();
    for(double i = 0;i<Num;i++){
        double t = data["T"][i], dmin = data["Dl_T"][i], dmax = data["Dr_T"][i];
        if (t>Tmax) Tmax = t;
        if (t<Tmin) Tmin = t;
        if (dmin<Dmin) Dmin = dmin;
        if (dmax>Dmax) Dmax = dmax;
    }
    TwoPhaseBoundary Bnd;
    Bnd.Dmax = Dmax;Bnd.Dmin = Dmin;
    Bnd.Tmax = Tmax;Bnd.Tmin = Tmin;
    Stroka ret;
    ret += MakeSpline(Bnd.P_T,NumSplPnt,SplMisfit*0.0001,&(data["P_T"][0]),&(data["T"][0]),Num,"P_T");
    ret += MakeSpline(Bnd.E_T,NumSplPnt,SplMisfit,&(data["E_T"][0]),&(data["T"][0]),Num,"E_T");
    ret += MakeSpline(Bnd.dPdT_T,NumSplPnt,SplMisfit*1e-6,&(data["dPdT_T"][0]),&(data["T"][0]),Num,"dPdT_T");

    ret += MakeSpline(Bnd.Dr_T,NumSplPnt,SplMisfit,&(data["Dr_T"][0]),&(data["T"][0]),Num,"Dr_T");
    ret += MakeSpline(Bnd.Dl_T,NumSplPnt,SplMisfit,&(data["Dl_T"][0]),&(data["T"][0]),Num,"Dl_T");
    ret += MakeSpline(Bnd.DSr_T,NumSplPnt,SplMisfit,&(data["DSr_T"][0]),&(data["T"][0]),Num,"DSr_T");
    ret += MakeSpline(Bnd.DSl_T,NumSplPnt,SplMisfit,&(data["DSl_T"][0]),&(data["T"][0]),Num,"DSl_T");

    Bnd.save_data_state(FilterTextOut(res_file_name.c_str()));
    return ret;
};
