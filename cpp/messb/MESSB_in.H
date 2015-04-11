
#ifndef __MESSB_IN_H
#define __MESSB_IN_H
#include "messb.h"
#include "mat\cmatrics.h"
#include "messb_angle_gen.h"





struct TimeShiftGenerator:RefCount
{
  TimeShiftGenerator(int numtimes,double endtime):NumTimes(numtimes),EndTime(endtime)
  {Start();}

  void Start();
  void ReStartSeq(){CurPos=-1;};
  int GetNextShift(double &StartTime,CMatrCl &LftAmp,CMatrCl &RgtAmp);
  double GetMaxShift(){return Time[Time.size()-1];}
  void AddCurrentShift(CMatrCl &LftU,CMatrCl &RgtU);
private:
  vector<CMatrCl> Lft,Rgt;
  vector<double> Time;
  int CurPos;
  int NumTimes,EndTime;
};

struct ContiniousRelaxationStepSolver_Base:Solver
{
  ContiniousRelaxationStepSolver_Base():LftAmp(2),RgtAmp(4){Start();}
  virtual void Init(HamData *ham) =0;// {Ham=ham;}
  virtual void SetCurState(double strttime,double endtime,double totalstart,CMatrCl &lftamp,CMatrCl &rgtamp,int stateindex) 
  {strtTime=strttime;endTime=endtime;TotalStart=totalstart;LftAmp=lftamp;RgtAmp=rgtamp;StateIndex=stateindex;}

  virtual void Solve(double *x,double *Res)=0;
  virtual void GetShift(CMatrCl &Ug,CMatrCl &Ue){Ug=UgShift;Ue=UeShift;}
  virtual void Start(){strtTime=0;endTime=1000;LftAmp=LftAmp*0+my_comp(1,0);RgtAmp=RgtAmp*0+my_comp(1,0);}
  void GenerateAmp_Shifted(vector<CMatrCl> &AmpL,vector<CMatrCl> &AmpR);

protected:
  double strtTime,endTime,TotalStart;
  CMatrCl LftAmp,RgtAmp;
  CMatrCl UeShift,UgShift;
  int StateIndex;
};


struct ContiniousRelaxationStepSolver:Solver_Free2_4,ContiniousRelaxationStepSolver_Base
{
  ContiniousRelaxationStepSolver():Solver_Free2_4(NULL),ContiniousRelaxationStepSolver_Base()
  {}
  void Init(HamData *ham)
  {Ham=ham;Solver_Free2_4::SolveCurState();}

  void Solve(double *x,double *Res){Solver_Free2_4::Solve(x,Res);}
  virtual void GenerateAmp(vector<CMatrCl> &AmpL,vector<CMatrCl> &AmpR)
  {GenerateAmp_Shifted(AmpL,AmpR);};
  void Intencity(CVecCl &Int,CVecCl &Pos,double *x,double *Res);
  void SolveCurState();
  //void Start(){strtTime=0;endTime=1000;LftAmp=LftAmp*0+my_comp(1,0);RgtAmp=RgtAmp*0+my_comp(1,0);}

};


struct ContiniousRelaxationStepMultiSolver:Solver
{
  Ref<TimeShiftGenerator> TimeShift;
  Ref<ContiniousRelaxationStepSolver_Base> Step;
  ContiniousRelaxationStepMultiSolver(TimeShiftGenerator *timeshift,
     ContiniousRelaxationStepSolver_Base *step):
                                      Step(step),TimeShift(timeshift){};
  void Init(HamTimeStepData *stpdat,double totalend){StpDat=stpdat;TotalEnd=totalend;}
  void Start(){Step->Start();TimeShift->Start();}
  void Solve(double *x,double *Res);
protected:
  Ref<HamTimeStepData> StpDat;
  double TotalEnd;
};

struct ContiniousRelaxationSolver_Base:Solver
{
  Ref<HamTimeGenerator> HamSer;
  ContiniousRelaxationSolver_Base():HamSer(NULL){}
  ContiniousRelaxationSolver_Base(HamTimeGenerator *hamser):HamSer(hamser){}
  void Solve(double *x,double *Res)=0;
};
struct ContiniousRelaxationSolver:ContiniousRelaxationSolver_Base
{
  //Ref<HamTimeGenerator> HamSer;
  Ref<ContiniousRelaxationStepMultiSolver> MultiSolver;
  ContiniousRelaxationSolver(HamTimeGenerator *hamser,
                ContiniousRelaxationStepMultiSolver *multisolver):
                ContiniousRelaxationSolver_Base(hamser),MultiSolver(multisolver){}
  void Solve(double *x,double *Res);
};

struct ContiniousRelaxationAverageSolver:Solver
{
  int NumToAverage;
  Ref<ContiniousRelaxationSolver> IterSolve;
  Ref<HamTimeAverageGenerator> HamAver;
  ContiniousRelaxationAverageSolver(ContiniousRelaxationSolver *itersolve,
                                   HamTimeAverageGenerator *hamaver,int numtoaverage):
                                   IterSolve(itersolve),HamAver(hamaver),NumToAverage(numtoaverage)
   {IterSolve->HamSer=HamAver;};
  void Solve(double *x,double *Res)
  {
    VecCl sum(x[0]),cur(x[0]);sum=sum*0;Res[0]=x[0];
    for (int k=1;k<=NumToAverage;k++)
    {
      HamAver->ReGenerate();
      IterSolve->Solve(x,cur.Ptr);
      sum=sum+cur;
      //sum=cur;
    }
    sum=sum/NumToAverage;
    sum.Copy2Double(Res);
  }
};



#include "lib\std\ex_out.h"
struct ContiniousRelaxationAverageSolver_DirectMatrMul:Solver
{
  Ref<HamTimeAverageGenerator> HamSer;
  int NumToAverage,NumSteps,OutAmp;
  double Width;
  Time_struct ClcTime;
  ContiniousRelaxationAverageSolver_DirectMatrMul(HamTimeAverageGenerator *hamser,
    int numtoaverage,int numsteps,int outamp):
    NumToAverage(numtoaverage),NumSteps(numsteps),OutAmp(outamp),HamSer(hamser){}

  void Solve(double *x,double *Res);
  void Start(){SumTime.SetDim(0);SumAmp.SetDim(0);}
  void InitAmplitude();
  void SolveHamSerie();
  double SolveUniHam(HamData *Ham,double Tstp,CMatrCl &gShift,CMatrCl &eShift);
  void Fourie(double *x,double *Res);
  double GetInt(my_comp &freq);
  void GenerateAmpSpline(CurveSpline *amp_spl);
  double Evaluate(double x);

private:
  VecCl SumAmp,SumTime;
  CurveSpline *Amp_T_Spl;
  my_comp IntegralFreq;
  double SplineIndexCoef;
  static ContiniousRelaxationAverageSolver_DirectMatrMul *CurPtr;
  static double IntegralAmp(double x)
  {
    double amp=CurPtr->Evaluate(x);
//    return amp*my_real(exp(CurPtr->IntegralFreq*x));
    my_comp freq=CurPtr->IntegralFreq;
    return amp*exp(freq.real()*x)*cos(freq.image()*x);
  };
  static double IntegralAmpFourie(double x)
  {
    double amp=CurPtr->Evaluate(x);
    return amp*exp(CurPtr->IntegralFreq.real()*x);
  };
};



#endif





//// In mes_fr.* - working previous programm - without beg time average
//struct MessRelaxClcPar
// {
//  double EndTime;
//  CMatrCl RelInt;
//  CMatrCl RelIntTmp;
//  CVecCl Freq;
//  friend ostream& operator<<(ostream &output,MessRelaxClcPar &par);
//
// };
//
//struct MessRelaxInpPar
// {
//  int NumHam;// Number Ham to clc
////  int NumTimeAver; // Number times to average
//  VecCl Teta,Phi,Times,TimeToAver,Hm;
//  double BegTime;
////  double *Teta,*Phi,*Times;
//  MessRelaxInpPar(int N=0) {DeleteStruct(0);ResetDim(N);}
//  ~MessRelaxInpPar(){DeleteStruct();};
//
//  void ResetDim(int N)
//   { 
//     if (N!=NumHam)
//      {Teta.SetDim(N);Phi.SetDim(N);Times.SetDim(N);NumHam=N;Hm=Times*0+1;}
//   };
//    
//  void DeleteStruct(int DelVar=1)
//   {if (DelVar) {Teta.SetDim(0);Phi.SetDim(0);Times.SetDim(0);}
//    NumHam=0;
//   };
//  MessRelaxInpPar& operator=(MessRelaxInpPar& par)
//   {
//    NumHam=par.NumHam;
//    Teta=par.Teta;Phi=par.Phi;Times=par.Times;
//    TimeToAver=par.TimeToAver;
//    return (*this);
//   };  
//
// };
//
//  struct MC_AverageParam
//  {
//	int NumToAverage;
//	double EndTime;
//	int NumTimeAver; // Number times to average
//	double EndTimeAver; // Average from BegTime to EndTimeAver
//	double OutPhi;
//  };
//  struct MC_KashubaParam
//  {
//	double FurieDiscr,StrongShort,Phi0,Teta0;
//	int NumTimes;//NumHam
//	double MeanRotTime;
//  };
//  struct MC_RotationParam
//  {
//	double MeanRotTime,TimeStp,DispTimeStp,Phi0,Teta0;
//  };
//  struct MC_RndWalkParam
//  {
//	  double TimeTeta,SigTimeTeta,AngTeta,SigAngTeta,AnisTeta;
//	  double TimePhi, SigTimePhi, AngPhi ,SigAngPhi ,AnisPhi;
//	  int NoPhi,HmulCos;
//    double AnisPower;
//  };
//  struct MC_RndJumpParam
//  {
//	  double TimeTeta,SigTimeTeta,AnisTeta;
//	  double TimePhi ,SigTimePhi ,AnisPhi;
//	  int NoPhi,HmulCos;
//    double AnisPower;
//	  VecCl axPhi,axTeta,axProb;
//	  double axTimeDisp;
//  };
//struct RelaxAverageStruct
// {
//  MC_AverageParam Aver;
//  MC_KashubaParam Kash;
//  MC_RotationParam Rot;
//  MC_RndWalkParam RndWalk;
//  MC_RndJumpParam RndJump;
//  enum AverageType {Nothing,Kashuba,Rotation,RndWalkType,RndJumpType};
//  AverageType RelaxType;
//  RelaxAverageStruct(){RelaxType=Nothing;};
//
//
//  void SetAverage(int nFullAngleClc,int nTimeClc,double TimeClc,double EndTime2TimeAver,
//	              int outPhi)
//  {
//	  Aver.NumToAverage=nFullAngleClc;Aver.EndTime=TimeClc;Aver.NumTimeAver=nTimeClc;
//	  Aver.EndTimeAver=EndTime2TimeAver;Aver.OutPhi=outPhi;
//  }
//
//
//
//  void SetKashuba(double phi0,double teta0,int num,double main_harm,double furie_discr,double strong_short)
//  {
//	  Kash.FurieDiscr=furie_discr;Kash.StrongShort=strong_short;Kash.Phi0=phi0;Kash.Teta0=teta0;
//	  Kash.NumTimes=num;Kash.MeanRotTime=main_harm;
//	  RelaxType=Kashuba;
//  }
//  void SetRotation(double phi0,double teta0,double mean_rot_time,double time_stp,double disp_time_stp)
//  {
//	  Rot.MeanRotTime=mean_rot_time;Rot.TimeStp=time_stp;Rot.DispTimeStp=disp_time_stp;
//	  Rot.Phi0=phi0;Rot.Teta0=teta0;
//	  RelaxType=Rotation;
//  }
//  void SetRndWalk(double time_teta,double sig_time_teta,double ang_teta,double sig_ang_teta,double anis_teta,
//				  double time_phi,double sig_time_phi,double ang_phi ,double sig_ang_phi ,double anis_phi,
//				  int no_phi,int h_mul_cos,double AnisPower)
//  {
//	  RndWalk.TimeTeta=time_teta;RndWalk.SigTimeTeta=sig_time_teta;RndWalk.AngTeta=ang_teta;RndWalk.SigAngTeta=sig_ang_teta;RndWalk.AnisTeta=anis_teta;
//	  RndWalk.TimePhi=time_phi;  RndWalk.SigTimePhi=sig_time_phi;  RndWalk.AngPhi =ang_phi ;RndWalk.SigAngPhi=sig_ang_phi  ;RndWalk.AnisPhi=anis_phi;
//    RndWalk.NoPhi=no_phi;RndWalk.AnisPower=AnisPower;
//	  if ((RndWalk.NoPhi) && h_mul_cos ) RndWalk.HmulCos=1;else RndWalk.HmulCos=0;
//	  RelaxType=RndWalkType;
//  }
//  void SetRndJump(double time_teta,double sig_time_teta,double anis_teta,
//				  double time_phi,double sig_time_phi,double anis_phi,
//				  int no_phi,int h_mul_cos,double AnisPower,
//				  VecCl &ax_phi,VecCl &ax_teta,VecCl &ax_prob,double ax_time_disp)
//  {
//	  RndJump.TimeTeta=time_teta;RndJump.SigTimeTeta=sig_time_teta;RndJump.AnisTeta=anis_teta;
//	  RndJump.TimePhi=time_phi;  RndJump.SigTimePhi=sig_time_phi;  ;RndJump.AnisPhi=anis_phi;
//	  RndJump.NoPhi=no_phi;RndJump.AnisPower=AnisPower;
//	  if ((RndJump.NoPhi) && h_mul_cos ) RndJump.HmulCos=1;else RndJump.HmulCos=0;
//	  RndJump.axTimeDisp=ax_time_disp;
//	  RndJump.axPhi=ax_phi;RndJump.axTeta=ax_teta;RndJump.axProb=ax_prob;
//	  RelaxType=RndJumpType;
//  }
//
//};
////  double TetaPlane,PhiPlane,RotPlane;
////  double BestDir;
////  double HMaxStp;
////  double Teta0;
////  double Pup,Pdown,AnisotrpCoef;
////  double Teta1,Phi1,TimeTeta0,TimeTeta1,TimePhi0,TimePhi1,AnisotrpCoef1;
//
//// };
//  
//
//
//void RelaxOneHamInt(MessRelaxInpPar &RelaxPar,
//                    double H,double Qs,double Eta,double Is,double Wid,
//                    double *x,double *y);
//void GenerateRelaxPar(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver);
//void RelaxHamInt(RelaxAverageStruct &Aver,
//                 double H,double Qs,double Eta,double Is,double Wid,
//                 double *x,double *y);
//
//
//
//
//
//
