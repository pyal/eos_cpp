
#pragma once

#include "spl_interface.h"
#include "mat/matrics.h"


struct TSpline1DCalculator : ISplineCalculatorStd {  
    TSpline1DCalculator(int splineOrder = 3) 
        : ISplineCalculatorStd()
        , SplineOrder(splineOrder) {
    }


    virtual int  Evaluate (const vector<vector<double> > &dat, vector<double> &res);
    virtual void SaveSplineData(FilterOut &out) {
        out << " SplineOrder " << SplineOrder;
        ISplineCalculatorStd::SaveSplineData(out);
    }
    virtual void ReadSplineData(FilterIn &in) {
        Stroka tmp;
        in >> tmp >> SplineOrder;
    }
    void SetOrder(int splineOrder) {
        SplineOrder = splineOrder;
    }
    int GetSplineDim() {
        return 1;
    }

private:
    int SplineOrder;
//    vector<double> Min, Max;
//    double SplMisfit;
//    Stroka SplineDescr, SplineFile;
//    vector<vector<real> > SplineCoefs;
};

struct TSpline1DGenerator : ISplineGeneratorStd {
    TSpline1DGenerator(int splineOrder = 3) 
        : ISplineGeneratorStd()
        , SplineOrder(splineOrder)
        , NumX(100) 
        , MinXStep(1e-3) {
    }

    int GetSplineDim() {
        return 1;
    }
       
    int  Generate(vector<vector<double> > &dat, int Cont = 0);
    int read_data_state(FilterIn &in) {
        if (!ISplineGeneratorStd::read_data_state(in))
            return 0;
        Stroka tmp;
        in >> tmp >> NumX >> tmp >> SplineOrder >> tmp >> MinXStep;
        return 1;
    }
    int save_data_state(FilterOut &out) {
        if (!ISplineGeneratorStd::save_data_state(out))
            return 0;
        out << " NumX " << NumX << " SplineOrder " << SplineOrder << " MinXStep " << MinXStep;
        return 1;
    }
private:
    int NumX, SplineOrder;
    double MinXStep;
    void PushItem(vector<vector<double> > &dst, const vector<vector<double> > &src, int i) {
        for(size_t o = 0; o < src.size(); o++) 
            dst[o].push_back(src[o][i]);
    }
    void CheckMinXStep(vector<vector<double> > &dst, const vector<vector<double> > &src) {
        dst.clear();
        if (MinXStep <= 0) {
            for(size_t o = 0; o < src.size(); o++) 
                dst.push_back(src[o]);
            return;
        }
        dst.resize(src.size());
        VecCl x(src[0]);
        double min = VecCl::Min(x), max = VecCl::Max(x);
        int num = (int)src[0].size();
        double minStepVal = (max - min) / num * MinXStep;
        PushItem(dst, src, 0);
        for(size_t i = 1; i < src[0].size(); i++) 
            if (src[0][i] - dst[0][dst[0].size() - 1] > minStepVal)
                PushItem(dst, src, (int)i);
//cout << " got numX " << dst[0].size() << " from " << src[0].size() << "\n";
//cout.flush();

    }
    //Ref<TSpline2DCalculator> SplineCalculator;
    //double GenerationMisfit;

};

