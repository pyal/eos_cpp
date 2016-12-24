

#pragma once

extern "C"
#include "zubr_import.h"
#include "zubr_calculator_tab.h"
    extern "C"

#include "util\code_gen\analyse_base\lex_anal.h"
#include "lib/ref/stroka.h"
#include "lib/data/VecRes.h"


    struct DataBase : DescribedClass {
    enum { Pls, Mns, Div, Mul, Not, Eq, Element, Cvt, Func } Operation;
    DataBase(){};
    DataBase(const DataBase &dat){};

    Stroka name; /* имя переменной или функции */
    virtual int MakeOperation(
        Operation oper,
        DataBase *res,
        DataBase *op1,
        DataBase *op2) {
        return 0;
    };
    virtual Stroka ToStr() {
        return Stroka("NODATA");
    }
    friend ostream &operator<<(ostream &out, DataBase &dat) {
        out << dat.ToStr().c_str() << " ";
    }
};
template <class T>
struct DataStd<T> : DataBase {
    virtual int Convert(DataBase *o, DataStd<T> &*res) {
        res = NULL;
        if(!o)
            return 1;
        res = dynamic_cast<DataStd<T> *>(o);
        if(res)
            return 1;
        if(!o->MakeOperation(DataBase::Cvt, res, o, NULL))
            return 0;
        return 1;
    }
    virtual int MakeOperation(
        Operation oper,
        DataBase *res,
        DataBase *op1,
        DataBase *op2) {
        DataStd<T> *o1, *o2, *r;
        if(!Convert(op1, o1) || !Convert(op2, o2) || !Convert(res, r))
            return 0;
        if(oper == DataBase::Pls) {
            r->v = o1->v + o2->v;
            return 1;
        }
        if(oper == DataBase::Mns) {
            r->v = o1->v - o2->v;
            return 1;
        }
        if(oper == DataBase::Div) {
            r->v = o1->v / o2->v;
            return 1;
        }
        if(oper == DataBase::Mul) {
            r->v = o1->v * o2->v;
            return 1;
        }
        //		if (oper==DataBase::Mul){ r->v=o1->v*o2->v;return 1;}
        return 0;
    }
    Stroka ToStr() = 0;
    T v;
};
typedef DataStd<double> DataDouble;


struct CalculatorVars : RefCount {
    map<Stroka, Ref<DataBase>> progr_vars;
    ResizeVector<Ref<DataBase>> progr_const;

    void AddVar(DataBase *v, const char *n) {
        if(!n)
            progr_const.push(v);
        else
            progr_vars[Stroka(n)] = v;
        //		return Pack_ZUBR(sym);
    }
    int GetVar(const char *name, DataBase &*zvar) {
        if(progr_vars.find(Stroka(name)) == progr_vars.end())
            return 0;
        zvar = progr_vars[Stroka(name)];
        return 1;
    };
};
