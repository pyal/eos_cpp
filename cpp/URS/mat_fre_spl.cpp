#include <lib/precompiled/eos.h>

#include "mat_fre_spl.h"


static ClassDesc MatterFreeSpl_ClassDesc(typeid(MatterFreeSpl),"MatterFreeSpl",1,"MatterFreeE_category",create<MatterFreeSpl>);

MatterFreeSpl::MatterFreeSpl(): FreeEIO(), spl_name("FreeE.spl"), F_rt(NULL){};
