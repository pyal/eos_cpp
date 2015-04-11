#ifndef Grid_Center_h
#define Grid_Center_h 1

#include "grid_interface.h"


struct GridCenterPnt : public SavableClass  
{
  public:

   GridCenterPnt():IndNum(0),Coord(0),Neibous(0),Volume(0){};
   GridCenterPnt(const SpacePoint &Spnt,int Npnt=0):IndNum(Npnt),Coord(Spnt),Volume(0), Neibous(0){};
   GridCenterPnt(const GridCenterPnt &pnt)            {IndNum=pnt.IndNum;Coord=pnt.Coord;Volume=pnt.Volume;Neibous=pnt.Neibous;}
   GridCenterPnt &operator=(const GridCenterPnt &pnt) {IndNum=pnt.IndNum;Coord=pnt.Coord;Volume=pnt.Volume;Neibous=pnt.Neibous;return *this;}

   virtual int save_data_state (FilterOut &so);
   virtual int read_data_state (FilterIn &si);

    GridCenterPnt& Set(const SpacePoint &Spnt,int ind,double vol, DataVector<GridIndex> &Neib)
     {Coord=Spnt;Volume=vol;IndNum=ind;Neibous=Neib;return *this;}

 //  protected://  private:

    GridIndex IndNum;
    SpacePoint Coord;
    double Volume;
    DataVector<GridIndex> Neibous;

};
  

struct GridCenter : public Grid<GridCenterPnt>
 {
   GridCenter(){};//{SetRefMask(SingleFileStorage);};

//   virtual void AddPnt(GridIndex k){DefaultValue.IndNum=k;Grid<GridCenterPnt>::AddPnt(k);};
   virtual void DelPnt(GridIndex k);//{Grid<GridCenterPnt>::DelPnt(k);};
//   virtual void ModifyPnt(GridIndex k){Grid<GridCenterPnt>::ModifyPnt(k);};
   virtual void MovePnt(GridIndex From,GridIndex To);


// Not standart procedures, cannot be accessed through manipulator
   virtual void ModifyPnt(GridIndex k,GridCenterPnt& val) {val.IndNum=k;Grid<GridCenterPnt>::ModifyPnt(k,val);};
   virtual void AddPnt(GridIndex k,GridCenterPnt& val) {val.IndNum=k;Grid<GridCenterPnt>::AddPnt(k,val);};

 };

struct GridCenterManipulator : public GridManipulatorStd<GridCenter>
 {   
//   GridCenterManipulator(){};


   virtual SpacePoint GetGridPnt(GridIndex k)  {return grid->GetPnt(k).Coord;}
   virtual void SetGridPnt(SpacePoint newVal,GridIndex k) {grid->GetPnt(k).Coord=newVal;}
   virtual DataVector<SpacePoint,CopyStructSlow<SpacePoint> > GetGridMaskedPnt(DataVector<GridIndex> &mask);
     
   virtual void SetGridMaskedPnt(DataVector<SpacePoint,CopyStructSlow<SpacePoint> > &data,DataVector<GridIndex> &mask);

//   SpacePoint GetOldPnt()                       {return store->OldGridKValue.Coord;}
   DataVector<GridIndex> GetNeibous(GridIndex k){return grid->GetPnt(k).Neibous;}

 };

struct BoundaryCenter;
struct TriangulatorSingleSquare : public TriangulatorInterface
 {
  TriangulatorSingleSquare(){};
  ~TriangulatorSingleSquare(){};
  TriangulatorSingleSquare(SpaceBound &bnd,IndBound &ind) {SetRegion(bnd,ind);}
  void SetRegion(SpaceBound &bnd,IndBound &ind){bound=bnd;index=ind;}

  virtual int save_data_state (FilterOut &so)
   {
    TriangulatorInterface::save_data_state(so);
    so<<KeyDesc("bound")<<bound<<KeyDesc("index")<<index; return 1;
   };
  virtual int read_data_state (FilterIn &si)
   {
    TriangulatorInterface::read_data_state(si);
    si>>StdKey>>bound>>StdKey>>index;return 1;
   };
  SpaceBound bound;
  IndBound index;

  virtual FieldInterface *CreateGridField();
  virtual GridManipulator *CreateGridManip() { return new GridCenterManipulator;};

  virtual MaskManipulatorGeneral *CreateMaskGen(){return new MaskManipulatorGeneral;}
  virtual MaskManipulatorStruct *CreateMaskStr(){return new MaskManipulatorStruct;}
  virtual BoundaryInterface *CreateBound();

 };


struct BoundaryCenter : public BoundaryInterfaceStd
 {
  BoundaryCenter(){};
  TriangulatorSingleSquare *triang;
  inline void Init() { BoundaryInterfaceStd::Init();AreaStoreInterface *ar=dynamic_cast<AreaStoreInterface*>(get_StoredBy());triang=dynamic_cast<TriangulatorSingleSquare*>(ar->GetTriang());}
  
  void GetBoundary(GridBoundPnt &bnd);
  virtual int save_data_state (FilterOut &so)
   {BoundaryInterfaceStd::save_data_state(so);so<<KeyDesc("triang")<<(SavableClass*)triang; return 1;};
  virtual int read_data_state (FilterIn &si)
   {BoundaryInterfaceStd::read_data_state(si);SavableClass* tmp;si>>StdKey>>tmp;triang=(TriangulatorSingleSquare*)tmp;return 1;};
 };

#endif








