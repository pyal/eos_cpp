
#include "lib/precompiled/poly_test.h"

#include "lib/std/util.h"
#include "lib/ref/commandline.h"
#include "lib/std/ex_out.h"
#include "lib/ref/file_manip.h"
#include "gasd/Polygon/lib/PolyMarchBase.h"


#include "gasd/Polygon/lib/polygon_forcelnk.h"
#include "lib/test_fact/test_case.h"


Stroka GenerateDetailedHelp() {
    Stroka ret =
        Stroka("Program build timeStamp ") + __TIMESTAMP__ + " date? " + __DATE__ + "\n";
    //ret += Stroka(" Registered test objects:\n\n\n") + SavableClass::HelpForCategory("TestCase_TestTest_category");
    ret += "\n\n\nNPolygon::Constructor_category: ~+\n" +
           SavableClass::HelpForCategory("NPolygon::Constructor_category");
    ret += "~-\n\nNPolygon::March_category :~+\n" +
           SavableClass::HelpForCategory("NPolygon::March_category");
    ret +=
        "~-\n\n\nSample for NPolygon::TSimpleContructor(NPolygon::Constructor_category) :~+\n";
    Ref<NPolygon::TSimpleContructor> constr1 = new NPolygon::TSimpleContructor;
    constr1->Childs.push_back(new NPolygon::TSimpleContructor::TRegData(
        "EOSTest",
        new NPolygon::TSimpleContructor::TRegData,
        5,
        NPolygon::TSimpleContructor::TRegData::TGridVar("Y", 1, 2)));
    ret += SavableClass::object2string(constr1);
    ret += "~-\n\n\nSample for NPolygon::TPolyMarchBody(NPolygon::March_category) :~+\n";
    Ref<NPolygon::TPolyMarchBody> march = new NPolygon::TPolyMarchBody;
    ret += SavableClass::object2string(march);
    ret + "~-\n";
    return ret;
};


//ForceLinkVoid<TestCase_Gasd_Static> Dummy_TestCase_Gasd_Static;


void PrintHelp(map<Stroka, Stroka> par) {
    cout << GenerateDetailedHelp();
}


void TestFunc(map<Stroka, Stroka> par) {
    //cout<<"OK\n";
    TestSuite::RunTest("TestCase_Gasd_Static");
}


void March(map<Stroka, Stroka> par) {
    vector<Ref<SavableClass>> objVector = File::ReadConf(~par["ConfFile"], 2, 1);
    Ref<NPolygon::TSimpleContructor> constr =
        SavableClass::TestType<NPolygon::TSimpleContructor>(
            objVector[0], "Have to define constructor");
    Ref<NPolygon::TPolyMarchBody> march =
        SavableClass::TestType<NPolygon::TPolyMarchBody>(
            objVector[1], "Have to define march");
    Ref<NPolygon::TPolyRegion> reg = constr->MakeRegion();
    march->DoIt(reg);
}


int main(int argc, const char *argv[]) {
    SetLeakTest();
    Time_struct Time;
    try {
        NRef::TCommandParse Cmd(
            "Usage: poly_test [Params_Key_Spec] [/][-]key  \n without parameters - standart test\n");
        Cmd.MainHelp += GenerateDetailedHelp();
        Cmd.Add(PrintHelp, "help", "show help?", "");
        Cmd.Add(TestFunc, "test", "test it", "");
        Cmd.Add(
            March,
            "march",
            "clc region",
            "ConfFile par.cfg par.cfg file has the format: \"RegionConstructor NPolygon::TSimpleContructor RegionMarch  NPolygon::TPolyMarchBody\"");
        Cmd.SimpleRun(argc, argv);
    }
    CATCHMAINEXCEPTION(" poly_test failed ");

    LeakTest();
    cout << "Done in " << Time << "\n";
    return 0;
}
