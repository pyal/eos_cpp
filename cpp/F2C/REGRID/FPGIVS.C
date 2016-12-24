/* FPGIVS.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Subroutine */ int fpgivs_(real *piv, real *ww, real *cos__, real *sin__)
{
    /* System generated locals */
    real r__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    real store, dd, one;

/*  subroutine fpgivs calculates the parameters of a givens */
/*  transformation . */
/*  .. */
/*  ..scalar arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  .. */
    one = (float)1.;
    store = dabs(*piv);
    if (store >= *ww) {
/* Computing 2nd power */
	r__1 = *ww / *piv;
	dd = store * sqrt(one + r__1 * r__1);
    }
    if (store < *ww) {
/* Computing 2nd power */
	r__1 = *piv / *ww;
	dd = *ww * sqrt(one + r__1 * r__1);
    }
    *cos__ = *ww / dd;
    *sin__ = *piv / dd;
    *ww = dd;
    return 0;
} /* fpgivs_ */

#ifdef __cplusplus
	}
#endif
