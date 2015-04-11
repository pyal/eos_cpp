/* qage.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Table of constant values */

static integer c__4 = 4;
static integer c__1 = 1;

/* Subroutine */ int qage_(E_fp f, real *a, real *b, real *epsabs, real *
	epsrel, integer *key, integer *limit, real *result, real *abserr, 
	integer *neval, integer *ier, real *alist__, real *blist, real *rlist,
	 real *elist, integer *iord, integer *last)
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2;

    /* Local variables */
    real area;
    integer keyf;
    real area1, area2, area12;
    integer k;
    real erro12, a1, a2, b1, b2, defab1, defab2;
    integer nrmax;
    real uflow;
    extern /* Subroutine */ int qpsrt_(integer *, integer *, integer *, real *
	    , real *, integer *, integer *);
    extern doublereal r1mach_(integer *);
    integer iroff1, iroff2;
    real error1, error2, defabs, epmach, errbnd, resabs, errmax;
    integer maxerr;
    real errsum;
    extern /* Subroutine */ int qk21_(E_fp, real *, real *, real *, real *, 
	    real *, real *), qk31_(E_fp, real *, real *, real *, real *, real 
	    *, real *), qk15_(E_fp, real *, real *, real *, real *, real *, 
	    real *), qk41_(E_fp, real *, real *, real *, real *, real *, real 
	    *), qk51_(E_fp, real *, real *, real *, real *, real *, real *), 
	    qk61_(E_fp, real *, real *, real *, real *, real *, real *);

/* ***begin prologue  qage */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a1a1 */
/* ***keywords  automatic integrator, general-purpose, */
/*             integrand examinator, globally adaptive, */
/*             gauss-kronrod */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  the routine calculates an approximation result to a given */
/*            definite integral   i = integral of f over (a,b), */
/*            hopefully satisfying following claim for accuracy */
/*            abs(i-reslt).le.max(epsabs,epsrel*abs(i)). */
/* ***description */

/*        computation of a definite integral */
/*        standard fortran subroutine */
/*        real version */

/*        parameters */
/*         on entry */
/*            f      - real */
/*                     function subprogram defining the integrand */
/*                     function f(x). the actual name for f needs to be */
/*                     declared e x t e r n a l in the driver program. */

/*            a      - real */
/*                     lower limit of integration */

/*            b      - real */
/*                     upper limit of integration */

/*            epsabs - real */
/*                     absolute accuracy requested */
/*            epsrel - real */
/*                     relative accuracy requested */
/*                     if  epsabs.le.0 */
/*                     and epsrel.lt.max(50*rel.mach.acc.,0.5d-28), */
/*                     the routine will end with ier = 6. */

/*            key    - integer */
/*                     key for choice of local integration rule */
/*                     a gauss-kronrod pair is used with */
/*                          7 - 15 points if key.lt.2, */
/*                         10 - 21 points if key = 2, */
/*                         15 - 31 points if key = 3, */
/*                         20 - 41 points if key = 4, */
/*                         25 - 51 points if key = 5, */
/*                         30 - 61 points if key.gt.5. */

/*            limit  - integer */
/*                     gives an upperbound on the number of subintervals */
/*                     in the partition of (a,b), limit.ge.1. */

/*         on return */
/*            result - real */
/*                     approximation to the integral */

/*            abserr - real */
/*                     estimate of the modulus of the absolute error, */
/*                     which should equal or exceed abs(i-result) */

/*            neval  - integer */
/*                     number of integrand evaluations */

/*            ier    - integer */
/*                     ier = 0 normal and reliable termination of the */
/*                             routine. it is assumed that the requested */
/*                             accuracy has been achieved. */
/*                     ier.gt.0 abnormal termination of the routine */
/*                             the estimates for result and error are */
/*                             less reliable. it is assumed that the */
/*                             requested accuracy has not been achieved. */
/*            error messages */
/*                     ier = 1 maximum number of subdivisions allowed */
/*                             has been achieved. one can allow more */
/*                             subdivisions by increasing the value */
/*                             of limit. */
/*                             however, if this yields no improvement it */
/*                             is rather advised to analyze the integrand */
/*                             in order to determine the integration */
/*                             difficulties. if the position of a local */
/*                             difficulty can be determined(e.g. */
/*                             singularity, discontinuity within the */
/*                             interval) one will probably gain from */
/*                             splitting up the interval at this point */
/*                             and calling the integrator on the */
/*                             subranges. if possible, an appropriate */
/*                             special-purpose integrator should be used */
/*                             which is designed for handling the type of */
/*                             difficulty involved. */
/*                         = 2 the occurrence of roundoff error is */
/*                             detected, which prevents the requested */
/*                             tolerance from being achieved. */
/*                         = 3 extremely bad integrand behaviour occurs */
/*                             at some points of the integration */
/*                             interval. */
/*                         = 6 the input is invalid, because */
/*                             (epsabs.le.0 and */
/*                              epsrel.lt.max(50*rel.mach.acc.,0.5d-28), */
/*                             result, abserr, neval, last, rlist(1) , */
/*                             elist(1) and iord(1) are set to zero. */
/*                             alist(1) and blist(1) are set to a and b */
/*                             respectively. */

/*            alist   - real */
/*                      vector of dimension at least limit, the first */
/*                       last  elements of which are the left */
/*                      end points of the subintervals in the partition */
/*                      of the given integration range (a,b) */

/*            blist   - real */
/*                      vector of dimension at least limit, the first */
/*                       last  elements of which are the right */
/*                      end points of the subintervals in the partition */
/*                      of the given integration range (a,b) */

/*            rlist   - real */
/*                      vector of dimension at least limit, the first */
/*                       last  elements of which are the */
/*                      integral approximations on the subintervals */

/*            elist   - real */
/*                      vector of dimension at least limit, the first */
/*                       last  elements of which are the moduli of the */
/*                      absolute error estimates on the subintervals */

/*            iord    - integer */
/*                      vector of dimension at least limit, the first k */
/*                      elements of which are pointers to the */
/*                      error estimates over the subintervals, */
/*                      such that elist(iord(1)), ..., */
/*                      elist(iord(k)) form a decreasing sequence, */
/*                      with k = last if last.le.(limit/2+2), and */
/*                      k = limit+1-last otherwise */

/*            last    - integer */
/*                      number of subintervals actually produced in the */
/*                      subdivision process */

/* ***references  (none) */
/* ***routines called  qk15,qk21,qk31,qk41,qk51,qk61,qpsrt,r1mach */
/* ***end prologue  qage */




/*            list of major variables */
/*            ----------------------- */

/*           alist     - list of left end points of all subintervals */
/*                       considered up to now */
/*           blist     - list of right end points of all subintervals */
/*                       considered up to now */
/*           rlist(i)  - approximation to the integral over */
/*                      (alist(i),blist(i)) */
/*           elist(i)  - error estimate applying to rlist(i) */
/*           maxerr    - pointer to the interval with largest */
/*                       error estimate */
/*           errmax    - elist(maxerr) */
/*           area      - sum of the integrals over the subintervals */
/*           errsum    - sum of the errors over the subintervals */
/*           errbnd    - requested accuracy max(epsabs,epsrel* */
/*                       abs(result)) */
/*           *****1    - variable for the left subinterval */
/*           *****2    - variable for the right subinterval */
/*           last      - index for subdivision */


/*           machine dependent constants */
/*           --------------------------- */

/*           epmach  is the largest relative spacing. */
/*           uflow  is the smallest positive magnitude. */

/* ***first executable statement  qage */
    /* Parameter adjustments */
    --iord;
    --elist;
    --rlist;
    --blist;
    --alist__;

    /* Function Body */
    epmach = r1mach_(&c__4);
    uflow = r1mach_(&c__1);

/*           test on validity of parameters */
/*           ------------------------------ */

    *ier = 0;
    *neval = 0;
    *last = 0;
    *result = (float)0.;
    *abserr = (float)0.;
    alist__[1] = *a;
    blist[1] = *b;
    rlist[1] = (float)0.;
    elist[1] = (float)0.;
    iord[1] = 0;
/* Computing MAX */
    r__1 = epmach * (float)50.;
    if (*epsabs <= (float)0. && *epsrel < dmax(r__1,(float)5e-15)) {
	*ier = 6;
    }
    if (*ier == 6) {
	goto L999;
    }

/*           first approximation to the integral */
/*           ----------------------------------- */

    keyf = *key;
    if (*key <= 0) {
	keyf = 1;
    }
    if (*key >= 7) {
	keyf = 6;
    }
    *neval = 0;
    if (keyf == 1) {
	qk15_((E_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 2) {
	qk21_((E_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 3) {
	qk31_((E_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 4) {
	qk41_((E_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 5) {
	qk51_((E_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 6) {
	qk61_((E_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    *last = 1;
    rlist[1] = *result;
    elist[1] = *abserr;
    iord[1] = 1;

/*           test on accuracy. */

/* Computing MAX */
    r__1 = *epsabs, r__2 = *epsrel * dabs(*result);
    errbnd = dmax(r__1,r__2);
    if (*abserr <= epmach * (float)50. * defabs && *abserr > errbnd) {
	*ier = 2;
    }
    if (*limit == 1) {
	*ier = 1;
    }
    if (*ier != 0 || *abserr <= errbnd && *abserr != resabs || *abserr == (
	    float)0.) {
	goto L60;
    }

/*           initialization */
/*           -------------- */


    errmax = *abserr;
    maxerr = 1;
    area = *result;
    errsum = *abserr;
    nrmax = 1;
    iroff1 = 0;
    iroff2 = 0;

/*           main do-loop */
/*           ------------ */

    i__1 = *limit;
    for (*last = 2; *last <= i__1; ++(*last)) {

/*           bisect the subinterval with the largest error estimate. */

	a1 = alist__[maxerr];
	b1 = (alist__[maxerr] + blist[maxerr]) * (float).5;
	a2 = b1;
	b2 = blist[maxerr];
	if (keyf == 1) {
	    qk15_((E_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 2) {
	    qk21_((E_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 3) {
	    qk31_((E_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 4) {
	    qk41_((E_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 5) {
	    qk51_((E_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 6) {
	    qk61_((E_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 1) {
	    qk15_((E_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 2) {
	    qk21_((E_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 3) {
	    qk31_((E_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 4) {
	    qk41_((E_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 5) {
	    qk51_((E_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 6) {
	    qk61_((E_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}

/*           improve previous approximations to integral */
/*           and error and test for accuracy. */

	++(*neval);
	area12 = area1 + area2;
	erro12 = error1 + error2;
	errsum = errsum + erro12 - errmax;
	area = area + area12 - rlist[maxerr];
	if (defab1 == error1 || defab2 == error2) {
	    goto L5;
	}
	if ((r__1 = rlist[maxerr] - area12, dabs(r__1)) <= dabs(area12) * (
		float)1e-5 && erro12 >= errmax * (float).99) {
	    ++iroff1;
	}
	if (*last > 10 && erro12 > errmax) {
	    ++iroff2;
	}
L5:
	rlist[maxerr] = area1;
	rlist[*last] = area2;
/* Computing MAX */
	r__1 = *epsabs, r__2 = *epsrel * dabs(area);
	errbnd = dmax(r__1,r__2);
	if (errsum <= errbnd) {
	    goto L8;
	}

/*           test for roundoff error and eventually */
/*           set error flag. */

	if (iroff1 >= 6 || iroff2 >= 20) {
	    *ier = 2;
	}

/*           set error flag in the case that the number of */
/*           subintervals equals limit. */

	if (*last == *limit) {
	    *ier = 1;
	}

/*           set error flag in the case of bad integrand behaviour */
/*           at a point of the integration range. */

/* Computing MAX */
	r__1 = dabs(a1), r__2 = dabs(b2);
	if (dmax(r__1,r__2) <= (epmach * (float)100. + (float)1.) * (dabs(a2) 
		+ uflow * (float)1e3)) {
	    *ier = 3;
	}

/*           append the newly-created intervals to the list. */

L8:
	if (error2 > error1) {
	    goto L10;
	}
	alist__[*last] = a2;
	blist[maxerr] = b1;
	blist[*last] = b2;
	elist[maxerr] = error1;
	elist[*last] = error2;
	goto L20;
L10:
	alist__[maxerr] = a2;
	alist__[*last] = a1;
	blist[*last] = b1;
	rlist[maxerr] = area2;
	rlist[*last] = area1;
	elist[maxerr] = error2;
	elist[*last] = error1;

/*           call subroutine qpsrt to maintain the descending ordering */
/*           in the list of error estimates and select the */
/*           subinterval with the largest error estimate (to be */
/*           bisected next). */

L20:
	qpsrt_(limit, last, &maxerr, &errmax, &elist[1], &iord[1], &nrmax);
/* ***jump out of do-loop */
	if (*ier != 0 || errsum <= errbnd) {
	    goto L40;
	}
/* L30: */
    }

/*           compute final result. */
/*           --------------------- */

L40:
    *result = (float)0.;
    i__1 = *last;
    for (k = 1; k <= i__1; ++k) {
	*result += rlist[k];
/* L50: */
    }
    *abserr = errsum;
L60:
    if (keyf != 1) {
	*neval = (keyf * 10 + 1) * ((*neval << 1) + 1);
    }
    if (keyf == 1) {
	*neval = *neval * 30 + 15;
    }
L999:
    return 0;
} /* qage_ */

#ifdef __cplusplus
	}
#endif
