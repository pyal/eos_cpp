#pragma once

#include <vector>
#include <sys/time.h>
#include <sstream>
#include "Util.h"

namespace NLogger {
    enum ELevel {
        always = 0,
        error,
        info,
        warning,
        debug,
        max_level
    };
    extern vector<string> LevelNames;

    struct TLogger {
        static TLogger &GetLogger() {
            static TLogger logger(&cout);
            return logger;
        }
        TLogger(ostream *out): Out(out) {}
        static ELevel &GetLevel() {
            return GetLogger().LogLevel;
        }
        static void Print(const char *file, int line, const char *func, ELevel level, const string &msg) {
            const char *f = strrchr(file, '/');
            f = f ? f + 1 : file;
            cout << LineFormat(level, string(f) + ":" + Itoa(line) + "[" + func + "]" + msg + "\n");
        }
        static ostream &OutStream(ostream *out = NULL) {
            if (!out) GetLogger().Out = out;
            return *GetLogger().Out;
        }


        static string LineFormat(ELevel level, const string& msg) {
            struct timeval tmnow;
            struct tm * timeinfo;
            char buffer[80];

            gettimeofday(&tmnow, NULL);
            timeinfo = localtime(&tmnow.tv_sec);
            strftime(buffer,80,"%Y-%m-%d %I:%M:%S",timeinfo);
            ostringstream out;
            out << buffer << ' ' << int(tmnow.tv_usec/1000) << ' ' << LevelNames[int(level)] << ' ' << msg;
            return out.str();
        }


        ELevel LogLevel = info;
        ostream *Out = &cout;
    };



}
#define log_print(level, msg) NLogger::TLogger::Print(__FILE__, __LINE__, __FUNCTION__, level, (msg))

#define log_always(msg) log_print(NLogger::always, (msg))
#define log_debug(msg)  log_print(NLogger::debug, (msg))




