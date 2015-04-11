#include "DataVec.h"
#include "DataArr.h"
#include "grid_center.h"
//#include "grid_bound.h"
#include "ex_out.h"
#include "..\Alg_clc\std_alg.h"

#define HEAD(variable) "\n========================================\n============"<<variable<<"=============\n===========================================\n"

#include "NamedObject.h"


void main(int argc,char **argv)
 {
/*
SetLeakTest();
  FilterTextIn *in_ptr;
  FilterTextOut *out_ptr;
  Ref<Algorithm_TimeWrite> time_ptr;
  Ref<Algorithm_MakeStp> clc_ptr;
  
  in_ptr=new FilterTextIn(argv[1]);
  out_ptr=new FilterTextOut(argv[2]);
  SavableClass *stp,*clc;
  (*in_ptr)>>StdKey>>stp;
//  delete stp;
  (*in_ptr)>>StdKey>>clc;
//  delete clc;
  clc_ptr<<clc;time_ptr<<stp;
//  in_ptr->CloseBuf();  out_ptr->CloseBuf();
  delete in_ptr;
  delete out_ptr;
  clc_ptr=NULL;time_ptr=NULL;
LeakTest();
*/
///*
SetLeakTest();
  Algorithm_MainAction_Gasd act;
  if (act.SetParam(argc,argv)) act.WriteRes();
  act.Delete();
LeakTest();
//_CrtDumpMemoryLeaks( );

//*/
 };