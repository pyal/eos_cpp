#ifndef _FAMESSB_TEST_H_
#define _FAMESSB_TEST_H_

#include "lib\test_fact\test_case.h"
#include "test_urs_resource.h"
#include "lib\test_fact\resource_reader.h"
extern int CopyFile(const char *src,const char *dst);




struct ResourcesStruct{int data;int config;double err;const char *Name;};

struct TestCase_Urs_Static:TestCase_Std//TestCase_Buffered
{
    static const char *TestExeOutput;
    static int DeleteGoodResults;
    TestCase_Urs_Static(){};//:manip(NULL),in(NULL),buf(NULL){};
	~TestCase_Urs_Static(){Finish();}
	TestSuite* CreateTest()
	{ 
		TestSuite_Std* suite=new TestSuite_Std(class_name());
		suite->AddTest(Test_Func,"Test_Func");
		return suite;
	}
	void Init()	{		Finish();	}
    void Finish()	{};//{delete in;in=NULL;delete buf;buf=NULL;delete manip;manip=NULL;	}
    void Text_Func_Out(ResourcesStruct *resources,int Length,const char *name);
	static TestCase_Urs_Static* Restore(DescribedClass *ptr)
	{
		TestCase_Urs_Static *cur=dynamic_cast<TestCase_Urs_Static*>(ptr);
		if (!cur) throw(info_except("ptr is not TestCase_Urs_Static"));
		return cur;
	}
	static void Test_Func(TestCase *ptr)
	{ 
	    TestCase_Urs_Static *cur=Restore(ptr);
        unlink(TestExeOutput);
        string err,ResErr,Message;

// Simple type
        ResourcesStruct ABUSTEEL_resources[]={
            {ABUSTEEL_HUG_TEST, ABUSTEEL_HUG_CFG, 1e-6,"ABuSteel.Hug"},
            {ABUSTEEL_ISOE_TEST, ABUSTEEL_ISOE_CFG, 1e-6,"ABuSteel.IsoE"}
        };
        cur->Text_Func_Out(&ABUSTEEL_resources[0],sizeof(ABUSTEEL_resources)/sizeof(ResourcesStruct),"ABUSTEEL");
        ResourcesStruct FREEEH2ELECTRON_resources[]={
            {FREEEH2ELECTRON_HUG_TEST, FREEEH2ELECTRON_HUG_CFG, 1e-6,"FreeEH2Electron.Hug"},
            {FREEEH2ELECTRON_ISOT_TEST, FREEEH2ELECTRON_ISOT_CFG, 1e-6,"FreeEH2Electron.IsoT"}
        };
        cur->Text_Func_Out(&FREEEH2ELECTRON_resources[0],sizeof(FREEEH2ELECTRON_resources)/sizeof(ResourcesStruct),"FREEEH2ELECTRON");
        ResourcesStruct FREEEIDEAL_resources[]={
            {FREEEIDEALH2_HUG_TEST, FREEEIDEALH2_HUG_CFG, 1e-6,"FreeEIdealH2.Hug"},
            {FREEEIDEALH2_ISOT_TEST, FREEEIDEALH2_ISOT_CFG, 1e-6,"FreeEIdealH2.IsoT"}
        };
        cur->Text_Func_Out(&FREEEIDEAL_resources[0],sizeof(FREEEIDEAL_resources)/sizeof(ResourcesStruct),"FREEEIDEAL");
        ResourcesStruct GASONEATOM_resources[]={
            {GASONEATOM_HUG_TEST, GASONEATOM_HUG_CFG, 1e-6,"GasOneAtom.Hug"},
            {GASONEATOM_ISOE_TEST, GASONEATOM_ISOE_CFG, 1e-6,"GasOneAtom.IsoE"}
        };
        cur->Text_Func_Out(&GASONEATOM_resources[0],sizeof(GASONEATOM_resources)/sizeof(ResourcesStruct),"GASONEATOM");
        ResourcesStruct VANDERVALSN2_resources[]={
            {VANDERVALSN2_HUG_TEST, VANDERVALSN2_HUG_CFG, 1e-6,"VanDerValsN2.Hug"},
            {VANDERVALSN2_ISOT_TEST, VANDERVALSN2_ISOT_CFG, 1e-6,"VanDerValsN2.IsoT"}
        };
        cur->Text_Func_Out(&VANDERVALSN2_resources[0],sizeof(VANDERVALSN2_resources)/sizeof(ResourcesStruct),"VANDERVALSN2");
        ResourcesStruct FREEEMETHCOLD_resources[]={
            {FREEEMETHCOLD_HUG_TEST, FREEEMETHCOLD_HUG_CFG, 1e-6,"FreeEMetHCold.Hug"},
            {FREEEMETHCOLD_ISOT_TEST, FREEEMETHCOLD_ISOT_CFG, 1e-6,"FreeEMetHCold.IsoT"}
        };
        cur->Text_Func_Out(&FREEEMETHCOLD_resources[0],sizeof(FREEEMETHCOLD_resources)/sizeof(ResourcesStruct),"FREEEMETHCOLD");
        ResourcesStruct MURNAGANGEKS_resources[]={
            {MURNAGANGEKS_HUG_TEST, MURNAGANGEKS_HUG_CFG, 1e-6,"MurnaganGeks.Hug"},
            {MURNAGANGEKS_ISOE_TEST, MURNAGANGEKS_ISOE_CFG, 1e-6,"MurnaganGeks.IsoE"}
        };
        cur->Text_Func_Out(&MURNAGANGEKS_resources[0],sizeof(MURNAGANGEKS_resources)/sizeof(ResourcesStruct),"MURNAGANGEKS");
        ResourcesStruct ZUBAREVGEKS_resources[]={
            {ZUBAREVGEKS_HUG_TEST, ZUBAREVGEKS_HUG_CFG, 1e-6,"ZubarevGeks.Hug"},
            {ZUBAREVGEKS_ISOE_TEST, ZUBAREVGEKS_ISOE_CFG, 1e-6,"ZubarevGeks.IsoE"}
        };
        cur->Text_Func_Out(&ZUBAREVGEKS_resources[0],sizeof(ZUBAREVGEKS_resources)/sizeof(ResourcesStruct),"ZUBAREVGEKS");



// Ivl type
		ResourcesStruct ABUEXPFITBR5_resources[]={
            {ABUEXPFITBR5_HUG_TEST, ABUEXPFITBR5_HUG_CFG, 1e-6,"ABuExpFitBr5.Hug"},
            {ABUEXPFITBR5_ISOE_TEST, ABUEXPFITBR5_ISOE_CFG, 1e-6,"ABuExpFitBr5.IsoE"}
        };
        cur->Text_Func_Out(&ABUEXPFITBR5_resources[0],sizeof(ABUEXPFITBR5_resources)/sizeof(ResourcesStruct),"ABUEXPFITBR5");
        ResourcesStruct HISCHBR4_resources[]={
            {HISCHBR4_HUG_TEST, HISCHBR4_HUG_CFG, 1e-6,"HischBr4.Hug"},
            {HISCHBR4_ISOE_TEST, HISCHBR4_ISOE_CFG, 1e-6,"HischBr4.IsoE"}
        };
        cur->Text_Func_Out(&HISCHBR4_resources[0],sizeof(HISCHBR4_resources)/sizeof(ResourcesStruct),"HISCHBR4");
        ResourcesStruct IVLPOLYSTERENE_resources[]={
            {IVLPOLYSTERENE_HUG_TEST, IVLPOLYSTERENE_HUG_CFG, 1e-6,"IVLPolysterene.Hug"},
            {IVLPOLYSTERENE_ISOE_TEST, IVLPOLYSTERENE_ISOE_CFG, 1e-6,"IVLPolysterene.IsoE"}
        };
        cur->Text_Func_Out(&IVLPOLYSTERENE_resources[0],sizeof(IVLPOLYSTERENE_resources)/sizeof(ResourcesStruct),"IVLPOLYSTERENE");
        ResourcesStruct MO_BUSHMAN_resources[]={
            {MO_BUSHMAN_HUG_TEST, MO_BUSHMAN_HUG_CFG, 1e-6,"Mo_Bushman.Hug"},
            {MO_BUSHMAN_ISOT_TEST, MO_BUSHMAN_ISOT_CFG, 1e-6,"Mo_Bushman.IsoT"}
        };
        cur->Text_Func_Out(&MO_BUSHMAN_resources[0],sizeof(MO_BUSHMAN_resources)/sizeof(ResourcesStruct),"MO_BUSHMAN");
        ResourcesStruct NI_BUSHMAN_resources[]={
            {NI_BUSHMAN_HUG_TEST, NI_BUSHMAN_HUG_CFG, 1e-6,"Ni_Bushman.Hug"},
            {NI_BUSHMAN_ISOT_TEST, NI_BUSHMAN_ISOT_CFG, 1e-6,"Ni_Bushman.IsoT"}
        };
        cur->Text_Func_Out(&NI_BUSHMAN_resources[0],sizeof(NI_BUSHMAN_resources)/sizeof(ResourcesStruct),"NI_BUSHMAN");




// Ross type
		ResourcesStruct FREEEROSSH2DISGOOD_resources[]={
            {FREEEROSSH2DISGOOD_ISOT_TEST, FREEEROSSH2DISGOOD_ISOT_CFG, 1e-6,"FreeERossH2DisGood.IsoT"}
        };
        cur->Text_Func_Out(&FREEEROSSH2DISGOOD_resources[0],sizeof(FREEEROSSH2DISGOOD_resources)/sizeof(ResourcesStruct),"FREEEROSSH2DISGOOD");
        ResourcesStruct FREEEROSSN2HE1_resources[]={
            {FREEEROSSN2HE1_ISOT_TEST, FREEEROSSN2HE1_ISOT_CFG, 1e-6,"FreeERossN2He1.IsoT"}
        };
        cur->Text_Func_Out(&FREEEROSSN2HE1_resources[0],sizeof(FREEEROSSN2HE1_resources)/sizeof(ResourcesStruct),"FREEEROSSN2HE1");
        ResourcesStruct FREEEROSSN2VCOOL_resources[]={
            {FREEEROSSN2VCOOL_ISOT_TEST, FREEEROSSN2VCOOL_ISOT_CFG, 1e-6,"FreeERossN2Vcool.IsoT"}
        };
        cur->Text_Func_Out(&FREEEROSSN2VCOOL_resources[0],sizeof(FREEEROSSN2VCOOL_resources)/sizeof(ResourcesStruct),"FREEEROSSN2VCOOL");
        ResourcesStruct FREEEROSSN2XE1_resources[]={
            {FREEEROSSN2XE1_ISOT_TEST, FREEEROSSN2XE1_ISOT_CFG, 1e-6,"FreeERossN2Xe1.IsoT"}
        };
        cur->Text_Func_Out(&FREEEROSSN2XE1_resources[0],sizeof(FREEEROSSN2XE1_resources)/sizeof(ResourcesStruct),"FREEEROSSN2XE1");
        ResourcesStruct XESTR1_resources[]={
            {XESTR1_ISOT_TEST, XESTR1_ISOT_CFG, 1e-6,"XeStr1.IsoT"}
        };
        cur->Text_Func_Out(&XESTR1_resources[0],sizeof(XESTR1_resources)/sizeof(ResourcesStruct),"XESTR1");


    }

};


























//struct TestCase_Lex_Analyser:TestCase_Buffered
//{
//	TestCase_Lex_Analyser():lex_an(NULL),in(NULL),buf(NULL){};
//	~TestCase_Lex_Analyser(){Finish();}
//	TestSuite* CreateTest()
//	{ 
//		TestSuite_Std* suite=new TestSuite_Std(class_name());
//		suite->AddTest(Test_String,"Test_String");
//		suite->AddTest(Test_Number,"Test_Number");
//		suite->AddTest(Test_Token,"Test_Token");
//
//		return suite;
//	}
//	void Init()
//	{
//		Finish();//          10
//	}
//	void Finish()	{delete in;in=NULL;delete buf;buf=NULL;delete lex_an;lex_an=NULL;	}
//	istream *in;
//	char *buf;
//	Lex_Analyser *lex_an;
//	static TestCase_Lex_Analyser* Start(DescribedClass *ptr,const char *str)
//	{
//		TestCase_Lex_Analyser *cur=dynamic_cast<TestCase_Lex_Analyser*>(ptr);
//		if (!cur) throw(TestExeption("ptr is not TestCase_Lex_Analyser"));
//		cur->buf=strdup(str);//"\"test stream\"\n123456789\n1e23\n1e\n1.e"
//		cur->in=new strstream(cur->buf,strlen(cur->buf),ios::in);
//		cur->lex_an=new Lex_Analyser(*cur->in,0);
//		//cur->lex_an=new Lex_Analyser(*cur->in,1);// to draw all the results
//		return cur;
//	}
//	static void CheckRes(Lex_Result *fst,Lex_Result *sec)
//	{
//		if (Lex_Result::Equals(fst,sec)) 
////			throw(TestExeption("OK"));
//		return;
//		if (fst==NULL) throw(TestExeption(string("Lex result returned NULL and have to return \n").append(sec->ToString()).c_str()));
//		if (sec==NULL) throw(TestExeption(string("Lex result returned ").append(fst->ToString()).append(" and have to return NULL ").c_str()));
//		throw(TestExeption(string("Lex result returned ").append(fst->ToString()).append(" and have to return  ").append(sec->ToString()).c_str()));
//	};
//	static void Test_String(TestCase *ptr)
//	{ 
//		TestCase_Lex_Analyser *cur=Start(ptr,"\"Start\"\t\"Another\034\" \t\'\035\'\t\n\
//\"aa\
//a\naa\"");
//		Lex_Result *res,tobe(0,0,"Start",Lex_Result::String);
//		res=cur->lex_an->Analyse();
//		CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"Another\034",Lex_Result::String);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"\035",Lex_Result::Char);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"aaa\naa",Lex_Result::String);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		CheckRes(res,NULL);
//	}
//	static void Test_Number(TestCase *ptr)
//	{ 
//		TestCase_Lex_Analyser *cur=Start(ptr,"123 0x3a 022 022.\t1e-1 1.\n 1e23 1.e1 1e 1.e");
//		Lex_Result *res,tobe;
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(123,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0x3a,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(022,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,22,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0.1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,1e23,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,10,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(1,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"e",Lex_Result::Identifier);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"e",Lex_Result::Identifier);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		CheckRes(res,NULL);
//	}
//	static void Test_Token(TestCase *ptr)
//	{ 
//		TestCase_Lex_Analyser *cur=Start(ptr,"+ - word wor\t word + word +");
//		Lex_Result *res,tobe;
//		cur->lex_an->PushTokenLevel();
//
//		cur->lex_an->AddToken("word",4);
//		cur->lex_an->AddToken("wor",3);
//		cur->lex_an->AddToken("+",1);
//		cur->lex_an->AddToken("-",2);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(2,0,"-",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(4,0,"word",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(3,0,"wor",Lex_Result::Token);CheckRes(res,&tobe);
//
//		cur->lex_an->PushTokenLevel();
//		cur->lex_an->AddToken("word",5);
//
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(5,0,"word",Lex_Result::Token);CheckRes(res,&tobe);
//
//		cur->lex_an->PopTokenLevel();
//
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(4,0,"word",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);
//
//		res=cur->lex_an->Analyse();
//		CheckRes(res,NULL);
//	}
//};
//
#endif