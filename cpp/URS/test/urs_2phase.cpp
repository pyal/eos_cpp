#include "lib/precompiled/urs_curve_lib.h"

#include "urs_2phase.h"

double ResDencLow, ResDencHgh, FindPres, CurTemp;
InterfaceFreeEIO *FindMat;


static ClassDesc UrsCurve_PT_Bnd_Constructor_ClassDesc(
    typeid(UrsCurve_PT_Bnd_Constructor),
    "UrsCurve_PT_Bnd_Constructor",
    1,
    "URS_Curve_ClcVar_category",
    create<UrsCurve_PT_Bnd_Constructor>);

static ClassDesc UrsCurve_PT_Bnd_Binary_ClassDesc(
    typeid(UrsCurve_PT_Bnd_Binary),
    "UrsCurve_PT_Bnd_Binary",
    1,
    "URS_Curve_ClcVar_category",
    create<UrsCurve_PT_Bnd_Binary>);

static ClassDesc UrsCurve_PT_UnstBnd_Clc_ClassDesc(
    typeid(UrsCurve_PT_UnstBnd_Clc),
    "UrsCurve_PT_UnstBnd_Clc",
    1,
    "URS_Curve_ClcVar_category",
    create<UrsCurve_PT_UnstBnd_Clc>);
static ClassDesc UrsCurve_PT_UnstBnd_Binary_ClassDesc(
    typeid(UrsCurve_PT_UnstBnd_Binary),
    "UrsCurve_PT_UnstBnd_Binary",
    1,
    "URS_Curve_ClcVar_category",
    create<UrsCurve_PT_UnstBnd_Binary>);


static ClassDesc UrsCurve_PT_clc_ClassDesc(
    typeid(UrsCurve_PT_clc),
    "UrsCurve_PT_clc",
    1,
    "URS_Curve_ClcVar_category",
    create<UrsCurve_PT_clc>);

double UrsCurve_PT_Bnd_Constructor::StaticClcError,
    UrsCurve_PT_Bnd_Constructor::StaticDencLow,
    UrsCurve_PT_Bnd_Constructor::StaticDencHgh,
    UrsCurve_PT_Bnd_Constructor::StaticDencFrom,
    UrsCurve_PT_Bnd_Constructor::StaticDencTo;

InterfaceFreeEIO *UrsCurve_PT_Bnd_Constructor::StaticFreeLowPStat,
    *UrsCurve_PT_Bnd_Constructor::StaticFreeHghPStat,
    *UrsCurve_PT_Bnd_Constructor::FindMat;

double UrsCurve_PT_Bnd_Constructor::ResDencLow, UrsCurve_PT_Bnd_Constructor::ResDencHgh,
    UrsCurve_PT_Bnd_Constructor::FindPres, UrsCurve_PT_Bnd_Constructor::CurTemp;


UrsCurve_PT_Bnd_Constructor::UrsCurve_PT_Bnd_Constructor()
    : UrsCurve_EOS_PT_Equilib(),
      ClcError(1e-6),
      FindPDencFrom(1e-5),
      FindPDencTo(5),
      FindGLowPres(0),
      FindGHghPres(1e4),
      StartPres(100) {
    ClcVarNames["Pressure_T"] = 0;
    ClcVarNames["DencityLowP_T"] = 1;
    ClcVarNames["DencityHghP_T"] = 2;
    ClcVarNames["EnergyLowP_T"] = 3;
    ClcVarNames["EnergyHghP_T"] = 4;
};


UrsCurve_PT_clc *UrsCurve_PT_clc::CurPtr = NULL;