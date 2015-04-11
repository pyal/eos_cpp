/* alg779_FermiDirac_int.f -- translated by f2c (version 20060506).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "lib/std/f2c.h"

/* Table of constant values */

static integer c__9 = 9;
static integer c__1 = 1;

doublereal fdm0p5_(doublereal *xvalue)
{
    /* Initialized data */

    static doublereal arrfd1[15] = { 1.7863596385102264,-.0999372007632333,
	    .0064144652216054,-4.356415371345e-4,3.0521670031e-5,
	    -2.181064811e-6,1.580050781e-7,-1.1562057e-8,8.52586e-10,
	    -6.32529e-11,4.7159e-12,-3.53e-13,2.65e-14,-2e-15,2e-16 };
    static doublereal gam1p5 = .886226925452758;
    static doublereal twoe = 5.4365636569180905;
    static integer nterm1 = 14;
    static integer nterm2 = 23;
    static integer nterm3 = 58;
    static doublereal xmin1 = -36.39023;
    static doublereal xmin2 = -708.39641;
    static doublereal xhigh = 67108864.;
    static doublereal arrfd2[24] = { 1.6877111526052352,.5978360226336983,
	    .0357226004541669,-.0132144786506426,-4.040134207447e-4,
	    5.330011846887e-4,-1.48923504863e-5,-2.18863822916e-5,
	    1.9652084277e-6,8.565830466e-7,-1.407723133e-7,-3.05175803e-8,
	    8.3524532e-9,9.02575e-10,-4.455471e-10,-1.48342e-11,2.19266e-11,
	    -6.579e-13,-1.0009e-12,9.36e-14,4.2e-14,-7.1e-15,-1.6e-15,4e-16 };
    static doublereal arrfd3[59] = { .8707195029590563,.0059833110231733,
	    -.0432670470895746,-.039308368160859,-.0191482688045932,
	    -.0065582880980158,-.0022276691516312,-8.466786936178e-4,
	    -2.807459489219e-4,-9.55575024348e-5,-3.62367662803e-5,
	    -1.09158468869e-5,-3.9356701e-6,-1.3108192725e-6,-2.468816388e-7,
	    -1.048380311e-7,2.36181487e-8,2.27145359e-8,1.45775174e-8,
	    1.53926767e-8,5.6924772e-9,5.0623068e-9,2.3426075e-9,1.2652275e-9,
	    8.927773e-10,2.994501e-10,2.822785e-10,9.10685e-11,6.96285e-11,
	    3.66225e-11,1.24351e-11,1.45019e-11,1.6645e-12,4.5856e-12,
	    6.092e-13,9.331e-13,5.238e-13,-5.6e-15,3.17e-13,-9.26e-14,
	    1.265e-13,-3.27e-14,2.76e-14,3.3e-15,-4.2e-15,1.01e-14,-7.3e-15,
	    6.4e-15,-3.7e-15,2.3e-15,-9e-16,2e-16,2e-16,-3e-16,4e-16,-3e-16,
	    2e-16,-1e-16,1e-16 };
    static doublereal zero = 0.;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal three = 3.;
    static doublereal forty2 = 42.;
    static doublereal fifty = 50.;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double exp(doublereal), sqrt(doublereal);

    /* Local variables */
    static doublereal t, x, chv, xsq, expx;
    extern doublereal cheval_(integer *, doublereal *, doublereal *);


/*   DESCRIPTION: */

/*      This function computes the Fermi-Dirac function of */
/*      order -1/2, defined as */

/*                     Int{0 to inf} t**(-1/2) / (1+exp(t-x)) dt */
/*         FDM0P5(x) = ----------------------------------------- */
/*                                 Gamma(1/2) */

/*      The function uses Chebyshev expansions which are given to */
/*      16 decimal places for x <= 2, but only 10 decimal places */
/*      for x > 2. */


/*   ERROR RETURNS: */

/*      None. */


/*   MACHINE-DEPENDENT CONSTANTS: */

/*      NTERMS1 - INTEGER - The number of terms used from the array */
/*                          ARRFD1. The recommended value is such that */
/*                               ABS(ARRFD1(NTERMS1)) < EPS/10 */
/*                          subject to 1 <= NTERMS1 <= 14. */

/*      NTERMS2 - INTEGER - The number of terms used from the array */
/*                          ARRFD2. The recommended value is such that */
/*                               ABS(ARRFD2(NTERMS2)) < EPS/10 */
/*                          subject to 1 <= NTERMS1 <= 23. */

/*      NTERMS3 - INTEGER - The number of terms used from the array */
/*                          ARRFD3. The recommended value is such that */
/*                               ABS(ARRFD3(NTERMS3)) < EPS/10 */
/*                          subject to 1 <= NTERMS3 <= 28. */

/*      XMIN1 - REAL - The value of x below which */
/*                         FDM0P5(x) = exp(x) */
/*                     to machine precision. The recommended value */
/*                     is    LN ( SQRT(2) * EPSNEG ) */

/*      XMIN2 - REAL - The value of x below which */
/*                         FDM0P5(x) = 0.0 */
/*                     to machine precision. The recommended value */
/*                     is    LN ( XMIN ) */

/*      XHIGH - REAL - The value of x above which */
/*                         FDM0P5(x) = 2 sqrt (x/pi) */
/*                     to machine precision. The recommended value */
/*                     is    1 / sqrt( 2 * EPSNEG ) */

/*      For values of EPS, EPSNEG, and XMIN the user should refer to the */
/*      paper by Cody in ACM. Trans. Math. Soft. Vol. 14 (1988) p303-311. */

/*      This code is provided with single and double precision values */
/*      of the machine-dependent parameters, suitable for machines */
/*      which satisfy the IEEE floating-point standard. */


/*   AUTHOR: */
/*          DR. ALLAN MACLEOD, */
/*          DEPT. OF MATHEMATICS AND STATISTICS, */
/*          UNIVERSITY OF PAISLEY, */
/*          HIGH ST., */
/*          PAISLEY, */
/*          SCOTLAND */
/*          PA1 2BE */

/*          (e-mail: macl-ms0@paisley.ac.uk ) */


/*   LATEST UPDATE: */
/*                 20 NOVEMBER, 1996 */


/*   Machine-dependent constants */


/*   Start calculation */

    x = *xvalue;

/*   Code for x < -1 */

    if (x < -one) {
	if (x > xmin1) {
	    expx = exp(x);
	    t = twoe * expx - one;
	    ret_val = expx * cheval_(&nterm1, arrfd1, &t);
	} else {
	    if (x < xmin2) {
		ret_val = zero;
	    } else {
		ret_val = exp(x);
	    }
	}
    } else {

/*   Code for -1 <= x <= 2 */

	if (x <= two) {
	    t = (two * x - one) / three;
	    ret_val = cheval_(&nterm2, arrfd2, &t);
	} else {

/*   Code for x > 2 */

	    ret_val = sqrt(x) / gam1p5;
	    if (x <= xhigh) {
		xsq = x * x;
		t = (fifty - xsq) / (forty2 + xsq);
		chv = cheval_(&nterm3, arrfd3, &t);
		ret_val *= one - chv / xsq;
	    }
	}
    }
    return ret_val;
} /* fdm0p5_ */

doublereal fdp0p5_(doublereal *xvalue)
{
    /* Initialized data */

    static doublereal arrfd1[14] = { 1.8862968392734597,-.0543580817644053,
	    .002364497543972,-1.21692936588e-4,6.8695130622e-6,
	    -4.112076172e-7,2.56351628e-8,-1.6465008e-9,1.081948e-10,
	    -7.2392e-12,4.915e-13,-3.38e-14,2.3e-15,-2e-16 };
    static doublereal arrfd2[24] = { 2.6982492788170612,1.2389914141133012,
	    .2291439379816278,.0090316534687279,-.0025776524691246,
	    -5.83681605388e-5,6.93609458725e-5,-1.8061670265e-6,
	    -2.1321530005e-6,1.754983951e-7,6.6532547e-8,-1.01675977e-8,
	    -1.9637597e-9,5.075769e-10,4.91469e-11,-2.33737e-11,-6.645e-13,
	    1.0115e-12,-3.13e-14,-4.12e-14,3.8e-15,1.6e-15,-3e-16,-1e-16 };
    static doublereal arrfd3[54] = { 2.5484384198009122,.0510439408960652,
	    .0077493527628294,-.0075041656584953,-.0077540826320296,
	    -.0045810844539977,-.0023431641587363,-.0011788049513591,
	    -5.802739359702e-4,-2.825350700537e-4,-1.388136651799e-4,
	    -6.80695084875e-5,-3.35356350608e-5,-1.66533018734e-5,
	    -8.2714908266e-6,-4.1425714409e-6,-2.0805255294e-6,
	    -1.0479767478e-6,-5.315273802e-7,-2.694061178e-7,-1.374878749e-7,
	    -7.02308887e-8,-3.59543942e-8,-1.85106126e-8,-9.5023937e-9,
	    -4.9184811e-9,-2.537195e-9,-1.3151532e-9,-6.835168e-10,
	    -3.538244e-10,-1.853182e-10,-9.58983e-11,-5.04083e-11,
	    -2.62238e-11,-1.37255e-11,-7.234e-12,-3.7429e-12,-2.0059e-12,
	    -1.0269e-12,-5.551e-13,-2.857e-13,-1.52e-13,-8.11e-14,-4.1e-14,
	    -2.34e-14,-1.1e-14,-6.7e-15,-3e-15,-1.9e-15,-9e-16,-5e-16,-3e-16,
	    -1e-16,-1e-16 };
    static doublereal zero = 0.;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal three = 3.;
    static doublereal forty2 = 42.;
    static doublereal fifty = 50.;
    static doublereal gam2p5 = 1.329340388179137;
    static doublereal twoe = 5.4365636569180905;
    static integer nterm1 = 13;
    static integer nterm2 = 23;
    static integer nterm3 = 53;
    static doublereal xmin1 = -35.7;
    static doublereal xmin2 = -708.394;
    static doublereal xhigh1 = 74546700.;
    static doublereal xhigh2 = 3.8392996e205;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);
    double exp(doublereal), sqrt(doublereal);

    /* Local variables */
    static doublereal t, x, chv, xsq, expx;
    extern doublereal cheval_(integer *, doublereal *, doublereal *);

    /* Fortran I/O blocks */
    static cilist io___42 = { 0, 6, 0, 0, 0 };



/*   DESCRIPTION: */

/*      This function computes the Fermi-Dirac function of */
/*      order 1/2, defined as */

/*                     Int{0 to inf} t**(1/2) / (1+exp(t-x)) dt */
/*         FDP0P5(x) = ----------------------------------------- */
/*                                 Gamma(3/2) */

/*      The function uses Chebyshev expansions which are given to */
/*      16 decimal places for x <= 2, but only 10 decimal places */
/*      for x > 2. */


/*   ERROR RETURNS: */

/*      If XVALUE too large and positive, the function value */
/*      will overflow. An error message is printed and the function */
/*      returns the value 0.0. */


/*   MACHINE-DEPENDENT CONSTANTS: */

/*      NTERMS1 - INTEGER - The number of terms used from the array */
/*                          ARRFD1. The recommended value is such that */
/*                               ABS(ARRFD1(NTERMS1)) < EPS/10 */
/*                          subject to 1 <= NTERMS1 <= 13. */

/*      NTERMS2 - INTEGER - The number of terms used from the array */
/*                          ARRFD2. The recommended value is such that */
/*                               ABS(ARRFD2(NTERMS2)) < EPS/10 */
/*                          subject to 1 <= NTERMS1 <= 23. */

/*      NTERMS3 - INTEGER - The number of terms used from the array */
/*                          ARRFD3. The recommended value is such that */
/*                               ABS(ARRFD3(NTERMS3)) < EPS/10 */
/*                          subject to 1 <= NTERMS3 <= 32. */

/*      XMIN1 - REAL - The value of x below which */
/*                         FDP0P5(x) = exp(x) */
/*                     to machine precision. The recommended value */
/*                     is   1.5*LN(2) + LN(EPSNEG) */

/*      XMIN2 - REAL - The value of x below which */
/*                         FDP0P5(x) = 0.0 */
/*                     to machine precision. The recommended value */
/*                     is    LN ( XMIN ) */

/*      XHIGH1 - REAL - The value of x above which */
/*                         FDP0P5(x) = x**(3/2)/GAMMA(5/2) */
/*                     to machine precision. The recommended value */
/*                     is   pi / SQRT(8*EPS) */

/*      XHIGH2 - REAL - The value of x above which FDP0P5 would */
/*                      overflow. The reommended value is */
/*                              (1.329*XMAX)**(2/3) */

/*      For values of EPS, EPSNEG, and XMIN the user should refer to the */
/*      paper by Cody in ACM. Trans. Math. Soft. Vol. 14 (1988) p303-311. */

/*      This code is provided with single and double precision values */
/*      of the machine-dependent parameters, suitable for machines */
/*      which satisfy the IEEE floating-point standard. */


/*   AUTHOR: */
/*          DR. ALLAN MACLEOD, */
/*          DEPT. OF MATHEMATICS AND STATISTICS, */
/*          UNIVERSITY OF PAISLEY, */
/*          HIGH ST., */
/*          PAISLEY, */
/*          SCOTLAND */
/*          PA1 2BE */

/*          (e-mail: macl-ms0@paisley.ac.uk ) */


/*   LATEST UPDATE: */
/*                 20 NOVEMBER, 1996 */


/*   Machine-dependent constants (suitable for IEEE machines) */


/*   Start calculation */

    x = *xvalue;

/*   Test for error condition */

    if (x > xhigh2) {
	s_wsle(&io___42);
	do_lio(&c__9, &c__1, "** ERROR ** - X TOO LARGE FOR FDP0P5", (ftnlen)
		36);
	e_wsle();
	ret_val = zero;
	return ret_val;
    }

/*   Code for x < -1 */

    if (x < -one) {
	if (x > xmin1) {
	    expx = exp(x);
	    t = twoe * expx - one;
	    ret_val = expx * cheval_(&nterm1, arrfd1, &t);
	} else {
	    if (x < xmin2) {
		ret_val = zero;
	    } else {
		ret_val = exp(x);
	    }
	}
    } else {

/*   Code for -1 <= x <= 2 */

	if (x <= two) {
	    t = (two * x - one) / three;
	    ret_val = cheval_(&nterm2, arrfd2, &t);
	} else {

/*   Code for x > 2 */

	    ret_val = x * sqrt(x) / gam2p5;
	    if (x <= xhigh1) {
		xsq = x * x;
		t = (fifty - xsq) / (forty2 + xsq);
		chv = cheval_(&nterm3, arrfd3, &t);
		ret_val *= one + chv / xsq;
	    }
	}
    }
    return ret_val;
} /* fdp0p5_ */

doublereal fdp1p5_(doublereal *xvalue)
{
    /* Initialized data */

    static doublereal arrfd1[13] = { 1.940654921037865,-.0287867475518043,
	    8.509157952313e-4,-3.32784525669e-5,1.5171202058e-6,
	    -7.62200874e-8,4.0955489e-9,-2.311964e-10,1.35537e-11,-8.187e-13,
	    5.07e-14,-3.2e-15,2e-16 };
    static doublereal arrfd2[23] = { 3.5862251615634306,1.8518290056265751,
	    .461234910241715,.0579303976126881,.0017043790554875,
	    -3.970520122496e-4,-7.070249189e-6,7.6599748792e-6,
	    -1.857811333e-7,-1.832237956e-7,1.39249495e-8,4.6702027e-9,
	    -6.671984e-10,-1.161292e-10,2.84438e-11,2.4906e-12,-1.1431e-12,
	    -2.79e-14,4.39e-14,-1.4e-15,-1.6e-15,1e-16,1e-16 };
    static doublereal arrfd3[56] = { 12.1307581736884627,-.1547501111287255,
	    -.0739007388850999,-.0307235377959258,-.0114548579330328,
	    -.0040567636809539,-.0013980158373227,-4.454901810153e-4,
	    -1.173946112704e-4,-1.48408980093e-5,1.18895154223e-5,
	    1.46476958178e-5,1.1322874173e-5,7.5762292948e-6,4.7120400466e-6,
	    2.8132628202e-6,1.6370517341e-6,9.351076272e-7,5.27868921e-7,
	    2.95107987e-7,1.63860019e-7,9.05205409e-8,4.97756975e-8,
	    2.72955863e-8,1.49214585e-8,8.1420359e-9,4.43492e-9,2.4116032e-9,
	    1.3105018e-9,7.109736e-10,3.856721e-10,2.089529e-10,1.131735e-10,
	    6.12785e-11,3.31448e-11,1.79419e-11,9.6953e-12,5.2463e-12,
	    2.8343e-12,1.5323e-12,8.284e-13,4.472e-13,2.421e-13,1.304e-13,
	    7.07e-14,3.81e-14,2.06e-14,1.11e-14,6e-15,3.3e-15,1.7e-15,1.1e-15,
	    5e-16,3e-16,1e-16,1e-16 };
    static doublereal zero = 0.;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal three = 3.;
    static doublereal forty2 = 42.;
    static doublereal fifty = 50.;
    static doublereal gam3p5 = 3.323350970447843;
    static doublereal twoe = 5.4365636569180905;
    static integer nterm1 = 12;
    static integer nterm2 = 22;
    static integer nterm3 = 55;
    static doublereal xmin1 = -35.004;
    static doublereal xmin2 = -708.396418;
    static doublereal xhigh1 = 166674733.2;
    static doublereal xhigh2 = 3.204467e123;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);
    double exp(doublereal), sqrt(doublereal);

    /* Local variables */
    static doublereal t, x, chv, xsq, expx;
    extern doublereal cheval_(integer *, doublereal *, doublereal *);

    /* Fortran I/O blocks */
    static cilist io___66 = { 0, 6, 0, 0, 0 };



/*   DESCRIPTION: */

/*      This function computes the Fermi-Dirac function of */
/*      order 3/2, defined as */

/*                     Int{0 to inf} t**(3/2) / (1+exp(t-x)) dt */
/*         FDP1P5(x) = ----------------------------------------- */
/*                                 Gamma(5/2) */

/*      The function uses Chebyshev expansions which are given to */
/*      16 decimal places for x <= 2, but only 10 decimal places */
/*      for x > 2. */


/*   ERROR RETURNS: */

/*      If XVALUE too large and positive, the function value */
/*      will overflow. An error message is printed and the function */
/*      returns the value 0.0. */


/*   MACHINE-DEPENDENT CONSTANTS: */

/*      NTERMS1 - INTEGER - The number of terms used from the array */
/*                          ARRFD1. The recommended value is such that */
/*                               ABS(ARRFD1(NTERMS1)) < EPS/10 */
/*                          subject to 1 <= NTERMS1 <= 12. */

/*      NTERMS2 - INTEGER - The number of terms used from the array */
/*                          ARRFD2. The recommended value is such that */
/*                               ABS(ARRFD2(NTERMS2)) < EPS/10 */
/*                          subject to 1 <= NTERMS1 <= 22. */

/*      NTERMS3 - INTEGER - The number of terms used from the array */
/*                          ARRFD3. The recommended value is such that */
/*                               ABS(ARRFD3(NTERMS3)) < EPS/10 */
/*                          subject to 1 <= NTERMS3 <= 33. */

/*      XMIN1 - REAL - The value of x below which */
/*                         FDP1P5(x) = exp(x) */
/*                     to machine precision. The recommended value */
/*                     is   2.5*LN(2) + LN(EPSNEG) */

/*      XMIN2 - REAL - The value of x below which */
/*                         FDP1P5(x) = 0.0 */
/*                     to machine precision. The recommended value */
/*                     is    LN ( XMIN ) */

/*      XHIGH1 - REAL - The value of x above which */
/*                         FDP1P5(x) = x**(5/2)/GAMMA(7/2) */
/*                     to machine precision. The recommended value */
/*                     is   pi * SQRT(1.6/EPS) */

/*      XHIGH2 - REAL - The value of x above which FDP1P5 would */
/*                      overflow. The reommended value is */
/*                              (3.233509*XMAX)**(2/5) */

/*      For values of EPS, EPSNEG, and XMIN the user should refer to the */
/*      paper by Cody in ACM. Trans. Math. Soft. Vol. 14 (1988) p303-311. */

/*      This code is provided with single and double precision values */
/*      of the machine-dependent parameters, suitable for machines */
/*      which satisfy the IEEE floating-point standard. */


/*   AUTHOR: */
/*          DR. ALLAN MACLEOD, */
/*          DEPT. OF MATHEMATICS AND STATISTICS, */
/*          UNIVERSITY OF PAISLEY, */
/*          HIGH ST., */
/*          PAISLEY, */
/*          SCOTLAND */
/*          PA1 2BE */

/*          (e-mail: macl_ms0@paisley.ac.uk ) */


/*   LATEST UPDATE: */
/*                 21 NOVEMBER, 1996 */


/*   Machine-dependent constants (suitable for IEEE machines) */


/*   Start calculation */

    x = *xvalue;

/*   Test for error condition */

    if (x > xhigh2) {
	s_wsle(&io___66);
	do_lio(&c__9, &c__1, "** ERROR ** - X TOO LARGE FOR FDP1P5", (ftnlen)
		36);
	e_wsle();
	ret_val = zero;
	return ret_val;
    }

/*   Code for x < -1 */

    if (x < -one) {
	if (x > xmin1) {
	    expx = exp(x);
	    t = twoe * expx - one;
	    ret_val = expx * cheval_(&nterm1, arrfd1, &t);
	} else {
	    if (x < xmin2) {
		ret_val = zero;
	    } else {
		ret_val = exp(x);
	    }
	}
    } else {

/*   Code for -1 <= x <= 2 */

	if (x <= two) {
	    t = (two * x - one) / three;
	    ret_val = cheval_(&nterm2, arrfd2, &t);
	} else {

/*   Code for x > 2 */

	    ret_val = x * x * sqrt(x) / gam3p5;
	    if (x <= xhigh1) {
		xsq = x * x;
		t = (fifty - xsq) / (forty2 + xsq);
		chv = cheval_(&nterm3, arrfd3, &t);
		ret_val *= one + chv / xsq;
	    }
	}
    }
    return ret_val;
} /* fdp1p5_ */

doublereal fdp2p5_(doublereal *xvalue)
{
    /* Initialized data */

    static doublereal arrfd1[12] = { 1.9694416685896693,-.0149691794643492,
	    3.006955816627e-4,-8.946248595e-6,3.298072025e-7,-1.39239298e-8,
	    6.455885e-10,-3.20623e-11,1.6783e-12,-9.16e-14,5.2e-15,-3e-16 };
    static doublereal arrfd2[22] = { 4.2642838398655301,2.3437426884912867,
	    .6727119780052076,.1148826327965569,.0109363968046758,
	    2.567173957015e-4,-5.05889983911e-5,-7.376215774e-7,
	    7.352998758e-7,-1.66421736e-8,-1.40920499e-8,9.949192e-10,
	    2.991457e-10,-4.01332e-11,-6.3546e-12,1.4793e-12,1.181e-13,
	    -5.24e-14,-1.1e-15,1.8e-15,-1e-16,-1e-16 };
    static doublereal arrfd3[62] = { 30.2895676859802579,1.1678976642060562,
	    .6420591800821472,.3461723868407417,.1840816790781889,
	    .0973092435354509,.0513973292675393,.0271709801041757,
	    .0143833271401165,.0076264863952155,.0040503695767202,
	    .0021543961464149,.0011475689901777,6.120622369282e-4,
	    3.268340337859e-4,1.747145522742e-4,9.3487845786e-5,
	    5.00692212553e-5,2.68373821846e-5,1.43957191251e-5,7.72724407e-6,
	    4.1503820336e-6,2.2305118261e-6,1.1993697093e-6,6.452344369e-7,
	    3.472822881e-7,1.869964215e-7,1.007300272e-7,5.42807561e-8,
	    2.92607829e-8,1.57785918e-8,8.5110768e-9,4.592276e-9,2.4785001e-9,
	    1.3380255e-9,7.225103e-10,3.90235e-10,2.108157e-10,1.139122e-10,
	    6.15638e-11,3.32781e-11,1.79919e-11,9.7288e-12,5.2617e-12,
	    2.8461e-12,1.5397e-12,8.331e-13,4.508e-13,2.44e-13,1.321e-13,
	    7.15e-14,3.87e-14,2.1e-14,1.14e-14,6.1e-15,3.3e-15,1.8e-15,
	    1.1e-15,5e-16,3e-16,2e-16,1e-16 };
    static doublereal zero = 0.;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal three = 3.;
    static doublereal forty2 = 42.;
    static doublereal fifty = 50.;
    static doublereal gam4p5 = 11.63172839656745;
    static doublereal twoe = 5.4365636569180905;
    static integer nterm1 = 11;
    static integer nterm2 = 21;
    static integer nterm3 = 61;
    static doublereal xmin1 = -34.3107854;
    static doublereal xmin2 = -708.396418;
    static doublereal xhigh1 = 254599860.5;
    static doublereal xhigh2 = 2.383665e88;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);
    double exp(doublereal), sqrt(doublereal);

    /* Local variables */
    static doublereal t, x, chv, xsq, expx;
    extern doublereal cheval_(integer *, doublereal *, doublereal *);

    /* Fortran I/O blocks */
    static cilist io___90 = { 0, 6, 0, 0, 0 };



/*   DESCRIPTION: */

/*      This function computes the Fermi-Dirac function of */
/*      order 5/2, defined as */

/*                     Int{0 to inf} t**(5/2) / (1+exp(t-x)) dt */
/*         FDP2P5(x) = ----------------------------------------- */
/*                                 Gamma(7/2) */

/*      The function uses Chebyshev expansions which are given to */
/*      16 decimal places for x <= 2, but only 10 decimal places */
/*      for x > 2. */


/*   ERROR RETURNS: */

/*      If XVALUE too large and positive, the function value */
/*      will overflow. An error message is printed and the function */
/*      returns the value 0.0. */


/*   MACHINE-DEPENDENT CONSTANTS: */

/*      NTERMS1 - INTEGER - The number of terms used from the array */
/*                          ARRFD1. The recommended value is such that */
/*                               ABS(ARRFD1(NTERMS1)) < EPS/10 */
/*                          subject to 1 <= NTERMS1 <= 11. */

/*      NTERMS2 - INTEGER - The number of terms used from the array */
/*                          ARRFD2. The recommended value is such that */
/*                               ABS(ARRFD2(NTERMS2)) < EPS/10 */
/*                          subject to 1 <= NTERMS1 <= 21. */

/*      NTERMS3 - INTEGER - The number of terms used from the array */
/*                          ARRFD3. The recommended value is such that */
/*                               ABS(ARRFD3(NTERMS3)) < EPS/10 */
/*                          subject to 1 <= NTERMS3 <= 39. */

/*      XMIN1 - REAL - The value of x below which */
/*                         FDP2P5(x) = exp(x) */
/*                     to machine precision. The recommended value */
/*                     is   3.5*LN(2) + LN(EPSNEG) */

/*      XMIN2 - REAL - The value of x below which */
/*                         FDP2P5(x) = 0.0 */
/*                     to machine precision. The recommended value */
/*                     is    LN ( XMIN ) */

/*      XHIGH1 - REAL - The value of x above which */
/*                         FDP2P5(x) = x**(7/2)/GAMMA(9/2) */
/*                     to machine precision. The recommended value */
/*                     is   pi * SQRT(35/(12*EPS)) */

/*      XHIGH2 - REAL - The value of x above which FDP2P5 would */
/*                      overflow. The reommended value is */
/*                              (11.6317*XMAX)**(2/7) */

/*      For values of EPS, EPSNEG, and XMIN the user should refer to the */
/*      paper by Cody in ACM. Trans. Math. Soft. Vol. 14 (1988) p303-311. */

/*      This code is provided with single and double precision values */
/*      of the machine-dependent parameters, suitable for machines */
/*      which satisfy the IEEE floating-point standard. */


/*   AUTHOR: */
/*          DR. ALLAN MACLEOD, */
/*          DEPT. OF MATHEMATICS AND STATISTICS, */
/*          UNIVERSITY OF PAISLEY, */
/*          HIGH ST., */
/*          PAISLEY, */
/*          SCOTLAND */
/*          PA1 2BE */

/*          (e-mail: macl-ms0@paisley.ac.uk ) */


/*   LATEST UPDATE: */
/*                 21 NOVEMBER, 1996 */


/*   Machine-dependent constants (suitable for IEEE machines) */


/*   Start calculation */

    x = *xvalue;

/*   Test for error condition */

    if (x > xhigh2) {
	s_wsle(&io___90);
	do_lio(&c__9, &c__1, "** ERROR ** - X TOO LARGE FOR FDP2P5", (ftnlen)
		36);
	e_wsle();
	ret_val = zero;
	return ret_val;
    }

/*   Code for x < -1 */

    if (x < -one) {
	if (x > xmin1) {
	    expx = exp(x);
	    t = twoe * expx - one;
	    ret_val = expx * cheval_(&nterm1, arrfd1, &t);
	} else {
	    if (x < xmin2) {
		ret_val = zero;
	    } else {
		ret_val = exp(x);
	    }
	}
    } else {

/*   Code for -1 <= x <= 2 */

	if (x <= two) {
	    t = (two * x - one) / three;
	    ret_val = cheval_(&nterm2, arrfd2, &t);
	} else {

/*   Code for x > 2 */

	    ret_val = x * x * x * sqrt(x) / gam4p5;
	    if (x <= xhigh1) {
		xsq = x * x;
		t = (fifty - xsq) / (forty2 + xsq);
		chv = cheval_(&nterm3, arrfd3, &t);
		ret_val *= one + chv / xsq;
	    }
	}
    }
    return ret_val;
} /* fdp2p5_ */

doublereal cheval_(integer *n, doublereal *a, doublereal *t)
{
    /* Initialized data */

    static doublereal zero = 0.;
    static doublereal half = .5;
    static doublereal test = .6;
    static doublereal two = 2.;

    /* System generated locals */
    doublereal ret_val;

    /* Local variables */
    static integer i__;
    static doublereal d1, d2, u0, u1, u2, tt;


/*   DESCRIPTION: */

/*      This function evaluates a Chebyshev series, using the */
/*      Clenshaw method with Reinsch modification, as analysed */
/*      in the paper by Oliver. */


/*   INPUT PARAMETERS */

/*       N - INTEGER - The no. of terms in the sequence */

/*       A - REAL ARRAY, dimension 0 to N - The coefficients of */
/*           the Chebyshev series */

/*       T - REAL - The value at which the series is to be */
/*           evaluated */


/*   REFERENCES */

/*        "An error analysis of the modified Clenshaw method for */
/*         evaluating Chebyshev and Fourier series" J. Oliver, */
/*         J.I.M.A., vol. 20, 1977, pp379-391 */


/*   MACHINE-DEPENDENT CONSTANTS: NONE */


/*   INTRINSIC FUNCTIONS USED; */

/*      ABS */


/*    AUTHOR:  Dr. Allan J. MacLeod, */
/*             Dept. of Mathematics and Statistics, */
/*             University of Paisley , */
/*             High St., */
/*             PAISLEY, */
/*             SCOTLAND */
/*             ( e-mail:  macl-ms0@paisley.ac.uk ) */


/*   LATEST MODIFICATION: */
/*                       21 September , 1995 */


    u1 = zero;

/*   If ABS ( T )  < 0.6 use the standard Clenshaw method */

    if (abs(*t) < test) {
	u0 = zero;
	tt = *t + *t;
	for (i__ = *n; i__ >= 0; --i__) {
	    u2 = u1;
	    u1 = u0;
	    u0 = tt * u1 + a[i__] - u2;
/* L100: */
	}
	ret_val = (u0 - u2) / two;
    } else {

/*   If ABS ( T )  > =  0.6 use the Reinsch modification */

	d1 = zero;

/*   T > =  0.6 code */

	if (*t > zero) {
	    tt = *t - half - half;
	    tt += tt;
	    for (i__ = *n; i__ >= 0; --i__) {
		d2 = d1;
		u2 = u1;
		d1 = tt * u2 + a[i__] + d2;
		u1 = d1 + u2;
/* L200: */
	    }
	    ret_val = (d1 + d2) / two;
	} else {

/*   T < =  -0.6 code */

	    tt = *t + half + half;
	    tt += tt;
	    for (i__ = *n; i__ >= 0; --i__) {
		d2 = d1;
		u2 = u1;
		d1 = tt * u2 + a[i__] - d2;
		u1 = d1 - u2;
/* L300: */
	    }
	    ret_val = (d1 - d2) / two;
	}
    }
    return ret_val;
} /* cheval_ */

