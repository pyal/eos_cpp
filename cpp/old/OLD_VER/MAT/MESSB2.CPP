#include "messb.h"
#include <malloc.h>

#define MCmu_e  2.2363       //This is old value
#define MCmu_g -3.9156       //This is old value

//#define MCmu_e  2.2843       // Value got from Phys Encyclop relation
//#define MCmu_g  -3.8333045254  // Value got from Phys Encyclop relation
   //   to use them - have to vary MCmms_tesl const

//#define MCmu_e -3.9156     //This set is done according to Phys Encyclop
//#define MCmu_g  2.2843

#define MCmms_tesl 84.5
#define Pi M_PI
//3.14159265358979

void FormMessbInput(double &H,double &Teta,double &Phi)
  {
   H/=MCmms_tesl;
   Teta*=Pi/180;
   Phi*=Pi/180;
  };
void HQsMat(CMatrCl &Mat,double Teta,double Phi,
            double H,double Qs,double Eta)
   {
    FormMessbInput(H,Teta,Phi);
    double mx=Qs/(3*sqrt(1+sqr(Eta)/3));
    double mb=-H*fabs(1/MCmu_g)*MCmu_e;
    double mc=mx*(1-sqr(sin(Teta))*(3-Eta*cos(2*Phi))/2);
    double md1=mx*sin(Teta)*cos(Teta)*(3-Eta*cos(2*Phi))/sqrt(3);
    double md2=mx*sin(Teta)*Eta*sin(2*Phi)/sqrt(3);
    my_comp md=my_comp(md1,md2);
    double me1=mx*(3+Eta*cos(2*Phi)-sqr(cos(Teta))*(3-Eta*cos(2*Phi)))/(2*sqrt(3));
    double me2=-mx*Eta*cos(Teta)*sin(2*Phi)/sqrt(3);
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
//cout<<Mat;
  };

void QsHMat(CMatrCl &Mat,double Teta,double Phi,
             double H,double Qs,double Eta,double IS)
   {
    Mat.Dim(4);
//cout<<" Teta "<<Teta<<" Phi "<<Phi<<" H "<<H<<"\nQs "<<Qs<<" Eta "<<Eta
//     <<" IS "<<IS<<"\n";
    FormMessbInput(H,Teta,Phi);
    double ma=fabs(H/MCmu_g)*MCmu_e;
    double mb=Qs/sqrt(1+sqr(Eta)/3)*0.5;
    double caii=cos(Teta)/2*ma;
    my_comp mul=sin(Teta)*my_comp(cos(Phi),sin(Phi));
    my_comp ca21=-sqrt(3)*0.5*mul*ma;
    my_comp ca32=-mul*ma;
    double  ca31=Eta*mb/sqrt(3);

    Mat(1,1)=mb-3*caii;        Mat(1,2)=conj(ca21);
    Mat(2,1)=ca21;             Mat(2,2)=-mb-caii;
    Mat(3,1)=ca31;             Mat(3,2)=ca32;
    Mat(4,1)=0;                Mat(4,2)=ca31;

    Mat(1,3)=ca31;             Mat(1,4)=0;
    Mat(2,3)=conj(ca32);       Mat(2,4)=ca31;
    Mat(3,3)=-mb+caii;         Mat(3,4)=conj(ca21);
    Mat(4,3)=ca21;             Mat(4,4)=mb+3*caii;

//cout<<"Exited Matrics\n"<<Mat;
//  Now Form Ground
    ma=fabs(H/MCmu_g)*MCmu_g;
    caii=-ma*cos(Teta)*0.5;ca21=-ma/ /*sqrt*/(2)*conj(mul);
    CMatrCl Gr(2);Gr(1,1)=caii;Gr(1,2)=conj(ca21);Gr(2,1)=ca21;Gr(2,2)=-caii;
    Gr=ExtendMat(Gr,4);
//cout<<"Ground Matrics\n"<<Gr;
    CMatrCl Zer(4);Zer=Mat*0;
    Mat=Sum2Mat(Mat,Zer,Zer,Mat);Mat=Mat-Gr+IS;
//cout<<"Full Matrics\n"<<Mat;
  };


// ===========================================================
// ======================  ===================================
// ===========================================================
void IntPos(CMatrCl &EigenVect,double *EigenVal,double *Intencity,
                               double *Position,double H,double IS,VecCl Amp2)
  {
   double shi=MCmu_g*H/(2*fabs(MCmu_g))/MCmms_tesl;
   my_comp s,s1;
   for (int k=1;k<=4;k++)
     {
      Position[k]=EigenVal[k]-shi+IS;
      Position[k+4]=EigenVal[k]+shi+IS;
     }
   for (k=1;k<=4;k++)
     {
      Intencity[k]=0;Intencity[k+4]=0;
      for (int k1=1;k1<=4;k1++) 
        {
         double tmp=norm(EigenVect(k1,k));
         Intencity[k]+=    Amp2[k1]*tmp;
         Intencity[k+4]+=Amp2[5-k1]*tmp;
        }
     }
  };
int HQsMatClc(double Teta,double Phi,double H,double Qs,double IS,double Eta,
           double *Intencity,double *Position)
  {
//   FormMessbInput();
   CMatrCl Hamilt(4),EigenVect(4);
   double *EigenVal=new double[5];EigenVal[0]=4;
   HQsMat(Hamilt,Teta,Phi,H,Qs,Eta);
   
   CVecCl EVal;CMatrCl EigenVect1(4);int Ret=1;
   URComeig(Hamilt,EigenVect1,EigenVect);          
   for (int k=1;k<=4;k++) EigenVal[k]=real(Hamilt(k,k));EigenVect=Transpon(EigenVect);
//   int Ret=CReduceLQ(Hamilt,EigenVect,EigenVal,StndErr);

   VecCl Amp2(4);Amp2[1]=0.25;Amp2[2]=1./6.;Amp2[3]=1./12.;Amp2[4]=0;
//cout<<" IntPos ? \n";
   IntPos(EigenVect,EigenVal,Intencity,Position,H,IS,Amp2);
//cout<<" IntPos ! \n";
   delete EigenVal;
   return Ret;
  };

int HQsMatMonoClc(double Teta,double Phi,double H,double Qs,double IS,
                  double Eta,double TetaH_View,double PhiH_View,
                  double *Intencity,double *Position)
  {
   CMatrCl Hamilt(4),EigenVect(4);
   double *EigenVal=new double[5];EigenVal[0]=4;
   HQsMat(Hamilt,Teta,Phi,H,Qs,Eta);

   CVecCl EVal;CMatrCl EigenVect1(4);int Ret=1;
   URComeig(Hamilt,EigenVect1,EigenVect);          
   for (int i=1;i<=4;i++) EigenVal[i]=real(Hamilt(i,i));EigenVect=Transpon(EigenVect);
//   int Ret=CReduceLQ(Hamilt,EigenVect,EigenVal,StndErr);

   VecCl Amp2(4);
   double Cos=cos(TetaH_View*Pi/180),Sin=sin(TetaH_View*Pi/180);
   double Cos2=sqr(Cos);
   Amp2[1]=(1+Cos2)*0.75;Amp2[2]=1.-Cos2;Amp2[3]=(1+Cos2)*0.25;Amp2[4]=0;
   IntPos(EigenVect,EigenVal,Intencity,Position,H,IS,Amp2);
//  Add interference 
   my_comp e1=my_comp(cos(PhiH_View*Pi/180),sin(PhiH_View*Pi/180)),e2=e1*e1;
   my_comp C1=sqrt(3)*Sin*Cos*e1,C2=0.5*sqrt(3)*sqr(Sin)*e2,C3=C1/sqrt(3);
   for (int k=1;k<=4;k++)
     {
      Intencity[k]+=real(   C1*conj(EigenVect(1,k))*EigenVect(2,k)+
                            C2*conj(EigenVect(1,k))*EigenVect(3,k)-
                            C3*conj(EigenVect(2,k))*EigenVect(3,k));
//cout<<" Int "<<Intencity[k];
      Intencity[k+4]+=real(-C1*conj(EigenVect(4,k))*EigenVect(3,k)+
                            C2*conj(EigenVect(4,k))*EigenVect(2,k)+
                            C3*conj(EigenVect(3,k))*EigenVect(2,k));
//cout<<" Int "<<Intencity[k+4];
     }
//cout<<"\n";
   for (k=1;k<=8;k++) if (Intencity[k]<0) cout<<" Monocryst. Negative.\n";
   delete EigenVal;
   return Ret;
  };
   
double HQsInt(double *Intencity,double *Position,double Wid,double x)
  {
   double Res=0;
   double K=Wid/(2*Pi);
   for (int k=1;k<=8;k++) Res+=Intencity[k]/(sqr(Wid/2)+sqr(x-Position[k]));
   return Res*K;
  };
      
void HQsInt(double *Intencity,double *Position,double Wid,double *x,double *y)
  {
   double s,p,W=sqr(Wid/2);
   double K=Wid/(2*Pi);
//VecCl Tmp;Tmp.Ptr=Position;cout<<" Pos "<<Tmp;Tmp.Ptr=Intencity;cout<<"\nInt "<<Tmp<<"\n";Tmp.Ptr=NULL;
   for (int k=1;k<=x[0];k++)
     {
      p=x[k];s=0;
      for (int k1=1;k1<=8;k1++) s+=Intencity[k1]/(W+sqr(Position[k1]-p));
      y[k]=s*K;
     }
  };
// ==============================================
// =============  Relaxation case   =============
// ==============================================

// ===========================================================
// ========= Calculating AmpVec - tors  ======================
// ===========================================================

//  H//z

CVecCl R0HQsAmpVec()
  {
   CVecCl Amp(8);
   Amp[1]=my_comp(-0.5,0);       Amp[2]=my_comp(1/sqrt(6),0);
   Amp[3]=my_comp(-1/sqrt(12),0);Amp[4]=my_comp(0,0);
   for (int k=1;k<=4;k++) Amp[9-k]=-Amp[k];
   return Amp;
  };
CMatrCl R0HQsAmpVec_()
  {
   CMatrCl Ret(8,3);Ret=Ret*0;
   my_comp d3=my_comp(sqrt(1./3),0),d2=my_comp(2*sqrt(1./3),0);
   Ret(1,1)=1; Ret(3,1)=d3;Ret(8,1)=1;Ret(6,1)=d3;
   Ret(1,2)=1; Ret(3,2)=-d3;Ret(8,2)=-1;Ret(6,2)=d3;
   Ret(2,3)=d2; Ret(7,3)=d2;
//    my_comp d1=my_comp(0.5,0),d12=my_comp(sqrt(1./12),0),d6=my_comp(sqrt(1./6),0);
//    Ret(1,1)=-d1; Ret(6,1)=d12;
//    Ret(3,2)=-d12;Ret(8,2)=d1;
//    Ret(2,3)= d6;Ret(7,3)=-d6;
   Ret=Ret*sqrt(1./8);
   return Ret;
  }; 

void R2HQsAmpVec(double t1,double t2,CMatrCl &AmpL,CMatrCl &AmpR)
  {
   double p1,p2,s=fabs(t1)+fabs(t2);
   if (s>MathZer) {p1=fabs(t2)/s;p2=fabs(t1)/s;}
   else {p1=0.5;p2=0.5;}
   CMatrCl H(8,3);H=R0HQsAmpVec_();
   CMatrCl Zer(8,3);Zer=Zer*0;
   AmpR=Sum2Mat(H,H,H,H); 
   AmpL=Sum2Mat(H*p1,Zer ,Zer ,H*p2);
  };
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

// ==============================================
// ========= Forms  relaxing Ham matrics. =======
// ==============================================

void R0HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid)
  {
   my_comp shi=my_comp(MCmu_g*H/(2*fabs(MCmu_g))/MCmms_tesl,0);
   my_comp sh=my_comp(Is,-Wid/2);
   CMatrCl H1(4),H2(4),H3(4),H4(4),Ham(4);
   HQsMat(Ham,Teta,Phi,H,Qs,Eta);
   H1=Ham+(sh-shi);H4=Ham+(shi+sh);
   H2=H2*my_comp(0,0);H3=H2;
//  Formed SubMatrics of one Full HQs matrics
   Mat=Sum2Mat(H1,H2,H3,H4);
  };

//  Teta - Phi - Qs main vectors.  
//  Teta_H Phi_H H  main vectors. 
//  Constructing H=HamQs+HamH
void R01HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Teta_H,double Phi_H)
  {
   CMatrCl H1(8);
   R0HQs(H1,Teta, Phi, 0, Qs, Eta, Is, Wid);
   QsHMat(Mat,Teta_H,Phi_H, H,0,0,0);
//   Mat=FormHHam(Teta_H,Phi_H,MCmu_g/MCmu_e)*
//                          (H*fabs(1/MCmu_g)*MCmu_e/MCmms_tesl);
   Mat=Mat+H1;
  };


// ==============================================
// =============  Calculate intecity  matr ======
// =============  univercial for All relax ======
// ==============================================

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

//  Adds turn -  H (Teta,Phi) z
void GetVecIntTurn(CMatrCl &Mat,CVecCl &Int,CVecCl &Pos,
                                      CMatrCl &AmpL,CMatrCl &AmpR)
  {
   int N=Mat.Dim(),k;
   CVecCl VL(N),VecL(N),VecR(N),VR(N);
   CMatrCl Eig(N),Lft(N),Rgt(N);Eig=Mat;
   URComeig(Eig,Lft,Rgt);
   Rgt=(Rgt*AmpR);
   Lft=(Conj(Transpon(AmpL))*Lft);
   Pos.Dim(N);Int.Dim(N);
   CVecCl tmp;
//   int Base;Lft.DimMN(Base,N);
   for (k=1;k<=N;k++) 
     {
      Int[k]=CMatrCl::GetCol(Lft,k)*CMatrCl::GetRow(Rgt,k);
//      tmp=GetCol(Lft,k)-Conj(GetRow(Rgt,k));Int[k]=Conj(tmp)*tmp;
//      tmp=GetRow(Rgt,k);Int[k]=Conj(tmp)*tmp;
      Pos[k]=Eig(k,k);
     }
   for (k=1;k<=N;k++) 
//     if ( (fabs(imag(Int[k]))>1e-3) || (real(Int[k])<-1e-3) ) 
     if ( imag(Int[k]/(Pos[k]-real(Pos[k])))<-1e-1 )
       {cout<<" Error. Very Bad;\nPos "<<Pos<<"\nInt "<<Int<<"\n";break;}
//cout<<" \nPos "<<Pos<<"\nInt "<<Int<<"\n";
  };

// ==============================================
// ============ Calculate Result Intencities ====
// =============  univercial for All relax ======
// ==============================================
/*
//H - not turned
double RInt(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double x,double Wid1)
  {
   int N=Mat.Dim();
   CVecCl Int(N),Pos(N);

   GetVecInt(Mat,Int,Pos,AmpL,AmpR);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=N;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      for (int k1=1;k1<=N;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s*2/Wid1;
     }
  };

*/
// H - turned
double RIntTurn(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double x,double Wid1)
  {
   int N=Mat.Dim();
   CVecCl Int(N),Pos(N);

   GetVecIntTurn(Mat,Int,Pos,AmpL,AmpR);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=N;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
   return s/Pi;
  };
   
void RIntTurn(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double *x,double *y,double Wid1)
  {
   CVecCl Int,Pos;
   GetVecIntTurn(Mat,Int,Pos,AmpL,AmpR);
   int N=Pos.Dim();
   double s;
   my_comp pos;
//cout<<"\n Int "<<Int<<"\n Pos "<<Pos<<"\n";
   for (int k=1;k<=x[0];k++)
     {
      s=0;pos=my_comp(x[k],0);
      for (int k1=1;k1<=N;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
      y[k]=s/Pi;
     }
  };


// =========================================================
// =============  Form Full Relax   matrics ================
// =========================================================


// =============  Form Full Relax  Free1 - Free2 matrics ===
void R2Free(CMatrCl &Mat,
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
   Mat=Sum2Mat(H1,H2,H3,H4);
  };

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

// ==============================================
// ====== External field ========================
// ==============================================
VecCl FormHVec(double H,double H_zx,double H_xy)
  {
   VecCl Ret(3);
   double Teta=H_zx*Pi/180,Phi=H_xy*Pi/180;
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
   if (H_xy<StndErr) {Hs_xy=0;Hs_zx*=180/Pi; return ;}
   Hs_xy=acos(Hi[2]/H_xy);
//   if (Hi[2]<0) Hs_xy=Pi-Hs_xy; 
   if (Hi[3]<0) { Hs_zx=2*Pi-Hs_zx;Hs_xy=Pi-Hs_xy; }
   Hs_xy*=180/Pi;Hs_zx*=180/Pi;
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
   Y[1]=0;Y[3]=cos(H_xy*Pi/180);Y[2]=-sin(H_xy*Pi/180);
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
   double  d1=sqrt((sqr(cos(Teta_V*Pi/180))+1)/2),d2=fabs(sin(Teta_V*Pi/180));
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
   double Coef=Pi/12,Hs, Hs_zx,Hs_xy;
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
   Coef=Pi/12;
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
   

void R2CircleInt(double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Teta_H,double Phi_H, 
                    double *x,double *y,double TCircle)
  {
   double a;
   a=Teta;a=Phi;a=H;a=Qs;a=Eta;a=Is;a=Wid;a=Teta_H;a=Phi_H;a=x[1];a=y[1];
   a=TCircle;
  };

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
   for (int i=1;i<=x[0];i++) y[i]=real(result[i]);
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
   for (int i=1;i<=x[0];i++) y[i]=real(result[i]);
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
   for (int k1=1;k1<=48;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      for (int k1=1;k1<=48;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
   for (int k1=1;k1<=8;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      for (int k1=1;k1<=8;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
   Mat=Sum2Mat(H1,H2,H3,H4);
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
   for (int k1=1;k1<=16;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      for (int k1=1;k1<=16;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
   for (int k1=1;k1<=24;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      for (int k1=1;k1<=24;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
   Mat=Sum2Mat(H1,H2,H3,H4);
  };
double RHQs0QsInt(CMatrCl &Mat,double x,double Tup,double Tdown,double Wid1)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec(Tup,Tdown);
   CVecCl Int(16),Pos(16);
   GetVecInt(Mat,Int,Pos,Amp);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=16;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      for (int k1=1;k1<=16;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
   Mat=Sum2Mat(H1,H2,H3,H4);
  };
double RFreeInt(CMatrCl &Mat,double x,double Tup,double Tdown,double Wid1)
  {
   CVecCl Amp(16);Amp=RHQsAmpVec(Tup,Tdown);
   CVecCl Int(16),Pos(16);
   GetVecInt(Mat,Int,Pos,Amp);
   double s=0; my_comp pos=my_comp(x,0);
   for (int k1=1;k1<=16;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      for (int k1=1;k1<=16;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
   for (int k1=1;k1<=N;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      for (int k1=1;k1<=N;k1++) s+=imag(Int[k1]/(Pos[k1]-pos));
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
      ret+=imag(Amp1*Res);
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



