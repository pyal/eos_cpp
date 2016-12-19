/* qc25f.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Table of constant values */

static integer c__2 = 2;

/* Subroutine */ int qc25f_(E_fp f, real *a, real *b, real *omega, integer *
	integr, integer *nrmom, integer *maxp1, integer *ksave, real *result, 
	real *abserr, integer *neval, real *resabs, real *resasc, integer *
	momcom, real *chebmo)
{
    /* Initialized data */

    static real x[11] = { (float).9914448613738104,(float).9659258262890683,(
	    float).9238795325112868,(float).8660254037844386,(float)
	    .7933533402912352,(float).7071067811865475,(float)
	    .6087614290087206,(float).5,(float).3826834323650898,(float)
	    .2588190451025208,(float).1305261922200516 };

    /* System generated locals */
    integer chebmo_dim1, chebmo_offset, i__1;
    real r__1, r__2;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    real conc, asap, par22, fval[25], estc, cons;
    integer iers;
    extern /* Subroutine */ int qk15w_(E_fp, E_fp, real *, real *, real *, 
	    real *, integer *, real *, real *, real *, real *, real *, real *)
	    ;
    real ests;
    integer isym, noeq1;
    real cheb12[13], d__[25], cheb24[25];
    integer i__, j, k, m;
    extern /* Subroutine */ int qcheb_(real *, real *, real *, real *);
    real v[28], resc12, resc24, hlgth, centr, ress12, ress24, d1[25], d2[25], 
	    oflow;
    extern doublereal qwgtf_(real *x, real *omega, real *p2, real *p3, real *p4, 
							 integer *integr);
    integer noequ;
    extern /* Subroutine */ int sgtsl_(integer *, real *, real *, real *, 
	    real *, integer *);
    real p2, p3, p4;
    extern doublereal r1mach_(integer *);
    real ac, an, as, cospar, sinpar, parint, an2, ass, par2;

/* ***begin prologue  qc25f */
/* ***date written   810101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a2a2 */
/* ***keywords  integration rules for functions with cos or sin */
/*             factor, clenshaw-curtis, gauss-kronrod */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  to compute the integral i=integral of f(x) over (a,b) */
/*            where w(x) = cos(omega*x) or (wx)=sin(omega*x) */
/*            and to compute j=integral of abs(f) over (a,b). for small */
/*            value of omega or small intervals (a,b) 15-point gauss- */
/*            kronrod rule used. otherwise generalized clenshaw-curtis us */
/* ***description */

/*        integration rules for functions with cos or sin factor */
/*        standard fortran subroutine */
/*        real version */

/*        parameters */
/*         on entry */
/*           f      - real */
/*                    function subprogram defining the integrand */
/*                    function f(x). the actual name for f needs to */
/*                    be declared e x t e r n a l in the calling program. */

/*           a      - real */
/*                    lower limit of integration */

/*           b      - real */
/*                    upper limit of integration */

/*           omega  - real */
/*                    parameter in the weight function */

/*           integr - integer */
/*                    indicates which weight function is to be used */
/*                       integr = 1   w(x) = cos(omega*x) */
/*                       integr = 2   w(x) = sin(omega*x) */

/*           nrmom  - integer */
/*                    the length of interval (a,b) is equal to the length */
/*                    of the original integration interval divided by */
/*                    2**nrmom (we suppose that the routine is used in an */
/*                    adaptive integration process, otherwise set */
/*                    nrmom = 0). nrmom must be zero at the first call. */

/*           maxp1  - integer */
/*                    gives an upper bound on the number of chebyshev */
/*                    moments which can be stored, i.e. for the */
/*                    intervals of lengths abs(bb-aa)*2**(-l), */
/*                    l = 0,1,2, ..., maxp1-2. */

/*           ksave  - integer */
/*                    key which is one when the moments for the */
/*                    current interval have been computed */

/*         on return */
/*           result - real */
/*                    approximation to the integral i */

/*           abserr - real */
/*                    estimate of the modulus of the absolute */
/*                    error, which should equal or exceed abs(i-result) */

/*           neval  - integer */
/*                    number of integrand evaluations */

/*           resabs - real */
/*                    approximation to the integral j */

/*           resasc - real */
/*                    approximation to the integral of abs(f-i/(b-a)) */

/*         on entry and return */
/*           momcom - integer */
/*                    for each interval length we need to compute the */
/*                    chebyshev moments. momcom counts the number of */
/*                    intervals for which these moments have already been */
/*                    computed. if nrmom.lt.momcom or ksave = 1, the */
/*                    chebyshev moments for the interval (a,b) have */
/*                    already been computed and stored, otherwise we */
/*                    compute them and we increase momcom. */

/*           chebmo - real */
/*                    array of dimension at least (maxp1,25) containing */
/*                    the modified chebyshev moments for the first momcom */
/*                    momcom interval lengths */

/* ***references  (none) */
/* ***routines called  qcheb,qk15w,qwgtf,r1mach,sgtsl */
/* ***end prologue  qc25f */




/*           the vector x contains the values cos(k*pi/24) */
/*           k = 1, ...,11, to be used for the chebyshev expansion of f */

    /* Parameter adjustments */
    chebmo_dim1 = *maxp1;
    chebmo_offset = 1 + chebmo_dim1 * 1;
    chebmo -= chebmo_offset;

    /* Function Body */

/*           list of major variables */
/*           ----------------------- */

/*           centr  - mid point of the integration interval */
/*           hlgth  - half-length of the integration interval */
/*           fval   - value of the function f at the points */
/*                    (b-a)*0.5*cos(k*pi/12) + (b+a)*0.5, */
/*                    k = 0, ..., 24 */
/*           cheb12 - coefficients of the chebyshev series expansion */
/*                    of degree 12, for the function f, in the */
/*                    interval (a,b) */
/*           cheb24 - coefficients of the chebyshev series expansion */
/*                    of degree 24, for the function f, in the */
/*                    interval (a,b) */
/*           resc12 - approximation to the integral of */
/*                    cos(0.5*(b-a)*omega*x)*f(0.5*(b-a)*x+0.5*(b+a)) */
/*                    over (-1,+1), using the chebyshev series */
/*                    expansion of degree 12 */
/*           resc24 - approximation to the same integral, using the */
/*                    chebyshev series expansion of degree 24 */
/*           ress12 - the analogue of resc12 for the sine */
/*           ress24 - the analogue of resc24 for the sine */


/*           machine dependent constant */
/*           -------------------------- */

/*           oflow is the largest positive magnitude. */

/* ***first executable statement  qc25f */
    oflow = r1mach_(&c__2);

    centr = (*b + *a) * (float).5;
    hlgth = (*b - *a) * (float).5;
    parint = *omega * hlgth;

/*           compute the integral using the 15-point gauss-kronrod */
/*           formula if the value of the parameter in the integrand */
/*           is small. */

    if (dabs(parint) > (float)2.) {
	goto L10;
    }
    qk15w_((E_fp)f, (E_fp)qwgtf_, omega, &p2, &p3, &p4, integr, a, b, result, 
	    abserr, resabs, resasc);
    *neval = 15;
    goto L170;

/*           compute the integral using the generalized clenshaw- */
/*           curtis method. */

L10:
    conc = hlgth * cos(centr * *omega);
    cons = hlgth * sin(centr * *omega);
    *resasc = oflow;
    *neval = 25;

/*           check whether the chebyshev moments for this interval */
/*           have already been computed. */

    if (*nrmom < *momcom || *ksave == 1) {
	goto L120;
    }

/*           compute a new set of chebyshev moments. */

    m = *momcom + 1;
    par2 = parint * parint;
    par22 = par2 + (float)2.;
    sinpar = sin(parint);
    cospar = cos(parint);

/*           compute the chebyshev moments with respect to cosine. */

    v[0] = sinpar * (float)2. / parint;
    v[1] = (cospar * (float)8. + (par2 + par2 - (float)8.) * sinpar / parint) 
	    / par2;
    v[2] = ((par2 - (float)12.) * (float)32. * cospar + ((par2 - (float)80.) *
	     par2 + (float)192.) * (float)2. * sinpar / parint) / (par2 * 
	    par2);
    ac = cospar * (float)8.;
    as = parint * (float)24. * sinpar;
    if (dabs(parint) > (float)24.) {
	goto L30;
    }

/*           compute the chebyshev moments as the */
/*           solutions of a boundary value problem with 1 */
/*           initial value (v(3)) and 1 end value (computed */
/*           using an asymptotic formula). */

    noequ = 25;
    noeq1 = noequ - 1;
    an = (float)6.;
    i__1 = noeq1;
    for (k = 1; k <= i__1; ++k) {
	an2 = an * an;
	d__[k - 1] = (an2 - (float)4.) * (float)-2. * (par22 - an2 - an2);
	d2[k - 1] = (an - (float)1.) * (an - (float)2.) * par2;
	d1[k] = (an + (float)3.) * (an + (float)4.) * par2;
	v[k + 2] = as - (an2 - (float)4.) * ac;
	an += (float)2.;
/* L20: */
    }
    an2 = an * an;
    d__[noequ - 1] = (an2 - (float)4.) * (float)-2. * (par22 - an2 - an2);
    v[noequ + 2] = as - (an2 - (float)4.) * ac;
    v[3] -= par2 * (float)56. * v[2];
    ass = parint * sinpar;
    asap = (((((par2 * (float)210. - (float)1.) * cospar - (par2 * (float)
	    105. - (float)63.) * ass) / an2 - ((float)1. - par2 * (float)15.) 
	    * cospar + ass * (float)15.) / an2 - cospar + ass * (float)3.) / 
	    an2 - cospar) / an2;
    v[noequ + 2] -= asap * (float)2. * par2 * (an - (float)1.) * (an - (float)
	    2.);

/*           solve the tridiagonal system by means of gaussian */
/*           elimination with partial pivoting. */

    sgtsl_(&noequ, d1, d__, d2, &v[3], &iers);
    goto L50;

/*           compute the chebyshev moments by means of forward */
/*           recursion. */

L30:
    an = (float)4.;
    for (i__ = 4; i__ <= 13; ++i__) {
	an2 = an * an;
	v[i__ - 1] = ((an2 - (float)4.) * ((par22 - an2 - an2) * (float)2. * 
		v[i__ - 2] - ac) + as - par2 * (an + (float)1.) * (an + (
		float)2.) * v[i__ - 3]) / (par2 * (an - (float)1.) * (an - (
		float)2.));
	an += (float)2.;
/* L40: */
    }
L50:
    for (j = 1; j <= 13; ++j) {
	chebmo[m + ((j << 1) - 1) * chebmo_dim1] = v[j - 1];
/* L60: */
    }

/*           compute the chebyshev moments with respect to sine. */

    v[0] = (sinpar - parint * cospar) * (float)2. / par2;
    v[1] = ((float)18. - (float)48. / par2) * sinpar / par2 + ((float)48. / 
	    par2 - (float)2.) * cospar / parint;
    ac = parint * (float)-24. * cospar;
    as = sinpar * (float)-8.;
    if (dabs(parint) > (float)24.) {
	goto L80;
    }

/*           compute the chebyshev moments as the */
/*           solutions of a boundary value problem with 1 */
/*           initial value (v(2)) and 1 end value (computed */
/*           using an asymptotic formula). */

    an = (float)5.;
    i__1 = noeq1;
    for (k = 1; k <= i__1; ++k) {
	an2 = an * an;
	d__[k - 1] = (an2 - (float)4.) * (float)-2. * (par22 - an2 - an2);
	d2[k - 1] = (an - (float)1.) * (an - (float)2.) * par2;
	d1[k] = (an + (float)3.) * (an + (float)4.) * par2;
	v[k + 1] = ac + (an2 - (float)4.) * as;
	an += (float)2.;
/* L70: */
    }
    an2 = an * an;
    d__[noequ - 1] = (an2 - (float)4.) * (float)-2. * (par22 - an2 - an2);
    v[noequ + 1] = ac + (an2 - (float)4.) * as;
    v[2] -= par2 * (float)42. * v[1];
    ass = parint * cospar;
    asap = (((((par2 * (float)105. - (float)63.) * ass + (par2 * (float)210. 
	    - (float)1.) * sinpar) / an2 + (par2 * (float)15. - (float)1.) * 
	    sinpar - ass * (float)15.) / an2 - ass * (float)3. - sinpar) / 
	    an2 - sinpar) / an2;
    v[noequ + 1] -= asap * (float)2. * par2 * (an - (float)1.) * (an - (float)
	    2.);

/*           solve the tridiagonal system by means of gaussian */
/*           elimination with partial pivoting. */

    sgtsl_(&noequ, d1, d__, d2, &v[2], &iers);
    goto L100;

/*           compute the chebyshev moments by means of */
/*           forward recursion. */

L80:
    an = (float)3.;
    for (i__ = 3; i__ <= 12; ++i__) {
	an2 = an * an;
	v[i__ - 1] = ((an2 - (float)4.) * ((par22 - an2 - an2) * (float)2. * 
		v[i__ - 2] + as) + ac - par2 * (an + (float)1.) * (an + (
		float)2.) * v[i__ - 3]) / (par2 * (an - (float)1.) * (an - (
		float)2.));
	an += (float)2.;
/* L90: */
    }
L100:
    for (j = 1; j <= 12; ++j) {
	chebmo[m + (j << 1) * chebmo_dim1] = v[j - 1];
/* L110: */
    }
L120:
    if (*nrmom < *momcom) {
	m = *nrmom + 1;
    }
    if (*momcom < *maxp1 - 1 && *nrmom >= *momcom) {
	++(*momcom);
    }

/*           compute the coefficients of the chebyshev expansions */
/*           of degrees 12 and 24 of the function f. */

    r__1 = centr + hlgth;
    fval[0] = (*f)(&r__1) * (float).5;
    fval[12] = (*f)(&centr);
    r__1 = centr - hlgth;
    fval[24] = (*f)(&r__1) * (float).5;
    for (i__ = 2; i__ <= 12; ++i__) {
	isym = 26 - i__;
	r__1 = hlgth * x[i__ - 2] + centr;
	fval[i__ - 1] = (*f)(&r__1);
	r__1 = centr - hlgth * x[i__ - 2];
	fval[isym - 1] = (*f)(&r__1);
/* L130: */
    }
    qcheb_(x, fval, cheb12, cheb24);

/*           compute the integral and error estimates. */

    resc12 = cheb12[12] * chebmo[m + chebmo_dim1 * 13];
    ress12 = (float)0.;
    k = 11;
    for (j = 1; j <= 6; ++j) {
	resc12 += cheb12[k - 1] * chebmo[m + k * chebmo_dim1];
	ress12 += cheb12[k] * chebmo[m + (k + 1) * chebmo_dim1];
	k += -2;
/* L140: */
    }
    resc24 = cheb24[24] * chebmo[m + chebmo_dim1 * 25];
    ress24 = (float)0.;
    *resabs = dabs(cheb24[24]);
    k = 23;
    for (j = 1; j <= 12; ++j) {
	resc24 += cheb24[k - 1] * chebmo[m + k * chebmo_dim1];
	ress24 += cheb24[k] * chebmo[m + (k + 1) * chebmo_dim1];
	*resabs = (r__1 = cheb24[k - 1], dabs(r__1)) + (r__2 = cheb24[k], 
		dabs(r__2));
	k += -2;
/* L150: */
    }
    estc = (r__1 = resc24 - resc12, dabs(r__1));
    ests = (r__1 = ress24 - ress12, dabs(r__1));
    *resabs *= dabs(hlgth);
    if (*integr == 2) {
	goto L160;
    }
    *result = conc * resc24 - cons * ress24;
    *abserr = (r__1 = conc * estc, dabs(r__1)) + (r__2 = cons * ests, dabs(
	    r__2));
    goto L170;
L160:
    *result = conc * ress24 + cons * resc24;
    *abserr = (r__1 = conc * ests, dabs(r__1)) + (r__2 = cons * estc, dabs(
	    r__2));
L170:
    return 0;
} /* qc25f_ */

#ifdef __cplusplus
	}
#endif
