#pragma once


#include "matt_fac.h"
#include "mat/fun_fac.h"
#include "fre_fac.h"

// =============================================================================
// =====================  Two Phase EOS   ======================================
// ========================== Start ============================================
#include "mat/spl_fac.h"
#include "lib/std/util.h"
#include "mat/quad.h"


struct MatterStable : MatterIO {

    struct TwoPhaseBoundary : SavableClass {
        // Binodal Spline
        CurveSpline E_T, P_T, dPdT_T, Dr_T, Dl_T;
        double Tmin, Tmax, Dmin, Dmax;

        CurveSpline DSl_T, DSr_T;
        TwoPhaseBoundary() : Tmin(1), Tmax(1000), Dmin(1e-6), Dmax(100) {}

        //   CurveSpline BinLftD_E,BinRgtD_E,SpinLftD_E,SpinRgtD_E;
        //   double Emin,Emax;

        double Stability(double Dencity, double Temperature) {
            if(!In_Lim(Temperature, Tmin, Tmax) || !In_Lim(Dencity, Dmin, Dmax))
                return 0;
            double DencL = Dl_T.Evaluate(Temperature), DencR = Dr_T.Evaluate(Temperature);
            double DencSL = DSl_T.Evaluate(Temperature),
                   DencSR = DSr_T.Evaluate(Temperature);
            if((In_Lim(Dencity, Dmin, DencL)) || (In_Lim(Dencity, DencR, Dmax)))
                return 0;
            if(In_Lim(Dencity, DencSL, DencSR))
                return 1;   // MayBeOnlyStable
            if(In_Lim(Dencity, DencL, DencSL))
                return (Dencity - DencL) / (DencSL - DencL);
            return (DencR - Dencity) / (DencR - DencSR);
        };

        int InTwoPhase(double Dencity, double Temperature) {
            if(!In_Lim(Temperature, Tmin, Tmax) || !In_Lim(Dencity, Dmin, Dmax))
                return 0;
            double DencL = Dl_T.Evaluate(Temperature), DencR = Dr_T.Evaluate(Temperature);
            if(In_Lim(Dencity, DencL, DencR))
                return 1;
            return 0;
        };

        virtual int read_data_state(FilterIn &input) {
            char tmp[256];
            input >> tmp >> Tmin >> tmp >> Tmax >> tmp >> Dmin >> tmp >> Dmax;

            E_T.ReadIni(input);
            P_T.ReadIni(input);
            dPdT_T.ReadIni(input);
            Dr_T.ReadIni(input);
            Dl_T.ReadIni(input);
            DSl_T.ReadIni(input);
            DSr_T.ReadIni(input);
            return 1;
        };
        virtual int save_data_state(FilterOut &output) {
            output.precision(12);
            output << " Binodal_Tmin " << Tmin << " Tmax " << Tmax << " Denc_min " << Dmin
                   << " Denc_max " << Dmax << "\n";
            E_T.SaveIni(output);
            P_T.SaveIni(output);
            dPdT_T.SaveIni(output);
            Dr_T.SaveIni(output);
            Dl_T.SaveIni(output);
            DSl_T.SaveIni(output);
            DSr_T.SaveIni(output);
            return 1;
        };
        // Construction of the boundary from the EOS....
        static map<Stroka, double> ClcBnd(
            double Temp,
            double Dmin,
            double Dmax,
            double NumPnt,
            double NumSplPnt,
            FreeEIO *Mat,
            double Misfit);
        static Stroka SetBnd(
            map<Stroka, vector<double>> &data,
            const Stroka &res_file_name,
            double SplMisfit,
            double NumSplPnt);
        static vector<Stroka> GetBndNames() {
            vector<Stroka> ret(8);
            ret[0] = "E_T";
            ret[1] = "P_T";
            ret[2] = "dPdT_T";
            ret[3] = "Dr_T";
            ret[4] = "Dl_T";
            ret[5] = "DSr_T";
            ret[6] = "DSl_T";
            ret[7] = "T";
            return ret;
        };

    private:
        static Stroka MakeSpline(
            CurveSpline &spl,
            int Nspl,
            double Misf,
            double *y,
            double *x,
            int NumX,
            const char *err_name);
        double T, Mis;   //, Dmin, Dmax;
        int NumPnt, NumSplPnt;
        FreeEIO *Mat;
        static TwoPhaseBoundary *TwoPhaseBoundaryStaticPtr;
        CurveSpline IntegralSpline_P_V;
        int IntegralFunc_Negative;   //,ZeroFunc_ErrorM;
        double ZeroFunc_Misf, ZeroFunc_Spl_Vmin, ZeroFunc_Spl_Vmax, ZeroFunc_SplRes_Bmin,
            ZeroFunc_SplRes_Bmax;
        double IntegralFunc_Add;

        static double IntegralFunc_PV_Int_Static(double v) {
            return TwoPhaseBoundaryStaticPtr->IntegralFunc_PV_Int(v);
        }
        double IntegralFunc_PV_Int(double v);
        static double ZeroFunc_PV_Int_Static(double pres) {
            return TwoPhaseBoundaryStaticPtr->ZeroFunc_PV_Int(pres);
        }
        double ZeroFunc_PV_Int(double pres);
        int SetDP(VecCl &V, VecCl &D, VecCl &P, int &SpinIndMin, int &SpinIndMax);
        void PrepareStatic(VecCl &P, VecCl &V);
        void FindMinMax(VecCl &V, int CentInd, double &ResV, double &ResP, int Min);
        map<Stroka, double> MakeBndPnt();
    };

    Ref<MatterIO> MatUnstable;

    TwoPhaseBoundary Bnd;
    char TwoPhaseName[256];
    int FixedUnstable;

    virtual double Energy(double Dencity, double Temperature) {
        if((!Bnd.InTwoPhase(Dencity, Temperature)) && (!FixedUnstable))
            return MatUnstable->Energy(Dencity, Temperature);
        double e = Bnd.E_T.Evaluate(Temperature), Dr = Bnd.Dr_T.Evaluate(Temperature),
               deriv =
                   Bnd.dPdT_T.Evaluate(Temperature);   //p=Bnd.P_T.Evaluate(Temperature)
        return e + (1 / Dr - 1 / Dencity) * deriv;
    };
    double DencityCold() {
        return MatUnstable->DencityCold();
    };
    double DencityMax() {
        return MatUnstable->DencityMax();
    };

    double E_, D_, L_T, L_E, L_D, Zero_Temp;
    static MatterStable *Current;
    static double DeltaFree(double T) {
        return Current->Energy(Current->D_, T) - Current->E_;
    }

    double Temperature(double Dencity, double Energy) {
        if(fabs(L_D - Dencity) + fabs(L_E - Energy) < MathZer)
            return L_T;
        double T = MatUnstable->Temperature(Dencity, Energy);
        L_D = Dencity;
        L_E = Energy;
        L_T = T;
        if(!Bnd.InTwoPhase(Dencity, T))
            return T;
        double T0 = L_T;
        E_ = Energy;
        D_ = Dencity;
        MatterStable *Old = Current;
        Current = this;
        int err;
        FixedUnstable = 1;
        if(4 <
           (err = Fzero(DeltaFree, 10, 1e6, T0, StndErr * 0.01, StndErr * 0.01, 200))) {
            cout << " Bad ZeroFzeroFreeE in MatterStable::Temperature Not Zero\nfound T "
                 << T0 << " Mis " << DeltaFree(T0) << " err " << err << "\n";
            T0 = Zero_Temp;
        }
        FixedUnstable = 0;
        L_T = T0;
        Current = Old;
        return T0;
    }

    double Pressure(double Dencity, double Energy) {
        double T = MatUnstable->Temperature(Dencity, Energy);
        if(!Bnd.InTwoPhase(Dencity, T))
            return MatUnstable->Pressure(Dencity, Energy);
        //cout<<"Wau!!!!!!!!! Dencity "<<Dencity<<" Energy "<<Energy<<" T "<<T<<"\n";
        double t = Temperature(Dencity, Energy);
        return Bnd.P_T.Evaluate(t);
    };
    MatterStable(double Zer_T = 100) {
        MatUnstable = new MatterABu;
        L_T = 1000;
        L_D = 0;
        L_E = 0;
        Zero_Temp = Zer_T;
        strcpy(TwoPhaseName, "TwoPhaseName");
    };
    virtual int save_data_state(FilterOut &output) {
        output << " TwoPhaseBndFile " << TwoPhaseName << " UnstableMatter ";
        SavableClass::Save(output, MatUnstable);
        return 1;
    };
    virtual int read_data_state(FilterIn &input) {
        char tmp[256];
        FixedUnstable = 0;
        input >> tmp >> TwoPhaseName >> tmp;
        //     fstream in(TwoPhaseName,ios::in);Bnd.ReadIni(in);in.close();
        FilterTextIn in(TwoPhaseName);
        Bnd.read_data_state(in);
        //delete MatUnstable;
        return !(!(MatUnstable << SavableClass::Read(input)));
    };
    //~MatterStable(){delete MatUnstable;MatUnstable=NULL;};
    //   virtual void MakeHelp(FilterTextOut&out){
    //	out<<"-----------   "<<this->class_name()<<"   --------------------------\n";
    //	out<<"Create:\n\t";SavableClass::SavableClassStore(out,this);
    //	out<<"\nTwoPhaseName file - is a binary file... Help for it is not available....\n";
    //	out<<"\n----------------------------------------------\n";
    //}
    virtual Stroka MakeHelp() {
        Stroka res = MatterIO::MakeHelp();
        res += Stroka(
            " TwoPhaseName file - is a binary file... Help for it is not available....\n");
        return res;
    }
};

//#ifdef InternalPointsUrs

struct MatterBinodalSpinodal : virtual MatterIO {
    Ref<MatterStable> MatStable;
    double TimeUnstable;
    void SetInternalNames() {
        MatterIO::SetInternalNames();
        InputNames["MatterStable"] = NULL;
    }

    double Energy(double Dencity, double Temperature) {
        if(InputValStorage["MatterStable"] < 1)
            return MatStable->MatUnstable->Energy(Dencity, Temperature);
        return MatStable->Energy(Dencity, Temperature);
    };
    double Temperature(double Dencity, double Energy) {
        if(InputValStorage["MatterStable"] < 1)
            return MatStable->MatUnstable->Temperature(Dencity, Energy);
        return MatStable->Temperature(Dencity, Energy);
    }

    double Pressure(double Dencity, double Energy) {
        if(InputValStorage["MatterStable"] < 1)
            return MatStable->MatUnstable->Pressure(Dencity, Energy);
        return MatStable->Pressure(Dencity, Energy);
    };

    MatterBinodalSpinodal() : MatterIO(), MatStable(new MatterStable) {
        SetInternalNames();
    };
    virtual int save_data_state(FilterOut &output) {
        output << " TimeUnstable " << TimeUnstable << " StableMatter \n";
        SavableClass::Save(output, MatStable);
        return 1;
    };
    virtual int read_data_state(FilterIn &input) {
        char tmp[256];
        input >> tmp >> TimeUnstable >> tmp;
        MatStable << SavableClass::Read(input);
        return MatStable != NULL;
    };
    int NumIntPar() {
        return 1;
    }
    virtual void ChangeInputVal(double Dencity, double Energy, double Time) {
        double Temperature = MatStable->Temperature(Dencity, Energy);
        double x = MatStable->Bnd.Stability(Dencity, Temperature);
        if(x == 0)
            InputValStorage["MatterStable"] = 0;
        else if(x == 1)
            InputValStorage["MatterStable"] = 1;
        else
            InputValStorage["MatterStable"] =
                min<double>(InputValStorage["MatterStable"] + Time * TimeUnstable * x, 1);
    };
};

//#endif    //#ifdef InternalPointsUrs


struct MatterLiqSol : MatterIO {
    struct LiquidSolidBnd : SavableClass {
        // Binodal Spline
        CurveSpline Eliq_D, Esol_D, Pliq_D, Psol_D, T_D;
        LiquidSolidBnd(){};
        double Dmin, Dmax;


        double LiqPressure(double Dencity, double Energy) {
            if(!In_Lim(Dencity, Dmin, Dmax))
                return -100;
            double Eliq = Eliq_D.Evaluate(Dencity);
            if(Energy > Eliq)
                return -100;
            double Esol = Esol_D.Evaluate(Dencity);
            if(Energy < Esol)
                return -200;
            double Pliq = Pliq_D.Evaluate(Dencity), Psol = Psol_D.Evaluate(Dencity);
            return Pliq - (Eliq - Energy) / (Eliq - Esol) * (Pliq - Psol);
        };
        double Temperature(double Dencity) {
            return T_D.Evaluate(Dencity);
        };

        virtual int save_data_state(FilterOut &output) {
            throw info_except("Not implemented???????????\n");
        };
        virtual int read_data_state(FilterIn &input) {
            throw info_except("Not implemented???????????\n");
        }
    };

    Ref<MatterIO> Solid, Liquid;
    LiquidSolidBnd Bnd;
    Stroka BndFile;

    virtual double Pressure(double Dencity, double Energy) {
        double ret = Bnd.LiqPressure(Dencity, Energy);
        if(ret == -100)
            return Liquid->Pressure(Dencity, Energy);
        if(ret == -200)
            return Solid->Pressure(Dencity, Energy);
        return ret;
    }
    //   virtual double Sound(double Dencity,double Energy);
    virtual double Temperature(double Dencity, double Energy) {
        double ret = Bnd.LiqPressure(Dencity, Energy);
        if(ret == -100)
            return Liquid->Temperature(Dencity, Energy);
        if(ret == -200)
            return Solid->Temperature(Dencity, Energy);
        return Bnd.Temperature(Dencity);
    };
    virtual double DencityCold() {
        return Solid->DencityCold();
    }
    virtual double DencityMax() {
        return Solid->DencityMax();
    }
    virtual double Energy(double Dencity, double Temperature) {
        double Temp = Bnd.Temperature(Dencity);
        if(Temperature > Temp)
            return Liquid->Energy(Dencity, Temperature);
        if(Temperature < Temp)
            return Solid->Energy(Dencity, Temperature);
        return Liquid->Energy(Dencity, Temperature);
    };


    MatterLiqSol() : BndFile("BndFileName") {
        Solid = new MatterABu;
        Liquid = new MatterABu;
    };
    virtual int save_data_state(FilterOut &output) {
        output << " MatterLiquidClass ";
        SavableClass::Save(output, Liquid);
        output << " MatterSolidClass ";
        SavableClass::Save(output, Solid);
        output << " LiquidSolidBndFile " << BndFile << " ";
        return 1;
    };
    virtual int read_data_state(FilterIn &input) {
        char tmp[256];
        input >> tmp;
        //delete Liquid;delete Solid;
        if(!(Liquid << SavableClass::Read(input)))
            return 0;
        input >> tmp;
        if(!(Solid << SavableClass::Read(input)))
            return 0;
        input >> tmp >> BndFile;
        FilterTextIn in(BndFile.c_str());
        if(!Bnd.read_data_state(in))
            return 0;
        return 1;
    };
    //   virtual void MakeHelp(FilterTextOut&out){
    //	out<<"-----------   "<<this->class_name()<<"   --------------------------\n";
    //	out<<"Create:\n\t";SavableClass::SavableClassStore(out,this);
    //	out<<"\n----------------------------------------------\n";
    //}

    //~MatterLiqSol(){delete Solid;Solid=NULL;delete Liquid;Liquid=NULL;};
};


// =============================================================================
// =====================  Two Phase EOS   ======================================
// ========================== Finish ===========================================

struct FreeELiqSol_Bnd : TwoVarFunction {
    FreeELiqSol_Bnd() : NameFile("zero.bin"){};
    CurveSpline Rhgh_T, Rlow_T;
    Stroka NameFile;
    virtual double Calculate(double Denc, double Temp) {
        //		double low = Rhgh_T.Evaluate(Temp), up = Rlow_T.Evaluate(Temp);
        if(Temp < Rhgh_T.Xlow)
            return 0;
        if(Temp > Rhgh_T.Xup)
            return 1;
        double low = Rlow_T.Evaluate(Temp), up = Rhgh_T.Evaluate(Temp);
        if(low > up) {
            double tmp = low;
            low = up;
            up = tmp;
        }
        //double a;if (Denc<low) a = 1;else if (Denc>up)	a = 0;else a = (up-Denc)/(up-low);
        //cout<<Temp<<" "<<low<<" "<<up<<" "<<Denc<<" "<<a<<"\n";

        if(Denc > Rhgh_T.Evaluate(Rhgh_T.Xup))
            return 0;   //hgh
                        //if (Temp>Rhgh_T.Xup)
        if(Denc < low)
            return 1;   //low
        if(Denc > up)
            return 0;   //hgh
        return (up - Denc) / (up - low);
        //return (1/up-1/Denc)/(1/low-1/up);
    };
    void GetLowUp(double &low, double &up, double Temp) {
        low = Rlow_T.Evaluate(Temp);
        up = Rhgh_T.Evaluate(Temp);
        if(low > up) {
            double tmp = low;
            low = up;
            up = tmp;
        }
    }
    virtual int save_data_state(FilterOut &so) {
        so << "NameBndSplFile" << NameFile;
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        char tmp[256];
        si >> tmp >> NameFile;
        ReadFile(NameFile.c_str(), this);
        return 1;
    }
    static void ReadFile(const char *name, FreeELiqSol_Bnd *bnd) {
        FilterTextIn in(name);
        char tmp[256];
        in >> tmp >> bnd->Rhgh_T >> tmp >> bnd->Rlow_T;
        if(!in)
            throw(info_except("Could not read file <%s>\n", name));
    }
    static void SaveFile(const char *name, FreeELiqSol_Bnd *bnd) {
        FilterTextOut out(name);
        out << "HighDenc_TSpl\n" << bnd->Rhgh_T << "\nLowDenc_TSpl\n" << bnd->Rlow_T;
    }
    static FreeELiqSol_Bnd *Generate(
        double *Rhgh,
        double *Rlow,
        double *T,
        int Num,
        double Err) {
        log_debug(Stroka("FreeELiqSol_Bnd::Generate ") + Num);
        FreeELiqSol_Bnd *res = new FreeELiqSol_Bnd;
        int NumRes = Num;
        if(!res->Rhgh_T.Generate(
               NumRes,
               Err,
               0,
               T,
               Rhgh,
               Num))   //int &NumX,double &Misf,int Cont,double *x,double *y,int n,
            throw info_except("could not generate spline for Hgh denc\n");
        NumRes = Num;
        if(!res->Rlow_T.Generate(
               NumRes,
               Err,
               0,
               T,
               Rlow,
               Num))   //int &NumX,double &Misf,int Cont,double *x,double *y,int n,
            throw info_except("could not generate spline for Low denc\n");
        return res;
    }
};

struct FreeELiqSol : FreeEIOFind {
    FreeELiqSol()
        : FreeEIOFind("material.cfg", "FreeELiqSol_Bushman_Mg"),
          MatHghTemp(new FreeEIdeal),
          MatLowTemp(new FreeEIdeal),
          Bnd(new FreeELiqSol_Bnd) {
        SetInternalNames();
    };
    Ref<InterfaceFreeEIO> MatHghTemp, MatLowTemp;
    Ref<FreeELiqSol_Bnd> Bnd;
    double FreeE(double Denc, double Temp) {
        double x = Bnd->Calculate(Denc, Temp);
        //double hgh = MatHghTemp->FreeE(Denc,Temp),low = MatLowTemp->FreeE(Denc,Temp);cout<<Denc<<" "<<x<<" "<<hgh<<" "<<low<<" "<<(1-x)*hgh+x*low<<"\n";
        if(x == 0)
            return MatHghTemp->FreeE(Denc, Temp);
        if(x == 1)
            return MatLowTemp->FreeE(Denc, Temp);
        double low, up;
        Bnd->GetLowUp(low, up, Temp);
        double f1 = MatHghTemp->FreeE(up, Temp), f2 = MatLowTemp->FreeE(low, Temp);
        double pb = (f1 - f2) / (1 / low - 1 / up);
        return f1 + pb * (1 / up - 1 / Denc);
        //		return (1-x)*MatHghTemp->FreeE(up,Temp)+x*MatLowTemp->FreeE(low,Temp);
        //		return x*MatHghTemp->FreeE(low,Temp)+(1-x)*MatLowTemp->FreeE(up,Temp);
    }
    void ReadSubstancePar(istream &in) {
        char tmp[256];
        in >> tmp;
        MatHghTemp << SavableClass::Read(in);
        in >> tmp;
        MatLowTemp << SavableClass::Read(in);
        in >> tmp;
        Bnd << SavableClass::Read(in);
        if(!MatHghTemp || !MatLowTemp || !Bnd)
            throw info_except(
                "Could not register MatHghTemp?%i or MatLowTemp?%i or Bnd?%i\n",
                !MatHghTemp,
                !MatLowTemp,
                !Bnd);
    };
    void SaveSubstancePar(ostream &out) {
        out << " MatHghTemp ";
        SavableClass::Save(out, MatHghTemp);
        out << " MatLowTemp ";
        SavableClass::Save(out, MatLowTemp);
        out << " Bnd ";
        SavableClass::Save(out, Bnd);
    }
    virtual Stroka MakeHelp() {
        Stroka res = FreeEIOFind::MakeHelp();
        res += Stroka(
            " To generate bnd class - Make bounds by urs_curve;\nBounds generation config:\n");
        res += Stroka(
            "\nURS_Curve { \n    Variables {  \n\n        MatLowTemp   EOS_Savable { MatterFreeE {  FreeEBushman   {  Material_File mat.bad Substance Mg_BushmanSolid } } }\n        MatHghTemp EOS_Savable { MatterFreeE {  FreeEBushman   {  Material_File mat.bad Substance Mg_BushmanLiquid } }  }\n\n        Temperature   UrsCurve_StepClc { MinVal 922 MaxVal 9000  NumDivStp 100  LogScale 1 NumSame 1  Centered 0 }    \n\n	BndMat  UrsCurve_PT_Bnd_Constructor { NameTemp Temperature NameMatterHghP MatHghTemp NameMatterLowP MatLowTemp ClcError 1e-006 FindPDencFrom  1. FindPDencTo 8 FindGLowPres 1e-6 FindGHghPres 800 StartPres 1 }   \n	BinGen  UrsCurve_PT_Bnd_Binary { NameTemp Temperature NameHghD BndMat.DencityHghP_T NameLowD BndMat.DencityLowP_T NameFileToGenerate HghLow_bnd.bin ClcError 1e-6 } \n\n    }\n\n    Output   URS_Curve::Output { \n        FileToStore mol.bnd2.dat VectorOfNames { \n              BinGen BndMat.Pressure_T\n        }\n    }   \nNumIter  100   }\n\n");
        return res;
    }
    virtual void SetInternalNames() {
        FreeEIOFind::SetInternalNames();
        int k = 1;
        OutputNames["Low_M"] = &k;
        OutputNames["Hgh_M"] = &k;
    }
    virtual double OutputValClc(const map<Stroka, double> &Input, const Stroka &Output) {
        void *ptr;
        if(CheckPresence(OutputNames, Output, ptr) && ptr != NULL) {
            if(Output == "Low_M")
                return DisLowM(
                    CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"));
            if(Output == "Hgh_M")
                return 1 - DisLowM(
                               CheckPresence(Input, "Dencity"),
                               CheckPresence(Input, "Temperature"));
        }
        return FreeEIOFind::OutputValClc(Input, Output);
    }
    double DisLowM(double Denc, double Temp) {
        double x = Bnd->Calculate(Denc, Temp);
        if(x == 0)
            return 0;
        if(x == 1)
            return 1;
        double low, up;
        Bnd->GetLowUp(low, up, Temp);
        double f1 = 0, f2 = 1;
        double pb = (f1 - f2) / (1 / low - 1 / up);
        return f1 + pb * (1 / up - 1 / Denc);
    }
};
