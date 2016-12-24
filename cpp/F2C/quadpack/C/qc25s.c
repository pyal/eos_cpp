/* qc25s.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Subroutine */ int qc25s_(E_fp f, real *a, real *b, real *bl, real *br, 
	real *alfa, real *beta, real *ri, real *rj, real *rg, real *rh, real *
	result, real *abserr, real *resasc, integer *integr, integer *nev)
{
    /* Initialized data */

    static real x[11] = { (float).9914448613738104,(float).9659258262890683,(
	    float).9238795325112868,(float).8660254037844386,(float)
	    .7933533402912352,(float).7071067811865475,(float)
	    .6087614290087206,(float).5,(float).3826834323650898,(float)
	    .2588190451025208,(float).1305261922200516 };

    /* System generated locals */
    real r__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), log(doublereal);

    /* Local variables */
    real fval[25], res12, res24;
    extern /* Subroutine */ int qk15w_(E_fp, E_fp, real *, real *, real *, 
	    real *, integer *, real *, real *, real *, real *, real *, real *)
	    ;
    integer isym;
    real cheb12[13], cheb24[25];
    integer i__;
    extern /* Subroutine */ int qcheb_(real *, real *, real *, real *);
    real u, hlgth, centr;
    extern doublereal qwgts_(real *x, real *a, real *b, real *alfa, real *beta, 
							 integer *integr);
    real dc, factor, resabs, fix;

/* ***begin prologue  qc25s */
/* ***date written   810101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a2a2 */
/* ***keywords  25-point clenshaw-curtis integration */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  to compute i = integral of f*w over (bl,br), with error */
/*            estimate, where the weight function w has a singular */
/*            behaviour of algebraico-logarithmic type at the points */
/*            a and/or b. (bl,br) is a part of (a,b). */
/* ***description */

/*        integration rules for integrands having algebraico-logarithmic */
/*        end point singularities */
/*        standard fortran subroutine */
/*        real version */

/*        parameters */
/*           f      - real */
/*                    function subprogram defining the integrand */
/*                    f(x). the actual name for f needs to be declared */
/*                    e x t e r n a l  in the driver program. */

/*           a      - real */
/*                    left end point of the original interval */

/*           b      - real */
/*                    right end point of the original interval, b.gt.a */

/*           bl     - real */
/*                    lower limit of integration, bl.ge.a */

/*           br     - real */
/*                    upper limit of integration, br.le.b */

/*           alfa   - real */
/*                    parameter in the weight function */

/*           beta   - real */
/*                    parameter in the weight function */

/*           ri,rj,rg,rh - real */
/*                    modified chebyshev moments for the application */
/*                    of the generalized clenshaw-curtis */
/*                    method (computed in subroutine dqmomo) */

/*           result - real */
/*                    approximation to the integral */
/*                    result is computed by using a generalized */
/*                    clenshaw-curtis method if b1 = a or br = b. */
/*                    in all other cases the 15-point kronrod */
/*                    rule is applied, obtained by optimal addition of */
/*                    abscissae to the 7-point gauss rule. */

/*           abserr - real */
/*                    estimate of the modulus of the absolute error, */
/*                    which should equal or exceed abs(i-result) */

/*           resasc - real */
/*                    approximation to the integral of abs(f*w-i/(b-a)) */

/*           integr - integer */
/*                    which determines the weight function */
/*                    = 1   w(x) = (x-a)**alfa*(b-x)**beta */
/*                    = 2   w(x) = (x-a)**alfa*(b-x)**beta*log(x-a) */
/*                    = 3   w(x) = (x-a)**alfa*(b-x)**beta*log(b-x) */
/*                    = 4   w(x) = (x-a)**alfa*(b-x)**beta*log(x-a)* */
/*                                 log(b-x) */

/*           nev    - integer */
/*                    number of integrand evaluations */

/* ***references  (none) */
/* ***routines called  qcheb,qk15w */
/* ***end prologue  qc25s */




/*           the vector x contains the values cos(k*pi/24) */
/*           k = 1, ..., 11, to be used for the computation of the */
/*           chebyshev series expansion of f. */

    /* Parameter adjustments */
    --rh;
    --rg;
    --rj;
    --ri;

    /* Function Body */

/*           list of major variables */
/*           ----------------------- */

/*           fval   - value of the function f at the points */
/*                    (br-bl)*0.5*cos(k*pi/24)+(br+bl)*0.5 */
/*                    k = 0, ..., 24 */
/*           cheb12 - coefficients of the chebyshev series expansion */
/*                    of degree 12, for the function f, in the */
/*                    interval (bl,br) */
/*           cheb24 - coefficients of the chebyshev series expansion */
/*                    of degree 24, for the function f, in the */
/*                    interval (bl,br) */
/*           res12  - approximation to the integral obtained from cheb12 */
/*           res24  - approximation to the integral obtained from cheb24 */
/*           qwgts - external function subprogram defining */
/*                    the four possible weight functions */
/*           hlgth  - half-length of the interval (bl,br) */
/*           centr  - mid point of the interval (bl,br) */

/* ***first executable statement  qc25s */
    *nev = 25;
    if (*bl == *a && (*alfa != (float)0. || *integr == 2 || *integr == 4)) {
	goto L10;
    }
    if (*br == *b && (*beta != (float)0. || *integr == 3 || *integr == 4)) {
	goto L140;
    }

/*           if a.gt.bl and b.lt.br, apply the 15-point gauss-kronrod */
/*           scheme. */


    qk15w_((E_fp)f, (E_fp)qwgts_, a, b, alfa, beta, integr, bl, br, result, 
	    abserr, &resabs, resasc);
    *nev = 15;
    goto L270;

/*           this part of the program is executed only if a = bl. */
/*           ---------------------------------------------------- */

/*           compute the chebyshev series expansion of the */
/*           following function */
/*           f1 = (0.5*(b+b-br-a)-0.5*(br-a)*x)**beta */
/*                  *f(0.5*(br-a)*x+0.5*(br+a)) */

L10:
    hlgth = (*br - *bl) * (float).5;
    centr = (*br + *bl) * (float).5;
    fix = *b - centr;
    r__1 = hlgth + centr;
    d__1 = (doublereal) (fix - hlgth);
    d__2 = (doublereal) (*beta);
    fval[0] = (*f)(&r__1) * (float).5 * pow_dd(&d__1, &d__2);
    d__1 = (doublereal) fix;
    d__2 = (doublereal) (*beta);
    fval[12] = (*f)(&centr) * pow_dd(&d__1, &d__2);
    r__1 = centr - hlgth;
    d__1 = (doublereal) (fix + hlgth);
    d__2 = (doublereal) (*beta);
    fval[24] = (*f)(&r__1) * (float).5 * pow_dd(&d__1, &d__2);
    for (i__ = 2; i__ <= 12; ++i__) {
	u = hlgth * x[i__ - 2];
	isym = 26 - i__;
	r__1 = u + centr;
	d__1 = (doublereal) (fix - u);
	d__2 = (doublereal) (*beta);
	fval[i__ - 1] = (*f)(&r__1) * pow_dd(&d__1, &d__2);
	r__1 = centr - u;
	d__1 = (doublereal) (fix + u);
	d__2 = (doublereal) (*beta);
	fval[isym - 1] = (*f)(&r__1) * pow_dd(&d__1, &d__2);
/* L20: */
    }
    d__1 = (doublereal) hlgth;
    d__2 = (doublereal) (*alfa + (float)1.);
    factor = pow_dd(&d__1, &d__2);
    *result = (float)0.;
    *abserr = (float)0.;
    res12 = (float)0.;
    res24 = (float)0.;
    if (*integr > 2) {
	goto L70;
    }
    qcheb_(x, fval, cheb12, cheb24);

/*           integr = 1  (or 2) */

    for (i__ = 1; i__ <= 13; ++i__) {
	res12 += cheb12[i__ - 1] * ri[i__];
	res24 += cheb24[i__ - 1] * ri[i__];
/* L30: */
    }
    for (i__ = 14; i__ <= 25; ++i__) {
	res24 += cheb24[i__ - 1] * ri[i__];
/* L40: */
    }
    if (*integr == 1) {
	goto L130;
    }

/*           integr = 2 */

    dc = log(*br - *bl);
    *result = res24 * dc;
    *abserr = (r__1 = (res24 - res12) * dc, dabs(r__1));
    res12 = (float)0.;
    res24 = (float)0.;
    for (i__ = 1; i__ <= 13; ++i__) {
	res12 += cheb12[i__ - 1] * rg[i__];
	res24 = res12 + cheb24[i__ - 1] * rg[i__];
/* L50: */
    }
    for (i__ = 14; i__ <= 25; ++i__) {
	res24 += cheb24[i__ - 1] * rg[i__];
/* L60: */
    }
    goto L130;

/*           compute the chebyshev series expansion of the */
/*           following function */
/*           f4 = f1*log(0.5*(b+b-br-a)-0.5*(br-a)*x) */

L70:
    fval[0] *= log(fix - hlgth);
    fval[12] *= log(fix);
    fval[24] *= log(fix + hlgth);
    for (i__ = 2; i__ <= 12; ++i__) {
	u = hlgth * x[i__ - 2];
	isym = 26 - i__;
	fval[i__ - 1] *= log(fix - u);
	fval[isym - 1] *= log(fix + u);
/* L80: */
    }
    qcheb_(x, fval, cheb12, cheb24);

/*           integr = 3  (or 4) */

    for (i__ = 1; i__ <= 13; ++i__) {
	res12 += cheb12[i__ - 1] * ri[i__];
	res24 += cheb24[i__ - 1] * ri[i__];
/* L90: */
    }
    for (i__ = 14; i__ <= 25; ++i__) {
	res24 += cheb24[i__ - 1] * ri[i__];
/* L100: */
    }
    if (*integr == 3) {
	goto L130;
    }

/*           integr = 4 */

    dc = log(*br - *bl);
    *result = res24 * dc;
    *abserr = (r__1 = (res24 - res12) * dc, dabs(r__1));
    res12 = (float)0.;
    res24 = (float)0.;
    for (i__ = 1; i__ <= 13; ++i__) {
	res12 += cheb12[i__ - 1] * rg[i__];
	res24 += cheb24[i__ - 1] * rg[i__];
/* L110: */
    }
    for (i__ = 14; i__ <= 25; ++i__) {
	res24 += cheb24[i__ - 1] * rg[i__];
/* L120: */
    }
L130:
    *result = (*result + res24) * factor;
    *abserr = (*abserr + (r__1 = res24 - res12, dabs(r__1))) * factor;
    goto L270;

/*           this part of the program is executed only if b = br. */
/*           ---------------------------------------------------- */

/*           compute the chebyshev series expansion of the */
/*           following function */
/*           f2 = (0.5*(b+bl-a-a)+0.5*(b-bl)*x)**alfa */
/*                *f(0.5*(b-bl)*x+0.5*(b+bl)) */

L140:
    hlgth = (*br - *bl) * (float).5;
    centr = (*br + *bl) * (float).5;
    fix = centr - *a;
    r__1 = hlgth + centr;
    d__1 = (doublereal) (fix + hlgth);
    d__2 = (doublereal) (*alfa);
    fval[0] = (*f)(&r__1) * (float).5 * pow_dd(&d__1, &d__2);
    d__1 = (doublereal) fix;
    d__2 = (doublereal) (*alfa);
    fval[12] = (*f)(&centr) * pow_dd(&d__1, &d__2);
    r__1 = centr - hlgth;
    d__1 = (doublereal) (fix - hlgth);
    d__2 = (doublereal) (*alfa);
    fval[24] = (*f)(&r__1) * (float).5 * pow_dd(&d__1, &d__2);
    for (i__ = 2; i__ <= 12; ++i__) {
	u = hlgth * x[i__ - 2];
	isym = 26 - i__;
	r__1 = u + centr;
	d__1 = (doublereal) (fix + u);
	d__2 = (doublereal) (*alfa);
	fval[i__ - 1] = (*f)(&r__1) * pow_dd(&d__1, &d__2);
	r__1 = centr - u;
	d__1 = (doublereal) (fix - u);
	d__2 = (doublereal) (*alfa);
	fval[isym - 1] = (*f)(&r__1) * pow_dd(&d__1, &d__2);
/* L150: */
    }
    d__1 = (doublereal) hlgth;
    d__2 = (doublereal) (*beta + (float)1.);
    factor = pow_dd(&d__1, &d__2);
    *result = (float)0.;
    *abserr = (float)0.;
    res12 = (float)0.;
    res24 = (float)0.;
    if (*integr == 2 || *integr == 4) {
	goto L200;
    }

/*           integr = 1  (or 3) */

    qcheb_(x, fval, cheb12, cheb24);
    for (i__ = 1; i__ <= 13; ++i__) {
	res12 += cheb12[i__ - 1] * rj[i__];
	res24 += cheb24[i__ - 1] * rj[i__];
/* L160: */
    }
    for (i__ = 14; i__ <= 25; ++i__) {
	res24 += cheb24[i__ - 1] * rj[i__];
/* L170: */
    }
    if (*integr == 1) {
	goto L260;
    }

/*           integr = 3 */

    dc = log(*br - *bl);
    *result = res24 * dc;
    *abserr = (r__1 = (res24 - res12) * dc, dabs(r__1));
    res12 = (float)0.;
    res24 = (float)0.;
    for (i__ = 1; i__ <= 13; ++i__) {
	res12 += cheb12[i__ - 1] * rh[i__];
	res24 += cheb24[i__ - 1] * rh[i__];
/* L180: */
    }
    for (i__ = 14; i__ <= 25; ++i__) {
	res24 += cheb24[i__ - 1] * rh[i__];
/* L190: */
    }
    goto L260;

/*           compute the chebyshev series expansion of the */
/*           following function */
/*           f3 = f2*log(0.5*(b-bl)*x+0.5*(b+bl-a-a)) */

L200:
    fval[0] *= log(hlgth + fix);
    fval[12] *= log(fix);
    fval[24] *= log(fix - hlgth);
    for (i__ = 2; i__ <= 12; ++i__) {
	u = hlgth * x[i__ - 2];
	isym = 26 - i__;
	fval[i__ - 1] *= log(u + fix);
	fval[isym - 1] *= log(fix - u);
/* L210: */
    }
    qcheb_(x, fval, cheb12, cheb24);

/*           integr = 2  (or 4) */

    for (i__ = 1; i__ <= 13; ++i__) {
	res12 += cheb12[i__ - 1] * rj[i__];
	res24 += cheb24[i__ - 1] * rj[i__];
/* L220: */
    }
    for (i__ = 14; i__ <= 25; ++i__) {
	res24 += cheb24[i__ - 1] * rj[i__];
/* L230: */
    }
    if (*integr == 2) {
	goto L260;
    }
    dc = log(*br - *bl);
    *result = res24 * dc;
    *abserr = (r__1 = (res24 - res12) * dc, dabs(r__1));
    res12 = (float)0.;
    res24 = (float)0.;

/*           integr = 4 */

    for (i__ = 1; i__ <= 13; ++i__) {
	res12 += cheb12[i__ - 1] * rh[i__];
	res24 += cheb24[i__ - 1] * rh[i__];
/* L240: */
    }
    for (i__ = 14; i__ <= 25; ++i__) {
	res24 += cheb24[i__ - 1] * rh[i__];
/* L250: */
    }
L260:
    *result = (*result + res24) * factor;
    *abserr = (*abserr + (r__1 = res24 - res12, dabs(r__1))) * factor;
L270:
    return 0;
} /* qc25s_ */

#ifdef __cplusplus
	}
#endif
