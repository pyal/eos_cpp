/* sadinf.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Table of constant values */

static integer c__4 = 4;
static integer c__2 = 2;
static integer c__1 = 1;

/* Subroutine */ int sadinf_(integer *numfun, U_fp funsub, real *period, real 
	*gamma, real *a, real *b, integer *wrksub, integer *emax, integer *
	minpts, integer *maxpts, real *epsabs, real *epsrel, integer *restar, 
	integer *lenw, integer *key, real *result, real *abserr, integer *
	neval, integer *nsub, integer *ifail, logical *done, real *values, 
	real *errors, real *greate, real *c__, real *d__, real *u, real *e, 
	real *t, real *ct, real *exterr, real *beta, real *unew, real *uerr, 
	real *work2, integer *worst, integer *list, integer *upoint)
{
    /* System generated locals */
    integer values_dim1, values_offset, errors_dim1, errors_offset, u_dim1, 
	    u_offset, e_dim1, e_offset, beta_dim1, beta_offset, i__1, i__2;
    real r__1;

    /* Local variables */
    integer flag__;
    real aold, bold;
    static real left;
    integer ndiv;
    logical more;
    static integer numu;
    integer i__, j, k;
    static real p;
    real great;
    integer index, i1, i2, i3, l1, l2, l3;
    extern doublereal r1mach_(integer *);
    static real cc;
    integer jj;
    real epmach;
    integer vacant;
    static real maxeer;
    integer update;
    extern /* Subroutine */ int sgaint_(real *, real *, integer *, U_fp, 
	    logical *, logical *, real *, real *, real *, real *, integer *, 
	    real *, real *, real *, integer *, real *, real *), sexinf_(
	    integer *, real *, real *, integer *, integer *, real *, real *, 
	    integer *, integer *, real *, real *, real *, real *, real *, 
	    real *, real *);
    integer sbrgns, pointr;
    extern /* Subroutine */ int strint_(integer *, integer *, real *, integer 
	    *, integer *);
    integer one, top;

/* ***BEGIN PROLOGUE SADINF */
/* ***KEYWORDS Quasi-linear extrapolation, infinite integration, */
/*            oscillating functions, adaptive strategy. */
/* ***PURPOSE  To compute better estimates to a vector of approximations */
/*            to integrals of oscillating functions over an infinite */
/*            interval. */

/* ***LAST MODIFICATION 94-03-10 */

/* ***REFER TO SAQINF */

/* ***DESCRIPTION Computation of integrals over an infinite interval */
/*   by combining extrapolation and adaptive strategies. */

/*   SADINF uses extrapolation on a sum of integrals over subintervals */
/*   Each subinterval has length P = PERIOD/2 and the function involved i */
/*   assumed to be asymptotically periodic with period PERIOD. */
/*   This routine will EITHER take another extrapolation step */
/*   in order to reduce the pure extrapolation error, OR */
/*   subdivide the interval in the collection with greatest estimated */
/*   error. In both cases estimate(s) of the integral(s) and the error(s) */
/*   to the new sub-interval(s) are computed; this process continues */
/*   until the error request is met or MAXPTS function evaluations have */
/*   been used. */

/*   The 21 point rule is: Gauss. */

/*   ON ENTRY */

/*     NUMFUN Integer. */
/*            The number of components of the integral. */
/*     FUNSUB Externally declared subroutine for computing */
/*            all components of the integrand at all NP */
/*            evaluation points. */
/*            It must have parameters (X,NUMFUN,FUNVLS) */
/*            Input parameters: */
/*              X      The x-coordinates of the NP evaluation points. */
/*              NUMFUN Integer that defines the number of */
/*                     components of I. */
/*              NP     Integer that gives the number of evaluation points */
/*            Output parameter: */
/*              FUNVLS Real array of dimension NUMFUN */
/*                     that defines NUMFUN components of the integrand. */

/*     PERIOD Real. */
/*            All functions are assumed to have the same asymptotic */
/*            period. */
/*     GAMMA  Real. */
/*            All functions are assumed to decay as c/x**GAMMA, */
/*            when x >> 1 and we go in steps of length PERIOD, */
/*            (GAMMA > 0). */
/*     A      Real array of dimension WRKSUB. */
/*     B      Real array of dimension WRKSUB. */
/*            A(1) is the left endpoint of integration. */
/*            B(1) >= A(1) is a user specified subdivision point. */
/*            We assume that the assumptions on which this code is */
/*            based hold for x >= B(1). We will not extrapolate based */
/*            on the function values between A(1) and B(1). */
/*     WRKSUB Integer. */
/*            Defines the length of the arrays A and B . */
/*     EMAX   Integer. */
/*            The maximum number of extrapolation steps. */
/*     MINPTS Integer. */
/*            Minimum number of function evaluations. */
/*     MAXPTS */
/*            Maximum number of function evaluations. */
/*     EPSABS Real. */
/*            Requested absolute error. */
/*     EPSREL Real. */
/*            Requested relative error. */

/*     RESTAR Integer. */
/*            If RESTAR = 0, this is the first attempt to compute */
/*            the integral. */
/*            If RESTAR = 1, */
/*            then we restart a previous attempt. */
/*            In this case the only parameters for SAQINF that may */
/*            be changed (with respect to the previous call of SAQINF) */
/*            are MINPTS, MAXPTS, EPSABS, EPSREL and RESTAR. */
/*     LENW   Integer. */
/*            Length of the workspace WORK2. */
/*     KEY    Integer */
/*            Choice of extrapolation method: */
/*            KEY = 0 : Euler's method. */
/*            KEY = 1 : Modified Euler. */
/*            KEY = 2 : Overholt's polynomial order 2-method. */
/*                      This last method is the only one that needs */
/*                      GAMMA. */

/*   ON RETURN */

/*     RESULT Real array of dimension NUMFUN. */
/*            Approximations to all components of the integral. */
/*     ABSERR Real array of dimension NUMFUN. */
/*            Estimates of absolute errors. */
/*     NEVAL  Integer. */
/*            The number of function evaluations. */
/*     NSUB   Integer. */
/*            The number of intervals in the data structure. */
/*     A      Real array of dimension WRKSUB. */
/*     B      Real array of dimension WRKSUB. */
/*            On exit A and B contains the endpoints of the intervals */
/*            used to produce the approximations to the integrals. */
/*            Warning: A(1) and B(1) may have changed due to */
/*            adaptive subdivision of the first interval. */
/*     IFAIL  Integer. */
/*            IFAIL = 0 for normal exit. */
/*            IFAIL = +1 if MAXPTS was too small for SADINF */
/*                      to obtain the required accuracy. In this case */
/*                     SADINF returns values of RESULT with estimated */
/*                     absolute errors ABSERR. */
/*            IFAIL = -1 if EMAX was too small for SADINF */
/*                      to obtain the required accuracy. In this case */
/*                     SADINF returns values of RESULT with estimated */
/*                     absolute errors ABSERR. */

/*     DONE   Logical array of dimension NUMFUN. */
/*            On exit : DONE(I)=.TRUE. if function number I has been */
/*            integrated to the specified precision, else DONE(I)=.FALSE. */

/*     VALUES Real array of dimension (NUMFUN,WRKSUB). */
/*            The estimated components of the integrals over the */
/*            sub-intervals. */
/*     ERRORS Real array of dimension (NUMFUN,WRKSUB). */
/*            The estimated errors over the sub-intervals. */
/*     GREATE Real array of dimension WRKSUB. */
/*            The greatest error in each sub-interval. */
/*     C      Real array of dimension WRKSUB. */
/*            The left sub-division points of all intervals in the heap. */
/*     D      Real array of dimension WRKSUB. */
/*            The right sub-division points of all intervals in the heap. */
/*     U      Real array of dimension (NUMFUN,WRKSUB) */
/*            contains the terms in series. */
/*     E      Real array of dimension (NUMFUN,WRKSUB) */
/*            contains the estimated errors in each U-term. */
/*     T      Real array of dimension NUMFUN. */
/*            Dummy parameter. */
/*     CT     Real array of dimension NUMFUN. */
/*            Dummy parameter. */
/*     EXTERR Real array of dimension NUMFUN. */
/*            These errors are associated with the region (LEFT,infinity) */
/*            and they are the pure extrapolation errors. */
/*     BETA   Real array of dimension (0:EMAX,0:EMAX). */
/*            Dummy parameter. */
/*     UNEW   Real array of dimension NUMFUN. */
/*            This gives the next terms in the series (new extrapolation */
/*            step) else it is the correction to the U-values (updating). */
/*     UERR   Real array of dimension NUMFUN. */
/*            The estimated errors of all U-terms in the series. */
/*     WORK2  Real array of dimension LENW. */
/*            Work array used in SGAINT. */
/*     WORST  Integer array of dimension WRKSUB. */
/*            Index to the greatest error in each sub-interval. */
/*     LIST   Integer array used in STRINT of dimension WRKSUB. */
/*            Is a partially sorted list, where LIST(1) is the top */
/*            element in a heap of sub-intervals. */
/*     UPOINT Integer array of dimension WRKSUB. */
/*            Is an array of pointers to where in the U-sequence */
/*            a region belongs. This information is used when updating */
/*            the corresponding U-term after a subdivision. */

/* ***ROUTINES CALLED STRINT, SGAINT, SEXINF, R1MACH. */
/* ***END PROLOGUE SADINF */

/*   Global variables. */


/*   Local variables. */

/*   SBRGNS is the number of stored sub-intervals. */
/*   NDIV   The number of sub-intervals to be divided in each main step. */
/*   POINTR Pointer to the position in the data structure where */
/*          the new sub-intervals are to be stored. */
/*   G      is the homogeneous coordinates of the integration rule. */
/*   W      is the weights of the integration rule and the null rules. */
/*   TOP    is a pointer to the top element in the heap of sub-intervals. */
/*   AOLD   Left-endpoint of the interval to be processed. */
/*   BOLD   Right-endpoint of the interval to be processed. */
/*   FLAG   Signals small interval. */
/*   MORE   Signals more work to do. */
/*   P      Half a period. */
/*   NUMU   The number of U-terms. */
/*   NUM    The number of points in the basic rule. */
/*   NUMNUL The number nullrules used. */
/*   UPDATE Index to U-term to be updated. If UPDATE < 0, then this means */
/*          a new extrapolation step. */
/*   VACANT Index to vacant position in the data-structure. */
/*   EPMACH Machine epsilon */
/*   CC     Ratio: the user specified subdivision point B(1)/PERIOD. */

/* ***FIRST EXECUTABLE STATEMENT SADINF */

/*   Define machine epsilon. */

    /* Parameter adjustments */
    --uerr;
    --unew;
    --exterr;
    --ct;
    --t;
    --done;
    --abserr;
    --result;
    --upoint;
    --list;
    --worst;
    e_dim1 = *numfun;
    e_offset = 1 + e_dim1 * 1;
    e -= e_offset;
    u_dim1 = *numfun;
    u_offset = 1 + u_dim1 * 1;
    u -= u_offset;
    --d__;
    --c__;
    --greate;
    errors_dim1 = *numfun;
    errors_offset = 1 + errors_dim1 * 1;
    errors -= errors_offset;
    values_dim1 = *numfun;
    values_offset = 1 + values_dim1 * 1;
    values -= values_offset;
    --b;
    --a;
    beta_dim1 = *emax - 0 + 1;
    beta_offset = 0 + beta_dim1 * 0;
    beta -= beta_offset;
    --work2;

    /* Function Body */
    epmach = r1mach_(&c__4);

/*   Set some pointers for WORK2. */

    l1 = 1;
    l2 = l1 + (*numfun << 4);
    l3 = l2 + *numfun * 21;

    if (*restar == 1) {
	sbrgns = *nsub;
	goto L90;
    } else {
	flag__ = 0;
	cc = b[1] / *period;
    }

/*   Initiate SBRGNS, DONE, MORE, P, A and B. */

    more = TRUE_;
    sbrgns = 0;
    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	done[j] = FALSE_;
/* L10: */
    }
    p = *period / 2;
    a[2] = b[1];
    b[2] = a[2] + p;
    left = b[2];

/*   Initialize E and U */

    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *wrksub;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    e[j + i__ * e_dim1] = (float)0.;
/* L20: */
	    u[j + i__ * u_dim1] = (float)0.;
	}
    }

/*   Apply the rule procedure  over the two first intervals. */

    if (a[1] == b[1]) {
	i__2 = *numfun;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    values[i__ + values_dim1] = (float)0.;
/* L30: */
	    errors[i__ + errors_dim1] = (float)0.;
	}
	greate[1] = (float)0.;
	worst[1] = 1;
	one = 2;
	sbrgns = 1;
	upoint[1] = 1;
    } else {
	one = 1;
    }
    for (i__ = one; i__ <= 2; ++i__) {
	sgaint_(&a[i__], &b[i__], numfun, (U_fp)funsub, &done[1], &more, &
		epmach, &work2[l1], &work2[l2], &work2[l3], &flag__, &values[
		i__ * values_dim1 + 1], &errors[i__ * errors_dim1 + 1], &
		greate[i__], &worst[i__], &c__[i__], &d__[i__]);
	*neval += 21;
	++sbrgns;
	upoint[i__] = i__;
/* L40: */
    }

/*   Initialize RESULT, U, E and NUMU. */

    for (i__ = 1; i__ <= 2; ++i__) {
	i__2 = *numfun;
	for (j = 1; j <= i__2; ++j) {
	    u[j + i__ * u_dim1] = values[j + i__ * values_dim1];
	    e[j + i__ * e_dim1] = errors[j + i__ * errors_dim1];
/* L50: */
	}
/* L60: */
    }
    numu = 2;

/*   Store results in heap. */

    for (i__ = 1; i__ <= 2; ++i__) {
	great = (float)0.;
	i__2 = *numfun;
	for (j = 1; j <= i__2; ++j) {
	    if (errors[j + i__ * errors_dim1] > great) {
		great = errors[j + i__ * errors_dim1];
		worst[i__] = j;
	    }
/* L70: */
	}
	greate[i__] = great;
	c__[i__] = a[i__] + (b[i__] - a[i__]) / 3;
	d__[i__] = b[i__] - (b[i__] - a[i__]) / 3;
	index = i__;
	strint_(&c__2, &index, &greate[1], &list[1], &i__);
/* L80: */
    }

/*   Extrapolate for the first time based on these 2 terms. */
/*   This will initialize the global error as well, and */
/*   MAXEER: the greatest extrapolation error. */
/*   Finally RESULT will be initialized. */

    update = -1;
    i__2 = numu - 1;
    sexinf_(numfun, gamma, &cc, key, &i__2, &t[1], &ct[1], &update, emax, &u[
	    u_offset], &e[e_offset], &result[1], &abserr[1], &exterr[1], &
	    beta[beta_offset], &maxeer);

/*   Check for termination. */

    if (more || *neval < *minpts) {
	goto L90;
    }
    *ifail = 0;
    goto L170;

/* ***End initialization. */

/* ***Begin loop while the error is too great, */

/*    First we check the ranking of the TOP element. */
/*    Then we determine if we will create a new term by integrating */
/*    from LEFT to LEFT + P and then extrapolate, or subdivide the TOP */
/*    element in the heap of subregions. */


L90:
    top = list[1];

/*   New term? */

    if (greate[top] < maxeer) {

/*   We want to extrapolate. Check if we */
/*   are allowed to take a new extrapolation step. */

	if (numu - 1 > *emax) {
	    *ifail = -1;
	    goto L170;
	} else {
	    vacant = 0;
	    ndiv = 1;
	}
    } else {
	vacant = top;
	ndiv = 3;
    }

/*   Check if NEVAL+NDIV*NUM is less than or equal to MAXPTS: */
/*   MAXPTS is the maximum number of function evaluations that are */
/*   allowed to be computed. */

    if (*neval + ndiv * 21 <= *maxpts) {

/*   When we pick an interval to divide in three, then one of the new */
/*   sub-intervals is stored in the original interval's position in the */
/*   data structure. */

/*   Let POINTR point to the first free position in the data structure. */

/*   Determine if this is a new extrapolation step or an update. */
/*   UPDATE will point to the element in the */
/*   U-series to be corrected or created. */

	if (vacant == 0) {
	    pointr = sbrgns + 1;
	    ++numu;
	    update = numu;
	    index = pointr;
	    a[index] = left;
	    left += p;
	    b[index] = left;
	    top = index;
	    i__2 = *numfun;
	    for (j = 1; j <= i__2; ++j) {
		uerr[j] = (float)0.;
/* L100: */
		unew[j] = (float)0.;
	    }
	} else {
	    update = upoint[vacant];

/*   Save the endpoints. */

	    aold = a[top];
	    bold = b[top];

/*   Remove the top element from the heap.(The value of K does not matter */

	    pointr = sbrgns + 2;
	    strint_(&c__1, &sbrgns, &greate[1], &list[1], &k);

/*   Compute the three new intervals. */

	    i1 = top;
	    i2 = pointr - 1;
	    i3 = pointr;
	    a[i1] = aold;
	    b[i1] = c__[top];
	    a[i2] = b[i1];
	    b[i2] = d__[top];
	    a[i3] = b[i2];
	    b[i3] = bold;
	    index = vacant;
	    i__2 = *numfun;
	    for (j = 1; j <= i__2; ++j) {
		uerr[j] = -errors[j + index * errors_dim1];
/* L110: */
		unew[j] = -values[j + index * values_dim1];
	    }
	}

/*   Apply basic rule. */

	i__2 = ndiv;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    sgaint_(&a[index], &b[index], numfun, (U_fp)funsub, &done[1], &
		    more, &epmach, &work2[l1], &work2[l2], &work2[l3], &
		    flag__, &values[index * values_dim1 + 1], &errors[index * 
		    errors_dim1 + 1], &greate[index], &worst[index], &c__[
		    index], &d__[index]);
	    upoint[index] = update;

/*   Compute the U-term */

	    i__1 = *numfun;
	    for (j = 1; j <= i__1; ++j) {
		unew[j] += values[j + index * values_dim1];
		uerr[j] += errors[j + index * errors_dim1];
/* L120: */
	    }
	    index = sbrgns + i__ + 1;
/* L130: */
	}
	*neval += ndiv * 21;

/*   Compute the E and U terms (These may be new terms or terms that */
/*   have to be updated), */

	i__2 = *numfun;
	for (j = 1; j <= i__2; ++j) {
	    u[j + update * u_dim1] += unew[j];
	    e[j + update * e_dim1] += uerr[j];
/* L140: */
	}

/*   Do the extrapolation and compute the global results and errors. */
/*   UPDATE is used to signal an extrapolation step. */

	if (vacant == 0) {
	    update = -1;
	}
	i__2 = numu - 1;
	sexinf_(numfun, gamma, &cc, key, &i__2, &t[1], &ct[1], &update, emax, 
		&u[u_offset], &e[e_offset], &result[1], &abserr[1], &exterr[1]
		, &beta[beta_offset], &maxeer);

/*   Check each integration problem and set DONE(J)=.TRUE. if they */
/*   are finished. MORE will signal if there is more to do. */

	more = FALSE_;
	i__2 = *numfun;
	for (j = 1; j <= i__2; ++j) {
	    if (abserr[j] <= *epsrel * (r__1 = result[j], dabs(r__1)) || 
		    abserr[j] <= *epsabs) {
		done[j] = TRUE_;
	    } else {
		more = TRUE_;
		done[j] = FALSE_;
	    }
/* L150: */
	}

/*   Store results in heap. */

	index = top;
	i__2 = ndiv;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    jj = sbrgns + i__;
	    strint_(&c__2, &jj, &greate[1], &list[1], &index);
	    index = sbrgns + i__ + 1;
/* L160: */
	}
	sbrgns += ndiv;

/*   Check for termination. */

	if (more || *neval < *minpts) {
	    goto L90;
	}

/*   Else we did not succeed with the */
/*   given value of MAXPTS. Note: We do not use IFAIL to signal */
/*   FLAG in the current implementation. */

    } else {
	*ifail = 1;
    }

L170:
    *nsub = sbrgns;
    return 0;

/* ***END SADINF */

} /* sadinf_ */

#ifdef __cplusplus
	}
#endif
