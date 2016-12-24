#include "lib/Ref/CommandParse.h"


#include "lib/std/EX_OUT.H"
#include "cpp/lib/std/unit_test.h"


void DoAll(map<Stroka, Stroka> par) {
    auto t = par["Tests"];
    if(t == "ALL")
        UnitTest::DoAll(0);
    else
        UnitTest::DoAll(~t);
}
int main(int argc, const char *argv[]) {
    SetLeakTest();
    Time_struct Time;
    try {
        NRef::TCommandParse Cmd("Usage: unit_tests \n without parameters - make help\n");
        Cmd.Add(
            DoAll,
            "tests",
            " Run all tests",
            "Tests ALL to run specific tests\nDebug 3 Debug level",
            1);
        Cmd.SimpleRun(argc, argv);
    } catch(exception &ex) {
        cout << " Exeption caught:\n" << ex.what() << "\n";
        return 1;
    }
    cout << "Test time: " << Time << "\n";
    LeakTest();
    return 0;
}
