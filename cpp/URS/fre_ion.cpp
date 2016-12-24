#include <lib/precompiled/eos.h>

#include "fre_ion.h"


static ClassDesc ClcIonisation_FreeEIonSimple_ClassDesc(
    typeid(ClcIonisation::FreeEIonSimple),
    "ClcIonisation::FreeEIonSimple",
    1,
    "MatterFreeE_category",
    create<ClcIonisation::FreeEIonSimple>);

static ClassDesc ClcIonisation_FreeEIonGrig_ClassDesc(
    typeid(ClcIonisation::FreeEIonGrig),
    "ClcIonisation::FreeEIonGrig",
    1,
    "MatterFreeE_category",
    create<ClcIonisation::FreeEIonGrig>);

static ClassDesc ClcIonisation_OneVarRosLowering_ClassDesc(
    typeid(ClcIonisation::OneVarRosLowering),
    "ClcIonisation::OneVarRosLowering",
    1,
    "OneVarFunction_category",
    create<ClcIonisation::OneVarRosLowering>);

static ClassDesc ClcIonisation_FreeEIonLowVolDepend_ClassDesc(
    typeid(ClcIonisation::FreeEIonLowVolDepend),
    "ClcIonisation::FreeEIonLowVolDepend",
    1,
    "MatterFreeE_category",
    create<ClcIonisation::FreeEIonLowVolDepend>);

static ClassDesc ClcIonisation_FreeEIonDeby_ClassDesc(
    typeid(ClcIonisation::FreeEIonDeby),
    "ClcIonisation::FreeEIonDeby",
    1,
    "MatterFreeE_category",
    create<ClcIonisation::FreeEIonDeby>);

static ClassDesc ClcIonisation_FreeEIonDebyHuckel_ClassDesc(
    typeid(ClcIonisation::FreeEIonDebyHuckel),
    "ClcIonisation::FreeEIonDebyHuckel",
    1,
    "MatterFreeE_category",
    create<ClcIonisation::FreeEIonDebyHuckel>);

static ClassDesc ClcIonisation_FreeEIonDebyAnisimov_ClassDesc(
    typeid(ClcIonisation::FreeEIonDebyAnisimov),
    "ClcIonisation::FreeEIonDebyAnisimov",
    1,
    "MatterFreeE_category",
    create<ClcIonisation::FreeEIonDebyAnisimov>);


static ClassDesc ClcIonisation_AnisimovLoweringCurve_ClassDesc(
    typeid(ClcIonisation::AnisimovLoweringCurve),
    "ClcIonisation::AnisimovLoweringCurve",
    1,
    "NamedOneVarFunction_category",
    create<ClcIonisation::AnisimovLoweringCurve>);

namespace ClcIonisation {

    FreeEIonSimple *FreeEIonSimple::CurPtr = NULL;
    FreeEIonDeby *FreeEIonDeby::CurD = NULL;
    //FreeEIonDebyHuckel *FreeEIonDebyHuckel::CurD = NULL;
    //map<Stroka, Ref<FreeEIonDebyHuckel::LoweringFunc> > FreeEIonDebyHuckel::CorrectFuncs;


};   //ClcIonisation


//static ClassDesc FreeEIonAdd_ClassDesc(typeid(FreeEIonAdd),"FreeEIonAdd",1,"MatterFreeE_category",create<FreeEIonAdd>);
////static ClassDesc FreeEIon_ClassDesc(typeid(FreeEIon),"FreeEIon",1,"MatterFreeE_category",create<FreeEIon>);
//
//static ClassDesc FreeEIonOneMat_ClassDesc(typeid(FreeEIonOneMat),"FreeEIonOneMat",1,"MatterFreeE_category",create<FreeEIonOneMat>);
//
//static ClassDesc FreeEIonLowVolDepend_ClassDesc(typeid(FreeEIonLowVolDepend),"FreeEIonLowVolDepend",1,"MatterFreeE_category",create<FreeEIonLowVolDepend>);
//static ClassDesc RosLowering_ClassDesc(typeid(RosLowering),"RosLowering",1,"OneVarFunction_category",create<RosLowering>);
//
//
//FreeEIonAdd::FreeEIonAdd():FreeEIOFind("material.cfg","MatterFreeEIonAddTmp"),MolVeight(2){
//    IonLevels.push_back(21);
//    Degeneration.push_back(1);
//	SetInternalNames();
//};
////FreeEIon *FreeEIon::CurPtr = NULL;
////double FreeEIon::LastElDencStatic = sqrt(MathZer);
//FreeEIonOneMat *FreeEIonOneMat::CurPtr = NULL;
//double FreeEIonOneMat::LastElDencStatic = sqrt(MathZer);
//
//
////FreeEIonLowVolDepend *FreeEIonLowVolDepend::CurPtr = NULL;
////double FreeEIonLowVolDepend::LastElDencStatic = sqrt(MathZer);
