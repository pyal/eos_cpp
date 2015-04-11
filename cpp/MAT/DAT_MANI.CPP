#include <lib\precompiled\math.h>
#include "dat_mani.h"

void ConstructSpl(TData<double> &XY,int X,int Y,TData<double> *&S)
  {
   int i[3]={XY.I[0],XY.I[0],XY.I[0]};
   DataRegister("TDataF",S);S->SetDim(3,i);
   Spline(XY.I[0],&(XY.D[X][-1]),&(XY.D[Y][-1]),
          &(S->D[0][-1]),&(S->D[1][-1]),&(S->D[2][-1]));
//cout<<" i "<<i[0]<<" Spline OK!!\n";ChRead();
  };        
   
double EvalSpl(TData<double> &XY,int X,int Y,TData<double> &S,double x_v)
  {
//cout<<" x_V "<<x_v<<"\n";
   int Imin=0,Imax=XY.I[X]-1;
   double V1=XY.D[X][Imin],V2=XY.D[X][Imax],tmp;
   if (V1>V2) { tmp=V2;V2=V1;V1=tmp;Imin=Imax;Imax=0; }
   if (x_v>V2)  return XY.D[Y][Imax];if (x_v<V1)  return XY.D[Y][Imin];
   return Seval(XY.I[0],x_v,&(XY.D[X][-1]),&(XY.D[Y][-1]),
                            &(S.D[0][-1]),&(S.D[1][-1]),&(S.D[2][-1]));};
          
