
#include "grid_manip.h"
#include "grid_mask.h"


DescribedClass* create_AreaStoreInterface_cd(){return new AreaStoreInterface;}


static ClassDesc AreaStoreInterface_cd(typeid(AreaStoreInterface),"AreaStoreInterface",1,"Area",&create_AreaStoreInterface_cd);



void AreaStoreInterface::CreateArea(TriangulatorInterface *newTriang)
 {
  GetTriang(newTriang);
  GetGridField(newTriang->CreateGridField());
  GetGridManip(newTriang->CreateGridManip());
  GetMaskGen(newTriang->CreateMaskGen());
  GetMaskStr(newTriang->CreateMaskStr());
  GetBound(newTriang->CreateBound());

  GetGridManip()->Init();
  GetBound()->Init();
  GetMaskGen()->Init();
  GetMaskStr()->Init();
//GetMaskGen();
 };

MaskManipulatorGeneral* AreaStoreInterface::GetMaskGen(MaskManipulatorGeneral *newMask) 
 {
  if (newMask!=NULL) VarList.Add(newMask,MaskGen);
  return dynamic_cast<MaskManipulatorGeneral*>
                      (VarList.GetPtr(MaskGen));
 };
MaskManipulatorStruct* AreaStoreInterface::GetMaskStr(MaskManipulatorStruct *newMask) 
 {
  if (newMask!=NULL) VarList.Add(newMask,MaskStr);
  return dynamic_cast<MaskManipulatorStruct*>
                      (VarList.GetPtr(MaskStr));
 };
