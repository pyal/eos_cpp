#include <lib/precompiled/box_lib.h>
#include <lib/box_lib/ivl_box.h>



MatterConverterSplIO *MatterConverterSplIO::CurPtr = NULL;

static ClassDesc MatterConverterSplIO_ClassDesc(typeid(MatterConverterSplIO),"MatterConverterSplIO",1,"BlackBox_category",create<MatterConverterSplIO>);



MatterConverterSplIO::MatterConverterSplIO()
    : IBlackBoxBase()
    , MisfitFindE(1e-6)
    , NumEGridPnt(100)
    , NumVGridPnt(100)
    , EOSspl("ivl_cvt.spl")
    , IvlParam(new TIvlParametersSplIO)
    , Pspl(new ExtendedRegridSpline)
    , Tspl(new ExtendedRegridSpline) {
}
