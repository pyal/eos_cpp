
#include "str_manip.h"
#include "CommandParse.h"
#include "version.h"

namespace NRef {
    Stroka TFormatOutput::Format(const char *str) {
        Stroka ret, Line;
        size_t pos = 0, len = strlen(str);
        do {
            pos = FormLine(str, pos, Line);
            ret += Line + "\n";
        } while(pos < len);
        return ret;
    }

    size_t TFormatOutput::FormLine(const char *str, size_t pos, Stroka &Line) {
        Line = "";
        size_t len = strlen(str);
        for(size_t i = 0; i < Level; i++)
            Line += Ident;
        int NextLine;
        size_t pos1 = pos;
        while(1) {
            pos1 = skipspace(str, pos1, len, NextLine, Line);
            if(NextLine || Line.size() >= MaxLen || pos1 >= len)
                return pos1;
            pos1 = skipword(str, pos1, len, NextLine, Line);
            if(NextLine || Line.size() >= MaxLen || pos1 >= len)
                return pos1;
        }
    }

    size_t TFormatOutput::skipspace(
        const char *str,
        size_t pos,
        size_t len,
        int &NextLine,
        Stroka &Line) {
        for(size_t i = pos; i < len; i++) {
            if(str[i] == '\n') {
                NextLine = 1;
                Line += Stroka(str, pos, i - pos);
                return i + 1;
            }
            if(str[i] == '~' && str[i + 1] == '+') {
                Level++;
                Line += Stroka(str, pos, i - pos);
                i += 1;
                pos = i + 1;
                continue;
            }
            if(str[i] == '~' && str[i + 1] == '-') {
                Level--;
                Line += Stroka(str, pos, i - pos);
                i += 1;
                pos = i + 1;
                continue;
            }
            if(str[i] < 0 || !isspace(str[i])) {
                Line += Stroka(str, pos, i - pos);
                NextLine = 0;
                return i;
            }
        }
        Line += Stroka(str, pos, len - pos);
        NextLine = 0;
        return len;
    }

    size_t TFormatOutput::skipword(
        const char *str,
        size_t pos,
        size_t len,
        int &NextLine,
        Stroka &Line) {
        for(size_t i = pos; i < len; i++) {
            if(str[i] == '~' && str[i + 1] == '+') {
                Level++;
                Line += Stroka(str, pos, i - pos);
                i += 1;
                pos = i + 1;
                continue;
            }
            if(str[i] == '~' && str[i + 1] == '-') {
                Level--;
                Line += Stroka(str, pos, i - pos);
                i += 1;
                pos = i + 1;
                continue;
            }
            if(str[i] > 0 && isspace(str[i])) {
                NextLine = 0;
                Line += Stroka(str, pos, i - pos);
                return i;
            }
        }
        NextLine = 0;
        Line += Stroka(str, pos, len - pos);
        return len;
    }

    TCommandParse::TKeyData::TKeyData(
        const char *mode,
        const char *help,
        const char *params,
        MAINFUNC *func,
        int givenDefaultsMode)
        : Mode(mode), Help(help), GivenDefaultsMode(givenDefaultsMode), Func(func) {
        vector<Stroka> vec = Str::SplitLine(params, 0, '\n');
        Par2Descr.clear();
        Params2Defaults.clear();
        ParamsSettings = "";
        ParamsNames = "";
        for(size_t i = 0; i < vec.size(); i++) {
            vector<Stroka> par = Str::SplitLine(vec[i], 0);
            if(!par.size())
                continue;
            Par2Descr[par[0]] = Str::JoinLine(par, ' ', 2);
            Stroka defaultValue = par.size() > 1 ? par[1] : " ";
            Params2Defaults[par[0]] = defaultValue;
            Stroka prefix = (ParamsSettings.size() != 0) ? "  " : "";
            ParamsSettings += prefix + par[0] + Stroka(" ") + defaultValue;
            ParamsNames += prefix + par[0];
        }
    };

    Stroka TCommandParse::TKeyData::MakeHelp() {
        Stroka addInfo =
            GivenDefaultsMode ? "(define only non default ones)" : "(define all of them)";
        Stroka ret = Help + "\nParameters" + addInfo + ": ~+\n";
        for(map<Stroka, Stroka>::iterator it = Par2Descr.begin(); it != Par2Descr.end();
            it++) {
            ret += it->first + " :~+~+ " + it->second + "~-~-\n";
        }
        ret += " ~- ";
        return ret;
    }

    int TCommandParse::SimpleRun(int argc, const char *argv[]) {
        int formattedLength = 90;
        if(argc < 2) {
            Stroka descr = "Program build date is :" + Stroka(__DATE__) + "\t" +
                           Stroka(__TIME__) + "\n";
            descr += Stroka("Build version ") + VERSION + "\n\n";
            descr += "General help for the program is:~+\n" + MainHelp + "~-\n";
            descr += "Parameters defined: {";
            for(int i = 1; i < std::min(argc, 5); i++)
                descr += Stroka("<") + argv[i] + "> ";
            descr += "}\nHave to define: mode  \"mode parameters\"\n" + MakeHelp() + "\n";
            cout << TFormatOutput("    ", 0, formattedLength).Format(~descr);
            cout.flush();
            exit(1);
        }
        Stroka par;
        if(argc == 3)
            par = argv[2];
        else
            for(int i = 2; i < argc; i++)
                par += Stroka(argv[i]) + " ";
        map<Stroka, TKeyData>::iterator it = Params.find(argv[1]);
        if(it == Params.end()) {
            cout << "Mode <" << argv[1] << "> is not defined.\n"
                 << TFormatOutput("    ", 0, formattedLength).Format(~MakeHelp()) << "\n";
            exit(1);
        }
        if(it->second.GivenDefaults())
            it->second.Func(Str::ReadDefinedParams(
                ~par, it->second.GetParams2Defaults(), "TOBEDEFINED"));
        else
            it->second.Func(Str::ReadParams(~par, ~it->second.GetParamsNames()));
        return 1;
    }

    Stroka TCommandParse::MakeHelp() {
        Stroka ret = "Available modes are:~+\n";
        for(map<Stroka, TKeyData>::iterator iter = Params.begin(); iter != Params.end();
            iter++) {
            ret += iter->first + " \"" + iter->second.GetParamsSettings() + "\"~+\n";
            ret += iter->second.MakeHelp() + "~-\n";
        }
        ret += "~-";
        return ret;
    }
}