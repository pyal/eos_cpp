#include <lib/precompiled/eos.h>
#include "fre_url.h"


static ClassDesc FreeEUrlLiquid_ClassDesc(
    typeid(FreeEUrlLiquid),
    "FreeEUrlLiquid",
    1,
    "MatterFreeE_category",
    create<FreeEUrlLiquid>);
static ClassDesc FreeEUrlSolid_ClassDesc(
    typeid(FreeEUrlSolid),
    "FreeEUrlSolid",
    1,
    "MatterFreeE_category",
    create<FreeEUrlSolid>);

FreeEUrlLiquid::FreeEUrlLiquid() : FreeEIOFind("h2_param.cfg", "no_help"){};
