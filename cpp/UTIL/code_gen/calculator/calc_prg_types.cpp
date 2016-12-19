#include <lib/precompiled/calc_lib.h>

#include "calc_prg_types.h"



static ClassDesc StdClcString_ClassDesc(typeid(StdClcString),"StdClcString",1,"StdClcTypes_category",create<StdClcString>);
static ClassDesc StdClcDouble_ClassDesc(typeid(StdClcDouble),"StdClcDouble",1,"StdClcTypes_category",create<StdClcDouble>);
static ClassDesc StdClcUndefined_ClassDesc(typeid(StdClcUndefined),"StdClcUndefined",1,"StdClcTypes_category",create<StdClcUndefined>);

//TypeDefStorage *StdClcTypes::Ptr=NULL;

StdClcUndefined::StdClcUndefined(){Init();};
void StdClcUndefined::Init(){
    type = new TypeDef(Stroka("undef"), 0);
}

StdClcDouble::StdClcDouble(){Init();};
