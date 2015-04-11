#ifndef Grid_Mask_Vector_h
#define Grid_Mask_Vector_h 1


//struct GridMask;


//#include "Grid_Mask_Data.h"

#include "lib\ref\ref.h"
#include "grid_action.h"


template <class T,class CpM>
struct GridVectorField;

template <class T,class CpM=CopyStructFast<T> >
struct MaskedVector
 {
//  DataVector<MaskedData*> masks;

  Ref<GridVectorField<T,CpM> > vec;
  Ref<GridMaskVector> mask;
  int ToDeleteVec;
  ~MaskedVector(){if ((ToDeleteVec) && (vec->GetGridManip())) //vec->SetStore(NULL);}
 vec->GetGridManip()->DelVar(vec.pointer());vec=NULL;mask=NULL;}//
//   {vec->unmanage();delete vec.pointer();vec=NULL;}};//vec->SetStore(NULL);}//
// Create new stand alone dat,vec
  MaskedVector(GridVectorField<T,CpM> *dat,GridMaskVector *m);
// Do not create vec,mask, make simple ref - used in copy
  MaskedVector(const MaskedVector<T,CpM> &dat){vec=dat.vec;mask=dat.mask;ToDeleteVec=0;}
// Create new stand alone mask
  MaskedVector(GridVectorField<T,CpM> *dat,const GridMaskVector &m);

// Create new stand alone dat,vec
  MaskedVector(GridField<T,CpM> *dat,const GridMaskVector &m);

// Sets points which are good in new and old mask
  MaskedVector<T,CpM>& operator = (const MaskedVector<T,CpM> &dat) ;
//==============================================================
//======= Operates  only on good points MaskedData +-*/ T    ===
//==============================================================
  MaskedVector<T,CpM> operator * (const T& dat);
  MaskedVector<T,CpM> operator / (const T& dat);
  MaskedVector<T,CpM> operator + (const T& dat);
  MaskedVector<T,CpM> operator - (const T& dat);
//==============================================================
//================ End Operations  MaskedData +-*/ T   =========
//==============================================================

//==============================================================
// Operates  only on both good points MaskedData +-*/ MaskedData
//==============================================================
  MaskedVector<T,CpM> operator * (const MaskedVector<T,CpM> &dat);
  MaskedVector<T,CpM> operator / (const MaskedVector<T,CpM> &dat);
  MaskedVector<T,CpM> operator + (const MaskedVector<T,CpM> &dat);
  MaskedVector<T,CpM> operator - (const MaskedVector<T,CpM> &dat);


 };

 
#include "Grid_Array.h"
#include "Grid_Interface.h"

//=================================================================

template <class T,class CpM >
MaskedVector<T,CpM>::MaskedVector(GridVectorField<T,CpM> *dat,GridMaskVector *m)
 {
  if (dat->SetDim()!=m->SetDim()) {fcout<<" Error in MaskedVector<T,CpM>(GridVectorField<T,CpM> *dat,GridMaskVector *m) . Abort\n";fcout.flush();abort();}
  ToDeleteVec=1;
  vec=new GridVectorField<T,CpM>(dat->SetDim(),dat->GetGridManip());
  mask=new GridMaskVector;
  *vec=*dat;*mask=*m;
 }
template <class T,class CpM >
MaskedVector<T,CpM>::MaskedVector(GridVectorField<T,CpM> *dat,const GridMaskVector &m)
 {  
  if (dat->SetDim()!=m.SetDim()) {fcout<<" Error in MaskedVector<T,CpM>(GridVectorField<T,CpM> *dat,const GridMaskVector &m) . Abort\n";fcout.flush();abort();}
  ToDeleteVec=0;vec=dat;mask=new GridMaskVector;*mask=m;}

template <class T,class CpM >
MaskedVector<T,CpM>& MaskedVector<T,CpM>::operator = (const MaskedVector<T,CpM> &dat) 
 {
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedVector<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
//  GridField *v=vec->GetX(0),*v1=dat.vec->GetX(0):

  for (k=0;k<N;k++) ( *(vec->GetX(k)) )((*mask)[k])=
                       (*(dat.vec->GetX(k)))((*(dat.mask))[k]);
  return *this;
 };



template <class T,class CpM >
MaskedVector<T,CpM>::MaskedVector(GridField<T,CpM> *dat,const GridMaskVector &m)
 {
  ToDeleteVec=1;
  int D=m.SetDim();
  vec=new GridVectorField<T,CpM>(D,dat->SetStore());mask=new GridMaskVector;
  for (int k=0;k<D;k++) *(vec->GetX(k))=*dat;
  mask=new GridMaskVector;*mask=m;
 };










//================================================================================
template <class T,class CpM >
MaskedVector<T,CpM> MaskedVector<T,CpM>::operator * (const T& dat)
 {
  MaskedVector<T,CpM> ret(this->vec,this->mask);
  for (int k=0;k<mask->SetDim();k++)  
   (*ret.vec->GetX(k))((*mask)[k])*=dat;
  return ret;
 };

template <class T,class CpM >
MaskedVector<T,CpM> MaskedVector<T,CpM>::operator / (const T& dat)
 {
  MaskedVector<T,CpM> ret(this->vec,this->mask);
  for (int k=0;k<mask->SetDim();k++) (*ret.vec->GetX(k))((*mask)[k])/=dat;
  return ret;
 };

template <class T,class CpM >
MaskedVector<T,CpM> MaskedVector<T,CpM>::operator + (const T& dat)
 {
  MaskedVector<T,CpM> ret(this->vec,this->mask);
  for (int k=0;k<mask->SetDim();k++) (*ret.vec->GetX(k))((*mask)[k])+=dat;
  return ret;
 };

template <class T,class CpM >
MaskedVector<T,CpM> MaskedVector<T,CpM>::operator - (const T& dat)
 {
  MaskedVector<T,CpM> ret(this->vec,this->mask);
  for (int k=0;k<mask->SetDim();k++) (*ret.vec->GetX(k))((*mask)[k])-=dat;
  return ret;
 };

//================================================================================
template <class T,class CpM >
MaskedVector<T,CpM> MaskedVector<T,CpM>::operator * (const MaskedVector<T,CpM> &dat) 
 {
  MaskedVector<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedVector<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  for (k=0;k<N;k++) 
   (*ret.vec->GetX(k))((*(ret.mask))[k])*=(*dat.vec->GetX(k))((*(dat.mask))[k]);
  return ret;
 };

template <class T,class CpM >
MaskedVector<T,CpM> MaskedVector<T,CpM>::operator / (const MaskedVector<T,CpM> &dat) 
 {
  MaskedVector<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedVector<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  for (k=0;k<N;k++) 
   (*ret.vec->GetX(k))((*(ret.mask))[k])/=(*dat.vec->GetX(k))((*(dat.mask))[k]);
  return ret;
 };

template <class T,class CpM >
MaskedVector<T,CpM> MaskedVector<T,CpM>::operator + (const MaskedVector<T,CpM> &dat) 
 {
  MaskedVector<T,CpM> ret(this->vec,this->mask);
  int k=1,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedVector<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  for (k=0;k<N;k++) 
   (*ret.vec->GetX(k))((*(ret.mask))[k])+=(*dat.vec->GetX(k))((*(dat.mask))[k]);
//   (*ret.vec->GetX(k))(*ret.mask[k])+=(*dat.vec->GetX(k))(*dat.mask[k]);
  return ret;
 };

template <class T,class CpM >
MaskedVector<T,CpM> MaskedVector<T,CpM>::operator - (const MaskedVector<T,CpM> &dat) 
 {
  MaskedVector<T,CpM> ret(this->vec,this->mask);
  int k,N=mask->SetDim(),N1=dat.mask->SetDim();
  if (N1!=N) {fcout<<" Error in MaskedVector<T,CpM>& operator = . Abort\n";fcout.flush();abort();}
  for (k=0;k<N;k++) 
   (*ret.vec->GetX(k))((*(ret.mask))[k])-=(*dat.vec->GetX(k))((*(dat.mask))[k]);
  return ret;
 };




#endif