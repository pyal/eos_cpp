#pragma once

#include "calc_prg.h"


struct StdClcTypes:DescribedClass{
    Ref<TypeDef> type;
    static void AddStdTypes(TypeDefStorage* stor){
        Ref<StdClcTypes> ptr;
        for(ClassDesc *desc = ClassDesc::list_category_start("StdClcTypes_category");desc;desc = ClassDesc::list_category_next(desc,"StdClcTypes_category")){
            ptr << desc->create();
            if (ptr)
                stor->AddTypeDef(ptr->type);
        }
    }
};

struct StdClcUndefined:StdClcTypes{
    StdClcUndefined();//{Init();};
    void Init(){
        type = new TypeDef(Stroka("undef"), 0);
    }
};


struct StdClcDouble:StdClcTypes{
    StdClcDouble(){Init();};
    ~StdClcDouble(){
        type = NULL;
    }
    struct Operations:FuncStruct{
        Operations(){};
        void Function(Ref<FuncArgs> args,Stroka Oper){
            list<DataSimpleStorage::Pos>::iterator it = args->dat.begin();
            double *res = (double*)DataSimpleStorage::GetData(args->res);
            double *d1  = (double*)DataSimpleStorage::GetData(*it);it++;

            if (Oper==Stroka("print")){
                cout<<*d1<<"\n";
                return;
            }
            if (Oper==Stroka("log")){
                if ((*d1)<0)
                    *res = log(M_MinDouble)-100*fabs(*d1);
                else
                    *res = log(*d1);
                return;
            }
            if (Oper==Stroka("exp")){
                if ((*d1)>300) //log(1/M_MinDouble)~log(0.5e-308)~300
                    *res = 1/M_MinDouble;
                else
                    *res = exp(*d1);
                return;
            }
            if (Oper==Stroka("sin")){
                *res = sin(*d1);
                return;
            }
            if (Oper==Stroka("cos")){
                *res = cos(*d1);
                return;
            }
            if (Oper==Stroka("tan")){
                *res = tan(*d1);
                return;
            }
            if (Oper==Stroka("asin")){
                *res = asin(*d1);
                return;
            }
            if (Oper==Stroka("acos")){
                *res = acos(*d1);
                return;
            }
            if (Oper==Stroka("atan")){
                *res = atan(*d1);
                return;
            }
            if (Oper==Stroka("rnd")){
                *res = RandomClass::Rnd(*d1);
                return;
            }
            if (Oper==Stroka("teta")){
                *res = *d1<0?0:1;
                return;
            }
            if (Oper==Stroka("abs")){
                *res = fabs(*d1);
                return;
            }

            double *d2  = (double*)DataSimpleStorage::GetData(*it);
            if (Oper==Stroka("+")){
                *res = *d1+*d2;
                return;
            }
            if (Oper==Stroka("-")){
                *res = *d1-*d2;
                return;
            }
            if (Oper==Stroka("/")){
                *res = *d1/(*d2);
                return;
            }
            if (Oper==Stroka("*")){
                *res = *d1 *(*d2);
                return;
            }
            if (Oper==Stroka("^")){
                *res = pow(*d1,*d2);
                return;
            }
            if (Oper==Stroka("min")){
                *res = min(*d1,*d2);
                return;
            }
            if (Oper==Stroka("max")){
                *res = max(*d1,*d2);
                return;
            }

            if (Oper==Stroka("=")){
                *res = *d1 = *d2;
                return;
            }
        }
    };
    void Init(){
        type = new TypeDef(Stroka("double"),sizeof(double));
        DataConstr dat(NULL,type,0,NULL);
        list<DataConstr*> lst;
        lst.push_back(&dat);lst.push_back(&dat);
        Ref<FuncStruct> func= new Operations;
        Ref<TypeDef::ArgDef> arg = new TypeDef::ArgDef(&dat,lst);
        Ref<TypeDef::FuncDef> f_arg = new TypeDef::FuncDef(arg,func);
        type->AddFunc(f_arg,Stroka("="));
        type->AddFunc(f_arg,Stroka("+"));
        type->AddFunc(f_arg,Stroka("-"));
        type->AddFunc(f_arg,Stroka("*"));
        type->AddFunc(f_arg,Stroka("/"));
        type->AddFunc(f_arg,Stroka("^"));
        type->AddFunc(f_arg,Stroka("min"));
        type->AddFunc(f_arg,Stroka("max"));


        lst.pop_front();
        f_arg = new TypeDef::FuncDef(new TypeDef::ArgDef(&dat,lst) ,func);
        type->AddFunc(f_arg,Stroka("log"));
        type->AddFunc(f_arg,Stroka("exp"));
        type->AddFunc(f_arg,Stroka("sin"));
        type->AddFunc(f_arg,Stroka("cos"));
        type->AddFunc(f_arg,Stroka("tan"));
        type->AddFunc(f_arg,Stroka("atan"));
        type->AddFunc(f_arg,Stroka("asin"));
        type->AddFunc(f_arg,Stroka("acos"));
        type->AddFunc(f_arg,Stroka("rnd"));
        type->AddFunc(f_arg,Stroka("teta"));
        type->AddFunc(f_arg,Stroka("abs"));
        DataConstr tmp(NULL,NULL,0,NULL);
        f_arg = new TypeDef::FuncDef( new TypeDef::ArgDef(&tmp ,lst), func );
        type->AddFunc(f_arg,Stroka("print"));
   }
};

struct StdClcString:StdClcTypes{
    StdClcString(){Init();};
    struct Operations:FuncStruct{
        Operations(){};
        void Function(Ref<FuncArgs> args,Stroka Oper){
            list<DataSimpleStorage::Pos>::iterator it = args->dat.begin();
            Ref<Stroka> d1  = *(Ref<Stroka>*)DataSimpleStorage::GetData(*it);it++;
            if (Oper==Stroka("print")){
                cout<<d1->c_str()<<"\n";
                return;
            }
            Ref<Stroka> res = *(Ref<Stroka>*)DataSimpleStorage::GetData(args->res);
            Ref<Stroka> d2  = *(Ref<Stroka>*)DataSimpleStorage::GetData(*it);
            if (Oper==Stroka("+")){
                *res = *d1+*d2;
                return;
            }
            if (Oper==Stroka("=")){
                *res = *d1 = *d2;
                return;
            }
        }
    };
    void Init(){
        type = new TypeDef(Stroka("string"),sizeof(Ref<Stroka>*));
        DataConstr dat(NULL,type,0,NULL);
        list<DataConstr*> lst;lst.push_back(&dat);lst.push_back(&dat);
        Ref<FuncStruct> func= new Operations;
        Ref<TypeDef::ArgDef> arg = new TypeDef::ArgDef(&dat,lst);
        Ref<TypeDef::FuncDef> f_arg = new TypeDef::FuncDef(arg,func);
        type->AddFunc(f_arg,Stroka("+"));
        type->AddFunc(f_arg,Stroka("="));

        lst.pop_front();
        DataConstr tmp(NULL,NULL,0,NULL);
        f_arg = new TypeDef::FuncDef( new TypeDef::ArgDef(&tmp ,lst), func );
        type->AddFunc(f_arg,Stroka("print"));
    }
};

//struct StdClcUndefined:StdClcTypes{
//    StdClcUndefined(){Init();};
//    void Init(){
//        type = new TypeDef(Stroka("undef"),NULL);
//    }
//};

