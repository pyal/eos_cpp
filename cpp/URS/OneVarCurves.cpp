#include <lib/precompiled/eos.h>

#include "OneVarCurves.h"

static ClassDesc Cold_Vinet_ClassDesc(
    typeid(Cold_Vinet),
    "Cold_Vinet",
    1,
    "NamedOneVarFunction_category",
    create<Cold_Vinet>);
//static ClassDesc Cold_VinetE_ClassDesc(typeid(Cold_VinetE),"Cold_VinetE",1,"OneVarFunction_category",create<Cold_VinetE>);
static ClassDesc Cold_TF_Kalitkin_P_ClassDesc(
    typeid(Cold_TF_Kalitkin_P),
    "Cold_TF_Kalitkin_P",
    1,
    "NamedOneVarFunction_category",
    create<Cold_TF_Kalitkin_P>);
static ClassDesc ManyVarSplGenerator_ClassDesc(
    typeid(ManyVarSplGenerator),
    "ManyVarSplGenerator",
    1,
    "ManyVarFunction_category",
    create<ManyVarSplGenerator>);
static ClassDesc NoiseRemoval_ClassDesc(
    typeid(NoiseRemoval),
    "NoiseRemoval",
    1,
    "ManyVarFunction_category",
    create<NoiseRemoval>);


Cold_Vinet::Cold_Vinet()
    : NamedOneVarFunction("Pressure Energy"),
      B0(6.36),
      B0prime(7.61),
      V0(102.128),
      MolVeight(10){};
