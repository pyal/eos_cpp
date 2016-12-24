#include <lib/precompiled/eos.h>
#include "fre_ros.h"


static ClassDesc FreeEPureRoss_ClassDesc(
    typeid(FreeEPureRoss),
    "FreeEPureRoss",
    1,
    "MatterFreeE_category",
    create<FreeEPureRoss>);
static ClassDesc FreeERossN2_ClassDesc(
    typeid(FreeERossN2),
    "FreeERossN2",
    1,
    "MatterFreeE_category",
    create<FreeERossN2>);
static ClassDesc FreeERossH2_ClassDesc(
    typeid(FreeERossH2),
    "FreeERossH2",
    1,
    "MatterFreeE_category",
    create<FreeERossH2>);
static ClassDesc FreeERossH2_DisLim0_ClassDesc(
    typeid(FreeERossH2_DisLim0),
    "FreeERossH2_DisLim0",
    1,
    "MatterFreeE_category",
    create<FreeERossH2_DisLim0>);
static ClassDesc FreeERossH2_Minim_ClassDesc(
    typeid(FreeERossH2_Minim),
    "FreeERossH2_Minim",
    1,
    "MatterFreeE_category",
    create<FreeERossH2_Minim>);
static ClassDesc FreeERossXe_ClassDesc(
    typeid(FreeERossXe),
    "FreeERossXe",
    1,
    "MatterFreeE_category",
    create<FreeERossXe>);
static ClassDesc FreeERossBin_ClassDesc(
    typeid(FreeERossBin),
    "FreeERossBin",
    1,
    "MatterFreeE_category",
    create<FreeERossBin>);


FreeERossN2::FreeERossN2()
    : FreeEIOFind("material.cfg", "FreeERossN2Vcool"),
      DoubleState(1),
      AddElectronN(1),
      AddElectronN2(1),
      DissRoss(1),
      MetalE(0),
      DissA(0.045),
      DissV0(20),
      DissE(9.91) {
    par = new Exp6Param;
    Fid1 = new FreeEIdeal;
    Fid2 = new FreeEIdeal;
    SetInternalNames();
}

void FreeERossH2_DisLim0::OutputCompare(int N) {
    double Denc = 0.07, T = 1000, DencStp = 0.1;
    //  double FreERos1,FreERos2,FreEMet;
    cout << " Denc  Temp  FreERos1   FreERos2  FreEMet   DisCoef\n";
    for(int k = 1; k <= N; k++) {
        double Ros = Ross6Config(*par, Fid1->MolVeight, Denc, T);
        double Id1 = Fid1->FreeE(Denc, T), Id2 = Fid2->FreeE(Denc, T);
        // 	  double v0=DissV0,Eb=DissE,v=Fid1->MolVeight/Denc,dV=v-v0;
        //    if (v<=v0) Eb+=Diss1*dV+Diss2*sqr(dV)+Diss3*dV*dV*dV;
        //    Eb=max<double>(Eb,0);Eb=Eb*M_eV_K*M_Rconst/Fid1->MolVeight;
        double Eb = max<double>(Dis.EnergyShift(Denc, T), 0);
        double DisCoef = Eb / Dis.EnergyShift(0, T);

        cout << FM << Denc << FMT << T << FMT << Id1 + Ros << FMT << Ros + Id2 + Eb << FMT
             << Fmet->FreeE(Denc, T) + AddMet << FMT << DisCoef << "\n";
        Denc += DencStp;
    }
};
