/* sgainf.f -- translated by f2c (version 20000118).
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
static integer c__2 = 2;

/* Subroutine */ int sgainf_(integer *numfun, S_fp f, real *b, real *period, 
	real *gamma, integer *neval, integer *ifail)
{
    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
    double log(doublereal);
    integer pow_ii(integer *, integer *);

    /* Local variables */
    real fold, fnew, sign, last, xold, xnew, a[16], h__;
    integer i__, m;
    real p, x;
    extern doublereal r1mach_(integer *);
    real da, df, fi[16], fr, xb, epmach, fx, xr, factor[16], fxp;


/* ***BEGIN PROLOGUE SGAINF */
/* ***KEYWORDS Linear extrapolation, */
/*            oscillating functions, asymptotic behavior. */
/* ***PURPOSE  To compute an estimate of the exponent GAMMA which governs */
/*            the asymptotic behavior of an oscillating function: */
/*            F(X+PERIOD*K) appr. C(X)/(X+PERIOD*K)**GAMMA, for all X */
/*            and all integers K sufficiently great. */
/*            The function is assumed to */
/*            have asymptotical oscillating behavior and */
/*            F(X+PERIOD*(K+1/2)) approx. - F(X+PERIOD*K). */

/* ***LAST MODIFICATION 93-01-31 */

/* ***REFER TO SQAINF */

/* ***DESCRIPTION Computation of the exponent GAMMA > 0 which governs */
/*   the asymptotic behavior of the functions F. All functions in the */
/*   vector F are assumed to be asymptotically periodic with the same */
/*   PERIOD and the same asymptotical behavior. Furthermore we assume */
/*   that for all functions in the vector we have */
/*   F(X+PERIOD*K) approx. C(X)/(X+PERIOD*K)**GAMMA, for all values of X */
/*   and K, such that (X+PERIOD*K) >> B. C may be zero for some values of */
/*   X. Assuming that C is non-zero then we have that */
/*   F(X+2*PERIOD*K)/F(X+PERIOD*K) approx. (1 + D)**GAMMA, with */
/*   D = PERIOD*K/(X+PERIOD*K). */

/*   ON ENTRY */

/*     NUMFUN Integer. */
/*            The number of components of the integral. */
/*     F      Externally declared subroutine for computing */
/*            all components of the integrand at all NP */
/*            evaluation points. */
/*            It must have parameters (X,NUMFUN,NP,FUNVLS) */
/*            Input parameters: */
/*              X      The NP x-coordinates of the evaluation points. */
/*              NUMFUN Integer that defines the number of */
/*                     components of I. */
/*              NP     Integer that gives the number of evaluation points */
/*            Output parameter: */
/*              FUNVLS Real array of dimension (NUMFUN,NP) */
/*                     that defines NUMFUN components of the integrand fo */
/*                     each evaluation point. */

/*     B      Real. */
/*            Asymptotic behavior assumed satisfied for all X >= B and */
/*            for all the integrands. */
/*     PERIOD Real. */
/*            All functions are assumed to be asymptotically periodic, */
/*            and with the same period. */

/*  ON RETURN */

/*     GAMMA  Real */
/*            Estimated value of the rate of decay of the functions */
/*            when X tends to +infinity. */
/*     NEVAL  Integer */
/*            The number of function evaluations. */
/*     IFAIL  Integer. */
/*            IFAIL = 0 for normal exit. */
/*            IFAIL = 12 if either PERIOD is wrong or B(1) is too small. */
/*            IFAIL = 13 if unable to estimate GAMMA (In case KEY=2 only) */

/* ***ROUTINES CALLED F,R1MACH */
/* ***END PROLOGUE SGAINF */

/*   Global variables. */


/*   Local variables. */


/* ***FIRST EXECUTABLE STATEMENT SGAINF */

    epmach = r1mach_(&c__4);

/*   First: compute a starting point X >= B where the function value F(X) */
/*   is reasonably large in absolute value. Remark: We simply want to */
/*   avoid that F(X) becomes approximately 0. */

    xold = *b;
    (*f)(&xold, &c__1, &c__1, &fold);
    p = *period / 4;
    xnew = xold;
    for (i__ = 1; i__ <= 3; ++i__) {
	xnew += p;
	(*f)(&xnew, &c__1, &c__1, &fnew);
	if (dabs(fold) < dabs(fnew)) {
	    xold = xnew;
	    if (fold * fnew > (float)0.) {
		p = -p;
	    }
	    fold = fnew;
	} else {
	    if (fold * fnew > (float)0.) {
		xnew = xold;
	    }
	}
	p /= 2;
/* L10: */
    }
    x = xold;
    fx = fold;

/*   Check that the function values do change sign */
/*   and that the absolute value of the function is decreasing */
/*   when evaluated at points which differ by PERIOD*integer/2. */

    r__1 = x + *period / 2;
    (*f)(&r__1, &c__1, &c__1, &fxp);
    *neval = 5;
    last = fxp;
    sign = fx;
    fi[0] = x;
    if (sign * fxp < (float)0. && dabs(fx) > dabs(fxp)) {
	fr = -fxp / fx;
	xr = x / (x + *period / 2);
	a[0] = log(fr) / log(xr);
	factor[0] = a[0] * (1 / (r__1 = fr * log(fr), dabs(r__1)) + 1 / (r__2 
		= xr * log(xr), dabs(r__2))) * epmach;
    } else {
	*ifail = 12;
	return 0;
    }

/*   Compute a sequence of estimates to GAMMA and extrapolate. */

/*   FACTOR is meant to keep track of the increase in the error due to */
/*   the extrapolation. We assume that PERIOD is known correctly to full */
/*   precision. If X is an exact local extremum of F(X) then it is */
/*   not important to know PERIOD with such a high precision. */
/*   However, since X is just a point where ABS(F(X)) is not too small */
/*   the quality of PERIOD becomes essential when we estimate GAMMA. */

    h__ = *period / 2;
    xb = x;
    for (i__ = 1; i__ <= 15; ++i__) {
	h__ *= 2;
	x = xb + h__;
	fi[i__] = x;
	r__1 = x + *period / 2;
	(*f)(&r__1, &c__1, &c__1, &fxp);
	(*f)(&x, &c__1, &c__1, &fx);
	*neval += 2;

/*   Check the computed function values. */

	if (sign * fx > (float)0. && sign * fxp < (float)0. && dabs(fx) > 
		dabs(fxp) && dabs(last) > dabs(fx)) {
	    last = fxp;
	    fr = -fxp / fx;
	    xr = x / (x + *period / 2);
	    a[i__] = log(fr) / log(xr);
	    factor[i__] = a[0] * (pow_ii(&c__2, &i__) / (r__1 = fr * log(fr), 
		    dabs(r__1)) + 1 / (r__2 = xr * log(xr), dabs(r__2))) * 
		    epmach;
	} else {

/*   Either PERIOD is wrong or B is too small. */

	    *ifail = 12;
	    return 0;
	}
	for (m = i__ - 1; m >= 0; --m) {
	    da = (a[m + 1] - a[m]) / ((fi[i__] - fi[m]) / fi[m]);
	    df = (factor[m + 1] + factor[m]) / ((fi[i__] - fi[m]) / fi[m]);
	    a[m] = a[m + 1] + da;
	    factor[m] = factor[m + 1] + df;
/* L20: */
	}
	if (dabs(da) <= factor[0]) {
	    *gamma = a[0];

/*   Normal return */

	    return 0;
	}
/* L30: */
    }

/*   We did not succeed in estimating GAMMA to sufficient precision. */

    *gamma = a[0];
    *ifail = 13;
    return 0;

/* ***END SGAINF */

} /* sgainf_ */

#ifdef __cplusplus
	}
#endif
