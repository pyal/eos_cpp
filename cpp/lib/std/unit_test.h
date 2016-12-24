#pragma once
#include <string>

struct UnitTest {
private:
    struct STest {
        std::string Name;
        void (*F)();
        STest(const char *name, void (*f)()) : Name(name), F(f) {}
    } Test;
    UnitTest *Next;
    static UnitTest *First, *Last;

public:
    UnitTest(const char *name, void (*fn)()) : Test(name, fn) {
        if(Last)
            Last->Next = this;
        else
            First = this;
        Last = this;
    }

    static void DoAll(const char *test_name);
    void Run();

private:
    std::string GetName() {
        static std::string pref = std::string("\033[1;31m"),
                           post = std::string("\033[0m");
        return pref + Test.Name + post;
    }
};
