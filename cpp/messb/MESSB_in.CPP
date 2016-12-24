#include <lib/precompiled/Messb.h>
#include "messb_in.h"
#include "lib/std/ex_out.h"
//#define OnlyStat


//
//=====================================================================
//=====================================================================
//=====================================================================




void TimeShiftGenerator::Start()
{
  double StartTime=0;
  VecCl T=VecCl::MakeStepVector(NumTimes,StartTime,EndTime,0);
  Lft.clear();Rgt.clear();Time.clear();
  CMatrCl U2(2),U4(4);
  U2=U2*my_comp(0,0)+my_comp(1,0);U4=U4*my_comp(0,0)+my_comp(1,0);
  for (int k=0;k<NumTimes;k++) 
  {Lft.push_back(U2);Rgt.push_back(U4);Time.push_back(T[k+1]);}
  ReStartSeq();
}

int TimeShiftGenerator::GetNextShift(double &StartTime,CMatrCl &LftAmp,CMatrCl &RgtAmp)
{
  CurPos++;
  if (CurPos>=(int)Lft.size()) return 0;
  LftAmp=Lft[CurPos];RgtAmp=Rgt[CurPos];StartTime=Time[CurPos];
  return 1;
}
void TimeShiftGenerator::AddCurrentShift(CMatrCl &LftU,CMatrCl &RgtU)
{
  int i=CurPos;if ((i<0)|| (CurPos>=(int)Lft.size())) {cout<<" Very bad, in AddCurrentShift\n";return;}
  Lft[i]=Lft[i]*LftU;
  Rgt[i]=RgtU*Rgt[i];
}




void ContiniousRelaxationStepSolver_Base::GenerateAmp_Shifted(vector<CMatrCl> &AmpL,vector<CMatrCl> &AmpR)
{
  GenerateAmp_DirectSol(AmpL,AmpR);
  for (int j=0;j<(int)AmpL.size();j++) 
  //{AmpL[j]=RgtAmp*AmpL[j];AmpR[j]=LftAmp*AmpR[j];}
  {AmpL[j]=RgtAmp*AmpL[j]*LftAmp;}//AmpR[j]=AmpR[j]*RgtAmp;}
}



void ContiniousRelaxationStepSolver::Intencity(CVecCl &Int,CVecCl &Pos,double *x,double *Res)
{
  int N=Pos.Dim();
  double s,eT=endTime-TotalStart,sT=strtTime-TotalStart;
  my_comp pos;
  CVecCl TransFreq(N); 
// Rough thing - due to the fact = all is included !!!
  for (int k=1;k<=N;k++) {TransFreq[k]=my_comp(0,1)*(Pos[k]+my_comp(-Ham->Is,Ham->Wid/2));}//TransFreq[k].real()=-fabs(TransFreq[k].real());}
  for (int k=1;k<=x[0];k++)
  {
    s=0;//pos=my_comp(0,x[k]);
    pos=my_comp(-Ham->Wid/2,x[k]-Ham->Is);
    for (int k1=1;k1<=N;k1++) 
    {
      //s+=my_real(Int[k1]*( exp(pos*endTime-TransFreq[k1]*(endTime-strtTime))-
      //                    exp(pos*strtTime) )/(pos-TransFreq[k1])*exp(-TransFreq[k1]*strtTime));
      s+=my_real(Int[k1]*( exp(pos*eT-TransFreq[k1]*(eT-sT))-
                          exp(pos*sT) )/(pos-TransFreq[k1]));
    }
    Res[k]=s/M_PI;
  }
}
void ContiniousRelaxationStepSolver::SolveCurState()
{
  //Solver_Free2_4::SolveCurState();
  double dt=endTime-strtTime;
  CMatrCl H4(4),H2(2);H4=H4*0;H2=H2*0;
  for (int k=1;k<=4;k++) H4(k,k)=exp(my_comp(0,-dt)*UeEig[k]);
  for (int k=1;k<=2;k++) H2(k,k)=exp(my_comp(0,-dt)*UgEig[k]);
  UeShift=UeLft*H4*UeRgt;
  UgShift=UgLft*H2*UgRgt;

}



void ContiniousRelaxationStepMultiSolver::Solve(double *x,double *Res)
{
  VecCl sum(x[0]),cur(x[0]);sum=sum*0;
  CMatrCl LftAmp,RgtAmp,Ue,Ug;
  double ShiftStart;
  Step->Init(StpDat->Ham);
  TimeShift->ReStartSeq();
  int i=0;
  while (TimeShift->GetNextShift(ShiftStart,LftAmp,RgtAmp))
  {
    i++;
    if (ShiftStart>StpDat->Tto) continue;
    double tfrom=max<double>(StpDat->Tfrom,ShiftStart);
    double tto=min(StpDat->Tto,TotalEnd-TimeShift->GetMaxShift()+ShiftStart);
    Step->SetCurState(tfrom,tto,ShiftStart,LftAmp,RgtAmp,i-1);
    Step->Solve(x,cur.Ptr);
    Step->GetShift(Ug,Ue);
    TimeShift->AddCurrentShift(Ug,Ue);
    sum=sum+cur;
  }
  sum=sum/i;
  sum.Copy2Double(Res);

}

void ContiniousRelaxationSolver::Solve(double *x,double *Res)
{
  HamTimeStepData *StpDat;
  VecCl sum(x[0]),cur(x[0]);sum=sum*0;Res[0]=x[0];
  HamSer->ReStartSeq();
  MultiSolver->Start();
  while (HamSer->GetNextHam(StpDat))
  {
    MultiSolver->Init(StpDat,HamSer->GetTotalEnd());
    MultiSolver->Solve(x,cur.Ptr);sum=sum+cur;
  }
  sum.Copy2Double(Res);
};






void ContiniousRelaxationAverageSolver_DirectMatrMul::Solve(double *x,double *Res)
{
  ClcTime.Mark();
  Start();
  for (int k=1;k<=NumToAverage;k++)
  {
    InitAmplitude();
    SolveHamSerie();
  }
  cout<<" Clc mean amp has taken :"<<ClcTime<<"\n";cout.flush();
  Fourie(x,Res);
};
void ContiniousRelaxationAverageSolver_DirectMatrMul::InitAmplitude()
{
//  HamSer->ReStartSeq();
  HamSer->ReGenerate();
  if (SumAmp.Dim()==0) 
  {
    SumTime=VecCl::MakeStepVector(NumSteps,0,HamSer->GetTotalEnd(),0);
    SumAmp=SumTime*0;Width=-1;
  }
}
void ContiniousRelaxationAverageSolver_DirectMatrMul::SolveHamSerie()
{
  HamTimeStepData *CurHamStep;
  double HamEndT=0;
  double Tstp=SumTime[2]-SumTime[1];
  CMatrCl gShift(2),eShift(4);gShift=gShift*0+1;eShift=eShift*0+1;
  SumAmp[1]+=1;
  for (int k=2;k<=SumTime[0];k++)
  {
    while ((HamEndT<SumTime[k]) && (HamSer->GetNextHam(CurHamStep)) ) HamEndT=CurHamStep->Tto;
    if (Width==-1) Width=CurHamStep->Ham->Wid;
    SumAmp[k]+=SolveUniHam(CurHamStep->Ham,Tstp,gShift,eShift);
  }
}
double ContiniousRelaxationAverageSolver_DirectMatrMul::SolveUniHam(HamData *Ham,double Tstp,CMatrCl &gShift,CMatrCl &eShift)
{
  CMatrCl mat2=Ham->Make2Ham(),mat4=Ham->Make4Ham();
  gShift=(mat2*0+1+mat2*my_comp(0,-Tstp))*gShift;eShift=eShift*(mat4*0+1+mat4*my_comp(0,-Tstp));
  my_comp amp(0,0);
  vector<CMatrCl> AmpL,AmpR;
  GenerateAmp_DirectSol(AmpL,AmpR);
  for (int k1=0;k1<(int)AmpL.size();k1++) amp=amp+Trace(eShift*AmpL[k1]*gShift*AmpR[k1]);
  return amp.real();
}
void ContiniousRelaxationAverageSolver_DirectMatrMul::Fourie(double *x,double *Res)
{
  //ClcTime.Mark;
  Amp_T_Spl=new CurveSpline(1);
  GenerateAmpSpline(Amp_T_Spl);
//  cout<<" Spline clc have taken: "<<ClcTime<<"\n";cout.flush();ClcTime.Mark;
  for (int ifreq=1;ifreq<=x[0];ifreq++)
  {
    my_comp freq(-0.5*Width,x[ifreq]);
    Res[ifreq]=GetInt(freq);
    Res[ifreq]=Res[ifreq]/M_PI/NumToAverage;
  }
  cout<<" Integration have taken: "<<ClcTime<<"\n";cout.flush();
  delete Amp_T_Spl;
}

//double ContiniousRelaxationAverageSolver_DirectMatrMul::GetInt(my_comp &freq)
//{
//  CurPtr=this;
//  IntegralFreq=freq;
//  double Misf=1e-4,ResMis,MaxT=SumTime[SumTime[0]];
//// NumInt=MaxT/2 - To make integration on intervals x..x+2 - usial period of amp - 1
//  int MaxNumInt=10000,ResErr,NumInt=max<double>(MaxT,2);
//  VecCl IntegrTimes=VecCl::MakeStepVector(NumInt,0,MaxT,0);
//  double ret=0;
//  for (int k=1;k<=NumInt-1;k++)
//    ret+=Qag_OscilInt(IntegralAmp,IntegrTimes[k],IntegrTimes[k+1],Misf,Misf,MaxNumInt,ResMis,ResErr);
//  return ret;
//}
double ContiniousRelaxationAverageSolver_DirectMatrMul::GetInt(my_comp &freq)
{
  CurPtr=this;
  IntegralFreq=freq;
  double Misf=1e-4,ResMis,MaxT=SumTime[SumTime.Dim()];
// NumInt=MaxT/2 - To make integration on intervals x..x+2 - usial period of amp - 1
  int MaxNumInt=10000,ResErr,NumInt=max(MaxT,2);
  VecCl IntegrTimes=VecCl::MakeStepVector(NumInt,0,MaxT,0);
  double ret=0;
  for (int k=1;k<=NumInt-1;k++)
    ret+=Qawo_FourierIntAB(IntegralAmpFourie,IntegrTimes[k],IntegrTimes[k+1],Misf,Misf,MaxNumInt,ResMis,ResErr,
                           freq.image(),1);
  return ret;
//double Qawo_FourierIntAB(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
//                  int NumSubInt,double &ResErr,int &Err,double Omega,int ToMulOnCos)
}


ContiniousRelaxationAverageSolver_DirectMatrMul *ContiniousRelaxationAverageSolver_DirectMatrMul::CurPtr;
void ContiniousRelaxationAverageSolver_DirectMatrMul::GenerateAmpSpline(CurveSpline *amp_spl)
{
  int NumX=SumTime.Dim();
  //double Misf=1e-5;
  //amp_spl->Generate(NumX,Misf,0,&SumTime[1],&SumAmp[1],SumTime[0]);
  SplineIndexCoef=NumX/SumTime[NumX];
  if (OutAmp)
  {
    VecCl ttt=VecCl::MakeStepVector(2000,0,SumTime[NumX],0);
    cout<<" Generated Amplitude vector\n";
    for (int k=1;k<=ttt[0];k++) cout<<ttt[k]<<" "<<Evaluate(ttt[k])<<" \n";
  }
}

double ContiniousRelaxationAverageSolver_DirectMatrMul::Evaluate(double x)
{
  int CurI=min(int(x*SplineIndexCoef)+1,SumTime.Dim()-1);
  double x1=SumTime[CurI],x2=SumTime[CurI+1],y1=SumAmp[CurI],y2=SumAmp[CurI+1];
  return y1+(y2-y1)/(x2-x1)*(x-x1);
};

//protected:
//  double strtTime,endTime,TotalStart;
//  CMatrCl LftAmp,RgtAmp;
//  CMatrCl UeShift,UgShift;
//};
//
//
//
//double ContiniousRelaxationStepSolver_DirectMatrMul::MaxMatrNormPerStep=1e-3;
//double ContiniousRelaxationStepSolver_DirectMatrMul::Misfit=1e-5;
//ContiniousRelaxationStepSolver_DirectMatrMul* 
//  ContiniousRelaxationStepSolver_DirectMatrMul::CurPtr;
//
//void ContiniousRelaxationStepSolver_DirectMatrMul::Solve(double *x,double *Res)
//{
//// Generate Ue,Ug shift
//  CMatrCl mat2=Ham->Make2Ham(),mat4=Ham->Make4Ham();
//  double dt0=endTime-strtTime,sT=strtTime-TotalStart,eT=endTime-TotalStart;
//  int NumInt=int(MatrNorm(mat4)*dt0/MaxMatrNormPerStep)+1;
//  double dt=dt0/NumInt,curT=strtTime-TotalStart;
//  CMatrCl gShift=(mat2*0+1),eShift=(mat4*0+1),gMul=gShift+mat2*my_comp(0,-dt),
//    eMul=eShift+mat4*my_comp(0,-dt);
//  vector<CMatrCl> AmpsL,AmpsR;
//  GenerateAmp_Shifted(AmpsL,AmpsR);
//  if (Times.size()<=StateIndex) 
//    {
//      Amps.push_back(vector<my_comp>());//Amps[StateIndex].reserve(100000);
//      Times.push_back(vector<double>());//Times[StateIndex].reserve(100000);
//      Times[StateIndex].push_back(0);
//      Amps[StateIndex].push_back(my_comp(1,0));
//      LastAmp.push_back(1);
//    }
//  //VecCl CurAmps(NumInt+1),CurTime(NumInt+1);CurAmps[1]=curT;CurTime[1]=curT;
//  for (int k=1;k<=NumInt;k++)
//  {
//    gShift=gMul*gShift;eShift=eShift*eMul;
//    curT+=dt;
//    my_comp amp(0,0);
//    for (int k1=0;k1<AmpsL.size();k1++) amp=amp+Trace(eShift*AmpsL[k1]*gShift*AmpsR[k1]);
//    if (Times[StateIndex].size()==Times[StateIndex].capacity())
//    {Amps[StateIndex].reserve(100000);Times[StateIndex].reserve(100000);}
//    Times[StateIndex].push_back(curT);
//    Amps[StateIndex].push_back(amp);
//    //CurAmps[k+1]=amp.real();CurTime[k+1]=curT;
//  }
//  //for (int k=1;k<=x[0];k++)
//  //{
//  //  my_comp freq(-0.5*Ham->Wid,x[k]);
//  //  Res[k]=0;
//  //  for (int k1=1;k1<=NumInt;k1++)
//  //  {
//  //    Res[k]+=my_real((CurAmps[k1]*exp(freq*CurTime[k1])+CurAmps[k1+1]*exp(freq*CurTime[k1+1]))*dt/2);
//  //  }
//  //  Res[k]/=M+PI;
//  //}
//  //CurAmps[1]=CurAmps[NumInt+1];
//  (VecCl(x[0])*0).Copy2Double(Res);
//  UgShift=gShift;UeShift=eShift;
//
//}
////  }
////  UgShift=(mat2*0+1)+mat2*my_comp(0,-dt);UeShift=(mat4*0+1)+mat4*my_comp(0,-dt);
////// Clc trace
////  double MeanT=(endTime+strtTime)/2-TotalStart;
////  double 
//////  cout<<" UeShift \n"<<UeShift<<" UgShift \n"<<UgShift<<" amp "<<amp<<"\n";
////  for (int k=1;k<=x[0];k++)
////  {
////    my_comp freq(-0.5*Ham->Wid,x[k]);
////    //Res[k]=my_real(1./freq*(amp*exp(freq*eT)-TraceLast*exp(freq*sT)));
////
////    Res[k]=my_real((amp*exp(freq*eT)+TraceLast*exp(freq*sT))*dt/2);
////  }
////  TraceLast=amp;
////
////}
//  VecCl Time,reAmp;
//
//void ContiniousRelaxationStepSolver_DirectMatrMul::Finish(double *x,double *Res)
//{
//  //return;
//  CurveSpline *Ore=ReAmp_T_Spl,*Oim=ImAmp_T_Spl;
//  ReAmp_T_Spl=new CurveSpline;ImAmp_T_Spl=new CurveSpline;
//  double MaxT=GenerateAmpSpline(ReAmp_T_Spl,ImAmp_T_Spl),Misf=Misfit,IntResMis;
//  int MaxNumInt=10000,IntErr;
//  CurPtr=this;
//  for (int ifreq=1;ifreq<=x[0];ifreq++)
//  {
//    my_comp freq(-0.5*Ham->Wid,x[ifreq]);
//    IntegralFreq=freq;
////    Res[ifreq]=Qag_OscilInt(IntegralAmp,0,MaxT,Misf,Misf,MaxNumInt,IntResMis,IntErr)/M_PI;
//    double s=0;
//    for (int k=1;k<=Time.Dim();k++) 
//      s+=(reAmp[k]*my_real(exp(freq*Time[k]))+reAmp[k+1]*my_real(exp(freq*Time[k])))*0.5*(Time[k+1]-Time[k]);
//      //s+=my_real( (reAmp[k]*exp(freq*Time[k]))*(Time[k+1]-Time[k]) );
//    Res[ifreq]=s/M_PI;
//  }
//  delete ReAmp_T_Spl;delete ImAmp_T_Spl;
//  ReAmp_T_Spl=Ore;ImAmp_T_Spl=Oim;
//}
//
//void ContiniousRelaxationStepSolver_DirectMatrMul::SumAmps(VecCl &TimeIndex,double &reSum,double &imSum,double time)
//{
//  reSum=0;imSum=0;
//  my_comp sum=0;
//  for (int k=0;k<TimeIndex[0];k++)
//  {
//    int CurInd=TimeIndex[k+1];
//    while((CurInd<Times[k].size()-2)&&(Times[k][CurInd+1]<time)) CurInd++;
//    double begT=Times[k][CurInd],endT=Times[k][CurInd+1];
//    my_comp begX=Amps[k][CurInd],endX=Amps[k][CurInd+1];
//    sum=sum+begX+(endX-begX)*((time-begT)/(endT-begT));
//    TimeIndex[k+1]=CurInd;
//  }
//  reSum=sum.real();imSum=sum.image();
//};
//double ContiniousRelaxationStepSolver_DirectMatrMul::GenerateAmpSpline(CurveSpline *reSpl,CurveSpline *imSpl)
//{
//  double sum=0,Misf=Misfit;
//  int nSum=Amps.size(),nT=Times[0].size();
////  VecCl reAmp(nT),Time(nT);
//  VecCl TimeIndex(nSum),imAmp(nT);TimeIndex=TimeIndex*0;
//  Time.SetDim(nT);reAmp.SetDim(nT);
//  for (int iT=0;iT<nT;iT++)
//  {
//    Time[iT+1]=Times[0][iT];
//    SumAmps(TimeIndex,reAmp[iT+1],imAmp[iT+1],Time[iT+1]);
////cout<<Time[iT+1]<<" "<<reAmp[iT+1]<<"\n";//<<" "<<imAmp[iT+1]
//  }
//  int NumX=Time[0];
////  reSpl->Generate(NumX,Misf,0,&Time[1],&reAmp[1],Time[0]);
////  //imSpl->Generate(NumX,Misf,0,&Time[1],&imAmp[1],Time[0]);
////VecCl ttt=VecCl::MakeStepVector(2000,0,Times[0][Times[0].size()-1],0);
////for (int k=1;k<=ttt[0];k++) cout<<ttt[k]<<" "<<reSpl->Evaluate(ttt[k])<<" \n";//<<imSpl->Evaluate(ttt[k])<<
//  return Times[0][Times[0].size()-1];
//}
//




















/*
void GenerateRelaxParTimeAver(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver,
                              double &TimeStp,double &Teta0,double &Phi0)
 {
  int N=Aver.Aver.NumTimeAver;RelaxPar.TimeToAver.SetDim(N);
  RelaxPar.BegTime=0;
  double CurT=RelaxPar.BegTime,Tstp=(Aver.Aver.EndTimeAver-RelaxPar.BegTime)/N;
  for (int k=1;k<=N;k++) {RelaxPar.TimeToAver[k]=CurT;CurT+=Tstp;}
  N=Aver.NumTimes;
  TimeStp=(Aver.EndTime-RelaxPar.BegTime)/N;
  Teta0=Aver.Teta0;//double(rand())/RAND_MAX*180;
  Phi0=Aver.Phi0;
  double PrevTime=RelaxPar.BegTime;
  for ( k=1;k<=N;k++)
   {
    double Cstp=TimeStp;//double(rand())/RAND_MAX*TimeStp*2;//
    PrevTime+=Cstp;
    RelaxPar.Times[k]=PrevTime;
    RelaxPar.Teta[k]=Teta0;
    RelaxPar.Hm[k]=1;
   }

 }
void GenerateRelaxPar(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver)
 {
  double TimeStp,Teta0,Phi0;
  srand( (unsigned)time( NULL ) );
  int N=Aver.NumTimes,k;RelaxPar.ResetDim(N);
  GenerateRelaxParTimeAver(RelaxPar,Aver,TimeStp,Teta0,Phi0);
  double PhiStp=TimeStp/Aver.MeanRotTime*360;//  PhiStp=M_PI;
  if (Aver.SingleRotation==0)
//   MC_Step(RelaxPar.Phi,Phi0,N,Aver.MeanRotTime,N/2,//Aver.FurieDiscr, - have to be N/2
   MC_Step(RelaxPar.Phi,Phi0,N,Aver.MeanRotTime,Aver.FurieDiscr, //- have to be N/2
//   MC_StepOld(RelaxPar.Phi,Phi0,N,Aver.MeanRotTime,N/2,//Aver.FurieDiscr, - have to be N/2
           Aver.EndTime,Aver.StrongShort);
  else
    if (Aver.SingleRotation==2) 
      MC_Step3D(RelaxPar.Phi,Phi0,N,Aver.MeanRotTime,Aver.FurieDiscr,
           Aver.EndTime,Aver.StrongShort);
  else
    if (Aver.SingleRotation==-1) 
     {
      VecCl tmpPhi,tmpTimes;
      MC_AxisStep(tmpPhi,tmpTimes,0,N,Aver.MeanRotTime,Aver.EndTime/Aver.TimeTeta0,
           Aver.EndTime,Aver.TimeTeta1/Aver.TimeTeta0,
		   Aver.Pup,Aver.Pdown,Aver.AnisotrpCoef);
	  MC_AxisStepShorten(5,tmpTimes,tmpPhi);
      RelaxPar.ResetDim(tmpPhi[0]);
      RelaxPar.Teta=tmpPhi;RelaxPar.Times=tmpTimes;RelaxPar.Phi=tmpPhi*0;//+Phi0;
	  if (Aver.StrongShort) 
	  {for (k=1;k<=tmpPhi[0];k++) RelaxPar.Hm[k]=fabs(cos(RelaxPar.Teta[k]*M_PI/180));}
     }
	else
    if (Aver.SingleRotation==-2) 
     {
      VecCl tmpPhi,tmpTeta,tmpTimes;
      MC_AxisStepFull(tmpPhi,tmpTeta,tmpTimes,Aver.EndTime,
		  Aver.TimeTeta0,Aver.TimeTeta1,Aver.Teta0,Aver.Teta1,Aver.AnisotrpCoef,
		  Aver.TimePhi0,Aver.TimePhi1,Aver.Phi0,Aver.Phi1,Aver.AnisotrpCoef1,
		  Aver.StrongShort);
      RelaxPar.ResetDim(tmpPhi[0]);
      RelaxPar.Teta=tmpTeta;RelaxPar.Times=tmpTimes;RelaxPar.Phi=tmpPhi;
	  if (Aver.StrongShort) 
	  {for (k=1;k<=tmpPhi[0];k++) RelaxPar.Hm[k]=fabs(cos(RelaxPar.Teta[k]*M_PI/180));}
     }

    else 
     {RelaxPar.Phi[1]=Phi0;for ( k=2;k<=N;k++) RelaxPar.Phi[k]=PhiStp+RelaxPar.Phi[k-1];}

#ifdef OnlyStat
if (NumSumTimes==0) SumTimes=RelaxPar.Phi*0;
for ( k=1;k<=SumTimes[0];k++)  SumTimes[k]+=sqr(RelaxPar.Phi[k]-RelaxPar.Phi[1]);NumSumTimes++;
#endif
//  MakeDoubleTime(RelaxPar); // Exclude the same angles from the serie

  if (Aver.OutPhi)
    {
	  cout<<"Calculated params:\n Time   Phi  Teta   Hcoef \n";
	  for ( k=1;k<=RelaxPar.NumHam;k++)  cout<<FM<<RelaxPar.Times[k]<<FMT<<RelaxPar.Phi[k]<<FMT<<RelaxPar.Teta[k]<<FMT<<RelaxPar.Hm[k]<<"\n";
	  cout<<" End Calculated params\n";
	}

 };
*/


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



//
// ======================= Fixed Ham precalculation ============================
//                              Begin

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
       L3=GetCol(Lft3,k3);L1=GetCol(Lft1,k1);
       R3=GetRow(Rgt3,k3);R1=GetRow(Rgt1,k1);
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

//
// ======================= Fixed Ham precalculation ============================
//                              End
*/



//ostream& operator<<(ostream &output,MessRelaxClcPar &par)
//  {for (int k=1;k<=1;k++)//my_real(par.RelInt[0]);k++)
//     output<<FM<<par.Freq[k]<<FMT<<par.RelInt(k,1)<<FMT<<par.EndTime<<"\n";
//   return output;};
//
//#define SameAngle 5.
//
//int WorkPos(double Phi)
// {
//   int Work= (Phi-int(Phi/360.)*360)/SameAngle;
//   if (Work==0) Work=360./SameAngle;
//   return Work;
// };
//
//
//// ==================================================================
//// ===================  Version with eig acceleration ============
//// ======================= BEGIN ====================================
//
//void FormAmp(CMatrCl &AmpR1,CMatrCl &AmpL1,CMatrCl &AmpR2,CMatrCl &AmpL2,
//             CMatrCl &AmpR3,CMatrCl &AmpL3)
// {
//  AmpR1.SetMatr(2,4);AmpR1=AmpR1*0;AmpR2=AmpR1;AmpR3=AmpR1;
//  my_comp d3=my_comp(sqrt(2./(3*8)),0),d2=my_comp(sqrt(4./(3*8)),0),
//          d1=my_comp(sqrt(6./(3*8)),0);
//
//  AmpR1(1,1)=d1;AmpR1(2,2)=d3;
//  AmpR2(1,3)=d3;AmpR2(2,4)=d1;
//  AmpR3(1,2)=d2;AmpR3(2,3)=d2;
//  AmpL1=Transpon(AmpR1);AmpL2=Transpon(AmpR2);AmpL3=Transpon(AmpR3);
// };
//
//  
//
//void GetVecIntTurn(CMatrCl &Int,CVecCl &Pos,
//                   CMatrCl &Mat3,CMatrCl &Mat1,
//                   CMatrCl *AmpL,CMatrCl *AmpR,
//                   CMatrCl *retEig3,CMatrCl *retLft3,CMatrCl *retRgt3,
//                   CMatrCl *retEig1,CMatrCl *retLft1,CMatrCl *retRgt1)
////                   double ShiftAngle)
//  {
//   int N3=Mat3.Dim(),N1=Mat1.Dim(),k3,k1;
//   CMatrCl Eig3(N3),Lft3(N3),Rgt3(N3);Eig3=Mat3;
//   CMatrCl Eig1(N1),Lft1(N1),Rgt1(N1);Eig1=Mat1;
//   URComeig(Eig3,Lft3,Rgt3);URComeig(Eig1,Lft1,Rgt1);
//   if (retEig3!=NULL) 
//    { 
//     *retEig3=Eig3;*retLft3=Lft3;*retRgt3=Rgt3;
//     *retEig1=Eig1;*retLft1=Lft1;*retRgt1=Rgt1;
//    }
//
//   CMatrCl AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3;FormAmp(AmpR1,AmpL1,AmpR2,AmpL2,AmpR3,AmpL3);
//   Pos.Dim(N3*N1);//Int.Dim(N3*N1);
//   CVecCl L3,L1,R3,R1,RotL3,RotR1;
//   
//   int k;
//   for (k3=1;k3<=N3;k3++) 
//     {
//     for (k1=1;k1<=N1;k1++) 
//      {
//       k=(k1-1)*N3+k3;
//       L3=GetCol(Lft3,k3);L1=GetCol(Lft1,k1);
//       R3=GetRow(Rgt3,k3);R1=GetRow(Rgt1,k1);
//       for ( int k0=1;k0<=Int.DimX;k0++)
//        {
//         RotL3=R3*AmpL[k0];RotR1=AmpR[k0]*L1;
//         Int(k0,k)=
//                ( ( (AmpR1*L3)*R1 )*((RotL3*AmpL1)*RotR1) )+//*dPhi+
//                ( ( (AmpR2*L3)*R1 )*((RotL3*AmpL2)*RotR1) )+//*dPhi_+
//                ( ( (AmpR3*L3)*R1 )*((RotL3*AmpL3)*RotR1) );
//        }
//       Pos[k]=Eig3(k3,k3)-Eig1(k1,k1);
//      }
//     }
//   Int=Int*(1/M_PI);
//  };
//
//  void InitTimeShift(CMatrCl *&TimeShiftLeft,CMatrCl *&TimeShiftRight,int NumTimeAver)
// {
//  CMatrCl Uni3(4),Uni1(2);Uni3=Uni3*0+1;Uni1=Uni1*0+1;
//  int N=NumTimeAver;
//  TimeShiftLeft=new CMatrCl[N+1];TimeShiftRight=new CMatrCl[N+1];
//  for (int k=1;k<=N;k++) 
//   {
//    TimeShiftLeft[k]=Uni3;TimeShiftRight[k]=Uni1;
//   }
// };
//void FormTimeIntAver(VecCl &TimeToAver,double t0,double t1,
////                     CMatrCl &IntGet,CVecCl &IntSet,VecCl &TimeDelta,int &AllTheSame)
//                     VecCl &TimeDelta,int &AllTheSame)
// {
//  double dt=t1-t0;
//  AllTheSame=1;//IntSet.SetDim(IntGet.DimY);
//  TimeDelta.SetDim(TimeToAver[0]);
////  IntSet=IntSet*my_comp(0,0);
////cout<<GetCol(IntGet,1);cout.flush();
//  for (int k=TimeToAver[0];k>=1;k--) 
//   { 
//    if (TimeToAver[k]<=t0) 
//     { 
//     for (int k1=k;k1>=1;k1--) {TimeDelta[k1]=dt;}//IntSet=IntSet+GetRow(IntGet,k1);} // Column or Row?
//      break;
//     }
//    AllTheSame=0;
//    TimeDelta[k]=max<double>(0,t1-TimeToAver[k]);
////    if (TimeDelta[k]>0) {IntSet=IntSet+GetRow(IntGet,k)*(TimeDelta[k]/max<double>(dt,MathZer));}
//   }
//
////  IntSet=IntSet/TimeToAver[0];
////cout<<IntSet;cout.flush();
// };
//void FormTimeShift(CMatrCl &MatEig,CMatrCl &MatL,CMatrCl &MatR,VecCl &TimeDelta,
//                   CMatrCl *TimeShift,int Left,int AllTheSame)
// {
//  CMatrCl Mat=MatEig*0,Zer;Zer=Mat;
//  int i,k;
//  VecCl TD=TimeDelta;
//  if (!Left) TD=TD*(-1);
//  double dt=TD[1];
//  if (AllTheSame)
//   {
//    for (i=1;i<=MatEig.Dim();i++) 
//      Zer(i,i)=exp(my_comp(0,-my_real(MatEig(i,i))*dt));
//    Mat=MatL*Zer*MatR;
//    for (k=1;k<=TD[0];k++) 
//      if (Left) TimeShift[k]=Mat*TimeShift[k];
//      else TimeShift[k]=TimeShift[k]*Mat;
//   }
//  else 
//   {
//    for (k=1;k<=TD[0];k++) 
//     {
//      dt=TD[k];if (dt==0) continue;
//      for (i=1;i<=MatEig.Dim();i++) 
//        Zer(i,i)=exp(my_comp(0,-my_real(MatEig(i,i))*dt));
//      Mat=MatL*Zer*MatR;
//      if (Left) TimeShift[k]=Mat*TimeShift[k];
//      else TimeShift[k]=TimeShift[k]*Mat;
//     }
//   }
// };
//
//
//// !!Changed - Addedd variable H, OK
//void FormIntArr(MessRelaxClcPar *RelaxClc,MessRelaxInpPar &RelaxPar,
//                    double H,double Qs,double Eta,double Is)
// {
//  int N=8;//Mat.Dim();
//  CMatrCl Mat(N),*TimeShiftLeft,*TimeShiftRight,Int;Int.SetMatr(RelaxPar.TimeToAver[0],N);
//  CMatrCl Mat3,Mat3Eig,Mat3L,Mat3R,     Mat1,Mat1Eig,Mat1L,Mat1R;
//  InitTimeShift(TimeShiftLeft,TimeShiftRight,RelaxPar.TimeToAver[0]);
//
//  CVecCl Pos(N);
//  VecCl TimeDelta;
////  RelaxPar.Times[0]=0;
//  double PrevTime=RelaxPar.BegTime;
//  RelaxClc[0].EndTime=PrevTime;
//  
////  RelaxClc[0].EndTime=0;
//  for (int k=1;k<=RelaxPar.NumHam;k++)
//   {
////    QsHMat(Mat,RelaxPar.Teta[k],RelaxPar.Phi[k],H, Qs, Eta, Is,&Mat3,&Mat1);
//    QsHMat(Mat,RelaxPar.Teta[k],RelaxPar.Phi[k],H*RelaxPar.Hm[k], Qs, Eta, Is,&Mat3,&Mat1);
//
////    CMatrCl Eig,Lft,Rgt;
//    Mat1=Transpon(Mat1);  // UPS!!!!!!
//    GetVecIntTurn(Int,Pos,Mat3,Mat1,TimeShiftLeft,TimeShiftRight,
//                   &Mat3Eig,&Mat3L,&Mat3R,&Mat1Eig,&Mat1L,&Mat1R);
//    RelaxClc[k].RelInt=Int;
//    RelaxClc[k].Freq=Pos;
//    int AllSame;
////    FormTimeIntAver(RelaxPar.TimeToAver,PrevTime,RelaxPar.Times[k],Int,RelaxClc[k].RelInt,TimeDelta,AllSame);
//    FormTimeIntAver(RelaxPar.TimeToAver,PrevTime,RelaxPar.Times[k],TimeDelta,AllSame);
//    FormTimeShift(Mat3Eig,Mat3L,Mat3R,TimeDelta,TimeShiftLeft,1,AllSame);
//    FormTimeShift(Mat1Eig,Mat1L,Mat1R,TimeDelta,TimeShiftRight,0,AllSame);
//    PrevTime=RelaxPar.Times[k];
//    RelaxClc[k].EndTime=RelaxPar.Times[k];
////cout<<RelaxClc[k];
//   }
//  delete []TimeShiftRight;delete []TimeShiftLeft;
// };
//
//my_comp SumSpectr(CVecCl &Sum,double Wid,double x,CVecCl &Freq,double t1,double t0)
////my_comp SumSpectr(CVecCl &Sum,double Wid,double x,CVecCl &Freq,double t1,double t0,double t2)
// {
//  my_comp res=0,fr;
//  if (t1==t0) return res;
////  if (t0-t2<0) {t1-=t2;t0=0;t2=0;}
//  for (int k1=1;k1<=my_real(Sum[0]);k1++)
//   {
//    fr=my_comp(-Wid,x-my_real(Freq[k1]));
////    res=res+Sum[k1]/fr*exp(my_comp( fabs(Wid*t2),-x*t2))*
//    res=res+Sum[k1]/fr*
//
//            ( exp(my_comp( -(Wid*t1),my_imag(fr)*(t1-t0)+x*t0 ))-
//              exp(my_comp( -(Wid*t0),x*t0)) );
//       
//   }
//  return res;
// };
////      for (int k1=1;k1<=N;k1++)
////       {
////        my_comp freq=my_comp(-Wid,x[i]-my_real(RelaxClc[k].Freq[k1]));
////        res=res+RelaxClc[k].RelInt(k0,k1)/freq*
////            ( exp(my_comp( -fabs(Wid*t1),my_imag(freq)*(t1-t0)+x[i]*t0 ))-
////              exp(my_comp(-fabs(Wid*t0),x[i]*t0)) );
////       }
//void RelaxOneHamInt(MessRelaxInpPar &RelaxPar,
//                    double H,double Qs,double Eta,double Is,double Wid,
//                    double *x,double *y)
// {
//  MessRelaxClcPar *RelaxClc=new MessRelaxClcPar[RelaxPar.NumHam+1];
//  Wid=Wid/2;
//  for (int u=1;u<=x[0];u++) y[u]=0;
//  FormIntArr(RelaxClc,RelaxPar,H, Qs, Eta, Is);
//  my_comp res;
//  double t0,t1,t2;
////  int N=my_real(RelaxClc[1].RelInt[0]);
//  int N=RelaxClc[1].RelInt.DimY,k0,NTAv=RelaxClc[1].RelInt.DimX,k;
//  CVecCl S(N);
//  for ( k=1;k<=RelaxPar.NumHam;k++) RelaxClc[k].RelIntTmp=RelaxClc[k].RelInt;
//  for (int i=1;i<=x[0];i++)
//   {
//    res=0;
//    for ( k0=2;k0<=NTAv;k0++)
//     {
//      t2=RelaxPar.TimeToAver[k0];my_comp coef=exp(my_comp( fabs(Wid*t2),-x[i]*t2));
//      for ( k=1;k<=RelaxPar.NumHam;k++)
//        SetRow(RelaxClc[k].RelIntTmp,k0,GetRow(RelaxClc[k].RelInt,k0)*coef);
//     }
//    for ( k=1;k<=RelaxPar.NumHam;k++)
//     {
/////*
//      S=S*0;
//      t1=RelaxClc[k].EndTime;t0=RelaxClc[k-1].EndTime;
//      for ( k0=1;k0<=RelaxClc[1].RelInt.DimX;k0++)
//       {
//        t2=RelaxPar.TimeToAver[k0];
//        if (t1-t2<=0) continue;
//        if (t0-t2<0) res=res+SumSpectr(GetRow(RelaxClc[k].RelInt,k0),Wid,x[i],
//                                        RelaxClc[k].Freq,t1-t2,0);
//        else {S=S+GetRow(RelaxClc[k].RelIntTmp,k0);}
//       }
//      res=res+SumSpectr(S,Wid,x[i],RelaxClc[k].Freq,t1,t0);
//     }
//    y[i]+=my_real(res)/RelaxClc[1].RelInt.DimX;
//   }
//  delete []RelaxClc;
////for ( k=1;k<=RelaxPar.NumHam;k++)  cout<<FM<<RelaxPar.Times[k]<<FMT<<RelaxPar.Phi[k]<<FMT<<RelaxPar.Teta[k]<<"\n";
//
// };
//
//// ==================================================================
//// ===================  Version with eig acceleration ============
//// =======================  END  ====================================
//
//#include "kashub.h"
//#include "messb_rel_gen.h"
//#ifdef OnlyStat
//VecCl SumTimes;
//int NumSumTimes;
//#endif
//
///*
//// =========================================================
//// ==========Exclude the same angles from the serie=========
//// ==================  BEGIN  ==============================
//void SetRelaxPar(MessRelaxInpPar &dest,MessRelaxInpPar &source,int i,int k,double Phi)
// {dest.Teta[i]=source.Teta[k];dest.Phi[i]=Phi;dest.Times[i]=source.Times[k];};
//void MakeDoubleTime(MessRelaxInpPar &RelaxPar)
// {
//  MessRelaxInpPar shor=RelaxPar;
//  int i=1,W=WorkPos(RelaxPar.Phi[1]);
//  shor.Phi[1]=W*SameAngle;
//  for (int k=2;k<=RelaxPar.NumHam-1;k++)
//   {
//    if (WorkPos(RelaxPar.Phi[k])!=W)
//     {i++;W=WorkPos(RelaxPar.Phi[k]);SetRelaxPar(shor,RelaxPar,i,k,W*SameAngle);}
//   }
//  if (RelaxPar.NumHam>=2) 
//   {i++;SetRelaxPar(shor,RelaxPar,i,RelaxPar.NumHam,WorkPos(RelaxPar.Phi[RelaxPar.NumHam])*SameAngle);}
//  shor.NumHam=i;
//  shor.Phi[0]=i;shor.Teta[0]=i;shor.Times[0]=i;RelaxPar=shor;
// };
//// =========================================================
//// ==========Exclude the same angles from the serie=========
//// ==================   END   ==============================
//*/
////
//// =========== Generate H(t)... - parameters for calculating =====================
////                             BEGIN
//
//void GenerateRelaxParTimeAver(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver)
// {
//  int N=Aver.Aver.NumTimeAver;RelaxPar.TimeToAver.SetDim(N);
//  RelaxPar.BegTime=0;
//  double CurT=RelaxPar.BegTime,Tstp=(Aver.Aver.EndTimeAver-RelaxPar.BegTime)/N;
//  for (int k=1;k<=N;k++) {RelaxPar.TimeToAver[k]=CurT;CurT+=Tstp;}
// }
//void FillRelaxPar_TimeStp(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver,int NumHam,
//				    	  double Teta0,double Phi0,double H0)
// {
//  RelaxPar.ResetDim(NumHam);
//  double TimeStp=(Aver.Aver.EndTime-RelaxPar.BegTime)/NumHam;
//  double PrevTime=RelaxPar.BegTime;
//  RelaxPar.Teta=RelaxPar.Teta*0+Teta0;
//  RelaxPar.Phi=RelaxPar.Phi*0+Phi0;
//  RelaxPar.Hm=RelaxPar.Hm*0+1;
//  for (int k=1;k<=NumHam;k++)
//   {
//    PrevTime+=TimeStp;
//    RelaxPar.Times[k]=PrevTime;
//   }
//}
//void MakeShift(MessRelaxInpPar &RelaxPar)
//{
//  for (int k=RelaxPar.NumHam-1;k>=1;k--)
//  {
//	  RelaxPar.Teta[k+1]=RelaxPar.Teta[k];
//	  RelaxPar.Phi[k+1]=RelaxPar.Phi[k];
//	  RelaxPar.Hm[k+1]=RelaxPar.Hm[k];
//  }
//}
//
//void GenerateRelaxPar(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver)
// {
//	GenerateRelaxParTimeAver(RelaxPar,Aver);
//	switch( Aver.RelaxType)
//	{
//		case RelaxAverageStruct::Kashuba :
//			{
//			 int N=Aver.Kash.NumTimes;
//			 FillRelaxPar_TimeStp(RelaxPar,Aver,N,Aver.Kash.Teta0,Aver.Kash.Phi0,1);
//             MC_Step(RelaxPar.Phi,Aver.Kash.Phi0,N,Aver.Kash.MeanRotTime,
//				 Aver.Kash.FurieDiscr, //- have to be N/2
////   MC_StepOld(RelaxPar.Phi,Phi0,N,Aver.MeanRotTime,N/2,//Aver.FurieDiscr, - have to be N/2
//                 Aver.Aver.EndTime,Aver.Kash.StrongShort);
//			 break;
//			}
//		case RelaxAverageStruct::Rotation :
//			{
//			 int N=Aver.Aver.EndTime/Aver.Rot.TimeStp+1,k;
//			 double PhiStp=Aver.Rot.TimeStp*360/Aver.Rot.MeanRotTime;
//			 FillRelaxPar_TimeStp(RelaxPar,Aver,N,Aver.Rot.Teta0,Aver.Rot.Phi0,1);
//			 RelaxPar.Phi[1]=Aver.Rot.Phi0;for ( k=2;k<=N;k++) RelaxPar.Phi[k]=PhiStp+RelaxPar.Phi[k-1];
//			 break;
//			}
//		case RelaxAverageStruct::RndWalkType :
//			{
//			 VecCl tmpPhi,tmpTeta,tmpTime;
//             MC_Anis2DAnglesWalk(tmpPhi,tmpTeta,tmpTime,Aver.Aver.EndTime,
//					 Aver.RndWalk.TimeTeta,Aver.RndWalk.SigTimeTeta,Aver.RndWalk.AngTeta,
//					 Aver.RndWalk.SigAngTeta,Aver.RndWalk.AnisTeta,
//					 Aver.RndWalk.TimePhi,Aver.RndWalk.SigTimePhi,Aver.RndWalk.AngPhi,
//					 Aver.RndWalk.SigAngPhi,Aver.RndWalk.AnisPhi,
//					 Aver.RndWalk.NoPhi,Aver.RndWalk.AnisPower);
// 			 RelaxPar.ResetDim(tmpPhi[0]);
//			 RelaxPar.Teta=tmpTeta;RelaxPar.Times=tmpTime;RelaxPar.Phi=tmpPhi;//+Phi0;
//	         if (Aver.RndWalk.HmulCos) 
//				{for (int k=1;k<=tmpPhi[0];k++) RelaxPar.Hm[k]=fabs(cos(RelaxPar.Teta[k]*M_PI/180));}
//			 break;
//			}
//		case RelaxAverageStruct::RndJumpType :
//			{
//			 VecCl tmpPhi,tmpTeta,tmpTime;
//             MC_Anis2DAnglesJump(tmpPhi,tmpTeta,tmpTime,Aver.Aver.EndTime,
//					 Aver.RndJump.TimeTeta,Aver.RndJump.SigTimeTeta,Aver.RndJump.AnisTeta,
//					 Aver.RndJump.TimePhi ,Aver.RndJump.SigTimePhi ,Aver.RndJump.AnisPhi,
//					 Aver.RndJump.NoPhi,Aver.RndJump.AnisPower,
//					 Aver.RndJump.axPhi,Aver.RndJump.axTeta,Aver.RndJump.axProb,Aver.RndJump.axTimeDisp);
// 			 RelaxPar.ResetDim(tmpPhi[0]);
//			 RelaxPar.Teta=tmpTeta;RelaxPar.Times=tmpTime;RelaxPar.Phi=tmpPhi;//+Phi0;
//	         if (Aver.RndJump.HmulCos) 
//				{for (int k=1;k<=tmpPhi[0];k++) RelaxPar.Hm[k]=fabs(cos(RelaxPar.Teta[k]*M_PI/180));}
//			 break;
//			}
//
//	}
//	MakeShift(RelaxPar);// Reason - used shifted params for calculation of ham...
//
//#ifdef OnlyStat
//if (NumSumTimes==0) SumTimes=RelaxPar.Phi*0;
//for ( k=1;k<=SumTimes[0];k++)  SumTimes[k]+=sqr(RelaxPar.Phi[k]-RelaxPar.Phi[1]);NumSumTimes++;
//#endif
////  MakeDoubleTime(RelaxPar); // Exclude the same angles from the serie
//
//  if (Aver.Aver.OutPhi)
//    {
//	  cout<<"Calculated params:\n Time   Phi  Teta   Hcoef \n";
//	  for (int k=1;k<=RelaxPar.NumHam;k++)  cout<<FM<<RelaxPar.Times[k]<<FMT<<RelaxPar.Phi[k]<<FMT<<RelaxPar.Teta[k]<<FMT<<RelaxPar.Hm[k]<<"\n";
//	  cout<<" End Calculated params\n";
//	}
//
// };
////
//// =========== Generate H(t)... - parameters for calculating =====================
////                                  END
//
////=====================================================================
////==================== MAIN    ========================================
////=====================================================================
//
//
//void RelaxHamInt(RelaxAverageStruct &Aver,
//                 double H,double Qs,double Eta,double Is,double Wid,
//                 double *x,double *y)
// {
//  VecCl res(x[0]);res=res*0;y[0]=x[0];
//  VecCl tmp;
//  MessRelaxInpPar RelaxPar;
//  Time_struct time;
//#ifdef OnlyStat  
//  NumSumTimes=0; 
//#endif
//
//  for (int k=1;k<=Aver.Aver.NumToAverage;k++)
//   {
//    GenerateRelaxPar(RelaxPar,Aver);
//#ifndef OnlyStat   
//    RelaxOneHamInt(RelaxPar,H, Qs, Eta, Is, Wid, x,y);tmp=y;res=res+tmp;
//#endif
//   }
//
//  res=res/Aver.Aver.NumToAverage;
//#ifdef OnlyStat 
//  res=res*0+1e4;
//#endif
//  MemoryMove(res.Ptr,y,sizeof(double)*(y[0]+1));
//#ifdef OnlyStat 
////  for ( k=1;k<=SumTimes[0];k++)  cout<<FM<<k<<FMT<<SumTimes[k]/NumSumTimes<<FMT<<SumTimes[k]/NumSumTimes/log(max<double>(k,2))<<"\n";
//#endif
//
//cout<<" Time "<<time<<"\n";cout.flush();
// };
