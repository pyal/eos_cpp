/* qc25c.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Subroutine */ int qc25c_(E_fp f, real *a, real *b, real *c__, real *result,
	 real *abserr, integer *krul, integer *neval)
{
    /* Initialized data */

    static real x[11] = { (float).9914448613738104,(float).9659258262890683,(
	    float).9238795325112868,(float).8660254037844386,(float)
	    .7933533402912352,(float).7071067811865475,(float)
	    .6087614290087206,(float).5,(float).3826834323650898,(float)
	    .2588190451025208,(float).1305261922200516 };

    /* System generated locals */
    real r__1;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    real fval[25], res12, res24;
    extern /* Subroutine */ int qk15w_(E_fp, E_fp, real *, real *, real *, 
	    real *, integer *, real *, real *, real *, real *, real *, real *)
	    ;
    integer isym;
    real amom0, amom1, amom2, cheb12[13], cheb24[25];
    integer i__, k;
    extern /* Subroutine */ int qcheb_(real *, real *, real *, real *);
    real u, hlgth, centr;
    extern doublereal qwgtc_(real *x, real *c__, real *p2, real *p3, real *p4, 
							 integer *kp);
    real p2, p3, p4, cc;
    integer kp;
    real resabs, resasc, ak22;

/* ***begin prologue  qc25c */
/* ***date written   810101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a2a2,j4 */
/* ***keywords  25-point clenshaw-curtis integration */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  to compute i = integral of f*w over (a,b) with */
/*            error estimate, where w(x) = 1/(x-c) */
/* ***description */

/*        integration rules for the computation of cauchy */
/*        principal value integrals */
/*        standard fortran subroutine */
/*        real version */

/*        parameters */
/*           f      - real */
/*                    function subprogram defining the integrand function */
/*                    f(x). the actual name for f needs to be declared */
/*                    e x t e r n a l  in the driver program. */

/*           a      - real */
/*                    left end point of the integration interval */

/*           b      - real */
/*                    right end point of the integration interval, b.gt.a */

/*           c      - real */
/*                    parameter in the weight function */

/*           result - real */
/*                    approximation to the integral */
/*                    result is computed by using a generalized */
/*                    clenshaw-curtis method if c lies within ten percent */
/*                    of the integration interval. in the other case the */
/*                    15-point kronrod rule obtained by optimal addition */
/*                    of abscissae to the 7-point gauss rule, is applied. */

/*           abserr - real */
/*                    estimate of the modulus of the absolute error, */
/*                    which should equal or exceed abs(i-result) */

/*           krul   - integer */
/*                    key which is decreased by 1 if the 15-point */
/*                    gauss-kronrod scheme has been used */

/*           neval  - integer */
/*                    number of integrand evaluations */

/* ***references  (none) */
/* ***routines called  qcheb,qk15w,qwgtc */
/* ***end prologue  qc25c */




/*           the vector x contains the values cos(k*pi/24), */
/*           k = 1, ..., 11, to be used for the chebyshev series */
/*           expansion of f */


/*           list of major variables */
/*           ---------------------- */
/*           fval   - value of the function f at the points */
/*                    cos(k*pi/24),  k = 0, ..., 24 */
/*           cheb12 - chebyshev series expansion coefficients, */
/*                    for the function f, of degree 12 */
/*           cheb24 - chebyshev series expansion coefficients, */
/*                    for the function f, of degree 24 */
/*           res12  - approximation to the integral corresponding */
/*                    to the use of cheb12 */
/*           res24  - approximation to the integral corresponding */
/*                    to the use of cheb24 */
/*           qwgtc - external function subprogram defining */
/*                    the weight function */
/*           hlgth  - half-length of the interval */
/*           centr  - mid point of the interval */


/*           check the position of c. */

/* ***first executable statement  qc25c */
    cc = ((float)2. * *c__ - *b - *a) / (*b - *a);
    if (dabs(cc) < (float)1.1) {
	goto L10;
    }

/*           apply the 15-point gauss-kronrod scheme. */

    --(*krul);
    qk15w_((E_fp)f, (E_fp)qwgtc_, c__, &p2, &p3, &p4, &kp, a, b, result, 
	    abserr, &resabs, &resasc);
    *neval = 15;
    if (resasc == *abserr) {
	++(*krul);
    }
    goto L50;

/*           use the generalized clenshaw-curtis method. */

L10:
    hlgth = (*b - *a) * (float).5;
    centr = (*b + *a) * (float).5;
    *neval = 25;
    r__1 = hlgth + centr;
    fval[0] = (*f)(&r__1) * (float).5;
    fval[12] = (*f)(&centr);
    r__1 = centr - hlgth;
    fval[24] = (*f)(&r__1) * (float).5;
    for (i__ = 2; i__ <= 12; ++i__) {
	u = hlgth * x[i__ - 2];
	isym = 26 - i__;
	r__1 = u + centr;
	fval[i__ - 1] = (*f)(&r__1);
	r__1 = centr - u;
	fval[isym - 1] = (*f)(&r__1);
/* L20: */
    }

/*           compute the chebyshev series expansion. */

    qcheb_(x, fval, cheb12, cheb24);

/*           the modified chebyshev moments are computed */
/*           by forward recursion, using amom0 and amom1 */
/*           as starting values. */

    amom0 = log((r__1 = ((float)1. - cc) / (cc + (float)1.), dabs(r__1)));
    amom1 = cc * amom0 + (float)2.;
    res12 = cheb12[0] * amom0 + cheb12[1] * amom1;
    res24 = cheb24[0] * amom0 + cheb24[1] * amom1;
    for (k = 3; k <= 13; ++k) {
	amom2 = cc * (float)2. * amom1 - amom0;
	ak22 = (real) ((k - 2) * (k - 2));
	if (k / 2 << 1 == k) {
	    amom2 -= (float)4. / (ak22 - (float)1.);
	}
	res12 += cheb12[k - 1] * amom2;
	res24 += cheb24[k - 1] * amom2;
	amom0 = amom1;
	amom1 = amom2;
/* L30: */
    }
    for (k = 14; k <= 25; ++k) {
	amom2 = cc * (float)2. * amom1 - amom0;
	ak22 = (real) ((k - 2) * (k - 2));
	if (k / 2 << 1 == k) {
	    amom2 -= (float)4. / (ak22 - (float)1.);
	}
	res24 += cheb24[k - 1] * amom2;
	amom0 = amom1;
	amom1 = amom2;
/* L40: */
    }
    *result = res24;
    *abserr = (r__1 = res24 - res12, dabs(r__1));
L50:
    return 0;
} /* qc25c_ */

#ifdef __cplusplus
	}
#endif
