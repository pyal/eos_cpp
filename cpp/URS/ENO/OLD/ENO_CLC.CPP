

#include "eno_clc.h"

#define R_  WorkBody.Pos  

#define U_  WorkBody.Velocity
#define Ro_ WorkBody.Dencity 

#define P_  WorkBody.Pressure
#define M_  WorkBody.Mass    
#define E_  WorkBody.Energy  
#define F_  WorkBody.Fixed
#define V_  WorkBody.Volume

#define Num WorkBody.NumPnt

#define UilWorkDim WorkBody.UilWorkDim


double PressureIse(double Dencity)
 {
  return 0;
 }

double DerivN(double *Pos,double *Par,int Order,double &Cent)
  {
   double D1,D2,C1,C2;
   if (Order>1)
    {
     D1=DerivN(Pos,Par,Order-1,C1);
     D2=DerivN(&Pos[1],&Par[1],Order-1,C2);
    }
   else 
     if (Order==1) {D1=Par[0];D2=Par[1];C1=Pos[0];C2=Pos[1];}
     else {Cent=Pos[0];return Par[0];}
   Cent=(C1+C2)*0.5;                                 
   return (D2-D1)/(C2-C1);
  }
                                                                     
double ClcPoli(double x,double *Pos,Poli3CoefStruct &Coef,int Order)
 {
  double res=Coef.a[0];
  for (int k=1;k<=Order;k++)
   {
    double tmp=Coef.a[k];
    for (int o=0;o<k;o++) tmp*=(x-Pos[Coef.k[k-1]+o]);
    res+=tmp;
   }
  return res;
 };
void ClcPoliCoef(double *Pos,double *Par,Poli3CoefStruct &Coef,int Order)
 {
  int CelNum=Order;
  int Kmin=CelNum;
  Coef.a[0]=Par[Kmin];Coef.k[0]=Kmin;
  for (int k=1;k<=Order;k++)
   {
    double a,b,c;
    if ( (a=fabs(DerivN(&Pos[Kmin],&Par[Kmin],k,c)))>
         (b=fabs(DerivN(&Pos[Kmin-1],&Par[Kmin-1],k,c))) )
      { Coef.a[k]=b; Kmin=Kmin-1;}
    else
       Coef.a[k]=a;
    Coef.k[k]=Kmin;
   }
  for (k=1;k<=Order;k++)
   {  Coef.pos[k]=Pos[Kmin+k-1]; Coef.k[k]-=Kmin+1; }
 };
// in  DeriveVec [k] - k-1 derivative is stored
void ClcPoliDeriv(double x,Poli3CoefStruct &Coef,
                  int Order,VecCl &DeriveVec)
 {
  DeriveVec.Dim(Order+1);
  VecCl Z(Order+1);
  MatrCl L(Order+1);L(0,Order+1)=1;
  for (int k=1;k<=Order;k++)
   { Z[k]=x-Coef.pos[k];L(1,k)=1; }
  for (int l=2;l<=Order+1;l++)
   {
    L(l,l)=L(l-1,l-1)*Z[l-1];
    for (k=l+1;k<=Order+1;k++) L(l,k)=L(l,k-1)+L(l-1,k-1)*Z[k-1];
   }
  double FactK=1,sum;
  for ( k=1;k<=Order+1;k++)
   {
    FactK*=k;sum=0;
    for (l=k;l<=Order+1;l++) sum+=L(l-k,l)*Coef.a[l];
    DeriveVec[k]=FactK*sum;
   }
 };






#define ENO_U_Dim 3
int ENOVarExtractDouble;
struct ENOVarStruct {double u[ENO_U_Dim+1],pos;};

void SetExtractK(int k){ENOVarExtractDouble=k;};
ENOVarStruct U_ENO(double u,ENOVarStruct &par)
 {ENOVarStruct ret=par;int k=ENOVarExtractDouble;
  if (k!=0) ret.u[k]=u;else ret.pos=u;return ret;}
double ENO_U(ENOVarStruct &par)
 {int k=ENOVarExtractDouble;if (k!=0) return par.u[k];else return par.pos;}

ENOVarStruct ENO_TTSum(ENOVarStruct &par,ENOVarStruct &par1)
 {ENOVarStruct ret=par;
  for (int k=1;k<=ENO_U_Dim;k++) ret.u[k]+=par1.u[k];
  return ret;}

VecCl ENO_Vec(ENOVarStruct &par)
 {VecCl ret(ENO_U_Dim);for (int k=1;k<=ENO_U_Dim;k++) ret[k]=par.u[k];return ret;}
void Vec_ENO(ENOVarStruct &par,VecCl &set)
 {for (int k=1;k<=ENO_U_Dim;k++) par.u[k]=set[k];}
//struct ENOGridStruct {double pos;};
struct ENOParStruct {VecCl LambdaMax,LambdaNew;int Order,NumPnt;};

void ENOSetClcVar(){};
void ENOClcTimeStp() { };

void PolyEstimate(double x,double dx,int Order,
              VecFreeCl<ENOVarStruct> &f,ENOVarStruct &res)
 {
  res.pos=x;
  Poli3CoefStruct Coef;//=new Poli3CoefStruct[2*Order+2];
  SetExtractK(0);
  VecCl pos=f.ExtractDouble(),DeriveVec(Order);
  VecCl TailorCoef(Order);TailorCoef=TailorCoef*0;
  TailorCoef[1]=1;if (Order>=2) TailorCoef[3]=1;
  if (Order>=4)  TailorCoef[5]=1;
  for (int k=1;k<=ENO_U_Dim;k++)
   { 
    SetExtractK(k);
    VecCl par=f.ExtractDouble();
    ClcPoliCoef(pos.Ptr,par.Ptr,Coef,Order);
    ClcPoliDeriv(x,Coef,Order,DeriveVec);
    res.u[k]=DeriveVec*TailorCoef;
   }
//  delete Coef;

 }
void ConstructFlux(VecFreeCl<ENOVarStruct> &u,
              VecFreeCl<ENOVarStruct> &f,
              MatrCl &Lft,MatrCl &Eig,MatrCl &Rgt,MatrCl &LambdaMax,
              MatrCl &LambdaMaxNew,int Order,int Cent,
              ENOVarStruct  &fk0_p,
              ENOVarStruct &fk0_m)
 {
  VecFreeCl<ENOVarStruct> fk_p,fk_m;fk_p=u;fk_p.SetIndex(-Order,Order);
  fk_m=fk_p;
  for ( int k=-Order;k<=Order;k++)
   {  
//    uk[k]=u[k+Cent];fk[k]=f[k+Cent];  
    Vec_ENO( fk_p[k],ENO_Vec(f[k+Cent])+Lft*(Eig+LambdaMax)*Rgt*ENO_Vec(u[k+Cent]) );
    Vec_ENO( fk_m[k],ENO_Vec(f[k+Cent])+Lft*(Eig-LambdaMax)*Rgt*ENO_Vec(u[k+Cent]) );
    Vec_ENO( fk_p[k],Lft*ENO_Vec(fk_p[k]) );
    Vec_ENO( fk_m[k],Lft*ENO_Vec(fk_m[k]) );
   }
  double x_m=fk_p[Cent-1].pos,x_0=fk_p[Cent].pos,x_p=fk_p[Cent+1].pos;
  PolyEstimate((x_0+x_p)*0.5, x_p-x_0,Order,fk_p,fk0_p);
  PolyEstimate((x_0+x_m)*0.5, x_0-x_m,Order,fk_m,fk0_m);
  Vec_ENO( fk0_p,ENO_Vec(fk0_p)*Rgt );Vec_ENO( fk0_m,ENO_Vec(fk0_m)*Rgt );
  for (k=1;k<=ENO_U_Dim;k++) { LambdaMaxNew(k,k)=max<double>(LambdaMaxNew(k,k),Eig(k,k));};
 }
void SetFluxK(VecFreeCl<ENOVarStruct> &u,
              VecFreeCl<ENOVarStruct> &f,ENOParStruct &par,int Cent,
              ENOVarStruct &fk_p,ENOVarStruct fk_m)
 {
  int Order=par.Order;
  VecCl LambdaMax=par.LambdaMax*2;
  MatrCl L_Max,L_New;L_Max=LambdaMax;L_New=L_Max*0;
  VecFreeCl<ENOVarStruct> fk,uk;

  MatrCl Lft,Rgt,Eig;
//  DecomposeFlux(u[k],f[k],Lft,Eig,Rgt);  // TO DEFINE
  ConstructFlux(u,f,Lft,Eig,Rgt,L_Max,L_New,Order,Cent,fk_p,fk_m);
  LambdaMax=L_New;
  par.LambdaMax=LambdaMax;
 };  
VecFreeCl<ENOVarStruct> 
  ENOTimeShiftFlux(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par)
 {
  int NumPnt=par.NumPnt;
  VecFreeCl<ENOVarStruct> f_plus,f_minus,f;
//  f=U_To_Flux(u,par);  // TO DEFINE !!!!!!!!!!!!!!!
  f_plus=f;f_minus=f; // Setting operators
  for (int k=0;k<=NumPnt+1;k++)
    SetFluxK(u,f,par,k,f_plus[k],f_minus[k]);
  for (k=0;k<=NumPnt+1;k++)
    f[k]=ENO_TTSum(f_minus[k],f_plus[k+1]);
  return f;
 };
    
void ClcRungKurt(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par,double TimeStp)
 {
  VecFreeCl<ENOVarStruct> u0=u;
  VecFreeCl<ENOVarStruct> u1=u0+TimeStp*ENOTimeShiftFlux(u,par);
//  VecFreeCl<ENOVarStruct> u2=0.5*(u0+u1+TimeStp*TimeShiftFlux(u1,par);
  u=0.5*(u0+u1+TimeStp*ENOTimeShiftFlux(u1,par));
 };

void ClcRungKurt()
 {
  VecFreeCl<ENOVarStruct> Vec(1,10);
  ENOVarStruct u;//u.u=1;//u.pos=10;
  for (int k=1;k<=10;k++) Vec[k]=u;
  Vec.SetSumMul(&ENO_TTSum,NULL,NULL);
//  Vec.SetFunc(&ENO_U,&U_ENO);
  Vec=Vec+Vec;
 };


//  =================================================================
//  ================      UIL    CLC    =============================
//  =================================================================

void ClcMassInBody(Body &WorkBody)
  {
   if (UilWorkDim==1)
     {for(int i=1;i<Num-1;i++)  M_[i]=Ro_[i]*(R_[i+1]-R_[i]);}
   else 
     {for(int i=1;i<Num-1;i++)  
        M_[i]=Ro_[i]*fabs(pow(R_[i+1],UilWorkDim)-pow(R_[i],UilWorkDim))/UilWorkDim;}

   M_[Num-1]=M_[Num-2];
ClcRungKurt();
  }; 

void CellFixed(Body &WorkBody)
  {
   int k;
   for (k=1;k<=Num;k++) F_[k]=1;
   F_[Num]=0;
   F_[2]=0;
   F_[1]=0;
   if (UilWorkDim==1)
    {
     for (k=2;k<Num;k++) 
       { if ((fabs(U_[k]-U_[k+1])>1e-4) || (fabs(P_[k+1]-P_[k])>11))
        { F_[k]=0;F_[k+1]=0;F_[k-1]=0;}}
    }
   else 
    {
     for (k=2;k<Num;k++) 
       { if ((fabs(U_[k])+fabs(U_[k+1])>1e-4) || (fabs(P_[k+1]-P_[k])>11))
        { F_[k]=0;F_[k+1]=0;F_[k-1]=0;}}
    }

   WorkBody.Pres();
  };


void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability)
  {
   if (Ro_[1]==0) return ;
   int k;
   double Coef,*Sound=new double[Num+3],*L=new double[Num+3],*tmp=new double[Num+3];
   double MinTimeCoef=0;
   WorkBody.Sound(Sound);
//  Next TimeStep Clc
   for (k=1;k<Num-1;k++) if (!F_[k]) 
    {
     tmp[k]=max<double>(fabs(Sound[k]),fabs(U_[k]-U_[k+1]));
     tmp[k]=max<double>(tmp[k],fabs(U_[k-1]-U_[k]));
     L[k]=min(R_[k]-R_[k-1],R_[k+1]-R_[k]);
     if ( (L[k]<MathZer) || (R_[k]<0) ){ 
      cout<<" Wrong cell size, negative.\n";}//exit(0);}
    }
   for (k=1;k<Num-1;k++) 
      if (!F_[k])
        { 
         Coef=fabs(tmp[k]/L[k]);
         if (Coef>MinTimeCoef ) MinTimeCoef=Coef;
        } 
   MinTimeStp=min(MinTimeStp,TimeStability/MinTimeCoef*1e-5);
//  Viscouse addition to pressure
   WorkBody.MullVisc(Sound);
   for (k=1;k<Num;k++) 
      if (!F_[k])
        { 
         double du=U_[k+1]-U_[k];
double r0=R_[Num-1]-R_[Num-2],rc=R_[k]-R_[k-1];
if ((k<2) || (k>Num-1)) rc=r0;
tmp[k]=Sound[k]*(1+WorkBody.Viscouse2*max<double>(1,fabs(du)));
         P_[k]-=Ro_[k]*du*tmp[k]*1e5;
        } 
   delete Sound;delete L;delete tmp;
  };

void ClcAddU(Body &WorkBody,double *AddU)
  {
   int k;
   if (UilWorkDim==1)
    {
     for (k=1;k<=Num;k++) 
        if ( (!F_[k]) && (M_[k]+M_[k-1]>0) ) AddU[k]=2*(P_[k]-P_[k-1])/(M_[k]+M_[k-1]);
        else  AddU[k]=0;
     WorkBody.BoundV(AddU[1],AddU[Num]);		  // AddU[1]*=2;AddU[Num]*=2;
    }

   else 
    {
     double R;
     for (k=1;k<=Num;k++) 
        if ( (!F_[k]) && (M_[k]+M_[k-1]>0) ) 
          {
           if (UilWorkDim==2) R=fabs(R_[k]+R_[k-1])*0.5;
           else R=sqr(R_[k]+R_[k-1])*0.25; // - not so, rough approximation
           AddU[k]=2*(P_[k]-P_[k-1]) *R/
                    (M_[k]+M_[k-1]);}
        else  AddU[k]=0;
     WorkBody.BoundV(AddU[1],AddU[Num]);
    }     

  }; 

void CalcPosE(Body &WorkBody,double TimeStp)
  {
//   WorkBody.BoundPV();						               //P_[0]=10;P_[Num]=10;
// Norm of velocity recalculation
   double *AddU=new double[Num+3];
   ClcAddU(WorkBody,AddU);
// velocity + pos  calculation
   double c=TimeStp*1e5;
   for (int k=1;k<=Num;k++) 
     {
      AddU[k]=TimeStp*AddU[k];
      R_[k]+=c*(U_[k]-AddU[k]*0.5);
      U_[k]-=AddU[k];
     }
//  density + energy calculation
   if (UilWorkDim==1)
    {
     for (k=1;k<Num;k++) 
       {
        if ((!F_[k]) && (M_[k]>0))
          {
           double Del=1/M_[k];
           double AddL=TimeStp*(U_[k+1]-U_[k]);
           E_[k]-=P_[k]*AddL*Del;
		         Ro_[k]=1/(1/Ro_[k]+AddL*1e5*Del);
          }    
       }
    }
   else 
    {
     for (k=1;k<Num;k++) 
       {
        if ((!F_[k]) && (M_[k]>0))
          {
           double Del=1/M_[k];
           double AddL=TimeStp*(U_[k+1]  *pow(R_[k+1],UilWorkDim-1)-
                                U_[k]  *pow(R_[k],UilWorkDim-1));
           E_[k]-=P_[k]*AddL*Del;
		         Ro_[k]=1/(1/Ro_[k]+AddL*1e5*Del);
          }    
       }
    }
   delete AddU;
};			









