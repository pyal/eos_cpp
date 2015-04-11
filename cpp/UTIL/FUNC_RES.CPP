//         Given a transition function
//         restore the true spectra from experimental data.
//         Method of random approcsimation. (from Kulish )

//#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream.h>
#include <iomanip.h>
#include <alloc.h>
#include <dir.h>
#include <string.h>
#include "fobj.h"
#include "util.h"
#include "grlib.h"
#include <alloc.h>
#include <iostream.h>

#define MaxRow 2048
#define ESC   283 
//#define USERWIDTH 4
//#define USE_H 50

extern unsigned _stklen=60000;
double USERWIDTH;
double USE_H;
double  ExtArrX[MaxRow+1], ExtArrY[MaxRow+1];
int iter;
GrScr *InitScreen(double *xarr,double *yarr,int Num)
  {
   InitGrph();
   GrArr *ar;
   ar=new GrArr(xarr,yarr,Num);
   ar->PixColor(YELLOW);
//   cout<<Num<<"\n";ChRead();
   GrScr *sum=AxGraph(ar,"  Time   ","  mV   ");
   sum->LstAdd(ar);
   sum->Show();
   return sum;
  };
int KeyPressed(GrScr *screen,GrArr *&ar,double *xarr,double *yarr,int Num)
  {  
   int Exit=0;
   if (!bioskey(1)) return Exit;
   if (ChRead()==ESC) Exit=1;
   if (ar!=NULL) {ar->Hide();delete ar;}
   movmem(xarr,ExtArrX,sizeof(ExtArrX));
   movmem(yarr,ExtArrY,sizeof(ExtArrY));
   ar=new GrArr(ExtArrX,ExtArrY,Num-1,screen->Scr,screen->Gr,GREEN);  
//   void SetGr(double x_low,double y_low,double x_up,double y_up);
//   void SetGr(Rect to);
   Rect gr=ar->ArrRect();
   ar->SetGr(ar->BlowRect(gr,ar->CentRect(gr),1.2));
   ar->Show();
   iter=1;
   return Exit;
  };
// ====================== Iterations Begin ===========================

double UserFunc(double x,double x0)
  { return 1/(sqr(x-x0)+sqr(USERWIDTH/2)); };
double MulFunc(double *xarr,double *cur_del,int pos)
  {
   double sum=0; 
   for (int k=1;k<=xarr[0];k++)
      sum+=cur_del[k]*UserFunc(xarr[k],xarr[pos]);
//cout<<"MULL "<<sum<<" Pos "<<pos<<"\n";ChRead();
   return sum;
  };  
void AddDelta(double *xarr,double *cur_rea,double *cur_del,
                                      double amp,int pos)
  {  
   if (cur_rea[pos]+amp<0) amp=-cur_rea[pos];
   cur_rea[pos]+=amp;
   for (int k=1;k<=xarr[0];k++)
      cur_del[k]-=amp*UserFunc(xarr[k],xarr[pos]);
  };
void StepApr(double *xarr,double *cur_rea,double *cur_del,int iter)
  {
   int pos;
   double amp;
//   pos=(int)((double)rand()/RAND_MAX*(xarr[0]-1))+1;
   pos=int(RandomClass::Rnd(xarr[0]-1))+1;
   if ((pos<1)||(pos>xarr[0])) {cout<<"EERRRROORR rand";pos=1;ChRead();}
//   amp=USE_H/iter*MulFunc(xarr,cur_del,pos);
   amp=USE_H/iter*cur_del[pos]*UserFunc(pos,pos);
   AddDelta(xarr,cur_rea,cur_del,amp,pos);
  };  
void SetYarr(double *yarr)
  {
   for (int k=1;k<=yarr[0];k++) yarr[k]=0;
  };
// ====================== Iterations End  ============================

void main(int argc,char *argv[])
  {
   if (registerbgidriver(EGAVGA_driver)<0) exit(1);
   double xarr[MaxRow+1],yarr[MaxRow+1],farr[MaxRow+1];
   int Num=0;
   GrArr *ar=NULL;
   GrScr *screen=NULL;
   fstream out(argv[2],ios::out);
   fstream in (argv[1],ios::in);
   in>>USERWIDTH>>USE_H;
   while ( (!seek_eof(in)) && (Num<MaxRow) ) 
     {Num++;in>>xarr[Num]>>yarr[Num];line_feed(in);}
//      cout<<xarr[Num]<<"  "<<yarr[Num]<<"\n";}   ChRead();
   yarr[0]=Num;xarr[0]=Num;farr[0]=Num;
   screen=InitScreen(xarr,yarr,Num);
   SetYarr(farr);
   int Exit=0;
   iter=1;
//   randomize();
   while (!Exit)
     {
      StepApr(xarr,farr,yarr,iter);
//      iter++;
      Exit=KeyPressed(screen,ar,xarr,farr,Num);
     }
   for (int k=1;k<=xarr[0];k++) 
      out<<xarr[k]<<"      "<<farr[k]<<"     "<<yarr[k]<<"\n";
   delete screen;
   delete ar;
   closegraph();
  };                        
