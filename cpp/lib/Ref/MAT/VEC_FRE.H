#ifndef __VEC_FRE_H
#define __VEC_FRE_H


//#include "util.h"
//#include "matrics.h"





template <class T>
struct VecFreeCl 
  {
typedef double (*T_Double_Func)   (T &x);
typedef T (*Double_T_Func)   (double  x,T &From);
typedef T (*T_TSum_Func)   (T &Fst,T &Sec);
typedef T (*T_TMul_Func)   (T &Fst,T &Sec);
typedef T (*D_TMul_Func)   (double Fst,T &Sec);


   T *Ptr;
   int MinIndex,MaxIndex;
   T_Double_Func T_Double;
   Double_T_Func Double_T;
   T_TSum_Func T_TSum;
   T_TMul_Func T_TMul;
   D_TMul_Func D_TMul;

   VecCl ExtractDouble();
   void SetDouble(VecCl &set);

   T &operator[](int k) { return Ptr[k-MinIndex];};
   VecFreeCl& operator=(VecFreeCl<T>& vec)
    { SetIndex(vec);MemoryMove(vec.Ptr,Ptr,sizeof(T)*(MaxIndex-MinIndex+1));return (*this);};  
   VecFreeCl& operator=(T *vec)
     { MemoryMove(vec,Ptr,sizeof(T)*(MaxI-MinI+1));return (*this);};  


   void SetIndex(VecFreeCl<T> &vec) 
    {int MaxI,MinI;vec.GetIndex(MinI,MaxI);SetIndex(MinI,MaxI);
     T_Double=vec.T_Double;Double_T=vec.Double_T;T_TSum=vec.T_TSum;
     T_TMul=vec.T_TMul;D_TMul=vec.D_TMul;}

   void GetIndex(int &MinI,int &MaxI) {MinI=MinIndex;MaxI=MaxIndex;}
   void SetIndex(int MinI,int MaxI) 
     {
      int MinI_=min(MinI,MaxI), MaxI_=max(MinI,MaxI);
      if ((Ptr==NULL) || (MaxI_-MinI_!=MaxIndex-MinIndex))
       {
        if (Ptr!=NULL) delete Ptr;
        Ptr=new T[MaxI_-MinI_+1];
        MaxIndex=MaxI_;MinIndex=MinI_;
       }
     };
   void SetNullVec()
    {Ptr=NULL;MinIndex=-1;MaxIndex=-1;
     T_Double=NULL;Double_T=NULL;T_TSum=NULL;T_TMul=NULL;D_TMul=NULL;}
     
   VecFreeCl(int MinI,int MaxI){ Ptr=NULL;SetIndex(MinI,MaxI);};
   VecFreeCl() {SetNullVec();}
   VecFreeCl(VecFreeCl<T>& vec)
     { 
       SetNullVec();
       SetIndex(vec);
       MemoryMove(vec.Ptr,Ptr,sizeof(T)*(MaxIndex-MinIndex+1));
     };
   VecFreeCl(T *Addr,int MinI,int MaxI)
     {SetNullVec();Ptr=Addr;MinIndex=MinI;MaxIndex=MaxI;};
   ~VecFreeCl(){if (Ptr!=NULL) delete Ptr;Ptr=NULL;MinIndex=-1;MaxIndex=-1;};


   SetDoubleCvt(T_Double_Func TD,Double_T_Func DT){T_Double=TD;Double_T=DT;};
   SetSumMul(T_TSum_Func T_Ts,T_TMul_Func T_Tm,D_TMul_Func D_Tm)
    {T_TSum=T_Ts;T_TMul=T_Tm;D_TMul=D_Tm;};
   
//   friend VecFreeCl operator*(VecFreeCl &vec,double coef);
//   friend VecFreeCl operator/(VecFreeCl &vec,double coef);
//   friend VecFreeCl operator+(VecFreeCl &vec,double cons);
//   friend double operator*(VecFreeCl &vec,VecFreeCl &vec1);
   friend VecFreeCl operator+(VecFreeCl<T> &vec,VecFreeCl<T> &vec1);
   friend VecFreeCl operator*(double x,VecFreeCl<T> &vec);
//   friend VecFreeCl operator-(VecFreeCl &vec,VecFreeCl &vec1);
   friend ostream& operator<<(ostream &output,VecFreeCl &vec);
//   friend istream& operator>>(istream &input,VecFreeCl &vec);
//   friend (double *) operator (double *)(VecFreeCl<T> &vec)
//   friend double ModMax(VecFreeCl &Vec,double NewMax=0);
                                   // ModMax - fabs( Max(Vec[k]) )
  };
template <class T> VecFreeCl<T> operator+(VecFreeCl<T> &vec,VecFreeCl<T> &vec1)
  { 
   VecFreeCl<T> ret=vec;
   if (vec.T_TSum==NULL) 
    {cout<<"VecFreeCl - trying to sum, not defined function\n";return ret;}
   int MaxI,MinI;vec.GetIndex(MinI,MaxI);
   for (int k=0;k<=MaxI-MinI;k++) 
    ret.Ptr[k]=vec.T_TSum(vec.Ptr[k],vec1.Ptr[k]);
   return ret;
  };

template <class T> VecFreeCl<T> operator*(double x,VecFreeCl<T> &vec)
  { 
   VecFreeCl<T> ret=vec;
   if (vec.D_TMul==NULL) 
    {cout<<"VecFreeCl - trying to mul on double, not defined function\n";return ret;}
   int MaxI,MinI;vec.GetIndex(MinI,MaxI);
   for (int k=0;k<=MaxI-MinI;k++) 
    ret.Ptr[k]=vec.D_TMul(x,vec.Ptr[k]);
   return ret;
  };

template <class T> VecCl VecFreeCl<T>::ExtractDouble()
  { 
   VecCl ret(MaxIndex-MinIndex+1);ret=ret*0;
   if (T_Double==NULL) 
    {cout<<"VecFreeCl - trying to ExtractDouble, not defined function\n";return ret;}
   for (int k=1;k<=MaxIndex-MinIndex+1;k++)
    ret[k]=T_Double(Ptr[k-1]);
   return ret;
  };
template <class T> void VecFreeCl<T>::SetDouble(VecCl &set)
  { 
   if (set[0]!=MaxIndex-MinIndex+1)
    {cout<<"VecFreeCl - trying to SetDouble, different dim\n";return;}
   if (T_Double==NULL) 
    {cout<<"VecFreeCl - trying to SetDouble, not defined function\n";return;}
   for (int k=0;k<=MaxIndex-MinIndex;k++)
    Ptr[k]=Double_T(set[k+1],Ptr[k]);
  };
template <class T> ostream& operator<<(ostream &output,VecFreeCl<T> &vec)
  {
   int MinI,MaxI;vec.GetIndex(MinI,MaxI);
   output<<" MinIndex "<<MinI<<" MaxIndex "<<MaxI<<"\n";
   for (int k=MinI;k<=MaxI;k++) output<<k<<"  "<<vec[k]<<" ";
   output<<"VecFreeCl End\n";
   return output;
  }
  
#endif




