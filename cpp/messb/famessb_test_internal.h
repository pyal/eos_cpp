#ifndef _FAMESSB_TEST_INTERNAL_H_
#define _FAMESSB_TEST_INTERNAL_H_

#include "lib/test_fact/test_case.h"
#include <vector>


#include "mat/turnmat.h"
#include "messb.h"
#include "messb_av.h"
struct TestCase_Famessb_Internal : TestCase_Std   //TestCase_Buffered
{
    TestCase_Famessb_Internal(){};   //:manip(NULL),in(NULL),buf(NULL){};
    ~TestCase_Famessb_Internal() {
        Finish();
    }
    TestSuite *CreateTest() {
        TestSuite_Std *suite = new TestSuite_Std(class_name());
        suite->AddTest(Test_Turn_Matr, "Test_Turn_Matr");
        suite->AddTest(Test_ZTurnedQsG_QsG, "Test_ZTurnedQsG_QsG");
        suite->AddTest(Test_Qs_Gamma_Eiler, "Test_Qs_Gamma_Eiler");
        suite->AddTest(Test_Turn_Angles, "Test_Turn_Angles");
        //suite->AddTest(Test_Rnd_Angles_Generators,"Test_Rnd_Angles_Generators");

        return suite;
    }
    static TestCase_Famessb_Internal *Restore(DescribedClass *ptr) {
        TestCase_Famessb_Internal *cur = dynamic_cast<TestCase_Famessb_Internal *>(ptr);
        if(!cur)
            throw(info_except("ptr is not TestCase_Famessb_Internal"));
        return cur;
    }
    void Init() {
        Finish();
    }
    void Finish(){};
    static double MakeParamsFit(
        double (*mis)(VecCl par),
        VecCl &BestPar,
        double *low,
        double *up,
        double *num,
        int Dims);

    static void Test_Turn_Matr(TestCase *ptr);

    static CMatrCl *BaseMisMatr;
    static Ham_Qs_Gamma *MisHam;
    static double FitMisf(VecCl par) {
        MisHam->Gamma = par[1];
        MisHam->Phi = par[2];
        return MatrNorm(*BaseMisMatr - MisHam->Make4Ham());
    }

    static void Test_ZTurnedQsG_QsG(TestCase *ptr);
    static void Test_Qs_Gamma_Eiler(TestCase *ptr);
    static void Test_Turn_Angles(TestCase *ptr);
    static void Test_Rnd_Angles_Generators(TestCase *ptr);
    //istream *in;
    //char *buf;
    //StreamManip_Base *manip;
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