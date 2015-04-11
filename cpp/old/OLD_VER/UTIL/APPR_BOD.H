#ifndef __APPR_BOD_H
#define __APPR_BOD_H
//#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream.h>
#include <iomanip.h>
#include <alloc.h>
#include <dir.h>
#include <string.h>
#include <iostream.h>
#include "util.h"
#include <PROCESS.H>
//#include "grlib.h"
#include "matrics.h"
#include "matlib.h"
#include "eigen_ma.h"
#include "myappr.h"
#include "funcobj.h"

#define MaxFunctions   50

typedef double (*FUNC_LONG) (double *par,double Xval);
//typedef double *DoublePtr;
//typedef DoublePtr TwoDim[MaxFunctions+1];
void Get_Theo(double *par,double *res);
double Hi_Func(double *par);
double min_f(double x,double y,X_func Func,double Tol);
int GetFunction(char *Name,FUNC_LONG &func);
void  GetStep(char *NameF,double *par,double *stp);
void  Show(char *arguments[]);
struct ExternHiStruc
  {
   double *Xval;
   double *Yval;
   int NumIt,NumT,Hi_Dgr;
   double hi,MinError;
   double FrVel,ToVel;
   FuncSum FuncObj;
  };

struct AprStrc
  {
//   double *stp;
   AprStrc(char *NameF,double *x,double *y,fstream &in);
   ~AprStrc();
   void Calc();
  };
        
struct OutParStrc
  {
   int ColFr,ColTo,NumRow,FirstRow;
   char nameout[256],namein[256];
  };



#endif
