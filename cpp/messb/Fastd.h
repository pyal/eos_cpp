#ifndef __FASTD_H
#define __FASTD_H

#include "faobj.h"


//int FAStdRegister(fstream &in,FAIO *&Func,char *name);
extern int FARegister(istream &in, FAIO *&Func);

//=====================================================================
//=========================    FAZer          =========================
//=====================================================================


struct FAZerExp : FAZerBase {
    FAZerExp();
    void ClcBase(double *par, double *Xval, double *Res);
};

struct FAZerLor : FAZerBase {
    FAZerLor();
    void ClcBase(double *par, double *Xval, double *Res);
};

struct FAZerPoly : FAZerBase {
    FAZerPoly();
    void ClcBase(double *par, double *Xval, double *Res);
};

struct FAZerGaus : FAZerBase {
    FAZerGaus();
    void ClcBase(double *par, double *Xval, double *Res);
};


//=====================================================================
//=========================    FAArrAdd       =========================
//=====================================================================


struct FAArrAdd : FAArr {
    FAArrAdd() {
        name = strdup("sum_func");
    };
    ~FAArrAdd() {
        DeleteFAArr();
    };
    double Clc(double Xval);
    using FAArr::Clc;
    void Clc(double *Xval, double *Res);
    void Input(istream &input);
    void Output(ostream &Output);
    void DrawHelp(ostream &out);
};

struct FAArrSet : FAArr {
    int correlatedpar[2][MaxCorrelations];
    double correlatedcoef[MaxCorrelations];
    int NumCor;
    double CorFuncConst0, CorFuncExpConst;
    double Bnd[MaxFunctions + 1];
    double *BndStp[MaxFunctions + 1];

    FAArrSet() {
        name = strdup("set_func");
        MaxIter = 0;
        CurIter = 0;
        NumFunc = 0;
        CorFuncConst0 = 1;
        CorFuncExpConst = 3;
    };
    void DeleteFAArrSet();
    ~FAArrSet() {
        DeleteFAArrSet();
    };

    int GetNumPar() {
        return NumParArr[0] + NumFunc - 1;
    };
    double Clc(double Xval);
    using FAArr::Clc;
    void Clc(double *Xval, double *Res);

    void SetPar(double *par);
    void SetParStp(double *par, double *stp);
    void GetPar(double *par);
    void GetParStp(double *par, double *stp);

    double *FormShf();
    void virtual Output(ostream &output);
    void virtual Input(istream &input);

    virtual void SaveInPar(ostream &output);
    virtual void GetInPar(istream &input);

    void DrawHelp(ostream &out);

    Stroka GetParName(int n) {
        if(n < 1 || n > GetNumPar())
            throw info_except(" cannot get name of parameter %i for Array\n", n);
        if(n <= NumFunc - 1)
            return Stroka("Bnd") + Stroka::Int2Str(n);
        return FAArr::GetParName(n - NumFunc + 1);
    }

    int GetParNameNum(const Stroka &par) {
        if(Stroka(par, 0, 3) == Stroka("Bnd")) {
            Stroka num(par, 3, par.length());
            return atoi(num.c_str());
        }
        return FAArr::GetParNameNum(par) + NumFunc - 1;
    }

protected:
    virtual void SetNewFunc(FAIO *func, int Num);
};
/*
struct FARoll:FAOne
  {
   char *name;
   int NParam;
   char NameInt[30];
   double *XParamPnt,*YParamPnt;
   FARoll(){name=strdup("Roll_func");XParamPnt=NULL;XParamPnt=NULL;};
   ~FARoll(){delete XParamPnt;delete YParamPnt;
             XParamPnt=NULL;XParamPnt=NULL;DeleteFAOne();};

   virtual double Clc(double *par,double Xval){SetPar(par);return Clc(Xval);};
   virtual void   Clc(double *Xval,double *Res);
   virtual double Clc(double Xval);

   void Input(fstream &input)
     { char tmp[256];
       input>>tmp>>NParam >>tmp>>NameInt;
       if (!FARegister(input,Func))
          {cout<<"Could not register. Halt\n";exit(0);}
       InputXYParam();
     };
   void Output(fstream &Output)
     {Output<<" NumberParam  "<<NParam<<" NameFileInt  "<<NameInt <<"   ";
      Func->Output(Output);};

   virtual void SaveInPar(fstream &output)
     { output<<NParam<<"   "<<NameInt<<"   ";
       FAOne::SaveInPar(output);};
   virtual void GetInPar(fstream &input)
     { input>>NParam       >>NameInt       ;FAOne::GetInPar(input);
       InputXYParam(); };
   void InputXYParam(); 
  }; 
*/
struct FARoll : FASingleIO {
    //char *name;
    int NParam;
    char NameInt[30];
    double *XParamPnt, *YParamPnt;
    FARoll() : FASingleIO() {
        delete name;
        name = strdup("Roll_func");
        XParamPnt = NULL;
        YParamPnt = NULL;
    };
    ~FARoll() {
        delete XParamPnt;
        delete YParamPnt;
        XParamPnt = NULL;
        XParamPnt = NULL;
    };   //FASingleIO::~FASingleIO(); - done automatically !!!


    virtual void Clc(double *Xval, double *Res);
    virtual double Clc(double Xval);

    void Input(istream &input) {
        char tmp[256];
        input >> tmp >> NParam >> tmp >> NameInt;
        FASingleIO::Input(input);
        InputXYParam();
    };
    void Output(ostream &Output) {
        Output << " NumberParam  " << NParam << " NameFileInt  " << NameInt << "   ";
        FASingleIO::Output(Output);
    };

    virtual void SaveInPar(ostream &output) {
        output << NParam << "   " << NameInt << "   ";
        FASingleIO::SaveInPar(output);
    };
    virtual void GetInPar(istream &input) {
        input >> NParam >> NameInt;
        FASingleIO::GetInPar(input);
        InputXYParam();
    };
    void InputXYParam();
    void DrawHelp(ostream &out) {
        out << " No help for function " << name << " yet\n";
    }
};

#endif
