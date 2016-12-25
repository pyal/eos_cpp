
#pragma once


//#include "GridMask.h"
//#include "RegBoundary.h"
//#include "PolyRegion.h"
#include "PolyRegConstructor.h"
#include "lib/test_fact/test_case.h"

namespace NPolygon {

    struct TTest : SavableClass {
        TTest(TTest *fr) : SavableClass() {   //, ptr(fr) {
            //ptr = fr;
            ptr = NULL;
            fr->ptr = this;
            lst.push_back(fr);
        };
        TTest();   // : SavableClass() { //, ptr(fr) {
        //    ptr = NULL;
        //};
        TTest *ptr;
        list<Ref<TTest>> lst;
        int save_data_state(FilterOut &so) {
            //RefManip::operator<<(so, lst)<<" ptr ";
            so << lst << " ptr ";
            so << ptr;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            //RefManip::operator>>(si, lst)>>tmp;
            si >> lst >> tmp;
            ptr = SavableClass::TestType<TTest>(si.getobject());
            return 1;
        };
    };
    struct TTestSimple : SavableClass {
        TTestSimple(TTestSimple *fr) : SavableClass() {
            stor = fr;
            ptr = NULL;
            fr->ptr = this;
        };
        TTestSimple() : SavableClass() {
            ptr = NULL;
            stor = NULL;
        };
        Ref<TTestSimple> stor;
        TTestSimple *ptr;
        int save_data_state(FilterOut &so) {
            so << stor.pointer() << " " << ptr;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            //char  tmp[256]; si>>tmp;
            stor = SavableClass::TestType<TTestSimple>(si.getobject());
            ptr = SavableClass::TestType<TTestSimple>(si.getobject());
            return 1;
        };
    };

    struct TestCase_Gasd_Static : TestCase_Std   //TestCase_Buffered
    {
        TestCase_Gasd_Static(){};   //:manip(NULL),in(NULL),buf(NULL){};
        ~TestCase_Gasd_Static() {
            Finish();
        }
        TestSuite *CreateTest() {
            TestSuite_Std *suite = new TestSuite_Std(class_name());
            suite->AddTest(Test_Simple, "Test_Simple");
            suite->AddTest(Test_ListSave, "Test_ListSave");
            suite->AddTest(Test_Region, "Test_Region");
            suite->AddTest(Test_Constructor, "Test_Constructor");

            return suite;
        }
        void Init() {
            Finish();
        }
        void Finish(){};
        static void Test_Simple(TestCase *ptr) {
//            TestCase_Gasd_Static *cur = TestCase_RestorePtr<TestCase_Gasd_Static>(ptr);
            SavableClass::SetRefOutMethod(SavableClass::SingleFileStorage);
            Ref<TTestSimple> t1 = new TTestSimple(new TTestSimple), t2 = new TTestSimple,
                             t3 = new TTestSimple;
            Stroka objStr1 = SavableClass::object2string(t1);
            t2 = SavableClass::TestType<TTestSimple>(
                SavableClass::string2object(SavableClass::object2string(t1.pointer())));
            Stroka objStr2 = SavableClass::object2string(t2);
            t3->LoadClass(*t1);
            Stroka objStr3 = SavableClass::object2string(t3);
            Ref<SavableClass> t4 = t3->Duplicate();
            Stroka objStr4 = SavableClass::object2string(t4);
            //cout<<" Fst class :\n"<<~objStr1<<" Sec class:\n"<<~objStr2<<" Third class:\n"<<~objStr3;
            if(objStr1 != objStr2 || objStr1 != objStr3 || objStr1 != objStr4)
                throw info_except(
                    " Objects are different. Fst:\n%s\nSec:\n%s\nThird:\n%s\nFourth\n%s\n",
                    ~objStr1,
                    ~objStr2,
                    ~objStr3,
                    ~objStr4);
        }
        static void Test_ListSave(TestCase *ptr) {
//            TestCase_Gasd_Static *cur = TestCase_RestorePtr<TestCase_Gasd_Static>(ptr);
            SavableClass::SetRefOutMethod(SavableClass::SingleFileStorage);
            Ref<TTest> t1 = new TTest(new TTest);
            Stroka objStr1 = SavableClass::object2string(t1.pointer());
            Ref<TTest> t2;
            //cout<<~objStr1<<"\n";
            t2 = dynamic_cast<TTest *>(SavableClass::string2object(objStr1));
            Stroka objStr2 =
                SavableClass::object2string(t2, SavableClass::SingleFileStorage);
            Ref<TTest> t3 = new TTest;
            t3->LoadClass(*t2);
            Stroka objStr3 =
                SavableClass::object2string(t3, SavableClass::SingleFileStorage);
            Ref<TTest> t4 = new TTest;
            *t4 = *t2;
            t4->LoadClass(*t2);
            Stroka objStr4 =
                SavableClass::object2string(t4, SavableClass::SingleFileStorage);

            Ref<SavableClass> t5 = t3->Duplicate();
            Stroka objStr5 = SavableClass::object2string(t5);


            //cout<<" Fst class :\n"<<~objStr1<<" Sec class:\n"<<~objStr2<<" Third class:\n"<<~objStr3<<" Fourth class:\n"<<~objStr4;
            if(objStr1 != objStr2 || objStr1 != objStr3 || objStr1 != objStr4 ||
               objStr1 != objStr5)
                throw info_except(
                    " Objects are different. Fst:\n%s\nSec:\n%s\nThird:\n%s\nFourth:\n%s\nFifth:\n%s\n",
                    ~objStr1,
                    ~objStr2,
                    ~objStr3,
                    ~objStr4,
                    ~objStr5);
        }
        static void Test_Region(TestCase *ptr) {
            SavableClass::SetRefOutMethod(SavableClass::SingleFileStorage);

            Ref<NPolygon::TPolyRegion> reg1 = new NPolygon::TPolyRegion();
            reg1->AddChild(new NPolygon::TPolyRegion);
            Stroka objStr1 = SavableClass::object2string(reg1);
            cout << " objStr1\n" << objStr1 << "\n";
            Ref<NPolygon::TPolyRegion> reg2 = new NPolygon::TPolyRegion();
            reg2 = SavableClass::TestType<NPolygon::TPolyRegion>(
                SavableClass::string2object(objStr1));
            Stroka objStr2 = SavableClass::object2string(reg2);
            //cout<<" objStr2\n"<<objStr2<<"\n";
            Ref<NPolygon::TPolyRegion> reg3 = new NPolygon::TPolyRegion();
            //reg3 << reg1->Duplicate();
            //reg3->LoadClass(*reg1);
            reg3 << reg1;
            Stroka objStr3 = SavableClass::object2string(reg3);
            //cout<<" objStr3\n"<<objStr3<<"\n";
            Ref<SavableClass> reg4;
            reg4 << reg3;
            //reg4 << reg3->Duplicate();
            Stroka objStr4 = SavableClass::object2string(reg4);
            //cout<<" Fst class :\n"<<~objStr1<<" Sec class:\n"<<~objStr2<<" Third class:\n"<<~objStr3;
            if(objStr1 != objStr2 || objStr1 != objStr3 || objStr1 != objStr4)
                throw info_except(
                    "Objects are different.\nObj1:\n%s\nObj2:\n%2\nObj3:\n%s\nObj4:\n%s\n",
                    ~objStr1,
                    ~objStr2,
                    ~objStr3,
                    ~objStr4);
        }

        static void Test_Constructor(int methOut) {
            SavableClass::SetRefOutMethod(methOut);
            cout << " OutMethod " << methOut << "\n";
            Ref<NPolygon::TSimpleContructor> constr1 = new NPolygon::TSimpleContructor;
            constr1->Childs.push_back(new TSimpleContructor::TRegData(
                "EOSTest",
                new TSimpleContructor::TRegData,
                5,
                TSimpleContructor::TRegData::TGridVar("Y", 1, 2)));
            Stroka objStr1 = SavableClass::object2string(constr1);
            cout << " objStr1\n" << objStr1 << "\n";
            Ref<NPolygon::TSimpleContructor> constr2 = new NPolygon::TSimpleContructor;
            constr2->LoadClass(*constr1);
            Stroka objStr2 = SavableClass::object2string(constr2);
            Ref<SavableClass> constr3 = constr2->Duplicate();
            Stroka objStr3 = SavableClass::object2string(constr2);
            //cout<<" Fst class :\n"<<~objStr1<<" Sec class:\n"<<~objStr2<<" Third class:\n"<<~objStr3;
            if(objStr1 != objStr2 || objStr1 != objStr3)
                throw info_except(
                    "OutMethod is %i\nObjects are different.\nObj1:\n%s\nObj2:\n%s\nObj3:\n%s\n",
                    methOut,
                    ~objStr1,
                    ~objStr2,
                    ~objStr3);
        }
        static void Test_Constructor(TestCase *ptr) {
            //Test_Constructor(SavableClass::StorePtr);


            Test_Constructor(SavableClass::SingleFileStorage);
            Test_Constructor(SavableClass::UnlinkedObjects);
            Test_Constructor(SavableClass::SimpleEdit);
        }
    };


};   //namespace NPolygon