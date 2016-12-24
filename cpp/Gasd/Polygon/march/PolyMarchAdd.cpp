
#include "PolyMarchAdd.h"
#include "wilkins.h"

static ClassDesc NPolygon_TPolyMarchPlusE_ClassDesc(
    typeid(NPolygon::TPolyMarchPlusE),
    "NPolygon::TPolyMarchPlusE",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TPolyMarchPlusE>);
static ClassDesc NPolygon_TPolyMarchPlusGase_ClassDesc(
    typeid(NPolygon::TPolyMarchPlusGase),
    "NPolygon::TPolyMarchPlusGase",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TPolyMarchPlusGase>);


namespace NPolygon {

    TPolyMarchPlusBase::TPolyMarchPlusBase() : Marcher(new NPolygon::TMarchWilkins) {}

    TPolyMarchPlusE::TPolyMarchPlusE()
        : TPolyMarchPlusBase(),
          YmeanL(1),
          IntegralInt(1e9),
          Tmin(0),
          Tmax(1),
          TsplMisfit(1e-6),
          Xsigma(1),
          Xcenter(1),
          Ysigma(1),
          TimeDepFile("time.dat"),
          RegCoefName("AddECoef"),
          EnergyAddName("Energy"),
          PosName("Pos") {}

};   // namespace NPolygon {
