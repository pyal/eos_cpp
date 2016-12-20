
#include "lib\data\DataVec.h"
//#include "IndBase.h"
#include "lib\data\DataArr.h"
//#include "IndStruc.h"
//#include "FreeReg.h"
//#include "exenv.h"
#include "Gasd\Topology\grid_center.h"
//#include "grid_bound.h"
#include "lib/std/ex_out.h"

#define HEAD(variable) "\n========================================\n============"<<variable<<"=============\n===========================================\n"

#define ENDL "\n";fcout.flush()



struct TestRef;
struct TestStruct:SavableClass
 {
//  DataVector<double> a;
  Ref<TestRef> a;
//  TestStruct():a(0){};
//  TestStruct(int len,double tmp){a.SetDim(len);a=tmp;};
  TestStruct(TestRef *tmp){a=tmp;};
  virtual int save_data_state (FilterOut &so)
   {so<<KeyDesc("A")<<a;return 1;};
  virtual int read_data_state (FilterIn &si)
   {si>>StdKey>>a;return 1;};
 };

struct TestRef:SavableClass
 {
//     ResizeVector< Ref<FieldInterface> > VarList;
  ResizeVector< Ref<TestStruct> > VarList;
  virtual int save_data_state (FilterOut &so)
   {so<<KeyDesc("VarList")<<VarList;return 1;};
  virtual int read_data_state (FilterIn &si)
    { si>>StdKey>>VarList;return 1;};

  int AddVar(TestStruct *NewVar);
  void DelVar(TestStruct *NewVar);
//  ~TestRef();
 };


int TestRef::AddVar(TestStruct *NewVar)
 {
  int k,N=VarList.SetNumEl();
   for (k=0;k<N;k++) if (VarList[k]==NULL) {VarList[k]=NewVar;return k;};
   VarList[N]=NewVar;
   return N;
 }
void TestRef::DelVar(TestStruct *NewVar)
 {
  int k,N=VarList.SetNumEl();
  for (k=0;k<N;k++) 
   if (VarList[k]==NewVar) 
    {
     VarList[k]=NULL;break ;
    }
 }
//TestRef::~TestRef(){ int k,N=VarList.SetNumEl(); for (k=0;k<N;k++) VarList[k]=NULL;}

void TestRefCl()
 {
  TestRef str;str.unmanage();
//  str.AddVar(new TestStruct(10,5));
  str.AddVar(new TestStruct(&str));
  fcout<<str;
 };

struct TestResStruct:  SavableClass
 {
  TestResStruct(){SetRefMask(SingleFileStorage);}
  ResizeVector<GridBoundPnt,CopyStructSlow<GridBoundPnt> > data;
  virtual int save_data_state (FilterOut &so)  {so<<KeyDesc("data")<<data;return 1;};
  virtual int read_data_state (FilterIn &si)   { si>>StdKey>>data;return 1;};
 };
static ClassDesc TestResStruct_cd(typeid(TestResStruct),"TestResStruct",1,"Grid");//,&create_GridField_double_cd);


#define NumToTest 100  
void TestResVect()
 {
  GridBoundPnt bnd;
  Ref<TestResStruct> vec_ptr=new TestResStruct;
  for (int k=0;k<NumToTest;k++) {  bnd.First=k;vec_ptr->data[k]=bnd;}
  Ref<TestResStruct> vec1_ptr=new TestResStruct;
  fcout.SetRefMask(SavableClass::SingleFileStorage);
  
  fcout<<HEAD("Created Vec")<<*vec_ptr;
  vec1_ptr->SavableClass::operator=(*vec_ptr);

  fcout<<HEAD("Copyed Vec")<<*vec1_ptr;
fcout.ClearRef();
 }

    
#include "Gasd\Topology\grid_array.h"
  


void CreateGrid(GridCenter &stor,SpaceBound &bound,IndBound &index)
 {
   GridCenterPnt Cent;
   int k,k1,N=index.NumPnt(),N0=index.Low.SetDim();
   IndexPoint dim=index.Size(),ind,tmp_ind;
   SpacePoint add=bound.Low,mul=bound.Size(),pnt(N0);
   for (k=0;k<N0;k++) mul[k]/=dim[k];
   DataVector<GridIndex> neib(2*N0);
   double vol=mul[0];
   for (k=1;k<N0;k++) vol*=mul[k];
   for (k=0;k<N;k++) 
    {
     ind=index.Int2Pnt(k);
//fcout<<"\nind"<<ind;
     for (k1=0;k1<N0;k1++) pnt[k1]=add[k1]+(ind[k1]-index.Low[k1])*mul[k1];
     for (k1=0;k1<N0;k1++) 
      {tmp_ind=ind;tmp_ind[k1]=ind[k1]-1;neib[2*k1]=index.Pnt2Int(tmp_ind);
       tmp_ind[k1]=ind[k1]+1;neib[2*k1+1]=index.Pnt2Int(tmp_ind);}

//fcout<<"\nPnt"<<pnt<<"k"<<k<<"neib"<<neib<<"\n";
     stor.AddPnt(k,Cent.Set(pnt,k,vol,neib));
//fcout<<stor.data[k]<<"\n";
    }
 };

//Ref<GridField<double> > test;
//typedef   
#define WorkDim 1
void TestGridCenter()
 {
  Ref<GridCenter> stor;//=new GridCenter;//stor.unmanage();
//  SpacePoint low(3),up(3);  low=1.;up=2.;
//  IndexPoint lowi(3),upi(3);lowi=(int)1;upi=(int)11;
  SpacePoint low(WorkDim),up(WorkDim);  low=1.;up=2.;
  IndexPoint lowi(WorkDim),upi(WorkDim);lowi=(int)1;upi=(int)10;
  SpaceBound bound(low,up);
  IndBound index(lowi,upi);
//fcout<<"Bound\n"<<bound<<"\nindex\n"<<index;
//SetLeakTest();
  TriangulatorSingleSquare* triang=new TriangulatorSingleSquare(bound,index);
  stor=(GridCenter*)triang->GetGrid();
//  CreateGrid(*stor,bound,index);
//LeakTest();
  Ref<GridCenter> stor1=new GridCenter;

  Ref<GridField<double> > 
   test=new GridField<double>(stor->SetGridManip(),"test");
//   test=new GridField<double>;
  Ref<GridVarBoundInd> bnd=new GridVarBoundInd(stor->SetGridManip(),"bound_ind");
  GridVectorField<double> *doub=new GridVectorField<double>(2,stor->SetGridManip(),"doub");
  *doub=1.;
  GridVectorField<double> doub1(2,stor->SetGridManip(),"doub1",1);
  doub1=2.;
//  GridBound *stor_bound=bnd->gridbound_ptr;
//  Ref<GridField<double> > 
//   test_bound=new GridField<double>(stor_bound->SetGridManip());
  
//    GridField<double> test(&stor);
//    GridField<double> test;
//test=NULL;
//  stor.AddVar(NULL);
//fcout.SetRefMask(SavableClass::SingleFileStorage);
//  fcout<<HEAD("Created storage")<<*stor;//wait(10);
//  fcout<<HEAD("Created boundary")<<*stor_bound;fcout.flush();
  stor1->SavableClass::operator=(*stor);
  stor=NULL;
//  fcout<<HEAD("Copyed storage")<<*stor1;
//  FieldInterface *dat=stor1->GetVar("bound_ind");
//  fcout<<HEAD("Copyed bound")<<*(((GridVarBoundInd*)(stor1->GetVar("bound_ind")))->gridbound_ptr);
//  fcout<<HEAD("Copyed test ")<<*(stor1->GetVar("test"));
  fcout<<HEAD("Copyed doub ")<<*(stor1->GetVar("doub"))<<ENDL;
  fcout<<HEAD("Copyed doub1 ")<<*(stor1->GetVar("doub1"))<<ENDL;
//  bnd->
//GridField<double> check(stor1->SetGridManip());check.unmanage();
//  fcout.SetRefMask(SavableClass::SingleFileStorage);
//  fcout<<*stor;
//  fcout<<" Test Ref "<<(test);cout<<"Test addr "<<test<<"\n";cout.flush();
//  delete test;

//  fcout<<stor1;
//  test=NULL;
//  stor1=NULL;
fcout.ClearRef();
fcout.flush();
 }
  
void main()
{
SetLeakTest();
//fcout<<" Test";fcout.flush();
cout<<Coreleft()<<"\n";cout.flush();
Time_struct time;
  TestGridCenter();
fcout<<time;
//  TestResVect();
//  TestRefCl();
fcout<<Coreleft()<<"\n";fcout.flush();
//cout<<test<<"\n";cout.flush(); test=NULL;
//fcout<<Coreleft()<<"\n";fcout.flush();
//fcout<<Coreleft()<<"\n";fcout.flush();
LeakTest();
};

