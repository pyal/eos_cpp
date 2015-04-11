#ifndef Grid_Manip_h
#define Grid_Manip_h 1

#include "Grid_Action.h"
#include "Grid_Mask.h"

#include "lib\data\NamedObject.h"




struct FieldInterface;
struct BoundaryInterface;
// General procedures for all types of grids are defined
struct GridManipulator : public NamedObjectInited
 {
// Init internal references - to be called after the Area is formed
//   virtual void Init()=0;
   virtual FieldInterface *GetGridField()=0;
   virtual BoundaryInterface *GetBound()=0;

// For GridIndex==-1 - return OldPnt Par
   virtual SpacePoint GetGridPnt(GridIndex k)=0;
   virtual void SetGridPnt(SpacePoint newVal,GridIndex k)=0;
   virtual DataVector<SpacePoint,CopyStructSlow<SpacePoint> > GetGridMaskedPnt(DataVector<GridIndex> &mask)=0;
   virtual void SetGridMaskedPnt(DataVector<SpacePoint,CopyStructSlow<SpacePoint> > &data,DataVector<GridIndex> &mask)=0;
//   virtual DataVector<GridIndex> ConstructMask(FieldInterface *dat)=0;

// For GridIndex==-1 - return OldPnt Par
   virtual DataVector<GridIndex, CopyStructSlow<GridIndex> > GetNeibous(GridIndex k)=0;

// Field Manipulator
   virtual int AddVar(FieldInterface *NewVar)=0;
   virtual void DelVar(FieldInterface *NewVar)=0;
   virtual FieldInterface* GetVar(char *name)=0;
   virtual FieldInterface* GetVar(int Num)=0;
   virtual int GetVar(FieldInterface* get2var)=0;
//   virtual FieldInterface* NextVar(int i);// virtual int StartVar();

// FieldPoints Manipulator
   virtual int NumPnt(int CountDeleted=0)=0; // if CountDeleted - return MaxInd of grid
   virtual GridIndex NextPnt(GridIndex i)=0;
   virtual GridIndex StartPnt() =0;
   virtual GridIndex StartEmptyIndex()=0;
   virtual GridIndex NextEmptyIndex(GridIndex i)=0;


// Frozen - means making a block of linked point addition
// All Points have to be added (deleted) first before sending
// information about changes to Vars linked Grid
// Frozen operations : AddVar DelVar save_data_state()
   virtual int GridFrozen(int frozen=-1)=0;//{if (frozen!=-1) Frozen=frozen;return Frozen;};

// This with the help of BoundaryInterface class from AreaStorage
   virtual void GetPntBound(GridIndex pnt,int pntpos,GridBoundPnt &bnd)=0;

   virtual void PrintAllGridVars(FilterOut &out)=0;
 };

// General procedures for all types of variables on grid are defined
struct FieldInterface : public NamedObject
 {
   virtual void AddPnt(GridIndex k)=0;    //Only After
   virtual void DelPnt(GridIndex k)=0;    //Only After (what about before ?)
   virtual void ModifyPnt(GridIndex k)=0; //Only After - grid structure not changed !!!
   virtual void MovePnt(GridIndex From,GridIndex To)=0; //Only After

// Grids and Field will redefine it (differently)
   virtual GridManipulator* GetGridManip()=0;
//   ~FieldInterface(){fcout<<" Deleting Field "<<get_obj_name()<<"\n";NamedObject::~NamedObject();}
 };

struct BoundaryInterface : public NamedObjectInited  
 {
// One point is boundary ( -1 - is a second neibour ); bnd First,Second,Pos are already formed by GetPntBound
//  virtual void GetBoundary(GridBoundPnt &bnd)=0;

// For all points - forming bnd
   virtual void GetPntBound(GridIndex pnt,int pntpos,GridBoundPnt &bnd)=0;
// One point is boundary ( -1 - is a neibour ); bnd First,Second,Pos are already formed by GetPntBound
   virtual void GetBoundary(GridBoundPnt &bnd)=0;
 };




struct MaskManipulatorGeneral;
struct MaskManipulatorStruct;
struct TriangulatorInterface : public NamedObject  
 {
  
  virtual FieldInterface  *CreateGridField()=0;
  virtual GridManipulator *CreateGridManip()=0;

  virtual MaskManipulatorGeneral *CreateMaskGen()=0;
  virtual MaskManipulatorStruct  *CreateMaskStr()=0;
  virtual BoundaryInterface *CreateBound()=0;

 };


//#include "grid_mask.h"
struct AreaStoreInterface: public NamedObject, public NamedObjectStoreAbstract 
 {
  enum { Triang,GridField,GridManip,MaskGen,MaskStr,Bound } AreaVars;
  AreaStoreInterface(){VarList.set_DefaultStorage(this);};
  virtual void CreateArea(TriangulatorInterface *newTriang);

  inline GridManipulator* GetGridManip(GridManipulator *newGrid=NULL) 
   {if (newGrid!=NULL) VarList.Add(newGrid,GridManip); 
    return dynamic_cast<GridManipulator*>(VarList.GetPtr(GridManip));}
  inline BoundaryInterface* GetBound(BoundaryInterface *newBound=NULL) 
   {if (newBound!=NULL) VarList.Add(newBound,Bound);
    return dynamic_cast<BoundaryInterface*>(VarList.GetPtr(Bound));}
  inline TriangulatorInterface* GetTriang(TriangulatorInterface *newTriang=NULL) 
   {if (newTriang!=NULL) VarList.Add(newTriang,Triang);
    return dynamic_cast<TriangulatorInterface*>(VarList.GetPtr(Triang));}
  inline FieldInterface* GetGridField(FieldInterface *newGrid=NULL) 
   {if (newGrid!=NULL) VarList.Add(newGrid,GridField);
    return dynamic_cast<FieldInterface*>(VarList.GetPtr(GridField));}
/*
  inline MaskManipulatorGeneral* GetMaskGen(MaskManipulatorGeneral *newMask=NULL) 
   {if (newMask!=NULL) VarList[MaskGen]=newMask; 
    return dynamic_cast<MaskManipulatorGeneral*>(VarList[MaskGen].pointer());}
  inline MaskManipulatorStruct* GetMaskStr(MaskManipulatorStruct *newMask=NULL) 
   {if (newMask!=NULL) VarList[MaskStr]=newMask; 
    return dynamic_cast<MaskManipulatorStruct*>(VarList[MaskStr].pointer());}
*/
  MaskManipulatorGeneral* GetMaskGen(MaskManipulatorGeneral *newMask=NULL); 
  MaskManipulatorStruct* GetMaskStr(MaskManipulatorStruct *newMask=NULL) ;

  virtual int save_data_state (FilterOut &so) {so<<KeyDesc("VarList")<<VarList; return 1;};
  virtual int read_data_state (FilterIn &si)  {si>>StdKey>>VarList; return 1;};
  

  NamedStore VarList;
// from NamedObjectStoreAbstract
   inline void DeleteObject(NamedObject *obj)       {VarList.DeleteObject(obj);};
   inline  int Add (NamedObject* dat, int pos = -1) {return VarList.Add(dat,pos);};
 };



/*
#include "grid_mask.h"
inline MaskManipulatorGeneral* AreaStoreInterface::GetMaskGen(MaskManipulatorGeneral *newMask=NULL) 
 {
  if (newMask!=NULL) VarList[MaskGen]=newMask; 
  return dynamic_cast<MaskManipulatorGeneral*>
                     (VarList[MaskGen].pointer());
 };
inline MaskManipulatorStruct* AreaStoreInterface::GetMaskStr(MaskManipulatorStruct *newMask=NULL) 
 {if (newMask!=NULL) VarList[MaskStr]=newMask; 
  return dynamic_cast<MaskManipulatorStruct*>
                     (VarList[MaskStr].pointer());
 };
*/
 

#endif















