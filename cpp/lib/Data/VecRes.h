
#ifndef __VecRes_H
#define __VecRes_H 1


#include "lib\ref\class_sav.h"
#include "lib\std\util.h"
#include "datavec.h"



//template <class T>
template <class T,class CpM__=CopyStructFast<T> >
struct ResizeVector : public SavableClass  
{
  public:
      ResizeVector (const ResizeVector<T,CpM__> &vec);
      ResizeVector (int Start=50,int AddBy=-50,int Release=0,T* Fill=NULL);
      virtual ~ResizeVector(){Delete();};

      void Delete (){CpM__::DeleteArray(data,ElementNum);data=NULL;ElementNum=0;ArrayLen=0;};

      ResizeVector<T,CpM__>& operator = (const ResizeVector<T,CpM__>& vec);
      ResizeVector<T,CpM__>& operator = (const T& vec);
      
      inline T& top() { return data[SetNumEl()-1];}
      inline void pop() { SetNumEl(SetNumEl()-1);}
      inline void push(const T& val) { Resize(SetNumEl()+1);data[SetNumEl()-1] = val;}
      inline int empty()const { return SetNumEl()==0;}

      operator T*() const {return data;};
      inline T& operator [] (const int k){	if (k>=ElementNum) Resize(k+1);return data[k];};
      virtual int save_data_state (FilterOut &so);
      virtual int read_data_state (FilterIn &si);
      inline int SetNumEl()const { return ElementNum;}
      inline int SetNumEl(int NewLen){Resize(NewLen);return ElementNum;}

      void Release()
       {if (ReleasePolicy==0) {Delete();Resize(0);} 
        else { int len=ElementNum;Delete();Resize(len*ReleasePolicy/100.);SetNumEl(0);};}
      void ReleaseUnused(){Resize(SetNumEl(),1);}
      void SetArr(T* arr,int NumToPut,int Len)
       {Resize(NumToPut+Len);CpM__::CopyArray(arr,&data[NumToPut],Len);}
      void GetArr(T* arr,int NumToGet,int Len)
	  {int n=min(Len,SetNumEl()-NumToGet);CpM__::CopyArray(&data[NumToGet],arr,Len);}

// AddBy - positive - add by num, negative - max(multiply in percents,AddBy)
// Release - 0 - Release to StartNum, positive - release in percents
// FillElement - Unsettled will be filled by element
      void SetPolicy(int Start,int AddBy,int Release,T *Fill)
       {StartNum=Start;AddByNum=AddBy;ReleasePolicy=Release;
      if (Fill==NULL) NotFill=1;else { NotFill=0;FillElement=*Fill;}}

      inline const void GetPolicy(int &Start,int &AddBy,int &Release,T *Fill) const
       {Start=StartNum;AddBy=AddByNum;Release=ReleasePolicy;
        if (NotFill) Fill=NULL;else Fill=&FillElement;}
      void SetFill(const T *Fill){if (Fill==NULL) NotFill=1;else { NotFill=0;FillElement=*Fill;FillUnused();}}
      inline  T* GetFill() 
       {if (NotFill) return NULL;else return &FillElement;}

// Sets ElementsNum 

  protected:
  private:
  private:  //## implementation

      void Resize(int NewElLen,int Force=0);// internal use only
      void FillUnused()  {for (int k=ElementNum;k<ArrayLen;k++) data[k]=FillElement;};

      int StartNum,AddByNum,ReleasePolicy,NotFill;
      T FillElement;

      int ElementNum,ArrayLen;
      T* data;
};








// ===========================================================
// =================   End of the head =======================
// ===========================================================



template <class T,class CpM__>
ResizeVector<T,CpM__>::ResizeVector 
(const ResizeVector<T,CpM__> &vec):data(NULL), ElementNum(0),ArrayLen(0)
 {
  SetPolicy(vec.StartNum,vec.AddByNum,vec.ReleasePolicy,NULL);
  if (!vec.NotFill) SetFill(&(vec.FillElement));
//  if (vec.NotFill) SetPolicy(vec.StartNum,vec.AddByNum,vec.ReleasePolicy,NULL);
//  else SetPolicy(vec.StartNum,vec.AddByNum,vec.ReleasePolicy,&(vec.FillElement));
  Resize(vec.ElementNum);//MemoryMove(vec.data,data,sizeof(T)*ElementNum);
  CpM__::CopyArray(vec.data,data,ElementNum);
 };

template <class T,class CpM__>
ResizeVector<T,CpM__>::ResizeVector (int Start,int AddBy,int Release,T* Fill)
                                            :data(NULL), ElementNum(0),ArrayLen(0)
 {SetPolicy(Start,AddBy,Release,Fill);Resize(0);};

template <class T,class CpM__>
ResizeVector<T,CpM__>& 
ResizeVector<T,CpM__>::operator = (const ResizeVector<T,CpM__>& vec)
 {
  if (vec.NotFill) SetPolicy(vec.StartNum,vec.AddByNum,vec.ReleasePolicy,NULL);
  else 
   {T tmp=vec.FillElement;SetPolicy(vec.StartNum,vec.AddByNum,vec.ReleasePolicy,&tmp);}//(vec.FillElement));}
  Resize(vec.ElementNum);//MemoryMove(vec.data,data,sizeof(T)*ElementNum);
  CpM__::CopyArray(vec.data,data,ElementNum);
  return *this;
 };
template <class T,class CpM__>
ResizeVector<T,CpM__>& 
ResizeVector<T,CpM__>::operator = (const T& vec)
 {
  for (int k=0;k<ElementNum;k++) data[k]=vec;
  return *this;
 };


template <class T,class CpM__>
void ResizeVector<T,CpM__>::Resize(int NewElLen,int Force)// internal use only
 {
    if (Force) {
            if (NewElLen==ArrayLen) return;
            int OldLen=ElementNum;
            T* tmp=new T[NewElLen];
            if (data!=NULL) {CpM__::CopyArray(data,tmp,NewElLen);Delete();}
            data=tmp;
            ArrayLen=NewElLen;ElementNum=NewElLen;
            return;
    }
    if (NewElLen<=ArrayLen && ArrayLen>=StartNum ) {
            if (!NotFill) {
                for (int k=ElementNum;k<NewElLen;k++) 
                    data[k]=FillElement;
            }
            ElementNum=NewElLen;return;
    }
    int NewArLen;
    if (StartNum>=NewElLen) NewArLen=StartNum;
    else {
        int Addition;
        if (AddByNum>0) Addition=AddByNum;
        else Addition=ArrayLen*(-AddByNum)/100.;
        if (NewElLen<ArrayLen ){
            if (NewElLen+Addition>ArrayLen){
                if (!NotFill) {
                    for (int k=ElementNum;k<NewElLen;k++) 
                        data[k]=FillElement;
                }
                ElementNum=NewElLen;return;
            }
            NewArLen = NewElLen+0.5*Addition;
        } else {
            if (Addition+ArrayLen>=NewElLen) 
                NewArLen=Addition+ArrayLen;
            else NewArLen=NewElLen+0.5*Addition;
        }
    }
    T* tmp=new T[NewArLen];
    int OldArLen=ArrayLen;
    if (data!=NULL) {
        CpM__::CopyArray(data,tmp,ArrayLen);
        //    MemoryMove(data,tmp,sizeof(T)*ArrayLen);
        Delete();
    }
    data=tmp;
    if (!NotFill) {
        for (int k=OldArLen;k<NewArLen;k++) data[k]=FillElement;
    }
    ArrayLen=NewArLen;ElementNum=NewElLen;
 };



template <class T,class CpM__>
int ResizeVector<T,CpM__>::save_data_state (FilterOut &so)
  {
   so<<KeyDesc("StartNum")<<StartNum<<KeyDesc(" AddByNum ")<<AddByNum
     <<KeyDesc("ReleasePolicy")<<ReleasePolicy<<KeyDesc("NotFill")<<NotFill;
   if (!NotFill) so<<KeyDesc("FillElement")<<FillElement;
   so<<KeyDesc("ElementNum")<<ElementNum;
  	so<<KeyDesc(" Data\n");
   for (int k=0;k<ElementNum;k++) so<<data[k];so<<KeyDesc("EndData\n");
  	return 1;
  };

template <class T,class CpM__>
int ResizeVector<T,CpM__>::read_data_state (FilterIn &si)
 {
   Delete();
   int NewElNum,Start,Add,Pol,NotF;
   T Fill;
   si>>StdKey>>Start>>StdKey>>Add>>StdKey>>Pol>>StdKey>>NotF;
   if (!NotF) {si>>StdKey>>Fill;SetPolicy(Start,Add,Pol,&Fill);}
   else SetPolicy(Start,Add,Pol,NULL);
   si>>StdKey>>NewElNum>>StdKey;

   Resize(NewElNum);
   
   for (int k=0;k<NewElNum;k++) si>>data[k];//so<<"\n";
   si>>StdKey;
  	return 1;
 }


//template <class T>
//static void CopyStructSlow<T>::CopyArray(T* src,T* dst,int Num)
// {
//  for (int k=0;k<Num;k++) dst[k]=src[k];
// };




#endif
