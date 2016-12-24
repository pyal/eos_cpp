/* qmomo.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Table of constant values */

static doublereal c_b2 = 2.;

/* Subroutine */ int qmomo_(real *alfa, real *beta, real *ri, real *rj, real *
	rg, real *rh, integer *integr)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    real ralf, rbet, alfp1, alfp2, betp1, betp2;
    integer i__;
    real an;
    integer im1;
    real anm1;

/* ***begin prologue  qmomo */
/* ***date written   810101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a2a1,c3a2 */
/* ***keywords  modified chebyshev moments */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  this routine computes modified chebsyshev moments. the k-th */
/*            modified chebyshev moment is defined as the integral over */
/*            (-1,1) of w(x)*t(k,x), where t(k,x) is the chebyshev */
/*            polynomial of degree k. */
/* ***description */

/*        modified chebyshev moments */
/*        standard fortran subroutine */
/*        real version */

/*        parameters */
/*           alfa   - real */
/*                    parameter in the weight function w(x), alfa.gt.(-1) */

/*           beta   - real */
/*                    parameter in the weight function w(x), beta.gt.(-1) */

/*           ri     - real */
/*                    vector of dimension 25 */
/*                    ri(k) is the integral over (-1,1) of */
/*                    (1+x)**alfa*t(k-1,x), k = 1, ..., 25. */

/*           rj     - real */
/*                    vector of dimension 25 */
/*                    rj(k) is the integral over (-1,1) of */
/*                    (1-x)**beta*t(k-1,x), k = 1, ..., 25. */

/*           rg     - real */
/*                    vector of dimension 25 */
/*                    rg(k) is the integral over (-1,1) of */
/*                    (1+x)**alfa*log((1+x)/2)*t(k-1,x), k = 1, ..., 25. */

/*           rh     - real */
/*                    vector of dimension 25 */
/*                    rh(k) is the integral over (-1,1) of */
/*                    (1-x)**beta*log((1-x)/2)*t(k-1,x), k = 1, ..., 25. */

/*           integr - integer */
/*                    input parameter indicating the modified */
/*                    moments to be computed */
/*                    integr = 1 compute ri, rj */
/*                           = 2 compute ri, rj, rg */
/*                           = 3 compute ri, rj, rh */
/*                           = 4 compute ri, rj, rg, rh */
/* ***references  (none) */
/* ***routines called  (none) */
/* ***end prologue  qmomo */




/* ***first executable statement  qmomo */
    /* Parameter adjustments */
    --rh;
    --rg;
    --rj;
    --ri;

    /* Function Body */
    alfp1 = *alfa + (float)1.;
    betp1 = *beta + (float)1.;
    alfp2 = *alfa + (float)2.;
    betp2 = *beta + (float)2.;
    d__1 = (doublereal) alfp1;
    ralf = pow_dd(&c_b2, &d__1);
    d__1 = (doublereal) betp1;
    rbet = pow_dd(&c_b2, &d__1);

/*           compute ri, rj using a forward recurrence relation. */

    ri[1] = ralf / alfp1;
    rj[1] = rbet / betp1;
    ri[2] = ri[1] * *alfa / alfp2;
    rj[2] = rj[1] * *beta / betp2;
    an = (float)2.;
    anm1 = (float)1.;
    for (i__ = 3; i__ <= 25; ++i__) {
	ri[i__] = -(ralf + an * (an - alfp2) * ri[i__ - 1]) / (anm1 * (an + 
		alfp1));
	rj[i__] = -(rbet + an * (an - betp2) * rj[i__ - 1]) / (anm1 * (an + 
		betp1));
	anm1 = an;
	an += (float)1.;
/* L20: */
    }
    if (*integr == 1) {
	goto L70;
    }
    if (*integr == 3) {
	goto L40;
    }

/*           compute rg using a forward recurrence relation. */

    rg[1] = -ri[1] / alfp1;
    rg[2] = -(ralf + ralf) / (alfp2 * alfp2) - rg[1];
    an = (float)2.;
    anm1 = (float)1.;
    im1 = 2;
    for (i__ = 3; i__ <= 25; ++i__) {
	rg[i__] = -(an * (an - alfp2) * rg[im1] - an * ri[im1] + anm1 * ri[
		i__]) / (anm1 * (an + alfp1));
	anm1 = an;
	an += (float)1.;
	im1 = i__;
/* L30: */
    }
    if (*integr == 2) {
	goto L70;
    }

/*           compute rh using a forward recurrence relation. */

L40:
    rh[1] = -rj[1] / betp1;
    rh[2] = -(rbet + rbet) / (betp2 * betp2) - rh[1];
    an = (float)2.;
    anm1 = (float)1.;
    im1 = 2;
    for (i__ = 3; i__ <= 25; ++i__) {
	rh[i__] = -(an * (an - betp2) * rh[im1] - an * rj[im1] + anm1 * rj[
		i__]) / (anm1 * (an + betp1));
	anm1 = an;
	an += (float)1.;
	im1 = i__;
/* L50: */
    }
    for (i__ = 2; i__ <= 25; i__ += 2) {
	rh[i__] = -rh[i__];
/* L60: */
    }
L70:
    for (i__ = 2; i__ <= 25; i__ += 2) {
	rj[i__] = -rj[i__];
/* L80: */
    }
/* L90: */
    return 0;
} /* qmomo_ */

#ifdef __cplusplus
	}
#endif
