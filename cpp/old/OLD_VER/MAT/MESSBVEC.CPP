#include "messb.h"
#define MCmu_g -3.9156
#define MCmu_e  2.2363
#define MCmms_tesl 84.5
#define Pi 3.141

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
    double me1=mx*(3+Eta*cos(2*Phi)-sqr(cos(Teta))*(3-Eta*cos(2*Phi)))/(2*sqrt(3));
    double me2=-mx*Eta*cos(Teta)*sin(2*Phi)/sqrt(3);
    Mat(1,1)=my_comp(-mb+mc,0);
    Mat(2,1)=my_comp(md1,-md2);
    Mat(3,1)=my_comp(me1,-me2);
    Mat(4,1)=my_comp(0,0);

    Mat(1,2)=my_comp(md1,md2);
    Mat(2,2)=my_comp(-mb/3-mc,0);
    Mat(3,2)=my_comp(0,0);
    Mat(4,2)=my_comp(me1,-me2);

    Mat(1,3)=my_comp(me1,me2);
    Mat(2,3)=my_comp(0,0);
    Mat(3,3)=my_comp(mb/3-mc,0);
    Mat(4,3)=my_comp(-md1,md2);

    Mat(1,4)=my_comp(0,0);
    Mat(2,4)=my_comp(me1,me2);
    Mat(3,4)=my_comp(-md1,-md2);
    Mat(4,4)=my_comp(mb+mc,0);

    Mat=Mat*1.5;
//cout<<Mat;
  };

CVecCl R0HQsAmpVec()
  {
   CVecCl Amp(8);
   Amp[1]=my_comp(-0.5,0);       Amp[2]=my_comp(1/sqrt(6),0);
   Amp[3]=my_comp(-1/sqrt(12),0);Amp[4]=my_comp(0,0);
   for (int k=1;k<=4;k++) Amp[9-k]=-Amp[k];
   return Amp;
  };

void IntPos(CMatrCl &EigenVect,double *EigenVal,double *Intencity,
                               double *Position,double H,double IS)
  {
   double shi=MCmu_g*H/(2*fabs(MCmu_g))/MCmms_tesl;
   CVecCl Amp(8);Amp=R0HQsAmpVec();
//   double *Amp=new double[5];Amp[1]=0.25;Amp[2]=(double)1/6;
//   Amp[3]=(double)1/12;Amp[4]=0;
          
   my_comp s,s1;
   for (int k=1;k<=4;k++)
     {
      Position[k]=EigenVal[k]-shi+IS;
      Position[k+4]=EigenVal[k]+shi+IS;
     }
   for (k=1;k<=4;k++)
     {
//      Intencity[k]=0;Intencity[k+4]=0;
      s=my_comp(0);s1=my_comp(0);
      for (int k1=1;k1<=4;k1++) 
        {
//         Intencity[k]+=    Amp[k1]*(sqr(real(EigenVect(k1,k)))+
//                                    sqr(imag(EigenVect(k1,k))));
//         Intencity[k+4]+=Amp[5-k1]*(sqr(real(EigenVect(k1,k)))+
//                                    sqr(imag(EigenVect(k1,k))));
//         Intencity[k]  +=Amp[k1]  *abs(EigenVect(k1,k));
//         Intencity[k+4]+=Amp[k1+4]*abs(EigenVect(k1,k));
         s+=Amp[k1]  *(EigenVect(k1,k));
         s1+=Amp[k1+4]*(EigenVect(k1,k));
//cout<<Intencity[k]<<"  ";
        }
//cout<<"\n";
      Intencity[k]=sqr(real(s))+sqr(imag(s));
      Intencity[k+4]=sqr(real(s1))+sqr(imag(s1));
     }
//VecCl PV(Intencity),IV(Position);
//cout<<" Position "<<IV<<" Intencity "<<PV;
//PV.Ptr=NULL;IV.Ptr=NULL;
//   delete Amp;
  };
int HQsMatClc(double Teta,double Phi,double H,double Qs,double IS,double Eta,
           double *Intencity,double *Position)
  {
//   FormMessbInput();
   CMatrCl Hamilt(4),EigenVect(4);
   double *EigenVal=new double[5];EigenVal[0]=4;
   HQsMat(Hamilt,Teta,Phi,H,Qs,Eta);
   int Ret=CReduceLQ(Hamilt,EigenVect,EigenVal,StndErr);
//VecCl V(EigenVal);
//cout<<" Ret = "<<Ret<<" Hamilt\n"<<Hamilt<<"EigenVal  "<<V
//<<"EigenVect\n"<<EigenVect;V.Ptr=NULL;
//cout<<EigenVect;
   IntPos(EigenVect,EigenVal,Intencity,Position,H,IS);
   delete EigenVal;
   return Ret;
  };

double HQsInt(double *Intencity,double *Position,double Wid,double x)
  {
   double Res=0;
   for (int k=1;k<=8;k++) Res+=Intencity[k]/(sqr(Wid/2)+sqr(x-Position[k]));
   return Res;
  };
      
void HQsInt(double *Intencity,double *Position,double Wid,double *x,double *y)
  {
   double s,p,W=sqr(Wid/2);
   for (int k=1;k<=x[0];k++)
     {
      p=x[k];s=0;
      for (int k1=1;k1<=8;k1++) s+=Intencity[k1]/(W+sqr(Position[k1]-p));
      y[k]=s;
     }
  };

// ==============================================
// =============  Relaxation case   =============
// ==============================================

// ==============================================
// =============  Form Relax   matrics  =========
// ==============================================

CMatrCl RHqSumMat(CMatrCl &H1,CMatrCl &H2,CMatrCl &H3,CMatrCl &H4)
  {
   int N=H1.Dim();
   CMatrCl Ret(N*2);
   for (int k=1;k<=N;k++)
     {
      for (int k1=1;k1<=N;k1++)
        {
         Ret(k,k1)=H1(k,k1);Ret(k,k1+N)=H2(k,k1);
         Ret(k+N,k1)=H3(k,k1);Ret(k+N,k1+N)=H4(k,k1);
        }
     }
   return Ret;
  };

void R0HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid)
  {
   my_comp shi=my_comp(MCmu_g*H/(2*fabs(MCmu_g))/MCmms_tesl,0);
   my_comp sh=my_comp(Is,-Wid/2);
   CMatrCl H1(4),H2(4),H3(4),H4(4),Ham(4);
   HQsMat(Ham,Teta,Phi,H,Qs,Eta);H1=Ham+(sh-shi);
   H4=Ham+(shi+sh);
   H2=H2*my_comp(0,0);H3=H2;
//  Formed SubMatrics of one Full HQs matrics
   Mat=RHqSumMat(H1,H2,H3,H4);
  };


// ==============================================
// =============  Calculate intecity    =========
// ==============================================





double R0HQsInt(CMatrCl &Mat,double x)
  {
   CMatrCl Tmp(8);Tmp=Mat-my_comp(x,0);
   CVecCl Amp(8),Res(8);Amp=R0HQsAmpVec();
   CSolveAxb(Tmp,Amp,Res);
   return 5*imag(Amp*Res);
  };
void R0HQsInt(CMatrCl &Mat,double *x,double *y)
  {
   CVecCl Amp(8),Res(8);Amp=R0HQsAmpVec();
   CMatrCl Tmp(8);
   for (int k=1;k<=x[0];k++)
     {
      Tmp=Mat-my_comp(x[k],0);
      CSolveAxb(Tmp,Amp,Res);
//if (k==100) {cout<<Tmp<<Res;ChRead();}
      y[k]=5*imag(Amp*Res);
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
   H2=H2-my_comp(0,Tup);H1=H1+my_comp(0,Tup);
   H3=H3-my_comp(0,Tdown);H4=H4+my_comp(0,Tdown);
   Mat=RHqSumMat(H1,H2,H3,H4);
//cout<<Mat;
  };
   

CVecCl RHQsAmpVec()
  {
   CVecCl A(8);A=R0HQsAmpVec();
   CVecCl Ret(16);
   for (int k=1;k<=8;k++) {Ret[k]=A[k];Ret[k+8]=A[k];}
   return Ret;
  }
// ==============================================
// =============  Calculate intecity    =========
// ==============================================

double RHQsInt(CMatrCl &Mat,double x)
  {
   CMatrCl Tmp(16);Tmp=Mat-my_comp(x,0);
   CVecCl Amp(16),Res(16);Amp=RHQsAmpVec();
   CSolveAxb(Tmp,Amp,Res);
   return 2.5*imag(Amp*Res);
  };
   
void RHQsInt(CMatrCl &Mat,double *x,double *y)
  {
   CVecCl Amp(16),Res(16);Amp=RHQsAmpVec();
   CMatrCl Tmp(16);
   for (int k=1;k<=x[0];k++)
     {
      Tmp=Mat-my_comp(x[k],0);
      CSolveAxb(Tmp,Amp,Res);
//if (k==10) {cout<<Tmp<<"\n"<<Res<<"\n"<<Amp;}
      y[k]=2.5*imag(Amp*Res);
     }
  };
