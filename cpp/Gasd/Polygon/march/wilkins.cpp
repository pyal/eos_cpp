#include "lib/precompiled/poly_lib.h"


#include "wilkins.h"


static ClassDesc NPolygon_TPolyMarchDriverFluxGasdWilkins_ClassDesc(
    typeid(NPolygon::TPolyMarchDriverFluxGasdWilkins),
    "NPolygon::TPolyMarchDriverFluxGasdWilkins",
    1,
    "NPolygon::FluxCalculator_category",
    create<NPolygon::TPolyMarchDriverFluxGasdWilkins>);
static ClassDesc NPolygon_TMarchWilkins_ClassDesc(
    typeid(NPolygon::TMarchWilkins),
    "NPolygon::TMarchWilkins",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TMarchWilkins>);


namespace NPolygon {

    //Stroka TPolyMarchDriverFluxGasdWilkins::GetVarName() {
    //    return CombiVar;
    //};
    Stroka TMarchWilkins::MakeHelp() {
        Stroka ret("Class to make gasd calculation in the wilkikins model\n");
        return ret;
    }

};   // namespace NPolygon {
