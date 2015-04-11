//
// CommandLine.h
//

#ifndef _COMMANDLINE_H
#define _COMMANDLINE_H

#include "class_sav.h"
#include "stroka.h"

#include "str_manip.h"
    

namespace NRef {

// Formating output - given maxlen - will limit the string length
// If in the string ~- or ~+ - will decrease or increase ident levels (given ident str) of the lines
    class TFormatOutput {
    private:
        size_t Level, MaxLen;
        Stroka Ident;
        size_t FormLine(const char *str, size_t pos, Stroka &line);
        size_t skipspace(const char *str, size_t pos, size_t len, int &NextLine, Stroka &line);
        size_t skipword(const char *str, size_t pos, size_t len, int &NextLine, Stroka &line);
    public:
        TFormatOutput(const char *ident = 0, int level = 0, int maxlen = -1)
            : Level(level)
            , MaxLen(maxlen) {
            if (ident != 0) 
                Ident = ident;
        }
        Stroka Format(const char *str);
    };

// Class TCommandParse - start functions of the type
typedef void (MAINFUNC) (map<Stroka,Stroka> );

// Generate help for the program and all its modes. Parse program input, check given parameters.
// Std command line format
// program.exe mode "Input aaa Output bbb"
    class TCommandParse {
    private:
// Data for the mode
        struct TKeyData {
        private:
            char Key;
            int Exist;
            Stroka Mode, Help;
            map<Stroka, Stroka> Par2Descr;
        public:
            TKeyData() {
            }
            TKeyData(const char *mode,const char *help, const char *params, MAINFUNC *func);
            Stroka MakeHelp();

            Stroka Params, ParamsPlus;
            MAINFUNC *Func;
        };
        Stroka MakeHelp();
        map<Stroka, TKeyData> Params;
    public:
        TCommandParse(const char *mainhelp)
            : MainHelp(mainhelp) {
        };
// Add new program mode
        void Add(void (func) (map<Stroka,Stroka> ), const char *mode, const char *help, const char *params) {
            Params[Stroka(mode)] = TKeyData(mode, help, params, func);
        }
// Parse input, run proper function
        int SimpleRun(int argc, const char *argv[]);
// Put here general information about the program
        Stroka MainHelp;
    };


// Class for extracting command line keys and purging the rest command arguments
// stores help info for all keys, capable to print it
// Keys are referenced by there names
// checks if the key is setted in the command line, 
// if argument of the type /p23 - 23 will be stored as value of key
class CommandLineExe{
public:
    //TFormatOutput Format;
    //Ref<FormatStrClass> Format;
    CommandLineExe(const char *mainhelp, char commandprefix = 0)
        : MainHelp(mainhelp), CommandPrefix(commandprefix){
        //:Format(new aFormatStrSimple("   ", 0)), MainHelp(mainhelp), CommandPrefix(commandprefix){
            if (CommandPrefix==0){
#ifdef WIN32
                CommandPrefix = '/';
#else
                CommandPrefix = '-';
#endif
            }
    };
    struct KeyData{
        KeyData(char key,const char *name,const char *help):Key(key), Exist(0), Help(help), Name(name){
        };
        KeyData():Key(0), Exist(0){
        };
        void SetVal(const char *val, int exist){
            Exist = exist;
            Val = val;
        }
        char Key;
        int Exist;
        Stroka Help;
        Stroka Name;
        Stroka Val;
    };
// Get the KeyData info for the key with given name
    KeyData& Get(const char *name);
// Generate help for the program and all its keys
    Stroka MakeHelp();
// Store next key
    void Add(char key, const char *name, const char *help){
        Params[Stroka(name)] = KeyData(key, name, help);
    }
// Insert command line arguments, to analize presence of keys
// Creates purged command line arguments  Argc    Argv  - without key args
    void Analize(int argc, const char *argv[]);
// Extract array of args, with or without keys ( parameter ExtractKeys  )
    int Get(vector<Stroka> &args, int ExtractKeys = 1);
// purged command line arguments  Argc    Argv  - without key args
    size_t Argc;
    const char *Argv[1024];
    Stroka MainHelp;
private:
    char CommandPrefix;
    vector<Stroka> GetCommandLine(int argc,const char *argv[]);
    void SetCommandLine(const vector<Stroka> vec, size_t &argc,const char *argv[]);
    void ExtractKeyData();
    int KeyName(const char *arg, Stroka &Name);


    map<Stroka, KeyData> Params;
    vector<Stroka> Arguments;
    vector<Stroka> ArgumentsExtra;
    KeyData Zero;
};


} // namespace NRef



//// Class for formating of the string, by level
//class FormatStrClass: public DescribedClass{
//public:
//    virtual int GetLevel() = 0;
//    virtual int SetLevel(int level) = 0;
//    virtual Stroka Format(const char *str) = 0;
//    virtual ~FormatStrClass(){}
//};
//class aFormatStrSimple:public FormatStrClass{
//public:
//    aFormatStrSimple(const char *ident = 0, int level = 0):Level(level){
//        if (ident!=0) Ident = ident;
//    }
//    virtual int GetLevel(){
//        return Level;
//    }
//    virtual int SetLevel(int level){
//        Level = level;
//        return Level;
//    }
//    virtual Stroka Format(const char *str);
//private:
//    int Level;
//    Stroka Ident;
//};
//// Class for extracting command line keys and purging the rest command arguments
//// stores help info for all keys, capable to print it
//// Keys are referenced by there names
//// checks if the key is setted in the command line, 
//// if argument of the type /p23 - 23 will be stored as value of key
//class CommandLineExe{
//public:
//    Ref<FormatStrClass> Format;
//    CommandLineExe(const char *mainhelp, char commandprefix = 0)
//        :Format(new aFormatStrSimple("   ", 0)), MainHelp(mainhelp), CommandPrefix(commandprefix){
//            if (CommandPrefix==0){
//#ifdef WIN32
//                CommandPrefix = '/';
//#else
//                CommandPrefix = '-';
//#endif
//            }
//    };
//    struct KeyData{
//        KeyData(char key,const char *name,const char *help):Key(key), Exist(0), Help(help), Name(name){
//        };
//        KeyData():Key(0), Exist(0){
//        };
//        void SetVal(const char *val, int exist){
//            Exist = exist;
//            Val = val;
//        }
//        char Key;
//        int Exist;
//        Stroka Help;
//        Stroka Name;
//        Stroka Val;
//    };
//// Get the KeyData info for the key with given name
//    KeyData& Get(const char *name);
//// Generate help for the program and all its keys
//    Stroka MakeHelp();
//// Store next key
//    void Add(char key, const char *name, const char *help){
//        Params[Stroka(name)] = KeyData(key, name, help);
//    }
//// Insert command line arguments, to analize presence of keys
//// Creates purged command line arguments  Argc    Argv  - without key args
//    void Analize(int argc, const char *argv[]);
//// Extract array of args, with or without keys ( parameter ExtractKeys  )
//    int Get(vector<Stroka> &args, int ExtractKeys = 1);
//// purged command line arguments  Argc    Argv  - without key args
//    size_t Argc;
//    const char *Argv[1024];
//    Stroka MainHelp;
//private:
//    char CommandPrefix;
//    vector<Stroka> GetCommandLine(int argc,const char *argv[]);
//    void SetCommandLine(const vector<Stroka> vec, size_t &argc,const char *argv[]);
//    void ExtractKeyData();
//    int KeyName(const char *arg, Stroka &Name);
//
//
//    map<Stroka, KeyData> Params;
//    vector<Stroka> Arguments;
//    vector<Stroka> ArgumentsExtra;
//    KeyData Zero;
//};


#endif