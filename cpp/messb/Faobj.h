#ifndef __FAOBJ_H
#define __FAOBJ_H
/*
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream.h>
#include <iomanip.h>
#include <alloc.h>
#include <dir.h>
#include <string.h>
#include <iostream.h>
#include <PROCESS.H>
*/
#include "messb_defs.h"
#include "lib/std/util.h"
//#include "described.h"
//#include "class.h"
#include "lib\ref\class_sav.h"
#include "lib\std\stdexception.h"
#include "lib\ref\stroka.h"

#include "lib\ref\ref.h"
#include "mat\matrics.h"

#define MaxFunctions    60
#define MaxFunctionPar  60
#define MaxCorrelations 60
#define MaxIterations   10

struct FAZer:SavableClass
  {
   char *name;
   int NumPar;
   char *namepar[MaxFunctionPar+1];
   double valpar[MaxFunctionPar+1];
   char *helppar[MaxFunctionPar+1];
   virtual int GetNumPar(){return NumPar;};

   virtual double Clc(double *par,double Xval)=0;
   virtual void   Clc(double *par,double *Xval,double *Res)=0;
   virtual void ParStp(double *par,double *stp)=0;  
   // only for the use in FAZerIO no more

   FAZer(){name=NULL;NumPar=0;helppar[0]=0;};
   virtual int HelpPresent(){return (helppar[0]!=0);}
   virtual void DeleteFAZer();
   virtual void DrawHelp(ostream &out);
   ~FAZer(){DeleteFAZer();};
   virtual void OutputInternPar(ostream &output){};
   virtual void InputInternPar(istream &input){};
   Stroka MakeHelp(){
        const int BufSize = 100000;
        char buf[BufSize+1];
        std::strstream out(buf,BufSize);
        DrawHelp(out);
        return Stroka(buf,0,out.pcount());
   }
  };
struct FAZerBase:FAZer
{
// returns number of settled params (2)
	FAZerBase():FAZer(){};
   int SetStdPar_GrInt(char **namepar,double *valpar,char **helppar,int StartNum); 
   double Clc(double *par,double Xval) {double x[]={1,0},y[]={1,0};x[1]=Xval;Clc(par,&x[0],&y[0]);return y[1];}
   void   Clc(double *par,double *Xval,double *Res);
   virtual void ClcBase(double *par,double *Xval,double *Res)=0;
   void ParStp(double *par,double *stp);
};

struct FAIO:DescribedClass
  {
   virtual int GetNumPar()=0;
   virtual int GetNumIter(int CurIter=0)=0;
   virtual Stroka GetParName(int n)=0;
   virtual int GetParNameNum(const Stroka &par)=0;

   virtual double Clc(double *par,double Xval)=0;
   virtual void   Clc(double *Xval,double *Res)=0;
   virtual double Clc(double Xval)=0;

   virtual void GetPar(double *par)=0;
   virtual void SetPar(double *Par)=0;
   virtual void GetParStp(double *par,double *stp)=0;
   virtual void SetParStp(double *par,double *stp)=0;

   virtual void Output(ostream &output)=0;
   virtual void Input(istream &input)=0;

   virtual void DrawHelp(ostream &out){};

   virtual void SaveInPar(ostream &output);
   virtual void GetInPar(istream &input);

   char *name;
   FAIO(){name=NULL;};
   virtual ~FAIO(){delete name;};
   static void list_all_functions(ostream &out);
   static FAIO *create(FAZer *clc_func);


//  int Register(char *name);
  };

struct FASingleIO:FAIO
  {
   Stroka GetParName(int n){
	   if (n<1 || n>GetNumPar())
           throw info_except(" cannot get name of parameter %i for func %s\n ",n,Func->name);
       return Stroka(Func->namepar[n]).Trim();
  }
   int GetParNameNum(const Stroka &par){
	   int k;
       for(k=1;k<=GetNumPar()&&par!=Stroka(Func->namepar[k]).Trim();k++)
           ;
       if (k>GetNumPar())
           throw info_except(" name %s was not found\n",par.c_str());
       return k;
   }
   double *Par,*Stp[MaxIterations+1];

   FAZer *Func;
   int NumPar,CurIter,MaxIter;

   FASingleIO()           {Par=NULL;Func=NULL;NumPar=0;CurIter=1;MaxIter=0;name=strdup("FASingle");};
   ~FASingleIO()          {DeleteFASingleIO();};
   void DeleteFASingleIO();
//   {delete Par;delete Func;Par=NULL;Func=NULL;NumPar=0;MaxIter=0;};
    
   virtual int GetNumPar() {return Func->NumPar;};
   int ControlIter(int N) 
     { if (MaxIter==0) return 0;if ((N>MaxIter) || (N<=0)) N=1;return N;};
   virtual int GetNumIter(int iter=0)
     { 
      if (!iter) return MaxIter;
      if (!(CurIter=ControlIter(iter))) 
        {cout<<"FASingleIO::GetNumIter=0 Error; Halt\n";exit(1);};
      Func->ParStp(Par,Stp[CurIter]);
      return CurIter;
     } 

   virtual void GetPar(double *par)
      {MemoryMove(Par,par,sizeof(double)*(Par[0]+1));};
   virtual void SetPar(double *par)
      {MemoryMove(&par[1],&Par[1],sizeof(double)*Par[0]);};
   virtual void GetParStp(double *par,double *stp)
      {MemoryMove(Par,par,sizeof(double)*(Par[0]+1));
       MemoryMove(Stp[CurIter],stp,sizeof(double)*(Par[0]+1));};
   virtual void SetParStp(double *par,double *stp)
      {MemoryMove(&par[1],&Par[1],sizeof(double)*Par[0]);
       if (!(CurIter=ControlIter(CurIter))) 
         {cout<<"FASingleIO::SetParStp=0 Error; Halt\n";exit(1);};
       MemoryMove(&stp[1],&Stp[CurIter][1],sizeof(double)*Par[0]);};

   virtual double Clc(double *par,double Xval)
      {SetPar(par);return Func->Clc(Par,Xval);};
   virtual void   Clc(double *Xval,double *Res){ Func->Clc(Par,Xval,Res);};
   virtual double Clc(double Xval)             {return  Func->Clc(Par,Xval);};

   virtual void Output(ostream &output);
   virtual void Input(istream &input);
   virtual void SetNewFunc(FAZer *func,int NumIter);
   virtual void DrawHelp(ostream &out);
  private:
   virtual void DrawHelp(ostream &out,const char *category);
  };

// If Sum Function does not have internal parameters then the only
// function to change is Clc()
// Base pure virtual class
struct FAArr:FAIO
  {
   FAIO *FuncArr[MaxFunctions+1];
   int NumParArr[MaxFunctions+1];
   int NumFunc,CurIter,MaxIter;

//Iter - it looks like that is appr method...-which have to be fixed...
   int ControlIter(int N)
     { if (MaxIter==0) return 0;if ((N>MaxIter) || (N<=0)) N=1;return N;};
   virtual int GetNumIter(int iter=0);

   FAArr();//:FAIO(){NumFunc=0;};
   virtual ~FAArr(){DeleteFAArr();};
   void DeleteFAArr();

   virtual int GetNumPar(){return NumParArr[0];};

   double Clc(double *par,double Xval){SetPar(par);return Clc(Xval);};
//   virtual void   Clc(double *Xval,double *Res)=0;
   virtual double Clc(double Xval)=0;

   virtual void GetPar(double *Par);
   virtual void SetPar(double *Par);
   virtual void GetParStp(double *par,double *stp);
   virtual void SetParStp(double *par,double *stp);

   void Input(istream &input);
   void Output(ostream &Output);

   virtual void SaveInPar(ostream &output);
   virtual void GetInPar(istream &input);

   virtual void DrawHelp(ostream &out);

   Stroka GetParName(int n){
	    if (n<1 || n>GetNumPar())
		    throw info_except(" cannot get name of parameter %i for Array\n",n);
	    int s=0, k;
	    for (k=1;k<=NumFunc && s<n;k++) {
		    s+=FuncArr[k]->GetNumPar();
	    }
        k--;
        s-=FuncArr[k]->GetNumPar();
        return SetPrefixName(k)+FuncArr[k]->GetParName(n-s);
    }

   int GetParNameNum(const Stroka &par){
       Stroka par_n(par);
       int N = GetPrefixNum(par_n);
       int ret = FuncArr[N]->GetParNameNum(par_n);
       for (int k=1;k<N;k++) 
		    ret+=FuncArr[k]->GetNumPar();
       return ret;
   }

 protected:
  virtual void SetNewFunc(FAIO *func,int Num=0);
    Stroka SetPrefixName(int NumFunc){
        return Stroka(name,0,3)+Stroka::Int2Str(NumFunc)+Stroka("::");
    }
    int GetPrefixNum(Stroka &name){
        size_t pos = name.find(':');
        if (pos==0)
            throw info_except(" cannot find delimiter in str %s\n",name.c_str());
        Stroka res(name,pos+2,name.length());
        const char *str= name.c_str();
        size_t beg = pos-1;
        while (strchr("0123456789",str[beg])!=0)
            beg--;
        Stroka num(name,beg+1,pos-beg);
        int n = atoi(num.c_str());
        if (n==0)
            throw info_except(" number is not found in %s number is %s func name is %s\n",
                              name.c_str(),num.c_str(),res.c_str());
        name = res;
        return n;
    }

  private:
   void SetNumPar();
   void CheckIterNumber();

  };
extern int FARegister(istream &in,FAIO *&Func);


/*
struct FAOne:FAIO
  {
   char *name;
   FAIO *Func;
   FAOne(){Func=NULL;name=NULL;};
   virtual ~FAOne(){DeleteFAOne();};
   void DeleteFAOne(){delete name;delete Func; name=NULL;Func=NULL;};

   virtual int GetNumPar(){return Func->GetNumPar();};

   virtual double Clc(double *par,double Xval){SetPar(par);return Clc(Xval);};
//   virtual void   Clc(double *Xval,double *Res)=0;
//   virtual double Clc(double Xval)=0;

   virtual void GetPar(double *Par){Func->GetPar(Par);};
   virtual void SetPar(double *Par){Func->SetPar(Par);};
   virtual void GetParStp(double *par,double *stp){Func->GetParStp(par,stp);};
   virtual void SetParStp(double *par,double *stp){Func->SetParStp(par,stp);};

   void Input(fstream &input)
     { if (!FARegister(input,Func)){cout<<"Could not register. Halt\n";exit(0);}};
   void Output(fstream &Output){Func->Output(Output);};

   virtual void SaveInPar(fstream &output)
     { Func->SaveInPar(output);output<<"      ";};
   virtual void GetInPar(fstream &input){Func->GetInPar(input);};

  };
*/
//int FARegister(char *name,FAIO *&Func);


#endif
