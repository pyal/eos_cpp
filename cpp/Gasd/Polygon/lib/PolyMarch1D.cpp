#include "lib/precompiled/poly_lib.h"


#include "PolyMarch1D.h"


static ClassDesc NPolygon_TPolyMarchU2_ClassDesc(
    typeid(NPolygon::TPolyMarchU2),
    "NPolygon::TPolyMarchU2",
    1,
    "NPolygon::March_category",
    create<NPolygon::TPolyMarchU2>);


namespace NPolygon {

    TPolyMarchU2::TPolyMarchU2() : TPolyMarchRegionBase(){};

};   // namespace NPolygon {
