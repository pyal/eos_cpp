#include <lib/precompiled/urs_curve.h>
#include <lib/precompiled/madiex.h>
#include <lib/precompiled/cell_kru.h>
#include <lib/precompiled/cell_dat.h>
#include <lib/precompiled/adi_dou.h>
#include <lib/precompiled/Madi_construction.h>
#include "lib/precompiled/poly_test.h"



#include "urs_forcelnk.h"

// VarFunction_category - category
// Used in Fre_Bus only
ForceLinkVoid<FreeCold_V> Dummy_FreeCold_V;
ForceLinkVoid<TetaExp_Sigma> Dummy_TetaExp_Sigma;
ForceLinkVoid<BfuncE_T> Dummy_BfuncE_T;

ForceLinkVoid<HeatCapacity_SigmaT> Dummy_HeatCapacity_SigmaT;
//ForceLinkVoid<TetaL_SigmaLT> Dummy_TetaL_SigmaLT;
ForceLinkVoid<TetaL_SigmaLT_Short> Dummy_TetaL_SigmaLT_Short;
ForceLinkVoid<FreeM_SigmaMT> Dummy_FreeM_SigmaMT;
ForceLinkVoid<HeatCapacityE_SigmaT> Dummy_HeatCapacityE_SigmaT;
ForceLinkVoid<GammaE_SigmaT> Dummy_GammaE_SigmaT;


//ForceLinkVoid<Cold_VinetE> Dummy_Cold_VinetE;
ForceLinkVoid<ManyVarFunc2OneVar> Dummy_ManyVarFunc2OneVar;
ForceLinkVoid<Spl2DGenerator> Dummy_Spl2DGenerator;
ForceLinkVoid<NoiseRemoval> Dummy_NoiseRemoval;
ForceLinkVoid<PolynomFunc> Dummy_PolynomFunc;
ForceLinkVoid<NoiseWavelet> Dummy_NoiseWavelet;

ForceLinkVoid<ColorTemp> Dummy_ColorTemp;
ForceLinkVoid<PlancEmission> Dummy_PlancEmission;


// OneVarFunction
ForceLinkVoid<Cold_Vinet> Dummy_Cold_Vinet;
ForceLinkVoid<BrigtnesTemp> Dummy_BrigtnesTemp;
ForceLinkVoid<Cold_TF_Kalitkin_P> Dummy_Cold_TF_Kalitkin_P;



// Spline_category - category
// Used in spline urs
ForceLinkVoid<RegridSpline> Dummy_RegridSpline;
ForceLinkVoid<CurveSpline> Dummy_CurveSpline;
ForceLinkVoid<ExtendedRegridSpline> Dummy_ExtendedRegridSpline;


ForceLinkVoid<TExpConverter> Dummy_TExpConverter;
ForceLinkVoid<TAutoExpConverter> Dummy_TAutoExpConverter;
ForceLinkVoid<TEncodedSplineCalculator> Dummy_TEncodedSplineCalculator;
ForceLinkVoid<TEncodedSplineGenerator> Dummy_TEncodedSplineGenerator;
ForceLinkVoid<TSpline2DCalculator> Dummy_TSpline2DCalculator;
ForceLinkVoid<TSpline2DGenerator> Dummy_TSpline2DGenerator;
ForceLinkVoid<TSpline1DCalculator> Dummy_TSpline1DCalculator;
ForceLinkVoid<TSpline1DGenerator> Dummy_TSpline1DGenerator;




// MatterFreeE_category
ForceLinkVoid<FreeEBushman> Dummy_FreeEBushman;

#include "urs/fre_ros.h"
ForceLinkVoid<FreeEPureRoss> Dummy_FreeEPureRoss;
ForceLinkVoid<FreeERossN2> Dummy_FreeERossN2;
ForceLinkVoid<FreeERossH2> Dummy_FreeERossH2;
ForceLinkVoid<FreeERossH2_DisLim0> Dummy_FreeERossH2_DisLim0;
ForceLinkVoid<FreeERossH2_Minim> Dummy_FreeERossH2_Minim;
ForceLinkVoid<FreeERossXe> Dummy_FreeERossXe;
ForceLinkVoid<FreeERossBin> Dummy_FreeERossBin;

#include "fre_url.h"
ForceLinkVoid<FreeEUrlLiquid> Dummy_FreeEUrlLiquid;
ForceLinkVoid<FreeEUrlSolid> Dummy_FreeEUrlSolid;


ForceLinkVoid<FreeETmp> Dummy_FreeETmp;
ForceLinkVoid<FreeEFormula> Dummy_FreeEFormula;
ForceLinkVoid<FreeEIdeal> Dummy_FreeEIdeal;
ForceLinkVoid<FreeEIdealElectron> Dummy_FreeEIdealElectron;
ForceLinkVoid<FreeEVanDerVals> Dummy_FreeEVanDerVals;
ForceLinkVoid<FreeEVirial> Dummy_FreeEVirial;
ForceLinkVoid<FreeEH2Liquid> Dummy_FreeEH2Liquid;
ForceLinkVoid<FreeEH2Solid> Dummy_FreeEH2Solid;
ForceLinkVoid<FreeEEmpiLiquidPres> Dummy_FreeEEmpiLiquidPres;
ForceLinkVoid<FreeEEmpiLiquid> Dummy_FreeEEmpiLiquid;
ForceLinkVoid<FreeEEmpiSolid> Dummy_FreeEEmpiSolid;
ForceLinkVoid<FreeEMetHCold> Dummy_FreeEMetHCold;

//ForceLinkVoid<StdClcDouble> Dummy_StdClcDouble;
//ForceLinkVoid<StdClcUndefined> Dummy_StdClcUndefined;
//ForceLinkVoid<StdClcString> Dummy_StdClcString;
//

ForceLinkVoid<FreeESumMatter> Dummy_FreeESumMatter;
//ForceLinkVoid<FreeEChemMatter> Dummy_FreeEChemMatter;
//ForceLinkVoid<FreeEChemMatterMultiV> Dummy_FreeEChemMatterMultiV;
//ForceLinkVoid<FreeEChemMatterDiss> Dummy_FreeEChemMatterDiss;


//ForceLinkVoid<DissMultiV> Dummy_DissMultiV;
//ForceLinkVoid<ChemMultiV> Dummy_ChemMultiV;

//ForceLinkVoid<FreeEDis> Dummy_FreeEDis;
//ForceLinkVoid<DisFindMinimE> Dummy_DisFindMinimE;
//ForceLinkVoid<PureDis_FindE> Dummy_PureDis_FindE;

//ForceLinkVoid<DisFindH2Minim> Dummy_DisFindH2Minim;

//ForceLinkVoid<Dis_RelatNum_MinimFunc_Single> Dummy_Dis_RelatNum_MinimFunc_Single;
//ForceLinkVoid<Dis_RelatNum_MinimFunc_SingleH2> Dummy_Dis_RelatNum_MinimFunc_SingleH2;
//ForceLinkVoid<PureDis_Minim1Pres> Dummy_PureDis_Minim1Pres;

ForceLinkVoid<ClcDissociation::FreeEDis2> Dummy_FreeEDis2;

ForceLinkVoid<ClcMixture::FreeEMixture> Dummy_FreeEMixture;


//ForceLinkVoid<FreeEIonAdd> Dummy_FreeEIonAdd;
////ForceLinkVoid<FreeEIon> Dummy_FreeEIon;
//ForceLinkVoid<FreeEIonOneMat> Dummy_FreeEIonOneMat;
//ForceLinkVoid<FreeEIonLowVolDepend> Dummy_FreeEIonLowVolDepend;
//ForceLinkVoid<RosLowering> Dummy_RosLowering;


//ForceLinkVoid<FreeEIonAnisimov> Dummy_FreeEIonAnisimov;

ForceLinkVoid<ClcIonisation::FreeEIonSimple> Dummy_ClcIonisation_FreeEIonSimple;
ForceLinkVoid<ClcIonisation::FreeEIonGrig> Dummy_ClcIonisation_FreeEIonGrig;
ForceLinkVoid<ClcIonisation::OneVarRosLowering> Dummy_ClcIonisation_OneVarRosLowering;
ForceLinkVoid<ClcIonisation::FreeEIonLowVolDepend> Dummy_ClcIonisation_FreeEIonLowVolDepend;
ForceLinkVoid<ClcIonisation::FreeEIonDeby> Dummy_ClcIonisation_FreeEIonDeby;
ForceLinkVoid<ClcIonisation::FreeEIonDebyHuckel> Dummy_ClcIonisation_FreeEIonDebyHuckel;
ForceLinkVoid<ClcIonisation::FreeEIonDebyAnisimov> Dummy_ClcIonisation_FreeEIonDebyAnisimov;
ForceLinkVoid<ClcIonisation::AnisimovLoweringCurve> Dummy_ClcIonisation_AnisimovLoweringCurve;


ForceLinkVoid<FreeEElectron> Dummy_FreeEElectron;

// MatterBnd_category
ForceLinkVoid<MatterStable::TwoPhaseBoundary> Dummy_TwoPhaseBoundary;
ForceLinkVoid<MatterLiqSol::LiquidSolidBnd> Dummy_LiquidSolidBnd;

 //Matter_category
#include "matt_h2.h"
ForceLinkVoid<MatterH2Sol> Dummy_MatterH2Sol;
ForceLinkVoid<MatterH2Liq> Dummy_MatterH2Liq;
#include "matt_his.h"
ForceLinkVoid<MatterHisch> Dummy_MatterHisch;
ForceLinkVoid<MatterIVL> Dummy_MatterIVL;
ForceLinkVoid<MatterABuExpFit> Dummy_MatterABuExpFit;
#include "matt_exp.h"
ForceLinkVoid<MatterABuAdd> Dummy_MatterABuAdd;
ForceLinkVoid<MatterABuExp> Dummy_MatterABuExp;
#include "matt_spl.h"
ForceLinkVoid<MatterSpl> Dummy_MatterSpl;
#include "matt_ispl.h"
ForceLinkVoid<TMatterSpl> Dummy_TMatterSpl;
#include "matt_caloric_spl.h"
ForceLinkVoid<MatterCaloricSpl> Dummy_MatterCaloricSpl;

ForceLinkVoid<MatterFreeSpl> Dummy_MatterFreeSpl;

ForceLinkVoid<MatterFreeE> Dummy_MatterFreeE;

ForceLinkVoid<MatterABu> Dummy_MatterABu;
ForceLinkVoid<MatterGas> Dummy_MatterGas;
ForceLinkVoid<MatterBornMaier> Dummy_MatterBornMaier;
ForceLinkVoid<MatterMurnaganBerch> Dummy_MatterMurnaganBerch;
ForceLinkVoid<MatterZubarev> Dummy_MatterZubarev;
ForceLinkVoid<MatterVacuum> Dummy_MatterVacuum;

ForceLinkVoid<Matter2State> Dummy_Matter2State;
ForceLinkVoid<MatterSum> Dummy_MatterSum;
ForceLinkVoid<Matter2StateProb> Dummy_Matter2StateProb;

ForceLinkVoid<MatterStable> Dummy_MatterStable;
ForceLinkVoid<MatterLiqSol> Dummy_MatterLiqSol;
ForceLinkVoid<MatterBinodalSpinodal> Dummy_MatterBinodalSpinodal;
ForceLinkVoid<FreeELiqSol> Dummy_FreeELiqSol;


ForceLinkVoid<OneVarFormula> Dummy_OneVarFormula;


ForceLinkVoid<FreeEElectronStrict> Dummy_FreeEElectronStrict;



//ForceLinkVoid<URS_Curve> Dummy_URS_Curve;
//ForceLinkVoid<URS_Curve::Output> Dummy_URS_Curve_Output;
//ForceLinkVoid<URS_Curve::ClcVar> Dummy_URS_Curve_ClcVar;
//ForceLinkVoid<UrsCurve_StepClc> Dummy_UrsCurve_StepClc;
//ForceLinkVoid<UrsCurve_Caloric> Dummy_UrsCurve_Caloric;
//ForceLinkVoid<UrsCurve_FreeE> Dummy_UrsCurve_FreeE;
//ForceLinkVoid<UrsCurve_EOS_FindE> Dummy_UrsCurve_EOS_FindE;
//ForceLinkVoid<UrsCurve_EOS_Isoentrope> Dummy_UrsCurve_EOS_Isoentrope;
//ForceLinkVoid<UrsCurve_EOS_Hugoniot> Dummy_UrsCurve_EOS_Hugoniot;
//ForceLinkVoid<UrsCurve_FileReader> Dummy_UrsCurve_FileReader;
//ForceLinkVoid<UrsCurve_VarStorage> Dummy_UrsCurve_VarStorage;
//ForceLinkVoid<UrsCurve_Output2Store> Dummy_UrsCurve_Output2Store;
//
//ForceLinkVoid<UrsCurve_Calculator> Dummy_UrsCurve_Calculator;
//
////Tmp
//ForceLinkVoid<UrsCurve_HugCheck> Dummy_UrsCurve_HugCheck;
//ForceLinkVoid<UrsCurve_XY_Test> Dummy_UrsCurve_XY_Test;
//ForceLinkVoid<UrsCurve_SplConstr> Dummy_UrsCurve_SplConstr;
//ForceLinkVoid<EOS_Savable> Dummy_EOS_Savable;
//
//ForceLinkVoid<UrsCurve_PT_Bnd_Constructor> Dummy_UrsCurve_PT_Bnd_Constructor;
//ForceLinkVoid<UrsCurve_PT_Bnd_Binary> Dummy_UrsCurve_PT_Bnd_Binary;
//
