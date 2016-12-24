/* dqpsrt.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Subroutine */ int dqpsrt_(integer *limit, integer *last, integer *maxerr, 
	doublereal *ermax, doublereal *elist, integer *iord, integer *nrmax)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer ibeg, jbnd, i__, j, k, isucc, jupbn;
    doublereal errmin, errmax;
    integer ido;

/* ***begin prologue  dqpsrt */
/* ***refer to  dqage,dqagie,dqagpe,dqawse */
/* ***routines called  (none) */
/* ***revision date  810101   (yymmdd) */
/* ***keywords  sequential sorting */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  this routine maintains the descending ordering in the */
/*            list of the local error estimated resulting from the */
/*            interval subdivision process. at each call two error */
/*            estimates are inserted using the sequential search */
/*            method, top-down for the largest error estimate and */
/*            bottom-up for the smallest error estimate. */
/* ***description */

/*           ordering routine */
/*           standard fortran subroutine */
/*           double precision version */

/*           parameters (meaning at output) */
/*              limit  - integer */
/*                       maximum number of error estimates the list */
/*                       can contain */

/*              last   - integer */
/*                       number of error estimates currently in the list */

/*              maxerr - integer */
/*                       maxerr points to the nrmax-th largest error */
/*                       estimate currently in the list */

/*              ermax  - double precision */
/*                       nrmax-th largest error estimate */
/*                       ermax = elist(maxerr) */

/*              elist  - double precision */
/*                       vector of dimension last containing */
/*                       the error estimates */

/*              iord   - integer */
/*                       vector of dimension last, the first k elements */
/*                       of which contain pointers to the error */
/*                       estimates, such that */
/*                       elist(iord(1)),...,  elist(iord(k)) */
/*                       form a decreasing sequence, with */
/*                       k = last if last.le.(limit/2+2), and */
/*                       k = limit+1-last otherwise */

/*              nrmax  - integer */
/*                       maxerr = iord(nrmax) */

/* ***end prologue  dqpsrt */


/*           check whether the list contains more than */
/*           two error estimates. */

/* ***first executable statement  dqpsrt */
    /* Parameter adjustments */
    --iord;
    --elist;

    /* Function Body */
    if (*last > 2) {
	goto L10;
    }
    iord[1] = 1;
    iord[2] = 2;
    goto L90;

/*           this part of the routine is only executed if, due to a */
/*           difficult integrand, subdivision increased the error */
/*           estimate. in the normal case the insert procedure should */
/*           start after the nrmax-th largest error estimate. */

L10:
    errmax = elist[*maxerr];
    if (*nrmax == 1) {
	goto L30;
    }
    ido = *nrmax - 1;
    i__1 = ido;
    for (i__ = 1; i__ <= i__1; ++i__) {
	isucc = iord[*nrmax - 1];
/* ***jump out of do-loop */
	if (errmax <= elist[isucc]) {
	    goto L30;
	}
	iord[*nrmax] = isucc;
	--(*nrmax);
/* L20: */
    }

/*           compute the number of elements in the list to be maintained */
/*           in descending order. this number depends on the number of */
/*           subdivisions still allowed. */

L30:
    jupbn = *last;
    if (*last > *limit / 2 + 2) {
	jupbn = *limit + 3 - *last;
    }
    errmin = elist[*last];

/*           insert errmax by traversing the list top-down, */
/*           starting comparison from the element elist(iord(nrmax+1)). */

    jbnd = jupbn - 1;
    ibeg = *nrmax + 1;
    if (ibeg > jbnd) {
	goto L50;
    }
    i__1 = jbnd;
    for (i__ = ibeg; i__ <= i__1; ++i__) {
	isucc = iord[i__];
/* ***jump out of do-loop */
	if (errmax >= elist[isucc]) {
	    goto L60;
	}
	iord[i__ - 1] = isucc;
/* L40: */
    }
L50:
    iord[jbnd] = *maxerr;
    iord[jupbn] = *last;
    goto L90;

/*           insert errmin by traversing the list bottom-up. */

L60:
    iord[i__ - 1] = *maxerr;
    k = jbnd;
    i__1 = jbnd;
    for (j = i__; j <= i__1; ++j) {
	isucc = iord[k];
/* ***jump out of do-loop */
	if (errmin < elist[isucc]) {
	    goto L80;
	}
	iord[k + 1] = isucc;
	--k;
/* L70: */
    }
    iord[i__] = *last;
    goto L90;
L80:
    iord[k + 1] = *last;

/*           set maxerr and ermax. */

L90:
    *maxerr = iord[*nrmax];
    *ermax = elist[*maxerr];
    return 0;
} /* dqpsrt_ */

#ifdef __cplusplus
	}
#endif
