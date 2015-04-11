#ifndef __stdexception_h
#define __stdexception_h

#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdarg.h>
#include "lib\std\util.h"

class stdexception: public exception {
#define LOCSLASH_C '\\'
public:
	stdexception(){ message[0] = 0; }
    stdexception(const char* format, ...) {
        va_list args;
        va_start(args, format);
        print(format, args);
    }

    virtual const char *what() const throw() {
        return message;
    }

protected:
    void print(const char* fmt, va_list args) {
        if (!fmt) {
            message[0] = 0;
            return;
        }
        char format[sizeof(message)+1], *ptr;
        MemoryMove(fmt, format, min(strlen(fmt), sizeof(message)));
        format[sizeof(message)] = 0;
        format[strlen(fmt)] = 0;
        if ((ptr = strstr(format, "%m")) != NULL) {
            *ptr++ = '$';
            *ptr = 'S';
        }
        vsnprintf(message, sizeof(message), format, args);
//        _snprintf(message, sizeof(message), format, args);
        if ((ptr = strstr(message, "$S")) != NULL) {
            const char *sptr = strerror(errno);
            insert(ptr - message, 2, strlen(sptr), sptr);
        }

    }

    int vsnprintf(char *str, size_t size, const char *format, va_list argptr)
    {
    int i = _vsnprintf(str, size-1, format, argptr);
    if (i < 0) {
        str[size - 1] = '\x00';
        i = (int)size;
    } else if (i < (int)size) {
        str[i] = '\x00'; //Bug in MS library - sometimes it happens. Never trust MS :(
    }
    return i;
    }

    void insert(size_t off, size_t rep, size_t len, const char *text) {
//        MemoryMove(message + off + rep, message + off + len, max(size_t(0), min(
        MemoryMove(message + off + rep, message + off + len, max(size_t(0), min(
            sizeof(message) - off - rep,
            sizeof(message) - off - len)));
//        MemoryMove(text, message + off, max(size_t(0), min(
        MemoryMove(text, message + off, max(size_t(0), min(
            len,
            sizeof(message) - off)));
        message[sizeof(message)-1] = 0;
//		out.operator::strstream(
    }
    char message[40024];
};

#define info_except stdexception_with_line_info(__FILE__, __LINE__,__FUNCTION__)
#define info_mark (Stroka("Current pos: file ") + __FILE__ + " line " + Stroka::Int2Str(__LINE__) + " function " + __FUNCTION__ + "\n" ).c_str()

class stdexception_with_line_info: public stdexception {
public:
    stdexception_with_line_info(const char *f, int l,const char *fn)
        : stdexception(NULL), file(f), line(l), func(fn)
        {}

    stdexception_with_line_info &operator()(const char* format, ...) {
        va_list args;
        va_start(args, format);
        print(format, args);
        char linfo[194];
        const char *f = strrchr(file, LOCSLASH_C);
        f = f ? f + 1 : file;
        sprintf(linfo, "%.30s:%d:%.130s: ", f, line, func);
        insert(0, 0, strlen(linfo), linfo);
        return *this;
    }
	// friend stdexception_with_line_info &operator<<(stdexception_with_line_info &out,const char*str){
	//	out.insert(strlen(out.message),0,strlen(str),str);
	//	return out;
	//}
	// friend stdexception_with_line_info &operator<<(stdexception_with_line_info &out,int i){
	//	char tmp[256];return out<<(const char*)itoa(i,&tmp[0],10);
	//}
	// friend stdexception_with_line_info &operator<<(stdexception_with_line_info &out,double d){
	//	char tmp[256];return out<<(const char*)_gcvt(d,8,&tmp[0]);
	//}
protected:
    const char *file;
    int line;
    const char *func;
};
//#define CATCHMAINEXCEPTION(NAME) catch(stdexception &e)\
//{ cout<<NAME<<"\n"<<"Exception caught:"<<e.what()<<"\n";return 1; }
#define CATCHMAINEXCEPTION(NAME) catch(exception &e)\
{ cout<<NAME<<"\n"<<"Exception caught:"<<e.what()<<"\n";return 1; }
#define CATCHEXCEPTION(NAME) catch(exception &e) { cout<<"In file:"<<__FILE__<<"\non line:"<<__LINE__<<"\nin function:"<<__FUNCDNAME__<<"\nexception caught name:"<<NAME<<"\nReason:"<<e.what()<<"\n"; }
//{ cout<<"In file:"<<__FILE__<<"\non line:"<<__LINE__<<"\nin function:"<<__FUNCTION__<<"\nexception caught name:"<<NAME<<"\nReason:"<<e.what()<<"\n"; }


//static inline void fputs(const exception &e, FILE *f = stderr) {
//    char message[256];
//    size_t len = min(sizeof(message) - 2, MemoryMove(e.what(), message, sizeof(message) - 1));
//    message[len++] = '\n';
//    message[len] = 0;
//    fputs(message, f);
//}

#endif // stdexception_h

