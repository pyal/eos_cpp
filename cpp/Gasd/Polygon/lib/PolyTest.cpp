#include "lib/precompiled/poly_lib.h"


#include "PolyTest.h"

namespace NPolygon {


    static ClassDesc TTest_ClassDesc(
        typeid(TTest),
        "TTest",
        1,
        "Test_category",
        create<TTest>);
    static ClassDesc TTestSimple_ClassDesc(
        typeid(TTestSimple),
        "TTestSimple",
        1,
        "Test_category",
        create<TTestSimple>);

    static ClassDesc TestCase_Gasd_Static_ClassDesc(
        typeid(TestCase_Gasd_Static),
        "TestCase_Gasd_Static",
        1,
        "TestCase_TestTest_category",
        create<TestCase_Gasd_Static>);


    TTest::TTest() : SavableClass() {
        ptr = NULL;
    };

};   //namespace NPolygon {
