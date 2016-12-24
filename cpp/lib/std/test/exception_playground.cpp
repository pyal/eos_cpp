#include <execinfo.h>	// for backtrace
#include <dlfcn.h>		// for dladdr
#include <cxxabi.h>		// for __cxa_demangle

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include "lib/std/logger.h"
// This function produces a stack backtrace with demangled function & method names.
//#include "lib/std/Util.h"
#include "lib/std/unit_test.h"
#include <cpp/lib/std/stdexception.h>
//#include "extra_test.cpp"


UnitTest PlayGround("PlayGround", [](){
    log_always(string("Generating backtrace:\n") + info_except.what());
    log_always("Done");

});
