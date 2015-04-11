
#pragma once

//extern "C" 
//#include "zubr_import.h"
//#include "zubr_calculator_tab.h"
//extern "C" 

#include "util\code_gen\analyse_base\lex_anal.h"
#include "lib\ref\stroka.h"
#include "lib\data\VecRes.h"



typedef char DataSimple;



struct DataConstrStorage;
struct DataSimpleStorage:RefCount{
	ResizeVector<char> stor;
	ResizeVector<DataSimple*> ptrs;
//	map<Stroka, DataSimple*> N2D;
    friend struct DataConstrStorage;

    DataSimpleStorage(){char c=0;stor.SetFill(&c);};
    struct Pos{
        DataSimpleStorage *stor;
        int pos;
        Pos(DataSimpleStorage *stor_=NULL,int p=0):stor(stor_),pos(p){};
    };
    static DataSimple *GetData(const Pos &pos){
        return &pos.stor->stor[pos.pos];
    }

protected:
    
    //DataSimple* GenerateVar(int size){
    //    int l = stor.SetNumEl();
    //    stor.SetNumEl(l+size);
    //    return &stor[l];
    //};
    DataSimpleStorage::Pos GenerateVar(int size){
        int l = stor.SetNumEl();
        stor.SetNumEl(l+size);
        return DataSimpleStorage::Pos(this,l);
    };
    //DataSimple* GetVar(const DataSimpleStorage::Pos &pos){
    //    return &stor[pos];
    //};

};
struct TypeDef;
struct DataConstr:RefCount{
    DataConstr(const DataSimpleStorage::Pos &dat_,TypeDef* Type_,int Spec_,const char *name)
        :dat(dat_), Name(name), Type(Type_), Spec(Spec_){};
    ~DataConstr(){
        Name.erase();
        Type = NULL;
    }
    DataSimpleStorage::Pos dat;
	Stroka Name;
	TypeDef* Type;
	int Spec;
    DataSimpleStorage *stor;
	static int SameType(DataConstr *src, DataConstr *dest){
		if (!src && !dest)
			return 1;
		if (!src || !dest)
			return 0;
		if (src->Type==dest->Type)// && src->Spec==dest->Spec)
			return 1;
		return 0;
	}
    friend ostream &operator<<(ostream &out, const DataConstr& dat);
};

struct FuncArgs;
struct FuncStruct:RefCount{
	virtual void Function(Ref<FuncArgs> args,Stroka Oper)=0;
};


struct TypeDef:RefCount{
    TypeDef(const Stroka &typen, int size):TypeName(typen),DataSize(size){};
    ~TypeDef(){
        for(map<Stroka, list<Ref<FuncDef> > >::iterator iter = Funcs.begin();
            iter != Funcs.end();iter++){
            for(list<Ref<FuncDef> >::iterator i = iter->second.begin();
                i!= iter->second.end();i++)
                *i = NULL;
        }
    }
	struct ArgDef:RefCount{
		list<Ref<DataConstr> > dat;
		Ref<DataConstr> res;
        ArgDef(DataConstr* r,const list<DataConstr* > &d){
            if (r)
                res = new DataConstr(NULL,r->Type,r->Spec,NULL);
            else
                res = new DataConstr(NULL,NULL,NULL,NULL);
            for (list<DataConstr* >::const_iterator it = d.begin();it!=d.end();it++){
                dat.push_back(new DataConstr(NULL,(*it)->Type,(*it)->Spec,NULL));
            }
        }
	};
	struct FuncDef:RefCount{
		Ref<ArgDef> Arg;
		Ref<FuncStruct> Func;
        FuncDef(Ref<ArgDef> arg, Ref<FuncStruct> func)
            :Arg(arg),Func(func){};
	};

	map<Stroka, list<Ref<FuncDef> > > Funcs;
	Stroka TypeName;
	int DataSize;
	Ref<FuncDef> CheckOperArgs(Ref<ArgDef> arg_def, Stroka &Oper){
		list<Ref<FuncDef> > funcs = Funcs[Oper];
        if (funcs.empty())
            return 0;
		for (list<Ref<FuncDef> >::iterator iter = funcs.begin();iter!=funcs.end();iter++)
			if (SameArgs(arg_def,(*iter)->Arg))
				return *iter;
		
        return 0;
    }
    void AddFunc(Ref<FuncDef> func, Stroka &Oper){
        Funcs[Oper].push_back(func);
    }
    Stroka OperationsDefined(){
        Stroka Res;
        for(map<Stroka, list<Ref<FuncDef> > >::iterator iter = Funcs.begin();iter!=Funcs.end();iter++){
            Res += iter->first + " : \n";
            for(list<Ref<FuncDef> >::iterator it = iter->second.begin();it!=iter->second.end();it++){
                Res+="\tArgs: ";
                for(list<Ref<DataConstr> >::iterator i = (*it)->Arg->dat.begin();i!=(*it)->Arg->dat.end();i++){
                    if ((*i)->Type)
                        Res += (*i)->Type->TypeName + " ";
                    else
                        Res += "undef ";
                }
                Res+="\n\tRes: ";
                    if ((*it)->Arg->res->Type)
                        Res += (*it)->Arg->res->Type->TypeName + " \n";
                    else
                        Res += "undef \n";
            }
        }
        return Res;
    }

    friend ostream &operator<<(ostream &out, const TypeDef  &type){
        out<<" TypeName <"<<type.TypeName.c_str()<<"> Size <"<<type.DataSize<<"> ";
        return out;
    }
private:
	int SameArgs(Ref<ArgDef> arg1,Ref<ArgDef> arg2){
		if (!arg1 || !arg2) 
			return 0;
        list<Ref<DataConstr> >::iterator iter1 = arg1->dat.begin();
		list<Ref<DataConstr> >::iterator iter2 = arg2->dat.begin();
		do{
			if (!DataConstr::SameType(iter1!=arg1->dat.end()?*iter1:NULL,iter2!=arg2->dat.end()?*iter2:NULL))
				return 0;
			if (iter1!=arg1->dat.end())
				iter1++;
			if (iter2!=arg2->dat.end())
				iter2++;
		}while(!(iter1==arg1->dat.end() && iter2==arg2->dat.end()));
		return 1;
	}
};

struct TypeDefStorage:TypeDef{
    TypeDefStorage():TypeDef("",0){};
    //~TypeDefStorage(){
    //    defs.Delete();
    //    defs_vars.Delete();
    //}
    Ref<DataConstr> GetTypeVar(const Stroka &Name){
		map<Stroka,int>::iterator it = N2Def.find(Name);
		if (it == N2Def.end())
			return NULL;
		return defs_vars[it->second];
	}
    void AddTypeDef(Ref<TypeDef> type){
        int n = defs_vars.SetNumEl();
        defs[n] = type;
        N2Def[type->TypeName] = n;
        defs_vars[n] = new DataConstr(NULL,type,0,NULL);
        for(map<Stroka, list<Ref<FuncDef> > >::iterator iter = type->Funcs.begin();iter!=type->Funcs.end();iter++){
            for(list<Ref<FuncDef> >::iterator it = iter->second.begin();it!=iter->second.end();it++)
                Funcs[iter->first].push_back(*it);
        }

    }
private:
	map<Stroka, int> N2Def;
	ResizeVector<Ref<TypeDef>,CopyStructRef<Ref<TypeDef> > > defs;
	ResizeVector<Ref<DataConstr>, CopyStructRef<Ref<DataConstr> > > defs_vars;
};

struct DataConstrStorage:RefCount{
    DataConstrStorage(){types = new TypeDefStorage;data = new DataSimpleStorage;};
    //~DataConstrStorage(){
    //    data = NULL;
    //    for (int k=0;k<Tmp.SetNumEl();k++) {
    //        cout<<"del:"<<Tmp[k]->nreference()<<" "<<(*Tmp[k])<<" \n";
    //        //Tmp[k]=NULL;
    //    }
    //    Tmp.Delete();
    //    N2D.clear();
    //    types = NULL;
    //}
    void DeleteVar(const char *name){
        if (strlen(name)==0)
            return;
        if ( N2D.find(name)==N2D.end() )
            throw info_except("Trying to delete absent var name %s",name);
        N2D.erase(N2D.find(name));
    }
    Ref<DataConstr> GenerateVar(const Ref<DataConstr> &dat){
		if (!dat)
			throw info_except("Something strange... no data... ups\n");
        const char *name = dat->Name.c_str();
        int size = dat->Type!=NULL?dat->Type->DataSize:NULL;
//        int size = dat->Type->DataSize;
        DataSimpleStorage::Pos pos = data->GenerateVar(size);
        Ref<DataConstr> ret = new DataConstr(pos,dat->Type,dat->Spec,name);
        if (strlen(name)!=0){
            if ( N2D.find(name)!=N2D.end() )
                throw info_except("Trying to make var with the existing name %s",name);
            N2D[Stroka(name)] = ret;
        }
        else{
			Tmp.push(ret);
//cout<<"Added new tmp var :\n";
//for (int k=0;k<Tmp.SetNumEl();k++)
//cout<<"var["<<k<<"]:"<<Tmp[k]->nreference()<<" "<<Tmp[k]->dat.pos<<" "
//<<(*( (Ref<Stroka>*)DataSimpleStorage::GetData(Tmp[k]->dat) ))->c_str()
//<<" "<<(*Tmp[k])<<" \n";
//cout<<"\n";
        }
        return ret;
    }
    Stroka GetTypeName(DataConstr *dat){
        if (!dat) return Stroka(" void ");
        return dat->Type->TypeName;
    }
	Ref<DataConstr> GetTypeDef(const Stroka &str){
        Ref<DataConstr> ret = types->GetTypeVar(str);
        if (!ret)
            throw info_except(" type %s was not found\n",str.c_str());
		return ret;
	}
    bool IsTypeDef(const Stroka &str){
        Ref<DataConstr> ret = types->GetTypeVar(str);
        return (ret!=NULL);
    }

	Ref<DataConstr> GetVar(const Stroka &str){
		map<Stroka, Ref<DataConstr> >::iterator iter = N2D.find(str);
		if (iter==N2D.end())
			return NULL;
		return iter->second;
	}
    DataSimpleStorage* GetData(){
        return data;
    }
    TypeDefStorage* GetTypeDef(){
		return types;
	}
private:
    Ref<DataSimpleStorage> data;
//	ResizeVector<Ref<DataConstr> > Tmp;
	ResizeVector<Ref<DataConstr>, CopyStructRef<Ref<DataConstr> > > Tmp;
	map<Stroka, Ref<DataConstr> > N2D;
    Ref<TypeDefStorage> types;

};



struct FuncArgs:RefCount{
	list<DataSimpleStorage::Pos> dat;
	DataSimpleStorage::Pos res;
    FuncArgs(DataSimpleStorage::Pos r, const list<DataSimpleStorage::Pos> &d):res(r),dat(d){};
};
struct InstrSimple:RefCount{
	void Execute(list<Ref<InstrSimple> >::iterator &it){
		Func->Function(dat,Oper);
        it++;
	}
    InstrSimple():Func(NULL),dat(NULL){};
    //InstrSimple(Ref<TypeDef::FuncDef> DefF, Stroka &oper):Oper(oper){
    //    Func = DefF->Func;
    //    dat = new FuncArgs(DefF->Arg);
    //}
    InstrSimple(Ref<FuncArgs> dat_, FuncStruct* Func_, const Stroka &oper)
        :dat(dat_),Func(Func_),Oper(oper){};
private:
	FuncStruct* Func;
	Stroka Oper;
	Ref<FuncArgs> dat;
	list<Ref<InstrSimple> >::iterator next;
};
struct InstrSimpleProgr:InstrSimple{
    InstrSimpleProgr(){ProgrSimpleData = new DataSimpleStorage;}
	Ref<DataSimpleStorage> ProgrSimpleData;
	list<Ref<InstrSimple> > ProgrSimpleInstr;
// To be compatible
	void Execute(list<Ref<InstrSimple> >::iterator &it){
		//for(list<Ref<InstrSimple> >::iterator iter = ProgrSimpleInstr.begin();iter!=ProgrSimpleInstr.end();)
		//	(*iter)->Execute(iter);
		for(list<Ref<InstrSimple> >::iterator iter = it;iter!=ProgrSimpleInstr.end();)
			(*iter)->Execute(iter);
	}
	void Execute(){
		list<Ref<InstrSimple> >::iterator it = ProgrSimpleInstr.begin();
		Execute(it);
	}
};


struct ConstructProgram:RefCount{
    ConstructProgram(){Clear();}
    ~ConstructProgram(){
        progr = NULL;
        data = NULL;
    }
	Ref<InstrSimpleProgr> progr;
    Ref<DataConstrStorage> data;

    void Clear(){progr = new InstrSimpleProgr;data =  new DataConstrStorage;}
	int AddFunc(DataConstr* dat1, DataConstr* dat2, Stroka Oper,
        Ref<DataConstr> &res){
        list<DataConstr* > lst;
        lst.push_back(dat1);lst.push_back(dat2);
        return AddFunc(lst,Oper,res);
    }
	int AddFunc(const list<DataConstr* > &dat, Stroka Oper, Ref<DataConstr> &res){
  //      Ref<DataConstr> dat1 = dat.front();
		//if (!dat1)
		//	return 0;
		//TypeDef::FuncDef* func = dat1->Type->CheckOperArgs(new TypeDef::ArgDef(NULL,dat), Oper);
        TypeDef::FuncDef* func = data->GetTypeDef()->CheckOperArgs(new TypeDef::ArgDef(NULL,dat), Oper);
		if (!func){
            Error=" Operation: " + Oper + " with arguments: \n";
            int k=0;
            char tmp[256];
            for(list<DataConstr* >::const_iterator it = dat.begin();it!=dat.end();it++,k++)
                Error+=Stroka("\tdat")+Stroka(itoa(k,&tmp[0],10))+": " + data->GetTypeName(*it) + " \n" ;
            Error+="Is not defined\n";
            Error+="Operations defined:\n";
            Error+=data->GetTypeDef()->OperationsDefined();
            return 0;
        }
        res = data->GenerateVar(func->Arg->res);
        list<DataSimpleStorage::Pos> dst;
        for(list<DataConstr* >::const_iterator it = dat.begin();it!=dat.end();it++)
            dst.push_back((*it)->dat);

        progr->ProgrSimpleInstr.push_back(new InstrSimple(new FuncArgs(res->dat,dst),func->Func, Oper));
        return 1;
	};

    void Execute(){
        progr->ProgrSimpleData = data->GetData();
		progr->Execute();
	}
    Stroka Error;



private:

};





