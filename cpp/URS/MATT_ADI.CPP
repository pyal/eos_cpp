#include <lib\precompiled\madiex.h>
#include <lib\precompiled\cell_kru.h>
#include <lib\precompiled\adi_dou.h>

#include "matt_adi.h"
//#include "empi_equ.h"


double *ConstPar;
//XYZ_func PressurePtr;
MatterIO *MatterAdiabat;


//#define Pcritical 1e-4
#define Pcritical -0.01e5

static int NumHugIteration=10;
/*
double Sound(double r,double e)
  {
   double P=(*PressurePtr)(r,e);
   double dr=StndErr;
   double S=sqrt( max<double>(0,((*PressurePtr)(r+dr,e+P*dr/(r*r))-P))/dr );
   return S;
  };
*/
void SetIntPar(double r,double e)
  {
//#ifdef InternalPointsUrs  
//double IntPar=0;MatterAdiabat->SetIntPar(&IntPar);MatterAdiabat->ChangeIntPar(r,e,1);
//#endif
  };
double PressurePorous(double r,double e)
  {
   SetIntPar(r,e);
   double P=MatterAdiabat->Pressure(r,e);
   return max<double>(MatterAdiabat->Pressure(r,e),Pcritical);
//   return MatterAdiabat->Pressure(r,e);
  };
double SoundPorous(double r,double e)
  {
   SetIntPar(r,e);return MatterAdiabat->Sound(r,e);
  };
double TemperaturePorous(double r,double e)
  {
   SetIntPar(r,e);return MatterAdiabat->Temperature(r,e);
  };
double EnergyPorous(double r,double t)
  {
   SetIntPar(r,0);return MatterAdiabat->Energy(r,t);
  };


// ========================================================
// =========   Hugoniot calculations  =====================
// ========================================================

double Stat_e,Stat_p0,Stat_coef,Stat_r2,Stat_v1;
//int NumPres,NumDenc,NumPresClc,NumDencClc;
double MisHugDenc(double de)
  {
//NumDencClc++;
   double ret=PressurePorous(Stat_r2,Stat_e+de)+Stat_p0+Stat_coef*de;
   return ret;
  }; 
void HugDenc(double u1,double r1,double p1,double e1,
             double &u2,double r2,double &p2,double &e2,
             double &D)
  {
// de - begining energy. Sometimes have to be lifted
   double de=180;//,Delta;
   Stat_e=e1;Stat_p0=p1;Stat_coef=2/(1/r2-1/r1);Stat_r2=r2;
   if (fabs(u1-u2)<StndErr) {u2=u1;r2=r1;e2=e1;D=SoundPorous(r1,e1);
                             p2=PressurePorous(r1,e1);return ;}
#ifdef FZERO_HUGONIO_SEARCH
//int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
//                  int MaxIter);
   if (3<Fzero(MisHugDenc,30,1000,de,StndErr,StndErr,100))
     { cout <<" Error, could not find HugDenc ZeroFzeroIt\n";}
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp,double BegStp) 
#else
#ifdef NEWTON_HUGONIO_SEARCH
   if (!ZeroNewtonIt(MisHugDenc,de,StndErr,30,1000,-100,100))
     { cout <<" Error, could not find HugDenc\n";}
#else
   if (!FindZeroPos(MisHugDenc,de,Delta,StndErr,1))
     cout <<" Error, could not find HugDenc";
#endif
#endif
   e2=e1+de;
   p2=-(Stat_p0+Stat_coef*de);//p2=PressurePorous(r2,e2);
cout<<" Hugonio!!!! ";p2=PressurePorous(r2,e2);
cout<<" Hugonio Discrep "<<p2+(Stat_p0+Stat_coef*de)<<"\n";
   double tmp=sqrt(max<double>(0,-(p2-p1)/(1/r2-1/r1)));
   D=u1-tmp/r1;u2=tmp/r2+D;
  };
   
double MisHugPres(double dv)
  {
   double r2=1/(Stat_v1+dv);if (r2<StndErr) return 1e30;
   double ret=PressurePorous(r2,Stat_e+Stat_coef*dv)-Stat_p0;
   return ret;
  }; 
int HugPres(double u1,double r1,double p1,double e1,
             double &u2,double &r2,double p2,double &e2,
             double &D)
  {
   if (fabs(PressurePorous(r1,e1)-p2)<StndErr*10) {u2=u1;r2=r1;e2=e1;D=SoundPorous(r1,e1);return 1;}
   Stat_e=e1;Stat_p0=p2;Stat_coef=-(p1+p2)/2;Stat_v1=1/r1;
   double dv=0.01*Stat_v1;//,Ctrl;//,Delta
   double k=0.2;int MaxN=200;
   int ret=1;
   while ( (PressurePorous(1/(Stat_v1-dv),Stat_e+Stat_coef*dv)<=Pcritical) && (--MaxN>0) )
	   if (Stat_v1<(1+k)*dv) k=k*0.01;else dv+=k*dv;
   dv=-dv;
#ifdef FZERO_HUGONIO_SEARCH
//int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
//                  int MaxIter);
   if (3<Fzero(MisHugPres,-1e6,1e6,dv,StndErr,StndErr,1000))
     { cout <<" Error, could not find HugDenc ZeroFzeroIt\n";ret=0;}
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp,double BegStp) 
#else
#ifdef NEWTON_HUGONIO_SEARCH
   if (!ZeroNewtonIt(MisHugPres,dv,StndErr,100,1e6,-1e6,0.3*Stat_v1))
   {if ((Ctrl=fabs(MisHugPres(dv)))>1e-3) 
     cout <<" Error, could not find HugPres";ret=0;}
#else
   if (!FindZeroPos(MisHugPres,dv,Delta,StndErr,0.3*Stat_v1))
      {cout <<" Error, could not find HugPres";ret=0;}
#endif
#endif
//   dv=-dv;
   r2=1/(Stat_v1+dv);e2=e1+Stat_coef*dv;
   double tmp=sqrt(max<double>(0,-(p2-p1)/(1/r2-Stat_v1)));
   D=u1-tmp/r1;u2=tmp/r2+D;
   return ret;
  }; 





struct IsentropeIntegralClass 
{
public:
  IsentropeIntegralClass(MatterIO *mat):Mat(mat),OldPtr(CurPtr){CurPtr=this;};
  virtual ~IsentropeIntegralClass(){Finish();};
  //void SetStart(MatterIO *mat) {Mat=mat;OldPtr=CurPtr;CurPtr=this;}
  void Finish(){CurPtr=OldPtr;}
  double IsentropeDU(double FromDenc,double FromEnergy,double ToDenc) //To caluclate deltaU
  {
    double ResErr;int Err;
    Denc=FromDenc;Energy=FromEnergy;
    return Qags_IntSubInt(IntegUFunc,FromDenc,ToDenc,StndErr,StndErr,10,ResErr,Err);
  }
  double IsentropeEnergy(double FromDenc,double FromEnergy,double ToDenc)
  {
    Denc=FromDenc;Energy=FromEnergy; return IsentropeEnergy(ToDenc);
  }
  int IsentropePres(double FromDenc,double FromEnergy,double ToPres,
                    double &ResDenc,double &ResEnergy)
  {
    int MaxNum=10000,Num=0;
    double Pres=Mat->Pressure(FromDenc,FromEnergy),
           NextDenc=FromDenc,NextEnergy=FromEnergy;
    double Stp=(Pres>ToPres)?-1:1;
    Stp*=FromDenc*0.1;
    do {  FromDenc=NextDenc;FromEnergy=NextEnergy;}
    while (NextPressure(NextDenc,NextEnergy,Pres,ToPres,Stp)&&(++Num<MaxNum));
//cout<<"\n";
    double Err=1e-6;
    ResDenc=0.5*(NextDenc+FromDenc);ResEnergy=0.5*(NextEnergy+FromEnergy);
    if (Num>=MaxNum) return 0;
    Denc=FromDenc;Energy=FromEnergy;Pressure=ToPres;
    int ret=Fzero(PresMisf,FromDenc,NextDenc,ResDenc,Err,Err,MaxNum);
    ResEnergy=IsentropeEnergy(ResDenc);
    return 1;
  }
private:
  int NextPressure(double &NextDenc,double &NextEnergy,double &NextPres,
                   double ToPres,double &Stp)
  {
    double StartPres=NextPres,StartDenc=NextDenc,StartEnergy=NextEnergy;
    NextDenc+=Stp;
    Stp*=1.2;
    if (NextDenc<=0) NextDenc=M_Eps2;
    NextEnergy=IsentropeEnergy(StartDenc, StartEnergy, NextDenc);
    NextPres=Mat->Pressure(NextDenc,NextEnergy);
//cout<<" StartDenc "<<StartDenc<<" NextDenc "<<NextDenc<<" StartPres "<<StartPres<<" NextPres "<<NextPres<<" ToPres "<<ToPres<<"\n";
//cout.flush();
    if (NextDenc<=1.1*M_Eps2) return 0;
    return (!In_Lim(ToPres,StartPres,NextPres,1));
  }
//To use in calculation of isentrope form
  MatterIO *Mat;
  double Denc,Energy,Pressure;
  IsentropeIntegralClass *OldPtr;
  double IsentropeEnergy(double denc)
  {
    double ResDenc,ResEnergy,Stp=max<double>(1e-2*fabs(Denc-denc),1e-4);
    int res;
    if ((res=NotRungCurt(IsoentropeDeriv,Denc,Energy,denc,ResDenc,ResEnergy,Stp))!=0)
		{ cout<<" Bad in  NotRungCurt "<<res<<" FromDenc= "<<Denc<<" FromEnergy "
          <<Energy<<" ToDenc "<<denc<<" ResDenc "<<ResDenc<<" ResEnergy "
          <<ResEnergy<<" Stp "<<Stp<<"\n";}
    return ResEnergy;
  }
  static IsentropeIntegralClass *CurPtr;
  static double IsoentropeDeriv(double d,double e){return CurPtr->Mat->Pressure(d,e)/sqr(d);}
  static double IntegUFunc(double x)
  {double E=CurPtr->IsentropeEnergy(x);
   CurPtr->Denc=x;CurPtr->Energy=E;
   double ret=max<double>(CurPtr->Mat->Sound(x,E),1e-1)/x;
//cout<<" Denc "<<x<<" s/x "<<ret<<"\n";cout.flush();
   return ret;
  }
  static double PresMisf(double CurDenc)
  {
    double E=CurPtr->IsentropeEnergy(CurDenc);
    double ret= CurPtr->Mat->Pressure(CurDenc,E)-CurPtr->Pressure;
//cout<<" PresMis "<<ret<<"\n";cout.flush();
    return ret;
  }

};
IsentropeIntegralClass* IsentropeIntegralClass::CurPtr;


//MatterIO  *IsoM;
//double IsoDeriv(double D,double E) 
//  { return IsoM->Pressure(D,E)/sqr(D);};
//int BuildE_R(double FromDenc,double ToDenc,double FromE,int Num,double Misf,
//             CurveSpline &spl,double &Rmin,double &Rmax,double ToPres=1e-100)
//{
//  double stp=(ToDenc-FromDenc)/(Num-1);
//  double *Denc=new double[Num+1];Denc[0]=FromDenc;
//  double *E=new double[Num+1];E[0]=FromE;
//  IsoM=MatterAdiabat;
//  double tmp,s=1e-3,FromPres=MatterAdiabat->Pressure(FromDenc,FromE);
//  for (int k=1;k<Num;k++) 
//    {
//     Denc[k]=Denc[k-1]+stp;
//     NotRungCurt(IsoDeriv,Denc[k-1],E[k-1],Denc[k],tmp,E[k],s);
//     if ((ToPres!=1e-100) && (!In_Lim(MatterAdiabat->Pressure(Denc[k],E[k]),FromPres,ToPres)) )
//       {k++;break;}
//    }
//  k--;Rmin=Denc[k-1];Rmax=Denc[k];
//  int ret=spl.Generate(Num,Misf,0,&Denc[0],&E[0],k+1);
//  delete E;delete Denc;
//  return ret;
//}
//double E_found;
//CurveSpline *SplIsent;
//double MisIsentPres(double r2)
//{
//  E_found=SplIsent->Evaluate(r2);
//  return IsoM->Pressure(r2,E_found)-Stat_p0;
//};
//
//double IsentDU(double D)
//{ double E=SplIsent->Evaluate(D);
//  return IsoM->Sound(D,E)/D;
//};
//


// OLD
//void IsentropePres(double u1,double r1,double p1,double e1,
//             double &u2,double &r2,double p2,double &e2,
//             double &D)
//  {
//   if (fabs(PressurePorous(r1,e1)-p2)<StndErr*10) {u2=u1;r2=r1;e2=e1;D=SoundPorous(r1,e1);return ;}
//// (p1+p2)/2*dV=-de; de*3*Denc=dp
//   if (!HugPres(u1,r1, p1,e1,u2,r2,p2,e2,D)) return;
//
////   r2=1/(1/r1+dv);
////   IsoM=MatterAdiabat;
////   Stat_e=e1;Stat_p0=p2;Stat_v1=r1;
////cout<<" IsentropePres "<<p2<<" "<<r1<<" "<<r2<<"\n"<<flush;
//
////   if (3<Fzero(MisHugIsent,1e-3,100,r2,StndErr,StndErr,1000))
////     { cout <<" Error, could not find IsentDenc ZeroFzeroIt\n";}
////cout<<" Fzero \n"<<flush;
//   double tmp,s=1e-3;
//   double ResErr,Rmin,Rmax;int Err;
//   CurveSpline spl;
////cout<<" Build\n"<<flush;
//   BuildE_R(r1,100*(r2-r1)+r2,e1,10000,StndErr,spl,Rmin,Rmax,p2);SplIsent=&spl;
////cout<<" Spl\n"<<flush;
//   r2=(Rmin+Rmax)*0.5;
//   if (3<Fzero(MisIsentPres,Rmin,Rmax,r2,StndErr,StndErr,1000))
//     { cout <<" Error, could not find IsentDenc ZeroFzeroIt\n";}
////cout<<" Fzero \n"<<flush;
//   e2=E_found;D=SoundPorous(r2,e2);
//
//   u2=u1+Qags_IntSubInt(IsentDU,min(r1,r2),max<double>(r1,r2),StndErr,StndErr,1000,ResErr,Err);
//  }; 
void IsentropePres(double u1,double r1,double p1,double e1,
             double &u2,double &r2,double p2,double &e2,
             double &D)
  {
   if (fabs(PressurePorous(r1,e1)-p2)<StndErr*10) {u2=u1;r2=r1;e2=e1;D=SoundPorous(r1,e1);return ;}
   //p2=max<double>(p2,1e-4);
// (p1+p2)/2*dV=-de; de*3*Denc=dp
//cout<<" u1 "<<u1<<" r1 "<<r1<<" p1 "<<p1<<" p2 "<<p2;
   if (!HugPres(u1,r1, p1,e1,u2,r2,p2,e2,D)) return;
   IsentropeIntegralClass Isent(MatterAdiabat);
   Isent.IsentropePres(r1,e1,p2,r2,e2);
   D=SoundPorous(r2,e2);
   //u2=u1+Isent.IsentropeDU(r1,e1,r2);
   double du=Isent.IsentropeDU(r1,e1,r2),Zero=50*M_Eps2;
   if (p2<Zero) du=-(Zero-p2)/Zero*10000;
   u2=u1-du;
//cout<<" u2 "<<u2<<" r2 "<<r2<<"\n";
   //u2=u1+Qags_IntSubInt(IsentDU,min(r1,r2),max<double>(r1,r2),StndErr,StndErr,1000,ResErr,Err);
  }; 
// ========================================================
// =========   End Hugoniot calculations  =================
// ========================================================



void ShowHugPres(fstream &in,double u1,double r1,double p1,double e1,
                        double u2,double r2,double p2,double e2,double D)
  {
   char buf[256];
   double min,max,stp;
   if (InputPar(in,u1,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
out<<"         U            Dencity               Pressure        Energy                 D           Temperature     S\n";
      for (double k=min;k<=max;k+=stp)
        {
         HugPres(u1,r1,p1,e1,u2,r2,k,e2,D);
         out<<FM<<u2<<FMT<<r2<<FMT<<k<<FMT<<e2<<FMT<<D<<FMT<<
           TemperaturePorous(r2,e2)<<FMT<<SoundPorous(r2,e2)<<"\n";out.flush();
        }
     }   
  };

void ShowHugDenc(fstream &in,double u1,double r1,double p1,double e1,
                        double u2,double r2,double p2,double e2,double D)
  {
   char buf[256];
   double min,max,stp;
   if (InputPar(in,u1,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
out<<"         U            Dencity               Pressure        Energy                 D           Temperature     S\n";
      for (double k=min;k<=max;k+=stp)
        {
         HugDenc(u1,r1,p1,e1,u2,k,p2,e2,D);
         out<<FM<<u2<<FMT<<k<<FMT<<p2<<FMT<<e2<<FMT<<D<<FMT<<
           TemperaturePorous(k,e2)<<FMT<<SoundPorous(k,e2)<<"\n";out.flush();
        }
     }   
  };





#ifdef MatterH2Liq_Register
#include "matt_h2.h"
void ShowColdH2(fstream &in)
  {
   char buf[256];

//   MatterIO *Matt;
//   MatterRegister(in,Matt);MatterH2Liq *H2=(MatterH2Liq&)Matt;
   MatterH2Sol H2;H2.read_data_state(FilterTextIn(in.rdbuf(),0));
   if (!in) {cout<<" ShowColdH2: could not read param\n";exit(0);}
   double min,max,stp,x,Rconst=ColdParam.Denc,T0;
   if (InputPar(in,T0,min,max,stp,&buf[0]))
     {
//      e1=Eliquid(min/Rconst,t1);
//cout<<" Energy for temp "<<t1<<" E= "<<e1<<"\n";
//cout<<" Found Temp "<<TemperatureL(min/Rconst,e1)<<"\n";ChRead();
      fstream out(buf,ios::out);
out<<"        Dencity   Pcold      Ecold       Grunizen  Teta Z\n";
      for (double k=min;k<=max;k+=stp)  
        {
         x=k/Rconst;
         out<<FM<<k<<
         FMT<<Pcold(x)<<FMT<<
         Ecold(x)<<FMT<<
//            Ecold_Ehug(x)<<
           FMT<<Grun(x)<<
                   FMT<<Teta(x)<<FMT<<
                      Zparam(x,T0)<<FMT<<

//                   Ccold2(x)<<FMT<<PNull(x)<<
"\n";
        };
     }
  };   
#include "empi_equ.h"
extern EmpiParamStruct *EmpiColdParam;
void ShowColdEmpi(fstream &in)
  {
   char buf[256];
   MatterIO *Matt=dynamic_cast<MatterIO*>(SavableClass::SavableClassRegister(in));
   double min,max,stp,x,Rconst=EmpiColdParam->Denc,T0;
   if (InputPar(in,T0,min,max,stp,&buf[0]))
     {
      fstream out(buf,ios::out);
out<<"        Dencity   Pcold      Ecold       Grunizen  Teta Z\n";
      for (double k=min;k<=max;k+=stp)  
        {
         x=k/Rconst;
         out<<FM<<k<<FMT<<EmpiPcold(x)<<FMT<<EmpiEcold(x)<<FMT<<
             EmpiGrun(x)<<FMT<<EmpiTeta(x)<<FMT<<EmpiZparam(x,T0)<<FMT<<"\n";
        };
     }
   delete Matt;
  };   
#endif

void ShowIsoTherm(fstream &in)
  {
   char buf[256];
   double T,P,E=1,T0;//,x;
//   MatterH2Sol H2;
//cout<<" ShowIsoTherm beg coreleft "<<Coreleft()<<"\n";
   MatterIO *M;
   M=MatterAdiabat;
//   if (!MatterRegister(in,M)) { cout<<"IsoTherm Could not register. Exit.\n";exit(0);}
   double min,max,stp;
   if (InputPar(in,T0,min,max,stp,buf))
     {
//#ifdef InternalPointsUrs  
//double IntPar=0;MatterAdiabat->SetIntPar(&IntPar);
//#endif
      fstream out(buf,ios::out);
//     Rcritical=Rconst*Bconst/(fabs(Bconst-1)+StndErr);
out<<" Isotherm T= "<<T0<<"\n";
out<<"    Dencity           Pressure       Temperature         Energy \n";
      for (double k=min;k<=max;k+=stp)  
        {
//break;
cout<<" D "<<k<<" T0 "<<T0;
//#ifdef InternalPointsUrs  
//M->ChangeIntPar(k,E,1);
//#endif
         E=M->Energy(k,T0);
//#ifdef InternalPointsUrs  
//M->ChangeIntPar(k,E,1);
//#endif
cout<<" E "<<E<<"\n";
//         x=k/Rconst;
//         E=Eliquid(x,T0);  // T0 is Temperature
         T=M->Temperature(k,E);
         P=M->Pressure(k,E);
         out<<FM<<k<<FMT<<P<<FMT<<T
                   <<FMT<<E<<FMT<<"\n";
        }
     }   
//   delete M;
//cout<<" ShowIsoTherm end coreleft "<<Coreleft()<<"\n";
  };
//  {	return D/E;};
void ShowIsoEntrope(fstream &in)
  {
   char buf[256];
   double T,P,E,E0,s=1e-3;//x,x_c,
//			int res;
   MatterIO *M;M=MatterAdiabat;
//   if (!MatterRegister(in,M)) { cout<<"Could not register. Exit.\n";exit(0);}
   //IsoM=M;
   double min,max,stp,Rconst=M->DencityCold(),Cur_U=0;
   IsentropeIntegralClass Isent(MatterAdiabat);
   if (InputPar(in,E0,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
//     Rcritical=Rconst*Bconst/(fabs(Bconst-1)+StndErr);
out<<" Isoentrope\n";
out<<"        Dencity          Pressure          Temperature         Energy       dU\n";
      double Ecur=E0,Rcur=min;
//      for (double k=min(min,max);k<=max<double>(max,min);k+=max<double>(min(fabs(stp),(max<double>(max,min)-k)*(1+2*M_Eps)),2*k*M_Eps) )
      stp=max<double>(fabs(stp),2*min*M_Eps);(min>max)?stp=-stp:stp=stp;
      double k=min-stp;
      while (In_Lim(k=k+stp,min,max,1))
      //{
      //for (double k=min;k<=max;k+=max<double>(fabs(stp),2*k*M_Eps))
        {
//         x=k/Rconst;
//         E=Eliquid(x,T0);  // T0 is Temperature
//int NotRungCurt(XYZ_func func,double x_beg,double y_beg,double x_end,
//                            double &x,double &y,
//                            double &stp,double err,int MaxIter,
//                                                 BrkXY_func Brk)
//         if (res=NotRungCurt(IsoDeriv,Rcur,Ecur,k,x_c,E,s)!=0)
////         if (res=NotRungCurt(IsoDeriv,min,E0,k,x_c,E,s)!=0)
//									  { cout<<" Bad in  NotRungCurt "<<res<<" x1= "<<min<<" E0 "<<E0
//											      <<" x_ "<<k<<" x2 "<<x_c<<" y2 "<<E<<" s "<<s<<"\n";}
////									out<<FM<<k<<FMT<<E<<"\n";
         //Isent.SetStart(M);
         E=Isent.IsentropeEnergy(Rcur,Ecur,k);
         T=M->Temperature(k,E);
         P=M->Pressure(k,E);
//         Cur_U+=Isent.IsentropeDU(min(Rcur,k),max<double>(Rcur,k));
         //if (k==min(min,max)) Cur_U=-Cur_U;
         Cur_U+=Isent.IsentropeDU(Rcur,Ecur,k);
//         double ResDenc,ResEnergy;
//         Isent.IsentropePres(min,E0,P,ResDenc,ResEnergy);
//cout<<" E "<<E<<" PE "<<ResEnergy<<" D "<<k<<" PD "<<ResDenc<<"\n";
         //Isent.Finish();

         out<<FM<<k<<FMT<<P<<FMT<<T<<FMT<<E<<FMT<<Cur_U<<"\n";out.flush();
         Ecur=E;Rcur=k;
        }
     }   
//   delete M;
  };

double ShowIntFuncTemp(double Denc,double Temp)
  {
   double ret=Temp;
//#ifdef INTERNAL_FUNC
//   double inp[3],outp[2];
//   inp[1]=Denc;inp[2]=Temp;
//   MatterAdiabat->Int_Par(inp,outp);
//   ret=outp[1];
//#endif
   return ret;
  }

void ShowEnergy(fstream &in)
  {
   double Dmax,Dmin,Emax,Emin;
   int ShowSound;
   double R,E,P,T,S;
   char data[256],res[256];
   if (InputPar(in,Dmax,Dmin,Emax,Emin,data))
     {
      ShowSound=int(Emin);
      SetExt(data,res,"res");
      fstream in1(data,ios::in);fstream out(res,ios::out);
out<<"        Dencity          Pressure      Temperature         Energy        Sound\n";
      while (!seek_eof(in1)) 
        {in1>>R>>E;
         P=PressurePorous(R,E);
//cout<<" P \n";
         T=TemperaturePorous(R,E);
//cout<<" T \n";
         if (ShowSound) S=SoundPorous(R,E);
         else S=ShowIntFuncTemp(R,T);
//cout<<" S \n";
         out<<FM<<R<<FMT<<P<<FMT<<T<<FMT<<E<<FMT<<S<<"\n";
         out.flush();
        } 
      in1.close();out.close();
     }
  };    


void ShowTemperature(fstream &in)
  {
   double Dmax,Dmin,Emax,Emin;
   int ShowSound;
   double R,E,P,T,S;
   char data[256],res[256];
   if (InputPar(in,Dmax,Dmin,Emax,Emin,data))
     {
      ShowSound=int(Emin);
      SetExt(data,res,"res");
      fstream in1(data,ios::in);fstream out(res,ios::out);
out<<"        Dencity          Pressure      Temperature         Energy        Sound|(Temp/Diss)\n";
      while (!seek_eof(in1)) 
        {in1>>R>>T;
         E=EnergyPorous(R,T);
cout<<" E "<<E<<" D "<<R<<" T "<<T<<"\n";
         P=PressurePorous(R,E);
//cout<<" P \n";
         T=TemperaturePorous(R,E);
//cout<<" T \n";
         if (ShowSound) S=SoundPorous(R,E);
         else S=ShowIntFuncTemp(R,T);
//cout<<" S \n";
         out<<FM<<R<<FMT<<P<<FMT<<T<<FMT<<E<<FMT<<S<<"\n";
out.flush();
        } 
      in1.close();out.close();
     }
  };    

//#ifdef MatterSpline_Register
#include "matt_spl.h"
void FormSplineDat(fstream &in)
  {
   double Dmax,Dmin,Emax,Emin;
   char buf[256];
   if (InputPar(in,Dmax,Dmin,Emax,Emin,buf))
     {
      fstream row(buf,ios::in);
      GenerateMatterSpl(row,MatterAdiabat);
      row.close();
     }
  };    

#include "spl_2phase.h"

void FormTwoPhaseBnd(fstream &in)
  {
   double Dmax,Dmin,Emax,Emin;
   char buf[256];
   if (InputPar(in,Dmax,Dmin,Emax,Emin,buf))
     {
      fstream row(buf,ios::in);
      GenerateTwoPhaseBnd(row,MatterAdiabat);
      row.close();
     }
  };    
void FindBushCoefABC_Mat(fstream &in)
{
   double dPdT,AminC,AmaxC,Stp;
   char buf[256];
   if (InputPar(in,dPdT,AminC,AmaxC,Stp,buf))
     {
      fstream row(buf,ios::in);
      FindBushCoefABC(row,MatterAdiabat,dPdT,AminC,AmaxC,Stp);
      row.close();
     }
};    

void FormSolLiqBnd(fstream &in)
  {
   double Dmax,Dmin,Emax,Emin;
   char buf[256];
   if (InputPar(in,Dmax,Dmin,Emax,Emin,buf))
     {
      fstream row(buf,ios::in);
      GenerateSolLiqPhaseBnd(row,MatterAdiabat);
      row.close();
     }
  };    

//#endif
#ifdef RossDiameter
extern Exp6Param *CalculateExp6Struct;
void ShowRossDiameter(fstream &in)
  {
   char buf[256];
   double T,P,E=1,R0,x;
   FreeERossH2 H2;
   if (!H2.ReadIni(in)) { cout<<"RossDiameter Could not register. Exit.\n";exit(0);}
   MatterFreeE Mat;Mat.FreeEPtr=&H2;
   double min,max,stp;
   if (InputPar(in,R0,min,max,stp,buf))
     {
//#ifdef InternalPointsUrs  
//double IntPar=0;MatterAdiabat->SetIntPar(&IntPar);
//#endif
      fstream out(buf,ios::out);
out<<" RossDiameter  Denc0= "<<R0<<"\n";
out<<"    Temperature           Pressure          Energy      Diameter\n";
      for (double k=min;k<=max;k+=stp)  
        {
//cout<<" T "<<k<<" 0 "<<T0;
         E=H2.FreeE(R0,k);
         x=CalculateExp6Struct->L_Niu;
//E=0;P=0;
         E=Mat.Energy(R0,k);
         P=Mat.Pressure(R0,E);
//cout<<" E "<<E<<"\n";
         out<<FM<<k<<FMT<<P<<FMT<<E<<FMT<<x<<"\n";
        }
     }   
//   delete M;
//cout<<" ShowIsoTherm end coreleft "<<Coreleft()<<"\n";
  };
#endif

#include "fre_fac.h"

double WorkDencity;
InterfaceFreeEIO *WorkSolid,*WorkLiquid;
double DeltaFreeEnergy(double Temp)
{
  return WorkSolid->FreeE(WorkDencity,Temp)-WorkLiquid->FreeE(WorkDencity,Temp);
};
void FormSolidLiquidDat(fstream &in)
  {
//   double Dmax,Dmin,Emax,Emin;
   char buf[256],tmp[256];
   double min,max,stp,ResT;
   if (InputPar(in,ResT,min,max,stp,buf))
     {
      fstream row(buf,ios::in);
      MatterIO *Solid,*Liquid;
      //row>>tmp;if (NULL==(Solid=dynamic_cast<MatterIO*>(SavableClass::SavableClassRegister(row))))
      row>>tmp;if (NULL==(Solid=dynamic_cast<MatterIO*>(SavableClass::Read(row))))
          throw info_except("Could not register EOS::Solid \n");
      //row>>tmp;if (NULL==(Liquid=dynamic_cast<MatterIO*>(SavableClass::SavableClassRegister(row))))
	  row>>tmp;if (NULL==(Liquid=dynamic_cast<MatterIO*>(SavableClass::Read(row))))
          throw info_except("Could not register EOS::Liquid \n");
      row>>tmp>>tmp;
      fstream out(tmp,ios::out);
      row.close();
      WorkSolid=((MatterFreeE *)Solid)->FreeEPtr;
      WorkLiquid=((MatterFreeE *)Liquid)->FreeEPtr;
      for (double k=min;k<=max;k+=stp)  
        {
         WorkDencity=k;
//int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
//                  int MaxIter);
         if (3<Fzero(DeltaFreeEnergy,30,100000,ResT,StndErr,StndErr,500))
          { cout <<" Error, could not find FormSolidLiquidDat ZeroFzeroIt\n";}
         double Esol=Solid->Energy(k,ResT);
         double Eliq=Liquid->Energy(k,ResT);
         out<<FM<<k<<FMT<<ResT<<FMT<<Solid->Pressure(k,Esol)<<FMT<<Esol
                              <<FMT<<Liquid->Pressure(k,Eliq)<<FMT<<Eliq<<"\n";
        }
      delete Solid;delete Liquid;
      out.close();
     }
  };    





























//double XXX;
//===========================================================
//==============  Calculate Hugoniot From D          ========
//===========================================================


double MisfitDHug(double r2)
  {
//   double porous=ConstPar[0];
if (r2<StndErr) cout<<" MisfitDHug  r2 = "<<r2<<"\n";
   double v2=1/r2;
   double p1=ConstPar[1];
   double e1=ConstPar[2];
   double v1=ConstPar[3];
   double j1=ConstPar[4];
   double p2=p1-(v2-v1)*j1;
   double e2=e1-(v2-v1)*(p1-0.5*(v2-v1)*j1);
//if (v2<StndErr) { cout<<"MisfitDHug, too little volume "<<v2<<"\n";}
   double misfit=PressurePorous(r2,e2)-p2;
//cout<<"r1 "<<r1<<" p1 "<<p1<<" e1 "<<e1<<" mis "<<misfit<<"\n";
   return misfit;
  };

void DHug(double u1,double r1,double p1,double e1,
          double &u2,double &r2,double &p2,double &e2,
          double D,double porous,double Rcritical)
// Calculates from D hugoniot. Do not forget about
// the lowest possible D.
  {
   double v1=1/r1,j1=sqr(r1*(D-u1)),v2;
//   if (fabs(a1)>MathZer)
     {
      ConstPar=new double [5];
      ConstPar[0]=porous;
      ConstPar[1]=p1;
      ConstPar[2]=e1;
      ConstPar[3]=v1;
      ConstPar[4]=j1;
      v2=1/(Rcritical-100*StndErr);
      r2=(Rcritical-100*StndErr);
//      if (!ZeroNewtonIt(MisfitDHug,v2,StndErr,1000,1000,1/Rcritical))
      if (!ZeroNewtonIt(MisfitDHug,r2,StndErr,1000,Rcritical-StndErr,StndErr))
        {cout<<" Could not find match in DHug.\n";ChRead() ;}
      delete ConstPar;
      v2=1/r2;
//      r2=1/v2;
      p2=p1-(v2-v1)*j1;
      e2=e1-(v2-v1)*(p1-0.5*(v2-v1)*j1);
      u2=D-sqrt(j1)/r2;
double misfit=PressurePorous(r2,e2)-p2;
if (misfit>10*StndErr) cout<<" mis "<<misfit<<" r2 "<<r2<<"\n";//ChRead();
     }
//   else {};
  };


//===========================================================
//==============  Calculate Hugoniot From Velocities ========
//====================New version  ===========================
//=================== Finds only upper root =================
//===========================================================

double MisfitHug(double x)
  {
   double porous=ConstPar[0],v1=ConstPar[1],p1=ConstPar[2];
   double e1=ConstPar[3],du=ConstPar[4];
	  
//   if (x>-MathZer) { cout<<" Wrong Sound x = "<<x<<"\n";exit(0);}
   if (x<MathZer) { cout<<" Wrong Sound x = "<<x<<"\n";exit(0);}
   double  v2=v1+du*v1/x;
if (v2<StndErr) return 1/MathZer;
   double  p2=p1-du*x/v1;
   double  e2=e1-du*p1*v1/x+0.5*du*du;
   double misfit=PressurePorous(1/v2,e2)-p2;
//cout<<" S "<<x<<" Mis "<<misfit<<" Pi "<<p2<<" du "<<du<<"\n";
//<<" v2 "<<v2<<" du "<<du
//<<" p2 "<<p2<<" pres "<<PressurePorous(1/v2,e2)
//<<"\n";
   return misfit;
  };

int SoundIter(double v1,double p1,double e1,double du,double &s,double &s0,
               double &Mis)
  {
   double v2=v1+du*v1/s;
   double e2=e1-du*p1*v1/s+0.5*du*du;
   double  p2=p1-du*s/v1;
   double p2i=PressurePorous(1/v2,e2);
   double N=NumHugIteration;
   s=(p1-(p2i+p2*(N-1))/N)*v1/du;
//   double ret=s;
//cout<<" S "<<s<<" p2i "<<p2i<<" p2 "<<p2<<"\n";
   return 1;
  }; 
void SetHugPar(double &v1,double &x,double &du,
               double u1,double r1,double &p1,double e1,double u2,double &xstp,
               double porous=1)
  {
//                         2       dP     v1    2
//  zero aprocsimation:   x  + x * -- du* -- + u  = 0
//                                 dE     2     0
//                                    dP    v1
//  linear aprocsimation:   x = -u  - -- *  --- * du  zero x=c +3*du
//                                0   dE     4                0
			du=u2-u1;v1=1/r1;
//   p1=PressurePorous(r1,e1);
//   p1=0;
			ConstPar=new double [5];ConstPar[0]=porous; 
			ConstPar[1]=v1;ConstPar[2]=p1; ConstPar[3]=e1;  ConstPar[4]=du;
//cout<<" du "<<du<<"\n";
//   double PEderiv=( PressurePorous(r1,e1+StndErr)-p1 )/StndErr;
//cout<<" PE "<<PEderiv<<" ";PEderiv=max<double>(PEderiv,1);
   double Sound=SoundPorous(r1,e1);
   double s0=Sound;

   xstp=-2.*du;x=Sound+xstp;
   double x0=x,Mis=MathInf;
   for (int k=1;k<=NumHugIteration;k++)  if (!SoundIter(v1,p1,e1,du,x,x0,Mis)) break;
//   xstp=-0.05*(xstp-x0);x=x0;
   xstp=-0.05*(x-x0);
//cout<<"\nMis "<<MisfitHug(x)<<" M0 "<<MisfitHug(x0)<<" s  "<<x<<" s_stp "<<xstp<<"\n";
   if (fabs(xstp)<100*StndErr) xstp=100*StndErr;
   if (x<=-du) x=fabs(-du+Sound);
  };

void Hug(double u1,double r1,double p1,double e1,
          double u2,double &r2,double &p2,double &e2,double &D,
          double porous,double Rcritical)
// Calculates from velocites hugoniot. D - calculates

//  formulas : v1=1/r1; du=u2-u1; x=u1      v2=v1+du*v1/x
//                                          p2=p1-du*x/v1
//                                          e2=e1-du*p1*v1/x+0.5*du*du
//                         2       dP     v1    2
//  zero aprocsimation:   x  + x * -- du* -- + u  = 0
//                                 dE     2     0

  {
   double v2,x,du,v1,x0,xstp;
   SetHugPar(v1,x0,du,u1,r1,p1,e1,u2,xstp,porous);
//cout<<" P1 "<<p1<<"\n";
   if (fabs(du)>sqrt(StndErr))
     {
      x=x0;
//cout<<" X "<<x<<"\n";
//cout<<" Par1 "<<ConstPar[1]<<" Par[2] "<<ConstPar[2]<<" Par3 "<<ConstPar[3]<<" Par4 "<<ConstPar[4]<<"\n";
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp=MathInf) ;
      if (!ZeroNewtonIt(MisfitHug,x,StndErr,1000,1/StndErr,sqrt(StndErr),xstp))
        {cout<<" Could not find match in Hug.\n";}//ChRead();}
      v2=v1+du*v1/x;
      p2=p1-du*x/v1;
      e2=e1-du*p1*v1/x+0.5*du*du;
      r2=1/v2;
//      D=x-u1;
      D=x;
//						XXX=x;
double misfit=PressurePorous(r2,e2)-p2;
//cout<<" p2 "<<p2<<" r2 "<<r2<<" e2 "<<e2<<" du "<<du<<"\n";
//cout<<" found x "<<x<<" x_fst "<<x0<<" misf "<<misfit<<"\n";
if (fabs(misfit)>100*StndErr) 
cout<<" mis "<<misfit<<"  "<<MisfitHug(x)<<" D "<<D<<" r1 "<<r1<<" p1 "<<p1<<"\n";//ChRead();
     }
   else {D=SoundPorous(r2,e2);r2=r1;p2=p1;e2=e1;};
   delete ConstPar;
  };

//===========================================================
//==============  Draws     Hugoniot From Velocities ========
//===========================================================
void ShowHugMis(fstream &in,double u1,double r1,double p1,double e1,
                           double u2,double r2,double p2,double e2,double D,
                           double porous,double Rcritical)
  {
   char buf[256];
   double min,max,stp;
   if (InputPar(in,u2,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
   			double v2,y,du,v1;
      SetHugPar(v1,y,du,u1,r1,p1,e1,u2,porous);
out<<" FstApr x = "<<y<<" p1 "<<p1<<"\n";
out<<"        X_=1/(r1*u0)      Dencity          Pressure          DeltaP              Energy                 D              Sound              Temperature\n";
      for (double x=min;x<=max;x+=stp)
        {
         v2=v1+du*v1/x;
         p2=p1-du*x/v1;
         e2=e1-du*p1*v1/x+0.5*du*du;
         r2=1/v2;
   						if (fabs(x)<MathZer) { cout<<" Strange in Hugoniot. x= "<<x<<"\n";D=0;}
         else D=x;
         out<<FM<<x<<FMT<<r2<<FMT<<p2<<FMT<<MisfitHug(x)
                <<FMT<<e2<<FMT<<D<<FMT
                <<SoundPorous(r2,e2)<<FMT
                <<TemperaturePorous(r2,e2)<<FMT<<"\n";
        };
      delete ConstPar;
     }
  };

void ShowDHugMis(fstream &in,double u1,double r1,double p1,double e1,
                            double u2,double r2,double p2,double e2,double D,
                            double porous,double Rcritical)
  {
   char buf[256];
   double min,max,stp;
   if (InputPar(in,D,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
      ConstPar=new double [4];
      double v1=1/r1,j1=sqr(r1*(D-u1));
      ConstPar[0]=porous;
      ConstPar[1]=p1;
      ConstPar[2]=e1;
      ConstPar[3]=v1;
      ConstPar[4]=j1;
out<<"        Dencity          Pressure          DeltaP              Energy                 U\n";
      for (r2=min;r2<=max;r2+=stp)
        {
         double v2=1/r2;
         p2=p1-(v2-v1)*j1;
         e2=e1-(v2-v1)*(p1-0.5*(v2-v1)*j1);
         u2=sqrt(j1)/r2+D;
         out<<FM<<r2<<FMT<<p2<<FMT<<MisfitDHug(r2)<<FMT<<e2<<FMT<<u2<<FMT<<
             TemperaturePorous(r2,e2)<<FMT<<"\n";
        };
      delete ConstPar;
     }
  };

void ShowDHug(fstream &in,double u1,double r1,double p1,double e1,
                        double u2,double r2,double p2,double e2,double D,
                        double porous,double Rcritical)
  {
   char buf[256];
   double min,max,stp;
   if (InputPar(in,D,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
//     Rcritical=Rconst*Bconst/(fabs(Bconst-1)+StndErr);
out<<"         U               Dencity               Pressure        Energy                 D\n";
      for (double k=min;k<=max;k+=stp)  
        {
         DHug(u1,r1,p1,e1,u2,r2,e2,p2,k,porous,Rcritical);
         out<<FM<<u2<<FMT<<r2<<FMT<<p2<<FMT<<e2<<FMT<<k<<"\n";
        }
     }   
  };
void ShowHug(fstream &in,double u1,double r1,double p1,double e1,
                        double u2,double r2,double p2,double e2,double D,
                        double porous,double Rcritical)
  {
   char buf[256];
   double min,max,stp;
   if (InputPar(in,u1,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
//     Rcritical=Rconst*Bconst/(fabs(Bconst-1)+StndErr);
out<<"         U            Dencity               Pressure        Energy                 D           Temperature     S\n";
      for (double k=min;k<=max;k+=stp)
        {
         Hug(u1,r1,p1,e1,k,r2,p2,e2,D,porous,Rcritical);
         out<<FM<<k
//<<FMT<<XXX
<<FMT<<r2<<FMT<<p2<<FMT<<e2<<FMT<<D<<FMT<<
  TemperaturePorous(r2,e2)<<FMT<<
  SoundPorous(r2,e2)<<"\n";
        }
     }   
  };
