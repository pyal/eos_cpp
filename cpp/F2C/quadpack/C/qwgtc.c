/* qwgtc.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

doublereal qwgtc_(real *x, real *c__, real *p2, real *p3, real *p4, integer *
	kp)
{
    /* System generated locals */
    real ret_val;

/* ***begin prologue  qwgtc */
/* ***refer to qk15w */
/* ***routines called  (none) */
/* ***revision date  810101   (yymmdd) */
/* ***keywords  weight function, cauchy principal value */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  this function subprogram is used together with the */
/*            routine qawc and defines the weight function. */
/* ***end prologue  qwgtc */

/* ***first executable statement */
    ret_val = (float)1. / (*x - *c__);
    return ret_val;
} /* qwgtc_ */

#ifdef __cplusplus
	}
#endif
