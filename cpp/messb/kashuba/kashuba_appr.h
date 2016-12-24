#ifndef __KASHUBA_APPR_H
#define __KASHUBA_APPR_H


#include "..\messb_in.h"
struct RndAngleGenerator_Base;
struct RndAngleGenerator_Kashuba : RndAngleGenerator_Base {
    const double Phi0, MainHarm, FullTime, StrongShort;
    const int N, FurieDiscr;
    RndAngleGenerator_Kashuba(
        double phi0,
        int n,
        double mainharm,
        int furiediscr,
        double fulltime,
        double strongshort);
    virtual void Rnd(VecCl &Rnd_Angle, VecCl &Rnd_Time);
};


#endif