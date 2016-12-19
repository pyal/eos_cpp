#ifndef _LEX_ANAL_TEST_H_
#define _LEX_ANAL_TEST_H_

#include "lib/test_fact/test_case.h"
#include "lex_anal.h"

struct TestCase_Manip:TestCase_Buffered
{
	//TestCase_Manip():manip(NULL),in(NULL),buf(NULL){};
	TestCase_Manip();//():manip(NULL),in(NULL),buf(NULL){};
	~TestCase_Manip(){Finish();}
	TestSuite* CreateTest()
	{ 
		TestSuite_Std* suite=new TestSuite_Std(class_name());
		suite->AddTest(Test_Get,"Test_Get");
		suite->AddTest(Test_GetUnget,"Test_GetUnget");
		suite->AddTest(Test_Freeze_Unfreeze,"Test_Freeze_Unfreeze");
		suite->AddTest(Test_Freeze_Unfreeze_Unget,"Test_Freeze_Unfreeze_Unget");
		suite->AddTest(Test_Freeze_Restore,"Test_Freeze_Restore");
		suite->AddTest(Test_Undo_Freeze_Restore,"Test_Undo_Freeze_Restore");
		suite->AddTest(Test_Freeze_Restore_Freeze_Unfreeze,"Test_Freeze_Restore_Freeze_Unfreeze");
		suite->AddTest(Test_Freeze_Restore_Freeze_Restore,"Test_Freeze_Restore_Freeze_Restore");
		suite->AddTest(Test_Freeze_Restore_Undo_Freeze_Restore,"Test_Freeze_Restore_Undo_Freeze_Restore");
		suite->AddTest(Test_Freeze_Restore_Undo_Freeze_UnFreeze,"Test_Freeze_Restore_Undo_Freeze_UnFreeze");
		suite->AddTest(Test_Freeze_Restore_Freeze_UnFreeze_End,"Test_Freeze_Restore_Freeze_UnFreeze_End");
		suite->AddTest(Test_Freeze_Restore_Freeze_Restore_End,"Test_Freeze_Restore_Freeze_Restore_End");
		suite->AddTest(Test_Freeze_Restore_Undo_Freeze_Restore_End,"Test_Freeze_Restore_Undo_Freeze_Restore_End");
		suite->AddTest(Test_Freeze_Restore_Freeze_Undo_Restore_End,"Test_Freeze_Restore_Freeze_Undo_Restore_End");
		suite->AddTest(Test_Freeze_Restore_Freeze_Undo_Restore_Empty,"Test_Freeze_Restore_Freeze_Undo_Restore_Empty");
		suite->AddTest(Test_Freeze_Restore_Freeze_UnToStr,"Test_Freeze_Restore_Freeze_UnToStr");
		suite->AddTest(Test_Freeze_Restore_Freeze_GetUnget_UnToStr,"Test_Freeze_Restore_Freeze_GetUnget_UnToStr");

		return suite;
	}
	void Init()
	{
		Finish();//          10
//                            11   14 17 20 23  26  29  32 35
//                           10  13  16 19 22  25 28  31  34
//                   123456789 12   15 18 21  24 27  30  33
		buf=strdup("\"test stream\"\n123456789\n1e23\n1e\n1.e");
		in=new strstream(buf,strlen(buf),ios::in);
		manip=new StreamManip_Base(in);
	}
	void Finish()	{delete in;in=NULL;delete buf;buf=NULL;delete manip;manip=NULL;	}
	istream *in;
	char *buf;
	StreamManip_Base *manip;
	static TestCase_Manip* Restore(DescribedClass *ptr)
	{
		TestCase_Manip *cur=dynamic_cast<TestCase_Manip*>(ptr);
		//info_except
		if (!cur) throw(info_except("ptr is not TestCase_Manip"));
		return cur;
	}
	static void Test_Get(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		//cout<<"\n\nTest_Get\n\n"<<" Last char "<<cur->manip->LastGetChar<<"\n";
		//char tmp[256];
		//for (int k=0;k<=33;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
		for (int k=0;k<=33;k++) 
			if ( cur->manip->get()!=cur->buf[k] )
				throw(info_except("Trying to get for ind <%i> found error /n",k));

//		throw(info_except("OK"));
	}
	static void Test_GetUnget(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char tmp[256];
		//cout<<"Test_GetUnget\n\n";
		for (int k=0;k<=10;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
		cur->manip->unget();
		for (int k=10;k<=33;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
//		throw(info_except("OK"));
	}
	static void Test_Freeze_Unfreeze(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char tmp[256];
		//cout<<"\n\nTest_Freeze_Unfreeze\n\n";
		for (int k=0;k<=10;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
		cur->manip->FreezePos();
		for (int k=11;k<=20;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
		cur->manip->UnfreezePos();
		for (int k=21;k<=33;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Unfreeze_Unget(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char tmp[256];
		//cout<<"\n\nTest_Freeze_Unfreeze_Unget\n\n";
		for (int k=0;k<=10;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
        cur->manip->unget();
		cur->manip->FreezePos();
		for (int k=10;k<=20;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
		cur->manip->UnfreezePos();
		for (int k=21;k<=33;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));

//		throw(info_except("OK"));
	}

	static void Test_Freeze_Restore(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char tmp[256];
		//cout<<"\n\nTest_Freeze_Restore\n\n";
		for (int k=0;k<=10;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
//        cur->manip->unget();
		cur->manip->FreezePos();
		for (int k=11;k<=20;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
		cur->manip->RestorePos();
		for (int k=11;k<=33;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Undo_Freeze_Restore(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char tmp[256];
		//cout<<"\n\n Test_Undo_Freeze_Restore\n\n";
		for (int k=0;k<=10;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
        cur->manip->unget();
		cur->manip->FreezePos();
		for (int k=10;k<=20;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));
		cur->manip->RestorePos();
		for (int k=10;k<=33;k++) if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Freeze_Unfreeze(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		//cout<<"\n\n Test_Freeze_Restore_Freeze_Unfreeze\n\n";
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
//        cur->manip->unget();
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=15;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=16;k<=20;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}

		cur->manip->UnfreezePos();

		for (int k=21;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Freeze_Restore(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		//cout<<"\n\n Test_Freeze_Restore_Freeze_Unfreeze\n\n";
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=15;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=16;k<=20;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}

		cur->manip->RestorePos();

		for (int k=16;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Undo_Freeze_Restore(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		//cout<<"\n\n Test_Freeze_Restore_Freeze_Unfreeze\n\n";
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=15;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->unget();
		cur->manip->FreezePos();  
		for (int k=15;k<=20;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}

		cur->manip->RestorePos();

		for (int k=15;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Undo_Freeze_UnFreeze(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=15;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->unget();
		cur->manip->FreezePos();  
		for (int k=15;k<=20;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}

		cur->manip->UnfreezePos();

		for (int k=21;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Freeze_UnFreeze_End(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=24;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
//		cur->manip->unget();
		cur->manip->FreezePos();  
		for (int k=25;k<=30;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}

		cur->manip->UnfreezePos();

		for (int k=31;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Freeze_Restore_End(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=24;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
//		cur->manip->unget();
		cur->manip->FreezePos();  
		for (int k=25;k<=30;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}

		cur->manip->RestorePos();

		for (int k=25;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Undo_Freeze_Restore_End(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=24;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->unget();
		cur->manip->FreezePos();  
		for (int k=24;k<=30;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}

		cur->manip->RestorePos();

		for (int k=24;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Freeze_Undo_Restore_End(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=24;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->unget();
		cur->manip->FreezePos();  
		for (int k=24;k<=30;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}

		cur->manip->unget();
		cur->manip->RestorePos();
		for (int k=24;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));
//		cur->manip->UnfreezePos();
//		for (int k=30;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Freeze_Undo_Restore_Empty(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=24;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->unget();
		cur->manip->get();
		cur->manip->unget();
		cur->manip->FreezePos();  
//		cur->manip->unget();
//		cur->manip->RestorePos();
//		cur->manip->UnfreezePos();
		for (int k=24;k<=30;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));
		cur->manip->UnfreezePos();
		for (int k=31;k<=33;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));

//		throw(info_except("OK"));
	}

	static void Test_Freeze_Restore_Freeze_UnToStr(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=14;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=15;k<=22;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));
		string str=cur->manip->UnfreezeToStr();
		string str1(&cur->buf[15],8);
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->FreezePos();  
		for (int k=23;k<=30;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));
		 str=cur->manip->UnfreezeToStr();
		 str1=string(&cur->buf[23],8);
		if (str!=str1) throw(info_except(string("Unfrozen String differs (this is the and of buffer test getstring src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
	static void Test_Freeze_Restore_Freeze_GetUnget_UnToStr(TestCase *ptr)
	{ 
		TestCase_Manip *cur=Restore(ptr);
		char ch,tmp[256];
		for (int k=0;k<=10;k++)   {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->FreezePos();  
		for (int k=11;k<=25;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->RestorePos(); 
		for (int k=11;k<=14;k++) {if (cur->manip->get()!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error ").c_str()));}
		cur->manip->unget();
		cur->manip->FreezePos();  
		for (int k=14;k<=22;k++) if ((ch=cur->manip->get())!=cur->buf[k]) throw(info_except(string("Trying to get for ind ").append(Itoa(k,tmp,10)).append(" found error; char in buf <").append(1,cur->buf[k]).append("> and read char <").append(1,ch).append(">\n").c_str()));
		string str=cur->manip->UnfreezeToStr();
		string str1(&cur->buf[14],9);
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->unget();
		cur->manip->FreezePos();  
		str=cur->manip->UnfreezeToStr();
		str1="";
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->FreezePos();  
		cur->manip->get();
		cur->manip->unget();
		str=cur->manip->UnfreezeToStr();
		str1="";
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->get();
		cur->manip->unget();
		cur->manip->FreezePos();  
		cur->manip->get();
		cur->manip->unget();
		str=cur->manip->UnfreezeToStr();
		str1="";
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->get();
		cur->manip->unget();
		cur->manip->FreezePos();  
		cur->manip->get();
		cur->manip->get();

		cur->manip->unget();
		str=cur->manip->UnfreezeToStr();
		str1=cur->buf[22];
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->FreezePos();  
		cur->manip->get();
		cur->manip->get();

		cur->manip->unget();
		str=cur->manip->UnfreezeToStr();
		str1=cur->buf[23];
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->FreezePos();  
		cur->manip->get();
		cur->manip->get();

		cur->manip->unget();
		str=cur->manip->UnfreezeToStr();
		str1=cur->buf[24];
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->get();
		cur->manip->unget();
		cur->manip->FreezePos();  
		cur->manip->get();
		cur->manip->get();

		cur->manip->unget();
		str=cur->manip->UnfreezeToStr();
		str1=cur->buf[25];
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

		cur->manip->get();
		cur->manip->unget();
		cur->manip->FreezePos();  
		cur->manip->get();
		cur->manip->get();
		cur->manip->unget();
		str=cur->manip->UnfreezeToStr();
		str1=cur->buf[26];
		if (str!=str1) throw(info_except(string("Unfrozen String differs src:<").append(str1).append("> result one is:<").append(str).append(">\n").c_str()));

//		throw(info_except("OK"));
	}
};
struct TestCase_Lex_Analyser:TestCase_Buffered
{
	TestCase_Lex_Analyser():in(NULL),buf(NULL), lex_an(NULL){};
	~TestCase_Lex_Analyser(){Finish();}
	TestSuite* CreateTest()
	{ 
		TestSuite_Std* suite=new TestSuite_Std(class_name());
		suite->AddTest(Test_String,"Test_String");
		suite->AddTest(Test_Number,"Test_Number");
		suite->AddTest(Test_Token,"Test_Token");

		return suite;
	}
	void Init()
	{
		Finish();//          10
	}
	void Finish()	{delete in;in=NULL;delete buf;buf=NULL;delete lex_an;lex_an=NULL;	}
	istream *in;
	char *buf;
	Lex_Analyser *lex_an;
	static TestCase_Lex_Analyser* Start(DescribedClass *ptr,const char *str)
	{
		TestCase_Lex_Analyser *cur=dynamic_cast<TestCase_Lex_Analyser*>(ptr);
		if (!cur) throw(info_except("ptr is not TestCase_Lex_Analyser"));
		cur->buf=strdup(str);//"\"test stream\"\n123456789\n1e23\n1e\n1.e"
		cur->in=new strstream(cur->buf,strlen(cur->buf),ios::in);
		cur->lex_an=new Lex_Analyser(cur->in,0);
		//cur->lex_an=new Lex_Analyser(*cur->in,1);// to draw all the results
		return cur;
	}
	static void CheckRes(Lex_Result *fst,Lex_Result *sec)
	{
		if (Lex_Result::Equals(fst,sec)) 
//			throw(info_except("OK"));
		return;
		if (fst==NULL) throw(info_except(string("Lex result returned NULL and have to return \n").append(sec->ToString()).c_str()));
		if (sec==NULL) throw(info_except(string("Lex result returned ").append(fst->ToString()).append(" and have to return NULL ").c_str()));
		throw(info_except(string("Lex result returned ").append(fst->ToString()).append(" and have to return  ").append(sec->ToString()).c_str()));
	};
	static void Test_String(TestCase *ptr)
	{ 
		TestCase_Lex_Analyser *cur=Start(ptr,"\"Start\"\t\"Another\034\" \t\'\035\'\t\n\
\"aa\
a\naa\"");
		Lex_Result *res,tobe(0,0,"Start",Lex_Result::String);
		res=cur->lex_an->Analyse();
		CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,0,"Another\034",Lex_Result::String);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,0,"\035",Lex_Result::Char);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,0,"aaa\naa",Lex_Result::String);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		CheckRes(res,NULL);
	}
	static void Test_Number(TestCase *ptr)
	{ 
		TestCase_Lex_Analyser *cur=Start(ptr,"123 0x3a 022 022.\t1e-1 1.\n 1e23 1.e1 1e 1.e");
		Lex_Result *res,tobe;
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(123,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0x3a,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(022,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,22,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,0.1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,1e23,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,10,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(1,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,0,"e",Lex_Result::Identifier);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(0,0,"e",Lex_Result::Identifier);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		CheckRes(res,NULL);
	}
	static void Test_Token(TestCase *ptr)
	{ 
		TestCase_Lex_Analyser *cur=Start(ptr,"+ - word wor\t word + word +");
		Lex_Result *res,tobe;
		cur->lex_an->PushTokenLevel();

		cur->lex_an->AddToken("word",4);
		cur->lex_an->AddToken("wor",3);
		cur->lex_an->AddToken("+",1);
		cur->lex_an->AddToken("-",2);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(2,0,"-",Lex_Result::Token);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(4,0,"word",Lex_Result::Token);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(3,0,"wor",Lex_Result::Token);CheckRes(res,&tobe);

		cur->lex_an->PushTokenLevel();
		cur->lex_an->AddToken("word",5);

		res=cur->lex_an->Analyse();
		tobe.GenerateResult(5,0,"word",Lex_Result::Token);CheckRes(res,&tobe);

		cur->lex_an->PopTokenLevel();

		res=cur->lex_an->Analyse();
		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(4,0,"word",Lex_Result::Token);CheckRes(res,&tobe);
		res=cur->lex_an->Analyse();
		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);

		res=cur->lex_an->Analyse();
		CheckRes(res,NULL);
	}
};

#endif