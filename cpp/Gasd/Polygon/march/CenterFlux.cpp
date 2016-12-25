#include "lib/precompiled/poly_lib.h"


#include "CenterFlux.h"


static ClassDesc NPolygon_TPolyMarchU2_ClassDesc(
    typeid(NPolygon::TPolyMarchU2),
    "NPolygon::TPolyMarchU2",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TPolyMarchU2>);


static ClassDesc NPolygon_TGridVar_Double_ClassDesc(
    typeid(NPolygon::TGridVar<double>),
    "NPolygon::TGridVar_Double",
    1,
    "NPolygon::TGridVar_category",
    create<NPolygon::TGridVar<double>>);
static ClassDesc NPolygon_TGridVectorVar_Double_ClassDesc(
    typeid(NPolygon::TGridVectorVar<double>),
    "NPolygon::TGridVectorVar_Double",
    1,
    "NPolygon::TGridVar_category",
    create<NPolygon::TGridVectorVar<double>>);


static ClassDesc NPolygon_TPolyMarchDriver_ClassDesc(
    typeid(NPolygon::TPolyMarchDriver),
    "NPolygon::TPolyMarchDriver",
    1,
    "NPolygon::RegionMarch_category",
    create<NPolygon::TPolyMarchDriver>);

static ClassDesc NPolygon_TPolyMarchDriverFluxU2_ClassDesc(
    typeid(NPolygon::TPolyMarchDriverFluxU2),
    "NPolygon::TPolyMarchDriverFluxU2",
    1,
    "NPolygon::FluxCalculator_category",
    create<NPolygon::TPolyMarchDriverFluxU2>);

static ClassDesc NPolygon_TPolyMarchDriverFluxGasdLagrange_ClassDesc(
    typeid(NPolygon::TPolyMarchDriverFluxGasdLagrange),
    "NPolygon::TPolyMarchDriverFluxGasdLagrange",
    1,
    "NPolygon::FluxCalculator_category",
    create<NPolygon::TPolyMarchDriverFluxGasdLagrange>);


namespace NPolygon {

    TPolyMarchU2::TPolyMarchU2()
        : TPolyMarchRegionBase(),
          ClcForce("FluxLF", "FluxRI"),
          ClcU2(),
          ClcLF("FluxVar"),
          ClcRI("FluxVar", &ClcU2),
          BoundaryMakerY(TRegionBounds(0, -1)),
          MarchFlux("FluxLF"),
          MarchCourant(0.5)
    {}

};   // namespace NPolygon {
