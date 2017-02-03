
#include <lib/precompiled/Ref.h>


#include "file_manip.h"
#include "class_sav.h"

namespace File {
    FILE *open(const char *file, const char *mod, const char *mes) {
        if(Stricmp(mod, "r") == 0) {
            if(strcmp(file, "STDIN") == 0)
                return stdin;
            if(strcmp(file, "NULL") == 0)
                return NULL;
        } else {
            if(Stricmp(mod, "w") != 0)
                throw info_except(
                    "Files mode is \"r\" or \"w\", trying to use mode \"%s\"\n", mod);
            if(file == NULL || strlen(file) == 0 || strcmp(file, "NULL") == 0)
                return NULL;
            if(strcmp(file, "STDOUT") == 0)
                return stdout;
        }
        Stroka m = Stroka(mod) + "b";
        //FILE *ret = fopen(file, mod);
        FILE *ret = fopen(file, m.c_str());
        if(!ret && mes != NULL)
            throw info_except("Could not read file <%s>. %s\n", file, mes);
        return ret;
    }
    void close(FILE *&file) {
        if(file != stdin && file != stdout && file != 0)
            fclose(file);
        file = 0;
    }

    int GetLine(FILE *file, Stroka &str) {
#define MAXLINELEN 5000
        char line[MAXLINELEN + 1];
        line[MAXLINELEN] = 0;
        char *buf;
        str = "";
        if((buf = fgets(line, MAXLINELEN, file)) == NULL)
            return 0;
        while(buf != 0 && strlen(line) == MAXLINELEN - 1 &&
              !(line[MAXLINELEN - 2] == '\n')) {
            str += line;
            buf = fgets(line, MAXLINELEN, file);
        }
        Str::Chomp(line);
        str += line;
        return 1;
    }
    //    int GetLine(FILE *file, Stroka &str){
    //#define MAXLINELEN 1000
    //        char Line[MAXLINELEN+1];
    //        Line[MAXLINELEN] = 0;
    //        char *buf;
    //        str = "";
    //        if ( (buf=fgets(Line, MAXLINELEN, file)) == NULL )
    //            return 0;
    //        while( buf!=0 && strlen(Line)==MAXLINELEN-1 && !(Line[MAXLINELEN-2]=='\n')){
    //            str += Line;
    //            buf = fgets(Line, MAXLINELEN, file);
    //        }
    //        str += Line;
    //        return 1;
    //    }
    int GetLine(FILE *file, Stroka &str, char *Buf, int BufLen) {
        char *buf;
        str = "";
        if((buf = fgets(Buf, BufLen, file)) == NULL)
            return 0;
        while(buf != 0 && (int)strlen(Buf) == BufLen - 1 && !(Buf[BufLen - 2] == '\n')) {
            str += Buf;
            buf = fgets(Buf, BufLen, file);
        }
        str += Buf;
        return 1;
    }
    int GetLine(FILE *file, std::vector<Stroka> &line, char delim) {
        Stroka str;
        if(!GetLine(file, str))
            return 0;
        line = (delim == ' ') ? Str::SplitLine(str) : Str::SplitLine(str, 1, delim);
        return 1;
    }
    int GetLine(FILE *file, std::vector<string> &line, char delim) {
        vector<Stroka> tmp;
        int ret = GetLine(file, tmp, delim);
        for(auto &s: tmp) line.push_back(~s);
        return ret;
    }

    int GetLine(FILE *file, std::vector<Stroka> &line, char *Buf, int BufLen) {
        Stroka str;
        if(!GetLine(file, str, Buf, BufLen))
            return 0;
        line = Str::SplitLine(str);
        return 1;
    }
    std::map<Stroka, Stroka> ReadFile2Map(
        const char *file,
        const char delim,
        int KeyCol,
        int StrCol,
        const char *err_mes) {
        FILE *in = open(file, "r", err_mes);
        Stroka Buf(10000);
        map<Stroka, Stroka> res;
        Stroka line;
        while(GetLine(in, line, (char *)Buf.c_str(), (int)Buf.size())) {
            if(int(delim) == 0)
                res[line] = "";
            else {
                vector<Stroka> lines;
                if(delim == ' ')
                    lines = Str::SplitLine(line);
                else
                    lines = Str::SplitLine(line, 1, delim);
                if(lines.size() <= (size_t)KeyCol)
                    continue;
                if(StrCol == -1 || lines.size() <= (size_t)StrCol)
                    res[lines[KeyCol]] = "";
                else
                    res[lines[KeyCol]] = lines[StrCol];
            }
        }
        close(in);
        if(res.size() == 0 && err_mes)
            throw info_except(
                "File <%s> - is zero. Error mesage is:\n<%s>\n", file, err_mes);
        return res;
    }

    //  make vector of columns (for tableData)
    //  else - make vector of rows
    int ReadFile(
        const char *file,
        vector<vector<double>> &vec,
        const char *mes,
        int makeVectorOfColumns) {
        if(!file)
            throw info_except("File name is null?\n%s\n", mes);
        FILE *in = open(file, "r", mes);
        vec.clear();
        if(!in) {
            if(mes != NULL)
                throw info_except("Cannot open file %s.\n%s\n", file, mes);
            else
                return 0;
        }
        std::vector<Stroka> line;
        vector<double> cur;
        Stroka err = "";
        if(mes)
            err = mes;
        while(GetLine(in, line)) {
            cur.clear();
            for(size_t i = 0; i < line.size(); i++) {
                double f;
                if(!IsDouble(line[i].c_str(), f, 1)) {
                    cur.clear();
                    break;
                }
                cur.push_back(f);
            }
            if(cur.size() == 0)
                continue;
            if(makeVectorOfColumns) {
                if(vec.size() == 0)
                    vec.resize(cur.size());
                if(cur.size() != vec.size())
                    throw info_except(
                        "File <%s>\nstr <%s>\nsize of str %i vector size %i\n%s",
                        file,
                        Str::JoinLine(line).c_str(),
                        cur.size(),
                        vec.size(),
                        err.c_str());
                for(size_t i = 0; i < line.size(); i++)
                    vec[i].push_back(cur[i]);
            } else
                vec.push_back(cur);
        }
        close(in);
        return 1;
    }
    void WriteFile(
        const char *file,
        const vector<vector<double>> &vec,
        const char *head,
        const char *mes) {
        FILE *out = open(file, "w", mes);
        if(head) {
            Stroka tmp = Stroka(head) + "\n";
            fputs(tmp.c_str(), out);
        }
        if(vec.size() == 0)
            return;
        char Buf[256];
        for(size_t i = 0; i < vec[0].size(); i++) {
            for(size_t j = 0; j < vec.size(); j++) {
                if(vec[j][i] == NullNumber)
                    fputs("    \"\"      ", out);
                else {
                    sprintf(Buf, "%10.9g  ", vec[j][i]);
                    fputs(Buf, out);
                }
            }
            fputs("\x0d\x0a", out);
        }
        close(out);
    }
    vector<Ref<SavableClass>> ReadConf(
        const char *conf,
        size_t NumObject,
        int BreakNull) {
        FilterTextIn in(conf);
        in.SetRefMask(SavableClass::SimpleEdit);
        //SavableClass::SetRefOutMethod(SavableClass::SimpleEdit);
        Stroka tmp;
        vector<Ref<SavableClass>> ret;
        in >> tmp;
        while(!(!in) && ret.size() < NumObject) {
            Ref<SavableClass> sim = in.getobject();
            if(BreakNull && !sim)
                break;
            ret.push_back(sim);
            in >> tmp;
        }
        if(ret.size() < NumObject)
            throw info_except(
                "Wanted %i objects and got only %i\n", NumObject, ret.size());
        return ret;
    }
// TODO(MAC/WIDOWS)
//    long GetFileSize(const char *file) {
//        FILE *in = open(file, "r", NULL);
//        if (!in)
//            return -1;
//        long ret = _filelength(_fileno(in));
//        close(in);
//        return ret;
//    }
#include <sys/stat.h>
    long GetFileSize(const char *file) {
        struct stat stat_buf;
        int rc = stat(file, &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
    }


}   //    namespace File {
