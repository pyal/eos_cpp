#include "logger.h"


vector<string> NLogger::LevelNames = {"Always", "Error", "Info", "Warning", "Debug"};

void NLogger::TLogger::Print(const char *file, int line, const char *func, NLogger::ELevel level, const string &msg) {
    if (level > GetLogger().LogLevel)
        return;
    const char *f = strrchr(file, '/');
    f = f ? f + 1 : file;
    cout << LineFormat(
            level, string(f) + ":" + Itoa(line) + " [" + func + "] " + msg + "\n");
}

ostream &NLogger::TLogger::OutStream(ostream *out) {
    if(!out)
        GetLogger().Out = out;
    return *GetLogger().Out;
}

string NLogger::TLogger::LineFormat(NLogger::ELevel level, const string &msg) {
    struct timeval tmnow;
    struct tm *timeinfo;
    char buffer[80];

    gettimeofday(&tmnow, NULL);
    timeinfo = localtime(&tmnow.tv_sec);
    strftime(buffer, 80, "%Y-%m-%d %I:%M:%S", timeinfo);
    ostringstream out;
    out << buffer << '.' << int(tmnow.tv_usec / 1000) << " ["
        << LevelNames[int(level)] << "] " << msg;
    return out.str();
}
