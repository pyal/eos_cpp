#pragma once

#include "fre_fac.h"
#include "mat/NdimMin.h"
#include "mat/fun_fac.h"
#include "lib/ref/refmanip.h"

namespace ClcDissociation {

    struct NamedMatter : RefCount {
        NamedMatter() : Dencity(1e5), Temperature(-1) {
            Init(new FreeEIdeal, 1, "IdealStub", 1, 1, 1, M_Eps2 * 2);
        }
        NamedMatter(const NamedMatter &mat) {
            Init(
                mat.FreeMat,
                mat.MolWeight,
                mat.Name.c_str(),
                mat.StartProportion,
                mat.StartVolumeCoef,
                mat.SubstMixtures,
                mat.DeriveConst);
        }
        void Init(
            Ref<InterfaceFreeEIO> freemat,
            double molweight,
            const char *name,
            double startproportion,
            double startvolumecoef,
            double substmixtures,
            double derivconst) {
            MolWeight = molweight;
            FreeMat = freemat;
            Dencity = MathZer;
            Temperature = -1;
            DeriveConst = derivconst;
            Name = name;
            StartProportion = startproportion;
            StartVolumeCoef = startvolumecoef;
            SubstMixtures = substmixtures;
        }
        InterfaceFreeEIO *GetMatter() {
            return FreeMat;
        }
        Stroka GetName() {
            return Name;
        }
        virtual double ClcFreeE(double denc, double temp) {
            CheckParams(denc, temp);
            return FreeE;
        }
        virtual double ClcPres(double denc, double temp) {
            CheckParams(denc, temp);
            return Pressure;
        }
        virtual double GetFreeE() {
            return FreeE;
        }
        virtual double GetPres() {
            return Pressure;
        }
        int Read(istream &in) {
            char tmp[256];
            in >> tmp;
            if(Stricmp(tmp, "}") == 0) return 0;
            in >> Name >> Verify("MolWeight") >> MolWeight >> Verify("StartProportion") >> StartProportion >>
               Verify("StartVolumeCoef") >> StartVolumeCoef >> Verify("SubstMixtures") >> SubstMixtures >>
               Verify("DeriveConst") >> DeriveConst >> Verify("MatterFreeE");
            verify(in, "Could not read params");
            verify(FreeMat << SavableClass::Read(in), "Could not convert matter with Name " + Name + " to InterfaceFreeEIO");
            Temperature = -1;
            Dencity = 1e5;
            return 1;
        }
        void Save(ostream &out) {
            out << " Name " << Name.c_str() << " MolWeight " << MolWeight
                << " StartProportion " << StartProportion;
            out << " StartVolumeCoef " << StartVolumeCoef << " SubstMixtures "
                << SubstMixtures << " DeriveConst " << DeriveConst;
            out << " MatterFreeE ";
            SavableClass::Save(out, FreeMat);
        }
        double MolWeight, StartProportion, StartVolumeCoef, SubstMixtures;

    private:
        double DeriveConst;
        Ref<InterfaceFreeEIO> FreeMat;
        double Dencity, Temperature;
        double FreeE, Pressure;
        Stroka Name;

        void CheckParams(double denc, double temp) {
            if(denc < 2 * MathZer)
                denc = 2 * MathZer;
            if(fabs(denc - Dencity) < 0.001 * DeriveConst * Dencity &&
               fabs(temp - Temperature) < 0.1 * StndErr)
                return;
            Dencity = denc;
            Temperature = temp;
            double d = max<double>(Dencity, MathZer),
                   t = max<double>(Temperature, MathZer);
            FreeE = FreeMat->FreeE(d, t);
            double FreeEder = (FreeMat->FreeE(d * (1 + 0.5 * DeriveConst), t) -
                               FreeMat->FreeE(d * (1 - 0.5 * DeriveConst), t)) /
                              (d * DeriveConst);
            Pressure = FreeEder * sqr(d);
        }
    };
    struct ChemEquation : RefCount {
        ChemEquation() {
            SubstNames.push_back("Mat1");
            SubstNames.push_back("Mat2");
            //SubstNums.push_back(0);SubstNums.push_back(1);
            SubstCoefs.push_back(1);
            SubstCoefs.push_back(1);
        };
        ChemEquation(const ChemEquation &eq)
            : SubstNames(eq.SubstNames),
              SubstCoefs(eq.SubstCoefs),
              NumExt2NumInt(eq.NumExt2NumInt){};
        int Read(istream &in) {
            SubstNames.clear();
            SubstCoefs.clear();
            char tmp[256];
            in >> tmp;
            if(Stricmp(tmp, "}") == 0)
                return 0;
            if(Stricmp(tmp, "{") != 0)
                throw info_except("Expected { and got %s\n", tmp);
            in >> tmp;
            while(Stricmp(tmp, "}") != 0) {
                Stroka Name = tmp;
                SubstNames.push_back(tmp);
                double d;
                in >> d;
                SubstCoefs.push_back(d);
                in >> tmp;
            }
            return 1;
        }
        void Save(ostream &out) {
            out << " { ";
            for(size_t k = 0; k < SubstNames.size(); k++)
                out << "  " << SubstNames[k].c_str() << " " << SubstCoefs[k] << "    ";
            out << " }\n";
        }
        void CheckWeights(vector<NamedMatter> &substV, Str::StrokaStorage &name2Int) {
            vector<double> weights;
            NumExt2NumInt.resize(substV.size());
            for(size_t j = 0; j < NumExt2NumInt.size(); j++)
                NumExt2NumInt[j] = -1;
            vector<int> numInt2ext(SubstNames.size());
            for(size_t j = 0; j < SubstNames.size(); j++) {
                int jExt = name2Int.Str2Num(~SubstNames[j]);
                NumExt2NumInt[jExt] = (int)j;
                numInt2ext[j] = jExt;
                weights.push_back(substV[jExt].MolWeight);
            }
            double Res = VecCl(weights) * VecCl(SubstCoefs);
            if(fabs(Res) > MathZer) {
                FilterTextOut out("tmp", DataSource::Memory);
                out << "\nBad chem react result is: " << Res << "\n";
                out << "Vector Of Chem React Coef " << VecCl(SubstCoefs);
                out << "Vector Subst Weights      " << VecCl(weights) << "Matters:";
                for(size_t i = 0; i < SubstNames.size(); i++) {
                    out << "\n" << SubstNames[i] << " -> ";
                    substV[numInt2ext[i]].GetMatter()->save_data_state(out);
                }
                out << "\n";
                throw info_except(DataSource::GetStr("tmp").c_str());
            }
        }
        double GetSubstCoef(int num) {
            return SubstCoefs[NumExt2NumInt[num]];
        }

    private:
        vector<Stroka> SubstNames;
        vector<double> SubstCoefs;
        vector<int> NumExt2NumInt;
    };

    struct MatterCalculator : RefCount {
        MatterCalculator() : Substs(2), ChemEqs(1) {}
        void ClcFreeE() {
            vector<double> dencV = ClcDencV().Copy2Vector();
            for(size_t i = 0; i < Substs.size(); i++)
                Substs[i].ClcFreeE(max<double>(dencV[i], MathZer), Temp);
        }
        void SetClc(double denc, double temp) {
            Denc = max<double>(denc, MathZer);
            Temp = temp;
            if(NumCoefs.size() != MolWeights.size())
                NumCoefs = (VecCl(MolWeights) * 0 + 1).Copy2Vector();
        }
        void SetVolCoefs(const vector<double> &volCoefs) {
            VolCoefs = volCoefs;
            MulCoef = 1;
            MulCoef = ClcMasV().SumElements() / ClcVolV().SumElements() / Denc;
        }
        VecCl ClcMasV() {
            return VecCl::ComponentMul(VecCl(NumCoefs), VecCl(MolWeights));
        }
        VecCl ClcVolV() {
            return VecCl::ComponentMul(VecCl(NumCoefs), VecCl(VolCoefs)) * MulCoef;
        }
        VecCl ClcDencV() {
            return VecCl::ComponentDiv(ClcMasV(), ClcVolV());
        }

        vector<double> GetResultVolumeCoefs() {
            return VolCoefs;
        }
        vector<double> GetResultRelatMasses() {
            VecCl mas = VecCl::ComponentMul(VecCl(NumCoefs), VecCl(MolWeights));
            mas = mas / mas.SumElements();
            return mas.Copy2Vector();
        }
        vector<double> GetResultPressures() {
            vector<double> res;
            for(size_t i = 0; i < Substs.size(); i++) {
                res.push_back(Substs[i].GetPres());
            }
            return res;
        }
        vector<double> GetResultDencities() {
            return ClcDencV().Copy2Vector();
        }

        vector<double> GetResultFreeEPures() {
            vector<double> res;
            for(size_t i = 0; i < Substs.size(); i++) {
                res.push_back(Substs[i].GetFreeE());
            }
            return res;
        }

        double MulCoef, Denc, Temp;
        vector<NamedMatter> Substs;
        vector<double> NumCoefs, MolWeights;
        vector<ChemEquation> ChemEqs;

        vector<int> MapNumExt2NumInt(vector<Stroka> &names) {
            vector<int> numExt2NumInt(Substs.size());
            for(size_t j = 0; j < numExt2NumInt.size(); j++)
                numExt2NumInt[j] = -1;
            for(size_t i = 0; i < names.size(); i++)
                numExt2NumInt[Name2Int.Str2Num(names[i].c_str())] = (int)i;
            return numExt2NumInt;
        }
        vector<int> MapNumInt2NumExt(vector<Stroka> &names) {
            vector<int> numInt2NumExt(names.size());
            for(size_t j = 0; j < numInt2NumExt.size(); j++)
                numInt2NumExt[j] = -1;
            for(size_t i = 0; i < names.size(); i++)
                numInt2NumExt[i] = Name2Int.Str2Num(names[i].c_str());
            return numInt2NumExt;
        }
        void Read(istream &in) {
            char tmp[256];
            in >> tmp;
            TestNextWord(in, "{", info_mark);
            NamedMatter sbst;
            Name2Int.Clear();
            Substs.clear();
            MolWeights.clear();
            while(!(!in) && !SavableClass::TestNextChar(in, '}')) {
                verify(sbst.Read(in), "Could not read substance....\n");
                verify(Name2Int.Str2Num(~sbst.GetName(), 1) == Substs.size(), "Duplicate? name <" + sbst.GetName() + "> found");
                Substs.push_back(sbst);
                MolWeights.push_back(sbst.MolWeight);
            }
            in >> tmp;
            TestNextWord(in, "{", info_mark);
            ChemEquation eq;
            ChemEqs.clear();
            while(!(!in) && !SavableClass::TestNextChar(in, '}')) {
                verify(eq.Read(in), "Could not read substance....\n");
                eq.CheckWeights(Substs, Name2Int);
                ChemEqs.push_back(eq);
            }
            NumCoefs = (VecCl((int)Substs.size()) * 0 + 1).Copy2Vector();
        }
        void Save(ostream &out) {
            out << " EnterList_SubsName_FreeESubstance_SubsMolVeight { ";
            SavableClass::IncIdent();

            for(size_t i = 0; i < Substs.size(); i++) {
                out << SavableClass::EOLN();
                Substs[i].Save(out);
            }
            SavableClass::DecIdent();
            out << " } " << SavableClass::EOLN()
                << " EnterListOfVectors_Vector_SubsName_SubsCoef { ";
            SavableClass::IncIdent();
            for(size_t k = 0; k < ChemEqs.size(); k++) {
                out << SavableClass::EOLN();
                ChemEqs[k].Save(out);
            }
            SavableClass::DecIdent();
            out << " } " << SavableClass::EOLN();
        }

        Str::StrokaStorage Name2Int;

    private:
        vector<double> VolCoefs;
    };

    struct EquilibriumBase : SavableClass {
        virtual void FindEquilib(MatterCalculator *calc) = 0;
    };

    struct Equilibrium2Pressure : EquilibriumBase {

        Equilibrium2Pressure() : EquilibriumBase(), PolicyEnum("Std Fixed") {
            MinDenc = (VecCl(2) * 0 + 1e-4).Copy2Vector();
            MaxDenc = (VecCl(2) * 0 + 1).Copy2Vector();
            MinVolCoef = (VecCl(2) * 0 + 1e-4).Copy2Vector();
            MaxVolCoef = (VecCl(2) * 0 + 1e4).Copy2Vector();
            FixedVolumeCoefs = 0;
            MaxPresMisfit = M_Eps2 * 0.01;
            Names.push_back("Fst");
            Names.push_back("Sec");
            AddPolicy = 0;
            AlphaMullCoef = 1;
            StartAddPressure = 1e6;
        }

        int read_data_state(FilterIn &si);
        int save_data_state(FilterOut &so);
        Stroka MakeHelp() {
            Stroka res =
                "Class for finding volume coefs of 2 matters : Temp, MeanDenc is given... Policy may be " +
                PolicyEnum.ListOptions();
            return res;
        }
        virtual void FindEquilib(MatterCalculator *mats);
        vector<double> ClcN1N2Cage(MatterCalculator *mat);

    private:
        vector<double> MinDenc, MaxDenc, MinVolCoef, MaxVolCoef;
        int FixedVolumeCoefs;
        double MaxPresMisfit;

        double AlphaMullCoef, StartAddPressure;
        int AddPolicy;
        Str::OptionSet PolicyEnum;

        MatterCalculator *Matters;
        vector<Stroka> Names;
        vector<int> NumInt2NumExt, NumSrt2NumInt;
        static Equilibrium2Pressure *StaticPtr;

        double LastAlphaVal, AddPressure;
        double LastPosAlpha, LastNegAlpha;
        double LastPosMisfit, LastNegMisfit;

        virtual void FindEquilibFixed(
            MatterCalculator *mats,
            double &alphaGuess,
            double alphaMin,
            double alphaMax);
        void SetResult(double alphaVal);
        vector<double> ClcAlphas(const vector<double> &x);
        double ClcAlphaLim(double &aMin, double &aMax);
        void InitAddPressure(double aMin, double aMax, int addPolicy);
        vector<double> CagePresClc();
        void SetStartParams(MatterCalculator *mats);

        static double FindMisfitPressureStatic(double alphaVal) {
            return StaticPtr->FindMisfitPressure(alphaVal);
        }
        double FindMisfitPressure(double alphaVal);
        vector<double> GetPressures(double alphaVal) {
            SetResult(alphaVal);
            Matters->ClcFreeE();
            return Matters->GetResultPressures();
        }
        double EncodeRes(const vector<double> &pres);
    };


    struct Equilibrium2NumberBase : EquilibriumBase {
        Equilibrium2NumberBase() : EquilibriumBase() {
            ClasterSizes = (VecCl(2) * 0 + 1).Copy2Vector();
            //MaxNumMisfit = M_Eps2*0.00001;
            MaxNumMisfit = M_Eps2;
            Names.push_back("Fst");
            Names.push_back("Sec");
        }

        int read_data_state(FilterIn &si);
        int save_data_state(FilterOut &so);
        Stroka MakeHelp() {
            Stroka res =
                "Class for finding numbers of dis atoms of 2 matters : Temp, MeanDenc, VolCoefs are given...";
            return res;
        }
        // To be called after FindEquilib
        double ClcFreeE(MatterCalculator *mats);
        // To be called after FindEquilib
        vector<double> ClcFreeEMixt(MatterCalculator *mats);
        void SetPresMinim(Equilibrium2Pressure *presFinder) {
            PresFinder = presFinder;
        }
        void SetLastLogX(double logXguess) {
            LastLogX = logXguess;
        }
        double GetLastLogX() {
            return LastLogX;
        }
        vector<double> MakeLogXCage();

    protected:
        Equilibrium2Pressure *PresFinder;
        vector<double> ClasterSizes;
        double MaxNumMisfit;
        vector<int> NumInt2NumExt;

        MatterCalculator *Matters;
        vector<Stroka> Names;

        double LastLogX;
        double SetStartParams(MatterCalculator *mats) {
            Matters = mats;
            if(Matters->ChemEqs.size() != 1)
                throw info_except("Bad chem eq size:<%i>\n", Matters->ChemEqs.size());

            NumInt2NumExt = Matters->MapNumInt2NumExt(Names);
            return LastLogX;
        }
        void SetMatterNumbers(double logX);
    };

    struct Equilibrium2Number : Equilibrium2NumberBase {
        Equilibrium2Number() : Equilibrium2NumberBase() {}

        virtual void FindEquilib(MatterCalculator *mats) {
            double logXguess = SetStartParams(mats);
            double ErrorAbs = MaxNumMisfit, ErrorRel = MaxNumMisfit;
            vector<double> clcCage = MakeLogXCage();
            int MaxIter = 100;
            int err;
            Equilibrium2Number *OldPtr = StaticPtr;
            StaticPtr = this;
            if(4 < (err = Fzero(
                        FindMisfitLogXStatic,
                        clcCage[0],
                        clcCage[1],
                        logXguess,
                        ErrorAbs,
                        ErrorRel,
                        MaxIter))) {
                cout << " Bad FZero Equilibrium2Number \nfound X " << logXguess << " Mis "
                     << FindMisfitLogX(logXguess) << " err " << err << "\n";
            }
            StaticPtr = OldPtr;
            SetMatterNumbers(logXguess);
            LastLogX = logXguess;
        }

    private:
        static Equilibrium2Number *StaticPtr;

        static double FindMisfitLogXStatic(double logX) {
            return StaticPtr->FindMisfitLogX(logX);
        }
        double FindMisfitLogX(double logX);
    };


    struct Equilibrium2NumberMin : Equilibrium2NumberBase {
        Equilibrium2NumberMin() : Equilibrium2NumberBase() {}

        void SetGuess(double xGuess) {}
        virtual void FindEquilib(MatterCalculator *mats) {
            double logXguess = SetStartParams(mats);
            double ErrorAbs = MaxNumMisfit, resFreeE;
            vector<double> clcCage = MakeLogXCage();
            Equilibrium2NumberMin *OldPtr = StaticPtr;
            StaticPtr = this;
            logXguess = Fmin(
                FindMisfitLogXStatic,
                clcCage[0],
                clcCage[1],
                logXguess,
                ErrorAbs,
                resFreeE);
            StaticPtr = OldPtr;
            SetMatterNumbers(logXguess);
            LastLogX = logXguess;
        }

    private:
        static Equilibrium2NumberMin *StaticPtr;

        static double FindMisfitLogXStatic(double logX) {
            return StaticPtr->FindMisfitLogX(logX);
        }
        double FindMisfitLogX(double logX) {
            SetMatterNumbers(logX);
            PresFinder->FindEquilib(Matters);
            double res = ClcFreeE(Matters);
            return res;
        }
    };


    struct FreeEDisStdIO : FreeEIOFind {
        virtual vector<double> GetResultFreeECorrs() = 0;
        FreeEDisStdIO() : FreeEIOFind() {
            MattersClc = new MatterCalculator();
            SetInternalNames();
        };
        virtual void SetInternalNames();
        virtual double OutputValClc(
            const map<Stroka, double> &Input,
            const Stroka &Output);

        void ReadSubstancePar(istream &in);
        void SaveSubstancePar(ostream &out) {
            MattersClc->Save(out);
        }
        Stroka MakeHelp() {
            Stroka res = FreeEIOFind::MakeHelp();
            res +=
                "Additionaly this class can calculate for MatName component: MatName_M (mass)  MatName_D (dencity) MatName_V (volumeCoefs) MatName_P (pressure) MatName_Fp (FreeE of  pure component) MatName_Fc (FreeE corrected(entropy?+-) of component)  \n";
            res +=
                "Calculate dissosiation of matters - adding to the free E of pure mat - entropy mixture addition. Pure free E calc under assumption that relat denc of matters are the same. \n";
            return res;
        }

    protected:
        Ref<MatterCalculator> MattersClc;
    };

    struct FreeEDis2 : FreeEDisStdIO {
        FreeEDis2();   // : FreeEDisStdIO() {};
        vector<double> GetResultFreeECorrs() {
            return NumMinim.ClcFreeEMixt(MattersClc);
        };
        void ReadSubstancePar(istream &in) {
            FreeEDisStdIO::ReadSubstancePar(in);
            in >> Verify("PressureMinim");
            FilterTextIn fin(in.rdbuf(), 0);
            PresMinim.read_data_state(fin);
            fin >> Verify("NumberMinim");
            NumMinim.read_data_state(fin);
            fin >> Verify("NumberMinimMin");
            NumMinimMin.read_data_state(fin);
            VolCoefMisfit = M_Eps2;
            verify(in, "Could not read class\n");
        }
        void SaveSubstancePar(ostream &out) {
            FreeEDisStdIO::SaveSubstancePar(out);
            out << " PressureMinim ";
            FilterTextOut fout(out.rdbuf(), 0);
            PresMinim.save_data_state(fout);
            fout << " NumberMinim ";
            NumMinim.save_data_state(fout);
            fout << " NumberMinimMin ";
            NumMinimMin.save_data_state(fout);
        }
        Stroka MakeHelp() {
            Stroka res = FreeEDisStdIO::MakeHelp();
            res += "Calculate dissosiation of two matters\n";
            return res;
        }
        double FreeE(double Dencity, double Temperature);
//        double FreeEvar(double Dencity, double Temperature);
//        double FreeEgood(double Dencity, double Temperature);
//        double FreeE1(double Dencity, double Temperature);

    private:
        Equilibrium2Pressure PresMinim;
        Equilibrium2Number NumMinim;
        Equilibrium2NumberMin NumMinimMin;
        double VolCoefMisfit;
    };


}   // ClcDissociation


