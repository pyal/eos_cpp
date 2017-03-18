#include <lib/precompiled/eos.h>


#include "fre_dis2.h"

static ClassDesc ClcDissociation_FreeEDis2_ClassDesc(
    typeid(ClcDissociation::FreeEDis2),
    "ClcDissociation::FreeEDis2",
    1,
    "MatterFreeE_category",
    create<ClcDissociation::FreeEDis2>);

namespace ClcDissociation {
    Equilibrium2Pressure *Equilibrium2Pressure::StaticPtr = NULL;

    int Equilibrium2Pressure::read_data_state(FilterIn &si) {
        char tmp[256];
        Names.clear();
        MinDenc.clear();
        MaxDenc.clear();
        MinVolCoef.clear();
        MaxVolCoef.clear();
        si >> tmp >> FixedVolumeCoefs >> tmp >> AddPolicy >> tmp >> AlphaMullCoef >>
           tmp >> MaxPresMisfit >> tmp >> StartAddPressure >> tmp;
        //si>>PresDerivStp>>tmp;
        TestNextWord(si, "{", info_mark);
        for(size_t i = 0; i < 2; i++) {
            Stroka name;
            double minDenc, maxDenc, minVolC, maxVolC;
            si >> tmp >> name >> tmp >> minDenc >> tmp >> maxDenc >> tmp >> minVolC >>
               tmp >> maxVolC;
            Names.push_back(name);
            MinDenc.push_back(minDenc);
            MaxDenc.push_back(maxDenc);
            MinVolCoef.push_back(minVolC);
            MaxVolCoef.push_back(maxVolC);
        }
        TestNextWord(si, "}", info_mark);
        if(!si)
            throw info_except("Could not read class\n");
        LastAlphaVal = 0;
        AddPressure = StartAddPressure;
        return 1;
    }

    int Equilibrium2Pressure::save_data_state(FilterOut &so) {
        so << " FixedVolumeCoef " << FixedVolumeCoefs << " AddPolicy " << AddPolicy
           << " AlphaMullCoef " << AlphaMullCoef;
        so << " MaxPresMisfit " << MaxPresMisfit << " StartAddPressure "
           << StartAddPressure;   //<<" PresDerivStp "<<PresDerivStp;
        so << " PressureMinParams {";
        SavableClass::IncIdent();
        for(size_t i = 0; i < 2; i++)
            so << SavableClass::EOLN() << " Matter " << Names[i] << " MinDenc "
               << MinDenc[i] << " MaxDenc " << MaxDenc[i] << " MinVolCoef "
               << MinVolCoef[i] << " MaxVolCoef " << MaxVolCoef[i];
        SavableClass::DecIdent();
        so << EOLN() << "}" << SavableClass::EOLN();
        if(!so)
            throw info_except("Could not save class\n");
        return 1;
    }

    void Equilibrium2Pressure::FindEquilib(MatterCalculator *mats) {
        SetStartParams(mats);
        double alphaMin, alphaMax;
        double alphaGuess = ClcAlphaLim(alphaMin, alphaMax);
        if(FixedVolumeCoefs) {
            vector<double> volCoefs;
            volCoefs.push_back(mats->Substs[0].StartVolumeCoef);
            volCoefs.push_back(mats->Substs[1].StartVolumeCoef);
            Matters->SetVolCoefs(
                    (VecCl(volCoefs) / VecCl(volCoefs).SumElements()).Copy2Vector());
            return;
        }
        InitAddPressure(alphaMin, alphaMax, AddPolicy);
        FindEquilibFixed(mats, alphaGuess, alphaMin, alphaMax);
        if(AddPolicy == PolicyEnum.Int("Std") || LastPosMisfit == 1. / MathZer ||
           LastNegMisfit == 1. / MathZer)
            return;
        double maxAddP = AddPressure;
        alphaMin = min<double>(LastPosAlpha, LastNegAlpha);
        alphaMax = max<double>(LastPosAlpha, LastNegAlpha);
        InitAddPressure(alphaMin, alphaMax, PolicyEnum.Int("Std"));
        FindEquilibFixed(mats, alphaGuess, alphaMin, alphaMax);
        AddPressure = maxAddP;
    }

    vector<double> Equilibrium2Pressure::ClcN1N2Cage(MatterCalculator *mat) {
        vector<double> ret(2);
        ret[0] = 0;
        ret[1] = 1;
        //int i0 = NumInt2NumExt[NumSrt2NumInt[0]], i1 = NumInt2NumExt[NumSrt2NumInt[1]];
        int i0 = NumInt2NumExt[0], i1 = NumInt2NumExt[1];

        double d = mat->Denc;
        if(d < VecCl::Min(VecCl(MinDenc))) {
            ret[0] = -1;
            return ret;
        }
        if(d > VecCl::Max(VecCl(MaxDenc))) {
            ret[1] = -1;
            return ret;
        }
        if(In_Lim(d, MinDenc[0], MinDenc[1], 1)) {
            double lim = (MinDenc[1] - d) / (MinDenc[1] - MinDenc[0]);
            if(MinDenc[0] < MinDenc[1])
                ret[i0] = lim;
            else
                ret[i1] = lim;
            return ret;
        }
        if(In_Lim(d, MaxDenc[0], MaxDenc[1], 1)) {
            double lim = (MaxDenc[1] - d) / (MaxDenc[1] - MaxDenc[0]);
            if(MaxDenc[0] < MaxDenc[1])
                ret[i0] = lim;
            else
                ret[i1] = lim;
            return ret;
        }
        return ret;
    }

    void Equilibrium2Pressure::FindEquilibFixed(MatterCalculator *mats, double &alphaGuess, double alphaMin, double alphaMax) {
        double ErrorAbs = MaxPresMisfit, ErrorRel = MaxPresMisfit;
        int MaxIter = 100;
        int err;
        if(!In_Lim(alphaGuess, alphaMin, alphaMax, 0))
            alphaGuess = 0.5 * (alphaMin + alphaMax);
        LastPosMisfit = LastNegMisfit = 1 / MathZer;
        Equilibrium2Pressure *OldPtr = StaticPtr;
        StaticPtr = this;
        if(4 < (err = Fzero(
                FindMisfitPressureStatic,
                alphaMin,
                alphaMax,
                alphaGuess,
                ErrorAbs,
                ErrorRel,
                MaxIter))) {
            cout << " Bad FZero Equilibrium2Pressure \nfound X " << alphaGuess
                 << " Mis " << FindMisfitPressure(alphaGuess) << " err " << err
                 << "\n";
        }
        StaticPtr = OldPtr;
        SetResult(alphaGuess);
    }

    void Equilibrium2Pressure::SetResult(double alphaVal) {
        vector<double> volC(2);
        volC[NumInt2NumExt[NumSrt2NumInt[0]]] = 1;
        volC[NumInt2NumExt[NumSrt2NumInt[1]]] = alphaVal;
        Matters->SetVolCoefs(volC);
        LastAlphaVal = alphaVal;
    }

    vector<double> Equilibrium2Pressure::ClcAlphas(const vector<double> &x) {   //denc
        vector<double> nums = Matters->NumCoefs;
        vector<double> mius = Matters->MolWeights;
        vector<double> mass = Matters->ClcMasV().Copy2Vector();
        double M0 = mass[0] + mass[1];
        int i0 = NumInt2NumExt[NumSrt2NumInt[0]],
                i1 = NumInt2NumExt[NumSrt2NumInt[1]];

        double Ro = Matters->Denc;
        vector<double> alphas;

        alphas.push_back((-Ro * mass[i0] + x[0] * M0) / (Ro * nums[i1] * mius[i0]));
        alphas.push_back((Ro * nums[i0] * mius[i1]) / (-Ro * mass[i1] + x[1] * M0));

        return alphas;
    }

    double Equilibrium2Pressure::ClcAlphaLim(double &aMin, double &aMax) {
        vector<double> maxD = ClcAlphas(MaxDenc);
        vector<double> minD = ClcAlphas(MinDenc);

        int i0 = NumInt2NumExt[NumSrt2NumInt[0]],
                i1 = NumInt2NumExt[NumSrt2NumInt[1]];
        aMax = min<double>(maxD[i0], minD[i1]);
        if(minD[i1] < MathZer)
            aMax = maxD[i0];
        aMin = max<double>(maxD[i1], minD[i0]);
        if(!In_Lim(LastAlphaVal, aMin, aMax, 1))
            LastAlphaVal = (aMin + aMax) * 0.5;
        return LastAlphaVal;
    }

    void Equilibrium2Pressure::InitAddPressure(double aMin, double aMax, int addPolicy) {
        if(addPolicy == PolicyEnum.Int("Fixed")) {
            return;
        }
        VecCl presMin = GetPressures(aMin), presMax = GetPressures(aMax);
        double minV = min(VecCl::Min(presMin), VecCl::Min(presMax));
        if(minV < M_Eps2)
            AddPressure = -(minV + 5) * 20;
        else
            AddPressure = 0;
    }

    vector<double> Equilibrium2Pressure::CagePresClc() {
        vector<double> denc = Matters->ClcDencV().Copy2Vector(), ret;
        for(size_t i = 0; i < 2; i++) {
            int matI = NumInt2NumExt[i];
            if(denc[matI] < MinDenc[i])
                denc[matI] = MinDenc[i];
            if(denc[matI] > MaxDenc[i])
                denc[matI] = MaxDenc[i];
            ret.push_back(Matters->Substs[matI].ClcPres(denc[matI], Matters->Temp));
        }
        return ret;
    }

    void Equilibrium2Pressure::SetStartParams(MatterCalculator *mats) {
        Matters = mats;
        Matters->SetVolCoefs((VecCl(2) * 0 + 1).Copy2Vector());
        Matters->ClcFreeE();
        NumInt2NumExt = Matters->MapNumInt2NumExt(Names);
        vector<double> pres = CagePresClc();   // Matters->GetResultPressures();
        int i0 = 0, i1 = 1;
        NumSrt2NumInt.resize(2);
        NumSrt2NumInt[0] = i0;
        NumSrt2NumInt[1] = i1;
    }

    double Equilibrium2Pressure::FindMisfitPressure(double alphaVal) {
        vector<double> pres = GetPressures(alphaVal);
        double res = EncodeRes(pres);
        if(res > 0 && res < LastPosMisfit) {
            LastPosMisfit = res;
            LastPosAlpha = alphaVal;
        }
        if(res < 0 && fabs(res) < LastNegMisfit) {
            LastNegMisfit = fabs(res);
            LastNegAlpha = alphaVal;
        }

        return res;
    }

    double Equilibrium2Pressure::EncodeRes(const vector<double> &pres) {
        double minP = min(pres[0], pres[1]);
        if(minP + AddPressure < M_Eps2) {
            AddPressure = max<double>(0, -minP) * 2 + 1;
            log_debug(Stroka("Shifting AddPressure ") + AddPressure );
        }
        return log((pres[0] + AddPressure) / (pres[1] + AddPressure));
    }

    Equilibrium2Number *Equilibrium2Number::StaticPtr = NULL;

    void Equilibrium2NumberBase::SetMatterNumbers(double logX) {
        double x = 1 / (1 + exp(-logX));
        double xm = 1 / (1 + exp(logX));
        ChemEquation &eq = Matters->ChemEqs[0];
        vector<double> numC =
                (VecCl((int)Matters->MolWeights.size()) * 0).Copy2Vector();

        for(int iEq = 0; iEq < 2; iEq++)
            if(eq.GetSubstCoef(iEq) > 0)
                numC[NumInt2NumExt[iEq]] = eq.GetSubstCoef(iEq) * xm;
            else
                numC[NumInt2NumExt[iEq]] = -eq.GetSubstCoef(iEq) * x;
        Matters->NumCoefs = numC;
    }

    vector<double> Equilibrium2NumberBase::MakeLogXCage() {
        vector<double> n1n2Cage = PresFinder->ClcN1N2Cage(Matters);
        vector<double> ret(2);
        ret[0] = -30;
        ret[1] = 30;
        if(VecCl::Min(VecCl(n1n2Cage)) < 0)
            throw info_except("Cannot calculate for dencity <%d>\n", Matters->Denc);
        if(n1n2Cage[0] != 0)
            ret[1] = log(n1n2Cage[0] / (1 - n1n2Cage[0]));
        if(n1n2Cage[1] != 1)
            ret[0] = log(n1n2Cage[1] / (1 - n1n2Cage[1]));
        if(Matters->ChemEqs[0].GetSubstCoef(0) < 0) {   // n1-n2 exchange
            double t = ret[0];
            ret[0] = -ret[1];
            ret[1] = -t;
        }

        return ret;
    }

    vector<double> Equilibrium2NumberBase::ClcFreeEMixt(MatterCalculator *mats) {
        mats->ClcFreeE();
        double kT = M_Rconst * mats->Temp;
        vector<double> nums = mats->NumCoefs;
        vector<double> vols = mats->ClcVolV().Copy2Vector();
        double m0 = 0;
        double v0 = 0;

        vector<double> M = mats->ClcMasV().Copy2Vector();
        for(size_t iClast = 0; iClast < NumInt2NumExt.size(); iClast++) {
            int iMat = NumInt2NumExt[iClast];
            m0 += M[iMat];
            v0 += vols[iMat];
        }
        vector<double> fMix(nums.size());

        for(size_t iClast = 0; iClast < NumInt2NumExt.size(); iClast++) {
            int iMat = NumInt2NumExt[iClast];
            log_debug(Stroka("mat wei ") + mats->Substs[iMat].MolWeight + " clust " + ClasterSizes[iClast]);
            fMix[iMat] = nums[iMat] * kT *
                         log(vols[iMat] / v0) / ClasterSizes[iClast] ; //  M[]i] kT/ Miu[i] = N[i] kT
        }
        return (VecCl(fMix) / m0).Copy2Vector();
    }

    double Equilibrium2NumberBase::ClcFreeE(MatterCalculator *mats) {
        double fMix = VecCl(ClcFreeEMixt(mats)).SumElements();
//            vector<double> nums = mats->NumCoefs;
        vector<double> fE = mats->GetResultFreeEPures();
        vector<double> M = mats->ClcMasV().Copy2Vector();
        double m0 = 0;
        double freeE = 0;
        for(size_t iClast = 0; iClast < NumInt2NumExt.size(); iClast++) {
            int iMat = NumInt2NumExt[iClast];
            m0 += M[iMat];
            freeE += M[iMat] * fE[iMat];
        }
        return freeE / m0 + fMix;
    }

    int Equilibrium2NumberBase::save_data_state(FilterOut &so) {
        so << " MatName_ClastSize { ";
        SavableClass::IncIdent();
        for(size_t i = 0; i < 2; i++)
            so << SavableClass::EOLN() << " Matter " << Names[i] << " ClasterSize "
               << ClasterSizes[i];
        SavableClass::DecIdent();
        so << EOLN() << "}" << SavableClass::EOLN();
        if(!so)
            throw info_except("Could not save class\n");
        return 1;
    }

    int Equilibrium2NumberBase::read_data_state(FilterIn &si) {
        ClasterSizes.resize(2);
        Names.resize(2);
        si >> Verify("MatName_ClastSize") >> Verify("{");
        for(size_t i = 0; i < 2; i++) {
            Stroka name;
            double clasterSize;
            si >> Verify("Matter") >> name >> Verify("ClasterSize") >> clasterSize;
            verify(si, "Failed reading stream");
            Names[i] = name;
            ClasterSizes[i] = clasterSize;
        }
        TestNextWord(si, "}", info_mark);
        verify(si, "Could not read class");
        LastLogX = 0;
        return 1;
    }

    double Equilibrium2Number::FindMisfitLogX(double logX) {
        SetMatterNumbers(logX);
        PresFinder->FindEquilib(Matters);

        Matters->ClcFreeE();
        double kT = M_Rconst * Matters->Temp;
        double N0 = VecCl(Matters->NumCoefs).SumElements();
        vector<double> vols = Matters->ClcVolV().Copy2Vector(),
                dencs = Matters->ClcDencV().Copy2Vector();
        vector<double> fE = Matters->GetResultFreeEPures(),
                pres = Matters->GetResultPressures();
        double V0 = VecCl(vols).SumElements();
        ChemEquation &eq = Matters->ChemEqs[0];
        double mis = 0;
        for(int iClast = 0; iClast < 2; iClast++) {
            size_t iMat = NumInt2NumExt[iClast];
            double miu = Matters->MolWeights[iMat];
            log_debug(Stroka("min wei ") + Matters->Substs[iMat].MolWeight + " clust " + ClasterSizes[iClast] + " miu " + miu);
            double clastRT = kT / (ClasterSizes[iClast]) / miu;
            mis += eq.GetSubstCoef(iClast) *
                   (clastRT + pres[iMat] * miu / dencs[iMat] -
                    clastRT * vols[iMat] * N0 / (V0 * Matters->NumCoefs[iMat]) +
                    fE[iMat] * miu + clastRT * log(vols[iMat] / V0));
        }
        return mis;
    }

    Equilibrium2NumberMin *Equilibrium2NumberMin::StaticPtr = NULL;
    FreeEDis2::FreeEDis2() : FreeEDisStdIO(){}

    double FreeEDis2::FreeE(double Dencity, double Temperature) {
        MattersClc->SetClc(Dencity, Temperature);
        NumMinim.SetPresMinim(&PresMinim);
        NumMinimMin.SetPresMinim(&PresMinim);
        log_debug(Stroka(" denc ") + MattersClc->Denc + " temp " + MattersClc->Temp);
        PresMinim.FindEquilib(MattersClc);
        NumMinim.FindEquilib(MattersClc);
        log_debug(Stroka(" Zer lgX ") + NumMinim.GetLastLogX() + " val " + NumMinim.ClcFreeE(MattersClc));
        NumMinimMin.SetGuess(NumMinim.GetLastLogX());
        NumMinimMin.FindEquilib(MattersClc);
        log_debug(Stroka(" Min lgX ") + NumMinim.GetLastLogX() + " val " + NumMinim.ClcFreeE(MattersClc));
        return NumMinimMin.ClcFreeE(MattersClc);
    }


    void FreeEDisStdIO::ReadSubstancePar(istream &in) {
        MattersClc = new MatterCalculator;
        MattersClc->Read(in);
        SetInternalNames();
    }

    double FreeEDisStdIO::OutputValClc(const map<Stroka, double> &Input, const Stroka &Output) {
        void *ptr;
        if(CheckPresence(OutputNames, Output, ptr) && ptr != NULL) {
            FreeE(
                    CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"));
            int k1 = MattersClc->Name2Int.Str2Num(
                    Stroka(Output, 0, Output.length() - 2).c_str(), 0);
            int k2 = MattersClc->Name2Int.Str2Num(
                    Stroka(Output, 0, Output.length() - 3).c_str(), 0);
            Stroka Test1 = Stroka(Output, Output.length() - 1, Output.length());
            Stroka Test2 = Stroka(Output, Output.length() - 2, Output.length());
            if(Test1 == Stroka("V"))
                return MattersClc->GetResultVolumeCoefs()[k1];
            if(Test1 == Stroka("M"))
                return MattersClc->GetResultRelatMasses()[k1];
            if(Test1 == Stroka("P"))
                return MattersClc->GetResultPressures()[k1];
            if(Test1 == Stroka("D"))
                return MattersClc->GetResultDencities()[k1];
            if(Test2 == Stroka("Fp"))
                return MattersClc->GetResultFreeEPures()[k2];
            if(Test2 == Stroka("Fc"))
                return GetResultFreeECorrs()[k2];
        }
        return FreeEIOFind::OutputValClc(Input, Output);
    }

    void FreeEDisStdIO::SetInternalNames() {
        FreeEIOFind::SetInternalNames();
        for(size_t i = 0; i < MattersClc->Substs.size(); i++) {
            OutputNames[MattersClc->Substs[i].GetName() + "_M"] = &i;
            OutputNames[MattersClc->Substs[i].GetName() + "_V"] = &i;
            OutputNames[MattersClc->Substs[i].GetName() + "_P"] = &i;
            OutputNames[MattersClc->Substs[i].GetName() + "_D"] = &i;
            OutputNames[MattersClc->Substs[i].GetName() + "_Fp"] = &i;
            OutputNames[MattersClc->Substs[i].GetName() + "_Fc"] = &i;
        }
    }












//    double FreeEDis2::FreeEvar(double Dencity, double Temperature) {
//        MattersClc->SetClc(Dencity, Temperature);
//        PresMinim.FindEquilib(MattersClc);
//        NumMinim.SetPresMinim(&PresMinim);
//        NumMinimMin.SetPresMinim(&PresMinim);
//
//        NumMinim.FindEquilib(MattersClc);
//
//        VecCl numCoefs(MattersClc->NumCoefs), numCoefsOld;
//        numCoefs = numCoefs / numCoefs.SumElements();
//        numCoefsOld = numCoefs;
//        int numIter = 0, maxNumIter = 30;
//        do {
//            MattersClc->NumCoefs = (numCoefs * 0.5 + numCoefsOld * 0.5).Copy2Vector();
//            numCoefs = VecCl(MattersClc->NumCoefs);
//
//            numCoefsOld = numCoefs;
//            NumMinim.FindEquilib(MattersClc);
//            NumMinimMin.SetGuess(NumMinim.GetLastLogX());
//            NumMinimMin.FindEquilib(MattersClc);
//
//            numCoefs = VecCl(MattersClc->NumCoefs);
//            numCoefs = numCoefs / numCoefs.SumElements();
//
//        } while(VolCoefMisfit < VecCl::ModMax(numCoefsOld - numCoefs) &&
//                numIter++ < maxNumIter);
//        if(numIter >= maxNumIter) {
//            cout << " Bad result, numCoefsMisfit "
//                 << VecCl::ModMax(numCoefsOld - numCoefs) << "\n";
//            cout << " numCoefs " << numCoefs << "\n";
//            cout << " numCoefsOld " << numCoefsOld << "\n";
//        }
//        return NumMinim.ClcFreeE(MattersClc);
//    }
//
//    double FreeEDis2::FreeEgood(double Dencity, double Temperature) {
//        MattersClc->SetClc(Dencity, Temperature);
//        PresMinim.FindEquilib(MattersClc);
//        NumMinim.SetPresMinim(&PresMinim);
//        NumMinim.FindEquilib(MattersClc);
//
//        VecCl numCoefs(MattersClc->NumCoefs), numCoefsOld;
//        numCoefs = numCoefs / numCoefs.SumElements();
//        numCoefsOld = numCoefs;
//        int numIter = 0, maxNumIter = 10;
//        do {
//            MattersClc->NumCoefs = (numCoefs * 0.5 + numCoefsOld * 0.5).Copy2Vector();
//            numCoefs = VecCl(MattersClc->NumCoefs);
//
//            numCoefsOld = numCoefs;
//            NumMinim.FindEquilib(MattersClc);
//
//            numCoefs = VecCl(MattersClc->NumCoefs);
//            numCoefs = numCoefs / numCoefs.SumElements();
//
//        } while(VolCoefMisfit < VecCl::ModMax(numCoefsOld - numCoefs) &&
//                numIter++ < maxNumIter);
//        //PresMinim.FindEquilib(MattersClc);
//        if(numIter >= maxNumIter) {
//            cout << " Bad result, numCoefsMisfit "
//                 << VecCl::ModMax(numCoefsOld - numCoefs) << "\n";
//            cout << " numCoefs " << numCoefs << "\n";
//            cout << " numCoefsOld " << numCoefsOld << "\n";
//        }
//        //cout<<" numIter "<<numIter<<" res "<<VecCl::ModMax(volCoefsOld - volCoefs)<<"\n";
//        //NumMinim.FindEquilib(MattersClc);
//        return NumMinim.ClcFreeE(MattersClc);
//    }
//
//    double FreeEDis2::FreeE1(double Dencity, double Temperature) {
//        MattersClc->SetClc(Dencity, Temperature);
//        PresMinim.FindEquilib(MattersClc);
//        NumMinim.SetPresMinim(&PresMinim);
//        VecCl volCoefs = MattersClc->ClcVolV(), volCoefsOld;
//        volCoefs = volCoefs / volCoefs.SumElements();
//        VecCl numCoefs(MattersClc->NumCoefs), numCoefsOld;
//        numCoefs = numCoefs / numCoefs.SumElements();
//        int numIter = 0;
//        do {
//            volCoefsOld = volCoefs;
//            numCoefsOld = numCoefs;
//
//            NumMinim.FindEquilib(MattersClc);
//
//            numCoefs = VecCl(MattersClc->NumCoefs);
//            numCoefs = numCoefs / numCoefs.SumElements();
//            MattersClc->NumCoefs = (numCoefs * 0.5 + numCoefsOld * 0.5).Copy2Vector();
//            numCoefs = VecCl(MattersClc->NumCoefs);
//
//            PresMinim.FindEquilib(MattersClc);
//
//            volCoefs = MattersClc->ClcVolV();
//            volCoefs = volCoefs / volCoefs.SumElements();
//            //cout<<VecCl::ModMax(volCoefsOld - volCoefs)<<"\nvC "<<volCoefs<<" vCold "<<volCoefsOld<<" numC "<< numCoefs<<" numCold "<<numCoefsOld;
//        } while(VolCoefMisfit < VecCl::ModMax(volCoefsOld - volCoefs) &&
//                numIter++ < 30);
//        if(numIter >= 100) {
//            cout << " Bad result, volCoefsMisfit "
//                 << VecCl::ModMax(volCoefsOld - volCoefs) << "\n";
//            cout << " volCoefs " << volCoefs << "\n";
//            cout << " volCoefsOld " << volCoefsOld << "\n";
//            cout << " numCoefs " << numCoefs << "\n";
//            cout << " numCoefsOld " << numCoefsOld << "\n";
//        }
//        //cout<<" numIter "<<numIter<<" res "<<VecCl::ModMax(volCoefsOld - volCoefs)<<"\n";
//        NumMinim.FindEquilib(MattersClc);
//        return NumMinim.ClcFreeE(MattersClc);
//    };
};   //ClcDissociation