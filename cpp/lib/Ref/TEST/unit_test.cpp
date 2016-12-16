
#include "unit_test.h"
#include <lib/std/EX_OUT.H>

UnitTest *UnitTest::First = 0;
UnitTest *UnitTest::Last = 0;

void UnitTest::DoAll(const char * test_name) {
    UnitTest *test = First;
    Time_struct Time;
    int n = 0;
    while(test) {
        if(!test_name || Stricmp(test_name,test->Test.Name.c_str()) == 0) {
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

