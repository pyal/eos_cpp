
#include <lib/precompiled/Ref.h>
#include <lib/ref/data_manip.h>
#include <lib/ref/file_manip.h>

#include "str_manip.h"


static ClassDesc StrokaStorage_ClassDesc(
    typeid(Str::StrokaStorage),
    "StrokaStorage",
    1,
    "StrManip_category",
    create<Str::StrokaStorage>);


namespace Str {

    vector<Stroka> SplitLine(const Stroka &word, int CreateZeroString, char Delim) {
        vector<Stroka> splittedWords;
        Stroka word1(word);
        splittedWords.clear();
        char *dat = word1.begin(), *Start;
        int i = 0;
        if(!CreateZeroString) {
            while(dat[i] && dat[i] == Delim)
                i++;
        }
        Start = &dat[i];
        while(dat[i])
            if(dat[i] == Delim) {
                dat[i] = 0;
                splittedWords.push_back(Stroka(Start));
                i++;
                if(!CreateZeroString) {
                    while(dat[i] && dat[i] == Delim)
                        i++;
                }
                Start = &dat[i];
            } else
                i++;
        if(CreateZeroString || (Start != &dat[i]))
            splittedWords.push_back(Stroka(Start));
        return splittedWords;
    };
    vector<Stroka> SplitLine(const Stroka &word) {
        vector<Stroka> splittedWords;
        char *str = (char *)word.c_str();
        splittedWords.clear();
        size_t len = word.size(), pos = 0;
        pos = SkipSpace(str, pos, len, 1);
        while(pos < len) {
            size_t end = SkipWord(str, pos, len, 1);
            splittedWords.push_back(Stroka(str, pos, end - pos));
            pos = SkipSpace(str, end, len, 1);
        }
        return splittedWords;
    };
    Stroka JoinLine(const std::vector<Stroka> &vec, char Delim, int FromInd, int ToInd) {
        char tmp[2];
        tmp[0] = Delim;
        tmp[1] = 0;
        return JoinLine(vec, tmp, FromInd, ToInd);
    }
    Stroka JoinLine(
        const std::vector<Stroka> &vec,
        const char *Delim,
        int FromInd,
        int ToInd) {
        if(FromInd < 0)
            FromInd = 0;
        if(ToInd < 0)
            ToInd = (int)vec.size();
        if(FromInd >= (int)vec.size())
            return Stroka("");
        Stroka ret = "";
        if((size_t)FromInd < vec.size())
            ret = vec[FromInd];
        for(int i = FromInd + 1; i < ToInd; i++)
            ret = ret + Delim + vec[i];
        return ret;
    }

    std::map<Stroka, Stroka> ReadParams(const vector<Stroka> &Params, const char *CheckParams) {
        std::map<Stroka, Stroka> Res;
        std::vector<Stroka> wrds = Params;
        for(size_t i = 0; i + 1 < wrds.size(); i += 2)
            Res[wrds[i]] = wrds[i + 1];
        std::vector<Stroka> tst = SplitLine(Stroka(CheckParams));
        for(size_t i = 0; i + 1 < tst.size(); i++)
            if(Res.find(tst[i]) == Res.end())
                throw info_except(
                    "Did not find parameter <%s>. Parameters to be defined are:\n%s\nInput string \n%s\n",
                    tst[i].c_str(),
                    CheckParams,
                    ~Str::JoinLine(Params));
        return Res;
    }

    map<Stroka, Stroka> ReadDefinedParams(
        const vector<Stroka> &params,
        const map<Stroka, Stroka> &definedParams,
        const char *nonDefaultMark,
        const char *errorDescr_) {
        map<Stroka, Stroka> res = definedParams;
        Stroka errorDescr = Stroka("Can set parameters:\n") +
                            JoinLine(DataManip::Map2Vector(definedParams), '|') +
                            "\nInput string \n" + Str::JoinLine(params) + "\n";
        if(errorDescr_)
            errorDescr += Stroka("Description:<") + errorDescr_ + ">\n";
        vector<Stroka> wrds = params;
        if(wrds.size() % 2 != 0)
            throw info_except(
                "Number of words have to even, and it is not.\n%s", ~errorDescr);
        for(size_t i = 0; i + 1 < wrds.size(); i += 2) {
            if(definedParams.find(wrds[i]) == definedParams.end()) {
                //cout << " Descr " << errorDescr << " \nParameter <" << wrds[i] << "\n";  cout.flush();
                throw info_except("Unknown parameter <%s>. %s\n", ~wrds[i], ~errorDescr);
                //throw info_except(~(Stroka("Unknown parameter <") + wrds[i] + ">\nDescr:\n" + errorDescr));
            }
            res[wrds[i]] = wrds[i + 1];
        }
        for(map<Stroka, Stroka>::iterator it = res.begin(); it != res.end(); it++)
            if(nonDefaultMark && it->second == Stroka(nonDefaultMark))
                throw info_except(
                    "Mandatory parameter (marked as %s ) is not defined <%s> %s. Formed params:\n%s\n",
                    nonDefaultMark,
                    ~it->first,
                    ~errorDescr,
                    ~DataManip::Map2Str(res, "\t"));
        return res;
    }

    int FindWord(
        const Stroka &word_lst,
        const Stroka &word,
        int CreateZeroString,
        char Delim) {
        std::vector<Stroka> wrds = SplitLine(word_lst, CreateZeroString, Delim);
        for(size_t i = 0; i < wrds.size(); i++)
            if(wrds[i] == word)
                return (int)i;
        throw info_except(
            "Word %s was not found in the list \n%s\n", word.c_str(), word_lst.c_str());
    }


    size_t SkipWord(const char *str, size_t pos, size_t len, int CheckSpace) {
        if(CheckSpace) {
            while(pos < len && !isspace((unsigned char)str[pos]))
                pos++;
        } else {
            while(pos < len && str[pos] != 0)
                pos++;
        }
        return pos;
    }
    size_t SkipSpace(const char *str, size_t pos, size_t len, int CheckSpace) {
        if(CheckSpace) {
            while(pos < len && isspace((unsigned char)str[pos]))
                pos++;
        } else {
            while(pos < len && str[pos] == 0)
                pos++;
        }
        return pos;
    }
    size_t ReverseSkipWord(const char *str, size_t pos, int CheckSpace) {
        if(CheckSpace) {
            while(pos > 0 && !isspace((unsigned char)str[pos]))
                pos--;
        } else {
            while(pos > 0 && str[pos] != 0)
                pos--;
        }
        return pos;
    }
    size_t ReverseSkipSpace(const char *str, size_t pos, int CheckSpace) {
        if(CheckSpace) {
            while(pos > 0 && isspace((unsigned char)str[pos]))
                pos--;
        } else {
            while(pos > 0 && str[pos] == 0)
                pos--;
        }
        return pos;
    }

    int IsGlasn(char ch) {
        return (strchr("йуеыаоэяиюё", ch) != NULL);
    }
    int IsSoGlasn(char ch) {
        return (strchr("цкншщхъфвпрлджчсмть", ch) != NULL);
    }
    int IsRussianLanguageFast(const char *str1) {
        for(size_t k = 0; k < strlen(str1); k++)
            if(str1[k] > 0 && strchr("%&$#!~`;=+_", str1[k]) == NULL)
                return 0;
        return 1;
    }
    int IsRussianLanguage(const char *str1) {
        if(strchr("-0123456789", str1[0]))
            return 0;
        int RusLet = NumRussianLetters(str1), EngLet = NumEnglishLetters(str1);
        return (RusLet > EngLet);
        //return (NumRussianLetters(str1)>0.5*std::strlen(str1));
    }
    int IsEnglishLanguage(const char *str1) {
        if(strchr("-0123456789", str1[0]))
            return 0;
        int RusLet = NumRussianLetters(str1), EngLet = NumEnglishLetters(str1);
        if(RusLet > 0.3 * EngLet)
            return 0;
        return (EngLet > 0.5 * std::strlen(str1));
        //return (NumRussianLetters(str1)>0.5*std::strlen(str1));
    }
    int IsLanguage(const char *str1, int lang) {
        if(lang == English)
            return IsEnglishLanguage(str1);
        if(lang == Russian)
            return IsRussianLanguage(str1);
        throw info_except("Language %i is not defined.\n", lang);
        //return 0;
    }

    int IsRussianLanguageExt(const char *str1) {
        int RusLet = NumRussianLetters(str1), EngLet = NumEnglishLetters(str1);
        if(RusLet <= EngLet)
            return 0;
        return (RusLet > 0.5 * std::strlen(str1));
    }
    int NumRussianLetters(const char *str1, int CountSpec) {
        int ret = 0;
        for(size_t k = 0; k < strlen(str1); k++)
            if(IsRussianChar(str1[k], CountSpec))
                ret++;
        return ret;
    }
    bool IsRussianChar(char ch, int CountSpec) {
        if(!CountSpec)
            return (
                strchr(
                    "ёйцукенгшщзхъфывапролджэячсмитьбюЁЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ",
                    (unsigned char)ch) != NULL);
        return (
            strchr(
                "ёйцукенгшщзхъфывапролджэячсмитьбюЁЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ",
                (unsigned char)ch) != NULL);
    }
    bool IsEnglishChar(char ch) {
        return (
            strchr(
                "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM",
                (unsigned char)ch) != NULL);
    }
    bool IsExtendedChar(char ch) {
        return (strchr("_-1234567890()", (unsigned char)ch) != NULL);
    }
    int NumEnglishLetters(const char *str1) {
        int ret = 0;
        for(size_t k = 0; k < strlen(str1); k++)
            if(strchr(
                   "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM",
                   (unsigned char)str1[k]) != NULL)
                ret++;
        return ret;
    }


    void EncodeData(char *str, size_t len, char table[256]) {
        assert(len < 10000000);
        for(size_t i = 0; i < len; i++)
            str[i] = (unsigned char)table[(unsigned char)str[i]];
    }
    void MakeEncodeTable(char *buf, const char *fr, const char *to, char def) {
        for(size_t i = 0; i < 256; i++)
            buf[i] = def;
        for(size_t i = 0; i < strlen(fr); i++) {
            buf[(unsigned char)fr[i]] = (unsigned char)to[i];
        }
    }
    void MakeSpaceEncodeTable(char *buf, char def) {
        for(size_t i = 0; i < 256; i++)
            if(!isspace((int)i))
                buf[i] = (unsigned char)i;
            else
                buf[i] = def;
    }
    map<int, Stroka> MakeErrDescr(const Stroka &str) {
        vector<Stroka> vec = SplitLine(str, 0, ';');
        map<int, Stroka> ret;
        for(size_t i = 0; i < vec.size(); i++) {
            vector<Stroka> v = SplitLine(vec[i], 0, ':');
            assert(v.size() == 2);
            ret[atoi(v[0].c_str())] = v[1];
        }
        return ret;
    }

    bool GetNumberedVar(
        const Stroka &input,
        const Stroka &prefix,
        int &res,
        int LimLow,
        int LimUp) {
        if(input.substr(0, prefix.length()) != prefix)
            return false;
        res = atoi(input.substr(prefix.length()).c_str());
        if(LimLow == -1 && LimUp == -1)
            return true;
        return (res >= LimLow && res < LimUp);
    }

    const char *Chomp(const char *src_) {
        char *src = (char *)src_;
        if(!src)
            return src;
        int l = strlen(src) - 1;
        while(l >= 0 && (src[l] == '\n' || src[l] == '\r'))
            l--;
        src[l + 1] = 0;
        return src;
    }

    void StrokaStorage::ReadFromFile(const char *file) {
        Clear();
        FILE *in = File::open(file, "r", "Could not open StrokaStorage file");
        std::vector<Stroka> line;
        while(File::GetLine(in, line)) {
            if(line.size() >= 1) {
                std::map<Stroka, int>::iterator it = StrMap.find(line[0]);
                if(it == StrMap.end())
                    Add(~line[0]);
            }
        }
    }
    void StrokaStorage::SaveToFile(const char *file) {
        FILE *out = File::open(file, "w", "Could not write open StrokaStorage file");
        fprintf(out, "%s", ~Str::JoinLine(ToStrVec(), "\n"));

        File::close(out);
    }


    void TestChar(const Stroka &str, int pos, char ch) {
        if(str[pos] != ch)
            throw info_except(
                "in string \n%s\nchar at pos %i is %c not %c\n", ~str, pos, str[pos], ch);
    }

    template <>
    Stroka Obj2Str<Ref<SavableClass>>(const Ref<SavableClass> &val) {
        return SavableClass::object2string(val.pointer());
    }
    template <>
    Stroka Obj2Str<Stroka>(const Stroka &val) {
        return val;
    }
    template <>
    Stroka Obj2Str<int>(const int &val) {
        char tmp[256];
        sprintf(tmp, "%i", val);
        return Stroka(tmp);
    }
    template <>
    Stroka Obj2Str<double>(const double &val) {
        char tmp[256];
        sprintf(tmp, "%f", val);
        return Stroka(tmp);
    }

    template <>
    int Str2Obj<int>(int &val, const Stroka &str, int start) {
        start = SkipSpace(~str, start, str.size(), 1);
        int end = SkipWord(~str, start, str.size(), 1);
        val = atoi(~Stroka(str, start, end));
        return end;
    }
    template <>
    int Str2Obj<double>(double &val, const Stroka &str, int start) {
        start = SkipSpace(~str, start, str.size(), 1);
        int end = SkipWord(~str, start, str.size(), 1);
        val = atof(~Stroka(str, start, end));
        return end;
    }


}   // namespace Str
