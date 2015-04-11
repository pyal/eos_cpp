#include <lib\precompiled\calc_lib.h>

#include "calc_prg.h"


ostream &operator<<(ostream &out, const DataConstr& dat){
    out<<" Data: { Name <"<<dat.Name.c_str()<<"> Type <"<<*dat.Type<<"> ";
    return out;
}
