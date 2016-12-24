

#pragma once

#include <lib/ref/class_sav.h>

struct IBlackBoxBase : SavableClass {
    virtual void DoIt() = 0;
    virtual int UseStdout() {
        return 0;
    }
};
