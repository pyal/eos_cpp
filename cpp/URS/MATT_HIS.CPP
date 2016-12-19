#include <lib/precompiled/eos.h>
#include "matt_his.h"
#define TE_Zero 1e3

static ClassDesc MatterHisch_ClassDesc(typeid(MatterHisch),"MatterHisch",1,"Matter_category",create<MatterHisch>);
static ClassDesc MatterIVL_ClassDesc(typeid(MatterIVL),"MatterIVL",1,"Matter_category",create<MatterIVL>);
static ClassDesc MatterABuExpFit_ClassDesc(typeid(MatterABuExpFit),"MatterABuExpFit",1,"Matter_category",create<MatterABuExpFit>);


double MatterABuExpFit::S_A,MatterABuExpFit::S_B,MatterABuExpFit::S_TA,MatterABuExpFit::S_TB;
double MatterABuExpFit::S_TL,MatterABuExpFit::S_Denc,MatterABuExpFit::S_TP;
double MatterABuExpFit::S_TT0;
HischConst MatterABuExpFit::S_H;

//T(u);D(u);G(u);Dens
// Tmin,Umin,Umax,NumPnt
// CurveSpline *Ph_s,*Eh_s,*G_s,*T_s,*Teta0_s,*Cv_s;
TempParStruct *ConstTempPar;
#define CT ConstTempPar 
double TetaInt(double V)
  {
   double D=1/V,Grun=EvalSpl((*CT->data),0,5,(*CT->Grun_Denc),D);
//   return 1/Grun*D; 
   return Grun*D; 
  }; 

void GenerateTemp(X_func ShockU,X_func TempU,X_func GrunU,
                  double Misfit,double Denc,double Tmin,double Umin,double Umax,int NumPnt,TempParStruct *TempPar)
  {
   CT=TempPar;
//cout<<" Generating  Ptr "<<CT->Grun_Denc<<" NumPnt "<<NumPnt<<"\n";ChRead();

   delete CT->Grun_Denc;CT->Grun_Denc=NULL;delete CT->Temp_Denc;CT->Temp_Denc=NULL;
   delete CT->E_Denc;CT->E_Denc=NULL;      delete CT->Teta_Denc;CT->Teta_Denc=NULL;
   delete CT->Cv_Teta;CT->Cv_Teta=NULL;    delete CT->data;CT->data=NULL;
   CT->Tmin=Tmin;DataRegister("TDataF",CT->data);
   int I[6];for (int k=0;k<6;k++) I[k]=NumPnt;CT->data->SetDim(6,I);
   double CurU=Umin,StpU=(Umax-Umin)/(NumPnt-1),tmp;
   double ShockDeriv,TempDeriv,DerivStp=StpU*0.01,CurShock,CurGrun,CurTemp,CurDenc;
//cout<<" Making data \n";ChRead();
   for (int k=0;k<NumPnt;k++) 
     {
      tmp=ShockU(CurU);ShockDeriv=(ShockU(CurU+DerivStp)-tmp)/DerivStp;CurShock=tmp;
      tmp=GrunU(CurU);
      CT->data->D[5][k]=tmp;CurGrun=tmp;
      tmp=TempU(CurU);TempDeriv=(TempU(CurU+DerivStp)-tmp)/DerivStp;
      CT->data->D[2][k]=log(tmp+TE_Zero);CurTemp=tmp;
// Cv
      CT->data->D[3][k]=sqr(CurU)*ShockDeriv*(CurShock-CurU)/
                   (CurShock*(CurShock-CurU)*TempDeriv+
                            CurGrun*CurTemp*(ShockDeriv*CurU-CurShock) );
      CT->data->D[0][k]=(Denc*CurShock/(CurShock-CurU));
      CT->data->D[4][k]=log((0.5*sqr(CurU))+TE_Zero);
      CurU+=StpU;
     };
//cout<<" Constr Spl+Teta data CoreLeft "<<Coreleft()<<" "<<stackavail()<<"\n";ChRead();
   ConstructSpl((*CT->data),0,2,CT->Temp_Denc);
   ConstructSpl((*CT->data),0,4,CT->E_Denc);
   ConstructSpl((*CT->data),0,5,CT->Grun_Denc);
//cout<<" Integrating\n";ChRead();
   double integ,DencA;DencA=CT->data->D[0][0];
   for (int k=0;k<NumPnt;k++) 
     {
      CurDenc=CT->data->D[0][k];
      integ=RecInt(TetaInt,1/DencA,1/CurDenc,1e-6);
      CurTemp=exp(CT->data->D[2][k])-TE_Zero;
//      CT->data->D[1][k]=log(integ+0.5*(sqr(CurTemp)-sqr(Tmin))+TE_Zero);
      CT->data->D[1][k]=integ+log(CurTemp/Tmin);
//cout<<" k "<<k<<"\n";
     } 
//cout<<" Constr Spl end data \n";ChRead();
   ConstructSpl((*CT->data),0,1,CT->Teta_Denc);
   ConstructSpl((*CT->data),1,3,CT->Cv_Teta);
fstream out("data.tmp",ios::out);out<<" Density Teta log(T+1) Cv log(E+1) G \n";OutputTDataF(out,*CT->data);out.close();
   delete CT->Grun_Denc;CT->Grun_Denc=NULL;
//fstream out("data.tmp",ios::out);OutputTDataF(out,*CT->data);out.close();
//cout<<"  end data \n";//ChRead();
  };
double TempDenc0,TempEnergy0,TempTHug,TempEHug,TempTetaHug,TempSubT;

double TempCvFunc(double T)
  {
   double teta=TempTetaHug+log(max<double>(T,MathZer))-TempSubT;
//   double teta=TempTetaHug+0.5*sqr(T)-TempSubT;
//cout<<teta<<"     "<<EvalSpl((*CT->data),1,3,(*CT->Cv_Teta),teta)<<"\n";
//   return EvalSpl((*CT->data),1,3,(*CT->Cv_Teta),log(teta+TE_Zero));
   return EvalSpl((*CT->data),1,3,(*CT->Cv_Teta),teta);
  }; 
double TempMisfitE(double T)
  {
   if (T<sqrt(2*StndErr)) return 0;
//   T=max<double>(T,2*MathZer) ;
   double ret;ret=RecInt(TempCvFunc,TempTHug,T,StndErr);
   return ret+TempEHug-TempEnergy0; 
  }; 
double GetTemp(double Dencity,double Energy,TempParStruct *TempPar)
  {
   CT=TempPar;TempDenc0=Dencity;TempEnergy0=Energy;
   double THug=exp(EvalSpl((*CT->data),0,2,(*CT->Temp_Denc),Dencity))-TE_Zero;TempTHug=THug;
//   return THug;
   double EHug=exp(EvalSpl((*CT->data),0,4,(*CT->E_Denc),Dencity))-TE_Zero;TempEHug=EHug;
//   double TetaHug=exp(EvalSpl((*CT->data),0,1,(*CT->Teta_Denc),Dencity))-TE_Zero;TempTetaHug=TetaHug;
   double TetaHug=EvalSpl((*CT->data),0,1,(*CT->Teta_Denc),Dencity);TempTetaHug=TetaHug;

   TempSubT=log(TempTHug);
//   TempSubT=0.5*sqr(TempTHug);
   double T0=max<double>(THug,sqrt(4*StndErr));
   if (!ZeroMullerIt(TempMisfitE,T0,StndErr,1000))
        {cout<<" match in Temp. Mis "<<TempMisfitE(T0)<<" T0 "<<T0<<"\n";}
//cout<<" Lst "<<T0<<" Thug"<<THug<<"\n";
    return T0;
   };
double GetEnergy(double Dencity,double Temperature,TempParStruct *TempPar)
  {
//cout<<" GetEnergy D "<<Dencity<<" T "<<Temperature<<"\n";
   CT=TempPar;TempDenc0=Dencity;
   double THug=exp(EvalSpl((*CT->data),0,2,(*CT->Temp_Denc),Dencity))-TE_Zero;TempTHug=THug;
   double EHug=exp(EvalSpl((*CT->data),0,4,(*CT->E_Denc),Dencity))-TE_Zero;TempEHug=EHug;
//   double TetaHug=exp(EvalSpl((*CT->data),0,1,(*CT->Teta_Denc),Dencity))-TE_Zero;TempTetaHug=TetaHug;
   double TetaHug=EvalSpl((*CT->data),0,1,(*CT->Teta_Denc),Dencity);TempTetaHug=TetaHug;

//cout<<" THug "<<THug<<" EHug "<<EHug<<" TetaHug "<<TetaHug;
   TempSubT=log(TempTHug);
//   TempSubT=0.5*sqr(TempTHug);
//   double T0=max<double>(THug,sqrt(4*StndErr));
//cout<<" E "<<EHug+RecInt(TempCvFunc,TempTHug,Temperature,StndErr)<<"\n";
   return EHug+RecInt(TempCvFunc,TempTHug,Temperature,StndErr);
  };

