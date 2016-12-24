#pragma once

#include "lib/std/Util.h"
#include "Stroka.h"
#include <vector>

namespace NRef {
    // Formating output - given maxlen - will limit the string length
    // If in the string ~- or ~+ - will decrease or increase ident levels (given ident str) of the lines
    class TFormatOutput {
    private:
        size_t Level, MaxLen;
        Stroka Ident;

        size_t FormLine(const char *str, size_t pos, Stroka &line);

        size_t skipspace(
            const char *str,
            size_t pos,
            size_t len,
            int &NextLine,
            Stroka &line);

        size_t skipword(
            const char *str,
            size_t pos,
            size_t len,
            int &NextLine,
            Stroka &line);

    public:
        TFormatOutput(const char *ident = 0, int level = 0, int maxlen = -1)
            : Level(level), MaxLen(maxlen) {
            if(ident != 0)
                Ident = ident;
        }

        Stroka Format(const char *str);
    };

    // Class TCommandParse - start functions of the type
    typedef void(MAINFUNC)(map<Stroka, Stroka>);

    // Generate help for the program and all its modes. Parse program input, check given parameters.
    class TCommandParse {
    private:
        // Data for the mode
        struct TKeyData {
        private:
            char Key;
            int Exist;
            Stroka Mode, Help;
            map<Stroka, Stroka> Par2Descr, Params2Defaults;
            Stroka ParamsSettings;
            Stroka ParamsNames;
            int GivenDefaultsMode;

        public:
            TKeyData() {}

            TKeyData(
                const char *mode,
                const char *help,
                const char *params,
                MAINFUNC *func,
                int givenDefaultsMode);

            Stroka MakeHelp();

            Stroka GetParamsSettings() {
                return ParamsSettings;
            }

            Stroka GetParamsNames() {
                return ParamsNames;
            }

            map<Stroka, Stroka> &GetParams2Defaults() {
                return Params2Defaults;
            }

            int GivenDefaults() {
                return GivenDefaultsMode;
            }

            MAINFUNC *Func;
        };

        Stroka MakeHelp();

        map<Stroka, TKeyData> Params;

    public:
        TCommandParse(const char *mainhelp) : MainHelp(mainhelp){};

        // Add new program mode
        void Add(
            void(func)(map<Stroka, Stroka>),
            const char *mode,
            const char *help,
            const char *params,
            int givenDefaultsMode = 0) {
            Params[Stroka(mode)] = TKeyData(mode, help, params, func, givenDefaultsMode);
        }

        // Parse input, run proper function
        int SimpleRun(int argc, const char *argv[]);

        // Put here general information about the program
        Stroka MainHelp;
    };
}