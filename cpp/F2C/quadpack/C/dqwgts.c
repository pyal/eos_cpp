/* dqwgts.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

doublereal dqwgts_(doublereal *x, doublereal *a, doublereal *b, doublereal *
	alfa, doublereal *beta, integer *integr)
{
    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), log(doublereal);

    /* Local variables */
    doublereal xma, bmx;

/* ***begin prologue  dqwgts */
/* ***refer to dqk15w */
/* ***routines called  (none) */
/* ***revision date  810101   (yymmdd) */
/* ***keywords  weight function, algebraico-logarithmic */
/*             end-point singularities */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  this function subprogram is used together with the */
/*            routine dqaws and defines the weight function. */
/* ***end prologue  dqwgts */

/* ***first executable statement  dqwgts */
    xma = *x - *a;
    bmx = *b - *x;
    ret_val = pow_dd(&xma, alfa) * pow_dd(&bmx, beta);
    switch (*integr) {
	case 1:  goto L40;
	case 2:  goto L10;
	case 3:  goto L20;
	case 4:  goto L30;
    }
L10:
    ret_val *= log(xma);
    goto L40;
L20:
    ret_val *= log(bmx);
    goto L40;
L30:
    ret_val = ret_val * log(xma) * log(bmx);
L40:
    return ret_val;
} /* dqwgts_ */

#ifdef __cplusplus
	}
#endif
