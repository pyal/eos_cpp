#include "Grid_Center.h"


DescribedClass* create_GridCenterManipulator_cd(){return new GridCenterManipulator;}
static ClassDesc GridCenter_cd(typeid(GridCenter),"GridCenter");
static ClassDesc GridCenterManipulator_cd(typeid(GridCenterManipulator),"GridCenterManipulator",1,"Grid",&create_GridCenterManipulator_cd);

static ClassDesc GridCenterPnt_cd(typeid(GridCenterPnt),"GridCenterPnt");

static ClassDesc GridField_GridCenterPnt_Slow_cd(typeid(GridField<GridCenterPnt,CopyStructSlow<GridCenterPnt> >),"GridField_GridCenterPnt_Slow");

DescribedClass* create_TriangulatorSingleSquare_cd(){return new TriangulatorSingleSquare;}
static ClassDesc TriangulatorSingleSquare_cd(typeid(TriangulatorSingleSquare),"TriangulatorSingleSquare",1,"Grid",&create_TriangulatorSingleSquare_cd);

//BoundaryCenter
DescribedClass* create_BoundaryCenter_cd(){return new BoundaryCenter;}
static ClassDesc BoundaryCenter_cd(typeid(BoundaryCenter),"BoundaryCenter",1,"Grid",&create_BoundaryCenter_cd);

// =====================================================================
// =================         GridCenterPnt         =====================
// =====================================================================
int GridCenterPnt::save_data_state (FilterOut &so)
 {
// { SetLeakTest();so<<KeyDesc("Store")<<store;LeakTest(); return 1;}
  so<<KeyDesc("IndNum")<<IndNum<<KeyDesc("Coord")<<Coord
    <<KeyDesc("Volume")<<Volume<<KeyDesc("Neibous")<<Neibous<<KeyDesc("EndPnt\n");
  return 1;
 }
int GridCenterPnt::read_data_state (FilterIn &si)
 {
  si>>StdKey>>IndNum>>StdKey>>Coord>>StdKey>>Volume>>StdKey>>Neibous>>StdKey;
  return 1;
 }


// =====================================================================
// =================        GridCenter          =====================
// =====================================================================



  

void GridCenter::MovePnt(GridIndex From,GridIndex To)
  {
   int i,k,k1;
   for (k=0;k<data[From].Neibous.SetDim();k++) 
    if ((i=data[From].Neibous[k])!=-1) 
     {
      for (k1=0;k1<data[i].Neibous.SetDim();k1++) 
        if (data[i].Neibous[k1]==From) {data[i].Neibous[k1]=To;}
     }
   data[From].IndNum=To;
   Grid<GridCenterPnt>::MovePnt(From,To);
  };

void GridCenter::DelPnt(GridIndex x)
 {
   int i,k,k1;
   for (k=0;k<data[x].Neibous.SetDim();k++) 
    if ((i=data[x].Neibous[k])!=-1) 
     {
      for (k1=0;k1<data[i].Neibous.SetDim();k1++) 
        if (data[i].Neibous[k1]==x) {data[i].Neibous[k1]=-1;}
     }
  
  Grid<GridCenterPnt>::DelPnt(x);
 };


// =====================================================================
// =================        GridCenterManipulator::=====================
// =====================================================================


DataVector<SpacePoint,CopyStructSlow<SpacePoint> > GridCenterManipulator::
                       GetGridMaskedPnt(DataVector<GridIndex> &mask)
 { 
  DataVector<GridCenterPnt,CopyStructSlow<GridCenterPnt> > pnt=grid->GetMaskedPnt(mask);
  int k,N=pnt.SetDim();
  DataVector<SpacePoint,CopyStructSlow<SpacePoint> > ret(N);
  for (k=0;k<N;k++) ret[k]=pnt[k].Coord;
//fcout<<"\n\n\n"<<ret<<"\n";
//delete pnt;
  return ret;
 };
  
void GridCenterManipulator::
     SetGridMaskedPnt(DataVector<SpacePoint,CopyStructSlow<SpacePoint> > &data,DataVector<GridIndex> &mask)
 { 
  DataVector<GridCenterPnt,CopyStructSlow<GridCenterPnt> > pnt;pnt=grid->GetMaskedPnt(mask);
  int k,N=pnt.SetDim();
  DataVector<SpacePoint> ret(N);
  for (k=0;k<N;k++) pnt[k].Coord=data[k];
  grid->SetMaskedPnt(pnt,mask);
 };

// =====================================================================
// =================        TriangulatorSingleSquare ===================
// =====================================================================

FieldInterface *TriangulatorSingleSquare::CreateGridField()
 {
  GridCenter *stor=new GridCenter;
  GridCenterPnt Cent;
  int k,k1,N=index.NumPnt(),N0=index.Low.SetDim();
  IndexPoint dim=index.Size(),ind,tmp_ind;
  SpacePoint add=bound.Low,mul=bound.Size(),pnt(N0);
//  for (k=0;k<N0;k++) mul[k]/=(dim[k]+1);
  for (k=0;k<N0;k++) mul[k]/=(dim[k]);
  add=add+mul*0.5;
  DataVector<GridIndex> neib(2*N0);
  double vol=mul[0];
  for (k=1;k<N0;k++) vol*=mul[k];
  for (k=0;k<N;k++) 
   {
    ind=index.Int2Pnt(k);
    for (k1=0;k1<N0;k1++) pnt[k1]=add[k1]+(ind[k1]-index.Low[k1])*mul[k1];
    for (k1=0;k1<N0;k1++) 
     {tmp_ind=ind;tmp_ind[k1]=ind[k1]-1;neib[2*k1]=index.Pnt2Int(tmp_ind);
      tmp_ind[k1]=ind[k1]+1;neib[2*k1+1]=index.Pnt2Int(tmp_ind);}
    stor->AddPnt(k,Cent.Set(pnt,k,vol,neib));
   }
  return stor;
 };

BoundaryInterface *TriangulatorSingleSquare::CreateBound(){return new BoundaryCenter;}

//GridManipulator *TriangulatorSingleSquare::CreateGridManip()
//MaskManipulatorGeneral *TriangulatorSingleSquare::CreateMask()
//BoundaryInterface *TriangulatorSingleSquare::CreateBound()

// =====================================================================
// =================        BoundaryCenter           ===================
// =====================================================================
  
void BoundaryCenter::GetBoundary(GridBoundPnt &bnd)
 {
   BoundaryInterfaceStd::GetBoundary(bnd);
   SpacePoint BndPnt=grid_manip->GetGridPnt(bnd.First);
   SpacePoint mul=triang->bound.Size();
   IndexPoint dim=triang->index.Size();
   double div=bnd.FirstPos/2.;
   int dir=floor(div);
   double add=(mul[dir]/(dim[dir]))*0.5;
   if (dir==div) BndPnt[dir]-=add;
   else BndPnt[dir]+=add;
   bnd.Center=BndPnt;
 }





