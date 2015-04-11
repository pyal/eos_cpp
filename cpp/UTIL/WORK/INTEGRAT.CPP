
//   For heat transfer, simple integral over region
#define MaxD2Points 256
#include "spline.h"
//#include "minfunc.h"
#include "util.h"
#include "zero.h"
#include "fstream.h"
#include "process.h"
#include <math.h>

#define PNT Sum.Matter
//#define MATTERSNAME "matter.cfg"

//typedef double D2Array [2][MaxD2Points];
double DencDiscr,StndErr=1e-6;

struct ExternConst
  {
   int Num;
   double *XVal;
   double *Cv;
   double *CvSpline1,*CvSpline2,*CvSpline3;
   double *Rc;
   double *RcSpline1,*RcSpline2,*RcSpline3;
   ExternConst(char *name) {GetFile(name);};

   ExternConst()
     {Num=0;XVal=NULL;Cv=NULL;CvSpline1=NULL;CvSpline2=NULL;CvSpline3=NULL;
                      Rc=NULL;RcSpline1=NULL;RcSpline2=NULL;RcSpline3=NULL;};
   double GetCv(double x)
      { 
       double ret= Seval(Num,x,XVal,Cv,CvSpline1,CvSpline2,CvSpline3);  
//       cout<<" trans_coef "<<ret;
       return ret;
      };
   double GetRc(double x)
      { 
       double ret= Seval(Num,x,XVal,Rc,RcSpline1,RcSpline2,RcSpline3);  
//       cout<<" trans_coef "<<ret;
       return ret;
      };
   void GetFile(char *name);
   ~ExternConst()
     {delete XVal;delete Cv;delete CvSpline1;delete CvSpline2;delete CvSpline3;
      delete Rc;delete RcSpline1;delete RcSpline2;delete RcSpline3;};
  };
void SetDouble(double &x,double val) {if (val==NullNumber ) val=0;x=val;};
void   ExternConst::GetFile(char *name)
  {
   fstream *in=new fstream(name,ios::in);
   double ar[256];
   Num=0;
   while (!seek_eof(*in)) if (EnterStr(*in,ar,255)==3) Num++;
   delete in;in=new fstream(name,ios::in);
   XVal=new double [Num+1];Cv=new double [Num+1];Rc=new double [Num+1];
   int k=0;
   while (!seek_eof(*in)) if (EnterStr(*in,ar,255)==3)
     { k++;XVal[k]=ar[1];SetDouble(Cv[k],ar[2]);SetDouble(Rc[k],ar[3]);}
   delete in;
   CvSpline1=new double [Num+1];CvSpline2=new double [Num+1];
   CvSpline3=new double [Num+1];
   Spline(Num,XVal,Cv,CvSpline1,CvSpline2,CvSpline3);

   RcSpline1=new double [Num+1];RcSpline2=new double [Num+1];
   RcSpline3=new double [Num+1];
   Spline(Num,XVal,Rc,RcSpline1,RcSpline2,RcSpline3);
  };
struct IntegrStruct
  {
   double Lft,Rgt;
   int  NumIter;
   double XStp;
   ExternConst TransConst;

   friend istream& operator>>(istream &input,IntegrStruct &Pnt);
  };

istream& operator>>(istream &input,IntegrStruct &Pnt)
  {
   char tmp[256],subst_name[256];
   input>>tmp>>subst_name;
   input>>tmp>>Pnt.Lft;
   input>>tmp>>Pnt.Rgt;
   input>>tmp>>Pnt.NumIter;
   Pnt.TransConst.GetFile(subst_name);
   Pnt.XStp=(Pnt.Rgt-Pnt.Lft)/Pnt.NumIter;
   return input;
  };

// ===================== Main Body Begin ==================

double Integrate(IntegrStruct &Pnt)
  {
   Pnt.XStp=(Pnt.Rgt-Pnt.Lft)/Pnt.NumIter;
   double cur=Pnt.Lft+Pnt.XStp/2,Sum=0;
   for (int k=1;k<=Pnt.NumIter;k++) 
     {Sum+=Pnt.TransConst.GetCv(cur)*Pnt.XStp;cur+=Pnt.XStp; }
   return Sum;
  };

IntegrStruct CurPnt1,CurPnt2;
double HeatEvaporation;
double Func(double x)
  {
   CurPnt1.Rgt=x;CurPnt2.Lft=x;
   double res1=Integrate(CurPnt1);
   double res2=Integrate(CurPnt2);
   cout<<" X "<<x<<" Delta "<<DencDiscr*res1-res2-HeatEvaporation<<"\n";
   return (DencDiscr*res1-res2-HeatEvaporation );
  };
   
// ========================Input Output ===========================

int ControlArgv(int argc,char *argv[])
  {
   if (argc!=2) { cout<<"Wrong Number of Parameters";return 0; }
   return 1;
  };
// ===============Main =================
void main(int argc,char *argv[])
  {
   IntegrStruct Pnt1,Pnt2;
   char tmp[256];
//   D2Array Midue1, Midue2;
   if ( !ControlArgv(argc,argv) ) 
     {cout<<" Use : Integrat cfg_file\n"<<
            "       cfg_file       : HeatEvapor 0 DencDiscr 0 Pnt1 Pnt2/n"<<
            "       Pnt1           : /n"<<
            "                        File_Func_Data    pb650_cv.dat/n"<<
            "                        Lft_Point         4900/n"<<
            "                        Rgt_Point         10860/n"<<
            "                        Number_Iterations 100 /n"<<
            "       File_Func_Data : Temtperature Denc*Cv Denc/n"<<
            "       if DencDisc=0  : then will recalc masses to be equal"<<
            "                        in the finit point";
      exit(0);
     }
   fstream in(argv[1],ios::in);
   in>>tmp>>HeatEvaporation;
   in>>tmp>>DencDiscr;
   in>>CurPnt1>>CurPnt2;

   double x;
//double stp=(CurPnt2.Rgt-CurPnt1.Lft)/100;x=CurPnt1.Lft;
//cout<<x<<"    "<<CurPnt2.Rgt<<"\n";
//while (x<=CurPnt2.Rgt) {cout<<x<<"    "<<CurPnt1.TransConst.GetRc(x)
//<<"     "<<CurPnt2.TransConst.GetRc(x)<<"\n";x+=stp;}; cout<<"\n";
   if (DencDiscr!=0) 
     {if (DencDiscr<0) 
         DencDiscr=CurPnt1.TransConst.GetRc(CurPnt1.Lft)
                  /CurPnt2.TransConst.GetRc(CurPnt2.Rgt);
cout<<" DencDisc "<<DencDiscr<<"\n";
      x=ZeroIn(CurPnt1.Lft,CurPnt2.Rgt,Func,1e-4);
     }
   else
     {
      DencDiscr=1;
      for (int k=1;k<=10;k++)
        { x=ZeroIn(CurPnt1.Lft,CurPnt2.Rgt,Func,1e-4);
          DencDiscr=CurPnt1.TransConst.GetRc(x)/CurPnt2.TransConst.GetRc(x);
        }
      }
   CurPnt1.Rgt=x;CurPnt2.Lft=x;
   double Mis=DencDiscr*Integrate(CurPnt1)-Integrate(CurPnt2)-HeatEvaporation;
   cout<<" Result Temperature "<<x<<" Misfit "<<Mis<<" DencDiscr "<<DencDiscr
                                                             <<"\n";                  
  };
