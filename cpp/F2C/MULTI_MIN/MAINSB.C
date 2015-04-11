/* mainsb.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* *********************************************************************** */
/* EASY TO USE, BOUNDS ON VARIABLES */
/* *********************************************************************** */
/* MAIN PROGRAM TO MINIMIZE A FUNCTION (REPRESENTED BY THE ROUTINE SFUN) */
/* SUBJECT TO BOUNDS ON THE VARIABLES X */

/* Main program */ MAIN__()
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    extern /* Subroutine */ int tnbc_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, U_fp, 
	    doublereal *, doublereal *, integer *);
    extern /* Subroutine */ int sfun_(...);
    doublereal f, g[50];
    integer i__, n;
    doublereal w[700], x[50];
    integer lw;
    doublereal up[50];
    integer ierror, ipivot[50];
    doublereal low[50];


/* DEFINE SUBROUTINE PARAMETERS */
/* N   - NUMBER OF VARIABLES */
/* X   - INITIAL ESTIMATE OF THE SOLUTION */
/* LOW - LOWER BOUNDS */
/* UP  - UPPER BOUNDS */
/* F   - ROUGH ESTIMATE OF FUNCTION VALUE AT SOLUTION */
/* LW  - DECLARED LENGTH OF THE ARRAY W */

    n = 10;
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	low[i__ - 1] = 0.;
	up[i__ - 1] = 6.;
	x[i__ - 1] = i__ / (real) (n + 1);
/* L10: */
    }
    f = 1.;
    lw = 700;
    tnbc_(&ierror, &n, x, &f, g, w, &lw, (U_fp)sfun_, low, up, ipivot);
    s_stop("", (ftnlen)0);
    return 0;
} /* MAIN__ */



/* Subroutine */ int sfun_(integer *n, doublereal *x, doublereal *f, 
	doublereal *g)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer i__;
    doublereal t;


/* ROUTINE TO EVALUATE FUNCTION (F) AND GRADIENT (G) OF THE OBJECTIVE */
/* FUNCTION AT THE POINT X */

    /* Parameter adjustments */
    --g;
    --x;

    /* Function Body */
    *f = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = x[i__] - i__;
	*f += t * t;
	g[i__] = t * 2.;
/* L10: */
    }
    return 0;
} /* sfun_ */

#ifdef __cplusplus
	}
#endif
