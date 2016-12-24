#include <lib/precompiled/Messb.h>

#include "gasd_min.h"


//=====================================================================
//=========================    FAZerExp ===============================
//=====================================================================
static ClassDesc FAZerColdExp6Curve_ClassDesc(
    typeid(FAZerColdExp6Curve),
    "exp6cold",
    1,
    "FAZer",
    create<FAZerColdExp6Curve>);
static ClassDesc FAZerCurveMain_ClassDesc(
    typeid(FAZerCurveMain),
    "FAZerCurveMain",
    1,
    "FAZer",
    create<FAZerCurveMain>);
static ClassDesc FAZerCurveMatterSum_0_H2Minim_ClassDesc(
    typeid(FAZerCurveMatterSum_0_H2Minim),
    "FAZerCurveMatterSum_0_H2Minim",
    1,
    "FAZerCurveBase_category",
    create<FAZerCurveMatterSum_0_H2Minim>);
static ClassDesc FAZerCurveMatterSum_FreeERoss_ClassDesc(
    typeid(FAZerCurveMatterSum_FreeERoss),
    "FAZerCurveMatterSum_FreeERoss",
    1,
    "FAZerCurveBase_category",
    create<FAZerCurveMatterSum_FreeERoss>);

FAZerColdExp6Curve::FAZerColdExp6Curve() {

    FreeE = new MatterFreeE();
    FreeE->SetNewMatter(new FreeESumMatter());
    name = strdup("exp6cold");
    helppar[0] = strdup(" cold curve approximation\n");
    int i = SetStdPar_GrInt(namepar, valpar, helppar, 1);
    namepar[i + 1] = strdup("  Temp     ");
    valpar[i + 1] = 0;
    helppar[i + 1] = strdup("");
    namepar[i + 2] = strdup("  Alpha    ");
    valpar[i + 2] = 0;
    helppar[i + 2] = strdup("");
    namepar[i + 3] = strdup(" Epsilon   ");
    valpar[i + 3] = 1;
    helppar[i + 3] = strdup("");
    namepar[i + 4] = strdup(" Radius0   ");
    valpar[i + 4] = 1;
    helppar[i + 4] = strdup("");
    namepar[i + 5] = strdup(" RadiusExp ");
    valpar[i + 5] = 1;
    helppar[i + 5] = strdup("auto generated");
    namepar[i + 6] = strdup(" MulExp    ");
    valpar[i + 6] = 1;
    helppar[i + 6] = strdup("auto generated");
    namepar[i + 7] = strdup(" ArgExp    ");
    valpar[i + 7] = 1;
    helppar[i + 7] = strdup("auto generated");
    namepar[i + 8] = strdup(" CutNiuDiam");
    valpar[i + 8] = 1;
    helppar[i + 8] = strdup("");
    namepar[i + 9] = strdup(" EOS_TYPE  ");
    valpar[i + 9] = 0;
    helppar[i + 9] =
        strdup("EOS types: MET=0,ROS1=1,ROS2=2,ROSID1=3,ROSID2=4,METID2=5,LIQWEAK=6");
    namepar[i + 10] = strdup(" GetPres   ");
    valpar[i + 10] = 1;
    helppar[i + 10] =
        strdup("Get pressure (1) or free energy (F) (2) or F+PV (3) or Pow6 poten (0)");
    NumPar = i + 10;
};
void FAZerColdExp6Curve::ClcBase(double *par, double *Xval, double *Res) {
    FreeESumMatter *Sum =
        dynamic_cast<FreeESumMatter *>((InterfaceFreeEIO *)FreeE->FreeEPtr);
    FreeERossH2_Minim *Mat =
        dynamic_cast<FreeERossH2_Minim *>((InterfaceFreeEIO *)Sum->FreeVect[0]);
    double Temp;
    Temp = par[3];
    Mat->par->Alpha = par[4];
    Mat->par->Epsilon = par[5];
    Mat->par->Radius0 = fabs(par[6]);

    Exp6Param::FindRexp(*(Mat->par));
    cout << " Rexp " << Mat->par->RadiusExp << " MulExp " << Mat->par->MulExp
         << " ArgExp " << Mat->par->ArgExp << "\n";
    //par[7] = Mat->par->RadiusExp;
    //par[8] = Mat->par->MulExp;
    //par[9] = Mat->par->ArgExp;

    Mat->par->CutDiameter_Niu = par[10];
    Mat->eos = FreeERossH2_Minim::EOSTYPE(int(par[11]));
    int GetPres = (int)par[12];

    for(int k = 1; k <= Xval[0]; k++) {
        if(GetPres == 1)
            Res[k] = -Sum->Pressure(Xval[k], Temp);
        else if(GetPres == 2)
            Res[k] = -Sum->FreeE(Xval[k], Temp);
        else if(GetPres == 3)
            Res[k] = -Sum->FreeE(Xval[k], Temp) - Sum->Pressure(Xval[k], Temp) / Xval[k];
        else
            Res[k] = -Exp6Param::Pow6Clc(Xval[k], *Mat->par);
    }
};


FAZerCurveMatterSum_0_H2Minim::FAZerCurveMatterSum_0_H2Minim() : FAZerCurveBase() {
    FreeE = new MatterFreeE();
    FreeE->SetNewMatter(new FreeESumMatter());
    name = strdup("FAZerCurveMatterSum_0_H2Minim");
    helppar[0] =
        strdup(" MatterSum with FreeERossH2_Minim - minim matter - first matter \n");
    int i = SetStdPar_GrInt(namepar, valpar, helppar, 1);
    namepar[i + 1] = strdup("  Alpha    ");
    valpar[i + 1] = 0;
    helppar[i + 1] = strdup("");
    namepar[i + 2] = strdup(" Epsilon   ");
    valpar[i + 2] = 1;
    helppar[i + 2] = strdup("");
    namepar[i + 3] = strdup(" Radius0   ");
    valpar[i + 3] = 1;
    helppar[i + 3] = strdup("");
    namepar[i + 4] = strdup(" CutNiuDiam");
    valpar[i + 4] = 1;
    helppar[i + 4] = strdup("");
    namepar[i + 5] = strdup(" EOS_TYPE  ");
    valpar[i + 5] = 0;
    helppar[i + 5] =
        strdup("EOS types: MET=0,ROS1=1,ROS2=2,ROSID1=3,ROSID2=4,METID2=5,LIQWEAK=6");
    namepar[i + 6] = strdup(" RadiusExp ");
    valpar[i + 6] = 1;
    helppar[i + 6] = strdup(
        "if negative= use fixed (taken by module),if zero - autogenerate , positive - autogenerate*coef");
    namepar[i + 7] = strdup(" MulExp    ");
    valpar[i + 7] = 1;
    helppar[i + 7] = strdup("auto generated");
    namepar[i + 8] = strdup(" ArgExp    ");
    valpar[i + 8] = 1;
    helppar[i + 8] = strdup("auto generated");
    NumPar = i + 8;
};
Ref<SavableClass> FAZerCurveMatterSum_0_H2Minim::GenerateVar(double *par) {
    FreeESumMatter *Sum =
        dynamic_cast<FreeESumMatter *>((InterfaceFreeEIO *)FreeE->FreeEPtr);
    FreeERossH2_Minim *Mat =
        dynamic_cast<FreeERossH2_Minim *>((InterfaceFreeEIO *)Sum->FreeVect[0]);
    Mat->par->Alpha = par[3];
    Mat->par->Epsilon = par[4];
    Mat->par->Radius0 = par[5];
    double RadiusExp = par[8];
    if(RadiusExp >= 0) {
        Exp6Param::FindRexp(*(Mat->par));
        if(RadiusExp > MathZer)
            Mat->par->RadiusExp *= RadiusExp;
        cout << " Rexp " << Mat->par->RadiusExp << " MulExp " << Mat->par->MulExp
             << " ArgExp " << Mat->par->ArgExp << "\n";
    } else {
        Mat->par->RadiusExp = fabs(RadiusExp);
        Exp6Param::Clc_ArgExpMulExp(*(Mat->par));
    }

    //   par[8] = Mat->par->RadiusExp;
    par[9] = Mat->par->MulExp;
    par[10] = Mat->par->ArgExp;

    Mat->par->CutDiameter_Niu = par[6];
    Mat->eos = FreeERossH2_Minim::EOSTYPE(int(par[7]));
    return new EOS_Savable(FreeE);
};

void FAZerCurveMatterSum_0_H2Minim::OutputInternPar(ostream &output) {
    if(!SavableClass::Save(output, FreeE))
        throw info_except(" Could not save matter\n");
    output << "\n";
};
void FAZerCurveMatterSum_0_H2Minim::InputInternPar(istream &input) {
    MatterIO *Matter = dynamic_cast<MatterIO *>(SavableClass::Read(input));
    if(!Matter)
        throw info_except(" Could not register matter\n");
    FreeE = dynamic_cast<MatterFreeE *>(Matter);
    if(!FreeE)
        throw info_except(
            " Could not convert matter %s to freeE\n", Matter->class_name());

    FreeESumMatter *Check;
    if(!(Check = dynamic_cast<FreeESumMatter *>((InterfaceFreeEIO *)FreeE->FreeEPtr)))
        throw info_except(" Could not get FreeESumMatter from FreeEMatter\n");
    if(!(dynamic_cast<FreeERossH2_Minim *>((InterfaceFreeEIO *)Check->FreeVect[0])))
        throw info_except(" First matter in FreeESumMatter is not FreeERossH2_Minim\n");
};


FAZerCurveMatterSum_FreeERoss::FAZerCurveMatterSum_FreeERoss() : FAZerCurveBase() {
    FreeE = new MatterFreeE();
    FreeE->SetNewMatter(new FreeESumMatter());
    name = strdup("FAZerCurveMatterSum_FreeERoss");
    helppar[0] = strdup(" MatterSum with FreeEPureRoss - minim matter - first matter \n");
    int i = SetStdPar_GrInt(namepar, valpar, helppar, 1);
    namepar[i + 1] = strdup("  Alpha    ");
    valpar[i + 1] = 0;
    helppar[i + 1] = strdup("");
    namepar[i + 2] = strdup(" Epsilon   ");
    valpar[i + 2] = 1;
    helppar[i + 2] = strdup("");
    namepar[i + 3] = strdup(" Radius0   ");
    valpar[i + 3] = 1;
    helppar[i + 3] = strdup("");
    namepar[i + 4] = strdup(" CutNiuDiam");
    valpar[i + 4] = 1;
    helppar[i + 4] = strdup("");
    namepar[i + 5] = strdup(" RadiusExp ");
    valpar[i + 5] = 1;
    helppar[i + 5] = strdup(
        "if negative= use fixed (taken by module),if zero - autogenerate , positive - autogenerate*coef");
    namepar[i + 6] = strdup(" MulExp    ");
    valpar[i + 6] = 1;
    helppar[i + 6] = strdup("auto generated");
    namepar[i + 7] = strdup(" ArgExp    ");
    valpar[i + 7] = 1;
    helppar[i + 7] = strdup("auto generated");
    NumPar = i + 7;
};
Ref<SavableClass> FAZerCurveMatterSum_FreeERoss::GenerateVar(double *par) {
    FreeESumMatter *Sum =
        dynamic_cast<FreeESumMatter *>((InterfaceFreeEIO *)FreeE->FreeEPtr);
    FreeEPureRoss *Mat =
        dynamic_cast<FreeEPureRoss *>((InterfaceFreeEIO *)Sum->FreeVect[0]);
    Mat->par->Alpha = par[3];
    Mat->par->Epsilon = par[4];
    Mat->par->Radius0 = par[5];
    double RadiusExp = par[7];
    if(RadiusExp >= 0) {
        Exp6Param::FindRexp(*(Mat->par));
        if(RadiusExp > MathZer)
            Mat->par->RadiusExp *= RadiusExp;
        cout << " Rexp " << Mat->par->RadiusExp << " MulExp " << Mat->par->MulExp
             << " ArgExp " << Mat->par->ArgExp << "\n";
    } else {
        Mat->par->RadiusExp = fabs(RadiusExp);
        Exp6Param::Clc_ArgExpMulExp(*(Mat->par));
    }
    par[8] = Mat->par->MulExp;
    par[9] = Mat->par->ArgExp;
    Mat->par->CutDiameter_Niu = par[6];
    return new EOS_Savable(FreeE);
};

void FAZerCurveMatterSum_FreeERoss::OutputInternPar(ostream &output) {
    if(!SavableClass::Save(output, FreeE))
        throw info_except(" Could not save matter\n");
    output << "\n";
};
void FAZerCurveMatterSum_FreeERoss::InputInternPar(istream &input) {
    MatterIO *Matter = dynamic_cast<MatterIO *>(SavableClass::Read(input));
    if(!Matter)
        throw info_except(" Could not register matter\n");
    FreeE = dynamic_cast<MatterFreeE *>(Matter);
    if(!FreeE)
        throw info_except(
            " Could not convert matter %s to freeE\n", Matter->class_name());

    FreeESumMatter *Check;
    if(!(Check = dynamic_cast<FreeESumMatter *>((InterfaceFreeEIO *)FreeE->FreeEPtr)))
        throw info_except(" Could not get FreeESumMatter from FreeEMatter\n");
    if(!(dynamic_cast<FreeEPureRoss *>((InterfaceFreeEIO *)Check->FreeVect[0])))
        throw info_except(" First matter in FreeESumMatter is not FreeEPureRoss\n");
};
