#ifndef __FUNCOBJ_H
#define __FUNCOBJ_H
/*
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream.h>
#include <iomanip.h>
#include <alloc.h>
#include <dir.h>
#include <string.h>
#include <iostream.h>
#include <PROCESS.H>
*/
#include "util.h"

#define MaxFunctions    50
#define MaxFunctionType 30
#define MaxCorrelations 50

struct FAZer
  {
   char *name;
   int NumPar;
   char *namepar[MaxFunctionType];
   virtual int GetNumPar(){return NumPar;};

   virtual double Clc(double *par,double Xval)=0;
   virtual void   Clc(double *par,double *Xval,double *Res)=0;
   virtual void ParStp(double *par,double *stp)=0;
   FAZer(){name=NULL;NumPar=0;};
   virtual void DeleteFAZer();
   virtual ~FAZer(){DeleteFAZer();};
  };

struct FAZerIO:FAZer
  {
   double *Par,*Stp;
   FAZerIO();
   ~FAZerIO();
   void DeleteFAZerIO();

   virtual double Clc(double *par,double Xval);
   virtual void   Clc(double *Xval,double *Res);
   virtual double Clc(double Xval);

   virtual void GetPar(double *Par);
   virtual void SetPar(double *Par);
   virtual void GetParStp(double *par,double *stp);
   virtual void SetParStp(double *par,double *stp);

   virtual void Output(ostream &output);
   virtual void Input(istream &input);
//  int Register(char *name);
  };

// If Sum Function does not have internal parameters then the only
// function to change is Clc()
struct FAArr:FAZerIO
  {
   FAZerIO *FuncArr[MaxFunctions+1];
   int NumParArr[MaxFunctions+1];
   int NumFunc;
   FAZerArrIO();
   virtual int GetNumPar(){return NumParArr[0];};

   virtual double Clc(double *par,double Xval);
   virtual void   Clc(double *Xval,double *Res);
   virtual double Clc(double Xval);

   virtual void GetPar(double *Par);
   virtual void SetPar(double *Par);
   virtual void GetParStp(double *par,double *stp);
   virtual void SetParStp(double *par,double *stp);

   virtual ~FAArr();
   void DeleteFAArr();
  };

int Register(char *name,FAZerIO &Func);

//=====================================================================
//=========================    FAZer          =========================
//=====================================================================


struct FAZerExp:FAZer
  {
   FAZerExp();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
   ~FAZerExp();
  };

struct FAZerLor:FAZer
  {
   FAZerLor();
   double Clc(double *par,double Xval);
   void Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
   ~FAZerLor();
  };

struct FAZerPoly:FAZer
  {
   FAZerPoly();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
   ~FAZerPoly();
  };

//=====================================================================
//=========================    FAArrAdd       =========================
//=====================================================================




struct FAArrAdd:FAArrIO
  {
   FAArrAdd();
   ~FAArrAdd();
   double Clc(double Xval);
   void   Clc(double *Xval,double *Res);
//   friend ostream& operator<<(ostream &output,FuncSumCellAdd &Func);
//   friend istream& operator>>(istream &input,FuncSumCellAdd &Func);
  };

struct FAArrSet:FAArrIO
  {
//   char *namepar[MaxFunctions+1];
   int correlatedpar[2][MaxCorrelations];
   int NumCor;
   double Bnd[MaxFunctions+1];
   double BndStp[MaxFunctions+1];
   FAArrSet();
   ~FAArrSet();
   int GetNumPar(){return NumParArr[0]+NumFunc-1;};
   double Clc(double Xval);
   void   Clc(double *Xval,double *Res);
   void SetPar(double *par);
   void SetParStp(double *par,double *stp);
   void GetPar(double *par);
   void GetParStp(double *par,double *stp);
   double *FormShf();
//   friend ostream& operator<<(ostream &output,FuncSumCellSet &Func);
//   friend istream& operator>>(istream &input,FuncSumCellSet &Func);
   void virtual Output(ostream &output);
   void virtual Input(istream &input);
  };

#endif






