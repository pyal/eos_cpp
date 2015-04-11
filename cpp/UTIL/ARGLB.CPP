
// uses grlib,memf,fobj,util;
//#include "util.h"
//#include "grlib.h"
//#include "memf.h"
#include <stdio.h>
#include <fstream.h>
#include <stdlib.h>
#include <iomanip.h>
#include "util.h"
#include "anal.h"
#include "analglb.h"
#define MaxCol 100
extern unsigned _stklen=32000;
/*
int EnterStr(istream &in,double *pnt,double MaxPnt)
  {  
   char s[256],*s1;s1=s;
   int ret=0; 
//   double d;
   do
      {
       if (MaxPnt<=ret) {line_feed(in);break;}
       ret++;
       s1=s;in>>s;
       if (!IsDouble(s1,pnt[ret])) {line_feed(in);ret=0;}
      }
   while (!seek_eoln(in));
   return ret;
  };

void WriteStr(fstream &out,double *pnt,double MaxPnt)
  {
   for (int k=1;k<=MaxPnt;k++) out<<setw(13)<<setprecision(6)<<pnt[k]<<"  ";
   if (MaxPnt>0) out<<"\n";
  };
*/
void GetWork(char *name,WorkDo *Wdo)
  {
   char s[256];
   fstream in(name,ios::in);
   in.getline(s,255);
   int k;
   FormWork(s,Wdo);
  }
int DoWork(double (&ar)[MAXFILEWORK][MAXROWWORK],WorkDo *act,double GetCol)
  {
   GetCol=DoMes(act,ar,GetCol);
   return GetCol;
  }
void main( int argc, char *argv[] )
  {
if (argc<=2) {cout<<" use :  \narglb get_files.. put_file calc"<<"\n";exit(1);}
//cout<<argc<<" "<<argv[1]<<argv[2]<<" mem "<<coreleft();
   WorkDo War[10];
   argc--;
   GetWork(argv[argc],War);
   int k,GetCol;
//cout<<" mem "<<coreleft();
   istream *in[MAXFILEWORK];
   for (k=1;k<=argc-2;k++)
      in[k]=(istream*)(new fstream(argv[k],ios::in));
   fstream out(argv[argc-1],ios::out);
   out<<setiosflags(ios::showpoint|ios::scientific|ios::left);
   double ar[MAXFILEWORK][MAXROWWORK];
   int arnum[MAXFILEWORK];
   do
     {
      for (k=1;k<=argc-2;k++)  arnum[k]=EnterStr(*in[k],ar[k],MaxCol);
      GetCol=DoWork(ar,War,arnum[1]);
      WriteStr(out,ar[1],GetCol);
     }
   while ( !seek_eof(*in[1]) );
   for (k=1;k<=argc-2;k++)  delete in[k];
  };
