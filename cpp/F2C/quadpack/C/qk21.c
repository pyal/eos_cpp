/* qk21.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Table of constant values */

static integer c__4 = 4;
static integer c__1 = 1;
static doublereal c_b7 = 1.5;

/* Subroutine */ int qk21_(E_fp f, real *a, real *b, real *result, real *
	abserr, real *resabs, real *resasc)
{
    /* Initialized data */

    static real xgk[11] = { (float).9956571630258081,(float).9739065285171717,
	    (float).9301574913557082,(float).8650633666889845,(float)
	    .7808177265864169,(float).6794095682990244,(float)
	    .5627571346686047,(float).4333953941292472,(float)
	    .2943928627014602,(float).1488743389816312,(float)0. };
    static real wgk[11] = { (float).01169463886737187,(float)
	    .03255816230796473,(float).054755896574352,(float)
	    .07503967481091995,(float).09312545458369761,(float)
	    .1093871588022976,(float).1234919762620659,(float)
	    .1347092173114733,(float).1427759385770601,(float)
	    .1477391049013385,(float).1494455540029169 };
    static real wg[5] = { (float).06667134430868814,(float).1494513491505806,(
	    float).219086362515982,(float).2692667193099964,(float)
	    .2955242247147529 };

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
    real fc, epmach, dhlgth, fv1[10], fv2[10];
    integer jtw;

/* ***begin prologue  qk21 */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a1a2 */
/* ***keywords  21-point gauss-kronrod rules */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  to compute i = integral of f over (a,b), with error */
/*                           estimate */
/*                       j = integral of abs(f) over (a,b) */
/* ***description */

/*           integration rules */
/*           standard fortran subroutine */
/*           real version */

/*           parameters */
/*            on entry */
/*              f      - real */
/*                       function subprogram defining the integrand */
/*                       function f(x). the actual name for f needs to be */
/*                       declared e x t e r n a l in the driver program. */

/*              a      - real */
/*                       lower limit of integration */

/*              b      - real */
/*                       upper limit of integration */

/*            on return */
/*              result - real */
/*                       approximation to the integral i */
/*                       result is computed by applying the 21-point */
/*                       kronrod rule (resk) obtained by optimal addition */
/*                       of abscissae to the 10-point gauss rule (resg). */

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
/* ***end prologue  qk21 */



/*           the abscissae and weights are given for the interval (-1,1). */
/*           because of symmetry only the positive abscissae and their */
/*           corresponding weights are given. */

/*           xgk    - abscissae of the 21-point kronrod rule */
/*                    xgk(2), xgk(4), ...  abscissae of the 10-point */
/*                    gauss rule */
/*                    xgk(1), xgk(3), ...  abscissae which are optimally */
/*                    added to the 10-point gauss rule */

/*           wgk    - weights of the 21-point kronrod rule */

/*           wg     - weights of the 10-point gauss rule */





/*           list of major variables */
/*           ----------------------- */

/*           centr  - mid point of the interval */
/*           hlgth  - half-length of the interval */
/*           absc   - abscissa */
/*           fval*  - function value */
/*           resg   - result of the 10-point gauss formula */
/*           resk   - result of the 21-point kronrod formula */
/*           reskh  - approximation to the mean value of f over (a,b), */
/*                    i.e. to i/(b-a) */


/*           machine dependent constants */
/*           --------------------------- */

/*           epmach is the largest relative spacing. */
/*           uflow is the smallest positive magnitude. */

/* ***first executable statement  qk21 */
    epmach = r1mach_(&c__4);
    uflow = r1mach_(&c__1);

    centr = (*a + *b) * (float).5;
    hlgth = (*b - *a) * (float).5;
    dhlgth = dabs(hlgth);

/*           compute the 21-point kronrod approximation to */
/*           the integral, and estimate the absolute error. */

    resg = (float)0.;
    fc = (*f)(&centr);
    resk = wgk[10] * fc;
    *resabs = dabs(resk);
    for (j = 1; j <= 5; ++j) {
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
    for (j = 1; j <= 5; ++j) {
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
    *resasc = wgk[10] * (r__1 = fc - reskh, dabs(r__1));
    for (j = 1; j <= 10; ++j) {
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
} /* qk21_ */

#ifdef __cplusplus
	}
#endif
