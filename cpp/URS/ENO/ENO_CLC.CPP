

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

//#define DebugPolynomOut
//#define FLUX_DEBUG
//#define FLUXSUM_DEBUG
                                                                     
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
/*
double ClcMul(int MulN,int DeriveN,int *pos)
 {
  int k,Add=0;
  double res=0;
  if (DeriveN==0) 
    { 
     Mul=1;
     for (k=1;k<=MulN;k++) 
       {
        while (pos[k+Add]==-1) Add++;
        Mul*=(x-Coef.pos[pos[k+Add]]);
       }
     return Mul;
    }
  int *Npos=new int[MulN+DeriveN+1];
  for (k=1;k<=MulN;k++) res+=ClcMul(MulN,DeriveN-1);
  delete Npos;
  return res;
 }
   
double ClcPoliDerive(double x,Poli3CoefStruct &Coef,int Order,int Derive)
 {
  double res=Coef.a[Derive];
  for (int k=Derive+1;k<=Order;k++)
   {
    double tmp=Coef.a[k];
    for (int o=0;o<k;o++) tmp*=(x-Coef.pos[Coef.k[k-1]+o]);
    for (int o=0;o<k-Derive;o++) tmp*=(x-Coef.pos[Coef.k[k-1]+o]);
    res+=tmp;
   }
  return res;
 };
*/
// in  DeriveVec [k] - k-1 derivative is stored
void ClcPoliDeriv(double x,Poli3CoefStruct &Coef,
                  int Order,VecCl &DeriveVec)
 {
  DeriveVec.Dim(Order+1);

  DeriveVec=DeriveVec*0;DeriveVec[1]=ClcPoli(x,Coef,Order);return;
//  if (Order==3) 
//    { double d3=Coef.a[2];
//      for (int o=0;o<3;o++) d3+=Coef.a[3]*(x-Coef.pos[Coef.k[2]+o]);
//      DeriveVec[3]=d3;
//    }
//  return ;

  VecCl Z(Order+1);
  MatrCl L(Order+1);L(1,Order+1)=1;
  for (int k=1;k<=Order;k++)
//   { Z[k]=x-Coef.pos[k];L(1,k)=1; }
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
//    for (l=k+1;l<=Order+1;l++) sum+=L(l-k,l)*Coef.a[l];
    double Lfact=FactK;
    for (l=k;l<=Order+1;l++) {sum+=L(l-k+1,l)*Coef.a[l-1]/Lfact;Lfact*=l;}
//    for (l=k;l<=Order+1;l++) {sum+=L(l-k+1,l)*Coef.a[l-1]/Lfact;Lfact*=l;}

//    for (l=k;l<=Order+1;l++) sum+=L(l-k+1,l)*Coef.a[l-1];
    DeriveVec[k]=sum;
//    DeriveVec[k]=FactK*sum;
    FactK*=k;
   }
//  DeriveVec[1]=ClcPoli(x,Coef,Order);  // Setting work procedure!!!!
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
//  x=Coef.pos[0];double dx=(Coef.pos[Order]-x)/10;x+=dx/2;
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
//   Cent=(C1+C2)*0.5;                                 
//   return (D2-D1)/(C2-C1);
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

//  int Fact=1;for (k=2;k<=Order;k++)  {Fact*=k;Coef.a[k]/=Fact;}
/*
  Coef.a[0]=Par[Kmin];Coef.k[0]=0;
  for (k=1;k<=Order;k++)
   {
    double c;
    Coef.a[k]=DerivN(&Pos[Kmin],&Par[Kmin],k,c);
    Coef.k[k]=0;
   }
*/

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
//struct ENOGridStruct {double pos;};
//struct ENOParStruct 
// {VecCl LambdaMax,LambdaNew;int Order,NumPnt;double CFL_number;};

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
//    { ret[k].u[1]=u[k].u[2];ret[k].u[2]=-PresSimple(u[k].u[1],k,par);}
//    { ret[k].u[1]=u[k].u[1];ret[k].u[2]=u[k].u[2];}
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

//MatrCl cmp=Lft*0;cmp(1,2)=1;cmp(2,1)=sqr(alpha);cout<<Lft*Rgt<<Lft*Eig*Rgt<<cmp;
//  Lft(1,1)=1;Lft(2,2)=1;
//  Rgt(1,1)=1;Rgt(2,2)=1;
//  Eig(1,1)=1;Eig(2,2)=1;

 };
void PolyEstimate(double x,double dx,int Order,
              VecFreeCl<ENOVarStruct> &f,ENOVarStruct &res)
 {
  res.pos=x;
  Poli3CoefStruct Coef;//=new Poli3CoefStruct[2*Order+2];
  SetExtractK(0);
  VecCl pos=f.ExtractDouble(),DeriveVec(Order);
//cout<<" PolyEstimate InFlux "<<f;cout.flush();
//cout<<pos;cout.flush();
  VecCl TailorCoef(Order+1);TailorCoef=TailorCoef*0;
  TailorCoef[1]=1;if (Order>=3) TailorCoef[3]=-1./24*sqr(dx);;
  if (Order>=5)  TailorCoef[5]=7./5760*pow(dx,4);
  for (int k=1;k<=ENO_U_Dim;k++)
   { 
    SetExtractK(k);
    VecCl par=f.ExtractDouble();
//cout<<" InPar "<<par;cout.flush();
//cout<<" XPos  "<<pos;cout.flush();
    ClcPoliCoef(pos.Ptr,par.Ptr,Coef,Order);
    ClcPoliDeriv(x,Coef,Order,DeriveVec);
//cout<< "x "<<x<<" REsDeriv "<<DeriveVec;cout.flush();
//cout<<" TailorCoef "<<TailorCoef;cout.flush();
    res.u[k]=DeriveVec*TailorCoef;
   }
//  delete Coef;
//cout<<" InFlux "<<f;
//cout<<" PolyEstimate : "<<res;
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
//    Vec_ENO( fk[k],Lft*ENO_Vec(fk[k]) );
    Vec_ENO( fk[k],ENO_Vec(fk[k])*Lft );
   }
//  double x_m,x_0=fk_p[0].pos,x_p;
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
//    Vec_ENO( fk_p[k],ENO_Vec(f[k+Cent])+Rgt*LambdaMax*Lft*ENO_Vec(u[k+Cent]) );
//    Vec_ENO( fk_m[k],ENO_Vec(f[k+Cent])-Rgt*LambdaMax*Lft*ENO_Vec(u[k+Cent]) );
//    Vec_ENO( fk_p[k],Lft*ENO_Vec(fk_p[k]) );
//    Vec_ENO( fk_m[k],Lft*ENO_Vec(fk_m[k]) );
    Vec_ENO( fk_p[k],ENO_Vec(fk_p[k])*Lft );
    Vec_ENO( fk_m[k],ENO_Vec(fk_m[k])*Lft );
//cout<<fk_p[k];cout.flush();
//fk_p[k]=f[k+Cent];
//fk_m[k]=f[k+Cent];
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
//  Vec_ENO( fk0_p,Rgt*ENO_Vec(fk0_p) );Vec_ENO( fk0_m,Rgt*ENO_Vec(fk0_m) );
//  for (k=1;k<=ENO_U_Dim;k++) LambdaMaxNew(k,k)=max<double>(LambdaMaxNew(k,k),fabs(Eig(k,k)));
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
//L_Max=TempLambda;
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
//    f[k]=f_plus[k];
//f=f_plus;
//  for (k=NumPnt;k<=NumPnt+Order;k++) f[k].u[1]=0;
//  for (k=-Order;k<=0;k++) f[k].u[1]=0;
#ifdef FLUXSUM_DEBUG
cout<<" F SUM ------------------------------------------------\n"<<0.5*f;cout.flush();
#endif
  f_minus=-1*f;
//  f_plus=f;

  for (k=1;k<=NumPnt;k++) 
   {   
#ifdef FLUXSUM_DEBUG
cout<<k<<" "<<1./(f_plus[k].pos-f_plus[k-1].pos)<<" "<<ENO_TTSum(f[k],f_minus[k-1]);
#endif
    f[k]=ENO_DTMul(1./(f_plus[k].pos-f_plus[k-1].pos),ENO_TTSum(f[k],f_minus[k-1]));
//    f[k]=ENO_TTSum(f[k],f_minus[k-1]);
//    f[k].u[1]=(f_plus[k+1].u[1]+f_minus[k].u[1])/(u[k].u[1]-u[k-1].u[1]);
//    f[k]=ENO_DTMul(1./(u[k+1].u[1]-u[k].u[1]),ENO_TTSum(f_plus[k],f_minus[k-1]));
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
   WorkBody.Eno_dat.LambdaMax.Dim(ENO_U_Dim);
   WorkBody.Eno_dat.LambdaMax=WorkBody.Eno_dat.LambdaMax*0+5;
   WorkBody.Eno_dat.Order=Order;
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
/*
   for (int k=1;k<=Num;k++) F_[k]=1;
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
*/
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
void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability)
  {
   if (Ro_[1]==0) return ;
   VecFreeCl<ENOVarStruct> u;
   ENOParStruct par;
   ClcBody_ENO(u,par,WorkBody);
//cout<<" U Beg ================================================================\n"<<u;
   ClcRungKurt(u,par,TimeStp);
   ClcENO_Body(u,par,WorkBody,TimeStp);
   MinTimeStp=par.CFL_number*TimeStability;
//cout<<TimeStp<<"  "<<MinTimeStp<<"\n";


 /*
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
*/
  };

void CalcPosE(Body &WorkBody,double TimeStp)
  {
//   WorkBody.BoundPV();						               //P_[0]=10;P_[Num]=10;
// Norm of velocity recalculation
//   double *AddU=new double[Num+3];
//   ClcAddU(WorkBody,AddU);
// velocity + pos  calculation
//   double c=TimeStp*1e5;
//   for (int k=1;k<=Num;k++) 
//     {
//      AddU[k]=TimeStp*AddU[k];
//      R_[k]+=c*(U_[k]-AddU[k]*0.5);
//      U_[k]-=AddU[k];
//     }
//  density + energy calculation
//   delete AddU;
};			

void Test()
 {
  VecFreeCl<ENOVarStruct> Vec(1,10);
  ENOVarStruct u;//u.u=1;//u.pos=10;
//  for (int k=1;k<=10;k++) Vec[k]=u;
  Vec.SetSumMul(&ENO_TTSum,NULL,NULL);
  Vec.SetDoubleCvt(&ENO_U,&U_ENO);
  Vec=Vec+Vec;
 };

//  =================================================================
//  ================      UIL    CLC    =============================
//  =================================================================
/*
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

*/







