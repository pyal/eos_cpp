#ifndef Grid_Util_h
#define Grid_Util_h 1

#include "Grid_Mask.h"
#include "Grid_Interface.h"



struct MaskBoundaryCond
 {
  GridMask MaskGet,MaskPut;
 };


GridMask Dat2Mask(FieldInterface *dat);
void Mask2Dat(GridMask &mask,FieldInterface *dat);
void SetData2Coord(FieldInterface *data,GridMask &mask,int axis);
void SetCoord2Data(FieldInterface *data,GridMask &mask,int axis);
// Circle Boundary Masks  Do not work for unstructured of cause
// Algorithm now do not treat corners,can be added
// For each ghost point - finds point in region on the boundary 
// Result example : reg 0..50, accuracy 2 ; Get : 2 3 50 51 Put 52 53 0 1
void SetCircleBoundaryMask(GridManipulator *grid,MaskBoundaryCond &BndCond,int AccuracySpace);

DataVector<double> GetGridCoord(GridManipulator *grid,GridMask &mask,int axis);
void SetGridCoord(GridManipulator *grid,GridMask &mask,int axis,DataVector<double> &ToSet);



/*
struct GridBound;

// Just now do not work for unstructured,
// Try to use different mask ideology for unstructured case
// No right-left shift, only Direction + Shift; -> Dim= Sum(i,NumNeib(i))
struct MaskBoundaryClc :SavableClass
 {
  MaskBoundaryClc(){};
  MaskBoundaryClc(GridManipulator *m_grid,GridMask &CenterMask,char *Bound_name=NULL) {SetNewGrid(m_grid,CenterMask,Bound_name);}

  virtual void SetNewGrid(GridManipulator *m_grid,GridMask &CenterMask,char *Bound_name=NULL);
//  virtual GridMask GetDirKMask(GridMask &ToGetFrom,int dir);
  virtual int save_data_state (FilterOut &so);
  virtual int read_data_state (FilterIn &si);
  
  GridManipulator *bgrid,*grid;
  GridBound *bgrid_main;
  char name_of_boundary[50];
// MaskC         - Main Center Mask                   - Dim k*SpaceDim
// BMaskL,BMaskR - Linked Left and Right boundaries   - Dim k*SpaceDim
  GridMask MaskC,BMaskL,BMaskR;
// BMaskC        - Main Boundary Mask                 - Dim (N+1)*SpaceDim
// MaskL,MaskR   - Linked Left and Right center points- Dim (N+1)*SpaceDim
  GridMask BMaskC,MaskL,MaskR;
 };


*/

#endif