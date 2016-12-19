#include <lib/precompiled/eos.h>


#include "fre_multi.h"

static ClassDesc FreeESumMatter_ClassDesc(typeid(FreeESumMatter),"FreeESumMatter",1,"MatterFreeE_category",create<FreeESumMatter>);
//static ClassDesc FreeEChemMatterMultiV_ClassDesc(typeid(FreeEChemMatterMultiV),"FreeEChemMatterMultiV",1,"MatterFreeE_category",create<FreeEChemMatterMultiV>);
//static ClassDesc FreeEChemMatterDiss_ClassDesc(typeid(FreeEChemMatterDiss),"FreeEChemMatterDiss",1,"MatterFreeE_category",create<FreeEChemMatterDiss>);




FreeESumMatter::FreeESumMatter()
	:FreeEIOFind("material.cfg","MatterSumH2Met"){FreeCoef.push_back(1);FreeVect.push_back(new FreeEIdeal);}



//static ClassDesc DissMultiV_ClassDesc(typeid(DissMultiV),"DissMultiV",1,"ChemReactMinimFreeE_category",create<DissMultiV>);
//static ClassDesc ChemMultiV_ClassDesc(typeid(ChemMultiV),"ChemMultiV",1,"ChemReactMinimFreeE_category",create<ChemMultiV>);
//ChemEqMinim *ChemEqMinim::StaticPtr;
//ReactCoefMinimFunc *ReactCoefMinimFunc::FunPtr;

SingleDiss_MinimFunc * SingleDiss_MinimFunc::StaticPtr;
RelatVolumeMinimFunc *RelatVolumeMinimFunc::FunPtr;

FreeEChemMatterMultiV::FreeEChemMatterMultiV()
	:FreeEIOFind("material.cfg","MatterChemH2Met"),FreeMinim(new DissMultiV()){
	SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),2,1,1,1);
	SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,0,1,1);
	ChemEq eq(Stroka("M1"),-1);eq.AddSubst(Stroka("M2"),1);
	ChemEqs.push_back(eq);
	ClcStartParams();
}

//static ClassDesc FreeEChemMatter_ClassDesc(typeid(FreeEChemMatter),"FreeEChemMatter",1,"MatterFreeE_category",create<FreeEChemMatter>);
//FreeEChemMatter::FreeEChemMatter():FreeEIOFind("material.cfg","MatterChemH2Met"){
//	SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),2,1);
//	SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,0);
//	ChemEq eq(Stroka("M1"),-1);eq.AddSubst(Stroka("M2"),1);
//	ChemEqs.push_back(eq);
//	ClcStartParams();
//}

VecCl RelatVolumeMinimFunc::RelatVolumesCoefs;
//size_t DissMultiV::CallNum = 0;