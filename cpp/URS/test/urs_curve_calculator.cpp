#include "lib/precompiled/urs_curve_lib.h"


#include "urs_curve_calculator.h"

static ClassDesc UrsCurve_Calculator_ClassDesc(
    typeid(UrsCurve_Calculator),
    "UrsCurve_Calculator",
    1,
    "URS_Curve_ClcVar_category",
    create<UrsCurve_Calculator>);


//#include "util/code_gen/calculator/calculator_std.h"
UrsCurve_Calculator::UrsCurve_Calculator() : URS_Curve::ClcVar(), DefaultValue(0){};
