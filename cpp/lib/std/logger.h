#pragma once

#include <vector>
#include <sys/time.h>
#include <sstream>
#include "Util.h"

namespace NLogger {
    enum ELevel { always = 0, error, info, warning, debug, max_level };
    extern vector<string> LevelNames;

    struct TLogger {
        static TLogger &GetLogger() {
            static TLogger logger(&cout);
            return logger;
        }
        TLogger(ostream *out) : Out(out) {}
        static ELevel &GetLevel() {
            return GetLogger().LogLevel;
        }
        static void Print(
            const char *file,
            int line,
            const char *func,
            ELevel level,
            const string &msg);
        static ostream &OutStream(ostream *out = NULL);


        static string LineFormat(ELevel level, const string &msg);


        ELevel LogLevel = info;
        ostream *Out = &cout;
    };
}
#define log_print(level, msg)                                                            \
    NLogger::TLogger::Print(__FILE__, __LINE__, __FUNCTION__, level, (msg))

#define log_always(msg) log_print(NLogger::always, (msg))
#define log_debug(msg) log_print(NLogger::debug, (msg))
