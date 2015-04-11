
#ifndef Grid_Bound_h
#define Grid_Bound_h 1

#include "grid_center.h"


// Defines simply numbers of boundary points ( in boundary grid storage ) for the
// current cell
// and virtual destructor
#define GridBoundIndPnt DataVector<GridIndex>
//struct GridBoundPnt : public SavableClass  
//{
//   GridBoundPnt(){};
//   virtual ~GridBoundPnt(){};

//   DataVector< GridIndex > GridBoundIndex;
//};

struct GridVarBoundInd;
struct GridBoundManipulator;
struct GridBound: Grid<GridBoundPnt>
 {
   GridVarBoundInd *centerind_ptr;
   GridBound(GridVarBoundInd *center=NULL);

   virtual int save_data_state (FilterOut &so) 
     {Grid<GridBoundPnt>::save_data_state(so);
      so<<KeyDesc("centerind_ptr")<<(SavableClass*)centerind_ptr; return 1;};
   virtual int read_data_state (FilterIn &si);
//   virtual void AddPnt(GridIndex k){Grid<BoundDesc>::AddPnt(k);data[k].IndNum=k;};
//   virtual void DelPnt(GridIndex k);//{Grid<GridCenterPnt>::DelPnt(k);};
//   virtual void ModifyPnt(GridIndex k){Grid<GridCenterPnt>::ModifyPnt(k);};
//   virtual void MovePnt(GridIndex From,GridIndex To);
  
// Not standart procedures, cannot be accessed through manipulator
   virtual GridVarBoundInd* SetCenterGrid(GridVarBoundInd *center=NULL);
// Dir - here means direction to compare normal
   virtual  DataVector<GridIndex> MaskCenterShift(DataVector<GridIndex> &mask,int dir,int Up)
    {
     int k,N=mask.SetDim(),up;
     DataVector<GridIndex> ret(N);
     GridBoundPnt tmp;
     for (k=0;k<N;k++) 
      {
       tmp=data[mask[k]];if (tmp.NormalDir[dir]<0) up=!Up;else up=Up;
       if (up) ret[k]=tmp.Second;else ret[k]=tmp.First;
      }
     return ret; 
    };
//   virtual void ModifyPnt(GridIndex k,BoundDesc& val) {val.IndNum=k;Grid<BoundDesc>::ModifyPnt(k,val);};
//   virtual void AddPnt(GridIndex k,BoundDesc& val) {val.IndNum=k;Grid<BoundDesc>::AddPnt(k,val);};
 };

// In the main store (GridCenter ) stored  GridVarBoundInd - variable to
// access the store of boundaries ( GridBound ) and connected
// with it boundary variables
struct GridVarBoundInd : public GridField<GridBoundIndPnt>
 {
   Ref<GridBound> gridbound_ptr;

//   GridVarBoundInd():GridField<GridBoundIndPnt>()
//    {  gridbound_ptr=new GridBound(this);}//CreateGridData();}
   GridVarBoundInd(GridManipulator *stor=NULL,char *name=NULL):GridField<GridBoundIndPnt>(stor,name)
    {  gridbound_ptr=new GridBound(this);CreateGridData();}
//   ~GridCenter()  {   gridbound_ptr=NULL;  };
//   void Delete();

   void CreateGridData();
// Procedures to be added
//   void ResetAllNeibous(int TopologyChanged=0);
//   void DeleteAllGrid();
// FrozeOperations  Not to react on any command, just to reset afterwards
//   int SetFrozen(int froze=-1); 

   virtual void AddPnt(GridIndex k,int ResetNeib=1);
   virtual void DelPnt(GridIndex k);
   virtual void ModifyPnt(GridIndex k);
   virtual void MovePnt(GridIndex From,GridIndex To);


// Not standart procedures, cannot be accessed through manipulator
//   virtual  DataVector<GridIndex> MaskBound(DataVector<GridIndex> &mask,IndexPoint &shift)
//    {DataVector<GridIndex> ret;return ret; };
//   Make shift in given direction
   virtual  DataVector<GridIndex> MaskBoundShift(DataVector<GridIndex> &mask,int dir)
    {
     int k,N=mask.SetDim();
     DataVector<GridIndex> ret(N);
     for (k=0;k<N;k++) ret[k]=data[mask[k]][dir];
     return ret; 
    };

   virtual int save_data_state (FilterOut &so) 
     {GridField<GridBoundIndPnt>::save_data_state(so); 
      so<<KeyDesc("gridbound_ptr")<<gridbound_ptr; return 1;};
   virtual int read_data_state (FilterIn &si)
     {GridField<GridBoundIndPnt>::read_data_state(si); 
      si>>StdKey>>gridbound_ptr;return 1;};

 protected:
   void DeleteOldBoundary(GridIndex k,int kpos);
   void AddNewBoundary(GridIndex k,int kpos);
   DataVector<GridIndex> GetCurNeib(GridIndex k);
   void ResetOldPointGeometry(GridIndex k);
   void ResetNewPointGeomtry(GridIndex k);
   void ResetPointNeibGeometry(GridIndex k,int PntExist);
   void ResetPointBounds(GridIndex k);
   void ResizeIndPnt(GridIndex k,int NewDim);
//  void CreatePointNeib(DataVector<GridBoundPnt> &bound_vect,GridIndex k);
//  void ResetPointNeib(DataVector<GridBoundPnt> &bound_vect,GridIndex k); 

//  int FrozenOperations;
 };

//works only with GridBound
struct GridBoundManipulator : public GridManipulatorStd<GridBound>
 {   
   GridBoundManipulator(){};//:GridManipulatorStd<GridBound>

   virtual SpacePoint GetGridPnt(GridIndex k)  {return grid->GetPnt(k).Center;}
   virtual void SetGridPnt(SpacePoint newVal,GridIndex k) {grid->GetPnt(k).Center=newVal;}


   virtual DataVector<SpacePoint,CopyStructSlow<SpacePoint> > GetGridMaskedPnt(DataVector<GridIndex> &mask);
   virtual void SetGridMaskedPnt(DataVector<SpacePoint,CopyStructSlow<SpacePoint> > &data,DataVector<GridIndex> &mask);



   DataVector<GridIndex> GetNeibous(GridIndex k);//{return store->GetPnt(k).Neibous;}
 };

#endif
