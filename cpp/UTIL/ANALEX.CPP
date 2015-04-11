
#include "anal.h"
#include <iostream.h>
#include <stdlib.h>
#include <alloc.h>
extern unsigned _stklen=30000;
void main(int argc,char *argv[])
  {
cout<<Coreleft()<<" mem\n";
   Action act[100] ;
   int k;
   parsestr(argv[1],act,k);
    PackAct(act,k);
   double ar[3]={1,2,3};
   EvalAct(act,ar);
   cout<<ar[1]<<"\n";
cout<<"\n"<<coreleft()<<" mem\n";
  };

