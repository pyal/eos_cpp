#include <lib/precompiled/data.h>


#include "DataTyp.h"

double StndErr=1e-6;

int In_Lim(SpacePoint &Pnt,SpaceBound &Rect,int IncludeBound)
 {
  int k,N=Pnt.SetDim(),ret=1;
  for (k=0;k<N;k++) 
    if (!In_Lim(Pnt[k],Rect.Low[k],Rect.Up[k],IncludeBound) ) {ret=0;break;}
  return ret;
 };

//int SamePoint(SpacePoint &Pnt1,SpacePoint &Pnt2,double Accuracy=StndErr,
//              SpacePoint *DirWeigths=NULL)
int SamePoint(SpacePoint &Pnt1,SpacePoint &Pnt2,double Accuracy,
              SpacePoint *DirWeigths)
 {
  int N=Pnt1.SetDim(),k,ret=1;
  Ref<SpacePoint> RefWeight= DirWeigths;
  if (RefWeight==NULL) { RefWeight=new SpacePoint(N);(*RefWeight)=1.;}
  (*RefWeight)=(*RefWeight)*Accuracy;
  for (k=0;k<N;k++) 
    if (!In_Lim(Pnt1[k],Pnt2[k]+fabs(*RefWeight[k]),Pnt2[k]-fabs(*RefWeight[k]),1) )
     {ret=0;break;}
  return ret;

  return 1;
 };

int IndBound::NumPnt()
 {
   IndexPoint dim;dim=Size();
   int k,N=dim.SetDim();
   if (N<=0) return 0;
   int NumPnt=dim[0];
   for (k=1;k<N;k++) NumPnt*=dim[k];
   return NumPnt;
 }
IndexPoint IndBound::Int2Pnt(int num)
 {
   IndexPoint dim=Size(),pnt=dim;
   int k,N=dim.SetDim(),pos=num;
   for (k=1;k<N;k++) {dim[k]=dim[k-1]*dim[k];}
   for (k=N-1;k>=1;k--) 
    {pnt[k]=floor((double)pos/dim[k-1]);
     pos-=pnt[k]*dim[k-1];}
   pnt[0]=pos;
   return pnt+Low;
 }
int IndBound::Pnt2Int(const IndexPoint &ind)
 {
   IndexPoint dim=Size(),work=(IndexPoint)ind-(IndexPoint)Low;
   int k,N=dim.SetDim();
   if (N<=0) return -1;
   int pos=work[0],coef=dim[0];
   for (k=0;k<N;k++) if ((work[k]<0) || (work[k]>=dim[k])) return -1;
   for (k=1;k<N;k++) {pos+=work[k]*coef;coef*=dim[k];}
   return pos;
  };
