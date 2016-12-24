
#include "unit_test.h"
#include <lib/std/EX_OUT.H>
#include "logger.h"
UnitTest *UnitTest::First = 0;
UnitTest *UnitTest::Last = 0;

string ToLower(const char *str) {
    string ret(str);
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;

}
void UnitTest::DoAll(const char * test_name) {
    UnitTest *test = First;
    Time_struct Time;
    int n = 0;
    while(test) {
        if(!test_name || ToLower(test->Test.Name.c_str()).find(ToLower(test_name)) != string::npos) {
            test->Run();
            n++;
        }
        test = test->Next;
    }
    cout << "Tests done " << n << " test time " << Time << "\n";
}
void UnitTest::Run() {
    Time_struct time;
    std::cout << "Running test " << GetName() << "\n";
    Test.F();
    std::cout << "Test complete " << GetName() << " in " << time << "\n";
}

