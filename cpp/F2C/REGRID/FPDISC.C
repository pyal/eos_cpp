/* FPDISC.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Subroutine */ int fpdisc_(real *t, integer *n, integer *k2, real *b, 
	integer *nest)
{
    /* System generated locals */
    integer b_dim1, b_offset, i__1, i__2, i__3;

    /* Local variables */
    real prod, h__[12];
    integer i__, j, k, l, nrint, k1;
    real an;
    integer ik, jk, lj, lk, lp, nk1;
    real fac;
    integer lmk;

/*  subroutine fpdisc calculates the discontinuity jumps of the kth */
/*  derivative of the b-splines of degree k at the knots t(k+2)..t(n-k-1) 
*/
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  .. */
    /* Parameter adjustments */
    --t;
    b_dim1 = *nest;
    b_offset = b_dim1 + 1;
    b -= b_offset;

    /* Function Body */
    k1 = *k2 - 1;
    k = k1 - 1;
    nk1 = *n - k1;
    nrint = nk1 - k;
    an = (real) nrint;
    fac = an / (t[nk1 + 1] - t[k1]);
    i__1 = nk1;
    for (l = *k2; l <= i__1; ++l) {
	lmk = l - k1;
	i__2 = k1;
	for (j = 1; j <= i__2; ++j) {
	    ik = j + k1;
	    lj = l + j;
	    lk = lj - *k2;
	    h__[j - 1] = t[l] - t[lk];
	    h__[ik - 1] = t[l] - t[lj];
/* L10: */
	}
	lp = lmk;
	i__2 = *k2;
	for (j = 1; j <= i__2; ++j) {
	    jk = j;
	    prod = h__[j - 1];
	    i__3 = k;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		++jk;
		prod = prod * h__[jk - 1] * fac;
/* L20: */
	    }
	    lk = lp + k1;
	    b[lmk + j * b_dim1] = (t[lk] - t[lp]) / prod;
	    ++lp;
/* L30: */
	}
/* L40: */
    }
    return 0;
} /* fpdisc_ */

#ifdef __cplusplus
	}
#endif
