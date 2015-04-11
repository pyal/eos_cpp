#include "zero.h"
double func(double x) { return (x+25)*(x-5)*(x+10);}
    
void main()
  {  
   cout<<ZeroIn(-100,100,func,1e-4);
  };
