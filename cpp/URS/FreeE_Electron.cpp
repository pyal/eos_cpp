#include <lib/precompiled/eos.h>

#include "FreeE_Electron.h"


static ClassDesc FreeEElectronStrict_ClassDesc(
    typeid(FreeEElectronStrict),
    "FreeEElectronStrict",
    1,
    "MatterFreeE_category",
    create<FreeEElectronStrict>);

FreeEElectronStrict *FreeEElectronStrict::Current_FreeEElectronStrict = NULL;
