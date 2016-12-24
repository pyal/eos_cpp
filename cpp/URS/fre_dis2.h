#pragma once

#include "fre_fac.h"
#include "mat/NdimMin.h"
#include "mat/fun_fac.h"


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
            if(Stricmp(tmp, "}") == 0)
                return 0;
            in >> Name >> tmp >> MolWeight >> tmp >> StartProportion >> tmp >>
                StartVolumeCoef >> tmp >> SubstMixtures >> tmp;
            in >> DeriveConst >> tmp;
            if(!(FreeMat << SavableClass::Read(in)))
                throw info_except(
                    "Could not convert matter with Name %s to InterfaceFreeEIO\n",
                    Name.c_str());
            Temperature = -1;
            //DeriveConst = 2 * M_Eps2;
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
            //if (fabs(log(denc / Dencity)) + fabs(temp - Temperature) < 0.1 * StndErr)
            //    return ;
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
            //Pressure = FreeMat->Pressure(Dencity, Temperature);
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
        //int GetInd(const char *name) {
        //    return NameInt[name];
        //}
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
            //cout<<"Denc "<<Denc<<" Temp "<<Temp<<"\n"<<mas<<" NumCoefs "<<NumCoefs<<" MolWeights "<<MolWeights<<"\n";

            //if (fabs(fabs(mas[1]) + fabs(mas[2]) - 1) > StndErr)
            //cout<<"WAU mas "<<mas<<" NumCoefs "<<NumCoefs<<" MolWeights "<<MolWeights;
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
        //map<Stroka, int> NameInt;
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
                //while (1){
                //          if (GuessNextChar(in, '}'))
                //              break;
                if(!sbst.Read(in))
                    throw info_except("Could not read substance....\n");
                if(Name2Int.Str2Num(sbst.GetName().c_str(), 1) != Substs.size())
                    throw info_except(
                        "Duplicate? name <%s> found\n", sbst.GetName().c_str());
                Substs.push_back(sbst);
                MolWeights.push_back(sbst.MolWeight);
            }
            //in>>tmp>>tmp;
            in >> tmp;
            TestNextWord(in, "{", info_mark);
            ChemEquation eq;
            ChemEqs.clear();
            while(!(!in) && !SavableClass::TestNextChar(in, '}')) {
                //while (1){
                //          if (GuessNextChar(in, '}'))
                //              break;
                if(!eq.Read(in))
                    throw info_except("Could not read substance....\n");
                eq.CheckWeights(Substs, Name2Int);
                ChemEqs.push_back(eq);
            }
            //in>>tmp; // "}"
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

        int read_data_state(FilterIn &si) {
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
        int save_data_state(FilterOut &so) {
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
        Stroka MakeHelp() {
            Stroka res =
                "Class for finding volume coefs of 2 matters : Temp, MeanDenc is given... Policy may be " +
                PolicyEnum.ListOptions();
            return res;
        }
        virtual void FindEquilib(MatterCalculator *mats) {
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
        vector<double> ClcN1N2Cage(MatterCalculator *mat) {
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
            double alphaMax) {
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
            //vector<double> p = GetPressures(alphaGuess); cout<<" found "<<alphaGuess<<" mis "<<fabs(p[0]-p[1])<<" pres "<<p;
            SetResult(alphaGuess);
        }
        void SetResult(double alphaVal) {
            vector<double> volC(2);
            volC[NumInt2NumExt[NumSrt2NumInt[0]]] = 1;
            volC[NumInt2NumExt[NumSrt2NumInt[1]]] = alphaVal;
            Matters->SetVolCoefs(volC);
            LastAlphaVal = alphaVal;
        }
        vector<double> ClcAlphas(const vector<double> &x) {   //denc
            vector<double> nums = Matters->NumCoefs;
            vector<double> mius = Matters->MolWeights;
            vector<double> mass = Matters->ClcMasV().Copy2Vector();
            double M0 = mass[0] + mass[1];
            int i0 = NumInt2NumExt[NumSrt2NumInt[0]],
                i1 = NumInt2NumExt[NumSrt2NumInt[1]];

            double Ro = Matters->Denc;
            vector<double> alphas;

            //alphas.push_back(max<double>(MathZer, (- Ro * mass[i0] + x[0] * M0) / (Ro * nums[i1] * mius[i0])));
            //alphas.push_back(max<double>(MathZer, (Ro * nums[i0] * mius[i1]) / (- Ro * mass[i1] + x[1] * M0)));
            alphas.push_back((-Ro * mass[i0] + x[0] * M0) / (Ro * nums[i1] * mius[i0]));
            alphas.push_back((Ro * nums[i0] * mius[i1]) / (-Ro * mass[i1] + x[1] * M0));

            //cout<<"Rmean "<<Ro<<" nums "<<nums<<"M0 "<<M0<<" mass "<<mass<<" i0 "<<i0<<" i1 "<<i1<<" miu "<<VecCl(mius)<<"xDenc "<<VecCl(x)<<"Alphas "<<VecCl(alphas);
            //vector<double> tmp(2);tmp[i0] = 1;
            //tmp[i1] = alphas[0];Matters->SetVolCoefs(tmp);cout<<"alpha0 "<<alphas[0]<<" result dencV[0] "<<Matters->ClcDencV();
            //tmp[i1] = alphas[1];Matters->SetVolCoefs(tmp);cout<<"alpha1 "<<alphas[1]<<" result dencV[1] "<<Matters->ClcDencV();

            return alphas;
        }
        double ClcAlphaLim(double &aMin, double &aMax) {
            vector<double> maxD = ClcAlphas(MaxDenc);
            vector<double> minD = ClcAlphas(MinDenc);

            int i0 = NumInt2NumExt[NumSrt2NumInt[0]],
                i1 = NumInt2NumExt[NumSrt2NumInt[1]];
            //int i0 = NumInt2NumExt[0], i1 = NumInt2NumExt[1];
            aMax = min<double>(maxD[i0], minD[i1]);
            if(minD[i1] < MathZer)
                aMax = maxD[i0];
            aMin = max<double>(maxD[i1], minD[i0]);
            //vector<double> lims = ClcN1N2Cage(Matters);
            //cout<<" LimN1N2 "<<VecCl(lims);
            //cout<<"Setting lims "<<aMin<<" "<<aMax<<"\n";
            if(!In_Lim(LastAlphaVal, aMin, aMax, 1))
                LastAlphaVal = (aMin + aMax) * 0.5;
            return LastAlphaVal;
        }
        void InitAddPressure(double aMin, double aMax, int addPolicy) {
            if(addPolicy == PolicyEnum.Int("Fixed")) {
                //AddPressure = StartAddPressure;
                return;
            }
            VecCl presMin = GetPressures(aMin), presMax = GetPressures(aMax);
            double minV = min(VecCl::Min(presMin), VecCl::Min(presMax));
            if(minV < M_Eps2)
                AddPressure = -(minV + 5) * 20;
            else
                AddPressure = 0;
        }
        vector<double> CagePresClc() {
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
        void SetStartParams(MatterCalculator *mats) {
            Matters = mats;
            Matters->SetVolCoefs((VecCl(2) * 0 + 1).Copy2Vector());
            Matters->ClcFreeE();
            NumInt2NumExt = Matters->MapNumInt2NumExt(Names);
            vector<double> pres = CagePresClc();   // Matters->GetResultPressures();
            int i0 = 0, i1 = 1;
            //if (pres[NumInt2NumExt[0]] > pres[NumInt2NumExt[1]])
            //    std::swap(i0, i1);
            NumSrt2NumInt.resize(2);
            NumSrt2NumInt[0] = i0;
            NumSrt2NumInt[1] = i1;
        }

        static double FindMisfitPressureStatic(double alphaVal) {
            return StaticPtr->FindMisfitPressure(alphaVal);
        }
        double FindMisfitPressure(double alphaVal) {
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
        vector<double> GetPressures(double alphaVal) {
            SetResult(alphaVal);
            Matters->ClcFreeE();
            //cout<<" alpha "<<alphaVal<<" denc "<<Matters->ClcDencV()<<" pres "<<Matters->GetResultPressures();
            return Matters->GetResultPressures();
        }
        double EncodeRes(const vector<double> &pres) {
            double minP = min(pres[0], pres[1]);
            if(minP + AddPressure < M_Eps2) {
                AddPressure = max<double>(0, -minP) * 2 + 1;
                cout << "Shifting AddPressure...\n";
            }
            return log((pres[0] + AddPressure) / (pres[1] + AddPressure));
        }
    };


    struct Equilibrium2NumberBase : EquilibriumBase {
        Equilibrium2NumberBase() : EquilibriumBase() {
            ClasterSizes = (VecCl(2) * 0 + 1).Copy2Vector();
            //MaxNumMisfit = M_Eps2*0.00001;
            MaxNumMisfit = M_Eps2;
            Names.push_back("Fst");
            Names.push_back("Sec");
        }

        int read_data_state(FilterIn &si) {
            char tmp[256];
            ClasterSizes.resize(2);
            Names.resize(2);
            si >> tmp;
            TestNextWord(si, "{", info_mark);
            for(size_t i = 0; i < 2; i++) {
                Stroka name;
                double clasterSize;
                si >> tmp >> name >> tmp >> clasterSize;
                Names[i] = name;
                ClasterSizes[i] = clasterSize;
            }
            TestNextWord(si, "}", info_mark);
            if(!si)
                throw info_except("Could not read class\n");
            LastLogX = 0;
            return 1;
        }
        int save_data_state(FilterOut &so) {
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
        Stroka MakeHelp() {
            Stroka res =
                "Class for finding numbers of dis atoms of 2 matters : Temp, MeanDenc, VolCoefs are given...";
            return res;
        }
        // To be called after FindEquilib
        double ClcFreeE(MatterCalculator *mats) {
            double fMix = VecCl(ClcFreeEMixt(mats)).SumElements();
            vector<double> nums = mats->NumCoefs;
            vector<double> fE = mats->GetResultFreeEPures();
            vector<double> M = mats->ClcMasV().Copy2Vector();
            double m0 = 0;
            double freeE = 0;
            for(size_t iClast = 0; iClast < NumInt2NumExt.size(); iClast++) {
                int iMat = NumInt2NumExt[iClast];
                m0 += M[iMat];
                freeE += nums[iMat] * fE[iMat] * mats->MolWeights[iMat];
            }
            return freeE / m0 + fMix;
        }
        // To be called after FindEquilib
        vector<double> ClcFreeEMixt(MatterCalculator *mats) {
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
            vector<double> fMix(
                2);   // = (VecCl((int)mats->Substs.size()) * 0).Copy2Vector();

            for(size_t iClast = 0; iClast < NumInt2NumExt.size(); iClast++) {
                int iMat = NumInt2NumExt[iClast];
                fMix[iMat] = nums[iMat] * mats->MolWeights[iMat] * kT *
                             log(vols[iMat] / v0) / ClasterSizes[iClast];
            }
            return (VecCl(fMix) / m0).Copy2Vector();
        }
        void SetPresMinim(Equilibrium2Pressure *presFinder) {
            PresFinder = presFinder;
        }
        void SetLastLogX(double logXguess) {
            LastLogX = logXguess;
        }
        double GetLastLogX() {
            return LastLogX;
        }
        vector<double> MakeLogXCage() {
            vector<double> n1n2Cage = PresFinder->ClcN1N2Cage(Matters);
            vector<double> ret(2);
            ret[0] = -30;
            ret[1] = 30;
            //return ret;
            if(VecCl::Min(VecCl(n1n2Cage)) < 0)
                throw info_except("Cannot calculate for dencity <%d>\n", Matters->Denc);
            if(n1n2Cage[0] != 0)
                ret[1] = log(n1n2Cage[0] / (1 - n1n2Cage[0]));
            if(n1n2Cage[1] != 1)
                ret[0] = log(n1n2Cage[1] / (1 - n1n2Cage[1]));
            //ChemEquation &eq = Matters->ChemEqs[0];
            if(Matters->ChemEqs[0].GetSubstCoef(0) < 0) {   // n1-n2 exchange
                double t = ret[0];
                ret[0] = -ret[1];
                ret[1] = -t;
            }

            return ret;
        }

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
        //        static double FindMisfitLogXStatic(double logX) {
        //            return StaticPtr->FindMisfitLogX(logX);
        //        }
        //        double FindMisfitLogX(double logX){
        //            SetMatterNumbers(logX);
        //            PresFinder->FindEquilib(Matters);
        //
        //            Matters->ClcFreeE();
        //            double kT = M_Rconst * Matters->Temp;
        //// UUUU
        //            double N0 = VecCl(Matters->NumCoefs).SumElements();
        //            vector<double> vols = Matters->ClcVolV().Copy2Vector(), dencs = Matters->ClcDencV().Copy2Vector();
        //            vector<double> fE = Matters->GetResultFreeEPures(), pres = Matters->GetResultPressures();
        //            //pres = (VecCl(pres) + VecCl(pres).SumElements() / pres.size()).Copy2Vector();
        //            double V0 = VecCl(vols).SumElements();
        //            ChemEquation &eq = Matters->ChemEqs[0];
        //            double mis = 0;
        //            for (size_t iClast = 0; iClast < eq.SubstCoefs.size(); iClast++) {
        //                size_t iMat = NumInt2NumExt[iClast];
        //                double miu = Matters->MolWeights[iMat];
        //                double clastRT = kT / ClasterSizes[iClast];
        //                mis += eq.SubstCoefs[iClast] * (clastRT + pres[iMat] * miu / dencs[iMat] -
        //                    clastRT * vols[iMat] * N0 / (V0 * Matters->NumCoefs[iMat]) + fE[iMat] * miu + clastRT * log(vols[iMat]/V0));
        //            }
        //            return mis;
        //        }

        void SetMatterNumbers(double logX) {
            double x = 1 / (1 + exp(-logX));
            double xm = 1 / (1 + exp(logX));
            ChemEquation &eq = Matters->ChemEqs[0];
            vector<double> numC =
                (VecCl((int)Matters->MolWeights.size()) * 0).Copy2Vector();

            //for(size_t iEq = 0; iEq < eq.SubstCoefs.size();iEq++)
            for(int iEq = 0; iEq < 2; iEq++)
                if(eq.GetSubstCoef(iEq) > 0)
                    numC[NumInt2NumExt[iEq]] = eq.GetSubstCoef(iEq) * xm;
                else
                    numC[NumInt2NumExt[iEq]] = -eq.GetSubstCoef(iEq) * x;
            Matters->NumCoefs = numC;
        }
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
        double FindMisfitLogX(double logX) {
            SetMatterNumbers(logX);
            PresFinder->FindEquilib(Matters);

            Matters->ClcFreeE();
            double kT = M_Rconst * Matters->Temp;
            // UUUU
            double N0 = VecCl(Matters->NumCoefs).SumElements();
            vector<double> vols = Matters->ClcVolV().Copy2Vector(),
                           dencs = Matters->ClcDencV().Copy2Vector();
            vector<double> fE = Matters->GetResultFreeEPures(),
                           pres = Matters->GetResultPressures();
            //pres = (VecCl(pres) + VecCl(pres).SumElements() / pres.size()).Copy2Vector();
            double V0 = VecCl(vols).SumElements();
            ChemEquation &eq = Matters->ChemEqs[0];
            double mis = 0;
            //for (size_t iClast = 0; iClast < eq.SubstCoefs.size(); iClast++) {
            for(int iClast = 0; iClast < 2; iClast++) {
                size_t iMat = NumInt2NumExt[iClast];
                double miu = Matters->MolWeights[iMat];
                double clastRT = kT / ClasterSizes[iClast];
                mis += eq.GetSubstCoef(iClast) *
                       (clastRT + pres[iMat] * miu / dencs[iMat] -
                        clastRT * vols[iMat] * N0 / (V0 * Matters->NumCoefs[iMat]) +
                        fE[iMat] * miu + clastRT * log(vols[iMat] / V0));
            }
            return mis;
        }
    };


    struct Equilibrium2NumberMin : Equilibrium2NumberBase {
        Equilibrium2NumberMin() : Equilibrium2NumberBase() {}

        void SetGuess(double xGuess) {}
        virtual void FindEquilib(MatterCalculator *mats) {
            double logXguess = SetStartParams(mats);
            double ErrorAbs = MaxNumMisfit, resFreeE;
            vector<double> clcCage = MakeLogXCage();
            //            int MaxIter = 100;
            //int err;
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
            //cout<<"logX "<<logX<<" fE "<<res<<"\n";
            return res;
        }
    };


    struct FreeEDisStdIO : FreeEIOFind {
        virtual vector<double> GetResultFreeECorrs() = 0;
        FreeEDisStdIO() : FreeEIOFind() {
            MattersClc = new MatterCalculator();
            SetInternalNames();
        };
        virtual void SetInternalNames() {
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
        virtual double OutputValClc(
            const map<Stroka, double> &Input,
            const Stroka &Output) {
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

        void ReadSubstancePar(istream &in) {
            MattersClc = new MatterCalculator;
            MattersClc->Read(in);
            SetInternalNames();
        }
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
            char tmp[256];
            in >> tmp;
            FilterTextIn fin(in.rdbuf(), 0);
            PresMinim.read_data_state(fin);
            fin >> tmp;
            NumMinim.read_data_state(fin);
            fin >> tmp;
            NumMinimMin.read_data_state(fin);
            VolCoefMisfit = M_Eps2;
            if(!in)
                throw info_except("Could not read class\n");
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
        double FreeE(double Dencity, double Temperature) {
            MattersClc->SetClc(Dencity, Temperature);
            NumMinim.SetPresMinim(&PresMinim);
            NumMinimMin.SetPresMinim(&PresMinim);
            cout << " denc " << MattersClc->Denc << " temp " << MattersClc->Temp << "\n";
            PresMinim.FindEquilib(MattersClc);
            NumMinim.FindEquilib(MattersClc);
            cout << " Zer lgX " << NumMinim.GetLastLogX() << " val "
                 << NumMinim.ClcFreeE(MattersClc) << "\n";
            //PresMinim.FindEquilib(MattersClc);
            NumMinimMin.SetGuess(NumMinim.GetLastLogX());
            NumMinimMin.FindEquilib(MattersClc);
            cout << " Min lgX " << NumMinimMin.GetLastLogX() << " val "
                 << NumMinimMin.ClcFreeE(MattersClc) << "\n";
            return NumMinimMin.ClcFreeE(MattersClc);
        }
        double FreeEvar(double Dencity, double Temperature) {
            MattersClc->SetClc(Dencity, Temperature);
            PresMinim.FindEquilib(MattersClc);
            NumMinim.SetPresMinim(&PresMinim);
            NumMinimMin.SetPresMinim(&PresMinim);

            NumMinim.FindEquilib(MattersClc);

            VecCl numCoefs(MattersClc->NumCoefs), numCoefsOld;
            numCoefs = numCoefs / numCoefs.SumElements();
            numCoefsOld = numCoefs;
            int numIter = 0, maxNumIter = 30;
            do {
                MattersClc->NumCoefs = (numCoefs * 0.5 + numCoefsOld * 0.5).Copy2Vector();
                numCoefs = VecCl(MattersClc->NumCoefs);

                numCoefsOld = numCoefs;
                NumMinim.FindEquilib(MattersClc);
                NumMinimMin.SetGuess(NumMinim.GetLastLogX());
                NumMinimMin.FindEquilib(MattersClc);

                numCoefs = VecCl(MattersClc->NumCoefs);
                numCoefs = numCoefs / numCoefs.SumElements();

            } while(VolCoefMisfit < VecCl::ModMax(numCoefsOld - numCoefs) &&
                    numIter++ < maxNumIter);
            if(numIter >= maxNumIter) {
                cout << " Bad result, numCoefsMisfit "
                     << VecCl::ModMax(numCoefsOld - numCoefs) << "\n";
                cout << " numCoefs " << numCoefs << "\n";
                cout << " numCoefsOld " << numCoefsOld << "\n";
            }
            return NumMinim.ClcFreeE(MattersClc);
        }
        double FreeEgood(double Dencity, double Temperature) {
            MattersClc->SetClc(Dencity, Temperature);
            PresMinim.FindEquilib(MattersClc);
            NumMinim.SetPresMinim(&PresMinim);
            NumMinim.FindEquilib(MattersClc);

            VecCl numCoefs(MattersClc->NumCoefs), numCoefsOld;
            numCoefs = numCoefs / numCoefs.SumElements();
            numCoefsOld = numCoefs;
            int numIter = 0, maxNumIter = 10;
            do {
                MattersClc->NumCoefs = (numCoefs * 0.5 + numCoefsOld * 0.5).Copy2Vector();
                numCoefs = VecCl(MattersClc->NumCoefs);

                numCoefsOld = numCoefs;
                NumMinim.FindEquilib(MattersClc);

                numCoefs = VecCl(MattersClc->NumCoefs);
                numCoefs = numCoefs / numCoefs.SumElements();

            } while(VolCoefMisfit < VecCl::ModMax(numCoefsOld - numCoefs) &&
                    numIter++ < maxNumIter);
            //PresMinim.FindEquilib(MattersClc);
            if(numIter >= maxNumIter) {
                cout << " Bad result, numCoefsMisfit "
                     << VecCl::ModMax(numCoefsOld - numCoefs) << "\n";
                cout << " numCoefs " << numCoefs << "\n";
                cout << " numCoefsOld " << numCoefsOld << "\n";
            }
            //cout<<" numIter "<<numIter<<" res "<<VecCl::ModMax(volCoefsOld - volCoefs)<<"\n";
            //NumMinim.FindEquilib(MattersClc);
            return NumMinim.ClcFreeE(MattersClc);
        }
        double FreeE1(double Dencity, double Temperature) {
            MattersClc->SetClc(Dencity, Temperature);
            PresMinim.FindEquilib(MattersClc);
            NumMinim.SetPresMinim(&PresMinim);
            VecCl volCoefs = MattersClc->ClcVolV(), volCoefsOld;
            volCoefs = volCoefs / volCoefs.SumElements();
            VecCl numCoefs(MattersClc->NumCoefs), numCoefsOld;
            numCoefs = numCoefs / numCoefs.SumElements();
            int numIter = 0;
            do {
                volCoefsOld = volCoefs;
                numCoefsOld = numCoefs;

                NumMinim.FindEquilib(MattersClc);

                numCoefs = VecCl(MattersClc->NumCoefs);
                numCoefs = numCoefs / numCoefs.SumElements();
                MattersClc->NumCoefs = (numCoefs * 0.5 + numCoefsOld * 0.5).Copy2Vector();
                numCoefs = VecCl(MattersClc->NumCoefs);

                PresMinim.FindEquilib(MattersClc);

                volCoefs = MattersClc->ClcVolV();
                volCoefs = volCoefs / volCoefs.SumElements();
                //cout<<VecCl::ModMax(volCoefsOld - volCoefs)<<"\nvC "<<volCoefs<<" vCold "<<volCoefsOld<<" numC "<< numCoefs<<" numCold "<<numCoefsOld;
            } while(VolCoefMisfit < VecCl::ModMax(volCoefsOld - volCoefs) &&
                    numIter++ < 30);
            if(numIter >= 100) {
                cout << " Bad result, volCoefsMisfit "
                     << VecCl::ModMax(volCoefsOld - volCoefs) << "\n";
                cout << " volCoefs " << volCoefs << "\n";
                cout << " volCoefsOld " << volCoefsOld << "\n";
                cout << " numCoefs " << numCoefs << "\n";
                cout << " numCoefsOld " << numCoefsOld << "\n";
            }
            //cout<<" numIter "<<numIter<<" res "<<VecCl::ModMax(volCoefsOld - volCoefs)<<"\n";
            NumMinim.FindEquilib(MattersClc);
            return NumMinim.ClcFreeE(MattersClc);
        }

    private:
        Equilibrium2Pressure PresMinim;
        Equilibrium2Number NumMinim;
        Equilibrium2NumberMin NumMinimMin;
        double VolCoefMisfit;
    };


}   // ClcDissociation


//    struct Equilibrium2Pressure : EquilibriumBase {
//
//        Equilibrium2Pressure() : EquilibriumBase(), PolicyEnum("Std Fixed") {
//            MinDenc = (VecCl(2) * 0 + 1e-4).Copy2Vector();
//            MaxDenc = (VecCl(2) * 0 + 1).Copy2Vector();
//            MinVolCoef = (VecCl(2) * 0 + 1e-4).Copy2Vector();
//            MaxVolCoef = (VecCl(2) * 0 + 1e4).Copy2Vector();
//            FixedVolumeCoefs = 0;
//            MaxPresMisfit = M_Eps2*0.01;
//            Names.push_back("Fst");
//            Names.push_back("Sec");
//            AddPolicy = 0;
//            AlphaMullCoef = 1;
//            StartAddPressure = 1e6;
//        }
//
//        int read_data_state(FilterIn &si){
//            char tmp[256];
//            Names.clear(); MinDenc.clear();MaxDenc.clear(); MinVolCoef.clear(); MaxVolCoef.clear();
//            si>>tmp>>FixedVolumeCoefs>>tmp>>AddPolicy>>tmp>>AlphaMullCoef>>tmp>>MaxPresMisfit>>tmp>>StartAddPressure>>tmp;
//            //si>>PresDerivStp>>tmp;
//            TestNextWord(si, "{", info_mark);
//            for (size_t i = 0; i < 2; i++) {
//                Stroka name;
//                double minDenc, maxDenc, minVolC, maxVolC;
//                si>>tmp>>name>>tmp>>minDenc>>tmp>>maxDenc>>tmp>>minVolC>>tmp>>maxVolC;
//                Names.push_back(name);
//                MinDenc.push_back(minDenc);MaxDenc.push_back(maxDenc); MinVolCoef.push_back(minVolC); MaxVolCoef.push_back(maxVolC);
//            }
//            TestNextWord(si, "}", info_mark);
//            if (!si)
//                throw info_except("Could not read class\n");
//            LastAlphaVal = 0;
//            AddPressure = StartAddPressure;
//            return 1;
//        }
//        int save_data_state(FilterOut &so){
//            so<<" FixedVolumeCoef "<<FixedVolumeCoefs<<" AddPolicy "<<AddPolicy<<" AlphaMullCoef "<<AlphaMullCoef;
//            so<<" MaxPresMisfit "<<MaxPresMisfit<<" StartAddPressure "<<StartAddPressure;//<<" PresDerivStp "<<PresDerivStp;
//            so<<" PressureMinParams {";
//            SavableClass::IncIdent();
//            for (size_t i = 0; i < 2; i++)
//                so<<SavableClass::EOLN()<<" Matter "<<Names[i] << " MinDenc "<<MinDenc[i]<<" MaxDenc "<<MaxDenc[i]<<" MinVolCoef "<<MinVolCoef[i]<<" MaxVolCoef "<<MaxVolCoef[i];
//            SavableClass::DecIdent();
//            so<<EOLN()<<"}"<<SavableClass::EOLN();
//            if (!so)
//                throw info_except("Could not save class\n");
//            return 1;
//        }
//        Stroka MakeHelp(){
//            Stroka res = "Class for finding volume coefs of 2 matters : Temp, MeanDenc is given... Policy may be " +
//                PolicyEnum.ListOptions();
//            return res;
//        }
//        virtual void FindEquilib(MatterCalculator *mats) {
//            double alphaGuess = SetStartParams(mats);
//            double alphaMin, alphaMax;
//            if (!ClcAlphaLim(alphaMin, alphaMax)) {
//                SetResult(0);
//                return ;
//            }
//            if (FixedVolumeCoefs) {
//                vector<double> volCoefs;
//                volCoefs.push_back(mats->Substs[0].StartVolumeCoef);
//                volCoefs.push_back(mats->Substs[1].StartVolumeCoef);
//                Matters->SetVolCoefs((VecCl(volCoefs) / VecCl(volCoefs).SumElements()).Copy2Vector());
//                return ;
//            }
//            InitAddPressure(alphaMin, alphaMax, AddPolicy);
//            FindEquilibFixed(mats, alphaGuess, alphaMin, alphaMax);
//            if (AddPolicy == PolicyEnum.Int("Std"))
//                return ;
//            double maxAddP = AddPressure;
//            alphaMin = min(LastPosAlpha, LastNegAlpha);
//            alphaMax = max<double>(LastPosAlpha, LastNegAlpha);
//            InitAddPressure(alphaMin, alphaMax, PolicyEnum.Int("Std"));
//            FindEquilibFixed(mats, alphaGuess, alphaMin, alphaMax);
//            AddPressure = maxAddP;
//        }
//    private:
//
//
//        vector<double> MinDenc, MaxDenc, MinVolCoef, MaxVolCoef;
//        int FixedVolumeCoefs;
//        double MaxPresMisfit;
//
//        double AlphaMullCoef, StartAddPressure;
//        int AddPolicy;
//        Str::OptionSet PolicyEnum;
//
//        MatterCalculator *Matters;
//        vector<Stroka> Names;
//        vector<int> NumInt2NumExt;
//        //vector<int>  NumSrt2NumInt;
//        static Equilibrium2Pressure *StaticPtr;
//
//        double LastAlphaVal, AddPressure;
//        double LastPosAlpha, LastNegAlpha;
//        double LastPosMisfit, LastNegMisfit;
//        //vector<double> NormalMasses;
//
//        virtual void FindEquilibFixed(MatterCalculator *mats, double &alphaGuess, double alphaMin, double alphaMax) {
//            double ErrorAbs = MaxPresMisfit, ErrorRel = MaxPresMisfit;
//            int MaxIter = 100;
//            int err;
//            if (!In_Lim(alphaGuess, alphaMin, alphaMax, 0))
//                alphaGuess = 0.5 * (alphaMin + alphaMax);
//            LastPosMisfit = LastNegMisfit = 1/MathZer;
//            Equilibrium2Pressure *OldPtr = StaticPtr;
//		    StaticPtr = this;
//            if (4 < (err = Fzero(FindMisfitPressureStatic, alphaMin , alphaMax, alphaGuess, ErrorAbs, ErrorRel, MaxIter))) {
//                cout<<" Bad FZero Equilibrium2Pressure \nfound X "
//                    <<alphaGuess<<" Mis "<<FindMisfitPressure(alphaGuess)<<" err "<<err<<"\n";
//            }
//            StaticPtr = OldPtr;
////vector<double> p = GetPressures(alphaGuess); cout<<" found "<<alphaGuess<<" mis "<<fabs(p[0]-p[1])<<" pres "<<p;
//            SetResult(alphaGuess);
//        }
//        //void SetResult(double alphaVal) {
//        //    vector<double> volC(2);
//        //    volC[NumInt2NumExt[NumSrt2NumInt[0]]] = 1;
//        //    volC[NumInt2NumExt[NumSrt2NumInt[1]]] = alphaVal;
//        //    Matters->SetVolCoefs(volC);
//        //    LastAlphaVal = alphaVal;
//        //}
//
//
//
//        void InitAddPressure(double aMin, double aMax, int addPolicy) {
//            if (addPolicy == PolicyEnum.Int("Fixed")) {
//                return;
//            }
//            VecCl presMin = GetPressures(aMin), presMax = GetPressures(aMax);
//            double minV = min(VecCl::Min(presMin), VecCl::Min(presMax));
//            if (minV < M_Eps2)
//                AddPressure = - (minV + 5) * 20;
//            else
//                AddPressure = 0;
//        }
//        void SetResult(double alphaVal) {
//            vector<double> volC(2);
//            volC[NumInt2NumExt[0]] = 1. / (Matters->NumCoefs[0] * (1 + 1/alphaVal));
//            volC[NumInt2NumExt[1]] = 1. / (Matters->NumCoefs[1] * (1 + alphaVal));
//            //volC[NumInt2NumExt[0]] = 1. / (Matters->NumCoefs[0] * (1 + exp(-alphaVal)));
//            //volC[NumInt2NumExt[1]] = 1. / (Matters->NumCoefs[1] * (1 + exp(alphaVal)));
//            Matters->SetVolCoefs(volC);
//            LastAlphaVal = alphaVal;
////cout<<" alpha "<<alphaVal<<" denc "<<Matters->ClcDencV();
//        }
//        int ClcAlphaLim(double &aMin, double &aMax) {
//            vector<double> masRaw = Matters->ClcMasV().Copy2Vector();
//            double constC = VecCl(masRaw).SumElements() / Matters->Denc;
//            int iExt0 = NumInt2NumExt[0], iExt1 = NumInt2NumExt[1];
//            vector<double> masNorm = (VecCl(masRaw) / constC).Copy2Vector();
//            double aMax0 = min(masNorm[0] / MinDenc[iExt0], 1 - masNorm[1] / MinDenc[iExt1]);
//            double aMax1 = max<double>(1 - masNorm[0] / MinDenc[iExt0], masNorm[1] / MinDenc[iExt1]);
//            double aMin0 = max<double>(masNorm[0] / MaxDenc[iExt0], 1 - masNorm[1] / MaxDenc[iExt1]);
//            double aMin1 = min(1 - masNorm[0] / MaxDenc[iExt0], masNorm[1] / MaxDenc[iExt1]);
//            aMax = max<double>(aMax0, MathZer) / min(aMax1, 1);
//            aMin = min(aMin0, 1) / max<double>(aMin1, MathZer);
//            //aMax = log(max<double>(aMax0, MathZer) / max<double>(1 - aMax0, MathZer));
//            //aMin = log(max<double>(aMin0, MathZer) / max<double>(1 - aMin0, MathZer));
//            //aMax = log(max<double>(aMax0, MathZer) / min(aMax1, 1));
//            //aMin = log(min(aMin0, 1) / max<double>(aMin1, MathZer));
//            if (!In_Lim(LastAlphaVal, aMin, aMax, 0))
//                LastAlphaVal = 0.5 * (aMin + aMax);
////            if (aMin >= aMax) {
////double aMax00 = masNorm[0] / MinDenc[iExt0], aMax01 = 1 - masNorm[1] / MinDenc[iExt1];
////double aMax10 = 1 - masNorm[0] / MinDenc[iExt0], aMax11 = masNorm[1] / MinDenc[iExt1];
////double aMin00 = masNorm[0] / MaxDenc[iExt0], aMin01 = 1 - masNorm[1] / MaxDenc[iExt1];
////double aMin10 = 1 - masNorm[0] / MaxDenc[iExt0], aMin11 = masNorm[1] / MaxDenc[iExt1];
////    SetResult(log(max<double>(aMax00, MathZer) / max<double>(aMax10, MathZer)));
////    cout<<" aMax0 "<<log(max<double>(aMax00, MathZer) / max<double>(aMax10, MathZer))<<" Denc[aMax0] "<<Matters->ClcDencV();
////    SetResult(log(max<double>(aMax01, MathZer) / max<double>(aMax11, MathZer)));
////    cout<<" aMax1 "<<log(max<double>(aMax01, MathZer) / min(aMax11, 1))<<" Denc[aMax1] "<<Matters->ClcDencV();
////    SetResult(log(max<double>(aMin00, MathZer) / max<double>(aMin10, MathZer)));
////    cout<<" aMin0 "<<log(min(aMin00, 1) / max<double>(aMin10, MathZer))<<" Denc[aMin0] "<<Matters->ClcDencV();
////    SetResult(log(max<double>(aMin01, MathZer) / max<double>(aMin11, MathZer)));
////    cout<<" aMin1 "<<log(min(aMin01, 1) / max<double>(aMin11, MathZer))<<" Denc[aMin1] "<<Matters->ClcDencV();
////cout<<"Incompatible dencities - cannot satisfy them...\n";
////return 1;
////            }
//            return 1;
//        }
//        double SetStartParams(MatterCalculator *mats) {
//            Matters = mats;
//            Matters->SetVolCoefs((VecCl(2) * 0 + 1).Copy2Vector());
//            Matters->ClcFreeE();
//            vector<double> pres = Matters->GetResultPressures();
//            NumInt2NumExt = Matters->MapNumInt2NumExt(Names);
//            int i0 = 0, i1 = 1;
//            if (pres[NumInt2NumExt[0]] > pres[NumInt2NumExt[1]])
//                std::swap(i0, i1);
//            //NumSrt2NumInt.resize(2);
//            //NumSrt2NumInt[0] = i0;
//            //NumSrt2NumInt[1] = i1;
//
//            return LastAlphaVal;
//        }
//
//        static double FindMisfitPressureStatic(double alphaVal) {
//            return StaticPtr->FindMisfitPressure(alphaVal);
//        }
//        double FindMisfitPressure(double alphaVal){
//            vector<double> pres = GetPressures(alphaVal);
//            double res = EncodeRes(pres);
//            if (res > 0 && res < LastPosMisfit) {
//                LastPosMisfit = res;
//                LastPosAlpha = alphaVal;
//            }
//            if (res < 0 && fabs(res) < LastNegMisfit) {
//                LastNegMisfit = fabs(res);
//                LastNegAlpha = alphaVal;
//            }
//
//            return res;
//        }
//        int InLim(double Val,double from,double to) {
//            if (fabs(Val - from) < M_Eps2 * fabs(from) || fabs(Val - to) < M_Eps2 * fabs(to))
//                return 1;
//            if ( (Val > to) || (Val < from) )
//                return 0;
//            return 1;
//          };
//        vector<double> GetPressures(double alphaVal){
//            SetResult(alphaVal);
//            Matters->ClcFreeE();
//VecCl denc = Matters->ClcDencV();if (!InLim(denc[1], MinDenc[0], MaxDenc[0]) || !InLim(denc[2], MinDenc[1], MaxDenc[1])) {
//    cout<<" bad alhpa "<<alphaVal<<" dencVector "<<denc<<" MinDenc "<<MinDenc<<" MaxDenc "<<MaxDenc;
//    double aMin, aMax;
//    ClcAlphaLim(aMin, aMax);
//    cout<<" aMin "<< aMin<<" aMax "<<aMax<<"\n";
//    SetResult(aMin);
//    cout<<" Denc[aMin] "<<Matters->ClcDencV();
//    SetResult(aMax);
//    cout<<" Denc[aMax] "<<Matters->ClcDencV();
//
//
//vector<double> masRaw = Matters->ClcMasV().Copy2Vector();
//double constC = VecCl(masRaw).SumElements() / Matters->Denc;
//int iExt0 = NumInt2NumExt[0], iExt1 = NumInt2NumExt[1];
//vector<double> masNorm = (VecCl(masRaw) / constC).Copy2Vector();
//double aMax00 = masNorm[0] / MinDenc[iExt0], aMax01 = 1 - masNorm[1] / MinDenc[iExt1];
//double aMax10 = 1 - masNorm[0] / MinDenc[iExt0], aMax11 = masNorm[1] / MinDenc[iExt1];
//double aMin00 = masNorm[0] / MaxDenc[iExt0], aMin01 = 1 - masNorm[1] / MaxDenc[iExt1];
//double aMin10 = 1 - masNorm[0] / MaxDenc[iExt0], aMin11 = masNorm[1] / MaxDenc[iExt1];
//    SetResult(log(max<double>(aMax00, MathZer) / min(aMax10, 1)));
//    cout<<" aMax0 "<<log(max<double>(aMax00, MathZer) / min(aMax10, 1))<<" Denc[aMax0] "<<Matters->ClcDencV();
//    SetResult(log(max<double>(aMax01, MathZer) / min(aMax11, 1)));
//    cout<<" aMax1 "<<log(max<double>(aMax01, MathZer) / min(aMax11, 1))<<" Denc[aMax1] "<<Matters->ClcDencV();
//    SetResult(log(min(aMin00, 1) / max<double>(aMin10, MathZer)));
//    cout<<" aMin0 "<<log(min(aMin00, 1) / max<double>(aMin10, MathZer))<<" Denc[aMin0] "<<Matters->ClcDencV();
//    SetResult(log(min(aMin01, 1) / max<double>(aMin11, MathZer)));
//    cout<<" aMin1 "<<log(min(aMin01, 1) / max<double>(aMin11, MathZer))<<" Denc[aMin1] "<<Matters->ClcDencV();
//
//            //aMax = log(max<double>(aMax0, MathZer) / min(aMax1, 1));
//            //aMin = log(min(aMin0, 1) / max<double>(aMin1, MathZer));
//
//}
//            return Matters->GetResultPressures();
//        }
//        double EncodeRes(const vector<double> &pres) {
//            double minP = min(pres[0], pres[1]);
//            if (minP + AddPressure < M_Eps2) {
//                AddPressure = max<double>(0, -minP) * 2 + 1;
//cout<<"Shifting AddPressure...\n";
//            }
//            return log((pres[0] + AddPressure)/(pres[1] + AddPressure));
//        }
//
//
//
//
//
//
//
//
//
////        vector<double> ClcDencity(double alpha) {
////            SetResult(alpha);
////            return Matters->ClcDencV().Copy2Vector();
////
////            vector<double> denc(2);
////            denc[0] = NormalMasses[0] * (1 + exp(-alpha / AlphaMullCoef));
////            denc[1] = NormalMasses[1] * (1 + exp(alpha / AlphaMullCoef));
////            return denc;
////        }
////        void InitAddPressure(double aMin, double aMax, int addPolicy) {
////            if (addPolicy == PolicyEnum.Int("Fixed"))
////                return;
////            VecCl presMin = GetPressures(aMin), presMax = GetPressures(aMax);
////            double minV = min(VecCl::Min(presMin), VecCl::Min(presMax));
////            if (minV < M_Eps2)
////                AddPressure = - (minV + 5) * 20;
////            else
////                AddPressure = 0;
////        }
////        double SetStartParams(MatterCalculator *mats) {
////            Matters = mats;
////            MatterInd.resize(2);
////            MatterInd[0] = Matters->NameInt[Name2Int.Num2Str(0)];
////            MatterInd[1] = Matters->NameInt[Name2Int.Num2Str(1)];
////            return LastAlphaVal;
////        }
////
////        static double FindMisfitPressureStatic(double alphaVal) {
////            return StaticPtr->FindMisfitPressure(alphaVal);
////        }
////        double FindMisfitPressure(double alphaVal){
////            vector<double> pres = GetPressures(alphaVal);
////            double res = EncodeRes(pres);
////            if (res > 0 && res < LastPosMisfit) {
////                LastPosMisfit = res;
////                LastPosAlpha = alphaVal;
////            }
////            if (res < 0 && fabs(res) < LastNegMisfit) {
////                LastNegMisfit = fabs(res);
////                LastNegAlpha = alphaVal;
////            }
////
////            return res;
////        }
////        vector<double> GetPressures(double alphaVal){
////            vector<double> denc = ClcDencity(alphaVal);
////		    vector<double> pres;
////            for (size_t i = 0; i < denc.size(); i++)
////                pres.push_back(Matters->Substs[MatterInd[i]].ClcPres(denc[i], Matters->Temp));
//////cout<<" alpha "<<alphaVal<<" denc "<<VecCl(denc)<<" pres "<<VecCl(pres);
////            return pres;
////        }
////        double EncodeRes(const vector<double> &pres) {
////            double minP = min(pres[0], pres[1]);
////            if (minP < M_Eps2) {
////                AddPressure = -minP * 1.1 + 1;
////cout<<"Shifting AddPressure...\n";
////            }
//////cout<<" misfit "<<log((pres[0] + AddPressure)/(pres[1] + AddPressure))<<"\n";
////            return log((pres[0] + AddPressure)/(pres[1] + AddPressure));
////        }
////        void SetResult(double alphaVal) {
////            vector<double> volC;
////            //volC.push_back(1. / (Matters->NumCoefs[MatterInd[0]] * (1 + exp(-alphaVal / AlphaMullCoef))));
////            //volC.push_back(1. / (Matters->NumCoefs[MatterInd[1]] * (1 + exp(alphaVal / AlphaMullCoef))));
////            volC.push_back(1. / (Matters->NumCoefs[MatterInd[0]] * (1 + 1/alphaVal)));
////            volC.push_back(1. / (Matters->NumCoefs[MatterInd[1]] * (1 + alphaVal)));
////            Matters->SetVolCoefs(volC);
////            LastAlphaVal = alphaVal;
////        }
//    };
//
//
//
//
//


//    struct Equilibrium2Pressure : EquilibriumBase {
//
//        Equilibrium2Pressure() : EquilibriumBase(), PolicyEnum("Std Fixed") {
//            MinDenc = (VecCl(2) * 0 + 1e-4).Copy2Vector();
//            MaxDenc = (VecCl(2) * 0 + 1).Copy2Vector();
//            MinVolCoef = (VecCl(2) * 0 + 1e-4).Copy2Vector();
//            MaxVolCoef = (VecCl(2) * 0 + 1e4).Copy2Vector();
//            FixedVolumeCoefs = 0;
//            MaxPresMisfit = M_Eps2*0.01;
//            Name2Int.Str2Num("Fst", 1);
//            Name2Int.Str2Num("Sec", 1);
//            AddPolicy = 0;
//            AlphaMullCoef = 1;
//            StartAddPressure = 1e6;
//        }
//
//        int read_data_state(FilterIn &si){
//            char tmp[256];
//            Name2Int.Clear(); MinDenc.clear();MaxDenc.clear(); MinVolCoef.clear(); MaxVolCoef.clear();
//            si>>tmp>>FixedVolumeCoefs>>tmp>>AddPolicy>>tmp>>AlphaMullCoef>>tmp>>MaxPresMisfit>>tmp>>StartAddPressure>>tmp;
//            TestNextWord(si, "{", info_mark);
//            for (size_t i = 0; i < 2; i++) {
//                Stroka name;
//                double minDenc, maxDenc, minVolC, maxVolC;
//                si>>tmp>>name>>tmp>>minDenc>>tmp>>maxDenc>>tmp>>minVolC>>tmp>>maxVolC;
//                Name2Int.Str2Num(name.c_str(), 1);
//                MinDenc.push_back(minDenc);MaxDenc.push_back(maxDenc); MinVolCoef.push_back(minVolC); MaxVolCoef.push_back(maxVolC);
//            }
//            TestNextWord(si, "}", info_mark);
//            if (!si)
//                throw info_except("Could not read class\n");
//            LastAlphaVal = 0;
//            AddPressure = StartAddPressure;
//            return 1;
//        }
//        int save_data_state(FilterOut &so){
//            so<<" FixedVolumeCoef "<<FixedVolumeCoefs<<" AddPolicy "<<AddPolicy<<" AlphaMullCoef "<<AlphaMullCoef;
//            so<<" MaxPresMisfit "<<MaxPresMisfit<<" StartAddPressure "<<StartAddPressure<<" PressureMinParams {";
//            SavableClass::IncIdent();
//            for (size_t i = 0; i < 2; i++)
//                so<<SavableClass::EOLN()<<" Matter "<<Name2Int.Num2Str((int)i) << " MinDenc "<<MinDenc[i]<<" MaxDenc "<<MaxDenc[i]<<" MinVolCoef "<<MinVolCoef[i]<<" MaxVolCoef "<<MaxVolCoef[i];
//            SavableClass::DecIdent();
//            so<<EOLN()<<"}"<<SavableClass::EOLN();
//            if (!so)
//                throw info_except("Could not save class\n");
//            return 1;
//        }
//        Stroka MakeHelp(){
//            Stroka res = "Class for finding volume coefs of 2 matters : Temp, MeanDenc is given... Policy may be " +
//                PolicyEnum.ListOptions();
//            return res;
//        }
//        virtual void FindEquilib(MatterCalculator *mats) {
//            double alphaGuess = SetStartParams(mats);
//            double alphaMin, alphaMax;
//            ClcAlphaLim(alphaMin, alphaMax);
//            if (FixedVolumeCoefs) {
//                vector<double> volCoefs;
//                volCoefs.push_back(mats->Substs[0].StartVolumeCoef);
//                volCoefs.push_back(mats->Substs[1].StartVolumeCoef);
//                Matters->SetVolCoefs((VecCl(volCoefs) / VecCl(volCoefs).SumElements()).Copy2Vector());
//                return ;
//            }
//            InitAddPressure(alphaMin, alphaMax, AddPolicy);
//            FindEquilibFixed(mats, alphaGuess, alphaMin, alphaMax);
//            if (AddPolicy == PolicyEnum.Int("Std"))
//                return ;
//            double maxAddP = AddPressure;
//            alphaMin = min(LastPosAlpha, LastNegAlpha);
//            alphaMax = max<double>(LastPosAlpha, LastNegAlpha);
//            InitAddPressure(alphaMin, alphaMax, PolicyEnum.Int("Std"));
//            FindEquilibFixed(mats, alphaGuess, alphaMin, alphaMax);
//            AddPressure = maxAddP;
//        }
//    private:
//
//
//        vector<double> MinDenc, MaxDenc, MinVolCoef, MaxVolCoef;
//        int FixedVolumeCoefs;
//        double MaxPresMisfit;
//
//        double AlphaMullCoef, StartAddPressure;
//        int AddPolicy;
//        Str::OptionSet PolicyEnum;
//
//        MatterCalculator *Matters;
//        Str::StrokaStorage Name2Int;
//        vector<int> MatterInd;
//        static Equilibrium2Pressure *StaticPtr;
//
//        double LastAlphaVal, AddPressure;
//        double LastPosAlpha, LastNegAlpha;
//        double LastPosMisfit, LastNegMisfit;
//        vector<double> NormalMasses;
//
//        virtual void FindEquilibFixed(MatterCalculator *mats, double &alphaGuess, double alphaMin, double alphaMax) {
//            double ErrorAbs = MaxPresMisfit, ErrorRel = MaxPresMisfit;
//            int MaxIter = 100;
//            int err;
//            LastPosMisfit = LastNegMisfit = 1/MathZer;
//            Equilibrium2Pressure *OldPtr = StaticPtr;
//		    StaticPtr = this;
////cout<<"min "<<alphaMin<<" max "<<alphaMax<<" guess "<<alphaGuess<<" err "<<ErrorAbs<<"\n";
//            if (4 < (err = Fzero(FindMisfitPressureStatic, alphaMin , alphaMax, alphaGuess, ErrorAbs, ErrorRel, MaxIter))) {
//                cout<<" Bad FZero Equilibrium2Pressure \nfound X "
//                    <<alphaGuess<<" Mis "<<FindMisfitPressure(alphaGuess)<<" err "<<err<<"\n";
//            }
//            StaticPtr = OldPtr;
//            SetResult(alphaGuess);
//        }
//
//        void ClcAlphaLim(double &aMin, double &aMax) {
//            NormalMasses.clear();
//            vector<double> masRaw = Matters->ClcMasV().Copy2Vector();
//            NormalMasses.push_back(max<double>(MathZer, masRaw[MatterInd[0]]));
//            NormalMasses.push_back(max<double>(MathZer, masRaw[MatterInd[1]]));
//            double constC = VecCl(NormalMasses).SumElements() / Matters->Denc;
//            Matters->MulCoef = constC;
//            NormalMasses = (VecCl(NormalMasses) / constC).Copy2Vector();
//            double aMax0 = min(NormalMasses[0] / MinDenc[0], 1 - NormalMasses[1] / MinDenc[1]);
//            double aMax1 = max<double>(1 - NormalMasses[0] / MinDenc[0], NormalMasses[1] / MinDenc[1]);
//            double aMin0 = max<double>(NormalMasses[0] / MaxDenc[0], 1 - NormalMasses[1] / MaxDenc[1]);
//            double aMin1 = min(1 - NormalMasses[0] / MaxDenc[0], NormalMasses[1] / MaxDenc[1]);
//            //aMax = log(max<double>(aMax0, MathZer) / min(aMax1, 1));
//            //aMin = log(min(aMin0, 1) / max<double>(aMin1, MathZer));
//            //aMin *= AlphaMullCoef;
//            //aMax *= AlphaMullCoef;
//            aMax = max<double>(aMax0, MathZer) / min(aMax1, 1);
//            aMin = min(aMin0, 1) / max<double>(aMin1, MathZer);
//            if (!In_Lim(LastAlphaVal, aMin, aMax, 0))
//                LastAlphaVal = 0.5 * (aMin + aMax);
//        }
//        vector<double> ClcDencity(double alpha) {
//            SetResult(alpha);
//            return Matters->ClcDencV().Copy2Vector();
//
//            vector<double> denc(2);
//            denc[0] = NormalMasses[0] * (1 + exp(-alpha / AlphaMullCoef));
//            denc[1] = NormalMasses[1] * (1 + exp(alpha / AlphaMullCoef));
//            return denc;
//        }
//        void InitAddPressure(double aMin, double aMax, int addPolicy) {
//            if (addPolicy == PolicyEnum.Int("Fixed"))
//                return;
//            VecCl presMin = GetPressures(aMin), presMax = GetPressures(aMax);
//            double minV = min(VecCl::Min(presMin), VecCl::Min(presMax));
//            if (minV < M_Eps2)
//                AddPressure = - (minV + 5) * 20;
//            else
//                AddPressure = 0;
//        }
//        double SetStartParams(MatterCalculator *mats) {
//            Matters = mats;
//            MatterInd.resize(2);
//            MatterInd[0] = Matters->NameInt[Name2Int.Num2Str(0)];
//            MatterInd[1] = Matters->NameInt[Name2Int.Num2Str(1)];
//            return LastAlphaVal;
//        }
//
//        static double FindMisfitPressureStatic(double alphaVal) {
//            return StaticPtr->FindMisfitPressure(alphaVal);
//        }
//        double FindMisfitPressure(double alphaVal){
//            vector<double> pres = GetPressures(alphaVal);
//            double res = EncodeRes(pres);
//            if (res > 0 && res < LastPosMisfit) {
//                LastPosMisfit = res;
//                LastPosAlpha = alphaVal;
//            }
//            if (res < 0 && fabs(res) < LastNegMisfit) {
//                LastNegMisfit = fabs(res);
//                LastNegAlpha = alphaVal;
//            }
//
//            return res;
//        }
//        vector<double> GetPressures(double alphaVal){
//            vector<double> denc = ClcDencity(alphaVal);
//		    vector<double> pres;
//            for (size_t i = 0; i < denc.size(); i++)
//                pres.push_back(Matters->Substs[MatterInd[i]].ClcPres(denc[i], Matters->Temp));
////cout<<" alpha "<<alphaVal<<" denc "<<VecCl(denc)<<" pres "<<VecCl(pres);
//            return pres;
//        }
//        double EncodeRes(const vector<double> &pres) {
//            double minP = min(pres[0], pres[1]);
//            if (minP < M_Eps2) {
//                AddPressure = -minP * 1.1 + 1;
//cout<<"Shifting AddPressure...\n";
//            }
////cout<<" misfit "<<log((pres[0] + AddPressure)/(pres[1] + AddPressure))<<"\n";
//            return log((pres[0] + AddPressure)/(pres[1] + AddPressure));
//        }
//        void SetResult(double alphaVal) {
//            vector<double> volC;
//            //volC.push_back(1. / (Matters->NumCoefs[MatterInd[0]] * (1 + exp(-alphaVal / AlphaMullCoef))));
//            //volC.push_back(1. / (Matters->NumCoefs[MatterInd[1]] * (1 + exp(alphaVal / AlphaMullCoef))));
//            volC.push_back(1. / (Matters->NumCoefs[MatterInd[0]] * (1 + 1/alphaVal)));
//            volC.push_back(1. / (Matters->NumCoefs[MatterInd[1]] * (1 + alphaVal)));
//            Matters->SetVolCoefs(volC);
//            LastAlphaVal = alphaVal;
//        }
//    };
//
