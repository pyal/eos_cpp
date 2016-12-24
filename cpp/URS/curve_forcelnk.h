

#pragma once

//#include "fre_bus.h"
//#include "mat/spl_fac.h"

#include "urs_forcelnk.h"

#include "urs/test/urs_curve.h"
extern ForceLinkVoid<URS_Curve> Dummy_URS_Curve;
extern ForceLinkVoid<URS_Curve::Output> Dummy_URS_Curve_Output;
extern ForceLinkVoid<URS_Curve::ClcVar> Dummy_URS_Curve_ClcVar;
extern ForceLinkVoid<UrsCurve_StepClc> Dummy_UrsCurve_StepClc;
extern ForceLinkVoid<UrsCurve_Caloric> Dummy_UrsCurve_Caloric;
extern ForceLinkVoid<UrsCurve_FreeE> Dummy_UrsCurve_FreeE;
extern ForceLinkVoid<UrsCurve_EOS_FindE> Dummy_UrsCurve_EOS_FindE;
extern ForceLinkVoid<UrsCurve_EOS_Isoentrope> Dummy_UrsCurve_EOS_Isoentrope;
extern ForceLinkVoid<UrsCurve_EOS_Hugoniot> Dummy_UrsCurve_EOS_Hugoniot;
extern ForceLinkVoid<UrsCurve_FileReader> Dummy_UrsCurve_FileReader;
extern ForceLinkVoid<UrsCurve_VarStorage> Dummy_UrsCurve_VarStorage;
extern ForceLinkVoid<UrsCurve_Output2Store> Dummy_UrsCurve_Output2Store;
extern ForceLinkVoid<UrsCurve_InOut> Dummy_UrsCurve_InOut;

extern ForceLinkVoid<UrsCurve_OneVarFunction> Dummy_UrsCurve_OneVarFunction;
extern ForceLinkVoid<UrsCurve_NamedOneVarFunction> Dummy_UrsCurve_NamedOneVarFunction;


#include "urs/test/urs_curve_calculator.h"
extern ForceLinkVoid<UrsCurve_Calculator> Dummy_UrsCurve_Calculator;

#include "urs/urs_opac_curve.h"
extern ForceLinkVoid<UrsCurve_BrightTempClc> Dummy_UrsCurve_BrightTempClc;


//Tmp
extern ForceLinkVoid<UrsCurve_HugCheck> Dummy_UrsCurve_HugCheck;
extern ForceLinkVoid<UrsCurve_XY_Test> Dummy_UrsCurve_XY_Test;
extern ForceLinkVoid<UrsCurve_SplConstr> Dummy_UrsCurve_SplConstr;

extern ForceLinkVoid<EOS_Savable> Dummy_EOS_Savable;

#include "urs/test/urs_2phase.h"
extern ForceLinkVoid<UrsCurve_PT_Bnd_Constructor> Dummy_UrsCurve_PT_Bnd_Constructor;
extern ForceLinkVoid<UrsCurve_PT_Bnd_Binary> Dummy_UrsCurve_PT_Bnd_Binary;
extern ForceLinkVoid<UrsCurve_PT_UnstBnd_Clc> Dummy_UrsCurve_PT_UnstBnd_Clc;
extern ForceLinkVoid<UrsCurve_PT_UnstBnd_Binary> Dummy_UrsCurve_PT_UnstBnd_Binary;

extern ForceLinkVoid<UrsCurve_PT_clc> Dummy_UrsCurve_PT_clc;
