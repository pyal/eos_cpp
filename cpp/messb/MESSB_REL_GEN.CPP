#include <lib/precompiled/Messb.h>

#include "messb_rel_gen.h"





struct RelaxAngleWalkParam
{
	double Time,SigTime,Ang,SigAng;
	double Anis;
  double AnisPower;
	RelaxAngleWalkParam(double meanT,double sigT,double meanA,double sigA,double anis,
    double anisP)
	{Set(meanT,sigT,meanA,sigA,anis,anisP);}
	void Set(double meanT,double sigT,double meanA,double sigA,double anis,double anisP)
	{Time=meanT;SigTime=sigT;Ang=meanA;SigAng=sigA;Anis=anis;AnisPower=anisP;}
//	void SetWalk();
	VecCl ResT,ResAngle;
	double NormPhi(double x)
	{x-=360*(int)(x/360);x-=360*(int)(x/180);return x;}
	void Walk(double TimeFrom,double TimeTo,int Jump)
	{
		if (Anis>1e4) 
		{ResT.SetDim(2);ResT[1]=TimeFrom;ResT[2]=TimeTo;
		ResAngle.SetDim(2);ResAngle[1]=0;ResAngle[2]=0;return;}

		RndGaus timeStp(Time,SigTime),angleStp(Ang,SigAng);
		RndBoltsmanSinPower anis(Anis,AnisPower);
		timeStp.Init(max<double>(0,Time-4*SigTime),Time+4*SigTime,100,1e-5);
		if (!Jump) angleStp.Init(max<double>(0,Ang-4*SigAng),Ang+4*SigAng,100,1e-5);
    if (AnisPower>0) anis.Init(-90,90,800,1e-5);
		else anis.Init(-180,180,800,1e-5);
		double CurT=TimeFrom,CurAngle;//=anis.Rnd();
		CurAngle=anis.Rnd();
		int EstMaxN=2*(TimeTo-TimeFrom)/Time+50,i=0;
		ResT.SetDim(EstMaxN);ResAngle.SetDim(EstMaxN);
		while ((CurT<TimeTo) && (i<EstMaxN-2))
		{
			i++;
			CurAngle=NormPhi(CurAngle);
			ResT[i]=CurT;ResAngle[i]=CurAngle;
			CurT=min(TimeTo,CurT+timeStp.Rnd());
			if (Jump) CurAngle=anis.Rnd();
			else
			{
				double stp=angleStp.Rnd();
				double a=CurAngle,a1=CurAngle-stp,a2=CurAngle+stp;
				if (In_Lim(90,a,a1)) a1=90;else if (In_Lim(-90,a,a1)) a1=-90;
				if (In_Lim(90,a,a2)) a2=90;else if (In_Lim(-90,a,a2)) a2=-90;
				double p1=anis.Clc(a1),p2=anis.Clc(a2);
//				double rnd=(double(rand())/RAND_MAX);
				double rnd=RandomClass::Rnd(1);
				if (rnd<p1/max<double>(MathZer,p1+p2)) CurAngle-=stp;else CurAngle+=stp;
			}
		}
		i++;
		CurAngle=NormPhi(CurAngle);
		ResT[i]=CurT;ResAngle[i]=CurAngle;
		ResAngle[0]=i;ResT[0]=i;
	}
};
void LinkWalk(VecCl &Phi,VecCl &TPhi,VecCl &Teta,VecCl &TTeta,
				VecCl &resT,VecCl &resPhi,VecCl &resTeta)
{
//	cout<<Phi<<"\n"<<TPhi<<"\n"<<Teta<<"\n"<<TTeta<<"\n";
  int N=Teta[0]+Phi[0];
  int TetaI=1,PhiI=1,k=1;
  resPhi.SetDim(N);resTeta.SetDim(N);resT.SetDim(N);
  for (k=1;k<=N;k++)
  {
	  resTeta[k]=Teta[TetaI];resPhi[k]=Phi[PhiI];
	  if (TTeta[TetaI]==TPhi[PhiI]) {resT[k]=TTeta[TetaI];N--;TetaI++;PhiI++;}
	  else
	  if (TTeta[TetaI]<TPhi[PhiI]) {resT[k]=TTeta[TetaI];TetaI++;}
	  else {resT[k]=TPhi[PhiI];PhiI++;}
//	  resTeta[k]=Teta[TetaI];resPhi[k]=Phi[PhiI];
  }
  resPhi[0]=N;resTeta[0]=N;resT[0]=N;
//  cout<<resPhi<<"\n"<<resTeta<<"\n"<<resT<<"\n";
}
void LinkJump(VecCl &WPhi,VecCl &WTeta,VecCl &Twalk,VecCl &JPhi,VecCl &JTeta,VecCl &Tjump,
				VecCl &resT,VecCl &resPhi,VecCl &resTeta)
{
//cout<<"\nJPhi\n"<<JPhi<<"\nJTeta\n"<<JTeta<<"\nTjump\n"<<Tjump   <<"\nWPhi\n"<<WPhi<<"\nWTeta\n"<<WTeta<<"\nTwalk\n"<<Twalk<<"\n";
  int N=Twalk[0]+Tjump[0];
  int WalkI=1,JumpI=1;
  resPhi.SetDim(N);resTeta.SetDim(N);resT.SetDim(N);
  for (int k=1;k<=N;k++)
  {
	  resTeta[k]=WTeta[WalkI]+JTeta[JumpI];resPhi[k]=WPhi[WalkI]+JPhi[JumpI];
	  if (Twalk[WalkI]==Tjump[JumpI]) {N--;resT[k]=Twalk[WalkI];WalkI++;JumpI++;}
	  else
	  if (Twalk[WalkI]<Tjump[JumpI]) {resT[k]=Twalk[WalkI];WalkI++;}
	  else {resT[k]=Tjump[JumpI];JumpI++;}
  }
//  N--;
  resPhi[0]=N;resTeta[0]=N;resT[0]=N;
//cout<<"\nresPhi\n"<<resPhi<<"\nresTeta\n"<<resTeta<<"\nresT\n"<<resT<<"\n";
}
void MC_Anis2DAnglesWalk(VecCl &phi,VecCl &teta,VecCl &time,double TimeClc,
					 double TimeTeta,double SigTimeTeta,double AngTeta,double SigAngTeta,double AnisTeta,
					 double TimePhi,double SigTimePhi,double AngPhi,double SigAngPhi,double AnisPhi,
					 int NoPhi,double AnisPower)
 {
  RelaxAngleWalkParam Teta(TimeTeta,SigTimeTeta,AngTeta,SigAngTeta,AnisTeta,AnisPower);
  Teta.Walk(0,TimeClc,0);
  if (NoPhi) {teta=Teta.ResAngle;phi=teta*0;time=Teta.ResT;return;}
  RelaxAngleWalkParam Phi(TimePhi,SigTimePhi,AngPhi,SigAngPhi,AnisPhi,AnisPower);
  Phi.Walk(0,TimeClc,0);
  LinkWalk(Phi.ResAngle,Phi.ResT,Teta.ResAngle,Teta.ResT,time,phi,teta);
}

void MC_AnisJump(VecCl &phi,VecCl &teta,VecCl &time,double TimeClc,
					 double TimeTeta,double SigTimeTeta,double AnisTeta,
					 double TimePhi,double SigTimePhi,double AnisPhi,
					 int NoPhi,double AnisPower)
 {
  RelaxAngleWalkParam Teta(TimeTeta,SigTimeTeta,1,0.1,AnisTeta,AnisPower);
  Teta.Walk(0,TimeClc,1);
  if (NoPhi) {teta=Teta.ResAngle;phi=teta*0;time=Teta.ResT;return;}
  RelaxAngleWalkParam Phi(TimePhi,SigTimePhi,1,0.1,AnisPhi,AnisPower);
  Phi.Walk(0,TimeClc,1);
  LinkWalk(Phi.ResAngle,Phi.ResT,Teta.ResAngle,Teta.ResT,time,phi,teta);

}
void MC_AxisJump(VecCl &resPhi,VecCl &resTeta,VecCl &resT,VecCl &axPhi,VecCl &axTeta,
				 VecCl &axProb,double fullTime,double timeDisp)
// axProb - Blumes treatment - prob[k] prob to leave [k] axis
{
	int k,N=axPhi[0];
	VecCl Num(N);for (k=1;k<=N;k++) Num[k]=k;

	RndBlume rnd(Num,axProb);
	rnd.GenerateTimes(timeDisp);
	double curT=0;
	int Nest=2*fullTime/rnd.RelaxTime+50,curN=rnd.RndStart(),i=1,prevN;
	resPhi.SetDim(Nest);resTeta.SetDim(Nest);resT.SetDim(Nest);

	prevN=curN;
	resPhi[i]=axPhi[curN];resTeta[i]=axTeta[curN];resT[i]=curT;
	while ((curT<fullTime) && (i<Nest-2))
	{
		curT+=rnd.RndTstp();
		if ((curN=rnd.Rnd())==prevN) continue;
		i++;prevN=curN;
		resPhi[i]=axPhi[curN];resTeta[i]=axTeta[curN];resT[i]=curT;
	}
	resT[i]=fullTime;
	resPhi[0]=i;resTeta[0]=i;resT[0]=i;
}


void MC_Anis2DAnglesJump(VecCl &resPhi,VecCl &resTeta,VecCl &resTime,double TimeClc,
					 double TimeTeta,double SigTimeTeta,double AnisTeta,
					 double TimePhi,double SigTimePhi,double AnisPhi,
					 int NoPhi,double AnisPower,
					 VecCl &axPhi,VecCl &axTeta,VecCl &axProb,double axTimeDisp)
 {
	VecCl wPhi,wTeta,wT,jPhi,jTeta,jT;
    MC_AnisJump(wPhi,wTeta,wT,TimeClc,TimeTeta,SigTimeTeta,AnisTeta,
			                          TimePhi,SigTimePhi,AnisPhi, NoPhi,AnisPower);
    MC_AxisJump(jPhi,jTeta,jT,axPhi,axTeta,axProb,TimeClc,axTimeDisp);
    LinkJump(wPhi,wTeta,wT,jPhi,jTeta,jT,resTime,resPhi,resTeta);
}



/*
// Model H=Asin(ph)^2
// Probability  of state exp(-H/T)=exp(-AnisotrpCoefsin(ph)^2)
// MeanTimeRelax      1/FurieDiscr
// TimeRelaxDispers   StrongShort*MeanTimeRelax
// Probability to change Dir P1,P2
//  MC_AxisStep(tmpPhi,tmpTimes,Phi0,N,Aver.MeanRotTime,Aver.FurieDiscr,
//         Aver.EndTime,Aver.StrongShort,Aver.Pup,Aver.Pdown,Aver.AnisotrpCoef);
void MC_AxisStep(VecCl &ph,VecCl &time,double Phi0,int N,double MainHarm,
                 double FurieDiscr,double FullTime,double StrongShort,double p1,double p2,
                 double AnisotrpCoef)
 {
  double gZero=FullTime/FurieDiscr,gSigma=gZero*StrongShort;
  RndGaus gaus(gZero,gSigma);
  RndBoltsman bolt(AnisotrpCoef);
  gaus.Init(gZero-4*gSigma,gZero+4*gSigma,100,1e-5);
  bolt.Init(-90,90,500,1e-5);

  double CurT=0,CurPhi,Tstp,sP=p1+p2,ChDir[2]={p1/sP,p2/sP},AddAngle[2]={0,180};
//  double CurT=0,CurPhi,Tstp,sP=p1+p2,ChDir[2]={p2/sP,p1/sP},AddAngle[2]={0,57.4};
  int NumTimesEst=2*FurieDiscr+50,i=0,LstDir;
//  double rnd=(double(rand())/RAND_MAX)*(p1+p2);if (rnd>p1) LstDir=1;else LstDir=0;
  double rnd=(double(rand())/RAND_MAX);if (rnd>ChDir[1]) LstDir=0;else LstDir=1;
  if (AnisotrpCoef<3e5) CurPhi=bolt.Rnd();else CurPhi=0;
  CurPhi+=Phi0+AddAngle[LstDir];
  VecCl Times(NumTimesEst),Phi(NumTimesEst);
int StatUp=0;
  while (CurT<FullTime)
   {
    i++;Phi[i]=CurPhi;Times[i]=CurT;
//    if ((Tstp=FindRndVal(GausDist))<StndErr) Tstp=StndErr;
    if ((Tstp=gaus.Rnd())<StndErr) Tstp=StndErr;
    CurT+=Tstp;
//    if (AnisotrpCoef<3e5) CurPhi=FindRndVal(PhiDist);else CurPhi=0;
    if (AnisotrpCoef<3e5) CurPhi=bolt.Rnd();else CurPhi=0;
    rnd=(double(rand())/RAND_MAX);   
    if (rnd<ChDir[!LstDir]) LstDir=!LstDir;         // May be not correct ?
    CurPhi+=Phi0+AddAngle[LstDir];
StatUp+=LstDir;
    if (i>=NumTimesEst) break;
   };
cout<<" Stat "<<StatUp<<" from "<<i<<" coef "<<(i-StatUp)/max<double>(StatUp,StndErr)<<" HaveToBe "<<p1/p2<<"\n";

  Phi[0]=i;Times[0]=i;ph=Phi;time=Times;
 };

// Generate 

void MC_AxisTestBlume(VecCl &ResAngle,VecCl &ResTime,VecCl &PhiDir,VecCl &BlumeProb,
				      double TimeFrom,double TimeTo,double TimeDisp)
 {
  RndBlume(PhiDir,BlumeProb);
  RndBlume.GenerateTimes(TimeDisp);

  double CurT=TimeFrom,Tstp;
  int NumTimesEst=2*(TimeTo-TimeFrom)/RndBlume.RelaxTime+50,i=0;

  VecCl Times(NumTimesEst),Phi(NumTimesEst);
int StatUp=0;
  while (CurT<FullTime)
   {
    i++;Phi[i]=CurPhi;Times[i]=CurT;
//    if ((Tstp=FindRndVal(GausDist))<StndErr) Tstp=StndErr;
    if ((Tstp=gaus.Rnd())<StndErr) Tstp=StndErr;
    CurT+=Tstp;
//    if (AnisotrpCoef<3e5) CurPhi=FindRndVal(PhiDist);else CurPhi=0;
    if (AnisotrpCoef<3e5) CurPhi=bolt.Rnd();else CurPhi=0;
    rnd=(double(rand())/RAND_MAX);   
    if (rnd<ChDir[!LstDir]) LstDir=!LstDir;         // May be not correct ?
    CurPhi+=Phi0+AddAngle[LstDir];
StatUp+=LstDir;
    if (i>=NumTimesEst) break;
   };
cout<<" Stat "<<StatUp<<" from "<<i<<" coef "<<(i-StatUp)/max<double>(StatUp,StndErr)<<" HaveToBe "<<p1/p2<<"\n";

  Phi[0]=i;Times[0]=i;ph=Phi;time=Times;
 };

void MC_AxisStepShorten(double SameAng,VecCl &time,VecCl &phi)
{
	int k,N=time[0],i=1;
	double PrevAngle=phi[1];
	VecCl resT(N),resP(N);
	resT[1]=time[1];resP[1]=phi[1];
	for (k=2;k<=N;k++)
		if (fabs(phi[k]-PrevAngle)>SameAng) {i++;PrevAngle=phi[k];resT[i]=time[k];resP[i]=phi[k];}
	resT[0]=i;resP[0]=i;
	time=resT;phi=resP;
}


*/




