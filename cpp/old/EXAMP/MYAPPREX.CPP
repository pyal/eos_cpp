// int 48H - info about memory 
// int 58H - get memory strategy
// uses matlib,memf,grlib;
#include "myappr.h"
#include "memf.h"
#include "grlib.h"

// y=ax2+bx+c
extern unsigned _stklen=60000;

double Exp[1000];
double Yval[1000];
double TruePar[6];
double HiBeg;
//ClcMinStrc MinObj;
#define SHOUT 2

double Work_F(double *par,double where)
  { return par[1]-par[2]*sqr(par[4]/2)/(sqr(where-par[3])+sqr(par[4]/2)
                                  +par[5]*sqr(sqr(where-par[3])));};
double Hi_Func(double *par)
  {
   double hi=0;
   for (int k=1;k<=Exp[0];k++)  hi+=sqr(Work_F(par,Yval[k])-Exp[k]);//Exp[k];
   return hi;
  };

void Get_Theo(double *par,double *theo)
  {
   for (int k=1;k<=Exp[0];k++)  theo[k]=Work_F(par,Yval[k]);
   theo[0]=Exp[0];
  };


void InputParam(double *Param,double *Exper,double *Stp,double &NumIt)
  {
   cout<<" InputParam Beg"<<coreleft()<<"\n";ChRead();
   Stp[0]=5;Param[0]=5;TruePar[0]=5;
   cout<<" Enter Num Chan \n ";cin>>Exper[0];
   cout<<" Enter Num Iter\n ";cin>>NumIt;
   for (int k=1;k<=Exper[0];k++) Yval[k]=k;
   cout<<" Enter true parameters\n";
   cin>>TruePar[1]>>TruePar[2]>>TruePar[3]>>TruePar[4]>>TruePar[5];
   Get_Theo(TruePar,Exper);

   cout<<" Enter parameters\n";cin>>Param[1]>>Param[2]>>Param[3]>>Param[4]>>Param[5];
   cout<<" Enter step par\n";cin>>Stp[1]>>Stp[2]>>Stp[3]>>Stp[4]>>Stp[5];
   HiBeg=Hi_Func(Param);
   cout<<" InputParam End"<<coreleft()<<"\n";ChRead();
  };
void OutFile(double *theo,double *Param)
  {
   char str[256];

   cout<<" Write data to file out.dat[y/n]\n";
input:
   cin>>str;
   if (Stricmp(str,"y")==0)
     {
      fstream f("out.dat",ios::out);
      double HiEnd=Hi_Func(Param);
      f<<" HiBeg= "<<HiBeg<<"  HiEnd   ="<<HiEnd<<"\n";
      f<<" par 1= "<<Param[1]<<" par 2= "<<Param[2]<<
        " par 3= "<<Param[3]<<" par 4= "<<Param[4]<<" par 5= "<<Param[5]<<"\n";
      f<<" Tpar1= "<<TruePar[1]<<" Tpar2= "<<TruePar[2]<<
       " Tpar3= "<<TruePar[3]<<" Tpar4= "<<TruePar[4]<<" Tpar5= "<<TruePar[5]<<"\n";
randomize();
      for (int k=1;k<=Exp[0];k++)
         f<<Yval[k]<<"    "<<Exp[k]<<"    "<<theo[k]+0*(double)rand()/RAND_MAX*TruePar[2]/SHOUT<<"\n";
     }
   else if (Stricmp(str,"n")!=0) goto input;
  };

void OutputParam(double *Param)
  {
   double theo[1000];
   cout<<" OutParam Beg"<<coreleft()<<"\n";ChRead();
   Get_Theo(Param,theo);
   int graphdrive=DETECT,graphmode;
   initgraph(&graphdrive,&graphmode,"");
//   cout<<"Error grph "<<grapherrormsg(graphresult())<<"\n";


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
       <<" par 4= "<<Param[4] <<" par 5= "<<Param[5]<<"\n";
   cout<<" Tpar1= "<<TruePar[1]<<" Tpar2= "<<TruePar[2]<<" Tpar3= "
    <<TruePar[3]<<" Tpar4= "<<TruePar[4]<<" Tpar5= "<<TruePar[5]<<"\n";
   cout<<" OutParam End"<<coreleft()<<"\n";ChRead();
  };


void main()
  {
   cout<<" Main Beg"<<coreleft()<<"\n";ChRead();
   double NumIt;
//                                       int num;
   double Param[6];Param[0]=5;double Stp[6];Stp[0]=5;
   InputParam(Param,Exp,Stp,NumIt);
   MAPClc(Param,Stp,NumIt,&Hi_Func,&Get_Theo,&Exp[0],1e-6);
//   MAPClc(Param,Stp,NumIt,&Hi_Func,&Get_Theo,&Exp[0],1e-6);
   ChRead();
   OutputParam(Param);
   cout<<"\n Main End"<<coreleft()<<"\n";ChRead();
  };

