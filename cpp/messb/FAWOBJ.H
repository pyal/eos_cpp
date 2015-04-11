
#ifndef __FAWOBJ_H
#define __FAWOBJ_H

//#include <math.h>
//#include <stdlib.h>
//#include <conio.h>
//#include <fstream.h>
//#include <iomanip.h>
//#include <alloc.h>
//#include <dir.h>
//#include <string.h>
//#include <iostream.h>
//#include "util.h"
//#include <PROCESS.H>
#include "faobj.h"
#include "mat\eigen_ma.h"
#include "mat\myappr.h"
#include "mat\matrics.h"
#include "lib\ref\class.h"
#include "mat\ndimmin.h"


   

struct FAWZer:DescribedClass
  {
   char   *name;
   double *XArr;
   double *YArr;
   double *HiArr;
   FAIO   *Func;

   
   virtual void FAWDo()=0;
   virtual void Output(ostream &output)=0;
   virtual void Input(istream &input)=0;

   virtual void SaveInPar(ostream &output) { if (Func) Func->SaveInPar(output);};
   virtual void  GetInPar(istream &input)  { if (Func) Func->GetInPar(input);
      };
    
   virtual void SetFuncPnt(FAIO *func,double *xarr,double *yarr,double *hiarr)
      {Func=func;XArr=xarr;YArr=yarr;HiArr=hiarr;
      };
   virtual void GetFuncPnt(FAIO *&func,double *&xarr,double *&yarr)
      { func=Func;xarr=XArr;yarr=YArr;
       };
   virtual void FAWZerDelete()
      { 
        delete Func;delete XArr;delete YArr;delete HiArr;
        XArr=NULL;YArr=NULL;Func=NULL;HiArr=NULL;
        delete name;name=NULL;
      };

   FAWZer():name(NULL),XArr(NULL),YArr(NULL),Func(NULL),HiArr(NULL){};
   virtual ~FAWZer(){FAWZerDelete();};
   virtual void DrawHelp(ostream &out)
   { out<<" Function is created by string\n"<<name<<" "; };
   static void list_all_functions(ostream &out);
  };

struct FAWStand:FAWZer
  {
   long NumIt,NumT;
   double MinError;

   int NumFuncEval;
   FAWStand();
   //~FAWStand(){};
   void FAWDo();
   static FAWStand* CurPtr;
   static double Hi_Func(double *par);
   static void Get_Theo(double *par,double *res);

   void Output(ostream &output);
   void Input(istream &input);

   void SaveInPar(ostream &output)
     {output<<"NumIt "<<NumIt<<" NumT "<<NumT<<" MinError "<<MinError<<"\n";
      FAWZer::SaveInPar(output);
     };
   void  GetInPar(istream &input)
     {char tmp[256];
      input>>tmp     >>NumIt>>tmp     >>NumT>>tmp     >>MinError;
      FAWZer::GetInPar(input);
     };
   void DrawHelp(ostream &out)
   {
     FAWZer::DrawHelp(out);
     NumIt=3;NumT=3;MinError=1e-6;
     SaveInPar(out);
     out<<"   This is an old algorithm,\n NumT  - times is generating 2-order\n";
     out<<"   matrics of deriv ( N function calculations are used fof this )\n";
     out<<" NumIt - times for each 2D deriv calulates parameters estimations\n";
     out<<" MinError - of the order 1e-6\n";
   };
  };


struct FAWOptim:FAWZer
  {
   long NumIt,NumT;
   double FuncError,ResError;
   int SumFuncMinim;

   int NumFuncEval;
   FAWOptim();
   void FAWDo();
   static void Cur_Theo(VecCl &par,VecCl &res);

   void Output(ostream &output);
   void Input(istream &input);

   void SaveInPar(ostream &output);
   void  GetInPar(istream &input);
   void DrawHelp(ostream &out)
   {
     FAWZer::DrawHelp(out);
     NumIt=1;NumT=6;FuncError=M_Eps2;ResError=FuncError;
     SaveInPar(out);
     out<<"   A new algorithm,\n NumT  - times is generating 2-order";
     out<<"   matrics of deriv \n( N function calculations are used for this )\n";
     out<<" NumIt - have to be 1\n";
     out<<" FuncError - Function evaluation error have to be at least "<<M_Eps2<<"\n";
     out<<"             if it is less - will be used a standart machine error\n";
     out<<" ResError  - Stoping iterations after step is less then have to be at least "<<M_Eps2<<"\n";
     out<<"             if it is less - will be used a standart machine error\n";
   };
  protected:
   MapVars Code;
   static FAWOptim* CurPtr;
   void Cur_Theo_Sum(double *X,double *Y);

  };

struct FAWUncMin:FAWOptim
  {
   void Output(ostream &output);
   void Input(istream &input);

   void SaveInPar(ostream &output);
   void  GetInPar(istream &input);
   FAWUncMin();
   void FAWDo();

   void DrawHelp(ostream &out)
   {
     FAWOptim::DrawHelp(out);
     out<<" This is UncMin algorithm base\n";
     out<<" For Minimizer you can use one of the algorithms\n";
	 out<<SavableClass::HelpForCategory("MDMin").c_str()<<"\n";
     //Ref<MultiDimMinimizer> minim;
     //minim=dynamic_cast<MultiDimMinimizer*>(ClassDesc::list_category_start("MDMin"));
     //int k=0;
     //while (minim) 
     //{
     //  out<<"\n Algorithm <"<<minim->class_name()<<">\n\n";
     //  AutoStoreClass::Write(out,minim);
     //  minim=dynamic_cast<MultiDimMinimizer*>(ClassDesc::list_category_next(minim,"MDMin"));
     //}
   };
  protected:
  static double FuncClc(double *x,int n);
  Ref<MultiDimMinimizer> Minimizer;
  Ref<Derive2DClass> Deriv2D;
  double Min1D_Coef;
  int Min1D_MaxLinSrch,Min1D_MaxRetry;

  };

int FAWRegister(istream &in,FAWZer *&WDo);


#endif






/*
struct FAWMProb:FAWZer
  {
   long NumIt,NumT;
   int NParam;
   double SmoothPar;
   char FileAppr[30];

   FAWIter(){FileAppr[0]=0;NumIt=1000;NumT=1;
             NParam=1;name=strdup("Iter_appr_clc"); };
   ~FAWIter(){FAWZerDelete();};
   void FAWDo();
   void FormCurFunc(double *CurDelFunc,double *XParamPnt,
                             double *YParamPnt,double *Par,double *FuncPnt);
   double MulFunc(double *CurDelFunc,double NewParam,double *FuncPnt,
                                                     double *Par);
   void AddDelta(double *CurDelFunc,double DeltaAmp,double &AmpOld,
                                    double *FuncPnt);
//   void SmoothYParam(double *XParamPnt,double *YParamPnt,
//                                       double *Par,double *FuncPnt);
   double FAWIter::FormShift(double *YParamPnt,int k);

   void Output(fstream &output);
   void Input(fstream &input);

   void SaveInPar(fstream &output)
     {output<<"NumTimes  "<<NumT<<"  NumIt "<<NumIt<<" NParam "
          <<NParam <<" SmoothPar "<<SmoothPar<<" FileAppr "<<FileAppr<<"\n";
      FAWZer::SaveInPar(output);
     };
   void  GetInPar(fstream &input)
     {char tmp[256];
      input>>tmp>>NumT>>tmp>>NumIt>>tmp>>NParam>>tmp>>SmoothPar>>tmp>>FileAppr;
      FAWZer::GetInPar(input);
     };             
  };
*/



/*
struct FAWIter:FAWZer
  {
   long NumIt,NumT;
   int NParam;
   double SmoothPar;
   char FileAppr[30];

   FAWIter(){FileAppr[0]=0;NumIt=1000;NumT=1;
             NParam=1;name=strdup("Iter_appr_clc"); };
   ~FAWIter(){FAWZerDelete();};
   void FAWDo();
   void FormCurFunc(double *CurDelFunc,double *XParamPnt,
                             double *YParamPnt,double *Par,double *FuncPnt);
   double MulFunc(double *CurDelFunc,double NewParam,double *FuncPnt,
                                                     double *Par);
   void AddDelta(double *CurDelFunc,double DeltaAmp,double &AmpOld,
                                    double *FuncPnt);
//   void SmoothYParam(double *XParamPnt,double *YParamPnt,
//                                       double *Par,double *FuncPnt);
   double FAWIter::FormShift(double *YParamPnt,int k);

   void Output(fstream &output);
   void Input(fstream &input);

   void SaveInPar(fstream &output)
     {output<<"NumTimes  "<<NumT<<"  NumIt "<<NumIt<<" NParam "
          <<NParam <<" SmoothPar "<<SmoothPar<<" FileAppr "<<FileAppr<<"\n";
      FAWZer::SaveInPar(output);
     };
   void  GetInPar(fstream &input)
     {char tmp[256];
      input>>tmp>>NumT>>tmp>>NumIt>>tmp>>NParam>>tmp>>SmoothPar>>tmp>>FileAppr;
      FAWZer::GetInPar(input);
     };             
  };

*/
