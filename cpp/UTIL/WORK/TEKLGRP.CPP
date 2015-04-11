#include "teklgrp.h"
//========================================================
//==================== IntArr:GrLine ======================
//========================================================
IntArr::IntArr(long n,ConvPar cnv)
 { 
   Hidden=1;  
   cvt=cnv;Num=n;
   Rect gr=ArrRect();
   SetGr(BlowRect(gr,CentRect(gr),1.2));
  };
void IntArr::Draw()
  {
   DrawLine(cvt(1,1),cvt(0,1),cvt(1,2),cvt(0,2));GrGoto(cvt(1,2),cvt(0,2));
   long Stp=ceil(Num/500)+1;
   for (long k=3;k<Num;k+=Stp) LineTo(cvt(1,k),cvt(0,k));
  };
void IntArr::GetArMax(int x,double &xmin,double &xmax)
  {
   double d;
   if (!x)
     {
      xmin=MathInf,xmax=-MathInf;
      for (long k=1;k<=Num;k++)
        {
         d=cvt(x,k);
         if (xmin>d) xmin=d;if (xmax<d) xmax=d;
        };
     }
   else {xmin=cvt(x,1);xmax=cvt(x,Num);}
   if (xmax<xmin) {d=xmax;xmax=xmin;xmin=d;}
  };
void IntArr::Show()
  {
   if ((!Hidden) || (Num<2)) return;
   setwritemode(COPY_PUT);
   Draw();
   Hidden=0;
  };
void IntArr::Hide()
  {
   cleardevice();
   Hidden=1;
  };
Rect IntArr::ArrRect()
  {
   Rect ret;
   GetArMax(1,ret.dn.x,ret.up.x);        
   GetArMax(0,ret.dn.y,ret.up.y);        
   if (ret.up.x-ret.dn.x<MathZer)  { ret.up.x+=1; ret.dn.x-=1; }
   if (ret.up.y-ret.dn.y<MathZer)  { ret.up.y+=1; ret.dn.y-=1; }
   return ret;
  };
void IntArr::ArrReset(long n,ConvPar cnv)
  { 
   Hide();
   cvt=cnv;Num=n;
   Rect gr=ArrRect();
   SetGr(BlowRect(gr,CentRect(gr),1.2));
  };








