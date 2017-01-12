#include "lib/precompiled/poly_lib.h"


#include "wilkins.h"


static ClassDesc NPolygon_TMarchWilkins_ClassDesc(
    typeid(NPolygon::TMarchWilkins),
    "NPolygon::TMarchWilkins",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TMarchWilkins>);


namespace NPolygon {

    Stroka TMarchWilkins::MakeHelp() {
        Stroka ret("Class to make gasd calculation in the wilkikins model\n");
        return ret;
    }

};   // namespace NPolygon {
