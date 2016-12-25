#include <lib/precompiled/eos.h>

#include "fre_fac.h"

#include "mat/NdimMin.h"

static ClassDesc MatterFreeE_ClassDesc(typeid(MatterFreeE),"MatterFreeE",1,"Matter_category",create<MatterFreeE>);

static ClassDesc FreeETmp_ClassDesc(typeid(FreeETmp),"FreeETmp",1,"MatterFreeE_category",create<FreeETmp>);
static ClassDesc FreeEIdeal_ClassDesc(typeid(FreeEIdeal),"FreeEIdeal",1,"MatterFreeE_category",create<FreeEIdeal>);
static ClassDesc FreeEIdealElectron_ClassDesc(typeid(FreeEIdealElectron),"FreeEIdealElectron",1,"MatterFreeE_category",create<FreeEIdealElectron>);
static ClassDesc FreeEVanDerVals_ClassDesc(typeid(FreeEVanDerVals),"FreeEVanDerVals",1,"MatterFreeE_category",create<FreeEVanDerVals>);
static ClassDesc FreeEVirial_ClassDesc(typeid(FreeEVirial),"FreeEVirial",1,"MatterFreeE_category",create<FreeEVirial>);
static ClassDesc FreeEH2Liquid_ClassDesc(typeid(FreeEH2Liquid),"FreeEH2Liquid",1,"MatterFreeE_category",create<FreeEH2Liquid>);
static ClassDesc FreeEH2Solid_ClassDesc(typeid(FreeEH2Solid),"FreeEH2Solid",1,"MatterFreeE_category",create<FreeEH2Solid>);
static ClassDesc FreeEEmpiLiquidPres_ClassDesc(typeid(FreeEEmpiLiquidPres),"FreeEEmpiLiquidPres",1,"MatterFreeE_category",create<FreeEEmpiLiquidPres>);
static ClassDesc FreeEEmpiLiquid_ClassDesc(typeid(FreeEEmpiLiquid),"FreeEEmpiLiquid",1,"MatterFreeE_category",create<FreeEEmpiLiquid>);
static ClassDesc FreeEEmpiSolid_ClassDesc(typeid(FreeEEmpiSolid),"FreeEEmpiSolid",1,"MatterFreeE_category",create<FreeEEmpiSolid>);
static ClassDesc FreeEMetHCold_ClassDesc(typeid(FreeEMetHCold),"FreeEMetHCold",1,"MatterFreeE_category",create<FreeEMetHCold>);
static ClassDesc FreeEElectron_ClassDesc(typeid(FreeEElectron),"FreeEElectron",1,"MatterFreeE_category",create<FreeEElectron>);

static ClassDesc FreeEFormula_ClassDesc(typeid(FreeEFormula),"FreeEFormula",1,"MatterFreeE_category",create<FreeEFormula>);


MatterFreeE* MatterFreeE::Current=NULL;

// ===============================================================
// =============      InterfaceFreeEIO            =========================
// ===============================================================





void InterfaceFreeEIO::FreeE(double *F,double *Dencity,double *Temperature,int Num)
  {		for (int k=1;k<=Num;k++) { F[k]=FreeE(Dencity[k],Temperature[k]);}}
void InterfaceFreeEIO::Pressure(double *P,double *Dencity,double *Temperature,int Num)
  {		for (int k=1;k<=Num;k++) { P[k]=Pressure(Dencity[k],Temperature[k]);}}
void InterfaceFreeEIO::Energy(double *E,double *Dencity,double *Temperature,int Num)
  {		for (int k=1;k<=Num;k++) { E[k]=Energy(Dencity[k],Temperature[k]);}}
void InterfaceFreeEIO::SetTLimits(double &minT, double &maxT, double &stpTcoef, double &stpTmin) {
    minT = 50;
    maxT = 1e6;
    stpTcoef = 1e-2;
    stpTmin = 10;
}


// ===============================================================
// =============      FreeStandart       =========================
// ===============================================================

double MatterFreeE::AbsoluteMinTemperature = 0;

MatterFreeE::MatterFreeE(){
    FreeEPtr=NULL;
    SetNewMatter(new FreeEIdeal);
    L_T=1000;L_D=0;L_E=0;
    MinimTmin = 50;
    MinimTmax = 1e6;
    MinimT2EstpCoef = 3e-3;
    MinimT2EstpMin = 10;
};

void MatterFreeE::Pressure(double *P,double *Dencity,double *Energy,int Num)
  {
   double *T=new double[Num+1];
   Temperature(T,Dencity,Energy,Num);
   FreeEPtr->Pressure(P,Dencity,T,Num);
   delete []T;
  };
void MatterFreeE::Sound(double *S,double *Dencity,double *Energy,int Num)
{
   VecCl T(Num),Eplus(Num),Tplus(Num),Dplus(Num),Pplus(Num),P(Num);
   Temperature(T.Ptr,Dencity,Energy,Num);
   FreeEPtr->Pressure(P.Ptr,Dencity,T.Ptr,Num);
   Tplus= T * (1 + MinimT2EstpCoef) + MinimT2EstpMin;
   FreeEPtr->Energy(Eplus.Ptr,Dencity,Tplus.Ptr,Num);
   for (int k=1;k<=Num;k++)
    { 
     Dplus[k]=Dencity[k];
     if (Dencity[k]<MathZer) { cout<<"MatterFreeEIO::Sound; Dencity<MathZer :"
                                <<Dencity<<"\n";Tplus[k]=0;continue;}
     double dE=max<double>(Eplus[k]-Energy[k],MathZer);
     double dr=Dencity[k]+dE*sqr(Dencity[k])/max<double>(1e-4,P.Ptr[k]);
     if (fabs(dr)<MathZer) {Tplus[k]=0;continue;}
     Dplus[k]=Dplus[k]+dr;
    }
   FreeEPtr->Pressure(Pplus.Ptr,Dplus.Ptr,Tplus.Ptr,Num);
   for (int k=1;k<=Num;k++)   S[k]=sqrt( max<double>(0,Pplus[k]-P[k])/(Dplus[k]-Dencity[k]) );
  }; 

double MatterFreeE::TemperatureFind(double En,double Den,double Low,double Up,
                                    double Start,double Err,int NumIt)
{
   Current=this;
   E=En;D=Den;
   double T0=Start;
   int err;
#ifdef FZERO_TEMPERATURE_ITERATION
      if (4<(err=Fzero(DeltaFree,Low,Up,T0,Err,Err,100)) )
        {cout<<" Bad ZeroFzeroFreeE in MatterFreeE::TemperatureFind Not Zero\nfound T "
             <<T0<<" Mis "<<DeltaFree(T0)<<" err "<<err<<"\n";T0=Zero_Temp;}
#else
#ifdef FZERO_LOGTEMPERATURE_ITERATION
//                   ln(50)=3.91 ln(1e6)=13.8  ln(100)=4.6
    T0=log(Start);Low=log(Low);Up=log(Up);Err=0.1*Err;
    if (4<(err=Fzero(DeltaFreeLog,Low,Up,T0,Err,Err,100)) ) {
        cout<<" Bad ZeroFzeroFreeE in MatterFreeE::Temperature Not Zero\nfound T "
            <<exp(T0)<<" Mis "<<DeltaFreeLog(T0)<<" err "<<err<<"\n";
        T0 = MinimTmin;
    } else 
        T0=exp(T0);
#else
  MinFind1DClass *min1d=new MinFind1DClass(1,NULL);
  double MinError=Err,FoundEMis;
  int Cont=1,MaxIt=7,NumT=MaxIt,it;
  VecCl TmpPar(1),TmpStp(1);TmpPar[1]=L_T;TmpStp[1]=500;
  T0=Start;
#ifdef MULTIDIM_TEMPERATURE_ITERATION
  Derive2DStd *D2=new Derive2DStd(new DeriveStaticFunc(DeltaFree));
  MinFindNDimClass MinNd(D2,min1d,1e-10,1e-8,MinError);
  for (it=1;((it<=NumT)&&(Cont));it++) Cont=MinNd.Execute(TmpPar,TmpStp,1,FoundEMis);
  if (it>MaxIt)
      if (4<(err=Fzero(DeltaFree,Low,Up,T0,Err,Err,100)) )
        {cout<<" Bad ZeroFzeroFreeE in MatterFreeE::TemperatureFind Not Zero\nfound T "
             <<T0<<" Mis "<<DeltaFree(T0)<<" err "<<err<<"\n";T0=Zero_Temp;}
      else T0=TmpPar[1];
#else
  T0=log(Start);Low=log(Low);Up=log(Up);Err=0.1*Err;
  TmpPar[1]=log(TmpPar[1]);TmpStp[1]=log(TmpStp[1]);
  Derive2DStd *D2=new Derive2DStd(new DeriveStaticFunc(DeltaFreeLog));
  MinFindNDimClass MinNd(D2,min1d,1e-10,1e-8,MinError);
  for (it=1;((it<=NumT)&&(Cont));it++) Cont=MinNd.Execute(TmpPar,TmpStp,1,FoundEMis);
  if (it>MaxIt)
    if (4<(err=Fzero(DeltaFreeLog,Low,Up,T0,Err,Err,100)) )
     {cout<<" Bad ZeroFzeroFreeE in MatterFreeE::Temperature Not Zero\nfound T "
          <<exp(T0)<<" Mis "<<DeltaFreeLog(T0)<<" err "<<err<<"\n";T0=Zero_Temp;}
    else T0=exp(T0);
  else T0=exp(TmpPar[1]);

#endif
#endif
#endif
//#ifdef FZERO_TEMPERATURE_ITERATION
////   if (!ZeroNewtonIt(DeltaFree,T0,StndErr*0.1,100,1e6,Zero_Temp,0.8*T0,0.01))
////int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
////                  int MaxIter)
//    if (4<(err=Fzero(DeltaFree,50,1e6,T0,StndErr*0.1,StndErr*0.1,100)) )
//     {cout<<" Bad ZeroFzeroFreeE in MatterFreeE::Temperature Not Zero\nfound T "
//          <<T0<<" Mis "<<DeltaFree(T0)<<" err "<<err<<"\n";T0=Zero_Temp;}
////if (err!=0) cout<<" Mis E "<<DeltaFree(T0)<<" T "<<T0<<" err "<<err<<"\n";

  return T0;
}

//double MatterFreeE::MinTemperature = 3;
void MatterFreeE::Temperature(double *T,double *Dencity,double *Energy,int Num) {
    double T0=L_T;//,Delta;
    for (int k=1;k<=Num;k++) {
        if (fabs(Dencity[k]) < MathZer || fabs(L_D/Dencity[k] - 1 ) + fabs(L_E-Energy[k]) < 1e-6) {
            T[k]=L_T;
            continue;
        }
//      T0=TemperatureFind(Energy[k],Dencity[k],50,1e6,T0,StndErr*0.1,100);
        T0=TemperatureFind(Energy[k], Dencity[k], max<double>(AbsoluteMinTemperature, MinimTmin), MinimTmax,T0,StndErr*0.1,100);
//cout<<Dencity[k]<< " " << Energy[k] << " " << T0 << " " << max<double>(AbsoluteMinTemperature, MinimTmin) << " " << MinimTmax << "\n";
//      T0=TemperatureFind(Energy[k],Dencity[k],100,1e6,T0,StndErr*0.1,100);
        L_D=Dencity[k];
        L_E=Energy[k];
        L_T=T0;
        T[k]=T0;
    };
}                                          




//double MatterFreeE::DeltaFree(double T)
double MatterFreeE::Pressure(double Dencity,double Energy)
  {
   double T;
   if (fabs(L_D-Dencity)+fabs(L_E-Energy)<MathZer) T=L_T;
   else T=Temperature(Dencity,Energy);
   return FreeEPtr->Pressure(Dencity,T);
  }; 
double MatterFreeE::Sound(double Dencity,double Energy)
  {
   if (Dencity<MathZer) { cout<<"MatterFreeEIO::Sound; Dencity<MathZer :"
                        <<Dencity<<"\n";return 0;}
   double r=Dencity;
   double T;
   if (fabs(L_D-Dencity)+fabs(L_E-Energy)<MathZer) T=L_T;
   else T=Temperature(Dencity,Energy);
   double dT = T * MinimT2EstpCoef + MinimT2EstpMin;//*StndErr;
   double P=FreeEPtr->Pressure(r,T);
   double E=FreeEPtr->Energy(r,T);
   double dE=max<double>(FreeEPtr->Energy(r,T+dT)-E,MathZer);
   double dr=dE*sqr(Dencity)/max<double>(1e-4,P);
   if (fabs(dr)<MathZer) return 0;
   double Pp=FreeEPtr->Pressure(r+dr,T+dT)-P;
   if (Pp<0)
     {
//       cout<<" MatterIO::Sound;  deriv is negative: " <<Pp<<" Sqrt from negative value          \n";
     }
   double S=sqrt( max<double>(0,Pp)/dr );
   return S;

  }; 
//   double Sound(double Dencity,double Energy);
double MatterFreeE::Temperature(double Dencity,double Energy)
  {
    VecCl Den(1),En(1),Temper(1);Den[1]=Dencity;En[1]=Energy;
    Temperature(Temper.Ptr,Den.Ptr,En.Ptr,1);
    return Temper[1];
  }
//   if (fabs(L_D-Dencity)+fabs(L_E-Energy)<MathZer) return L_T;
//   double T0=L_T,Delta;
//   E=Energy;D=Dencity;
//   Current=this;
////   T0=1000;
//   int err;
////int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
////                             double Up,double Down,double MaxStp=MathInf) ;
//#ifdef FZERO_LOGTEMPERATURE_ITERATION
////                   ln(50)=3.91 ln(1e6)=13.8  ln(100)=4.6
//    T0=log(T0);
//    if (4<(err=Fzero(DeltaFreeLog,3.91,13.8,T0,StndErr*0.01,StndErr*0.01,100)) )
//     {cout<<" Bad ZeroFzeroFreeE in MatterFreeE::Temperature Not Zero\nfound T "
//          <<T0<<" Mis "<<DeltaFreeLog(T0)<<" err "<<err<<"\n";T0=Zero_Temp;}
//    else T0=exp(T0);
////if (err!=0) cout<<" Mis E "<<DeltaFreeLog(log(T0))<<" T "<<T0<<" err "<<err<<"\n";
//#else
//
//#ifdef FZERO_TEMPERATURE_ITERATION
////   if (!ZeroNewtonIt(DeltaFree,T0,StndErr*0.1,100,1e6,Zero_Temp,0.8*T0,0.01))
////int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
////                  int MaxIter)
//    if (4<(err=Fzero(DeltaFree,50,1e6,T0,StndErr*0.1,StndErr*0.1,100)) )
//     {cout<<" Bad ZeroFzeroFreeE in MatterFreeE::Temperature Not Zero\nfound T "
//          <<T0<<" Mis "<<DeltaFree(T0)<<" err "<<err<<"\n";T0=Zero_Temp;}
////if (err!=0) cout<<" Mis E "<<DeltaFree(T0)<<" T "<<T0<<" err "<<err<<"\n";
//#else
//#ifdef NEWTON_TEMPERATURE_ITERATION
//   if (!ZeroNewtonIt(DeltaFree,T0,StndErr*0.1,100,1e6,Zero_Temp,0.8*T0,0.01))
//     {cout<<" Bad ZeroNewtonFreeE in MatterFreeE::Temperature Not Zero\nfound T "
//          <<T0<<" Mis "<<DeltaFree(T0)<<"\n";T0=Zero_Temp;}
//#else
//   if (!FindZeroPos(DeltaFree,T0,Delta,0.01*StndErr,500,60))
//     {cout<<" Bad FindZeroPos in MatterFreeE::Temperature Not Zero\nfound T "
//          <<T0<<" Mis "<<Delta<<"\n";T0=Zero_Temp;}
//#endif
//#endif
//#endif
//
////      if (!ZeroMullerIt(DeltaFree,T0,StndErr*1e-2,10000))
////        {cout<<" Bad ZeroMullerIt in MatterFreeE::Temperature Not Zero\n";}
////      T0=ZeroIn(MathZer,100000,DeltaFree,StndErr*1e-2);
//   L_T=T0;L_D=Dencity;L_E=Energy;
//   return T0;
//  }                                          
double MatterFreeE::Energy(double Dencity,double Temperature)
 {if (fabs(L_D-Dencity)+fabs(L_T-Temperature)<MathZer) return L_E;
  else { L_T=Temperature;L_D=Dencity;L_E=FreeEPtr->Energy(Dencity,Temperature);return L_E;};
};



// ===============================================================
// =============      FreeEFormula         =========================
// ===============================================================
double FreeEFormula::FreeE(double Denc,double T){
    CalculatorProgam::GetCurPtr(&prg);
    *(double*)DataSimpleStorage::GetData( prg.GetVar("Denc")->dat ) = Denc;
    *(double*)DataSimpleStorage::GetData( prg.GetVar("Temp")->dat ) = T;
    prg.Execute();    
    double ret = *(double*)DataSimpleStorage::GetData( prg.GetVar("FreeE")->dat );
    CalculatorProgam::RestorePtr();
    return  ret;
};
void FreeEFormula::InitProgram(){
    strstream ins((char*)PrgStr.c_str(),(streamsize)PrgStr.length(),ios::in);
    prg.ResetProgram(new Lex_Calcul_Analyser(&ins));
    CalculatorProgam::GetCurPtr(&prg);
    prg.GenerateDouble(0,"FreeE");prg.GenerateDouble(0,"Denc");prg.GenerateDouble(0,"Temp");
    prg.Read();
    CalculatorProgam::RestorePtr();
}


// ===============================================================
// =============      FreeEIdeal         =========================
// ===============================================================
double FreeEIdeal::FreeE(double Denc,double T){
     if ((T<MathZer) || (Denc<MathZer)) 
        { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
     double kt=M_Rconst/MolVeight*T;
	 double Xrot=Trot/T,Erot=0,Evib=0;
	 if (Trot>StndErr) Erot=log(Xrot);//-Xrot/3-sqr(Xrot)/90;
	 if (Tvib>StndErr) {
         if (T > 20 * Tvib)
             Evib = log(Tvib / T);
         else
             Evib = log(1 - exp(-Tvib / T));
     }
	 double Evol=log(Denc);
	 double F=Zero+kt*(Evol+Erot+Evib-CvId*log(T)+HiT);
     return F;
    };
double FreeEIdeal::ClcAddHiT(double molveight, double cvid){
   //double kb=4.811975e-15,d;
   //d=log(2*M_PI*kb/molveight);
   //return d*3/2-1-log(molveight/M_Na);
     double Trans = M_ProtonMass_K*molveight/(2*M_PI*sqr(M_PlankCros_K));
     Trans = Trans / sqr(M_C);
     double ret = - cvid*log(Trans);//cvid=3/2
     ret = ret-(1+log(molveight/M_Na));
     return ret;
}
// ===============================================================
// =============      FreeEIdealElectron  ========================
// ===============================================================
double FreeEIdealElectron::FreeE(double Denc,double T){
     if ((T<MathZer) || (Denc<MathZer)) 
        { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
     double cvid = 1.5;
     double kt=M_Rconst/MolVeight*T;
	 double Evol=log(Denc);
	 double F=Zero+kt*(Evol-cvid*log(T)+HiT);
     return F;
    };
double FreeEIdealElectron::ClcAddHiT(double molveight){
    double cvid = 1.5, g = 2;
    double Trans = M_Mass_Electron_eV*M_eV_K*molveight/(2*M_PI*sqr(M_PlankCros_K));
    Trans = Trans / sqr(M_C);
    double ret = - cvid*log(Trans) - log(g);
    ret = ret-(1+log(molveight/M_Na));
    return ret;
}

// ===============================================================
// =============      FreeEVirial        =========================
// ===============================================================
//  Second virial is calculated in the Гельшфельдер ( Гильшфельдер )
//                                                        B
//  F=  Zero - R*T*ln(V) - R*T*C *ln(T) - HiT*R*T + R*T* ---
//                -k            v                         V
//  B_= Sum(b_k*t   )+delta(t)  B_*V0 = B  t*T0 = T
double FreeEVirial::FreeE(double Denc,double T)
    {
     if ((T<MathZer) || (Denc<MathZer)) 
        { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
     double s=0,tau=T/T0,tc=1,add=0;
     for (int i=1;i<=NCi;i++) {s+=Ci[i]*tc;tc/=tau;}
     if (He_delta) 
       {
        double x=tau/(100+1.8*tau);
        add=-0.2823762*x+0.0810073*sqr(x)-4.1242084*pow(x,6);
       }
     double F=Zero-(M_Rconst/MolVeight*T)*(log(max<double>(1/Denc,MathZer))+CvId*log(T)+HiT-
                    (s+add)*V0*Denc);
     return F;
    };

void FreeEVirial::ReadSubstancePar(istream &in)
  {
   char tmp[256];
   in>>tmp>>T0>>tmp>>V0>>tmp>>CvId>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT>>
       tmp>>NCi;
   V0=1000*V0/MolVeight;
   for (int k=1;k<=NCi;k++) in>>Ci[k];
   in>>tmp>>He_delta;
cout<<" He_delta "<<tmp<<"\n";
  };
void FreeEVirial::SaveSubstancePar(ostream &out){
    out<<" T0 "<<T0<<" V0 "<<V0<<" CvId "<<CvId<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT
        <<" NCi "<<NCi<<" ";
   for (int k=1;k<=NCi;k++) out<<Ci[k]<<" ";
   out<<"\t He_delta "<<He_delta<<" ";
  };



// ===============================================================
// =============      FreeEVanDerVals    =========================
// ===============================================================
//                     B       A
//  F= Zero-R*T*ln(1- ---) - ----- - R*T*ln(V) - R*T*C *ln(T) - HiT*R*T
//                     V       V                      v
double FreeEVanDerVals::FreeE(double Denc,double T)
    {
     if ((T<MathZer) || (Denc<MathZer)) 
       { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
     double F=-A*Denc-Zero-(M_Rconst/MolVeight*T)*
               (log(max<double>(1/Denc-B,MathZer))+CvId*log(T)+HiT);
     return F;
    };
void FreeEVanDerVals::ReadSubstancePar(istream &in)
    {
     char tmp[256];
     in>>tmp>>A>>tmp>>B>>tmp>>CvId>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT;
     in.getline(tmp,256);
cout<<" Pkr(GPa) "<<A/27/sqr(B)<<" Tkr(K) "<<8*A*MolVeight /(27*M_Rconst*B)
    <<" Rkr(g/cm3) "<<1/(3*B)<<"\n";

//   double kb=4.811975e-15,d;
//   d=log(2*M_PI*kb/MolVeight);
//   if (Trot>StndErr) HiT+=log(Trot);
      HiT += FreeEIdeal::ClcAddHiT(MolVeight,CvId);
      };
void FreeEVanDerVals::SaveSubstancePar(ostream &out){
    out<<" A "<<A<<" B "<<B<<" CvId "<<CvId<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT - FreeEIdeal::ClcAddHiT(MolVeight,CvId)<<"\n";
  };



double FreeEMetHCold::FreeE(double Denc,double T)
 {
if ((T<MathZer) || (Denc<MathZer)) { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
  double Rs=pow(2.696/Denc,1./3.);
  double K_Ryd=6.3344e-6;
  double kT=T*K_Ryd,Ze=1,G14,G,F=0,sige=-2.6;
//sige=0;
  G=sqr(Ze)/(Rs*kT/2); // Ry= 2* 1 a.e.e
  G14=pow(G,1./4.); 
  if (AddEG) F+=2.21/sqr(Rs)-0.916/Rs-0.88/(Rs+7.8)-1.792/Rs;
  if (AddLDA) F+=(-0.11382+0.003554*Rs-0.012707*sqr(Rs));
  if (AddOCP) 
  {
	  double a=0,b=0.9528,c=0.18907,d=-0.81773,e=-2.59;                     // Ross   notation
//	  double a=-0.89752,b=0.94544,c=0.17954,d=-0.80049,e;e=a+4*(b-c)+1.135; // DeWitt notation
	  F+=(a*G+4*(b*G14-c/G14)+d*log(G)-e+sige*10.85 )*kT;//10.85 - t=10000 - 11 - good dis
  }
  if (AddDeb) 
    {double TD=max<double>((Denc-DebDenc0)*DebKoef,100);
//cout<<" D "<<Denc<<" T "<<T<<" TD "<<TD<<"\n";
     F+=kT*(3*log(1-exp(-TD/T))-DebyFunc(TD/T));
    }; 
  if (AddTransl) F+=3*kT*log(G);
//  F+=sige;
  F*=1313;  // Conversion from Ry to (km/c)^2
  return F;
 };
//   double Energy(double Dencity,double Temperature)
//     {double dr=StndErr*Temperature;
//      double ret=-sqr(Temperature)/dr*
//             (FreeE(Dencity,Temperature+dr)/(Temperature+dr)-
//                FreeE(Dencity,Temperature)/Temperature);
//cout<<" D "<<Dencity<<" T "<<Temperature<<" E "<<ret<<"\n";
//      return ret;};
void FreeEMetHCold::ReadSubstancePar(istream &in)
  {char tmp[256];
   in>>tmp>>AddEG>>tmp>>AddLDA>>tmp>>AddOCP
     >>tmp>>AddDeb>>tmp>>DebDenc0>>tmp>>DebKoef>>tmp>>AddTransl;in.getline(tmp,256);
  }; 
void FreeEMetHCold::SaveSubstancePar(ostream &out){
    out<<" AddEG "<<AddEG<<" AddLDA "<<AddLDA<<" AddOCP "<<AddOCP<<" AddDeb "<<AddDeb
        <<" DebDenc0 "<<DebDenc0<<" DebKoef "<<DebKoef<<" AddTransl "<<AddTransl<<"\n"; 
};


/*
double *FreeERoss::S_ParInteract;
double FreeERoss::S_Niu,FreeERoss::S_Diameter,
       FreeERoss::S_DensDia,FreeERoss::S_Temp,FreeERoss::S_MaxR_PY;
const double FreeERoss::Rconst=8.31441e-3;
SplineXYIO *FreeERoss::S_PercYerv;

double FreeERoss::FreeE(double Denc,double T)
  {
   if ((T<MathZer) || (Denc<MathZer)) 
     { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
   S_Temp=Rconst/MolVeight*T;S_PercYerv=PercYerv;
   S_ParInteract=&ParInteract[0];S_DensDia=Denc/MolVeight;
   S_MaxR_PY=6;
//cout<<" MinCalc\n";
//   double PY_add=MinFunc(0.035,0.65,PY_Min,10*StndErr);
   double PY_add;
   if ((fabs(DL-Denc)<1e-2) && (fabs(TL-T)<1)) ;
//   else { DL=Denc;TL=T;NiuL=MinFunc(0.035,0.65,PY_Min,2e-3);}
   else { DL=Denc;TL=T;NiuL=MinFunc(0.035,0.62,PY_Min,2e-3);}
   PY_add=PY_Min(NiuL);
cout<<" F "<<NiuL<<"\n";
   double F=Zero+PY_add-S_Temp*
//   double F=Zero-PY_add-S_Temp*
            (log(max<double>(1/Denc,MathZer))+CvId*log(T)+HiT+log(1-exp(-Tvib/T)));
   return F;
  }; 
void FreeERoss::ReadSubstancePar(fstream &in)
    {
//cout<<" FreeERoss reading\n";
     DeleteVar();
     char tmp[256];
     in>>tmp>>CvId>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT>>tmp>>Tvib;
     in>>tmp>>ParInteract[1]>>tmp>>ParInteract[2]>>tmp>>ParInteract[3];
     in>>tmp>>ParInteract[0];
     in>>tmp>>tmp;//spl_name=strdup(tmp);
     fstream sp(tmp,ios::in);
     int ret=1;
//cout<<" FreeERoss spline registering. Name: "<<tmp<<"\n";
     if (!(ret=SurfRegister(sp,PercYerv))) 
         {cout<<" Could not register Percus. file: "<<tmp
              <<" :\nNow exititng\n";exit(0);}
cout<<" FreeERoss Entered. Coreleft "<<Coreleft()<<"\n";
     S_ParInteract=&ParInteract[0];
     double x=ParInteract[0]*(1+StndErr);
     double c=InteractFunc(x);
     double c1=InteractFunc(x*(1+StndErr));
     c1=(c1-c)/(x*StndErr);
     ParInteract[4]=c/(exp(c1/c*x));
     ParInteract[5]=c1/c;
//     return ret;
     };

*/

/*
double StndErr=1e-6;
void main(int argc,char *argv[])
  {
   fstream out("fre_fac.dat",ios::out);
   fstream in(argv[1],ios::in);
   FreeERoss fe;fe.ReadIni(in);
   
   double MinD=0.06,MaxD=1.3,MinT=100,MaxT=50000;StndErr=1e-5;
   int NumD=10,NumT=10;
   double MinD1=log(MinD),MaxD1=log(MaxD),MinT1=log(MinT),MaxT1=log(MaxT);
   double StpD=(MaxD1-MinD1)/NumD,StpT=(MaxT1-MinT1)/NumT;
   double CurD=MinD1+0.5*StpD,CurT=MinT1+0.5*StpT;
out<<" Log(Density)   Log(Temperature)   Niu  Diameter";
   while (CurD<MaxD1)
     {
      CurT=MinT1+0.5*StpT;
//CurNiu=0.5;
cout<<"\n======================================================\n"<<exp(CurD)<<"\n";
      while (CurT<MaxT1)
        {
         fe.FreeE(exp(CurD),exp(CurT));
         out<<exp(CurD)<<" "<<exp(CurT)<<" "<<fe.NiuL<<" "<<fe.S_Diameter<<" "<<fe.PY_Min(fe.NiuL)<<"\n";
         CurT+=StpT;
//cout<<"I";
        } 
      CurD+=StpD;
//cout<<" CurR "<<CurR<<" "<<Coreleft()<<"\n";
     }
   out.close();in.close();
  } 







*/
