
#include "mat\dat_mani.h";
#include "mat\zero.h"
#include "matt_his.h";
#include "hisch.h";




TData<double> *data,*Shock,*Temp,*GS_par;
double StndErr=1e-8;
// Grun==-100 - GetGrun, Else - set it


int InputUDT(char *name,int S,double Grun)
  {
   InputTDataF(name,data);
   ConstructSpl(*data,0,2,Shock);
   ConstructSpl(*data,0,4,Temp);
   if (S) ConstructSpl(*data,0,1,GS_par);
   else 
     {
      if (Grun!=-100) 
       {SetTDataColumn(*data,3,Grun);
        for (int k=0;k<data->I[0];k++) 
          data->D[3][k]=Grun*(data->D[2][k]-data->D[0][k])/data->D[2][k];
       }   
      ConstructSpl(*data,0,3,GS_par);
     } 
cout<<" input ";fstream tmp("tmp",ios::out);
OutputTDataF(tmp,*data);
//DataStore(tmp,Shock); 
//DataStore(tmp,Temp); DataStore(tmp,GS_par);
tmp.close();
//cout<<" Grun "<<Grun<<"\n";
   return 1;
  };                 

void ClcU_G(double u,double &d,double &t,double &g,double &c,double &s,int Set_s)
  {
//cout<<" u "<<u;
   d=EvalSpl(*data,0,2,*Shock,u);
   double Stp=max(fabs(u*StndErr),MathZer);
   double d1=EvalSpl(*data,0,2,*Shock,u+Stp);d1=(d1-d)/Stp;
   t=EvalSpl(*data,0,4,*Temp,u);
   double t1=EvalSpl(*data,0,4,*Temp,u+Stp);t1=(t1-t)/Stp;
   if (!Set_s)
     {
      g=EvalSpl(*data,0,3,*GS_par,u);
      s=sqrt(fabs(( g*d1*(d-u)*sqr(u)-sqr(d-u)*(d1*u+d) )/(d1*u-d)));
     }
   else 
     {
      s=EvalSpl(*data,0,1,*GS_par,u);
      g=(sqr(d-u)*(d1*u+d)+sqr(s)*(d1*u-d))/(d1*(d-u)*sqr(u));
     }  
   c=sqr(u)*d1*(d-u)/( d*(d-u)*t1+g*t*(d1*u-d) );
//cout<<" d "<<d<<" t "<<t<<" g "<<g<<" c "<<c<<" s "<<s<<"\n ";
  };                 

int InputPar(fstream &in,double &from,double &min,double &max,
                                                 double &stp,char *write)
  {
//   fstream in(name,ios::in);
   int ret=0;
   in>>write>>from>>write>>min>>write>>max>>write>>stp;in.getline(write,255);
   in>>write>>ret>>write;
   return ret;
  };
void SetHisch(MatterHisch &Hisch,double Ed,double Denc,
              double ZeroUPoint,int SetS)
  {
//   double ZeroPoint=0;
   double Stp=StndErr;
   double d0,t0,g0,c0,s0,d1,t1,g1,c1,s1;
   ClcU_G(ZeroUPoint    ,d0,t0,g0,c0,s0,SetS);
   ClcU_G(ZeroUPoint+Stp,d1,t1,g1,c1,s1,SetS);
   double C0,a,b,Y0,Yd;
   C0=s0-(s1-s0)*ZeroUPoint/Stp;b =(d1-d0)/Stp;
   double dR=Denc*(d1/(d1-ZeroUPoint-Stp)-d0/(d0-ZeroUPoint));
   Y0=g0-(g1-g0)*ZeroUPoint/Stp;Yd=(g1-g0)/dR;
   double MyGd=(Hisch.Hisch.G0c-2./3)*2*Hisch.Hisch.V0*log(Hisch.Hisch.Sm)/
               (sqr(Hisch.Hisch.Sn)+sqr(log(Hisch.Hisch.Sm)));
   double Sigm=Hisch.Hisch.V0c/Hisch.Hisch.V0;
   double HGd=(Grun(Sigm+Stp,0,Hisch.Hisch)-Grun(Sigm,0,Hisch.Hisch))/Stp*Hisch.Hisch.V0c;
   double BV=Hisch.Hisch.V0c*Hisch.Hisch.B0c;
   cout<<"\n    K      My\n";
   cout<<" V0 "<<Hisch.Hisch.V0<<" "<<1/Denc<<"\n";
   cout<<" C0 "<<sqrt(BV)<<" "<<C0<<" "<<s0<<" "<<d0<<"\n";
   cout<<" b  "<<(Hisch.Hisch.N+Hisch.Hisch.M+3.)/4<<" "<<b<<"\n";
   cout<<" Ed "<<BV/(Hisch.Hisch.M*Hisch.Hisch.N)<<" "<<Ed<<"\n";
   cout<<" Y0 "<<Hisch.Hisch.G0c<<" "<<Y0<<"\n";
   cout<<" Yd "<<HGd<<" MyYd "<<MyGd<<" TabGd "<<Yd<<"\n";
   cout<<" B0c "<<Hisch.Hisch.B0c<<" "<<sqr(C0)*Denc<<"\n";
   double Con1=(Hisch.Hisch.G0c-2./3)*2*Hisch.Hisch.V0*log(Hisch.Hisch.Sm);
   double Con2=sqr(log(Hisch.Hisch.Sm));
   cout<<" sn  "<<Hisch.Hisch.Sn<<" "<<sqrt(Con1/MyGd-Con2)<<" "
                <<sqrt(Con1/Yd-Con2)<<"\n";
   double N1,N2;SqPolynom(1,3-4*b,sqr(C0)/Ed,N1,N2);
   cout<<" Hisch M "<<Hisch.Hisch.M<<" N "<<Hisch.Hisch.N<<"\n";
   cout<<" N   "<<N1<<"  "<<N2<<"\n";
   cout<<" M   "<<sqr(C0)/(N1*Ed)<<" "<<sqr(C0)/(N2*Ed)<<"\n";

  };

void Draw_SoundCv(fstream &in)
  {
   char tmp[256],udt[50];
   double ZeroUPoint,Ed,Denc;
   in>>tmp>>udt>>tmp>>ZeroUPoint>>tmp>>Ed>>tmp>>Denc;in.getline(tmp,256);
   MatterHisch Hisch;Hisch.ReadSubstancePar(in);
   double G0,min,max,stp,d,t,g,c,s;
   if (InputPar(in,G0,min,max,stp,tmp))
     {
      if (!InputUDT(udt,0,G0)) return ;
      fstream out(tmp,ios::out);
out<<" Hug\n";
out<<"      U            S             D           Gamma       T      Cv\n";
      for (double u=min;u<=max;u+=stp)  
        {
         ClcU_G(u,d,t,g,c,s,0);
         out<<FMT<<u<<FMT<<s<<FMT<<d<<FMT<<g<<FMT<<t<<FMT<<c<<"\n";
        } 
      out.close();
      SetHisch(Hisch,Ed,Denc,ZeroUPoint,0);
     }
  };    
void Draw_GammaCv(fstream &in)
  {
   char tmp[256],udt[50];
   double ZeroUPoint,Ed,Denc;
   in>>tmp>>udt>>tmp>>ZeroUPoint>>tmp>>Ed>>tmp>>Denc;in.getline(tmp,256);
   MatterHisch Hisch;Hisch.ReadSubstancePar(in);
   double G0,min,max,stp,d,t,g,c,s;
   if (InputPar(in,G0,min,max,stp,tmp))
     {
      if (!InputUDT(udt,1,G0)) return ;
      fstream out(tmp,ios::out);
out<<" Hug\n";
out<<"      U      S      D            Gamma           T        Cv\n";
      for (double u=min;u<=max;u+=stp)  
        {
         ClcU_G(u,d,t,g,c,s,1);
         out<<FMT<<u<<FMT<<s<<FMT<<d<<FMT<<g<<FMT<<t<<FMT<<c<<"\n";
        } 
      out.close();
      SetHisch(Hisch,Ed,Denc,ZeroUPoint,1);
     }
  };    
double CurrentU,DencZero,EnergyZero,DencBeg,CurrentDensity,GrunBeg;
double UBefore;
MatterHisch Hisch;
double MisfitDens(double UBeg)
  {double d=EvalSpl(*data,0,2,*Shock,UBeg);
//cout<<" CurrentDensity "<<CurrentDensity<<" d "<<d<<" UBeg "<<UBeg<<" DencBeg "<<DencBeg<<" de "<<CurrentDensity-DencBeg*d/(d-UBeg)<<"\n";
   return CurrentDensity-DencBeg*d/(d-UBeg);
  };
double MisfitPres(double d)
  {
   CurrentDensity=DencZero*d/(d-CurrentU);
//cout<<" CurrentDensity "<<CurrentDensity<<" d "<<d<<" CurrentU "<<CurrentU<<"\n";
//   double UBeg=FindU(CurrentDensity);
   double UBeg=1;
   if (!ZeroNewtonIt(MisfitDens,UBeg,StndErr,1000,1/StndErr,sqrt(StndErr),0.5))
     {cout<<" Bad MisfitPres in ZeroNewtonIt.\n";}//ChRead();}
   UBefore=UBeg;
   double DBeg=EvalSpl(*data,0,2,*Shock,UBeg);
   double Stp=max(fabs(UBeg*StndErr),MathZer);
   double D1Beg=EvalSpl(*data,0,2,*Shock,UBeg+Stp);D1Beg=(D1Beg-DBeg)/Stp;
   double PHugI=DencBeg*DBeg*UBeg;
   double EHugI=sqr(UBeg)/2;
//   return PHugI-(DencZero*d*CurrentU-EvalSpl(*data,0,3,*GS_par,UBeg)*DencZero*
//                (sqr(CurrentU)/2+EnergyZero-EHugI));
   double s=EvalSpl(*data,0,1,*GS_par,UBeg);
   double g=(sqr(DBeg-UBeg)*(D1Beg*UBeg+DBeg)+sqr(s)*(D1Beg*UBeg-DBeg))
       /(D1Beg*(DBeg-UBeg)*sqr(UBeg));
   GrunBeg=g;
   return PHugI-(DencZero*d*CurrentU-g*CurrentDensity*
                (sqr(CurrentU)/2+EnergyZero-EHugI));
  };

   
void Hgh_D(fstream &in)
  {
   char tmp[256],udt[50];
   in>>tmp>>udt;in.getline(tmp,256);
   in>>tmp>>DencZero>>tmp>>EnergyZero>>tmp>>DencBeg;in.getline(tmp,256);
   Hisch.ReadSubstancePar(in);
   double G0,min,max,stp,d,t,g,c,s;
   if (InputPar(in,G0,min,max,stp,tmp))
     {
      if (!InputUDT(udt,1,G0)) return ; // input Grunizen ( calc sound )
      fstream out(tmp,ios::out);
out<<" Hug\n";
out<<"      U       D      GrunBeg   \n";
      d=min+2;
      for (double u=min;u<=max;u+=stp)
        {
         CurrentU=u;
         if (!ZeroNewtonIt(MisfitPres,d,StndErr,1000,1/StndErr,sqrt(StndErr),1))
           {cout<<" Bad ZeroNewtonIt in Recalc_All. Mis="<<MisfitPres(d)<<"\n";}
//         ClcU_G(u,d,t,g,c,s,0);         //  calc sound 
//cout<<"I\n";
MisfitPres(d);
//double PHeat=DencBeg*EvalSpl(*data,0,2,*Shock,UBefore)*UBefore-DencZero*d*u;

         out<<FMT<<u<<FMT<<d<<FMT<<GrunBeg<<"\n";
        } 
      out.close();
     }
  };    

void Hgh_G(fstream &in)
  {
   char tmp[256],udt[50];
   in>>tmp>>udt;in.getline(tmp,256);
   in>>tmp>>DencZero>>tmp>>EnergyZero>>tmp>>DencBeg;in.getline(tmp,256);
   Hisch.ReadSubstancePar(in);
   double G0,min,max,stp,d,t,g,c,s;
   if (InputPar(in,G0,min,max,stp,tmp))
     {
      if (!InputUDT(udt,1,G0)) return ; // input Grunizen ( calc sound )
      fstream out(tmp,ios::out);
out<<" Hug\n";
out<<"      U       D      GrunBeg   \n";
      d=min+2;
      for (double u=min;u<=max;u+=stp)
        {
         CurrentU=u;
         if (!ZeroNewtonIt(MisfitPres,d,StndErr,1000,1/StndErr,sqrt(StndErr),1))
           {cout<<" Bad ZeroNewtonIt in Recalc_All. Mis="<<MisfitPres(d)<<"\n";}
//         ClcU_G(u,d,t,g,c,s,0);         //  calc sound 
//cout<<"I\n";
MisfitPres(d);
//double PHeat=DencBeg*EvalSpl(*data,0,2,*Shock,UBefore)*UBefore-DencZero*d*u;

         out<<FMT<<u<<FMT<<d<<FMT<<GrunBeg<<"\n";
        } 
      out.close();
     }
  };    

   
void main( int argc, char *argv[] )
  {
   data=NULL;Shock=NULL;Temp=NULL;GS_par=NULL;
   fstream in(argv[1],ios::in);
//   char tmp[256];in>>tmp>>ColdDenc;in.getline(tmp,256);
cout<<"Draw_GammaCv     beg    Coreleft() "<<coreleft()<<"\n";
   Draw_GammaCv(in);
cout<<"Draw_SoundCv  beg  Coreleft() "<<coreleft()<<"\n";
   Draw_SoundCv(in);
cout<<"Hgh_D  beg  Coreleft() "<<coreleft()<<"\n";
   Hgh_D(in);
cout<<"Hgh_G  beg  Coreleft() "<<coreleft()<<"\n";
   Hgh_G(in);
  };
