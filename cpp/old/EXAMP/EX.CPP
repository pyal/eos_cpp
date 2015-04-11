#include "iostream.h"
//#include "alloc.h"
#include "Ros_equ.h"
#include "Quad.h"
#include "mtquad.h"

void ReadIn(istream &in,double &From,double &To,double &Step,double &RelErr,
            Exp6Param *&Par)
  {
   char tmp[256];
   double niu,Diameter;
   in>>tmp>>From>>tmp>>To>>tmp>>Step>>tmp>>RelErr>>tmp>>niu>>tmp>>Diameter>>tmp;
   Exp6Read(in,Par);
   double old;
   old=IntExp6Correlation(*Par,niu,Diameter);
   NumRecIntIterations=0;
   old=		RecInt(DoubleIntegral,0.1*StndErr,400,0.0001);

   cout<<" Old Clc "<<old<<" Num "<<NumRecIntIterations<<"\n";
  };
double CurFunc(double x)
  {
   double ret=DoubleIntegral(x);
//   double ret=MainIntegral(x);
//   double ret=PYfunc(x);
   if (x>200) ret=ret*4e4/sqr(x);
   return ret;
  };

double StndErr=1e-6;
void main(int argc,char *argv[])
 {
  cout<<" mem "<<Coreleft()<<"\n";
  fstream in(argv[1],ios::in);
  fstream out(argv[2],ios::out);
  double From,To,Step,Cur,RelErr,ResErr;
  int Err;
  Exp6Param *Par=NULL;
  ReadIn(in,From,To,Step,RelErr,Par);
//  From=300;
  Cur=From;
  while (Cur<To)
  {
   double tmp=Qags_IntSubInt(CurFunc,From,Cur,1e-6,RelErr,1000,ResErr,Err);
//   double tmp=Qag_OscilInt(CurFunc,From,Cur,1e-6,RelErr,10000,ResErr,Err);
   out<<FM<<Cur<<FMT<<CurFunc(Cur)<<FMT<<tmp<<FMT<<ResErr<<FMT<<NumberEvaluations<<"\n";
   out.flush();
   Cur+=Step;
  }
  cout<<" Qagi_InfinInt "<<Qagi_InfinInt(CurFunc,From,1,1e-6,RelErr,10000,ResErr,Err);
  cout<<" ResErr "<<ResErr<<" Err "<<Err<<" NumEval "<<NumberEvaluations<<"\n";
  cout<<" Qag_OscilInt "<<Qag_OscilInt(CurFunc,From,1000,1e-6,RelErr,10000,ResErr,Err);
  cout<<" ResErr "<<ResErr<<" Err "<<Err<<" NumEval "<<NumberEvaluations<<"\n";
  cout<<" Qags_IntSubInt "<<Qags_IntSubInt(CurFunc,From,1000,1e-6,RelErr,50000,ResErr,Err);
  cout<<" ResErr "<<ResErr<<" Err "<<Err<<" NumEval "<<NumberEvaluations<<"\n";
  cout<<" mem "<<Coreleft()<<"\n";
  out.close();
  delete Par;
 };
