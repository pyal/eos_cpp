//  Main for FreeE - coexistance Curve
#include "free_fun.h"

double StndErr=1e-6;


InterfaceFreeEIO *F1,*F2,*F;
double CurPres,CurTemp,CurDenc1,CurDenc2,EnterCoef;
double DeltaPres(double Denc)
  {
cout<<" DeltaPres Denc "<<Denc<<" Temp "<<CurTemp<<" CurP "<<CurPres;
cout<<" Delta "<< CurPres-F->Pressure(Denc,CurTemp)<<"\n";
   return CurPres-F->Pressure(Denc,CurTemp);
   }
void FindDenc(double &Denc1,double &Denc2,double Temp)
  { 
   CurTemp=Temp;
   Denc1=CurDenc1;F=F1;
//cout<<" FindDenc1 Temp "<<CurTemp<<" Denc1 "<<Denc1<<"\n";
   if (!ZeroNewtonIt(DeltaPres,Denc1,StndErr*0.01,1000,1e6,MathZer,0.01*Denc1,0.1))
      {cout<<" Bad ZeroNewtonIt in FindDenc1 Not Zero\nfound Denc "
           <<Denc1<<" Mis "<<DeltaPres(Denc1)<<"\n";}
   Denc2=CurDenc2;F=F2;
//cout<<" FindDenc2 Temp "<<CurTemp<<" Denc2 "<<Denc2<<"\n";
   if (!ZeroNewtonIt(DeltaPres,Denc2,StndErr*0.01,1000,1e6,MathZer,0.01*Denc2,0.1))
      {cout<<" Bad ZeroNewtonIt in FindDenc2 Not Zero\nfound Denc "
           <<Denc2<<" Mis "<<DeltaPres(Denc2)<<"\n";}
   CurDenc1=Denc1;CurDenc2=Denc2;
  }  
   
double DeltaFree(double Temp)
  {
   double Denc1,Denc2;
   FindDenc(Denc1,Denc2,Temp);
   return F1->FreeE(Denc1,Temp)-F2->FreeE(Denc2,Temp) 
                   +CurPres*EnterCoef*(1/Denc1-1/Denc2);
  };

void ShowBound(fstream &in)
  {
   char buf[256];
   double min,max,stp,T0=10;
   double E1,E2,P1,P2;
   F1=NULL;F2=NULL;
   if ( (!FreeERegister(in,F1)) || (!FreeERegister(in,F2)) )
     { cout<<"ShowBound -> could not register.Exiting.\n";
       InputPar(in,CurDenc1,min,max,stp,buf);return ;}
   in>>buf>>CurDenc1>>buf>>CurDenc2>>buf>>EnterCoef;
   if (InputPar(in,T0,min,max,stp,buf))
     {
//      CurDenc2=CurDenc1;
      fstream out(buf,ios::out);
//  from min to max, for dencity, given dencity - search temperature
//  draw - denctiy, temp, pressure, energy
out<<" Coexistence curve \n";
out<<"   P      Temperature        E1        E2        D1        D2        dP1          dP2    dFE\n";
      for (double k=min;k<=max;k+=stp)
        {
//cout<<" Iter\n";
         CurPres=k;
         if (!ZeroNewtonIt(DeltaFree,T0,StndErr*0.01,1000,1e6,MathZer,0.3*T0,0.1))
            {cout<<" Bad ZeroNewtonFreeE in MatterFreeE::Temperature Not Zero\nfound T "
                 <<T0<<" Mis "<<DeltaFree(T0)<<"\n";}
         E1=F1->Energy(CurDenc1,T0);
         P1=F1->Pressure(CurDenc1,T0);
         E2=F2->Energy(CurDenc2,T0);
         P2=F2->Pressure(CurDenc2,T0);
         double D1=CurDenc1,D2=CurDenc2;
         out<<FM<<k<<FMT<<T0<<FMT<<FMT<<E1<<FMT<<E2
            <<FMT<<CurDenc1<<FMT<<CurDenc2<<FMT<<P1-k<<FMT<<P2-k<<FMT<<DeltaFree(T0)<<FMT<<D1-CurDenc1<<FMT<<D2-CurDenc2<<"\n";
        }
     }
   delete F1;delete F2;
  };

double DeltaFree1(double Temp)
  {
cout<<" D "<<CurDenc1<<" T "<<Temp<<" Mis "
    <<F1->Pressure(CurDenc1,Temp)-F2->Pressure(CurDenc1,Temp)<<"\n";
   return F1->Pressure(CurDenc1,Temp)-F2->Pressure(CurDenc1,Temp);
  };

void ShowBound_DencEQ(fstream &in)
  {
   char buf[256];
   double min,max,stp,T0=10;
   double E1,E2,P1;//,P2;
   F1=NULL;F2=NULL;
   if ( (!FreeERegister(in,F1)) || (!FreeERegister(in,F2)) )
     { cout<<"ShowBound -> could not register.Exiting.\n";
       InputPar(in,CurDenc1,min,max,stp,buf);return ;}
//   in>>buf>>CurDenc1>>buf>>CurDenc2;
   if (InputPar(in,T0,min,max,stp,buf))
     {
//      CurDenc2=CurDenc1;
      fstream out(buf,ios::out);
//  from min to max, for dencity, given dencity - search temperature
//  draw - denctiy, temp, pressure, energy
out<<" Coexistence curve (V1=V2) PVT1=PVT2\n";
out<<"            D      Temperature        E1        E2       P1    dP1    \n";
      for (double k=min;k<=max;k+=stp)
        {
//cout<<" Iter\n";
         CurDenc1=k;
         if (!ZeroNewtonIt(DeltaFree1,T0,StndErr,1000,1e6,MathZer,0.3*T0,0.1))
            {cout<<" Bad ZeroNewtonFreeE in MatterFreeE::Temperature Not Zero\nfound T "
                 <<T0<<" Mis "<<DeltaFree1(T0)<<"\n";}
         E1=F1->Energy(CurDenc1,T0);
         P1=F1->Pressure(CurDenc1,T0);
         E2=F2->Energy(CurDenc1,T0);

         out<<FM<<k<<FMT<<T0<<FMT<<FMT<<E1<<FMT<<E2
            <<FMT<<P1<<FMT<<DeltaFree1(T0)<<"\n";
        }
     }
   delete F1;delete F2;
  };

CurveSpline *Spl;
double DeltaFree2(double Temp)
  {
//cout<<" D "<<CurDenc1<<" T "<<Temp<<" Mis "
//    <<F1->Pressure(CurDenc1,Temp)-F2->Pressure(CurDenc1,Temp)<<"\n";
   double DeltaP;Spl->Evaluate(&CurDenc1,&DeltaP,1);
   return F1->Pressure(CurDenc1,Temp)-F2->Pressure(CurDenc1,Temp)-DeltaP;
  };

void ShowBound_DencEQ1(fstream &in)
  {
   char buf[256],name[256];
   double min,max,stp,T0=10;
   double E1,E2,P1;//,P2;
   F1=NULL;F2=NULL;
   if ( (!FreeERegister(in,F1)) || (!FreeERegister(in,F2)) )
     { cout<<"ShowBound -> could not register.Exiting.\n";
       InputPar(in,CurDenc1,min,max,stp,buf);return ;}
   
   in>>buf>>name;

   if (InputPar(in,T0,min,max,stp,buf))
     {
//      CurDenc2=CurDenc1;
      fstream out(buf,ios::out);

      int NumX;double Misf;TData<real> *dat=NULL;
      InputTDataF(name,dat);
      fstream f_in(name,ios::in);f_in>>buf>>NumX>>buf>>Misf;f_in.close();
      Spl=new CurveSpline;Spl->Generate(NumX,Misf,0,*dat,0,1);
      Spl->Evaluate(*dat);SetExt(name,name,"cmp");fstream f_out(name,ios::out);OutputTDataF(f_out,*dat);

//  from min to max, for dencity, given dencity - search temperature
//  draw - denctiy, temp, pressure, energy
out<<" Coexistence curve (V1=V2) P1(VT)=P2(VT)+Delta\n";
out<<"      D    Temperature  E1        E2       P1    dP1    \n";
      for (double k=min;k<=max;k+=stp)
        {
//cout<<" Iter\n";
         CurDenc1=k;
         if (!ZeroNewtonIt(DeltaFree2,T0,StndErr,1000,1e6,MathZer,0.3*T0,0.1))
            {cout<<" Bad ZeroNewtonFreeE in MatterFreeE::Temperature Not Zero\nfound T "
                 <<T0<<" Mis "<<DeltaFree2(T0)<<"\n";}
         E1=F1->Energy(CurDenc1,T0);
         P1=F1->Pressure(CurDenc1,T0);
         E2=F2->Energy(CurDenc1,T0);

         out<<FM<<k<<FMT<<T0<<FMT<<FMT<<E1<<FMT<<E2
            <<FMT<<P1<<FMT<<DeltaFree2(T0)<<"\n";
        }
      delete Spl;delete dat;
     }
   delete F1;delete F2;
  };


void ShowIsotherm(fstream &in)
  {
   char buf[256];
   double min,max,stp,T0=10;
   double E1,P1;
   F1=NULL;
   if (!FreeERegister(in,F1))
     { cout<<"ShowIsotherm -> could not register.Exiting.\n";
       InputPar(in,CurDenc1,min,max,stp,buf);return ;}
   if (InputPar(in,T0,min,max,stp,buf))
     {
      fstream out(buf,ios::out);
out<<" Isotherm T="<<T0<<"\n";
out<<"     D      P        E1  \n";
      for (double k=min;k<=max;k+=stp)
        {
         E1=F1->Energy(k,T0);
         P1=F1->Pressure(k,T0);
         out<<FM<<k<<FMT<<P1<<FMT<<E1<<"\n";
        }
     }
   delete F1;
  };
   

void main(int argc,char *argv[])
  {
//   StndErr=1e-4;
   fstream in(argv[1],ios::in);
   ShowBound(in);
   ShowBound_DencEQ(in);
   ShowBound_DencEQ1(in);
   ShowIsotherm(in);
  };
