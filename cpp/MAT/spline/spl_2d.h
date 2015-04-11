

#pragma once

#include "spl_interface.h"
#include <mat\spl_fac.h>


struct TSpline2DCalculator : ISplineCalculatorStd {
    TSpline2DCalculator()
        : ISplineCalculatorStd() {
    }
    int GetSplineDim() {
        return 2;
    }
        

    virtual int  Evaluate (const vector<vector<double> > &dat, vector<double> &res);

//private:
//    vector<double> Min, Max;
//    double SplMisfit;
//    Stroka SplineDescr, SplineFile;
//    vector<vector<real> > SplineCoefs;
};

struct TSpline2DGenerator : ISplineGeneratorStd {
    TSpline2DGenerator() 
        : ISplineGeneratorStd()
        , NumX(100)
        , NumY(100) {
    }
    int GetSplineDim() {
        return 2;
    }
    int  Generate(vector<vector<double> > &dat, int Cont = -1);
    int read_data_state(FilterIn &in) {
        if (!ISplineGeneratorStd::read_data_state(in))
            return 0;
        Stroka tmp;
        in >> tmp >> NumX >> tmp >> NumY;
        return 1;
    }
    int save_data_state(FilterOut &out) {
        if (!ISplineGeneratorStd::save_data_state(out))
            return 0;
        out << " NumX " << NumX << " NumY " << NumY;
        return 1;
    }
private:
    int NumX, NumY;
    //Ref<TSpline2DCalculator> SplineCalculator;
    //double GenerationMisfit;

};

