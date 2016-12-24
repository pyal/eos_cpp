#include "lib/precompiled/poly_lib.h"


#include "PolyMarch.h"

static ClassDesc NPolygon_TPolyMarchRegionTest_ClassDesc(
    typeid(NPolygon::TPolyMarchRegionTest),
    "NPolygon::TPolyMarchRegionTest",
    1,
    "NPolygon::March_category",
    create<NPolygon::TPolyMarchRegionTest>);

static ClassDesc NPolygon_TPolyMarchBody_ClassDesc(
    typeid(NPolygon::TPolyMarchBody),
    "NPolygon::TPolyMarchBody",
    1,
    "NPolygon::March_category",
    create<NPolygon::TPolyMarchBody>);

namespace NPolygon {

    TPolyMarchBody::TPolyMarchBody()
        : TPolyMarchRegionBase(),
          FromTime(0),
          ToTime(1000),
          MaxTimeStep(1),
          OutputNames("X:Y"),
          ResultsFile("output.dat"),
          Marcher(new TPolyMarchRegionTest) {}


};   //namespace NPolygon {
