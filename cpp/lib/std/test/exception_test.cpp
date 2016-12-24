#include <assert.h>
#include <cpp/lib/std/stdexception.h>
#include "lib/std/logger.h"
#include "lib/std/unit_test.h"

struct TExceptionTester {
    void DoTest() throw() {
        log_always("Log test");
        try {
            throw info_except("Testing %s", "ok");
        } catch (exception &ex) {
            log_always(string("Got exception:\n") + ex.what());
        }

        log_always(info_mark);
//        verify(false);
//        verify(0==1);
        verify(0==0);
//        verify(false, "FALSE");
    }

};

UnitTest ExceptionTester("ExceptionTest", [](){make_shared<TExceptionTester>()->DoTest();});