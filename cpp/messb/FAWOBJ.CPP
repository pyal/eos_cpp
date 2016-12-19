#include <lib\precompiled\Messb.h>

#include "fawobj.h"

//#define USE_H 10
#define MaxParPnt 1024

void FAWZer::list_all_functions(ostream &out)
{
	//DescribedClass* tmp=ClassDesc::list_category_start("FAWZer");
	ClassDesc* tmp=ClassDesc::list_category_start("FAWZer");
    //Ref<FAWZer> func=dynamic_cast<FAWZer*>(tmp->create());
	while (tmp)
	{
        Ref<DescribedClass> tmpr = tmp->create();
        if (!tmpr){
            cout<<" Ups... Could not create class for classname<"<<tmp->name()<<">\n";
        }
        Ref<FAWZer> func;func<<tmpr;
		func->DrawHelp(out);
        tmp=ClassDesc::list_category_next(tmp,"FAWZer");
		out<<"\n\n";
        //return ;
	}
}


//===========================================================
//===================   FAWStand   ==========================
//===========================================================

#ifndef WCPP_NET
DescribedClass * Dummy_FAWStand(){return new FAWStand();}
static ClassDesc FAWStand_ClassDesc(typeid(FAWStand),"Std_appr_clc",1,"FAWZer",Dummy_FAWStand);
#else
static ClassDesc FAWStand_ClassDesc(typeid(FAWStand),"Std_appr_clc",1,"FAWZer",create<FAWStand>);
#endif

FAWStand::FAWStand():FAWZer(),MinError(1e-6),NumT(1),NumIt(3)
{  name=strdup("Std_appr_clc");};


FAWStand* FAWStand::CurPtr=NULL;



void FAWStand::FAWDo()
  { 
   int Num=Func->GetNumPar(),Good=1;
   NumFuncEval=0;
   CurPtr=this;
   double *TmpPar=new double[Num+1],*TmpStp=new double[Num+1];
   for (int NumApp=1;NumApp<=Func->GetNumIter();NumApp++)
     {
      if (NumApp>1) Func->GetNumIter(NumApp);
      cout<<" NumIter "<<NumApp<<" Hi After Iter= ";
      Func->GetParStp(TmpPar,TmpStp);TmpPar[0]=Num;TmpStp[0]=Num;
      double ResHi=0;
//cout<<"Stp: ";for (int i=1;i<=TmpStp[0];i++) cout<<TmpStp[i]<<"  ";cout<<"\n";
      for (int k=1;k<=NumT;k++)
        {
         if (!MAPClc(TmpPar,TmpStp,NumIt,&Hi_Func,MinError,1,&Get_Theo,YArr,HiArr))
           { Good=0;break;}
         ResHi=sqrt(Hi_Func(TmpPar))/XArr[0];
         cout<<ResHi<<"  ";
        }
      cout<<"\n      ResHi="<<ResHi<<" NumFEval "<<NumFuncEval<<"  ";
      cout<<"\n";
      if (Good) Func->SetPar(TmpPar);
      else break;
                                                                        
     }
   delete TmpPar;delete TmpStp;
  }

double FAWStand::Hi_Func(double *par)
  {
   double hi=0;
   CurPtr->Func->SetPar(par);
   double *Res=new double[CurPtr->XArr[0]+1];
   CurPtr->Func->Clc(CurPtr->XArr,Res);
   if (CurPtr->HiArr==NULL) for (int k=1;k<=CurPtr->XArr[0];k++)  hi+=sqr(Res[k]-CurPtr->YArr[k]);//cout<<"????";}
   else             for (int k=1;k<=CurPtr->XArr[0];k++)  hi+=sqr(Res[k]-CurPtr->YArr[k])*CurPtr->HiArr[k];//cout<<"!NULL";}
   delete Res;
   CurPtr->NumFuncEval++;
   return hi;
  };
void FAWStand::Get_Theo(double *par,double *res)
  {
   CurPtr->Func->SetPar(par);
   CurPtr->Func->Clc(CurPtr->XArr,res);
   res[0]=CurPtr->XArr[0];
   CurPtr->NumFuncEval++;
  };

void FAWStand::Output(ostream &output)
  { 
//cout<<" Name FAWStand "<<name<<"\n";
   output<<name<<"   NumIt  "<<NumIt<<" NumT  "<<NumT
                                     <<" MinError "<<MinError<<"\n";};
void FAWStand::Input(istream &input)
  {
   char tmp[256];
   input>>tmp>>NumIt>>tmp>>NumT>>tmp>>MinError;
  };







int FAWRegister(istream &in,FAWZer *&WDo)
{
	char name[256];in>>name;
	ClassDesc* descr=ClassDesc::name_to_class_desc(name);
	WDo=0;
  if ( (!descr) || (Stricmp(descr->category(),"FAWZer")!=0) )
    {cout<<" Could not find approcsimation function with name:"<<name<<"\n";delete descr;return 0;}
//  else
	WDo=dynamic_cast<FAWZer*>(descr->create());WDo->Input(in);return 1;
}















#ifndef WCPP_NET
DescribedClass * Dummy_FAWOptim(){return new FAWOptim();}
static ClassDesc FAWOptim_ClassDesc(typeid(FAWOptim),"Optim_appr_clc",1,"FAWZer",Dummy_FAWOptim);
#else
static ClassDesc FAWOptim_ClassDesc(typeid(FAWOptim),"Optim_appr_clc",1,"FAWZer",create<FAWOptim>);
#endif

FAWOptim* FAWOptim::CurPtr=NULL;
FAWOptim::FAWOptim():FAWZer(),FuncError(M_Eps2),ResError(M_Eps2),NumT(1),NumIt(3),SumFuncMinim(0)
{  delete name;name=strdup("Optim_appr_clc");};

void FAWOptim::Output(ostream &output)
  {output<<name<<" NumIt "<<NumIt<<" NumT "<<NumT<<" FuncError "<<FuncError<<" ResError "<<ResError<<" SumFuncMinim "<<SumFuncMinim<<"\n";};
void FAWOptim::Input(istream &input)
  {  char tmp[256]; input>>tmp>>NumIt>>tmp>>NumT>>tmp>>FuncError; 
     if (!seek_eoln(input)) input>>tmp>>ResError>>tmp>>SumFuncMinim;
     else {ResError=FuncError;SumFuncMinim=0;}
  };
void FAWOptim::SaveInPar(ostream &output)
  {output<<"NumIt "<<NumIt<<" NumT "<<NumT<<" FuncError "<<FuncError<<" ResError "<<ResError<<" SumFuncMinim "<<SumFuncMinim<<"\n";
   FAWZer::SaveInPar(output);};
void  FAWOptim::GetInPar(istream &input)
  {char tmp[256];input>>tmp>>NumIt>>tmp>>NumT>>tmp>>FuncError;
   if (!seek_eoln(input)) input>>tmp>>ResError>>tmp>>SumFuncMinim;
   else {ResError=FuncError;SumFuncMinim=0;}
   FAWZer::GetInPar(input);};

void FAWOptim::FAWDo()
{ 
  int Num=Func->GetNumPar(),Good=1;
  NumFuncEval=0;
  CurPtr=this;
  VecCl Par(Num),Stp(Num);
  Ref<MinFind1DClass> min1d=new MinFind1DClass(NULL,ResError,FuncError,1.2,5,5);
  if (HiArr==NULL) {VecCl tmp(XArr[0]);tmp=tmp*0+1;HiArr=tmp.Ptr;tmp.Ptr=NULL;}
  HiArr[0]=XArr[0];YArr[0]=XArr[0];VecCl w,y;w=HiArr;
  y=YArr;
  Cur_Theo_Sum(XArr,y.Ptr);
  Ref<Derive2DClass> deriv=new Derive2DHi2(w,y,Cur_Theo);
  //Ref<Derive2DClass> deriv=new Derive2DStd(new Derive2DHi2(w,y,Cur_Theo));
  Ref<MultiDimMinimizer> minim=new EigenVectMinim(deriv,min1d,1e-8,1e-10,FuncError,2);
  MinFindNDimClass MinNd(minim,ResError);

  for (int NumApp=1;NumApp<=Func->GetNumIter();NumApp++)
  {
    if (NumApp>1) Func->GetNumIter(NumApp);
    cout<<" NumIter: "<<NumApp<<"\n         Hi_After_Iter\n";
    Func->GetParStp(Par.Ptr,Stp.Ptr);
    Code.SetMap(Par,Stp);
    VecCl TmpPar,TmpStp;TmpPar=Code.CodeVec(Par);TmpStp=Code.CodeVec(Stp);
    int Cont=1;
    double HiMis=-1;
    for (int k=1;((k<=NumT)&&(Cont));k++)
      {
       double Misf;
       Cont=MinNd.Execute(TmpPar,TmpStp,NumIt,Misf);
       HiMis=sqrt(Misf)/sqrt(XArr[0]);
       cout<<"     "<<FMT<<HiMis<<" FuncMis "<<FMT<<Misf<<"\n";
     
      }
    VecCl ParMis;
    //if (HiMis==-1)
    double FuncMis;int aaa=NumFuncEval;
      HiMis=sqrt(FuncMis=minim->ParamsCorrelation(TmpPar,TmpStp,ParMis))/sqrt(XArr[0]);
      NumFuncEval=aaa;
    cout<<"\n         ResHi="<<HiMis<<" FuncMis "<<FuncMis<<" NumFEval "<<NumFuncEval<<"\n";
    if (TmpPar[0]!=0)
    { cout<<"         ParamsCorrelation "<<ParMis*HiMis<<"\n"; }
    //{ cout<<"         ParamsCorrelation "<<ParMis*sqrt(sqr(HiMis)*XArr[0])<<"\n"; }
    else cout<<"\n";
    cout.flush();
    Func->SetPar(Code.DeCodeVec(TmpPar).Ptr);
  }
}
void FAWOptim::Cur_Theo_Sum(double *X,double *Y)
{
  if (SumFuncMinim) {for (int k=2;k<=X[0];k++) Y[k]+=Y[k-1];}
}

void FAWOptim::Cur_Theo(VecCl &par,VecCl &res)
{
  VecCl p;p=CurPtr->Code.DeCodeVec(par);
  res=CurPtr->XArr;
  CurPtr->Func->SetPar(p.Ptr);
  CurPtr->Func->Clc(CurPtr->XArr,res.Ptr);
  CurPtr->Cur_Theo_Sum(CurPtr->XArr,res.Ptr);
  CurPtr->NumFuncEval++;
};












#ifndef WCPP_NET
DescribedClass * Dummy_FAWUncMin(){return new FAWUncMin();}
static ClassDesc FAWUncMin_ClassDesc(typeid(FAWUncMin),"UncMin_appr_clc",1,"FAWZer",Dummy_FAWUncMin);
#else
static ClassDesc FAWUncMin_ClassDesc(typeid(FAWUncMin),"UncMin_appr_clc",1,"FAWZer",create<FAWUncMin>);
#endif

FAWUncMin::FAWUncMin():FAWOptim(),Min1D_Coef(1.2),Min1D_MaxLinSrch(5),Min1D_MaxRetry(5)
{
   delete name;name=strdup("UncMin_appr_clc");Minimizer=new LMMinim();
};

void FAWUncMin::Output(ostream &output)
{
  FAWOptim::Output(output);//AutoStoreClass::Write(output,Minimizer);
  SavableClass::Save(output, Minimizer, SavableClass::SimpleEdit);
  output<<" { ";
  output<<" Min1D_Coef "<<Min1D_Coef<<" Min1D_MaxLinSrch "<<Min1D_MaxLinSrch
      <<" Min1D_MaxRetry "<<Min1D_MaxRetry<<" }\n";;
}

void FAWUncMin::Input(istream &input)
{
  FAWOptim::Input(input);
  Minimizer << SavableClass::Read(input, SavableClass::SimpleEdit);
  //Minimizer=dynamic_cast<MultiDimMinimizer*>(AutoStoreClass::Read(input));
  char tmp[256];input>>tmp;
  input>>tmp>>Min1D_Coef>>tmp>>Min1D_MaxLinSrch>>tmp>>Min1D_MaxRetry>>tmp;
}

void FAWUncMin::SaveInPar(ostream &output)
{
  FAWOptim::SaveInPar(output);//AutoStoreClass::Write(output,Minimizer);
  SavableClass::Save(output, Minimizer, SavableClass::SimpleEdit);
  output<<" { ";
  output<<" Min1D_Coef "<<Min1D_Coef<<" Min1D_MaxLinSrch "<<Min1D_MaxLinSrch
      <<" Min1D_MaxRetry "<<Min1D_MaxRetry<<" }\n";;
}
void  FAWUncMin::GetInPar(istream &input)
{
  FAWOptim::GetInPar(input);//Minimizer=dynamic_cast<MultiDimMinimizer*>(AutoStoreClass::Read(input));
  Minimizer << SavableClass::Read(input, SavableClass::SimpleEdit);
  char tmp[256];input>>tmp;
  input>>tmp>>Min1D_Coef>>tmp>>Min1D_MaxLinSrch>>tmp>>Min1D_MaxRetry>>tmp;
}


double FAWUncMin::FuncClc(double *x,int n)
{ 
  VecCl Par(n);for (int k=1;k<=n;k++) Par[k]=x[k-1];
  FAWUncMin *cur=dynamic_cast<FAWUncMin*>(CurPtr);
  if (cur==NULL) return 0;
  double ret=cur->Deriv2D->Execute(Par);
cout<<" Res "<<ret<<" Par "<<Par;
  return ret;
};
#include "mat\quad.h"
void FAWUncMin::FAWDo()
{ 
  int Num=Func->GetNumPar(),Good=1;
  NumFuncEval=0;
  CurPtr=this;
  VecCl Par(Num),Stp(Num);
//  Ref<MinFind1DClass> min1d=new MinFind1DClass(NULL,ResError,M_MinDouble2,5,5);
  Ref<MinFind1DClass> min1d=new MinFind1DClass(NULL,ResError,FuncError,
                               Min1D_Coef,Min1D_MaxLinSrch,Min1D_MaxRetry);
  if (HiArr==NULL) {VecCl tmp(XArr[0]);tmp=tmp*0+1;HiArr=tmp.Ptr;tmp.Ptr=NULL;}
  HiArr[0]=XArr[0];YArr[0]=XArr[0];VecCl w,y;w=HiArr;
  y=YArr;
  Cur_Theo_Sum(XArr,y.Ptr);
  Ref<Derive2DClass> deriv =new Derive2DHi2(w,y,Cur_Theo);
  //Ref<Derive2DClass> deriv=new Derive2DStd(new Derive2DHi2(w,y,Cur_Theo));
  //LambdaData Lambda(10,20,0.5);
  //Ref<LMMinim> minim=new LMMinim(deriv,min1d,1e-8,1e-10,FuncError,Lambda);
  Minimizer->SetFunc(min1d,deriv);
  MinFindNDimClass MinNd(Minimizer,ResError);

  for (int NumApp=1;NumApp<=Func->GetNumIter();NumApp++)
  {
    if (NumApp>1) Func->GetNumIter(NumApp);
    cout<<" NumIter: "<<NumApp<<"\n         Hi_After_Iter\n";
    Func->GetParStp(Par.Ptr,Stp.Ptr);
    Code.SetMap(Par,Stp);
    VecCl TmpPar,TmpStp;TmpPar=Code.CodeVec(Par);TmpStp=Code.CodeVec(Stp);
    int Cont=1;
    double HiMis=-1;
    for (int k=1;((k<=NumT)&&(Cont));k++)
      {
       double Misf;
       Cont=MinNd.Execute(TmpPar,TmpStp,NumIt,Misf);
       //minim->Lambda=Lambda;
       HiMis=sqrt(Misf)/sqrt(XArr[0]);
       cout<<"     "<<FMT<<HiMis<<" FuncMis "<<FMT<<Misf<<"\n";
     
      }
    VecCl ParMis;
    //if (HiMis==-1)
    double FuncMis;int aaa=NumFuncEval;
      HiMis=sqrt(FuncMis=Minimizer->ParamsCorrelation(TmpPar,TmpStp,ParMis))/sqrt(XArr[0]);
      NumFuncEval=aaa;
    cout<<"\n         ResHi="<<HiMis<<" FuncMis "<<FuncMis<<" NumFEval "<<NumFuncEval<<"\n";
    if (TmpPar.Dim()!=0)
    { cout<<"         ParamsCorrelation "<<ParMis*HiMis<<"\n"; }
    //{ cout<<"         ParamsCorrelation "<<ParMis*sqrt(sqr(HiMis)*XArr[0])<<"\n"; }
    else cout<<"\n";
    cout.flush();
    Func->SetPar(Code.DeCodeVec(TmpPar).Ptr);
  }
}








//  void FormStpDir(vector<MatrCl> &D2Vec,VecCl &Delta,double D0,MatrCl &StpDir,VecCl &StpEst,VecCl &StpOrder)
//  {
//    int N=D2Vec.size();
//    vector<MatrCl> StpDirK(N);
//    vector<VecCl> StpEstK(N),StpOrderK(N);
//    vector<IndexedVal> MatrOrder(N);
//    double MatrPrior;
//    for (int k=0;k<N;k++)
//    {
//       MatrPrior=FormStpDir(D2Vec[k],Delta,D0,StpDirK[k],StpEstK[k],StpOrderK[k]);
//       if (k==N-1) MatrPrior=-1;
//       MatrOrder[k]=IndexedVal(k,-MatrPrior);
//    }
//    sort(MatrOrder.begin(),MatrOrder.end());
//    StpDir.Dim(0);StpEst.Dim(0);StpOrder.Dim(0);
//    for (int k=0;k<N;k++)
//    {
//      int k1=MatrOrder[k].ValInd();
//      AppendStp(StpDirK[k1],StpEstK[k1],StpOrderK[k1],StpDir,StpEst,StpOrder);
//    }
////cout<<" StpDir\n"<<StpDir;
//  }
//  void AppendStp(MatrCl &StpDirK,VecCl &StpEstK,VecCl &StpOrderK,
//                 MatrCl &StpDir,VecCl &StpEst,VecCl &StpOrder)
//  {
//    int StpRow,StpCol;StpDir.GetDimMN(StpRow,StpCol);
//    int StpKRow,StpKCol;StpDirK.GetDimMN(StpKRow,StpKCol);
//    MatrCl StpDir1(StpKRow,StpCol+StpKCol);
//    VecCl StpEst1(StpCol+StpKCol),StpOrder1(StpCol+StpKCol);
//    for (int k=1;k<=StpCol;k++)
//    {
//      SetCol(StpDir1,k,GetCol(StpDir,k));
//      StpEst1[k]=StpEst[k];StpOrder1[k]=StpOrder[k];
//    }
//    for (int k=StpCol+1;k<=StpCol+StpKCol;k++)
//    {
//      int k1=k-StpCol;
//      SetCol(StpDir1,k,GetCol(StpDirK,k1));
//      StpEst1[k]=StpEstK[k1];StpOrder1[k]=StpOrderK[k1]+StpCol;
//    }
//    StpDir=StpDir1;StpEst=StpEst1;StpOrder=StpOrder1;
////cout<<" StpDir\n"<<StpDir;
//  }










//===========================================================
//===================   FAWIter    ==========================
//===========================================================
/*
void FAWIter::FAWDo()
  {
   double *XParamPnt=new double[MaxParPnt];
   double *YParamPnt=new double[MaxParPnt],tmp[3];
   long NumXPP=0;
   fstream InXPP(FileAppr,ios::in);
   while (!seek_eof(InXPP))
     {
      if (EnterStr(InXPP,&tmp[0],2))
        { NumXPP++;XParamPnt[NumXPP]=tmp[1];YParamPnt[NumXPP]=tmp[2];}
     }
   if (NumXPP<=1) { cout<<"No data points in input file \n";return;}
   XParamPnt[0]=NumXPP;YParamPnt[0]=NumXPP;
   InXPP.close();
//   Entered First Approscimation from external file

//   double *CurGetFunc=new double [NumXPP+1];CurGetFunc[0]=NumXPP;
   int NumPar=Func->GetNumPar();
   double *Par=new double [NumPar+1];Func->GetPar(Par);
   double *CurDelFunc=new double [XArr[0]+1];CurDelFunc[0]=XArr[0];
   double *FuncPnt=new double [XArr[0]+1];FuncPnt[0]=XArr[0];

   int XToChg;
   long FstNum=NumIt/10;
   if (FstNum>100) FstNum=100;
   FstNum=NumT;
   NumT=(long)(XArr[0]/log(NumIt/FstNum));
   double DeltaAmp,Shift;
//   double *TmpPar=new double[Num+1],*TmpStp=new double[Num+1];
//   Func->GetParStp(TmpPar,TmpStp);TmpPar[0]=Num;TmpStp[0]=Num;

   randomize();
   long k;
   for (long i=1;i<=NumT;i++)
     {
//      SmoothYParam(XParamPnt,YParamPnt,Par,FuncPnt);
      FormCurFunc(CurDelFunc,XParamPnt,YParamPnt,Par,FuncPnt);
double s=0;for (k=2;k<NumXPP;k++) 
  s+=sqr(YParamPnt[k-1]+YParamPnt[k+1]-2*YParamPnt[k]);
cout<<" Hi "<<s<<"\n";
      for (k=1;k<=NumIt;k++)
        {
         XToChg=(int)((double)rand()/RAND_MAX*(NumXPP-1))+1;
         if ((XToChg<1)||(XToChg>NumXPP)) {cout<<"EERRRROORR rand";XToChg=1;ChRead();}

//         double tmp,tmp1;
//         tmp=MulFunc(CurDelFunc,XParamPnt[XToChg],FuncPnt,Par);

         DeltaAmp=MulFunc(CurDelFunc,XParamPnt[XToChg],FuncPnt,Par);
//                       Just smoothing
         Shift=FormShift(YParamPnt,XToChg);
         DeltaAmp=1/(double)(FstNum+k)*(DeltaAmp-SmoothPar*Shift);

         AddDelta(CurDelFunc,DeltaAmp,YParamPnt[XToChg],FuncPnt);
//         tmp1=MulFunc(CurDelFunc,XParamPnt[XToChg],FuncPnt,Par);
//         if ((k*tmp1-tmp*(k-1))>1e-10) 
//           cout<<k*tmp1-tmp*(k-1)<<"  "<<tmp<<"   "<<tmp1<<"\n";
//cout<<XParamPnt[XToChg]<<"   "<<DeltaAmp<<"\n";ChRead();
        }
     }
   fstream OutXPP(FileAppr,ios::out);
   for (k=1;k<=NumXPP;k++) OutXPP<<XParamPnt[k]<<"    "<<YParamPnt[k]<<"\n";
   
   fstream Out("!!tmp.dat",ios::out);
   double *CurDelFunc1=new double [XArr[0]+1];CurDelFunc1[0]=XArr[0];
   FormCurFunc(CurDelFunc1,XParamPnt,YParamPnt,Par,FuncPnt);
   for (k=1;k<=XArr[0];k++) 
     Out<<XArr[k]<<"      "<<YArr[k]<<"       "<<CurDelFunc[k]
                                    <<"       "<<CurDelFunc1[k]<<"\n";
   delete CurDelFunc1;

   delete XParamPnt;delete YParamPnt;
   delete CurDelFunc;delete Par;delete FuncPnt;
//   delete TmpPar;delete TmpStp;
  };
double FAWIter::FormShift(double *YParamPnt,int k)
  {
   double Shift;
   if (k==1) Shift=YParamPnt[1]-2*YParamPnt[2]+YParamPnt[3];
   else if (k==2) Shift=-2*YParamPnt[1]+5*YParamPnt[2]-4*YParamPnt[3]+YParamPnt[4];
     else if (k==YParamPnt[0]) Shift=YParamPnt[k-2]-2*YParamPnt[k-1]+YParamPnt[k];
       else if (k==YParamPnt[0]-1) Shift=YParamPnt[k-2]-4*YParamPnt[k-1]+5*YParamPnt[k]-2*YParamPnt[k+1];
        else Shift=YParamPnt[k-2]+YParamPnt[k+2]-4*(YParamPnt[k-1]+YParamPnt[k+1])+6*YParamPnt[k];
   return Shift;
  };
void FAWIter::FormCurFunc(double *CurDelFunc,double *XParamPnt,
                          double *YParamPnt,double *Par,double *FuncPnt)
  {
   int NumPnt=XArr[0];
   movmem(YArr,CurDelFunc,sizeof(double)*(NumPnt+1));
   double Coef;
   for (int k=1;k<=XParamPnt[0];k++)
     {
      Par[NParam]=XParamPnt[k];Func->SetPar(Par);
      Func->Clc(XArr,FuncPnt);
      Coef=YParamPnt[k];
      for (int k1=1;k1<=NumPnt;k1++)  CurDelFunc[k1]-=Coef*FuncPnt[k1];
     }
  };

double FAWIter::MulFunc(double *CurDelFunc,double NewParam,double *FuncPnt,
                        double *Par)
  {
   Par[NParam]=NewParam;Func->SetPar(Par);
   double sum=0,sum1=0;
   Func->Clc(XArr,FuncPnt);
   for (int k=1;k<=CurDelFunc[0];k++)
      {sum+=CurDelFunc[k]*FuncPnt[k];sum1+=FuncPnt[k]*FuncPnt[k];};
//cout<<"MULL "<<sum<<" sum1 "<<sum1<<"\n";
   return sum/sum1;
  };
void FAWIter::AddDelta(double *CurDelFunc,double DeltaAmp,double &AmpOld,
                         double *FuncPnt)
  {
   if (AmpOld+DeltaAmp<0) DeltaAmp=-AmpOld;
   
   for (int k=1;k<=CurDelFunc[0];k++)
     CurDelFunc[k]-=DeltaAmp*FuncPnt[k];
   AmpOld+=DeltaAmp;
  };


void FAWIter::Output(fstream &output)
  { output<<name<<"   NumTimes  "<<NumT<<"   NumIt  "<<NumIt<<" NParam  "
          <<NParam <<" SmoothPar "<<SmoothPar<<" FileAppr "<<FileAppr<<"\n";};
void FAWIter::Input(fstream &input)
  {
   char tmp[256];
   input>>tmp>>NumT>>tmp>>NumIt>>tmp>>NParam>>tmp>>SmoothPar>>tmp>>FileAppr;
  };
*/
/*
void FAWIter::SmoothYParam(double *XParamPnt,double *YParamPnt,
                           double *Par,double *FuncPnt)
  {
   double Mean;
   for (int k=1;k<=XParamPnt[0]-2;k++)
     {
      Mean=(YParamPnt[k]+YParamPnt[k+1]+YParamPnt[k+2])/3;
      YParamPnt[k]=Mean;YParamPnt[k+1]=Mean;YParamPnt[k+2]=Mean;
     }
  };
*/
//int FAWRegister(fstream &in,FAWZer *&WDo)
//  {
//   char name[256]; in>>name;
//   FAWZer *Func;
//   int ret=1;
//   if (!Stricmp(name,"std_appr_clc")) {Func=(FAWZer*)new FAWStand;}
//   else 
////     if (!Stricmp(name,"iter_appr_clc")) {Func=(FAWZer*)new FAWIter;}
////     else 
//       ret=0;
//   if (!ret)  { cout<<" There is no such work "<<name<<"\n";  }
//   else
//      {
//       if (WDo!=NULL) delete WDo;
//       WDo=Func;
//       Func->Input(in);
//      }
////cout<<" Mem after FAZerIORegister  "<<coreleft()<<"\n";
//   return ret;
//  };


/*

void FAWIter::FAWDo()
  {
   double *XParamPnt=new double[MaxParPnt];
   double *YParamPnt=new double[MaxParPnt],tmp[3];
   long NumXPP=0;
   fstream InXPP(FileAppr,ios::in);
   while (!seek_eof(InXPP))
     {
      if (EnterStr(InXPP,&tmp[0],2))
        { NumXPP++;XParamPnt[NumXPP]=tmp[1];YParamPnt[NumXPP]=tmp[2];}
     }
   if (NumXPP<=1) { cout<<"No data points in input file \n";return;}
   XParamPnt[0]=NumXPP;YParamPnt[0]=NumXPP;
   InXPP.close();
//   Entered First Approscimation from external file

//   double *CurGetFunc=new double [NumXPP+1];CurGetFunc[0]=NumXPP;
   int NumPar=Func->GetNumPar();
   double *Par=new double [NumPar+1];Func->GetPar(Par);
   double *CurDelFunc=new double [XArr[0]+1];CurDelFunc[0]=XArr[0];
   double *FuncPnt=new double [XArr[0]+1];FuncPnt[0]=XArr[0];

   int XToChg;
   long FstNum=NumIt/10;
   if (FstNum>100) FstNum=100;
   FstNum=NumT;
   NumT=(long)(XArr[0]/log(NumIt/FstNum));
   double DeltaAmp,Shift;
//   double *TmpPar=new double[Num+1],*TmpStp=new double[Num+1];
//   Func->GetParStp(TmpPar,TmpStp);TmpPar[0]=Num;TmpStp[0]=Num;

   randomize();
   long k;
   for (long i=1;i<=NumT;i++)
     {
//      SmoothYParam(XParamPnt,YParamPnt,Par,FuncPnt);
      FormCurFunc(CurDelFunc,XParamPnt,YParamPnt,Par,FuncPnt);
double s=0;for (k=2;k<NumXPP;k++) 
  s+=sqr(YParamPnt[k-1]+YParamPnt[k+1]-2*YParamPnt[k]);
cout<<" Hi "<<s<<"\n";
      for (k=1;k<=NumIt;k++)
        {
         XToChg=(int)((double)rand()/RAND_MAX*(NumXPP-1))+1;
         if ((XToChg<1)||(XToChg>NumXPP)) {cout<<"EERRRROORR rand";XToChg=1;ChRead();}

//         double tmp,tmp1;
//         tmp=MulFunc(CurDelFunc,XParamPnt[XToChg],FuncPnt,Par);

         DeltaAmp=MulFunc(CurDelFunc,XParamPnt[XToChg],FuncPnt,Par);
//                       Just smoothing
         Shift=FormShift(YParamPnt,XToChg);
         DeltaAmp=1/(double)(FstNum+k)*(DeltaAmp-SmoothPar*Shift);

         AddDelta(CurDelFunc,DeltaAmp,YParamPnt[XToChg],FuncPnt);
//         tmp1=MulFunc(CurDelFunc,XParamPnt[XToChg],FuncPnt,Par);
//         if ((k*tmp1-tmp*(k-1))>1e-10) 
//           cout<<k*tmp1-tmp*(k-1)<<"  "<<tmp<<"   "<<tmp1<<"\n";
//cout<<XParamPnt[XToChg]<<"   "<<DeltaAmp<<"\n";ChRead();
        }
     }
   fstream OutXPP(FileAppr,ios::out);
   for (k=1;k<=NumXPP;k++) OutXPP<<XParamPnt[k]<<"    "<<YParamPnt[k]<<"\n";
   
   fstream Out("!!tmp.dat",ios::out);
   double *CurDelFunc1=new double [XArr[0]+1];CurDelFunc1[0]=XArr[0];
   FormCurFunc(CurDelFunc1,XParamPnt,YParamPnt,Par,FuncPnt);
   for (k=1;k<=XArr[0];k++) 
     Out<<XArr[k]<<"      "<<YArr[k]<<"       "<<CurDelFunc[k]
                                    <<"       "<<CurDelFunc1[k]<<"\n";
   delete CurDelFunc1;

   delete XParamPnt;delete YParamPnt;
   delete CurDelFunc;delete Par;delete FuncPnt;
//   delete TmpPar;delete TmpStp;
  };
double FAWIter::FormShift(double *YParamPnt,int k)
  {
   double Shift;
   if (k==1) Shift=YParamPnt[1]-2*YParamPnt[2]+YParamPnt[3];
   else if (k==2) Shift=-2*YParamPnt[1]+5*YParamPnt[2]-4*YParamPnt[3]+YParamPnt[4];
     else if (k==YParamPnt[0]) Shift=YParamPnt[k-2]-2*YParamPnt[k-1]+YParamPnt[k];
       else if (k==YParamPnt[0]-1) Shift=YParamPnt[k-2]-4*YParamPnt[k-1]+5*YParamPnt[k]-2*YParamPnt[k+1];
        else Shift=YParamPnt[k-2]+YParamPnt[k+2]-4*(YParamPnt[k-1]+YParamPnt[k+1])+6*YParamPnt[k];
   return Shift;
  };
void FAWIter::FormCurFunc(double *CurDelFunc,double *XParamPnt,
                          double *YParamPnt,double *Par,double *FuncPnt)
  {
   int NumPnt=XArr[0];
   movmem(YArr,CurDelFunc,sizeof(double)*(NumPnt+1));
   double Coef;
   for (int k=1;k<=XParamPnt[0];k++)
     {
      Par[NParam]=XParamPnt[k];Func->SetPar(Par);
      Func->Clc(XArr,FuncPnt);
      Coef=YParamPnt[k];
      for (int k1=1;k1<=NumPnt;k1++)  CurDelFunc[k1]-=Coef*FuncPnt[k1];
     }
  };

double FAWIter::MulFunc(double *CurDelFunc,double NewParam,double *FuncPnt,
                        double *Par)
  {
   Par[NParam]=NewParam;Func->SetPar(Par);
   double sum=0,sum1=0;
   Func->Clc(XArr,FuncPnt);
   for (int k=1;k<=CurDelFunc[0];k++)
      {sum+=CurDelFunc[k]*FuncPnt[k];sum1+=FuncPnt[k]*FuncPnt[k];};
//cout<<"MULL "<<sum<<" sum1 "<<sum1<<"\n";
   return sum/sum1;
  };
void FAWIter::AddDelta(double *CurDelFunc,double DeltaAmp,double &AmpOld,
                         double *FuncPnt)
  {
   if (AmpOld+DeltaAmp<0) DeltaAmp=-AmpOld;
   
   for (int k=1;k<=CurDelFunc[0];k++)
     CurDelFunc[k]-=DeltaAmp*FuncPnt[k];
   AmpOld+=DeltaAmp;
  };


void FAWIter::Output(fstream &output)
  { output<<name<<"   NumTimes  "<<NumT<<"   NumIt  "<<NumIt<<" NParam  "
          <<NParam <<" SmoothPar "<<SmoothPar<<" FileAppr "<<FileAppr<<"\n";};
void FAWIter::Input(fstream &input)
  {
   char tmp[256];
   input>>tmp>>NumT>>tmp>>NumIt>>tmp>>NParam>>tmp>>SmoothPar>>tmp>>FileAppr;
  };
*/
