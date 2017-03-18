#include <lib/precompiled/urs_curve.h>

#include "urs/test/urs_curve.h"
#include "lib/ref/commandline.h"
#include "urs/curve_forcelnk.h"

void DrawCurve(int argc, const char *argv[]) {
    FilterTextIn in(argv[1]);
    in.SetRefMask(SavableClass::SimpleEdit);
    if(!in)
        throw info_except("could not read input cfg file %s\n", argv[1]);
    Ref<URS_Curve> curve;
    in >> curve;
    if(!curve)
        throw info_except("Could not read URS_Curve class from the <%s> file\n", argv[1]);
    curve->Do();
}


#include "curve_forcelnk.h"
#include "urs_forcelnk.h"

Stroka GenerateDetailedHelp() {
    Stroka res = Stroka(" Registered curves:\n\n\n") +
           SavableClass::HelpForCategory("URS_Curve_category");
    res += Stroka(" Registered EOS FreeE:\n\n\n") +
           SavableClass::HelpForCategory("MatterFreeE_category");
    res += Stroka(" Registered EOS caloric:\n\n\n") +
           SavableClass::HelpForCategory("Matter_category");

    return res;
};
void SetStack() {
    const rlim_t kStackSize = 32 * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result != 0) {
        log_always(Stroka("getrlimit returned result = ") + result);
        return;
    }
    if (rl.rlim_cur > kStackSize) {
        log_always(Stroka("current stack size ") + (int)rl.rlim_cur);
        return;
    }
    rl.rlim_cur = kStackSize;
    result = setrlimit(RLIMIT_STACK, &rl);
    if (result != 0) log_always(Stroka("setrlimit returned result = ") + result);
    log_always(Stroka("Settled stack size ") + (int)rl.rlim_cur);


}
int main(int argc, const char *argv[]) {
    SetLeakTest();
    Time_struct Time;
    NRef::CommandLineExe Cmd(
        "Usage: urs_curve [-]key  \n without parameters - help\n");
    Cmd.Add('h', "PrintHelp", " - print this help \n");
    Cmd.Add('d', "DebugLevel", " - sets debug level (-d3) \n");
    Cmd.MainHelp += GenerateDetailedHelp();
    Cmd.Analize(argc, argv);

    if(Cmd.Argc == 1 || Cmd.Get("PrintHelp").Exist) {
        cout << Cmd.MakeHelp();
    } else {
        if(Cmd.Get("DebugLevel").Exist) {
            int level = min(NLogger::max_level - 1, atoi(Cmd.Get("DebugLevel").Val.c_str()));
            log_always(Stroka("Setting Debug_level ") + NLogger::LevelNames[level] + ":" + level);
            NLogger::TLogger::GetLogger().LogLevel = NLogger::ELevel(level);
        }
        DrawCurve((int) Cmd.Argc, Cmd.Argv);
    }

    LeakTest();
    cout << "Done in " << Time << "\n";
    return 0;
}
