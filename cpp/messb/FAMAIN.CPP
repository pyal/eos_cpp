


#include "famain.h"
//#include "test_case.h"
#define MaxArrPnt      15000

extern unsigned _stklen=64500;
double StndErr=1e-6;
extern int CopyFile(const char *src,const char *dst);


extern void WriteExample(char *name);
int  EnterXY(fstream &in_data,double &x,double &y,double &hi,
                                                  int coly,int colhi)
  { double ar[256];
    if (!EnterStr(in_data,&ar[0],256)) return 0;
    x=ar[1];y=ar[coly];
    if (colhi>0) hi=ar[colhi];
    return 1;
  };

int EnterAll(OutParStrc &OutPar,istream &in)
  {
   fstream in_data(OutPar.namein,ios::in);
   if (!seek_str(in,"SetBegin")) { cout<<"SetBegin not found\n";return 0;}
//   int HiCol;
   in>>OutPar.ColFr>>OutPar.ColTo>>OutPar.FrVel>>OutPar.ToVel>>OutPar.HiCol;
//cout<<"name "<<OutPar.namein<<" nameF "<<OutPar.nameout
//    <<" column r "<<OutPar.ColFr<<" column w "<<OutPar.ColTo
//    <<" FromV  "<<OutPar.FrVel<<" ToVel  "<<OutPar.ToVel<<"\n";
   double *Xval=new double[MaxArrPnt];
   double *Yval=new double[MaxArrPnt];
   double *Hival=new double[MaxArrPnt];
   OutPar.FirstRow=1;
   while (!seek_eof(in_data)) {
     if (EnterXY(in_data,Xval[1],Yval[1],Hival[1],OutPar.ColFr,OutPar.HiCol))
       if (Xval[1]<OutPar.FrVel) OutPar.FirstRow++;else break;}
   int Num=2;
   while ( (!seek_eof(in_data)) && (Num<MaxArrPnt) ){
     if (EnterXY(in_data,Xval[Num],Yval[Num],Hival[Num],OutPar.ColFr,OutPar.HiCol))
       if (Xval[Num]<=OutPar.ToVel) Num++;else break;}
   Num--;
   if (Num<=2) { cout<<"\nNo data points in input file \n";exit(1);}
   OutPar.NumRow=Num;Xval[0]=Num;Yval[0]=Num;
//cout<<"FirstRow "<<OutPar.FirstRow<<" Num R "<<Num<<"\n";ChRead();
   FAIO   *Func=NULL;

   if (!FAWRegister(in,WorkFunc)) {cout<<"Unknown work.\n";exit(0);}
   if (!FARegister(in,Func)) {cout<<"Unknown function.\n";exit(0);}
//cout<<" HiCol "<<HiCol;
   if (OutPar.HiCol==0) {delete Hival;Hival=NULL;}
   if (OutPar.HiCol<0) for (int k=1;k<=Num;k++) Hival[k]=1/Yval[k];
   WorkFunc->SetFuncPnt(Func,Xval,Yval,Hival);
//cout<<coreleft()<<" Entered All Good\n";
   return 1;
  };

void OutputStr(fstream &out,double *ar,int NumCol)
  {
   for (int k=1;k<=NumCol;k++)
     { 
      if (ar[k]!=NullNumber) out<<ar[k]<<"         "; else out<<"     \"\"     ";
     }
   out<<"\n";
  };

void  OutGrDat(OutParStrc &OutPar)
  {
   fstream out(OutPar.nameout,ios::out);
   fstream in (OutPar.namein ,ios::in);
//for (int k=1;k<=ExternHi.Par[0];k++) cout<<ExternHi.Par[k]<<"  ";
//cout<<" Par \n";
   int Num=0,NumCol;
   double ar[257],*Xval,*Yval,*Func_y;
   FAIO *Func;WorkFunc->GetFuncPnt(Func,Xval,Yval);
   Func_y=new double[Xval[0]+1];Func_y[0]=Xval[0];Func->Clc(Xval,Func_y);

   while (!seek_eof(in))
     {
      if ((NumCol=EnterStr(in,&ar[0],256))!=0)
        {
         Num++;
         for (int k=NumCol+1;k<=OutPar.ColTo;k++) ar[k]=NullNumber;
         if ((Num>=OutPar.FirstRow) && (Num<OutPar.FirstRow+OutPar.NumRow))
           {
            ar[OutPar.ColTo]=
                Func_y[Num-OutPar.FirstRow+1];
//              Func->Clc(Xval[Num-OutPar.FirstRow+1]);
           }
         else ar[OutPar.ColTo]=NullNumber;
         WriteStr(out,&ar[0],max(OutPar.ColTo,NumCol));
        }
     }
   delete Func_y;
   out.close();in.close();
  };

void  OutParDat(fstream &out,OutParStrc &OutPar)
  {
   out<<OutPar.ColFr<<"  "<<OutPar.ColTo<<"  ";
   out<<OutPar.FrVel<<"   "<<OutPar.ToVel<<"  "<<OutPar.HiCol<<"\n";
   WorkFunc->SaveInPar(out);
   out<<"\n";
  };

//#include "c:\alex\cpp\inc\util.h"

void ReturnInNew(fstream &in,fstream &in_new,char *inname)
  {
   in.close();in_new.close();
//   char tmp[256];
// strcpy(tmp,"copy ");strcat(tmp,"##intmp ");strcat(tmp,inname);
// system(tmp);
   if (! CopyFile ("##intmp",inname) ) exit(1);
   cout<<" Restore failure \n";
  };
void ChangeFile(char *inname,char *outname,int LoadColumn,int LoadFileNames,
                                           int LoadParameters)
  {
//cout<<" Begin Change of the file\n";
   try{
   char tmp[512],tmp1[512];
   double val,val1;
   int i,i1;
   fstream par(outname,ios::in);
   if ( (seek_eof(par)) || (par.fail()) || (par.bad()) )
     {
//      cout<<"seek_eof(par)"<<seek_eof(par)<<"par.fail()"<<par.fail()<<
//          "par.bad()"<<par.bad()<<"\n";
      cout<<" ChangeFile  ParamFile "<<outname
           <<" does not exist. Exiting.\n";exit(1);
     }
//   strcpy(tmp,"copy ");strcat(tmp,inname);strcat(tmp," ##intmp");system(tmp);
   if (!CopyFile (inname,"##intmp")) exit(1);

   fstream in("##intmp",ios::in);
   fstream in_new(inname,ios::out);
//   in.getline(tmp,511);in_new<<tmp<<"\n";
   while ( (Stricmp(tmp,"GeneralBegin")!=0) && (!seek_eof(in)) )
     { in>>tmp;in_new<<tmp<<"   ";if (seek_eoln(in)) in_new<<"\n";}
   if (!LoadFileNames)
     {in>>tmp>>tmp1;in_new<<tmp<<"     "<<tmp1<<"\n";par>>tmp>>tmp;}
   else
     {par>>tmp>>tmp1;in_new<<tmp<<"     "<<tmp1<<"\n";in>>tmp>>tmp;}
// =================   Seek SetBegin    =========

   while ( !(seek_eof(in)) )
     {
      strcpy(tmp,"copy ");
      while ( (Stricmp(tmp,"SetBegin")!=0) && (!seek_eof(in)) )
        { in>>tmp;in_new<<tmp<<"   ";if (seek_eoln(in)) in_new<<"\n";}
      if (seek_eof(in)) break;
//                Analize SetBegin String

      in>>i>>i1;
      if (!LoadColumn) par>>val>>val;
      else par>>i>>i1;
      in_new<<i<<"    "<<i1<<"   ";
      in>>val>>val1;par>>val>>val1;in_new<<val<<"   "<<val1;
      in>>i;if (!LoadColumn) par>>val;else par>>i;
      in_new<<"   "<<i<<"\n";
//      in>>i>>i1;in_new<<i<<"    "<<i1<<"\n";

//cout<<" Begin to Enter WorkFunc \n";
      if (!FAWRegister(in,WorkFunc)) {cout<<"Unknown work.\n";exit(0);}
      FAIO *Func=NULL;
      if (!FARegister(in,Func)) {cout<<"Unknown function.\n";exit(0);}
//cout<<"  == Register ==";
      WorkFunc->SetFuncPnt(Func,NULL,NULL,NULL);
      if (LoadParameters)  WorkFunc->GetInPar(par);   // Also Func par
//cout<<"  == LoadPar ==";
      WorkFunc->Output(in_new);
      Func->Output(in_new);
//cout<<"  == Output  ==";
      if (!LoadParameters) WorkFunc->GetInPar(par);
      delete WorkFunc;WorkFunc=NULL;
//cout<<" End to Enter FuncObj\n";ChRead();
     }
//cout<<" Deleting\n";
   in.close();par.close();in_new.close();
   unlink("##intmp");
   }  CATCHEXCEPTION("");
//   system("del ##intmp");
//cout<<" Sucssesfull ChangeFile\n";
  };
//#include <time.h>
#include <process.h>
int main( int argc,  char *argv[] )
  {
//   RandomInit();
   WorkFunc=NULL;
SetLeakTest();
try{
   if ((argc<2) || (GetCmd("/h",argc-1,argv)!=NULL))
     {
      cout<<"usage:\n"<<argv[0]<<"  in_method  [data_par_file] [keys] \n"<<
		   "      data_par_file     - file to save all parameters ( to use them after with /[aupcn] keys )\n"<<
		   "                          Keys can be used :\n"<<
           "      /a[name par file] - load all par from external data par, so [ /a!par ] is the same as [ /u!par /p /c /n ] \n"<<
           "      /u[name par file] - use  external data par from which params  will loaded according to setted (not setted ) keys - [/p /c /n] \n"<<
           "      /p - load  param   from par\n"<<
           "      /c - load  columns from par\n"<<
           "      /n - load  files from par\n"<<
           "      /o - write result function to the file(outfile specif in in_method), by default the result is not written there\n"<<
           "      /si - show i - positioned aprocsimation only - \n"<<
		   "           in one in_method file can be any number of [SetBegin] blocks, with different functions, \n"<<
		   "           and data for calculation, /si - shows ony i-th function(SetBegin block), by default i=1\n"<<
           "      /di - run calculation only for i-th - [SetBegin] block , can use \n"<<
		   "           /d0 - only to output(input) parameters, not to make any calculations\n"<<
           "      /e - edit current config file in notepad \n"<<
           "      /r - restore old parameters after minim \n"<<
           "      /v - use pseudo-random numbers (fixed rnd serie) \n"<<
           "      /h - display help\n"<<
           "      in file appr.fm now is example of the method file\n"<<
		   "      program version date "<<__DATE__<<" time: "<<__TIME__<<" \n";
      WriteExample("appr.fm");
//   double *testLeak=new double[10];
	  LeakTest();
	  exit(1);
     }
   char tmp1[256],*tmpptr;//tmp[256],data_in[256],data_out[256]
   int WriteGrDat=0,LoadColumn=0,LoadFileNames=0,LoadParameters=0,NotCalc=0,RestoreParam=0;
//   if ( (tmpptr=GetCmd("/e",argc-1,argv))!=NULL )  {system(string("notepad ").append(argv[1]).c_str());return;}
   if ( (tmpptr=GetCmd("/v",argc-1,argv))!=NULL )  RandomClass::InitSeed(RandomClass::RandomInit::PseudoRandom);
   if ( (tmpptr=GetCmd("/e",argc-1,argv))!=NULL )  {_execlp("notepad","notepad",argv[1],NULL);return 1;}
   if ( (tmpptr=GetCmd("/c",argc-1,argv))!=NULL )  LoadColumn=1;
   if ( (tmpptr=GetCmd("/p",argc-1,argv))!=NULL )  LoadParameters=1;
   if ( (tmpptr=GetCmd("/n",argc-1,argv))!=NULL )  LoadFileNames=1;
   if ( (tmpptr=GetCmd("/r",argc-1,argv))!=NULL )  RestoreParam=1;
   if ( (tmpptr=GetCmd("/u",argc-1,argv))!=NULL )
     ChangeFile(argv[1],tmpptr,LoadColumn,LoadFileNames,LoadParameters);
   else if ( (tmpptr=GetCmd("/a",argc-1,argv))!=NULL )
     {
//cout<<coreleft()<<" Mem avail Before while \n";
      
     ChangeFile(argv[1],tmpptr,1,1,1);
//cout<<coreleft()<<" Mem avail Before while \n";
     }

   if ( (tmpptr=GetCmd("/o",argc-1,argv))!=NULL ) WriteGrDat=1;
   if ( (tmpptr=GetCmd("/s",argc-1,argv))!=NULL) 
      { Show(argv,atoi(tmpptr));exit(1);}
   if ( (tmpptr=GetCmd("/d",argc-1,argv))!=NULL) 
      { NotCalc=atoi(tmpptr);if (NotCalc==0) NotCalc=-1;}

//   strcpy(tmp,"copy ");strcat(tmp,argv[1]);strcat(tmp," ");

   SetExt(argv[1],tmp1,"bak");//strcat(tmp,tmp1);system(tmp);
   if (!CopyFile (argv[1],tmp1)) exit(0);

   OutParStrc OutPar;
   int CurCalc=0;
//cout<<Coreleft()<<" Mem avail Before while \n";

   fstream *in=new fstream(argv[1],ios::in);
   if (!seek_str(*in,"GeneralBegin")) exit(0);

   *in>>OutPar.namein>>OutPar.nameout;

   if (WriteGrDat) {WriteGr(*in,OutPar,argv[1]);exit(0);}
   char outpar_name[256];
   if ((argc<3) || (argv[2][0]=='/')) strcpy(outpar_name,"##!tmp.par");
   else strcpy(outpar_name,argv[2]);
//cout<<" File for temp par "<<outpar_name<<"   "<<argv[2]<<"  "<<&argv[2][1]<<"\n";
   fstream outpar(outpar_name,ios::out);
   outpar<<OutPar.namein<<"    "<<OutPar.nameout<<"\n";
   int i=0;
   Time_struct time_it,time_full;
   while (!seek_eof(*in))
     {
//cout<<coreleft()<<" Mem avail Begin while \n";
      cout<<"Number of configuration under calculation "<<++i<<"\n";
      if (!EnterAll(OutPar,*in)) break;
//cout<<coreleft()<<" Mem avail Enter All   \n";
      if (!NotCalc) WorkFunc->FAWDo();
      else if (NotCalc==++CurCalc) WorkFunc->FAWDo();
cout<<" Work Time "<<time_it<<"\n";time_it.Mark();
//cout<<coreleft()<<" Mem avail Calc        \n";
      OutParDat(outpar,OutPar);
//cout<<coreleft()<<" Mem avail Circle Apr \n";
     }
cout<<" Full work time "<<time_full<<"\n";
   outpar.close();
   delete in;
   delete WorkFunc;WorkFunc=NULL;
//cout<<Coreleft()<<" Mem avail Delete Apr \n";
//cout<<" File for temp par "<<outpar_name<<"\n";

   if (!RestoreParam) ChangeFile(argv[1],outpar_name,0,0,1);
   else 
   {
     SetExt(argv[1],tmp1,"sav");//strcat(tmp,tmp1);system(tmp);
     if (!CopyFile (argv[1],tmp1)) exit(1);
     ChangeFile(tmp1,outpar_name,0,0,1);
   }
//  if (Stricmp(outpar_name,argv[2])!=0)
   if (Stricmp(outpar_name,"##!tmp.par")==0) unlink(outpar_name);
//   double *testLeak=new double[10];
   }
   CATCHMAINEXCEPTION(" famessb failed ");
   LeakTest();
   return 0;
  };
//
void WriteGr(fstream &in,OutParStrc &OutPar,const char *name)
  {
SetLeakTest();
   while (!seek_eof(in))
     {
      delete WorkFunc;WorkFunc=NULL;
//cout<<" GR Beg \n";
      if (!EnterAll(OutPar,in)) break;
//cout<<" GR OK\n";
      OutGrDat(OutPar);
// Just to work with changed data file
      if (!CopyFile (OutPar.nameout,"##tmp")) exit(1);
      strcpy(OutPar.namein,"##tmp");
     }
   delete WorkFunc;WorkFunc=NULL;
   unlink("##tmp");
//   delete WorkFunc;WorkFunc=NULL;
   cout<<" Generated output for "<<name<<" Testing memory\n";
LeakTest();

  };



   

void Show(char *argv[],int NumToShow)
  {
//cout<<NumToShow<<"\n";

   if (NumToShow<=0) NumToShow=1;
//   char data_in[256],data_out[256],tmp[256],*tmpptr;
   fstream *in=new fstream(argv[1],ios::in);
   OutParStrc OutPar;
   if (!seek_str(*in,"GeneralBegin")) exit(0);
   *in>>OutPar.namein>>OutPar.nameout;
   for (int k=1;k<=NumToShow;k++)
     {
//      delete WorkFunc;WorkFunc=NULL;
      if (!EnterAll(OutPar,*in)) exit(0);
     }
   fstream out("##tmp.dat",ios::out);
   fstream in1(OutPar.namein ,ios::in);
//   int Num=0,NumCol;

   double *Xval,*Yval,*Func_y;
   FAIO *Func=NULL;
   WorkFunc->GetFuncPnt(Func,Xval,Yval);
   Func_y=new double [Xval[0]+1];Func_y[0]=Xval[0];
   Func->Clc(Xval,Func_y);
   int N=Func->GetNumPar();
   double *TmpPar=new double[N+1];TmpPar[0]=N;
   Func->GetPar(TmpPar);Func->SetPar(TmpPar);
   delete TmpPar; //?
   double ar[257];
   for (int k=1;k<=Xval[0];k++)
     {
      ar[1]=Xval[k];ar[2]=Yval[k];
//      ar[3]=Func->Clc(Xval[k]);
      ar[3]=Func_y[k];
      OutputStr(out,&ar[0],3);
     }
   delete Func_y;delete in;
   out.close();
   delete WorkFunc;WorkFunc=NULL;
//cout<<coreleft()<<" Mem avail before dat \n";ChRead();
LeakTest();
   system("call dat ##tmp.dat");
//   _execlp("dat","dat","##tmp.dat",NULL);
  };









void WriteExample(char *name)
  {
   fstream out(name,ios::out);
   out
<<"=====================  Ignore up to GeneralBegim =============\n"   
<<"NameFrom   NameTo                                             \n"
<<"GeneralBegin   1in.dat     1out.dat                           \n"
<<"=====================   Ignore   up   to   SetBegim   ========\n"      
<<"Names      FrCol   ToCol   FrVel   ToVel   Col_Pnt_Val_0      \n"
<<"SetBegin      2        4   0        100    0                  \n"
<<"UncMin_appr_clc NumIt 1 NumT 6 FuncError 1.49012e-008 ResError 1.49012e-008 SumFuncMinim 0 \n"
<<"minimN_LM {                                                                                \n"
<<"    ErrorLQ  1e-008  MathEps05  1.49012e-008  ZeroEig  1e-010  BreakIterStp  1.555         \n"
<<"    Lambda   { CoefStart 10 DivideCoef 20 DivideVal 0.1 }                                  \n"
<<"    Optimized 2                                                                            \n"
<<"}                                                                                          \n"
<<"{  Min1D_Coef 1.2 Min1D_MaxLinSrch 5 Min1D_MaxRetry 5 }                                    \n"
<<"sum_func    2                                                 \n"
<<"lorents                                                       \n"
<<"Ground            V  F  V       0                             \n"
<<"Intencity         V  V  V       1                             \n"
<<"Position          F  V  V       30                            \n"
<<"Width             F  F  V       10                            \n"
<<"4-ord             F  F  F       0                             \n"
<<"lorents                                                       \n"
<<"Ground            F  F  V       0                             \n"
<<"Intencity         V  V  V       0                             \n"
<<"Position          F  V  V       0                             \n"
<<"Width             F  F  V       5                             \n"
<<"4-ord             F  F  F       0                             \n"
<<"                                                              \n"
<<"SetBegin   2         4    0        100      0                 \n"
<<"Std_appr_clc   NumIt  3 NumT  3 MinError 1e-06                \n"
<<"set_func    2                                                 \n"
<<"CorFuncConst0  3 CorFuncExpConst   1                          \n"
<<"Bnd         F F V V    245                                    \n"
<<"Num_Correlated_Par        3                                   \n"
<<"1    4    1                                                   \n"
<<"1    12   1                                                   \n"
<<"2    10   1                                                   \n"
<<"polynom                                                       \n"
<<"Ground            V  V  V  V     -764.439602                  \n"
<<"Intencity         F  F  F  F     1                            \n"
<<"Position          F  F  F  F     245                          \n"
<<"second            F  V  F  V     1.3                          \n"
<<"third             F  F  F  V     0                            \n"
<<"fourth            F  F  F  F     0                            \n"
<<"fifth             F  F  F  F     0                            \n"
<<"sixth             F  F  F  F     0                            \n"
<<"polynom                                                       \n"
<<"Ground            F  F  F  F     0                            \n"
<<"Intencity         F  F  F  F     1                            \n"
<<"Position          F  F  F  F     245.                         \n"
<<"second            F  V  F  V     20                           \n"
<<"third             F  F  F  V     -0.137803                    \n"
<<"fourth            F  F  F  F     0                            \n"
<<"fifth             F  F  F  F     0                            \n"
<<"sixth             F  F  F  F     0                            \n"
<<"                                                              \n"
<<"SetBegin      2    4   400   600            0                 \n"
<<"Std_appr_clc   NumIt  3 NumT  3 MinError 1e-06                \n"
<<"exponent                                                      \n"
<<"Ground            V  V  F  F  V     2211.17386                \n"
<<"Intencity         F  V  V  F  V     580.603287                \n"
<<"Position          F  F  F  V  V     500.01843                 \n"
<<"1/alpha_in_exp    F  F  V  F  V     31.268516                 \n"

<<"\n\n This is the standart head. Now detailed list of all functions:\n\n";
    Stroka res;
#ifdef FAMESSB_TEST
  //ClassDesc::list_all_classes("FAZer_Test");
    res += Stroka(" Registered FAZer_Test:\n\n\n") + SavableClass::list_all_classes("FAZer_Test");
        //HelpForCategory("FAZer_Test");
#endif
    res += Stroka(" Registered FAZer :\n\n\n") + SavableClass::list_all_classes("FAZer");
    //res += Stroka(" Registered URS_Curve_category :\n\n\n") + SavableClass::list_all_classes("URS_Curve_category");
    //res += Stroka(" Registered URS_Curve_Output_category :\n\n\n") + SavableClass::list_all_classes("URS_Curve_Output_category");
    //res += Stroka(" Registered URS_Curve_ClcVar_category :\n\n\n") + SavableClass::list_all_classes("URS_Curve_ClcVar_category");
    //Ref<URS_Curve> cur = new URS_Curve();
    //FilterTextIn in("Aaa");
    //cur->read_data_state(in);
        //HelpForCategory("FAZer");
  //ClassDesc::list_all_classes("FAZer");
    //res += Stroka(" Registered FAIO :\n\n\n") + SavableClass::HelpForCategory("FAIO");
    out<<res.c_str();
   out<<"\n\n ============================\nList of all functions:\n===================================\n\n";
   FAIO::list_all_functions(out);
   out<<"\n\n ============================\nList of all aproscimation methods:\n===================================\n\n";
   FAWZer::list_all_functions(out);
   out<<"\n";out.close();
  };




// Change inname File (CFG)  parameters according to outname data (DATA)
void WriteExample1(char *name)
  {
   fstream out(name,ios::out);
   out
<<"=====================  Ignore up to GeneralBegim =============\n"   
<<"NameFrom   NameTo                                             \n"
<<"GeneralBegin   1in.dat     1out.dat                           \n"
<<"=====================   Ignore   up   to   SetBegim   ========\n"      
<<"Names      FrCol   ToCol   FrVel   ToVel   Col_Pnt_Val_0      \n"
<<"SetBegin      2        4   0        100    0                  \n"
<<"Std_appr_clc   NumIt  3 NumT  3 MinError 1e-06                \n"
<<"sum_func    2                                                 \n"
<<"lorents                                                       \n"
<<"Ground            V  F  V       0                             \n"
<<"Intencity         V  V  V       1                             \n"
<<"Position          F  V  V       30                            \n"
<<"Width             F  F  V       10                            \n"
<<"4-ord             F  F  F       0                             \n"
<<"lorents                                                       \n"
<<"Ground            F  F  V       0                             \n"
<<"Intencity         V  V  V       0                             \n"
<<"Position          F  V  V       0                             \n"
<<"Width             F  F  V       5                             \n"
<<"4-ord             F  F  F       0                             \n"
<<"                                                              \n"
<<"SetBegin   2         4    0        100      0                 \n"
<<"Std_appr_clc   NumIt  3 NumT  3 MinError 1e-06                \n"
<<"set_func    2                                                 \n"
<<"CorFuncConst0  3 CorFuncExpConst   1                          \n"
<<"Bnd         F F V V    245                                    \n"
<<"Num_Correlated_Par        3                                   \n"
<<"1    4    1                                                   \n"
<<"1    12   1                                                   \n"
<<"2    10   1                                                   \n"
<<"polynom                                                       \n"
<<"Ground            V  V  V  V     -764.439602                  \n"
<<"Intencity         F  F  F  F     1                            \n"
<<"Position          F  F  F  F     245                          \n"
<<"second            F  V  F  V     1.3                          \n"
<<"third             F  F  F  V     0                            \n"
<<"fourth            F  F  F  F     0                            \n"
<<"fifth             F  F  F  F     0                            \n"
<<"sixth             F  F  F  F     0                            \n"
<<"polynom                                                       \n"
<<"Ground            F  F  F  F     0                            \n"
<<"Intencity         F  F  F  F     1                            \n"
<<"Position          F  F  F  F     245.                         \n"
<<"second            F  V  F  V     20                           \n"
<<"third             F  F  F  V     -0.137803                    \n"
<<"fourth            F  F  F  F     0                            \n"
<<"fifth             F  F  F  F     0                            \n"
<<"sixth             F  F  F  F     0                            \n"
<<"                                                              \n"
<<"SetBegin      2    4   400   600            0                 \n"
<<"Std_appr_clc   NumIt  3 NumT  3 MinError 1e-06                \n"
<<"exponent                                                      \n"
<<"Ground            V  V  F  F  V     2211.17386                \n"
<<"Intencity         F  V  V  F  V     580.603287                \n"
<<"Position          F  F  F  V  V     500.01843                 \n"
<<"1/alpha_in_exp    F  F  V  F  V     31.268516                 \n";

#ifdef __FAMessbRegist_H
   out
<<"HQ_poly           polycristall case                              \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       400                           \n"
<<"   QS             F  V  F  V       5                             \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0.5                           \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"HQ_mono            monocrystall                                  \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       400                           \n"
<<"   QS             F  V  F  V       5                             \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0.5                           \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"TetaH_View        F  F  F  F       0                             \n"
<<"PhiH_View         F  F  F  F       0                             \n"
<<"HQ_poly_rotate      polycristall rotate case                     \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       100                           \n"
<<"   QS             F  V  F  V       0.1                           \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0.2                           \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"   T              F  F  F  F       100                           \n"
<<"  Tkbt            F  F  F  F       28                            \n"
<<"   A              F  F  F  F       0.01                          \n"
<<"  W_coef          F  F  F  F       0.001                         \n"
<<"  Calculating according Kashuba clc of additional line-width     \n"
<<"	   double Wc=Wid*0.5+W_coef*fabs(ZerPos-Position[k1])*atan(M_PI*T/(2*(8*Tbkt-T)));  \n"
<<"	   s+=Intencity[k1]*(1-Sw*A*Mul/Wid)/(sqr(Wc)+sqr(Sw));                             \n"
<<"                                                                 \n"
<<"                                                                 \n"
<<"HQ_aver            average over angles, main axiz in I_Hx main Vect\n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       400                           \n"
<<"   QS             F  V  F  V       5                             \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0.5                           \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"  I_Hx            F  F  F  F       0                             \n"
<<"  I_Hy            F  F  F  F       0                             \n"
<<"  I_Hz            F  F  F  F       0                             \n"
<<"   Hc             F  F  F  F       0                             \n"
<<" Teta_V_H         F  F  F  F       0                             \n"
<<" Add_H            F  F  F  F       1                             \n"
<<"FineAver          F  F  F  F       0                             \n"
<<" Sum8             F  F  F  F       1                             \n"
<<"                                                                 \n"
<<"qh_relax6            Relaxation 6-axis                           \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       400                           \n"
<<"   QS             F  V  F  V       5                             \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0.5                           \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"  T0              F  F  F  F       0                             \n"
<<"  T60             F  F  F  F       0                             \n"
<<"  T120            F  F  F  F       0                             \n"
<<"  T180            F  F  F  F       0                             \n"
<<"  T240            F  F  F  F       0                             \n"
<<"  T300            F  F  F  F       1                             \n"
<<"                                                                 \n"
/*
<<"                                                                 \n"
<<"QH_poly         do not work for Phi                              \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       30                            \n"
<<"   QS             F  V  F  V       10                            \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0                             \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"                                                                 \n"
<<"QH_relax0       not relax case calculates as relax               \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       30                            \n"
<<"   QS             F  V  F  V       10                            \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0                             \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"Teta_H            F  F  F  F       0                             \n"
<<"Phi_H             F  F  F  F       0                             \n"
<<"                                                                 \n"
*/
<<"HQ_relax            relax + - H    case                          \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       30                            \n"
<<"   QS             F  V  F  V       10                            \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0                             \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"  Tup             F  F  F  F       0                             \n"
<<"  Tdown           F  F  F  F       0                             \n"
<<"                                                                 \n"
<<"HQ_relax0h          relax field - no field                       \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       30                            \n"
<<"   QS             F  V  F  V       10                            \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0                             \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"  TField          F  F  F  F       0                             \n"
<<"  TQs             F  F  F  F       0                             \n"
<<"                                                                 \n"
<<"                                                                 \n"
<<"F_relax2         relax 2 free case                               \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H1             F  V  F  V       30                            \n"
<<"   QS1            F  V  F  V       10                            \n"
<<"   IS1            F  V  F  V       0                             \n"
<<"   W1             F  F  F  V       0                             \n"
<<"  Teta1           F  F  F  F       0                             \n"
<<"  Phi1            F  F  F  F       0                             \n"
<<"  Eta1            F  F  F  F       0                             \n"
<<"  AlphH_zx1       F  F  F  F       0                             \n"
<<"  AlphH_zy1       F  F  F  F       0                             \n"
<<"   H2             F  F  F  F       0                             \n"
<<"   QS2            F  F  F  F       0                             \n"
<<"   IS2            F  F  F  F       0                             \n"
<<"   W2             F  F  F  F       0                             \n"
<<"  Teta2           F  F  F  F       0                             \n"
<<"  Phi2            F  F  F  F       0                             \n"
<<"  Eta2            F  F  F  F       0                             \n"
<<"  AlphH_zx2       F  F  F  F       0                             \n"
<<"  AlphH_zy2       F  F  F  F       0                             \n"
<<"  Tup             F  F  F  F       0                             \n"
<<"  Tdown           F  F  F  F       0                             \n"
<<"                                                                 \n"
<<"  void R01HQs - corrected ( is not correct?)                     \n"
<<"  Teta - Phi - Qs coordinates                                    \n"
<<"  AlphH_zx  AlphH_zy - H coordinates                             \n"
<<"  Relaxation Tup Tdown - compared to width                       \n"
<<"  (small - slow relax, high - high relax compred to width        \n"

<<"HQ_relax3           relax 3 case                                 \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       30                            \n"
<<"   QS             F  V  F  V       10                            \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0                             \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"  T1              F  F  F  F       0                             \n"
<<"  T2              F  F  F  F       0                             \n"
<<"  T3              F  F  F  F       0                             \n"
<<"  Alph_zx         F  F  F  F       0                             \n"
<<"  Alph_xy         F  F  F  F       0                             \n"
<<"                                                                 \n"
<<"qh_relax_aver           relax average over any angle             \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       30                            \n"
<<"   QS             F  V  F  V       10                            \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0.5                           \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"  RotTime         F  F  F  F       0                             \n"
<<"  NumTimes        F  F  F  F       0                             \n"
<<"  NumToAver       F  F  F  F       0                             \n"
<<"  FurieDisc       F  F  F  F       0                             \n"
<<"  Time            F  F  F  F       0                             \n"
<<"  OutPhi          F  F  F  F       0                             \n"
<<"  StrongSho       F  F  F  F       0                             \n"
<<"  SingleRot       F  F  F  F       0                             \n"
<<"  NTimeAver       F  F  F  F       10                            \n"
<<"  EndTAver        F  F  F  F       0                             \n"
<<"   Pup            F  F  F  F       0.1                           \n"
<<"  Pdown           F  F  F  F       0.001                         \n"
<<"  AnisK           F  F  F  F       10                            \n"
<<"                                                                 \n"
<<" Общие параметры                                                  \n"
<<" NumTimes 800  - сколько различных состояний вычисляется          \n"
<<"                (в каждом состоянии параметры поля - одинаковые), \n"
<<"                 дискретизация по времени.                        \n"
<<" NumToAver 10  -  Число усреднений ( перерасчет Phi(t), и для     \n"
<<"                  каждого варианта вычисляется спектр)            \n"
<<" Time      50  -  Сколько времени проводить интегрирование спектра\n"
<<"                 ( интенсивность излучения падает в е раз за      \n"
<<"                  время t=1/Width ), соответственно интенсивность \n"
<<"                  излучения на границе временного интервала       \n"
<<"                   exp(-Time*Width)                               \n"
<<" OutPhi    1  -  Выдавать или нет сгенерированные последовательности Phi(t)    \n"
<<" NumTAver 10  -  Сколько раз расчитывать спектр для заданного Phi(t),          \n"
<<"                 но с разным t0 - моментом начала излучения                    \n"
<<" EndTAver  5  -  На сколько последний момент t0 отличается от первого          \n"
<<" Различные методы вычисления Phi(t)                                            \n"
<<"                                                                               \n"
<<"SingleRot - варианты 0      -  Угол Phi вращается по модели Кашубы              \n"
<<"              RotTime 4000 -  Амплитуда гармоник - чем больше тем больше       \n"
<<"                              флуктуации поля для заданного шага по времени    \n"
<<"              FurieDisc 10 -  Число случайных гармоник расчитываемых в спектре.\n"
<<"              StrongSho 0.1 - Коэффициент при квадратичном члене, предложенным             \n"
<<"                              Сашей для нормализации спектра при высоких частотах          \n"
<<"                              определяет на сколько сильно подавляются высокие             \n"
<<"                              гармоники.                                                   \n"
<<"SingleRot - варианты 1         Угол Phi просто вращается с заданной частотой                \n"
<<"             RotTime    40  - Время за которое Phi изменяется на 360 градусов              \n"
<<"                              сравниваться должно с 1/Width                                \n"
//<<"SingleRot - варианты -1        Teta определяется по модели анизотроного магнетика            \n"
//<<"                              Model H=JT*sin(teta)^2                                         \n"
//<<"              AnisK          Probability  of state exp(-H/T)=exp(-AnisotrpCoef*sin(teta)^2)  \n"
//<<"              1/FurieDiscr      MeanTimeRelax                                              \n"
//<<"    StrongSho*MeanTimeRelax    TimeRelaxDispers                                            \n"
//<<"    Pup,Pdown                 Probability to change Dir                                    \n"
//<<"SingleRot - варианты -2       = вариант -1 плюс H(teta)=H*fabs(cos(teta))            \n"






<<"                                                                 \n"
<<"qh_relax_walk           relax average over random walk           \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       10                             \n"
<<"   H              F  V  F  V       400                           \n"
<<"   QS             F  V  F  V       0                             \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0.5                           \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"  OutPhi          F  F  F  F       1                             \n"
<<"  RelaxType       F  F  F  F       -2                             \n"
<<"  NumToAver       F  F  F  F       1                             \n"
<<"  NTimeAver       F  F  F  F       0                             \n"
<<"  EndTAver        F  F  F  F       1                             \n"
<<"  IntegTime       F  F  F  F       20                             \n"
<<"  JumpTimeTet     F  F  F  F       0.02                             \n"
<<"  JumpTimeTetS    F  F  F  F       0.01                             \n"
<<"  JumpTimePhi     F  F  F  F       0.02                            \n"
<<"  JumpTimePhiS    F  F  F  F       0.01                             \n"
<<"  JumpAngTet      F  F  F  F       5                           \n"
<<"  JumpAngTetS     F  F  F  F       1                         \n"
<<"  JumpAngPhi      F  F  F  F       5                            \n"
<<"  JumpAngPhiS     F  F  F  F       1                            \n"
<<"  AnisTeta        F  F  F  F       10                           \n"
<<"  AnisPhi         F  F  F  F       1                         \n"
<<"  PhiRelaxInf     F  F  F  F       0                            \n"
<<"  Pup             F  F  F  F       10                            \n"
<<"  Pdown           F  F  F  F       0.01                            \n"
<<"                                                                 \n"
<<" Общие параметры                                                  \n"
<<" OutPhi    1  -  Выдавать или нет сгенерированные последовательности Phi(t)    \n"
<<" NumToAver 10  -  Число усреднений ( перерасчет Phi(t), и для     \n"
<<"                  каждого варианта вычисляется спектр)            \n"
<<"    Специальные дополнительные типы усреднения (для ускорения?)   \n"
<<" NTimeAver 10  -  Сколько раз расчитывать спектр для заданного Phi(t),          \n"
<<"                 но с разным t0 - моментом начала излучения                    \n"
<<" EndTAver  5  -  На сколько последний момент t0 отличается от первого          \n"
<<"                                                                  \n"
<<" IntegTime 20  -  Сколько времени проводить интегрирование спектра\n"
<<"                 ( интенсивность излучения падает в е раз за      \n"
<<"                  время t=1/Width ), соответственно интенсивность \n"
<<"                  излучения на границе временного интервала       \n"
<<"                   exp(-Time*Width)                               \n"
<<" JumpTimeTet,JumpTimeTetS JumpTimePhi,JumpTimePhiS				 \n"
<<"                среднее время и дисперсия этого времени           \n"
<<"                в течение котого значения поля не меняются для    \n"
<<"                Teta, Phi соответственно (шаг по времени )        \n"
<<" JumpAngTet,JumpAngTetS JumpAngPhi,JumpAngPhiS				 \n"
<<"                среднее значение изменения угла за шаг по времени \n"
<<"                и дисперсия этого значения для Teta, Phi соответственно        \n"
<<" AnisTeta,AnisPhi  Probability  of state  \n"
<<"                     exp(-H/T)=exp(-AnisTeta*sin(teta)^2-AnisPhi*sin(phi)^2)  \n"
<<"  PhiRelaxInf       H(teta)=H*fabs(cos(teta)) ( очень большая скорость релаксации по phi) \n"
<<"    Pup,Pdown                 Probability to change Dir                                    \n"
<<" Различные методы вычисления Phi(t)                                            \n"
<<"                                                                               \n"
<<"RelaxType - варианты -1      -  Нет ограничений на скорость изменения угла     \n"
<<"                                в следующий момент - чисто случайные углы, вариация только Teta \n"
<<"                                также есть возможность переворота поля Pup/(Pup+Pdown) \n"
<<"RelaxType - варианты -2       = Есть ограничение скорости изменения углов, вариация и Teta и Phi \n"
<<"                                Pup,Pdown - не учитываются ( получаются автоматом )\n"

<<"qh_relax_Hn           relax average over n<7 H directions        \n"
<<"Ground      V  F  V  V       0                             \n"
<<"Intencity   V  F  V  V       10                            \n"
<<"   H        F  V  F  V       500                           \n"
<<"   QS       F  V  F  V       0.7                           \n"
<<"   IS       F  V  F  V       0                             \n"
<<"   W        F  F  F  V       0.5                           \n"
<<"  Teta      F  F  F  F       0                             \n"
<<"  Phi       F  F  F  F       0                             \n"
<<"  Eta       F  F  F  F       0                             \n"
<<"  numH      F  F  F  F       5                             \n"
<<"  T1        F  F  F  F       0.1                           \n"
<<"  H1        F  F  F  F       500                           \n"
<<" Alph1_zx   F  F  F  F       57.4                          \n"
<<" Alph1_xy   F  F  F  F       0                             \n"
<<"  T2        F  F  F  F       0.1                            \n"
<<"  H2        F  F  F  F       500                            \n"
<<" Alph2_zx   F  F  F  F       57.4                           \n"
<<" Alph2_xy   F  F  F  F       90                             \n"
<<"  T3        F  F  F  F       0.1                            \n"
<<"  H3        F  F  F  F       500                            \n"
<<" Alph3_zx   F  F  F  F       57.4                           \n"
<<" Alph3_xy   F  F  F  F       180                            \n"
<<"  T4        F  F  F  F       0.1                            \n"
<<"  H4        F  F  F  F       500                            \n"
<<" Alph4_zx   F  F  F  F       57.4                           \n"
<<" Alph4_xy   F  F  F  F       270                            \n"
<<"  T5        F  F  F  F       1.1                            \n"
<<"  H5        F  F  F  F       500                            \n"
<<" Alph5_zx   F  F  F  F       0                              \n"
<<" Alph5_xy   F  F  F  F       0                              \n"
<<"  T6        F  F  F  F       0.1                            \n"
<<"  H6        F  F  F  F       500                            \n"
<<" Alph6_zx   F  F  F  F       57.4                           \n"
<<" Alph6_xy   F  F  F  F       90                             \n"
 
<<" Общие параметры                                               \n"
<<" numH      5  -  <=6 Сколько углов поворота магнитного поля    \n"
<<"                  учитывать.                                   \n"
<<"  Считается что Qs фиксирован ( не меняется ) H - поворачивается\n"
<<"  относительно фиксированной системы координат                \n"
<<"  H стандартное - не используется Eta, phi - задают поворот    \n"
<<"  Qs относительно фиксированной системы координат            \n"

  
/*
<<"HQ_relax_circular   relax circular case                          \n"
<<"Ground            V  F  V  V       0                             \n"
<<"Intencity         V  F  V  V       1                             \n"
<<"   H              F  V  F  V       30                            \n"
<<"   QS             F  V  F  V       10                            \n"
<<"   IS             F  V  F  V       0                             \n"
<<"   W              F  F  F  V       0                             \n"
<<"  Teta            F  F  F  F       0                             \n"
<<"  Phi             F  F  F  F       0                             \n"
<<"  Eta             F  F  F  F       0                             \n"
<<"Tcircular         F  F  F  F       0                             \n"
<<"Alph_zx           F  F  F  F       0                             \n"
<<"Alph_xy           F  F  F  F       0                             \n"
<<"                                                                 \n"
 */
<<"Qs_T            QS(t) approcsimation                             \n"
<<"   QS_max            F             0                             \n"
<<"   Fst_split         F             0                             \n"
<<"Sec_split_coef       F             1                             \n"
<<"                                                                 \n"
<<"Is_T            IS(t) in the deby approcsimation                 \n"
<<"    Ground           F             0                             \n"
<<"    T_deby           F             0                             \n"
<<"                                                                 \n"
<<"Int_T           Int(t) in the deby approcsimation                \n"
<<"    Ground           F             0                             \n"
<<"    Intencity        F             0                             \n"
<<"    T_deby           F             0                             \n"
<<"                                                                 \n";

#endif
  };

/*
roll_func - multiply function given with Extern given by points

SetBegin   2    4   0   100                 0
Std_appr_clc   NumIt  3 NumT  3 MinError 1e-06
roll_func     Num 3  Name 2in.dat
lorents
Ground            f     0
Intencity         f     0.03
Position          f     30
Width             f     6
4-ord             F     0

Iterative approximation, given last appr in extern file
SetBegin   2    4   0   100                 0
Iter_appr_clc   NumTimes  50   NumIt  500 NParam  3 FileAppr 3in.dat
lorents
Ground            F     0
Intencity         F     0.03
Position          F     120
Width             F     6
4-ord             F     0
*/
