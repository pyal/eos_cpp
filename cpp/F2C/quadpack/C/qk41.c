/* qk41.f -- translated by f2c (version 20000118).
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

/* Subroutine */ int qk41_(E_fp f, real *a, real *b, real *result, real *
	abserr, real *resabs, real *resasc)
{
    /* Initialized data */

    static real xgk[21] = { (float).9988590315882777,(float).9931285991850949,
	    (float).9815078774502503,(float).9639719272779138,(float)
	    .9408226338317548,(float).9122344282513259,(float)
	    .878276811252282,(float).8391169718222188,(float)
	    .7950414288375512,(float).7463319064601508,(float)
	    .6932376563347514,(float).636053680726515,(float)
	    .5751404468197103,(float).5108670019508271,(float)
	    .4435931752387251,(float).3737060887154196,(float)
	    .301627868114913,(float).2277858511416451,(float)
	    .1526054652409227,(float).07652652113349733,(float)0. };
    static real wgk[21] = { (float).003073583718520532,(float)
	    .008600269855642942,(float).01462616925697125,(float)
	    .02038837346126652,(float).02588213360495116,(float)
	    .0312873067770328,(float).0366001697582008,(float)
	    .04166887332797369,(float).04643482186749767,(float)
	    .05094457392372869,(float).05519510534828599,(float)
	    .05911140088063957,(float).06265323755478117,(float)
	    .06583459713361842,(float).06864867292852162,(float)
	    .07105442355344407,(float).07303069033278667,(float)
	    .07458287540049919,(float).07570449768455667,(float)
	    .07637786767208074,(float).07660071191799966 };
    static real wg[10] = { (float).01761400713915212,(float)
	    .04060142980038694,(float).06267204833410906,(float)
	    .08327674157670475,(float).1019301198172404,(float)
	    .1181945319615184,(float).1316886384491766,(float)
	    .1420961093183821,(float).1491729864726037,(float)
	    .1527533871307259 };

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
    real fc, epmach, dhlgth, fv1[20], fv2[20];
    integer jtw;

/* ***begin prologue  qk41 */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a1a2 */
/* ***keywords  41-point gauss-kronrod rules */
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
/*                       declared e x t e r n a l in the calling program. */

/*              a      - real */
/*                       lower limit of integration */

/*              b      - real */
/*                       upper limit of integration */

/*            on return */
/*              result - real */
/*                       approximation to the integral i */
/*                       result is computed by applying the 41-point */
/*                       gauss-kronrod rule (resk) obtained by optimal */
/*                       addition of abscissae to the 20-point gauss */
/*                       rule (resg). */

/*              abserr - real */
/*                       estimate of the modulus of the absolute error, */
/*                       which should not exceed abs(i-result) */

/*              resabs - real */
/*                       approximation to the integral j */

/*              resasc - real */
/*                       approximation to the integal of abs(f-i/(b-a)) */
/*                       over (a,b) */

/* ***references  (none) */
/* ***routines called  r1mach */
/* ***end prologue  qk41 */



/*           the abscissae and weights are given for the interval (-1,1). */
/*           because of symmetry only the positive abscissae and their */
/*           corresponding weights are given. */

/*           xgk    - abscissae of the 41-point gauss-kronrod rule */
/*                    xgk(2), xgk(4), ...  abscissae of the 20-point */
/*                    gauss rule */
/*                    xgk(1), xgk(3), ...  abscissae which are optimally */
/*                    added to the 20-point gauss rule */

/*           wgk    - weights of the 41-point gauss-kronrod rule */

/*           wg     - weights of the 20-point gauss rule */



/*           list of major variables */
/*           ----------------------- */

/*           centr  - mid point of the interval */
/*           hlgth  - half-length of the interval */
/*           absc   - abscissa */
/*           fval*  - function value */
/*           resg   - result of the 20-point gauss formula */
/*           resk   - result of the 41-point kronrod formula */
/*           reskh  - approximation to mean value of f over (a,b), i.e. */
/*                    to i/(b-a) */

/*           machine dependent constants */
/*           --------------------------- */

/*           epmach is the largest relative spacing. */
/*           uflow is the smallest positive magnitude. */

/* ***first executable statement  qk41 */
    epmach = r1mach_(&c__4);
    uflow = r1mach_(&c__1);

    centr = (*a + *b) * (float).5;
    hlgth = (*b - *a) * (float).5;
    dhlgth = dabs(hlgth);

/*           compute the 41-point gauss-kronrod approximation to */
/*           the integral, and estimate the absolute error. */

    resg = (float)0.;
    fc = (*f)(&centr);
    resk = wgk[20] * fc;
    *resabs = dabs(resk);
    for (j = 1; j <= 10; ++j) {
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
    for (j = 1; j <= 10; ++j) {
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
    *resasc = wgk[20] * (r__1 = fc - reskh, dabs(r__1));
    for (j = 1; j <= 20; ++j) {
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
} /* qk41_ */

#ifdef __cplusplus
	}
#endif
