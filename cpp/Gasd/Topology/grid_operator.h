#ifndef Grid_Operator_h
#define Grid_Operator_h 1


#include "Grid_Interface.h"

struct OperatorMask
 {
  DataVector<GridMaskVector> Shift;//[dim][order][k]
  GridMask WorkMask;
  DataVector<int> Cvt;
  MaskManipulatorStruct *Manip;
  GridManipulator *Grid;
  int MaxShift;
  int Dim;

  void Init(GridManipulator *grid,int level,GridMask *mask=NULL)
   {
    MaxShift=level;Grid=grid;if (grid==NULL) return;
    AreaStoreInterface *ar=dynamic_cast<AreaStoreInterface*>(Grid->get_StoredBy());
    if (!ar) {fcout<<" OperatorMask::Init Grid is not in area = BAD.\n";fcout.flush();abort();}
    Manip=ar->GetMaskStr();
    Dim=Manip->Dim;
    if (mask==NULL) WorkMask=Manip->MaskMain;
    else WorkMask=*mask;
    Cvt=Manip->SetCvt(WorkMask);
    CreateShift();
   }
  DataVector<int> CvtFrom(GridMask &from) //Work[to[k]]=from[k]
   {
    int N=from.SetDim();
    DataVector<int> ret(N);
    for (int k=0;k<N;k++) ret[k]=Cvt[from[k]];
    return ret;
   }
  void CreateShift()
   {
    Shift.SetDim(Dim);
    for (int d=0;d<Dim;d++)
     {
      Shift[d].SetDim(2*MaxShift+1);
      Shift[d][MaxShift]=WorkMask;
      for (int or=1;or<=MaxShift;or++)
       {
        Shift[d][MaxShift+or]=Shift[d][MaxShift+or-1]+GridMaskShift(d,1,Manip);
        Shift[d][MaxShift-or]=Shift[d][MaxShift-or+1]+GridMaskShift(d,-1,Manip);
       }
     }
   }
  inline GridMask *GetShift(const int &axis,const int &lev) {return &(Shift[axis][lev+MaxShift]);}

 };

struct OperatorOneD:SavableClass
 {
  OperatorMask *Shift;
//  GridManipulator *Grid;
  int Axis,Level;
  virtual void Set(OperatorMask *shift,int ax,int lev)  {Shift=shift;Axis=ax;Level=lev;}
  virtual void Init(GridField<double> *vec0,GridField<double> *vec1=NULL){};
  virtual void Init(GridVectorField<double> *arr,GridField<double> *vec=NULL){};
  virtual void SetAction(OperatorOneD &oper){};
//  virtual void SetClcMask(GridMask &ClcMask){};

  virtual void Operate(GridMask &ClcMask,GridField<double> &dat,GridField<double> &res){};
  virtual void Operate(GridMask &ClcMask,GridField<double> &dat,GridVectorField<double> &res){};
  virtual void Operate(GridMask &ClcMask,GridVectorField<double> &dat,GridField<double> &res){};
  virtual void Operate(GridMask &ClcMask,GridVectorField<double> &dat,GridVectorField<double> &res){};
 };

struct OperatorSpace:OperatorOneD
 {
  enum {ResSeparate,ResSum} ResType;
  enum {SourceSeparate,SourceSame} SourceType;
  
  DataVector<Ref<OperatorOneD > > Oper;
  void Set(OperatorOneD *oper,OperatorMask *shift,int level)
   {
    Shift=shift;Level=level;
    int k,Dim=Shift->Dim;Oper.SetDim(Dim);
    ClassDesc* cd=ClassDesc::name_to_class_desc(oper->class_name());
    if (cd==NULL) {fcout<<"Something very bad in OperatorSpace. Could not create, ClassDesc ; may have to register \n"<<oper->class_name()<<"\n";fcout.flush();abort();}
    for (k=0;k<Dim;k++) 
     {
      Oper[k]=dynamic_cast<OperatorOneD*>(cd->create());
      if (Oper[k]==NULL) {fcout<<"Something very bad in OperatorSpace. Could not create, may have to register \n"<<oper->class_name()<<"\n";fcout.flush();abort();}
      Oper[k]->Axis=k;Oper[k]->Level=Level;Oper[k]->Shift=Shift;
     }
   }
     
     
  virtual void Init(GridField<double> *vec0,GridField<double> *vec1=NULL)
   { for (int k=0;k<Oper.SetDim();k++) Oper[k]->Init(vec0,vec1); }
  virtual void Init(GridVectorField<double> *arr,GridField<double> *vec=NULL)
   { for (int k=0;k<Oper.SetDim();k++) Oper[k]->Init(arr,vec); }
  virtual void SetAction(OperatorOneD &oper)
   { for (int k=0;k<Oper.SetDim();k++) Oper[k]->SetAction(oper); }
//  virtual void SetClcMask(GridMask &ClcMask);

  virtual void Operate(GridMask &ClcMask,GridField<double> &dat,GridField<double> &res)
   {
    int Dim=Oper.SetDim();if (Dim==0) return;
    Oper[0]->Operate(ClcMask,dat,res);if (Dim==1) return;
    GridField<double> tmp(dat.GetGridManip(),NULL,1);
    for (int k=1;k<Dim;k++) {Operate(ClcMask,dat,tmp);res(ClcMask)+=tmp(ClcMask);}
   };
   virtual void Operate(GridMask &ClcMask,GridField<double> &dat,GridVectorField<double> &res,int resType=ResSum)
   {
    int Dim=Oper.SetDim();if (Dim==0) return;
    if (Dim==1) {Oper[0]->Operate(ClcMask,dat,res);return;}
    int ResD=res.SetDim();
    if (resType==ResSum)
     {
      Oper[0]->Operate(ClcMask,dat,res);
      GridVectorField<double> tmp(ResD,dat.GetGridManip(),NULL,1);
      for (int k=1;k<Dim;k++) {Operate(ClcMask,dat,tmp);for (int k1=0;k1<ResD;k1++) (*res.GetX(k1))(ClcMask)+=(*tmp.GetX(k1))(ClcMask);}
     }
    else
     {
      int AxD=ResD/Dim;if (AxD*Dim!=ResD) {fcout<<" Bad Dim in Operate SpaceDim "<<Dim<<" ArrayDim "<<ResD<<"\n";abort();}
      GridVectorField<double> tmp(AxD,dat.GetGridManip(),NULL,1);
      for (int k=0;k<Dim;k++) {Operate(ClcMask,dat,tmp);for (int k1=0;k1<ResD;k1++) (*res.GetX(k1+k*Dim))(ClcMask)=(*tmp.GetX(k1))(ClcMask);}
     }
   };
//  virtual void Operate(GridMask &ClcMask,GridVectorField<double> &dat,GridField<double> &res);
//  virtual void Operate(GridMask &ClcMask,GridVectorField<double> &dat,GridVectorField<double> &res);
 };


#endif