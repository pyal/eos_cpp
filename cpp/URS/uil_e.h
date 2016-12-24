
#pragma once

#include "mat/spl_fac.h"
#include "mat/fun_fac.h"
#include "uil_reg.h"

struct ExternalEnergyInput : OneVarFunction {
    CurveSpline TimeDep, SpaceDep;
    double TimeInt, SpaceInt, SpaceMin, SpaceMax, TimeMin, TimeMax;
    Stroka TimeDepName, SpaceDepName;
    double MinX, MaxX, MinT, MaxT, CurTime, NextTime, TimeIntNormCoef;   //
    map<int, double> RegNum2Coef;
    //int RegionNumber;
    ExternalEnergyInput();   // : OneVarFunction(), TimeInt(1), SpaceMin(1), SpaceMax(100), TimeDepName("e_t.dat"), SpaceDepName("e_r.dat") {};
    double Calculate(double x) {
        if(x < SpaceMin || x > SpaceMax)
            return 0;
        double shift = (x - SpaceMin) / SpaceMax;
        shift = MinX + shift * (MaxX - MinX);
        double spaceI = SpaceDep.Evaluate(x);
        return spaceI;
    }
    void ClcAddition(Region *Reg, double CurTime, double CurStp) {
        double TimeCoef =
            ClcXint(CurTime + 0.5 * CurStp, MinT, MaxT, TimeMin, TimeMax, TimeDep) *
            CurStp * TimeIntNormCoef;
        //Body *body = Reg->B[RegionNumber];
        //for(size_t i = 1; i <= body->NumPnt - 1; i++)
        //    body->Energy[i] += ClcXint(body->Pos[i], MinX, MaxX, SpaceMin, SpaceMax, SpaceDep) * TimeCoef;
        for(map<int, double>::iterator it = RegNum2Coef.begin(); it != RegNum2Coef.end();
            it++) {
            Body *body = Reg->B[it->first];
            body->UnfixAll = 1;
            for(size_t i = 1; i <= body->NumPnt - 1; i++)
                //body->Energy[i] += ClcXint(body->Pos[i], MinX, MaxX, SpaceMin, SpaceMax, SpaceDep) * body->Dencity[i] * TimeCoef * it->second;
                body->Energy[i] +=
                    ClcXint(body->Pos[i], MinX, MaxX, SpaceMin, SpaceMax, SpaceDep) *
                    TimeCoef * it->second;
        }
        //exit(1);
    }
    inline double ClcXint(
        double x,
        double gMin,
        double gMax,
        double sMin,
        double sMax,
        CurveSpline &gSpl) {
        if(x < sMin || x > sMax)
            return 0;
        double shift = (x - sMin) / (sMax - sMin);   // / sMax;
        shift = gMin + shift * (gMax - gMin);
        double spaceI = gSpl.Evaluate(shift);
        //cout<<x<<" "<<shift<<" "<<spaceI<<" "<<gMin<<" "<<gMax<<" "<<sMin<<" "<<sMax<<"\n";
        return spaceI;
    }
    virtual int read_data_state(FilterIn &input) {
        char tmp[256];
        input >> tmp >> TimeDepName >> tmp >> SpaceDepName >> tmp >> TimeInt >> tmp >>
            SpaceInt >> tmp >> SpaceMin >> tmp >> SpaceMax;
        input >> tmp >> TimeMin >> tmp >> TimeMax;
        input >> tmp;
        SavableClass::ExeptionCheck(
            input, "{", "ExternalEnergyInput - expected begin of matter_coefs map...\n");
        RegNum2Coef.clear();
        while(!SavableClass::TestNextChar(input, '}') && input) {
            int n;
            double x;
            input >> n >> x;
            RegNum2Coef[n] = x;
            SavableClass::ExeptionCheck(input, ";", "Expected matters delimiter ;\n");
        }

        CurveSpline::SplineFromData(
            ~TimeDepName, TimeDep, 1e-7, "Time dependence spline");
        CurveSpline::SplineFromData(
            ~SpaceDepName, SpaceDep, 1e-7, "Space dependence spline");
        map<Stroka, double> bnd = SpaceDep.GetBoundary();
        MinX = bnd["Xmin"];
        MaxX = bnd["Xmax"];
        bnd = TimeDep.GetBoundary();
        MinT = bnd["Xmin"];
        MaxT = bnd["Xmax"];
        double tNorm = SplineXIO::Integrate(MinT, MaxT, 1e-7, &SpaceDep);
        tNorm *= (TimeMin - TimeMax) / (MinT - MaxT);
        TimeIntNormCoef = 1e-9 / tNorm * 2.405874;   //3.326;
        TimeIntNormCoef *= SpaceInt * TimeInt;

        return !(!input);
    }
    virtual int save_data_state(FilterOut &output) {
        output << " TimeDepName " << TimeDepName << " SpaceDepName " << SpaceDepName
               << " TimeInt " << TimeInt << " SpaceInt " << SpaceInt << " SpaceMin "
               << SpaceMin << " SpaceMax " << SpaceMax << " TimeMin " << TimeMin
               << " TimeMax " << TimeMax;
        output << " RegNum2Coef  { ";
        for(map<int, double>::iterator it = RegNum2Coef.begin(); it != RegNum2Coef.end();
            it++)
            output << it->first << " " << it->second << " ; ";
        output << " } ";
        return 1;
    }
    Stroka MakeHelp() {
        Stroka res =
            "Clc Energy input for time. TimeInt - given in number of beam 300Mev particles (10^9) gives energy of helium (3.326kJ). So for helium RegNum2Coef = 1, TimeInt - number of particles, for other matters - RegNum2Coef - define relative to helium absorption, SpaceInt * Int(x) - have to give 1 for the point of interest...\n";

        return res;
    }
};
