#ifndef __URS_FOR_H
#define __URS_FOR_H 

#include "util.h"
#include "ur_obj.h"
#include "fstream.h"
#include "process.h"
#include <math.h>

#define MaximalPressure 1e8

struct MatterArrStruct
  {
   double Length;
   double LftCoord;
   int  Num;
   double XStp;
   
   double Viscouse1,Viscouse2;
   double CellM;//CellM=Dencity*XStp  CellM=const for all mediu (Bnd?)
   Thermo *Arr;
   double *CellPos;
   friend ostream& operator<<(ostream &output,MatterArrStruct &Pnt);
   friend istream& operator>>(istream &input,MatterArrStruct &Pnt);
  };


struct MatterSumStruct
  {
   int Num;
   double Time;
   double TimeStab;
   double EndTime;
   double WriteTime;

   MatterArrStruct *Matter;
   friend ostream& operator<<(ostream &output,MatterSumStruct &Sum);
   friend istream& operator>>(istream &input,MatterSumStruct &Sum);
  };


struct IntegralConst
  {
   int First;
   Thermo *LastValues;
   Thermo *DeltaValues;
   IntegralConst(){First=1;};
   ~IntegralConst(){if (!First) {delete LastValues;delete DeltaValues;}};
   void Control(MatterSumStruct &Sum);
   void Write(MatterSumStruct &Sum,ostream &out);
  };

#endif

