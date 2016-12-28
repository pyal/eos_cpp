
#include "lib/precompiled/poly_test.h"

#include "lib/std/util.h"
#include "lib/ref/commandline.h"
#include "lib/std/ex_out.h"
#include "lib/ref/file_manip.h"
#include "gasd/Polygon/lib/PolyMarchBase.h"


#include "gasd/Polygon/lib/polygon_forcelnk.h"
Stroka GenerateDetailedHelp() {
    Stroka ret =
           Stroka("\n\n\nNPolygon::Constructor_category: ~+\n") +
           SavableClass::HelpForCategory("NPolygon::Constructor_category");
    ret += "~-\n\n\nNPolygon::March_category :~+\n\n" +
           SavableClass::HelpForCategory("NPolygon::March_category");
    return NRef::TFormatOutput("    ", 0, 110).Format(~ret);
};

void MarchConfig(map<Stroka, Stroka> par) {
    NLogger::TLogger::GetLogger().LogLevel = NLogger::ELevel(atoi(~par["LogLevel"]));
    Stroka conf = par["ConfFile"];
    fstream out(~conf, ios::out);
    Ref<NPolygon::TSimpleContructor> constr1 = new NPolygon::TSimpleContructor;
    Ref<NPolygon::TPolyMarchBody> march = new NPolygon::TPolyMarchBody;
    out << "RegionConstructor\n" << SavableClass::object2string(constr1) << "\n\n";
    out << "RegionMarch\n" << SavableClass::object2string(march) << "\n";
    out.close();
    cout << GenerateDetailedHelp();
}


void TestFunc(map<Stroka, Stroka> par) {
    int level = atoi(~par["LogLevel"]);
    verify(level >= 0 && level < NLogger::max_level, "Max level is " + Itoa(NLogger::max_level) + " current level " + Itoa(level));
    NLogger::TLogger::GetLogger().LogLevel = NLogger::ELevel(level);
    TestSuite::RunTest("TestCase_Gasd_Static");

}


void March(map<Stroka, Stroka> par) {
    NLogger::TLogger::GetLogger().LogLevel = NLogger::ELevel(atoi(~par["LogLevel"]));
    log_always(~(Stroka("Log level is: ") + int(NLogger::TLogger::GetLogger().LogLevel)));
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
            "Usage: poly_test  - without parameters - help\n");
        Cmd.MainHelp += GenerateDetailedHelp();
        Cmd.Add(MarchConfig, "march.config", "build sample config", "ConfFile poly_test.march.cfg Config file name\nLogLevel 3 ", 1);
        Cmd.Add(TestFunc, "test", "test it", "LogLevel 3 Specify default debug level", 1);
        Cmd.Add(
            March,
            "march",
            "clc region",
            "ConfFile poly_test.march.cfg par.cfg file has the format: \"RegionConstructor NPolygon::TSimpleContructor RegionMarch  NPolygon::TPolyMarchBody\"\nLogLevel 3 Specify default debug level",
            1);
        Cmd.SimpleRun(argc, argv);
    }
    CATCHMAINEXCEPTION(" poly_test failed ");

    LeakTest();
    cout << "Done in " << Time << "\n";
    return 0;
}
