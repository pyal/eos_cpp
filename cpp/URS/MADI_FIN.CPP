#include "matt_adi.h"
#include "matt_h2.h"
#include "util.h"
#include <alloc.h>

#define MaxMatter 10

double  StndErr;


extern unsigned _stklen=64000;

MatterIO *Matter[MaxMatter];
double Par[MaxMatter][7];
double ParAfter[MaxMatter][7];
int CurMatter;

//MatterIO *MatterAdiabat;


void InputPar(fstream &in,int &N)
  {
   char tmp[256];
   in>>tmp>>N;
//cout<<" name "<<tmp<<" NumMatter "<<N<<"\n";
   for (int k=0;k<N;k++)
     {
      if (!MatterRegister(in,Matter[k])) 
        { cout<<"Could not register. Exit.\n";exit(0);}
      for (int i=0;i<7;i++) in>>tmp>>Par[k][i];
     }
  };
void OutputPar(fstream &out,int &N)
  {
   char tmp[7]="urpeltd";
   out<<"NumMatter "<<N<<"\n";
   for (int k=0;k<N;k++)
     {
      if (!MatterStore(out,Matter[k])) 
        { cout<<"Could not store. Exit.\n";exit(0);}
      for (int i=0;i<7;i++) out<<tmp[i]<<"  "<<Par     [k][i]<<"  ";out<<"\n";
      for (    i=0;i<7;i++) out<<tmp[i]<<"  "<<ParAfter[k][i]<<"  ";out<<"\n";
      out<<"\n";
     }
  };
//      u1   r1  p1    e1   l1   t1   d1;
//      0    1    2    3     4   5     6

void HugArr(double *in,double *ou,double x)
  {
   double tmp[7];movmem(ou,tmp,sizeof(double)*7);
   if (fabs(in[0]-x)<StndErr) {movmem(in,ou,sizeof(double)*7);return ;}
   tmp[0]=x;
//cout<<fabs(in[0]-x)<<" StndErr "<<StndErr<<" coreleft "<<coreleft()<<"\n";
   char ch[7]="urpeltd";
//   for (int i=0;i<7;i++) cout<<ch[i]<<"  "<<in[i]<<"  ";cout<<" X "<<x<<"\n";
//   for (    i=0;i<7;i++) cout<<ch[i]<<"  "<<tmp[i]<<"  ";cout<<"\n";
//   ChRead();
   Hug(in[0],in[1],in[2],in[3],tmp[0],tmp[1],tmp[2],tmp[3],tmp[6]);
//   for (    i=0;i<7;i++) cout<<ch[i]<<"  "<<tmp[i]<<"  ";cout<<"\n\n";
   movmem(tmp,ou,sizeof(double)*7);
  } 
double Misfit(double x)
  {
   MatterAdiabat=Matter[CurMatter];
   double res0[7],res1[7];
   movmem(Par[CurMatter],res0,sizeof(double)*7) ;
   movmem(Par[CurMatter+1],res1,sizeof(double)*7) ;
   HugArr(Par[CurMatter],res0,x);
   MatterAdiabat=Matter[CurMatter+1];
   Par[CurMatter+1][0]*=-1;
   x=-x;
   HugArr(Par[CurMatter+1],res1,x);
   Par[CurMatter+1][0]*=-1;
   x=-x;
//cout<<"Misfit "<<res0[2]-res1[2]<<"\n";
//cout<<" MidV "<<x;
   return res0[2]-res1[2];
  } 

//int CurMatter;

double AdiMisfit(double u)
  {
   double Beg;
   int k=CurMatter;
   Par[k][0]=u;
   if (fabs(Par[k][0]-Par[k+1][0])+fabs(Par[k][2]-Par[k+1][2])<100*StndErr) return 0;
   Beg=(Par[k][0]+Par[k+1][0])/2;
      if (!ZeroNewtonIt(Misfit,Beg,StndErr,1000)) 
         { cout<<"Could not find match. Halt.";exit(1);}
//cout<<" AdiMisfit(double u) "<<u<<" mis ";
   Par[k+1][0]*=-1;
   MatterAdiabat=Matter[k+1];
   HugArr(Par[k+1],ParAfter[k+1],-Beg);
   Par[k+1][0]*=-1;
   double ret=ParAfter[k+1][6]-Par[k+1][6];
//cout<<ret<<" To_Find "<<Par[k+1][6]<<"\n";
//    movmem(Par,ParAfter,sizeof(double)*7*(k+2));
   return ret;
  };

void ClcAdi(int N)
  {
   CurMatter=N;
   int k=N;
   double Beg=5;
      double tmp=Par[k][0];
      double M=Par[k][1]*Par[k][4];

      if (!ZeroNewtonIt(AdiMisfit,Beg,StndErr,1000)) 
         { cout<<"Could not find match. Halt.";exit(1);}
      Par[k][0]=Beg;    ParAfter[k][0]=Beg;
//    movmem(Par,ParAfter,sizeof(double)*7*(N+2));
cout<<" Found_Vel "<<Beg<<"\n";
   Beg=(Par[k][0]+Par[k+1][0])/2;
      if (!ZeroNewtonIt(Misfit,Beg,StndErr,1000)) 
         { cout<<"Could not find match. Halt.";exit(1);}
      MatterAdiabat=Matter[k];
      HugArr(Par[k],Par[k],Beg);
//cout<<" RESULT D "<<Par[k][6]<<" Beg V "<<tmp<<" Medium "<<Beg;
//cout<<" coreleft "<<coreleft()<<"\n";ChRead();
      Par[k][5]=Par[k][4]/Par[k][6];Par[k][4]=M/Par[k][1];
      M=Par[k+1][1]*Par[k+1][4];
   Par[k+1][0]*=-1;
      MatterAdiabat=Matter[k+1];
      HugArr(Par[k+1],ParAfter[k+1],-Beg);
   Par[k+1][0]*=-1;
      ParAfter[k+1][0]=-ParAfter[k+1][0];
//      ParAfter[k+1][6]=ParAfter[k+1][6]+Par[k+1][0];
//cout<<" RESULT D "<<Par[k][6]<<" Beg V "<<tmp<<" Medium "<<Beg;
//cout<<" coreleftf "<<coreleft()<<"\n";ChRead();
      ParAfter[k+1][5]=ParAfter[k+1][4]/ParAfter[k+1][6];
      ParAfter[k+1][4]=M/ParAfter[k+1][1];
  };    

void main(int argc,char *argv[])
  {
   StndErr=1e-6;
//      u1   r1  p1    e1   l1   t1   d1;
//      1    2    3     4   5     6   7
   int N,N_To_Find;
   char tmp[256];
   double Time=0;
   fstream in(argv[1],ios::in);
   if (argc<=1)
     {
      cout<<
" usage \n"<<
"  madi_fin.exe conf out_file [D_to_use] \n"<<
" NumMatterWithGivenD 1   NumMatters 2                   \n"<<
"                                              \n"<<
"MatterABu   Material_File material.cfg Substance Steel-Tern   \n"<<
"u  5.2  r  7.89  p  0  e  0  l  1  t  1  d  1                 \n"<<
"                                                              \n"<<
"MatterABu   Material_File material.cfg Substance Z-Al203_     \n"<<
"u  0  r  3.985  p  0  e  0  l  1  t  1  d  10                 \n";
     exit(0);
     }
   fstream out(argv[2],ios::out);
   in>>tmp>>N_To_Find;N_To_Find-=2;
   InputPar(in,N);	in.close();
   
   if (argc>3) Par[N_To_Find+1][6]=atof(argv[3]);
//cout<<" argc "<<argc<<" val "<<Par[0][0]<<"\n";
//   OutputPar(out,N);
    movmem(Par,ParAfter,sizeof(double)*7*N);
    ClcAdi(N_To_Find); 
    OutputPar(out,N);
   out.close();
  };
/*
 usage 
  madi_sum.exe conf out_file [Velocity_First]
 MatterToWork 1   NumMatter 2

MatterABu   Material_File material.cfg Substance Steel-Tern
u  2.977348  r  11.008019  p  137.610243  e  2.470095  l  0.71675  t  0.127438  d  7.846969  
u  5.2  r  7.89  p  0  e  0  l  1  t  1  d  1  

MatterABu   Material_File material.cfg Substance Z-Al203_
u  0  r  3.985  p  0  e  0  l  1  t  1  d  1  
u  2.977348  r  5.361275  p  137.610243  e  4.432307  l  0.743293  t  0.08622  d  11.598252  

*/
