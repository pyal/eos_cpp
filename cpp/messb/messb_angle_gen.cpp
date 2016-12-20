#include <lib/precompiled/Messb.h>
#include "messb_angle_gen.h"











int RndBaseTimeGenerator::Rnd(double &Rnd_Time)
{
  if (CurTime-FinalTime>MathZer) return 0;
  Rnd_Time=CurTime;
  double tmp=GetNextTime();
  if (fabs(CurTime-FinalTime)<MathZer) CurTime=FinalTime+1e5;
  else CurTime=min(CurTime+tmp,FinalTime);
  return 1;
}
void RndBaseTimeGenerator::Rnd(VecCl &Rnd_Time)
{
  Restart();
  vector<double> tv;
  double t;while (Rnd(t)) 
  {
    if (tv.size()==tv.capacity()) tv.reserve(tv.size()*0.3+50);
    tv.push_back(t);
  }
  Rnd_Time.SetDim((int)tv.size());
  for (int k=1;k<=(int)tv.size();k++) Rnd_Time[k]=tv[k-1];
} 


int RndAngleGenerator::Rnd(double &Rnd_Angle,double &Rnd_Time)
{
  if (!Time->Rnd(Rnd_Time)) return 0;
  Rnd_Angle=Angle->Rnd();
  return 1;
}
void RndAngleGenerator::Rnd(VecCl &Rnd_Angle,VecCl &Rnd_Time)
{
  Time->Rnd(Rnd_Time);
  Rnd_Angle.SetDim(Rnd_Time.Dim());
  for (int k=1;k<=Rnd_Time.Dim();k++) Rnd_Angle[k]=Angle->Rnd();
}









void RndBlumeNumGenerator::Rnd(VecCl &Rnd_Num,VecCl &Rnd_Time)
{
  RndBaseTimeGenerator::Rnd(Rnd_Time);Rnd_Num.SetDim((int)Ax_vec.size());
  for (int k=1;k<=(int)Ax_vec.size();k++) Rnd_Num[k]=Ax_vec[k-1];
  Ax_vec.clear();
}
double RndBlumeNumGenerator::GetNextTime()
{
  double ret=0;
  int NewAx,CurAx=rnd_iter.CurrentState;
  
  //int NewAx,CurAx=-1;
  //if (Ax_vec.size()>0) CurAx=Ax_vec[Ax_vec.size()-1];
  //else CurAx=rnd_iter.Rnd();
  while (ret<fabs(FinalTime-StartTime))
  {
		ret+=rnd_iter.RndTstp();
		if ((NewAx=rnd_iter.Rnd())!=CurAx) break;
  }

  if (Ax_vec.size()==Ax_vec.capacity()) Ax_vec.reserve(Ax_vec.size()*0.3+50);
  Ax_vec.push_back(NewAx);
  return ret;
}
void Rnd2AngleGenerator_Base::LinkAngles(vector<VecCl> &Angles,VecCl &Angles_Time,
                                VecCl &ToAdd,VecCl &ToAdd_Time)
{
  if (Angles.size()==0){Angles.push_back(ToAdd);Angles_Time=ToAdd_Time;return;}
  if (ToAdd_Time.Dim()==0) return;
  vector<int> pMet;
  int Nang=Angles_Time.Dim(),Nadd=ToAdd_Time.Dim();
  int add_pos=0,ang_pos=0;
  while (1)
  {
    if (pMet.size()==pMet.capacity()) pMet.reserve(pMet.size()*0.3+50);
    if (fabs(Angles_Time[ang_pos+1]-ToAdd_Time[add_pos+1])<MathZer)
      {ang_pos++;add_pos++;pMet.push_back(3);}
    else
      if (Angles_Time[ang_pos+1]<ToAdd_Time[add_pos+1])
        {ang_pos++;pMet.push_back(1);}
      else {add_pos++;pMet.push_back(2);}
    if (ang_pos==Nang) {while (add_pos!=Nadd) {add_pos++;pMet.push_back(2);}break;}
    if (add_pos==Nadd) {while (ang_pos!=Nang) {ang_pos++;pMet.push_back(1);}break;}
  }
  int Na=(int)Angles.size(),N=(int)pMet.size();
  vector<VecCl> resA;resA.reserve(Na+1);
  VecCl resT(N);
  for (int k=1;k<=Na+1;k++) resA.push_back(VecCl(N));
  add_pos=0;ang_pos=0;
  for (int k=1;k<=N;k++) 
  {
    if (pMet[k-1]==3) {add_pos++;ang_pos++;resT[k]=Angles_Time[ang_pos];}
    else if (pMet[k-1]==2) {add_pos++;resT[k]=ToAdd_Time[add_pos];}
          else {ang_pos++;resT[k]=Angles_Time[ang_pos];}
    for (int k1=0;k1<Na;k1++) resA[k1][k]=Angles[k1][max<double>(1,ang_pos)];
    resA[Na][k]=ToAdd[max(1,add_pos)];
  }
  Angles_Time=resT;
  Angles=resA;
}
void Rnd2AngleGenerator::Rnd(VecCl &Rnd_Teta,VecCl &Rnd_Phi,VecCl &Rnd_Time)
{
  Rnd_Time.SetDim(0);
  VecCl phi,tphi,teta,tteta;
  Phi->Rnd(phi,tphi);Teta->Rnd(teta,tteta);
//cout<<" time Teta  \n";for (int k=1;k<=tteta[0];k++) cout<<tteta[k]<<" "<<teta[k]<<"\n";
//cout<<" time Phi  \n";for (int k=1;k<=tphi[0];k++) cout<<tphi[k]<<" "<<phi[k]<<"\n";
  vector<VecCl> Angles;Angles.push_back(teta);
  LinkAngles(Angles,tteta,phi,tphi);
  Rnd_Phi=Angles[1];Rnd_Teta=Angles[0];Rnd_Time=tteta;
}

void Rnd2AngleGenerator_FixPhi::Rnd(VecCl &Rnd_Teta,VecCl &Rnd_Phi,VecCl &Rnd_Time)
{
  Rnd_Time.SetDim(0);
  VecCl teta,tteta;Teta->Rnd(Rnd_Teta,Rnd_Time);
  Rnd_Phi=Rnd_Time*0+PhiVal;
}
void Rnd2AngleAxGenerator::Rnd(VecCl &Rnd_Teta,VecCl &Rnd_Phi,VecCl &Rnd_Time)
{
  VecCl phi,tang,teta,num,tax;
  Angles->Rnd(teta,phi,tang);
  Ax->Rnd(num,tax);
//  for (int k=1;k<=num.Dim();k++) cout<<tax[k]<<" "<<num[k]<<"\n";
  vector<VecCl> Angles;Angles.push_back(teta);Angles.push_back(phi);
  LinkAngles(Angles,tang,num,tax);
  Rnd_Time=tang;
  Rnd_Phi=Angles[1];Rnd_Teta=Angles[0];
  for (int k=1;k<=Rnd_Time.Dim();k++)
  {Rnd_Phi[k]+=PhiAx[Angles[2][k]];Rnd_Teta[k]+=TetaAx[Angles[2][k]];}
}












HamTimeGenerator::HamTimeGenerator(HamData *ham,int Num,double Tto)
{
  double Tfrom=0;
  VecCl Times=VecCl::MakeStepVector(Num+1,Tfrom,Tto,0);
  dat_vec.clear();dat_vec.reserve(Num);
  for (int k=1;k<Num+1;k++)
  {
    dat_vec.push_back(new HamTimeStepData(ham,Times[k],Times[k+1]));
  }
  if (Num<=0) dat_vec.push_back(new HamTimeStepData(ham,Tfrom,Tto));
  ReStartSeq();
}

int HamTimeGenerator::GetNextHam(HamTimeStepData *&stpdat)
{
  if (CurPos>=(int)dat_vec.size()) return 0;
  stpdat=dat_vec[CurPos];
  CurPos++;
  return 1;
}


// To be overriden
void HamTimeAverageGenerator::ReGenerate()
{
  dat_vec.clear();
  HamData *ham;
  double time,Tlast=0;
  while (AddNewHam(ham,time)) 
  {
    if (dat_vec.size()==dat_vec.capacity()) dat_vec.reserve(dat_vec.size()*0.3+50);
    dat_vec.push_back(new HamTimeStepData(ham,Tlast,time));Tlast=time;
  }
  ReStartSeq();

};




void HamTimeAverageRndWalk::ReGenerate()
{
  angles->Rnd(Rnd_Teta,Rnd_Phi,Rnd_Time);
  VecPos=0;
  if (OutAngles)  {cout<<" HamTimeAverageRndWalk::ReGenerate() - angles\n";for (int k=1;k<=Rnd_Time[0];k++) cout<<Rnd_Time[k]<<" "<<Rnd_Teta[k]<<" "<<Rnd_Phi[k]<<"\n";}

  HamTimeAverageGenerator::ReGenerate();
}
int HamTimeAverageRndWalk::AddNewHam(HamData *&ham,double &TimeTo)
{
  if (VecPos>=Rnd_Time.Dim()-1) return 0;
  VecPos++;
  TimeTo=Rnd_Time[VecPos+1];
  double H=Ham->H;
  if (HmulCos) Ham->H=H*cos(Rnd_Teta[VecPos]*M_PI/180);
  ham=new HamHTurned_Z_Free_Qs_Gamma(*Ham,Gamma,Rnd_Teta[VecPos],Rnd_Phi[VecPos]);
  Ham->H=H;
  return 1;
}





