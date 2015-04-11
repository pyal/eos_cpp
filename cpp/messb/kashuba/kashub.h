
#ifndef __KASHUB_H
#define __KASHUB_H

#include "mat\mtrnd.h"
#include "mat\matrics.h"

void MC_Step(VecCl &ph,double Phi0,int N,double MainHarm,int FurieDiscr,
             double FullTime,double StrongShort);
void MC_StepOld(VecCl &ph,double Phi0,int N,double MainHarm,int FurieDiscr,
             double FullTime,double StrongShort);

//struct RndAngleGenerator_Base;
//#include "messb_in.h"

//struct RndAngleGenerator_Kashuba:RndAngleGenerator_Base
//{
//  double Phi0,MainHarm,FurieDiscr,FullTime,StrongShort;
//  int N;
//  RndAngleGenerator_Kashuba(double phi0,int n,double mainharm,double furiediscr,
//    double fulltime,double strongshort);
//             
////  int Rnd(double &Rnd_Angle,double &Rnd_Time)=0;
//  virtual void Rnd(VecCl &Rnd_Angle,VecCl &Rnd_Time);
//
//  //virtual void Restart(){};//{Time->Restart();};
//};




#endif