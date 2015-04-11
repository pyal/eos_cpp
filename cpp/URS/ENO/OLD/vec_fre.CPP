//#include "util.h"

#include "vec_fre.h"
/*
VecCl operator*(VecCl &vec,double coef)
  { VecCl ret=vec;
    if (fabs(coef)>MathZer){for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]*=coef;}
    else  { for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]=0;}
    return ret; };
VecCl operator/(VecCl &vec,double coef)
  { VecCl ret=vec;
    if (fabs(coef)>MathZer){for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]/=coef;}
    else  { for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]*=MathInf;
	    cout<<"Error......... Zero Division in VecCl/n";}
    return ret; };
VecCl operator+(VecCl &vec,double cons)
  { VecCl ret=vec;for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]+=cons;
    return ret;};
double operator*(VecCl &vec,VecCl &vec1)
  { double s=0;for (int k=1;k<=vec.Ptr[0];k++) s+=vec.Ptr[k]*vec1.Ptr[k];
    return s; };
VecCl operator+(VecCl &vec,VecCl &vec1)
  { VecCl ret=vec;for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]+=vec1.Ptr[k];
    return ret;};
VecCl operator-(VecCl &vec,VecCl &vec1)
  { VecCl ret=vec;for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]-=vec1.Ptr[k];
    return ret;};
ostream& operator<<(ostream &output,VecCl &vec)
  { for (int k=1;k<=vec.Ptr[0];k++) output<<vec.Ptr[k]<<"   ";
   output<<"\n";
   return output;};
istream& operator>>(istream &input,VecCl &vec)
  { for (int k=1;k<=vec.Ptr[0];k++) input>>vec.Ptr[k];
   return input;};
double ModMax(VecCl &Vec,double NewMax)   // ModMax - fabs( Max(Vec[k]) )
  {
   double s=-1,*pnt=Vec.Ptr;
   int N=pnt[0];
   for (int k=1;k<=N;k++) if (s<fabs(pnt[k])) s=fabs(pnt[k]);
   if (fabs(s)<MathZer) s=0;
   if ((NewMax!=0) && (s!=0))
     {
      s=NewMax/s;
      for (int k=1;k<=N;k++) pnt[k]*=s;
      s=NewMax;
     }
   return s;
  };
*/






