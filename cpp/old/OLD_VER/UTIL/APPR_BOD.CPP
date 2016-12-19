
#include "appr_bod.h"
#define MaxArrPnt      1024

extern unsigned _stklen=50000;

ExternHiStruc ExternHi;

void InitSum(double *x,double *y,fstream &in)
  {
   ExternHi.Xval=x;ExternHi.Yval=y;
   in>>ExternHi.NumIt>>ExternHi.NumT;
   in>>ExternHi.FuncObj;
   ExternHi.MinError=1e-6;
  };

void DeleteAppr()
  {
   delete ExternHi.Xval;
   delete ExternHi.Yval;
  };

void ApprCalc()
  {
   for(int k0=1;k0<=ExternHi.NumT;k0++)
     {
      int N=ExternHi.FuncObj.NumPar;
      double *TmpPar=new double[N+1];TmpPar[0]=N;
      double *TmpStp=new double[N+1];TmpStp[0]=N;
      ExternHi.FuncObj.GetParStp(TmpPar,TmpStp);
//for (int i=1;i<=TmpPar[0];i++) cout<<TmpPar[i]<<" "<<TmpStp[i]<<"   ";
//cout<<"\n";

      MAPClc(TmpPar,TmpStp,ExternHi.NumIt,
             &Hi_Func,&Get_Theo,ExternHi.Yval,ExternHi.MinError);
//cout<<" Hi After Iter= "<<Hi_Func(ExternHi.Par)<<"\n";
//for ( i=1;i<=TmpPar[0];i++) cout<<TmpPar[i]<<" "<<TmpStp[i]<<"   ";
//cout<<"\n";
//for ( i=1;i<=TmpPar[0];i++) cout<<TmpPar[i]<<" "<<TmpStp[i]<<"   ";
//cout<<"\n";
      ExternHi.FuncObj.SetPar(TmpPar);
cout<<"Hi After Iter= "<<sqrt(Hi_Func(TmpPar))/ExternHi.Xval[0]<<"\n";
      delete TmpPar;delete TmpStp;
     };
//   ChRead();
//   cout<<"\n Main End Calc AprStrc"<<coreleft()<<"\n";ChRead();
  };
double Hi_Func(double *par)
  {
//for (int i=1;i<=par[0];i++) cout<<par[i]<<"   ";
//cout<<"\n";
//cout<<" Hi ";
   double hi=0;
   ExternHi.FuncObj.SetPar(par);
   double *Res=new double[ExternHi.Xval[0]+1];
   ExternHi.FuncObj.Clc(ExternHi.Xval,Res);
   for (int k=1;k<=ExternHi.Xval[0];k++)
     hi+=sqr(Res[k]-ExternHi.Yval[k]);
   delete Res;
//cout<<hi<<"\n";//ChRead();
   return hi;
  };
void Get_Theo(double *par,double *res)
  {
   ExternHi.FuncObj.SetPar(par);
   ExternHi.FuncObj.Clc(ExternHi.Xval,res);
   res[0]=ExternHi.Xval[0];
  };

int  EnterXY(fstream &in_data,double &x,double &y,int col)
  { double ar[256];
    if (!EnterStr(in_data,&ar[0],256)) return 0;
    x=ar[1];y=ar[col];
    return 1;
  };
int EnterAll(OutParStrc &OutPar,fstream &in)
  {
   char nameF[256];
   double NumF;
   fstream in_data(OutPar.namein,ios::in);
   if (!seek_str(in,"SetBegin")) { cout<<"SetBegin not found\n";return 0;}
   in>>NumF>>OutPar.ColFr>>OutPar.ColTo>>ExternHi.FrVel>>ExternHi.ToVel;
//cout<<"name "<<OutPar.namein<<" nameF "<<OutPar.nameout
//    <<" column r "<<OutPar.ColFr<<" column w "<<OutPar.ColTo
//    <<" FromV  "<<ExternHi.FrVel<<" ToVel  "<<ExternHi.ToVel<<"\n";
   double *Xval=new double[MaxArrPnt];
   double *Yval=new double[MaxArrPnt];
   OutPar.FirstRow=1;
   while (!seek_eof(in_data)) {
     if (EnterXY(in_data,Xval[1],Yval[1],OutPar.ColFr))
       if (Xval[1]<=ExternHi.FrVel) OutPar.FirstRow++;else break;}
   int Num=2;
   while ( (!seek_eof(in_data)) && (Num<MaxArrPnt) ){
     if (EnterXY(in_data,Xval[Num],Yval[Num],OutPar.ColFr))
       if (Xval[Num]<=ExternHi.ToVel) Num++;else break;}
   Num--;
   if (Num==1) { cout<<"No data points in input file \n";return 0;}
   OutPar.NumRow=Num;Xval[0]=Num;Yval[0]=Num;
//cout<<"FirstRow "<<OutPar.FirstRow<<" Num R "<<Num<<"\n";ChRead();
   InitSum(Xval,Yval,in);
cout<<"Entered All Good\n";
   return 1;
  };
void OutputStr(fstream &out,double *ar,int NumCol)
  {
   for (int k=1;k<=NumCol;k++)
     { 
      if (ar[k]!=NullNumber) out<<ar[k]<<"  "; else out<<"     \"\"     ";
     }
   out<<"\n";
  };
void  OutGrDat(OutParStrc &OutPar)
  {
   double ar[256];
   fstream out(OutPar.nameout,ios::out);
   fstream in (OutPar.namein ,ios::in);
//for (int k=1;k<=ExternHi.Par[0];k++) cout<<ExternHi.Par[k]<<"  ";
//cout<<" Par \n";
   int Num=0,NumCol;
   while (!seek_eof(in))
     {
      if (NumCol=EnterStr(in,&ar[0],256))
        {
         Num++;
         for (int k=NumCol+1;k<=OutPar.ColTo;k++) ar[k]=NullNumber;
         if ((Num>=OutPar.FirstRow) && (Num<OutPar.FirstRow+OutPar.NumRow))
           { 
            ar[OutPar.ColTo]=
              ExternHi.FuncObj.Clc(ExternHi.Xval[Num-OutPar.FirstRow+1]);
           }
         else ar[OutPar.ColTo]=NullNumber;
         WriteStr(out,&ar[0],max<int>(OutPar.ColTo,NumCol));
        }
     }
  };

void  OutParDat(fstream &out,OutParStrc &OutPar)
  {
   int N=ExternHi.FuncObj.NumPar;
   double *TmpPar=new double[N+1];TmpPar[0]=N;
   ExternHi.FuncObj.GetPar(TmpPar);
   out<<OutPar.ColFr<<"  "<<OutPar.ColTo<<"  ";
   out<<ExternHi.FrVel<<"   "<<ExternHi.ToVel<<"   ";
   for (int k=1;k<=N;k++) out<<TmpPar[k]<<"   ";
   out<<" \n";
   delete TmpPar;
  };

// Change inname File (CFG)  parameters according to outname data (DATA)
void WriteExample(char *name)
  {
   fstream out(name,ios::out);
   out<<
"=====================  Ignore up to GeneralBegim ==============     \n"<<          
"                      NameFrom          NameTo                      \n"<<          
"GeneralBegin d:\\alex\\exp\\ap_in.dat   d:\\alex\\cpp\\util\\!ap_out.dat \n"<<
"=====================   Ignore   up   to   SetBegim   ==============\n"<<   
"Names   NumSet   FrCol   ToCol   FrVel   ToVel   NumIt   NumTimes   \n"<<
"SetBegin   2       2       4     0.00    100.0   3       3          \n"<<
"sum_func     2                                                      \n"<<
"Lorents                                                             \n"<<
"Ground             Fast               2.033323e-10                  \n"<<
"Intencity          Fast               0.0625                        \n"<<
"Position           Fi                 30                            \n"<<
"Width              Fi                 8                             \n"<<
"4-ord              Fix                0                             \n"<<
"Lorents                                                             \n"<<
"Ground             Fix                0                             \n"<<
"Intencity          Fast               0.08                          \n"<<
"Position           Fi                 60                            \n"<<
"Width              Fi                 10                            \n"<<
"4-ord              Fix                0                             \n"<<
"=====================   Ignore   up   to   SetBegim   ==============\n"<<   
"====================   End   of   First   Row   ====================\n"<<
"SetBegin   1      3  5    0.00000         100.00000    3     3      \n"<<
"sum_func     1                                                      \n"<<
"Lorents                                                             \n"<<
"Ground             Fast               -1.488547e-08                 \n"<<
"Intencity          Fast               0.1                           \n"<<
"Position           Fast               50                            \n"<<
"Width              Fast               20.000034                     \n"<<
"4-ord              Fix                0                             \n";
  };
void ReturnInNew(fstream &in,fstream &in_new,char *inname)
  {
   in.close();in_new.close();
   char tmp[256];
   strcpy(tmp,"copy ");strcat(tmp,"##intmp ");strcat(tmp,inname);
   system(tmp);
   cout<<" Restore failure \n";
  };
                                                                     
void ChangeFile(char *inname,char *outname,int LoadColumn,int LoadFileNames)
  {                                                                  
   char tmp[512],tmp1[512];
   double val;
   int i,i1; 
   strcpy(tmp,"copy ");strcat(tmp,inname);strcat(tmp," ##intmp");
   system(tmp);
   fstream in("##intmp",ios::in);
   fstream par(outname,ios::in);
   fstream in_new(inname,ios::out);
   in.getline(tmp,511);in_new<<tmp<<"\n";
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
//      if (seek_eof(par)) {ReturnInNew(in,in_new,inname);break;}
      while ( (Stricmp(tmp,"SetBegin")!=0) && (!seek_eof(in)) )
        { in>>tmp;in_new<<tmp<<"   ";if (seek_eoln(in)) in_new<<"\n";}
//                Analize SetBegin String      
      in>>i;in_new<<"   "<<i<<"   ";
      in>>i>>i1;
      if (!LoadColumn) par>>val>>val;
      else par>>i>>i1;
      in_new<<i<<"    "<<i1<<"   ";
      in>>val;in>>val;par>>val;in_new<<val<<"   ";par>>val;in_new<<val<<"   ";
      in>>i>>i1;in_new<<i<<"    "<<i1<<"\n";

      if (seek_eof(in)) break;
cout<<" Begin to Enter FuncObj\n";
      if (seek_eof(par)) {ReturnInNew(in,in_new,inname);break;}
      in>>ExternHi.FuncObj;
      int N=ExternHi.FuncObj.NumPar;
      double *TmpPar=new double[N+1];TmpPar[0]=N;
      for (int k=1;k<=N;k++) par>>TmpPar[k];
      ExternHi.FuncObj.SetPar(TmpPar);
      in_new<<ExternHi.FuncObj;
      delete TmpPar;
     }
   in.close();
   system("del ##intmp");
  };
void main( int argc, char *argv[] )
  {
   if ((argc==1) || (GetCmd("/h",argc,argv)!=NULL)) 
     {
      cout<<"usage:\n"<<argv[0]<<"  [in_method] [data par]\n"<<
           "      /d[name par file] - use  external data par \n"<<
           "      /c - load not only parameters but also columns and in out dat files\n"<<
           "      /o - write result function to the file(outfile specif in in_method)\n"<<
           "      /s - show first positioned aproscimation only\n"<<
           "      /h - display help\n"<<
           "      in file appr.cfg now is example of the method file\n";
      WriteExample("appr.cfg");exit(1);
     }
   char data_in[256],data_out[256],tmp[256],tmp1[256],*tmpptr;
   int WriteGrDat=0,LoadColumn;
   if ( (tmpptr=GetCmd("/c",argc-1,argv))!=NULL )  LoadColumn=1;
   if ( (tmpptr=GetCmd("/d",argc-1,argv))!=NULL )  
     ChangeFile(argv[1],tmpptr,LoadColumn,LoadColumn);
   if ( (tmpptr=GetCmd("/o",argc-1,argv))!=NULL )  WriteGrDat=1;
   if (GetCmd("/s",argc-1,argv)!=NULL) { Show(argv);exit(0);}
cout<<coreleft()<<" Mem avail Before while \n";

   fstream *in=new fstream(argv[1],ios::in);
   fstream *outpar=new fstream(argv[2],ios::out);
   OutParStrc OutPar;
   if (!seek_str(*in,"GeneralBegin")) exit(0);

   *in>>OutPar.namein>>OutPar.nameout;
   *outpar<<OutPar.namein<<"    "<<OutPar.nameout<<"\n";
   strcpy(tmp,"copy ");strcat(tmp,argv[1]);strcat(tmp," ");
   SetExt(argv[1],tmp1,"bak");strcat(tmp,tmp1);system(tmp);
//   strcpy(OutPar.namein,"##tmp");
   while (!seek_eof(*in))
     {
cout<<coreleft()<<" Mem avail Begin while \n";
      if (!EnterAll(OutPar,*in)) break;
cout<<coreleft()<<" Mem avail Enter All   \n";
      ApprCalc();
cout<<coreleft()<<" Mem avail Calc        \n";
      OutParDat(*outpar,OutPar);
// Just to work with changed data file 
      if (WriteGrDat)
        {
         OutGrDat(OutPar);
         strcpy(tmp,"copy ");strcat(tmp,OutPar.nameout);strcat(tmp," ##tmp");
         system(tmp);
         strcpy(OutPar.namein,"##tmp");
        }
cout<<coreleft()<<" Mem avail Circle Apr \n";
      DeleteAppr();
     }
//   delete ExternHi.Apr;
   delete in;delete outpar;
//   delete ExternHi.FuncObj
cout<<coreleft()<<" Mem avail Delete Apr \n";
  if (WriteGrDat) system("del ##tmp");
  ChangeFile(argv[1],argv[2],0,0);
  };

void Show(char *argv[])
  {
   char data_in[256],data_out[256],tmp[256],*tmpptr;
   fstream *in=new fstream(argv[1],ios::in);
   OutParStrc OutPar;
   if (!seek_str(*in,"GeneralBegin")) exit(0);
   *in>>OutPar.namein>>OutPar.nameout;
   if (!EnterAll(OutPar,*in)) exit(0);

   fstream out("##tmp.dat",ios::out);
   fstream in1(OutPar.namein ,ios::in);
//   int Num=0,NumCol;
   int N=ExternHi.FuncObj.NumPar;
   double *TmpPar=new double[N+1];TmpPar[0]=N;
   ExternHi.FuncObj.GetPar(TmpPar);
   ExternHi.FuncObj.SetPar(TmpPar);
   double ar[257];
   for (int k=1;k<=ExternHi.Xval[0];k++)
     {
      ar[1]=ExternHi.Xval[k];
      ar[2]=ExternHi.Yval[k];
      ar[3]=ExternHi.FuncObj.Clc(ExternHi.Xval[k]);
      OutputStr(out,&ar[0],3);
     }
   out.close();
//cout<<coreleft()<<" Mem avail before dat \n";ChRead();
   system("dat ##tmp.dat");
  };

