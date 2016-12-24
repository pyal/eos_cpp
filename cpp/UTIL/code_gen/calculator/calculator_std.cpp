#include <lib/precompiled/calc_simple.h>

#include "calculator_std.h"


//ForceLinkVoid<StdClcDouble> Dummy_StdClcDouble;
//ForceLinkVoid<StdClcUndefined> Dummy_StdClcUndefined;
//ForceLinkVoid<StdClcString> Dummy_StdClcString;


int main(int argc, const char *argv[]) {
    SetLeakTest();
    try {
        NRef::CommandLineExe Cmd(
            "Usage: calc_simple [Params_Key_Spec] [/][-]key  \n without parameters - standart test\n");
        Cmd.Add('h', "PrintHelp", " - print this help \n");
        //        Cmd.Add('p',"PrintDiscrepency"," - print discrepency estimations \n");
        //        Cmd.Add('a',"AllResults"," - not to delete good results \n");
        //        Cmd.MainHelp += GenerateDetailedHelp();

        Cmd.Analize(argc, argv);
        //        if (Cmd.Get("PrintDiscrepency").Exist) TestCase_Famessb_Static::PrintDiscrepencyEstimations=1;
        //        if (Cmd.Get("AllResults").Exist) TestCase_Famessb_Static::DeleteGoodResults=0;
        if(Cmd.Argc != 2 || Cmd.Get("PrintHelp").Exist) {
            cout << Cmd.MakeHelp();
            return 0;
        }
        fstream in(argv[1], ios::in);
        CalculatorProgam(new Lex_Calcul_Analyser(&in)).Read().Execute();
    }
    CATCHMAINEXCEPTION(" Main test failed ");
    LeakTest();
    return 0;
}