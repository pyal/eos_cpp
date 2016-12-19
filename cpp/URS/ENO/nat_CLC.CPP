

#include "nat_clc.h"

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

//#define DebugPolynomOut
//#define FLUX_DEBUG
//#define FLUXSUM_DEBUG
/*                                                                     
double ClcPoli(double x,Poli3CoefStruct &Coef,int Order)
 {
  double res=Coef.a[0];
  for (int k=1;k<=Order;k++)
   {
    double tmp=Coef.a[k];
//    for (int o=0;o<k;o++) tmp*=(x-Pos[Coef.k[k-1]+o]);
    for (int o=0;o<k;o++) tmp*=(x-Coef.pos[Coef.k[k-1]+o]);
    res+=tmp;
   }
  return res;
 };
// in  DeriveVec [k] - k-1 derivative is stored
void ClcPoliDeriv(double x,Poli3CoefStruct &Coef,
                  int Order,VecCl &DeriveVec)
 {
  DeriveVec.Dim(Order+1);

  DeriveVec=DeriveVec*0;DeriveVec[1]=ClcPoli(x,Coef,Order);return;

  VecCl Z(Order+1);
  MatrCl L(Order+1);L(1,Order+1)=1;
  for (int k=1;k<=Order;k++)
   { Z[k]=x-Coef.pos[k-1];L(1,k)=1; }
  for (int l=2;l<=Order+1;l++)
   {
    L(l,l)=L(l-1,l-1)*Z[l-1];
    for (k=l+1;k<=Order+1;k++) L(l,k)=L(l,k-1)+L(l-1,k-1)*Z[k-1];
   }
  double FactK=1,sum;
  for ( k=1;k<=Order+1;k++)
   {
    sum=0;
    double Lfact=FactK;
    for (l=k;l<=Order+1;l++) {sum+=L(l-k+1,l)*Coef.a[l-1]/Lfact;Lfact*=l;}
    DeriveVec[k]=sum;
    FactK*=k;
   }
 };
void TestPoliOutput(Poli3CoefStruct &Coef,int Order)
 {
  VecCl DeriveVec;
  double x=Coef.pos[Coef.k[0]];
  cout<<" =======================  TestPoliOutput  ======================\n";
  ClcPoliDeriv(x,Coef,Order,DeriveVec); cout<<x<<" Deriv ";

  for (int k=1;k<=Order+1;k++) cout<<DeriveVec[k]<<" ";
  cout<<"   a  ";
  for (k=0;k<=Order;k++) cout<<Coef.a[k]<<" ";
  cout<<"   k  ";
  for (k=0;k<=Order;k++) cout<<Coef.k[k]<<" ";
  cout<<"   pos  ";
  for (k=0;k<=Order;k++) cout<<Coef.pos[k]<<" ";
  cout<<"\n";
  x=Coef.pos[0];double dx=(Coef.pos[Order]-x)/3;//10;x+=dx/2;
  x-=2*dx;
  for (k=1;k<=10;k++) 
    { ClcPoliDeriv(x,Coef,Order,DeriveVec); cout<<x<<"  "<<DeriveVec;x+=dx;  }
  cout<<" ExactVal \n";
  for (k=0;k<=Order;k++) 
    {x=Coef.pos[k];ClcPoliDeriv(x,Coef,Order,DeriveVec); cout<<x<<"  "<<DeriveVec;}
  cout<<" =======================  TestPoliOutput  ======================\n";
 };
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
   return (D2-D1)/(Pos[Order]-Pos[0]);
  }

void ClcPoliCoef(double *Pos,double *Par,Poli3CoefStruct &Coef,int Order)
 {
#ifdef DebugPolynomOut
  cout<<"ClcPoliCoef\n";
  for (int o=1;o<=2*Order+1;o++) cout<<Pos[o]<<" "<<Par[o]<<"\n";cout.flush();
#endif
  int CelNum=Order+1;
  int Kmin=CelNum;
  Coef.a[0]=Par[Kmin];Coef.k[0]=Kmin;
  for (int k=1;k<=Order;k++)
   {
    double a,b,c;
    if ( (fabs(a=DerivN(&Pos[Kmin],&Par[Kmin],k,c)))>
         (fabs(b=DerivN(&Pos[Kmin-1],&Par[Kmin-1],k,c))) )
      { Coef.a[k]=b; Kmin=Kmin-1;}
    else
       Coef.a[k]=a;
    Coef.k[k]=Kmin;
   }
  for (k=0;k<=Order;k++)
   {  Coef.pos[k]=Pos[Kmin+k]; Coef.k[k]-=Kmin; }


#ifdef DebugPolynomOut
  TestPoliOutput(Coef,Order);
#endif
 };






//#define ENO_U_Dim 3
//#define ENO_U_Dim 1
int ENOVarExtractDouble;
//struct ENOVarStruct {double u[ENO_U_Dim+1],pos;};

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
ENOVarStruct ENO_TTMul(ENOVarStruct &par,ENOVarStruct &par1)
 {ENOVarStruct ret=par;
  for (int k=1;k<=ENO_U_Dim;k++) ret.u[k]*=par1.u[k];
  return ret;}
ENOVarStruct ENO_DTMul(double x,ENOVarStruct &par)
 {ENOVarStruct ret=par;
  for (int k=1;k<=ENO_U_Dim;k++) ret.u[k]*=x;
  return ret;}

VecCl ENO_Vec(ENOVarStruct &par)
 {VecCl ret(ENO_U_Dim);for (int k=1;k<=ENO_U_Dim;k++) ret[k]=par.u[k];return ret;}
void Vec_ENO(ENOVarStruct &par,VecCl &set)
 {for (int k=1;k<=ENO_U_Dim;k++) par.u[k]=set[k];}

void ENOSetClcVar(){};
void ENOClcTimeStp() { };

// ENOVarStruct - struct
// u[1] - 1/Dens
// u[2] - u
// Flux
// f[1] - -u[2]
// f[2] - P(Dens,E)
//  P=Const*V^(-Gamma) P_d=Gamma*P*V
#define PV_const 1
#define PV_gamma 1.5
double PresSimple(double Volume,int Cell,ENOParStruct &par)
 {  return PV_const*pow(Volume,-PV_gamma); };
double PresSimple_derive(double Volume,int Cell,ENOParStruct &par)
 {  return PV_const*pow(Volume,-PV_gamma+1)*PV_gamma; };
VecFreeCl<ENOVarStruct> U_To_Flux(VecFreeCl<ENOVarStruct> &u,
                                  ENOParStruct &par)
 {
  int Xfr,Xto;u.GetIndex(Xfr,Xto);
  VecFreeCl<ENOVarStruct> ret=u;
  for (int k=Xfr;k<=Xto;k++) 
    { ret[k].u[1]=-u[k].u[2];ret[k].u[2]=PresSimple(u[k].u[1],k,par);}
  return ret;
 };

void DecomposeFlux(ENOVarStruct &u,ENOVarStruct &f,
                   MatrCl &Lft,MatrCl &Eig,MatrCl &Rgt,
                   int Cell,ENOParStruct &par)
 {
  Lft.Dim(2);Lft=Lft*0;Rgt=Lft;Eig=Lft;
  double alpha=PresSimple_derive(u.u[1],Cell,par);alpha=sqrt(alpha)/u.u[1];
  double norm=sqrt(1+sqr(alpha));
  Lft(1,1)=1;Lft(1,2)=-1;Lft(2,1)=alpha;Lft(2,2)=alpha;Lft=Lft*(0.5/alpha);
  Rgt(1,1)=alpha;Rgt(1,2)=1;Rgt(2,1)=-alpha;Rgt(2,2)=1;
  Eig(1,1)=alpha;Eig(2,2)=-alpha;
  Rgt=Rgt*(1/norm);Lft=Lft*norm;//MatrCl tmp=Lft;Lft=Rgt;Rgt=tmp;
  Eig=Eig*(-1);


 };
void PolyEstimate(double x,double dx,int Order,
              VecFreeCl<ENOVarStruct> &f,ENOVarStruct &res)
 {
  res.pos=x;
  Poli3CoefStruct Coef;//=new Poli3CoefStruct[2*Order+2];
  SetExtractK(0);
  VecCl pos=f.ExtractDouble(),DeriveVec(Order);
  VecCl TailorCoef(Order+1);TailorCoef=TailorCoef*0;
  TailorCoef[1]=1;if (Order>=3) TailorCoef[3]=-1./24*sqr(dx);;
  if (Order>=5)  TailorCoef[5]=7./5760*pow(dx,4);
  for (int k=1;k<=ENO_U_Dim;k++)
   { 
    SetExtractK(k);
    VecCl par=f.ExtractDouble();
    ClcPoliCoef(pos.Ptr,par.Ptr,Coef,Order);
    ClcPoliDeriv(x,Coef,Order,DeriveVec);
    res.u[k]=DeriveVec*TailorCoef;
   }
 }
int TestBound(VecFreeCl<ENOVarStruct> &u,int Cent,int Order)
 {
  int MinI,MaxI;u.GetIndex(MinI,MaxI);
  if ((Cent-Order<MinI) || (Cent+Order>MaxI)) return 1;
  else return 0;
 }
  
void TestFluxOut(VecFreeCl<ENOVarStruct> &u,
              VecFreeCl<ENOVarStruct> &f,
              MatrCl &Lft,MatrCl &Eig,MatrCl &Rgt,MatrCl &LambdaMax,
              MatrCl &LambdaMaxNew,int Order,int Cent)
 {
  if (TestBound(u,Cent,Order)) return;
  ENOVarStruct f_tst;
  VecFreeCl<ENOVarStruct> fk;fk=u;fk.SetIndex(-Order,Order);
  for ( int k=-Order;k<=Order;k++)
   {  
    fk[k]=u[k+Cent]; // Making x-coordinate ( position )
    Vec_ENO( fk[k],ENO_Vec(f[k+Cent]));
    Vec_ENO( fk[k],ENO_Vec(fk[k])*Lft );
   }
  for ( k=-Order;k<=Order;k++)
   {  
    cout<<Lft<<" Flux was "<<f[Cent+k]<<" Lft Mul  "<<fk[k];
    PolyEstimate(fk[k].pos, 0,Order,fk,f_tst);Vec_ENO( f_tst,Rgt*ENO_Vec(f_tst) );
    cout<<" Flux now "<<f_tst<<Rgt<<"\n";
   }
 };
void ConstructFlux(VecFreeCl<ENOVarStruct> &u,
              VecFreeCl<ENOVarStruct> &f,
              MatrCl &Lft,MatrCl &Eig,MatrCl &Rgt,MatrCl &LambdaMax,
              MatrCl &LambdaMaxNew,int Order,int Cent,
              ENOVarStruct  &fk0_p,
              ENOVarStruct &fk0_m)
 {
  if (TestBound(u,Cent,Order)) Order=0;
  VecFreeCl<ENOVarStruct> fk_p,fk_m;fk_p=u;fk_p.SetIndex(-Order,Order);
  fk_m=fk_p;
//cout<<" U "<<u<<" F "<<f;cout.flush();exit(0);
  for ( int k=-Order;k<=Order;k++)
   {  
    fk_p[k]=u[k+Cent];fk_m[k]=u[k+Cent]; // Making x-coordinate ( position )
    Vec_ENO( fk_p[k],ENO_Vec(f[k+Cent])+Lft*LambdaMax*Rgt*ENO_Vec(u[k+Cent]) );
    Vec_ENO( fk_m[k],ENO_Vec(f[k+Cent])-Lft*LambdaMax*Rgt*ENO_Vec(u[k+Cent]) );
    Vec_ENO( fk_p[k],ENO_Vec(fk_p[k])*Lft );
    Vec_ENO( fk_m[k],ENO_Vec(fk_m[k])*Lft );
   }
//cout<<" Fk_p "<<fk_p<<" fk_m "<<fk_m;cout.flush();//exit(0);
  double x_m,x_0=fk_p[0].pos,x_p;
  if (Order==0) 
    {int MinI,MaxI;u.GetIndex(MinI,MaxI);
     if (Cent-1<MinI) { x_p=u[1+Cent].pos;x_m=2*x_0-x_p;}
     else { x_m=u[-1+Cent].pos;x_p=2*x_0-x_m;} }
  else {x_m=fk_p[-1].pos;x_p=fk_p[1].pos;}
//  x_m=fk_p[-1].pos;x_p=fk_p[1].pos;
//cout<<" ConstructFlux , x_p.x_m "<<(x_0+x_p)*0.5<<" "<<(x_0+x_m)*0.5<<"\n";
  PolyEstimate((x_0+x_p)*0.5, x_p-x_0,Order,fk_p,fk0_p);
  PolyEstimate((x_0+x_m)*0.5, x_0-x_m,Order,fk_m,fk0_m);
//cout<<" ConstructFlux , Lam,x_p.x_m "<<LambdaMax<<fk0_p<<" "<<fk0_m<<"\n";
  Vec_ENO( fk0_p,ENO_Vec(fk0_p)*Rgt );Vec_ENO( fk0_m,ENO_Vec(fk0_m)*Rgt );
  for (k=1;k<=ENO_U_Dim;k++) 
   if (fabs(LambdaMaxNew(k,k))<fabs(Eig(k,k))) LambdaMaxNew(k,k)=Eig(k,k);
//cout<<" Eig "<<Eig<<" LambdaMaxNew "<<LambdaMaxNew;

// TestFluxOut(u,f,Lft,Eig,Rgt,LambdaMax,LambdaMaxNew, Order, Cent);
};

//VecCl TempLambda;
void SetFluxK(VecFreeCl<ENOVarStruct> &u,
              VecFreeCl<ENOVarStruct> &f,ENOParStruct &par,int Cent,
              ENOVarStruct &fk_p,ENOVarStruct &fk_m)
 {
  int Order=par.Order;
  VecCl LambdaMax=par.LambdaMax*1.1;
  MatrCl L_Max,L_New;L_Max=LambdaMax;L_New=par.LambdaNew;
  VecFreeCl<ENOVarStruct> fk,uk;

  MatrCl Lft,Rgt,Eig;
  DecomposeFlux(u[Cent],f[Cent],Lft,Eig,Rgt,Cent,par);  // TO DEFINE
  ConstructFlux(u,f,Lft,Eig,Rgt,L_Max,L_New,Order,Cent,fk_p,fk_m);
  if (TestBound(u,Cent,Order)) return ;
  LambdaMax=L_New;
  par.LambdaNew=LambdaMax;
// New CFL calculation
  double max_lambda=0,dx=min(u[Cent].pos-u[Cent-1].pos,u[Cent+1].pos-u[Cent].pos);
  for (int k=1;k<=ENO_U_Dim;k++) max_lambda=max<double>(max_lambda,fabs(Eig(k,k)));
  par.CFL_number=min(par.CFL_number,dx/max_lambda);
 };  
void ClcFluxDif(VecFreeCl<ENOVarStruct> &f_plus,VecFreeCl<ENOVarStruct> &f_minus,
                int NumPnt,VecFreeCl<ENOVarStruct> &f,VecFreeCl<ENOVarStruct> &u,int Order)
 {
  for (int k=0;k<=NumPnt;k++)
    f[k]=ENO_TTSum(f_minus[k+1],f_plus[k]);
#ifdef FLUXSUM_DEBUG
cout<<" F SUM ------------------------------------------------\n"<<0.5*f;cout.flush();
#endif
  f_minus=-1*f;
  for (k=1;k<=NumPnt;k++) 
   {   
#ifdef FLUXSUM_DEBUG
cout<<k<<" "<<1./(f_plus[k].pos-f_plus[k-1].pos)<<" "<<ENO_TTSum(f[k],f_minus[k-1]);
#endif
    f[k]=ENO_DTMul(1./(f_plus[k].pos-f_plus[k-1].pos),ENO_TTSum(f[k],f_minus[k-1]));
   }   
#ifdef FLUXSUM_DEBUG
cout<<" F SUM_MUL ------------------------------------------------\n"<<0.5*f;cout.flush();
#endif
  f=(-0.5)*f;
 };  
void ClcFluxBnd(VecFreeCl<ENOVarStruct> &f_plus,VecFreeCl<ENOVarStruct> &f_minus,
                int NumPnt,VecFreeCl<ENOVarStruct> &f,VecFreeCl<ENOVarStruct> &u,int Order,
                ENOParStruct &par)
 {
//TempLambda.Dim(1);TempLambda=TempLambda*0+(-1);
  par.LambdaMax=fabs(par.LambdaMax)*(-1);
  SetFluxK(u,f,par,1,f_plus[0],f_minus[1]);
  SetFluxK(u,f,par,NumPnt,f_plus[NumPnt],f_minus[NumPnt+1]);
  par.LambdaMax=fabs(par.LambdaMax);
  SetFluxK(u,f,par,1,f_plus[0],f_minus[0]);
  SetFluxK(u,f,par,NumPnt,f_plus[NumPnt+1],f_minus[NumPnt+1]);
  f_minus[NumPnt+1].u[1]=f_plus[NumPnt+1].u[1];
  f_plus[0].u[1]=f_minus[0].u[1];

  f_minus[NumPnt+1].pos=f_plus[NumPnt].pos;
  f_plus[0].pos=f_minus[1].pos;
 };  

VecFreeCl<ENOVarStruct> 
  ENOTimeShiftFlux(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par,int Inverse=0)
 {
//exit(0);
  int NumPnt=par.NumPnt;
  int MinI,MaxI;u.GetIndex(MinI,MaxI);
  VecFreeCl<ENOVarStruct> f_plus,f_minus,f;
  f=U_To_Flux(u,par);  // TO DEFINE !!!!!!!!!!!!!!!
//cout<<" F ----------------------------------------------------\n"<<f;cout.flush();
//exit(0);
  f_plus=f;f_minus=f; // Setting operators
//VecFreeCl<ENOVarStruct> f0=f;
//TempLambda.Dim(1);
  if (!Inverse) par.LambdaMax=fabs(par.LambdaMax);
  else   par.LambdaMax=fabs(par.LambdaMax)*(-1);
//par.LambdaMax=par.LambdaMax*(-1);
//  for (int k=0;k<=NumPnt+1;k++)
  for (int k=MinI;k<=MaxI;k++)
   {
    SetFluxK(u,f,par,k,f_plus[k],f_minus[k]);
//cout<<k<<"  "<<par.CFL_number<<"\n";
   }
#ifdef FLUX_DEBUG
cout<<" U ----------------------------------------------------\n"<<u;cout.flush();
cout<<" F PLUS -----------------------------------------------\n"<<f_plus;cout.flush();
cout<<" F MINUS ----------------------------------------------\n"<<f_minus;cout.flush();
#endif
//  ClcFluxBnd(f_plus,f_minus,NumPnt,f,u,par.Order,par);
  ClcFluxDif(f_plus,f_minus,NumPnt,f,u,par.Order);

#ifdef FLUX_DEBUG
cout<<" FLUX  ------------------------------------------------\n"<<f;cout.flush();
exit(0);
#endif
//cout<<" Used \n"<<par.LambdaMax<<" Set\n"<<par.LambdaNew;
  return f;
 };
    
void ClcRungKurt(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par,double TimeStp)
 {
  int Order=par.Order;
Order=2;
  VecFreeCl<ENOVarStruct> u0=u,u1;
//cout<<" U0 ----------------------------------------------------\n"<<u;cout.flush();
  if (Order<=2)
   {
    u1=u0+TimeStp*ENOTimeShiftFlux(u,par);
//cout<<" U1 ----------------------------------------------------\n"<<u1;cout.flush();
    u=0.5*(u0+u1+TimeStp*ENOTimeShiftFlux(u1,par));
   } 
  else
    if (Order==3)
     {
      VecFreeCl<ENOVarStruct> u2;
      u1=u0+TimeStp*ENOTimeShiftFlux(u0,par);
      u2=0.75*u0+0.25*(u1+TimeStp*ENOTimeShiftFlux(u1,par));
      u=(1./3)*(u0+2*(u2+TimeStp*ENOTimeShiftFlux(u2,par)));
     }
  else
    if (Order>=4)
     {
      VecFreeCl<ENOVarStruct> u2,u3,u0m,u1p;
      u0m=TimeStp*ENOTimeShiftFlux(u0,par,1);
      u1=u0+(0.5*TimeStp)*ENOTimeShiftFlux(u,par);
      u1p=TimeStp*ENOTimeShiftFlux(u1,par);
      u2=0.5*(u0+(-0.5)*u0m+u1+u1p);
      u3=(1./9)*(u0+(-1)*u0m+2*u1+(-3)*TimeStp*ENOTimeShiftFlux(u1,par,1)+
         6*u2+TimeStp*ENOTimeShiftFlux(u2,par));
      u=(1./6)*(2*u1+u1p+2*u2+2*u3+
                TimeStp*ENOTimeShiftFlux(u3,par));
     }

//cout<<" U ----------------------------------------------------\n"<<u;cout.flush();
 };


void ClcBody_ENO_BndInfinity(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par,Body &WorkBody,
                             int Inf=1)
  {
   int k,NumPnt=WorkBody.NumPnt,Order=par.Order;
   double x0=WorkBody.Mass[1],dx=WorkBody.Mass[2]-x0,v0=WorkBody.Velocity[1];
   if (Inf) v0=1e300;
   for (k=-Order;k<=0;k++) 
     {
      for (int o=1;o<=ENO_U_Dim;o++) u[k].u[o]=v0;
      u[k].pos =x0+dx*(k-1);
     }
   x0=WorkBody.Mass[NumPnt-1];dx=x0-WorkBody.Mass[NumPnt-2];v0=WorkBody.Velocity[NumPnt];
   if (Inf) v0=1e300;
   for (k=NumPnt+1;k<=NumPnt+Order+1;k++) 
     { 
      for (int o=1;o<=ENO_U_Dim;o++) u[k].u[o]=v0;
      u[k].pos =x0+dx*(k-NumPnt+1);
     }
  }
void ClcBody_ENO_BndPolynomApp(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par,Body &WorkBody)
  {
   int k,NumPnt=WorkBody.NumPnt,Order=par.Order;
   ClcBody_ENO_BndInfinity(u,par,WorkBody);
   VecFreeCl<ENOVarStruct> Bnd;Bnd=u;Bnd.SetIndex(-Order,Order);
   for (k=-Order;k<=Order;k++) Bnd[k]=u[k+1];//   ENOVarStruct ret; 
   for (k=1-Order;k<=0;k++) { PolyEstimate(u[k].pos,0,Order,Bnd,u[k]);}

   for (k=-Order;k<=Order;k++) Bnd[k]=u[NumPnt+k];//-1];//  -1 - because last pnt - wrong est 
   for (k=NumPnt-1;k<=NumPnt+Order;k++) { PolyEstimate(u[k].pos,0,Order,Bnd,u[k]);}
  }
void ClcBody_ENO_BndCircle(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par,Body &WorkBody)
  {
   int k,NumPnt=WorkBody.NumPnt,Order=par.Order;
   ClcBody_ENO_BndInfinity(u,par,WorkBody);
   for (k=-Order;k<=0;k++)  
     {
      for (int o=1;o<=ENO_U_Dim;o++) 
       {u[k].u[o]=u[NumPnt+k].u[o];u[NumPnt-k+1].u[o]=u[1-k].u[o];}
     }
  };
void ClcBody_ENO(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par,Body &WorkBody)
  {
   par.Order=WorkBody.Eno_dat.Order;
   int Order=par.Order;
   int NumPnt=WorkBody.NumPnt;
   u.SetIndex(-Order,NumPnt+Order+1);
   u.SetSumMul(&ENO_TTSum,&ENO_TTMul,&ENO_DTMul);
   u.SetDoubleCvt(&ENO_U,&U_ENO);
   SetExtractK(0);

   for (int k=1;k<=NumPnt;k++)
    {
     u[k].u[1]=1/WorkBody.Dencity[k];
     u[k].u[2]=WorkBody.Velocity[k];
     u[k].pos =WorkBody.Mass[k]; 
    }
//  Calculation of boundary
   ClcBody_ENO_BndCircle(u,par,WorkBody);

   par.CFL_number=1e100;
   par.LambdaMax=WorkBody.Eno_dat.LambdaMax;
//cout<<par.LambdaMax;cout.flush();
   par.LambdaNew=par.LambdaMax*0;
   par.NumPnt=Num;par.Order=WorkBody.Eno_dat.Order;
  };
void ClcENO_Body(VecFreeCl<ENOVarStruct> &u,ENOParStruct &par,Body &WorkBody,double TimeStp)
  {
   int N=WorkBody.NumPnt;
   double T2=TimeStp*0.5;
//cout<<" Tstp "<<T2*2<<"\n";
   for (int k=1;k<=N;k++)
    {
     WorkBody.Pos[k]+=T2*(WorkBody.Velocity[k]+u[k].u[2]);
     WorkBody.Pressure[k]=PresSimple(u[k].u[1],k,par)*1e5;
     WorkBody.Velocity[k]=u[k].u[2];
     WorkBody.Dencity[k]=1/u[k].u[1];
//     WorkBody.Pos[k]=u[k].pos;
    }
   WorkBody.Eno_dat.LambdaMax=par.LambdaNew;
  };
*/

//  =================================================================
//  ================      Reion Call Proc   =========================
//  =================================================================

void ClcMassInBody(Body &WorkBody,int Order)
  {
   if (UilWorkDim==1)
     {for(int i=1;i<Num-1;i++)  M_[i]=Ro_[i]*(R_[i+1]-R_[i]);}
   else 
     {for(int i=1;i<Num-1;i++)  
        M_[i]=Ro_[i]*fabs(pow(R_[i+1],UilWorkDim)-pow(R_[i],UilWorkDim))/UilWorkDim;}
   
   M_[Num-1]=M_[Num-2];
   M_[Num]=M_[Num-1];M_[0]=0;double s=0;
   for(int i=1;i<=Num;i++)  {s+=M_[i];M_[i]=s;}
   U_[0]=U_[1];U_[Num]=U_[Num-1];


//   par.CFL_number=1e100;
//   WorkBody.Eno_dat.LambdaMax.Dim(ENO_U_Dim);
//   WorkBody.Eno_dat.LambdaMax=WorkBody.Eno_dat.LambdaMax*0+5;
//   WorkBody.Eno_dat.Order=Order;
// SetLast Pnt - wrong aproximated ( Fst Body - NumPnt+1 - points ) setting for circle
   int NumPnt=WorkBody.NumPnt;
//   WorkBody.Velocity[NumPnt]=(WorkBody.Velocity[NumPnt-1]+WorkBody.Velocity[1])*0.5;
   WorkBody.Dencity[NumPnt]=WorkBody.Dencity[NumPnt-1];
//cout<<NumPnt<<"  "<<WorkBody.Velocity[NumPnt]<<"\n";
//   WorkBody.Velocity[NumPnt]=WorkBody.Velocity[1];
  }; 

void CellFixed(Body &WorkBody)
  {
   for (int k=1;k<=Num;k++) F_[k]=0;
  };

//struct NATParStruct {VecCl Pos,Dencity,NatL;int NumPnt;double CFL_number;};
  double Pos,Dencity,Length,PosP,DencityP,LengthP;
ostream& operator<<(ostream &output,ClcType &Pnt)
{output<<FMT<<Pnt.Pos<<FMT<<Pnt.Dencity<<FMT<<Pnt.Length<<"\n";return output;}

void ClcBody_Nat(Body &WorkBody)
{

  int N=WorkBody.NumPnt-1,k;
  VecFreeCl<ClcType> &Pnt=WorkBody.Nat_dat.Pnt;
  WorkBody.Nat_dat.NumPnt=N;
  double MulD=WorkBody.Viscouse2;
  WorkBody.Nat_dat.CFL_number=MulD;
  Pnt.SetIndex(0,N+1);
  for (k=1;k<=N;k++)
    {
     Pnt[k].Dencity=WorkBody.Dencity[k]*MulD;
     Pnt[k].Pos=WorkBody.Pos[k];
  }
//  Pnt[0].Dencity=2*Pnt[1].Dencity-Pnt[2].Dencity;
//  Pnt[N+1].Dencity=2*Pnt[N].Dencity-Pnt[N-1].Dencity;
  Pnt[0].Dencity=Pnt[N].Dencity;
  Pnt[N+1].Dencity=Pnt[1].Dencity;
  double dx=0.5*(Pnt[N].Pos-Pnt[N-1].Pos+Pnt[2].Pos-Pnt[1].Pos);
//  Pnt[0].Pos=Pnt[1].Pos+Pnt[N-1].Pos-Pnt[N].Pos;
//  Pnt[N+1].Pos=Pnt[N].Pos+Pnt[2].Pos-Pnt[1].Pos;
  Pnt[0].Pos=Pnt[1].Pos-dx;
  Pnt[N+1].Pos=Pnt[N].Pos+dx;
  for (k=0;k<=N;k++)
     Pnt[k].Length=sqrt( sqr(Pnt[k+1].Dencity-Pnt[k].Dencity)+
                         sqr(Pnt[k+1].Pos-Pnt[k].Pos) );
//     Pnt[k].Length=Pnt[k+1].Pos-Pnt[k].Pos;
//  Pnt[N+1].Length=Pnt[N].Length;
//cout<<"BegPnt ClcBody_Nat\n"<<Pnt<<"EndPnt ClcBody_Nat\n";

};
void ClcNat_Body(Body &WorkBody)
{
  int N=WorkBody.Nat_dat.NumPnt;
  VecFreeCl<ClcType> &Pnt=WorkBody.Nat_dat.Pnt;
  double MulD=WorkBody.Viscouse2;
  for (int k=0;k<=N+1;k++)
    {
     WorkBody.Dencity[k]=Pnt[k].Dencity/MulD;
     WorkBody.Pos[k]=Pnt[k].Pos;
     WorkBody.Pressure[k]=Pnt[k].Length*300*300*100;
     WorkBody.Velocity[k]=k/300.;
  }

};

void ClcNat_Tstp(NATParStruct &dat,double Tstp)
{
  int N=dat.NumPnt;
  VecFreeCl<ClcType> &Pnt=dat.Pnt;
  double Tstp2=Tstp*0.5,dRdL,dRdL2,R,X,dR,dX,L,dFdL,dFdL2,dX1,dR1,dRp,dXp,dFdX1,dFdX2;
//cout<<"BegPnt ClcNat_Tstp\n"<<Pnt<<"EndPnt ClcNat_Tstp\n";
  for (int k=0;k<=N;k++) 
    {
     dRdL=(Pnt[k+1].Dencity-Pnt[k].Dencity)/Pnt[k].Length;dRdL2=sqr(dRdL);
     R=(Pnt[k].Dencity+Pnt[k+1].Dencity)/2;
     X=(Pnt[k].Pos+Pnt[k+1].Pos)/2;
     dX=R*Tstp*dRdL2;dR=-dX*sqrt(1/dRdL2-1);
     Pnt[k].DencityP=R+dR;Pnt[k].PosP=X+dX;

     dRp=Pnt[k+1].Dencity-Pnt[k].Dencity-
         dX/R*(Pnt[k+1].Dencity-Pnt[k].Dencity)*sqrt(1/dRdL2-1);
     dXp=Pnt[k+1].Pos-Pnt[k].Pos+dX/R*(Pnt[k+1].Dencity-Pnt[k].Dencity);
     Pnt[k].DerivP=dRp/sqrt(sqr(dRp)+sqr(dXp));
    }
  for (k=0;k<=N;k++) 
    {
     dFdX2=1/sqrt(1/sqr(Pnt[k+1].DerivP)-1);
     dFdX1=1/sqrt(1/sqr(Pnt[k].DerivP)-1);
     X=Pnt[k].PosP+(Pnt[k+1].PosP-Pnt[k].PosP)/(Pnt[k+1].DencityP-Pnt[k].DencityP)*
                   (dFdX2-1);
     R=Pnt[k].PosP+dFdX1*(X-Pnt[k].PosP);
     if (!In_Lim(X,Pnt[k].PosP,Pnt[k+1].PosP,0))
      {
cout<<k<<" X1 "<<Pnt[k].PosP<<" X2 "<<Pnt[k+1].PosP<<" X "<<X
       <<" F1 "<<Pnt[k].DencityP<<" F2 "<<Pnt[k+1].DencityP
       <<" F1' "<<dFdX1<<" F2' "<<dFdX2<<"\n";
       X=(Pnt[k+1].PosP+Pnt[k].PosP)*0.5;
       R=(Pnt[k+1].DencityP+Pnt[k].DencityP)*0.5;
//       if (In_Lim(Pnt[k].PosP,X,Pnt[k+1].PosP))
//       {}
//       else
//       {}
      }
//     else
      {
       Pnt[k+1].Pos=X;
       Pnt[k+1].Dencity=R;
      }
    }
//cout<<"BegPnt ClcNat_Tstp\n"<<Pnt<<"EndPnt ClcNat_Tstp\n";
};
void CtrlNat(NATParStruct &dat,double TimeStp,double &MintimeStp,double TimeStability)
{
  int N=dat.NumPnt;
  VecFreeCl<ClcType> &Pnt=dat.Pnt;
//cout<<"BegPnt CtrlNat\n"<<Pnt;

  for (int k=0;k<=N;k++) 
    MintimeStp=min(MintimeStp,fabs(Pnt[k].Length/Pnt[k].Dencity*TimeStability));

};
void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability)
  {
//   if (Ro_[1]==0) return ;
//cout<<"BegPnt ClcNat_Tstp\n";
   ClcBody_Nat(WorkBody);
   CtrlNat(WorkBody.Nat_dat,TimeStp,MinTimeStp,TimeStability);
   ClcNat_Tstp(WorkBody.Nat_dat,TimeStp);
   ClcNat_Body(WorkBody);


  };

void CalcPosE(Body &WorkBody,double TimeStp)
  {
};			
int In_Lim(double Val,double from,double to,int BndInclude)
  {
   if ( from>to) ExchVar(&from,&to,sizeof(to));
   if ( (Val>to) || (Val<from) ) return 0;
   if ( (!BndInclude) && ((Val==from) || (Val==to)) ) return 0;
   return 1;
  };
/*
void ClcNat_Tstp(NATParStruct &dat,double Tstp)
{
  int N=dat.NumPnt;
  VecFreeCl<ClcType> &Pnt=dat.Pnt;
  double Tstp2=Tstp*0.5,dRdL,dRdL2,R,X,dR,dX,L,dFdL,dFdL2;
//cout<<"BegPnt ClcNat_Tstp\n"<<Pnt<<"EndPnt ClcNat_Tstp\n";
  for (int k=0;k<=N;k++) 
    {
     dRdL=(Pnt[k+1].Dencity-Pnt[k].Dencity)/Pnt[k].Length;dRdL2=sqr(dRdL);
     dFdL=(sqr(Pnt[k+1].Dencity)-sqr(Pnt[k].Dencity))*0.5/Pnt[k].Length;dFdL2=sqr(dFdL);
     R=(Pnt[k+1].Dencity+Pnt[k].Dencity)/2;
     X=(Pnt[k+1].Pos+Pnt[k].Pos)/2;
//     dX=R*Tstp2*dRdL2;
//     dR=-dX*sqrt(1/dRdL2-1);

     dX=Tstp2*dRdL*dFdL;
     dR=-dX*sqrt(1/dRdL2-1);

//     if (fabs(1-dRdL2)>0.5) { dX=0; dR=-Tstp2*dFdL/sqrt(1-dRdL2); }
     Pnt[k].DencityP=R+dR;
     Pnt[k].PosP=X+dX;
    }
  for (k=0;k<=N-1;k++) 
    {
     L=sqrt(sqr(Pnt[k+1].DencityP-Pnt[k].DencityP)+sqr(Pnt[k+1].PosP-Pnt[k].PosP) );
//     L=Pnt[k+1].PosP-Pnt[k].PosP;
     dRdL=(Pnt[k+1].DencityP-Pnt[k].DencityP)/L;dRdL2=sqr(dRdL);
     dFdL=(sqr(Pnt[k+1].Dencity)-sqr(Pnt[k].Dencity))*0.5/L;dFdL2=sqr(dFdL);
     R=Pnt[k+1].Dencity;
     X=Pnt[k+1].Pos;
//     dX=R*Tstp*dRdL2;
//     dR=-dX*sqrt(1/dRdL2-1);

     dX=Tstp*dRdL*dFdL;
     dR=-dX*sqrt(1/dRdL2-1);
//     if (fabs(1-dRdL2)>0.5) { dX=0;dR=-Tstp*dFdL/sqrt(1-dRdL2); }
     Pnt[k+1].Dencity=R+dR;
     Pnt[k+1].Pos=X+dX;
//cout<<FMT<<dX<<FMT<<dR<<"\n";
    }
//cout<<"BegPnt ClcNat_Tstp\n"<<Pnt<<"EndPnt ClcNat_Tstp\n";
};
*/







