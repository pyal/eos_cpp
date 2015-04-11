#ifndef __TEKLGRP_H
#define __TEKLGRP_H


#include "grlib.h"

struct ConvPar
  {
   double ax,bx,ay,by;
   int huge *buf;
   ConvPar(){buf=NULL;ax=1;bx=1;ay=1;by=1;};
   ConvPar(int huge *b,double x_sh,double x_cnt,double y_sh,double y_cnt)
     { bx=x_sh;ax=x_cnt;by=y_sh;ay=y_cnt;buf=b;};
   void ResetConv(int huge *b,double x_sh,double x_cnt,double y_sh,double y_cnt)
     { bx=x_sh;ax=x_cnt;by=y_sh;ay=y_cnt;buf=b;};
   double operator()(long x,long n)
     {if (!x) return (double)(buf[n])*ay+by;
      return (double)n*ax+bx;};
   ~ConvPar(){buf=NULL;};
    ConvPar& operator=(ConvPar &cnv)
      {
       ResetConv(cnv.buf,cnv.bx,cnv.ax,cnv.by,cnv.ay);
       return *this;
      };
  };
struct IntArr:GrLine
  {  
   long Num; 
   ConvPar cvt;
   IntArr() { Num=0;Hidden=1;};
   IntArr(long n,ConvPar cnv);
   void ArrReset(long n,ConvPar cnv);
   void Draw();
   ~IntArr(){};// { Hide();};
   void Show();
   void Hide();
   Rect ArrRect();
   void GetArMax(int x,double &xmin,double &xmax);
  };
#endif
