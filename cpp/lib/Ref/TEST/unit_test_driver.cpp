#include "lib/Ref/CommandLine.h"


#include "lib/std/stdexception.h"
#include "lib/std/ex_out.h"
#include "unit_test.h"

int main( int argc, const char *argv[] )
{
    SetLeakTest();
    Time_struct Time;
    try {
        NRef::CommandLineExe Cmd("Usage: unit_tests [Params_Key_Spec] [/][-]key  \n without parameters - standart test\n");
        Cmd.Add('h',"PrintHelp"," - print this help \n");
        Cmd.Add('a',"AllTests"," run all tests \n");

        Cmd.Analize(argc, argv);
        if (Cmd.Get("PrintHelp").Exist){
            cout<<Cmd.MakeHelp();
            return 0;
        }
        if (Cmd.Get("AllTests").Exist)  UnitTest::DoAll(0);
        else UnitTest::DoAll(argv[1]);
    }
    catch(stdexception &ex){
        cout<<" Exeption caught:\n"<<ex.what()<<"\n";
        return 1;
    }
    cout<<"Test time: "<<Time<<"\n";
    LeakTest();
    return 0;
}
