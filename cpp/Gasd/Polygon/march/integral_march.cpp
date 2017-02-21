#include "lib/precompiled/poly_lib.h"


#include "integral_march.h"


static ClassDesc NPolygon_TIntegralMarch_ClassDesc(
    typeid(NPolygon::TIntegralMarch),
    "NPolygon::TIntegralMarch",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TIntegralMarch>);


namespace NPolygon {

    Stroka TIntegralMarch::MakeHelp() {
        Stroka ret("Class to make gasd calculation in the wilkikins model\n");
        return ret;
    }

};   // namespace NPolygon {
