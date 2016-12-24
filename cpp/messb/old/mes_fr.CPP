#include "mes_fr.h"




//
// ======================= Eig Var ============================
//
#define SameAngle 5.


int WorkPos(double Phi)
 {
   int Work= (Phi-int(Phi/360.)*360)/SameAngle;
   if (Work==0) Work=360./SameAngle;
   return Work;
 };
void FormAmp(CMatrCl &AmpR1,CMatrCl &AmpL1,CMatrCl &AmpR2,CMatrCl &AmpL2,
             CMatrCl &AmpR3,CMatrCl &AmpL3);

struct IntHam
 {
  CMatrCl Mat3,Mat1,Mat3Eig,Mat3L,Mat3R,Mat1Eig,Mat1L,Mat1R;
 };
void FormIntHam(IntHam *par,double H,double Qs,double Eta,double Is)
 {
  CMatrCl Mat;
  for (int k=1;k<=360/SameAngle;k++)
   {
    QsHMat(Mat,90,SameAngle*k,H, Qs, Eta, Is,&(par[k].Mat3),&(par[k].Mat1));
    par[k].Mat1=Transpon(par[k].Mat1);  // UPS!!!!!!
    URComeig(par[k].Mat3,par[k].Mat3L,par[k].Mat3R);
    URComeig(par[k].Mat1,par[k].Mat1L,par[k].Mat1R);

   }
 };
void GetVecIntTurn_Cur(CVecCl &Int,CVecCl &Pos,
                       CMatrCl &AmpL,CMatrCl &AmpR,
                       IntHam *par,double Phi,int &Work)
  {
   int N3=par[1].Mat3.Dim(),N1=par[1].Mat1.Dim(),k3,k1;
   Work= (Phi-int(Phi/360)*360)/SameAngle;
//   if (Work==0) Work=360/SameAngle;
   if (Work<=0) Work=360/SameAngle+Work;
   CMatrCl &Lft3=par[Work].Mat3L,&Rgt3=par[Work].Mat3R,&Eig3=par[Work].Mat3;
   CMatrCl &Lft1=par[Work].Mat1L,&Rgt1=par[Work].Mat1R,&Eig1=par[Work].Mat1;

   CMatrCl AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3;
   FormAmp(AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3);
   Pos.Dim(N3*N1);Int.Dim(N3*N1);
   CVecCl L3,L1,R3,R1,RotL3,RotR1;
   int k;
   for (k3=1;k3<=N3;k3++) 
     {
     for (k1=1;k1<=N1;k1++) 
      {
       k=(k1-1)*N3+k3;
       L3=CMatrCl::GetCol(Lft3,k3);L1=CMatrCl::GetCol(Lft1,k1);
	   R3=CMatrCl::GetRow(Rgt3,k3);R1=CMatrCl::GetRow(Rgt1,k1);
       RotL3=R3*AmpL;RotR1=AmpR*L1;
       Int[k]=
              ( ( (AmpR1*L3)*R1 )*((RotL3*AmpL1)*RotR1) )+
              ( ( (AmpR2*L3)*R1 )*((RotL3*AmpL2)*RotR1) )+
              ( ( (AmpR3*L3)*R1 )*((RotL3*AmpL3)*RotR1) );
        
       Pos[k]=Eig3(k3,k3)-Eig1(k1,k1);
      }
     }
   Int=Int/M_PI;
  };

void FormIntArr_Cur(MessRelaxClcPar *RelaxClc,MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is)
 {
  int N=8,Work;//Mat.Dim();
  CMatrCl Mat(N),TimeShiftLeft,TimeShiftRight;
//  CMatrCl Mat3,Mat3Eig,Mat3L,Mat3R,     Mat1,Mat1Eig,Mat1L,Mat1R;
  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;
  TimeShiftLeft=Uni3;TimeShiftRight=Uni1;
  IntHam *par=new IntHam[360./SameAngle+1];
  FormIntHam(par, H, Qs, Eta, Is);
  CVecCl Int(N),Pos(N);
//  RelaxPar.Times[0]=0;
  double PrevTime=RelaxPar.BegTime;
  RelaxClc[0].EndTime=PrevTime;
  
  for (int k=1;k<=RelaxPar.NumHam;k++)
   {
    CMatrCl Eig;
//    Mat1=Transpon(Mat1);  // UPS!!!!!!
    GetVecIntTurn_Cur(Int,Pos,TimeShiftLeft,TimeShiftRight,
                      par,RelaxPar.Phi[k],Work);

    RelaxClc[k].RelInt=Int;
    RelaxClc[k].Freq=Pos;

    Eig=par[Work].Mat3;
    for (int i=1;i<=par[Work].Mat3.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,-my_real(par[Work].Mat3(i,i))*
//t*
                             (RelaxPar.Times[k]-PrevTime)));
//    Mat3=Mat3L*Eig*Mat3R;
    TimeShiftLeft= par[Work].Mat3L*Eig*par[Work].Mat3R*TimeShiftLeft;
    Eig=par[Work].Mat1;
    for (    i=1;i<=par[Work].Mat1.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,my_real(par[Work].Mat1(i,i))*
//t*
                             (RelaxPar.Times[k]-PrevTime)));
//    Mat1=Mat1L*Eig*Mat1R;
    TimeShiftRight=TimeShiftRight*par[Work].Mat1L*Eig*par[Work].Mat1R;

    PrevTime=RelaxPar.Times[k];
    RelaxClc[k].EndTime=RelaxPar.Times[k];
   }
  delete []par;
 };


// ==================================================================
// ===================  Version without eig acceleration ============
// ==================================================================

void FormAmp(CMatrCl &AmpR1,CMatrCl &AmpL1,CMatrCl &AmpR2,CMatrCl &AmpL2,
             CMatrCl &AmpR3,CMatrCl &AmpL3)
 {
  AmpR1.SetMatr(2,4);AmpR1=AmpR1*0;AmpR2=AmpR1;AmpR3=AmpR1;
  my_comp d3=my_comp(sqrt(2./(3*8)),0),d2=my_comp(sqrt(4./(3*8)),0),
          d1=my_comp(sqrt(6./(3*8)),0);

  AmpR1(1,1)=d1;AmpR1(2,2)=d3;
  AmpR2(1,3)=d3;AmpR2(2,4)=d1;
  AmpR3(1,2)=d2;AmpR3(2,3)=d2;
  AmpL1=Transpon(AmpR1);AmpL2=Transpon(AmpR2);AmpL3=Transpon(AmpR3);
 };

  


void GetVecIntTurn_(CVecCl &Int,CVecCl &Pos,
                   CMatrCl &Mat3,CMatrCl &Mat1,
                   CMatrCl &AmpL,CMatrCl &AmpR,
                   CMatrCl *retEig3,CMatrCl *retLft3,CMatrCl *retRgt3,
                   CMatrCl *retEig1,CMatrCl *retLft1,CMatrCl *retRgt1,
                   double ShiftAngle)
  {
   int N3=Mat3.Dim(),N1=Mat1.Dim(),k3,k1;
   CMatrCl Eig3(N3),Lft3(N3),Rgt3(N3);Eig3=Mat3;
   CMatrCl Eig1(N1),Lft1(N1),Rgt1(N1);Eig1=Mat1;
//   my_comp dPhi=exp(my_comp(0,-ShiftAngle));
//   my_comp dPhi_=conj(dPhi);
   URComeig(Eig3,Lft3,Rgt3);URComeig(Eig1,Lft1,Rgt1);
   if (retEig3!=NULL) 
    { 
     *retEig3=Eig3;*retLft3=Lft3;*retRgt3=Rgt3;
     *retEig1=Eig1;*retLft1=Lft1;*retRgt1=Rgt1;
    }

   CMatrCl AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3;FormAmp(AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3);
   Pos.Dim(N3*N1);Int.Dim(N3*N1);
   CVecCl L3,L1,R3,R1,RotL3,RotR1;
   
   int k;
   for (k3=1;k3<=N3;k3++) 
     {
     for (k1=1;k1<=N1;k1++) 
      {
       k=(k1-1)*N3+k3;
       L3=CMatrCl::GetCol(Lft3,k3);L1=CMatrCl::GetCol(Lft1,k1);
       R3=CMatrCl::GetRow(Rgt3,k3);R1=CMatrCl::GetRow(Rgt1,k1);
       RotL3=R3*AmpL;RotR1=AmpR*L1;
       Int[k]=
              ( ( (AmpR1*L3)*R1 )*((RotL3*AmpL1)*RotR1) )+//*dPhi+
              ( ( (AmpR2*L3)*R1 )*((RotL3*AmpL2)*RotR1) )+//*dPhi_+
              ( ( (AmpR3*L3)*R1 )*((RotL3*AmpL3)*RotR1) );
        
       Pos[k]=Eig3(k3,k3)-Eig1(k1,k1);
      }
     }
   Int=Int/M_PI;
  };
    
void FormIntArr(MessRelaxClcPar *RelaxClc,MessRelaxInpPar &RelaxPar,
//                    double H,double Qs,double Eta,double Is,double t)
                    double H,double Qs,double Eta,double Is)
 {
  int N=8;//Mat.Dim();
  CMatrCl Mat(N),TimeShiftLeft,TimeShiftRight;
  CMatrCl Mat3,Mat3Eig,Mat3L,Mat3R,     Mat1,Mat1Eig,Mat1L,Mat1R;
  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;
  TimeShiftLeft=Uni3;TimeShiftRight=Uni1;

  CVecCl Int(N),Pos(N);
//  RelaxPar.Times[0]=0;
  double PrevTime=RelaxPar.BegTime;
  RelaxClc[0].EndTime=PrevTime;
  
//  RelaxClc[0].EndTime=0;
  for (int k=1;k<=RelaxPar.NumHam;k++)
   {
    QsHMat(Mat,RelaxPar.Teta[k],RelaxPar.Phi[k],H, Qs, Eta, Is,&Mat3,&Mat1);

    CMatrCl Eig,Lft,Rgt;
    Mat1=Transpon(Mat1);  // UPS!!!!!!
    double dPhi=RelaxPar.Teta[k]-RelaxPar.Teta[1];
    GetVecIntTurn_(Int,Pos,Mat3,Mat1,TimeShiftLeft,TimeShiftRight,
                   &Mat3Eig,&Mat3L,&Mat3R,&Mat1Eig,&Mat1L,&Mat1R,dPhi);

    RelaxClc[k].RelInt=Int;
    RelaxClc[k].Freq=Pos;

    Eig=Mat3Eig;
    for (int i=1;i<=Mat3.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,-my_real(Mat3Eig(i,i))*
//t*
                             (RelaxPar.Times[k]-PrevTime)));
    Mat3=Mat3L*Eig*Mat3R;
    TimeShiftLeft= Mat3*TimeShiftLeft;
    Eig=Mat1Eig;
    for (    i=1;i<=Mat1.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,my_real(Mat1Eig(i,i))*
//t*
                             (RelaxPar.Times[k]-PrevTime)));
    Mat1=Mat1L*Eig*Mat1R;
    TimeShiftRight=TimeShiftRight*Mat1;
cout<<TimeShiftLeft<<TimeShiftRight<<"\n\n\n";

    PrevTime=RelaxPar.Times[k];
    RelaxClc[k].EndTime=RelaxPar.Times[k];
   }
 };
void SetRelaxPar(MessRelaxInpPar &dest,MessRelaxInpPar &source,int i,int k,double Phi)
 {dest.Teta[i]=source.Teta[k];dest.Phi[i]=Phi;dest.Times[i]=source.Times[k];};
void MakeDoubleTime(MessRelaxInpPar &RelaxPar)
 {
  MessRelaxInpPar shor=RelaxPar;
  int i=1,W=WorkPos(RelaxPar.Phi[1]);
  shor.Phi[1]=W*SameAngle;
  for (int k=2;k<=RelaxPar.NumHam-1;k++)
   {
    if (WorkPos(RelaxPar.Phi[k])!=W)
     {i++;W=WorkPos(RelaxPar.Phi[k]);SetRelaxPar(shor,RelaxPar,i,k,W*SameAngle);}
   }
  if (RelaxPar.NumHam>=2) 
   {i++;SetRelaxPar(shor,RelaxPar,i,RelaxPar.NumHam,WorkPos(RelaxPar.Phi[RelaxPar.NumHam])*SameAngle);}
  shor.NumHam=i;
  shor.Phi[0]=i;shor.Teta[0]=i;shor.Times[0]=i;RelaxPar=shor;
 };

void RelaxOneHamInt(MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is,double Wid,
                    double *x,double *y)
 {
//  MakeDoubleTime(RelaxPar);
  MessRelaxClcPar *RelaxClc=new MessRelaxClcPar[RelaxPar.NumHam+1];
  Wid=Wid/2;
  for (int u=1;u<=x[0];u++) y[u]=0;
//for (int o=1;o<=1;o+=2)
//{
//  FormIntArr(RelaxClc,RelaxPar,H, Qs, Eta, Is,o);
//  FormIntArr_Cur(RelaxClc,RelaxPar,H, Qs, Eta, Is);
  FormIntArr(RelaxClc,RelaxPar,H, Qs, Eta, Is);
  my_comp res;
  double t0,t1;
  int N=my_real(RelaxClc[1].RelInt[0]);
  for (int i=1;i<=x[0];i++)
   {
    res=0;
    for (int k=1;k<=RelaxPar.NumHam;k++)
//    for (int k=RelaxPar.NumHam/2;k<=RelaxPar.NumHam;k++)
//    for (int k=1;k<=RelaxPar.NumHam/2;k++)
     {
//if (i==1) {cout<<" Freq \n"<<RelaxClc[k].Freq<<"\n"<<" Int \n"<<RelaxClc[k].RelInt<<"\n";}
      t1=RelaxClc[k].EndTime;t0=RelaxClc[k-1].EndTime;
      for (int k1=1;k1<=N;k1++)
       {
        
        my_comp freq=my_comp(-Wid,x[i]-my_real(RelaxClc[k].Freq[k1]));
//double f=my_real(RelaxClc[k].Freq[k1]);
// my_comp freq=my_comp(-Wid*o,x[i]-f);
//        if (t1<0) freq=conj(freq);//-2*my_real(freq);//
        res=res+
         RelaxClc[k].RelInt[k1]
// *o
          /freq*
            ( exp(my_comp( -fabs(Wid*t1),my_imag(freq)*(t1-t0)+x[i]*t0 ))-
              exp(my_comp(-fabs(Wid*t0),x[i]*t0)) );
//            ( exp(my_comp( -fabs(Wid*t1),-f*(t1-t0)*o+x[i]*t1*o ))-
//              exp(my_comp(-fabs(Wid*t0),x[i]*t0*o)) );
       
       }
     }
    y[i]+=my_real(res);
//cout<<i<<"  "<<my_imag(res)<<"  "<<my_real(res)<<"\n";
   }
//}
  delete []RelaxClc;
 };

///
// ======================= Eig Var ============================
//
#include "kashub.h"
VecCl SumTimes;
void GenerateRelaxPar(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver)
 {
  
  srand( (unsigned)time( NULL ) );
  int N=Aver.NumTimes,k;RelaxPar.ResetDim(N);
  double TimeStp=Aver.EndTime/N;
  double PhiStp=TimeStp/Aver.MeanRotTime*360;
  double Teta0=Aver.Teta0;//double(rand())/RAND_MAX*180;
  double Phi0=Aver.Phi0;
//  PhiStp=M_PI;
  RelaxPar.BegTime=0;
  double PrevTime=RelaxPar.BegTime;
//  RelaxPar.Times[0]=-Aver.EndTime;TimeStp*=2;
//  RelaxPar.Teta[0]=Teta0;RelaxPar.Phi[0]=Phi0;
  if (!Aver.SingleRotation)
   MC_Step(RelaxPar.Phi,300,Phi0,N,Aver.MeanRotTime,Aver.FurieDiscr,
           Aver.EndTime,Aver.StrongShort);
  else
    if (Aver.SingleRotation==2) 
      MC_Step3D(RelaxPar.Phi,300,Phi0,N,Aver.MeanRotTime,Aver.FurieDiscr,
           Aver.EndTime,Aver.StrongShort);
    else 
     {for ( k=1;k<=N;k++) RelaxPar.Phi[k]=PhiStp+RelaxPar.Phi[k-1];}


  for ( k=1;k<=N;k++)
   {
    double Cstp=TimeStp;//double(rand())/RAND_MAX*TimeStp*2;//
    PrevTime+=Cstp;
    RelaxPar.Times[k]=PrevTime;
    RelaxPar.Teta[k]=Teta0;
   }
for ( k=1;k<=N;k++) SumTimes[k]+=sqr(RelaxPar.Phi[k]);//sqr
//SumTimes=SumTimes+RelaxPar.Phi*RelaxPar.Phi;
//MakeDoubleTime(RelaxPar);

  if (Aver.OutPhi)
    {for ( k=1;k<=RelaxPar.NumHam;k++)  cout<<RelaxPar.Times[k]<<"  "<<RelaxPar.Phi[k]<<"\n";}
 };

//=====================================================================
//==================== MAIN    ========================================
//=====================================================================


void RelaxHamInt(RelaxAverageStruct &Aver,
                 double H,double Qs,double Eta,double Is,double Wid,
                 double *x,double *y)
 {
  VecCl res(x[0]);res=res*0;y[0]=x[0];
  VecCl tmp;
  MessRelaxInpPar RelaxPar;
  Time_struct time;
SumTimes.SetDim(Aver.NumTimes);SumTimes=SumTimes*0;
  for (int k=1;k<=Aver.NumToAverage;k++)
   {
    GenerateRelaxPar(RelaxPar,Aver);
//    RelaxOneHamInt(RelaxPar,H, Qs, Eta, Is, Wid, x,y);// Correct

//    RelaxOneHamIntMul(RelaxPar,H, Qs, Eta, Is, Wid, x,y);
//    tmp=y;res=res+tmp;                                  // Correct
//    Aver.Teta0=double(rand())/RAND_MAX*180;
//    Aver.Phi0=double(rand())/RAND_MAX*360;
//    Aver.MeanRotTime*=-1;
   }
RelaxOneHamInt(RelaxPar,H, Qs, Eta, Is, Wid, x,y);tmp=y;res=res+tmp;

  res=res/Aver.NumToAverage;
  MemoryMove(res.Ptr,y,sizeof(double)*(y[0]+1));
for (k=1;k<=Aver.NumTimes;k++) cout<<k<<" "<<SumTimes[k]/Aver.NumToAverage<<"\n";
cout<<" Time "<<time<<"\n";cout.flush();
//  return res;
 };

//=====================================================================
//=====================================================================
//=====================================================================


















/*
///
// ======================= Mul Var ============================
//

void GenerateInt(CMatrCl &Ham3,CMatrCl &Rot3Rgt,
                 CMatrCl &Ham1,CMatrCl &Rot1Lft,
                 double t0,double t1,double MaxNorm,my_comp *&res,double &Tstp,int &Nit)
 {
  double M=max<double>(MatrNorm(Ham3),MatrNorm(Ham1));
  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;

  Nit=int((t1-t0)*M/MaxNorm)+1;
  Tstp=(t1-t0)/Nit;
// CMatrCl AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3;FormAmp(AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3);

  CMatrCl 
          H3=Uni3+Ham3*my_comp(0,Tstp),
          H1=Uni1-Transpon(Ham1)*my_comp(0,Tstp);
  delete []res;res=new my_comp[Nit+1];res[0]=Nit;
  my_comp d3=sqrt(1./12),d2=sqrt(1./6),d1=0.5;
  for (int k=1;k<=Nit;k++)
   {
    Rot3Rgt=H3*Rot3Rgt;
    Rot1Lft=Rot1Lft*H1;

    res[k]=
Rot3Rgt(1,1)*d1*d1*Rot1Lft(1,1)+Rot3Rgt(1,2)*d3*d1*Rot1Lft(2,1)+
Rot3Rgt(2,2)*d3*d3*Rot1Lft(2,2)+Rot3Rgt(2,1)*d3*d1*Rot1Lft(1,2)+

Rot3Rgt(3,3)*d3*d3*Rot1Lft(1,1)+Rot3Rgt(3,4)*d3*d1*Rot1Lft(2,1)+
Rot3Rgt(4,4)*d1*d1*Rot1Lft(2,2)+Rot3Rgt(4,3)*d3*d1*Rot1Lft(1,2)+

Rot3Rgt(2,2)*d2*d2*Rot1Lft(1,1)+Rot3Rgt(2,3)*d2*d2*Rot1Lft(2,1)+
Rot3Rgt(3,3)*d2*d2*Rot1Lft(2,2)+Rot3Rgt(3,2)*d2*d2*Rot1Lft(1,2);

//               Trace(AmpR1*Rot3Rgt*AmpL1*Rot1Lft)+
//               Trace(AmpR2*Rot3Rgt*AmpL2*Rot1Lft)+
//               Trace(AmpR3*Rot3Rgt*AmpL3*Rot1Lft);
   }
  for (k=1;k<=Nit;k++) res[k]/=M_PI;

 };


void RelaxOneHamIntMul(MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is,double Wid,
                    double *x,double *y)
 {
  CMatrCl Rot3Rgt(4),Rot1Lft(2),Ham(8),Ham3(4),Ham1(2),tmp3(4),tmp1(2);
  Rot1Lft=Rot1Lft*0+1;Rot3Rgt=Rot3Rgt*0+1;
  my_comp *res=NULL;
  double Tstp;  
  Wid=Wid/2;
  int Nit;
  double CurT,CurItT,t0,t1;
  for (int xi=1;xi<=x[0];xi++) y[xi]=0;
  for (int t=1;t<=RelaxPar.NumHam;t++)
   {
    t1=RelaxPar.Times[t];t0=RelaxPar.Times[t-1];
    QsHMat(Ham,RelaxPar.Teta[t],RelaxPar.Phi[t],H, -Qs, Eta, -Is,&Ham3,&Ham1);

    GenerateInt(Ham3,Rot3Rgt,Ham1,Rot1Lft,t0,t1,0.01,res,Tstp,Nit);
    CurT=t0;
    for (xi=1;xi<=x[0];xi++)
     {
      my_comp freq=my_comp(-Wid,x[xi]);
      CurItT=CurT;
      for (int ti=1;ti<=Nit;ti++)
       {
        y[xi]+=my_real(res[ti]*exp(freq*CurItT))*Tstp;
        CurItT+=Tstp;
       }
      }
     }
  delete []res;
 };
     
///
// ======================= END OF Mul Var ============================
//


*/