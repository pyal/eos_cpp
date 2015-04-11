#include "aut_fac.h"
#ifndef WCPP
extern unsigned _stklen=64000;
#endif
double StndErr;

#ifdef WCPP
#define ESC 27
#else
#define ESC 283
#endif

int ReadInput(istream &in,ostream &out,AutIO &reg,double &EndTime,double &CurTime,double &TimeWrite) 
  {
   char tmp[256];
   in>>tmp>>EndTime>>tmp>>CurTime>>tmp>>TimeWrite;in.getline(tmp,256);
   if (!reg.ReadIni(in)) return 0;
   out<<" EndTime "<<EndTime<<" BegTime "<<CurTime<<" TimeWrite "<<TimeWrite<<"\n";
   reg.SaveIni(out);
   while (!seek_eof(in))
       { reg.ReadPar(in);if (!reg.BadPar) reg.SavePar(out);else break;};
   if (!reg.BadPar) {CurTime=reg.CurTime;FormClc(reg);return 1;}
   return 0;
  };

void main(int argc,char *argv[])
  {
   argc--;
   StndErr=1e-6;
//cout<<" argc "<<argc<<"\n";
   if (argc<=1)
     { cout<<" usage \n"<<"      aut_ite.exe conf out_file \n"<<"\n";exit(0); }
   AutIO reg;
   fstream in(argv[1],ios::in),out(argv[2],ios::out);
   int FstIter=0;
   double TimeStp,EndTime,CurTime,TimeWrite,NewTimeWrite;
   if (!ReadInput(in,out,reg,EndTime,CurTime,TimeWrite)) 
       { cout<<" Could not read. Exit.\n";exit(0); }

   int BreakSignal=0;
   double elapsed_time;
   clock_t start, finish;start=clock();
   while ((CurTime<EndTime) && (!BreakSignal))
     {
      NewTimeWrite=min(CurTime+TimeWrite,EndTime);
      ClcTim(NewTimeWrite,reg);
      reg.SavePar(out);CurTime=reg.CurTime;
cout<<" Write !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
cout<<" CurTime "<<CurTime<<"\n";     
#ifdef WCPP
		 if (_kbhit()) { BreakSignal=_getch();if (BreakSignal==ESC) break;}
#else
         while ((BreakSignal=bioskey(1))!=0) if (BreakSignal==ESC) break;else {cout<<" "<<BreakSignal;bioskey(0);}
#endif
     }
  finish=clock();elapsed_time=(finish-start);//CLK_TCK;
cout<<" RunTime "<<elapsed_time<<"\n";
cout<<" Coreleft "<<Coreleft()<<"\n";
   out.close();
  };
 