/* mainfc.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"
#include <math.h>
/* Common Block Declarations */

union {
    struct {
	doublereal accrcy;
    } _1;
    struct {
	doublereal h__;
    } _2;
} findif_;

#define findif_1 (findif_._1)
#define findif_2 (findif_._2)

/* Table of constant values */

static integer c__1 = 1;

/* *********************************************************************** */
/* FINITE-DIFFERENCING, CUSTOMIZED, NO BOUNDS */
/* *********************************************************************** */
/* MAIN PROGRAM TO MINIMIZE A FUNCTION (REPRESENTED BY THE ROUTINE SFUN) */
/* OF N VARIABLES X - CUSTOMIZED VERSION, WITH FINITE DIFFERENCING */

int/* Main program */ MAIN__()
{
    /* Format strings */
    static char fmt_800[] = "(//,\002 ERROR CODE =\002,i3,/)";
    static char fmt_810[] = "(10x,\002CURRENT SOLUTION IS \002,/14x,\002I\
\002,11x,\002X(I)\002)";
    static char fmt_820[] = "(10x,i5,2x,1pd22.15)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
//    double sqrt(doublereal);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe();
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    extern /* Subroutine */ int lmqn_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, U_fp, 
	    integer *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    extern /* Subroutine */ int sfun_(integer *, doublereal *, doublereal *, doublereal *);
    doublereal xtol, f, g[50];
    integer i__, n;
    doublereal faccy, w[700], x[50];
    integer maxit, lw, maxfun, ierror, msglvl;
    doublereal stepmx, eta;

    /* Fortran I/O blocks */
    static cilist io___16 = { 0, 6, 0, fmt_800, 0 };
    static cilist io___17 = { 0, 6, 0, fmt_810, 0 };
    static cilist io___18 = { 0, 6, 0, fmt_820, 0 };



/* SET UP FUNCTION AND VARIABLE INFORMATION */
/* N -  NUMBER OF VARIABLES */
/* X -  INITIAL ESTIMATE OF THE SOLUTION */
/* F -  ROUGH ESTIMATE OF FUNCTION VALUE AT SOLUTION */
/* LW - DECLARED LENGTH OF THE ARRAY W */

    n = 10;
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	x[i__ - 1] = i__ / (real) (n + 1);
/* L10: */
    }
    f = 1.;
    lw = 700;

/* SET UP CUSTOMIZING PARAMETERS */
/* ETA     - SEVERITY OF THE LINESEARCH */
/* MAXFUN  - MAXIMUM ALLOWABLE NUMBER OF FUNCTION EVALUATIONS */
/* XTOL    - DESIRED ACCURACY FOR THE SOLUTION X* */
/* STEPMX  - MAXIMUM ALLOWABLE STEP IN THE LINESEARCH */
/* FACCY   - ACCURACY OF COMPUTED FUNCTION VALUE */
/* ACCRCY  - ACCURACY OF COMPUTED FUNCTION AND GRADIENT VALUES */
/*           (VALUES ADJUSTED BECAUSE OF FINITE DIFFERENCING OF GRADIENT) */
/* MSGLVL  - DETERMINES QUANTITY OF PRINTED OUTPUT */
/*           0 = NONE, 1 = ONE LINE PER MAJOR ITERATION. */
/* MAXIT   - MAXIMUM NUMBER OF INNER ITERATIONS PER STEP */

    maxit = n / 2;
    maxfun = n * 150;
    eta = .25;
    stepmx = 10.;
    faccy = 1e-15;
    findif_1.accrcy = sqrt(faccy);
    xtol = sqrt(findif_1.accrcy);
    msglvl = 1;

/* MINIMIZE THE FUNCTION */

    lmqn_(&ierror, &n, x, &f, g, w, &lw, (U_fp)sfun_, &msglvl, &maxit, &
	    maxfun, &eta, &stepmx, &findif_1.accrcy, &xtol);

/* PRINT THE RESULTS */

    if (ierror != 0) {
	s_wsfe(&io___16);
	do_fio(&c__1, (char *)&ierror, (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (msglvl >= 1) {
	s_wsfe(&io___17);
	e_wsfe();
    }
    if (msglvl >= 1) {
	s_wsfe(&io___18);
	i__1 = n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&x[i__ - 1], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    s_stop("", (ftnlen)0);
    return 0;
} /* MAIN__ */



/* Subroutine */ int sfun_(integer *n, doublereal *x, doublereal *f, 
	doublereal *g)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    extern /* Subroutine */ int getf_(integer *, doublereal *, doublereal *);
    doublereal hinv;
    integer i__;
    doublereal fh, xh;


/* ROUTINE TO COMPUTE FUNCTION (F) AND GRADIENT (G) OF THE OBJECTIVE */
/* FUNCTION AT THE POINT X */
/* GRADIENT OBTAINED VIA FINITE-DIFFERENCING */
/* FUNCTION OBTAINED FROM SUBROUTINE GETF */

    /* Parameter adjustments */
    --g;
    --x;

    /* Function Body */
    hinv = 1. / findif_2.h__;
    getf_(n, &x[1], f);
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	xh = x[i__];
	x[i__] += findif_2.h__;
	getf_(n, &x[1], &fh);
	g[i__] = (fh - *f) * hinv;
	x[i__] = xh;
/* L10: */
    }
    return 0;
} /* sfun_ */



/* Subroutine */ int getf_(integer *n, doublereal *x, doublereal *f)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer i__;
    doublereal t;


/* ROUTINE TO EVALUATE FUNCTION (F) */

    /* Parameter adjustments */
    --x;

    /* Function Body */
    *f = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = x[i__] - i__;
	*f += t * t;
/* L10: */
    }
    return 0;
} /* getf_ */

#ifdef __cplusplus
	}
#endif
