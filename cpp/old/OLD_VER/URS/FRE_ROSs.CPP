#include "fre_ros.h"

RossParam *CalculateRossParam;
#define RP CalculateRossParam

double InteractFunc(double r)
  {
   if (r<RP->ParInteract[0]) return RP->ParInteract[4]*exp(RP->ParInteract[5]*r);
   double Epsilon=RP->ParInteract[1];
   double Alpha  =RP->ParInteract[2];
   double Radius =RP->ParInteract[3];
//cout<<"  "<<r<<" "<<Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6))<<"\n";
   return Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6));
  }; 
double IntegPY(double r)
  {
   double niu=RP->S_Niu,rad=r/RP->S_Diameter,res;
   RP->PercYerv->Evaluate(&rad,&niu,&res,1);
//cout<<"  "<<r<<" "<<res<<"\n";res=0;
   return sqr(r)*InteractFunc(r)*(res+1);
  }; 

double PY_Min(double niu)
  {
   RP->S_Niu=niu;
//   S_Temp=Rconst/MolVeight*T; S_DensDia=Denc/MolVeight;
// To obtain S_Diameter in Bor radius
   RP->S_Diameter=pow(6*niu/(RP->S_DensDia*M_PI),1./3)*2.238515;
   double niu2=sqr(niu);
   double F_HS=(4*niu-3*niu2)/(1-niu2);
   double F_SS=-(sqr(niu2)*0.5+niu2+niu*0.5);
   int Err;
   double MaxR=RP->S_MaxR_PY*RP->S_Diameter;
   double Int=RecInt(IntegPY,RP->S_Diameter*(1+StndErr),MaxR,1e-3);
   double Epsilon=RP->ParInteract[1],Alpha=RP->ParInteract[2],Radius=RP->ParInteract[3];
   double X0=Alpha*(1-MaxR/Radius),C=Epsilon/(Alpha-6),B=pow(Radius,6)*Alpha;
//cout<<" PY_Min Int Mis "<<Int-InFinQulb9(IntegPY,S_Diameter*(1+StndErr),
//                              3e-4,1e-3,10,3000,Err);
      Int-=C*(6*Radius/pow(Alpha,3)*(sqr(Alpha*MaxR)+2*Alpha*Radius*MaxR+2*sqr(Radius))*exp(X0)
           +B/(3*pow(MaxR,3)));
// Addition to the integral - integral from MaxR to the infinity with PY=1
//cout<<" PY_Min Int Mis+ "<<Int-InFinQulb9(IntegPY,S_Diameter*(1+StndErr),
//                              3e-4,1e-3,10,3000,Err)<<"\n";
//cout<<" PY_Min Int "<<S_Temp*(F_HS+F_SS)+S_DensDia*Rconst*Int*0.5<<" Err "<<Err<<"\n";
      return RP->S_Temp*(F_HS+F_SS)+RP->S_DensDia*RP->Rconst*Int*0.5;
     }; 
   
double FreeSSVT(RossParam *par,double Denc,double T)
  {
   RP=par;
   if ((T<MathZer) || (Denc<MathZer)) 
     { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
   RP->S_Temp=RP->Rconst/RP->MolVeight*T;RP->S_DensDia=Denc/RP->MolVeight;
   RP->S_MaxR_PY=6;
   double PY_add;
   if ((fabs(RP->DL-Denc)<1e-2) && (fabs(RP->TL-T)<1)) ;
   else { RP->DL=Denc;RP->TL=T;
//    RP->NiuL=MinFunc(0.035,0.62,PY_Min,2e-3);}
   if (!NAGMin(PY_Min,2e-3,2e-3,0.035,0.7,1000,RP->NiuL,PY_add))
      cout<<" Did not find RP->NiuL. Try old version\n";}
   PY_add=PY_Min(RP->NiuL);
   double F=RP->Zero+PY_add-RP->S_Temp*
            (log(max<double>(1/Denc,MathZer))+RP->CvId*log(T)+RP->HiT+
                 log(1-exp(-RP->Tvib/T)));
   return F;
  }; 

int FreeERossRead(fstream &in,RossParam *&par)
  {
//cout<<" FreeERoss reading\n";
   par=new RossParam;RP=par;
   char tmp[256];
   in>>tmp>>RP->CvId>>tmp>>RP->MolVeight>>tmp>>RP->Zero>>tmp>>RP->HiT>>tmp>>RP->Tvib;
   in>>tmp>>RP->ParInteract[1]>>tmp>>RP->ParInteract[2]>>tmp>>RP->ParInteract[3];
   in>>tmp>>RP->ParInteract[0];
   in>>tmp>>tmp;//spl_name=strdup(tmp);
   fstream sp(tmp,ios::in);
   int ret=1;
//cout<<" FreeERoss spline registering. Name: "<<tmp<<"\n";
   if (!(ret=SurfRegister(sp,RP->PercYerv))) 
       {cout<<" Could not register Percus. file: "<<tmp
            <<" :\nNow exititng\n";exit(0);}
cout<<" FreeERoss Entered. Coreleft "<<Coreleft()<<"\n";
//   S_ParInteract=&ParInteract[0];
   double x=RP->ParInteract[0]*(1+StndErr);
   double c=InteractFunc(x);
   double c1=InteractFunc(x*(1+StndErr));
   c1=(c1-c)/(x*StndErr);
   RP->ParInteract[4]=c/(exp(c1/c*x));
   RP->ParInteract[5]=c1/c;
   return 1;
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
