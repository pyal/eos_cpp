#include <lib/precompiled/math.h>
#include "spl_encode.h"


static ClassDesc TEncodedSplineCalculator_ClassDesc(
    typeid(TEncodedSplineCalculator),
    "TEncodedSplineCalculator",
    1,
    "SplineCalculator_category",
    create<TEncodedSplineCalculator>);
static ClassDesc TEncodedSplineGenerator_ClassDesc(
    typeid(TEncodedSplineGenerator),
    "TEncodedSplineGenerator",
    1,
    "SplineGenerator_category",
    create<TEncodedSplineGenerator>);

TEncodedSplineGenerator::TEncodedSplineGenerator()
    : ISplineGenerator(), SplineGen(new TSpline1DGenerator) {
    SetDumpCvtVec();
}
