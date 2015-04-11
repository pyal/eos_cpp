#include "matt_adi.h"

double *ConstPar;
//XYZ_func PressurePtr;
MatterIO *MatterAdiabat;


#define Pcritical 1e-4

/*
double Sound(double r,double e)
  {
   double P=(*PressurePtr)(r,e);
   double dr=StndErr;
   double S=sqrt( max(0,((*PressurePtr)(r+dr,e+P*dr/(r*r))-P))/dr );
   return S;
  };
*/
double PressurePorous(double r,double e)
//  {return max(MatterAdiabat->Pressure(r,e),Pcritical);};
  {return MatterAdiabat->Pressure(r,e);};

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
   double ret=MatterAdiabat->Sound(1/v2,e2);
   double M;

   if ((ret<s) && (ret!=0))
     {
      if ( (M=fabs(MisfitHug(s)))<=Mis) {Mis=M;s0=s;
//cout<<" Good ";
     }
      else return 0;
     }
   else if (ret!=0) ret=min(ret,s0);else ret=s0;
   ret=(max(ret,s)*39+min(ret,s))/40;
//cout<<" S "<<ret<<" S0 "<<s0<<" Mis "<<Mis<<"\n";
   s=ret;
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
   p1=PressurePorous(r1,e1);
			ConstPar=new double [5];ConstPar[0]=porous; 
			ConstPar[1]=v1;ConstPar[2]=p1; ConstPar[3]=e1;  ConstPar[4]=du;
//cout<<" du "<<du<<"\n";
//   double PEderiv=( PressurePorous(r1,e1+StndErr)-p1 )/StndErr;
//cout<<" PE "<<PEderiv<<" ";PEderiv=max(PEderiv,1);
   double Sound=MatterAdiabat->Sound(r1,e1);
   double s0=Sound;

   xstp=-2.*du;x=Sound+xstp;
   double x0=x,Mis=MathInf;
   for (int k=1;k<=40;k++)  if (!SoundIter(v1,p1,e1,du,x,x0,Mis)) break;
   xstp=-0.05*(xstp-x0);x=x0;
//cout<<"\nMis "<<MisfitHug(x0)<<" M1 "<<Mis<<" s  "<<x<<" s_stp "<<xstp<<"\n";
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
   else {D=MatterAdiabat->Sound(r2,e2);r2=r1;p2=p1;e2=e1;};
   delete ConstPar;
  };

//===========================================================
//==============  Draws     Hugoniot From Velocities ========
//===========================================================

int InputPar(fstream &in,double &from,double &min,double &max,
                                                 double &stp,char *write)
  {
//   fstream in(name,ios::in);
   int ret=0;
   in>>write>>from>>write>>min>>write>>max>>write>>stp;in.getline(write,255);
   in>>write>>ret>>write;
   return ret;
  };

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
                <<MatterAdiabat->Sound(r2,e2)<<FMT
                <<MatterAdiabat->Temperature(r2,e2)<<FMT<<"\n";
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
             MatterAdiabat->Temperature(r2,e2)<<FMT<<"\n";
        };
      delete ConstPar;
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
  MatterAdiabat->Temperature(r2,e2)<<FMT<<
  MatterAdiabat->Sound(r2,e2)<<"\n";
        }
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

#ifdef MatterH2Liq_Register

void ShowColdH2(fstream &in)
  {
   char buf[256];

//   MatterIO *Matt;
//   MatterRegister(in,Matt);MatterH2Liq *H2=(MatterH2Liq&)Matt;
   MatterH2Sol H2;H2.ReadIni(in);
   double min,max,stp,x,Rconst=ColdParam.Denc;
   if (InputPar(in,x,min,max,stp,&buf[0]))
     {
//      e1=Eliquid(min/Rconst,t1);
//cout<<" Energy for temp "<<t1<<" E= "<<e1<<"\n";
//cout<<" Found Temp "<<TemperatureL(min/Rconst,e1)<<"\n";ChRead();
      fstream out(buf,ios::out);
out<<"        Dencity          Pcold             Ecold               Ecold_Hug           Grunizen           Teta             Ccold2          PNull\n";
      for (double k=min;k<=max;k+=stp)  
        {
         x=k/Rconst;
         out<<FM<<k<<
         FMT<<Pcold(x)<<FMT<<
         Ecold(x)<<FMT<<
//            Ecold_Ehug(x)<<
           FMT<<Grun(x)<<
                   FMT<<Teta(x)<<FMT<<
//                   Ccold2(x)<<FMT<<PNull(x)<<
"\n";
        };
     }
  };   
#endif

void ShowIsoTherm(fstream &in)
  {
   char buf[256];
   double T,P,E,T0,x;
//   MatterH2Sol H2;
   MatterIO *M;
   if (!MatterRegister(in,M)) { cout<<"Could not register. Exit.\n";exit(0);}
//   MatterH2Liq H2;H2.ReadIni(in);
   double min,max,stp;
   if (InputPar(in,T0,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
//     Rcritical=Rconst*Bconst/(fabs(Bconst-1)+StndErr);
out<<" Isotherm\n";
out<<"    Dencity           Pressure       Temperature         Energy \n";
      for (double k=min;k<=max;k+=stp)  
        {
         E=M->Energy(k,T0);
//         x=k/Rconst;
//         E=Eliquid(x,T0);  // T0 is Temperature
         T=M->Temperature(k,E);
         P=M->Pressure(k,E);
         out<<FM<<k<<FMT<<P<<FMT<<T
                   <<FMT<<E<<FMT<<"\n";
        }
     }   
   delete M;
  };
MatterIO  *IsoM;
double IsoDeriv(double D,double E) 
  { return IsoM->Pressure(D,E)/sqr(D);};
//  {	return D/E;};
void ShowIsoEntrope(fstream &in)
  {
   char buf[256];
   double T,P,E,E0,x,x_c,s=1e-5;
			int res;
   MatterIO *M;
   if (!MatterRegister(in,M)) { cout<<"Could not register. Exit.\n";exit(0);}
   IsoM=M;
   double min,max,stp,Rconst=M->DencityCold();
   if (InputPar(in,E0,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
//     Rcritical=Rconst*Bconst/(fabs(Bconst-1)+StndErr);
out<<" Isoentrope\n";
out<<"        Dencity          Pressure          Temperature         Energy\n";
      for (double k=min;k<=max;k+=stp)  
        {
         x=k/Rconst;
//         E=Eliquid(x,T0);  // T0 is Temperature
         if (res=NotRungCurt(IsoDeriv,min,E0,k,x_c,E,s)!=0)
									  { cout<<" Bad in  NotRungCurt "<<res<<" x1= "<<min<<" E0 "<<E0
											      <<" x_ "<<k<<" x2 "<<x_c<<" y2 "<<E<<" s "<<s<<"\n";}
//									out<<FM<<k<<FMT<<E<<"\n";
         T=M->Temperature(k,E);
         P=M->Pressure(k,E);
         out<<FM<<k<<FMT<<P<<FMT<<T<<FMT<<E<<"\n";
        }
     }   
   delete M;
  };

void ShowTemp(fstream &in)
  {
   double Dmax,Dmin,Emax,Emin;
   double R,E,P,T,S;
   char data[256],res[256];
   if (InputPar(in,Dmax,Dmin,Emax,Emin,data))
     {
      SetExt(data,res,"res");
      fstream inp(data,ios::in);fstream out(res,ios::out);
out<<"        Dencity          Pressure      Temperature         Energy        Sound\n";
      while (!seek_eof(inp)) 
        {inp>>R>>E;
         P=MatterAdiabat->Pressure(R,E);
//cout<<" P \n";
         T=MatterAdiabat->Temperature(R,E);
//cout<<" T \n";
         S=MatterAdiabat->Sound(R,E);
//cout<<" S \n";
         out<<FM<<R<<FMT<<P<<FMT<<T<<FMT<<E<<FMT<<S<<"\n";
        } 
      inp.close();out.close();
     }
  };    

#ifdef MatterSpline_Register
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

#endif


//===========================================================
//==============  Calculate Hugoniot From Velocities ========
//====================Old New version  ======================
//=================== Finds only upper root =================
//===========================================================
/*
double MisfitHug(double r2)
  {
   double v2=1/r2;
   double porous=ConstPar[0];
   double p1=ConstPar[1];
   double e1=ConstPar[2];
   double v1=ConstPar[3];
   double du=ConstPar[4];
   double p2=p1-du/(v2-v1);
   double e2=e1-(v2-v1)*p1+0.5*du;
   double misfit=PressurePorous(r2,e2)-p2;
   return misfit;
  };

void SetHugPar(double &a1,double &a2,double &a3,double &a4,
               double u1,double r1,double p1,double e1,double u2,
               double porous=1)
  {
   a4=sqr(u2-u1);a1=p1;a2=e1;a3=1/r1;
//cout<<" a1 "<<a1<<" a2 "<<a2<<" a3 "<<a3<<" a4 "<<a4<<"\n";
   ConstPar=new double [5];
   ConstPar[0]=porous; ConstPar[1]=a1;
   ConstPar[2]=a2; ConstPar[3]=a3;  ConstPar[4]=a4;
  };
void Hug(double u1,double r1,double p1,double e1,
          double u2,double &r2,double &p2,double &e2,double &D,
          double porous,double Rcritical)
// Calculates from velocites hugoniot. D - calculates
  {
   double a1,a2,v1,du;
   SetHugPar(a1,a2,v1,du,u1,r1,p1,e1,u2,porous);
   if (fabs(du)>MathZer)
     {
//      double v2=1/(Rcritical-StndErr*100);
      double v2;
//      r2=(Rcritical-StndErr*100);      // Good for D=A+B*u EOS
      r2=max(r1,MatterAdiabat->DencityCold())+1e4*StndErr;   
      if (!ZeroNewtonIt(MisfitHug,r2,StndErr,1000,Rcritical-StndErr,StndErr))
        {cout<<" Could not find match in Hug.\n";ChRead();}
//      r2=1/v2;
      v2=1/r2;
      p2=p1-du/(v2-v1);
      e2=e1-(v2-v1)*p1+0.5*du;
      D=(r2*u2-r1*u1)/(r1-r2);
double misfit=PressurePorous(r2,e2)-p2;
cout<<" p2 "<<p2<<" r2 "<<r2<<" e2 "<<e2<<" du "<<du<<"\n";
if (fabs(misfit)>10*StndErr) cout<<" mis "<<misfit<<"  "<<MisfitHug(v2)<<" D "<<D<<" r1 "<<r1<<" p1 "<<p1<<"\n";//ChRead();
     }
   else {D=0;r2=r1;p2=p1;e2=e1;};
   delete ConstPar;
  };
*/

