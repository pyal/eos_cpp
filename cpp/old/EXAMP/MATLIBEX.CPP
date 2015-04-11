// uses matlib,memf,grlib;
#include "matlib.h"
#include "memf.h"
#include "grlib.h"

// y=ax2+bx+c
extern unsigned _stklen=40000;

double Exp[1000];
double Yval[1000];
double TruePar[4];
double HiBeg;
ClcMinStrc MinObj;

double Work_F(double *par,double where)
  { return par[1]*par[3]/(sqr(where-par[2])+sqr(par[3]));};
double Hi_Func(double *par)
  {
   double hi=0;
   for (int k=1;k<=Exp[0];k++)  hi+=sqr(Work_F(par,Yval[k])-Exp[k])/Exp[k];
   return hi;
  };
double min_f(double x,double y,X_func Func,double Tol)
  { return MinObj.MinGoldSqr(x,y,Func,Tol);  };

void Get_Theo(double *par,double *theo)
  {
   for (int k=1;k<=Exp[0];k++)  theo[k]=Work_F(par,Yval[k]);
   theo[0]=Exp[0];
  };


void InputParam(double *Param,double *Exper,double *Stp,double &NumIt)
  {
   cout<<" InputParam Beg"<<coreleft()<<"\n";ChRead();
   Stp[0]=3;Param[0]=3;TruePar[0]=3;
   cout<<" Enter Num Chan \n ";cin>>Exper[0];
   cout<<" Enter Num Iter\n ";cin>>NumIt;
   for (int k=1;k<=Exper[0];k++) Yval[k]=k;
   cout<<" Enter true parameters\n";cin>>TruePar[1]>>TruePar[2]>>TruePar[3];
   Get_Theo(TruePar,Exper);

   cout<<" Enter parameters\n";cin>>Param[1]>>Param[2]>>Param[3];
   cout<<" Enter step par\n";cin>>Stp[1]>>Stp[2]>>Stp[3];
   HiBeg=Hi_Func(Param);
   cout<<" InputParam End"<<coreleft()<<"\n";ChRead();
  };
void OutFile(double *theo,double *Param)
  {
   memf *f=new memf("out.dat",ios::out,"Graphic  points",1);
   double HiEnd=Hi_Func(Param);
   (*f->mfbuf)<<" HiBeg= "<<HiBeg<<"  HiEnd   ="<<HiEnd<<"\n";
   (*f->mfbuf)<<" par 1= "<<Param[1]<<" par 2= "<<Param[2]<<
		    " par 3= "<<Param[3]<<"\n";
   (*f->mfbuf)<<" Tpar1= "<<TruePar[1]<<" Tpar2= "<<TruePar[2]<<
		   " Tpar3= "<<TruePar[3]<<"\n";
   for (int k=1;k<=Exp[0];k++)
      (*f->mfbuf)<<Yval[k]<<"    "<<Exp[k]<<"    "<<theo[k]<<"\n";
   delete f;
  };

void OutputParam(double *Param)
  {
   cout<<" OutParam Beg"<<coreleft()<<"\n";ChRead();
   double theo[1000];
   Get_Theo(Param,theo);
   int graphdrive=DETECT,graphmode;
   initgraph(&graphdrive,&graphmode,"");
//   setcolor(YELLOW);
   GrArr *ex=new GrArr(Yval,Exp,Exp[0]);
   GrScr *sum=AxGraph(ex,"X- text","Y - text ");
   GrArr *th=new GrArr(Yval,theo,Exp[0]);th->PixColor(GREEN);
   sum->LstAdd( th );
   sum->LstAdd(ex);
   sum->Show();
   ChRead();
   delete sum;
   closegraph();
   OutFile(theo,Param);
   cout<<" par 1= "<<Param[1]<<" par 2= "<<Param[2]<<" par 3= "<<Param[3]
	    <<"\n";
   cout<<" Tpar1= "<<TruePar[1]<<" Tpar2= "<<TruePar[2]<<" Tpar3= "
	  <<TruePar[3]<<"\n";
   cout<<" OutParam End"<<coreleft()<<"\n";ChRead();
  };


void main()
  {
   cout<<" Main Beg"<<coreleft()<<"\n";ChRead();
   double NumIt;
//                                       int num;
   double Param[4];Param[0]=3;double Stp[4];Stp[0]=3;
   InputParam(Param,Exp,Stp,NumIt);
   GradAppr *Apr=new GradAppr;

   Apr->ResetAppr(1e-6,3,&(Hi_Func),&min_f,NULL);
   //&(MinObj.MinGoldSqr));
   for (int k=1;k<=NumIt;k++)
     { Apr->Clc(Param,Stp);cout<<"\nHi= "<<Hi_Func(Param);}
   ChRead();
   OutputParam(Param);
   delete Apr;
//   delete Apr;
//   DelMinFunc();
   cout<<"\n Main End"<<coreleft()<<"\n";ChRead();
  };

