#pragma once

#include "fun_fac.h"
#include "quad.h"
#include "spl_fac.h"
#include "lib/ref/data_manip.h"

struct BrigtnesTemp : OneVarFunction {
    BrigtnesTemp();
    //    :L2OSourceCal(new ManyVarFunc2OneVar), L2OSourceExp(new PolynomFunc), L2ODetector(new ManyVarFunc2OneVar), InterFilter(new PolynomFunc), ZeroFilterSignal(1e-5), WaveLength(500), SourceTemp(2700), Opacity(1), SingleLine(1), CalibrationSignal(0.6){ }

    Ref<OneVarFunction> L2OSourceCal, L2OSourceExp, L2ODetector;
    Ref<OneVarFunction> InterFilter;
    double ColorTempCoef;
    double WaveLength, SourceTemp, Opacity, ZeroFilterSignal;
    int SingleLine;
    double CalibrationSignal;
    static double PlancEmisE(double Temp, double Lambda);


    virtual int save_data_state(FilterOut &so) {
        so << " L2OSourceCal " << L2OSourceCal << " L2OSourceExp " << L2OSourceExp
           << " L2ODetector " << L2ODetector << " InterFilter " << InterFilter;
        so << " ZeroFilterSignal " << ZeroFilterSignal;
        so << " WaveLength " << WaveLength << " SourceTemp " << SourceTemp << " Opacity "
           << Opacity;
        so << " SingleLine " << SingleLine;
        so << " CalibrationSignal " << CalibrationSignal;

        return !(!so);
    };
    virtual int read_data_state(FilterIn &si) {
        char tmp[256];
        si >> tmp >> L2OSourceCal >> tmp >> L2OSourceExp >> tmp >> L2ODetector >> tmp >>
            InterFilter;
        si >> tmp >> ZeroFilterSignal;
        si >> tmp >> WaveLength >> tmp >> SourceTemp >> tmp >> Opacity;
        si >> tmp >> SingleLine;   //>>tmp>>UseFilter;
        si >> tmp >> CalibrationSignal;
        SetCalib();
        return !(!si);
    };
    void SetCalib() {
        Calibration1Const = 0;
        CalibrationIntConst = 0;
        if(SingleLine) {
            Calibration1Const =
                (exp(0.01438775220950165 / (WaveLength * 1e-9 * SourceTemp)) - 1) /
                L2OSourceCal->Calculate(WaveLength);
            return;
        }

        FindIntegralBegEnd(IntegralBeg, IntegralEnd, L2ODetector, InterFilter);
        MakeSpline(CalTract, L2OSourceCal, L2ODetector, InterFilter);
        MakeSpline(ExpTract, L2OSourceExp, L2ODetector, InterFilter);
        CalibrationIntConst = IntegrateInt(SourceTemp, &CalTract, 1);
    }
    double Calculate(double expsig) {
        expsig = fabs(expsig);
        if(SingleLine)
            return FindExpT2GivenLength(expsig);
        return FindExpT2DetectorIntegral(expsig);
    }

private:
    CurveSpline CalTract, ExpTract;
    double IntegralBeg, IntegralEnd;
    void FindIntegralBegEnd(
        double &intbeg,
        double &intend,
        OneVarFunction *f1,
        OneVarFunction *f2) {
        vector<double> vec = DataManip::MakeStepVector(500, 10, 10000, 1, 0);
        int i = 0;
        for(i = 0; i < (int)vec.size(); i++) {
            if(fabs(f1->Calculate(vec[i]) * f2->Calculate(vec[i])) > 1e-5)
                break;
        }
        intbeg = vec[max<int>(0, i - 1)];
        for(i = (int)vec.size() - 1; i >= 0; i--) {
            if(fabs(f1->Calculate(vec[i]) * f2->Calculate(vec[i])) > 1e-5)
                break;
        }
        intend = vec[min(i + 1, (int)vec.size() - 1)];
    }
    double Calibration1Const, CalibrationIntConst;
    static BrigtnesTemp *CurPtr;
    static double IntenValStatic(double lambda) {
        return CurPtr->IntenVal(lambda);
    }
    CurveSpline *L2OIntegral;
    double TempIntegral, ColorTempCoefIntegral;   //, BegIntegral, EndIntegral;
    double IntenVal(double lambda) {
        double intence = PlancEmisE(TempIntegral, lambda);
        intence *= L2OIntegral->Evaluate(lambda) * ColorTempCoefIntegral;
        return intence;
    }
    double IntegrateInt(double temp, CurveSpline *l2osource, double integralcoef) {
        TempIntegral = temp;
        CurPtr = this;
        L2OIntegral = l2osource;
        ColorTempCoefIntegral = integralcoef;
        double ResErr;
        int Err;
        return Qags_IntSubInt(
            IntenValStatic, IntegralBeg, IntegralEnd, 1e-6, 1e-6, 300, ResErr, Err);
    }
    // =============================
    static double MisfValStatic(double lambda) {
        return CurPtr->MisfVal(lambda);
    }
    double MisfVal(double temp) {
        return CalibrationSignal / CalibrationIntConst *
                   IntegrateInt(temp, &ExpTract, ColorTempCoef) -
               ExpSigMisf;
    }
    double GuessMisf, ExpSigMisf;
    double FindExpT2DetectorIntegral(double signal) {
        int res;
        ExpSigMisf = signal * Opacity;
        CurPtr = this;
        if((res = Fzero(MisfValStatic, 700, 1e5, GuessMisf, 1e-6, 1e-6, 1000)) > 3)
            cout << " Bad FindExpT2DetectorIntegral Fzero return " << res << " found val "
                 << GuessMisf << " found mis " << MisfValStatic(GuessMisf) << "\n";
        return GuessMisf;
    }
    double FindExpT2GivenLength(double signal) {
        double cal1 = Calibration1Const * ColorTempCoef;
        return 0.01438775220950165 /
               (WaveLength * 1e-9 *
                log(1 + CalibrationSignal * cal1 / (signal * Opacity)));
        //return  0.01438775220950165/(WaveLength*1e-9*log(1+CalibrationSignal*Calibration1Const/(signal*Opacity)));
    }
    void MakeSpline(
        CurveSpline &spl,
        OneVarFunction *f1,
        OneVarFunction *f2,
        OneVarFunction *f3) {
        vector<double> x = DataManip::MakeStepVector(200, IntegralBeg, IntegralEnd, 0, 0);
        vector<double> y(x.size());
        for(size_t i = 0; i < x.size(); i++)
            y[i] = f1->Calculate(x[i]) * f2->Calculate(x[i]) * f3->Calculate(x[i]);
        int numpnt = 500;
        double mis = 1e-6;
        spl.Generate(numpnt, mis, 0, &x[0], &y[0], (int)x.size());
    }
};


struct ColorTemp : ManyVarFunction {
    ColorTemp() : FoundOpacity(0.5) {
        Chanels.push_back(new BrigtnesTemp);
        Chanels.push_back(new BrigtnesTemp);
    };
    vector<Ref<BrigtnesTemp>> Chanels;

    virtual int save_data_state(FilterOut &so) {
        so << " Chanels { ";
        for(size_t i = 0; i < Chanels.size(); i++)
            so << Chanels[i] << "\n";
        so << " } ";
        return !(!so);
    };
    virtual int read_data_state(FilterIn &si) {
        char tmp[256];
        LastParams.clear();
        si >> tmp >> tmp;
        Ref<BrigtnesTemp> chan;
        Chanels.clear();
        while(!TestNextChar(si, '}', 1) && !(!si)) {
            si >> chan;
            Chanels.push_back(chan);
        }
        return !(!si);
    };
    map<Stroka, double> LastParams;
    double FoundOpacity, FoundTemp, FoundMisfit;
    virtual double Calculate(const map<Stroka, double> &exp, const Stroka &name) {
        if(!DataManip::MapsSame(exp, LastParams))
            FindOpacity(exp, FoundTemp, FoundOpacity, FoundMisfit);
        if(name == "Opacity")
            return FoundOpacity;
        if(name == "Temperature")
            return FoundTemp;

        if(name == "Misfit")
            return FoundMisfit;
        int chan = GetChanel(name, "T_");
        if(chan < 0 || chan >= (int)Chanels.size())
            throw info_except(
                "Unknown name %s. Can clc names: [ Opacity Temperature Misfit T_0 T_n ]. Number of channels %i\n",
                name.c_str(),
                Chanels.size());
        return FindTemp(FoundOpacity, exp, chan);
    }
    Stroka MakeHelp() {
        Stroka str =
            "Input brightness chanels. Calculate opacity to minimize temperature misfit. Can clc names: [ Opacity Temperature Misfit T_0 T_n ]\n";
        return str;
    }
    double FindTemp(double opac, const map<Stroka, double> &exp, int chan) {
        map<Stroka, double>::const_iterator it = exp.find(MakeChanel("Ch_", chan));
        if(it == exp.end())
            throw info_except(
                "Cannot calculate channel <%i> Ch_%i - is not defined\n", chan, chan);
        Chanels[chan]->ColorTempCoef = opac;
        return Chanels[chan]->Calculate(it->second);
    }
    int GetChanel(const Stroka &name, const Stroka &pref) {
        if(name.substr(0, pref.size()) != pref)
            return -1;
        return atoi(name.substr(pref.size(), name.size()).c_str());
    }
    Stroka MakeChanel(const Stroka &pref, int ch) {
        char tmp[256];
        sprintf(tmp, "%s%i", pref.c_str(), ch);
        return Stroka(tmp);
    }

    const map<Stroka, double> *ExpSignal;
    static ColorTemp *CurPtr;
    static double MisfitChanelsTempStatic(double opac) {
        double t;
        return CurPtr->FindMeanSigma(opac, t);
    }
    double FindMeanSigma(double opac, double &mean) {
        vector<double> temp(Chanels.size());
        double s = 0, s2 = 0;
        size_t N = Chanels.size();
        for(size_t i = 0; i < N; i++) {
            double t = FindTemp(opac, *ExpSignal, (int)i);
            s += t;
            s2 += t * t;
        }
        mean = s / N;
        //cout<<opac<<" "<<sqrt(s2/N - sqr(mean))<<" "<<mean<<"\n";
        return sqrt(s2 / N - sqr(mean));
    }

    void FindOpacity(
        const map<Stroka, double> &exp,
        double &foundtemp,
        double &foundopacity,
        double &foundmisfit) {
        foundopacity = 0.5;
        ExpSignal = &exp;
        CurPtr = this;
        double resMis;
        foundopacity = Fmin(MisfitChanelsTempStatic, 0.01, 1, foundopacity, 1e-3, resMis);
        FindMeanSigma(foundopacity, foundtemp);
        //cout<<" Found opac "<<foundopacity<<" temp "<<foundtemp<<" err "<<resMis<<"\n";
    }
};

struct PlancEmission : ManyVarFunction {
    PlancEmission() : ManyVarFunction() {}

    virtual double Calculate(const map<Stroka, double> &x, const Stroka &name) {
        if(x.find("Temp") == x.end() || x.find("Lambda") == x.end())
            throw info_except("Have to define variables <Temp> and <Lambda>\n");
        return BrigtnesTemp::PlancEmisE(x.find("Temp")->second, x.find("Lambda")->second);
    }
    Stroka MakeHelp() {
        return "Calculate EmisE, input Lambda(nm) Temp(K). Result in  Joule/Second/nanoMeter^3\n";
    }
};
