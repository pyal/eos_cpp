
#ifndef DataTyp_h_ADD
#define DataTyp_h_ADD 1

#include "DataVec.h"
#include "VecRes.h"
#include "lib\ref\class_sav.h"


typedef DataVector<double, CopyStructFast<double> > SpacePoint;
typedef DataVector<int, CopyStructFast<int> > IndexPoint;
typedef ResizeVector<int, CopyStructFast<int> > ResizeIndex;

template <class T>
struct BoundTemplate : public SavableClass
 {
  DataVector<T, CopyStructFast<T> > Low,Up;
  BoundTemplate(const DataVector<T, CopyStructFast<T> >& low,const DataVector<T, CopyStructFast<T> >& up){Low=low;Up=up;};
  BoundTemplate():Low(0),Up(0){};
  BoundTemplate(const BoundTemplate<T> &bnd):Low(bnd.Low),Up(bnd.Up){};

  virtual DataVector<T, CopyStructFast<T> > Size(){return Up-Low;};
  int save_data_state (FilterOut &so)
   {so<<KeyDesc("Low")<<Low<<KeyDesc("Up")<<Up;return 1;};
  int read_data_state (FilterIn &si)
   {si>>StdKey>>Low>>StdKey>>Up;return 1;};
  BoundTemplate<T>& operator=(const BoundTemplate<T> &bnd){Low=bnd.Low;Up=bnd.Up;return *this;};
  virtual BoundTemplate<T> Set(const DataVector<T> &low,const DataVector<T> &up){Low=low;Up=up;return *this;};
 };

typedef BoundTemplate<double> SpaceBound;

struct IndBound  : public BoundTemplate<int>
{ 
// IndexPoint Low,Up;
 IndBound():BoundTemplate<int>(){};
 IndBound(const IndexPoint& low,const IndexPoint& up){Low=low;Up=up;};
 IndBound(const IndBound& bnd){Low=bnd.Low;Up=bnd.Up;};
 IndBound& operator=(const IndBound &bnd){Low=bnd.Low;Up=bnd.Up;return *this;};

// IndexPoint Size(){return Up-Low;};
 int NumPnt();
 IndexPoint Int2Pnt(int num);
 int Pnt2Int(const IndexPoint &ind);
// IndBound Set(const IndexPoint &low,const IndexPoint &up){Low=low;Up=up;return *this;};
// IndBound():Low(0),Up(0){};
// int save_data_state (FilterOut &so)
// {so<<KeyDesc("Low")<<Low<<KeyDesc("Up")<<Up;return 1;};
// int read_data_state (FilterIn &si)
// {si>>StdKey>>Low>>StdKey>>Up;return 1;};
};

int In_Lim(SpacePoint &Pnt,SpaceBound &Rect,int IncludeBound=1);
int SamePoint(SpacePoint &Pnt1,SpacePoint &Pnt2,double Accuracy=StndErr,
              SpacePoint *DirWeigths=NULL);



#endif