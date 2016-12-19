/* dqwgtc.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

doublereal dqwgtc_(doublereal *x, doublereal *c__, doublereal *p2, doublereal 
	*p3, doublereal *p4, integer *kp)
{
    /* System generated locals */
    doublereal ret_val;

/* ***begin prologue  dqwgtc */
/* ***refer to dqk15w */
/* ***routines called  (none) */
/* ***revision date  810101   (yymmdd) */
/* ***keywords  weight function, cauchy principal value */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  this function subprogram is used together with the */
/*            routine qawc and defines the weight function. */
/* ***end prologue  dqwgtc */

/* ***first executable statement  dqwgtc */
    ret_val = 1. / (*x - *c__);
    return ret_val;
} /* dqwgtc_ */

#ifdef __cplusplus
	}
#endif
