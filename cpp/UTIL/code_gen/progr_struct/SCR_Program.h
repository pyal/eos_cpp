#pragma once


struct NamedClass {
    NamedClass() : name(""){};
    NamedClass(string &n_) : name(n_){};
    NamedClass(const char *n_) : name(n_){};
    virtual ~NamedClass(){};
    const string name;
    string ToString() {
        return string("name:").append(name).append("\n");
    };
} struct SCR_Type : NamedClass {
    SCR_Type(){};
    virtual ~SCR_Type(){};
};
struct SCR_Var : NamedClass {
    SCR_Var(){};
    virtual ~SCR_Var(){};
};
struct SCR_Program : NamedClass {
    SCR_Program(){};
    virtual ~SCR_Program(){};
    const char *name;
    // Type definitions
    SCR_TypeTable Types;
    SCR_VarTable Global_vars;
    SCR_Instruction *Instructions;
    SCR_Stack CallStack;
};
