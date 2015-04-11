/* qk15w.f -- translated by f2c (version 20000118).
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

/* Subroutine */ int qk15w_(E_fp f, E_fp w, real *p1, real *p2, real *p3, 
	real *p4, integer *kp, real *a, real *b, real *result, real *abserr, 
	real *resabs, real *resasc)
{
    /* Initialized data */

    static real xgk[8] = { (float).9914553711208126,(float).9491079123427585,(
	    float).8648644233597691,(float).7415311855993944,(float)
	    .5860872354676911,(float).4058451513773972,(float)
	    .2077849550078985,(float)0. };
    static real wgk[8] = { (float).02293532201052922,(float)
	    .06309209262997855,(float).1047900103222502,(float)
	    .1406532597155259,(float).1690047266392679,(float)
	    .1903505780647854,(float).2044329400752989,(float)
	    .2094821410847278 };
    static real wg[4] = { (float).1294849661688697,(float).2797053914892767,(
	    float).3818300505051889,(float).4179591836734694 };

    /* System generated locals */
    real r__1, r__2;
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    real absc, resg, resk, fsum, absc1, absc2, fval1, fval2;
    integer jtwm1, j;
    real hlgth, centr, reskh, uflow;
    extern doublereal r1mach_(integer *);
    real fc, epmach, dhlgth, fv1[7], fv2[7];
    integer jtw;

/* ***begin prologue  qk15w */
/* ***date written   810101   (yymmdd) */
/* ***revision date  830518   (mmddyy) */
/* ***category no.  h2a2a2 */
/* ***keywords  15-point gauss-kronrod rules */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  to compute i = integral of f*w over (a,b), with error */
/*                           estimate */
/*                       j = integral of abs(f*w) over (a,b) */
/* ***description */

/*           integration rules */
/*           standard fortran subroutine */
/*           real version */

/*           parameters */
/*             on entry */
/*              f      - real */
/*                       function subprogram defining the integrand */
/*                       function f(x). the actual name for f needs to be */
/*                       declared e x t e r n a l in the driver program. */

/*              w      - real */
/*                       function subprogram defining the integrand */
/*                       weight function w(x). the actual name for w */
/*                       needs to be declared e x t e r n a l in the */
/*                       calling program. */

/*              p1, p2, p3, p4 - real */
/*                       parameters in the weight function */

/*              kp     - integer */
/*                       key for indicating the type of weight function */

/*              a      - real */
/*                       lower limit of integration */

/*              b      - real */
/*                       upper limit of integration */

/*            on return */
/*              result - real */
/*                       approximation to the integral i */
/*                       result is computed by applying the 15-point */
/*                       kronrod rule (resk) obtained by optimal addition */
/*                       of abscissae to the 7-point gauss rule (resg). */

/*              abserr - real */
/*                       estimate of the modulus of the absolute error, */
/*                       which should equal or exceed abs(i-result) */

/*              resabs - real */
/*                       approximation to the integral of abs(f) */

/*              resasc - real */
/*                       approximation to the integral of abs(f-i/(b-a)) */

/* ***references  (none) */
/* ***routines called  r1mach */
/* ***end prologue  qk15w */



/*           the abscissae and weights are given for the interval (-1,1). */
/*           because of symmetry only the positive abscissae and their */
/*           corresponding weights are given. */

/*           xgk    - abscissae of the 15-point gauss-kronrod rule */
/*                    xgk(2), xgk(4), ... abscissae of the 7-point */
/*                    gauss rule */
/*                    xgk(1), xgk(3), ... abscissae which are optimally */
/*                    added to the 7-point gauss rule */

/*           wgk    - weights of the 15-point gauss-kronrod rule */

/*           wg     - weights of the 7-point gauss rule */





/*           list of major variables */
/*           ----------------------- */

/*           centr  - mid point of the interval */
/*           hlgth  - half-length of the interval */
/*           absc*  - abscissa */
/*           fval*  - function value */
/*           resg   - result of the 7-point gauss formula */
/*           resk   - result of the 15-point kronrod formula */
/*           reskh  - approximation to the mean value of f*w over (a,b), */
/*                    i.e. to i/(b-a) */

/*           machine dependent constants */
/*           --------------------------- */

/*           epmach is the largest relative spacing. */
/*           uflow is the smallest positive magnitude. */

/* ***first executable statement  qk15w */
    epmach = r1mach_(&c__4);
    uflow = r1mach_(&c__1);

    centr = (*a + *b) * (float).5;
    hlgth = (*b - *a) * (float).5;
    dhlgth = dabs(hlgth);

/*           compute the 15-point kronrod approximation to the */
/*           integral, and estimate the error. */

    fc = (*f)(&centr) * (*w)(&centr, p1, p2, p3, p4, kp);
    resg = wg[3] * fc;
    resk = wgk[7] * fc;
    *resabs = dabs(resk);
    for (j = 1; j <= 3; ++j) {
	jtw = j << 1;
	absc = hlgth * xgk[jtw - 1];
	absc1 = centr - absc;
	absc2 = centr + absc;
	fval1 = (*f)(&absc1) * (*w)(&absc1, p1, p2, p3, p4, kp);
	fval2 = (*f)(&absc2) * (*w)(&absc2, p1, p2, p3, p4, kp);
	fv1[jtw - 1] = fval1;
	fv2[jtw - 1] = fval2;
	fsum = fval1 + fval2;
	resg += wg[j - 1] * fsum;
	resk += wgk[jtw - 1] * fsum;
	*resabs += wgk[jtw - 1] * (dabs(fval1) + dabs(fval2));
/* L10: */
    }
    for (j = 1; j <= 4; ++j) {
	jtwm1 = (j << 1) - 1;
	absc = hlgth * xgk[jtwm1 - 1];
	absc1 = centr - absc;
	absc2 = centr + absc;
	fval1 = (*f)(&absc1) * (*w)(&absc1, p1, p2, p3, p4, kp);
	fval2 = (*f)(&absc2) * (*w)(&absc2, p1, p2, p3, p4, kp);
	fv1[jtwm1 - 1] = fval1;
	fv2[jtwm1 - 1] = fval2;
	fsum = fval1 + fval2;
	resk += wgk[jtwm1 - 1] * fsum;
	*resabs += wgk[jtwm1 - 1] * (dabs(fval1) + dabs(fval2));
/* L15: */
    }
    reskh = resk * (float).5;
    *resasc = wgk[7] * (r__1 = fc - reskh, dabs(r__1));
    for (j = 1; j <= 7; ++j) {
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
} /* qk15w_ */

#ifdef __cplusplus
	}
#endif
