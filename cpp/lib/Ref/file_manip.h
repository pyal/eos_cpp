#pragma once

#include "stroka.h"
//#include <stdio.h>
#include <vector>
#include "lib/std/util.h"
#include "str_manip.h"


namespace File {
    FILE *open(const char *file, const char *mod, const char *mes);
    void close(FILE *&file);

    int GetLine(FILE *file, Stroka &str);
    int GetLine(FILE *file, Stroka &str, char *Buf, int BufLen);

    int GetLine(FILE *file, std::vector<Stroka> &line, char delim = ' ');
    int GetLine(FILE *file, std::vector<string> &line, char delim = ' ');
    std::map<Stroka, Stroka> ReadFile2Map(
        const char *file,
        const char delim,
        int KeyCol,
        int StrCol,
        const char *err_mes);

    int GetLine(FILE *file, std::vector<Stroka> &line, char *Buf, int BufLen);
    //  make vector of columns (for tableData)
    //  else - make vector of rows
    int ReadFile(
        const char *file,
        vector<vector<double>> &vec,
        const char *mes,
        int makeVectorOfColumns = 1);
    //// If  split_line  = 0 - will create in output[0] - vector<Stroka>  - with file content
    //        yvector<yvector<Stroka> > ReadFile(const char *file, int split_line, const char *err_mes);
    void WriteFile(
        const char *file,
        const vector<vector<double>> &vec,
        const char *head,
        const char *mes);
    vector<Ref<SavableClass>> ReadConf(const char *conf, size_t NumObject, int BreakNull);
    long GetFileSize(const char *file);

    template <class T>
    int ReadObjVector(FilterIn &si, vector<Ref<T>> &objects) {
        objects.clear();
        Ref<T> obj;
        SavableClass::ExceptionCheck(si, "{");
        while(!SavableClass::TestNextChar(si, '}')) {
            obj << si.getobject();
            if(!obj)
                throw info_except(
                    "Could not read object. Error occured in\n%s",
                    ~SavableClass::ShowStreamPosition(&si));
            objects.push_back(obj);
        }
        return 1;
    }
    template <class T>
    int SaveObjVector(FilterOut &so, vector<Ref<T>> &objects) {
        so << "{" << SavableClass::ppEOLN();
        for(size_t i = 0; i < objects.size(); i++)
            so << objects[i];   //->save_data_state(so);
        so << SavableClass::mmEOLN() << "}" << SavableClass::EOLN();
        return 1;
    }


}   //    namespace File {
