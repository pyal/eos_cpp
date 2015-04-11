/* qk61.f -- translated by f2c (version 20000118).
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

/* Subroutine */ int qk61_(E_fp f, real *a, real *b, real *result, real *
	abserr, real *resabs, real *resasc)
{
    /* Initialized data */

    static real xgk[31] = { (float).9994844100504906,(float).9968934840746495,
	    (float).9916309968704046,(float).9836681232797472,(float)
	    .9731163225011263,(float).9600218649683075,(float).94437444474856,
	    (float).9262000474292743,(float).9055733076999078,(float)
	    .8825605357920527,(float).8572052335460611,(float)
	    .8295657623827684,(float).7997278358218391,(float)
	    .7677774321048262,(float).7337900624532268,(float)
	    .6978504947933158,(float).660061064126627,(float)
	    .6205261829892429,(float).5793452358263617,(float)
	    .5366241481420199,(float).4924804678617786,(float)
	    .4470337695380892,(float).4004012548303944,(float)
	    .3527047255308781,(float).3040732022736251,(float)
	    .2546369261678898,(float).2045251166823099,(float)
	    .1538699136085835,(float).102806937966737,(float)
	    .0514718425553177,(float)0. };
    static real wgk[31] = { (float).001389013698677008,(float)
	    .003890461127099884,(float).006630703915931292,(float)
	    .009273279659517763,(float).01182301525349634,(float)
	    .0143697295070458,(float).01692088918905327,(float)
	    .01941414119394238,(float).02182803582160919,(float)
	    .0241911620780806,(float).0265099548823331,(float)
	    .02875404876504129,(float).03090725756238776,(float)
	    .03298144705748373,(float).03497933802806002,(float)
	    .03688236465182123,(float).03867894562472759,(float)
	    .04037453895153596,(float).04196981021516425,(float)
	    .04345253970135607,(float).04481480013316266,(float)
	    .04605923827100699,(float).04718554656929915,(float)
	    .04818586175708713,(float).04905543455502978,(float)
	    .04979568342707421,(float).05040592140278235,(float)
	    .05088179589874961,(float).05122154784925877,(float)
	    .05142612853745903,(float).05149472942945157 };
    static real wg[15] = { (float).007968192496166606,(float)
	    .01846646831109096,(float).02878470788332337,(float)
	    .03879919256962705,(float).04840267283059405,(float)
	    .05749315621761907,(float).0659742298821805,(float)
	    .07375597473770521,(float).08075589522942022,(float)
	    .08689978720108298,(float).09212252223778613,(float)
	    .09636873717464426,(float).09959342058679527,(float)
	    .1017623897484055,(float).1028526528935588 };

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
    real fc, epmach, dhlgth, fv1[30], fv2[30];
    integer jtw;

/* ***begin prologue  qk61 */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a1a2 */
/* ***keywords  61-point gauss-kronrod rules */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  to compute i = integral of f over (a,b) with error */
/*                           estimate */
/*                       j = integral of dabs(f) over (a,b) */
/* ***description */

/*        integration rule */
/*        standard fortran subroutine */
/*        real version */


/*        parameters */
/*         on entry */
/*           f      - real */
/*                    function subprogram defining the integrand */
/*                    function f(x). the actual name for f needs to be */
/*                    declared e x t e r n a l in the calling program. */

/*           a      - real */
/*                    lower limit of integration */

/*           b      - real */
/*                    upper limit of integration */

/*         on return */
/*           result - real */
/*                    approximation to the integral i */
/*                    result is computed by applying the 61-point */
/*                    kronrod rule (resk) obtained by optimal addition of */
/*                    abscissae to the 30-point gauss rule (resg). */

/*           abserr - real */
/*                    estimate of the modulus of the absolute error, */
/*                    which should equal or exceed dabs(i-result) */

/*           resabs - real */
/*                    approximation to the integral j */

/*           resasc - real */
/*                    approximation to the integral of dabs(f-i/(b-a)) */


/* ***references  (none) */
/* ***routines called  r1mach */
/* ***end prologue  qk61 */



/*           the abscissae and weights are given for the */
/*           interval (-1,1). because of symmetry only the positive */
/*           abscissae and their corresponding weights are given. */

/*           xgk   - abscissae of the 61-point kronrod rule */
/*                   xgk(2), xgk(4)  ... abscissae of the 30-point */
/*                   gauss rule */
/*                   xgk(1), xgk(3)  ... optimally added abscissae */
/*                   to the 30-point gauss rule */

/*           wgk   - weights of the 61-point kronrod rule */

/*           wg    - weigths of the 30-point gauss rule */


/*           list of major variables */
/*           ----------------------- */

/*           centr  - mid point of the interval */
/*           hlgth  - half-length of the interval */
/*           absc   - abscissa */
/*           fval*  - function value */
/*           resg   - result of the 30-point gauss rule */
/*           resk   - result of the 61-point kronrod rule */
/*           reskh  - approximation to the mean value of f */
/*                    over (a,b), i.e. to i/(b-a) */

/*           machine dependent constants */
/*           --------------------------- */

/*           epmach is the largest relative spacing. */
/*           uflow is the smallest positive magnitude. */

/* ***first executable statement  qk61 */
    epmach = r1mach_(&c__4);
    uflow = r1mach_(&c__1);

    centr = (*b + *a) * (float).5;
    hlgth = (*b - *a) * (float).5;
    dhlgth = dabs(hlgth);

/*           compute the 61-point kronrod approximation to the */
/*           integral, and estimate the absolute error. */

    resg = (float)0.;
    fc = (*f)(&centr);
    resk = wgk[30] * fc;
    *resabs = dabs(resk);
    for (j = 1; j <= 15; ++j) {
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
    for (j = 1; j <= 15; ++j) {
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
    *resasc = wgk[30] * (r__1 = fc - reskh, dabs(r__1));
    for (j = 1; j <= 30; ++j) {
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
} /* qk61_ */

#ifdef __cplusplus
	}
#endif
