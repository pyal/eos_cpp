// uses util;
#ifndef __MINFUNC_H
#define __MINFUNC_H

#include "const.h"
#include "deffunc.h"
//#include "util.h"
//#include <math.h>

//=====================================================================
//===============         MinFunc           ===========================
//=====================================================================

struct ClcMinStrc
  {
   double lft,rgt,*clcx,Flft,Frgt,*Fclcx;
   double GoldCoef,MachZer,ClcZer;
   double ZerStp;
   double MinCurr;
   X_func func;
   ClcMinStrc();
   ~ClcMinStrc(){delete clcx;delete Fclcx;};
   double MinGoldSqr(double A,double B,X_func yfunc,double Tol);
   double MinNumGoldSqr(double A,double B,X_func yfunc,double Tol);
   double MinSqr(double A,double B,X_func yfunc,double Tol);
   double MinGold(double A,double B,X_func yfunc,double Tol);
   private:
   void SetBeg(double A,double B,X_func yfunc,double Tol);
   void GoldStp();
   void SqrStp();
   double CmpDelMin(double mainx,double newx,double Zer);
   double MinSqrAppr(double x1,double x2,double x3,
		     double y1,double y2,double y3);
   void NextBound();
   void SetBnd(double x1,double x2,double x3,double y1,double y2,double y3);
  };
static ClcMinStrc *MinStrc=NULL;

double MinFunc(double x,double y,X_func Func,double Tol);
double MinGold(double x,double y,X_func Func,double Tol);
double MinSqr(double x,double y,X_func Func,double Tol);
void DelMinFunc();

#endif
