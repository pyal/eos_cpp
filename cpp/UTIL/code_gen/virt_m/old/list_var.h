#ifndef _ANLIST_VAR_H
#define _ANLIST_VAR_H


struct Program_types {
    map<char *, type_def *> prg_types;
    list<oper_def> operations;
}
#define map <char *, type_var *> MAPVAR
struct Block_vars {
    MAPVAR blk_vars;
} struct Program_blk {
    array<Block_vars> prg_blk;
} struct Program_exe {
    Program_types types;
    stack<type_var> local_vars;
    stack<int> func_call;
    array<operation> instr;
}


#endif
