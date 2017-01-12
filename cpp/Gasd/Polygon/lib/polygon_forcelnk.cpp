#include "lib/precompiled/poly_test.h"
//#include "lib/precompiled/poly_lib.h"


#include "polygon_forcelnk.h"


//ForceLinkVoid<URS_Curve> Dummy_URS_Curve;

namespace NPolygon {
    ForceLinkVoid<TPolyRegion> Dummy_TPolyRegion;
    ForceLinkVoid<TSimpleContructor> Dummy_TSimpleContructor;
    ForceLinkVoid<TSimpleContructor::TRegData> Dummy_SimpleContructor_TRegData;

    ForceLinkVoid<TTest> Dummy_TTest;
    ForceLinkVoid<TTestSimple> Dummy_TTestSimple;
    ForceLinkVoid<TestCase_Gasd_Static> Dummy_TestCase_Gasd_Static;

    ForceLinkVoid<TGridMaskSimple> Dummy_TGridMaskSimple;
    ForceLinkVoid<TGridVar<double>> Dummy_TGridVar_Double;
    ForceLinkVoid<TPolyMarchBody> Dummy_TPolyMarchBody;
    ForceLinkVoid<TPolyMarchTestU2> Dummy_TPolyMarchTestU2;
    ForceLinkVoid<TPolyMarchU2> Dummy_TPolyMarchU2;

    ForceLinkVoid<TPolyMarchDriver> Dummy_TPolyMarchDriver;
    ForceLinkVoid<TPolyMarchDriverFluxU2> Dummy_TPolyMarchDriverFluxU2;


    ForceLinkVoid<TRegBoundaryCircle> Dummy_TRegBoundaryCircle;
    ForceLinkVoid<TRegBoundaryCircleX> Dummy_TRegBoundaryCircleX;

    ForceLinkVoid<TMarchWilkins> Dummy_TMarchWilkins;
    ForceLinkVoid<TMarchExactRieman> Dummy_TMarchExactRieman;
    ForceLinkVoid<TPolyMarchPlusE> Dummy_TPolyMarchPlusE;
    ForceLinkVoid<TPolyMarchPlusGase> Dummy_TPolyMarchPlusGase;
};
