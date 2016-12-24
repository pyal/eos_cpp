#include <lib/precompiled/math.h>


#include "SpecFunc.H"
#include "lib/std/stdexception.h"

#include "mat/quad.h"
#include "f2c/math_func/spec_func/lea.h"

#include "lib/std/f2c.h"

extern "C" {
extern doublereal dgamma_(doublereal *x);
extern doublereal dlgama_(doublereal *x);
extern doublereal fdm0p5_(doublereal *xvalue);
extern doublereal fdp0p5_(doublereal *xvalue);
extern doublereal fdp1p5_(doublereal *xvalue);
extern doublereal ppnd7_(real *p, integer *ifault);          //              prec 1e-7
extern doublereal ppnd16_(doublereal *p, integer *ifault);   //       prec 1e-16
}


// computes different powers of FermiInt(I,x)*Gamma(I)
//enum FermiDirac { Im0_5, Ip0_5, Ip1_5, Ip2_5 };
double SpecFunc::FermiDiracInt(FermiDirac power, double val) {
    doublereal X = val;
    switch(power) {
    case Im0_5:
        return fdm0p5_(&X) * Gamma(0.5);
    case Ip0_5:
        return fdp0p5_(&X) * Gamma(1.5);
    case Ip1_5:
        return fdp1p5_(&X) * Gamma(2.5);
    default:
        throw(info_except("Unknown type of Fermi integral %i ", int(power)));
    }
}


// math_func/specFunc/gamma - algama
// This routine calculates the GAMMA function for a real argument X.
// doublereal dgamma_(doublereal *x)

double SpecFunc::Gamma(double x) {
    doublereal X = x;
    return dgamma_(&X);
}
// This routine calculates the LOG(GAMMA) function for a positive real X
//doublereal dlgama_(doublereal *x)
double SpecFunc::LogGamma(double x) {
    doublereal X = x;
    return dlgama_(&X);
}

double Erf_slowInt(double t) {
    return exp(-t * t);
}
// Erf(z) = 2/Pi^0.5 * Integrate[exp(-t^2),{t,0,z}]
double SpecFunc::Erf_slow(double z) {
    double ResErr, AbsErr = 1e-9;
    int Err;
    double ret = Qags_IntSubInt(Erf_slowInt, 0, z, AbsErr, AbsErr, 100, ResErr, Err);
    if(ResErr > AbsErr)
        throw info_except("Large error %f. Have to be %f\n", ResErr, AbsErr);
    return ret * 2 * sqrt(M_PI);
}
double SpecFunc::Erf(double z) {
    return 1 - 2 * SpecFunc::stdnormal_cdf(-z * M_SQRT2);
}
double SpecFunc::iErf_leo(double z) {
    return SpecFunc::stdnormal_inv(z);
}
double SpecFunc::iErf(double z) {
    integer err;
    doublereal p = z;
    return ppnd16_(&p, &err);
}


//double SpecFunc::FermiDiracInt( FermiDirac power, double val, double tau){
//    doublereal X = val;
//    switch (power){
//        case Im0_5:
//            return fdm0p5_(&X)*Gamma(0.5);
//        case Ip0_5:
//            return fdp0p5_(&X)*Gamma(1.5);
//        case Ip1_5:
//            return fdp1p5_(&X)*Gamma(2.5);
//        default :
//            throw(info_except("Unknown type of Fermi integral %i ",int(power)));
//    }
//}
