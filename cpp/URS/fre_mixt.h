#pragma once

#include "fre_dis2.h"


namespace ClcMixture {

    struct MixtureFreeEClc : SavableClass {
        MixtureFreeEClc() : SavableClass() {
            Names.push_back("Fst");
            Names.push_back("Sec");
            Masses.push_back(0.5);
            Masses.push_back(0.5);
        }

        int read_data_state(FilterIn &si) {
            char tmp[256];
            Names.clear();
            Masses.clear();
            si >> tmp;
            TestNextWord(si, "{", info_mark);
            for(size_t i = 0; i < 2; i++) {
                Stroka name;
                double mass;
                si >> tmp >> name >> tmp >> mass;
                Names.push_back(name);
                Masses.push_back(mass);
            }
            TestNextWord(si, "}", info_mark);
            if(!si)
                throw info_except("Could not read class\n");
            Matters = NULL;

            return 1;
        }
        int save_data_state(FilterOut &so) {
            so << " MatName_MatMass { ";
            SavableClass::IncIdent();
            for(size_t i = 0; i < Names.size(); i++)
                so << SavableClass::EOLN() << " Matter " << Names[i] << " Mass "
                   << Masses[i];
            SavableClass::DecIdent();
            so << EOLN() << "}" << SavableClass::EOLN();
            if(!so)
                throw info_except("Could not save class\n");
            return 1;
        }
        Stroka MakeHelp() {
            Stroka res =
                "Class for calcution free of 2 matters mixture : Temp, MeanDenc, VolCoefs are given...";
            return res;
        }
        double ClcFreeE(ClcDissociation::MatterCalculator *mats) {
            double fMix = VecCl(ClcFreeEMixt(mats)).SumElements();
            vector<double> nums = mats->NumCoefs;
            vector<double> fE = mats->GetResultFreeEPures();
            vector<double> M = mats->ClcMasV().Copy2Vector();
            double m0 = 0;
            double freeE = 0;
            for(size_t iMat = 0; iMat < nums.size(); iMat++) {
                m0 += M[iMat];
                freeE += nums[iMat] * fE[iMat] * mats->MolWeights[iMat];
            }
            return freeE / m0 + fMix;
        }
        vector<double> ClcFreeEMixt(ClcDissociation::MatterCalculator *mats) {
            SetStartParams(mats);
            mats->ClcFreeE();
            double kT = M_Rconst * mats->Temp;
            vector<double> nums = mats->NumCoefs;
            vector<double> vols = mats->ClcVolV().Copy2Vector();
            double m0 = 0;
            double v0 = 0;
            vector<double> M = mats->ClcMasV().Copy2Vector();
            for(size_t iMat = 0; iMat < nums.size(); iMat++) {
                m0 += M[iMat];
                v0 += vols[iMat];
            }
            vector<double> fMix(
                2);   // = (VecCl((int)mats->Substs.size()) * 0).Copy2Vector();

            for(size_t iMat = 0; iMat < nums.size(); iMat++)
                fMix[iMat] =
                    nums[iMat] * mats->MolWeights[iMat] * kT * log(vols[iMat] / v0);
            return (VecCl(fMix) / m0).Copy2Vector();
        }
        void SetStartParams(ClcDissociation::MatterCalculator *mats) {
            if(Matters == mats)
                return;
            Matters = mats;
            if(Matters->MolWeights.size() != Names.size())
                throw info_except(
                    "Wrong sizes: Matters->MolWeights:%i and Names:%i\n",
                    Matters->MolWeights.size(),
                    Names.size());
            Matters = mats;
            vector<int> NumInt2NumExt = Matters->MapNumInt2NumExt(Names);
            vector<double> massesInt(NumInt2NumExt.size());
            for(size_t iMass = 0; iMass < NumInt2NumExt.size(); iMass++)
                massesInt[NumInt2NumExt[iMass]] = Masses[iMass];
            Matters->NumCoefs =
                VecCl::ComponentDiv(VecCl(massesInt), VecCl(Matters->MolWeights))
                    .Copy2Vector();
        }

    protected:
        ClcDissociation::MatterCalculator *Matters;
        vector<Stroka> Names;
        vector<double> Masses;
    };


    struct FreeEMixture : FreeEIOFind {
        FreeEMixture();   // : FreeEIOFind() {}
        void ReadSubstancePar(istream &in) {
            MattersClc.Read(in);
            char tmp[256];

            in >> tmp;
            FilterTextIn fin(in.rdbuf(), 0);
            PresMinim.read_data_state(fin);
            fin >> tmp;
            MixtureClc.read_data_state(fin);
            //VolCoefMisfit = M_Eps2;
            if(!in)
                throw info_except("Could not read class\n");
        }
        void SaveSubstancePar(ostream &out) {
            MattersClc.Save(out);
            out << " PressureMinim ";
            FilterTextOut fout(out.rdbuf(), 0);
            PresMinim.save_data_state(fout);
            fout << " MixtureClc ";
            MixtureClc.save_data_state(fout);
        }
        Stroka MakeHelp() {
            Stroka res = FreeEIOFind::MakeHelp();
            res += "Calculate two matter mixture  \n";
            return res;
        }
        double FreeE(double Dencity, double Temperature) {
            MattersClc.SetClc(Dencity, Temperature);
            MixtureClc.SetStartParams(&MattersClc);
            PresMinim.FindEquilib(&MattersClc);
            return MixtureClc.ClcFreeE(&MattersClc);
        }

    protected:
        ClcDissociation::MatterCalculator MattersClc;
        ClcDissociation::Equilibrium2Pressure PresMinim;
        MixtureFreeEClc MixtureClc;
    };
};
