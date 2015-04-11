
#include "tmstd.h"

double GetSysTime()
  {
   unsigned int a,b,d;
   asm {
   cli
   in  al,0x40             // read low byte timer count }
   mov bl,al
   in  al,0x40             // read high byte timer count }
   mov bh,al
   not bx
   inc bx                 // timer count in BX }
   mov  cx,0x40             //read time from BIOS data area}
   mov  es,cx              //INT 0x1A clears midnight flag!}
   mov  ax,es:[0x6C]        // low word time }
   mov  dx,es:[0x6E]        // high word time }
   sti
   mov a,ax
   mov b,bx
   mov d,dx
     }
   return (((double)d*256+(double)a)*256+(double)b)*50/59659;
  };
double RealTime(double Wait)
  {
cout<<" Wait = "<<Wait<<"\n";
   double Time=GetSysTime();
//   double tmp;
//   for (long int k=1;k<=Wait;k++) { tmp=log(2*exp(3)); }
   delay(Wait);
   double Delta=GetSysTime()-Time;
cout<<"Time before "<<Time<<"    "<<Delta<<"\n";
   return Delta;
  };

