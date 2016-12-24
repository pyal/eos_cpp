#include <lib/precompiled/Messb.h>
#include "famessb_test_INTERNAL.h"
//#include "cpp_base\test_case.h"
//#include "lex_anal_test.h"
static ClassDesc TestCase_Famessb_Internal_ClassDesc(
    typeid(TestCase_Famessb_Internal),
    "TestCase_Famessb_Internal",
    1,
    "TestCase_Famessb_Internal_category",
    create<TestCase_Famessb_Internal>);


CMatrCl *TestCase_Famessb_Internal::BaseMisMatr;
Ham_Qs_Gamma *TestCase_Famessb_Internal::MisHam;

double TestCase_Famessb_Internal::MakeParamsFit(
    double (*mis)(VecCl par),
    VecCl &BestPar,
    double *low,
    double *up,
    double *num,
    int Dims) {
    vector<VecCl> Params(Dims);
    double Norm = 1e10;
    VecCl bestpar(Dims);
    bestpar = bestpar * 0 + (-1);
    VecCl cur;
    cur = bestpar * 0 + 1;
    cur[Dims] = 0;
    VecCl curpar = cur;
    for(int k = 0; k < Dims; k++)
        Params[k] = VecCl::MakeStepVector(num[k], low[k], up[k], 0);
    //cout<<curpar<<cur<<"\n";
    while(1) {
        int done = 1;
        double tmpn;
        for(int k = Dims; k >= 1; k--)
            if(cur[k] < Params[k - 1].Dim()) {
                done = 0;
                cur[k]++;
                for(int k1 = k + 1; k1 <= Dims; k1++)
                    cur[k1] = 1;
                break;
            }
        if(done)
            break;
        for(int k = 1; k <= Dims; k++)
            curpar[k] = Params[k - 1][cur[k]];
        //cout<<curpar<<cur<<"\n";
        if((tmpn = mis(curpar)) < Norm) {
            Norm = tmpn;
            bestpar = curpar;
        }
    }
    BestPar = bestpar;
    return Norm;
};


void TestCase_Famessb_Internal::Test_Turn_Matr(TestCase *ptr) {
    TestCase_Famessb_Internal *cur = Restore(ptr);
    CMatrCl H2 = HamHTurned_Z_Qs(HamData(90, 90, 100, 10, 1, 1, 1), 90, 90).Make8Ham();
    CMatrCl H1 = Ham_Qs_Gamma(HamData(90, 0, 100, 10, 1, 1, 1), 90).Make8Ham();
    CMatrCl Turn = TurnSpinors::FormTurn4_2Ham(90, 90);
    Turn = Turn * H2 * TurnSpinors::InverseOper(Turn);
    if(MatrNorm(H1 - Turn) > 1e-6) {
        cout << H1 << "\n" << Turn << "\n" << H1 - Turn << "\n";
        throw(info_except(
            "Turned Ham_Qs_Gamma(HamData(90,0,100,10,1,1,1),90) is not as HamHTurned_Z_Qs(HamData(90,90,100,10,1,1,1),90,90). Test failed\n"));
    }
    //throw(info_except(" OK \n"));
}


void TestCase_Famessb_Internal::Test_ZTurnedQsG_QsG(TestCase *ptr) {
    Ref<HamData> Hzturned = new Ham_Z_Turned_Qs_Gamma(
        Ham_Qs_Gamma(HamData(90, 45, 100, 4, 1, 0, 0), 90), 90, 90);
    Ref<Ham_Qs_Gamma> Hgamma = new Ham_Qs_Gamma(HamData(45, 0, 100, 4, 1, 0, 0), 60);

    CMatrCl Mt = Hzturned->Make4Ham(), Mg;
    BaseMisMatr = &Mt;
    MisHam = Hgamma;
    VecCl BestPar;
    double low[] = {0, 0}, up[] = {350, 350}, num[] = {36, 36};
    double fit = MakeParamsFit(FitMisf, BestPar, low, up, num, 2);
    cout
        << " Hzturned : Ham_Z_Turned_Qs_Gamma(Ham_Qs_Gamma(HamData(90,45,100,4,1,0,0),90),90,90);\n";
    cout << " Gamma : Ham_Qs_Gamma(HamData(45,0,100,4,1,0,0),60);\n";
    cout << " best gam " << BestPar[1] << " best phi " << BestPar[2] << " norm " << fit
         << "\n";
    if(fit > 1e-6) {
        Hgamma->Teta = BestPar[1];
        Hgamma->Phi = BestPar[2];
        Mg = Hgamma->Make4Ham();
        cout << "Turned\n"
             << Mt << "\n"
             << "Gamma\n"
             << Mg << "\n"
             << " Delta \n"
             << Mt - Mg << "\n";
        throw(info_except(
            "For  Ham_Z_Turned_Qs_Gamma(Ham_Qs_Gamma(HamData(90,30,100,4,1,0,0),0),90,90) could not find Ham_Qs_Gamma. Test failed\n"));
    }
    //throw(info_except(" OK \n"));
}


void TestCase_Famessb_Internal::Test_Qs_Gamma_Eiler(TestCase *ptr) {
    Ref<Ham_Qs_Eiler> Heiler = new Ham_Qs_Eiler(HamData(25, 30, 100, 4, 1, 0, 0));
    Ref<Ham_Qs_Gamma> Hgamma =
        new Ham_Qs_Gamma(HamData(25, -90 - 30, 100, 4, 1, 0, 0), 180 + 30);

    CMatrCl Me = Heiler->Make4Ham(), Mg;
    BaseMisMatr = &Me;
    MisHam = Hgamma;
    VecCl BestPar;
    double low[] = {0, 0}, up[] = {350, 350}, num[] = {36, 36};
    double fit = MakeParamsFit(FitMisf, BestPar, low, up, num, 2);
    cout << " Eiler :Ham_Qs_Eiler(HamData(25,30,100,4,1,0,0))\n";
    cout << " Gamma :Ham_Qs_Gamma(HamData(25,-90-30,100,4,1,0,0),180+30)\n";
    cout << " best gam " << BestPar[1] << " best phi " << BestPar[2] << " norm " << fit
         << "\n";
    if(fit > 1e-6) {
        Hgamma->Teta = BestPar[1];
        Hgamma->Phi = BestPar[2];
        Mg = Hgamma->Make4Ham();
        cout << "Eiler\n"
             << Me << "\n"
             << "Gamma\n"
             << Mg << "\n"
             << " Delta \n"
             << Me - Mg << "\n";
        throw(info_except(
            "Ham_Qs_Eiler(HamData(90,30,100,4,1,0,0)) is not as Ham_Qs_Gamma(HamData(90,30-90,100,4,1,0,0),30). Test failed\n"));
    }
    //throw(info_except(" OK \n"));
}

void TestCase_Famessb_Internal::Test_Turn_Angles(TestCase *ptr) {
    V3D_Angles z_ang(1, 0, 0);
    V3D_Angles x_ang(1, 90, 0);
    V3D_Angles y_ang(1, 90, 90);
    V3D_Angles arb_ang(1, 30, 60);
    V3D_XYZ tmp;
    V3D_Angles tmp1;
    if(VecCl::ModMax(
           ((tmp = V3D_XYZ(Turn3D::TurnAngles(x_ang, Turn3D::ITurnMatr(x_ang)))) -
            V3D_XYZ(z_ang))
               .vec) > 1e-6) {
        cout << " X_ang inverse rotated is " << tmp;
    }
    if(VecCl::ModMax(
           ((tmp = V3D_XYZ(Turn3D::TurnAngles(y_ang, Turn3D::ITurnMatr(y_ang)))) -
            V3D_XYZ(z_ang))
               .vec) > 1e-6) {
        cout << " Y_ang inverse rotated is " << tmp;
    }
    if(VecCl::ModMax(
           ((tmp = V3D_XYZ(Turn3D::TurnAngles(z_ang, Turn3D::TurnMatr(x_ang)))) -
            V3D_XYZ(x_ang))
               .vec) > 1e-6) {
        cout << " Z_ang x_ang rotated is " << tmp;
    }
    if(VecCl::ModMax(
           ((tmp = V3D_XYZ(Turn3D::TurnAngles(z_ang, Turn3D::TurnMatr(y_ang)))) -
            V3D_XYZ(y_ang))
               .vec) > 1e-6) {
        cout << " Z_ang y_ang rotated is " << tmp;
    }
    if(VecCl::ModMax(
           (V3D_XYZ(tmp1 = Turn3D::TurnAngles(arb_ang, Turn3D::ITurnMatr(arb_ang))) -
            V3D_XYZ(z_ang))
               .vec) > 1e-6) {
        cout << " arb_ang :" << arb_ang << " inverse rotated is " << tmp1;
    }
    if(VecCl::ModMax(
           ((V3D_XYZ(tmp1 = Turn3D::TurnAngles(z_ang, Turn3D::TurnMatr(arb_ang)))) -
            V3D_XYZ(arb_ang))
               .vec) > 1e-6) {
        cout << " z_ang arb_ang :" << arb_ang << " rotated is " << tmp1;
    }
    if(VecCl::ModMax(((V3D_XYZ(
                          tmp1 = Turn3D::TurnAngles(
                              Turn3D::TurnAngles(y_ang, Turn3D::TurnMatr(arb_ang)),
                              Turn3D::ITurnMatr(arb_ang)))) -
                      V3D_XYZ(y_ang))
                         .vec) > 1e-6) {
        cout << " y_ang arb_ang :" << arb_ang << " rotated + irotated is " << tmp1;
    }
    if(MatrNorm(
           Turn3D::ITurnMatr(arb_ang) * Turn3D::TurnMatr(arb_ang) - (MatrCl(3) * 0 + 1)) >
       1e-6) {
        cout << " Iturn*Turn for arb_ang :" << arb_ang << " is "
             << Turn3D::ITurnMatr(arb_ang) * Turn3D::TurnMatr(arb_ang);
        cout << " ITurn: " << Turn3D::ITurnMatr(arb_ang);
        cout << " Turn: " << Turn3D::TurnMatr(arb_ang);
    }
    //throw(info_except(" OK \n"));
};

#include "messb_in.h"
#include "mat\mtrnd.h"

void TestCase_Famessb_Internal::Test_Rnd_Angles_Generators(TestCase *ptr) {
    VecCl teta, phi, time;
    RndAngleGenerator ang(
        new RndBoltsman_GausWalk(
            (RndGaus *)(new RndGaus(10, 5))->Init(0, 30, 100, 1e-5),
            (RndBoltsmanSinPower *)(new RndBoltsmanSinPower(1, -1))
                ->Init(-180, 180, 100, 1e-5)),
        new RndTimeGenerator((new RndGaus(1, 0.5))->Init(0.1, 5, 100, 1e-5), 0, 100));
    ang.Rnd(teta, time);
    for(int k = 1; k <= time[0]; k++)
        cout << time[k] << " " << teta[k] << "\n";
    Rnd2AngleGenerator ang2(
        new RndAngleGenerator(
            (new RndGaus(10, 0.5))->Init(8, 12, 100, 1e-5),
            new RndTimeGenerator((new RndGaus(1, 0.5))->Init(0.1, 5, 100, 1e-5), 0, 10)),
        new RndAngleGenerator(
            (new RndGaus(30, 0.5))->Init(28, 32, 100, 1e-5),
            new RndTimeGenerator(
                (new RndGaus(0.3, 0.5))->Init(0.01, 3, 100, 1e-5), 0, 10)));
    ang2.Rnd(phi, teta, time);
    for(int k = 1; k <= time[0]; k++)
        cout << time[k] << " " << teta[k] << " " << phi[k] << "\n";
}
