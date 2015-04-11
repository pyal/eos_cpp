#include <lib\precompiled\data.h>
#include "SparceArray.h"


//SparceArrayInd
DescribedClass* create_SparceArrayInd_cd(){return new SparceArrayInd;}
static ClassDesc SparceArrayInd_cd(typeid(SparceArrayInd),"SparceArrayInd",1,"",&create_SparceArrayInd_cd);
//SparceArray
//DescribedClass* create_SparceArray_cd(){return new SparceArray;}
//static ClassDesc SparceArray_cd(typeid(SparceArray),"SparceArray",1,"",&create_SparceArray_cd);

int SparceArrayInd::Num (int CountDeleted)
 {
  int k,N=sparce_ind.SetNumEl(),ret=0;
  if (CountDeleted) return N;
  for (k=0;k<N;k++) if (sparce_ind[k]) ret++;return ret;
 }

int SparceArrayInd::MaxNum() 
 {
  for (int k=sparce_ind.SetNumEl();k>0;k--) 
   if (sparce_ind[k-1]) return k;
  return -1;
 }


int SparceArrayInd::Next (int i)
 {
  int k,N=sparce_ind.SetNumEl();
  if (i<0) i=-1;
  for (k=i+1;k<N;k++) 
   if (sparce_ind[k]) return k;
  return -1;
 }

int SparceArrayInd::Prev (int i)
 {
  int k,N=sparce_ind.SetNumEl();
  if (i<0) return max(-1,MaxNum()-1);
  for (k=i-1;k>=0;k--) 
   if (sparce_ind[k]) return k;
  return -1;
 }

int SparceArrayInd::NextEmp (int i)
 {
  int k,N=sparce_ind.SetNumEl();
  if (i<0) i=-1;
  for (k=i+1;k<N;k++) 
   if (!sparce_ind[k]) return k;
  return N;
 }

int SparceArrayInd::PrevEmp (int i)
 {
  int k,N=sparce_ind.SetNumEl();
  if (i<0) i=N;
  for (k=i-1;k>=0;k--) 
   if (!sparce_ind[k]) return k;
  return N;
 }





