#ifndef __FRE_ROS_H
#define __FRE_ROS_H 


//#include "fre_fac.h"
#include "util.h"
#include "mtfunc.h"
//#include "h2_liqui.h"
//#include "empi_url.h"
#include "mtquad.h"
//#include "matlib.h"
#include "mtmin.h"

#include "spl_reg.h"
struct RossParam
  {
   double Rconst;
   double MolVeight,Zero,CvId,HiT,ParInteract[8],Tvib;
   double NiuL,TL,DL;
   double P6,Ec0,Ec1,E0,E1,X;
   SplineXYIO *PercYerv;
   double S_Niu,S_Diameter,S_DensDia,S_Temp,S_MaxR_PY;
   RossParam(){Rconst=8.31441e-3;PercYerv=NULL;};
   ~RossParam(){delete PercYerv;PercYerv=NULL;NiuL=0;DL=0;TL=0;}
  };

double FreeSSVT(RossParam *par,double Denc,double T);
int FreeERossRead(fstream &in,RossParam *&par);
double FreeRCheck(RossParam *par,double Denc,double T,double niu);

   
#endif


/*
#include "spl_reg.h"
struct FreeERoss:InterfaceFindSubstanceIO,FreeEIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}
   static const double Rconst;
   double MolVeight,Zero,CvId,HiT,ParInteract[8],Tvib;
   double NiuL,TL,DL;
   SplineXYIO *PercYerv;
   static SplineXYIO *S_PercYerv;
   static double *S_ParInteract,S_Niu,S_Diameter,S_DensDia,S_Temp,S_MaxR_PY;

   FreeERoss(){PercYerv=NULL;}
   void DeleteVar(){delete PercYerv;PercYerv=NULL;NiuL=0;DL=0;TL=0;}
   ~FreeERoss(){DeleteVar();}

   double FreeE(double Denc,double T);
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
   void ReadSubstancePar(fstream &in);

   static double InteractFunc(double r)
     {
      if (r<S_ParInteract[0]) return S_ParInteract[4]*exp(S_ParInteract[5]*r);
      double Epsilon=S_ParInteract[1];
      double Alpha  =S_ParInteract[2];
      double Radius =S_ParInteract[3];
//cout<<"  "<<r<<" "<<Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6))<<"\n";
      return Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6));
     }; 
   static double IntegPY(double r)
     {
      double niu=S_Niu,rad=r/S_Diameter,res;
      S_PercYerv->Evaluate(&rad,&niu,&res,1);
//cout<<"  "<<r<<" "<<res<<"\n";
//res=0;
      return sqr(r)*InteractFunc(r)*(res+1);
     }; 
   static void IntegFixed(double *x,double *y,double Min,double Max,int N)
     {
      double Cur=Min,Stp=(Max-Min)/(N-1),*tmp;
      tmp=new double[N+1];
      Cur/=S_Diameter;Stp/=S_Diameter;
      double niu=S_Niu;
      for (int k=1;k<=N;k++) {x[k]=Cur;tmp[k]=niu;Cur+=Stp; }
      S_PercYerv->Evaluate(&x[1],&tmp[1],&y[1],N);
      double Epsilon=S_ParInteract[1];
      double Alpha  =S_ParInteract[2];
      double Radius =S_ParInteract[3];
      double C=Epsilon/(Alpha-6);
      Cur=Min;Stp=(Max-Min)/(N-1);
      for (k=1;k<=N;k++) 
        {x[k]=Cur;
         y[k]=(y[k]+1)*sqr(Cur)*C*
                      (6*exp(Alpha*(1-Cur/Radius))-Alpha*pow(Radius/Cur,6));          
         Cur+=Stp; 
        }
      delete tmp;
     }; 

        
   static double PY_Min(double niu)
     {
//cout<<" PY_Min niu "<<niu<<"\n";
      S_Niu=niu;
//   S_Temp=Rconst/MolVeight*T; S_DensDia=Denc/MolVeight;
// To obtain S_Diameter in Bor radius
      S_Diameter=pow(6*niu/(S_DensDia*M_PI),1./3)*2.238515;
      double niu2=sqr(niu);
      double F_HS=(4*niu-3*niu2)/(1-niu2);
      double F_SS=-(sqr(niu2)*0.5+niu2+niu*0.5);
      int Err;
//      double Int=InFinQulb9(IntegPY,S_Diameter*(1+StndErr),3e-4,1e-3,
//                      10,3000,Err);
      double MaxR=S_MaxR_PY*S_Diameter;
//cout<<" PY_Min S_Diameter "<<S_Diameter;//cout<<" F(M) "<<IntegPY(MaxR);
      double Int=RecInt(IntegPY,S_Diameter*(1+StndErr),MaxR,1e-3);
// /*      int NInt=200,Flag;  <- add /*
      double *x,*y;x=new double [NInt+1];y=new double [NInt+1];
      IntegFixed(&x[0],&y[0],S_Diameter*(1+StndErr),1.7*S_Diameter,NInt);
      double er;
      double Int=d01gae_(&x[1],&y[1], NInt, er,Flag);
//cout<<" er "<<er<<" Flag "<<Flag;
      NInt=100;
      IntegFixed(&x[0],&y[0],S_Diameter*1.7,MaxR,NInt);
      Int+=d01gae_(&x[1],&y[1], NInt, er,Flag);
//cout<<" er "<<er<<" Flag "<<Flag<<"\n";
      delete x;delete y;
// -/                         <- end -/
      double Epsilon=S_ParInteract[1],Alpha=S_ParInteract[2],Radius=S_ParInteract[3];
      double X0=Alpha*(1-MaxR/Radius),C=Epsilon/(Alpha-6),B=pow(Radius,6)*Alpha;
//cout<<" PY_Min Int Mis "<<Int-InFinQulb9(IntegPY,S_Diameter*(1+StndErr),
//                              3e-4,1e-3,10,3000,Err);
      Int-=C*(6*Radius/pow(Alpha,3)*(sqr(Alpha*MaxR)+2*Alpha*Radius*MaxR+2*sqr(Radius))*exp(X0)
           +B/(3*pow(MaxR,3)));
//cout<<" PY_Min Int Mis+ "<<Int-InFinQulb9(IntegPY,S_Diameter*(1+StndErr),
//                              3e-4,1e-3,10,3000,Err)<<"\n";
//cout<<" PY_Min Int "<<S_Temp*(F_HS+F_SS)+S_DensDia*Rconst*Int*0.5<<" Err "<<Err<<"\n";
      return S_Temp*(F_HS+F_SS)+S_DensDia*Rconst*Int*0.5;
     }; 
  }; 
*/
