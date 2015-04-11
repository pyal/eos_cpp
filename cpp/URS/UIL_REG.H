#ifndef __UIL_REG_H
#define __UIL_REG_H

//#include "matt_reg.h"
#include "urs_forcelnk.h"
#include "uil_bod.h"
#include "uil_clc.h"
#include <malloc.h>
//#include <alloc.h>


#define MaxNumMatter 50


struct Region
{
   int NumMatter,NumIntPar,NumPnt;
   double LftPnt,RgtPnt,Viscouse,Viscouse2,TimeStability;
   int LftFree,RgtFree,BndMatter[MaxNumMatter+1];
   //MatterIO *Matter[MaxNumMatter+1];
   //Body *B[MaxNumMatter+1];
   DataVector< Ref<Body>, CopyStructRef<Ref<Body> > > B;
   //DataVector< Ref<Body> > B;
   DataVector< Ref<MatterIO>, CopyStructRef<Ref<MatterIO> > > Matter;
   //DataVector< Ref<MatterIO> > Matter;
   DataVector<DataVector<Stroka> > IntParNames;

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
