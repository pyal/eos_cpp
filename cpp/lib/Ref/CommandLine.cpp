#include <lib/precompiled/Ref.h>


#include "CommandLine.h"

namespace NRef {

    Stroka TFormatOutput::Format(const char *str) {
        Stroka ret, Line;
        size_t pos = 0, len = strlen(str);
        do {
            pos = FormLine(str, pos, Line);
            ret += Line + "\n";
        } while (pos<len);
        return ret;
    }
    size_t TFormatOutput::FormLine(const char *str, size_t pos, Stroka &Line) {
        Line = "";
        size_t len = strlen(str);
        for (size_t i = 0; i < Level; i++)
            Line += Ident;
        int NextLine;
        size_t pos1 = pos;
        while (1) {
            pos1 = skipspace(str, pos1, len, NextLine, Line);
            if (NextLine || Line.size() >= MaxLen || pos1 >= len)
                return pos1;
            pos1 = skipword(str, pos1, len, NextLine, Line);
            if (NextLine || Line.size() >= MaxLen || pos1 >= len)
                return pos1;
        }
    }
    size_t TFormatOutput::skipspace(const char *str, size_t pos, size_t len, int &NextLine, Stroka &Line) {
        for (size_t i = pos; i < len; i++) {
            if (str[i] == '\n') {
                NextLine = 1;
                Line += Stroka(str, pos, i - pos);
                return i + 1;
            }
            if (str[i] == '~' && str[i+1] == '+') {
                Level++;
                Line += Stroka(str, pos, i - pos);
                i += 1;
                pos = i + 1;
                continue;
            }
            if (str[i] == '~' && str[i+1] == '-') {
                Level--;
                Line += Stroka(str, pos, i - pos);
                i += 1;
                pos = i + 1;
                continue;
            }
            if (!isspace(str[i])) {
                Line += Stroka(str, pos, i - pos);
                NextLine = 0;
                return i;
            }
        }
        Line += Stroka(str, pos, len - pos);
        NextLine = 0;
        return len;
    }
    size_t TFormatOutput::skipword(const char *str, size_t pos, size_t len, int &NextLine, Stroka &Line) {
        for (size_t i = pos; i < len; i++) {
            if (str[i] == '~' && str[i+1] == '+') {
                Level++;
                Line += Stroka(str, pos, i - pos);
                i += 1;
                pos = i+1;
                continue;
            }
            if (str[i] == '~' && str[i+1] == '-') {
                Level--;
                Line += Stroka(str, pos, i - pos);
                i += 1;
                pos = i + 1;
                continue;
            }
            if (str[i] > 0 && isspace(str[i])) {
                NextLine = 0;
                Line += Stroka(str, pos, i - pos);
                return i;
            }
        }
        NextLine = 0;
        Line += Stroka(str, pos, len - pos);
        return len;
    }

    TCommandParse::TKeyData::TKeyData(const char *mode,const char *help, const char *params, MAINFUNC *func, int givenDefaultsMode)
        :Mode(mode)
        , Help(help)
        , GivenDefaultsMode(givenDefaultsMode)
        , Func(func) {
        vector<Stroka> vec = Str::SplitLine(params, 0, '\n');
        Par2Descr.clear();
        Params2Defaults.clear();
        ParamsSettings = "";
        ParamsNames = "";
        for (size_t i = 0; i < vec.size(); i++) {
            vector<Stroka> par = Str::SplitLine(vec[i], 0);
            if (!par.size())
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
        Stroka addInfo = GivenDefaultsMode ? "(define only non default ones)" : "(define all of them)";
        Stroka ret = Help + "\nParameters" + addInfo + ": ~+\n";
        for (map<Stroka, Stroka>::iterator it = Par2Descr.begin(); it != Par2Descr.end(); it++) {
            ret += it->first + " :~+~+ " + it->second + "~-~-\n";
        }
        ret += " ~- ";
        return ret;
    }

    int TCommandParse::SimpleRun(int argc, const char *argv[]) {
        int formattedLength = 90;
        if (argc<2) {
        Stroka descr = "Program build date is :" + Stroka(__DATE__) + "\t" + Stroka(__TIME__) + "\n\n";
            Stroka str = descr + "General help for the program is:~+\n" + MainHelp + "~-\n";
            str += "Parameters defined are: ";
            for (int i = 1; i < min(argc,5); i++)
                str += Stroka("<") + argv[i] + "> ";
            str += "\nHave to define 2 paramteres: mode \"mode parameters\"\n" + MakeHelp() + "\n";
            std::cout<<TFormatOutput("    ", 0, formattedLength ).Format(~str);
            std::cout.flush();
            exit(1);
        }
        Stroka par;
        if (argc == 3)
            par = argv[2];
        else
            for(int i = 2; i < argc; i++)
                par += Stroka(argv[i]) + " ";
        map<Stroka, TKeyData>::iterator it = Params.find(argv[1]);
        if (it == Params.end()) {
            std::cout<<"Mode <"<<argv[1]<<"> is not defined.\n"<<TFormatOutput("    ", 0, formattedLength ).Format(~MakeHelp())<<"\n";
            exit(1);
        }
        if (it->second.GivenDefaults())
            it->second.Func(Str::ReadDefinedParams(~par, it->second.GetParams2Defaults(), "TOBEDEFINED"));
        else
            it->second.Func(Str::ReadParams(~par, ~it->second.GetParamsNames()));
        return 1;
    }
    Stroka TCommandParse::MakeHelp() {
        Stroka ret = "Available modes are:~+\n";
        for (map<Stroka, TKeyData>::iterator iter = Params.begin(); iter != Params.end(); iter++) {
            ret += iter->first + " \"" + iter->second.GetParamsSettings() + "\"~+\n";
            ret += iter->second.MakeHelp() + "~-\n";
        }
        ret += "~-";
        return ret;
    }

    //TCommandParse::TKeyData::TKeyData(const char *mode,const char *help, const char *params, MAINFUNC *func)
    //    :Mode(mode)
    //    , Help(help)
    //    , Func(func) {
    //    vector<Stroka> vec = Str::SplitLine(params, 0, '\n');
    //    Par2Descr.clear();
    //    Params = "";
    //    ParamsPlus = "";
    //    for (size_t i = 0; i < vec.size(); i++) {
    //        vector<Stroka> par = Str::SplitLine(vec[i], 0);
    //        Par2Descr[par[0]] = Str::JoinLine(par, ' ', 2);

    //        Params = Params + Stroka(" ") + par[0];
    //        if (ParamsPlus != "")
    //            ParamsPlus += Stroka("  ");
    //        ParamsPlus += par[0] + Stroka(" ");
    //        if (par.size() > 1)
    //            ParamsPlus += par[1];

    //    }
    //};
    //Stroka TCommandParse::TKeyData::MakeHelp() {
    //    Stroka ret = Help + "\nParameters: ~+\n";
    //    for (map<Stroka, Stroka>::iterator it = Par2Descr.begin(); it != Par2Descr.end(); it++) {
    //        ret += it->first + " :~+~+ " + it->second + "~-~-\n";
    //    }
    //    ret += " ~- ";
    //    return ret;
    //}

    //int TCommandParse::SimpleRun(int argc, const char *argv[]) {
    //    if (argc<2 || argc>3) {
    //        Stroka str = "General help for the program is:~+\n" + MainHelp + "~-\n";
    //        str += "Parameters defined are: ";
    //        for (int i = 1; i < min(argc,5); i++)
    //            str += Stroka("<") + argv[i] + "> ";
    //        str += "\nHave to define 2 paramteres: mode \"mode parameters\"\n" + MakeHelp() + "\n";
    //        std::cout << ~(TFormatOutput("    ", 0, 70).Format(~str));
    //        std::cout.flush();
    //        exit(1);
    //    }
    //    Stroka par;
    //    if (argc == 3)
    //        par = argv[2];
    //    map<Stroka, TKeyData>::iterator it = Params.find(argv[1]);
    //    if (it == Params.end()) {
    //        std::cout<<"Mode <"<<argv[1]<<"> is not defined.\n"<<~(TFormatOutput("    ", 0, 70).Format(~MakeHelp()))<<"\n";
    //        exit(1);
    //    }
    //    it->second.Func(Str::ReadParams(par.c_str(), it->second.Params.c_str()));
    //    return 1;
    //}
    //Stroka TCommandParse::MakeHelp() {
    //    Stroka ret = "Available modes are:~+\n";
    //    for (map<Stroka, TKeyData>::iterator iter = Params.begin(); iter != Params.end(); iter++) {
    //        ret += iter->first + " \"" + iter->second.ParamsPlus + "\"~+\n";
    //        ret += iter->second.MakeHelp() + "~-\n";
    //    }
    //    ret += "~-";
    //    return ret;
    //}























CommandLineExe::KeyData& CommandLineExe::Get(const char *name){
    map<Stroka, KeyData>::iterator iter = Params.find(name);
    if (iter==Params.end()) return Zero;
    return iter->second;
};
Stroka CommandLineExe::MakeHelp(){
    //Stroka ret(Format->Format(MainHelp.c_str()));
    Stroka ret = "Main help:~+\n" + MainHelp + "~-\n" + "Available keys are:~+\n";
    //Format->SetLevel(Format->GetLevel()+1);
    for (map<Stroka, KeyData>::iterator iter = Params.begin(); iter != Params.end(); iter++){
        ret += Stroka(CommandPrefix) + iter->second.Key + "~+\n" + iter->second.Help + "~-\n";
        //Stroka Add("option:");
        //Add += CommandPrefix;
        //Add += Stroka(iter->second.Key)+iter->second.Help;//+"\n";
        //ret += Format->Format(Add.c_str());
    }
    //Format->SetLevel(Format->GetLevel()-1);
    return TFormatOutput("    ", 0, 70).Format(~ret);
    //return ret;
}
void CommandLineExe::Analize(int argc, const char *argv[]){
    Arguments = GetCommandLine(argc, argv);
    ExtractKeyData();
    SetCommandLine(ArgumentsExtra, Argc, Argv);
}
int CommandLineExe::Get(vector<Stroka> &args, int ExtractKeys){
    args.clear();
    if (Arguments.size()==0) return 0;
    if (!ExtractKeys) args = Arguments;
    else args = ArgumentsExtra;
    return 1;
}
void CommandLineExe::ExtractKeyData(){
    Stroka Name;
    for (map<Stroka, KeyData>::iterator iter = Params.begin();iter!=Params.end();iter++)
        iter->second.SetVal("", 0);
    for (vector<Stroka>::iterator iter = Arguments.begin();iter!=Arguments.end();iter++){
        if (!KeyName(iter->c_str(), Name)) ArgumentsExtra.push_back(*iter);
        else Params[Name].SetVal(&(iter->c_str()[2]), 1);
    }
}
vector<Stroka> CommandLineExe::GetCommandLine(int argc,const char *argv[]){
    vector<Stroka> out(argc);
    for (size_t  i = 0;i<out.size();i++)
        out[i] = argv[i];
    return out;
}
void CommandLineExe::SetCommandLine(const vector<Stroka> vec, size_t &argc,const char *argv[]){
    argc = vec.size();
    for (size_t  i = 0;i<vec.size();i++)
        argv[i] = vec[i].c_str();
}
int CommandLineExe::KeyName(const char *arg, Stroka &Name){
    Name = "";
    if (arg[0]!=CommandPrefix) return 0;
    for (map<Stroka, KeyData>::iterator iter = Params.begin();iter!=Params.end();iter++){
        if (iter->second.Key==arg[1]) {
            Name = iter->first;
            return 1;
        }
    }
    return 0;
}








} // namespace NRef


//
//Stroka aFormatStrSimple::Format(const char *str){
//    Stroka id;
//    Stroka ret;
//    for (int i = 0;i<Level;i++) 
//        id += Ident;
//    const char *beg = str, *end;
//    while ((end=strchr(beg,'\n'))!=0) {
//        ret = ret + id + Stroka(beg, 0, end-beg) + "\n";
//        beg = end + 1;
//    }
//    ret = ret + id + Stroka(beg) + "\n";
//    return ret;
//}
//CommandLineExe::KeyData& CommandLineExe::Get(const char *name){
//    map<Stroka, KeyData>::iterator iter = Params.find(name);
//    if (iter==Params.end()) return Zero;
//    return iter->second;
//};
//Stroka CommandLineExe::MakeHelp(){
//    Stroka ret(Format->Format(MainHelp.c_str()));
//    ret += Format->Format("Available keys are:\n");
//    Format->SetLevel(Format->GetLevel()+1);
//    for (map<Stroka, KeyData>::iterator iter = Params.begin();iter!=Params.end();iter++){
//        Stroka Add("option:");
//        Add += CommandPrefix;
//        Add += Stroka(iter->second.Key)+iter->second.Help;//+"\n";
//        ret += Format->Format(Add.c_str());
//    }
//    Format->SetLevel(Format->GetLevel()-1);
//    return ret;
//}
//void CommandLineExe::Analize(int argc, const char *argv[]){
//    Arguments = GetCommandLine(argc, argv);
//    ExtractKeyData();
//    SetCommandLine(ArgumentsExtra, Argc, Argv);
//}
//int CommandLineExe::Get(vector<Stroka> &args, int ExtractKeys){
//    args.clear();
//    if (Arguments.size()==0) return 0;
//    if (!ExtractKeys) args = Arguments;
//    else args = ArgumentsExtra;
//    return 1;
//}
//void CommandLineExe::ExtractKeyData(){
//    Stroka Name;
//    for (map<Stroka, KeyData>::iterator iter = Params.begin();iter!=Params.end();iter++)
//        iter->second.SetVal("", 0);
//    for (vector<Stroka>::iterator iter = Arguments.begin();iter!=Arguments.end();iter++){
//        if (!KeyName(iter->c_str(), Name)) ArgumentsExtra.push_back(*iter);
//        else Params[Name].SetVal(&(iter->c_str()[2]), 1);
//    }
//}
//vector<Stroka> CommandLineExe::GetCommandLine(int argc,const char *argv[]){
//    vector<Stroka> out(argc);
//    for (size_t  i = 0;i<out.size();i++)
//        out[i] = argv[i];
//    return out;
//}
//void CommandLineExe::SetCommandLine(const vector<Stroka> vec, size_t &argc,const char *argv[]){
//    argc = vec.size();
//    for (size_t  i = 0;i<vec.size();i++)
//        argv[i] = vec[i].c_str();
//}
//int CommandLineExe::KeyName(const char *arg, Stroka &Name){
//    Name = "";
//    if (arg[0]!=CommandPrefix) return 0;
//    for (map<Stroka, KeyData>::iterator iter = Params.begin();iter!=Params.end();iter++){
//        if (iter->second.Key==arg[1]) {
//            Name = iter->first;
//            return 1;
//        }
//    }
//    return 0;
//}


