#include <lib\precompiled\data.h>

#include "DataVec.h"
//template <class T>
//DataVector<T>::DataVector(int Num, T* Val)
//{
//	Dim=Num;data=new T[Num];
//    if (Val!=NULL) MemoryMove(Val.data,data,sizeof(T)*vec.Dim);
//}
// Do not work ? Really?
static ClassDesc DataVector_double_cd(typeid(DataVector<double, CopyStructFast<double> >),"DataVector_double");
static ClassDesc DataVector_int_cd(typeid(DataVector<int, CopyStructFast<int> >),"DataVector_int");
static ClassDesc DataVector_Stroka_cd(typeid(DataVector<Stroka>),"DataVector_Stroka");



double VectorNorm2(DataVector<double, CopyStructFast<double> > &vec)
 {return vec*vec;}
double VectorNorm1(DataVector<double, CopyStructFast<double> > &vec)
 {return sqrt(vec*vec);}
double VectorMod(DataVector<double, CopyStructFast<double> > &vec)
 {double ret=0;for (int k=0;k<vec.SetDim();k++) ret+=fabs(vec[k]);return ret;}
