#include <lib\precompiled\eos.h>


#include "fre_dis.h"
//
////static ClassDesc FreeESumMatter_ClassDesc(typeid(FreeESumMatter),"FreeESumMatter",1,"MatterFreeE_category",create<FreeESumMatter>);
////static ClassDesc FreeEChemMatterMultiV_ClassDesc(typeid(FreeEChemMatterMultiV),"FreeEChemMatterMultiV",1,"MatterFreeE_category",create<FreeEChemMatterMultiV>);
//static ClassDesc FreeEDis_ClassDesc(typeid(FreeEDis),"FreeEDis",1,"MatterFreeE_category",create<FreeEDis>);
//static ClassDesc DisFindMinimE_ClassDesc(typeid(DisFindMinimE),"DisFindMinimE",1,"DisFindMinimE_category",create<DisFindMinimE>);
////static ClassDesc DisFindH2Minim_ClassDesc(typeid(DisFindH2Minim),"DisFindH2Minim",1,"DisFindMinimE_category",create<DisFindH2Minim>);
//static ClassDesc PureDis_FindE_ClassDesc(typeid(PureDis_FindE),"PureDis_FindE",1,"DisFindMinimE_category",create<PureDis_FindE>);
//
//
//static ClassDesc Dis_RelatNum_MinimFunc_Single_ClassDesc(typeid(Dis_RelatNum_MinimFunc_Single),"Dis_RelatNum_MinimFunc_Single",1,"Dis_RelatNum_MinimFunc_category",create<Dis_RelatNum_MinimFunc_Single>);
//static ClassDesc Dis_RelatNum_MinimFunc_SingleH2_ClassDesc(typeid(Dis_RelatNum_MinimFunc_SingleH2),"Dis_RelatNum_MinimFunc_SingleH2",1,"Dis_RelatNum_MinimFunc_category",create<Dis_RelatNum_MinimFunc_SingleH2>);
//
//static ClassDesc PureDis_Minim1Pres_ClassDesc(typeid(PureDis_Minim1Pres),"PureDis_Minim1Pres",1,"PureDis_MinimPres_category",create<PureDis_Minim1Pres>);




//FreeESumMatter::FreeESumMatter()
//	:FreeEIOFind("material.cfg","MatterSumH2Met"){FreeCoef.push_back(1);FreeVect.push_back(new FreeEIdeal);}


FreeEDis::FreeEDis()
	:FreeEIOFind("material.cfg","MatterChemH2Met"),FreeMinim(new DisFindMinimE()){
	SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),1,0,1,1);
	SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,1,1,1);
	ChemEq eq(Stroka("M1"),-2);eq.AddSubst(Stroka("M2"),1);
	ChemEqs.push_back(eq);
	ClcStartParams();
}

Dis_RelatNum_MinimFunc_Single *Dis_RelatNum_MinimFunc_Single::StaticPtr = NULL;
DisSamePresFinder *DisSamePresFinder::CurPtr = NULL;

PureDis_MinimNum *PureDis_MinimNum::StaticPtr = NULL;
PureDis_Minim1Pres *PureDis_Minim1Pres::StaticPtr = NULL;

