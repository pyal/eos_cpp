#include <lib/precompiled/box_lib.h>


#include "ivl_data.h"


static ClassDesc TIvlParametersSplIO_ClassDesc(
    typeid(TIvlParametersSplIO),
    "TIvlParametersSplIO",
    1,
    "",
    create<TIvlParametersSplIO>);
static ClassDesc TIvlTable2Spl_ClassDesc(
    typeid(TIvlTable2Spl),
    "TIvlTable2Spl",
    1,
    "BlackBox_category",
    create<TIvlTable2Spl>);


IIvlParametersBase::IIvlParametersBase()
    : SavableClass(),
      MinVol(1e-3),
      MaxVol(1e3),
      MinT(50),
      MaxT(1e6),
      NumVolPnt(100),
      NumTempPnt(100),
      pTVname("ivl_ptv.dat"),
      eTVname("ivl_etv.dat") {}
