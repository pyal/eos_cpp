#include "lib/precompiled/poly_lib.h"


#include "PolyMarchTest.h"


static ClassDesc NPolygon_TPolyMarchTestU2_ClassDesc(
    typeid(NPolygon::TPolyMarchTestU2),
    "NPolygon::TPolyMarchTestU2",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TPolyMarchTestU2>);


namespace NPolygon {

    TPolyMarchTestU2::TPolyMarchTestU2() : TPolyMarchRegionBase(){};

};   // namespace NPolygon {
