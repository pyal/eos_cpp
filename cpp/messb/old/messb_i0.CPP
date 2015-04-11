#include "messb_in.h"

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

  


///
// ======================= Mul Var ============================
//
//int MaxNit;
void GenerateIntVar(CMatrCl &Ham3,CMatrCl &Rot3Rgt,
                 CMatrCl &Ham1,CMatrCl &Rot1Lft,
                 double t0,double t1,double MaxNorm,my_comp *&res,double &Tstp,int &Nit)
 {
  double M=max(MatrNorm(Ham3),MatrNorm(Ham1));
  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;

  Nit=int((t1-t0)*M/MaxNorm)+1;
//Nit=1;
  Tstp=(t1-t0)/Nit;
//if (Nit>MaxNit) MaxNit=Nit;

//  CMatrCl AmpR,AmpL;
//  AmpR=R0HQsAmpVec_();AmpL=Conj(Transpon(AmpR));
  CMatrCl AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3;
  FormAmp(AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3);

  CMatrCl 
          H3=Uni3+Ham3*my_comp(0,Tstp),
          H1=Uni1-Transpon(Ham1)*my_comp(0,Tstp);
//          H3=Uni3+Transpon(Ham3*my_comp(0,Tstp)),
//          H1=Uni1-Ham1*my_comp(0,Tstp);

  delete []res;res=new my_comp[Nit+1];res[0]=Nit;
  for (int k=1;k<=Nit;k++)
   {
    Rot3Rgt=H3*Rot3Rgt;
    Rot1Lft=Rot1Lft*H1;
//    Rot3Rgt=Rot3Rgt*H3;
//    Rot1Lft=H1*Rot1Lft;

        res[k]=Trace(AmpR1*Rot3Rgt*AmpL1*Rot1Lft)/M_PI+
               Trace(AmpR2*Rot3Rgt*AmpL2*Rot1Lft)/M_PI+
               Trace(AmpR3*Rot3Rgt*AmpL3*Rot1Lft)/M_PI;

//    Rot1Lft=H1*Rot1Lft;
//    res[k]=Trace(AmpL*DirectMultiplyOfMat(Rot3Rgt,Rot1Lft)*AmpR)/M_PI;
// The same as trace, but we have not to transpon(Ham1)
// and multiply Rot3,Rot1 from the same side
//cout<<t0<<" "<<my_real(res[k])<<" "<<my_imag(res[k])<<"\n";t0+=Tstp;
   }
 };

void GenerateInt(CMatrCl &Ham3,CMatrCl &Rot3Rgt,
                 CMatrCl &Ham1,CMatrCl &Rot1Lft,
                 double t0,double t1,double MaxNorm,my_comp *&res,double &Tstp,int &Nit)
 {
  double M=max(MatrNorm(Ham3),MatrNorm(Ham1));
  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;

  Nit=int((t1-t0)*M/MaxNorm)+1;
  Tstp=(t1-t0)/Nit;

  CMatrCl 
          H3=Uni3+Ham3*my_comp(0,Tstp),
          H1=Uni1-Transpon(Ham1)*my_comp(0,Tstp);
  delete []res;res=new my_comp[Nit+1];res[0]=Nit;
  my_comp d3=my_comp(sqrt(2./(3*8)),0),d2=my_comp(sqrt(4./(3*8)),0),
          d1=my_comp(sqrt(6./(3*8)),0);
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
//MaxNit=0;
  double CurT,CurItT,t0,t1;
  for (int xi=1;xi<=x[0];xi++) y[xi]=0;
  for (int t=1;t<=RelaxPar.NumHam;t++)
   {
    t1=RelaxPar.Times[t];t0=RelaxPar.Times[t-1];
    QsHMat(Ham,RelaxPar.Teta[t],RelaxPar.Phi[t],H, -Qs, Eta, -Is,&Ham3,&Ham1);

//    GenerateIntVar(Ham3,Rot3Rgt,Ham1,Rot1Lft,t0,t1,0.01,res,Tstp,Nit);
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
//cout<<MaxNit<<"\n";
//cout<<Ham3<<Ham1<<"\n";
 };
     
///
// ======================= Mul Var ============================
//
void GetVecIntTurn_(CVecCl &Int,CVecCl &Pos,
                   CMatrCl &Mat3,CMatrCl &Mat1,
                   CMatrCl &AmpL,CMatrCl &AmpR,
                   CMatrCl *retEig3,CMatrCl *retLft3,CMatrCl *retRgt3,
                   CMatrCl *retEig1,CMatrCl *retLft1,CMatrCl *retRgt1)
  {
   int N3=Mat3.Dim(),N1=Mat1.Dim(),k3,k1;
//   CVecCl VL(N),VecL(N),VecR(N),VR(N);
   CMatrCl Eig3(N3),Lft3(N3),Rgt3(N3);Eig3=Mat3;
   CMatrCl Eig1(N1),Lft1(N1),Rgt1(N1);Eig1=Mat1;
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
//       Int[k]=GetCol(Lft,k)*GetRow(Rgt,k);
       L3=CMatrCl::GetCol(Lft3,k3);L1=CMatrCl::GetCol(Lft1,k1);
       R3=CMatrCl::GetRow(Rgt3,k3);R1=CMatrCl::GetRow(Rgt1,k1);
       RotL3=R3*AmpL;RotR1=AmpR*L1;
       Int[k]=
              ( ( (AmpR1*L3)*R1 )*((RotL3*AmpL1)*RotR1) )+
              ( ( (AmpR2*L3)*R1 )*((RotL3*AmpL2)*RotR1) )+
              ( ( (AmpR3*L3)*R1 )*((RotL3*AmpL3)*RotR1) );
//            ( (AmpR1*GetCol(Lft3,k3))
//                                     *GetRow(Rgt1,k1) )*
//            ( ((GetRow(Rgt3,k3)*AmpL)*AmpL1)
//                                     *(AmpR*GetCol(Lft1,k1)) )+
//            ( (AmpR2*GetCol(Lft3,k3))
//                                     *GetRow(Rgt1,k1) )*
//            ( ((GetRow(Rgt3,k3)*AmpL)*AmpL2)
//                                     *(AmpR*GetCol(Lft1,k1)) )+
//            ( (AmpR3*GetCol(Lft3,k3))
//                                     *GetRow(Rgt1,k1) )*
//            ( ((GetRow(Rgt3,k3)*AmpL)*AmpL3)
//                                     *(AmpR*GetCol(Lft1,k1)) );
//              (AmpR1*GetCol(Lft3,k3))
//              *(   (( (GetRow(Rgt3,k3)*AmpR)*AmpL1 )*AmpL)*GetCol(Lft1,k1)   )
//         *GetRow(Rgt1,k1) );
        
       Pos[k]=Eig3(k3,k3)-Eig1(k1,k1);
      }
     }
   Int=Int/M_PI;
  };



//
// ======================= Eig Var ============================
//

void FormIntArr(MessRelaxClcPar *RelaxClc,MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is)
 {
  int N=8;//Mat.Dim();
  CMatrCl Mat(N),TimeShiftLeft,TimeShiftRight;
  CMatrCl Mat3,Mat3Eig,Mat3L,Mat3R,     Mat1,Mat1Eig,Mat1L,Mat1R;
  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;
  TimeShiftLeft=Uni3;TimeShiftRight=Uni1;

  CVecCl Int(N),Pos(N);
  RelaxPar.Times[0]=0;
  RelaxClc[0].EndTime=0;
  for (int k=1;k<=RelaxPar.NumHam;k++)
   {
    QsHMat(Mat,RelaxPar.Teta[k],RelaxPar.Phi[k],H, -Qs, Eta, -Is,&Mat3,&Mat1);
    RelaxClc[k].EndTime=RelaxPar.Times[k];

//    CMatrCl AmpL,AmpR;
//    AmpL=R0HQsAmpVec_();
//    AmpR=TimeShiftRight*AmpL;AmpL=Conj(Transpon(Conj(Transpon(AmpL))*TimeShiftLeft));
    CMatrCl Eig,Lft,Rgt;
Mat1=Transpon(Mat1);
    GetVecIntTurn_(Int,Pos,Mat3,Mat1,TimeShiftLeft,TimeShiftRight,
                   &Mat3Eig,&Mat3L,&Mat3R,&Mat1Eig,&Mat1L,&Mat1R);

    RelaxClc[k].RelInt=Int;
    RelaxClc[k].Freq=Pos;

    Eig=Mat3Eig;
    for (int i=1;i<=Mat3.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,-my_real(Mat3Eig(i,i))*
                             (RelaxPar.Times[k]-RelaxPar.Times[k-1])));
    Mat3=Mat3L*Eig*Mat3R;
    TimeShiftLeft= Mat3*TimeShiftLeft;
    Eig=Mat1Eig;
    for (    i=1;i<=Mat1.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,my_real(Mat1Eig(i,i))*
                             (RelaxPar.Times[k]-RelaxPar.Times[k-1])));
    Mat1=Mat1L*Eig*Mat1R;
    TimeShiftRight=TimeShiftRight*Mat1;
   }
 
 };


void RelaxOneHamInt(MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is,double Wid,
                    double *x,double *y)
 {
  MessRelaxClcPar *RelaxClc=new MessRelaxClcPar[RelaxPar.NumHam+1];
  FormIntArr(RelaxClc,RelaxPar,H, Qs, Eta, Is);
  my_comp res;
  double t0,t1;
  int N=my_real(RelaxClc[1].RelInt[0]);
  Wid=Wid/2;
  for (int i=1;i<=x[0];i++)
   {
    res=0;
    for (int k=1;k<=RelaxPar.NumHam;k++)
     {
      t1=RelaxClc[k].EndTime;t0=RelaxClc[k-1].EndTime;
      for (int k1=1;k1<=N;k1++)
       {
        my_comp freq=my_comp(-Wid,x[i]-my_real(RelaxClc[k].Freq[k1]));

        res=res+
         RelaxClc[k].RelInt[k1]
          /freq*
            ( exp(my_comp( -fabs(Wid*t1),my_imag(freq)*(t1-t0)+x[i]*t0 ))-
              exp(my_comp(-fabs(Wid*t0),x[i]*t0)) );
       
       }
     }
    y[i]=my_real(res);
   }
  delete []RelaxClc;
 };

///
// ======================= Eig Var ============================
//
#include "kashub.h"

void GenerateRelaxPar(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver)
 {
  
  srand( (unsigned)time( NULL ) );
  int N=Aver.NumTimes,k;RelaxPar.ResetDim(N);
  double TimeStp=Aver.EndTime/N;
  double PhiStp=TimeStp/Aver.MeanRotTime*360;
  double Teta0=Aver.Teta0;//double(rand())/RAND_MAX*180;
  double Phi0=Aver.Phi0;
//  PhiStp=M_PI;
  RelaxPar.Times[0]=0;
//  RelaxPar.Times[0]=-Aver.EndTime;TimeStp*=2;
  RelaxPar.Teta[0]=Teta0;RelaxPar.Phi[0]=Phi0;
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
//    RelaxPar.Phi[k]=PhiStp*k;RelaxPar.Teta[k]=M_PI/2;RelaxPar.Times[k]=TimeStp*k
//     +RelaxPar.Times[0];

    double Cstp=TimeStp;//double(rand())/RAND_MAX*TimeStp*2;//
    RelaxPar.Times[k]=RelaxPar.Times[k-1]+Cstp;
    RelaxPar.Teta[k]=Teta0;
//    RelaxPar.Phi[k]=PhiStp/TimeStp*RelaxPar.Times[k]+Phi0;
//    RelaxPar.Phi[k]=RelaxPar.Phi[k-1]+PhiStp*(double(rand())/RAND_MAX-0.4);
//    RelaxPar.Phi[k]=Phi0;
//    RelaxPar.Teta[k]=PhiStp/TimeStp*RelaxPar.Times[k]+Teta0;
//    double add=double(rand())/RAND_MAX*PhiStp/TimeStp*Cstp;
//    if (double(rand())/RAND_MAX>0.5)
//      RelaxPar.Teta[k]=RelaxPar.Teta[k-1]+add;
//    else RelaxPar.Teta[k]=RelaxPar.Teta[k-1]-add;
//  RelaxPar.Teta[k]=M_PI*k/2;
//    RelaxPar.Teta[k]=90;
//    RelaxPar.Phi[k]=PhiStp/TimeStp*RelaxPar.Times[k];
   }

  if (Aver.OutPhi)
    {for ( k=1;k<=N;k++)  cout<<RelaxPar.Times[k]<<"  "<<RelaxPar.Phi[k]<<"\n";}
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
  for (int k=1;k<=Aver.NumToAverage;k++)
   {
    GenerateRelaxPar(RelaxPar,Aver);
    RelaxOneHamInt(RelaxPar,H, Qs, Eta, Is, Wid, x,y);
//    RelaxOneHamIntMul(RelaxPar,H, Qs, Eta, Is, Wid, x,y);
    tmp=y;res=res+tmp;
//    Aver.Teta0=double(rand())/RAND_MAX*180;
//    Aver.Phi0=double(rand())/RAND_MAX*360;
	Aver.Phi0=RandomClass::Rnd(360);
//    Aver.MeanRotTime*=-1;
   }
  res=res/Aver.NumToAverage;
  MemoryMove(res.Ptr,y,sizeof(double)*(y[0]+1));
cout<<" Time "<<time<<"\n";cout.flush();
//  return res;
 };

//=====================================================================
//=====================================================================
//=====================================================================


















/*
void FormIntArrOld(MessRelaxClcPar *RelaxClc,MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is)
 {
  int N=8;//Mat.Dim();
  CMatrCl Mat(N),TimeShiftLeft,TimeShiftRight;
  CMatrCl Mat3,Mat3Eig,Mat3L,Mat3R,     Mat1,Mat1Eig,Mat1L,Mat1R;
  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;
  TimeShiftLeft=Mat*0;TimeShiftLeft=TimeShiftLeft+1;
  TimeShiftRight=TimeShiftLeft;

  CVecCl Int(N),Pos(N);
  RelaxPar.Times[0]=0;
  RelaxClc[0].EndTime=0;
  for (int k=1;k<=RelaxPar.NumHam;k++)
   {
    QsHMat(Mat,RelaxPar.Teta[k],RelaxPar.Phi[k],H, -Qs, Eta, -Is,&Mat3,&Mat1);
    RelaxClc[k].EndTime=RelaxPar.Times[k];

    CMatrCl AmpL,AmpR;
    AmpL=R0HQsAmpVec_();
    AmpR=TimeShiftRight*AmpL;AmpL=Conj(Transpon(Conj(Transpon(AmpL))*TimeShiftLeft));
    CMatrCl Eig,Lft,Rgt;
    GetVecIntTurn(Mat,Int,Pos,AmpL,AmpR,&Eig,&Lft,&Rgt);
    RelaxClc[k].RelInt=Int/M_PI;
    RelaxClc[k].Freq=Pos;


    Mat3Eig=Mat3;Mat1Eig=Mat1;
    URComeig(Mat3Eig,Mat3L,Mat3R);URComeig(Mat1Eig,Mat1L,Mat1R);

    Eig=Mat3Eig;
    for (int i=1;i<=Mat3.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,-my_real(Mat3Eig(i,i))*
                             (RelaxPar.Times[k]-RelaxPar.Times[k-1])));
    Mat3=Mat3L*Eig*Mat3R;
//    TimeShiftRight= DirectMultiplyOfMat(Mat3,Uni1)*TimeShiftRight;

    Eig=Mat1Eig;
    for (    i=1;i<=Mat1.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,my_real(Mat1Eig(i,i))*
                             (RelaxPar.Times[k]-RelaxPar.Times[k-1])));
    Mat1=Mat1L*Eig*Mat1R;
//    TimeShiftLeft=TimeShiftLeft*DirectMultiplyOfMat(Uni3,Mat1);
    TimeShiftLeft=TimeShiftLeft*DirectMultiplyOfMat(Mat3,Mat1);
   }

void FormIntArr(MessRelaxClcPar *RelaxClc,MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is)
 {
  int N=8;//Mat.Dim();
  CMatrCl Mat(N),TimeShiftLeft,TimeShiftRight;
  CMatrCl Mat3,Mat3Eig,Mat3L,Mat3R,     Mat1,Mat1Eig,Mat1L,Mat1R;
  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;
  TimeShiftLeft=Mat*0;TimeShiftLeft=TimeShiftLeft+1;
  TimeShiftRight=TimeShiftLeft;

  CVecCl Int(N),Pos(N);
  RelaxPar.Times[0]=0;
  RelaxClc[0].EndTime=0;
  for (int k=1;k<=RelaxPar.NumHam;k++)
   {
    QsHMat(Mat,RelaxPar.Teta[k],RelaxPar.Phi[k],H, Qs, Eta, Is,&Mat3,&Mat1);
    RelaxClc[k].EndTime=RelaxPar.Times[k];

    CMatrCl AmpL,AmpR;
    AmpL=R0HQsAmpVec_();
    AmpR=TimeShiftRight*AmpL;AmpL=Transpon(Transpon(AmpL)*TimeShiftLeft);
    CMatrCl Eig,Lft,Rgt;
    GetVecIntTurn(Mat,Int,Pos,AmpL,AmpR,&Eig,&Lft,&Rgt);
//cout<<Pos<<Lft<<"\n";
    RelaxClc[k].RelInt=Int/M_PI;
cout<<Int<<"\n";
    RelaxClc[k].Freq=Pos;

    Mat3Eig=Mat3;Mat1Eig=Mat1;
    URComeig(Mat3Eig,Mat3L,Mat3R);URComeig(Mat1Eig,Mat1L,Mat1R);

//    Eig=Eig*0;
    Eig=Mat3Eig;
    for (int i=1;i<=Mat3.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,-my_real(Mat3Eig(i,i))*
                             (RelaxPar.Times[k]-RelaxPar.Times[k-1])));
    Mat3=Mat3L*Eig*Mat3R;
    TimeShiftRight= DirectMultiplyOfMat(Mat3,Uni1)*TimeShiftRight;

    Eig=Mat1Eig;
    for (    i=1;i<=Mat1.Dim();i++) 
      Eig(i,i)=exp(my_comp(0,-my_real(Mat1Eig(i,i))*
                             (RelaxPar.Times[k]-RelaxPar.Times[k-1])));
    Mat1=Mat1L*Eig*Mat1R;
    TimeShiftLeft=TimeShiftLeft*DirectMultiplyOfMat(Uni3,Mat1);
   }
 
 };


void RelaxOneHamInt(MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is,double Wid,
                    double *x,double *y)
 {
  MessRelaxClcPar *RelaxClc=new MessRelaxClcPar[RelaxPar.NumHam+1];
  FormIntArr(RelaxClc,RelaxPar,H, Qs, Eta, Is);
  my_comp res;
  double t0,t1;
  int N=my_real(RelaxClc[1].RelInt[0]);
  Wid=Wid/2;
  for (int i=1;i<=x[0];i++)
   {
    res=0;
    for (int k=1;k<=RelaxPar.NumHam;k++)
     {
      t1=RelaxClc[k].EndTime;t0=RelaxClc[k-1].EndTime;
      for (int k1=1;k1<=N;k1++)
       {
        my_comp freq=my_comp(-Wid,x[i]-my_real(RelaxClc[k].Freq[k1]));

        res=res+
//         norm(RelaxClc[k].RelInt[k1]) 
         RelaxClc[k].RelInt[k1]
//           1
          /freq*
            ( exp(my_comp( -fabs(Wid*t1),my_imag(freq)*(t1-t0)+x[i]*t0 ))-
              exp(my_comp(-fabs(Wid*t0),x[i]*t0)) );
//            ( exp(my_comp( -Wid*t1,my_imag(freq)*(t1-t0)))-
//              exp(my_comp(-Wid*t0,0)) );
       
       }
     }
    y[i]=my_real(res);
//    y[i]=norm(res);
//    y[i]=my_imag(res);
   }
//  RelaxClc[0].Freq=RelaxClc[1].Freq;  RelaxClc[0].RelInt=RelaxClc[1].RelInt;
  delete []RelaxClc;
 };
void GenerateRelaxPar(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver)
 {
  
  srand( (unsigned)time( NULL ) );
  int N=Aver.NumTimes;RelaxPar.ResetDim(N);
  double TimeStp=Aver.EndTime/N;
  double PhiStp=TimeStp/Aver.MeanRotTime*M_PI;
//  PhiStp=M_PI;
  RelaxPar.Times[0]=0;
  RelaxPar.Times[0]=-Aver.EndTime;TimeStp*=2;
  for (int k=1;k<=N;k++)
   {
//    RelaxPar.Phi[k]=PhiStp*k;RelaxPar.Teta[k]=M_PI/2;RelaxPar.Times[k]=TimeStp*k
//     +RelaxPar.Times[0];

    double Cstp=double(rand())/RAND_MAX*TimeStp*2;
    RelaxPar.Times[k]=RelaxPar.Times[k-1]+Cstp;
    RelaxPar.Phi[k]=0;
//    RelaxPar.Teta[k]=PhiStp/TimeStp*RelaxPar.Times[k];
    RelaxPar.Teta[k]=M_PI*k/2;
   }
 };
void RelaxHamInt(RelaxAverageStruct &Aver,
                 double H,double Qs,double Eta,double Is,double Wid,
                 double *x,double *y)
 {
  VecCl res(x[0]);res=res*0;y[0]=x[0];
  VecCl tmp;
  MessRelaxInpPar RelaxPar;
  for (int k=1;k<=Aver.NumToAverage;k++)
   {
    GenerateRelaxPar(RelaxPar,Aver);
    RelaxOneHamInt(RelaxPar,H, Qs, Eta, Is, Wid, x,y);
    tmp=y;res=res+tmp;
   }
  res=res/Aver.NumToAverage;
  MemoryMove(res.Ptr,y,sizeof(double)*(y[0]+1));
//  return res;
 };



*/