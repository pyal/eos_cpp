#include "Grid_Interface.h"


DescribedClass* create_GridField_double_cd(){return new GridField<double>;}
static ClassDesc GridField_double_cd(typeid(GridField<double>),"GridField_double",1,"Grid",&create_GridField_double_cd);

DescribedClass* create_GridField_int_cd(){return new GridField<int>;}
static ClassDesc GridField_int_cd(typeid(GridField<int>),"GridField_int",1,"Grid",&create_GridField_int_cd);

static ClassDesc ResizeVector_FieldInterface_CopyStructRef_cd(
   typeid(ResizeVector< Ref<FieldInterface>,CopyStructRef<Ref<FieldInterface> > >),"ResizeVector_FieldInterface_CopyStructRef");



// =====================================================================
// =================        BoundaryInterfaceStd   =====================
// =====================================================================



// For all points - forming bnd
void BoundaryInterfaceStd::GetPntBound(GridIndex k,int kpos,GridBoundPnt &bnd)
 {
  DataVector<GridIndex, CopyStructSlow<GridIndex> > k1_ind,k_ind;
  k_ind=grid_manip->GetNeibous(k);

  GridIndex k1=k_ind[kpos];
  bnd.First=k;bnd.FirstPos=kpos;
  bnd.Second=k1;bnd.SecondPos=-1;
  if (k1==-1) {GetBoundary(bnd);return ;}

  k1_ind=grid_manip->GetNeibous(k1);
  for (int k1pos=0;k1pos<k1_ind.SetDim();k1pos++)
    if (k1_ind[k1pos]==k) {bnd.SecondPos=k1pos;goto Good;}
  ExEnv::err()<<" CouldNot Find Linked Reference to point in  Grid<T>::GetPntBound."<<endl;abort();
Good:;

  SpacePoint k_pnt,k1_pnt,delta_pnt;
  k_pnt=grid_manip->GetGridPnt(k);
  k1_pnt=grid_manip->GetGridPnt(k1);

  bnd.Surface=0;
  bnd.Center=(k1_pnt+k_pnt)*0.5;
  delta_pnt=k_pnt-k1_pnt;double norm=delta_pnt*delta_pnt;
  if (norm>MathZer) bnd.NormalDir=delta_pnt/sqrt(norm);
  else bnd.NormalDir=delta_pnt*0;

 }
// One point is boundary ( -1 - is a neibour ); bnd First,Second,Pos are already formed by GetPntBound
void BoundaryInterfaceStd::GetBoundary(GridBoundPnt &bnd)
 {
  if (bnd.Second!=-1)
   { ExEnv::err()<<" Point is not boundary in  Grid<T>::GetBoundary."<<endl;abort();}

  bnd.Surface=0;
  bnd.Center=grid_manip->GetGridPnt(bnd.First);
  bnd.NormalDir=bnd.Center*0.+1.;

 };








