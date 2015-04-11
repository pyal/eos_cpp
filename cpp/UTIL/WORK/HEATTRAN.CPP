#define MaxD2Points 256
#include "spline.h"
#include "util.h"
#include "fstream.h"
#include "process.h"
#include <math.h>

#define PNT Sum.Matter
#define MATTERSNAME "matter.cfg"

//typedef double D2Array [2][MaxD2Points];

struct ExternConst
  {
   int Num;
   double *XVal;
   double *YVal;
   double *Spline1,*Spline2,*Spline3;
   ExternConst(char *name) {GetFile(name);};

   ExternConst(){Num=0;XVal=NULL;YVal=NULL;
                 Spline1=NULL;Spline2=NULL;Spline3=NULL;};
   double GetYVal(double x)
      { 
       double ret= Seval(Num,x,XVal,YVal,Spline1,Spline2,Spline3);  
//       cout<<" trans_coef "<<ret;
       return ret;
      };
   void GetFile(char *name);
   ~ExternConst()
     {delete XVal;delete YVal;delete Spline1;delete Spline2;delete Spline3;};
  };
void   ExternConst::GetFile(char *name)
  {
   fstream *in=new fstream(name,ios::in);
   double ar[256];
   Num=0;
   while (!seek_eof(*in)) if (EnterStr(*in,ar,255)==2) Num++;
   delete in;in=new fstream(name,ios::in);
   XVal=new double [Num+1];YVal=new double [Num+1];
   int k=0;
   while (!seek_eof(*in)) if (EnterStr(*in,ar,255)==2)
     { k++;XVal[k]=ar[1];YVal[k]=ar[2]; }
   delete in;
   Spline1=new double [Num+1];Spline2=new double [Num+1];
   Spline3=new double [Num+1];
   Spline(Num,XVal,YVal,Spline1,Spline2,Spline3);
  };
struct MatterArrStruct
  {
   double Length;
   double LftCoord;
   int  Num;
   double XStp;

   double *Temp;
   double *Flow;       // Dimension Num+1

   double Dencity;
   double HeatCapacity;
   ExternConst TransConst;
   double GetTrans(double Temp)
     { return (1/Dencity*HeatCapacity)*TransConst.GetYVal(Temp);};
   friend ostream& operator<<(ostream &output,MatterArrStruct &Pnt);
   friend istream& operator>>(istream &input,MatterArrStruct &Pnt);
  };

ostream& operator<<(ostream &output,MatterArrStruct &Pnt)
  {
   double cur=Pnt.LftCoord+Pnt.XStp/2;
   for (int k=1;k<=Pnt.Num;k++)
     { output<<cur<<"         "<<Pnt.Temp[k]<<"\n";cur+=Pnt.XStp; }
   return output;
  };

istream& operator>>(istream &input,MatterArrStruct &Pnt)
  {
   char tmp[256],subst[256];
   double Temp;
   input>>tmp>>subst;
   input>>tmp>>Pnt.Length;
   input>>tmp>>Pnt.Num;
   input>>tmp>>Temp;
   Pnt.Temp=new double [Pnt.Num+1];
   Pnt.Flow=new double [Pnt.Num+2];
   for (int k=1;k<=Pnt.Num;k++)  Pnt.Temp[k]=Temp;
   fstream in(MATTERSNAME,ios::in);
   int Found=0;
   while ( (in) && (!Found) ) { in>>tmp;if (strncmp(tmp,subst,255)==0) Found=1;}
   if (!Found) { cout<<" Cannot Find Substance "<<subst<<"\n";exit(0);}
   in>>Pnt.Dencity>>Pnt.HeatCapacity>>subst;
   Pnt.TransConst.GetFile(subst);
   Pnt.LftCoord=0;
   Pnt.XStp=Pnt.Length/Pnt.Num;
   return input;
  };

struct MatterSumStruct
  {
   int Num;
   double Time;
   double TimeStab;
   double EndTime;
   double WriteTime;

   MatterArrStruct *Matter;
   friend ostream& operator<<(ostream &output,MatterSumStruct &Sum);
   friend istream& operator>>(istream &input,MatterSumStruct &Sum);
  };

ostream& operator<<(ostream &output,MatterSumStruct &Sum)
  {
   if (fmod(Sum.Time,Sum.WriteTime) <10*MathZer)
     { for (int k=1;k<=Sum.Num;k++) output<<Sum.Matter[k]; }
   return output;
  };
istream& operator>>(istream &input,MatterSumStruct &Sum)
  {
   double CurCoord=0;
   char tmp[256];
   input>>tmp>>Sum.Num;
   Sum.Matter=new MatterArrStruct[Sum.Num+1];
   input>>tmp>>Sum.EndTime;            
   input>>tmp>>Sum.TimeStab;//Coef stab
   input>>tmp>>Sum.WriteTime;
   Sum.Time=0;
   for (int k=1;k<=Sum.Num;k++)
     {
      input>>Sum.Matter[k];
      Sum.Matter[k].LftCoord=CurCoord;
      CurCoord+=Sum.Matter[k].Length;
     }
   return input;
  };

// ===================== Main Body Begin ==================


double HeatFlow(double TransConst1,double TransConst2,
                double Temp1,double Temp2,double HStp1,double HStp2)
  {
   //(Pnt.HeatCapacity*Pnt.Dencity*Pnt.XStp*Pnt.XStp);
   return 2*(Temp1-Temp2)/(HStp1/TransConst1+HStp2/TransConst2);
  };
void FlowArray(MatterArrStruct &Pnt)
  {
   for (int k=2;k<=Pnt.Num;k++)
      Pnt.Flow[k]=HeatFlow(Pnt.GetTrans(Pnt.Temp[k-1]),
                           Pnt.GetTrans(Pnt.Temp[k]),
                           Pnt.Temp[k-1],Pnt.Temp[k],Pnt.XStp,Pnt.XStp);
  };
void FlowBnd(MatterArrStruct &Pnt1,MatterArrStruct &Pnt2,int First)
  {
   if (First)
     {
      Pnt1.Flow[1]=Pnt1.Flow[2];
      Pnt1.Flow[Pnt1.Num+1]=
               HeatFlow(Pnt1.GetTrans(Pnt1.Temp[Pnt1.Num]),
                        Pnt2.GetTrans(Pnt2.Temp[1]),
                        Pnt1.Temp[Pnt1.Num],Pnt2.Temp[1],Pnt1.XStp,Pnt2.XStp);
     }
   else
     {
      Pnt2.Flow[Pnt2.Num+1]=Pnt2.Flow[Pnt2.Num];
      Pnt2.Flow[1]=Pnt1.Flow[Pnt1.Num+1];
     }
  };
void FlowBnd(MatterArrStruct &Pnt1,MatterArrStruct &Pnt2,
                                   MatterArrStruct &Pnt3)
  {
    Pnt2.Flow[1]=Pnt1.Flow[Pnt1.Num+1];
    Pnt2.Flow[Pnt1.Num+1]=
              HeatFlow(Pnt2.GetTrans(Pnt2.Temp[Pnt2.Num]),
                       Pnt3.GetTrans(Pnt3.Temp[1]),
                       Pnt2.Temp[Pnt2.Num],Pnt3.Temp[1],Pnt2.XStp,Pnt3.XStp);
  };
void TempArray(MatterArrStruct &Pnt,double TimeStp)
  {
   double coef=TimeStp/Pnt.XStp;
   for (int k=1;k<=Pnt.Num;k++)
      Pnt.Temp[k]+=(Pnt.Flow[k]-Pnt.Flow[k+1])*coef;
  };

double GetBndDT(MatterArrStruct &Pnt1,MatterArrStruct &Pnt2)
  {return fabs(Pnt1.Temp[Pnt1.Num]-Pnt2.Temp[1]);};
   
double ClcTimeStp(MatterSumStruct &Sum)
  {
   double stp=1,newstp;
   stp=Sum.Matter[1].XStp*Sum.Matter[1].XStp;
   stp/=Sum.TimeStab;
   double tmp=fmod(Sum.Time,Sum.WriteTime);
   tmp=Sum.WriteTime-fmod(Sum.Time,Sum.WriteTime);
   if (tmp<stp)
     stp=(floor(Sum.Time/Sum.WriteTime)+1)*Sum.WriteTime-Sum.Time;
cout<<" Time "<<Sum.Time<<" Stp "<<stp<<"\n";
   Sum.Time+=stp;
   
   return stp;
  };
int ClcTempStp(MatterSumStruct &Sum)
  {
   for (int k=1;k<=Sum.Num;k++)
     {
      FlowArray(Sum.Matter[k]);
      if (k==1) FlowBnd(Sum.Matter[k],Sum.Matter[k+1],1);
      else if (k==Sum.Num) FlowBnd(Sum.Matter[k-1],Sum.Matter[k],0);
        else FlowBnd(Sum.Matter[k-1],Sum.Matter[k],Sum.Matter[k+1]);
     }
   double stp=ClcTimeStp(Sum);
   for (k=1;k<=Sum.Num;k++) TempArray(Sum.Matter[k],stp);
   if (Sum.Time>Sum.EndTime) return 0;
   return 1;
  };

// ========================Input Output ===========================

int ControlArgv(int argc,char *argv[])
  {
   if (argc!=3) { cout<<"Wrong Number of Parameters";return 0; }
   return 1;
  };
// ===============Main =================
void main(int argc,char *argv[])
  {
   MatterSumStruct Sum;
//   D2Array Midue1, Midue2;
   if ( !ControlArgv(argc,argv) ) exit(0);
   fstream in(argv[1],ios::in);
   in>>Sum;
   fstream out(argv[2],ios::out);
   while ( ClcTempStp(Sum) )   out<<Sum;
  };
