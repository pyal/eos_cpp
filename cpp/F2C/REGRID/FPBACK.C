/* FPBACK.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Subroutine */ int fpback_(real *a, real *z__, integer *n, integer *k, real 
	*c__, integer *nest)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;

    /* Local variables */
    integer i__, j, l, m, i1;
    real store;
    integer k1;

/*  subroutine fpback calculates the solution of the system of */
/*  equations a*c = z with a a n x n upper triangular matrix */
/*  of bandwidth k. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --c__;
    --z__;
    a_dim1 = *nest;
    a_offset = a_dim1 + 1;
    a -= a_offset;

    /* Function Body */
    k1 = *k - 1;
    c__[*n] = z__[*n] / a[*n + a_dim1];
    i__ = *n - 1;
    if (i__ == 0) {
	goto L30;
    }
    i__1 = *n;
    for (j = 2; j <= i__1; ++j) {
	store = z__[i__];
	i1 = k1;
	if (j <= k1) {
	    i1 = j - 1;
	}
	m = i__;
	i__2 = i1;
	for (l = 1; l <= i__2; ++l) {
	    ++m;
	    store -= c__[m] * a[i__ + (l + 1) * a_dim1];
/* L10: */
	}
	c__[i__] = store / a[i__ + a_dim1];
	--i__;
/* L20: */
    }
L30:
    return 0;
} /* fpback_ */

#ifdef __cplusplus
	}
#endif
