
#ifndef Clc_Gasd_Main_h
#define Clc_Gasd_Main_h 1



#include "std_alg.h"
//#include "grid_bound.h"
#include "lib/data/dataarr.h"
#include "Gasd/Topology/grid_array.h"
#include "Gasd/Topology/grid_center.h"

#define HEAD(variable) "\n========================================\n============"<<variable<<"=============\n===========================================\n"

#define ENDL "\n";fcout.flush()

struct Config_Gasd_ReadPar;

struct GasdCalculator:SavableClass
 {
  int AccuracySpace,NumPnt,WorkDim;
  Config_Gasd_ReadPar *reader;
  Ref<AreaStoreInterface> area;

  GridMask MaskAll,MaskMain;
  GridManipulator *grid_manip;
  FieldInterface* grid_field;
  MaskManipulatorStruct *mask_manip;
  double Length,Shift;

  GasdCalculator():Length(10),Shift(0){};

  virtual FieldInterface *CreateGrid();
  virtual void MakeTimeStp(double TimeWas,double TimeNew,double &MaxTimeStp,
                           double &BoundWaveAmp)
   { BoundWaveAmp=0;MaxTimeStp=1e6; }

  virtual void StartCalculation(){};
  virtual int save_data_state (FilterOut &so)
   {so<<"WorkDim"<<WorkDim<<"AccuracySpace"<<AccuracySpace<<"NumPnt"<<NumPnt;
    return !so.fail();}

  virtual int read_data_state (FilterIn &si)
   { si>>StdKey>>WorkDim>>StdKey>>AccuracySpace>>StdKey>>NumPnt;return !si.fail();}

  virtual void print(ostream& = ExEnv::out())  const
   {fcout<<"GasdCalculator\n Creates grid, with given number of points, makes main calculations\n";
    fcout<<" Its format : "<<(SavableClass*)this;
   };

 };


struct Config_Gasd_ReadPar:Config_Gasd
 {
//  Ref<FieldInterface> grid;
  FieldInterface *stor;
  ResizeVector<KeyDesc> data_names;
  Ref<GasdCalculator> calc;
  char MaskStoredPoints[20];
  double ReadTime;
  int ReadIsGood,SpaceDim,ReadSpaceAsDouble,NotToReadPar;

  Config_Gasd_ReadPar():ReadIsGood(0),ReadTime(0){strcpy(MaskStoredPoints,"StoredPoints");}
  virtual int ReadData(FilterIn &in,FilterOut &out);

  virtual void WriteRes(double TimeNew,FilterOut &out);


  virtual void MakeTimeStp(double TimeWas,double TimeNew,double &MaxTimeStp,
                           double &BoundWaveAmp)
   { calc->MakeTimeStp(TimeWas,TimeNew,MaxTimeStp,BoundWaveAmp);  }


  
  virtual int save_data_state (FilterOut &so);

  virtual int read_data_state (FilterIn &si);

  virtual void print(ostream& = ExEnv::out())  const
   {fcout<<"Config_Gasd_ReadPar\n Reads parameters, in grid. To make main calculations calls GasdCalculator\n";
    fcout<<" Its format : "<<//(SavableClass*)this;
" NOT_NULL Config_Gasd_ReadPar { NumStoredVars 1 There_Names Press \nSpaceDim 1 ReadSpaceAsDouble 1 NotToReadPar 0\nGasdCalculator GasdCalculator_Object  }\n";
    fcout<<" Currently present GasdCalculator_Category classes\n";
    fcout<<SavableClass::HelpForCategory("GasdCalculator_Category",1);
   };
 };

  
#include "Gasd\Alg_Clc\ENO_Alg\eno_main.h"
#include "Gasd/Topology/grid_util.h"
#include "Gasd/Topology/grid_array.h"

  
struct GasdCalculator_Simple1D:GasdCalculator
 {
  char *Uname,Fname[256],Xname[256];
  MaskBoundaryCond CircleMask;

  Ref<DivOper> Dif;
  FluxOper Flux;
  ReconstrFlux Fmin;
  OperatorMask Dif_mask;
  Ref<MaskManipulatorStruct> shift;
  int Dim;


  virtual void MakeTimeStp(double TimeWas,double TimeNew,double &MaxTimeStp,
                           double &BoundWaveAmp)
   { 
    BoundWaveAmp=0;MaxTimeStp=1e6;
     
    GridField<double> *X=(GridField<double> *)grid_manip->GetVar("X_sh");
    GridField<double> *U=(GridField<double> *)grid_manip->GetVar(Uname);
    GridVectorField<double> *Coord=(GridVectorField<double> *)grid_manip->GetVar("Coord");
    GridVectorField<double> *F=(GridVectorField<double> *)grid_manip->GetVar(Fname);
    double Tstp=TimeNew-TimeWas;
    if (Tstp<1e-200) return;
//    int k;
    (*U)(CircleMask.MaskPut)=(*U)(CircleMask.MaskGet);

//fcout<<HEAD("BEGIN");grid_manip->PrintAllGridVars(fcout);fcout<<ENDL;
//fcout.SetRefMask(SavableClass::StorePtr);
//fcout<<HEAD("BEGIN")<<*grid_manip->GetGridField()<<ENDL;
    Flux.Init(NULL,U);
//fcout<<HEAD("After Init");grid_manip->PrintAllGridVars(fcout);fcout<<ENDL;
//fcout<<HEAD("After Init")<<*grid_manip->GetGridField()<<ENDL;
    Flux.Operate(MaskAll,*U,*F);
//fcout<<HEAD("After Operate")<<*grid_manip->GetGridField()<<ENDL;
//fcout<<HEAD("After Operate");grid_manip->PrintAllGridVars(fcout);fcout<<ENDL;
    (*F->GetX(0))(CircleMask.MaskPut)=(*F->GetX(0))(CircleMask.MaskGet);
    (*F->GetX(1))(CircleMask.MaskPut)=(*F->GetX(1))(CircleMask.MaskGet);

    (*F->GetX(2))(MaskAll)=(*F->GetX(0))(MaskAll)*(*F->GetX(0))(MaskAll)*0.5;
    (*F->GetX(3))(MaskAll)=(*F->GetX(1))(MaskAll)*(*F->GetX(0))(MaskAll)*0.5;
//fcout<<HEAD("Before Fmin")<<*grid_manip->GetGridField()<<ENDL;
//fcout<<HEAD("Before Fmin");grid_manip->PrintAllGridVars(fcout);fcout<<ENDL;
    Fmin.Operate(MaskAll,*F,*X);
//fcout<<HEAD("After Fmin")<<*grid_manip->GetGridField()<<ENDL;
//fcout<<HEAD("After Fmin");grid_manip->PrintAllGridVars(fcout);fcout<<ENDL;
    GridMask m=*Dif_mask.GetShift(0,0),mp=*Dif_mask.GetShift(0,1),
                                       mm=*Dif_mask.GetShift(0,-1);
    double Coef=0.1;
    
    (*U)(m)=(*U)(m)-((*X)(mp)-(*X)(m))*Coef;
//fcout<<HEAD("END ITER")<<*grid_manip->GetGridField()<<ENDL;
//fcout<<HEAD("END ITER");grid_manip->PrintAllGridVars(fcout);fcout<<ENDL;
   }

  virtual void StartCalculation()
   {
    int k;
    GridField<double> *X=(GridField<double> *)grid_manip->GetVar("X_sh");
    *X=*(shift->GetCenter()->GetX(0));
    GridField<double> *U=(GridField<double> *)grid_manip->GetVar(Uname);
    GridVectorField<double> *Coord=(GridVectorField<double> *)grid_manip->GetVar("Coord");
    double NumPeriods=1,Phase=0.5,Amp=1,ShiftAmp=2;

    for (k=0;k<MaskAll.SetDim();k++) U->GetPnt(MaskAll[k])=
     sin((X->GetPnt(MaskAll[k])/Length*NumPeriods+Phase)*2*M_PI)*Amp+ShiftAmp;

    Dif_mask.Init(grid_manip,AccuracySpace+1);
    Dif=new DivOper;
    Flux.Set(&Dif_mask,0,AccuracySpace,Dif);//Dif->MinGlobal=1;
    Flux.Init(NULL,NULL);
    Fmin.Set(&Dif_mask,0,AccuracySpace);
    Fmin.Alpha=3;
    Fmin.SetOperation(ReconstrFlux::Godunov);
   }
  virtual FieldInterface *CreateGrid()
   {
    FieldInterface *ret=GasdCalculator::CreateGrid();
    shift=new MaskManipulatorStruct(grid_manip,MaskAll);
    Uname=reader->data_names[0].name;
    strcpy(&Fname[0],"Flux");strcpy(&Xname[0],"X_sh");

    SetCircleBoundaryMask(grid_manip,CircleMask,AccuracySpace);
    GridField<double> *tmp;
    GridVectorField<double> *tmp1;

    new GridVectorField<double>(4,grid_manip,Fname);
    tmp=new GridField<double>(grid_manip,Xname);//SetCoord2Data(tmp,MaskAll,0);

    tmp1=new GridVectorField<double>(WorkDim,grid_manip,"Coord");
    *tmp1=*(shift->GetCenter());
    *tmp=*(tmp1->GetX(0));

    return ret;
   };

     
  virtual void print(ostream& = ExEnv::out())  const
   {fcout<<"GasdCalculator_Simple1D\n test calculator\n";
    fcout<<" Its format : "<<(SavableClass*)this;
   };

    

 };

#endif


/*
#include "eno_main.h"
#include "grid_util.h"
#include "grid_array.h"

  
struct GasdCalculator_Simple1D:GasdCalculator
 {
  char *Uname,Fname[256],Xname[256];
//  Ref<GridVarBoundInd> Boundary;
  MaskBoundaryCond CircleMask;
//  MaskBoundaryClc Bound;
//  ShiftMask Shift;

  DivOper *Dif;
  FluxOper Flux;
  ReconstrFlux Fmin;
  OperatorMask Dif_mask;
  MaskManipulatorStruct *shift;
  int Dim;


  virtual void MakeTimeStp(double TimeWas,double TimeNew,double &MaxTimeStp,
                           double &BoundWaveAmp)
   { 
    BoundWaveAmp=0;MaxTimeStp=1e6;
     
//    GridField<double> *B=(GridField<double> *)Bound.bgrid->GetVar("Flux");
//    GridVarBoundInd *bi=(GridVarBoundInd *)bgrid->GetVar("Boundary");
    GridField<double> *X=(GridField<double> *)grid->GetVar("X_sh");
    GridField<double> *U=(GridField<double> *)grid->GetVar(Uname);
    GridVectorField<double> *Coord=(GridVectorField<double> *)grid->GetVar("Coord");
    GridVectorField<double> *F=(GridVectorField<double> *)grid->GetVar(Fname);
//    GridField<double> *S=(GridField<double> *)grid->GetVar("Speed");
    double Tstp=TimeNew-TimeWas;
    if (Tstp<1e-200) return;
    int k;
    (*U)(CircleMask.MaskPut)=(*U)(CircleMask.MaskGet);

//    *U=*Coord->GetX(0);
//    Dif.Init(NULL,U);(*X)(MaskAll)=(*Coord->GetX(0))(MaskAll)+0.1;
//    Dif.Init(NULL,U);(*X)=0.5;
//    Dif.Operate(MaskAll,*X,*U);
    Flux.Init(NULL,U);Flux.Operate(MaskAll,*U,*F);
    (*F->GetX(0))(CircleMask.MaskPut)=(*F->GetX(0))(CircleMask.MaskGet);
    (*F->GetX(1))(CircleMask.MaskPut)=(*F->GetX(1))(CircleMask.MaskGet);

    (*F->GetX(2))(MaskAll)=(*F->GetX(0))(MaskAll)*(*F->GetX(0))(MaskAll)*0.5;
    (*F->GetX(3))(MaskAll)=(*F->GetX(1))(MaskAll)*(*F->GetX(1))(MaskAll)*0.5;
    Fmin.Operate(MaskAll,*F,*X);
    GridMask m=*Dif_mask.GetShift(0,0),mp=*Dif_mask.GetShift(0,1),
                                       mm=*Dif_mask.GetShift(0,-1);
    double Coef=0.1;
    
    (*U)(m)=(*U)(m)-((*X)(mp)-(*X)(m))*Coef;
//    (*U)(m)=(*U)(m)+((*X)(mm)-(*X)(m))*Coef;
//    (*U)(MaskAll)=(*X)(MaskAll);
//    (*X)=0.5;Flux.Div->Operate(MaskAll,*X,*U);
//    (*F->GetX(1))(MaskAll)=(*U)(MaskAll)-(*F->GetX(1))(MaskAll);
//fcout<<F->GetX(1)->data<<"\n";fcout.flush();//abort();
//    *U=*F->GetX(1);
   }

  virtual void StartCalculation()
   {
    int k;
    GridField<double> *X=(GridField<double> *)grid->GetVar("X_sh");
//    SetData2Coord(X,MaskAll,0);
    *X=*(shift->GetCenter()->GetX(0));
    GridField<double> *U=(GridField<double> *)grid->GetVar(Uname);
    GridVectorField<double> *Coord=(GridVectorField<double> *)grid->GetVar("Coord");
    double NumPeriods=1,Phase=0.5,Amp=1,ShiftAmp=2;

    for (k=0;k<MaskAll.SetDim();k++) U->GetPnt(MaskAll[k])=
     sin((X->GetPnt(MaskAll[k])/Length*NumPeriods+Phase)*2*M_PI)*Amp+ShiftAmp;

//    Shift.Set(AccuracySpace,grid->GetMask(),MaskAll);
//AccuracySpace--;
    Dif_mask.Init(grid,AccuracySpace+1);
//    Dif.Set(&Dif_mask,0,AccuracySpace); 
//    Dif.Init(Coord,NULL);
    Dif=new DivOper;
    Flux.Set(&Dif_mask,0,AccuracySpace,Dif);//Dif->MinGlobal=1;
    Flux.Init(NULL,NULL);
    Fmin.Set(&Dif_mask,0,AccuracySpace);
    Fmin.Alpha=3;
//    Fmin.SetOperation(ReconstrFlux::Godunov);
//    Dif.Set(&Dif_mask,0,AccuracySpace); 
//    Dif.Init(Coord,NULL);
//fcout<<Transpon(Flux.BoundRCoef)<<"\n"<<Transpon(Flux.BoundLCoef)<<"\n";fcout.flush();abort();
//AccuracySpace++;
   }
  virtual FieldInterface *CreateGrid()
   {
    FieldInterface *ret=GasdCalculator::CreateGrid();
    grid->mask_manip->shift=new MaskManipulatorStruct(grid,MaskAll);
    shift=grid->mask_manip->shift;
//fcout<<(shift->GetCenter()->GetX(0)->data);
    Uname=reader->data_names[0].name;
    strcpy(&Fname[0],"Flux");strcpy(&Xname[0],"X_sh");

//    Bound.SetNewGrid(grid,MaskMain);
    SetCircleBoundaryMask(grid,CircleMask,AccuracySpace);
    GridField<double> *tmp;
    GridVectorField<double> *tmp1;

//    tmp=new GridField<double>(Bound.bgrid,"Flux");
    new GridVectorField<double>(4,grid,Fname);
    tmp=new GridField<double>(grid,Xname);//SetCoord2Data(tmp,MaskAll,0);

    tmp1=new GridVectorField<double>(WorkDim,grid,"Coord");
    *tmp1=*(shift->GetCenter());
//fcout<<(tmp1->GetX(0)->data);
    *tmp=*(tmp1->GetX(0));
    

    return ret;
   };

     
  virtual void print(ostream& = ExEnv::out())  const
   {fcout<<"GasdCalculator_Simple1D\n test calculator\n";
    fcout<<" Its format : "<<(SavableClass*)this;
   };

    

 };

*/









 /*
 struct GasdCalculator_Simple1D:GasdCalculator
 {
  char *Uname,Fname[256],Xname[256];
//  Ref<GridVarBoundInd> Boundary;

  GridManipulator *bgrid;
  GridBound *bgrid_add;
  GridMask BMaskAll,BMaskA,MaskL,MaskR,BMaskL,BMaskR;
  GridMask MaskGetC,MaskPutC;


  virtual void MakeTimeStp(double TimeWas,double TimeNew,double &MaxTimeStp,
                           double &BoundWaveAmp)
   { 
    GridField<double> *X=(GridField<double> *)grid->GetVar("X_sh");
    GridField<double> *U=(GridField<double> *)grid->GetVar(Uname);
    GridField<double> *S=(GridField<double> *)grid->GetVar("Speed");
    GridField<double> *B=(GridField<double> *)bgrid->GetVar("Flux");
//    GridVarBoundInd *bi=(GridVarBoundInd *)bgrid->GetVar("Boundary");
    double Tstp=TimeNew-TimeWas;
    
    int k;
    for (k=0;k<MaskAll.SetDim();k++) S->GetPnt(MaskAll[k])=U->GetPnt(MaskAll[k]);
//    for (k=0;k<MaskAll.SetDim();k++) X->GetPnt(MaskAll[k])+=S->GetPnt(MaskAll[k])*Tstp;
//    for (k=0;k<MaskAll.SetDim();k++) if (X->GetPnt(MaskAll[k])>Length) X->GetPnt(MaskAll[k])-=Length;
    *B=0.;
    for (k=0;k<MaskGetC.SetDim();k++)
     {
      U->GetPnt(MaskPutC[k])=U->GetPnt(MaskGetC[k]);
     }
    for (k=0;k<BMaskA.SetDim();k++) 
     {
      B->GetPnt(BMaskA[k])=(U->GetPnt(MaskR[k])-U->GetPnt(MaskL[k]));
     }
//fcout<<"B\n"<<B->GetMaskedPnt(BMaskA)<<"\n";
    for (k=0;k<MaskMain.SetDim();k++)
     {
      U->GetPnt(MaskMain[k])+=(B->GetPnt(BMaskR[k])-B->GetPnt(BMaskL[k]))*Tstp;
     }

//    DataVector<SpacePoint,CopyStructSlow<SpacePoint> > coord;
//    coord=grid->GetGridMaskedPnt(MaskAll);
//    for (k=0;k<MaskAll.SetDim();k++) coord[k][0]=X->GetPnt(MaskAll[k]);
//    grid->SetGridMaskedPnt(coord,MaskAll);

    BoundWaveAmp=0;MaxTimeStp=1e6; 
   }

  virtual void StartCalculation()
   {
    int k;
    DataVector<SpacePoint,CopyStructSlow<SpacePoint> > coord;
    coord=grid->GetGridMaskedPnt(MaskAll);
    GridField<double> *X=(GridField<double> *)grid->GetVar("X_sh");
    for (k=0;k<MaskAll.SetDim();k++) coord[k][0]=X->GetPnt(MaskAll[k]);
    grid->SetGridMaskedPnt(coord,MaskAll);

    GridField<double> *U=(GridField<double> *)grid->GetVar(Uname);

    double NumPeriods=1,Phase=0.25,Amp=1,ShiftAmp=2;

    for (k=0;k<MaskAll.SetDim();k++) U->GetPnt(MaskAll[k])=
     sin((X->GetPnt(MaskAll[k])/Length*NumPeriods+Phase)*2*M_PI)*Amp+ShiftAmp;

    GridVarBoundInd *Boundary=new GridVarBoundInd(grid,"Boundary");
    bgrid_add=Boundary->gridbound_ptr;bgrid=bgrid_add->SetGridManip();
    GridField<double> *tmp=new GridField<double>(bgrid,"Flux");
    MaskManipulatorGeneral *mask_manip=bgrid->GetMask();
    BMaskL=Boundary->MaskBoundShift(MaskMain,0);
    BMaskR=Boundary->MaskBoundShift(MaskMain,1);
    BMaskA=mask_manip->AddMask(BMaskL,BMaskR);
    MaskL=bgrid_add->MaskCenterShift(BMaskA,0,0);
    MaskR=bgrid_add->MaskCenterShift(BMaskA,0,1);

    mask_manip=grid->GetMask();
    GridMask MaskEnd0=mask_manip->MaskBoundary();
    int N0=MaskEnd0.SetDim(),N1=AccuracySpace,k1;
    DataVector<GridMask,CopyStructSlow<GridMask> > SL(2*N1),SR(2*N1);
// Circle Boundary Masks
//    GridMask MaskGetC(N0*N1),MaskPutC(N0*N1);
    MaskGetC.SetDim(N0*N1);MaskPutC.SetDim(N0*N1);
    for (k=0;k<2*N1;k++) 
     {
      SL[k]=mask_manip->ShiftMask(MaskEnd0,1,k);
      SR[k]=mask_manip->ShiftMask(MaskEnd0,0,k);
     }
//fcout<<"\nSL "<<SL<<" \nSR "<<SR<<"\n";
    int ng=0,np=0;
    for (k=0;k<N0;k++) 
     {
      if (SL[1][k]<0) 
       {for (k1=0;k1<N1;k1++) {MaskPutC[ng]=SR[k1][k];ng++;}}
      else
       {for (k1=0;k1<N1;k1++) {MaskPutC[ng]=SL[k1][k];ng++;}}
     }
    ng=0;
    for (k=N0-1;k>=0;k--) 
     {
      if (SL[1][k]<0) 
       {for (k1=2*N1-1;k1>=N1;k1--) {MaskGetC[ng]=SR[k1][k];ng++;}}
      else
       {for (k1=2*N1-1;k1>=N1;k1--) {MaskGetC[ng]=SL[k1][k];ng++;}}
     }
fcout<<"\nMaskGetC "<<MaskGetC<<"\nMaskPutC "<<MaskPutC<<"\n";
    


fcout<<"\nbound all: "<<BMaskA.SetDim()<<"\nbound r: "<<BMaskR.SetDim()<<"\n";
fcout<<"\nbound l: "<<BMaskL.SetDim()<<"\ncenter r: "<<MaskR.SetDim()<<"\n";
fcout<<"\ncenter l: "<<MaskL.SetDim()<<"\n";
fcout<<" MaskAll "<<MaskAll.SetDim()<<"\n";
    for (k=0;k<MaskAll.SetDim();k++) 
     { fcout<<" K "<<k<<" MaskAll "<<MaskAll[k]<<"\n"; };
fcout<<" MaskMain "<<MaskMain.SetDim()<<"\n";
    for (k=0;k<MaskMain.SetDim();k++) 
     { fcout<<" K "<<k<<" MaskMain "<<MaskMain[k]<<"\n"; };
fcout<<" BMaskA "<<BMaskA.SetDim()<<" MaskR "<<MaskR.SetDim()<<" MaskL \n";
    for (k=0;k<BMaskA.SetDim();k++) 
     { fcout<<" K "<<k<<" BMaskA "<<BMaskA[k]<<" MaskR "<<MaskR[k]<<" MaskL "<<MaskL[k]<<"\n"; };
fcout<<" BMaskR  BMaskL "<<BMaskR.SetDim()<<"\n";
    for (k=0;k<BMaskR.SetDim();k++) 
     { fcout<<" K "<<k<<" CR "<<BMaskR[k]<<" CL "<<BMaskL[k]<<"\n"; };

   }
  virtual FieldInterface *CreateGrid()
   {
    FieldInterface *ret=GasdCalculator::CreateGrid();
    Uname=reader->data_names[0].name;
    strcpy(&Fname[0],"Speed");strcpy(&Xname[0],"X_sh");
    GridField<double> *tmp=new GridField<double>(grid,Fname);
    tmp=new GridField<double>(grid,Xname);
    DataVector<SpacePoint,CopyStructSlow<SpacePoint> > coord;
    coord=grid->GetGridMaskedPnt(MaskAll);
    for (int k=0;k<MaskAll.SetDim();k++) tmp->GetPnt(MaskAll[k])=coord[k][0];
    return ret;
   };

*/