#include "lib/precompiled/poly_lib.h"


#include "PolyRegion.h"

namespace NPolygon {

static ClassDesc NPolygon_PolyRegion_ClassDesc(typeid(NPolygon::TPolyRegion),"NPolygon::TPolyRegion",1,"NPolygon::PolyRegion_category",create<NPolygon::TPolyRegion>);


    TPolyRegion::TPolyRegion() : SavableClass(), Parent(NULL) {

    };


}; //namespace NPolygon {



