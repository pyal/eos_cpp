#include <lib\precompiled\eos.h>

#include "fre_anis_deby.h"
#include "fre_multi.h"

//static ClassDesc FreeEIonAdd_ClassDesc(typeid(FreeEIonAdd),"FreeEIonAdd",1,"MatterFreeE_category",create<FreeEIonAdd>);
////static ClassDesc FreeEIon_ClassDesc(typeid(FreeEIon),"FreeEIon",1,"MatterFreeE_category",create<FreeEIon>);
//
static ClassDesc FreeEIonAnisimov_ClassDesc(typeid(FreeEIonAnisimov),"FreeEIonAnisimov",1,"MatterFreeE_category",create<FreeEIonAnisimov>);
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

//namespace AnisimovIon {
//
//    MathAnisIon::MathAnisIon(){};
//
//}; //AnisimovIon


FreeEIonAnisimov::FreeEIonAnisimov():ClcIonisation::FreeEIonStdIO(), CalcMethod(2), 
    MatFreeE(new FreeESumMatter), ElFreeE(new FreeESumMatter){
    N_num.push_back(1);
    L_num.push_back(0);
};

