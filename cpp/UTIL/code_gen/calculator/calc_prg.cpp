#include <lib/precompiled/calc_lib.h>

#include "calc_prg.h"


ostream &operator<<(ostream &out, const DataConstr &dat) {
    out << dat.Name.c_str() << ": Type " << dat.Type->TypeName << " Val " << *(double *)DataSimpleStorage::GetData(dat.dat);
    return out;
}
