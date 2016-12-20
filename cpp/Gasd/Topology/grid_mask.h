#ifndef Grid_Mask_h
#define Grid_Mask_h 1


#include "Grid_Action.h"
#include "lib/data/NamedObject.h"

//#include "Grid_manip.h"
#//include "grid_array.h"





struct GridManipulator;

//template <class T,class CpM>
template <class T,class CpM=CopyStructFast<T> >
struct GridVectorField;
//struct GridVectorField <class T,class CpM>;

struct MaskManipulatorStruct:NamedObjectInited
 {
  GridManipulator *Grid;
  GridVectorField<double> *Center;
  GridVectorField<int, CopyStructSlow<int> > *Neibous;
  GridMask MaskMain;
  DataVector<int> Cvt;
  int Dim;

  MaskManipulatorStruct(GridManipulator *grid,GridMask &mask):Grid(NULL){SetNewGrid(grid,mask);}
  MaskManipulatorStruct():Grid(NULL){}
  virtual void Init();
  void SetNewGrid(GridManipulator *grid,GridMask &mask);
  DataVector<int> SetCvt(GridMask &CvtTo);
  void SetGridVal(GridVectorField<double> *pos=NULL);
  GridVectorField<double> *GetCenter(GridVectorField<double> *pos=NULL);
    
  GridMask Shift(const GridMask &mask,int Axis,int Level);
  GridMaskVector Shift(const GridMask &mask,int Level);
  GridMaskVector Shift(const GridMaskVector &mask,int Level);

 };


struct GridMaskShift
 {
  int Axis,Level;
  MaskManipulatorStruct *Shift;
  GridMaskShift(int axis,int level,MaskManipulatorStruct *sh):Axis(axis),Level(level),Shift(sh){};
 };
struct GridMaskShiftVector
 {
  int Level;
  MaskManipulatorStruct *Shift;
  GridMaskShiftVector(int level,MaskManipulatorStruct *sh):Level(level),Shift(sh){};
 };

inline GridMask operator + (const GridMask &m,const GridMaskShift &i)
 {return i.Shift->Shift(m,i.Axis,i.Level); }
inline GridMaskVector operator+(const GridMask &m,const GridMaskShiftVector &i)
 {return i.Shift->Shift(m,i.Level); }
inline GridMaskVector operator+(const GridMaskVector &m,const GridMaskShiftVector &i) 
 { return i.Shift->Shift(m,i.Level); }




struct FieldInterface;

struct MaskManipulatorGeneral : public NamedObjectInited
 {
 MaskManipulatorGeneral(GridManipulator *gr):grid(gr){}
  MaskManipulatorGeneral(){}

  virtual void Init();
  virtual GridMask MaskBoundary(int level=0);
  virtual GridMask MaskAll();
  virtual GridMask ShiftMask(GridMask &mask,int dir,int level=1,int sum=0);
  virtual GridMask AddMask(GridMask &one,GridMask &two);
// Direct summation of masks
  virtual GridMask SumMask(GridMask &one,GridMask &two);
  virtual GridMask RemoveMask(GridMask &was,GridMask &toremov);
//  GridMask OrderMask(GridMask &mask);

  virtual int save_data_state (FilterOut &so);
  virtual int read_data_state (FilterIn &si);

//  Ref<MaskManipulatorStruct> shift;
  GridManipulator *grid;
 private:
  int IsIndexNew(GridIndex &i,GridMask &arr,int Len);
 };


#endif