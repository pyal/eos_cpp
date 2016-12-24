#include <lib/precompiled/eos.h>

#include "matt_h2.h"

static ClassDesc MatterH2Sol_ClassDesc(
    typeid(MatterH2Sol),
    "MatterH2Sol",
    1,
    "Matter_category",
    create<MatterH2Sol>);
static ClassDesc MatterH2Liq_ClassDesc(
    typeid(MatterH2Liq),
    "MatterH2Liq",
    1,
    "Matter_category",
    create<MatterH2Liq>);

MatterH2::MatterH2() : MatterIOFind("depricated.file", "depricated.subst") {
    SetConstants();
};
