
#pragma once
#include "fre_fac.h"


#include "mat/SpecFunc.h"
class FreeEElectronStrict : public FreeEIOFind {
public:
    FreeEElectronStrict() : FreeEIOFind("material.cfg", "FreeEElS_H2") {
        SetInternalNames();
    };

public:
    ~FreeEElectronStrict(){};
    void ReadSubstancePar(istream &in) {
        char tmp[256];
        in >> tmp >> NumElPerAtom >> tmp >> MolVeight >> tmp >> Gfactor >> tmp >>
            ElMass_ElseNuclear >> tmp >> OutBoltzman >> tmp >> ColdCurve >> tmp >>
            ColdCurveTempCor;
    }
    void SaveSubstancePar(ostream &out) {
        out << " NumElPerAtom " << NumElPerAtom << " MolVeight " << MolVeight
            << " Gfactor " << Gfactor << " ElMass_ElseNuclear " << ElMass_ElseNuclear
            << " OutBoltzman " << OutBoltzman << " ColdCurve " << ColdCurve
            << " ColdCurveTempCor " << ColdCurveTempCor;
    }
    virtual Stroka MakeHelp() {
        return Stroka(
                   "EOS of gas of electrons in solid body. Strict formulas. Can calculate FermiTemp of e-. Also calculate FreeE of electrons(or atoms - if !ElMass_ElseNuclear) in the Boltzman assumption(take into account that we put Z electrons - as free - not one.... Gfactor have to be 2 - for electrons or what ever for atoms. If we set ColdCurve - we calculate FreeE at zero temperature. Always for electrons set Gfactor=2 - or will be errors. ColdCurveTempCor = 1 - we calculate low temperature corrections to the free energy ..\n") +
               FreeEIOFind::MakeHelp();
    };
    double FreeE(double Denc, double T) {
        if((T < MathZer) || (Denc < MathZer)) {
            cout << "FreeE Bad. Denc " << Denc << " T " << T << "\n";
            return 0;
        }
        Test_Temp = T;
        Test_Denc = Denc;
        if(OutBoltzman)
            return this->ClcFreeEBoltzman(Denc, T);
        if(ColdCurve || ColdCurveTempCor) {
            double ret = 0;
            if(ColdCurve)
                ret = ColdCurveLimit(Denc);
            if(ColdCurveTempCor)
                ret += ColdCurveTempCorr(Denc, T);
            return ret;
        }
        double neLambda = ClcNeLambda(Denc, T), FermiTemp = ClcFermiE(neLambda);
        double kt = M_Rconst / MolVeight * T * NumElPerAtom;
        //double fe = kt*(FermiTemp-8/(3*sqrt(M_PI)*neLambda)*SpecFunc::FermiDiracInt(SpecFunc::Ip1_5, FermiTemp));
        //cout<<" FermTemp "<<FermiTemp<<" FreeE "<<fe<<" ideal "<<ColdCurveLimit(Denc)<<" relat "<<fe/ColdCurveLimit(Denc)<<" assim "<<kt*(FermiTemp-8/(3*sqrt(M_PI)*neLambda)*2/5*pow(FermiTemp,5./2))<<"\n";
        return kt * (FermiTemp -
                     8 / (3 * sqrt(M_PI) * neLambda) *
                         SpecFunc::FermiDiracInt(SpecFunc::Ip1_5, FermiTemp));
    }

    virtual double OutputValClc(const map<Stroka, double> &Input, const Stroka &Output) {
        CheckPresence(OutputNames, Output);
        if(Output == Stroka("FermiTemp")) {
            if(ColdCurve || ColdCurveTempCor)
                return FermiColdLimit(CheckPresence(Input, "Dencity"));
            double Temp = CheckPresence(Input, "Temperature");
            return ClcFermiE(ClcNeLambda(CheckPresence(Input, "Dencity"), Temp)) * Temp;
        }
        if(Output == Stroka("BoltzFree")) {
            return ClcFreeEBoltzman(
                CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"));
        }

        return FreeEIOFind::OutputValClc(Input, Output);
    }
    void SetInternalNames() {
        FreeEIOFind::SetInternalNames();
        OutputNames["FermiTemp"] = NULL;
        OutputNames["BoltzFree"] = NULL;
    }

private:
    double NumElPerAtom, MolVeight;
    double Test_Temp, Test_Denc;
    int ElMass_ElseNuclear, OutBoltzman, Gfactor, ColdCurveTempCor, ColdCurve;

    double FindFermiInTemp(double FermiInt1_2_Val) {
        int stat;
        Current_FreeEElectronStrict = this;
        Current_FermiInt1_2_Val = FermiInt1_2_Val;
        Current_FermiInt_AddVal = 100;
        double Guess = (log(Current_FermiInt_AddVal - 50) + 21) / 2;
        if(3 < (stat = Fzero(
                    MisFermiInTemp,
                    log(Current_FermiInt_AddVal - 50),
                    21,
                    Guess,
                    StndErr * 0.001,
                    StndErr * 0.001,
                    100000)))

        {
            double Misf = MisFermiInTemp(Guess);
            if(fabs(Misf) > StndErr * 0.001) {
                double res = exp(Guess) - Current_FermiInt_AddVal;
                cout << " Error, could not find FermiTemp ZeroFzeroIt Temp " << Test_Temp
                     << " Denc " << Test_Denc << " res " << res << " mis " << Misf
                     << " Fzero_status " << stat << "\n";
            }
        }
        //if (Guess>1000){
        //    cout<<" Guess "<<Guess<<" FermiInt "<<SpecFunc::FermiDiracInt(SpecFunc::Ip0_5, Guess)<<" Misfit "<<SpecFunc::FermiDiracInt(SpecFunc::Ip0_5, Guess)-2./3*pow(Guess,3./2)<<"\n";
        //}
        //return (Guess);
        return exp(Guess) - Current_FermiInt_AddVal;
    }
    double Current_FermiInt1_2_Val, Current_FermiInt_AddVal;
    static FreeEElectronStrict *Current_FreeEElectronStrict;
    static double MisFermiInTemp(double log_x) {
        double x = exp(log_x) - Current_FreeEElectronStrict->Current_FermiInt_AddVal;
        double Int = SpecFunc::FermiDiracInt(SpecFunc::Ip0_5, x);
        double ret = Current_FreeEElectronStrict->Current_FermiInt1_2_Val - Int;
        //cout<<"Ln[x] "<<log_x<<" x "<<x<<" int "<<Int<<" mis "<<ret<<" val "<<Current_FreeEElectronStrict->Current_FermiInt1_2_Val<<"\n";
        return ret;
    }

    double ClcFreeEBoltzman(double Denc, double T) {
        double kt = M_Rconst / MolVeight * T * NumElPerAtom;
        return -kt * (log(1 / ClcNeLambda(Denc / 2, T)) +
                      1);   //  Denc/2 - to get right Gfactor for freeE bolt...
    }


    double ClcNeLambda(double Denc, double T) {
        double Mass = M_Mass_Electron_eV;
        if(!ElMass_ElseNuclear)
            Mass = M_ProtonMass_eV * MolVeight;
        double LambdaCoef =
            2 * M_PI * M_PlankCros_K * M_C / sqrt(2 * M_PI * Mass * M_eV_K * T);
        double neLambda3Coef = M_Na * pow(LambdaCoef, 3);

        //double Edenc = 2*Denc/MolVeight*NumElPerAtom/Gfactor;
        // Tested by freeE
        //double Edenc = Denc/MolVeight*NumElPerAtom/Gfactor;

        return neLambda3Coef * ClcNe(Denc);   //neLambda
    }
    double ClcFermiE(double neLambda) {
        double FermiInt1_2 = sqrt(M_PI) / 4 * neLambda;
        return FindFermiInTemp(FermiInt1_2);   //FermiE
    }


    double FermiColdLimit(double Denc) {
        double C0 = sqrt(2.) * pow(M_Mass_Electron_eV * M_eV_K / sqr(M_C), 1.5) /
                    pow(M_PlankCros_K, 3) / sqr(M_PI);

        //double ne = 2*Denc/MolVeight*NumElPerAtom/Gfactor;
        // in Landau formulas  g=2 is already included
        //double ne = 2*Denc/MolVeight*NumElPerAtom/Gfactor;
        double ret = pow(1.5 * M_Na * ClcNe(Denc) / C0, 2. / 3);

        return ret;
    }
    double ColdCurveLimit(double Denc) {
        double Coef = 3 * pow(3 * sqr(M_PI), 2. / 3) * sqr(M_PlankCros_K * M_C) /
                      (10 * M_Mass_Electron_eV * M_eV_K) * M_eV_kJ / M_eV_K;
        double ret =
            Coef * M_Na * NumElPerAtom / MolVeight * pow(M_Na * ClcNe(Denc), 2. / 3);
        return ret;
    }
    double ColdCurveTempCorr(double Denc, double T) {
        double Coef = M_Rconst * M_Mass_Electron_eV * M_eV_K /
                      (sqr(M_PlankCros_K * M_C) * pow(3 * M_Na, 2. / 3)) *
                      pow(M_PI, 2. / 3) / 4;
        // in Landau formulas  g=2 is already included
        //double ret =  Coef/MolVeight*sqr(T)*pow(2*Denc*NumElPerAtom/(Gfactor*MolVeight),-2./3);
        double ret = -Coef / MolVeight * sqr(T) * pow(ClcNe(Denc), -2. / 3);

        return ret;
    }
    double ClcNe(double Denc) {
        return 2 * Denc / MolVeight * NumElPerAtom / Gfactor;
    }
};
