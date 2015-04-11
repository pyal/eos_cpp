


#ifndef _ANSYMBOL_H
#define _ANSYMBOL_H


//    int MakeOper(array<operation> oper,oper_def *operation_type,symbol *lft, symbol *rgt);
//    int MakeInstr(array<operation> oper,symbol *instr, list<symbol*> *arg);

//struct type_def:ClassKey
//{
//	char *name;
//	list<type_def*> convertions;
//}
#define type_def ClassKey
#define modif_def int

struct symbol 
{
//	char *name;
	modif_def *modif;
	type_def *type;
	void *addr;
//	symbol(const char *name_,type_def *type_,int modifyer_)
//	symbol():name(0){};
//	~symbol(){delete name;}
}

struct func_var
{
	int progr_addr;
}
struct obj_var
{
	array<void*> comp;
}


struct obj_struct{	type_def *def;	int num;}
struct obj_def:type_def
{
	map<char *,obj_struct> comp;
//	type_def* get_sub_obj(const char *name);
}
struct func_def:type_def
{
	list<type_def*> arg;
	type_def *ret;
}

template<T>
struct simp_var
{
	T val;
}
struct oper_def:type_func
{
	int oper_type;
}
enum OPERATIONS {NEW,DELETE,EQUALS}


#endif