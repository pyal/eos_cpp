#include "lib/precompiled/poly_lib.h"


#include "PolyMarchBase.h"

static ClassDesc NPolygon_TPolyMarchTestZero_ClassDesc(typeid(NPolygon::TPolyMarchTestZero), "NPolygon::TPolyMarchTestZero", 1, "NPolygon::RegionMarch_category", create<NPolygon::TPolyMarchTestZero>);

static ClassDesc NPolygon_TPolyMarchBody_ClassDesc(typeid(NPolygon::TPolyMarchBody), "NPolygon::TPolyMarchBody", 1, "NPolygon::March_category", create<NPolygon::TPolyMarchBody>);

namespace NPolygon {

    TPolyMarchBody::TPolyMarchBody() : TPolyMarchRegionBase()
        , FromTime(0)
        , ToTime(1000)
        , MaxTimeStep(1)
        , OutputTime(0)
        , OutputNames("X:Y")
        , ResultsFile("output.dat")
        , Marcher(new TPolyMarchTestZero) {
    }


}; //namespace NPolygon {
