#pragma once

#include "fre_fac.h"
#include "mat/NdimMin.h"
#include "mat/fun_fac.h"

struct FreeESumMatter : FreeEIOFind {
    FreeESumMatter();   //:FreeEIOFind("material.cfg","MatterSumH2Met"){FreeCoef.push_back(1);FreeVect.push_back(new FreeEIdeal);}
    vector<Ref<InterfaceFreeEIO>> FreeVect;
    vector<double> FreeCoef;
    virtual ~FreeESumMatter() {
        EraseMat();
    }
    void EraseMat() {
        //for (size_t k=0;k<FreeVect.size();k++)
        //    delete FreeVect[k];
        FreeVect.clear();
        FreeCoef.clear();
    }
    double FreeE(double Denc, double T) {
        double ret = 0;
        for(size_t k = 0; k < FreeVect.size(); k++)
            if(fabs(FreeCoef[k]) > MathZer)
                ret += FreeVect[k]->FreeE(Denc, T) * FreeCoef[k];
        return ret;
    }
    void SetTLimits(double &minT, double &maxT, double &stpTcoef, double &stpTmin) {
        if(!FreeVect.size())
            return;
        int fst = 1;
        for(size_t k = 0; k < FreeVect.size(); k++)
            //if (fabs(FreeCoef[k]) > MathZer)
            if(fst) {
                FreeVect[k]->SetTLimits(minT, maxT, stpTcoef, stpTmin);
                fst = 0;
            } else {
                double minT_, maxT_, stpTcoef_, stpTmin_;
                FreeVect[k]->SetTLimits(minT_, maxT_, stpTcoef_, stpTmin_);
                minT = max<double>(minT, minT_);
                maxT = min(maxT, maxT_);
                stpTcoef = max<double>(stpTcoef, stpTcoef_);
                stpTmin = max<double>(stpTmin, stpTmin_);
            }
    }
    void ReadSubstancePar(istream &in) {
        Ref<InterfaceFreeEIO> f;
        double coef;
        char tmp[256];
        //      Name<H2Met> {  coef1 1 mat1 mat... }
        EraseMat();
        in >> tmp;
        if(Stricmp(tmp, "{") != 0)
            throw info_except("Reading FreeESumMatter expected { and got %s\n", tmp);
        //cout<<" FreeESumMatter::ReadSubstancePar start read matter \n";
        do {
            in >> tmp;   //Coef1
            if(Stricmp(tmp, "}") == 0)
                break;
            in >> coef;
            FreeCoef.push_back(coef);
            in >> tmp;   //Matter1
            if(!(f << SavableClass::Read(in)))
                throw info_except(
                    "Could not convert matter n=%i to InterfaceFreeEIO\n",
                    FreeCoef.size());
            //cout<<" FreeESumMatter::ReadSubstancePar read matter "<<f->class_name()<<"\n";
            FreeVect.push_back(f);
        } while(1);
        SetInternalNames();
        //cout<<" FreeESumMatter::ReadSubstancePar end read matter \n";
    }
    void SaveSubstancePar(ostream &out) {
        out << " { ";
        for(size_t k = 0; k < FreeCoef.size(); k++) {
            out << "\n coef " << FreeCoef[k] << " ";
            SavableClass::Save(out, FreeVect[k]);
        }
        out << " } ";
    }

    virtual void SetInternalNames() {
        FreeEIOFind::SetInternalNames();
        int numSubst = FreeCoef.size();
        for(size_t k = 0; k < size_t(numSubst); k++) {
            vector<Stroka> names = Str::SplitLine(FreeVect[k]->GetOutputNames());
            for(size_t i = 0; i < names.size(); i++)
                OutputNames[Stroka::Int2Str(int(k)) + "_" + names[i]] = NULL;
        }
    }

    int GetSubstNum_ParName(const Stroka &str, int &substNum, Stroka &parName) {
        vector<Stroka> strVec = Str::SplitLine(str, 1, '_');
        if(strVec.size() == 0)
            return 0;
        substNum = atoi(strVec[0].c_str());
        parName = Str::JoinLine(strVec, '_', 1);
        return substNum < int(FreeVect.size()) && substNum >= 0;
    }
    virtual double OutputValClc(const map<Stroka, double> &Input, const Stroka &Output) {
        int numVar;
        Stroka outName;
        if(GetSubstNum_ParName(Output, numVar, outName))
            return FreeVect[numVar]->OutputValClc(Input, outName);
        return FreeEIOFind::OutputValClc(Input, Output);
    }
};


// THE REST IS DEPRICATED PART - USE FREEEDIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//======================================================================================
//=============================   Strict formulas using for minimizing =================
//======================================================================================
// Defining minimization functions
// =====================================================================================
// Single dissosiation minimization function
struct SingleDiss_MinimFunc : RefCount {
    static SingleDiss_MinimFunc *StaticPtr;
    VecCl Veights;
    double Find_ChemCoef;
    SingleDiss_MinimFunc(const VecCl &veights) : Veights(veights) {
        if(Veights.Dim() != 2)
            throw info_except(
                " ReactCoef_DissMinimFunc -> works only in 2 dims. Trying to set init dim=%i\n",
                Veights.Dim());
    };
    double FindDissFstDenc(
        const VecCl &FreeE_NoMixt,
        double Denc,
        double Temp,
        double Guess = 0) {
        //cout<<"FreeEStat "<<FreeE_NoMixt;
        double DencA = Denc * M_Na;
        double arg = (FreeE_NoMixt[1] - FreeE_NoMixt[2]) * Veights[1] * Veights[2] /
                     (M_Rconst * Temp);
        if(arg > 100)
            return 0;
        if(arg < -100)
            return Denc;
        double ChemCoef = exp(arg);
        ChemCoef *= pow(DencA, Veights[2] - Veights[1]) * pow(Veights[1], -Veights[2]) *
                    pow(Veights[2], Veights[1]);
        Find_ChemCoef = ChemCoef;
        Guess = (Guess == 0) ? 0.5 : Guess;
        Guess = Encode(Guess);
        double ErrorAbs, ErrorRel;
        ErrorRel = ErrorAbs = 1e-8;
        int err, MaxIter = 100;
        SingleDiss_MinimFunc *OldPtr = StaticPtr;
        StaticPtr = this;
        if(4 <
           (err = Fzero(ExecuteStatic, -35, 35, Guess, ErrorAbs, ErrorRel, MaxIter))) {
            cout
                << " Bad FZero FindDissMass1 in SingleDiss_MinimFunc::FindDissMass1 \nfound X "
                << Decode(Guess) << " Mis " << ExecuteStatic(Guess) << " err " << err
                << "\n";
        }
        StaticPtr = OldPtr;
        double ret = Decode(Guess) * Denc;
        //cout<<" resDis1Denc "<<ret<<" Guess "<<Guess<<"\n";GetMixtureFreeE(FreeE_NoMixt,Denc,Temp,Veights,ret);
        return ret;
    }
    static double ExecuteStatic(double x) {
        return StaticPtr->Execute(x);
    }
    double Execute(double par) {
        double x = Decode(par);
        double ret = pow(1 - x, Veights[1]) * pow(x, -Veights[2]) - Find_ChemCoef;
        //cout<<" par "<<par<<" x "<<x<<" res "<<ret<<"\n";
        return ret;
    }
    static double Encode(double x) {
        x = x <= MathZer ? MathZer : (x >= 1 - MathZer ? 1 - MathZer : x);
        x = log(x / (1 - x));
        return x;
    };
    static double Decode(double par) {
        par = 1 / (1 + exp(-par));
        if(fabs(par) < 1e-17)
            return 0;
        return par;
    };
    static double GetMixtureCorrection(double Denc, double Temp, double SubstVeight) {
        double kt = M_Rconst / SubstVeight * Temp;
        Denc = Denc < MathZer ? MathZer : Denc;
        double F = kt * log(Denc * M_Na / SubstVeight / M_E);
        //double F=kt*log(Denc/SubstVeight/M_E);
        return F;
    }
    static double GetMixtureFreeE(
        const VecCl &FreeE_NoMixt,
        double Denc,
        double Temp,
        const VecCl &Veights,
        double DissFstDenc) {
        double R1 = DissFstDenc, R2 = Denc - DissFstDenc, R0 = Denc;
        double ret =
            ((FreeE_NoMixt[1] + GetMixtureCorrection(R1, Temp, Veights[1])) * R1 +
             (FreeE_NoMixt[2] + GetMixtureCorrection(R2, Temp, Veights[2])) * R2) /
            R0;
        //cout<<"R1 "<<R1<<" R2 "<<R2<<" Temp "<<Temp<<" V1 "<<Veights[1]<<" V2 "<<Veights[2]<<"\n";
        //cout<<" res "<<ret<<" F1 "<<FreeE_NoMixt[1]<<" F2 "<<FreeE_NoMixt[2]
        ////<<" Cor1 "<<GetMixtureCorrection(R1,Temp,Veights[1])<<" Cor2 "<<GetMixtureCorrection(R2,Temp,Veights[2])
        //<<" S1 "<<FreeE_NoMixt[1]+GetMixtureCorrection(R1,Temp,Veights[1])
        //<<" S2 "<<FreeE_NoMixt[2]+GetMixtureCorrection(R2,Temp,Veights[2])
        //<<"\n";
        return ret;
    }
};


// Any number dissosiation minimization function
struct MatrDiss_MinimFunc : RefCount {
    VecCl DissDenc, Veights;
    MatrDiss_MinimFunc(const VecCl &veights) : Veights(veights){};
    double GetMisfit(
        double Denc,
        double Temp,
        const VecCl &FreeE_NoMixt,
        VecCl &ResDenc,
        int LowInd,
        int UpInd) {
        VecCl veight(2);
        veight[1] = Veights[LowInd];
        veight[2] = Veights[UpInd];
        VecCl free(2);
        free[1] = FreeE_NoMixt[LowInd];
        free[2] = FreeE_NoMixt[UpInd];
        double denc = ResDenc[LowInd] + ResDenc[UpInd];
        //cout<<" Start Denc "<<ResDenc<<"denc "<<denc<<"\n";
        ResDenc[LowInd] = SingleDiss_MinimFunc(veight).FindDissFstDenc(
            free, denc, Temp, ResDenc[LowInd]);
        //cout<<" res1Denc "<<ResDenc[LowInd];
        double mis = ResDenc[UpInd] + ResDenc[LowInd] - denc;
        ResDenc[UpInd] = denc - ResDenc[LowInd];
        //cout<<" res2Denc "<<ResDenc[UpInd];
        //		return SingleDiss_MinimFunc::Encode(mis);
        return fabs(mis);
    }
    VecCl FindDissDenc(
        double Denc,
        double Temp,
        const VecCl &FreeE_NoMixt,
        VecCl *GuessDenc = NULL) {
        VecCl ResDenc;
        if(FreeE_NoMixt.Dim() < 1)
            return VecCl(0);
        if(FreeE_NoMixt.Dim() == 1)
            return VecCl(Denc);
        if(GuessDenc)
            ResDenc = (*GuessDenc);
        else
            ResDenc = FreeE_NoMixt * 0 + Denc / FreeE_NoMixt.Dim();
        vector<int> SortInd = VecCl::Sort(FreeE_NoMixt);
        double Misfit = 1;
        while(Misfit > StndErr) {
            Misfit = 0;
            for(size_t k = 1; k < SortInd.size(); k++)
                Misfit +=
                    GetMisfit(Denc, Temp, FreeE_NoMixt, ResDenc, SortInd[0], SortInd[k]);
        }
        DissDenc = ResDenc;
        return ResDenc;
    }
    static double GetMixtureFreeE(
        const VecCl &FreeE_NoMixt,
        double Denc,
        double Temp,
        const VecCl &Veights,
        const VecCl &DissDenc) {
        double ret = 0;
        for(int k = 1; k <= FreeE_NoMixt.Dim(); k++)
            ret += (FreeE_NoMixt[k] + SingleDiss_MinimFunc::GetMixtureCorrection(
                                          DissDenc[k], Temp, Veights[k])) *
                   DissDenc[k];
        return ret / Denc;
    }
};


// Base class for next minimization functions
struct ChemMultiMinimBase {
    //Used by RelatVolumeMinimFunc
    virtual VecCl ChemMulti_FindPressures(
        double Denc,
        double Temp,
        const VecCl &RelatNumbers,
        const VecCl &RelatVolumesCoefs) = 0;

    //Used by ReactCoefMinimFunc
    //virtual double ChemMulti_FreeE(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs)=0;
    //   virtual VecCl ChemMulti_ClcRelatNumbers(const VecCl &ReactCoef)=0;
};
// minimazing volumes.
struct RelatVolumeMinimFunc : RefCount {
    ChemMultiMinimBase *FreeClc;
    double Denc, Temp;
    static VecCl RelatVolumesCoefs;
    VecCl RelatNumbers;   //, RelatVolumesCoefs;
    static RelatVolumeMinimFunc *FunPtr;
    RelatVolumeMinimFunc *OldPtr;
    vector<int> VolMinimCvt;
    RelatVolumeMinimFunc(
        ChemMultiMinimBase *freeclc,
        double denc,
        double temp,
        const VecCl &relatnumbers,
        const VecCl &relatvolumescoefs)
        : FreeClc(freeclc), Denc(denc), Temp(temp), RelatNumbers(relatnumbers) {
        if(RelatVolumesCoefs.Dim() != relatvolumescoefs.Dim())
            RelatVolumesCoefs = relatvolumescoefs;
        RelatNumbers = RelatNumbers / sqrt(RelatNumbers * RelatNumbers);
        VolMinimCvt.clear();
        for(int i = 1; i <= RelatNumbers.Dim(); i++) {
            //if (RelatNumbers[i]>1e-15)
            VolMinimCvt.push_back(i);
            RelatNumbers[i] = max<double>(pow(MathZer, 0.25), RelatNumbers[i]);
        }
        OldPtr = FunPtr;
        FunPtr = this;
    };
    ~RelatVolumeMinimFunc() {
        FunPtr = OldPtr;
        RelatVolumesCoefs.SetDim(0);
    };
    static void ExecuteStatic(const VecCl &par, VecCl &res) {
        FunPtr->Execute(par, res);
    }
    void Execute(const VecCl &par, VecCl &res) {
        VecCl par_(par);
        for(int k = 1; k <= par.Dim(); k++)
            par_[k] = fabs(par[k]) > 50 ? (par[k] > 0 ? 50 : -50) : par[k];
        //par_[k] = fabs(par[k])>70?(par[k]>0?70:-70):par[k];
        par_ = Decode(par_);
        res = FreeClc->ChemMulti_FindPressures(Denc, Temp, RelatNumbers, par_) +
              ToAddPressure;
        res = EncodeRes(res);
        //cout<<"par "<<par<<" res "<<res<<"\n";
    }
    VecCl Encode(const VecCl &par) {
        VecCl res((int)VolMinimCvt.size());
        for(size_t i = 0; i < VolMinimCvt.size(); i++)
            res[(int)i + 1] = par[VolMinimCvt[i]];
        //res[VolMinimCvt[i]] = par[i+1];
        for(int k = 1; k <= res.Dim(); k++)
            res[k] = log(res[k]);
        return res;
    }
    VecCl Decode(const VecCl &par) {
        VecCl res(par.Dim());
        for(int k = 1; k <= par.Dim(); k++)
            res[k] = fabs(par[k]) > log(1 / MathZer)
                         ? (par[k] > 0 ? 1 / MathZer : MathZer)
                         : exp(par[k]);
        //              res[k] = exp(par[k]);
        VecCl res1 = RelatVolumesCoefs;
        for(size_t i = 0; i < VolMinimCvt.size(); i++)
            res1[VolMinimCvt[i]] = res[(int)i + 1];
        return res1;
    }
    VecCl EncodeRes(const VecCl &par) {
        if(par.Dim() == 1)
            return 0;
        VecCl res_((int)VolMinimCvt.size());
        for(size_t i = 0; i < VolMinimCvt.size(); i++)
            res_[(int)i + 1] = par[VolMinimCvt[i]];
        VecCl res(res_.Dim() - 1), par_;
        double minim = VecCl::Min(res_);
        if(minim <= 0)
            par_ = res_ + (-minim + 1);
        else
            par_ = res_;
        for(int k = 1; k <= res.Dim(); k++)
            res[k] = 10 * log(par_[k + 1] / par_[k]);
        //sqr( (par_[k+1]- par_[k]) );
        //res[res.Dim()] = sqr( 0.1*log(par_[1]/par_[res.Dim()]) )+
        //    sqr( 10*(par_[1]- par_[res.Dim()]) );
        return res;
    }
    int Static1D_N;
    VecCl Static1D_Par;

    double OneDMinimmize(double x) {
        Static1D_Par[Static1D_N] = x;
        VecCl res;
        Execute(Static1D_Par, res);
        return res[Static1D_N];
    };
    static double OneDMinimmizeStatic(double x) {
        return FunPtr->OneDMinimmize(x);
    }
    VecCl Minimize1D(const VecCl &MinimPar) {
        VecCl res;
        Static1D_Par = MinimPar;
        for(Static1D_N = 1; Static1D_N < MinimPar.Dim(); Static1D_N++) {
            double cv = MinimPar[Static1D_N];
            Fzero(OneDMinimmizeStatic, cv - 10, cv + 10, cv, 1e-7, 1e-7, 1000);
        }
        return Static1D_Par;
    }
    VecCl MinimizeND(const VecCl &MinimPar_) {
        VecCl y = RelatNumbers * 0, w, MinimPar = MinimPar_;
        y = EncodeRes(y);
        w = y * 0 + 1;
        double FuncError = 1e-4, ResError = FuncError;
        double Min1D_Coef = 1.2;
        int Min1D_MaxLinSrch = 5, Min1D_MaxRetry = 5;
        MinFindNDimClass MinNd(
            new EigenVectMinim(
                new Derive2DHi2(w, y, ExecuteStatic),
                new MinFind1DClass(
                    NULL,
                    ResError,
                    FuncError,
                    Min1D_Coef,
                    Min1D_MaxLinSrch,
                    Min1D_MaxRetry),
                1e-8,
                1e-15,
                FuncError,
                2),
            ResError);
        VecCl MinimStp(MinimPar.Dim());
        MinimStp = MinimStp * 0 + 0.2;
        int Cont = 1, MaxIt = 15, k;
        double FoundMisfit;
        for(k = 1; ((k <= MaxIt) && Cont); k++)
            Cont = MinNd.Execute(MinimPar, MinimStp, -1, FoundMisfit);
        return MinimPar;
    }

    double ToAddPressure;
    VecCl Minimize() {
        if(VolMinimCvt.size() == 1) {
            //cout<<"One only "<<RelatNumbers;
            return RelatVolumesCoefs * 0 + 1;
        }
        VecCl RelatVol = RelatVolumesCoefs;
        ToAddPressure = 0;
        VecCl tmp = FreeClc->ChemMulti_FindPressures(Denc, Temp, RelatNumbers, RelatVol);
        double mi = VecCl::Min(tmp);
        ToAddPressure = mi > 0 ? 0 : (-mi + 5) * 20;

        double StartT = Temp, T = Temp;
        int i;
        for(i = 1; i <= 20; i++)
            if(MinimizeTemp(T, RelatVol)) {
                //if (i>1)
                //    PrintErr(Encode(RelatVol));
                break;
            } else {
                T = T * 1.1 + 5;
                RelatVol = RelatVolumesCoefs;
            }
        if(i >= 20)
            PrintErr("Too many iterations ", Encode(RelatVol));
        else if(i > 1) {
            //cout<<"Temp+ found good, trying to find exact temp...\n";
            //PrintErr(Encode(RelatVol));
            T = StartT;
            Temp = StartT;
            //PrintErr(Encode(RelatVol));
            VecCl Best = RelatVol;
            if(MinimizeTemp(T, Best) || MinimizeTemp(T, Best) || MinimizeTemp(T, Best))
                RelatVol = Best;
            else {
                VecCl par;
                double err;
                TestOK(Encode(RelatVol), Encode(Best), par, err);
                RelatVol = Decode(par);
                PrintErr("Could not recalculate for start temp", Encode(RelatVol));
            }
            //cout<<"found ::\n";
            //PrintErr(Encode(Best));
        }
        Temp = StartT;
        RelatVol = RelatVol / sqrt(RelatVol * RelatVol);
        RelatVolumesCoefs = RelatVol;
        return RelatVol;
    }
    int MinimizeTemp(double T, VecCl &RelatVol) {
        Temp = T;
        VecCl MinimStart = Encode(RelatVol), MinimNew;
        double ErrStart;
        MinimNew = MinimizeND(MinimStart);
        if(TestOK(MinimStart, MinimNew, MinimStart, ErrStart)) {
            RelatVol = Decode(MinimStart);
            //cout<<"\n1OK\n";
            //cout<<" OK "<<ErrStart<<" T "<<Temp<<" Denc "<<Denc<<" par "<<RelatVol;
            //TestOK(MinimStart,ErrStart);
            //cout<<" Retested res "<<ErrStart<<"\n";
            return 1;
        }
        MinimNew = Minimize1D(MinimStart);
        if(TestOK(MinimStart, MinimNew, MinimStart, ErrStart)) {
            RelatVol = Decode(MinimStart);
            //cout<<"\n2OK\n";
            //cout<<" OK "<<ErrStart<<" T "<<Temp<<" Denc "<<Denc<<" par "<<RelatVol;
            //TestOK(MinimStart,ErrStart);
            //cout<<" Retested res "<<ErrStart<<"\n";
            return 1;
        }
        MinimNew = MinimizeND(MinimStart);
        if(TestOK(MinimStart, MinimNew, MinimStart, ErrStart)) {
            RelatVol = Decode(MinimStart);
            //cout<<"\n3OK\n";
            //cout<<" OK "<<ErrStart<<" T "<<Temp<<" Denc "<<Denc<<" par "<<RelatVol;
            //TestOK(MinimStart,ErrStart);
            //cout<<" Retested res "<<ErrStart<<"\n";
            return 1;
        }
        MinimNew = MinimizeND(MinimStart);
        if(TestOK(MinimStart, MinimNew, MinimStart, ErrStart) || ErrStart < 1e-4) {
            RelatVol = Decode(MinimStart);
            //cout<<"\n4OK\n";
            //cout<<" OK "<<ErrStart<<" T "<<Temp<<" Denc "<<Denc<<" par "<<RelatVol;
            //TestOK(MinimStart,ErrStart);
            //cout<<" Retested res "<<ErrStart<<"\n";
            return 1;
        }
        RelatVol = Decode(MinimStart);
        return 0;
    }
    void PrintErr(int Start = 1) {
        //#ifdef _DEBUG
        if(Start) {
            cout << " Very bad : \n";
            cout << "Denc	" << Denc << " Temp	" << Temp << "\n";
        } else
            cout << " \n\n\n";
        //#endif
    }
    void PrintErr(const char *help, const VecCl &par) {
        //#ifdef _DEBUG
        VecCl RelVolCoef = Decode(par);
        double Mist;
        TestOK(par, Mist);
        cout << " Very bad : " << help << "\n";
        cout << "Denc	" << Denc << " Temp	" << Temp << "\n";
        VecCl P = FreeClc->ChemMulti_FindPressures(Denc, Temp, RelatNumbers, RelVolCoef);
        cout << " Mistake " << Mist << "\n";
        cout << "Relat pres " << P;
        cout << "Relat volume	" << RelVolCoef;
        cout << "Relat Numbers " << RelatNumbers << "\n";
        cout << " \n\n\n";
        //#endif
    }
    int TestOK(const VecCl &par1, const VecCl &par2, VecCl &par, double &Mist) {
        double mist1, mist2;
        TestOK(par1, mist1);
        TestOK(par2, mist2);
        if(mist1 < mist2)
            par = par1;
        else
            par = par2;
        Mist = min(mist1, mist2);
        if(Mist < 1e-5)
            return 1;
        return 0;
    }

    int TestOK(const VecCl &par, double &Mist) {
        VecCl RelVolCoef = Decode(par);
        VecCl P = FreeClc->ChemMulti_FindPressures(Denc, Temp, RelatNumbers, RelVolCoef);
        VecCl test = EncodeRes(P);
        //double c = VecCl::ModMax(P);
        //for(int i=1;i<=P.Dim();i++){
        //    P[i] = log(fabs(P[i]/c));
        //}
        //Mist = sqrt(P*P);
        Mist = sqrt(test * test);
        if(Mist > 1e-4)
            return 0;
        return 1;
    }
};

//======================================================================================
//============================  Finding freeE for dissosiation =========================
//===================  base class for using in main FreeEChemMatterMultiV ==============
//======================================================================================
struct ChemReactMinimFreeE_Base : SavableClass {
    //MatrCl ChemReact;
    //VecCl StartSubstCoef, SubstVeights;
    //DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
    //   VecCl RelatVolumeCoef,SubstMixtures;
    virtual void Init(
        const VecCl &substveights,
        const DataVector<Ref<InterfaceFreeEIO>, CopyStructRef<Ref<InterfaceFreeEIO>>>
            &chemsubst,
        const VecCl &substmixtures,
        const VecCl &relatvolumecoef,
        const MatrCl &chemreact,
        const VecCl &startsubstcoef) = 0;
    //{
    //	SubstVeights = substveights;
    //	ChemSubst = chemsubst;
    //	SubstMixtures = substmixtures;
    //	RelatVolumeCoef = relatvolumecoef;
    //	ChemReact = chemreact;
    //	StartSubstCoef = startsubstcoef;
    //}
    virtual VecCl GetResultRelatMasses() = 0;
    virtual VecCl GetResultVolumeCoef() = 0;
    virtual VecCl GetResultPressures() = 0;
    virtual double ClcFreeE(double Denc, double Temp) = 0;
};
// Class for minimizing FreeE of substances with dissosiation
struct DissMultiV : ChemReactMinimFreeE_Base, ChemMultiMinimBase {
    VecCl SubstVeights;
    DataVector<Ref<InterfaceFreeEIO>, CopyStructRef<Ref<InterfaceFreeEIO>>> ChemSubst;
    VecCl StartRelatVolumes;

    DissMultiV() : ChemReactMinimFreeE_Base(), ChemMultiMinimBase(), MinimizeVolumes(1){};

    virtual void Init(
        const VecCl &substveights,
        const DataVector<Ref<InterfaceFreeEIO>, CopyStructRef<Ref<InterfaceFreeEIO>>>
            &chemsubst,
        const VecCl &,
        const VecCl &relatvolumecoef,
        const MatrCl &,
        const VecCl &) {
        SubstVeights = substveights;
        ChemSubst = chemsubst;
        StartRelatVolumes = relatvolumecoef;
        //SubstMixtures = substmixtures;
        //ChemReact = chemreact;
        //StartSubstCoef = startsubstcoef;
    }
    //static size_t CallNum;
    VecCl ChemMulti_FindPressures(
        double Denc,
        double Temp,
        const VecCl &RelatNumbers,
        const VecCl &RelatVolumesCoefs) {
        VecCl SubstDenc = ClcSubstDenc(Denc, RelatNumbers, RelatVolumesCoefs);
        VecCl pres(SubstVeights.Dim());
        for(int k = 1; k <= pres.Dim(); k++) {
            pres[k] = ChemSubst[k - 1]->Pressure(SubstDenc[k], Temp);
        }
        //cout<<"CallNum "<<CallNum++<<"\n";
        return pres;
    }
    // They are not used...
    //double ChemMulti_FreeE(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
    //	return 0;
    //}
    //VecCl ChemMulti_ClcRelatNumbers(const VecCl &ReactCoef){
    //	return VecCl();
    //}


    VecCl ClcSubstDenc(
        double Denc,
        const VecCl &RelatNumbers,
        const VecCl &RelatVolumesCoefs) {
        double Coef =
            (RelatNumbers * RelatVolumesCoefs) / (RelatNumbers * SubstVeights) * Denc;
        VecCl tmp(RelatVolumesCoefs);
        VecCl SubstDenc = VecCl::ComponentDiv(SubstVeights, tmp) * Coef;
        return SubstDenc;
    }
    VecCl ClcMassCoef(const VecCl &RelatNumbers) {
        double Coef = RelatNumbers * SubstVeights;
        VecCl MassCoef = VecCl::ComponentMul(RelatNumbers, SubstVeights) / Coef;
        return MassCoef;
    }
    void MinimRelatVolume(
        double Denc,
        double Temp,
        const VecCl &RelatNumbers,
        VecCl &RelatVolumesCoefs) {
        RelatVolumeMinimFunc func(this, Denc, Temp, RelatNumbers, RelatVolumesCoefs);
        RelatVolumesCoefs = func.Minimize();
    }

    VecCl ClcFreeEStatic(
        double Denc,
        double Temp,
        const VecCl &RelatNumbers,
        const VecCl &RelatVolumesCoefs) {
        VecCl SubstDenc = ClcSubstDenc(Denc, RelatNumbers, RelatVolumesCoefs);
        VecCl MassCoef = ClcMassCoef(RelatNumbers);
        VecCl ret = SubstVeights * 0;
        for(int k = 1; k <= SubstVeights.Dim(); k++) {
            ret[k] = ChemSubst[k - 1]->FreeE(SubstDenc[k], Temp);
            ret[k] -= SingleDiss_MinimFunc::GetMixtureCorrection(
                SubstDenc[k], Temp, SubstVeights[k]);
        }
        return ret;
    }
    VecCl ClcFreeEStatic_Test(
        double Denc,
        double Temp,
        const VecCl &RelatNumbers,
        const VecCl &RelatVolumesCoefs) {
        VecCl SubstDenc = ClcSubstDenc(Denc, RelatNumbers, RelatVolumesCoefs);
        VecCl MassCoef = ClcMassCoef(RelatNumbers);
        VecCl ret = SubstVeights * 0;
        for(int k = 1; k <= SubstVeights.Dim(); k++) {
            ret[k] = ChemSubst[k - 1]->FreeE(Denc, Temp);
        }
        return ret;
    }
    double MinimDissFreeE(
        double Denc,
        double Temp,
        VecCl &RelatNumbers,
        const VecCl &RelatVolumesCoefs) {
        MatrDiss_MinimFunc func(SubstVeights);
        VecCl FeS = ClcFreeEStatic(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
        VecCl DissDenc = func.FindDissDenc(Denc, Temp, FeS);
        RelatNumbers = VecCl::ComponentDiv(DissDenc, SubstVeights);
        //FeS = ClcFreeEStatic(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
        //VecCl test = func.FindDissDenc(Denc, Temp, FeS);
        //cout<<"was "<<DissDenc;
        //cout<<" difference "<<test-DissDenc;
        return MatrDiss_MinimFunc::GetMixtureFreeE(
            FeS, Denc, Temp, SubstVeights, DissDenc);
    };
    //double ToAddPressure;
    void ClcStart(
        VecCl &RelatNumbers,
        VecCl &RelatVolumeCoefs,
        double Denc,
        double Temp) {
        RelatNumbers = SubstVeights * 0 + 1;
        RelatVolumeCoefs = StartRelatVolumes;
        //ToAddPressure=0;
        //VecCl tmp = ChemMulti_FindPressures(Denc,Temp,RelatNumbers,RelatVolumeCoefs);
        //double mi = VecCl::Min(tmp);
        //ToAddPressure = mi>0?0:(-mi+5)*20;

        //RelatVolumeCoefs =  SubstVeights;
        //RelatVolumeCoefs = VecCl::ComponentDiv(SubstVeights*0 +1, SubstVeights);
    }
    double ClcFreeE(double Denc, double Temp) {
        VecCl RelatNumbers, RelatVolumesCoefs;
        ClcStart(RelatNumbers, RelatVolumesCoefs, Denc, Temp);
        if(MinimizeVolumes)
            MinimRelatVolume(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
        double resFreeE = MinimDissFreeE(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
        if(MinimizeVolumes) {
            MinimRelatVolume(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
            resFreeE = MinimDissFreeE(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
            MinimRelatVolume(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
            resFreeE = MinimDissFreeE(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
            StartRelatVolumes = RelatVolumesCoefs;
        }
#ifdef _DEBUG
        VecCl P = ChemMulti_FindPressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
        double Mist = sqrt((P - P[1]) * (P - P[1])) / sqrt(P * P);
        if(Mist > 1e-5) {
            cout << "Denc	" << Denc << " Temp	" << Temp << " Mistake " << Mist << "\n";
            cout << "Relat pres "
                 << ChemMulti_FindPressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
            cout << "Relat volume	" << RelatVolumesCoefs;
            cout << "Relat Numbers " << RelatNumbers;
            cout << "	 LastFreeE	" << resFreeE << " : "
                 << ClcFreeEStatic_Test(Denc, Temp, RelatNumbers, RelatVolumesCoefs)
                 << "\n";
        }
#endif
        ResultRelatMasses = ClcMassCoef(RelatNumbers);
        ResultVolumesCoefs = RelatVolumesCoefs / RelatVolumesCoefs[1];
        ResultPressures =
            ChemMulti_FindPressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
        return resFreeE;
    }
    virtual VecCl GetResultVolumeCoef() {
        return ResultVolumesCoefs;
    }
    virtual VecCl GetResultRelatMasses() {
        return ResultRelatMasses;
    }
    virtual VecCl GetResultPressures() {
        return ResultPressures;
    }


    int read_data_state(FilterIn &si) {
        char tmp[256];
        si >> tmp >> MinimizeVolumes;
        if(!si)
            throw info_except("Could not read MinimizeVolumes=%i\n", MinimizeVolumes);
        return ChemReactMinimFreeE_Base::read_data_state(si);
    }
    int save_data_state(FilterOut &so) {
        so << " MinimizeVolumes " << MinimizeVolumes;
        if(!so)
            throw info_except("Could not save class\n");
        return ChemReactMinimFreeE_Base::save_data_state(so);
    }

private:
    int MinimizeVolumes;
    VecCl ResultRelatMasses, ResultVolumesCoefs, ResultPressures;
};

// Main ChemReact EOS class!!!!
struct FreeEChemMatterMultiV : FreeEIOFind {
    //FreeEChemMatterMultiV():FreeEIOFind("material.cfg","MatterChemH2Met"),FreeMinim(new DissMultiV){
    //	SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),2,1,1,1);
    //	SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,0,1,1);
    //	ChemEq eq(Stroka("M1"),-1);eq.AddSubst(Stroka("M2"),1);
    //	ChemEqs.push_back(eq);
    //	ClcStartParams();
    //}
    FreeEChemMatterMultiV();
    Ref<ChemReactMinimFreeE_Base> FreeMinim;

    void CheckReactCoef(
        MatrCl &ChemReact,
        VecCl &SubstVeights,
        DataVector<Ref<InterfaceFreeEIO>, CopyStructRef<Ref<InterfaceFreeEIO>>>
            &ChemSubst) {
        for(int k = 1; k <= ChemReact.DimX; k++) {
            double Res = MatrCl::GetRow(ChemReact, k) * SubstVeights;
            if(fabs(Res) > MathZer) {
                FilterTextOut out("tmp", DataSource::Memory);
                out << "\nBad chem react " << k << " result is: " << Res << "\n";
                out << "Vector Of Chem React Coef " << MatrCl::GetRow(ChemReact, k);
                out << "Vector Subst Veights      " << SubstVeights << "Matters:\n";
                for(int i = 0; i < ChemReact.DimY; i++)
                    out << dynamic_cast<FreeEIOFind *>((InterfaceFreeEIO *)ChemSubst[i])
                               ->Finder.Substance
                        << " ";
                out << "\nNames\n";
                for(map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
                    iter != SubstMap.end();
                    iter++, k++)
                    out << iter->first.c_str() << " ";
                out << "\n";
                throw info_except(DataSource::GetStr("tmp").c_str());
            }
        }
    }
    void ClcStartParams() {
        MatrCl ChemReact;
        VecCl StartSubstCoef, SubstVeights;
        DataVector<Ref<InterfaceFreeEIO>, CopyStructRef<Ref<InterfaceFreeEIO>>> ChemSubst;
        VecCl RelatVolumeCoef, SubstMixtures;

        int NumReact = (int)ChemEqs.size(), NumSubst = (int)SubstMap.size();
        ChemSubst.SetDim(NumSubst);
        SubstVeights.SetDim(NumSubst);
        RelatVolumeCoef.SetDim(NumSubst);
        StartSubstCoef.SetDim(NumSubst);
        SubstMixtures.SetDim(NumSubst);
        int k = 0;
        for(map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
            iter != SubstMap.end();
            iter++, k++) {
            ChemSubst[k] = iter->second.Subst;
            SubstInd[iter->first] = k;
            SubstVeights[k + 1] = iter->second.MolVeight;
            StartSubstCoef[k + 1] = iter->second.StartProportion;
            RelatVolumeCoef[k + 1] = iter->second.RelatVolumeCoef;
            SubstMixtures[k + 1] = iter->second.SubstMixtures;
        }
        ChemReact.SetMatr(NumReact, NumSubst);
        ChemReact = ChemReact * 0.;
        for(int i = 1; i <= NumReact; i++) {
            for(int j = 0; j < (int)ChemEqs[i - 1].SubstName.size(); j++) {
                ChemReact(i, SubstInd[ChemEqs[i - 1].SubstName[j]] + 1) =
                    ChemEqs[i - 1].SubstCoef[j];
            }
        }
        CheckReactCoef(ChemReact, SubstVeights, ChemSubst);
        FreeMinim->Init(
            SubstVeights,
            ChemSubst,
            SubstMixtures,
            RelatVolumeCoef,
            ChemReact,
            StartSubstCoef);
        SetInternalNames();
    }
    virtual void SetInternalNames() {
        FreeEIOFind::SetInternalNames();
        int k = 1;
        for(map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
            iter != SubstMap.end();
            iter++, k++) {
            OutputNames[iter->first + "_M"] = &k;
            OutputNames[iter->first + "_V"] = &k;
            OutputNames[iter->first + "_P"] = &k;
        }
        //#ifdef _DEBUG
        //        FilterTextOut out("aa",DataSource::Memory);
        //		save_data_state(out);out<<"\n";
        //        SaveSubstancePar(out);out<<"\n=============================\n";
        //		cout<<DataSource::GetStr("aa").c_str();
        //#endif
    }
    virtual double OutputValClc(const map<Stroka, double> &Input, const Stroka &Output) {
        void *ptr;
        if(CheckPresence(OutputNames, Output, ptr) && ptr != NULL) {
            FreeE(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"));
            int k = SubstInd[Stroka(Output, 0, Output.length() - 2)];
            if(Stroka(Output, Output.length() - 1, Output.length()) == Stroka("V"))
                return FreeMinim->GetResultVolumeCoef()[k + 1];
            if(Stroka(Output, Output.length() - 1, Output.length()) == Stroka("M"))
                return FreeMinim->GetResultRelatMasses()[k + 1];
            if(Stroka(Output, Output.length() - 1, Output.length()) == Stroka("P"))
                return FreeMinim->GetResultPressures()[k + 1];
        }

        return FreeEIOFind::OutputValClc(Input, Output);
    }
    double FreeE(double Denc, double T) {
        if(!FreeMinim)
            throw info_except("Minimizing class is not inited...\n");
        return FreeMinim->ClcFreeE(Denc, T);
    }
    void ReadSubstancePar(istream &in) {
        char tmp[256];
        in >> tmp;
        FreeMinim << SavableClass::Read(in);
        in >> tmp >> tmp;
        NamedSubst sbst;
        SubstMap.clear();
        ChemEqs.clear();
        while(sbst.Read(in)) {
            SubstMap[sbst.Name] = sbst;
            in >> tmp;
            if(Stricmp(tmp, "}") == 0)
                break;
            if(Stricmp(tmp, ";") != 0)
                throw info_except(
                    "Read substance %s - expected '; } ' after it but got %s\n",
                    sbst.Name.c_str(),
                    tmp);
        }

        in >> tmp >> tmp;
        ChemEq eq;
        while(eq.Read(in)) {
            ChemEqs.push_back(eq);
        }
        ClcStartParams();
    }
    void SaveSubstancePar(ostream &out) {
        out << " EnterMinimClass ";
        SavableClass::Save(out, FreeMinim);
        out << " EnterList_SubsName_FreeESubstance_SubsMolVeight { ";
        for(map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
            iter != SubstMap.end();
            iter++) {
            iter->second.Save(out);
            out << " ; ";
        }
        out << " } \n EnterListOfVectors_Vector_SubsName_SubsCoef { ";
        for(size_t k = 0; k < ChemEqs.size(); k++)
            ChemEqs[k].Save(out);
        out << " } \n";
    }

private:
    struct NamedSubst {
        Stroka Name;
        Ref<InterfaceFreeEIO> Subst;
        double MolVeight, StartProportion;
        double RelatVolumeCoef, SubstMixtures;
        NamedSubst(const NamedSubst &sub)
            : Name(sub.Name),
              Subst(sub.Subst),
              MolVeight(sub.MolVeight),
              StartProportion(sub.StartProportion){};
        NamedSubst()
            : Name(""),
              Subst(NULL),
              MolVeight(0),
              StartProportion(0),
              RelatVolumeCoef(1){};
        NamedSubst(
            const Stroka &name,
            InterfaceFreeEIO *subst,
            double molveight,
            double startproportion,
            double relatvolumecoef,
            int substmixtures)
            : Name(name),
              Subst(subst),
              MolVeight(molveight),
              StartProportion(startproportion),
              RelatVolumeCoef(relatvolumecoef),
              SubstMixtures(substmixtures){};
        int Read(istream &in) {
            char tmp[256];
            in >> tmp;
            if(Stricmp(tmp, "}") == 0)
                return 0;
            in >> Name >> tmp >> MolVeight >> tmp >> StartProportion >> tmp >>
                RelatVolumeCoef >> tmp >> SubstMixtures >> tmp;
            if(!(Subst << SavableClass::Read(in)))
                throw info_except(
                    "Could not convert matter with Name %s to InterfaceFreeEIO\n",
                    Name.c_str());
            return 1;
        }
        void Save(ostream &out) {
            out << " Name " << Name.c_str() << " MolVeight " << MolVeight
                << " StartProportion " << StartProportion << " RelatVolumeCoef "
                << RelatVolumeCoef << " SubstMixtures " << SubstMixtures
                << " MatterFreeE ";
            SavableClass::Save(out, Subst);
        }
    };
    struct ChemEq {
        vector<Stroka> SubstName;
        vector<double> SubstCoef;
        ChemEq(){};
        ChemEq(const ChemEq &eq) : SubstName(eq.SubstName), SubstCoef(eq.SubstCoef){};
        ChemEq(const Stroka &substname, double substcoef) {
            AddSubst(substname, substcoef);
        };
        void AddSubst(const Stroka &substname, double substcoef) {
            SubstName.push_back(substname);
            SubstCoef.push_back(substcoef);
        };
        int Read(istream &in) {
            SubstName.clear();
            SubstCoef.clear();
            char tmp[256];
            in >> tmp;
            if(Stricmp(tmp, "}") == 0)
                return 0;
            if(Stricmp(tmp, "{") != 0)
                throw info_except("Expected { and got %s\n", tmp);
            in >> tmp;
            while(Stricmp(tmp, "}") != 0) {
                Stroka Name = tmp;
                SubstName.push_back(tmp);
                double d;
                in >> d;
                SubstCoef.push_back(d);
                in >> tmp;
            }
            return 1;
        }
        void Save(ostream &out) {
            out << " { ";
            for(size_t k = 0; k < SubstName.size(); k++)
                out << "  " << SubstName[k].c_str() << " " << SubstCoef[k] << "    ";
            out << " }\n";
        }
    };
    map<Stroka, NamedSubst> SubstMap;
    vector<ChemEq> ChemEqs;
    map<Stroka, int> SubstInd;
};


//=======================================================================================
//======================   Dissosiation   ===============================================
//=======================================================================================


// Check it before usage...
// Main ChemReact EOS class!!!!
struct FreeEChemMatterDiss : FreeEIOFind {
    //FreeEChemMatterMultiV():FreeEIOFind("material.cfg","MatterChemH2Met"),FreeMinim(new DissMultiV){
    //	SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),2,1,1,1);
    //	SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,0,1,1);
    //	ChemEq eq(Stroka("M1"),-1);eq.AddSubst(Stroka("M2"),1);
    //	ChemEqs.push_back(eq);
    //	ClcStartParams();
    //}
    FreeEChemMatterDiss()
        : FreeEIOFind("material.cfg", "MatterChemH2Diss"),
          FinalState(new FreeEIdeal()),
          StartState(new FreeEIdeal()),
          FinalCoef(new OneVarFormula()){};
    double FreeE(double Denc, double T) {
        double coef = FinalCoef->Calculate(Denc);
        //cout<<Denc<<" "<<coef<<"\n";
        if(coef == 0)
            return FinalState->FreeE(Denc, T);
        if(coef == 1)
            return StartState->FreeE(Denc, T);
        if(!In_Lim(coef, 0, 1, 1))
            throw info_except("Bad coef %e Denc %e Temp %e\n", coef, Denc, T);
        double Final = FinalState->FreeE(Denc, T);
        double Start = StartState->FreeE(Denc, T);
        return (1 - coef) * Final + coef * Start;
    }
    void ReadSubstancePar(istream &in) {
        char tmp[256];
        in >> tmp;
        StartState << SavableClass::Read(in);
        in >> tmp;
        FinalState << SavableClass::Read(in);
        in >> tmp;
        FinalCoef << SavableClass::Read(in);
        if(!FinalState || !in || !FinalCoef || !StartState)
            throw info_except("Could not read final state or final coef\n");
    }

    void SaveSubstancePar(ostream &out) {
        out << " StartState ";
        SavableClass::Save(out, StartState);
        out << " FinalState ";
        SavableClass::Save(out, FinalState);
        out << " FinalCoef ";
        SavableClass::Save(out, FinalCoef);
    }
    Stroka MakeHelp() {
        return Stroka("Not tested yet. Caution. Errors possible...\n") +
               FreeEIOFind::MakeHelp();
    }

private:
    Ref<InterfaceFreeEIO> FinalState, StartState;
    Ref<OneVarFunction> FinalCoef;
};


//// minimazing chem coefs. Old var - not really working...
//struct ReactCoefMinimFunc:RefCount{
//    double Denc,Temp;
//    ChemMultiMinimBase *FreeClc;
//    VecCl ReactCoefs, RelatVolumesCoefs;
//    static ReactCoefMinimFunc *FunPtr;
//    ReactCoefMinimFunc *OldPtr;
//    ReactCoefMinimFunc(ChemMultiMinimBase *freeclc, double denc, double temp,
//        const VecCl &relatvolumescoefs)
//        :FreeClc(freeclc), Denc(denc), Temp(temp),
//        RelatVolumesCoefs(relatvolumescoefs){
//            OldPtr = FunPtr;FunPtr = this;
//        };
//    ~ReactCoefMinimFunc(){FunPtr = OldPtr;};
//    static double ExecuteStatic(VecCl &par){
//        return FunPtr->Execute(par);
//    }
//    double Execute(VecCl &par){
//        ReactCoefs = Decode(par);
//		double ret = FreeClc->ChemMulti_FreeE(Denc, Temp, FreeClc->ChemMulti_ClcRelatNumbers(ReactCoefs), RelatVolumesCoefs);
////cout<<ret<<" : "<<par;
//        return ret;
//    }
//    VecCl Encode(const VecCl &ReactCoef){
//        VecCl par = ReactCoef;
//        for(int k=1;k<=par.Dim();k++){
//            par[k] = par[k]<=MathZer?MathZer:(par[k]>=1-MathZer?1-MathZer:par[k]);
//            par[k] = log(par[k]/(1-par[k]));
//        };
//        return par;
//    };
//    VecCl Decode(const VecCl &par){
//        VecCl ReactCoef = par;
//        for(int k=1;k<=par.Dim();k++) {
//            ReactCoef[k] = 1/(1+exp(-par[k]));
//            if (fabs(ReactCoef[k])<1e-17)
//                ReactCoef[k] = 0;
//        }
//        return ReactCoef;
//    };
//    VecCl MinimizeReactCoef(const VecCl &reactcoefs){
//        ReactCoefs = reactcoefs ;
//
//        double FuncError=1e-4, ResError=FuncError;
//        double Min1D_Coef=1.2;
//        int Min1D_MaxLinSrch=5,Min1D_MaxRetry=5;
//        MinFindNDimClass MinNd(
//            new EigenVectMinim(new Derive2DStd(new DeriveStaticFunc(ExecuteStatic)),
//                                new MinFind1DClass(NULL,ResError,FuncError,
//                                    Min1D_Coef,Min1D_MaxLinSrch,Min1D_MaxRetry),
//                                1e-8,1e-15,FuncError,2) ,
//            ResError);
//        VecCl MinimPar = Encode(ReactCoefs), MinimStp(MinimPar.Dim());
//        MinimStp = MinimStp*0 + 1.;
//        int Cont = 1, MaxIt = 15, k;
//        double FoundMisfit;
//        for (k=1;((k<=MaxIt)&&Cont);k++)
//            Cont=MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//        MinimStp = MinimStp*0 + 1.;
//        MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//        //MinimStp = MinimStp*0 + 1.;
//        //MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//        ReactCoefs = Decode(MinimPar);
//        return ReactCoefs;
//    }
//
//    static double ExecuteStatic(double x){
//        return FunPtr->Execute(x);
//    }
//    VecCl StaticEncodeReactCoef ;
//    int StaticN;
//    double Execute(double x){
//        StaticEncodeReactCoef [StaticN] = x;
//        double ret = FreeClc->ChemMulti_FreeE(Denc, Temp, FreeClc->ChemMulti_ClcRelatNumbers(Decode(StaticEncodeReactCoef )), RelatVolumesCoefs);
//        return ret;
//    }
//    VecCl Minimize1DReactCoef(const VecCl &reactcoefs){
//        ReactCoefs = reactcoefs ;
//        StaticEncodeReactCoef = Encode(ReactCoefs);
//        for(int k=1;k<=ReactCoefs.Dim();k++){
//            StaticN = k;
//            double res;
//            StaticEncodeReactCoef[k] = Fmin(ExecuteStatic,-25,25,0,1e-6,res);
//        }
//        ReactCoefs = Decode(StaticEncodeReactCoef);
//        return ReactCoefs;
//    }
//};
//
//

//// Class for minimizing FreeE of substances with chemical reactions. Not really good...
//struct ChemMultiV:ChemReactMinimFreeE_Base,ChemMultiMinimBase{
//
//	ChemMultiV(){};
//	virtual void Init(const VecCl &substveights,
//			const DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &chemsubst,
//			const VecCl &substmixtures, const VecCl &relatvolumecoef, const MatrCl &chemreact,
//			const VecCl &startsubstcoef){
//		SubstVeights = substveights;
//		ChemSubst = chemsubst;
//		SubstMixtures = substmixtures;
//		StartRelatVolumes = relatvolumecoef;
//		ChemReact = chemreact;
//		StartSubstCoef = startsubstcoef;
//	}
//    VecCl SubstVeights;
//	DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
//	VecCl SubstMixtures;
//	VecCl StartRelatVolumes;
//
//    MatrCl ChemReact;
//	VecCl StartSubstCoef;
//
//	VecCl ClcSubstDenc(double Denc,  const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
//		double Coef = (RelatNumbers*RelatVolumesCoefs)/(RelatNumbers*SubstVeights)*Denc;
//		VecCl tmp(RelatVolumesCoefs);
//		VecCl SubstDenc =  VecCl::ComponentDiv(SubstVeights,tmp) * Coef;
//		return SubstDenc;
//	}
//	VecCl ClcMassCoef(const VecCl &RelatNumbers){
//		double Coef = RelatNumbers*SubstVeights;
//		VecCl MassCoef = VecCl::ComponentMul(RelatNumbers, SubstVeights) / Coef;
//		return MassCoef;
//	}
//	VecCl ClcSubstDencMixt(double Denc,  const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs, VecCl &SubstDenc){
//		double Coef = VecCl::ComponentMul(SubstMixtures, RelatNumbers) * RelatVolumesCoefs;
//		VecCl SubsDencMixt = VecCl::ComponentMul(VecCl::ComponentMul(SubstDenc, RelatNumbers), RelatVolumesCoefs)/Coef;
//		return SubsDencMixt;
//	}
//	double FreeEMixt(double Denc, double Temp, double SubstVeight){
//		double kt=M_Rconst/SubstVeight*Temp;
//		Denc = Denc<MathZer?MathZer:Denc;
//		double F=kt*log(Denc);
//		return F;
//	}
//// V[i] = C * N[i] * RelatVolumesCoefs[i]
//	double ChemMulti_FreeE(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
//		VecCl SubstDenc = ClcSubstDenc(Denc,  RelatNumbers, RelatVolumesCoefs);
//		VecCl MassCoef = ClcMassCoef(RelatNumbers);
//		VecCl SubstDencMixt = ClcSubstDencMixt(Denc, RelatNumbers, RelatVolumesCoefs, SubstDenc);
//		double ret = 0;
//		for(int k=1;k<=SubstVeights.Dim();k++){
//			ret += MassCoef[k]*ChemSubst[k-1]->FreeE(SubstDenc[k],Temp);
//			if (SubstMixtures[k]!=0) {
//				ret += MassCoef[k]*( FreeEMixt(SubstDencMixt[k],Temp,SubstVeights[k]) - FreeEMixt(SubstDenc[k],Temp,SubstVeights[k]));
//			}
//		}
//		return ret;
//	}
//	VecCl FreeEVect(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
//		VecCl SubstDenc = ClcSubstDenc(Denc,  RelatNumbers, RelatVolumesCoefs);
//		VecCl MassCoef = ClcMassCoef(RelatNumbers);
//		VecCl SubstDencMixt = ClcSubstDencMixt(Denc, RelatNumbers, RelatVolumesCoefs, SubstDenc);
//		VecCl ret = SubstVeights*0;
//		for(int k=1;k<=SubstVeights.Dim();k++){
//			ret[k] = ChemSubst[k-1]->FreeE(SubstDenc[k],Temp);
//			if (SubstMixtures[k]!=0) {
//				ret[k] += FreeEMixt(SubstDencMixt[k],Temp,SubstVeights[k]) - FreeEMixt(SubstDenc[k],Temp,SubstVeights[k]);
//			}
//		}
//		return ret;
//	}
//	VecCl ChemMulti_FindPressures(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
//		VecCl SubstDenc = ClcSubstDenc(Denc,  RelatNumbers, RelatVolumesCoefs);
//		VecCl pres(SubstVeights.Dim());
//		for (int k=1;k<=pres.Dim();k++){
//			pres[k] = ChemSubst[k-1]->Pressure(SubstDenc[k], Temp);
//		}
//        return pres;
//	}
//    void MinimRelatVolume(double Denc, double Temp, const VecCl &RelatNumbers, VecCl &RelatVolumesCoefs){
//        RelatVolumeMinimFunc func(this,Denc, Temp, RelatNumbers, RelatVolumesCoefs);
//        RelatVolumesCoefs = func.Minimize();
//    }
//    VecCl ChemMulti_ClcRelatNumbers(const VecCl &ReactCoefs){
//		VecCl tmp = ReactCoefs;
//		VecCl RelatNumbers = StartSubstCoef + tmp*ChemReact;
//		return RelatNumbers;
//    };
//    void MinimReactCoef(double Denc, double Temp, VecCl &ReactCoefs, const VecCl &RelatVolumesCoefs){
//        ReactCoefMinimFunc func(this,Denc, Temp, RelatVolumesCoefs);
//        ReactCoefs = func.Minimize1DReactCoef(ReactCoefs);
//        ReactCoefs = func.MinimizeReactCoef(ReactCoefs);
//        ReactCoefs = func.Minimize1DReactCoef(ReactCoefs);
//        ReactCoefs = func.MinimizeReactCoef(ReactCoefs);
//    }
//
//    void ClcStartCoef(VecCl &ReactCoef, double Denc,double T){
//		int NumReact, NumSubst;
//		ChemReact.GetDimMN(NumReact, NumSubst);
//
//		ReactCoef.SetDim(NumReact);
//		ReactCoef = ReactCoef*0+0.33;
//
//		//OneDMinim(ReactCoef,Denc,T);
//	}
//    double ClcFreeE(double Denc, double Temp){
//        VecCl ReactCoefs;
//        VecCl RelatVolumesCoefs(StartRelatVolumes);
//        ClcStartCoef(ReactCoefs, Denc, Temp);
//        MinimReactCoef(Denc,Temp,ReactCoefs,RelatVolumesCoefs);
//        MinimRelatVolume(Denc,Temp,ChemMulti_ClcRelatNumbers(ReactCoefs),RelatVolumesCoefs);
//        MinimReactCoef(Denc,Temp,ReactCoefs,RelatVolumesCoefs);
//#ifdef _DEBUG
//cout<<"Denc "<<Denc<<" Temp "<<Temp<<"\n";
//cout<<"Relat pres "<<ChemMulti_FindPressures(Denc, Temp, ChemMulti_ClcRelatNumbers(ReactCoefs), RelatVolumesCoefs);
//cout<<"Relat volume "<<RelatVolumesCoefs;
//cout<<"Relat Numbers "<<ChemMulti_ClcRelatNumbers(ReactCoefs);
//cout<<"ReactCoef "<<ReactCoefs;
//cout<<"  LastFreeE  "<<ChemMulti_FreeE(Denc,Temp,ChemMulti_ClcRelatNumbers(ReactCoefs),RelatVolumesCoefs)<<" : "<<FreeEVect(Denc, Temp, ChemMulti_ClcRelatNumbers(ReactCoefs), RelatVolumesCoefs)<<"\n";
//#endif
//		ResultRelatNumbers = ChemMulti_ClcRelatNumbers(ReactCoefs);ResultVolumesCoefs = RelatVolumesCoefs;
//        return ChemMulti_FreeE(Denc,Temp,ChemMulti_ClcRelatNumbers(ReactCoefs),RelatVolumesCoefs);
//    }
//	VecCl ResultRelatNumbers,ResultVolumesCoefs;
//	VecCl GetResultRelatNumbers(){return ResultRelatNumbers;}
//	virtual VecCl GetResultVolumeCoef(){return ResultVolumesCoefs;}
//
//
//};
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//


//=======================================================================================
//
//// Calculation working only for ideal gases mixtures, question - if working...
//// It is only minimization part, real function - parameter reading - next class...
//struct ChemEqMinim:RefCount{
//	MatrCl ChemReact;
//	VecCl StartSubstCoef, SubstVeights;
//	DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
//	MatrCl SubstMixtures;
//	ChemEqMinim(MatrCl &chemreact, VecCl &startsubstcoef, VecCl &substveights,
//		DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > >  &chemsubst, MatrCl &substmixtures):
//			ChemReact(chemreact), StartSubstCoef(startsubstcoef), SubstVeights(substveights),
//			ChemSubst(chemsubst), SubstMixtures(substmixtures){
//
//	};
//
//	struct MinFuncClass:DeriveFuncClass{
//		ChemEqMinim *FreeClc;
//		double Denc,T;
//		MinFuncClass(ChemEqMinim *freeclc, double denc, double t)
//			:FreeClc(freeclc), Denc(denc), T(t){};
//		double Execute(VecCl &par){
//			VecCl ReactCoef = Decode(par);
//			double ret = FreeClc->FreeE(ReactCoef,Denc,T);
//			return ret;
//		}
//		static VecCl Encode(const VecCl &ReactCoef){
//			VecCl par = ReactCoef;
//			for(int k=1;k<=par.Dim();k++){
//				par[k] = par[k]<=MathZer?MathZer:(par[k]>=1-MathZer?1-MathZer:par[k]);
//				par[k] = log(par[k]/(1-par[k]));
//			};
//			return par;
//		};
//		static VecCl Decode(const VecCl &par){
//			VecCl ReactCoef = par;
//			for(int k=1;k<=par.Dim();k++) {
//				ReactCoef[k] = 1/(1+exp(-par[k]));
//				if (fabs(ReactCoef[k])<1e-17)
//					ReactCoef[k] = 0;
//			}
//			return ReactCoef;
//		};
//	};
//
//	void CheckReactCoef(){
//		for(int k=1;k<=ChemReact.DimX;k++){
//			double Res = MatrCl::GetRow(ChemReact,k)*SubstVeights;
//			if (fabs(Res)>MathZer)
//				throw info_except("Bad Chem react coefs");
//		}
//	}
//	static ChemEqMinim *StaticPtr;
//	double StaticD,StaticT;
//	VecCl StaticReact;
//	int StaticN;
//
//	static double StaticFreeE(double x){
//		StaticPtr->StaticReact[StaticPtr->StaticN] = x;
//		VecCl tmp = MinFuncClass::Decode(StaticPtr->StaticReact);
//		double res = StaticPtr->FreeE(tmp,StaticPtr->StaticD,StaticPtr->StaticT);
//		return res;
//	}
//	void OneDMinim(VecCl &ReactCoef,double Denc,double T){
//		StaticT = T; StaticD = Denc; StaticPtr = this; StaticReact = MinFuncClass::Encode(ReactCoef);
//		for(int k=1;k<=ReactCoef.Dim();k++){
//			StaticN = k;
//			double res;
//			StaticReact[k]=Fmin(StaticFreeE,-25,25,0,1e-6,res);
//		}
//		ReactCoef = MinFuncClass::Decode(StaticReact);
//	}
//	void ClcStartCoef(VecCl &ReactCoef, double Denc,double T){
//		int NumReact, NumSubst;
//		ChemReact.GetDimMN(NumReact, NumSubst);
//
//		ReactCoef.SetDim(NumReact);
//		ReactCoef = ReactCoef*0+0.5;
//
//		OneDMinim(ReactCoef,Denc,T);
//	}
//	void MinimizeCoef(VecCl &ReactCoef,double Denc,double T){
//		double FuncError=1e-4, ResError=FuncError;
//		double Min1D_Coef=1.2;
//		int Min1D_MaxLinSrch=5,Min1D_MaxRetry=5;
//		MinFindNDimClass MinNd(
//			new EigenVectMinim(new Derive2DStd(new MinFuncClass(this, Denc, T)),
//								new MinFind1DClass(NULL,ResError,FuncError,
//									Min1D_Coef,Min1D_MaxLinSrch,Min1D_MaxRetry),
//								1e-8,1e-15,FuncError,2) ,
//			ResError);
//		VecCl MinimPar = MinFuncClass::Encode(ReactCoef), MinimStp(MinimPar.Dim());
//		MinimStp = MinimStp*0 + 1.;
//		int Cont = 1, MaxIt = 15, k;
//		double FoundMisfit;
//		for (k=1;((k<=MaxIt)&&Cont);k++)
//			Cont=MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//		ReactCoef = MinFuncClass::Decode(MinimPar);
//		OneDMinim(ReactCoef,Denc,T);
//		MinimPar = MinFuncClass::Encode(ReactCoef);
//		MinimStp = MinimStp*0 + 1.;
//		MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//		ReactCoef = MinFuncClass::Decode(MinimPar);
//		OneDMinim(ReactCoef,Denc,T);
//		MinimPar = MinFuncClass::Encode(ReactCoef);
//		MinimStp = MinimStp*0 + 1.;
//		MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//		ReactCoef = MinFuncClass::Decode(MinimPar);
//	}
//	int ClcProportions(const VecCl &ReactCoef, VecCl &SubstDencCoef,double &MeanMiu, VecCl &SubstAtom){
//		VecCl tmp = ReactCoef;
//		SubstAtom = StartSubstCoef + tmp*ChemReact;
//		SubstDencCoef = VecCl::ComponentMul(SubstAtom,SubstVeights);
//		SubstDencCoef = SubstDencCoef/sqrt(SubstDencCoef*SubstDencCoef);
//		if (VecCl::ModMax(VecCl::fabs(SubstDencCoef)-SubstDencCoef)>MathZer)
//			return 0;
//		MeanMiu = (SubstAtom*SubstVeights)/(SubstAtom*(SubstAtom*0+1));
//		return 1;
//	}
//
//	double FreeE(VecCl &ReactCoef,double Denc,double T){
//		VecCl SubstDencCoef, SubstAtom;
//		double MeanMiu;
//		if ( !ClcProportions(ReactCoef, SubstDencCoef,MeanMiu, SubstAtom) )
//			return 1e5*( sqr(VecCl::ModMax(VecCl::fabs(SubstDencCoef)-SubstDencCoef))*Denc*T + 1 );
//		double res = 0;
//		for(int k=1;k<=SubstDencCoef.Dim();k++)
//			if (SubstDencCoef[k]>MathZer)
//				res += ChemSubst[k-1]->FreeE(Denc*SubstDencCoef[k],T)*SubstDencCoef[k];
//
//		return res;///MeanMiu;
//	}
//    double FreeE(double Denc,double T) {
//		VecCl ReactCoef;
//		ClcStartCoef(ReactCoef,Denc,T);
//		MinimizeCoef(ReactCoef,Denc,T);
//		return FreeE(ReactCoef,Denc,T);
//    }
//};
//
//
//
//// Calculation working only for ideal gases mixtures, question - if working...
//// EOS class - reading parameters, calling ChemEqMinim - to minimize...
//struct FreeEChemMatter:FreeEIOFind{
//	//FreeEChemMatter():FreeEIOFind("material.cfg","MatterChemH2Met"){
//	//	SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),2,1);
//	//	SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,0);
//	//	ChemEq eq(Stroka("M1"),-1);eq.AddSubst(Stroka("M2"),1);
//	//	ChemEqs.push_back(eq);
//	//	ClcStartParams();
//	//}
//    FreeEChemMatter();
//	Ref<ChemEqMinim> FreeMinim;
//
//	void CheckReactCoef(MatrCl &ChemReact, VecCl &SubstVeights,
//		DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &ChemSubst){
//		for(int k=1;k<=ChemReact.DimX;k++){
//			double Res = MatrCl::GetRow(ChemReact,k)*SubstVeights;
//			if (fabs(Res)>MathZer){
//				FilterTextOut out("tmp",DataSource::Memory);
//				out<<"\nBad chem react "<<k<<" result is: "<<Res<<"\n";
//				out<<"Vector Of Chem React Coef "<<MatrCl::GetRow(ChemReact,k);
//				out<<"Vector Subst Veights      "<<SubstVeights<<"Matters:\n";
//				for (int i=0;i<ChemReact.DimY;i++)
//					out<<dynamic_cast<FreeEIOFind*>((InterfaceFreeEIO*)ChemSubst[i])->Finder.Substance<<" ";
//				out<<"\nNames\n";
//				for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
//					iter!=SubstMap.end();iter++,k++)
//						out<<iter->first.c_str()<<" ";
//				out<<"\n";
//				throw info_except(DataSource::GetStr("tmp").c_str());
//
//			}
//		}
//	}
//	void ClcStartParams(){
//		MatrCl ChemReact,SubstMixtures;
//		VecCl StartSubstCoef, SubstVeights;
//		DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
//
//		int NumReact = (int)ChemEqs.size(), NumSubst = (int)SubstMap.size();
//		ChemSubst.SetDim(NumSubst);
//		SubstVeights.SetDim(NumSubst);
//		StartSubstCoef.SetDim(NumSubst);
//		map<Stroka,int> SubstInd;
//		int k = 0;
//		for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
//			iter!=SubstMap.end();iter++,k++){
//			ChemSubst[k] = iter->second.Subst;
//			SubstInd[iter->first] = k;
//			SubstVeights[k+1] = iter->second.MolVeight;
//			StartSubstCoef[k+1] = iter->second.StartProportion;
//		}
//		ChemReact.SetMatr(NumReact, NumSubst);
//		ChemReact = ChemReact*0.;
//		for (int i = 1;i<=NumReact;i++){
//			for (int j = 0;j<(int)ChemEqs[i-1].SubstName.size();j++){
//				ChemReact(i,SubstInd[ ChemEqs[i-1].SubstName[j] ]+1) = ChemEqs[i-1].SubstCoef[j];
//			}
//		}
//		CheckReactCoef(ChemReact,SubstVeights,ChemSubst);
//		FreeMinim = new ChemEqMinim(ChemReact, StartSubstCoef, SubstVeights, ChemSubst, SubstMixtures);
//	}
//    double FreeE(double Denc,double T) {
//		if (!FreeMinim)
//			throw info_except("Minimizing class is not inited...\n");
//		return FreeMinim->FreeE(Denc,T);
//    }
//
//
//    void ReadSubstancePar(istream &in) {
//		char tmp[256];
//		in>>tmp>>tmp;
//		NamedSubst sbst;
//		SubstMap.clear();
//		ChemEqs.clear();
//		while (sbst.Read(in)){
//			SubstMap[sbst.Name] = sbst;
//			in>>tmp;
//			if (Stricmp(tmp,"}")==0) break;
//			if (Stricmp(tmp,";")!=0)
//				throw info_except("Read substance %s - expected '; } ' after it but got %s\n",sbst.Name.c_str(),tmp);
//		}
//
//		in>>tmp>>tmp;
//		ChemEq eq;
//		while (eq.Read(in)){
//			ChemEqs.push_back(eq);
//		}
//		ClcStartParams();
//    }
//    void SaveSubstancePar(ostream &out) {
//		out<<" EnterList_SubsName_FreeESubstance_SubsMolVeight { ";
//		for(map<Stroka,NamedSubst>::iterator iter=SubstMap.begin();iter!=SubstMap.end();iter++){
//			iter->second.Save(out);out<<" ; ";
//		}
//        out<<" } \n EnterListOfVectors_Vector_SubsName_SubsCoef { ";
//		for(size_t k=0;k<ChemEqs.size();k++)
//			ChemEqs[k].Save(out);
//		out<<" } \n";
//    }
//private:
//	struct NamedSubst{
//		Stroka Name;
//		Ref<InterfaceFreeEIO> Subst;
//		double MolVeight,StartProportion;
//		NamedSubst(const NamedSubst &sub):Name(sub.Name),Subst(sub.Subst),MolVeight(sub.MolVeight),StartProportion(sub.StartProportion){};
//		NamedSubst():Name(""),Subst(NULL),MolVeight(0),StartProportion(0){};
//		NamedSubst(const Stroka &name,InterfaceFreeEIO *subst,double molveight,double startproportion)
//			:Name(name),Subst(subst),MolVeight(molveight),StartProportion(startproportion){};
//		int  Read(istream &in) {
//			char tmp[256];
//			in>>tmp;
//            if (Stricmp(tmp,"}")==0) return 0;
//			in>>Name>>tmp>>MolVeight>>tmp>>StartProportion>>tmp;
//            if (!(Subst=dynamic_cast<InterfaceFreeEIO*>(SavableClass::SavableClassRegister(in))))
//				throw info_except("Could not convert matter with Name %s to InterfaceFreeEIO\n",Name.c_str());
//			return 1;
//		}
//		void Save(ostream &out) {
//			out<<" Name "<<Name.c_str()<<" MolVeight "<<MolVeight<<" StartProportion "<<StartProportion<<" MatterFreeE ";
//			SavableClass::SavableClassStore(out,Subst);
//		}
//	};
//	struct ChemEq{
//		vector<Stroka> SubstName;
//		vector<double> SubstCoef;
//		ChemEq(){};
//		ChemEq(const ChemEq &eq):SubstName(eq.SubstName), SubstCoef(eq.SubstCoef){};
//		ChemEq(const Stroka &substname, double substcoef) { AddSubst(substname,substcoef); };
//		void AddSubst(const Stroka &substname, double substcoef){
//			SubstName.push_back(substname);SubstCoef.push_back(substcoef);
//		};
//		int  Read(istream &in) {
//			SubstName.clear();SubstCoef.clear();
//			char tmp[256];
//			in>>tmp;
//            if (Stricmp(tmp,"}")==0) return 0;
//			if (Stricmp(tmp,"{")!=0)
//				throw info_except("Expected { and got %s\n",tmp);
//			in>>tmp;
//			while (Stricmp(tmp,"}")!=0){
//				Stroka Name = tmp;
//				SubstName.push_back(tmp);
//				double d;in>>d;
//				SubstCoef.push_back(d);
//				in>>tmp;
//			}
//			return 1;
//		}
//		void Save(ostream &out) {
//			out<<" { ";
//			for (size_t k=0;k<SubstName.size();k++)
//				out<<"  "<<SubstName[k].c_str()<<" "<<SubstCoef[k]<<"    ";
//			out<<" }\n";
//		}
//
//	};
//	map<Stroka,NamedSubst> SubstMap;
//    vector<ChemEq> ChemEqs;
//};
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//struct FreeEChemMatterMultiV:FreeEIOFind{
//	//FreeEChemMatterMultiV():FreeEIOFind("material.cfg","MatterChemH2Met"){
//	//	SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),2,1,1,1);
//	//	SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,0,1,1);
//	//	ChemEq eq(Stroka("M1"),-1);eq.AddSubst(Stroka("M2"),1);
//	//	ChemEqs.push_back(eq);
//	//	ClcStartParams();
//	//}
//    FreeEChemMatterMultiV();
//	Ref<ChemMultiV> FreeMinim;
//
//	void CheckReactCoef(MatrCl &ChemReact, VecCl &SubstVeights,
//		DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &ChemSubst){
//		for(int k=1;k<=ChemReact.DimX;k++){
//			double Res = MatrCl::GetRow(ChemReact,k)*SubstVeights;
//			if (fabs(Res)>MathZer){
//				FilterTextOut out("tmp",DataSource::Memory);
//				out<<"\nBad chem react "<<k<<" result is: "<<Res<<"\n";
//				out<<"Vector Of Chem React Coef "<<MatrCl::GetRow(ChemReact,k);
//				out<<"Vector Subst Veights      "<<SubstVeights<<"Matters:\n";
//				for (int i=0;i<ChemReact.DimY;i++)
//					out<<dynamic_cast<FreeEIOFind*>((InterfaceFreeEIO*)ChemSubst[i])->Finder.Substance<<" ";
//				out<<"\nNames\n";
//				for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
//					iter!=SubstMap.end();iter++,k++)
//						out<<iter->first.c_str()<<" ";
//				out<<"\n";
//				throw info_except(DataSource::GetStr("tmp").c_str());
//
//			}
//		}
//	}
//	void ClcStartParams(){
//		MatrCl ChemReact;
//		VecCl StartSubstCoef, SubstVeights;
//		DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
//        VecCl RelatVolumeCoef,SubstMixtures;
//
//		int NumReact = (int)ChemEqs.size(), NumSubst = (int)SubstMap.size();
//		ChemSubst.SetDim(NumSubst);
//		SubstVeights.SetDim(NumSubst);
//        RelatVolumeCoef.SetDim(NumSubst);
//		StartSubstCoef.SetDim(NumSubst);
//        SubstMixtures.SetDim(NumSubst);
//		int k = 0;
//		for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
//			iter!=SubstMap.end();iter++,k++){
//			ChemSubst[k] = iter->second.Subst;
//			SubstInd[iter->first] = k;
//			SubstVeights[k+1] = iter->second.MolVeight;
//			StartSubstCoef[k+1] = iter->second.StartProportion;
//            RelatVolumeCoef[k+1] = iter->second.RelatVolumeCoef;
//            SubstMixtures[k+1] = iter->second.SubstMixtures;
//		}
//		ChemReact.SetMatr(NumReact, NumSubst);
//		ChemReact = ChemReact*0.;
//		for (int i = 1;i<=NumReact;i++){
//			for (int j = 0;j<(int)ChemEqs[i-1].SubstName.size();j++){
//				ChemReact(i,SubstInd[ ChemEqs[i-1].SubstName[j] ]+1) = ChemEqs[i-1].SubstCoef[j];
//			}
//		}
//		CheckReactCoef(ChemReact,SubstVeights,ChemSubst);
//		FreeMinim = new ChemMultiV(SubstVeights, ChemSubst, SubstMixtures, RelatVolumeCoef, ChemReact, StartSubstCoef);
//		SetInternalNames();
//	}
//	virtual void SetInternalNames(){
//		InputNames.clear(); OutputNames.clear();
//		FreeEIOFind::SetStdInternalNames(InputNames, OutputNames);
//		int k = 1;
//		for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
//			iter!=SubstMap.end();iter++,k++){
//                OutputNames[iter->first+"_N"] = &k;
//			}
//#ifdef _DEBUG
//        FilterTextOut out("aa",DataSource::Memory);
//		save_data_state(out);out<<"\n";
//        SaveSubstancePar(out);out<<"\n=============================\n";
//		cout<<DataSource::GetStr("aa").c_str();
//#endif
//  	}
//	virtual double InternalClc(const map<Stroka,double> &Input, const Stroka &Output){
//		void *ptr;
//		if (CheckPresence(OutputNames, Output, ptr) && ptr!=NULL){
//			FreeE(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"));
//			int k = SubstInd[Stroka(Output,0,Output.length()-2)];
//			return FreeMinim->ResultRelatNumbers[k+1];
//		}
//
//		try {
//			return FreeEIOFind::InternalClc(Input, Output);
//		} catch(stdexception &ex){
//			FilterTextOut out("aa",DataSource::Memory);
//			out<<"Name_ToClc "<<Output<<" Names_Present: ";
//			for (map<Stroka, void*>::iterator iter = OutputNames.begin();iter!=OutputNames.end();iter++)
//			out<<iter->first<<" ";out<<"\n";
//			SaveSubstancePar(out);
//			throw info_except("%s%s",ex.what(),DataSource::GetStr("aa").c_str());
//		}
//	}
////		StoreInternVal(Input);
//  //  void SetInternPars(VecCl subst_props){
//		//int k = 1;
//		//for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
//		//	iter!=SubstMap.end();iter++,k++)
//  //              InternalPar->Get(iter->first) = subst_props[k];
//  //  }
//    double FreeE(double Denc,double T) {
//		if (!FreeMinim)
//			throw info_except("Minimizing class is not inited...\n");
//		return FreeMinim->ClcFreeE(Denc,T);
//    }
//    void ReadSubstancePar(istream &in) {
//		char tmp[256];
//		in>>tmp>>tmp;
//		NamedSubst sbst;
//		SubstMap.clear();
//		ChemEqs.clear();
//		while (sbst.Read(in)){
//			SubstMap[sbst.Name] = sbst;
//			in>>tmp;
//			if (Stricmp(tmp,"}")==0) break;
//			if (Stricmp(tmp,";")!=0)
//				throw info_except("Read substance %s - expected '; } ' after it but got %s\n",sbst.Name.c_str(),tmp);
//		}
//
//		in>>tmp>>tmp;
//		ChemEq eq;
//		while (eq.Read(in)){
//			ChemEqs.push_back(eq);
//		}
//		ClcStartParams();
//    }
//    void SaveSubstancePar(ostream &out) {
//		out<<" EnterList_SubsName_FreeESubstance_SubsMolVeight { ";
//		for(map<Stroka,NamedSubst>::iterator iter=SubstMap.begin();iter!=SubstMap.end();iter++){
//			iter->second.Save(out);out<<" ; ";
//		}
//        out<<" } \n EnterListOfVectors_Vector_SubsName_SubsCoef { ";
//		for(size_t k=0;k<ChemEqs.size();k++)
//			ChemEqs[k].Save(out);
//		out<<" } \n";
//    }
//private:
//	struct NamedSubst{
//		Stroka Name;
//		Ref<InterfaceFreeEIO> Subst;
//		double MolVeight,StartProportion;
//        double RelatVolumeCoef,SubstMixtures;
//		NamedSubst(const NamedSubst &sub):Name(sub.Name),Subst(sub.Subst),MolVeight(sub.MolVeight),StartProportion(sub.StartProportion){};
//		NamedSubst():Name(""),Subst(NULL),MolVeight(0),StartProportion(0),RelatVolumeCoef(1){};
//		NamedSubst(const Stroka &name,InterfaceFreeEIO *subst,double molveight,double startproportion, double relatvolumecoef, int substmixtures)
//			:Name(name),Subst(subst),MolVeight(molveight),StartProportion(startproportion),RelatVolumeCoef(relatvolumecoef),SubstMixtures(substmixtures){};
//		int  Read(istream &in) {
//			char tmp[256];
//			in>>tmp;
//            if (Stricmp(tmp,"}")==0) return 0;
//			in>>Name>>tmp>>MolVeight>>tmp>>StartProportion>>tmp>>RelatVolumeCoef>>tmp>>SubstMixtures>>tmp;
//            if (!(Subst=dynamic_cast<InterfaceFreeEIO*>(SavableClass::SavableClassRegister(in))))
//				throw info_except("Could not convert matter with Name %s to InterfaceFreeEIO\n",Name.c_str());
//			return 1;
//		}
//		void Save(ostream &out) {
//			out<<" Name "<<Name.c_str()<<" MolVeight "<<MolVeight<<" StartProportion "<<StartProportion<<" RelatVolumeCoef "<<RelatVolumeCoef<<" SubstMixtures "<<SubstMixtures<<" MatterFreeE ";
//			SavableClass::SavableClassStore(out,Subst);
//		}
//	};
//	struct ChemEq{
//		vector<Stroka> SubstName;
//		vector<double> SubstCoef;
//		ChemEq(){};
//		ChemEq(const ChemEq &eq):SubstName(eq.SubstName), SubstCoef(eq.SubstCoef){};
//		ChemEq(const Stroka &substname, double substcoef) { AddSubst(substname,substcoef); };
//		void AddSubst(const Stroka &substname, double substcoef){
//			SubstName.push_back(substname);SubstCoef.push_back(substcoef);
//		};
//		int  Read(istream &in) {
//			SubstName.clear();SubstCoef.clear();
//			char tmp[256];
//			in>>tmp;
//            if (Stricmp(tmp,"}")==0) return 0;
//			if (Stricmp(tmp,"{")!=0)
//				throw info_except("Expected { and got %s\n",tmp);
//			in>>tmp;
//			while (Stricmp(tmp,"}")!=0){
//				Stroka Name = tmp;
//				SubstName.push_back(tmp);
//				double d;in>>d;
//				SubstCoef.push_back(d);
//				in>>tmp;
//			}
//			return 1;
//		}
//		void Save(ostream &out) {
//			out<<" { ";
//			for (size_t k=0;k<SubstName.size();k++)
//				out<<"  "<<SubstName[k].c_str()<<" "<<SubstCoef[k]<<"    ";
//			out<<" }\n";
//		}
//
//	};
//	map<Stroka,NamedSubst> SubstMap;
//    vector<ChemEq> ChemEqs;
//	map<Stroka,int> SubstInd;
//};
//
//
//
//
//
//
//
//
//
//
//
//
//
//======================================================================================
//======================================================================================
//======================================================================================

//struct FreeEChemMatter:FreeEIOFind{
//	FreeEChemMatter():FreeEIOFind("material.cfg","MatterChemH2Met"){
//		SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),2,1);
//		SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,0);
//		ChemEq eq(Stroka("M1"),-1);eq.AddSubst(Stroka("M2"),1);
//		ChemEqs.push_back(eq);
//		ClcStartParams();
//	}
//	struct MinFuncClass:DeriveFuncClass{
//		//VecCl MinV,MaxV;
//		FreeEChemMatter *FreeClc;
//		double Denc,T;
//		MinFuncClass(FreeEChemMatter *freeclc, double denc, double t)
//			:FreeClc(freeclc), Denc(denc), T(t){};
//		double Execute(VecCl &par){
//			//VecCl SubstDencCoef;
//			//if ( !FreeClc->ClcProportions(ReactCoef, SubstDencCoef) )
//			//	return 1e5*( sqr(VecCl::ModMax(VecCl::fabs(SubstDencCoef)-SubstDencCoef))*Denc*T + 1 );
//			VecCl ReactCoef = Decode(par);
//			double ret = FreeClc->FreeE(ReactCoef,Denc,T);
////cout<<" MinimPar "<<par<<" res "<<ret<<"\n";
////cout<<par[1]<<"  "<<ret<<"\n";
//			return ret;
//		}
//		static VecCl Encode(const VecCl &ReactCoef){
//			VecCl par = ReactCoef;
//			for(int k=1;k<=par.Dim();k++){
//				par[k] = par[k]<=MathZer?MathZer:(par[k]>=1-MathZer?1-MathZer:par[k]);
//				par[k] = log(par[k]/(1-par[k]));
//			};
//			return par;
//		};
//		static VecCl Decode(const VecCl &par){
//			VecCl ReactCoef = par;
//			for(int k=1;k<=par.Dim();k++) {
//				ReactCoef[k] = 1/(1+exp(-par[k]));
//				if (fabs(ReactCoef[k])<1e-17)
//					ReactCoef[k] = 0;
//			}
//			return ReactCoef;
//		};
//	};
//	//struct MinPresClass:MinFuncClass{
//	//	MinPresClass(FreeEChemMatter *freeclc, double denc, double t)
//	//		:MinFuncClass(freeclc,Denc, T){};
//	//	double Execute(VecCl &par){
//	//		double ret = FreeClc->PresMisfit(DecodeSize(Decode(par)),Denc,T) + sqrt(fabs(BadData))*1e6;
//	//		return ret;
//	//	}
//	//	static VecCl DecodeSize(const VecCl &par){
//	//		VecCl SubsCoef(par.Dim()+1);
//	//		double s = 0;
//	//		for(int k=1;k<=SubsCoef.Dim()-1;k++){
//	//			SubsCoef[k] = par[k];
//	//			s+ = par[k];
//	//		}
//	//		SubsCoef[SubsCoef.Dim()] = 1-s;
//	//		if (1-s>0)
//	//			BadData = 0
//	//		else
//	//			BadData = 1-s;
//	//		return par;
//	//	}
//	//	static VecCl EncodeSize(const VecCl &SubsCoef){
//	//		VecCl par(SubsCoef.Dim()-1);
//	//		for(int k=1;k<=SubsCoef.Dim()-1;k++){
//	//			par[k] = SubsCoef[k];
//	//		}
//	//		return par;
//	//	}
//	//	static double BadData;
//	//};
//
//	MatrCl ChemReact;
//	VecCl StartSubstCoef, SubstVeights;
//	DataVector< Ref< InterfaceFreeEIO > > ChemSubst;
//	//DataVector< Ref< MatterFreeE > > Config_Subst;
//	//VecCl Config_F;
//	void CheckReactCoef(){
////cout<<"ChemReact\n"<<ChemReact;
//		for(int k=1;k<=ChemReact.DimX;k++){
////cout<<"SubstVeights "<<SubstVeights<<" React[k]Coef "<<MatrCl::GetRow(ChemReact,k);
//			double Res = MatrCl::GetRow(ChemReact,k)*SubstVeights;
//			if (fabs(Res)>MathZer){
//				FilterTextOut out("tmp",DataSource::Memory);
//				out<<"\nBad chem react "<<k<<" result is: "<<Res<<"\n";
//				out<<"Vector Of Chem React Coef "<<MatrCl::GetRow(ChemReact,k);
//				out<<"Vector Subst Veights      "<<SubstVeights<<"Matters:\n";
//				for (int i=0;i<ChemSubst.SetDim();i++)
//					out<<dynamic_cast<FreeEIOFind*>((InterfaceFreeEIO*)ChemSubst[i])->Finder.Substance<<" ";
//				out<<"\nNames\n";
//				for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
//					iter!=SubstMap.end();iter++,k++)
//						out<<iter->first.c_str()<<" ";
//				out<<"\n";
//				throw info_except(DataSource::GetStr("tmp").c_str());
//
////				throw info_except("Bad chem react %i result is:%f\n",k,Res);
//			}
//		}
//	}
//	void ClcStartParams(){
//		int NumReact = (int)ChemEqs.size(), NumSubst = (int)SubstMap.size();
//		ChemSubst.SetDim(NumSubst);
//		SubstVeights.SetDim(NumSubst);
//		StartSubstCoef.SetDim(NumSubst);
//		map<Stroka,int> SubstInd;
//		int k = 0;
//		for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
//			iter!=SubstMap.end();iter++,k++){
//			ChemSubst[k] = iter->second.Subst;
//			SubstInd[iter->first] = k;
//			SubstVeights[k+1] = iter->second.MolVeight;
//			StartSubstCoef[k+1] = iter->second.StartProportion;
//		}
//		ChemReact.SetMatr(NumReact, NumSubst);
//		ChemReact = ChemReact*0.;
//		for (int i = 1;i<=NumReact;i++){
//			for (int j = 0;j<(int)ChemEqs[i-1].SubstName.size();j++){
//				ChemReact(i,SubstInd[ ChemEqs[i-1].SubstName[j] ]+1) = ChemEqs[i-1].SubstCoef[j];
//			}
//		}
//		CheckReactCoef();
//	}
//	static FreeEChemMatter *StaticPtr;
//	double StaticD,StaticT;
//	VecCl StaticReact;
//	int StaticN;
//
//	static double StaticFreeE(double x){
//		StaticPtr->StaticReact[StaticPtr->StaticN] = x;
//		VecCl tmp = MinFuncClass::Decode(StaticPtr->StaticReact);
//		double res = StaticPtr->FreeE(tmp,StaticPtr->StaticD,StaticPtr->StaticT);
////cout<<x<<" "<<res<<"\n";
//		return res;
//	}
////	double PresMisfit(const VecCl &DencProps, double Denc,double T){
////		VecCl Pres = DencProps*0;
////		double Mean = 0;
////		for(int k=0;k<=DencProps.Dim();k++){
////			Pres[k] = Config_Subst[k]->Pressure(Denc*DencProps[k],T);
////			Mean +=Pres[k];
////		}
////		Mean /= Pres.Dim();
////		double ret = 0;
////		for(int k=0;k<=DencProps.Dim();k++)
////			ret += sqr(Mean - Pres[k]);
////		return ret;
////	}
////
////	void ClcDencProportions(VecCl &DencProps, double Denc,double T){
////		double FuncError=1e-4, ResError=FuncError;
////		double Min1D_Coef=1.2;
////		int Min1D_MaxLinSrch=5,Min1D_MaxRetry=5;
////		MinFindNDimClass MinNd(
////			new EigenVectMinim(new Derive2DStd(new MinPresClass(this, Denc, T)),
////								new MinFind1DClass(NULL,ResError,FuncError,
////									Min1D_Coef,Min1D_MaxLinSrch,Min1D_MaxRetry),
////								//new MinFind1DClass(FuncError),
////								1e-8,1e-15,FuncError,2) ,
////			ResError);
////		VecCl MinimPar = MinFuncClass::Encode(MinPresClass::EncodeSize(DencProps)), MinimStp(MinimPar.Dim());
//////cout<<"ReactCoef "<<ReactCoef<<" MinimPar "<<MinimPar;
////		MinimStp = MinimStp*0 + 1.;
////		int Cont = 1, MaxIt = 15, k;
////		double FoundMisfit;
////		for (k=1;((k<=MaxIt)&&Cont);k++)
////			Cont=MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
////		MinimStp = MinimStp*0 + 1.;
////		Cont = 0;
////		for (k=1;((k<=MaxIt)&&Cont);k++)
////			Cont=MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
////
////		DencProps = MinFuncClass::Decode(MinPresClass::DecodeSize(MinimPar));
////	}
////
////	void ClcConfigPart(double Denc,double T){
////		Config_F = Config_F*0;
////		VecCl DencProps = Config_F;
////		ClcDencProportions(DencProps);
////		for (int k=1;k<=Config_F.Dim();k++){
////		}
////	}
//	void ClcStartCoef(VecCl &ReactCoef, double Denc,double T){
//		ReactCoef.SetDim((int)ChemEqs.size());
//		ReactCoef = ReactCoef*0+0.5;
//
//		//ClcConfigPart( Denc, T);
//
//		OneDMinim(ReactCoef,Denc,T);
//	}
//	void OneDMinim(VecCl &ReactCoef,double Denc,double T){
//		StaticT = T; StaticD = Denc; StaticPtr = this; StaticReact = MinFuncClass::Encode(ReactCoef);
//		for(int k=1;k<=ReactCoef.Dim();k++){
//			StaticN = k;
//			double res;
//			StaticReact[k]=Fmin(StaticFreeE,-25,25,0,1e-6,res);
//		}
//		ReactCoef = MinFuncClass::Decode(StaticReact);
//	}
//	void MinimizeCoef(VecCl &ReactCoef,double Denc,double T){
//		double FuncError=1e-4, ResError=FuncError;
//		double Min1D_Coef=1.2;
//		int Min1D_MaxLinSrch=5,Min1D_MaxRetry=5;
//		MinFindNDimClass MinNd(
//			new EigenVectMinim(new Derive2DStd(new MinFuncClass(this, Denc, T)),
//								new MinFind1DClass(NULL,ResError,FuncError,
//									Min1D_Coef,Min1D_MaxLinSrch,Min1D_MaxRetry),
//								//new MinFind1DClass(FuncError),
//								1e-8,1e-15,FuncError,2) ,
//			ResError);
//		VecCl MinimPar = MinFuncClass::Encode(ReactCoef), MinimStp(MinimPar.Dim());
////cout<<"ReactCoef "<<ReactCoef<<" MinimPar "<<MinimPar;
//		MinimStp = MinimStp*0 + 1.;
//		int Cont = 1, MaxIt = 15, k;
//		double FoundMisfit;
//		for (k=1;((k<=MaxIt)&&Cont);k++)
//			Cont=MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//		ReactCoef = MinFuncClass::Decode(MinimPar);
//		OneDMinim(ReactCoef,Denc,T);
//		MinimPar = MinFuncClass::Encode(ReactCoef);
//		MinimStp = MinimStp*0 + 1.;
//		MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//		ReactCoef = MinFuncClass::Decode(MinimPar);
//		OneDMinim(ReactCoef,Denc,T);
//		MinimPar = MinFuncClass::Encode(ReactCoef);
//		MinimStp = MinimStp*0 + 1.;
//		MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
//
//		ReactCoef = MinFuncClass::Decode(MinimPar);
////cout<<"Res:\nReactCoef "<<ReactCoef<<" MinimPar "<<MinimPar<<" FoundMisfit "<<FoundMisfit<<"\n";
//	}
//	int ClcProportions(const VecCl &ReactCoef, VecCl &SubstDencCoef,double &MeanMiu, VecCl &SubstAtom){
//		VecCl tmp = ReactCoef;
//		SubstAtom = StartSubstCoef + tmp*ChemReact;
//		SubstDencCoef = VecCl::ComponentMul(SubstAtom,SubstVeights);
//		SubstDencCoef = SubstDencCoef/sqrt(SubstDencCoef*SubstDencCoef);
////cout<<"ReactCoef "<<ReactCoef<<"StartSubstCoef "<<StartSubstCoef<<"SubstDencCoef "<<SubstDencCoef;
////cout<<"SubstAtom "<<SubstAtom ;
////cout<<" SubstVeights "<<SubstVeights<<"ChemReact\n"<<ChemReact;
////for (int k=0;k<ChemSubst.SetDim();k++) cout<<dynamic_cast<FreeEIOFind*>((InterfaceFreeEIO*)ChemSubst[k])->Finder.Substance<<" ";
////cout<<"\n";
//		if (VecCl::ModMax(VecCl::fabs(SubstDencCoef)-SubstDencCoef)>MathZer)
//			return 0;
//		MeanMiu = (SubstAtom*SubstVeights)/(SubstAtom*(SubstAtom*0+1));
//		return 1;
//	}
//
//	double FreeE(VecCl &ReactCoef,double Denc,double T){
//		VecCl SubstDencCoef, SubstAtom;
//		double MeanMiu;
//		if ( !ClcProportions(ReactCoef, SubstDencCoef,MeanMiu, SubstAtom) )
//			return 1e5*( sqr(VecCl::ModMax(VecCl::fabs(SubstDencCoef)-SubstDencCoef))*Denc*T + 1 );
//		double res = 0;
//		for(int k=1;k<=SubstDencCoef.Dim();k++)
//			if (SubstDencCoef[k]>MathZer)
//				res += ChemSubst[k-1]->FreeE(Denc*SubstDencCoef[k],T)*SubstDencCoef[k];
//
//		return res;///MeanMiu;
//	}
//    double FreeE(double Denc,double T) {
//		VecCl ReactCoef;
//		ClcStartCoef(ReactCoef,Denc,T);
//		MinimizeCoef(ReactCoef,Denc,T);
////cout<<"RESULT          "<<T<<" "<<ReactCoef[1]<<" "<<FreeE(ReactCoef,Denc,T)<<"\n";
//		return FreeE(ReactCoef,Denc,T);
//    }
//    void ReadSubstancePar(istream &in) {
//		char tmp[256];
//		in>>tmp>>tmp;
//		NamedSubst sbst;
//		SubstMap.clear();
//		ChemEqs.clear();
//		while (sbst.Read(in)){
//			SubstMap[sbst.Name] = sbst;
//			in>>tmp;
//			if (Stricmp(tmp,"}")==0) break;
//			if (Stricmp(tmp,";")!=0)
//				throw info_except("Read substance %s - expected '; } ' after it but got %s\n",sbst.Name.c_str(),tmp);
//		}
//
//		in>>tmp>>tmp;
//		ChemEq eq;
//		while (eq.Read(in)){
//			ChemEqs.push_back(eq);
//		}
//		ClcStartParams();
//    }
//    void SaveSubstancePar(ostream &out) {
//		out<<" EnterList_SubsName_FreeESubstance_SubsMolVeight { ";
//		for(map<Stroka,NamedSubst>::iterator iter=SubstMap.begin();iter!=SubstMap.end();iter++){
//			iter->second.Save(out);out<<" ; ";
//		}
//        out<<" } \n EnterListOfVectors_Vector_SubsName_SubsCoef { ";
//		for(size_t k=0;k<ChemEqs.size();k++)
//			ChemEqs[k].Save(out);
//		out<<" } \n";
//    }
//private:
//	struct NamedSubst{
//		Stroka Name;
//		Ref<InterfaceFreeEIO> Subst;
//		double MolVeight,StartProportion;
//		NamedSubst(const NamedSubst &sub):Name(sub.Name),Subst(sub.Subst),MolVeight(sub.MolVeight),StartProportion(sub.StartProportion){};
//		NamedSubst():Name(""),Subst(NULL),MolVeight(0),StartProportion(0){};
//		NamedSubst(const Stroka &name,InterfaceFreeEIO *subst,double molveight,double startproportion)
//			:Name(name),Subst(subst),MolVeight(molveight),StartProportion(startproportion){};
//		int  Read(istream &in) {
//			char tmp[256];
//			in>>tmp;
//            if (Stricmp(tmp,"}")==0) return 0;
//			in>>Name>>tmp>>MolVeight>>tmp>>StartProportion>>tmp;
//            if (!(Subst=dynamic_cast<InterfaceFreeEIO*>(SavableClass::SavableClassRegister(in))))
//				throw info_except("Could not convert matter with Name %s to InterfaceFreeEIO\n",Name.c_str());
//			return 1;
//		}
//		void Save(ostream &out) {
//			out<<" Name "<<Name.c_str()<<" MolVeight "<<MolVeight<<" StartProportion "<<StartProportion<<" MatterFreeE ";
//			SavableClass::SavableClassStore(out,Subst);
//		}
//	};
//	struct ChemEq{
//		vector<Stroka> SubstName;
//		vector<double> SubstCoef;
//		ChemEq(){};
//		ChemEq(const ChemEq &eq):SubstName(eq.SubstName), SubstCoef(eq.SubstCoef){};
//		ChemEq(const Stroka &substname, double substcoef) { AddSubst(substname,substcoef); };
//		void AddSubst(const Stroka &substname, double substcoef){
//			SubstName.push_back(substname);SubstCoef.push_back(substcoef);
//		};
//		int  Read(istream &in) {
//			SubstName.clear();SubstCoef.clear();
//			char tmp[256];
//			in>>tmp;
//            if (Stricmp(tmp,"}")==0) return 0;
//			if (Stricmp(tmp,"{")!=0)
//				throw info_except("Expected { and got %s\n",tmp);
//			in>>tmp;
//			while (Stricmp(tmp,"}")!=0){
//				Stroka Name = tmp;
//				SubstName.push_back(tmp);
//				double d;in>>d;
//				SubstCoef.push_back(d);
//				in>>tmp;
////cout<<"ChemReact: "<<Name<<" "<<d<<"\n";
//			}
////cout<<"\n";
//			return 1;
//		}
//		void Save(ostream &out) {
//			out<<" { ";
//			for (size_t k=0;k<SubstName.size();k++)
//				out<<"  "<<SubstName[k].c_str()<<" "<<SubstCoef[k]<<"    ";
//			out<<" }\n";
//		}
//
//	};
//	map<Stroka,NamedSubst> SubstMap;
//    vector<ChemEq> ChemEqs;
//    //vector<double> ChemCoefs;
//};
//
