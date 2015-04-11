

#pragma once


//#include "urs_forcelnk.h"

//#include "urs\test\urs_curve.h"
//#include "PolyRegion.h"
//#include "PolyRegConstructor.h"
#include "PolyTest.h"
#include "gasd/polygon/march/PolyMarchTest.h"
#include "gasd/polygon/march/CenterFlux.h"

#include "gasd/polygon/march/Wilkins.h"
#include "gasd/polygon/march/PolyMarchAdd.h"

//#include "PolyMarchBase.h"
//extern ForceLinkVoid<URS_Curve> Dummy_URS_Curve;
namespace NPolygon {
    extern ForceLinkVoid<TPolyRegion> Dummy_TPolyRegion;
    extern ForceLinkVoid<TSimpleContructor> Dummy_TSimpleContructor;
    extern ForceLinkVoid<TSimpleContructor::TRegData> Dummy_SimpleContructor_TRegData;


    extern ForceLinkVoid<TTest> Dummy_TTest;
    extern ForceLinkVoid<TTestSimple> Dummy_TTestSimple;
    extern ForceLinkVoid<TestCase_Gasd_Static> Dummy_TestCase_Gasd_Static;

    //extern ForceLinkVoid<TSimpleContructor> Dummy_TSimpleContructor;
    //extern ForceLinkVoid<TSimpleContructor::TRegData> Dummy_TSimpleContructor_TRegData;
    extern ForceLinkVoid<TGridMaskSimple> Dummy_TGridMaskSimple;
    extern ForceLinkVoid<TGridVar<double> > Dummy_TGridVar_Double;
    extern ForceLinkVoid<TPolyMarchTestZero> Dummy_TPolyMarchTestZero;
    extern ForceLinkVoid<TPolyMarchBody> Dummy_TPolyMarchBody;
    
    extern ForceLinkVoid<TPolyMarchTestU2> Dummy_TPolyMarchTestU2;
    extern ForceLinkVoid<TPolyMarchU2> Dummy_TPolyMarchU2;
    extern ForceLinkVoid<TPolyMarchDriver> Dummy_TPolyMarchDriver;
    extern ForceLinkVoid<TPolyMarchDriverFluxU2> Dummy_TPolyMarchDriverFluxU2;

    extern ForceLinkVoid<TRegBoundaryCircle> Dummy_TRegBoundaryCircle;
    extern ForceLinkVoid<TRegBoundaryCircleX> Dummy_TRegBoundaryCircleX;

    extern ForceLinkVoid<TPolyMarchDriverFluxGasdWilkins> Dummy_TPolyMarchDriverFluxGasdWilkins;
    extern ForceLinkVoid<TMarchWilkins> Dummy_TMarchWilkins;
    extern ForceLinkVoid<TPolyMarchPlusE> Dummy_TPolyMarchPlusE;
	extern ForceLinkVoid<TPolyMarchPlusGase> Dummy_TPolyMarchPlusGase;
    //extern ForceLinkVoid<> Dummy_;
    //extern ForceLinkVoid<> Dummy_;



};

