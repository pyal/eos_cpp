/* qcheb.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Subroutine */ int qcheb_(real *x, real *fval, real *cheb12, real *cheb24)
{
    real alam, alam1, alam2, part1, part2, part3;
    integer i__, j;
    real v[12];

/* ***begin prologue  qcheb */
/* ***refer to  qc25c,qc25f,qc25s */
/* ***routines called  (none) */
/* ***revision date  830518   (yymmdd) */
/* ***keywords  chebyshev series expansion, fast fourier transform */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  this routine computes the chebyshev series expansion */
/*            of degrees 12 and 24 of a function using a */
/*            fast fourier transform method */
/*            f(x) = sum(k=1,..,13) (cheb12(k)*t(k-1,x)), */
/*            f(x) = sum(k=1,..,25) (cheb24(k)*t(k-1,x)), */
/*            where t(k,x) is the chebyshev polynomial of degree k. */
/* ***description */

/*        chebyshev series expansion */
/*        standard fortran subroutine */
/*        real version */

/*        parameters */
/*          on entry */
/*           x      - real */
/*                    vector of dimension 11 containing the */
/*                    values cos(k*pi/24), k = 1, ..., 11 */

/*           fval   - real */
/*                    vector of dimension 25 containing the */
/*                    function values at the points */
/*                    (b+a+(b-a)*cos(k*pi/24))/2, k = 0, ...,24, */
/*                    where (a,b) is the approximation interval. */
/*                    fval(1) and fval(25) are divided by two */
/*                    (these values are destroyed at output). */

/*          on return */
/*           cheb12 - real */
/*                    vector of dimension 13 containing the */
/*                    chebyshev coefficients for degree 12 */

/*           cheb24 - real */
/*                    vector of dimension 25 containing the */
/*                    chebyshev coefficients for degree 24 */

/* ***end prologue  qcheb */



/* ***first executable statement  qcheb */
    /* Parameter adjustments */
    --cheb24;
    --cheb12;
    --fval;
    --x;

    /* Function Body */
    for (i__ = 1; i__ <= 12; ++i__) {
	j = 26 - i__;
	v[i__ - 1] = fval[i__] - fval[j];
	fval[i__] += fval[j];
/* L10: */
    }
    alam1 = v[0] - v[8];
    alam2 = x[6] * (v[2] - v[6] - v[10]);
    cheb12[4] = alam1 + alam2;
    cheb12[10] = alam1 - alam2;
    alam1 = v[1] - v[7] - v[9];
    alam2 = v[3] - v[5] - v[11];
    alam = x[3] * alam1 + x[9] * alam2;
    cheb24[4] = cheb12[4] + alam;
    cheb24[22] = cheb12[4] - alam;
    alam = x[9] * alam1 - x[3] * alam2;
    cheb24[10] = cheb12[10] + alam;
    cheb24[16] = cheb12[10] - alam;
    part1 = x[4] * v[4];
    part2 = x[8] * v[8];
    part3 = x[6] * v[6];
    alam1 = v[0] + part1 + part2;
    alam2 = x[2] * v[2] + part3 + x[10] * v[10];
    cheb12[2] = alam1 + alam2;
    cheb12[12] = alam1 - alam2;
    alam = x[1] * v[1] + x[3] * v[3] + x[5] * v[5] + x[7] * v[7] + x[9] * v[9]
	     + x[11] * v[11];
    cheb24[2] = cheb12[2] + alam;
    cheb24[24] = cheb12[2] - alam;
    alam = x[11] * v[1] - x[9] * v[3] + x[7] * v[5] - x[5] * v[7] + x[3] * v[
	    9] - x[1] * v[11];
    cheb24[12] = cheb12[12] + alam;
    cheb24[14] = cheb12[12] - alam;
    alam1 = v[0] - part1 + part2;
    alam2 = x[10] * v[2] - part3 + x[2] * v[10];
    cheb12[6] = alam1 + alam2;
    cheb12[8] = alam1 - alam2;
    alam = x[5] * v[1] - x[9] * v[3] - x[1] * v[5] - x[11] * v[7] + x[3] * v[
	    9] + x[7] * v[11];
    cheb24[6] = cheb12[6] + alam;
    cheb24[20] = cheb12[6] - alam;
    alam = x[7] * v[1] - x[3] * v[3] - x[11] * v[5] + x[1] * v[7] - x[9] * v[
	    9] - x[5] * v[11];
    cheb24[8] = cheb12[8] + alam;
    cheb24[18] = cheb12[8] - alam;
    for (i__ = 1; i__ <= 6; ++i__) {
	j = 14 - i__;
	v[i__ - 1] = fval[i__] - fval[j];
	fval[i__] += fval[j];
/* L20: */
    }
    alam1 = v[0] + x[8] * v[4];
    alam2 = x[4] * v[2];
    cheb12[3] = alam1 + alam2;
    cheb12[11] = alam1 - alam2;
    cheb12[7] = v[0] - v[4];
    alam = x[2] * v[1] + x[6] * v[3] + x[10] * v[5];
    cheb24[3] = cheb12[3] + alam;
    cheb24[23] = cheb12[3] - alam;
    alam = x[6] * (v[1] - v[3] - v[5]);
    cheb24[7] = cheb12[7] + alam;
    cheb24[19] = cheb12[7] - alam;
    alam = x[10] * v[1] - x[6] * v[3] + x[2] * v[5];
    cheb24[11] = cheb12[11] + alam;
    cheb24[15] = cheb12[11] - alam;
    for (i__ = 1; i__ <= 3; ++i__) {
	j = 8 - i__;
	v[i__ - 1] = fval[i__] - fval[j];
	fval[i__] += fval[j];
/* L30: */
    }
    cheb12[5] = v[0] + x[8] * v[2];
    cheb12[9] = fval[1] - x[8] * fval[3];
    alam = x[4] * v[1];
    cheb24[5] = cheb12[5] + alam;
    cheb24[21] = cheb12[5] - alam;
    alam = x[8] * fval[2] - fval[4];
    cheb24[9] = cheb12[9] + alam;
    cheb24[17] = cheb12[9] - alam;
    cheb12[1] = fval[1] + fval[3];
    alam = fval[2] + fval[4];
    cheb24[1] = cheb12[1] + alam;
    cheb24[25] = cheb12[1] - alam;
    cheb12[13] = v[0] - v[2];
    cheb24[13] = cheb12[13];
    alam = (float).16666666666666666;
    for (i__ = 2; i__ <= 12; ++i__) {
	cheb12[i__] *= alam;
/* L40: */
    }
    alam *= (float).5;
    cheb12[1] *= alam;
    cheb12[13] *= alam;
    for (i__ = 2; i__ <= 24; ++i__) {
	cheb24[i__] *= alam;
/* L50: */
    }
    cheb24[1] = alam * (float).5 * cheb24[1];
    cheb24[25] = alam * (float).5 * cheb24[25];
    return 0;
} /* qcheb_ */

#ifdef __cplusplus
	}
#endif
