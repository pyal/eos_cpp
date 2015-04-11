/* qk51.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Table of constant values */

static integer c__4 = 4;
static integer c__1 = 1;
static doublereal c_b7 = 1.5;

/* Subroutine */ int qk51_(E_fp f, real *a, real *b, real *result, real *
	abserr, real *resabs, real *resasc)
{
    /* Initialized data */

    static real xgk[26] = { (float).9992621049926098,(float).9955569697904981,
	    (float).9880357945340772,(float).9766639214595175,(float)
	    .9616149864258425,(float).9429745712289743,(float)
	    .9207471152817016,(float).8949919978782754,(float)
	    .8658470652932756,(float).833442628760834,(float)
	    .7978737979985001,(float).7592592630373576,(float)
	    .7177664068130844,(float).6735663684734684,(float)
	    .6268100990103174,(float).577662930241223,(float)
	    .5263252843347192,(float).473002731445715,(float)
	    .4178853821930377,(float).3611723058093878,(float)
	    .3030895389311078,(float).2438668837209884,(float)
	    .1837189394210489,(float).1228646926107104,(float)
	    .06154448300568508,(float)0. };
    static real wgk[26] = { (float).001987383892330316,(float)
	    .005561932135356714,(float).009473973386174152,(float)
	    .01323622919557167,(float).0168478177091283,(float)
	    .02043537114588284,(float).02400994560695322,(float)
	    .02747531758785174,(float).03079230016738749,(float)
	    .03400213027432934,(float).03711627148341554,(float)
	    .04008382550403238,(float).04287284502017005,(float)
	    .04550291304992179,(float).04798253713883671,(float)
	    .05027767908071567,(float).05236288580640748,(float)
	    .05425112988854549,(float).05595081122041232,(float)
	    .05743711636156783,(float).05868968002239421,(float)
	    .05972034032417406,(float).06053945537604586,(float)
	    .06112850971705305,(float).06147118987142532,(float)
	    .06158081806783294 };
    static real wg[13] = { (float).01139379850102629,(float)
	    .02635498661503214,(float).04093915670130631,(float)
	    .05490469597583519,(float).06803833381235692,(float)
	    .08014070033500102,(float).09102826198296365,(float)
	    .1005359490670506,(float).1085196244742637,(float)
	    .1148582591457116,(float).1194557635357848,(float).12224244299031,
	    (float).1231760537267155 };

    /* System generated locals */
    real r__1, r__2;
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    real absc, resg, resk, fsum, fval1, fval2;
    integer jtwm1, j;
    real hlgth, centr, reskh, uflow;
    extern doublereal r1mach_(integer *);
    real fc, epmach, dhlgth, fv1[25], fv2[25];
    integer jtw;

/* ***begin prologue  qk51 */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a1a2 */
/* ***keywords  51-point gauss-kronrod rules */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math & progr. div. - k.u.leuven */
/* ***purpose  to compute i = integral of f over (a,b) with error */
/*                           estimate */
/*                       j = integral of abs(f) over (a,b) */
/* ***description */

/*           integration rules */
/*           standard fortran subroutine */
/*           real version */

/*           parameters */
/*            on entry */
/*              f      - real */
/*                       function subroutine defining the integrand */
/*                       function f(x). the actual name for f needs to be */
/*                       declared e x t e r n a l in the calling program. */

/*              a      - real */
/*                       lower limit of integration */

/*              b      - real */
/*                       upper limit of integration */

/*            on return */
/*              result - real */
/*                       approximation to the integral i */
/*                       result is computed by applying the 51-point */
/*                       kronrod rule (resk) obtained by optimal addition */
/*                       of abscissae to the 25-point gauss rule (resg). */

/*              abserr - real */
/*                       estimate of the modulus of the absolute error, */
/*                       which should not exceed abs(i-result) */

/*              resabs - real */
/*                       approximation to the integral j */

/*              resasc - real */
/*                       approximation to the integral of abs(f-i/(b-a)) */
/*                       over (a,b) */

/* ***references  (none) */
/* ***routines called  r1mach */
/* ***end prologue  qk51 */



/*           the abscissae and weights are given for the interval (-1,1). */
/*           because of symmetry only the positive abscissae and their */
/*           corresponding weights are given. */

/*           xgk    - abscissae of the 51-point kronrod rule */
/*                    xgk(2), xgk(4), ...  abscissae of the 25-point */
/*                    gauss rule */
/*                    xgk(1), xgk(3), ...  abscissae which are optimally */
/*                    added to the 25-point gauss rule */

/*           wgk    - weights of the 51-point kronrod rule */

/*           wg     - weights of the 25-point gauss rule */



/*           list of major variables */
/*           ----------------------- */

/*           centr  - mid point of the interval */
/*           hlgth  - half-length of the interval */
/*           absc   - abscissa */
/*           fval*  - function value */
/*           resg   - result of the 25-point gauss formula */
/*           resk   - result of the 51-point kronrod formula */
/*           reskh  - approximation to the mean value of f over (a,b), */
/*                    i.e. to i/(b-a) */

/*           machine dependent constants */
/*           --------------------------- */

/*           epmach is the largest relative spacing. */
/*           uflow is the smallest positive magnitude. */

/* ***first executable statement  qk51 */
    epmach = r1mach_(&c__4);
    uflow = r1mach_(&c__1);

    centr = (*a + *b) * (float).5;
    hlgth = (*b - *a) * (float).5;
    dhlgth = dabs(hlgth);

/*           compute the 51-point kronrod approximation to */
/*           the integral, and estimate the absolute error. */

    fc = (*f)(&centr);
    resg = wg[12] * fc;
    resk = wgk[25] * fc;
    *resabs = dabs(resk);
    for (j = 1; j <= 12; ++j) {
	jtw = j << 1;
	absc = hlgth * xgk[jtw - 1];
	r__1 = centr - absc;
	fval1 = (*f)(&r__1);
	r__1 = centr + absc;
	fval2 = (*f)(&r__1);
	fv1[jtw - 1] = fval1;
	fv2[jtw - 1] = fval2;
	fsum = fval1 + fval2;
	resg += wg[j - 1] * fsum;
	resk += wgk[jtw - 1] * fsum;
	*resabs += wgk[jtw - 1] * (dabs(fval1) + dabs(fval2));
/* L10: */
    }
    for (j = 1; j <= 13; ++j) {
	jtwm1 = (j << 1) - 1;
	absc = hlgth * xgk[jtwm1 - 1];
	r__1 = centr - absc;
	fval1 = (*f)(&r__1);
	r__1 = centr + absc;
	fval2 = (*f)(&r__1);
	fv1[jtwm1 - 1] = fval1;
	fv2[jtwm1 - 1] = fval2;
	fsum = fval1 + fval2;
	resk += wgk[jtwm1 - 1] * fsum;
	*resabs += wgk[jtwm1 - 1] * (dabs(fval1) + dabs(fval2));
/* L15: */
    }
    reskh = resk * (float).5;
    *resasc = wgk[25] * (r__1 = fc - reskh, dabs(r__1));
    for (j = 1; j <= 25; ++j) {
	*resasc += wgk[j - 1] * ((r__1 = fv1[j - 1] - reskh, dabs(r__1)) + (
		r__2 = fv2[j - 1] - reskh, dabs(r__2)));
/* L20: */
    }
    *result = resk * hlgth;
    *resabs *= dhlgth;
    *resasc *= dhlgth;
    *abserr = (r__1 = (resk - resg) * hlgth, dabs(r__1));
    if (*resasc != (float)0. && *abserr != (float)0.) {
/* Computing MIN */
	d__1 = (doublereal) (*abserr * (float)200. / *resasc);
	r__1 = (float)1., r__2 = pow_dd(&d__1, &c_b7);
	*abserr = *resasc * dmin(r__1,r__2);
    }
    if (*resabs > uflow / (epmach * (float)50.)) {
/* Computing MAX */
	r__1 = epmach * (float)50. * *resabs;
	*abserr = dmax(r__1,*abserr);
    }
    return 0;
} /* qk51_ */

#ifdef __cplusplus
	}
#endif
