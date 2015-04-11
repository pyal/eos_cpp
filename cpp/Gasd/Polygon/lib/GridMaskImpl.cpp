#include "lib/precompiled/poly_lib.h"


#include "GridMaskImpl.h"


static ClassDesc NPolygon_TGridMaskSimple_ClassDesc(typeid(NPolygon::TGridMaskSimple),"NPolygon::TGridMaskSimple",1,"NPolygon::TGridMask_category",create<NPolygon::TGridMaskSimple>);

namespace NPolygon {
    TGridMaskSimple::TGridMaskSimple(int length) : TGridMaskBase(), Beg(0), End(length) {};

}; //namespace NPolygon {