
#include "test_center.h"
#include "lib\std\ex_out.h"

#include "UTIL\code_gen\analyse_base\lex_anal_test_forcelink.h"

static ClassDesc TestCase_TestLibs_ClassDesc(typeid(TestCase_TestLibs),"TestCase_TestLibs",1,"TestCase_TestTest_category",create<TestCase_TestLibs>);



int main( int argc, const char *argv[] )
{
    SetLeakTest();
	Time_struct Time;
    SavableClass::list_all_classes(NULL);
    try {
        NRef::CommandLineExe Cmd("Usage: test_center [Params_Key_Spec] [/][-]key  \n without parameters - standart test\n");
        Cmd.Add('h',"PrintHelp"," - print this help \n");
//        Cmd.Add('p',"PrintDiscrepency"," - print discrepency estimations \n");
//        Cmd.Add('a',"AllResults"," - not to delete good results \n");
//        Cmd.MainHelp += GenerateDetailedHelp();

        Cmd.Analize(argc, argv);
//        if (Cmd.Get("PrintDiscrepency").Exist) TestCase_Famessb_Static::PrintDiscrepencyEstimations=1;
//        if (Cmd.Get("AllResults").Exist) TestCase_Famessb_Static::DeleteGoodResults=0;
        if (Cmd.Argc!=1 || Cmd.Get("PrintHelp").Exist){
            cout<<Cmd.MakeHelp();
            return 0;
        } 

        TestSuite::RunTest("TestCase_TestLibs");
        //TestSuite::RunTest("TestCase_Famessb_Static");
		TestSuite::RunTest("TestCase_Manip");
        TestSuite::RunTest("TestCase_Lex_Analyser");

    }
    CATCHEXCEPTION(" Main test failed ");
    LeakTest();
	cout<<"Done in "<<Time<<"\n";
    return 0;
}
