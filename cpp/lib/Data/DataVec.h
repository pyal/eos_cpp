
#ifndef __DATAVEC_H
#define __DATAVEC_H 1


#include "lib/ref/class_sav.h"
#include "lib/std/util.h"

template <class T>
struct CopyStructFast
 {static void CopyArray(T* src,T* dst,int Num) {MemoryMove(src,dst,sizeof(T)*Num); }
  static void DeleteArray(T* src,int Num){delete[]src;}
 };
template <class T>
struct CopyStructSlow 
 { 
  static void CopyArray(T* src,T* dst,int Num) { for (int k=0;k<Num;k++) dst[k]=src[k];};
  static void DeleteArray(T* src,int Num){delete[]src;}
};
template <class T>
struct CopyStructRef 
 { 
// Possible errors!!! was  
  static void CopyArray(T* src,T* dst,int Num) {for (int k=0;k<Num;k++) dst[k]=src[k];};
//  static void CopyArray(T* src,T* dst,int Num) {MemoryMove(src,dst,sizeof(T)*Num); }
  static void DeleteArray(T* src,int Num){for (int k=0;k<Num;k++) src[k]=NULL;delete[] src;}
};



//template <class T,class CopyStructMethod=CopyStructFast<T> >
template <class T,class CopyStructMethod=CopyStructSlow<T> >
struct DataVector : public SavableClass  
{
  public:
      DataVector ():data(NULL), Dim(0){};
      DataVector (int Num, T* Val = NULL);
      DataVector (const DataVector<T,CopyStructMethod> &vec);
      virtual ~DataVector(){ Delete();};


      virtual int save_data_state (FilterOut &so);
      virtual int read_data_state (FilterIn &si);
      void Delete ();

      DataVector<T,CopyStructMethod>& operator = (const T& vec);
      DataVector<T,CopyStructMethod>& operator = (const DataVector<T,CopyStructMethod>& vec);
      DataVector<T,CopyStructMethod>& operator = (T* vec);
      inline int SetDim () const {return Dim;}
      int SetDim (int NewDim);
	     void Resize(int NewDim);
	     void SetData(T* dat,int From=-1,int To=-1);
      operator T*() const;
      T* Ptr() const {
          return data;
      }
	  friend inline int operator == (const DataVector<T,CopyStructMethod>& vec1, const DataVector<T,CopyStructMethod>& vec2)
	  {
		if (vec1.Dim!=vec2.Dim) return 0;
		for (int k=0;k<vec1.Dim;k++) if (vec1[k]!=vec2[k]) return 0;
		return 1;
	  }
      friend inline int operator != (const DataVector<T,CopyStructMethod>& vec1, const DataVector<T,CopyStructMethod>& vec2)
      {return !operator==(vec1,vec2);}
      inline T& operator [] (int k){	
#ifdef _DEBUG
	   if (k>=SetDim() || k<0)
		   throw info_except("Trying to get element %i for array with dim %i\n",k,SetDim());
#endif
        return data[k];
      };
//      DataVector<T>& operator = (double vec);
//      friend DataVector<T>& operator * (const DataVector<T>& vec, const double& coef);
//      friend DataVector<T>& operator / (const DataVector<T>& vec, const double& coef);
//      friend DataVector<T>& operator + (const DataVector<T>& vec, const double& coef);
//      friend DataVector<T>& operator - (const DataVector<T>& vec, const double& coef);

//      friend DataVector<T> operator / (const DataVector<T>& vec, const T& coef);
//      friend DataVector<T> operator * (const DataVector<T>& vec, const T& coef);
//      friend DataVector<T> operator + (const DataVector<T>& vec, const T& coef);
//      friend DataVector<T> operator - (const DataVector<T>& vec, const T& coef);
//      friend T operator * (const DataVector<T>& vec, const DataVector<T>& coef);
//      friend DataVector<T> operator + (const DataVector<T>& vec, const DataVector<T>& coef);
//      friend DataVector<T> operator - (const DataVector<T>& vec, const DataVector<T>& coef);
      DataVector<T,CopyStructMethod> operator / (const T& coef);
      DataVector<T,CopyStructMethod> operator * (const T& coef);
      DataVector<T,CopyStructMethod> operator + (const T& coef);
      DataVector<T,CopyStructMethod> operator - (const T& coef);

      T operator * (const DataVector<T,CopyStructMethod>& coef);

      DataVector<T,CopyStructMethod> operator + (const DataVector<T,CopyStructMethod>& coef);
      DataVector<T,CopyStructMethod> operator - (const DataVector<T,CopyStructMethod>& coef);

      static DataVector<T,CopyStructMethod> MakeStepVector(int Num, const T& low, const T& up, int centered);

  protected:
  private:
  private:  //## implementation
      T* data;
      int Dim;
};


template <class T,class T1>//,class C=CopyStructFast<T>,class C1=CopyStructFast<T1> >
DataVector<T1,CopyStructFast<T1> >& ConvertVectors(DataVector<T,CopyStructFast<T> > &src,DataVector<T1,CopyStructFast<T1> > &dst)
 {
  int k,N=src.SetDim();
  dst.SetDim(N);
  for (k=0;k<N;k++) dst[k]=src[k];
  return dst;
 };


double VectorNorm2(DataVector<double, CopyStructFast<double> > &vec);
double VectorNorm1(DataVector<double, CopyStructFast<double> > &vec);
double VectorMod(DataVector<double, CopyStructFast<double> > &vec);


// ===========================================================
// =================   End of the head =======================
// ===========================================================











template <class T,class CopyStructMethod >
DataVector<T,CopyStructMethod>::DataVector(int Num, T* Val)
{
	Dim=Num;data=new T[Num];
    if (Val!=NULL) 
     CopyStructMethod::CopyArray(Val,data,Dim);//MemoryMove(Val,data,sizeof(T)*Dim);
}

template <class T,class CopyStructMethod >
DataVector<T,CopyStructMethod>::DataVector (const DataVector<T,CopyStructMethod> &vec):data(NULL), Dim(0)
	{//operator=(vec);
  SetDim(vec.Dim);
  CopyStructMethod::CopyArray(vec.data,data,vec.Dim);//MemoryMove(vec.data,this->data,sizeof(T)*vec.Dim);
	}


template <class T,class CopyStructMethod>
int DataVector<T,CopyStructMethod>::save_data_state (FilterOut &so)
{
	so<<KeyDesc("DataDim")<<Dim<<KeyDesc(" Data ");
	for (int k=0;k<Dim;k++) so<<data[k];//so<<"\n";
	return 1;
}
template <class T,class CopyStructMethod>
int DataVector<T,CopyStructMethod>::read_data_state (FilterIn &si)
{
	int dim;si>>StdKey>>dim>>StdKey;SetDim(dim);
	for (int k=0;k<Dim;k++) si>>data[k];
	return 1;
}

template <class T,class CopyStructMethod>
void DataVector<T,CopyStructMethod>::Delete () 
 {
  CopyStructMethod::DeleteArray(data,Dim);//delete[] data;
  data=NULL;Dim=0;
 }


template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod>& DataVector<T,CopyStructMethod>::operator = (const T& vec)
{
//	DataVector<T> ret(vec.Dim);
	for (int k=0;k<Dim;k++) data[k]=vec;
	return *this;
}

template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod>& DataVector<T,CopyStructMethod>::operator = (const DataVector<T,CopyStructMethod>& vec)
{
	SetDim(vec.Dim);
 CopyStructMethod::CopyArray(vec.data,data,vec.Dim);//MemoryMove(vec.data,data,sizeof(T)*vec.Dim);
	return *this;
}

template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod>& DataVector<T,CopyStructMethod>::operator = (T* vec)
{
	for (int k=0;k<Dim;k++) data[k]=vec[k];
	return *this;
}

template <class T,class CopyStructMethod>
int DataVector<T,CopyStructMethod>::SetDim (int NewDim)
{
	if (NewDim!=Dim) {Delete();data=new T[NewDim];Dim=NewDim;}
	return Dim;
}
template <class T,class CopyStructMethod>
void DataVector<T,CopyStructMethod>::Resize (int NewDim)
{
	if (NewDim==Dim) return;
	int old_dim=Dim;
 if (Dim==0) {SetDim(NewDim);return;}
	T* old_dat=data;
 data=NULL;
	SetDim(NewDim);
	CopyStructMethod::CopyArray(old_dat,data,min(old_dim,NewDim));//MemoryMove(old_dat,data,sizeof(T)*min(Dim,NewDim));
	CopyStructMethod::DeleteArray(old_dat,old_dim);//delete[] old_dat;
}
template <class T,class CopyStructMethod>
void DataVector<T,CopyStructMethod>::SetData (T* dat,int From,int To)
{
	if (From==-1) From=0;if (To==-1) To=Dim;
 if (To>Dim) {cout<<"Very bad in DataVector::SetData().\n";cout.flush();abort();}
	CopyStructMethod::CopyArray(dat,data,To-From);//MemoryMove(dat,data,sizeof(T)*(To-From));
}

template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod>::operator T*() const {return data;}

//inline int operator == (const DataVector<T,CopyStructMethod>& vec1, const DataVector<T,CopyStructMethod>& vec2);
//template <class T,class CopyStructMethod>
//int operator == (const DataVector<T,CopyStructMethod>& vec1, const DataVector<T,CopyStructMethod>& vec2)
//{
//	if (vec1.Dim!=vec2.Dim) return 0;
//	for (int k=0;k<vec1.Dim;k++) if (vec1[k]!=vec2[k]) return 0;
//	return 1;
//}








//template <class T,class CopyStructMethod>
//T& DataVector<T,CopyStructMethod>::operator [] (int k){	return data[k];}

//template <class T>
//DataVector<T>& DataVector<T>::operator = (double vec){}
//template <class T>
//DataVector<T>& operator * (const DataVector<T>& vec, const double& coef){}
//template <class T>
//DataVector<T>& operator / (const DataVector<T>& vec, const double& coef){}
//template <class T>
//DataVector<T>& operator + (const DataVector<T>& vec, const double& coef){}
//template <class T>
//DataVector<T>& operator - (const DataVector<T>& vec, const double& coef){}

template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod> DataVector<T,CopyStructMethod>::operator / (const T& coef)
{
	DataVector<T,CopyStructMethod> ret=*this;
	for (int k=0;k<ret.Dim;k++) ret.data[k]=data[k]/coef;
	return ret;
}

template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod> DataVector<T,CopyStructMethod>::operator * (const T& coef)
{
	DataVector<T,CopyStructMethod> ret=*this;
	for (int k=0;k<ret.Dim;k++) ret.data[k]=data[k]*coef;
	return ret;
}
template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod> DataVector<T,CopyStructMethod>::operator + (const T& coef)
{
	DataVector<T,CopyStructMethod> ret=*this;
	for (int k=0;k<ret.Dim;k++) ret.data[k]=data[k]+coef;
	return ret;
}
template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod> DataVector<T,CopyStructMethod>::operator - (const T& coef)
{
	DataVector<T,CopyStructMethod> ret=*this;
	for (int k=0;k<ret.Dim;k++) ret.data[k]=data[k]-coef;
	return ret;
};


/*
template <class T>
DataVector<T> operator / (const DataVector<T>& vec, const T& coef)
{
	DataVector<T> ret=vec;
	for (int k=0;k<ret.Dim;k++) ret.data[k]=ret.data[k]/coef;
	return ret;
}
template <class T>
DataVector<T> operator * (const DataVector<T>& vec, const T& coef)
{
	DataVector<T> ret=vec;
	for (int k=0;k<ret.Dim;k++) ret.data[k]=ret.data[k]*coef;
	return ret;
}

template <class T>
DataVector<T> operator + (const DataVector<T>& vec, const T& coef)
{
	DataVector<T> ret=vec;
	for (int k=0;k<ret.Dim;k++) ret.data[k]=ret.data[k]+coef;
	return ret;
}

template <class T>
DataVector<T> operator - (const DataVector<T>& vec, const T& coef)
{
	DataVector<T> ret=vec;
	for (int k=0;k<ret.Dim;k++) ret.data[k]=ret.data[k]-coef;
	return ret;
}

template <class T>
T operator * (const DataVector<T>& vec, const DataVector<T>& coef)
{
	T ret=0;
	if (vec.Dim!=coef.Dim) 
		{cerr<<"dimensions are different in operator vec1*vec2\nvec1.Dim="
			<<vec.Dim<<" vec2.Dim="<<coef.Dim<<" return 0 \n";return ret;
		}
	for (int k=0;k<vec.Dim;k++) ret=ret+vec.data[k]*coef.data[k];
	return ret;
}
template <class T>
DataVector<T> operator + (const DataVector<T>& vec, const DataVector<T>& coef)
{
	DataVector<T> ret=vec;
	if (vec.Dim!=coef.Dim) 
		{cerr<<"dimensions are different in operator vec1+vec2\nvec1.Dim="
			<<vec.Dim<<" vec2.Dim="<<coef.Dim<<" return 0 \n";return ret;
		}
	for (int k=0;k<vec.Dim;k++) ret.data[k]=ret.data[k]+coef.data[k];
	return ret;
}

template <class T>
DataVector<T> operator - (const DataVector<T>& vec, const DataVector<T>& coef)
{
	DataVector<T> ret=vec;
	if (vec.Dim!=coef.Dim) 
		{cerr<<"dimensions are different in operator vec1-vec2\nvec1.Dim="
			<<vec.Dim<<" vec2.Dim="<<coef.Dim<<" return 0 \n";return ret;
		}
	for (int k=0;k<vec.Dim;k++) ret.data[k]=ret.data[k]-coef.data[k];
	return ret;
}
*/
template <class T,class CopyStructMethod>
T DataVector<T,CopyStructMethod>::operator*(const DataVector<T,CopyStructMethod>& coef)
{
	T ret=0;
	if (Dim!=coef.Dim) 
		{cerr<<"dimensions are different in operator vec1*vec2\nvec1.Dim="
			<<Dim<<" vec2.Dim="<<coef.Dim<<" return 0 \n";return ret;
		}
	for (int k=0;k<Dim;k++) ret=ret+data[k]*coef.data[k];
	return ret;
}
template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod> DataVector<T,CopyStructMethod>::operator + (const DataVector<T,CopyStructMethod>& coef)
{
	DataVector<T,CopyStructMethod> ret(Dim);
	if (Dim!=coef.Dim) 
		{cerr<<"dimensions are different in operator vec1+vec2\nvec1.Dim="
			<<Dim<<" vec2.Dim="<<coef.Dim<<" return 0 \n";return ret;
		}
	for (int k=0;k<Dim;k++) ret.data[k]=data[k]+coef.data[k];
	return ret;
}

template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod> DataVector<T,CopyStructMethod>::operator - (const DataVector<T,CopyStructMethod>& coef)
{
	DataVector<T,CopyStructMethod> ret(Dim);
	if (Dim!=coef.Dim) 
		{cerr<<"dimensions are different in operator vec1-vec2\nvec1.Dim="
			<<Dim<<" vec2.Dim="<<coef.Dim<<" return 0 \n";return ret;
		}
	for (int k=0;k<Dim;k++) ret.data[k]=data[k]-coef.data[k];
	return ret;
};

template <class T,class CopyStructMethod>
DataVector<T,CopyStructMethod> DataVector<T,CopyStructMethod>::MakeStepVector
						(int Num, const T& low, const T& up, int centered){
	int N=Num ;
	T stp=up-low,start=low;
	if (centered) { stp/=N;start+=0.5*stp; }
	else stp/=max<int>(N-1,1);
	DataVector<T,CopyStructMethod> vec(N);
	for (int k=0;k<N;k++) {vec[k]=start;start+=stp;}
	return vec;
}




//#include "DataVec.cpp"

//struct SpacePoint :DataVector<double>
//{
//};


//static ClassDesc SpacePoint_cd(typeid(SpacePoint),"SpacePoint");

#endif
