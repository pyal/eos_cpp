#include "lib/precompiled/urs_curve_lib.h"

#include "urs_curve.h"


static ClassDesc URS_Curve_ClassDesc(typeid(URS_Curve),"URS_Curve",1,"URS_Curve_category",create<URS_Curve>);
static ClassDesc URS_Curve_Output_ClassDesc(typeid(URS_Curve::Output),"URS_Curve::Output",1,"URS_Curve_Output_category",create<URS_Curve::Output>);
static ClassDesc URS_Curve_ClcVar_ClassDesc(typeid(URS_Curve::ClcVar),"URS_Curve::ClcVar",1,"URS_Curve_ClcVar_category",create<URS_Curve::ClcVar>);
static ClassDesc UrsCurve_StepClc_ClassDesc(typeid(UrsCurve_StepClc),"UrsCurve_StepClc",1,"URS_Curve_ClcVar_category",create<UrsCurve_StepClc>);
static ClassDesc UrsCurve_Caloric_ClassDesc(typeid(UrsCurve_Caloric),"UrsCurve_Caloric",1,"URS_Curve_ClcVar_category",create<UrsCurve_Caloric>);
static ClassDesc UrsCurve_FreeE_ClassDesc(typeid(UrsCurve_FreeE),"UrsCurve_FreeE",1,"URS_Curve_ClcVar_category",create<UrsCurve_FreeE>);
static ClassDesc UrsCurve_EOS_FindE_ClassDesc(typeid(UrsCurve_EOS_FindE),"UrsCurve_EOS_FindE",1,"URS_Curve_ClcVar_category",create<UrsCurve_EOS_FindE>);
static ClassDesc UrsCurve_EOS_Isoentrope_ClassDesc(typeid(UrsCurve_EOS_Isoentrope),"UrsCurve_EOS_Isoentrope",1,"URS_Curve_ClcVar_category",create<UrsCurve_EOS_Isoentrope>);
static ClassDesc UrsCurve_EOS_Hugoniot_ClassDesc(typeid(UrsCurve_EOS_Hugoniot),"UrsCurve_EOS_Hugoniot",1,"URS_Curve_ClcVar_category",create<UrsCurve_EOS_Hugoniot>);
static ClassDesc UrsCurve_FileReader_ClassDesc(typeid(UrsCurve_FileReader),"UrsCurve_FileReader",1,"URS_Curve_ClcVar_category",create<UrsCurve_FileReader>);

static ClassDesc UrsCurve_VarStorage_ClassDesc(typeid(UrsCurve_VarStorage),"UrsCurve_VarStorage",1,"URS_Curve_ClcVar_category",create<UrsCurve_VarStorage>);
static ClassDesc UrsCurve_Output2Store_ClassDesc(typeid(UrsCurve_Output2Store),"UrsCurve_Output2Store",1,"URS_Curve_Output_category",create<UrsCurve_Output2Store>);

//Tmp
static ClassDesc UrsCurve_HugCheck_ClassDesc(typeid(UrsCurve_HugCheck),"UrsCurve_HugCheck",1,"URS_Curve_ClcVar_category",create<UrsCurve_HugCheck>);


static ClassDesc UrsCurve_XY_Test_ClassDesc(typeid(UrsCurve_XY_Test),"UrsCurve_XY_Test",1,"URS_Curve_ClcVar_category",create<UrsCurve_XY_Test>);
static ClassDesc UrsCurve_SplConstr_ClassDesc(typeid(UrsCurve_SplConstr),"UrsCurve_SplConstr",1,"URS_Curve_ClcVar_category",create<UrsCurve_SplConstr>);
static ClassDesc UrsCurve_InOut_ClassDesc(typeid(UrsCurve_InOut),"UrsCurve_InOut",1,"URS_Curve_ClcVar_category",create<UrsCurve_InOut>);

static ClassDesc EOS_Savable_ClassDesc(typeid(EOS_Savable),"EOS_Savable",1,"URS_Curve_Var_category",create<EOS_Savable>);

static ClassDesc UrsCurve_OneVarFunction_ClassDesc(typeid(UrsCurve_OneVarFunction),"UrsCurve_OneVarFunction",1,"URS_Curve_ClcVar_category",create<UrsCurve_OneVarFunction>);

static ClassDesc UrsCurve_NamedOneVarFunction_ClassDesc(typeid(UrsCurve_NamedOneVarFunction),"UrsCurve_NamedOneVarFunction",1,"URS_Curve_ClcVar_category",create<UrsCurve_NamedOneVarFunction>);

static ClassDesc UrsCurve_ManyVarFunction_ClassDesc(typeid(UrsCurve_ManyVarFunction),"UrsCurve_ManyVarFunction",1,"URS_Curve_ClcVar_category",create<UrsCurve_ManyVarFunction>);



#undef NumClcNames
#define  NumClcNames 3
Stroka UrsCurve_Caloric::ClcNames[NumClcNames]={"Pressure","Temperature","Sound"}; 
MatterIO *UrsCurve_Caloric::MatStat = NULL;
double(*UrsCurve_Caloric::eos_clc[NumClcNames])(double,double) ={UrsCurve_Caloric::ClcPressure,UrsCurve_Caloric::ClcTemp,UrsCurve_Caloric::ClcSound};

#undef NumClcNames
#define  NumClcNames 6
Stroka UrsCurve_FreeE::ClcNames[NumClcNames]={"Pressure","Energy","Sound","FreeE","FreeG","Entropy"}; 
MatterIO *UrsCurve_FreeE::MatStat = NULL;
InterfaceFreeEIO *UrsCurve_FreeE::FreeStat = NULL;
double(*UrsCurve_FreeE::eos_clc[NumClcNames])(double,double) =
	{UrsCurve_FreeE::ClcPressure,UrsCurve_FreeE::ClcEnergy,UrsCurve_FreeE::ClcSound,UrsCurve_FreeE::ClcFreeE,UrsCurve_FreeE::ClcFreeG,UrsCurve_FreeE::ClcEntropy};




int URS_Curve::save_data_state( FilterOut&so){
	so<<"Variables { ";SavableClass::IncIdent();so<<SavableClass::EOLN().c_str();
	for (map<Stroka, Ref<SavableClass> >::iterator it = Vars.begin();it!=Vars.end();it++){
		so<<it->first.c_str();
		so<<"\t"<<it->second<<"\n";
	}
	SavableClass::DecIdent();so<<"} Output "<<out<<" NumIter "<<NumIter;
	return 1; 
};
int URS_Curve::read_data_state(FilterIn&si){ 
	char tmp[256];
	si>>tmp>>tmp>>tmp;
	while (Stricmp(tmp,"}")!=0){
		si>>Vars[Stroka(tmp)]>>tmp;
	}
	si>>tmp>>out>>tmp>>NumIter;
	return 1;
};

//
//void DrawCurve(int argc,const char *argv[]){
//    FilterTextIn in(argv[1]);
//    in.SetRefMask(SavableClass::UnlinkedObjects);
//    if (!in)
//        throw info_except("could not read input cfg file %s\n", argv[1]);
//    Ref<URS_Curve> curve;
//    in>>curve;
//    //fcout.SetRefMask(SavableClass::UnlinkedObjects);
//    //fcout<<curve;
//    curve->Do();
//}
//
//
//Stroka GenerateDetailedHelp(){
//    FilterTextOut tmp("tmp1",DataSource::Memory);
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
//    return Stroka(DataSource::GetStr("tmp1").c_str());
//};
//
//#include "lib/ref/commandline.h"
//#include "urs/urs_forcelnk.h"
//int main( int argc, const char *argv[] )
//{
//    SetLeakTest();
//    try {
//        CommandLineExe Cmd("Usage: urs_curve [Params_Key_Spec] [/][-]key  \n without parameters - standart test\n");
//        Cmd.Add('h',"PrintHelp"," - print this help \n");
//        Cmd.MainHelp += GenerateDetailedHelp();
//        Cmd.Analize(argc, argv);
//        //if (Cmd.Get("PrintDiscrepency").Exist) TestCase_Famessb_Static::PrintDiscrepencyEstimations=1;
//        //if (Cmd.Get("AllResults").Exist) TestCase_Famessb_Static::DeleteGoodResults=0;
//        if (Cmd.Argc==1 || Cmd.Get("PrintHelp").Exist){
//            cout<<Cmd.MakeHelp();
//            return 0;
//        } 
//        DrawCurve((int)Cmd.Argc,Cmd.Argv);
//    } catch(stdexception &ex){ 
//        cout<<" Exeption caught:\n"<<ex.what()<<"\n";
//        return 1;
//    }
//    LeakTest();
//    return 0;
//}
