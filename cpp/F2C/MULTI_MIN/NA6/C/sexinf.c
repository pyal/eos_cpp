/* sexinf.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Subroutine */ int sexinf_(integer *numfun, real *gamma, real *c__, integer 
	*key, integer *n, real *t, real *ct, integer *update, integer *emax, 
	real *u, real *e, real *result, real *abserr, real *exterr, real *
	beta, real *maxeer)
{
    /* System generated locals */
    integer u_dim1, u_offset, e_dim1, e_offset, beta_dim1, beta_offset, i__1, 
	    i__2;
    real r__1, r__2;

    /* Local variables */
    real base1, base2, save1, save2;
    integer i__, j;
    real p, q;

/* ***BEGIN PROLOGUE SEXINF */
/* ***KEYWORDS Quasi-linear extrapolation, infinite integration, */
/*            oscillating functions, error estimation. */
/* ***PURPOSE  To compute better estimates to a vector of approximations */
/*            to integrals of oscillating functions over an infinite */
/*            interval and to provide new and updated error estimates. */

/* ***LAST MODIFICATION 94-03-11 */

/* ***DESCRIPTION The routine uses linear extrapolation to compute better */
/*            approximations to each component in a vector of */
/*            integrals. All components are assumed to be integrals of */
/*            oscillating functions which asymptotically have the same */
/*            behavior at infinity. */
/*            A series approach is used, assuming */
/*            that the terms are given with estimates of the error in */
/*            each term. New error estimates are computed too. The */
/*            routine have two options: EITHER a new extrapolation term */
/*            is provided and we take a new extrapolation step, */
/*            OR we update one previously computed term in the series and */
/*            therefore have to update the extrapolation tableau. */

/*   ON ENTRY */

/*     NUMFUN Integer. */
/*            The number of components of the integral. */
/*     GAMMA  Real. */
/*            Asymptotic decay of the functions; 1/x**(GAMMA), GAMMA>0. */
/*     C      Real. */
/*            Reference point: User specified subdivision point/period. */
/*     KEY    Integer */
/*            Choice of extrapolation method: */
/*            KEY = 0 : Euler's method. */
/*            KEY = 1 : Modified Euler. */
/*            KEY = 2 : Overholt's polynomial order 2-method. */
/*                      This last method is the only one that needs */
/*                      the value of GAMMA. */
/*     N      Integer */
/*            The number of U-terms in the series: 0,1,2,...,N. */
/*            Makes it possible to perform N extrapolation steps. */
/*     T      Real array of dimension (NUMFUN,0:EMAX) */
/*            Contains the last row in the extrapolation tableau for */
/*            each function in the vector. */
/*     CT     Real array of dimension NUMFUN. */
/*            Contains the element to the left of the diagonal element */
/*            in the extrapolation tableau. */
/*     UPDATE Integer */
/*            If < 0 then this is a new extrapolation step. */
/*            If >= 1 then this is  a step where we have to correct the */
/*            existing tableau. The value of UPDATE gives the index to */
/*            the U-element that has been modified. */
/*     EMAX   Integer */
/*            The maximum allowed number of extrapolation steps. */
/*     U      Real array of dimension (NUMFUN,0:N) */

/*     E      Real array of dimension (NUMFUN,0:N) */
/*            The estimated errors of all U-terms in the series. */
/*   ON RETURN */

/*     T      Real array of dimension (NUMFUN:0:EMAX) */
/*            Contains the diagonal element in the extrapolation tableau */
/*            for each function in the vector after the extrapolation. */
/*     CT     Real array of dimension NUMFUN. */
/*            Contains the element to the left of the diagonal element */
/*            in the extrapolation tableau. */
/*     RESULT Real array of dimension NUMFUN */
/*            Contains the new approximations for the NUMFUN components */
/*            of the integral. */
/*     ABSERR Real array of dimension NUMFUN. */
/*            Returns the global errors for all components. */
/*            This includes both the pure extrapolation */
/*            error and the effect of not knowing the U-terms exactly. */
/*     EXTERR Real array of dimension NUMFUN. */
/*            These errors are the pure extrapolation errors. */
/*     BETA   Real Array of dimension (EMAX +1)*(EMAX +1) */
/*            A table of coefficients to be used in the extrapolation */
/*            and the error estimation. */
/*     MAXEER Real. */
/*            The maximum extrapolation error. */
/* ***REFERENCES */

/*   T.O.Espelid and K.J.Overholt, DQAINF: An Algorithm for Automatic */
/*   Integration of Infinite Oscillating Tails, */
/*   submitted for publication 1993. */

/*   K.J.Overholt: The P-algorithms for extrapolation, */
/*   Department of Informatics, Report No. 36, 1989. */

/* ***ROUTINES CALLED NONE */
/* ***END PROLOGUE SEXINF */

/*   Global variables. */


/*   Local variables. */


/*  CONST heuristic constant used in the error estimation. */

/* ***FIRST EXECUTABLE STATEMENT SEXINF */

/*   Initialize the beta-factors. */

    /* Parameter adjustments */
    --exterr;
    --abserr;
    --result;
    --ct;
    --t;
    e_dim1 = *numfun;
    e_offset = 1 + e_dim1 * 0;
    e -= e_offset;
    u_dim1 = *numfun;
    u_offset = 1 + u_dim1 * 0;
    u -= u_offset;
    beta_dim1 = *emax - 0 + 1;
    beta_offset = 0 + beta_dim1 * 0;
    beta -= beta_offset;

    /* Function Body */
    beta[0] = (float)1.;
    if (*key == 2) {
	base1 = *gamma;
/* Computing MAX */
	r__1 = *gamma - 1, r__2 = *c__ * 4;
	base2 = dmax(r__1,r__2);
	p = (float)2.;
    } else {
	base1 = (float)0.;
/* Computing MAX */
	r__1 = (float)0., r__2 = *c__ * 4;
	base2 = dmax(r__1,r__2);
	p = (float)1.;
    }

/*   Compute the new extrapolation coefficients. */

    if (*update < 0) {
	beta[*n * beta_dim1] = (float)1.;
	beta[*n] = (float)1.;
	i__1 = *n - 1;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    save1 = (float)1.;
	    i__2 = *n - 1;
	    for (j = *n - i__; j <= i__2; ++j) {
		if (*key == 0) {
		    q = (float).5;
		} else {
		    q = ((-base1 - p * j) / ((*n << 1) + base2) + 1) / (float)
			    2.;
		}
		save2 = save1 - (save1 - beta[i__ + j * beta_dim1]) * q;
		beta[i__ + j * beta_dim1] = save1;
/* L10: */
		save1 = save2;
	    }
	    beta[i__ + *n * beta_dim1] = save1;
/* L20: */
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    if (*key == 0) {
		q = (float).5;
	    } else {
		q = ((-base1 - p * (j - 1)) / ((*n << 1) + base2) + 1) / (
			float)2.;
	    }
/* L30: */
	    beta[*n + j * beta_dim1] = (1 - q) * beta[*n + (j - 1) * 
		    beta_dim1];
	}
    }

/*   Extrapolate; use the extrapolation coefficients. */

    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	t[j] = (float)0.;
	ct[j] = (float)0.;
	for (i__ = *n; i__ >= 0; --i__) {
	    ct[j] += beta[i__ + (*n - 1) * beta_dim1] * u[j + i__ * u_dim1];
/* L40: */
	    t[j] += beta[i__ + *n * beta_dim1] * u[j + i__ * u_dim1];
	}
    }

/*   Then compute the error estimates. */
/*   First the extrapolation error and then the U-effect. */
/*   The maximum extrapolation error is computed too. */

    *maxeer = (float)0.;
    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	exterr[j] = (r__1 = t[j] - ct[j], dabs(r__1)) * (float)1. / q;
/* Computing MAX */
	r__1 = exterr[j];
	*maxeer = dmax(r__1,*maxeer);

/*   Note: The last U-errors are effected by the extrapolation-process */
/*   Accumulate the total error in exterr. */

	i__2 = *n;
	for (i__ = 0; i__ <= i__2; ++i__) {
/* L50: */
	    exterr[j] += beta[i__ + *n * beta_dim1] * e[j + i__ * e_dim1];
	}
/* L60: */
    }

/*   Define the results: update the results only if the error is improved */

    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	if (exterr[j] <= abserr[j] || abserr[j] == (float)0.) {
	    result[j] = t[j];
	    abserr[j] = exterr[j];
	}
/* L70: */
    }
    return 0;

/* ***END SEXINF */

} /* sexinf_ */

#ifdef __cplusplus
	}
#endif
