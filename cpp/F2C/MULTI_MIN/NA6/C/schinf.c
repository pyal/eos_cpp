/* schinf.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Subroutine */ int schinf_(integer *numfun, U_fp funsub, real *period, real 
	*gamma, real *a, real *b, integer *minpts, integer *maxpts, real *
	epsabs, real *epsrel, integer *wrksub, integer *nw, integer *emax, 
	integer *key, integer *restar, integer *neval, integer *ifail)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer limit;
    real width;
    extern /* Subroutine */ int sgainf_(integer *, U_fp, real *, real *, real 
	    *, integer *, integer *);

/* ***BEGIN PROLOGUE SCHINF */
/* ***REFER TO SQAINF */
/* ***PURPOSE  SCHINF checks the validity of the input parameters to */
/*            SQAINF. */

/* ***LAST MODIFICATION 93-05-05 */

/* ***DESCRIPTION */
/*   SCHINF computes IFAIL as a function of the input */
/*   parameters to SQAINF, and checks the validity of the input */
/*   parameters to SQAINF. */

/*   ON ENTRY */

/*     NUMFUN Integer. */
/*            Number of components of the integral. */
/*     FUNSUB Externally declared subroutine for computing */
/*            all components of the integrand at the given */
/*            evaluation point. */
/*            It must have parameters (X,NUMFUN,FUNVLS) */
/*            Input parameters: */
/*              X      Real array of dimension 21 defining the */
/*                     x-coordinates of the evaluation points. */
/*              NUMFUN Integer that defines the number of */
/*                     components of I. */
/*              NP     Integer that gives the number of evaluation points */
/*            Output parameter: */
/*              FUNVLS Real array of dimension (21,NUMFUN) that defines */
/*                     the function values in the 21 evaluation points */
/*                     for the NUMFUN components of the integrand. */
/*     PERIOD Real. */
/*            All functions are assumed to have the same asymptotic */
/*            period. */
/*     GAMMA  Real. */
/*            All functions are assumed to decay as c/x**GAMMA, */
/*            when x >> 1 and we go in steps of length PERIOD, */
/*            (GAMMA > 0). */
/*     A      Real array of dimension WRKSUB. */
/*     B      Real array of dimension WRKSUB. */
/*            A(1) and B(1) are the left endpoint and right endpoint */
/*            of the first interval,  A(1) <= B(1). B(1) is chosen by the */
/*            user and it is assumed that the integrand oscillates for */
/*            X >= B(1). Asymptotic period is PERIOD. Thus oscillations */
/*            are expected to be observed for points of distance PERIOD/2 */
/*     MINPTS Integer. */
/*            Minimum number of function evaluations. */
/*     MAXPTS Integer. */
/*            Maximum number of function evaluations. */
/*            The number of function evaluations over each sub-interval */
/*            is NUM = 21. */
/*     EPSABS Real. */
/*            Requested absolute error. */
/*     EPSREL Real. */
/*            Requested relative error. */
/*     WRKSUB Integer. */
/*            Defines the length of the arrays A and B. (And thus the */
/*            maximum number of subregions allowed.) */
/*     NW     Integer. */
/*            Defines the length of the working array WORK. */
/*     EMAX   Integer. */
/*            The maximum number of extrapolation steps. (At least one */
/*            step!) */
/*     KEY    Integer. */
/*            Choice of extrapolation method: */
/*            KEY = 0 : Euler's method. */
/*            KEY = 1 : Modified Euler. */
/*            KEY = 2 : Overholt's polynomial order 2-method. */
/*                      This last method is the only one that needs */
/*                      GAMMA. */
/*     RESTAR Integer. */
/*            If RESTAR = 0, this is the first attempt to compute */
/*            the integral. */
/*            If RESTAR = 1, */
/*            then we restart a previous attempt. */
/*            RESTAR is allowed to take these two values only. */
/*   ON RETURN */

/*     NEVAL  Integer. */
/*            The number of function evaluations. */
/*     IFAIL  Integer. */
/*            IFAIL = 0 for normal exit. */
/*            IFAIL =  2 if NUMFUN is less than 1. */
/*            IFAIL =  3 if B(1) < A(1). */
/*            IFAIL =  4 if unlegal PERIOD. */
/*            IFAIL =  5 if MAXPTS is less than MINPTS or MINPTS < 21. */
/*            IFAIL =  6 if EPSABS <= 0 and EPSREL <= 0. */
/*            IFAIL =  7 if WRKSUB is too small. */
/*            IFAIL =  8 if unlegal value of EMAX. */
/*            IFAIL =  9 if unlegal RESTAR. */
/*            IFAIL = 10 if unlegal value of KEY. */
/*            IFAIL = 11 if NW is less than LIMIT (See the code SCHINF). */
/*            IFAIL = 12 if either PERIOD is wrong or B(1) is too small. */
/*            IFAIL = 13 if unable to estimate GAMMA (In case KEY=2 only) */

/* ***ROUTINES CALLED SGAINF */
/* ***END PROLOGUE SCHINF */

/*   Global variables. */


/*   Local variables. */


/* ***FIRST EXECUTABLE STATEMENT SCHINF */

    /* Parameter adjustments */
    --b;
    --a;

    /* Function Body */
    *ifail = 0;

/*   Check on legal NUMFUN. */

    if (*numfun < 1) {
	*ifail = 2;
	return 0;
    }

/*   Check on legal length of the first interval of integration. */

    width = b[1] - a[1];
    if (width < (float)0.) {
	*ifail = 3;
	return 0;
    }

/*   Check on legal sign of PERIOD. */

    if (*period <= (float)0.) {
	*ifail = 4;
	return 0;
    }

/*   Check on MAXPTS >= MINPTS. */

    if (*maxpts < *minpts || *minpts < 21) {
	*ifail = 5;
	return 0;
    }

/*   Check on legal accuracy requests. */

    if (*epsabs <= (float)0. && *epsrel <= (float)0.) {
	*ifail = 6;
	return 0;
    }

/*   Check on legal WRKSUB. */

    if (*wrksub * 21 < *maxpts) {
	*ifail = 7;
	return 0;
    }

/*   Check on legal value of EMAX. */

    if (*emax < 1) {
	*ifail = 8;
	return 0;
    }

/*    Check on legal RESTAR. */

    if (*restar != 0 && *restar != 1) {
	*ifail = 9;
	return 0;
    } else if (*restar == 0) {
	*neval = 0;
    }

/*    Check on legal KEY. */

    if (*key != 0 && *key != 1 && *key != 2) {
	*ifail = 10;
	return 0;
    }

/*   Check on big enough NW. */

/* Computing 2nd power */
    i__1 = *emax + 1;
    limit = *numfun * 5 + 1 + (*wrksub << 2) * *numfun + *wrksub * 3 + i__1 * 
	    i__1 + *numfun * 38;
    if (*nw < limit) {
	*ifail = 11;
	return 0;
    }

/*   If the user gives GAMMA <= 0 and KEY = 2 then SQAINF will try to */
/*   estimate GAMMA. */
/*   If the subroutine estimates GAMMA successfully within its error */
/*   bound, IFAIL will remain 0 while GAMMA is given the new value. */
/*   The routine SGAINF may detect errors: */

/*   IFAIL = 12 indicates that either */
/*   B is too small or the PERIOD is wrong, or maybe the function does no */
/*   have the desired properties. (NOTE: The code checks ONLY the first */
/*   function in the vector, assuming that if this is correct then the */
/*   other functions share its properties.) */

/*   IFAIL = 13 : No errors where detected, but the code was unable to */
/*   compute GAMMA sufficiently accurate. Remedy: use KEY = 1 instead. */

    if (*gamma <= (float)0. && *key == 2) {
	sgainf_(numfun, (U_fp)funsub, &b[1], period, gamma, neval, ifail);
	return 0;
    }

    return 0;

/* ***END SCHINF */

} /* schinf_ */

#ifdef __cplusplus
	}
#endif
