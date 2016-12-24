
#include <lib/precompiled/test_urs.h>


#include "test_urs.h"
#include "lib\test_fact\test_case.h"
#include "lib/ref/stroka.h"
#include "lib/std/ex_out.h"

static ClassDesc TestCase_Urs_Static_ClassDesc(
    typeid(TestCase_Urs_Static),
    "TestCase_Urs_Static",
    1,
    "TestCase_Urs_Static_category",
    create<TestCase_Urs_Static>);


const char *TestCase_Urs_Static::TestExeOutput = "test_urs_output";
int TestCase_Urs_Static::DeleteGoodResults = 1;
double StndErr = 1e-6;


#include "lib/std/stdexception.h"
void WriteResource(int ResourceNum, const char *FileName) {
    long size;
    fstream out(FileName, ios::out);
    char *dat = (char *)ResourceReader::Load(ResourceNum, "TXT", size);
    int k1 = 0;
    while(k1 < size) {
        if(dat[k1] == 13) {
            k1++;
            continue;
        }
        out.write(&dat[k1], 1);
        k1++;
    }
}
void TestCase_Urs_Static::Text_Func_Out(
    ResourcesStruct *resources,
    int Length,
    const char *name) {
    for(int k = 0; k < Length; k++) {
        Stroka nam_cfg = Stroka(resources[k].Name) + ".cfg";
        Stroka nam_test = Stroka(resources[k].Name) + ".test";
        Stroka nam_res = Stroka(resources[k].Name) + ".dat";

        unlink(nam_res.c_str());

        WriteResource(resources[k].data, nam_test.c_str());
        WriteResource(resources[k].config, nam_cfg.c_str());

        cout << nam_cfg.c_str() << "         ";
        cout.flush();
        if(system(
               Stroka(Stroka("urs_curve ") + nam_cfg + " >> " + TestExeOutput).c_str()) ==
           0) {
            cout << "OK\n";
            if(DeleteGoodResults) {
                unlink(nam_cfg.c_str());
                unlink(nam_test.c_str());
                unlink(nam_res.c_str());
            }
        } else {
            cout << " !!!!!BAD!!!!!Test not passed\n";
        }
        cout.flush();
    }
};

#include "lib/ref/commandline.h"
int main(int argc, const char *argv[]) {
    SetLeakTest();
    Time_struct Time;
    try {
        NRef::CommandLineExe Cmd(
            "Usage: test_urs [Params_Key_Spec] [/][-]key  \n without parameters - standart test\n");
        Cmd.Add('h', "PrintHelp", " - print this help \n");
        Cmd.Add('a', "AllResults", " - not to delete good results \n");
        //        Cmd.MainHelp += GenerateDetailedHelp();

        Cmd.Analize(argc, argv);
        if(Cmd.Get("AllResults").Exist)
            TestCase_Urs_Static::DeleteGoodResults = 0;
        if(Cmd.Argc != 1 || Cmd.Get("PrintHelp").Exist) {
            cout << Cmd.MakeHelp();
            return 0;
        }
        TestSuite::RunTest("TestCase_Urs_Static");
    } catch(exception &ex) {
        cout << " Exeption caught:\n" << ex.what() << "\n";
        return 1;
    }
    LeakTest();
    cout << "Done in " << Time << "\n";
    return 0;
}
