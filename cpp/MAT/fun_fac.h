#ifndef __FUN_FAC_H
#define __FUN_FAC_H

//#include "lib/std/interfac.h"
#include "lib/ref/class_sav.h"
#include "lib/ref/str_manip.h"
#include "util/code_gen/calculator/calculator_std.h"
#include "mat/matrics.h"

struct OneVarFunction : SavableClass {
    //   static const char * const name;
    //   virtual const char *InterfaceIOName() {return name;};
protected:
    static OneVarFunction *StaticCurPtr;
    static double StaticClc(double x) {
        return StaticCurPtr->Calculate(x);
    }

public:
    double DerivStep;
    OneVarFunction() {
        DerivStep = 1e-6;
    }

    virtual double Calculate(double x) = 0;
    virtual void Calculate(double *x, double *y, int Num);
    virtual double Derivative(double x);
    virtual double Integral(double Xmin, double Xmax);

    virtual int save_data_state(FilterOut &so) {
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        return 1;
    };
};
struct NamedOneVarFunction : OneVarFunction {
    Stroka DefaultName;
    map<Stroka, int> ClcNames;
    NamedOneVarFunction() : OneVarFunction(){};
    NamedOneVarFunction(const Stroka &names) : OneVarFunction() {
        SetClcList(names);
    };
    using OneVarFunction::Calculate;
    virtual double Calculate(double x, const Stroka &name) = 0;

    virtual double Calculate(double x) {
        return Calculate(x, DefaultName);
    }
    int KnownClcName(const Stroka &name, int Break = 0) {
        int ret = ClcNames.find(name) != ClcNames.end();
        if(Break && !ret)
            throw info_except(
                "Unknown clc name <%s>. Known names are <%s>\n",
                name.c_str(),
                GetNames().c_str());
        return ret;
    }
    Stroka SetClcName(const Stroka &name) {
        if(name != "") {
            KnownClcName(name, 1);
            DefaultName = name;
        }
        return DefaultName;
    }
    void SetClcList(const Stroka &names) {
        int k = 0;
        ClcNames.clear();
        while((k = SkipSpace(names, k)) != -1) {
            int k1 = SkipNonSpace(names, k);
            ClcNames[Stroka(names, k, k1 - k)] = 1;
            k = k1 + 1;
        }
    }
    Stroka GetNames() {
        Stroka tmp;
        for(map<Stroka, int>::iterator it = ClcNames.begin(); it != ClcNames.end(); it++)
            tmp += it->first + " ";
        return tmp;
    }
    Stroka MakeHelp() {
        Stroka ret = "Class can calculate one var functions:<" + GetNames() + ">\n";
        return ret;
    }

private:
    int SkipSpace(const Stroka &str, int pos) {
        for(int k = pos; k < (int)str.length(); k++) {
            if(!isspace(str[k]))
                return k;
        }
        return -1;
    }
    int SkipNonSpace(const Stroka &str, int pos) {
        for(int k = pos; k < (int)str.length(); k++) {
            if(isspace(str[k]))
                return k;
        }
        return (int)str.length();
    }
};

struct TwoVarFunction : SavableClass {

    virtual double Calculate(double x, double y) = 0;
    virtual void Calculate(double *x, double *y, double *res, int Num);
    virtual int save_data_state(FilterOut &so) {
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        return 1;
    };
};

struct ManyVarFunction : SavableClass {
    virtual void Calculate(const map<Stroka, double> &x, map<Stroka, double> &y) {
        if(y.size() == 0)
            Calculate(x, "");
        else {
            for(map<Stroka, double>::iterator it = y.begin(); it != y.end(); it++)
                it->second = Calculate(x, it->first);
        }
    }
    virtual double Calculate(const map<Stroka, double> &x, const Stroka &name) = 0;

    //virtual int ReadIni(istream &input)  {return 1; };
    //virtual void SaveIni(ostream &output){ };
    virtual int save_data_state(FilterOut &out) {
        return 1;
    }
    virtual int read_data_state(FilterIn &in) {
        return 1;
    }
};

int OneVarFuncRegister(istream &in, OneVarFunction *&Func);
int TwoVarFuncRegister(istream &in, TwoVarFunction *&Func);


struct OneVarFormula : OneVarFunction {
    OneVarFormula() : EndSymbol("}"), PrgStr("Y=X;\n") {
        InitProgram();
    }
    void InitProgram() {
        strstream ins((char *)PrgStr.c_str(), (streamsize)PrgStr.length(), ios::in);
        prg.ResetProgram(new Lex_Calcul_Analyser(&ins));
        CalculatorProgam::GetCurPtr(&prg);
        prg.GenerateDouble(0, "Y");
        prg.GenerateDouble(0, "X");
        prg.Read();
        CalculatorProgam::RestorePtr();
    }
    double Calculate(double x) {
        CalculatorProgam::GetCurPtr(&prg);
        *(double *)DataSimpleStorage::GetData(prg.GetVar("X")->dat) = x;
        prg.Execute();
        double ret = *(double *)DataSimpleStorage::GetData(prg.GetVar("Y")->dat);
        CalculatorProgam::RestorePtr();
        return ret;
    }
    virtual int save_data_state(FilterOut &out) {
        out << "EndStr " << EndSymbol << " \n " << PrgStr << " " << EndSymbol << " ";
        return 1;
    };
    virtual int read_data_state(FilterIn &in) {
        Stroka str;
        PrgStr = "";
        in >> str >> EndSymbol >> str;
        while(str != EndSymbol) {
            PrgStr += str;
            if(seek_eoln(in))
                PrgStr += "\n";
            else
                PrgStr += " ";
            in >> str;
        }
        InitProgram();
        return !(!in);
    };


private:
    Stroka EndSymbol, PrgStr;
    CalculatorProgam prg;
};


struct ManyVarFunc2OneVar : OneVarFunction {
    ManyVarFunc2OneVar();   //:OneVarFunction(), Func(new ManyVarSplGenerator), InVarName("X"), OutVarName("Y"){}
    virtual double Calculate(double x) {
        map<Stroka, double> arg;
        FixedParameters[InVarName] = x;
        assert(!(!Func));
        return Func->Calculate(FixedParameters, OutVarName);
    }
    virtual void Calculate(double *x, double *y, int Num) {
        //map<Stroka, double> arg = F;
        assert(!(!Func));
        for(int i = 0; i < Num; i++) {
            FixedParameters[InVarName] = x[i];
            y[i] = Func->Calculate(FixedParameters, OutVarName);
        }
    };

    virtual int save_data_state(FilterOut &so) {
        so << " ManyVarFunc " << Func << " InVarName " << InVarName << " OutVarName "
           << OutVarName;
        so << " FixedParameters { ";
        for(map<Stroka, double>::iterator it = FixedParameters.begin();
            it != FixedParameters.end();
            it++) {
            char tmp[256];
            sprintf(tmp, "%s:%g ", it->first.c_str(), it->second);
            so << tmp;
        }
        so << " } ";

        return !(!so);
    }
    virtual int read_data_state(FilterIn &si) {
        char tmp[256];
        si >> tmp >> Func >> tmp >> InVarName >> tmp >> OutVarName;
        FixedParameters.clear();
        si >> tmp >> tmp >> tmp;
        while(Stricmp(tmp, "}") != 0) {
            vector<Stroka> vec = Str::SplitLine(tmp, 0, ':');
            if(vec.size() != 2)
                throw info_except(
                    "Wrong input have to be of type <a:b> and is <%s>\n", tmp);
            FixedParameters[vec[0]] = atof(vec[1].c_str());
            si >> tmp;
        }
        return !(!si);
    }
    Ref<ManyVarFunction> Func;
    map<Stroka, double> FixedParameters;
    Stroka InVarName, OutVarName;
};

struct IFunctionConverter : SavableClass {

    virtual void SetAutoEncode(const vector<double> &xVec) {}
    virtual double Encode(double x, const char *errMes = NULL) = 0;
    virtual void Encode(
        const vector<double> &xVec,
        vector<double> &yVec,
        const char *errMes = NULL) {
        yVec.resize(xVec.size());
        try {
            for(size_t i = 0; i < xVec.size(); i++)
                yVec[i] = Encode(xVec[i], errMes);
        } catch(exception &e) {
            Stroka mes(e.what());
            mes += "original encoding vector is:\n" + VecCl(xVec).ToString();
            throw info_except(~mes);
        }
    }

    virtual double Decode(double x, const char *errMes = NULL) = 0;
    virtual void Decode(
        const vector<double> &xVec,
        vector<double> &yVec,
        const char *errMes = NULL) {
        yVec.resize(xVec.size());
        for(size_t i = 0; i < xVec.size(); i++)
            yVec[i] = Decode(xVec[i], errMes);
    }

    virtual void GetBoundary(double &minX, double &minY, double &maxX, double &maxY) {
        minX = minY = -MathInf;
        maxX = maxY = MathInf;
    }
    virtual int CheckVal(double x) {
        double miX, maX, miY, maY;
        GetBoundary(miX, maX, miY, maY);
        if(!In_Lim(x, miX, maX, 1))
            return 0;
        return 1;
    }
    virtual int save_data_state(FilterOut &so) {
        return 1;
    }
    virtual int read_data_state(FilterIn &si) {
        return 1;
    }
    Stroka MakeHelp() {
        return "1D function converter\n";
    }
    vector<double> MakeEncodedStepVector(
        double minX,
        double maxX,
        double numX,
        int centered) {
        VecCl vec = VecCl::MakeStepVector(numX, Encode(minX), Encode(maxX), centered);
        vector<double> ret;
        Decode(vec.Copy2Vector(), ret, "encoded step vector");
        return ret;
    }
};


#endif   //  __FUN_FAC_H
