/* qng.f -- translated by f2c (version 20000118).
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
static doublereal c_b17 = 1.5;
static integer c__26 = 26;
static integer c__0 = 0;

/* Subroutine */ int qng_(E_fp f, real *a, real *b, real *epsabs, real *
	epsrel, real *result, real *abserr, integer *neval, integer *ier)
{
    /* Initialized data */

    static real x1[5] = { (float).9739065285171717,(float).8650633666889845,(
	    float).6794095682990244,(float).4333953941292472,(float)
	    .1488743389816312 };
    static real w87a[21] = { (float).008148377384149173,(float)
	    .01876143820156282,(float).02734745105005229,(float)
	    .03367770731163793,(float).03693509982042791,(float)
	    .002884872430211531,(float).0136859460227127,(float)
	    .02328041350288831,(float).03087249761171336,(float)
	    .03569363363941877,(float)9.152833452022414e-4,(float)
	    .005399280219300471,(float).01094767960111893,(float)
	    .01629873169678734,(float).02108156888920384,(float)
	    .02537096976925383,(float).02918969775647575,(float)
	    .03237320246720279,(float).03478309895036514,(float)
	    .03641222073135179,(float).03725387550304771 };
    static real w87b[23] = { (float)2.741455637620724e-4,(float)
	    .001807124155057943,(float).004096869282759165,(float)
	    .006758290051847379,(float).009549957672201647,(float)
	    .01232944765224485,(float).01501044734638895,(float)
	    .01754896798624319,(float).01993803778644089,(float)
	    .02219493596101229,(float).02433914712600081,(float)
	    .02637450541483921,(float).0282869107887712,(float)
	    .0300525811280927,(float).03164675137143993,(float)
	    .0330504134199785,(float).03425509970422606,(float)
	    .03526241266015668,(float).0360769896228887,(float)
	    .03669860449845609,(float).03712054926983258,(float)
	    .03733422875193504,(float).03736107376267902 };
    static real x2[5] = { (float).9956571630258081,(float).9301574913557082,(
	    float).7808177265864169,(float).5627571346686047,(float)
	    .2943928627014602 };
    static real x3[11] = { (float).9993333609019321,(float).9874334029080889,(
	    float).9548079348142663,(float).9001486957483283,(float)
	    .8251983149831142,(float).732148388989305,(float)
	    .6228479705377252,(float).4994795740710565,(float)
	    .3649016613465808,(float).2222549197766013,(float)
	    .07465061746138332 };
    static real x4[22] = { (float).9999029772627292,(float).9979898959866787,(
	    float).9921754978606872,(float).9813581635727128,(float)
	    .9650576238583846,(float).9431676131336706,(float)
	    .9158064146855072,(float).8832216577713165,(float)
	    .8457107484624157,(float).803557658035231,(float)
	    .7570057306854956,(float).7062732097873218,(float)
	    .6515894665011779,(float).5932233740579611,(float)
	    .5314936059708319,(float).4667636230420228,(float)
	    .3994248478592188,(float).3298748771061883,(float)
	    .2585035592021616,(float).1856953965683467,(float)
	    .1118422131799075,(float).03735212339461987 };
    static real w10[5] = { (float).06667134430868814,(float).1494513491505806,
	    (float).219086362515982,(float).2692667193099964,(float)
	    .2955242247147529 };
    static real w21a[5] = { (float).03255816230796473,(float)
	    .07503967481091995,(float).1093871588022976,(float)
	    .1347092173114733,(float).1477391049013385 };
    static real w21b[6] = { (float).01169463886737187,(float).054755896574352,
	    (float).09312545458369761,(float).1234919762620659,(float)
	    .1427759385770601,(float).1494455540029169 };
    static real w43a[10] = { (float).01629673428966656,(float)
	    .0375228761208695,(float).05469490205825544,(float)
	    .06735541460947809,(float).07387019963239395,(float)
	    .005768556059769796,(float).02737189059324884,(float)
	    .04656082691042883,(float).06174499520144256,(float)
	    .0713872672686934 };
    static real w43b[12] = { (float).001844477640212414,(float)
	    .01079868958589165,(float).02189536386779543,(float)
	    .03259746397534569,(float).04216313793519181,(float)
	    .05074193960018458,(float).05837939554261925,(float)
	    .06474640495144589,(float).06956619791235648,(float)
	    .07282444147183321,(float).07450775101417512,(float)
	    .07472214751740301 };

    /* System generated locals */
    real r__1, r__2, r__3, r__4;
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    real absc, fval, res10, res21, res43, res87, fval1, fval2;
    integer k, l;
    real hlgth, centr, reskh, uflow;
    extern doublereal r1mach_(integer *);
    real epmach, dhlgth, resabs, resasc, fcentr, savfun[21], fv1[5], fv2[5], 
	    fv3[5], fv4[5];
    extern /* Subroutine */ int xerror_(char *, integer *, integer *, integer 
	    *, ftnlen);
    integer ipx;

/* ***begin prologue  qng */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a1a1 */
/* ***keywords  automatic integrator, smooth integrand, */
/*             non-adaptive, gauss-kronrod(patterson) */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl math & progr. div. - k.u.leuven */
/*           kahaner,david,nbs - modified (2/82) */
/* ***purpose  the routine calculates an approximation result to a */
/*            given definite integral i = integral of f over (a,b), */
/*            hopefully satisfying following claim for accuracy */
/*            abs(i-result).le.max(epsabs,epsrel*abs(i)). */
/* ***description */

/* non-adaptive integration */
/* standard fortran subroutine */
/* real version */

/*           f      - real version */
/*                    function subprogram defining the integrand function */
/*                    f(x). the actual name for f needs to be declared */
/*                    e x t e r n a l in the driver program. */

/*           a      - real version */
/*                    lower limit of integration */

/*           b      - real version */
/*                    upper limit of integration */

/*           epsabs - real */
/*                    absolute accuracy requested */
/*           epsrel - real */
/*                    relative accuracy requested */
/*                    if  epsabs.le.0 */
/*                    and epsrel.lt.max(50*rel.mach.acc.,0.5d-28), */
/*                    the routine will end with ier = 6. */

/*         on return */
/*           result - real */
/*                    approximation to the integral i */
/*                    result is obtained by applying the 21-point */
/*                    gauss-kronrod rule (res21) obtained by optimal */
/*                    addition of abscissae to the 10-point gauss rule */
/*                    (res10), or by applying the 43-point rule (res43) */
/*                    obtained by optimal addition of abscissae to the */
/*                    21-point gauss-kronrod rule, or by applying the */
/*                    87-point rule (res87) obtained by optimal addition */
/*                    of abscissae to the 43-point rule. */

/*           abserr - real */
/*                    estimate of the modulus of the absolute error, */
/*                    which should equal or exceed abs(i-result) */

/*           neval  - integer */
/*                    number of integrand evaluations */

/*           ier    - ier = 0 normal and reliable termination of the */
/*                            routine. it is assumed that the requested */
/*                            accuracy has been achieved. */
/*                    ier.gt.0 abnormal termination of the routine. it is */
/*                            assumed that the requested accuracy has */
/*                            not been achieved. */
/*           error messages */
/*                    ier = 1 the maximum number of steps has been */
/*                            executed. the integral is probably too */
/*                            difficult to be calculated by dqng. */
/*                        = 6 the input is invalid, because */
/*                            epsabs.le.0 and */
/*                            epsrel.lt.max(50*rel.mach.acc.,0.5d-28). */
/*                            result, abserr and neval are set to zero. */

/* ***references  (none) */
/* ***routines called  r1mach,xerror */
/* ***end prologue  qng */



/*           the following data statements contain the */
/*           abscissae and weights of the integration rules used. */

/*           x1      abscissae common to the 10-, 21-, 43- */
/*                   and 87-point rule */
/*           x2      abscissae common to the 21-, 43- and */
/*                   87-point rule */
/*           x3      abscissae common to the 43- and 87-point */
/*                   rule */
/*           x4      abscissae of the 87-point rule */
/*           w10     weights of the 10-point formula */
/*           w21a    weights of the 21-point formula for */
/*                   abscissae x1 */
/*           w21b    weights of the 21-point formula for */
/*                   abscissae x2 */
/*           w43a    weights of the 43-point formula for */
/*                   abscissae x1, x3 */
/*           w43b    weights of the 43-point formula for */
/*                   abscissae x3 */
/*           w87a    weights of the 87-point formula for */
/*                   abscissae x1, x2, x3 */
/*           w87b    weights of the 87-point formula for */
/*                   abscissae x4 */


/*           list of major variables */
/*           ----------------------- */

/*           centr  - mid point of the integration interval */
/*           hlgth  - half-length of the integration interval */
/*           fcentr - function value at mid point */
/*           absc   - abscissa */
/*           fval   - function value */
/*           savfun - array of function values which */
/*                    have already been computed */
/*           res10  - 10-point gauss result */
/*           res21  - 21-point kronrod result */
/*           res43  - 43-point result */
/*           res87  - 87-point result */
/*           resabs - approximation to the integral of abs(f) */
/*           resasc - approximation to the integral of abs(f-i/(b-a)) */

/*           machine dependent constants */
/*           --------------------------- */

/*           epmach is the largest relative spacing. */
/*           uflow is the smallest positive magnitude. */

/* ***first executable statement  qng */
    epmach = r1mach_(&c__4);
    uflow = r1mach_(&c__1);

/*           test on validity of parameters */
/*           ------------------------------ */

    *result = (float)0.;
    *abserr = (float)0.;
    *neval = 0;
    *ier = 6;
/* Computing MAX */
    r__1 = (float)5e-15, r__2 = epmach * (float)50.;
    if (*epsabs <= (float)0. && *epsrel < dmax(r__1,r__2)) {
	goto L80;
    }
    hlgth = (*b - *a) * (float).5;
    dhlgth = dabs(hlgth);
    centr = (*b + *a) * (float).5;
    fcentr = (*f)(&centr);
    *neval = 21;
    *ier = 1;

/*          compute the integral using the 10- and 21-point formula. */

    for (l = 1; l <= 3; ++l) {
	switch (l) {
	    case 1:  goto L5;
	    case 2:  goto L25;
	    case 3:  goto L45;
	}
L5:
	res10 = (float)0.;
	res21 = w21b[5] * fcentr;
	resabs = w21b[5] * dabs(fcentr);
	for (k = 1; k <= 5; ++k) {
	    absc = hlgth * x1[k - 1];
	    r__1 = centr + absc;
	    fval1 = (*f)(&r__1);
	    r__1 = centr - absc;
	    fval2 = (*f)(&r__1);
	    fval = fval1 + fval2;
	    res10 += w10[k - 1] * fval;
	    res21 += w21a[k - 1] * fval;
	    resabs += w21a[k - 1] * (dabs(fval1) + dabs(fval2));
	    savfun[k - 1] = fval;
	    fv1[k - 1] = fval1;
	    fv2[k - 1] = fval2;
/* L10: */
	}
	ipx = 5;
	for (k = 1; k <= 5; ++k) {
	    ++ipx;
	    absc = hlgth * x2[k - 1];
	    r__1 = centr + absc;
	    fval1 = (*f)(&r__1);
	    r__1 = centr - absc;
	    fval2 = (*f)(&r__1);
	    fval = fval1 + fval2;
	    res21 += w21b[k - 1] * fval;
	    resabs += w21b[k - 1] * (dabs(fval1) + dabs(fval2));
	    savfun[ipx - 1] = fval;
	    fv3[k - 1] = fval1;
	    fv4[k - 1] = fval2;
/* L15: */
	}

/*          test for convergence. */

	*result = res21 * hlgth;
	resabs *= dhlgth;
	reskh = res21 * (float).5;
	resasc = w21b[5] * (r__1 = fcentr - reskh, dabs(r__1));
	for (k = 1; k <= 5; ++k) {
	    resasc = resasc + w21a[k - 1] * ((r__1 = fv1[k - 1] - reskh, dabs(
		    r__1)) + (r__2 = fv2[k - 1] - reskh, dabs(r__2))) + w21b[
		    k - 1] * ((r__3 = fv3[k - 1] - reskh, dabs(r__3)) + (r__4 
		    = fv4[k - 1] - reskh, dabs(r__4)));
/* L20: */
	}
	*abserr = (r__1 = (res21 - res10) * hlgth, dabs(r__1));
	resasc *= dhlgth;
	goto L65;

/*          compute the integral using the 43-point formula. */

L25:
	res43 = w43b[11] * fcentr;
	*neval = 43;
	for (k = 1; k <= 10; ++k) {
	    res43 += savfun[k - 1] * w43a[k - 1];
/* L30: */
	}
	for (k = 1; k <= 11; ++k) {
	    ++ipx;
	    absc = hlgth * x3[k - 1];
	    r__1 = absc + centr;
	    r__2 = centr - absc;
	    fval = (*f)(&r__1) + (*f)(&r__2);
	    res43 += fval * w43b[k - 1];
	    savfun[ipx - 1] = fval;
/* L40: */
	}

/*          test for convergence. */

	*result = res43 * hlgth;
	*abserr = (r__1 = (res43 - res21) * hlgth, dabs(r__1));
	goto L65;

/*          compute the integral using the 87-point formula. */

L45:
	res87 = w87b[22] * fcentr;
	*neval = 87;
	for (k = 1; k <= 21; ++k) {
	    res87 += savfun[k - 1] * w87a[k - 1];
/* L50: */
	}
	for (k = 1; k <= 22; ++k) {
	    absc = hlgth * x4[k - 1];
	    r__1 = absc + centr;
	    r__2 = centr - absc;
	    res87 += w87b[k - 1] * ((*f)(&r__1) + (*f)(&r__2));
/* L60: */
	}
	*result = res87 * hlgth;
	*abserr = (r__1 = (res87 - res43) * hlgth, dabs(r__1));
L65:
	if (resasc != (float)0. && *abserr != (float)0.) {
/* Computing MIN */
	    d__1 = (doublereal) (*abserr * (float)200. / resasc);
	    r__1 = (float)1., r__2 = pow_dd(&d__1, &c_b17);
	    *abserr = resasc * dmin(r__1,r__2);
	}
	if (resabs > uflow / (epmach * (float)50.)) {
/* Computing MAX */
	    r__1 = epmach * (float)50. * resabs;
	    *abserr = dmax(r__1,*abserr);
	}
/* Computing MAX */
	r__1 = *epsabs, r__2 = *epsrel * dabs(*result);
	if (*abserr <= dmax(r__1,r__2)) {
	    *ier = 0;
	}
/* ***jump out of do-loop */
	if (*ier == 0) {
	    goto L999;
	}
/* L70: */
    }
L80:
    xerror_("abnormal return from  qng ", &c__26, ier, &c__0, (ftnlen)26);
L999:
    return 0;
} /* qng_ */

#ifdef __cplusplus
	}
#endif
