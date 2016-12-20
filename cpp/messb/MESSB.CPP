#include <lib/precompiled/Messb.h>
#include "messb.h"
//#include <malloc.h>
//
const double HamData::mms_tesl=84.5;
const double HamData::mu_e=2.2363;       //This is old value
const double HamData::mu_g=-3.9156;       //This is old value
//const double HamData::MCmu_e=2.2843;       // Value got from Phys Encyclop relation
//const double HamData::MCmu_g=-3.8333045254;  // Value got from Phys Encyclop relation
   //   to use them - have to vary MCmms_tesl const
//const double HamData::MCmu_e=-3.9156;     //This set is done according to Phys Encyclop
//const double HamData::MCmu_g=2.2843;



CMatrCl HamData::Make2Ham()
{
  my_comp shi=my_comp(mu_g*H/(2*fabs(mu_g))/mms_tesl,0);
  CMatrCl ret(2);ret(1,1)=shi;ret(2,2)=-shi;
  ret(2,1)=ret(1,2)=0;
  return ret*(-1);
}

CMatrCl HamData::Make8Ham()
{
  my_comp sh=my_comp(Is,-Wid/2);
  CMatrCl H2,Zer(4),H4,Gr;
  H4=Make4Ham()+sh;
  Zer=Zer*0;
  H2=Make2Ham();Gr=CombineMatr::ExtendMat(H2,4);
  return CombineMatr::Sum2Mat(H4,Zer,Zer,H4)+Gr;
};

CMatrCl Ham_Qs_Eiler::Make4Ham()
{
  CMatrCl Mat(4);
  double h,teta,phi;
  FormMessbInput(h,teta,phi);
  double mx=Qs/(3*sqrt(1+sqr(Eta)/3));
  double mb=-h*fabs(1/mu_g)*mu_e;
  double mc=mx*(1-sqr(sin(teta))*(3-Eta*cos(2*phi))/2);
  double md1=mx*sin(teta)*cos(teta)*(3-Eta*cos(2*phi))/sqrt(3.);
  double md2=mx*sin(teta)*Eta*sin(2*phi)/sqrt(3.);
  my_comp md=my_comp(md1,md2);
  double me1=mx*(3+Eta*cos(2*phi)-sqr(cos(teta))*(3-Eta*cos(2*phi)))/(2*sqrt(3.));
  double me2=-mx*Eta*cos(teta)*sin(2*phi)/sqrt(3.);
  my_comp me=my_comp(me1,me2);

  Mat(1,1)=-mb+mc;         Mat(1,2)=md;
  Mat(2,1)=conj(md);       Mat(2,2)=-mb/3-mc;
  Mat(3,1)=conj(me);       Mat(3,2)=0;
  Mat(4,1)=0;              Mat(4,2)=conj(me);

  Mat(1,3)=me;             Mat(1,4)=0;
  Mat(2,3)=0;              Mat(2,4)=me;
  Mat(3,3)=mb/3-mc;        Mat(3,4)=-md;
  Mat(4,3)=-conj(md);      Mat(4,4)=mb+mc;
  Mat=Mat*1.5;
  return Mat;
};
CMatrCl Ham_Qs_Gamma::Make4Ham()
{
  CMatrCl Mat,MatH,MatQs,Turn=TurnSpinors::Turn4Mat(Teta,Phi),ITurn=TurnSpinors::Turn4Mat(0,Gamma);
  Turn=ITurn*Turn;
  ITurn=Conj(Transpon(Turn));
  //CMatrCl Mat,MatH,MatQs,Turn,ITurn,mTeta,mPhi,mGamma;
  //mTeta=TurnSpinors::Turn4Mat(Teta,0);
  //mGamma=TurnSpinors::Turn4Mat(0,Gamma);
  //mPhi=TurnSpinors::Turn4Mat(0,Phi);
  //ITurn=mPhi*mTeta*mGamma;
  //Turn=TurnSpinors::InverseOper(ITurn);
  MatH=Ham_Qs_Eiler(0,0,H,0,0,0,0).Make4Ham();
  MatQs=Ham_Qs_Eiler(0,0,0,Qs,Eta,0,0).Make4Ham();
  Mat=MatH+ITurn*MatQs*Turn;
//HQsGamma_Mat(Mat,Teta,Phi,H,Qs,Eta,Gamma);
  return Mat;
}


CMatrCl Ham_Z_Turned_Qs_Gamma::Make4Ham()
{
  Ham_Qs_Gamma HamQs(*this);HamQs.H=0;
  CMatrCl MatQs=HamQs.Make4Ham();
  CMatrCl Turn=TurnSpinors::Turn4Mat(H_teta,H_phi),ITurn=Conj(Transpon(Turn));
  CMatrCl MatH=Ham_Qs_Eiler(0,0,H,0,0,0,0).Make4Ham();
  return MatH+Turn*MatQs*ITurn;
}

#ifndef Old_HamHTurned_Z_Qs


CMatrCl HamHTurned_Z_Qs::Make4Ham()
{
  CMatrCl Mat(4);
  double h,teta,phi;
  FormMessbInput(h,teta,phi);
  teta=H_teta*M_PI/180;phi=H_phi*M_PI/180;
  double mb=Qs/sqrt(1+sqr(Eta)/3)*0.5;
  double  ca31=Eta*mb/sqrt(3.);

  Mat(1,1)=mb;               Mat(1,2)=0;
  Mat(2,1)=0;                Mat(2,2)=-mb;
  Mat(3,1)=ca31;             Mat(3,2)=0;
  Mat(4,1)=0;                Mat(4,2)=ca31;

  Mat(1,3)=ca31;             Mat(1,4)=0;
  Mat(2,3)=0;                Mat(2,4)=ca31;
  Mat(3,3)=-mb;              Mat(3,4)=0;
  Mat(4,3)=0;                Mat(4,4)=mb;
  CMatrCl H_Mat=TurnSpinors::FormH4Ham(h,H_teta,H_phi,mu_g,mu_e);
  return Mat+H_Mat;
};

CMatrCl HamHTurned_Z_Qs::Make2Ham()
{
  double h,teta,phi;
  FormMessbInput(h,teta,phi);
  return TurnSpinors::FormH2Ham(h,H_teta,H_phi,mu_g,mu_e);
};


#else


  CMatrCl HamHTurned_Z_Qs::Make4Ham()
  {
    CMatrCl Mat(4);
    double h,teta,phi;
    FormMessbInput(h,teta,phi);
    teta=H_teta*M_PI/180;phi=H_phi*M_PI/180;
    double ma=h*fabs(1/mu_g)*mu_e;
    double mb=Qs/sqrt(1+sqr(Eta)/3)*0.5;
    double caii=cos(teta)/2*ma;
    my_comp mul=sin(teta)*my_comp(cos(phi),sin(phi));
    my_comp ca21=-sqrt(3.)*0.5*mul*ma;
    my_comp ca32=-mul*ma;
    double  ca31=Eta*mb/sqrt(3.);

    Mat(1,1)=mb-3*caii;        Mat(1,2)=conj(ca21);
    Mat(2,1)=ca21;             Mat(2,2)=-mb-caii;
    Mat(3,1)=ca31;             Mat(3,2)=ca32;
    Mat(4,1)=0;                Mat(4,2)=ca31;

    Mat(1,3)=ca31;             Mat(1,4)=0;
    Mat(2,3)=conj(ca32);       Mat(2,4)=ca31;
    Mat(3,3)=-mb+caii;         Mat(3,4)=conj(ca21);
    Mat(4,3)=ca21;             Mat(4,4)=mb+3*caii;

    return Mat;
  };

  CMatrCl HamHTurned_Z_Qs::Make2Ham()
  {
    double h,teta,phi;
    FormMessbInput(h,teta,phi);
    teta=H_teta*M_PI/180;phi=H_phi*M_PI/180;

    double ma=h*fabs(1/mu_g)*mu_g;
    my_comp mul=sin(teta)*my_comp(cos(phi),sin(phi)),ca21=-ma/ /*sqrt*/(2)*conj(mul);
    double caii=-ma*cos(teta)*0.5;
    CMatrCl Gr(2);Gr(1,1)=caii;Gr(1,2)=conj(ca21);Gr(2,1)=ca21;Gr(2,2)=-caii;
    return Gr*(-1);
  };
#endif


  CMatrCl HamHTurned_Z_Free_Qs_Eiler::Make8Ham()
  {
    CMatrCl MatH=HamHTurned_Z_Qs(HamData(0,0,H,0,0,Is,Wid),H_teta,H_phi).Make8Ham();
    CMatrCl MatQs=Ham_Qs_Eiler(HamData(Teta,Phi,0,Qs,Eta,0,0)).Make8Ham();
    return MatH+MatQs;

  }
  CMatrCl HamHTurned_Z_Free_Qs_Gamma::Make8Ham()
  {
    CMatrCl MatH=HamHTurned_Z_Qs(HamData(0,0,H,0,0,Is,Wid),H_teta,H_phi).Make8Ham();
    CMatrCl MatQs=Ham_Qs_Gamma(HamData(Teta,Phi,0,Qs,Eta,0,0),Gamma).Make8Ham();
    return MatH+MatQs;
  }


void Solver_H_Z_poly::Solve(double *x,double *Res)
{
  VecCl Int,Pos;
  SolveIntPos(Ham->Make4Ham(),Int,Pos);
  Intencity(Int,Pos,x,Res);
}
void Solver_H_Z_poly::SolveIntPos(CMatrCl &Hamilt,VecCl &Int,VecCl &Pos)
{
  int N=Hamilt.Dim();
  CMatrCl EigenVect(N),EigenVect1(N);
  VecCl EigenVal(N);
  URComeig(Hamilt,EigenVect1,EigenVect);          
  for (int k=1;k<=N;k++) EigenVal[k]=my_real(Hamilt(k,k));
  EigenVect=Transpon(EigenVect);
  IntPos(EigenVect,EigenVal,Int,Pos);
};
void Solver_H_Z_poly::IntPos(CMatrCl &EigenVect,VecCl &EigenVal,VecCl &Int,VecCl &Pos)
{
  double H=Ham->H,IS=Ham->Is;
  double shi=HamData::mu_g*H/(2*fabs(HamData::mu_g))/HamData::mms_tesl;
  VecCl Amp2=MakeAmpVec();
  Pos.SetDim(8);Int.SetDim(8);Int=Int*0;
  for (int k=1;k<=4;k++)
    { Pos[k]=EigenVal[k]-shi+IS;Pos[k+4]=EigenVal[k]+shi+IS;  }
  for (int k=1;k<=4;k++)
    {
    for (int k1=1;k1<=4;k1++) 
      {
        double tmp=norm(EigenVect(k1,k));
        Int[k]+=  Amp2[k1]*tmp;
        Int[k+4]+=Amp2[5-k1]*tmp;
      }
    }
};
void Solver_H_Z_poly::Intencity(VecCl &Int,VecCl &Pos,double *x,double *res)
{
  double W=sqr(Ham->Wid/2),K=Ham->Wid/(2*M_PI);
  for (int k=1;k<=x[0];k++)
    {
    double p=x[k],s=0;
    for (int k1=1;k1<=Int.Dim();k1++) s+=Int[k1]/(W+sqr(Pos[k1]-p));
    res[k]=s*K;
    }
};

void Solver_H_Z_poly_kashuba::Intencity(VecCl &Int,VecCl &Pos,double *x,double *res)
{
  double ZerPos=(Pos*(Pos*0+1))/8;
  double s,p,W=sqr(Ham->Wid/2);
  double K=Ham->Wid/(2*M_PI);
  for (int k=1;k<=x[0];k++)
  {
    p=x[k];s=0;
    for (int k1=1;k1<=Pos.Dim();k1++) //s+=Intencity[k1]/(W+sqr(Position[k1]-p));
    {
	   double Sw=p-Pos[k1];
	   double Mul=1;if (ZerPos-Pos[k1]>0) Mul=-1;
	   double Wc=Ham->Wid*0.5+W_coef*fabs(ZerPos-Pos[k1])*atan(M_PI*T/(2*(8*Tbkt-T)));
	   s+=Int[k1]*(1-Sw*A*Mul/Ham->Wid)/(sqr(Wc)+sqr(Sw));
	  }
    res[k]=s*K;
  }
};



VecCl Solver_H_Z_mono::MakeAmpVec()
{  
  VecCl ret(4);
  double Cos=cos(TetaH_View*M_PI/180),Sin=sin(TetaH_View*M_PI/180);
  double Cos2=sqr(Cos);
  ret[1]=(1+Cos2)*0.75;ret[2]=1.-Cos2;ret[3]=(1+Cos2)*0.25;ret[4]=0;
  return ret; 
}
void Solver_H_Z_mono::IntPos(CMatrCl &EigenVect,VecCl &EigenVal,VecCl &Int,VecCl &Pos)
{
  Solver_H_Z_poly::IntPos(EigenVect,EigenVal,Int,Pos);
  double Cos=cos(TetaH_View*M_PI/180),Sin=sin(TetaH_View*M_PI/180);
  double Cos2=sqr(Cos);

//  Add interference 
  my_comp e1=my_comp(cos(PhiH_View*M_PI/180),sin(PhiH_View*M_PI/180)),e2=e1*e1;
  my_comp C1=sqrt(3.)*Sin*Cos,C2=0.5*sqrt(3.)*sqr(Sin),C3=C1/sqrt(3.);
  for (int k=1;k<=4;k++)
    {
    Int[k]+=my_real(   C1*e1*conj(EigenVect(1,k))*EigenVect(2,k)+
                        C2*e2*conj(EigenVect(1,k))*EigenVect(3,k)-
                        C3*e1*conj(EigenVect(2,k))*EigenVect(3,k));
    Int[k+4]+=my_real(-C1*conj(e1*EigenVect(4,k))*EigenVect(3,k)+
                        C2*conj(e2*EigenVect(4,k))*EigenVect(2,k)+
                        C3*conj(e1*EigenVect(3,k))*EigenVect(2,k));
    }
  for (int k=1;k<=8;k++) 
  {
    if (Int[k]<0) cout<<" Monocryst. Negative.\n";
    Int[k]/=4;
  }
}




void Solver_H_Free_poly_Base::Intencity(CVecCl &Int,CVecCl &Pos,double *x,double *Res)
{
  int N=Pos.Dim();
  double s;
  my_comp pos;
  for (int k=1;k<=x[0];k++)
    {
    s=0;pos=my_comp(x[k],0);
    for (int k1=1;k1<=N;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
    Res[k]=s/M_PI;
    }
}




void Solver_H_Free_poly::Solve(double *x,double *Res)
{
  CVecCl Int,Pos;
  SolveIntPos(Ham->Make8Ham(),Int,Pos);
  Intencity(Int,Pos,x,Res);
}
void Solver_H_Free_poly::SolveIntPos(CMatrCl &Hamilt,CVecCl &Int,CVecCl &Pos)
{
  int N=Hamilt.Dim();
  CMatrCl Lft(N),Rgt(N);
  Pos.Dim(N);
  URComeig(Hamilt,Lft,Rgt);   
//  Pos=Hamilt;
  //for (int k=1;k<=N;k++) Pos[k]=Hamilt(k,k);
  Pos=Hamilt;
  IntClc(Lft,Rgt,Int);
}
void Solver_H_Free_poly::IntClc(CMatrCl &Lft,CMatrCl &Rgt,CVecCl &Int)
  {
   Rgt=(Rgt*MakeAmpMatrR());
   Lft=(Conj(Transpon(MakeAmpMatrL()))*Lft);
   int N=Rgt.Dim(),k;
   Int.Dim(N);
   for (k=1;k<=N;k++) Int[k]=CMatrCl::GetCol(Lft,k)*CMatrCl::GetRow(Rgt,k);
  }









void Solver_Blume_Nax_poly::Solve(double *x,double *Res)
{
  CVecCl Int,Pos;
  SolveIntPos(MakeMatr(),Int,Pos);
  Intencity(Int,Pos,x,Res);
}
CMatrCl Solver_Blume_Nax_poly::MakeAmpMatrLR(int Left)
{
  CMatrCl H=MakeAmpMatr();
  CMatrCl Mul(Nham);Mul=Mul*0;
  int k;
  if (Left)
  {
    VecCl p(Nham);
    for (k=1;k<=Nham;k++) p[k]=1/max<double>(fabs(Times[k]),MathZer);
    p=p/(p*(p*0+1));
    for ( k=1;k<=Nham;k++) Mul(k,k)=p[k];
  }
  else
  {
    for ( k=1;k<=Nham;k++) {for ( int k1=1;k1<=Nham;k1++) Mul(k,k1)=1;}
  }
  return DirectMultiplyOfMat(H,Mul);
}
CMatrCl Solver_Blume_Nax_poly::MakeMatr()
{
  int k,k1;
  CMatrCl One8(8),Res(Nham*8);Res=Res*0;One8=One8*0;
  CMatrCl Zer(Nham);Zer=Zer*0;
  my_comp sum=0;
  for (k=1;k<=Nham;k++)
  {
    for (k1=1;k1<=Nham;k1++)
    {
	    if (k==k1) One8=Ham[k-1]->Make8Ham()-my_comp(0,Times[k]*(Nham-1));
      else One8=One8*0+my_comp(0,Times[k]);
	    Zer(k,k1)=1;
      Res=Res+DirectMultiplyOfMat(One8,Zer);
      Zer(k,k1)=0;
    }
  }
  return Res;
}


void Solver_Free2_4::Solve(double *x,double *Res)
{
  CVecCl Int,Pos;
  SolveCurState();
  GenerateIntPos(Int,Pos);
  Intencity(Int,Pos,x,Res);
}
void Solver_Free2_4::GenerateIntPos(CVecCl &Int,CVecCl &Pos)
{
  vector<CMatrCl> AmpL,AmpR;
  GenerateAmp(AmpL,AmpR);
  Int.Dim(UeEig.Dim()*UgEig.Dim());Pos=Int*0;
  int k=0;
  for (int ke=1;ke<=UeEig.Dim();ke++)
  {
    for (int kg=1;kg<=UgEig.Dim();kg++)
    {
      k++;
      my_comp sum=0,tmp;
      for (int j=0;j<(int)AmpL.size();j++)
      {
        sum=sum+(CMatrCl::GetRow(UeRgt,ke)*AmpL[j]*CMatrCl::GetCol(UgLft,kg))*
                (CMatrCl::GetRow(UgRgt,kg)*AmpR[j]*CMatrCl::GetCol(UeLft,ke));
        //my_comp tmp=GetRow(UeRgt,ke)*AmpL[j]*GetCol(UgLft,kg);
        //sum=sum+tmp*conj(tmp);
//if ((kg==ke)&&(ke==1)) {cout<<" AmpL "<<AmpL[j];cout<<" AmpR "<<AmpR[j];}//<<"\n";}
      }
      Int[k]=sum;
      Pos[k]=UeEig[ke]+UgEig[kg]+my_comp(Ham->Is,-Ham->Wid/2);
    }
  }

}
void Solver_Free2_4::SolveCurState()
{
  //CMatrCl UeLft,UeRgt,UgLft,UgRgt;
  CMatrCl H4=Ham->Make4Ham(),H2=Ham->Make2Ham();
  URComeig(H4,UeLft,UeRgt);URComeig(H2,UgLft,UgRgt);
  UeEig=H4;UgEig=H2;
}
//void Solver_Free2_4::GenerateAmp(vector<CMatrCl> &AmpL,vector<CMatrCl> &AmpR)
//{
//  AmpL.clear();AmpR.clear();AmpR.resize(3);AmpL.resize(3);
//  AmpR[0].SetMatr(2,4);AmpR[0]=AmpR[0]*0;AmpR[1]=AmpR[0];AmpR[2]=AmpR[0];
//  my_comp d3=my_comp(sqrt(2./(3*8)),0),d2=my_comp(sqrt(4./(3*8)),0),
//          d1=my_comp(sqrt(6./(3*8)),0);
//
//  AmpR[0](1,1)=d1;AmpR[0](2,2)=d3;
//  AmpR[1](1,3)=d3;AmpR[1](2,4)=d1;
//  AmpR[2](1,2)=d2;AmpR[2](2,3)=d2;
//  AmpL[0]=Transpon(AmpR[0]);AmpL[1]=Transpon(AmpR[1]);AmpL[2]=Transpon(AmpR[2]);
//}


















































/*
void R2CircleInt(double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Teta_H,double Phi_H, 
                    double *x,double *y,double TCircle)
  {
   double a;
   a=Teta;a=Phi;a=H;a=Qs;a=Eta;a=Is;a=Wid;a=Teta_H;a=Phi_H;a=x[1];a=y[1];
   a=TCircle;
  };


// ==============================================
// ====== External field ========================
// ==============================================
VecCl FormHVec(double H,double H_zx,double H_xy)
  {
   VecCl Ret(3);
   double Teta=H_zx*M_PI/180,Phi=H_xy*M_PI/180;
   double M=H*sin(Teta);
   Ret[1]=H*cos(Teta);
   Ret[2]=M*cos(Phi);
   Ret[3]=M*sin(Phi);
   return Ret;
  }; 
void FormHPhi(VecCl &Hi,double &Hs,double &Hs_zx,double &Hs_xy)
  {
   Hs=sqrt(Hi*Hi);      if (Hs<StndErr)    {Hs_zx=0;Hs_xy=0;return ;}
   Hs_zx=acos(Hi[1]/Hs);                                    
   double H_xy=sqrt(sqr(Hi[2])+sqr(Hi[3]));
   if (H_xy<StndErr) {Hs_xy=0;Hs_zx*=180/M_PI; return ;}
   Hs_xy=acos(Hi[2]/H_xy);
//   if (Hi[2]<0) Hs_xy=M_PI-Hs_xy; 
   if (Hi[3]<0) { Hs_zx=2*M_PI-Hs_zx;Hs_xy=M_PI-Hs_xy; }
   Hs_xy*=180/M_PI;Hs_zx*=180/M_PI;
   VecCl Res=FormHVec(Hs,Hs_zx,Hs_xy);
//cout<<" HI "<<Hi<<" Res "<<Res-Hi;
  }; 
VecCl MulVec(VecCl &X,VecCl &Y)
  {
   VecCl Res(3);                           
   Res[1]=X[2]*Y[3]-X[3]*Y[2];
   Res[2]=X[3]*Y[1]-X[1]*Y[3];
   Res[3]=X[1]*Y[2]-X[2]*Y[1];
//cout<<" Res*X "<<Res*X<<" Res*Y "<<Res*Y;
   return Res;
  }; 
void FormHSum(double H,double H_zx,double H_xy,
              double I_Hx,double I_Hy,double I_Hz,double Hc,
              double &Hs,double &Hs_zx,double &Hs_xy,
              double &Q_zx,double &Q_xy)
  {
//  1  - z; 2 - x; 3 - y
   VecCl Hext=FormHVec(H,H_zx,H_xy);
   VecCl Hi(3);Hi[1]=I_Hz;Hi[2]=I_Hx;Hi[3]=I_Hy;
   for (int k=1;k<=3;k++) if (Hext[k]<0) Hi[k]=-Hi[k];
   Hi=Hi+Hext;Hs=sqrt(Hi*Hi);Hi=Hi*(Hc/Hs)+Hext;
// Find Hs in Hext basis. X axis in the plane z & Hext;
   VecCl Z=FormHVec(1,H_zx,H_xy);
   Hs=sqrt(Hi*Hi);if (Hs<StndErr) {Hs_zx=0;Hs_xy=0;return ;}
   VecCl Res(3);Res[1]=1;Res[2]=0;Res[3]=0;
   VecCl Y(3);Y=MulVec(Z,Res);double tmp=sqrt(Y*Y);
   if (tmp<StndErr) {Hs_zx=0;Hs_xy=0;return ;}
   Y=Y/tmp;VecCl X=MulVec(Y,Z);
   Res[1]=Hi*Z;Res[2]=Hi*X;Res[3]=Hi*Y;
   FormHPhi(Res,Hs,Q_zx,Q_xy);
   Y[1]=0;Y[3]=cos(H_xy*M_PI/180);Y[2]=-sin(H_xy*M_PI/180);
   X=MulVec(Y,Z);
//cout<<X*Y<<X*Z<<Z*Y<<"\n";
   Res[1]=Hi*Z;Res[2]=Hi*X;Res[3]=Hi*Y;
   FormHPhi(Res,Hs,Hs_zx,Hs_xy);
cout<<H<<"  "<<H_zx<<"  "<<H_xy<<"\n";
cout<<" Res "<<Res;
cout<<" Hi  "<<Hi;
cout<<Hs<<"  "<<Hs_zx<<"  "<<Hs_xy<<"\n\n";
  }; 
// This is Phi averaged?
void HQsAmpVecAver(double Teta_V,CMatrCl &AmpR)
  {
   CMatrCl A;A=R0HQsAmpVec_();
//   AmpR=A; return ;
   double  d1=sqrt((sqr(cos(Teta_V*M_PI/180))+1)/2),d2=fabs(sin(Teta_V*M_PI/180));
   SetCol(A,1,GetCol(A,1)*d1);
   SetCol(A,2,GetCol(A,2)*d1);
   SetCol(A,3,GetCol(A,3)*d2);
//cout<<GetRow(A,3);
   AmpR=A;
  };


void HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
              double Hc,double Teta_V,double *x,double *y)
  {
   double Coef=M_PI/12,Hs, Hs_zx,Hs_xy;
   double H_zx[7]={6,29.942,12.1007,35.0195,47.1329,43.7872,24.9733};
   double H_xy[7]={6,10.978,26.9384,36.0697,35.3578,12.3579,25.0513};
   double Az[5]={4,0,90,180,270};
   double Ax[3]={2,0,90};
   CMatrCl Mat(8),AmpR(8),AmpL(8);
   VecCl SumI(x[0]),I(x[0]);SumI=SumI*0;
   for (int az=1;az<=Az[0];az++)
     {
      for (int ax=1;ax<=Ax[0];ax++)
        {
         for (int k=1;k<=H_zx[0];k++)
           {
            double Q_zx,Q_xy;
            FormHSum(H,H_zx[k]+Az[az],H_xy[k]+Ax[ax], 
                     I_Hx,I_Hy, I_Hz, Hc, Hs, Hs_zx,Hs_xy,Q_zx,Q_xy);
//cout<<" bg_zx "<<H_zx[k]+Az[az]<<" bg_xy "<<H_xy[k]+Ax[ax]<<" Hs "<<Hs<<"\n";
//cout<<" HS_zx "<<Hs_zx<<" Hs_xy "<<Hs_xy<<" Hs "<<Hs<<"\n";
            R01HQs(Mat,//Teta,Phi,
                       Q_zx,Q_xy,Hs,Qs, Eta, Is, Wid, Hs_zx, Hs_xy);
            HQsAmpVecAver(Teta_V,AmpR);AmpL=AmpR;
            RIntTurn(Mat,AmpL,AmpR,x,I.Ptr,Wid);
            SumI=SumI+I;
           }
        }   
     }   
   Coef=M_PI/12;
   SumI=SumI*(Coef/H_zx[0]);movmem(SumI.Ptr,y,sizeof(double)*(x[0]+1));
  };  
double HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
              double Hc,double Teta_V,double x)
  {
   double xi[2]={1,x};
   double yi[2]={1,0};
   HQsAvInt(Teta,Phi,H,   Qs,  Eta, Is,  Wid,I_Hx,I_Hy,I_Hz, Hc,Teta_V , xi,  yi);
   return yi[1];
  }; 
*/   










/*

struct IntegrateParam
  {
   double *x;

   double TimeCircle;

   double Teta,Phi,H,Qs,Eta,Is,Wid,Teta_H,Phi_H;
  };

IntegrateParam *IntParam;

void FormAmpT(CMatrCl &Amp,CMatrCl &AmpT,CVecCl &Energy,
              CMatrCl &Mat,int N,double w)
  {}
void SumAmpT(CMatrCl &Sum,CMatrCl &Amp,CMatrCl &AmpT,CVecCl &Energy,
             CVecCl &OutX,double BkInt,double dt)
  {}

void Integrate()
  {
   CVecCl Pos;
   IntNext(CMatrCl &AmpL,CMatrCl &AmpR,CMatrCl &Turn,CMatrCl &Lft,
           CMatrCl &Rgt,CVecCl &Int);
   SumInt

   for (int k=1;k<=N;k++)
     {
      CMatrCl AmpRN=MullTau(Pos,AmpR);
      CVecCL IntN=IntAmp(AmpL,AmpRN);
      SumInt=SumInt+IntN*WPhi-IntP;
      TurnPhi(AmpL,AmpRN);
      AmpR=AmpRN;
      IntP=IntAmp(AmpL,AmpR);
     }; 

void GetVecIntTurn(CMatrCl &Mat,CVecCl &Int,CVecCl &Pos,CMatrCl &Amp)
  {
   int N=Mat.Dim();
   CVecCl Vec,VecL,VecR;
   CMatrCl Eig(N),Lft(N),Rgt(N);Eig=Mat;
   URComeig(Eig,Lft,Rgt);
///*
   for (int k=1;k<=N;k++)
     {
      Pos[k]=Eig(k,k);
      VecL=GetCol(Lft,k);
      VecR=GetRow(Rgt,k);
      Int[k]=0;
      for (int k1=1;k1<=N;k1++)
        {
         Vec=GetCol(Amp,k1);
         Int[k]+=(Conj(Vec)*(VecL))* (Vec*VecR);
        } 
     }
//   
      
   Rgt=Conj(Trans(Amp))*Lft*Rgt*Amp;
   for (int k=1;k<=N;k++) { Pos[k]=Eig(k,k);Int[k]=Rgt(k,k);}

  };

CVecCl R2CircleIntegrate(double time)
  {
   CMatrCl Mat;
void R0HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid)
   R01HQs(Mat,IntParam->Teta,IntParam->Phi,IntParam->H,IntParam->Qs,
              IntParam->Eta,IntParam->Is,IntParam->Wid,
              IntParam->Teta_H+time/IntParam->TimeCircle*360,
              IntParam->Phi_H);
   CVecCl Amp;Amp=R0HQsAmpVec();
   int N=IntParam->x[0];
//cout<<" N = "<<N<<"\n";
   double *y=new double[N+1];y[0]=N;
   RInt(Mat,Amp,IntParam->x,y,IntParam->Wid);
   CVecCl ret(N);
   for (int i=1;i<=N;i++) ret[i]=my_comp(y[i],0);
   ret=ret*exp(-time*IntParam->Wid);
   delete y;
   return ret;
  };

void R2CircleInt(double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Teta_H,double Phi_H, 
                    double *x,double *y,double TCircle)
                                          
  {
   IntParam=new IntegrateParam;
   IntParam->x=x;
   IntParam->TimeCircle=TCircle;

   IntParam->Teta=Teta;IntParam->Phi=Phi;IntParam->H=H;IntParam->Qs=Qs;
   IntParam->Eta=Eta;IntParam->Is=Is;IntParam->Wid=Wid;
   IntParam->Teta_H=Teta_H;IntParam->Phi_H=Phi_H;

   CVecCl result;
   int Flag;
   result=InFinQulb9(R2CircleIntegrate,0,1e-8,1e-12,10,100000,Flag);
   if (Flag!=0) { cout<<" Error in calc InFinQulb9 in R2CircleInt\n";}
   for (int i=1;i<=x[0];i++) y[i]=my_real(result[i]);
   delete IntParam;
  }
*/

/*


struct IntegrateParam
  {
   double *x;

   double TimeCircle;

   double Teta,Phi,H,Qs,Eta,Is,Wid,Teta_H,Phi_H;
  };

IntegrateParam *IntParam;

CVecCl R2CircleIntegrate(double time)
  {
   CMatrCl Mat;
   R01HQs(Mat,IntParam->Teta,IntParam->Phi,IntParam->H,IntParam->Qs,
              IntParam->Eta,IntParam->Is,IntParam->Wid,
              IntParam->Teta_H+time/IntParam->TimeCircle*360,
              IntParam->Phi_H);
   CVecCl Amp;Amp=R0HQsAmpVec();
   int N=IntParam->x[0];
//cout<<" N = "<<N<<"\n";
   double *y=new double[N+1];y[0]=N;
   RInt(Mat,Amp,IntParam->x,y,IntParam->Wid);
   CVecCl ret(N);
   for (int i=1;i<=N;i++) ret[i]=my_comp(y[i],0);
   ret=ret*exp(-time*IntParam->Wid);
   delete y;
   return ret;
  };

void R2CircleInt(double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Teta_H,double Phi_H, 
                    double *x,double *y,double TCircle)
                                          
  {
   IntParam=new IntegrateParam;
   IntParam->x=x;
   IntParam->TimeCircle=TCircle;

   IntParam->Teta=Teta;IntParam->Phi=Phi;IntParam->H=H;IntParam->Qs=Qs;
   IntParam->Eta=Eta;IntParam->Is=Is;IntParam->Wid=Wid;
   IntParam->Teta_H=Teta_H;IntParam->Phi_H=Phi_H;

   CVecCl result;
   int Flag;
   result=InFinQulb9(R2CircleIntegrate,0,1e-8,1e-12,10,100000,Flag);
   if (Flag!=0) { cout<<" Error in calc InFinQulb9 in R2CircleInt\n";}
   for (int i=1;i<=x[0];i++) y[i]=my_real(result[i]);
   delete IntParam;
  }
*/
/*
void R6HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double T1,double T2,double T3,
           double Alph_zx,double Alph_xy)
  {
   CMatrCl H11(8),H12(8),H13(8),
           H21(8),H22(8),H23(8),
           H31(8),H32(8),H33(8);
   R01HQs(H11,Teta, Phi, H, Qs, Eta, Is, Wid,    Alph_zx ,90+Alph_xy);
//cout<<" H11 \n"<<H11;
   R01HQs(H22,Teta, Phi, H, Qs, Eta, Is, Wid, 120+Alph_zx,90+Alph_xy);
//cout<<" H22 \n"<<H22;
   R01HQs(H33,Teta, Phi, H, Qs, Eta, Is, Wid,-120+Alph_zx,90+Alph_xy);
//cout<<" H33 \n"<<H33;
   H12=H12*0;H13=H12;H21=H12;H23=H12;H31=H12;H32=H12;
   double a12,a13,a21,a23,a31,a32;
   if ( (T1>MathZer) && (T2>MathZer) && (T3>MathZer) )
     {
      a12=T1*T2/(T3+T2);a13=T1*T3/(T3+T2);
      a21=T1*T2/(T3+T1);a23=T2*T3/(T3+T1);
      a31=T1*T3/(T1+T2);a32=T2*T3/(T1+T2);
      H11=H11-my_comp(0,T1);H12=H12+my_comp(0,a12);H13=H13+my_comp(0,a13);
      H21=H21+my_comp(0,a21);H22=H22-my_comp(0,T2);H23=H23+my_comp(0,a23);
      H31=H31+my_comp(0,a31);H32=H32+my_comp(0,a32);H33=H33-my_comp(0,T3);
     }
   Mat=Sum3Mat(H11,H12,H13,H21,H22,H23,H31,H32,H33);
//cout<<Mat;
  };
   

CMatrCl R6HQsAmpVec(double Alph_zx,double Alph_xy)
  {
   CVecCl A(8);A=R0HQsAmpVec();
   CMatrCl H11(8),H12(8),H13(8),
           H21(8),H22(8),H23(8),
           H31(8),H32(8),H33(8);
   H12=H12*0;H13=H12; H21=H12;H23=H12; H31=H12;H32=H12;
   H11=IFormTurnHam(    -Alph_zx,-90-Alph_xy);
   H22=IFormTurnHam(-120-Alph_zx,-90-Alph_xy);
   H33=IFormTurnHam( 120-Alph_zx,-90-Alph_xy);
   for (int k=1;k<=8;k++)
     {
      SetCol(H11,k,GetCol(H11,k)*A[k]);
      SetCol(H22,k,GetCol(H22,k)*A[k]);
      SetCol(H33,k,GetCol(H33,k)*A[k]);
     }
//cout<<"AmpVec\n"<<Sum3Mat(H11,H12,H13,H21,H22,H23,H31,H32,H33);
   return Sum3Mat(H11,H12,H13,H21,H22,H23,H31,H32,H33);
  }
void GetVec48Int(CMatrCl &Mat,CVecCl &Int,CVecCl &Pos,CMatrCl &Amp)
  {
   int N=48;
   CVecCl Vec,VecL,VecR;
   CMatrCl Eig(N),Lft(N),Rgt(N);Eig=Mat;
//cout<<" Eig-Eig_T \n"<<Eig-Conj(Transpon(Eig));
   URComeig(Eig,Lft,Rgt);
//cout<<" Delta EigConv \n"<<Lft*Eig*Rgt-Mat;
//cout<<" Lft_C_T-Rgt \n"<<Conj(Transpon(Lft))-Rgt;
//cout<<" Amp \n"<<Amp <<" Eig \n"<<Eig<<" Lft \n"<<Lft<<" Rgt \n"<<Rgt ;
   for (int k=1;k<=N;k++)
     {
      Pos[k]=Eig(k,k);
      VecL=GetCol(Lft,k);
      VecR=GetRow(Rgt,k);
//if (Mod2( Conj(VecL)-VecR )>StndErr) cout<<"E!!!!!!!!!!!!!!!!!!!"<<Mod2( Conj(VecL)-VecR )<<"\n"<<Conj(VecL)-VecR<<"\n";
      Int[k]=0;
      for (int k1=1;k1<=N;k1++)
        {
         Vec=GetCol(Amp,k1);
//if (norm( (Conj(Vec)*    (VecL))-(Vec*VecR))>StndErr) cout<<" Error notconj vec"<<(Conj(Vec)*    (VecL))-(Vec*VecR)<<"\n";
         Int[k]+=(Conj(Vec)*(VecL))* (Vec*VecR);
        } 
//      Int[k]=Conj(Vec)*( (Rgt*Vec) );
     }
cout<<"I ";
//cout<<" Int "<<Int<<"\n"<<" Pos "<<Pos<<"\n";
  };
// ==============================================
// =============  Calculate intecity    =========
// ==============================================

double R6HQsInt(CMatrCl &Mat,double x,double Alph_zx,double Alph_xy)
  {
   CMatrCl Amp(48);Amp=R6HQsAmpVec(Alph_zx,Alph_xy);
   CVecCl Int(48),Pos(48);

   GetVec48Int(Mat,Int,Pos,Amp);
//cout<<"Int "<<Int<<"\nPos "<<Pos<<"\n";
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=48;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
   return s*2.5;
  };
   
void R6HQsInt(CMatrCl &Mat,double *x,double *y,double Alph_zx,double Alph_xy)
  {
   CMatrCl Amp(48);Amp=R6HQsAmpVec(Alph_zx,Alph_xy);
//cout<<" Formed turn matr \n"<<Amp;
   CVecCl Int(48),Pos(48);
   GetVec48Int(Mat,Int,Pos,Amp);
//cout<<"Int "<<Int<<"\nPos "<<Pos<<"\n";
   double s;
   my_comp pos;
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=48;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*5/3;
     }
  };


// ===========================================  ===
// =============  Calculate intecity   relax=0. ===
// ==============  Just control  case  Not use it =
// ===========================================  ===

double R0HQsInt(CMatrCl &Mat,double x)
  {
   CVecCl Amp(8);Amp=R0HQsAmpVec();
   CVecCl Int(8),Pos(8);
   GetVecInt(Mat,Int,Pos,Amp);
   double s=0;my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=8;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
   return s*5;
  };

void R0HQsInt(CMatrCl &Mat,double *x,double *y)
  {
   CVecCl Amp(8);Amp=R0HQsAmpVec();
   CVecCl Int(8),Pos(8);
   GetVecInt(Mat,Int,Pos,Amp);
   double s;
   my_comp pos;
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=8;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*5;
     }
  };
// ==============================================
// =============  Form Full Relax  matrics  =====
// ==============================================


void RHQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown)
  {
   CMatrCl H1(8),H2(8),H3(8),H4(8);
   H2=H2*my_comp(0,0);H3=H2;
   R0HQs(H1,Teta, Phi, H, Qs, Eta, Is, Wid);                       
   R0HQs(H4,Teta, Phi, -H, Qs, Eta, Is, Wid);
   H2=H2+my_comp(0,Tup);H1=H1-my_comp(0,Tup);
   H3=H3+my_comp(0,Tdown);H4=H4-my_comp(0,Tdown);
   Mat=CombineMatr::Sum2Mat(H1,H2,H3,H4);
//cout<<Mat;
  };
   

CVecCl RHQsAmpVec(double t1,double t2)
  {
   CVecCl A(8);A=R0HQsAmpVec();
   CVecCl Ret(16);
   double c1,c2,s=fabs(t1)+fabs(t2);
   if (s>MathZer) {c1=fabs(t2)/s;c2=fabs(t1)/s;}
   else {c1=0.5;c2=0.5;}
   c1=sqrt(c1);c2=sqrt(c2);// Now
   for (int k=1;k<=8;k++) {Ret[k]=c1*A[k];Ret[k+8]=c2*A[k];}
   return Ret;
  }
// ==============================================
// =============  Calculate intecity    =========
// ==============================================

double RHQsInt(CMatrCl &Mat,double x,double Tup,double Tdown,double Wid1)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec(Tup,Tdown);
   CVecCl Int(16),Pos(16);
   GetVecInt(Mat,Int,Pos,Amp);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=16;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
   return s*2/Wid1;
  };
   
void RHQsInt(CMatrCl &Mat,double *x,double *y,double Tup,double Tdown,double Wid1)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec(Tup,Tdown);
   CVecCl Int(16),Pos(16);
   GetVecInt(Mat,Int,Pos,Amp);
   double s;
   my_comp pos;
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=16;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*2/Wid1;
     }
  };

// ==============================================
// =============  Form Full 3 Relax  matrics  ===
// ==============================================


void R3HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double T1,double T2,double T3,
           double Alph_zx,double Alph_xy)
  {
   CMatrCl H11(8),H12(8),H13(8),
           H21(8),H22(8),H23(8),
           H31(8),H32(8),H33(8);
   R01HQs(H11,Teta, Phi, H, Qs, Eta, Is, Wid,    Alph_zx ,Alph_xy);
//cout<<" H11 \n"<<H11;
   R01HQs(H22,Teta, Phi, H, Qs, Eta, Is, Wid, 120+Alph_zx,Alph_xy);
//cout<<" H22 \n"<<H22;
   R01HQs(H33,Teta, Phi, H, Qs, Eta, Is, Wid,-120+Alph_zx,Alph_xy);
//cout<<" H33 \n"<<H33;
   H12=H12*0;H13=H12;H21=H12;H23=H12;H31=H12;H32=H12;
///*
   double a12,a13,a21,a23,a31,a32;
   if ( (T1>MathZer) && (T2>MathZer) && (T3>MathZer) )
     {
      a12=T1*T2/(T3+T2);a13=T1*T3/(T3+T2);
      a21=T1*T2/(T3+T1);a23=T2*T3/(T3+T1);
      a31=T1*T3/(T1+T2);a32=T2*T3/(T1+T2);
      H11=H11-my_comp(0,T1);H12=H12+my_comp(0,a12);H13=H13+my_comp(0,a13);
      H21=H21+my_comp(0,a21);H22=H22-my_comp(0,T2);H23=H23+my_comp(0,a23);
      H31=H31+my_comp(0,a31);H32=H32+my_comp(0,a32);H33=H33-my_comp(0,T3);
     }
//* /
      H11=H11-my_comp(0,T3+T2);H12=H12+my_comp(0,T2);H13=H13+my_comp(0,T3);
      H21=H21+my_comp(0,T1);H22=H22-my_comp(0,T1+T3);H23=H23+my_comp(0,T3);
      H31=H31+my_comp(0,T1);H32=H32+my_comp(0,T2);H33=H33-my_comp(0,T1+T2);

   Mat=Sum3Mat(H11,H12,H13,H21,H22,H23,H31,H32,H33);
//cout<<Mat;
  };
   

// ==============================================
// =============  Calculate intecity    =========
// ==============================================

double R3HQsInt(CMatrCl &Mat,double x,double Alph_zx,double Alph_xy,
                             double t1,double t2,double t3,double Wid1)
  {
   CMatrCl Amp(24);Amp=R3HQsAmpVec(Alph_zx,Alph_xy,t1,t2,t3);
   CVecCl Int(24),Pos(24);

   GetVec24Int(Mat,Int,Pos,Amp);
//cout<<"Int "<<Int<<"\nPos "<<Pos<<"\n";
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=24;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
   return s*2/Wid1;
  };
   
void R3HQsInt(CMatrCl &Mat,double *x,double *y,double Alph_zx,double Alph_xy,
                             double t1,double t2,double t3,double Wid1)
  {
   CMatrCl Amp(24);Amp=R3HQsAmpVec(Alph_zx,Alph_xy,t1,t2,t3);
//cout<<" Formed turn matr \n"<<Amp;
   CVecCl Int(24),Pos(24);
   GetVec24Int(Mat,Int,Pos,Amp);
//cout<<"Int "<<Int<<"\nPos "<<Pos<<"\n";
   double s;
   my_comp pos;
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=24;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*2/Wid1;
     }
  };

// ==============================================
// =============  Form Full Relax  0H matrics ===
// ==============================================



void RHQs0Qs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown)
  {
   CMatrCl H1(8),H2(8),H3(8),H4(8);
   H2=H2*my_comp(0,0);H3=H2;
   R0HQs(H1,Teta, Phi, H, Qs, Eta, Is, Wid);                       
   R0HQs(H4,Teta, Phi, 0, Qs, Eta, Is, Wid);
   H2=H2+my_comp(0,Tup);H1=H1-my_comp(0,Tup);
   H3=H3+my_comp(0,Tdown);H4=H4-my_comp(0,Tdown);
   Mat=CombineMatr::Sum2Mat(H1,H2,H3,H4);
  };
double RHQs0QsInt(CMatrCl &Mat,double x,double Tup,double Tdown,double Wid1)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec(Tup,Tdown);
   CVecCl Int(16),Pos(16);
   GetVecInt(Mat,Int,Pos,Amp);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=16;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
   return s*2/Wid1;
  };
   
void RHQs0QsInt(CMatrCl &Mat,double *x,double *y,double Tup,double Tdown,double Wid1)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec(Tup,Tdown);
   CVecCl Int(16),Pos(16);
   GetVecInt(Mat,Int,Pos,Amp);
   double s;
   my_comp pos;
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=16;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*2/Wid1;
     }
  };
   

// =========================================================
// =============  Form Full Relax  Free1 - Free2 matrics ===
// =========================================================



void RFree(CMatrCl &Mat,
             double Teta1,double Phi1,double H01,double Qs1,double Eta1,
             double Is1,double Wid1,double Alph_H_zx1,double Alph_H_xy1,
             double Teta2,double Phi2,double H02,double Qs2,double Eta2,
             double Is2,double Wid2,double Alph_H_zx2,double Alph_H_xy2,
             double Tup,double Tdown)
  {
   CMatrCl H1(8),H2(8),H3(8),H4(8);
   H2=H2*my_comp(0,0);H3=H2;
   R01HQs(H1,Teta1, Phi1, H01, Qs1, Eta1, Is1, Wid1,Alph_H_zx1,Alph_H_xy1);
   R01HQs(H4,Teta2, Phi2, H02, Qs2, Eta2, Is2, Wid2,Alph_H_zx2,Alph_H_xy2);
   H2=H2+my_comp(0,Tup);H1=H1-my_comp(0,Tup);
   H3=H3+my_comp(0,Tdown);H4=H4-my_comp(0,Tdown);
   Mat=CombineMatr::Sum2Mat(H1,H2,H3,H4);
  };
double RFreeInt(CMatrCl &Mat,double x,double Tup,double Tdown,double Wid1)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec(Tup,Tdown);
   CVecCl Int(16),Pos(16);
   GetVecInt(Mat,Int,Pos,Amp);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=16;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
   return s*2/Wid1;
  };
   
void RFreeInt(CMatrCl &Mat,double *x,double *y,double Tup,double Tdown,double Wid1)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec(Tup,Tdown);
   CVecCl Int(16),Pos(16);
   GetVecInt(Mat,Int,Pos,Amp);
   double s;
   my_comp pos;
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=16;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*2/Wid1;
     }
  };
   
// ==============================================
// =============  Control Block. Not to be used =
// ==============================================

double QsHInt(CMatrCl Mat,double Teta,double Phi,double x)
  {
   int N=Mat.Dim();
   CVecCl Int(N),Pos(N);
   GetVec8Int(Mat,Teta,Phi,Pos,Int);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=N;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
   return s*5;
  };   
void QsHInt(CMatrCl Mat,double Teta,double Phi,double *x,double *y)
  {
   int N=Mat.Dim();
   CVecCl Int(N),Pos(N);
//cout<<Mat;
   GetVec8Int(Mat,Teta,Phi,Pos,Int);
   double s;
   my_comp pos;
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=N;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*5;
     }
  };


*/












/*
//       Method used in old very slow version. Mult vectors
//       of probability on the inverce Matrics. But it is
//       very strict.

CVecCl FormVec(CVecCl &Vec,int k)
  { CVecCl ret=Vec*0;ret[k]=Vec[k];return ret;}
double GetInt(CMatrCl &Ham,CVecCl &Amp)
  {
   double Tol=1e-14,ret=0;
   my_comp d1;int N=Ham.Dim(),l;
   CMatrCl LU;
   CVecCl Mis(N),Amp1(N),Res(N);
   int d2,*rear=new int[N+1];rear[0]=N;

   LU=Ham;
   int re=URCunsymdet(LU,Tol,d1,d2,rear);
   if (!re) {delete rear;cout<<" Error R0HQsInt\n";return 0;}
// Solution - trace after mult on single matrics
   for (int k1=1;k1<=N;k1++)
     {
      Amp1=FormVec(Amp,k1);
      re=URCunsymaccsol(Ham,LU,rear,Amp1,Tol,Res,Mis,l);
      ret+=my_imag(Amp1*Res);
//if (k==100) {cout<<Tmp<<Res;ChRead();}
     }
// Delete vars for solving Mat
   delete rear;
   return ret; 
  }

void R0HQsInt(CMatrCl &Mat,double *x,double *y)
  {
   CVecCl Amp(8);Amp=R0HQsAmpVec();
   CMatrCl Tmp(8);
   for (int k=1;k<=x[0];k++)
     {
      Tmp=Mat-my_comp(x[k],0);
      y[k]=5*GetInt(Tmp,Amp);
     }
  };

void RHQsInt(CMatrCl &Mat,double *x,double *y)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec();
   CMatrCl Tmp(16);
   for (int k=1;k<=x[0];k++)
     {
      Tmp=Mat-my_comp(x[k],0);
      y[k]=2.5*GetInt(Tmp,Amp);
     }
  };


*/






/*
// =============  Form Full Relax3   matrics ===

void R3Turn120(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double T1,double T2,double T3,
           double Alph_zx,double Alph_xy)
  {
   CMatrCl H11(8),H12(8),H13(8),
           H21(8),H22(8),H23(8),
           H31(8),H32(8),H33(8);
   R01HQs(H11,Teta, Phi, H, Qs, Eta, Is, Wid,    Alph_zx ,Alph_xy);
//Mat=H11;return ;

   R01HQs(H22,Teta, Phi, H, Qs, Eta, Is, Wid, 120+Alph_zx,Alph_xy);
   R01HQs(H33,Teta, Phi, H, Qs, Eta, Is, Wid,-120+Alph_zx,Alph_xy);
   H12=H12*0;H13=H12;H21=H12;H23=H12;H31=H12;H32=H12;
   T1=fabs(T1);T2=fabs(T2);T3=fabs(T3);
      H11=H11-my_comp(0,T3+T2);H12=H12+my_comp(0,T2);H13=H13+my_comp(0,T3);
      H21=H21+my_comp(0,T1);H22=H22-my_comp(0,T1+T3);H23=H23+my_comp(0,T3);
      H31=H31+my_comp(0,T1);H32=H32+my_comp(0,T2);H33=H33-my_comp(0,T1+T2);
   Mat=Sum3Mat(H11,H12,H13,H21,H22,H23,H31,H32,H33);
  };

// =============  Form Full Relax  0H matrics ===

void R2HUpHZer(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown)
  {
   CMatrCl H1(8),H2(8),H3(8),H4(8);
   H2=H2*my_comp(0,0);H3=H2;
   R0HQs(H1,Teta, Phi, H, Qs, Eta, Is, Wid);                       
   R0HQs(H4,Teta, Phi, 0, Qs, Eta, Is, Wid);
   H2=H2+my_comp(0,Tup);H1=H1-my_comp(0,Tup);
   H3=H3+my_comp(0,Tdown);H4=H4-my_comp(0,Tdown);
   Mat=Sum2Mat(H1,H2,H3,H4);
  };

// =============  Form Full Relax +-H matrics  =====

void R2HUpHDown(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown)
  {
   CMatrCl H1(8),H2(8),H3(8),H4(8);
   H2=H2*my_comp(0,0);H3=H2;
   R0HQs(H1,Teta, Phi, H, Qs, Eta, Is, Wid);                       
   R0HQs(H4,Teta, Phi, -H, Qs, Eta, Is, Wid);
   H2=H2+my_comp(0,Tup);H1=H1-my_comp(0,Tup);
   H3=H3+my_comp(0,Tdown);H4=H4-my_comp(0,Tdown);
   Mat=Sum2Mat(H1,H2,H3,H4);
//cout<<Mat;
  };


//  H(1&2) (Alph_zx,Alph_xy) z
void R2HQsAmpVec(double Alph_zx1,double Alph_xy1,
                    double Alph_zx2,double Alph_xy2,
                                   double t1,double t2,
                                   CMatrCl &AmpL,CMatrCl &AmpR)
  {
   double p1,p2,s=(fabs(t1)+fabs(t2));
   if (s>MathZer) {s=1/s;p1=fabs(t1)*s;p2=fabs(t2)*s;}
   else {p1=0.5;p2=0.5;s=1;};
   CMatrCl H(8,3);H=R0HQsAmpVec_();
   CMatrCl Zer(8,3);Zer=Zer*0;
   AmpR=Sum2Mat(H,H,H,H); 
   AmpL=Sum2Mat(H*p1,Zer ,Zer ,H*p2);
//cout<<AmpL<<"\n"<<AmpR;
   }

//  H (Alph_zx,Alph_xy) z
void R3HQsAmpVec(double Alph_zx1,double Alph_xy1,
                    double Alph_zx2,double Alph_xy2,
                    double Alph_zx3,double Alph_xy3,
                                   double t1,double t2,double t3,
                                   CMatrCl &AmpL,CMatrCl &AmpR)
  {
   double p1,p2,p3,s=fabs(t1)+fabs(t2)+fabs(t3);                 
   if (s>MathZer) {s=1/s;p1=fabs(t1)*s;p2=fabs(t2)*s;p3=fabs(t3)*s;}   
   else {s=1./3;p1=s;p2=s;p3=s;};
   CMatrCl H(8,3);H=R0HQsAmpVec_();

//AmpL=AmpR=H;return;
   CMatrCl Zer(8,3);Zer=Zer*0;
   AmpR=Sum3Mat(H,H,H,H,H,H,H,H,H); 
   AmpL=Sum3Mat(H*p1,Zer ,Zer ,Zer ,H*p2,Zer ,Zer ,Zer ,H*p3);
   }


// H//z
/*
void GetVecInt(CMatrCl &Mat,CVecCl &Int,CVecCl &Pos,CVecCl &Amp)
  {
   int N=Mat.Dim(); 
   CMatrCl Eig(N),Lft(N),Rgt(N);Eig=Mat;
   URComeig(Eig,Lft,Rgt);
   for (int k=1;k<=N;k++)
     {
      Pos[k]=Eig(k,k);
      Int[k]=0;
      for (int k1=1;k1<=N;k1++) 
        {
         Int[k]+=norm(Amp[k1])*Lft(k1,k)*Rgt(k,k1);
        }
     }
  };   
*/

/*
//H - not turned
double RInt(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double x,double Wid1)
  {
   int N=Mat.Dim();
   CVecCl Int(N),Pos(N);

   GetVecInt(Mat,Int,Pos,AmpL,AmpR);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=N;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
   return s*2/Wid1;
  };
   
void RInt(CMatrCl &Mat,CVecCl &AmpL,CMatrCl &AmpR,double *x,double *y,double Wid1)
  {
   int N=Mat.Dim();
   CVecCl Int(N),Pos(N);
   GetVecInt(Mat,Int,Pos,AmpL,AmpR);
   double s;
   my_comp pos;
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=N;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*2/Wid1;
     }
  };

* /






*/

//#define MCmu_e  2.2363       //This is old value
//#define MCmu_g -3.9156       //This is old value
//
////#define MCmu_e  2.2843       // Value got from Phys Encyclop relation
////#define MCmu_g  -3.8333045254  // Value got from Phys Encyclop relation
//   //   to use them - have to vary MCmms_tesl const
//
////#define MCmu_e -3.9156     //This set is done according to Phys Encyclop
////#define MCmu_g  2.2843
//
//#define MCmms_tesl 84.5
////#define Pi M_PI
////3.14159265358979
//
//void FormMessbInput(double &H,double &Teta,double &Phi)
//  {
//   H/=MCmms_tesl;
//   Teta*=M_PI/180;
//   Phi*=M_PI/180;
////   if (H<0) Teta+=M_PI;// New addition !!!!!!!!
//  };
//
//// Res ham: Z//H
//// Qs main axis system is rotated, first:
////      Phi - rot around Qs_z
////                                 then:
////     Teta - rot around new Fix_y
//// result - Eiler angles
//void HQsEiler_Mat(CMatrCl &Mat,double Teta,double Phi,
//            double H,double Qs,double Eta)
//   {
//    Mat.Dim(4);
//    FormMessbInput(H,Teta,Phi);
//    double mx=Qs/(3*sqrt(1+sqr(Eta)/3));
//    double mb=-H*fabs(1/MCmu_g)*MCmu_e;
//    double mc=mx*(1-sqr(sin(Teta))*(3-Eta*cos(2*Phi))/2);
//    double md1=mx*sin(Teta)*cos(Teta)*(3-Eta*cos(2*Phi))/sqrt(3.);
//    double md2=mx*sin(Teta)*Eta*sin(2*Phi)/sqrt(3.);
//    my_comp md=my_comp(md1,md2);
//    double me1=mx*(3+Eta*cos(2*Phi)-sqr(cos(Teta))*(3-Eta*cos(2*Phi)))/(2*sqrt(3.));
//    double me2=-mx*Eta*cos(Teta)*sin(2*Phi)/sqrt(3.);
//    my_comp me=my_comp(me1,me2);
//
//    Mat(1,1)=-mb+mc;         Mat(1,2)=md;
//    Mat(2,1)=conj(md);       Mat(2,2)=-mb/3-mc;
//    Mat(3,1)=conj(me);       Mat(3,2)=0;
//    Mat(4,1)=0;              Mat(4,2)=conj(me);
//
//    Mat(1,3)=me;             Mat(1,4)=0;
//    Mat(2,3)=0;              Mat(2,4)=me;
//    Mat(3,3)=mb/3-mc;        Mat(3,4)=-md;
//    Mat(4,3)=-conj(md);      Mat(4,4)=mb+mc;
//    Mat=Mat*1.5;
////cout<<Mat;
//  };
//
//// Now res ham: Z//H
//// Just now corrected !!!!!!!
//// Qs main axis system is rotated, first:
////     Teta - rot around Fix_x
////                                 then:
////      Phi - rot around Fix_z
//// result - for axial Field ( H//Z ) does not depend on Phi
////                                   for any angles Teta, values Eta
////          axis X' - remain always in X-Y plain
//// Now res ham: Z//H
//// Qs main axis system is rotated, first:
////     Gamma - rot around Fix_z
////                                 then
////     Teta - rot around Fix_x
////                                 then:
////      Phi - rot around Fix_z
//// result - for axial Field ( H//Z ) does not depend on Phi
////                                   for any angles Teta, values Eta
////          axis X' - remain always in X-Y plain
//// and then effectively we rotate the result around new Z' - by Gamma -
////          result depends on Gamma          
//void HQsGamma_Mat(CMatrCl &Mat,double Teta,double Phi,
//            double H,double Qs,double Eta,double Gamma)
//   {
//     CMatrCl MatH,MatQs,Turn=TurnSpinors::Turn4Mat(Teta,Phi),ITurn=TurnSpinors::Turn4Mat(0,Gamma);
//     Turn=ITurn*Turn;
//     ITurn=Conj(Transpon(Turn));
//     HQsEiler_Mat(MatH,0,0,H,0,0);
//     HQsEiler_Mat(MatQs,0,0,0,Qs,Eta);
//     Mat=MatH+ITurn*MatQs*Turn;
//  };
//
//void QsHMat(CMatrCl &Mat,double Teta,double Phi,
//             double H,double Qs,double Eta,double IS,CMatrCl *MatPureExite,CMatrCl *MatGroundState)
//   {
//    Mat.Dim(4);
//    FormMessbInput(H,Teta,Phi);
//    double ma=H*fabs(1/MCmu_g)*MCmu_e;
//    double mb=Qs/sqrt(1+sqr(Eta)/3)*0.5;
//    double caii=cos(Teta)/2*ma;
//    my_comp mul=sin(Teta)*my_comp(cos(Phi),sin(Phi));
//    my_comp ca21=-sqrt(3.)*0.5*mul*ma;
//    my_comp ca32=-mul*ma;
//    double  ca31=Eta*mb/sqrt(3.);
//
//    Mat(1,1)=mb-3*caii;        Mat(1,2)=conj(ca21);
//    Mat(2,1)=ca21;             Mat(2,2)=-mb-caii;
//    Mat(3,1)=ca31;             Mat(3,2)=ca32;
//    Mat(4,1)=0;                Mat(4,2)=ca31;
//
//    Mat(1,3)=ca31;             Mat(1,4)=0;
//    Mat(2,3)=conj(ca32);       Mat(2,4)=ca31;
//    Mat(3,3)=-mb+caii;         Mat(3,4)=conj(ca21);
//    Mat(4,3)=ca21;             Mat(4,4)=mb+3*caii;
//
////  Now Form Ground
//    ma=H*fabs(1/MCmu_g)*MCmu_g;
//    caii=-ma*cos(Teta)*0.5;ca21=-ma/ /*sqrt*/(2)*conj(mul);
//    CMatrCl Gr(2);Gr(1,1)=caii;Gr(1,2)=conj(ca21);Gr(2,1)=ca21;Gr(2,2)=-caii;
//// New addition - if necessary all information
////    Mat=Mat*(-1);Gr=Gr*(-1);
//    if ( (MatPureExite!=NULL) && (MatGroundState!=NULL) ) 
//		{*MatPureExite=(Mat+IS)*(1);*MatGroundState=Gr*(1);}
//
//    Gr=CombineMatr::ExtendMat(Gr,4);
////cout<<"Ground Matrics\n"<<Gr;
//    CMatrCl Zer(4);Zer=Mat*0;
//    Mat=CombineMatr::Sum2Mat(Mat,Zer,Zer,Mat);Mat=Mat-Gr+IS;
////cout<<"Full Matrics\n"<<Mat;
//  };
//
//
//// ===========================================================
//// ======================  ===================================
//// ===========================================================
//void IntPos(CMatrCl &EigenVect,double *EigenVal,double *Intencity,
//                               double *Position,double H,double IS,VecCl Amp2)
//  {
//   double shi=MCmu_g*H/(2*fabs(MCmu_g))/MCmms_tesl;
//   my_comp s,s1;
//   for (int k=1;k<=4;k++)
//     {
//      Position[k]=EigenVal[k]-shi+IS;
//      Position[k+4]=EigenVal[k]+shi+IS;
//     }
//   for (k=1;k<=4;k++)
//     {
//      Intencity[k]=0;Intencity[k+4]=0;
//      for (int k1=1;k1<=4;k1++) 
//        {
//         double tmp=norm(EigenVect(k1,k));
//         Intencity[k]+=    Amp2[k1]*tmp;
//         Intencity[k+4]+=Amp2[5-k1]*tmp;
//        }
//     }
//  };
//int HQsMatClc(CMatrCl &Mat,
//              double H,double IS, //for intencity calc 
//           double *Intencity,double *Position)
//  {
////   FormMessbInput();
//   CMatrCl Hamilt,EigenVect(4);
//   VecCl EigenVal(4);
//   Hamilt=Mat;//   HQsMat(Hamilt,Teta,Phi,H,Qs,Eta);
//   
//   CVecCl EVal;CMatrCl EigenVect1(4);int Ret=1;
//   URComeig(Hamilt,EigenVect1,EigenVect);          
//   for (int k=1;k<=4;k++) EigenVal[k]=my_real(Hamilt(k,k));EigenVect=Transpon(EigenVect);
//   VecCl Amp2(4);Amp2[1]=0.25;Amp2[2]=1./6.;Amp2[3]=1./12.;Amp2[4]=0;
//   IntPos(EigenVect,EigenVal.Ptr,Intencity,Position,H,IS,Amp2);
//   return Ret;
//  };
//
//// Three angles of Qs rotation Teta,Gamma,Phi( rot around Z)
//int HQsMatMono3Clc(double Teta,double Gamma,double Phi,double H,double Qs,double IS,
//                  double Eta,double TetaH_View,double PhiH_View,
//                  double *Intencity,double *Position)
//  {
//   CMatrCl Hamilt(4),EigenVect(4);
//   double *EigenVal=new double[5];EigenVal[0]=4;
//   HQsGamma_Mat(Hamilt,Teta,Phi,H,Qs,Eta,Gamma);
//
//   CVecCl EVal;CMatrCl EigenVect1(4);int Ret=1;
//   URComeig(Hamilt,EigenVect1,EigenVect);          
//   for (int i=1;i<=4;i++) EigenVal[i]=my_real(Hamilt(i,i));EigenVect=Transpon(EigenVect);
////   int Ret=CReduceLQ(Hamilt,EigenVect,EigenVal,StndErr);
//
//   VecCl Amp2(4);
//   double Cos=cos(TetaH_View*M_PI/180),Sin=sin(TetaH_View*M_PI/180);
//   double Cos2=sqr(Cos);
//   Amp2[1]=(1+Cos2)*0.75;Amp2[2]=1.-Cos2;Amp2[3]=(1+Cos2)*0.25;Amp2[4]=0;
//   IntPos(EigenVect,EigenVal,Intencity,Position,H,IS,Amp2);
////  Add interference 
//   my_comp e1=my_comp(cos(PhiH_View*M_PI/180),sin(PhiH_View*M_PI/180)),e2=e1*e1;
//   my_comp C1=sqrt(3.)*Sin*Cos,C2=0.5*sqrt(3.)*sqr(Sin),C3=C1/sqrt(3.);
//   for (int k=1;k<=4;k++)
//     {
//      Intencity[k]+=my_real(   C1*e1*conj(EigenVect(1,k))*EigenVect(2,k)+
//                            C2*e2*conj(EigenVect(1,k))*EigenVect(3,k)-
//                            C3*e1*conj(EigenVect(2,k))*EigenVect(3,k));
////cout<<" Int "<<Intencity[k];
//      Intencity[k+4]+=my_real(-C1*conj(e1*EigenVect(4,k))*EigenVect(3,k)+
//                            C2*conj(e2*EigenVect(4,k))*EigenVect(2,k)+
//                            C3*conj(e1*EigenVect(3,k))*EigenVect(2,k));
////cout<<" Int "<<Intencity[k+4];
//     }
////cout<<"\n";
//   for (k=1;k<=8;k++) 
//   {
//     if (Intencity[k]<0) cout<<" Monocryst. Negative.\n";
//// Why is it so ????
//     Intencity[k]/=4;
//   }
//   delete EigenVal;
//   return Ret;
//  };
//   
//double HQsInt(double *Intencity,double *Position,double Wid,double x)
//  {
//   double Res=0;
//   double K=Wid/(2*M_PI);
//   for (int k=1;k<=8;k++) Res+=Intencity[k]/(sqr(Wid/2)+sqr(x-Position[k]));
//   return Res*K;
//  };
//      
//void HQsInt(double *Intencity,double *Position,double Wid,double *x,double *y)
//  {
//   double s,p,W=sqr(Wid/2);
//   double K=Wid/(2*M_PI);
//   for (int k=1;k<=x[0];k++)
//     {
//      p=x[k];s=0;
//      for (int k1=1;k1<=8;k1++) s+=Intencity[k1]/(W+sqr(Position[k1]-p));
//      y[k]=s*K;
//     }
//  };
//
//
//// ==============================================
//// =============  Relaxation case   =============
//// ==============================================
//
//// ===========================================================
//// ========= Calculating AmpVec - tors  ======================
//// ===========================================================
//
////  H//z
//
//CVecCl R0HQsAmpVec()
//  {
//   CVecCl Amp(8);
//   Amp[1]=my_comp(-0.5,0);       Amp[2]=my_comp(1/sqrt(6.),0);
//   Amp[3]=my_comp(-1/sqrt(12.),0);Amp[4]=my_comp(0,0);
//   for (int k=1;k<=4;k++) Amp[9-k]=-Amp[k];
//   return Amp;
//  };
//CMatrCl R0HQsAmpVec_()
//  {
//   CMatrCl Ret(8,3);Ret=Ret*0;
//   my_comp d3=my_comp(sqrt(1./3),0),d2=my_comp(2*sqrt(1./3),0);
//   Ret(1,1)=1; Ret(3,1)=d3;Ret(8,1)=1;Ret(6,1)=d3;
//   Ret(1,2)=1; Ret(3,2)=-d3;Ret(8,2)=-1;Ret(6,2)=d3;
//   Ret(2,3)=d2; Ret(7,3)=d2;
//   Ret=Ret*sqrt(1./8);
//   return Ret;
//  }; 
//
//void R2HQsAmpVec(double t1,double t2,CMatrCl &AmpL,CMatrCl &AmpR)
//  {
//	  VecCl time(2);time[1]=t1;time[2]=t2;
//	  RnHQsAmpVec(2,time.Ptr,AmpL,AmpR);
///*
//   double p1,p2,s=fabs(t1)+fabs(t2);
//   if (s>MathZer) {p1=fabs(t2)/s;p2=fabs(t1)/s;}
//   else {p1=0.5;p2=0.5;}
//   CMatrCl H(8,3);H=R0HQsAmpVec_();
//   CMatrCl Zer(8,3);Zer=Zer*0;
//   AmpR=CombineMatr::Sum2Mat(H,H,H,H); 
//   AmpL=CombineMatr::Sum2Mat(H*p1,Zer ,Zer ,H*p2);
//*/
//  };
//
//void RnHQsAmpVec(int n,double *Time,CMatrCl &AmpL,CMatrCl &AmpR)
//  {
//   VecCl p(n);//Times[i]
////   double s=0;
////   for (int k=1;k<=n;k++) {p[k]=fabs(Time[k]);s+=p[k];}
//   //if (s>MathZer) {s=1/s;for ( k=1;k<=n;k++) p[k]*=s;}
//   //else {s=1./n;for ( k=1;k<=n;k++) p[k]=s;}
//   for (int k=1;k<=n;k++) p[k]=1/max<double>(fabs(Time[k]),MathZer);
//   p=p/(p*(p*0+1));
//   CMatrCl H(8,3);H=R0HQsAmpVec_();
//   CMatrCl Mul(n);Mul=Mul*0;
//   for ( k=1;k<=n;k++) {for ( int k1=1;k1<=n;k1++) Mul(k,k1)=1;}
////   for ( k=1;k<=n;k++) Mul(k,k)=1;
//   AmpR=DirectMultiplyOfMat(H,Mul);
//   Mul=Mul*0;for ( k=1;k<=n;k++) Mul(k,k)=p[k];
//   AmpL=DirectMultiplyOfMat(H,Mul);
//
//   }
//// ==============================================
//// ========= Forms  relaxing Ham matrics. =======
//// ==============================================
//
//void R0HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
//           double Is,double Wid)
//  {
//   my_comp shi=my_comp(MCmu_g*H/(2*fabs(MCmu_g))/MCmms_tesl,0);
//   my_comp sh=my_comp(Is,-Wid/2);
//   CMatrCl H1(4),H2(4),H3(4),H4(4),Ham(4);
//   HQsMat(Ham,Teta,Phi,H,Qs,Eta);
//   H1=Ham+(sh-shi);H4=Ham+(shi+sh);
//   H2=H2*my_comp(0,0);H3=H2;
////  Formed SubMatrics of one Full HQs matrics
//   Mat=CombineMatr::Sum2Mat(H1,H2,H3,H4);
//  };
//
////  Teta - Phi - Qs main vectors.  
////  Teta_H Phi_H H  main vectors. 
////  Constructing H=HamQs+HamH
//// to be corrected
//void R01HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
//           double Is,double Wid,double Teta_H,double Phi_H)
//  {
//   CMatrCl H1(8);
////   R0HQs(H1,Teta_H, Phi_H, 0, Qs, Eta, Is, Wid);
////   QsHMat(Mat,Teta,Phi, H,0,0,0);
//   R0HQs(H1,Teta, Phi, 0, Qs, Eta, Is, Wid);
//   QsHMat(Mat,Teta_H,Phi_H, H,0,0,0);
////   Mat=FormHHam(Teta_H,Phi_H,MCmu_g/MCmu_e)*
////                          (H*fabs(1/MCmu_g)*MCmu_e/MCmms_tesl);
//   Mat=Mat+H1;
//  };
//
//
//// ==============================================
//// =============  Calculate intecity  matr ======
//// =============  univercial for All relax ======
//// ==============================================
//
////  Adds turn -  H (Teta,Phi) z
//void GetVecIntTurn(CMatrCl &Mat,CVecCl &Int,CVecCl &Pos,
//                   CMatrCl &AmpL,CMatrCl &AmpR,
//                   CMatrCl *retEig,CMatrCl *retLft,CMatrCl *retRgt)
//  {
//   int N=Mat.Dim(),k;
//   CVecCl VL(N),VecL(N),VecR(N),VR(N);
//   CMatrCl Eig(N),Lft(N),Rgt(N);Eig=Mat;
//   URComeig(Eig,Lft,Rgt);
//   if (retEig!=NULL) { *retEig=Eig;*retLft=Lft;*retRgt=Rgt;}
//   Rgt=(Rgt*AmpR);
//   Lft=(Conj(Transpon(AmpL))*Lft);
//   Pos.Dim(N);Int.Dim(N);
//   CVecCl tmp;
////   int Base;Lft.DimMN(Base,N);
//   for (k=1;k<=N;k++) 
//     {
//      Int[k]=GetCol(Lft,k)*GetRow(Rgt,k);
//      Pos[k]=Eig(k,k);
//     }
////cout<<" Int\n"<<Int<<"\nPos\n"<<Pos<<"\n";
//  };
//
//// ==============================================
//// ============ Calculate Result Intencities ====
//// =============  univercial for All relax ======
//// ==============================================
//// H - turned
//double RIntTurn(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double x,double Wid1)
//  {
//   int N=Mat.Dim();
//   CVecCl Int(N),Pos(N);
//
//   GetVecIntTurn(Mat,Int,Pos,AmpL,AmpR);
//   double s=0; my_comp pos=my_comp(x,0);
//   for (int k1=1;k1<=N;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
//   return s/M_PI;
//  };
//   
//void RIntTurn(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double *x,double *y,double Wid1)
//  {
//   CVecCl Int,Pos;
//   GetVecIntTurn(Mat,Int,Pos,AmpL,AmpR);
//   int N=Pos.Dim();
//   double s;
//   my_comp pos;
////cout<<"\n Int "<<Int<<"\n Pos "<<Pos<<"\n";
//   for (int k=1;k<=x[0];k++)
//     {
//      s=0;pos=my_comp(x[k],0);
//      for (int k1=1;k1<=N;k1++) s+=my_imag(Int[k1]/(Pos[k1]-pos));
//      y[k]=s/M_PI;
//     }
//  };
//
//
//// =========================================================
//// =============  Form Full Relax   matrics ================
//// =========================================================
//
//
//// =============  Form Full Relax  Free1 - Free2 matrics ===
//void R2Free(CMatrCl &Mat,
//             double Teta1,double Phi1,double H01,double Qs1,double Eta1,
//             double Is1,double Wid1,double Alph_H_zx1,double Alph_H_xy1,
//             double Teta2,double Phi2,double H02,double Qs2,double Eta2,
//             double Is2,double Wid2,double Alph_H_zx2,double Alph_H_xy2,
//             double Tup,double Tdown)
//  {
//   CMatrCl H1(8),H2(8),H3(8),H4(8);
//   H2=H2*my_comp(0,0);H3=H2;
//   R01HQs(H1,Teta1, Phi1, H01, Qs1, Eta1, Is1, Wid1,Alph_H_zx1,Alph_H_xy1);
//   R01HQs(H4,Teta2, Phi2, H02, Qs2, Eta2, Is2, Wid2,Alph_H_zx2,Alph_H_xy2);
//   H2=H2+my_comp(0,Tdown);H1=H1-my_comp(0,Tdown);
//   H3=H3+my_comp(0,Tup);H4=H4-my_comp(0,Tup);
//   Mat=CombineMatr::Sum2Mat(H1,H2,H3,H4);
//  };
//
//// =============  Form Full RelaxN   matrics ===
//
//CMatrCl RnRelax(CMatrCl *Mat,double *time,int n)
// {
//  CMatrCl One8(8),Res(n*8);Res=Res*0;One8=One8*0;
//  CMatrCl Zer(n);Zer=Zer*0;
//  my_comp sum=0;
//  for (int k=1;k<=n;k++) sum=sum+my_comp(0,time[k]);
//    
//  for ( k=1;k<=n;k++)
//   {
////    Zer(k,k)=1;
////    Res=Res+DirectMultiplyOfMat(One8,Zer);Zer(k,k)=0;
//    for (int k1=1;k1<=n;k1++)
//     {
//	  if (k==k1) One8=Mat[k]-my_comp(0,time[k]*(n-1));
//      else One8=One8*0+my_comp(0,time[k]);
//	  Zer(k,k1)=1;
//      Res=Res+DirectMultiplyOfMat(One8,Zer);Zer(k,k1)=0;
////      One8=One8*0+my_comp(0,time[k]);Zer(k1,k)=1;
////      Res=Res+DirectMultiplyOfMat(One8,Zer);Zer(k1,k)=0;
//     }
//   }
//  return Res;
// };
//      
//// =============  Form Full RelaxN   matrics ===
//
////void RnRelax(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
////           double Is,double Wid,double *Time,int N)
//// {
////   CMatrCl *Ham=new CMatrCl[N+1];
////   for (int k=1;k<=N;k++) 
////     R01HQs(Ham[k],Teta, Phi, H, Qs, Eta, Is, Wid,360./N*(k-1) ,90);
////   Mat=RnRelax(&Ham[0],Time,N);
////   delete []Ham;
//// };
//void RnRelax(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
//           double Is,double Wid,VecCl &time,VecCl &field,VecCl &A_zx,VecCl &A_xy)
// {
//   int N=time[0];
//   CMatrCl *Ham=new CMatrCl[N+1];
//   for (int k=1;k<=N;k++) 
//     R01HQs(Ham[k],Teta, Phi, field[k], Qs, Eta, Is, Wid,A_zx[k] ,A_xy[k]);
//   Mat=RnRelax(&Ham[0],&time[0],N);
//   delete []Ham;
// };
//
//
//
//
//
//
//
//
//// =======================================================
//// ===========   Dummy to be deleted   ===================
//// =======================================================
//void HQsMat(CMatrCl &Mat,double Teta,double Phi,
//            double H,double Qs,double Eta)
//   {
//     //CMatrCl MatH,MatQs,Turn=TurnSpinors::Turn4Mat(Teta,Phi),ITurn;//=ITurn4Mat(Teta,Phi);
//     //ITurn=Conj(Transpon(Turn));
//     //HQsEiler_Mat(MatH,0,0,H,0,0);
//     //HQsEiler_Mat(MatQs,0,0,0,Qs,Eta);
//     //Mat=MatH+ITurn*MatQs*Turn;
//    HQsEiler_Mat(Mat,Teta,Phi,H,Qs,Eta);
//  };
//
//// =======================================================
//// ===========   Dummy to be deleted   ===================
//// =======================================================
//int HQsMatClc(double Teta,double Phi,double H,double Qs,double IS,double Eta,
//           double *Intencity,double *Position)
//  {
////   FormMessbInput();
//   CMatrCl Hamilt(4),EigenVect(4);
//   double *EigenVal=new double[5];EigenVal[0]=4;
//   HQsMat(Hamilt,Teta,Phi,H,Qs,Eta);
//   
//   CVecCl EVal;CMatrCl EigenVect1(4);int Ret=1;
//   URComeig(Hamilt,EigenVect1,EigenVect);          
//   for (int k=1;k<=4;k++) EigenVal[k]=my_real(Hamilt(k,k));EigenVect=Transpon(EigenVect);
////   int Ret=CReduceLQ(Hamilt,EigenVect,EigenVal,StndErr);
//
//   VecCl Amp2(4);Amp2[1]=0.25;Amp2[2]=1./6.;Amp2[3]=1./12.;Amp2[4]=0;
////cout<<" IntPos ? \n";
//   IntPos(EigenVect,EigenVal,Intencity,Position,H,IS,Amp2);
////cout<<" IntPos ! \n";
//   delete EigenVal;
//   return Ret;
//  };
//// =======================================================
//// ===========   Dummy to be deleted   ===================
//// =======================================================
////int HQsMatMonoClc(double Teta,double Phi,double H,double Qs,double IS,
////                  double Eta,double TetaH_View,double PhiH_View,
////                  double *Intencity,double *Position)
////  {
////   CMatrCl Hamilt(4),EigenVect(4);
////   double *EigenVal=new double[5];EigenVal[0]=4;
////   HQsMat(Hamilt,Teta,Phi,H,Qs,Eta);
////
////   CVecCl EVal;CMatrCl EigenVect1(4);int Ret=1;
////   URComeig(Hamilt,EigenVect1,EigenVect);          
////   for (int i=1;i<=4;i++) EigenVal[i]=my_real(Hamilt(i,i));EigenVect=Transpon(EigenVect);
//////   int Ret=CReduceLQ(Hamilt,EigenVect,EigenVal,StndErr);
////
////   VecCl Amp2(4);
////   double Cos=cos(TetaH_View*M_PI/180),Sin=sin(TetaH_View*M_PI/180);
////   double Cos2=sqr(Cos);
////   Amp2[1]=(1+Cos2)*0.75;Amp2[2]=1.-Cos2;Amp2[3]=(1+Cos2)*0.25;Amp2[4]=0;
////   IntPos(EigenVect,EigenVal,Intencity,Position,H,IS,Amp2);
//////  Add interference 
////   my_comp e1=my_comp(cos(PhiH_View*M_PI/180),sin(PhiH_View*M_PI/180)),e2=e1*e1;
////   my_comp C1=sqrt(3.)*Sin*Cos,C2=0.5*sqrt(3.)*sqr(Sin),C3=C1/sqrt(3.);
////   for (int k=1;k<=4;k++)
////     {
////      Intencity[k]+=my_real(   C1*e1*conj(EigenVect(1,k))*EigenVect(2,k)+
////                            C2*e2*conj(EigenVect(1,k))*EigenVect(3,k)-
////                            C3*e1*conj(EigenVect(2,k))*EigenVect(3,k));
//////cout<<" Int "<<Intencity[k];
////      Intencity[k+4]+=my_real(-C1*conj(e1*EigenVect(4,k))*EigenVect(3,k)+
////                            C2*conj(e2*EigenVect(4,k))*EigenVect(2,k)+
////                            C3*conj(e1*EigenVect(3,k))*EigenVect(2,k));
//////cout<<" Int "<<Intencity[k+4];
////     }
//////cout<<"\n";
////   for (k=1;k<=8;k++) if (Intencity[k]<0) cout<<" Monocryst. Negative.\n";
////   delete EigenVal;
////   return Ret;
////  };
//
//
//
//
//
////const char *HQsData::HamTypeNames[]={"Unknown","Z_H_Eiler","Z_H_Gamma",
////                                      "Z_Qs","Z_Free"};
////int HQsData::CheckType(const HamType &type,string Res) const
////{
////  Res.clear();
////  if ((type==HamType::Unknown) || (Type==HamType::Unknown) ||
////      (type==Type)) return 1;
////  Res.append(" var type is ").append(HamTypeNames[Type]).append(" have to be ")
////    .append(HamTypeNames[type]).append("\n");
////  return 0;
////}
//
