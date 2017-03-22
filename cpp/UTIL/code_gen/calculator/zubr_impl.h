
#pragma once

//extern "C" {
#include "zubr_import.h"
#include "zubr_calculator_tab.h"
//} //extern "C"

#include "util/code_gen/analyse_base/lex_anal.h"
#include "lib/ref/stroka.h"
#include "lib/data/VecRes.h"
#include "calc_prg.h"
#include "calc_prg_types.h"

struct Lex_Calcul_Analyser : Lex_Analyser {
    Lex_Calcul_Analyser(istream *in = NULL) : Lex_Analyser(in, 0) {
        Init_Analyser();
    };

    virtual void Init_Analyser() {
        browse.clear();
        //		browse.PushTokenLevel();
        browse.PushState(new StateSkipSpace);
        browse.PushState(new StateComment);

        browse.PushState(new StateNumber);
        browse.PushState(new StateString);
        browse.PushState(token = new StateToken);
        browse.PushState(new StateIdentifier);
        browse.PushState(new StateDefIsError);
        //		browse.PushState(new StateTest(WriteResults));
        PushTokenLevel();

        //AddToken("sin",BuildInFunc::SIN+1500);
        //AddToken("cos",BuildInFunc::COS+1500);
        //AddToken("exp",BuildInFunc::EXP+1500);

        //AddToken("+=",ADDEQ);
        //AddToken("-=",SUBEQ);
        //AddToken("*=",MULEQ);
        //AddToken("/=",DIVEQ);
        //AddToken("print",PRINT);
        //AddToken("read",READ);

        AddToken(",", int(','));
        AddToken("(", int('('));
        AddToken(")", int(')'));

        AddToken("+", int('+'));
        AddToken("-", int('-'));
        AddToken("=", int('='));
        AddToken("*", int('*'));
        AddToken("/", int('/'));
        AddToken("^", int('^'));
        AddToken(";", int('\n'));
    };
};

struct CalculatorProgam : RefCount {
    Ref<Lex_Analyser> lex;
    ConstructProgram Progr;

    CalculatorProgam(Ref<Lex_Analyser> Analyse = NULL) {
        ResetProgram(Analyse);
    };
    void ResetProgram(Ref<Lex_Analyser> Analyse) {
        lex = Analyse;
        Progr.Clear();
        StdClcTypes::AddStdTypes(Progr.data->GetTypeDef());
        AddStdVars();
    };

    Stroka ToString() {
        Stroka ret = "Data: " + Progr.data->ToString();
        ret += "Error: " + Progr.Error +"\n";
        ret += "Program" + Progr.progr->ToString();
        return ret;

    }
    CalculatorProgam &Execute() {
        GetCurPtr(this);
        Progr.Execute();
        RestorePtr();
        return *this;
    }
    CalculatorProgam &Read() {
        GetCurPtr(this);
        while(lex->IsGood()) {
            zubr_calculator_zubr_parse();
        }
        //		code(STOP);
        //    lex->ResetStream(NULL);
        // We are generating - ConstrStorage, have to copy its data
        // to program storage, instructions are already generated.
        RestorePtr();
        return *this;
    }
    static inline Ref<DataConstr> AddOper(DataConstr *fst, DataConstr *sec, char Oper) {
        CalculatorProgam *ptr = GetCurPtr();
        Ref<DataConstr> res;
        if(!ptr->Progr.AddFunc(fst, sec, Stroka(Oper), res)) {
            zubr_calculator_zubr_error(ptr->Progr.Error.c_str());
            return 0;
        }
        return res;
    }
    static inline void ListStart() {
        list<DataConstr *> lst;
        GetCurPtr()->arg_lst_stack.push(lst);
    }
    static inline Ref<DataConstr> ListAdd(DataConstr *fst) {
        GetCurPtr()->arg_lst_stack.top().push_back(fst);
        return fst;
    }
    static inline Ref<DataConstr> AddFunc(DataConstr *fst) {
        CalculatorProgam *p = GetCurPtr();

        Ref<DataConstr> res;
        if(fst->Type->TypeName != Stroka("undef"))
            zubr_calculator_zubr_error(
                (Stroka("Bad func name type:") + fst->Type->TypeName).c_str());
        //p->arg_lst_stack.top().push_front(fst);
        if(!p->Progr.AddFunc(p->arg_lst_stack.top(), fst->Name, res))
            zubr_calculator_zubr_error(p->Progr.Error.c_str());
        p->arg_lst_stack.pop();
        return res;
    }
    static inline Ref<DataConstr> DefineVar(DataConstr *fst) {
        CalculatorProgam *p = GetCurPtr();

        Ref<DataConstr> res;
        for(list<DataConstr *>::iterator i = p->arg_lst_stack.top().begin();
            i != p->arg_lst_stack.top().end();
            i++)
            res = DefineVar(fst, *i);
        p->arg_lst_stack.pop();
        return res;
    }
    static inline Ref<DataConstr> DefineVar(DataConstr *fst, DataConstr *sec) {
        CalculatorProgam *p = GetCurPtr();


        if(sec->Type->TypeName != Stroka("undef"))
            zubr_calculator_zubr_error(
                (Stroka("Bad var name:") + sec->Type->TypeName).c_str());
        if(fst->Type->TypeName == Stroka("undef"))
            zubr_calculator_zubr_error(
                (Stroka("Bad type name:") + fst->Type->TypeName).c_str());
        Ref<DataConstr> type = p->Progr.data->GetTypeDef(fst->Type->TypeName);

        sec->Type = type->Type;
        Ref<DataConstr> tmp = sec;   // Not to delete var ....
        p->Progr.data->DeleteVar(sec->Name.c_str());
        Ref<DataConstr> res = p->Progr.data->GenerateVar(sec);
        return res;
    }

    void AddStdVars() {
        GetCurPtr(this);
        GenerateDouble(M_PI, "M_PI");
        GenerateDouble(M_E, "M_E");
        GenerateDouble(M_Na, "M_Na");
        GenerateDouble(M_C, "M_C");
        GenerateDouble(M_MinDouble, "M_MinDouble");
        GenerateDouble(M_Eps, "M_Eps");
        GenerateDouble(M_Rconst, "M_Rconst");
        GenerateDouble(M_eV_K, "M_eV_K");
        GenerateDouble(M_eV_kJ, "M_eV_kJ");
        GenerateDouble(
            M_PartDist1Bor_is_VolIn_cm3_mol, "M_PartDist1Bor_is_VolIn_cm3_mol");
        GenerateDouble(M_Bor, "M_Bor");

        GenerateDouble(M_PlankCros_K, "M_PlankCros_K");
        GenerateDouble(M_eV_Joule, "M_eV_Joule");
        GenerateDouble(M_h_Plank, "M_h_Plank");
        GenerateDouble(M_Mass_Electron_kg, "M_Mass_Electron_kg");
        GenerateDouble(M_Mass_Electron_eV, "M_Mass_Electron_eV");
        GenerateDouble(M_ProtonMass_kg, "M_ProtonMass_kg");
        GenerateDouble(M_ProtonMass_eV, "M_ProtonMass_eV");
        GenerateDouble(M_ProtonMass_K, "M_ProtonMass_K");
        GenerateDouble(M_Rydberg, "M_Rydberg");
        GenerateDouble(M_ElectronCharge, "M_ElectronCharge");
        GenerateDouble(M_Rau_cm, "M_Rau_cm");
        GenerateDouble(M_Eau_J, "M_Eau_J");
        GenerateDouble(M_Eau_eV, "M_Eau_eV");
        GenerateDouble(M_Pau_GPa, "M_Pau_GPa");
        GenerateDouble(M_Epsilon0, "M_Epsilon0");
        GenerateDouble(M_ElectrInter_Div_MassElectron, "M_ElectrInter_Div_MassElectron");

        RestorePtr();
    }
    static inline Ref<DataConstr> GenerateDouble(double val, const char *name = NULL) {
        Ref<DataConstr> d = GetCurPtr()->Progr.data->GenerateVar(new DataConstr(
            NULL, GetCurPtr()->Progr.data->GetTypeDef(Stroka("double"))->Type, 0, name));

        *((double *)DataSimpleStorage::GetData(d->dat)) = val;
        return d;
    }
    static inline Ref<DataConstr> GenerateString(
        const Stroka &str,
        const char *name = NULL) {
        Ref<DataConstr> d = GetCurPtr()->Progr.data->GenerateVar(new DataConstr(
            NULL, GetCurPtr()->Progr.data->GetTypeDef(Stroka("string"))->Type, 0, name));
        char *ptr = (char *)DataSimpleStorage::GetData(d->dat);
        Ref<Stroka> *sss = (Ref<Stroka> *)(ptr);
        *sss = new Stroka(str);
        sss = NULL;
        //Ref<Stroka> *sss1 = (Ref<Stroka> *)((char*)DataSimpleStorage::GetData( d->dat ));
        //sss1 = NULL;
        return d;
    }
    static inline Ref<DataConstr> GetVar(const Stroka &str) {
        Ref<DataConstr> d = GetCurPtr()->Progr.data->GetVar(str);
        return d;
    };
    static inline Ref<DataConstr> GetType(const Stroka &str) {
        if(GetCurPtr()->Progr.data->IsTypeDef(str))
            return GetCurPtr()->Progr.data->GetTypeDef(str);
        return NULL;
    };
    static inline Ref<DataConstr> GenerateUndef(const Stroka &str) {
        return GetCurPtr()->Progr.data->GenerateVar(new DataConstr(
            NULL,
            GetCurPtr()->Progr.data->GetTypeDef(Stroka("undef"))->Type,
            0,
            str.c_str()));
    }
    static inline CalculatorProgam *GetCurPtr(CalculatorProgam *newPtr = NULL) {
        if(newPtr) {
            Ptr_Old = Ptr;
            Ptr = newPtr;
        }
        return Ptr;
    };
    static inline CalculatorProgam *RestorePtr() {
        Ptr = Ptr_Old;
        return Ptr;
    }

private:
    stack<list<DataConstr *>> arg_lst_stack;
    static CalculatorProgam *Ptr, *Ptr_Old;
};

//
//struct StaticFunc{
//    static Lex_Calcul_Analyser lex;
//};
//

extern int zubr_calculator_zubr_lex(void);
extern void zubr_calculator_zubr_error(const char *);

typedef void *SYMBOL;

SYMBOL EndStmt(SYMBOL fst);
SYMBOL AddVar(SYMBOL fst);
SYMBOL AddOper(SYMBOL fst, SYMBOL sec, char oper);
void ListStart(void);
SYMBOL ListAdd(SYMBOL fst);
SYMBOL AddFunc(SYMBOL fst);
SYMBOL GetEl(SYMBOL fst, SYMBOL sec);
SYMBOL DefineVar(SYMBOL fst, SYMBOL sec);
SYMBOL DefineVar(SYMBOL fst);
