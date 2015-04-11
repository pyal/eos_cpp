
#include "DataVec.h"
//#include "IndBase.h"
#include "DataArr.h"
//#include "IndStruc.h"
//#include "FreeReg.h"
//#include "exenv.h"
#include "grid_center.h"
//#include "grid_bound.h"
#include "ex_out.h"

#define HEAD(variable) "\n========================================\n============"<<variable<<"=============\n===========================================\n"

#define ENDL "\n";fcout.flush()



#define WorkDim 1
#define NumPnt 10
#define Length 2
#define AccuracySpace 2
#include "grid_array.h"
  
//AreaStoreInterface 
struct TObj : public NamedObjectInited 
 {
  TObj() {fcout<<"TOBbj Created\n";fcout.flush();}
  ~TObj(){fcout<<"TOBbj Deleted\n";fcout.flush();}
 };
struct TStor : public SavableClass, public NamedObjectStoreAbstract  
 { 
  NamedStore VarList;
  TStor(){VarList.set_DefaultStorage(this);}
//  ~TStor(){VarList.Add(NULL,0);}

// from NamedObjectStoreAbstract
   inline void DeleteObject(NamedObject *obj)       {VarList.DeleteObject(obj);};
   inline  int Add (NamedObject* dat, int pos = -1) {return VarList.Add(dat,pos);};
 };
struct TObj1 : public SavableClass 
 {
 TObj1():ptr(NULL) {fcout<<"TOBbj1 Created\n";fcout.flush();}
  ~TObj1(){fcout<<"TOBbj1 Deleted\n";fcout.flush();}
  SavableClass *ptr;
  int save_data_state (FilterOut &so) { so<<KeyDesc("TOBbj1_ptr")<<ptr; return 1;};
 };
struct TObj2 : public SavableClass 
 {
 TObj2():ptr(NULL) {fcout<<"TOBbj2 Created\n";fcout.flush();}
  ~TObj2(){fcout<<"TOBbj2 Deleted\n";fcout.flush();}
  SavableClass *ptr;
  int save_data_state (FilterOut &so) { so<<KeyDesc("TOBbj2_ptr")<<ptr; return 1;};
 };

DescribedClass* create_TObj1_cd(){return new TObj1;}
static ClassDesc TObj1_cd(typeid(TObj1),"TObj1",1,"Area",&create_TObj1_cd);
DescribedClass* create_TObj2_cd(){return new TObj2;}
static ClassDesc TObj2_cd(typeid(TObj2),"TObj2",1,"Area",&create_TObj2_cd);

void TestNamed()
 {
  TObj1 obj1;
  TObj2 obj2;
  obj1.ptr=&obj2;obj2.ptr=&obj1;
//fcout.SetRefMask(SavableClass::SingleFileStorage);
fcout.SetRefMask(SavableClass::StorePtr);
  fcout<<obj1;
 }
//AreaStoreInterface 
void 
CreateGrid()
 {
//  Ref<GridCenter> stor;//=new GridCenter;//stor.unmanage();
// AccuracySpace;
  AreaStoreInterface area;
  double add=(double)AccuracySpace*Length/(NumPnt),Shift=0;
  SpacePoint low(WorkDim),up(WorkDim);  low=Shift-add;up=Length+Shift+add;
  SpaceBound bound(low,up);
  IndexPoint lowi(WorkDim),upi(WorkDim);lowi=(int)0;upi=(int)NumPnt+2*AccuracySpace;
  IndBound index(lowi,upi);
  area.CreateArea(new TriangulatorSingleSquare(bound,index));
  GridManipulator *gr_man=area.GetGridManip();


  GridField<double> *f_d_ptr=new GridField<double>(gr_man,"f_d_ptr");
  GridVectorField<double> *doub1_ptr=new GridVectorField<double>(2,gr_man,"doub1_ptr");
fcout<<HEAD("Beg");gr_man->PrintAllGridVars(fcout);fcout<<ENDL;

  MaskManipulatorGeneral *mask_manip=area.GetMaskGen();
  GridMask MaskAll=mask_manip->MaskAll();
  GridMask MaskMain=mask_manip->MaskBoundary(AccuracySpace-1);
  MaskMain=mask_manip->RemoveMask(MaskAll,MaskMain);
  GridMaskVector MaskVector2(2);//MaskVector2[0]=MaskAll;MaskVector2[1]=MaskAll;
  MaskVector2=MaskAll;

  /*
//area.SetRefMask(SavableClass::SingleFileStorage);
fcout.SetRefMask(SavableClass::SingleFileStorage);
//fcout.SetRefMask(SavableClass::StorePtr);
  GridField<double> f_d(gr_man,"f_d",1);
//  fcout<<area<<"\n";
//  return ;
  *f_d_ptr=1.;
  GridVectorField<double> *doub2_ptr=new GridVectorField<double>(2,gr_man,"doub2_ptr");
  *doub1_ptr=2.;
fcout.SetRefMask(SavableClass::StorePtr);
//  fcout<<*area.GetGridField();
//  (*doub1_ptr->GetX(0))(MaskMain)=(*f_d_ptr)(MaskMain);
  (*doub2_ptr)(MaskVector2)=(*doub1_ptr)(MaskVector2);
//  fcout<<*area.GetGridField();
fcout<<HEAD("BeforeSelfSet");gr_man->PrintAllGridVars(fcout);fcout<<ENDL;
//  (*f_d_ptr)(MaskAll)=((*f_d_ptr)(MaskAll)+(*f_d_ptr)(MaskAll))*0.5;
//  (*doub2_ptr)(MaskVector2)=((*doub2_ptr)(MaskVector2)+(*doub2_ptr)(MaskVector2));//*0.5;
  (*doub2_ptr->GetX(1))(MaskAll)=((*doub2_ptr->GetX(0))(MaskAll)+
                                  (*doub2_ptr->GetX(1))(MaskAll))/0.5;
fcout<<HEAD("AfterSelfSet");gr_man->PrintAllGridVars(fcout);fcout<<ENDL;
  //return ;
  Ref<AreaStoreInterface> area1_ptr=new AreaStoreInterface; //&area;//
//fcout.SetRefMask(SavableClass::SingleFileStorage);
  area1_ptr->SavableClass::operator=(area);
//  fcout<<*(area1_ptr->GetGridManip()->GetVar("doub1"));
fcout.SetRefMask(SavableClass::StorePtr);
  fcout<<*(area1_ptr->GetGridManip()->GetVar("f_d_ptr"));//<<(void*)(area1_ptr->GetGridManip()->GetVar("f_d_ptr"));
  fcout<<*(((GridVectorField<double>*)area1_ptr->GetGridManip()->GetVar("doub2_ptr"))->GetX(1));//<<(void*)(gr_man->GetVar("f_d_ptr"));

*/
fcout<<HEAD("End");gr_man->PrintAllGridVars(fcout);fcout<<ENDL;
fcout.ClearRef();
fcout.flush();
//  GridVectorField<double> doub1(2,gr_man,"doub1",1);

//  return area;
 };


void main()
{
SetLeakTest();
//fcout<<" Test";fcout.flush();
cout<<Coreleft()<<"\n";cout.flush();
Time_struct time;
//  TestNamed();
  CreateGrid();
fcout<<time;
//  TestResVect();
//  TestRefCl();
fcout<<Coreleft()<<"\n";fcout.flush();
//cout<<test<<"\n";cout.flush(); test=NULL;
//fcout<<Coreleft()<<"\n";fcout.flush();
//fcout<<Coreleft()<<"\n";fcout.flush();
LeakTest();
};

