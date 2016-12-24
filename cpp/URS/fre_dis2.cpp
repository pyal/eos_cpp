#include <lib/precompiled/eos.h>


#include "fre_dis2.h"

static ClassDesc ClcDissociation_FreeEDis2_ClassDesc(
    typeid(ClcDissociation::FreeEDis2),
    "ClcDissociation::FreeEDis2",
    1,
    "MatterFreeE_category",
    create<ClcDissociation::FreeEDis2>);

namespace ClcDissociation {
    Equilibrium2Pressure *Equilibrium2Pressure::StaticPtr = NULL;
    Equilibrium2Number *Equilibrium2Number::StaticPtr = NULL;
    Equilibrium2NumberMin *Equilibrium2NumberMin::StaticPtr = NULL;
    FreeEDis2::FreeEDis2() : FreeEDisStdIO(){};

};   //ClcDissociation