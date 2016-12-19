#include <lib/precompiled/eos.h>

#include "fre_anis_deby.h"

//static ClassDesc FreeEIonAdd_ClassDesc(typeid(FreeEIonAdd),"FreeEIonAdd",1,"MatterFreeE_category",create<FreeEIonAdd>);
////static ClassDesc FreeEIon_ClassDesc(typeid(FreeEIon),"FreeEIon",1,"MatterFreeE_category",create<FreeEIon>);
//
//static ClassDesc FreeEIonOneMat_ClassDesc(typeid(FreeEIonOneMat),"FreeEIonOneMat",1,"MatterFreeE_category",create<FreeEIonOneMat>);
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



namespace ClcIonisation {

    MathAnisIon::MathAnisIon(){};

    Str::OptionSet MathAnisIon::CalcType("DebyAnis Deby NoDeby");
    MathAnisIon *MathAnisIon::CurPtr = NULL;


}; //ClcIonisation