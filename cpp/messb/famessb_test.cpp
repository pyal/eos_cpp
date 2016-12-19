#include "famessb_test.h"
#include "lib\test_fact\test_case.h"
#include "famessb_test_internal.h"
static ClassDesc TestCase_Famessb_Static_ClassDesc(typeid(TestCase_Famessb_Static),"TestCase_Famessb_Static",1,"TestCase_Famessb_Static_category",create<TestCase_Famessb_Static>);
const char *TestCase_Famessb_Static::TestExeOutput="test_exe_output";
int TestCase_Famessb_Static::DeleteGoodResults=1;
int TestCase_Famessb_Static::PrintDiscrepencyEstimations=0;
double StndErr=1e-6;

#include "mat\dat_mani.h"
#include "mat\turnmat.h"

#include "lib/std/stdexception.h"
struct MinDistStruct
{
  VecCl SrcX,SrcY;
  V2D_XY Pnt;
  MinDistStruct(){};
  void Init(VecCl &x,VecCl &y,V2D_XY &pnt){SrcX=x;SrcY=y;Pnt=pnt;}
  double GetDist(VecCl &x,VecCl &y,V2D_XY &pnt){Init(x,y,pnt);return GetDist();}
  double GetDist()
  {
    VecCl x;x=SrcX+(-Pnt.x());
    VecCl y;y=SrcY+(-Pnt.y());
    double MinDist=1/MathZer;
    for (int k=1;k<=x.Dim()-1;k++)
    {
      V2D_XY V1(x[k],y[k]),V2(x[k+1],y[k+1]),dV=V2-V1;
      double dVn2=dV*dV,V1n2=V1*V1,V2n2=V2*V2;
      double Dist2=0,Dist;
      if (dVn2>MathZer) Dist2=sqr(V2D_XY::VectorMul(V1,V2))/dVn2;
      if (2*Dist2+dVn2>=V1n2+V2n2) Dist=sqrt(Dist2);
      else Dist=sqrt(min(V1n2,V2n2));
      if (Dist<MinDist) MinDist=Dist;
    }
    return MinDist;
  }
  double GetLength(VecCl &x,VecCl &y){Init(x,y,V2D_XY(0,0));return GetLength();}
  double GetLength()
  {
    double res=0;
    for (int k=1;k<=SrcX.Dim()-1;k++)
    {
      V2D_XY V1(SrcX[k],SrcY[k]),V2(SrcX[k+1],SrcY[k+1]),dV=V2-V1;
      res+=sqrt(dV*dV);
    }
    return res;
  }
};
/*
double MinDist(VecCl &x,)
{
  for (int k=minI;k<=maxI;k++)
  {

  }
}
*/
int TestCase_Famessb_Static::GetHi(string &name,double &Hi,string &Err)
{
  fstream res_f(name.c_str(),ios::in);
  if ( (!res_f) || (seek_eof(res_f)) ) 
  {  Err.append("Results were not generated for test "); return 0; }
  res_f.close();
  TData<double> *InArr;
  InputTDataF<double>(name.c_str(),InArr);
  double test=0,maxY=1e-6;
  int N=InArr->I[0],num=0;
  VecCl x(N),y(N);MemoryMove(InArr->D[0],&x.Ptr[1],sizeof(double)*N);
  MemoryMove(InArr->D[1],&y.Ptr[1],sizeof(double)*N);
  MinDistStruct Clc;
//  for (int k=0;k<N;k++)
  for (int k=1;k<=N;k++)
  {
    num++;
    //maxY=max<double>(maxY,fabs(InArr->D[1][k]));
    //test+=fabs(InArr->D[1][k]-InArr->D[2][k]);
    int i=max(k-10,1),j=min(k+10,N);
    test+=Clc.GetDist(x.ExtractSubVector(i,j),y.ExtractSubVector(i,j),V2D_XY(InArr->D[0][k-1],InArr->D[2][k-1]));
  }
  maxY=Clc.GetLength(x,y);
  delete InArr;
  if (num==0) {Err.append("There is no data for test ");return 0;}
  Hi=test/maxY/num;
  Hi*=100;
  if (PrintDiscrepencyEstimations) cout<<" Hi "<<Hi<<" MaxY "<<maxY<<" n "<<num<<"\n";
  return 1;
}
int TestCase_Famessb_Static::Test_External_Func(ResourcesStruct *resources,int Length,string &Err)
{
  long size;
  Err.clear();
  for (int k=0;k<Length;k++) 
  {
    string nam_cfg(resources[k].Name);nam_cfg.append(".cfg");
    string nam_dat(resources[k].Name);nam_dat.append(".dat");
    string nam_res(resources[k].Name);nam_res.append(".res");
    string nam_bak(resources[k].Name);nam_bak.append(".bak");
    fstream dat_f(nam_dat.c_str(),ios::out);
        unlink(nam_res.c_str());
    char *dat=(char*)ResourceReader::Load(resources[k].data,"TXT",size);
    int k1=0;
    while (k1<size) {if (dat[k1]==13) {k1++;continue;} dat_f.write(&dat[k1],1);k1++;}
    fstream cfg_f(nam_cfg.c_str(),ios::out);
    dat=(char*)ResourceReader::Load(resources[k].config,"TXT",size);
    k1=0;
    while (k1<size) {if (dat[k1]==13) {k1++;continue;} cfg_f.write(&dat[k1],1);k1++;}
    cfg_f.close();dat_f.close();
    cout<<"                      "<<nam_cfg.c_str()<<"\n";
    cout.flush();
    system(string("famessb ").append(nam_cfg).append(" /o /v>>").append(TestExeOutput).c_str());
    double test;
    if (!GetHi(nam_res,test,Err)) Err.append(resources[k].Name).append("\n");
    else
    {
      char tmp[256],tmp1[256];
      if (test>resources[k].err) 
      {
        Err.append("Error ").append(gcvt(test,7,&tmp[0])).append(" is too big for test ").
          append(resources[k].Name).append(" It have to be ").
          append(gcvt(resources[k].err,7,&tmp1[0])).append("\n");
      }
      else 
      {
//          cout<<"Test "<<resources[k].Name<<" is OK. Misf "<<test<<" have to be "<<resources[k].err<<"\n";
        if (DeleteGoodResults)
        {unlink(nam_dat.c_str());unlink(nam_cfg.c_str()); unlink(nam_res.c_str());}
      }

    }
    unlink(nam_bak.c_str());
  }
  return (Err.length()==0);
//		throw(TestExeption("OK"));
}
void TestCase_Famessb_Static::Text_Func_Out(ResourcesStruct *res,int ResLen,const char *name)
{
  string err,ResErr,Message;
  Message.assign(" Test is BAD!!!  : ").append(name).append("\n");//throw(TestExeption("OK"));
  if (Test_External_Func(res,ResLen,err)) 
    Message.assign(" Test is OK  : ").append(name).append("\n");//throw(TestExeption("OK"));
  else ResErr.append("Error in ").append(name).append("function: \n    ").append(err).append("\n");
  cout<<Message.c_str();
  if (ResErr.length()!=0) cout<<string("Error in external functions: \n    ").append(ResErr).append("\n").c_str();
};

#include "lib/ref/commandline.h"
#include "lib/std/ex_out.h"
int main( int argc, const char *argv[] )
{
    SetLeakTest();
    Time_struct Time;
    try {
        CommandLineExe Cmd("Usage: famessb_test [Params_Key_Spec] [/][-]key  \n without parameters - standart test\n");
        Cmd.Add('h',"PrintHelp"," - print this help \n");
        Cmd.Add('p',"PrintDiscrepency"," - print discrepency estimations \n");
        Cmd.Add('a',"AllResults"," - not to delete good results \n");
//        Cmd.MainHelp += GenerateDetailedHelp();

        Cmd.Analize(argc, argv);
        if (Cmd.Get("PrintDiscrepency").Exist) TestCase_Famessb_Static::PrintDiscrepencyEstimations=1;
        if (Cmd.Get("AllResults").Exist) TestCase_Famessb_Static::DeleteGoodResults=0;
        if (Cmd.Argc!=1 || Cmd.Get("PrintHelp").Exist){
            cout<<Cmd.MakeHelp();
            return 0;
        } 
        TestSuite::RunTest("TestCase_Famessb_Internal");
        TestSuite::RunTest("TestCase_Famessb_Static");
        }
    catch(stdexception &ex){ 
        cout<<" Exeption caught:\n"<<ex.what()<<"\n";
        return 1;
    }
    cout<<"Test time: "<<Time<<"\n";
    LeakTest();
    return 0;
   //if ( (argc>=2) && (Stricmp(argv[1],"/h")==0) )
   //     {cout<<" usage "<<argv[0]<<" [/a] [/h] [/p] \n without parameters - standart test, /a - not to delete good results /h - draw this help /p - print discrepency estimations \n";exit(0);}
   //if ( GetCmd("/a",argc-1,argv)!=NULL ) TestCase_Famessb_Static::DeleteGoodResults=0;
   //if ( GetCmd("/p",argc-1,argv)!=NULL ) TestCase_Famessb_Static::PrintDiscrepencyEstimations=1;
   // 
   //TestSuite::RunTest("TestCase_Famessb_Internal");
   //TestSuite::RunTest("TestCase_Famessb_Static");
}
