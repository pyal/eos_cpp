#pragma once

#include "lib/std/util.h"
#include <vector>
#include <map>
#include "Stroka.h"

namespace Str {
    enum Language {Russian, English};
// Spliting string using delimiter Delim,
// If CreateZeroString - creating also ZeroWords, when delimiters go one after another without space
    //std::vector<Stroka> SplitLine(const Stroka &word, int CreateZeroString=0, char Delim=' ');
    std::vector<Stroka> SplitLine(const Stroka &word, int CreateZeroString, char Delim=' ');
// Spliting string - remove space
    std::vector<Stroka> SplitLine(const Stroka &word);
// Joid vector elements into string
    Stroka JoinLine(const std::vector<Stroka> &vec, char Delim=' ', int FromInd = -1, int ToInd = -1);
// Find a given word in the WordLst, uses SplitLine
// If CreateZeroString - creating also ZeroWords, when delimiters go one after another without space
    int FindWord(const Stroka &word_lst, const Stroka &word, int CreateZeroString=0, char Delim=' ');
// Extract given params from the string, if no - throw exception
    std::map<Stroka,Stroka> ReadParams(const Stroka &Params,const char *CheckParams);

    size_t SkipWord(const char *str, size_t pos, size_t len, int CheckSpace );
    size_t SkipSpace(const char *str, size_t pos, size_t len, int CheckSpace );
    size_t ReverseSkipWord(const char *str, size_t pos, int CheckSpace );
    size_t ReverseSkipSpace(const char *str, size_t pos, int CheckSpace );


    int IsRussianLanguageFast(const char *str1);
    int IsRussianLanguage(const char *str1);
    int IsEnglishLanguage(const char *str1);
    int IsLanguage(const char *str1, int lang);
    int IsRussianLanguageExt(const char *str1);
    int NumRussianLetters(const char *str1,int CountSpec = 0);
    bool IsRussianChar(char ch, int CountSpec = 0);
    bool IsEnglishChar(char ch);
    bool IsExtendedChar(char ch);
    int NumEnglishLetters(const char *str1);
// For unix compatibility... Not really necessary
    int stricmp(const char *str1, const char *str2);
    char* itoa(int i, char *buf);

    void EncodeData(char *str, size_t len, char table[256]);
    void MakeEncodeTable(char *buf, const char *fr, const char *to, char def);
    void MakeSpaceEncodeTable(char *buf, char def);

    map<int, Stroka> MakeErrDescr(const Stroka &str);//"1:OK;2:very bad"

    struct OptionSet{
        OptionSet(const Stroka &opt){
            Init(opt);
        }
        void Init(const Stroka &opt){
            iOpt = Str::SplitLine(opt);
            sOpt.clear();
            for(size_t i = 0;i<iOpt.size();i++)
                sOpt[iOpt[i]] = (int)i;
        }
        int Check(int i, const char *mes ){
            if (i < 0 || i >= (int)iOpt.size())
                if (mes)
                    throw info_except("Option <%i> is not defined. Defined options: [%s].\n%s\n", i, Str::JoinLine(iOpt).c_str(), mes);
                else
                    return 0;
            return 1;
        }
        int Check(const Stroka &str, const char *mes){
            if (sOpt.find(str) == sOpt.end())
                if (mes)
                    throw info_except("Option <%s> is not defined. Defined options: [%s].\n%s\n", str.c_str(), Str::JoinLine(iOpt).c_str(), mes);
                else
                    return 0;
            return 1;
        }
        Stroka Str(int i, const char *mes ){
            if (!Check(i, mes))
                return Stroka("");
            return iOpt[i];
        }
        int Int(const Stroka str, const char *mes = NULL){
            if (!Check(str, mes))
                return -1;
            return sOpt[str];
        }
        Stroka ListOptions() {
            Stroka ret;
            for (size_t i = 0; i < iOpt.size(); i++) {
                ret += Stroka::Int2Str((int)i) + ":" + iOpt[i] + " ";
            }
            return ret;
        }
    private:
        map<Stroka, int> sOpt;
        vector<Stroka> iOpt;
    };

    bool GetNumberedVar(const Stroka &input, const Stroka &prefix, int &res, int LimLow = -1, int LimUp = -1);

    struct StrokaStorage {
        StrokaStorage(){
            Init();
        }
        int Str2Num(const char *str, int AddIfNo){
            if (strlen(str) == 0)
                return -1;
            std::map<Stroka, int>::iterator it = StrMap.find(Stroka(str));
            if (it == StrMap.end())
                return AddIfNo ? Add(str) : -1;
            return it->second;
        }
        int Str2Num(const char *str){
            if (strlen(str) == 0)
                throw info_except("NULL str number?\n");
            std::map<Stroka, int>::iterator it = StrMap.find(Stroka(str));
            if (it == StrMap.end())
                throw info_except("Str <%s> is not found. Map size is %i\n", str, StrMap.size());
            return it->second;
        }
        int Size() {
            return LastNum;
        }
        const char *Num2Str(int Num) {
            if (Num >= 0 && Num < LastNum)
                return StrVec[Num].c_str();
            return NULL;
        }
        void Clear(){
            StrMap.clear();
            StrVec.clear();
            LastNum = 0;
            Init();
        }
        void Init(){
            LastNum = 0;
        }
    private:
        std::vector<Stroka>  StrVec;
        std::map<Stroka, int> StrMap;
        int LastNum;
        int Add(const char *str){
            assert(StrMap.size() == LastNum && StrVec.size() == LastNum);
            StrMap[str] = LastNum;
            StrVec.push_back(str);
            LastNum++;
            return LastNum-1;
        }

    };
}// namespace Str



