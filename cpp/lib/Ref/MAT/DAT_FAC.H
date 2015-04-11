#ifndef __REGDAT_FAC_H
#define __REGDAT_FAC_H

//#include "deffunc.h"
//#include "interfac.h"
//#include "f2c.h"

//#include "alloc.h"


//format TColumn:  Number N row x
//format TData  :  Number N Numberi Ni
//format TTable :  NumberX Nx NumberY Ny

struct RegInd :virtual public SavableClass
{
  int num;
  int *idx;
  Ref<RegDim> ref_lim;
  RegInd():idx(NULL),num(0){};
  RegInd(int *i,int n):idx(NULL),num(0),ref_lim(NULL){SetInd(i,n);};
  RegInd(RegInd &i):idx(NULL),num(0),ref_lim(NULL){SetInd(i.idx,i.num);SetLim(&i.ref_lim);};
  RegInd& operator=(RegInd& i){SetInd(i.idx,i.num);SetLim(&i.ref_lim);return *this;};

  ~RegInd(){Delete();}
  int operator[](int k) { return idx[k];};
  void SetInd(int *i,int n)
    {
     if (N!=num) {Delete();idx=new int[N];num=N;}
     MemoryMove(i,n,num*sizeof(int));
    };
  void SetLim(RegDim *lim){ref_lim=lim;if (ref_lim.Dim()!=num) cout<<" RegInd Error in SetLim\n";}

  void Delete(){delete idx;idx=NULL;num=0;ref_lim=NULL;}
  RegInd& operator=(int n){for (int k=0;k<num;k++) idx[k]=n;};
  int save_data_state(FilterOut&so) 
    {  so<<KeyDesc("RegInd")<<" NumDim "<<num; 
       for (int k=0;k<num;k++) so<<idx[k];return 1;
    };
  int read_data_state(FilterIn&si)
    { 
      char tmp[256];int N;si>>StdKey>>tmp>>N;
      if (N!=num) {Delete();idx=new int[N];num=N;}
      for (int k=0;k<num;k++) si>>idx[k];
      return 1;//(si);
    };

//  void Start();void End();int Eof();void Step(int Num);

//   VecFreeCl& operator=(T *vec) { MemoryMove(vec,Ptr,sizeof(T)*(MaxI-MinI+1));return (*this);};  
//   friend VecFreeCl operator+(VecFreeCl<T> &vec,VecFreeCl<T> &vec1);
//   friend VecFreeCl operator*(double x,VecFreeCl<T> &vec);
//   friend ostream& operator<<(ostream &output,VecFreeCl &vec);
  };


struct RegDim :virtual public SavableClass 
{
  int num;
  Ref<RegInd> Min,Max;
  RegDim():num(0),Max(NULL),Min(NULL) {};
  RegDim(int MinInd,int MaxInd,int Dim):num(0),Max(NULL),Min(NULL) 
    {SetDim(MinInd,MaxInd,Dim);}
  RegDim(RegDim &reg):num(0),Max(NULL),Min(NULL)
    {*this=reg;}
  ~RegDim()  {  cout << "delete RegDim\n"; Delete(); };
  int save_data_state(FilterOut&so) 
    {  so<<KeyDesc("RegDim")<<" Num "<<num<<" MinInd "<<Min<<" MaxInd "<<Max;
       return 1;};
  int read_data_state(FilterIn&si)
    { 
      char tmp[256];si>>StdKey>>tmp>>num>>tmp>>Min>>tmp>>Max;Check();
      return 1;//(si);
    };
 void Delete() { Min=NULL;Max=NULL;num=0;}

 void AddDim(int MinInd,int MaxInd,int DimToAdd=1)
  {
   int N=NumDim+DimToAdd-1;
   int *min=new int[N+1],*max=new int[N+1];
   if (N!=0) 
     {MemoryMove(DimIndexMin,min,(N)*sizeof(int));
      MemoryMove(DimIndexMax,max,(N)*sizeof(int));
      }
   for (int k=NumDim-1;k<N;k++) { min[k]=MinInd;max[k]=MaxInd;}
   Delete();DimIndexMin=min;DimIndexMax=max;Numdim=N+1;
 };
 void SetDim(int MinInd,int MaxInd,int DimToSet)
 {
   if (DimToSet>NumDim) {AddDim(MinInd,MaxInd,DimToSet-NumDim);return;}
   DimIndexMin[DimToSet-1]=MinInd;
   DimIndexMax[DimToSet-1]=MaxInd;
 };
 void IndMinAdd(int Num){ for (int k=0;k<NumDim;k++) DimIndexMin[k]+=Num;}
 void IndMaxAdd(int Num){ for (int k=0;k<NumDim;k++) DimIndexMax[k]+=Num;}
 void IndMinMaxAdd(int Num){IndMinAdd(Num);IndMaxAdd(Num);}
// void IndMinMul(int Num){ for (int k=0;k<NumDim;k++) DimIndexMin[k]*=Num;}
// void IndMaxMul(int Num){ for (int k=0;k<NumDim;k++) DimIndexMax[k]*=Num;}
// void IndMinMaxMul(int Num){IndMinMul(Num);IndMaxMul(Num);}
 int From(int Col) {return DimIndexMin[Col];}
 int To(int Col) {return DimIndexMax[Col];}
 int Length(int Col) {return DimIndexMax[Col]-DimIndexMin[Col];}
};
int Cmp(RegDim const &src,RegDim const &dest)
{
  int N,l,l1,k;
  if ((N=src.NumDim)!=dest.NumDim) return (src.NumDim>dest.NumDim) ? 3:-3;
  for (k=0;k<N;k++)
    if ( (l= src.DimIndexMax[k]-src.DimIndexMin[k]))!=
         (l1=dest.DimIndexMax[k]-dest.DimIndexMin[k]) )
         return (l>l1) ? 2:-2;
  for (k=0;k<N;k++)
     if ( (l= src.DimIndexMax[k])!=((l1= dest.DimIndexMax[k]) )
         return (l>l1) ? 1:-1;
  return 0;
};

template <class T>
struct RegData:virtual public SavableClass
  {
   T **D;
   RegDim reg_lim;
   RegData():D(NULL){}
   RegData(RegDim const &lim):D(NULL) {SetDim(lim);}
   RegData(RegData &dat);
//   virtual TData& operator=(TData &dat);
  int save_data_state(FilterOut&so) 
    {  so<<KeyDesc("RegData")<<reg_lim;
//       ForAllReg()
       return 1;};
  int read_data_state(FilterIn&si)
    { 
      char tmp[256];
      int N;
      RegDim lim;

      si>>StdKey>>tmp>>lim;
//      SetDim(lim);
//      if (abs(Cmp(reg_lim,lim))>1) {Dele
      ForAllReg()
      if (N!=NumDim) {Delete();DimIndexMin=new int[N];DimIndexMax=new int[N];NumDim=N;}
      for (int k=0;k<NumDim;k++) si>>DimIndexMin>>DimIndexMax;
      return 1;//(si);
    };
   virtual void Delete()
   {
     for (int k=0;k<reg_lim.NumDim;k++) delete D[k];
     delete D;D=NULL;reg_lim.Delete();
   };
   ~RegData(){Delete();}

   virtual void SetDim(RegDim const &lim)
   {
     if (abs(Cmp(reg_lim,lim))>1) 
        {
         Delete();D=new *T[lim.NumDim];
         for (int k=0;k<reg_lim.NumDim;k++)  D[k]=new T[lim.Length(k)];
        }
     reg_lim=lim;
   };

    

//   virtual void AddColumn(T* column,int IndMin,int IndMax);
//   virtual void SetColumn(T* column,int Col,int IndMin,int IndMax);
}
//   static TData *CurDat;
//   static int SortColumn;
//   static int SortCmp(const void *v1,const void *v2);
//   virtual void SortData(int SortCol=0);
//  };

















/*
template <class T> void RegData<T>::AddColumn(T* column,int Num)
  {
   int *i=new int[N+1];
   if (I!=NULL) MemoryMove(I,i,(N+1)*sizeof(int));
   T **d=new T*[N+1];
   int l=sizeof(T);
   for (int k=0;k<N;k++) {d[k]=new T[I[k]];MemoryMove(D[k],d[k],l*I[k]);}
   i[N]=Num;d[N]=new T[Num];MemoryMove(column,d[N],l*Num);
   if (I!=NULL)  {for (int k=0;k<N;k++) delete D[k];
                  delete D;delete I;}
   N++;D=d;I=i;
  };   
template <class T>  void TData<T>::SetColumn(T* column,int Num,int Col)
  {
   if ( (Col>=N) || (Col<0) )
     { cout<<" Formal Error in TData::SetColumn. NumCol is:"<<N+1
           <<" Setting Col:"<<Col+1<<"\n";AddColumn(column,Num);return ;}
   delete D[Col];
   I[Col]=Num;D[Col]=new T[Num];MemoryMove(column,D[Col],sizeof(T)*Num);
  };   

//static int SortCmp(const void *v1,const void *v2)
 template <class T> int TData<T>::SortCmp(const void *v1,const void *v2)
  {
//   int i1=*((int*)v1),i2=*((int*)v2);
   T *t1=&(CurDat->D[SortColumn][*((int*)v1)]),*t2=&(CurDat->D[SortColumn][*((int*)v2)]);
   if (*t1>*t2) return 1;
   if (*t1==*t2) return 0;
   return -1;
  }
//extern  int DataStore<class T>(ostream &out,TData<T> *dat);

template <class T>  void TData<T>::SortData(int SortCol)
  {
   if (N<=0) return;
   int k,Num=I[0];
   if ( (SortCol>=N) || (SortCol<0) )
     { cout<<" Formal Error in TData::SortData. NumCol is:"<<N+1
           <<" Setting Col:"<<SortCol+1<<"\n";return ;}
   for ( k=0;k<N-1;k++) if (I[k]!=I[k+1]) 
     { cout<<" Error in TData::SortData. Dif Sizes of columns I[k]= "<<I[k]
           <<" I[k+1]= "<<I[k+1]<<"\n";return ;};
   int *i=new int[Num];for (k=0;k<Num;k++) i[k]=k;
   SortColumn=SortCol;CurDat=this;
   qsort(i,Num,sizeof(int),SortCmp);
   TData<T> tmp=*this;
   for (k=0;k<Num;k++) if (i[k]!=k) { for (int k1=0;k1<N;k1++) D[k1][k]=tmp.D[k1][i[k]];}
//DataStore(cout,&tmp);cout"\n\n\n\n\n";DataStore(cout,this);

   delete i; return;
  }

*/
#endif
