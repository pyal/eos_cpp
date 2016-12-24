

#pragma once

extern "C"
#include "zubr_import.h"
    extern "C"
#include "zubr_calculator_tab.h"

#include "util\code_gen\analyse_base\lex_anal.h"
#include "lib/ref/stroka.h"
#include "lib/data/VecRes.h"


    typedef void DataSimple;

// Nec for construction
struct DataConstr : RefCount {
    DataSimple *dat;
    Stroka Name;
    int Spec;
    int Type;
    //	DataSimple *GetFinalData(){return dat;}
};


struct InstrFunction : RefCount {
    int Func(list<DataSimple *> vars, const Stroka &Operation);
    int CheckParams(list<Ref<DataConstr>> vars, const Stroka &Operation);
};
// Nec for construction
struct ProgrConstrTypes : RefCount {
    map<Stroka, Ref<InstrFunction>> Funcs;
    int DataSize;
    Stroka TypeName;
    int TypeId;   //?
};
struct ProgrConstrTypesStrorage : RefCount {
    map<Stroka, int> N2ID;
    vector<Ref<ProgrConstrTypes>> types;
};
// End construction
struct ProgrInstr : RefCount {
    // function auto moves ptr
    void Execute(list<ProgrInstr>::iterator &cur) {
        Instr->Func(V, Op);
        cur++;
    }

private:
    Ref<InstrFunction> Instr;
    //	list<ProgrInstr>::iterator Next;
    list<DataSimple *> V;
    Stroka Op;
};

struct ProgrDataSimple : RefCount {
    map<Stroka, DataSimple *> N2D;
    ResizeVector<char> D;
};


struct Program : RefCount {
    list<ProgrInstr> Prog;
    Ref<ProgrDataSimple> Dat;

    void Execute() {
        for(list<ProgrInstr>::iterator iter = Prog.begin(); iter != Prog.end();)
            iter->Execute(iter);
        return *this;
    };
};

struct ProgramConstructor : RefCount {
    Ref<Lex_Analyser> lex;
    Ref<Program> prog;
    Ref<ProgrConstrTypesStrorage> types;

    ProgramConstructor(Lex_Analyser *Analyse) : lex(Analyse){};
    CalculatorProgam &Read() {
        Ptr = this;
        while(lex->IsGood())
            zubr_calculator_zubr_parse();
        //		code(STOP);
        return *this;
    }
    static void AddAction(DataConstr *dat1, DataConstr *dat2, Stroka Oper) {
        Ref<ProgConstrTypes> t = types->types[dat1->Type];
    }
};
