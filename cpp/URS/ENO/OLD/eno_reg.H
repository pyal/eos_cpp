#ifndef __ENO_REG_H
#define __ENO_REG_H

#include "matt_reg.h"
#include "eno_bod.h"
#include "eno_clc.h"
#include <malloc.h>
//#include <alloc.h>


#define MaxNumMatter 10


struct Region
{
   int Order;
   int NumMatter,NumIntPar,NumPnt;
   double LftPnt,RgtPnt,Viscouse,Viscouse2,TimeStability;
   int LftFree,RgtFree,BndMatter[MaxNumMatter+1];
   MatterIO *Matter[MaxNumMatter+1];
   Body *B[MaxNumMatter+1];
   int UilWorkDim;

   int ReadData(istream &input);
   void SaveData(ostream &output);

   int ReadMatter(istream &input);
   void SaveMatter(ostream &output);

//   void SetBound();
   void TimeCh(double TimeStp);
   void StepRegion(double TimeStp,double &MinTimeStp,double &PresDeriv);
   void ClcMass();
   int SetWorkDim(int n=0); // To be called before mass clc, but after reading Region
   void SetBodySkipS(int NumSkipS);

   Region(){NumMatter=0;RegionShow=0;UilWorkDim=1;};
   void DeleteRegion();
   ~Region(){DeleteRegion(); };

   
   Body *BV[MaxNumMatter+1];
   int NumMatterV,MayBeSkippedNum,RegionShow;
   int MayBeSkipped[2][MaxNumMatter+1];
   void ControlSkipBody();
   void SetSkipBody();
//   int UilWorkDim(int n=0);


};



#endif
