
//#include "DataVec.h"
//#include "IndBase.h"
//#include "DataArr.h"

//#include "DataTyp.h"
//#include "VecRes.h"

//#include "exenv.h"

//FilterTextOut fcout("Test",DataSource::Console);
//FilterTextIn fcin("Test",DataSource::Console);
/*
void TestVector()
{
  SpacePoint vec1(10),vec2,tmp;
//	DataVector<double> vec1(10),vec2,tmp;
	 vec1=5;
//	(SavableClass&)vec2=(SavableClass&)vec1;
	 vec1[5]=55;
 	vec2=vec1=10;
 	fcout<<vec2<<"\n"<<vec1<<"\n";
 	fcout<<"vec1 "<<vec1<<"\n vec2 "<<vec2<<"\n";
  tmp=vec1+vec2;
//	DataVector<double> tmp=vec1+vec2;
 	fcout<<tmp<<"\n";
 	fcout<<"sum "<<vec1+vec2<<" \n min "<<vec2-vec1<<" \n mul "<<vec1*vec2<<"\n";
  {
  FilterTextOut fo("Test1",DataSource::Memory);
  vec2=100;
  fo<<vec2;
  FilterTextIn fi("Test1",DataSource::Memory);
  fi>>vec1;
  }
  fcout<<"vec1"<<vec1<<"\n vec2"<<vec2<<"\n";
  DataVector<SpacePoint> dat(10);
  dat[0].SetDim(10);dat[0]=5;
  for (int k=1;k<9;k++) dat[k]=dat[0];
  fcout<<dat<<"\n"<<(void*)&vec2<<"\n";
  void *tmpv=NULL;
  FilterTextOut fo("Test1",DataSource::Disk);
  fo<<(void*)&vec1<<10;
//fo<<10;
  fo.CloseBuf();
  FilterTextIn fi("Test1",DataSource::Disk);
int tmp_int;//fi>>tmp_int;fcout<<tmp_int;
  fi>>tmpv>>tmp_int;fcout<<tmpv<<tmp_int;
}

void TestArray()
{
 DataArray<double> ar(5,3),ar2;
 ar=1;
 DataVector<double> vec(5);vec=1;
 ar2=ar;//=vec;
 ExEnv::ferr()<<Transpon(ar)<<Transpon(ar2);
 ar2=ar=vec;
 ar2=ar+1;
 ExEnv::ferr()<<Transpon(ar)<<Transpon(ar2);
 ar=ar+ar2;
 ExEnv::ferr()<<Transpon(ar)<<Transpon(ar2);
 ar=ar-ar2;
 ExEnv::ferr()<<Transpon(ar)<<Transpon(ar2);
// ExEnv::ferr()<<vec<<"\n";
 ExEnv::ferr()<<ar2*ar<<"\n"<<ar2*vec<<"\n"<<ar2*ar;
 ar=ar+10;
 ar.AddColumn(vec);
 ExEnv::ferr()<<"Added\n"<<Transpon(ar)<<ar.SetColumn(2)<<"\n"<<ar.SetRow(2)<<"\n";
// fcout<<Transpon(ar);
// for (int k=0;k<100000;k++) ar=Transpon(ar2);
// ExEnv::ferr()<<Transpon(ar)<<Transpon(ar2);
}

void TestVecRes()
 {
  int k=-10;
  ResizeIndex ind(10,-50,0,&k),ind1;
  ind[0]=10;
  for (k=0;k<=30;k++) ind[k]=k;
  ind[45]=1000;
  fcout<<ind;
  ind1=ind;
  fcout<<ind1;
  ind1.SetNumEl(20);
  ind1.SetNumEl(50);
  fcout<<ind1;
  *((SavableClass*)&ind1)=*((SavableClass*)&ind);
  fcout<<ind1;
 };

void SetX(double &x)
 {
  double y=2;
//  &x=&y;
 };
void TestRef()
 {
  double x=1;
  cout<<(void*)&x<<"\n";
  SetX(x);
  cout<<(void*)&x<<"\n";
  cout<<x;
 }

*/
#include "test.vec.h"

DescribedClass* create_TestX_cd(){return new TestX;}
DescribedClass* create_TestY_cd(){return new TestY;}

static ClassDesc TestX_cd(typeid(TestX),"TestX",1,"Test",&create_TestX_cd);
static ClassDesc TestY_cd(typeid(TestY),"TestY",1,"Test",&create_TestY_cd);
TestX::TestX(TestY *ptr) {SetRefMask(SingleFileStorage);y=ptr;  if (y)     y->x=this; }
TestY::TestY(TestX *ptr) {x=ptr;  if (x)     x->y=this; }
TestX::~TestX() { if (y)     y->x=NULL; }
TestY::~TestY() { if (x)     x->y=NULL; }
int TestY::save_data_state (FilterOut &so)  
 {
//  Ref<TestX> X=x;
//  so<<KeyDesc("RefX")<<X;
  so<<KeyDesc("RefX")<<x;//(SavableClass *)x;
//  so.refmask=wasMask;
  return 1;
 };
int TestY::read_data_state (FilterIn &si)  
 {
//  int wasMask=si.refmask;si.refmask=SavableClass::SingleFileStorage;
//  Ref<TestX> X;
//  si>>StdKey>>X;
//  x=X;X.unmanage();
  si>>StdKey;
  SavableClass *sc;
  si>>sc;x=(TestX*)sc;
//  si.refmask=wasMask;
  return 1;
 };
int TestX::save_data_state (FilterOut &so)  
 {
//  int wasMask=so.refmask;so.refmask=SavableClass::SingleFileStorage;
  so<<KeyDesc("RefY")<<y;//(SavableClass *)y;
//  so.refmask=wasMask;
  return 1;
 };
int TestX::read_data_state (FilterIn &si)  
 {
//  int wasMask=si.refmask;si.refmask=SavableClass::SingleFileStorage;
  si>>StdKey;
//  SavableClass *sc;
  si>>y;//sc;y=(TestY*)sc;
//  si.refmask=wasMask;
  return 1;
 };


  
void TestStorage()
 {
  Ref<TestX> t=new TestX(),tx=new TestX(new TestY(NULL));
//  fcout.refmask=SavableClass::SingleFileStorage;
  fcout.SetRefMask(SavableClass::SingleFileStorage);
  fcout<<*tx<<"\n\n";
  fcout.flush();

  t->SavableClass::operator=(*tx);
//  tx=NULL;
  fcout<<*t;
//   <<*(t->y);
 }

int test_vec(int, const char*)
{
//fcout<<" Test";fcout.flush();
//  TestVector();
//  IndRegBase reg;
//  TestArray();
//  TestVecRes();
cout<<Coreleft()<<"\n";cout.flush();

  TestStorage();
cout<<Coreleft()<<"\n";cout.flush();
};

