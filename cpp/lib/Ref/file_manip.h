#pragma once

#include "stroka.h"
//#include <stdio.h>
#include <vector>
#include "lib/std/util.h"
#include "str_manip.h"


namespace File {
    FILE *open(const char *file, const char *mod, const char *mes);
    void close(FILE *& file);

    int GetLine(FILE *file, Stroka &str);
    int GetLine(FILE *file, Stroka &str, char *Buf, int BufLen);
    int GetLine(FILE *file, std::vector<Stroka> &line);
    int GetLine(FILE *file, std::vector<Stroka> &line, char *Buf, int BufLen);
//  make vector of columns (for tableData)
//  else - make vector of rows
    int ReadFile(const char *file, vector<vector<double > > &vec, const char *mes, int makeVectorOfColumns = 1);
    void WriteFile(const char *file, const vector<vector<double > > &vec, const char *head, const char *mes);
    vector<Ref<SavableClass> > ReadConf(const char *conf, size_t NumObject,int BreakNull);
    long GetFileSize(const char *file);

} //    namespace File {



