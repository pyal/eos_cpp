

#include "grid_util.h"


GridMask Dat2Mask(FieldInterface *dat)
 {
  GridField<int> *mask_arr=dynamic_cast<GridField<int>* >(dat);
  if (mask_arr==NULL) { ExEnv::err()<<" Dat2Mask dat - cannot be converted to GridField<GridIndex>. Error. \n"<<endl;abort();}
  GridManipulator *grid=mask_arr->GetGridManip();
  int N=grid->NumPnt(),n=0;
  GridMask ret(N);
  for (GridIndex i=grid->StartPnt();i>=0;i=grid->NextPnt(i)) 
    if (mask_arr->GetPnt(i)!=0) {ret[n]=i;n++;}
  ret.Resize(n);
  return ret;
 };
void Mask2Dat(GridMask &mask,FieldInterface *dat)
 {
  GridField<int> *mask_arr=dynamic_cast<GridField<int>* >(dat);
  if (mask_arr==NULL) { ExEnv::err()<<" Mask2Dat dat - cannot be converted to GridField<GridIndex>. Error. \n"<<endl;abort();}
  GridManipulator *grid=mask_arr->GetGridManip();
  int k;
  for (GridIndex i=grid->StartPnt();i>=0;i=grid->NextPnt(i)) mask_arr->GetPnt(i)=0;
  for (k=0;k<mask.SetDim();k++) mask_arr->GetPnt(mask[k])=1;
 };

//#include "Grid_Bound.h"

void SetCircleBoundaryMask(GridManipulator *grid,MaskBoundaryCond &BndCond,int AccuracySpace)
//void SetCircleBoundaryMask(GridManipulator *grid,GridMask &MaskBndGet,GridMask &MaskBndPut,int AccuracySpace)
 {
  GridMask &MaskBndGet=BndCond.MaskGet,&MaskBndPut=BndCond.MaskPut;
  AreaStoreInterface *tmp=dynamic_cast<AreaStoreInterface*>(grid->get_StoredBy());
  if (!tmp){fcout<<" MaskBoundaryClc::SetNewGrid NULL area\n";abort();}
  MaskManipulatorGeneral *mask_manip=tmp->GetMaskGen();
  GridMask MaskEnd0=mask_manip->MaskBoundary();
  int N0=MaskEnd0.SetDim(),N1=AccuracySpace,k1,k;
  DataVector<GridMask,CopyStructSlow<GridMask> > SL(2*N1),SR(2*N1);

  MaskBndGet.SetDim(N0*N1);MaskBndPut.SetDim(N0*N1);
  SL[0]=MaskEnd0;SR[0]=MaskEnd0;
  for (k=1;k<2*N1;k++) 
   {SL[k]=mask_manip->ShiftMask(SL[k-1],1,1);SR[k]=mask_manip->ShiftMask(SR[k-1],0,1);}
  int ng=0,np=0;
  for (k=0;k<N0;k++) 
   {
    if (SL[1][k]<0) 
     {for (k1=0;k1<N1;k1++) {MaskBndPut[ng]=SR[k1][k];ng++;}}
    else
     {for (k1=0;k1<N1;k1++) {MaskBndPut[ng]=SL[k1][k];ng++;}}
   }
  ng=0;
  for (k=N0-1;k>=0;k--) 
   {
    if (SL[1][k]<0) 
     {for (k1=2*N1-1;k1>=N1;k1--) {MaskBndGet[ng]=SR[k1][k];ng++;}}
    else
     {for (k1=2*N1-1;k1>=N1;k1--) {MaskBndGet[ng]=SL[k1][k];ng++;}}
   }
 };

void SetData2Coord(FieldInterface *data,GridMask &mask,int axis)
 {
  GridManipulator *grid=data->GetGridManip();
  GridField<double> *dat=dynamic_cast<GridField<double>* >(data);
  if (dat==NULL) {fcout<<" SetData2Coord : data have to be vector of double.\n";fcout.flush();abort();}
  DataVector<SpacePoint,CopyStructSlow<SpacePoint> > coord=grid->GetGridMaskedPnt(mask);
  DataVector<double, CopyStructFast<double> > set=dat->GetMaskedPnt(mask);
  for (int k=0;k<mask.SetDim();k++) coord[k][axis]=set[k];
  grid->SetGridMaskedPnt(coord,mask);
 }
void SetCoord2Data(FieldInterface *data,GridMask &mask,int axis)
 {
  GridManipulator *grid=data->GetGridManip();
  GridField<double> *dat=dynamic_cast<GridField<double>* >(data);
  if (dat==NULL) {fcout<<" SetData2Coord : data have to be vector of double.\n";fcout.flush();abort();}
  DataVector<SpacePoint,CopyStructSlow<SpacePoint> > coord=grid->GetGridMaskedPnt(mask);
  DataVector<double, CopyStructFast<double> > set(mask.SetDim());
  for (int k=0;k<mask.SetDim();k++) set[k]=coord[k][axis];
  dat->SetMaskedPnt(set,mask);
 }


DataVector<double> GetGridCoord(GridManipulator *grid,GridMask &mask,int axis)
 {
  DataVector<SpacePoint,CopyStructSlow<SpacePoint> > coord;
  coord=grid->GetGridMaskedPnt(mask);
  DataVector<double> ret(mask.SetDim());
  for (int k=0;k<mask.SetDim();k++) ret[k]=coord[k][axis];
  return ret;
 }
void SetGridCoord(GridManipulator *grid,GridMask &mask,int axis,DataVector<double> &ToSet)
 {
  DataVector<SpacePoint,CopyStructSlow<SpacePoint> > coord;
  coord=grid->GetGridMaskedPnt(mask);
  for (int k=0;k<mask.SetDim();k++) coord[k][axis]=ToSet[k];
  grid->SetGridMaskedPnt(coord,mask);
 }

// ===========================================================================
// ===========================================================================
// ===========================================================================


/*
// ===========================================================================
// ======================   MaskBoundaryClc     ==============================
// ===========================================================================

// Just now do not work for unstructured,

void MaskBoundaryClc::SetNewGrid(GridManipulator *m_grid,GridMask &CenterMask,char *Bound_name)
 {
  if (Bound_name==NULL) strcpy(name_of_boundary,"CellBoundary");
  else strcpy(name_of_boundary,Bound_name);
  MaskC=CenterMask;grid=m_grid;

  GridVarBoundInd *Boundary=dynamic_cast<GridVarBoundInd*>(grid->GetVar(name_of_boundary));
  if (Boundary==NULL) Boundary=new GridVarBoundInd(grid,name_of_boundary);

  bgrid_main=Boundary->gridbound_ptr;bgrid=bgrid_main->GetGridManip();
  AreaStoreInterface *tmp=dynamic_cast<AreaStoreInterface*>(m_grid->get_StoredBy());
  if (!tmp){fcout<<" MaskBoundaryClc::SetNewGrid NULL area\n";abort();}
  MaskManipulatorGeneral *mask_manip=tmp->GetMaskGen();
  int N=grid->GetGridPnt(grid->StartPnt()).SetDim();
  GridMask bl,br,bc,l,r,c;
  for (int k=0;k<N;k++)
   {
    BMaskL=Boundary->MaskBoundShift(MaskC,k);
    BMaskR=Boundary->MaskBoundShift(MaskC,k+1);
    BMaskC=mask_manip->AddMask(BMaskL,BMaskR);
    MaskL=bgrid_main->MaskCenterShift(BMaskC,k,0);
    MaskR=bgrid_main->MaskCenterShift(BMaskC,k,1);
    bl=mask_manip->SumMask(BMaskL,bl);
    br=mask_manip->SumMask(BMaskR,br);
    bc=mask_manip->SumMask(BMaskC,bc);
    l=mask_manip->SumMask(MaskL,l);
    r=mask_manip->SumMask(MaskR,r);
    c=mask_manip->SumMask(MaskC,c);
   }
  BMaskL=bl;BMaskR=br;BMaskC=bc;MaskL=l;MaskR=r;MaskC=c;
 };


int MaskBoundaryClc::save_data_state (FilterOut &so)
 {
  so<<KeyDesc("bgrid")<<bgrid<<KeyDesc("grid")<<grid<<KeyDesc("bgrid_main")<<bgrid_main
   <<KeyDesc("name_of_boundary")<<name_of_boundary
   <<KeyDesc("MaskC")<<MaskC<<KeyDesc("BMaskL")<<BMaskL<<KeyDesc("BMaskR")<<BMaskR
   <<KeyDesc("BMaskC")<<BMaskC<<KeyDesc("MaskL")<<MaskL<<KeyDesc("MaskR")<<MaskR;
  return 1;
 };
int MaskBoundaryClc::read_data_state (FilterIn &si)
 {
  si>>StdKey >>bgrid>>StdKey >>grid>>StdKey >>bgrid_main
   >>StdKey >>name_of_boundary
   >>StdKey >>MaskC>>StdKey >>BMaskL>>StdKey >>BMaskR
   >>StdKey >>BMaskC>>StdKey >>MaskL>>StdKey >>MaskR;
  return 1;
 };
*/