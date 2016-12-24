#include "lib/precompiled/test_lib.h"
#include "test_case.h"


//#ifndef WCPP_NET
//DescribedClass * TestCase_TestTest_FASingleIO(){return new TestCase_TestTest();}
//static ClassDesc TestCase_TestTest_ClassDesc(typeid(TestCase_TestTest),"TestCase_TestTest",1,"TestCase_TestTest_category",TestCase_TestTest_FASingleIO);
//#else
static ClassDesc TestCase_TestTest_ClassDesc(
    typeid(TestCase_TestTest),
    "TestCase_TestTest",
    1,
    "TestCase_TestTest_category",
    create<TestCase_TestTest>);
//#endif
static ClassDesc TestCase_Buffered_ClassDesc(
    typeid(TestCase_Buffered),
    "TestCase_Buffered",
    1,
    "TestCase_Buffered_category",
    create<TestCase_Buffered>);
