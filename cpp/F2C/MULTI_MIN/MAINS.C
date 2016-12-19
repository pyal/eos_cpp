/* mains.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/
/*
#include <stdio.h>
void main()
{
long double one=1,epsil=0.5;
 int i=sizeof(long double);
 int i_sh=sizeof(double);
while ( (long double)(one+epsil)!=(long double)(one) ) {epsil/=2;}
//while ( one+epsil!=one ) {epsil/=2;}
//i
printf("Decimal %f",epsil);
//std::cout<<"eps="<<eps*2;
 }
*/
#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* *********************************************************************** */
/* EASY TO USE, NO BOUNDS */
/* *********************************************************************** */
/* MAIN PROGRAM TO MINIMIZE A FUNCTION (REPRESENTED BY THE ROUTINE SFUN) */
/* OF N VARIABLES X */

// Main program  MAIN__()
int main()
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    extern /* Subroutine */ int sfun_();
//    extern /* Subroutine */ int sfun_(...);
    doublereal f, g[50];
    integer i__, n;
    doublereal w[700], x[50];
    extern /* Subroutine */ int tn_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, U_fp);
    integer lw, ierror;


/* DEFINE SUBROUTINE PARAMETERS */
/* N  - NUMBER OF VARIABLES */
/* X  - INITIAL ESTIMATE OF THE SOLUTION */
/* F  - ROUGH ESTIMATE OF FUNCTION VALUE AT SOLUTION */
/* LW - DECLARED LENGTH OF THE ARRAY W */

    n = 10;
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	x[i__ - 1] = i__ / (real) (n + 1);
/* L10: */
    }
    f = 1.;
    lw = 700;
    tn_(&ierror, &n, x, &f, g, w, &lw, (U_fp)sfun_);
//    s_stop("", (ftnlen)0);
// STOP - what is it for?
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
