

#pragma once

namespace SpecFunc {

    enum FermiDirac { Im0_5, Ip0_5, Ip1_5, Ip2_5 };
// math_func/alg779 - fortran src
// computes different powers of FermiInt(I,x)*Gamma(I)
// Im0_5
/*      This function computes the Fermi-Dirac function of */
/*      order -1/2, defined as */
/*                     Int{0 to inf} t**(-1/2) / (1+exp(t-x)) dt */
/*         FDM0P5(x) = ----------------------------------------- */
/*                                 Gamma(1/2) */
// Ip0_5
/*      This function computes the Fermi-Dirac function of */
/*      order 1/2, defined as */

/*                     Int{0 to inf} t**(1/2) / (1+exp(t-x)) dt */
/*         FDP0P5(x) = ----------------------------------------- */
/*                                 Gamma(3/2) */
// Ip1_5
/*      This function computes the Fermi-Dirac function of */
/*      order 3/2, defined as */

/*                     Int{0 to inf} t**(3/2) / (1+exp(t-x)) dt */
/*         FDP1P5(x) = ----------------------------------------- */
/*                                 Gamma(5/2) */
    double FermiDiracInt( FermiDirac power, double val);


// math_func/specFunc/gamma - algama
// This routine calculates the GAMMA function for a real argument X. 
// doublereal dgamma_(doublereal *x)
    double Gamma(double x);
// This routine calculates the LOG(GAMMA) function for a positive real X
//doublereal dlgama_(doublereal *x)
    double LogGamma(double x);


// math_func/specFunc/ei
//
//  E1(x), and  exp(-x)*Ei(x)  for real arguments  x  where 
//  Ei(x) =   integral (from t=-infinity to t=x) (exp(t)/t),  x > 0, 
//  Ei(x) =  -integral (from t=-x to t=infinity) (exp(t)/t),  x < 0, 
/*       Call                 ARG             RESULT         INT */
/*      EI(X)              X .NE. 0          Ei(X)            1 */
/*      EONE(X)            X .GT. 0         -Ei(-X)           2 */
/*      EXPEI(X)           X .NE. 0          exp(-X)*Ei(X)    3 */



// Erf(z) = 2/Pi^0.5 * Integrate[exp(-t^2),{t,0,z}]
    double Erf_slow(double z);
    double Erf(double z);
// Erf^-1(z)
    double iErf(double z);
    double iErf_leo(double z);


}; // SpecFunc 

