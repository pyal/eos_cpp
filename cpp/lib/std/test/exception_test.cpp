#include <assert.h>
#include <cpp/lib/std/stdexception.h>
#include "lib/std/logger.h"
#include "lib/std/unit_test.h"
#include "lib/Ref/Stroka.h"

struct TExceptionTester {
    void DoTest() throw() {
        log_always("Log test");
        try {
            throw info_except("Testing %s", "ok");
        } catch(exception &ex) {
            log_always(string("Got exception:\n") + ex.what());
        }

        log_always(info_mark);
        verify(0 == 0);
    }
};

UnitTest ExceptionTester("ExceptionTest", []() {
    make_shared<TExceptionTester>()->DoTest();
});