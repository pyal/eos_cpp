#ifndef _TEST_CENTER_H_
#define _TEST_CENTER_H_

//#include "famessb_test_resource.h"
//#include "lib\test_fact\resource_reader.h"
#include "lib\data\data_forcelink.h"

//#include <vector>
//extern int CopyFile(const char *src,const char *dst);

//#include "Util\code_gen\analyse_base\lex_anal_test_forcelink.h"

#include "lib\test_fact\test_case.h"
#include "lib\ref\commandline.h"
#include "lib\data\dataVec.h"

//extern void test_sav();

struct TestCase_TestLibs:TestCase_Std//TestCase_Buffered
{
    TestCase_TestLibs(){};//:in(NULL),buf(NULL){};
    ~TestCase_TestLibs(){};//{Finish();}
	TestSuite* CreateTest()
	{ 
		TestSuite_Std* suite=new TestSuite_Std(class_name());
		suite->AddTest(TestFilterText,"TestFilterText");
		suite->AddTest(TestMemory,"TestMemory");
		suite->AddTest(TestCopyVector,"TestCopyVector");
		suite->AddTest(TestFilterTextMemory,"TestFilterTextMemory");
		suite->AddTest(TestFilterOut,"TestFilterOut");
		suite->AddTest(TestCopyVectorStrok,"TestCopyVectorStrok");
		suite->AddTest(TestAllocatorDel,"TestAllocatorDel");
		suite->AddTest(TestRef,"TestRef");
        


		return suite;
	}
//	void Init()
//	{
//		Finish();
//		buf=strdup("\"test stream\"\n123456789\n1e23\n1e\1.e");
//		in=new strstream(buf,strlen(buf),ios::in);
//		manip=new StreamManip_Base(*in);
//	}
//	void Finish()	{delete in;in=NULL;delete buf;buf=NULL;};//delete manip;manip=NULL;	}
//	istream *in;
//	char *buf;
//	StreamManip_Base *manip;
	static TestCase_TestLibs* Restore(DescribedClass *ptr)
	{
		TestCase_TestLibs *cur=dynamic_cast<TestCase_TestLibs*>(ptr);
		if (!cur) throw(info_except("ptr is not TestCase_TestLibs"));
		return cur;
	}
	static void TestFilterText(TestCase *ptr)
	{ 
		TestCase_TestLibs *cur=Restore(ptr);
		const char *name = "###!!!.tmp";
		{
			FilterTextOut outf(name);
			outf<<234<<"AAA"<<34.6<<'r'<<"\n";
//			outf.CloseBuf();
		}
		FilterTextIn inf(name);
		int i;
		double d;
		char ch_str[256],str[256];
		inf>>i>>str>>d>>ch_str;
		if (i!=234 || d!=34.6 || stricmp(str,"AAA")!=0 || stricmp(ch_str,"r")!=0 )
			throw info_except("Error int(234)==%i double(34.6)==%f char('r')==%s string(\"AAA\")==%s",
								i,d,ch_str,str);
	}
	static void TestMemory(TestCase *ptr)
	{ 
		TestCase_TestLibs *cur=Restore(ptr);
		string err;
		SetLeakTest();
		if (LeakTest(err)) throw info_except(" Single Leak test error:%s\n",err.c_str());
		SetLeakTest();SetLeakTest();
		if (LeakTest(err)) throw info_except(" Double Leak test1 error:%s\n",err.c_str());
		if (LeakTest(err)) throw info_except(" Double Leak test2 error:%s\n",err.c_str());
//		throw info_except("OK");
	}
	static void TestFilterTextMemory(TestCase *ptr)
	{ 
		TestCase_TestLibs *cur=Restore(ptr);
		string err;
		DataVector<double, CopyStructFast<double> > test(1);
		test[0] = 0.5;
		FilterTextOut out("aa",DataSource::Memory);
		out<<"bad"<<5<<"good";
		if (stricmp(DataSource::GetStr("aa").c_str(),"bad 5 good ")!=NULL)
			throw info_except("Got :%s: and have to get :%s:\n",DataSource::GetStr("aa").c_str(),"bad 5 good ");
	}
	static void TestCopyVector(TestCase *ptr)
	{ 
		TestCase_TestLibs *cur=Restore(ptr);
		string err;
		DataVector<double, CopyStructFast<double> > res, test(1);
		test[0] = 0.5;
		int discr = 1 ,centered =1;
		double ufr = 0,uto = 1;
		res = DataVector<double, CopyStructFast<double> >::MakeStepVector(discr,ufr,uto,centered);
		if (res!=test){
			FilterTextOut out("aa",DataSource::Memory);
			out<<" bad vector generated: "<<res<<" and have to be "<<test;
			throw info_except(DataSource::GetStr("aa").c_str());
		}
	}
	static void TestFilterOut(TestCase *ptr){ 
		TestCase_TestLibs *cur=Restore(ptr);
        FilterTextOut tmp("tmp",DataSource::Memory);
        for (int k=1;k<1000;k++)
            tmp<<k;
        Stroka str(DataSource::GetStr("tmp").c_str());
        istrstream buf(str.c_str(),str.length());
        int i;
        for (int k=1;k<1000;k++){
            buf>>i;
            if (i!=k) 
                throw info_except(" String is bad, have to be a serie for int from 1 to 1000 and is %s\n",str.c_str());
        }
	}
	static void TestCopyVectorStrok(TestCase *ptr)
	{ 
		TestCase_TestLibs *cur=Restore(ptr);
		DataVector<Stroka> res, test(3);
		test[0] = "First";test[1] = "Sec";test[2] = " werff tr ";
		res = test;
//cout<<"Fst\n"<<SavableClass::object2string(&test)<<"\nRes\n"<<SavableClass::object2string(&res)<<"\n";
        //char *a = new char[256];
		if (res!=test){
			FilterTextOut out("aa",DataSource::Memory);
			out<<" bad vector generated: "<<res<<" and have to be "<<test;
			throw info_except(DataSource::GetStr("aa").c_str());
		}
	}
// Testing problems with memory...
    struct TestMem:SavableClass{
    };
	static void TestAllocatorDel(TestCase *ptr)
	{ 
		TestCase_TestLibs *cur=Restore(ptr);

		DataVector<double, CopyStructFast<double> > test(3);
		test[0] = 0;test[1] = 1;test[2] = 2;
        VectorNorm2(test);
		FilterTextOut out("aa",DataSource::Memory);
        out.SetRefMask(SavableClass::SingleFileStorage);
        out<<(SavableClass*)&test;

	}
    Ref<Stroka> TestRefTmp(Ref<Stroka> str){
        Ref<Stroka> str1(str);
        list<Ref<Stroka> > lst;
        lst.push_back(str);lst.push_back(str);
        return str1;

    };
    Ref<Stroka> TestRefTmp_(Ref<Stroka> &str){
        Ref<Stroka> str1(str);
        list<Ref<Stroka> > lst;
        lst.push_back(str);lst.push_back(str);
        return str1;

    };
    Ref<Stroka> TestRefTmp_c(const Ref<Stroka> &str){
        Ref<Stroka> str1(str);
        list<Ref<Stroka> > lst;
        lst.push_back(str);lst.push_back(str);
        return str1;

    };
	static void TestRef(TestCase *ptr){ 
		TestCase_TestLibs *cur=Restore(ptr);

        Ref<Stroka> str = new Stroka("aa");
        Ref<Stroka> str1(str);
        str1=NULL;
        if (!str) 
            throw info_except("deleted main ref\n");
        str1 = cur->TestRefTmp(str);
        if (!str)
            throw info_except("deleted main ref\n");
        str1 = NULL;
        if (!str) 
            throw info_except("deleted main ref\n");
        str1 = cur->TestRefTmp_(str);
        if (!str)
            throw info_except("deleted main ref\n");
        str1 = NULL;
        if (!str) 
            throw info_except("deleted main ref\n");
        str1 = cur->TestRefTmp_c(str);
        if (!str)
            throw info_except("deleted main ref\n");
        str1 = NULL;
        if (!str) 
            throw info_except("deleted main ref\n");

    }





};


//template <class T,class CopyStructMethod>
//int operator == (const DataVector<T,CopyStructMethod>& vec1, const DataVector<T,CopyStructMethod>& vec2)
//{
//	if (vec1.Dim!=vec2.Dim) return 0;
//	for (int k=0;k<vec1.Dim;k++) if (vec1[k]!=vec2[k]) return 0;
//	return 1;
//}

#endif