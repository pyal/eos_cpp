#include "zero.h"
#include <fstream.h>
#include "util.h"
int n;
double *ar;
double Body(double x)
  {
   double c=1,s=0;
   for (int k=0;k<=n-1;k++) {s+=c*ar[k];c*=x;}
   return s;
  };
void main(int argc,char *argv[])
  {
   fstream in(argv[1],ios::in);
   fstream out(argv[2],ios::out);
   char tmp[256];
   double Xbeg,Xfr,Xto,Xstp;
   in.getline(tmp,255);
   in>>n;ar=new double [n];
   for (int k=0;k<n;k++) in>>ar[k];
   in>>Xbeg>>Xfr>>Xto>>Xstp;
   ZeroMullerIt(Body,Xbeg,1e-6,1000);
cout<<" X Zero = "<<Xbeg<<"   Value "<<Body(Xbeg)<<"\n";
   for (double x=Xfr;x<=Xto;x+=Xstp) out<<x<<"     "<<Body(x)<<"\n";
  }
