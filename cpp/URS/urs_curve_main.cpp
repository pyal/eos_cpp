#include <lib/precompiled/urs_curve.h>

#include "urs/test/urs_curve.h"
#include "lib/ref/commandline.h"
#include "urs/curve_forcelnk.h"

void DrawCurve(int argc,const char *argv[]){
    FilterTextIn in(argv[1]);
    in.SetRefMask(SavableClass::SimpleEdit);
    if (!in)
        throw info_except("could not read input cfg file %s\n", argv[1]);
    Ref<URS_Curve> curve;
    in>>curve;
	if (!curve)
		throw info_except("Could not read URS_Curve class from the <%s> file\n",argv[1]);
    //fcout.SetRefMask(SavableClass::UnlinkedObjects);
    //fcout<<curve;
    curve->Do();
}


#include "curve_forcelnk.h"
#include "urs_forcelnk.h"

Stroka GenerateDetailedHelp() {
    Stroka res = Stroka("Program build time ") + __TIMESTAMP__ + "\n";
    res += Stroka(" Registered curves:\n\n\n") + SavableClass::HelpForCategory("URS_Curve_category");
    res += Stroka(" Registered EOS FreeE:\n\n\n") + SavableClass::HelpForCategory("MatterFreeE_category");
    res += Stroka(" Registered EOS caloric:\n\n\n") + SavableClass::HelpForCategory("Matter_category");

//    StdClcUndefined udef;
//    StdClcDouble dou;
//    StdClcString ss;
//    res += Stroka("\n") + udef.class_name()  + " " + dou.class_name() + " " + ss.class_name() + "\n";
//    res += Stroka(" Registered VARS:\n\n\n") + SavableClass::HelpForCategory("StdClcTypes_category");
//    Stroka str = DataSource::GenerateUnicName("tmp");
//    FilterTextOut tmp(str.c_str(),DataSource::Memory);
////    tmp.SetRefMask(SavableClass::SingleFileStorage);
//    tmp.SetRefMask(SavableClass::UnlinkedObjects);
//    ClassDesc::WriteHelpOnCategory(tmp, "URS_Curve_category", ClassDesc::PrintHelp);
//    tmp<<"\n\nRegistered FreeE EOS classes \n";
//    ExEnv::set_out(&tmp); 
//    ClassDesc::list_all_classes("MatterFreeE_category");
//    tmp<<"\n\nRegistered Caloric EOS classes \n";
//    ClassDesc::list_all_classes("Matter_category");
//    tmp<<"\n=====================================================\nDetailed help for matters\n";
//    ClassDesc::WriteHelpOnCategory(tmp,"MatterFreeE_category",ClassDesc::PrintHelp);
//    tmp<<"\n\n";
//    ClassDesc::WriteHelpOnCategory(tmp,"Matter_category",ClassDesc::PrintHelp);
//    ExEnv::set_out(&cout);
//	return Stroka(DataSource::GetStr(str.c_str()).c_str());
    return res;
};
int main( int argc, const char *argv[] )
{
	SetLeakTest();
	Time_struct Time;
    try {
        NRef::CommandLineExe Cmd("Usage: urs_curve [Params_Key_Spec] [/][-]key  \n without parameters - standart test\n");
        Cmd.Add('h',"PrintHelp"," - print this help \n");
        Cmd.MainHelp += GenerateDetailedHelp();
        Cmd.Analize(argc, argv);
        //if (Cmd.Get("PrintDiscrepency").Exist) TestCase_Famessb_Static::PrintDiscrepencyEstimations=1;
        //if (Cmd.Get("AllResults").Exist) TestCase_Famessb_Static::DeleteGoodResults=0;
        if (Cmd.Argc==1 || Cmd.Get("PrintHelp").Exist)
            cout<<Cmd.MakeHelp();
        else 
			DrawCurve((int)Cmd.Argc,Cmd.Argv);
    } CATCHMAINEXCEPTION(" Urs_curve failed ");

    LeakTest();
	cout<<"Done in "<<Time<<"\n";
    return 0;
}
