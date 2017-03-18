#ifndef __stdexception_h
#define __stdexception_h

#include <stdlib.h>
#include "lib/std/util.h"

#ifndef MAC
#define LOCSLASH_C '\\'
#else
#define LOCSLASH_C '/'
#endif

#define info_except stdexception_with_line_info(__FILE__, __LINE__, __FUNCTION__)
#define info_mark stdexception_with_line_info::FilePosition(__FILE__, __LINE__, __FUNCTION__).c_str()

#define verify(...) _GET_MACRO(__VA_ARGS__, verify_with_msg, verify_no_msg)(__VA_ARGS__)
#define _GET_MACRO(_1, _2, NAME, ...) NAME
#define verify_no_msg(cond) verify_with_msg((cond), "")
#define verify_with_msg(cond, msg) \
    ((void)((cond) ?  ((void)0) :  throw info_except(~(Stroka(#cond) + ": failed assertion: " + msg))))


#define unused(x) ((void)(x))


#ifdef MAC
#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <libunwind.h>
#include <sstream>

#endif

#define stdexception exception

class stdexception_with_line_info : public exception {
public:
    stdexception_with_line_info(const char *f, int l, const char *fn)
        : file(f), func(fn), back(""), line(l) {
#ifdef MAC
        back =  Backtrace();
#endif
    }

    virtual const char *what() const throw() {
        ostringstream out;
        out << FilePosition(file.c_str(), line, func.c_str()) << "\nProvided Description: [" << message << "]\n" << back;
        formatted = out.str();
        return formatted.c_str();
    }
    stdexception_with_line_info &operator()(const string &msg) {
        message = msg;
        return *this;
    }
    stdexception_with_line_info &operator()(const char *format, ...) {
        va_list args;
        va_start(args, format);
        char buf[20560];
        ::vsprintf(buf, format, args);
        va_end(args);
        message = buf;
        return *this;
    }

    static void decode_stack_simple(ostream &out) {
        void *callstack[128];
        int i, frames = backtrace(callstack, 128);
        char **strs = backtrace_symbols(callstack, frames);
        for(i = 0; i < frames; ++i) {
            out << strs[i] << "\n";
        }
        free(strs);
    }
    // This function produces a stack backtrace with demangled function & method names.
    static std::string GetModuleName(const char *frame) {
        std::string line(frame);
        std::string name(25, ' ');
        size_t p = line.find(' ', 0);
        if(std::string::npos == p)
            return name;
        while(line[p] == ' ')
            p++;
        size_t p1 = line.find(' ', p);
        name = &line[p];
        if(p1 != std::string::npos)
            name.resize(p1 - p);
        name.append(std::string(25 - name.length(), ' '));
        return name;
    }
    static std::string FilePosition(const char *file, int line, const char *func) {
        const char *f = strrchr(file, LOCSLASH_C);
        f = f ? f + 1 : file;
        return string(f) + ":" + Itoa(line) + " [" + func + "]";
    }
    static std::string Backtrace(int skip = 1) {
        void *callstack[128];
        const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
        char buf[10024];
        int nFrames = backtrace(callstack, nMaxFrames);
        char **symbols = backtrace_symbols(callstack, nFrames);

        std::ostringstream trace_buf;
        for(int i = skip; i < nFrames; i++) {
            std::string module = GetModuleName(symbols[i]);
            Dl_info info;
            if(dladdr(callstack[i], &info) && info.dli_sname) {
                char *demangled = NULL;
                int status = -1;
                if(info.dli_sname[0] == '_')
                    demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
                snprintf(
                    buf,
                    sizeof(buf),
                    "%-3d %s %*p %s + %zd\n",
                    i,
                    module.c_str(),
                    int(2 + sizeof(void *) * 2),
                    callstack[i],
                    status == 0 ? demangled
                                : info.dli_sname == 0 ? symbols[i] : info.dli_sname,
                    (char *)callstack[i] - (char *)info.dli_saddr);
                free(demangled);
            } else {
                snprintf(
                    buf,
                    sizeof(buf),
                    "%-3d %*p %s\n",
                    i,
                    int(2 + sizeof(void *) * 2),
                    callstack[i],
                    symbols[i]);
            }
            trace_buf << buf;
        }
        free(symbols);
        if(nFrames == nMaxFrames)
            trace_buf << "[truncated]\n";
        return trace_buf.str();
    }


protected:
    string file, func, back;
    int line;
    string message = "";
    mutable string formatted;
};


#define CATCHMAINEXCEPTION(NAME) CATCHEXCEPTION(NAME)
#ifndef MAC
#define CATCHEXCEPTION(NAME)                                                             \
    catch(exception & e) {                                                               \
        cout << info_mark  << " exception description:" << NAME  \
             << "\nReason:" << e.what() << "\n";                                         \
    }
#else
#define CATCHEXCEPTION(NAME)                                                             \
    catch(exception & e) {                                                               \
        cout << info_mark << " exception description: " << NAME   \
             << "\nReason:" << e.what() << "\n";                                         \
        abort();\
    }
#endif


#endif   // stdexception_h
