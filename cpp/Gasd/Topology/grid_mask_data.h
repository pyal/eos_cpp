#ifndef Grid_Mask_Data_h
#define Grid_Mask_Data_h 1


//struct GridMask;
#include "lib\ref\ref.h"

template <class T,class CpM>
struct GridField;

template <class T,class CpM=CopyStructFast<T> >
struct MaskedData
 {
  Ref<GridField<T,CpM> > vec;
  Ref<GridMask> mask;
  int ToDeleteVec;
  ~MaskedData(){if ((ToDeleteVec) && (vec->GetGridManip())) vec->GetGridManip()->DelVar(vec.pointer());}//vec=NULL;}//vec->GetGridManip(NULL);}
// Create new stand alone dat,vec
  MaskedData(GridField<T,CpM> *dat,GridMask *m);
// Do not create vec,mask, make simple ref - used in copy
  MaskedData(const MaskedData<T,CpM> &dat){vec=dat.vec;mask=dat.mask;ToDeleteVec=0;}
// Create new stand alone mask
  MaskedData(GridField<T,CpM> *dat,const GridMask &m);

// Sets points which are good in new and old mask
  MaskedData<T,CpM>& operator = (const MaskedData<T,CpM> &dat) ;
//==============================================================
//======= Operates  only on good points MaskedData +-* / T    ===
//==============================================================
  MaskedData<T,CpM> operator * (const T& dat);
  MaskedData<T,CpM> operator / (const T& dat);
  MaskedData<T,CpM> operator + (const T& dat);
  MaskedData<T,CpM> operator - (const T& dat);
//==============================================================
//================ End Operations  MaskedData +-* / T   =========
//==============================================================

//==============================================================
// Operates  only on both good points MaskedData +*-/T MaskedData
//==============================================================
  MaskedData<T,CpM> operator + (const MaskedData<T,CpM> &dat);
  MaskedData<T,CpM> operator - (const MaskedData<T,CpM> &dat);
  MaskedData<T,CpM> operator * (const MaskedData<T,CpM> &dat);
  MaskedData<T,CpM> operator / (const MaskedData<T,CpM> &dat);
//==============================================================
//================ End Operations  MaskedData +*-/T   =========
//==============================================================


//==============================================================
//======= Operates  only on good points MaskedData +*-/T    ===
//==============================================================
  MaskedData<T,CpM>& operator *= (const T& dat);
  MaskedData<T,CpM>& operator /= (const T& dat);
  MaskedData<T,CpM>& operator += (const T& dat);
  MaskedData<T,CpM>& operator -= (const T& dat);
//==============================================================
//================ End Operations  MaskedData +*-/T   =========
//==============================================================
//==============================================================
// Operates  only on both good points MaskedData +*-/T MaskedData
//==============================================================
  MaskedData<T,CpM>& operator *= (const MaskedData<T,CpM> &dat);
  MaskedData<T,CpM>& operator /= (const MaskedData<T,CpM> &dat);
  MaskedData<T,CpM>& operator += (const MaskedData<T,CpM> &dat);
  MaskedData<T,CpM>& operator -= (const MaskedData<T,CpM> &dat);
//==============================================================
//================ End Operations  MaskedData +*-/T   =========
//==============================================================



 };
 



#include "Grid_Interface.h"

//=================================================================

template <class T,class CpM >
MaskedData<T,CpM>::MaskedData(GridField<T,CpM> *dat,GridMask *m)
 {
  ToDeleteVec=1;
  vec=new GridField<T,CpM>(dat->GetGridManip());mask=new GridMask;
  *vec=*dat;*mask=*m;
 }
template <class T,class CpM >
MaskedData<T,CpM>::MaskedData(GridField<T,CpM> *dat,const GridMask &m)
 {  ToDeleteVec=0;vec=dat;mask=new GridMask;*mask=m;}

template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator = (const MaskedData<T,CpM> &dat) 
 {
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]=src[i1];
  return *this;
 };
template <class T,class CpM >
MaskedData<T,CpM> MaskedData<T,CpM>::operator * (const T& dat)
 {
  MaskedData<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim();
  GridIndex i;
  T* dst=(T*)ret.vec->data;
  GridIndex *m=(GridIndex*)*mask;
  for (k=0;k<N;k++) if ((i=m[k])>=0) dst[i]*=dat;
  return ret;
 };
template <class T,class CpM >
MaskedData<T,CpM> MaskedData<T,CpM>::operator / (const T& dat)
 {
  MaskedData<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim();
  GridIndex i;
  T* dst=(T*)(ret.vec->data);
  GridIndex *m=(GridIndex*)(*mask);
  for (k=0;k<N;k++) 
   if ((i=m[k])>=0) dst[i]/=dat;
  return ret;
 };
template <class T,class CpM >
MaskedData<T,CpM> MaskedData<T,CpM>::operator + (const T& dat)
 {
  MaskedData<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim();
  GridIndex i;
  T* dst=(T*)ret.vec->data;
  GridIndex *m=(GridIndex*)*mask;
  for (k=0;k<N;k++) if ((i=m[k])>=0) dst[i]+=dat;
  return ret;
 };
template <class T,class CpM >
MaskedData<T,CpM> MaskedData<T,CpM>::operator - (const T& dat)
 {
  MaskedData<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim();
  GridIndex i;
  T* dst=(T*)ret.vec->data;
  GridIndex *m=(GridIndex*)*mask;
  for (k=0;k<N;k++) if ((i=m[k])>=0) dst[i]-=dat;
  return ret;
 };
template <class T,class CpM >
MaskedData<T,CpM> MaskedData<T,CpM>::operator + (const MaskedData<T,CpM> &dat) 
 {
  MaskedData<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)ret.vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]+=src[i1];
  return ret;
 };
template <class T,class CpM >
MaskedData<T,CpM> MaskedData<T,CpM>::operator - (const MaskedData<T,CpM> &dat) 
 {
  MaskedData<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)ret.vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]-=src[i1];
  return ret;
 };
template <class T,class CpM >
MaskedData<T,CpM> MaskedData<T,CpM>::operator * (const MaskedData<T,CpM> &dat) 
 {
  MaskedData<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)ret.vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]*=src[i1];
  return ret;
 };
template <class T,class CpM >
MaskedData<T,CpM> MaskedData<T,CpM>::operator / (const MaskedData<T,CpM> &dat) 
 {
  MaskedData<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)ret.vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]/=src[i1];
  return ret;
 };

//=============================================================================
template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator *= (const T& dat)
 {
  int k,N=mask->SetDim();
  GridIndex i;
  T* dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)*mask;
  for (k=0;k<N;k++) if ((i=m[k])>=0) dst[i]*=dat;
  return *this;
 };

template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator /= (const T& dat)
 {
  int k,N=mask->SetDim();
  GridIndex i;
  T* dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)*mask;
  for (k=0;k<N;k++) if ((i=m[k])>=0) dst[i]/=dat;
  return *this;
 };

template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator += (const T& dat)
 {
  int k,N=mask->SetDim();
  GridIndex i;
  T* dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)*mask;
  for (k=0;k<N;k++) if ((i=m[k])>=0) dst[i]+=dat;
  return *this;
 };

template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator -= (const T& dat)
 {
  int k,N=mask->SetDim();
  GridIndex i;
  T* dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)*mask;
  for (k=0;k<N;k++) if ((i=m[k])>=0) dst[i]-=dat;
  return *this;
 };

//=============================================================================

template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator *= (const MaskedData<T,CpM> &dat) 
 {
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator += . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]*=src[i1];
  return *this;
 };

template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator /= (const MaskedData<T,CpM> &dat) 
 {
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator += . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]/=src[i1];
  return *this;
 };

template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator += (const MaskedData<T,CpM> &dat) 
 {
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator += . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]+=src[i1];
  return *this;
 };

template <class T,class CpM >
MaskedData<T,CpM>& MaskedData<T,CpM>::operator -= (const MaskedData<T,CpM> &dat) 
 {
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedData<T,CpM>& operator += . Abort\n";fcout.flush();abort();}
  T* src=(T*)dat.vec->data,*dst=(T*)vec->data;
  GridIndex *m=(GridIndex*)(*mask),*m1=(GridIndex*)(*(dat.mask));
  GridIndex i,i1;
  for (k=0;k<N;k++) if ( ((i=m[k])>=0) && ((i1=m1[k])>=0) )
    dst[i]-=src[i1];
  return *this;
 };


#endif