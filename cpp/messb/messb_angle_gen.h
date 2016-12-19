#ifndef __MESSB_ANGLE_GEN_H
#define __MESSB_ANGLE_GEN_H
#include "messb.h"
#include "mat\cmatrics.h"










struct RndBaseTimeGenerator:RefCount
{
  double StartTime,FinalTime;
  RndBaseTimeGenerator(double starttime,double finaltime):
                        StartTime(starttime),FinalTime(finaltime){Restart();};
  virtual void Restart(){CurTime=StartTime;};
  int Rnd(double &Rnd_Time);
  void Rnd(VecCl &Rnd_Time);
  virtual double GetNextTime()=0;
protected:
  double CurTime;
};

#include "mat\mtrnd.h"
struct RndTimeGenerator:RndBaseTimeGenerator
{
  Ref<RndFunc> Time;
  RndTimeGenerator(RndFunc *time,double starttime,double finaltime):
                   RndBaseTimeGenerator(starttime,finaltime),Time(time){};
  virtual double GetNextTime(){return Time->Rnd();}
};

struct RndAngleGenerator_Base:RefCount
{
  RndAngleGenerator_Base(){}

//  int Rnd(double &Rnd_Angle,double &Rnd_Time)=0;
  virtual void Rnd(VecCl &Rnd_Angle,VecCl &Rnd_Time)=0;
  virtual void Restart(){};//{Time->Restart();};
};

struct RndAngleGenerator:RndAngleGenerator_Base
{
  Ref<RndFunc> Angle;
  Ref<RndTimeGenerator> Time;
  RndAngleGenerator(RndFunc *angle,RndTimeGenerator *time):
                                       Time(time),Angle(angle){}

  int Rnd(double &Rnd_Angle,double &Rnd_Time);
  void Rnd(VecCl &Rnd_Angle,VecCl &Rnd_Time);
  void Restart(){Time->Restart();};
};



struct RndBlumeNumGenerator:RndBaseTimeGenerator
{
  RndBlume rnd_iter;
  RndBlumeNumGenerator(VecCl &axProb,double timeDisp,double starttime,
    double finaltime):RndBaseTimeGenerator(starttime,finaltime),rnd_iter(axProb,axProb) 
    {VecCl ParamVal=axProb*0;for (int k=1;k<=axProb.Dim();k++) ParamVal[k]=k;
     rnd_iter.SetParamVals(ParamVal);
     rnd_iter.GenerateTimes(timeDisp);rnd_iter.RndStart();}


  void Rnd(VecCl &Rnd_Num,VecCl &Rnd_Time);
  virtual double GetNextTime();
  //int GetMaxNum(){return NumAx;}
  virtual void Restart(){RndBaseTimeGenerator::Restart();Ax_vec.clear();};

private:
  vector<int> Ax_vec;
};
struct Rnd2AngleGenerator_Base:RefCount
{
  virtual void Rnd(VecCl &Rnd_Teta,VecCl &Rnd_Phi,VecCl &Rnd_Time)=0;
  static void LinkAngles(vector<VecCl> &Angles,VecCl &Angles_Time,
                                  VecCl &ToAdd,VecCl &ToAdd_Time);
};
struct Rnd2AngleGenerator:Rnd2AngleGenerator_Base
{
  Ref<RndAngleGenerator_Base> Phi,Teta;
  Rnd2AngleGenerator(RndAngleGenerator_Base *teta,RndAngleGenerator_Base *phi):
                              Teta(teta),Phi(phi){};
//  int Rnd(double &Rnd_Phi,double &Rnd_Teta,double &Rnd_Time)
  void Rnd(VecCl &Rnd_Teta,VecCl &Rnd_Phi,VecCl &Rnd_Time);
};
struct Rnd2AngleGenerator_FixPhi:Rnd2AngleGenerator_Base
{
  Ref<RndAngleGenerator_Base> Teta;
  double PhiVal;
  Rnd2AngleGenerator_FixPhi(RndAngleGenerator_Base *teta,double phival):
                              Teta(teta),PhiVal(phival){};
  void Rnd(VecCl &Rnd_Teta,VecCl &Rnd_Phi,VecCl &Rnd_Time);
};
struct Rnd2AngleAxGenerator:Rnd2AngleGenerator_Base
{
  Ref<RndBlumeNumGenerator> Ax;
  Ref<Rnd2AngleGenerator_Base> Angles;
  VecCl TetaAx,PhiAx;
  Rnd2AngleAxGenerator(RndBlumeNumGenerator *ax,Rnd2AngleGenerator_Base *angles,
                        VecCl &tetaax,VecCl &phiax):
                      Ax(ax),Angles(angles),TetaAx(tetaax),PhiAx(phiax){}
  void Rnd(VecCl &Rnd_Teta,VecCl &Rnd_Phi,VecCl &Rnd_Time);
};















struct HamTimeStepData:RefCount
{
  Ref<HamData> Ham;
  double Tfrom,Tto;
  HamTimeStepData(HamData *ham,double tfrom,double tto):Ham(ham),Tfrom(tfrom),Tto(tto){};
};






struct HamTimeGenerator:RefCount
{
  HamTimeGenerator(){dat_vec.clear();CurPos=-1;}
  HamTimeGenerator(HamTimeStepData *dat){dat_vec.clear();dat_vec.push_back(dat);ReStartSeq();}
  HamTimeGenerator(HamData *ham,int Num,double Tto);
  void ReStartSeq(){CurPos=0;};
  int GetNextHam(HamTimeStepData *&stpdat);
  int GetHamNum(){return (int)dat_vec.size();}
  double GetTotalEnd(){return dat_vec[dat_vec.size()-1]->Tto;}
protected:
  int CurPos;
  vector<Ref<HamTimeStepData> > dat_vec;
};

struct HamTimeAverageGenerator:HamTimeGenerator
{
// To be overriden
  virtual void ReGenerate();
  virtual int AddNewHam(HamData *&ham,double &TimeTo){return 0;}
  HamTimeAverageGenerator(){};
};





struct HamTimeAverageRndWalk:HamTimeAverageGenerator
{
  HamTimeAverageRndWalk(Rnd2AngleGenerator_Base *ang,HamData *ham,double gamma,
                        int hmulcos,int outangles):
                        Ham(ham),angles(ang),Gamma(gamma),HmulCos(hmulcos),OutAngles(outangles){}
  Ref<Rnd2AngleGenerator_Base> angles;
  Ref<HamData> Ham;
  double Gamma;
  int HmulCos,OutAngles;

  virtual void ReGenerate();
  virtual int AddNewHam(HamData *&ham,double &TimeTo);

private:
  int VecPos;
  VecCl Rnd_Phi,Rnd_Teta,Rnd_Time;
};

struct RndBoltsman_GausWalk:RndFunc
{
  Ref<RndGaus> rndGaus;
  Ref<RndBoltsmanSinPower> rndBolt;
  RndBoltsman_GausWalk(RndGaus *rndgaus,RndBoltsmanSinPower *rndbolt):
  rndGaus(rndgaus),rndBolt(rndbolt){CurAngle=rndBolt->Rnd(); };
  double Rnd() 
  { 
    double stp=rndGaus->Rnd();
		double a=CurAngle,a1=CurAngle-stp,a2=CurAngle+stp;
		//if (In_Lim(90,a,a1)) a1=90;else if (In_Lim(-90,a,a1)) a1=-90;
		//if (In_Lim(90,a,a2)) a2=90;else if (In_Lim(-90,a,a2)) a2=-90;
		double p1=rndBolt->Clc(a1),p2=rndBolt->Clc(a2);
//		double rnd=(double(rand())/RAND_MAX);
		double rnd=RandomClass::Rnd(1);
		if (rnd<p1/max<double>(MathZer,p1+p2)) CurAngle-=stp;else CurAngle+=stp;
    return CurAngle;
  };

private: double CurAngle;
};







#endif

