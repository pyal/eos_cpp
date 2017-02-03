
#include "lib/precompiled/poly_test.h"
#include "lib/std/util.h"
#include "lib/ref/commandline.h"
#include "lib/std/ex_out.h"
#include "lib/ref/file_manip.h"
#include "gasd/Polygon/lib/PolyMarchBase.h"
#include "gasd/Polygon/lib/polygon_forcelnk.h"


Stroka GenerateDetailedHelp() {
    Stroka ret =
           Stroka("\n\n\nNPolygon::Constructor_category: ~+\n") +
           SavableClass::HelpForCategory("NPolygon::Constructor_category");
    ret += "~-\n\n\nNPolygon::March_category :~+\n\n" +
           SavableClass::HelpForCategory("NPolygon::March_category");
    return NRef::TFormatOutput("    ", 0, 110).Format(~ret);
};

void SampleConfig(map<Stroka, Stroka> par) {
    Stroka conf = par["Sample"];
    log_info("Generating sample config: " + conf);
    fstream out(~conf, ios::out);
    Ref<NPolygon::TSimpleContructor> constr1 = new NPolygon::TSimpleContructor;
    Ref<NPolygon::TPolyMarchBody> march = new NPolygon::TPolyMarchBody;
    out << "RegionConstructor\n" << SavableClass::object2string(constr1) << "\n\n";
    out << "RegionMarch\n" << SavableClass::object2string(march) << "\n";
    out.close();
    cout << GenerateDetailedHelp();
}


void TestFunc(map<Stroka, Stroka> par) {
    int level = atoi(~par["LogLevel"]);
    verify(level >= 0 && level < NLogger::max_level, "Max level is " + Itoa(NLogger::max_level) + " current level " + Itoa(level));
    NLogger::TLogger::GetLogger().LogLevel = NLogger::ELevel(level);
    TestSuite::RunTest("TestCase_Gasd_Static");

}


void March(map<Stroka, Stroka> par) {
    vector<Ref<SavableClass>> objVector = File::ReadConf(~par["Config"], 2, 1);
    Ref<NPolygon::TSimpleContructor> constr =
        SavableClass::TestType<NPolygon::TSimpleContructor>(
            objVector[0], "Have to define constructor");
    Ref<NPolygon::TPolyMarchBody> march =
        SavableClass::TestType<NPolygon::TPolyMarchBody>(
            objVector[1], "Have to define march");
    Ref<NPolygon::TPolyRegion> reg = constr->MakeRegion();
    march->DoIt(reg);
}

void AddRegion(list<Ref<NPolygon::TSimpleContructor::TRegData>> &childs,
               MatterIO *matter, int numPnt, vector<double> &pres_dens_ener_vel_pos){
using TRegData = NPolygon::TSimpleContructor::TRegData;
    double dens = pres_dens_ener_vel_pos[1];
    if (dens < MathZer) dens = matter->DencityCold();
    double pres = matter->Pressure(dens, pres_dens_ener_vel_pos[2]);
    childs.push_back(
            new TRegData({"EOS"}, {matter}, numPnt, {
                    TRegData::TGridVar("Pres", pres, pres),
                    TRegData::TGridVar("Denc", dens, dens),
                    TRegData::TGridVar("Ener", pres_dens_ener_vel_pos[2], pres_dens_ener_vel_pos[2]),
                    TRegData::TGridVar("Vel",  pres_dens_ener_vel_pos[3], pres_dens_ener_vel_pos[3]),
                    TRegData::TGridVar("Pos",  pres_dens_ener_vel_pos[4], pres_dens_ener_vel_pos[5]),
                    TRegData::TGridVar("Sound", 4, 4)
            }));

}
void BuildConfig(map<Stroka, Stroka> par) {
    Stroka topology = par["Topology"];
    Stroka config = par["Config"];
    auto datFileArr = Str::SplitLine(config, 1, '.');
    datFileArr[datFileArr.size() - 1] = "dat";
    Stroka datFile = Str::JoinLine(datFileArr, '.');
    fstream in(~topology, ios::in);
    double begTime, timeStp, timeWrite, presDeriveCoef, endTime, lftPnt, rgtPnt;
    int numPnt, numIntPar, lftBndFree, rgtBndFree;
    double timeStability, viscouse, viscouse2;
    int numMatters;
    vector<int> matterBnds;
    vector<Ref<MatterIO> > matters; //DataVector< Ref<MatterIO>, CopyStructRef<Ref<MatterIO> > > Matter;
    in >> Verify("BegTime") >> begTime >> Verify("TimeStp") >> timeStp >> Verify("TimeWrite") >> timeWrite >>
        Verify("PresDerivCoef") >> presDeriveCoef >> Verify("EndTime") >> endTime;
    in >> Verify("NumPnt") >> numPnt >> Verify("LftPnt") >> lftPnt >> Verify("RgtPnt") >> rgtPnt >> Verify("NumIntPar") >> numIntPar;
    in >> Verify("LftBnd_Free") >> lftBndFree >> Verify("RgtBnd_Free") >> rgtBndFree >> Verify("TimeStability") >> timeStability;
    in >> Verify("Viscouse") >> viscouse >> Verify("SecondVisc") >> viscouse2;
    in >> Verify("NumMatters") >> numMatters >> Verify("Matter_Boundaries");
    matterBnds.resize(numMatters + 1);
    matterBnds[0] = 0;
    matterBnds[numMatters] = numPnt;
    for(int i = 1; i < matterBnds.size() - 1; i++) {
        in >> matterBnds[i];
    }
    matters.resize(numMatters);
    for(int i = 0; i < matters.size(); i++) {
        if ( !(matters[i] << SavableClass::Read(in)) )
            throw info_except("Could not read. Exit.\n");
        log_info("Read matter " + SavableClass::object2string(matters[i]));
    }
    auto marcher = new NPolygon::TMarchWilkins;
    marcher->C0 = 2;//viscouse;
    marcher->CL = 1;//viscouse2;
    marcher->MarchCourant = timeStability;

    Ref<NPolygon::TSimpleContructor> constr = new NPolygon::TSimpleContructor();
    constr->RegionBoundarySize = 2;
    constr->Childs.clear();
    char tmp[256];
    in >> tmp;
    in >> Verify("Parameters");
    double curPos = 0;
    for(int i = 0; i < matters.size(); i++) {
         vector<double> pres_dens_ener_vel_pos(6);
        in >> tmp >> pres_dens_ener_vel_pos[0] >> tmp >> pres_dens_ener_vel_pos[1] >> tmp >>
                      pres_dens_ener_vel_pos[2] >> tmp >> pres_dens_ener_vel_pos[3] >> tmp >> pres_dens_ener_vel_pos[5];
        pres_dens_ener_vel_pos[4] = curPos;
        pres_dens_ener_vel_pos[5] += curPos;
        AddRegion(constr->Childs, matters[i], matterBnds[i + 1] - matterBnds[i], pres_dens_ener_vel_pos);
        curPos = pres_dens_ener_vel_pos[5];
    }
    Ref<NPolygon::TPolyMarchBody> march = new NPolygon::TPolyMarchBody(marcher,
                       begTime, endTime, timeStp, timeWrite,
                       datFile, "Pos:Pres:Denc:Ener:Vel:Sound");

    march->OutputBounds = NPolygon::TRegionBounds(0, 0);
    log_info("Generating config: " + config);
    fstream out(~config, ios::out);
    out << "RegionConstructor\n" << SavableClass::object2string(constr) << "\n\n";
    out << "RegionMarch\n" << SavableClass::object2string(march) << "\n";
    out.close();
}
Stroka BuildConfigSample() {
    return Stroka() +
"~BegTime 0 TimeStp 1e-10 TimeWrite 1.e-07 PresDerivCoef 0.7 EndTime 1.9e-05~" +
"NumPnt   300  LftPnt   0  RgtPnt   10 NumIntPar 1~" +
"LftBnd_Free 1 RgtBnd_Free 1 TimeStability 0.3 ~" +
"Viscouse 2  SecondVisc  1  ~" +
"NumMatters 3 Matter_Boundaries 100    200 ~" +
"MatterABu  { Material_File material.cfg Substance Steel-Tern } ~" +
"MatterSpl {  h2_dis_met_sarov.spl  }  ~" +
"MatterABu {  Material_File material.cfg Substance Z-sapphir } ~" +
"bad ~" +
"Parameters ~" +
" P[1]   00  D[1] 0         E[1] 0    V[1]  5.15  Pos[2] 1.5 ~" +
" P[1]   00  D[1] 0.001     E[1] 0.3  V[1]  0  Pos[2] 3 ~" +
" P[1]   00  D[1] 0         E[1] 0    V[1]  0  Pos[2] 1 ";

}
int main(int argc, const char *argv[]) {
    SetLeakTest();
    Time_struct Time;
    try {
        NRef::TCommandParse Cmd(
            "Usage: poly_test  - without parameters - help\n");
        Cmd.MainHelp += GenerateDetailedHelp();
        Cmd.Add(SampleConfig, "sample", "build sample config",
                "Sample poly_test.march.cfg Sample config file name", 1);
        Cmd.Add(TestFunc, "test", "test it", "", 1);
        Cmd.Add(March, "march", "clc region",
            "Config poly_test.march.cfg par.cfg file has the format: \"RegionConstructor NPolygon::TSimpleContructor RegionMarch  NPolygon::TPolyMarchBody\" ",
            1);
        Cmd.Add(BuildConfig, "config", "build config for given topoplogy",
                "Topology poly_test.march.top input topology: Sample " + BuildConfigSample() +
                        "\nConfig poly_test.march.cfg Result config file name", 1);
        Cmd.SimpleRun(argc, argv);
    }
    CATCHMAINEXCEPTION(" poly_test failed ");

    LeakTest();
    cout << "Done in " << Time << "\n";
    return 0;
}
