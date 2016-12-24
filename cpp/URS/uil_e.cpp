#include <lib/precompiled/cell_kru.h>


#include "uil_e.h"


static ClassDesc ExternalEnergyInput_ClassDesc(
    typeid(ExternalEnergyInput),
    "ExternalEnergyInput",
    1,
    "ExternalEnergy_category",
    create<ExternalEnergyInput>);


ExternalEnergyInput::ExternalEnergyInput()
    : OneVarFunction(),
      TimeInt(1),
      SpaceMin(1),
      SpaceMax(100),
      TimeMin(0),
      TimeMax(1e-6),
      TimeDepName("e_t.dat"),
      SpaceDepName("e_r.dat") {
    RegNum2Coef[1] = 1;
};
