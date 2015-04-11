#ifndef _allocator_intellect_h
#define _allocator_intellect_h

#include "lib/std/util.h"

template <class T>
struct AllocatorIntellect
  {
  private:
   size_t LastAllocationSize, StartAllocationSize, DemandedSize;
   T *LastAllocationPtr;
   int Frozen;
#define MULTIPLY_COEF 1.3

   void InitAllocation(size_t size=0,T *ptr=NULL,int f=0,size_t st=100)
    {
     LastAllocationPtr=NULL;LastAllocationSize=0;DemandedSize=0;
     StartAllocationSize = st;
	 Frozen = f;
     if (size==0) return;
     SetNewSize(size);
     DemandedSize = size;
     if (ptr) MemoryMove(ptr,Ptr(),size*sizeof(T));
    }
    void SetNewSize(size_t size){
        T *new_p=new T[size];
        MemoryMove(LastAllocationPtr,new_p,min(LastAllocationSize,size)*sizeof(T));
        delete LastAllocationPtr;
        LastAllocationPtr=new_p;LastAllocationSize=size;
    }
  public:
   inline size_t Size(){ return DemandedSize;}
   inline size_t AllocSize(){ return LastAllocationSize;}
   inline T* Ptr(){ return LastAllocationPtr;}
   inline int IsFrozen(){ return Frozen;}
   T* Allocate(size_t size)
    {
     DemandedSize = size;
     if (LastAllocationSize>=size*sqr(MULTIPLY_COEF)+StartAllocationSize) {
         SetNewSize(size+StartAllocationSize);
     } else if (LastAllocationSize<size){
         SetNewSize(size*MULTIPLY_COEF+StartAllocationSize);
     }
     return LastAllocationPtr;
    }
   AllocatorIntellect(size_t size=0,T *ptr=0,int f=0,int st=100) {InitAllocation(size,ptr,f,st);};
   AllocatorIntellect(const AllocatorIntellect& alloc) {
       InitAllocation(alloc.DemandedSize(),alloc.Ptr(),alloc.Frozen,
                      alloc.StartAllocationSize);
   };
   void Empty(){
       if (!Frozen) {
           delete LastAllocationPtr;
       }
       LastAllocationPtr=NULL;LastAllocationSize=0;};
   T& operator[](size_t k){if (LastAllocationSize<k+2) Allocate(k+1);return LastAllocationPtr[k];}

   ~AllocatorIntellect() {
	   if (!Frozen) {
		   delete LastAllocationPtr;LastAllocationPtr=NULL;
	   };
   };
  };

#endif



/*
template <class T>
struct AllocatorIntelect_Old
  {
   long LastAllocationSize, StartAllocationSize;
   T *LastAllocationPtr;
  private:
   int Frozen;

   void InitAllocation(long size=0,T *ptr=NULL,int f=0,long st=100)
    {
     if ((ptr==NULL) && (size!=0)) ptr=new T[size];
     LastAllocationPtr=ptr;LastAllocationSize=size;
     StartAllocationSize=st;Frozen=f;
    }
  public:
   void* Allocate(long size)
    {
     if (LastAllocationSize>=size) return LastAllocationPtr;
     long new_s=LastAllocationSize;
     while (new_s<=size) 
      {double tmp=new_s*1.3;
       new_s=(long)tmp+StartAllocationSize;
      }
     T *new_p=new T[new_s];
     MemoryMove(LastAllocationPtr,new_p,LastAllocationSize*sizeof(T));
//     memmove(new_p,LastAllocationPtr,LastAllocationSize*sizeof(T));
     delete LastAllocationPtr;
     LastAllocationPtr=new_p;LastAllocationSize=new_s;
//cout<<" Last AA "<<LastAllocationSize<<" size "<<size<<"\n"<<flush;
     return LastAllocationPtr;
    }
   AllocatorIntelect_Old(long size=0,T *ptr=0,int f=0,int st=100) {InitAllocation(size,ptr,f,st);};
   void Empty(){delete LastAllocationPtr;LastAllocationPtr=NULL;LastAllocationSize=0;};
   T& operator[](int k){if (LastAllocationSize<k+2) Allocate(k+1);return LastAllocationPtr[k];}

   ~AllocatorIntelect_Old() {if (!Frozen) {delete LastAllocationPtr;LastAllocationPtr=NULL;};};
  };
*/
