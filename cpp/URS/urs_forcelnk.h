
#pragma once

#include "fre_bus.h"
#include "mat/spl_fac.h"


// VarFunction_category - category
// Used in Fre_Bus only
extern ForceLinkVoid<FreeCold_V> Dummy_FreeCold_V;
extern ForceLinkVoid<TetaExp_Sigma> Dummy_TetaExp_Sigma;
extern ForceLinkVoid<BfuncE_T> Dummy_BfuncE_T;

extern ForceLinkVoid<HeatCapacity_SigmaT> Dummy_HeatCapacity_SigmaT;
//extern ForceLinkVoid<TetaL_SigmaLT> Dummy_TetaL_SigmaLT;
extern ForceLinkVoid<TetaL_SigmaLT_Short> Dummy_TetaL_SigmaLT_Short;
extern ForceLinkVoid<FreeM_SigmaMT> Dummy_FreeM_SigmaMT;
extern ForceLinkVoid<HeatCapacityE_SigmaT> Dummy_HeatCapacityE_SigmaT;
extern ForceLinkVoid<GammaE_SigmaT> Dummy_GammaE_SigmaT;

// OneVarFunction
#include "mat/OneVarCurves.h"
extern ForceLinkVoid<Cold_Vinet> Dummy_Cold_Vinet;
//extern ForceLinkVoid<Cold_VinetE> Dummy_Cold_VinetE;
extern ForceLinkVoid<Cold_TF_Kalitkin_P> Dummy_Cold_TF_Kalitkin_P;
extern ForceLinkVoid<ManyVarFunc2OneVar> Dummy_ManyVarFunc2OneVar;
extern ForceLinkVoid<PolynomFunc> Dummy_PolynomFunc;
extern ForceLinkVoid<NoiseWavelet> Dummy_NoiseWavelet;


// ManyVarFunction
extern ForceLinkVoid<Spl2DGenerator> Dummy_Spl2DGenerator;
extern ForceLinkVoid<NoiseRemoval> Dummy_NoiseRemoval;

// OneVarFunction
#include "mat/OpacityClc.h"
extern ForceLinkVoid<BrigtnesTemp> Dummy_BrigtnesTemp;
extern ForceLinkVoid<ColorTemp> Dummy_ColorTemp;
extern ForceLinkVoid<PlancEmission> Dummy_PlancEmission;


// Spline_category - category
// Used in spline urs
extern ForceLinkVoid<RegridSpline> Dummy_RegridSpline;
extern ForceLinkVoid<CurveSpline> Dummy_CurveSpline;
extern ForceLinkVoid<ExtendedRegridSpline> Dummy_ExtendedRegridSpline;


#include "mat/spline/spl_inc.h"
extern ForceLinkVoid<TExpConverter> Dummy_TExpConverter;
extern ForceLinkVoid<TAutoExpConverter> Dummy_TAutoExpConverter;
extern ForceLinkVoid<TEncodedSplineCalculator> Dummy_TEncodedSplineCalculator;
extern ForceLinkVoid<TEncodedSplineGenerator> Dummy_TEncodedSplineGenerator;
extern ForceLinkVoid<TSpline2DCalculator> Dummy_TSpline2DCalculator;
extern ForceLinkVoid<TSpline2DGenerator> Dummy_TSpline2DGenerator;
extern ForceLinkVoid<TSpline1DCalculator> Dummy_TSpline1DCalculator;
extern ForceLinkVoid<TSpline1DGenerator> Dummy_TSpline1DGenerator;


// MatterFreeE_category
extern ForceLinkVoid<FreeEBushman> Dummy_FreeEBushman;

#include "urs/fre_ros.h"

extern ForceLinkVoid<FreeEPureRoss> Dummy_FreeEPureRoss;
extern ForceLinkVoid<FreeERossN2> Dummy_FreeERossN2;
extern ForceLinkVoid<FreeERossH2> Dummy_FreeERossH2;
extern ForceLinkVoid<FreeERossH2_DisLim0> Dummy_FreeERossH2_DisLim0;
extern ForceLinkVoid<FreeERossH2_Minim> Dummy_FreeERossH2_Minim;
extern ForceLinkVoid<FreeERossXe> Dummy_FreeERossXe;
extern ForceLinkVoid<FreeERossBin> Dummy_FreeERossBin;


#include "fre_url.h"
extern ForceLinkVoid<FreeEUrlLiquid> Dummy_FreeEUrlLiquid;
extern ForceLinkVoid<FreeEUrlSolid> Dummy_FreeEUrlSolid;


extern ForceLinkVoid<FreeETmp> Dummy_FreeETmp;
extern ForceLinkVoid<FreeEFormula> Dummy_FreeEFormula;
extern ForceLinkVoid<FreeEIdeal> Dummy_FreeEIdeal;
extern ForceLinkVoid<FreeEIdealElectron> Dummy_FreeEIdealElectron;
extern ForceLinkVoid<FreeEVanDerVals> Dummy_FreeEVanDerVals;
extern ForceLinkVoid<FreeEVirial> Dummy_FreeEVirial;
extern ForceLinkVoid<FreeEH2Liquid> Dummy_FreeEH2Liquid;
extern ForceLinkVoid<FreeEH2Solid> Dummy_FreeEH2Solid;
extern ForceLinkVoid<FreeEEmpiLiquidPres> Dummy_FreeEEmpiLiquidPres;
extern ForceLinkVoid<FreeEEmpiLiquid> Dummy_FreeEEmpiLiquid;
extern ForceLinkVoid<FreeEEmpiSolid> Dummy_FreeEEmpiSolid;
extern ForceLinkVoid<FreeEMetHCold> Dummy_FreeEMetHCold;
extern ForceLinkVoid<FreeEElectron> Dummy_FreeEElectron;

#include "fre_multi.h"
extern ForceLinkVoid<FreeESumMatter> Dummy_FreeESumMatter;

// depricated all of them....
//// bad - not working....
////extern ForceLinkVoid<FreeEChemMatter> Dummy_FreeEChemMatter;
//// OK
//extern ForceLinkVoid<FreeEChemMatterMultiV> Dummy_FreeEChemMatterMultiV;
//extern ForceLinkVoid<FreeEChemMatterDiss> Dummy_FreeEChemMatterDiss;
//// classes used by - FreeEChemMatterMultiV
//// OK
//extern ForceLinkVoid<DissMultiV> Dummy_DissMultiV;
//// ?
////extern ForceLinkVoid<ChemMultiV> Dummy_ChemMultiV;


#include "fre_dis.h"
extern ForceLinkVoid<FreeEDis> Dummy_FreeEDis; //TEST ONLY DEPRICATED!!!
//extern ForceLinkVoid<DisFindMinimE> Dummy_DisFindMinimE;
////extern ForceLinkVoid<PureDis_FindE> Dummy_PureDis_FindE;
////extern ForceLinkVoid<DisFindH2Minim> Dummy_DisFindH2Minim;
//
//
//extern ForceLinkVoid<Dis_RelatNum_MinimFunc_Single> Dummy_Dis_RelatNum_MinimFunc_Single;
//extern ForceLinkVoid<Dis_RelatNum_MinimFunc_SingleH2> Dummy_Dis_RelatNum_MinimFunc_SingleH2;
//extern ForceLinkVoid<PureDis_Minim1Pres> Dummy_PureDis_Minim1Pres;


#include "fre_dis2.h"
extern ForceLinkVoid<ClcDissociation::FreeEDis2> Dummy_FreeEDis2;

#include "fre_mixt.h"
extern ForceLinkVoid<ClcMixture::FreeEMixture> Dummy_FreeEMixture;


#include "fre_ion.h"
//extern ForceLinkVoid<FreeEIonAdd> Dummy_FreeEIonAdd;
////extern ForceLinkVoid<FreeEIon> Dummy_FreeEIon;
//extern ForceLinkVoid<FreeEIonOneMat> Dummy_FreeEIonOneMat;
//extern ForceLinkVoid<FreeEIonLowVolDepend> Dummy_FreeEIonLowVolDepend;
//extern ForceLinkVoid<RosLowering> Dummy_RosLowering;
extern ForceLinkVoid<ClcIonisation::FreeEIonSimple> Dummy_ClcIonisation_FreeEIonSimple;
extern ForceLinkVoid<ClcIonisation::FreeEIonGrig> Dummy_ClcIonisation_FreeEIonGrig;
extern ForceLinkVoid<ClcIonisation::OneVarRosLowering>
    Dummy_ClcIonisation_OneVarRosLowering;
extern ForceLinkVoid<ClcIonisation::FreeEIonLowVolDepend>
    Dummy_ClcIonisation_FreeEIonLowVolDepend;
extern ForceLinkVoid<ClcIonisation::FreeEIonDeby> Dummy_ClcIonisation_FreeEIonDeby;
extern ForceLinkVoid<ClcIonisation::FreeEIonDebyHuckel>
    Dummy_ClcIonisation_FreeEIonDebyHuckel;
extern ForceLinkVoid<ClcIonisation::FreeEIonDebyAnisimov>
    Dummy_ClcIonisation_FreeEIonDebyAnisimov;
extern ForceLinkVoid<ClcIonisation::AnisimovLoweringCurve>
    Dummy_ClcIonisation_AnisimovLoweringCurve;


//#include "fre_anis_deby.h"
//extern ForceLinkVoid<FreeEIonAnisimov> Dummy_FreeEIonAnisimov;
//
//#include "clc_anis_deby.h"


// MatterBnd_category
#include "matt_2phase.h"
extern ForceLinkVoid<MatterStable::TwoPhaseBoundary> Dummy_TwoPhaseBoundary;
extern ForceLinkVoid<MatterLiqSol::LiquidSolidBnd> Dummy_LiquidSolidBnd;


// Matter_category
#include "matt_h2.h"
extern ForceLinkVoid<MatterH2Sol> Dummy_MatterH2Sol;
extern ForceLinkVoid<MatterH2Liq> Dummy_MatterH2Liq;
#include "matt_his.h"
extern ForceLinkVoid<MatterHisch> Dummy_MatterHisch;
extern ForceLinkVoid<MatterIVL> Dummy_MatterIVL;
extern ForceLinkVoid<MatterABuExpFit> Dummy_MatterABuExpFit;
#include "matt_exp.h"
extern ForceLinkVoid<MatterABuAdd> Dummy_MatterABuAdd;
extern ForceLinkVoid<MatterABuExp> Dummy_MatterABuExp;
#include "matt_spl.h"
extern ForceLinkVoid<MatterSpl> Dummy_MatterSpl;
#include "matt_ispl.h"
extern ForceLinkVoid<TMatterSpl> Dummy_TMatterSpl;
#include "matt_caloric_spl.h"
extern ForceLinkVoid<MatterCaloricSpl> Dummy_MatterCaloricSpl;
#include "mat_fre_spl.h"
extern ForceLinkVoid<MatterFreeSpl> Dummy_MatterFreeSpl;


extern ForceLinkVoid<MatterFreeE> Dummy_MatterFreeE;

extern ForceLinkVoid<MatterABu> Dummy_MatterABu;
extern ForceLinkVoid<MatterGas> Dummy_MatterGas;
extern ForceLinkVoid<MatterBornMaier> Dummy_MatterBornMaier;
extern ForceLinkVoid<MatterMurnaganBerch> Dummy_MatterMurnaganBerch;
extern ForceLinkVoid<MatterZubarev> Dummy_MatterZubarev;
extern ForceLinkVoid<MatterVacuum> Dummy_MatterVacuum;


#include "matt_internal.h"
extern ForceLinkVoid<Matter2State> Dummy_Matter2State;
extern ForceLinkVoid<MatterSum> Dummy_MatterSum;
extern ForceLinkVoid<Matter2StateProb> Dummy_Matter2StateProb;

#include "matt_2phase.h"
extern ForceLinkVoid<MatterStable> Dummy_MatterStable;
extern ForceLinkVoid<MatterLiqSol> Dummy_MatterLiqSol;
extern ForceLinkVoid<MatterBinodalSpinodal> Dummy_MatterBinodalSpinodal;
//#ifdef InternalPointsUrs
//#endif
extern ForceLinkVoid<FreeELiqSol> Dummy_FreeELiqSol;

#include "mat/fun_fac.h"
extern ForceLinkVoid<OneVarFormula> Dummy_OneVarFormula;


#include "FreeE_Electron.h"
extern ForceLinkVoid<FreeEElectronStrict> Dummy_FreeEElectronStrict;

//
//#include "urs/test/urs_curve.h"
//extern ForceLinkVoid<URS_Curve> Dummy_URS_Curve;
//extern ForceLinkVoid<URS_Curve::Output> Dummy_URS_Curve_Output;
//extern ForceLinkVoid<URS_Curve::ClcVar> Dummy_URS_Curve_ClcVar;
//extern ForceLinkVoid<UrsCurve_StepClc> Dummy_UrsCurve_StepClc;
//extern ForceLinkVoid<UrsCurve_Caloric> Dummy_UrsCurve_Caloric;
//extern ForceLinkVoid<UrsCurve_FreeE> Dummy_UrsCurve_FreeE;
//extern ForceLinkVoid<UrsCurve_EOS_FindE> Dummy_UrsCurve_EOS_FindE;
//extern ForceLinkVoid<UrsCurve_EOS_Isoentrope> Dummy_UrsCurve_EOS_Isoentrope;
//extern ForceLinkVoid<UrsCurve_EOS_Hugoniot> Dummy_UrsCurve_EOS_Hugoniot;
//extern ForceLinkVoid<UrsCurve_FileReader> Dummy_UrsCurve_FileReader;
//extern ForceLinkVoid<UrsCurve_VarStorage> Dummy_UrsCurve_VarStorage;
//extern ForceLinkVoid<UrsCurve_Output2Store> Dummy_UrsCurve_Output2Store;
//
//
//#include "urs/test/urs_curve_calculator.h"
//extern ForceLinkVoid<UrsCurve_Calculator> Dummy_UrsCurve_Calculator;
//
////Tmp
//extern ForceLinkVoid<UrsCurve_HugCheck> Dummy_UrsCurve_HugCheck;
//extern ForceLinkVoid<UrsCurve_XY_Test> Dummy_UrsCurve_XY_Test;
//extern ForceLinkVoid<UrsCurve_SplConstr> Dummy_UrsCurve_SplConstr;
//
//extern ForceLinkVoid<EOS_Savable> Dummy_EOS_Savable;
//
//#include "urs/test/urs_2phase.h"
//extern ForceLinkVoid<UrsCurve_PT_Bnd_Constructor> Dummy_UrsCurve_PT_Bnd_Constructor;
//extern ForceLinkVoid<UrsCurve_PT_Bnd_Binary> Dummy_UrsCurve_PT_Bnd_Binary;
