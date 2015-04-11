/* E04ABE.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

double NAGMin(

#include "f2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Table of constant values */

static real c_b6 = (float)0.;
#define EPSMCH 1e-18
/* Subroutine */ int e04abe_(S_fp fun, real *eps, real *t, real *a, real *b, 
	integer *maxcal, real *x, real *f, integer *ifail)
{
    /* Initialized data */

    /* System generated locals */
    real r__1;

    /* Builtin functions */
//    double sqrt(doublereal);

    /* Local variables */
    real oldf;
    integer iloc, numf;
    real d__, e;
//    extern integer p01aae_(integer *, integer *, doublereal *);
//    extern doublereal x02aae_(real *);
    integer iflag;
    real u;
    extern /* Subroutine */ int abze04_(real *, real *, real *, real *, real *
	    , real *, real *, real *, real *, real *, real *, real *, real *, 
	    real *, real *, real *, real *, real *, real *, real *, real *, 
	    real *, real *, real *, real *, real *, real *, integer *, 
	    integer *);
    real scxbd;
    integer isave;
    real b1, f1, f2, rteps, x1, x2, fa, gtest1, gtest2, fu, fv, fw, gu, rr, 
	    ss, xlamda, xv, epsmch, xw, sftbnd, pt2, pt4, pt6, tol;


/*     MARK 6 RELEASE NAG COPYRIGHT 1977 */
/*     MARK 8 REVISED. IER-231 (MAR 1980). */
/*     MARK 8D REVISED. IER-272 (DEC 1980). */

/*     ************************************************************** */

/*     E04ABE ATTEMPTS TO FIND A MINIMUM IN AN INTERVAL A .LE. X .LE. */
/*     B OF A FUNCTION F(X) OF THE SCALAR X, USING FUNCTION VALUES */
/*     ONLY. */

/*     IT IS BASED ON THE SUBROUTINE UNIFUN IN THE NPL ALGORITHMS */
/*     LIBRARY (REF. NO. E4/13/F). THE FUNCTION F(X) IS DEFINED BY */
/*     THE USER-SUPPLIED SUBROUTINE FUN. T AND EPS DEFINE A TOLERANCE */
/*     TOL = EPS * ABS(X) + T, AND FUN IS NEVER EVALUATED AT TWO */
/*     POINTS CLOSER THAN TOL. IF FUN IS DELTA-UNIMODAL, FOR SOME */
/*     DELTA LESS THAN TOL, THEN X APPROXIMATES THE GLOBAL MINIMUM OF */
/*     FUN WITH AN ERROR LESS THAN 3*TOL. IF FUN IS NOT DELTA- */
/*     UNIMODAL ON (A, B), THEN X MAY APPROXIMATE A LOCAL, BUT NON */
/*     GLOBAL, MINIMUM. EPS SHOULD BE NO SMALLER THAN 2*EPSMCH, AND */
/*     PREFERABLY NOT MUCH LESS THAN SQRT(EPSMCH), WHERE EPSMCH IS */
/*     THE RELATIVE MACHINE PRECISION. T SHOULD BE POSITIVE. NOTE */
/*     THAT, FOR CONSISTENCY WITH OTHER E04 DOCUMENTATION, THE NAME */
/*     FUNCT IS USED INSTEAD OF FUN IN THE WRITE-UP. */

/*     PHILIP E. GILL, WALTER MURRAY, SUSAN M. PICKEN, HAZEL M. */
/*     BARBER AND MARGARET H. WRIGHT, D.N.A.C., NATIONAL PHYSICAL */
/*     LABORATORY, ENGLAND */

/*     ************************************************************** */

/*     .. SCALAR ARGUMENTS .. */
/*     .. SUBROUTINE ARGUMENTS .. */
/*     FUN */
/*     .. */
/*     .. LOCAL SCALARS .. */
/*     .. FUNCTION REFERENCES .. */
/*     .. SUBROUTINE REFERENCES .. */
/*     ABZE04 */
/*     .. */
    isave = *ifail;

/*     A MACHINE-DEPENDENT CONSTANT IS SET HERE. EPSMCH IS THE */
/*     SMALLEST POSITIVE REAL NUMBER SUCH THAT 1.0 + EPSMCH .GT. 1.0 */

    epsmch = EPSMCH;
    rteps = sqrt(epsmch);
    if (*eps < epsmch) {
	*eps = rteps;
    }
    if (*t < epsmch) {
	*t = rteps;
    }

/*     ERROR IN INPUT PARAMETERS */

    *ifail = 1;
    if (*a + *t >= *b || *maxcal < 3 || *ifail < 0 || *ifail > 1) {
	goto L140;
    }
    sftbnd = *a;
    pt2 = (*b - *a) * (float).2;
    pt4 = pt2 + pt2;
    pt6 = pt2 + pt4;
    x1 = *a + pt4;
    (*fun)(&x1, &f1);
    x2 = *b - pt4;
    (*fun)(&x2, &f2);
    xlamda = *b;
    if (f1 > f2) {
	goto L20;
    }
    *x = x1;
    *a = -pt4;
    *b = pt2;
    xw = pt2;
    b1 = pt2;
    rr = (float)1.;
    d__ = -pt2;
    fw = f2;
    fv = f1;
    *f = f1;

/*     SET STEP TO NEW POINT */

    u = -pt2;
    goto L40;
L20:
    *x = x2;
    *a = -pt2;
    *b = pt4 + *eps * dabs(xlamda) + *t;
    xw = -pt2;
    b1 = *b;
    rr = (float)-1.;
    d__ = pt2;
    fw = f1;
    fv = f2;
    *f = f2;

/*     SET STEP TO NEW POINT */

    u = pt2;
L40:
    xv = (float)0.;
    scxbd = pt4;
    e = pt6;
    ss = (float)0.;
    fa = fw + *t;
    oldf = fa;
    gtest1 = (float)0.;
    gtest2 = (float)0.;
    tol = *eps * dabs(*x) + *t;
    r__1 = *x + u;
    (*fun)(&r__1, &fu);
    gu = (float)0.;
    numf = 3;

/*     SET ILOC TO 3 SO THAT THE MAIN SECTION OF ABZE04 IS EXECUTED */
/*     AS THE INITIAL 3 POINTS HAVE ALREADY BEEN SET UP */

    iloc = 3;
L60:
    abze04_(eps, t, &c_b6, &sftbnd, &xlamda, &u, &fu, &gu, x, f, &xw, &fw, &
	    xv, &fv, a, &fa, b, &oldf, &b1, &scxbd, &e, &d__, &rr, &ss, &
	    gtest1, &gtest2, &tol, &iloc, &iflag);
    if (iflag != 1) {
	goto L100;
    }
    if (numf >= *maxcal) {
	goto L80;
    }
    r__1 = *x + u;
    (*fun)(&r__1, &fu);
    ++numf;
    goto L60;
L80:
    *ifail = 2;
    goto L120;
L100:
    *ifail = 0;
L120:
    *maxcal = numf;
    *a += *x;
    *b += *x;
L140:
    if (*ifail == 0) {
	return 0;
    }
    *ifail = 10;
    return 0;

/*     END OF E04ABE (UNIFUN) */

} /* e04abe_ */

/* Subroutine */ int abze04_(real *eps, real *t, real *eta, real *sftbnd, 
	real *xlamda, real *u, real *fu, real *gu, real *xmin, real *fmin, 
	real *xw, real *fw, real *xv, real *fv, real *a, real *fa, real *b, 
	real *oldf, real *b1, real *scxbd, real *e, real *d__, real *rr, real 
	*ss, real *gtest1, real *gtest2, real *tol, integer *iloc, integer *
	itest)
{
    /* System generated locals */
    real r__1;

    /* Builtin functions */
//    double sqrt(doublereal);

    /* Local variables */
    real q, r__, s, a1, d1, d2, t2, xm;


/*     MARK 6 RELEASE NAG COPYRIGHT 1977 */
/*     MARK 7 REISSUE */
/*     MARK 8 REVISED. IER-239 (APR 1980). */
/*     MARK 8 REVISED. IER-244 (MAY 1980). */
/*     MARK 9 REVISED. IER-317 (SEP 1981). */

/*     ************************************************************** */

/*     ABZE04 (NEWPTQ), AN ALGORITHM FOR FINDING A STEPLENGTH, CALLED */
/*     REPEATEDLY BY NPL OPTIMIZATION ROUTINES WHICH REQUIRE A STEP- */
/*     LENGTH TO BE COMPUTED USING QUADRATIC INTERPOLATION. */
/*     THE PARAMETERS SET UP BEFORE THE CALL OF ABZE04 CONTAIN */
/*     INFORMATION ABOUT THE INTERVAL IN WHICH A LOWER POINT IS TO BE */
/*     FOUND AND FROM THIS ABZE04 PRODUCES A POINT AT WHICH THE */
/*     FUNCTION CAN BE EVALUATED OUTSIDE THIS SUBROUTINE. */
/*     THE VALUE OF THE INTEGER PARAMETER ILOC DETERMINES THE PATH */
/*     TAKEN THROUGH THE CODE. FOR A FURTHER DESCRIPTION OF ILOC */
/*     AND THE OTHER PARAMETERS SEE NPL ALGORITHMS LIBRARY REF. NO. */
/*     E4/15/F. */

/*     PHILIP E. GILL, WALTER MURRAY, SUSAN M. PICKEN, */
/*     MARGARET H. WRIGHT AND ENID M. LONG */
/*     D.N.A.C. NATIONAL PHYSICAL LABORATORY, ENGLAND */

/*     ************************************************************** */

/*     .. SCALAR ARGUMENTS .. */
/*     .. */
/*     .. LOCAL SCALARS .. */
/*     .. FUNCTION REFERENCES .. */
/*     .. */

/*     BRANCH TO APPROPRIATE SECTION OF CODE DEPENDING ON THE */
/*     VALUE OF THE FLAG ILOC */
/*     THE SIGNIFICANCE OF THE FLAGS ILOC AND ITEST ARE DESCRIBED IN */
/*     NPL ALGORITHMS LIBRARY DOCUMENT REF. NO. E4/15/F. */

    switch (*iloc) {
	case 1:  goto L20;
	case 2:  goto L40;
	case 3:  goto L40;
	case 4:  goto L460;
	case 5:  goto L440;
    }

/*     ILOC = 1 */

/*     CHECK INPUT PARAMETERS */

L20:
    *itest = 2;
    *tol = *t;
    t2 = *tol + *tol;
    if (*u <= (float)0. || *xlamda <= t2 || *gu > (float)0.) {
	return 0;
    }
    *itest = 1;

/*     A AND B DEFINE THE INTERVAL OF UNCERTAINTY. XMIN DENOTES */
/*     THE LOWEST POINT OBTAINED SO FAR, XW THE LAST VALUE */
/*     OF XMIN AND XV THE SCALED VALUE OF ALPHA CORRESPONDING TO */
/*     THE HIGHEST FUNCTION VALUE OF THE THREE POINTS THROUGH */
/*     WHICH A PARABOLA MAY BE FITTED. INITIALIZE A, XV, XW, XMIN */
/*     AT ORIGIN AND CORRESPONDING FUNCTION VALUES AT LATEST */
/*     ESTIMATE OF MINIMUM. */

    *xmin = (float)0.;
    *xw = (float)0.;
    *xv = (float)0.;
    *a = (float)0.;
    *oldf = *fu;
    *fmin = *fu;
    *fw = *fu;
    *fv = *fu;
    *fa = *fu;
    *d__ = *u;

/*     THE PARAMETER RR HAS TWO USES DURING THE EXECUTION OF THIS */
/*     SUBROUTINE.  INITIALLY THE SIGN OF RR INDICATES WHETHER OR NOT */
/*     THE MINIMUM HAS BEEN BRACKETED. LATER, WHEN A POINT SATISFYING */
/*     THE GTEST2 CRITERION HAS BEEN FOUND, RR IS USED TO COMPUTE A */
/*     STEPLENGTH WHICH SATISFIES THE SECOND CRITERION INVOLVING */
/*     GTEST1. */

    *rr = (float)-1.;

/*     SET UP XBND AS A BOUND ON THE STEP TO BE TAKEN. (XBND IS NOT */
/*     COMPUTED EXPLICITLY BUT SCXBD IS ITS SCALED VALUE.) SET THE */
/*     UPPER BOUND ON THE INTERVAL OF UNCERTAINTY INITIALLY TO */
/*     XLAMDA + TOL(XLAMDA). */

    *scxbd = *xlamda;
    *b = *scxbd + *eps * dabs(*scxbd) + *t;
    *e = *b * (float)2.;
    *b1 = *b;

/*     COMPUTE THE CONSTANTS REQUIRED FOR THE TWO CONVERGENCE */
/*     CRITERIA. */

    *gtest1 = *gu * (float)-1e-4;
    *gtest2 = -(*eta) * *gu;

/*     SET ILOC TO INDICATE THAT ONLY TWO POINTS ARE AVAILABLE */

    *iloc = 2;
    goto L380;

/*     ILOC = 2 OR 3 */

/*     UPDATE A, B, XV, XW, AND XMIN. */

L40:
    if (*fu > *fmin) {
	goto L100;
    }

/*     IF FUNCTION VALUE NOT INCREASED, NEW POINT BECOMES */
/*     NEXT ORIGIN AND OTHER POINTS ARE SCALED ACCORDINGLY. */

    if (*u < (float)0.) {
	goto L60;
    }
    *a = (float)0.;
    *fa = *fmin;
    if (*xw == *xv && *fmin == *fu) {
	*rr = (float)1.;
    }
    goto L80;
L60:
    *b = (float)0.;
    *rr = (float)1.;
L80:
    *xv = *xw;
    *fv = *fw;
    *fw = *fmin;
    *fmin = *fu;
    *xmin += *u;
    *a -= *u;
    *b -= *u;
    *xv -= *u;
    *xw = (float)0. - *u;

/*     THIS MAY BE CHANGED TO XW = - U IF THE COMPUTER IS */
/*     SUCH THAT  - U AND 0.0 - U ARE IDENTICAL. */

    *scxbd -= *u;
    *tol = *eps * dabs(*xmin) + *t;
    goto L180;

/*     IF FUNCTION VALUE INCREASED, ORIGIN REMAINS UNCHANGED */
/*     BUT OTHER POINTS MAY BE INTERCHANGED. */

L100:
    if (*u >= (float)0.) {
	goto L120;
    }
    *a = *u;
    *fa = *fu;
    goto L140;
L120:
    *b = *u;
    *rr = (float)1.;
L140:
    if (*fu > *fw && *xw != (float)0.) {
	goto L160;
    }
    *xv = *xw;
    *fv = *fw;
    *xw = *u;
    *fw = *fu;
    goto L180;
L160:
    *xv = *u;
    *fv = *fu;
L180:
    t2 = *tol + *tol;
    xm = (*a + *b) * (float).5;

/*     CHECK TERMINATION CRITERIA. */

    if (dabs(xm) <= t2 - (*b - *a) * (float).5 || *xmin + *b <= *sftbnd || *
	    fa - *fmin <= dabs(*a) * *gtest2 && *fmin < *oldf && ((r__1 = *
	    xmin - *xlamda, dabs(r__1)) > *tol || *rr < (float)0.)) {
	goto L420;
    }
    r__ = (float)0.;
    q = (float)0.;
    s = (float)0.;
    if (dabs(*e) <= *tol) {
	goto L240;
    }

/*     FIT PARABOLA THROUGH XMIN, XV, XW. */

    if (*iloc != 2) {
	goto L200;
    }

/*     SPECIAL CASE. ONLY TWO POINTS ARE AVAILABLE FOR */
/*     QUADRATIC INTERPOLATION */

    q = (*fw - *fmin - *xw * *gu) * (float)2.;
    s = *gu * *xw * *xw;
    if (*xmin != (float)0.) {
	s = ((*fmin - *fw) * (float)2. + *xw * *gu) * *xw;
    }
    goto L220;
L200:
    r__ = *xw * (*fv - *fmin);
    q = *xv * (*fw - *fmin);
    s = r__ * *xw - q * *xv;
    q = (q - r__) * (float)2.;
L220:
    if (q > (float)0.) {
	s = -s;
    }
    if (q <= (float)0.) {
	q = -q;
    }
    r__ = *e;

/*     IF THE LAST STEP EXPANDED THE INTERVAL OR THE MINIMUM HAS */
/*     ALREADY BEEN BRACKETED SET E AS THE LAST STEP TAKEN */

    if (*d__ != *b1 || *rr > (float)0.) {
	*e = *d__;
    }

/*     CONSTRUCT AN ARTIFICIAL BOUND ON THE ESTIMATED STEPLENGTH. */

L240:
    a1 = *a;
    *b1 = *b;
    if (*xmin != *a) {
	goto L260;
    }
    *d__ = xm;
    goto L330;
L260:
    if (*rr > (float)0.) {
	goto L280;
    }
    *d__ = *a * (float)-4.;
    if (*d__ >= *scxbd) {
	*d__ = *scxbd;
    }
    goto L320;

/*     DETERMINE INTERVAL OF LENGTH D2 IN WHICH TO SET */
/*     ARTIFICIAL BOUND. */

L280:
    d1 = *a;
    d2 = *b;
    if (dabs(d2) > *tol && (*xw <= (float)0. || dabs(d1) <= *tol)) {
	goto L300;
    }
    *u = d1;
    d1 = d2;
    d2 = *u;
L300:
    *u = -d1 / d2;
    if (*u >= (float)1.) {
	*d__ = d2 * (float)5. * ((float)1. / *u + (float).1) / (float)11.;
    }
    if (*u < (float)1.) {
	*d__ = d2 * (float).5 * sqrt(*u);
    }

/*     IF THE MINIMUM IS BRACKETED BY XV AND XW THE STEP MUST LIE */
/*     WITHIN (A, B). */

L320:
    if (*xw < (float)0. && *xv > (float)0. || *xw > (float)0. && *xv < (float)
	    0.) {
	goto L330;
    }

/*     IF THE MINIMUM IS NOT BRACKETED BY XV AND XW THE STEP MUST LIE */
/*     WITHIN (A1, B1). */

    if (*d__ <= (float)0.) {
	a1 = *d__;
    }
    if (*d__ > (float)0.) {
	*b1 = *d__;
    }

/*     REJECT THE STEP OBTAINED BY INTERPOLATION IF IT LIES OUTSIDE */
/*     THE REQUIRED INTERVAL OR IT IS GREATER THAN HALF THAT */
/*     OBTAINED DURING THE LAST-BUT-ONE ITERATION. */

L330:
    if (dabs(s) >= (r__1 = q * (float).5 * r__, dabs(r__1)) || s <= q * a1 || 
	    s >= q * *b1) {
	goto L340;
    }

/*     A PARABOLIC INTERPOLATION STEP. */

    *d__ = s / q;

/*     F MUST NOT BE EVALUATED TOO CLOSE TO A OR B. */

    if (*d__ - *a >= t2 && *b - *d__ >= t2) {
	goto L360;
    }
    *d__ = *tol;
    if (xm <= (float)0.) {
	*d__ = -(*tol);
    }
    goto L360;

/*     A NON-INTERPOLATION STEP. */

L340:
    *e = *b;
    if (xm <= (float)0.) {
	*e = *a;
    }
L360:
    *iloc = 3;

/*     CHECK THAT THE NEW STEP LENGTH WILL NOT BE GREATER THAN */
/*     XLAMDA. */

L380:
    if (*d__ < *scxbd) {
	goto L400;
    }

/*     REPLACE THE STEP LENGTH BY THE SCALED BOUND (SO AS TO COMPUTE */
/*     THE NEW POINT ON THE BOUNDARY. */

    *d__ = *scxbd;

/*     MOVE SCXBD TO THE LEFT SO THAT NEWXBND + TOL(NEWXBND) = XBND. */

    *scxbd = (*scxbd - *tol) / (*eps + (float)1.);
L400:
    *u = *d__;
    if (dabs(*d__) < *tol && *d__ <= (float)0.) {
	*u = -(*tol);
    }
    if (dabs(*d__) < *tol && *d__ > (float)0.) {
	*u = *tol;
    }
    *itest = 1;
    return 0;

/*     THE FIRST CONVERGENCE CRITERION HAS BEEN SATISFIED. NOW CHECK */
/*     THAT THE FUNCTION VALUE HAS BEEN REDUCED SUFFICIENTLY. THE */
/*     VARIABLE RR IS NOW USED TO REDUCE THE STEP LENGTH. */

L420:
    *d__ = *rr;
    *rr = *xmin;
    *ss = (float).5;
    *fu = *fmin;
    if (*xmin == (float)0.) {
	*xmin = *t;
    }

/*     IF XMIN LIES WITHIN TOL OF THE BOUNDARY AND THE MINIMUM HAS */
/*     BEEN BRACKETED, THEN RECOMPUTE THE POINT ON THE BOUNDARY. */

L440:
    if ((r__1 = *xmin - *xlamda, dabs(r__1)) >= *tol || *xmin == *t) {
	goto L460;
    }
    if (*scxbd < (float)0. && *xw < (float)0. && *xv < (float)0.) {
	*xmin = *xlamda;
    }
    if (*d__ < (float)0.) {
	goto L460;
    }
    *u = (float)0.;
    *iloc = 4;
    *itest = 1;
    return 0;

/*     CHECK THAT THE NEW POINT SATISFIES SAFEGUARD CONDITIONS. */
/*     IF NECESSARY ATTEMPT TO FIND A SUFFICIENTLY LOWER POINT */
/*     BY SUCCESSIVELY DECREASING THE STEPLENGTH. */

L460:
    if (*xmin + *b > *sftbnd) {
	goto L480;
    }
    *itest = 4;

/*     ITEST = 4 IMPLIES THAT THE REQUIRED STEP LENGTH IS SMALLER */
/*     THAN SFTBND. */

    return 0;
L480:
    if (*oldf - *fu <= *gtest1 * *xmin) {
	goto L500;
    }
    *fmin = *fu;
    *itest = 0;

/*     THE ALGORITHM HAS SUCCESSFULLY FOUND A SUFFICIENTLY LOWER */
/*     POINT. */

    return 0;
L500:
    if (*xmin != *t) {
	goto L520;
    }
    *itest = 3;

/*     DESPITE REPEATED REDUCTIONS IN THE STEP SIZE, A LOWER POINT */
/*     COULD NOT BE FOUND. */

    return 0;

/*     A SUFFICIENT REDUCTION IN THE FUNCTION VALUE HAS NOT YET BEEN */
/*     FOUND, TRY A FURTHER REDUCTION IN THE STEP LENGTH. */

L520:
    *xmin = *rr * *ss;
    *ss *= *ss;
    if (*xmin < *t) {
	*xmin = *t;
    }
    *itest = 1;
    *u = (float)0.;
    *iloc = 5;
    return 0;

/*     END OF ABZE04 (NEWPTQ) */

} /* abze04_ */




#ifdef __cplusplus
	}
#endif
