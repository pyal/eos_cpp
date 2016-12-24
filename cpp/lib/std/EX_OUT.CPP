#include <lib/precompiled/std.h>


#include "ex_out.h"

int InputPar(fstream &in,double &from,double &min,double &max,
                                                 double &stp,char *write)
  {
   int ret=0;
   in>>write>>from>>write>>min>>write>>max>>write>>stp;in.getline(write,255);
   in>>write>>ret>>write;
   return ret;
  };




/*
double StndErr;
extern unsigned _stklen=64000;

void main(int argc,char *argv[])
  {
#ifdef WCPP
   double elapsed_time;
   clock_t start, finish;start=clock();
#endif

   fstream in(argv[1],ios::in);
   char tmp[512];
   double u2=0,r2,p2=0,e2=0,D=0,porous;
   double u1,r1,p1,e1;
   StndErr=1e-6;
   if (!MatterRegister(in,Matter)) { cout<<"Could not register. Exit.\n";exit(0);}
cout<<" Coreleft() "<<Coreleft()<<" Stack "<<StackAvail()<<"\n";

   in>>tmp>>u1>>tmp>>r1>>tmp>>p1>>tmp>>e1>>tmp>>porous>>tmp>>StndErr;r2=r1;

#ifdef InternalPointsUrs  
double IntPar=0;Matter->SetIntPar(&IntPar);Matter->ChangeIntPar(r1,e1,1);
#endif

   MatterAdiabat=Matter; p1=PressurePorous(r1,e1);

#ifdef WCPP
  finish=clock();elapsed_time=finish-start;start=finish;
cout<<" RunTime "<<elapsed_time<<"\n";
#endif
cout<<"ShowHugPres     beg    Coreleft() "<<Coreleft()<<"\n";
   ShowHugPres(in,u1,r1,p1,e1,u2,r2,p2,e2,D);
#ifdef WCPP
  finish=clock();elapsed_time=finish-start;start=finish;
cout<<" RunTime "<<elapsed_time<<"\n";
#endif
cout<<"ShowHugDenc    beg    Coreleft() "<<Coreleft()<<"\n";
   ShowHugDenc(in,u1,r1,p1,e1,u2,r2,p2,e2,D);

#ifdef WCPP
  finish=clock();elapsed_time=finish-start;start=finish;
cout<<" RunTime "<<elapsed_time<<"\n";
#endif

  };

*/




