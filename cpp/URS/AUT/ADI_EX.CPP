#include "adi_ex.h"


double *ConstPar;
MatterIO *MatterAdiabat;


#define Pcritical 1e-4

void SetIntPar(double r,double e)
  {
#ifdef InternalPointsUrs  
double IntPar=0;MatterAdiabat->SetIntPar(&IntPar);MatterAdiabat->ChangeIntPar(r,e,1);
#endif
  };
double PressurePorous(double r,double e)
  {
   SetIntPar(r,e);
   double P=MatterAdiabat->Pressure(r,e);
   return max(MatterAdiabat->Pressure(r,e),Pcritical);
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

// ========================================================
// =========   Hugoniot calculations  =====================
// ========================================================

double Stat_e,Stat_p0,Stat_coef,Stat_r2,Stat_v1;
int NumPres,NumDenc,NumPresClc,NumDencClc;
double MisHugDenc(double de)
  {
NumDencClc++;
   return PressurePorous(Stat_r2,Stat_e+de)+Stat_p0+Stat_coef*de;
  }; 
void HugDenc(double u1,double r1,double p1,double e1,
             double &u2,double r2,double &p2,double &e2,
             double &D)
  {
NumDenc++;
   double de=1;
   Stat_e=e1;Stat_p0=p1;Stat_coef=2/(1/r2-1/r1);Stat_r2=r2;
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp=MathInf) ;
   if (!ZeroNewtonIt(MisHugDenc,de,StndErr,1000,1e6,-1e6))
     { cout <<" Error, could not find HugDenc\n";}
   e2=e1+de;
   p2=-(Stat_p0+Stat_coef*de);//p2=PressurePorous(r2,e2);
   double tmp=sqrt(max(0,-(p2-p1)/(1/r2-1/r1)));
   D=u1-tmp/r1;u2=tmp/r2+D;
  };
   
double MisHugPres(double dv)
  {
//	dv=-dv;
NumPresClc++;
   double r2=1/(Stat_v1+dv);
   double ret=PressurePorous(r2,Stat_e+Stat_coef*dv)-Stat_p0;
   return ret;
  }; 
void HugPres(double u1,double r1,double p1,double e1,
             double &u2,double &r2,double p2,double &e2,
             double &D)
  {
NumPres++;
   Stat_e=e1;Stat_p0=p2;Stat_coef=-(p1+p2)/2;Stat_v1=1/r1;
   double dv=0.1*Stat_v1;
   while (PressurePorous(1/(Stat_v1-dv),Stat_e+Stat_coef*dv)<=Pcritical)
	   dv+=0.2*dv;
   dv=-dv;
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp=MathInf) ;
   if (!ZeroNewtonIt(MisHugPres,dv,StndErr,1000,1e6,-1e6,0.3*Stat_v1))
     { cout <<" Error, could not find HugPres";}
//   dv=-dv;
   r2=1/(Stat_v1+dv);e2=e1+Stat_coef*dv;
   double tmp=sqrt(max(0,-(p2-p1)/(1/r2-Stat_v1)));
   D=u1-tmp/r1;u2=tmp/r2+D;
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
           TemperaturePorous(r2,e2)<<FMT<<SoundPorous(r2,e2)<<"\n";
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
           TemperaturePorous(r2,e2)<<FMT<<SoundPorous(r2,e2)<<"\n";
        }
     }   
  };






double StndErr;
MatterIO *Matter;
extern unsigned _stklen=64000;

void main(int argc,char *argv[])
  {
   Time_struct Time;
   fstream in(argv[1],ios::in);
   char tmp[512];
   double u2=0,r2,p2=0,e2=0,D=0,porous;
   double u1,r1,p1,e1;
   StndErr=1e-6;
   if (!MatterRegister(in,Matter)) { cout<<"Could not register. Exit.\n";exit(0);}
cout<<" Coreleft() "<<Coreleft()<<" Stack "<<StackAvail()<<"\n";

   in>>tmp>>u1>>tmp>>r1>>tmp>>p1>>tmp>>e1>>tmp>>porous>>tmp>>StndErr;r2=r1;

#ifdef InternalPointsUrs  
double IntPar=0;Matter->SetIntPar(&IntPar);Matter->ChangeIntPar(r1,e1,1);
#endif

   MatterAdiabat=Matter; p1=PressurePorous(r1,e1);

cout<<" beg    Coreleft() "<<Coreleft()<<Time<<"\n";
NumPres=0;NumPresClc=0;NumDenc=0;NumDencClc=0;
   ShowHugPres(in,u1,r1,p1,e1,u2,r2,p2,e2,D);
cout<<"ShowHugPres  Coreleft() "<<Coreleft()<<Time<<"\n";
cout<<" NumCalcPnt "<<NumPres<<" NumPressCalc "<<NumPresClc<<"\n";
   ShowHugDenc(in,u1,r1,p1,e1,u2,r2,p2,e2,D);
cout<<"ShowHugDenc    beg    Coreleft() "<<Coreleft()<<Time<<"\n";
cout<<" NumCalcPnt "<<NumDenc<<" NumDencCalc "<<NumDencClc<<"\n";

cout<<" End Coreleft "<<Coreleft()<<Time<<"\n";
  };






