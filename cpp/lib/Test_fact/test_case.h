#ifndef _TEST_CASE_H_
#define _TEST_CASE_H_

#include "lib/ref/class.h"
#include "lib/ref/class_sav.h"
#include "lib/std/stdexception.h"
#include <list>

//#pragma warning( disable : 4267 )
#include "lib/std/logger.h"

struct TestBase : SavableClass {
    //	char *TestName;
    //	virtual ~TestBase(){};
};
struct TestSuite;
struct TestCase : TestBase {
    virtual TestSuite *CreateTest() = 0;
};
struct TestSuite : TestBase {
    virtual void RunTest() = 0;
    static void RunTest(const char *name) {
        try {
            TestCase *test = dynamic_cast<TestCase *>(
                ClassDesc::name_to_class_desc_check(name)->create());
            if(!test)
                throw(info_except(
                    string("Cannot create TestCase from name:").append(name).c_str()));
            TestSuite *suite = test->CreateTest();
            if(!suite)
                throw(
                    info_except(string("TestCase cannot create TestSuite TestCase name:")
                                    .append(name)
                                    .c_str()));
            suite->RunTest();
            delete suite;
            delete test;
        }
        CATCHEXCEPTION("Error in TestSuite::RunTest");
    }
};
struct TestSuiteComposite : TestSuite {
    //	TestSuiteComposite(){TestName=strdup("TestSuiteComposite");}
    ~TestSuiteComposite() {
        list<TestSuite *>::iterator it = tests.begin();
        for(; it != tests.end(); it++)
            delete(*it);
        tests.clear();
    }
    void AddTestCase(TestCase *test) {
        TestSuite *suite = test->CreateTest();
        AddTestSuite(suite);
    }
    void AddTestCaseCategory(const char *categ) {
        Ref<TestCase> test;
        test << ClassDesc::list_category_obj_start(categ);
        while(test) {
            AddTestCase(test);
            test << ClassDesc::list_category_obj_next(test, categ, 0);
        }
    }

    void RunTest() {
        list<TestSuite *>::iterator it = tests.begin();
    beg:
        try {
            for(; it != tests.end(); it++)
                (*it)->RunTest();
        }
        CATCHEXCEPTION("Error in TestSuiteComposite ");
        if(it != tests.end()) {
            it++;
            goto beg;
        }
    }

private:
    void AddTestSuite(TestSuite *test) {
        tests.push_back(test);
    };

    list<TestSuite *> tests;
};


//struct TestSuite:TestBase{	virtual void RunTest()=0;};
//struct TestCase:TestBase{	virtual TestSuite* CreateTest()=0;};
struct TestCase_Full : TestCase {
    virtual void RunTest() = 0;
};
struct TestSuite_Simple : TestSuite {
    TestSuite_Simple(TestCase_Full *t) : test(t){};
    ~TestSuite_Simple() {
        delete test;
    }
    void RunTest() {
        test->RunTest();
    }
    TestCase_Full *test;
};

template <class T>
T *TestCase_RestorePtr(DescribedClass *ptr) {
    T *cur = dynamic_cast<T *>(ptr);
    if(!cur)
        throw info_except(
            ~(Stroka("ptr is not ") + typeid(T).name() + " type. Its type is " +
              typeid(*ptr).name()));
    return cur;
}


struct TestCase_Std : TestCase {
    virtual void Init(){};
    virtual void Finish(){};
};

struct ListStruct {
    string Name;
    void (*Ptr)(TestCase *);
    ListStruct(void (*ptr)(TestCase *), const char *name) : Name(name), Ptr(ptr){};
};

struct TestSuite_Std : TestSuite {
    TestSuite_Std(const char *classname) {
        try {
            if(!classname)
                throw(info_except(string("ClassName is zero").c_str()));
            class_name.append(classname);
            test = dynamic_cast<TestCase_Std *>(
                ClassDesc::name_to_class_desc(classname)->create());
            if(!test)
                throw(info_except(string(" Class:")
                                      .append(classname)
                                      .append(" cannot be converted to TestCase_Std*")
                                      .c_str()));
        }
        CATCHEXCEPTION("Error in TestSuite_Std\n");
    };
    ~TestSuite_Std() {
        delete test;
    }
    void RunTest() {
        list<ListStruct>::iterator it = lst.begin();
        string err;
        //ret:
        log_debug(string("Starting test sequence: ") + test->class_name());
        for(; it != lst.end(); it++) {
            SetLeakTest();
            log_debug(string("Testing: ") + it->Name);
            try {
                test->Init();
                it->Ptr(test);
                test->Finish();
            }
            CATCHEXCEPTION(
                "Error in class " << class_name.c_str() << " function "
                                  << it->Name.c_str()
                                  << "\n");
            LeakTest(err);
            cout << "Function " << it->Name.c_str() << " memory: " << err.c_str() << "\n";
        }
    }
    void AddTest(void (*ptr)(TestCase *), const char *name) {
        lst.push_back(ListStruct(ptr, name));
    }
    TestCase_Std *test;
    list<ListStruct> lst;
    string class_name;
};

struct TestCase_TestTest : TestCase_Std {
    TestCase_TestTest(){};
    TestSuite *CreateTest() {
        TestSuite_Std *suite = new TestSuite_Std(class_name());
        suite->AddTest(Test1, "Test1");
        suite->AddTest(Test2, "Test2");
        return suite;
    }
    static TestCase_TestTest *Restore(TestCase *ptr) {
        TestCase_TestTest *cur = dynamic_cast<TestCase_TestTest *>(ptr);
        if(!cur)
            throw(info_except("prt is not TestCase_TestTest"));
        return cur;
    }
    static void Test1(TestCase *ptr) {
        //		TestCase_TestTest *cur=Restore(ptr);
        throw(info_except("OK"));
    }
    static void Test2(TestCase *ptr) {
        //		TestCase_TestTest *cur=Restore(ptr);
        throw(info_except("OK"));
    }
};


struct TestCase_Buffered : TestCase_Std {
    TestCase_Buffered() : in(NULL), buf(NULL){};
    ~TestCase_Buffered() {
        Finish();
    }
    TestSuite *CreateTest() {
        TestSuite_Std *suite = new TestSuite_Std(class_name());
        suite->AddTest(Test1, "Test1");
        suite->AddTest(Test2, "Test2");
        return suite;
    }
    void Init() {
        Finish();
        buf = strdup("\"test stream\"\n123456789\n1e23\n1e\1.e");
        in = new strstream(buf, strlen(buf), ios::in);
    }
    void Finish() {
        delete in;
        in = NULL;
        delete buf;
        buf = NULL;
    };   //delete manip;manip=NULL;	}
    istream *in;
    char *buf;
    //	StreamManip_Base *manip;
    static TestCase_Buffered *Restore(DescribedClass *ptr) {
        TestCase_Buffered *cur = dynamic_cast<TestCase_Buffered *>(ptr);
        if(!cur)
            throw(info_except("ptr is not TestCase_Buffered"));
        return cur;
    }
    static void Test1(TestCase *ptr) {
        TestCase_Buffered *cur = Restore(ptr);
        char tmp[256];
        //		char *a=new char[24];
        for(int k = 0; k <= 33; k++)
            if(cur->in->get() != cur->buf[k])
                throw(info_except(string("Trying to get for ind ")
                                      .append(Itoa(k, tmp, 10))
                                      .append(" found error ")
                                      .c_str()));
    }
    static void Test2(TestCase *ptr) {
        TestCase_Buffered *cur = Restore(ptr);
        char tmp[256];
        for(int k = 0; k <= 33; k++)
            if(cur->in->get() != cur->buf[k])
                throw(info_except(string("Trying to get for ind ")
                                      .append(Itoa(k, tmp, 10))
                                      .append(" found error ")
                                      .c_str()));
    }
};


#endif
