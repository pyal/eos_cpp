
#include "funcobj.h"
extern unsigned _stklen=50000;

void main( int argc, char *argv[] )
  {
cout<<" Mem in the begin "<<coreleft()<<"\n";
   FAIO *work_func=NULL;
//   FuncSum *work_func=new FuncSum;
   fstream *in=new fstream(argv[1],ios::in);
   fstream *out=new fstream(argv[2],ios::out);
cout<<" Mem after file open "<<coreleft()<<"\n";
   FARegister(*in,work_func);
//   (*in)>>*work_func;
cout<<" Mem after work init "<<coreleft()<<"\n";
   int N=work_func->GetNumPar();
   cout<<" Number of param "<<N<<"\n";
   double *par=new double[N+1];
   double *stp=new double[N+1];
   work_func->GetParStp(par,stp);
//   work_func->SetParStp(par,stp);
//   work_func->GetParStp(par,stp);
cout<<" Mem after par init "<<coreleft()<<"\n";
   for (int k=1;k<=N;k++)
     {
      cout<<" parameter ["<<k<<"]="<<par[k];
      cout<<" step      ["<<k<<"]="<<stp[k]<<"\n";
     }
   for (k=1;k<=100;k++) *out<<k<<"   "<<work_func->Clc(k)<<"\n";

   work_func->Output(*out);
   delete par;
   delete stp;
cout<<" Mem par delete"<<coreleft()<<"\n";
   delete work_func;
cout<<" Mem func delete"<<coreleft()<<"\n";
   delete out;
   delete in;
cout<<" Mem after file del "<<coreleft()<<"\n";
  };
   
