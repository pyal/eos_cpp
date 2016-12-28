#include <cpp/Gasd/Polygon/march/wilkins.h>
#include "lib/precompiled/poly_lib.h"


#include "PolyMarchBase.h"

static ClassDesc NPolygon_TPolyMarchTestZero_ClassDesc(
    typeid(NPolygon::TPolyMarchTestZero),
    "NPolygon::TPolyMarchTestZero",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TPolyMarchTestZero>);

static ClassDesc NPolygon_TPolyMarchBody_ClassDesc(
    typeid(NPolygon::TPolyMarchBody),
    "NPolygon::TPolyMarchBody",
    1,
    "NPolygon::March_category",
    create<NPolygon::TPolyMarchBody>);

namespace NPolygon {

    TPolyMarchBody::TPolyMarchBody()
        : TPolyMarchRegionBase(),
          Marcher(new TMarchWilkins),
          FromTime(0),
          ToTime(10),
          MaxTimeStep(1),
          OutputTime(0.1),
          ResultsFile("output.dat"),
          OutputNames("Pos:Vel:Pres:Denc:Ener:Sound")
    {}


};   //namespace NPolygon {
