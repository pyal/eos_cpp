/* C05AXE.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "f2c.h"

/* Subroutine */ int c05axe_(real *x, real *fx, real *tol, integer *ir, real *
	scale, real *c__, integer *ind, integer *ifail)
{


    /* System generated locals */
    real r__1, r__2;


    integer i__;
    real oldth, ab, se, sf, rel;

/*     MARK 8 RELEASE. NAG COPYRIGHT 1979. */
/*     MARK 9 REVISED. IER-301 (SEP 1981). */
/*     USES METHOD OF OF SWIFT AND LINDFIELD, */
/*     C.J.VOL 21. MINK=C(1),MAXK=C(2). */
/*     .. SCALAR ARGUMENTS .. */
/*     .. ARRAY ARGUMENTS .. */
/*     .. */
/*     .. LOCAL SCALARS .. */
/*     .. FUNCTION REFERENCES .. */
/*     .. */
    /* Parameter adjustments */
    --c__;

    /* Function Body */
    if (*tol <= (float)0.) {
	goto L380;
    }
    if (*x + *scale == *x) {
	goto L420;
    }
    if (*ind < -1 || *ind > 4) {
	goto L400;
    }
    if (*ir < 0 || *ir > 2) {
	goto L380;
    }
    ab = (float)1.;
    rel = (float)1.;
    if (*ir == 1) {
	rel = (float)0.;
    }
    if (*ir == 2) {
	ab = (float)0.;
    }
    i__ = *ind + 2;
    switch (i__) {
	case 1:  goto L40;
	case 2:  goto L400;
	case 3:  goto L20;
	case 4:  goto L40;
	case 5:  goto L80;
	case 6:  goto L260;
    }
L20:
    *ind = 2;
    return 0;
L40:
    if (*fx == (float)0.) {
	goto L520;
    }
    c__[1] = (float)3.;
    c__[2] = (float)8.;
    c__[4] = (float)1.;
    c__[6] = (float)1.;
    c__[7] = (float).5;
    c__[8] = *fx;
    c__[22] = *x;
    c__[25] = MIN_EPS;
    c__[19] = sqrt(c__[25]);
    c__[5] = (float)1. - c__[19] * (float).5;
    c__[23] = c__[19] * *scale;
    c__[20] = (float)0.;
L60:
    *x = c__[22] + c__[23];
    if (*x == c__[22]) {
	goto L100;
    }
    *ind = 3;
    return 0;
L80:
    if (*fx == (float)0.) {
	goto L520;
    }
    c__[24] = *fx - c__[8];
    *x = c__[22];
/* Computing MAX */
    r__1 = dabs(*fx), r__2 = dabs(c__[8]);
    if (dabs(c__[24]) > c__[25] * (float)100. * dmax(r__1,r__2)) {
	goto L120;
    }
L100:
    c__[23] *= (float)10.;
    if (dabs(c__[23]) > dabs(*scale)) {
	goto L420;
    }
    goto L60;
L120:
    c__[12] = c__[23] / c__[24];

/*     CONTINUATION OUTER LOOP */

L140:
    c__[14] = sqrt(*tol);
    if (c__[5] == (float)0.) {
	c__[14] = *tol;
    }
    c__[3] = (float)0.;
    c__[9] = *x;
    c__[13] = c__[12];
    c__[15] = c__[6] - c__[5];
    if (c__[15] < c__[25] * (float)100.) {
	goto L440;
    }
    c__[21] = c__[8] * c__[5];
    c__[10] = c__[15] * c__[8];
    c__[11] = c__[10];
    c__[26] = (float)0.;

/*     CONTINUATION INNER LOOP */

L160:
    c__[17] = c__[12] * c__[10];
    sf = c__[26];
    c__[26] = (float)0.;
/* Computing MAX */
    r__1 = rel * dabs(*x);
    if (dabs(c__[17]) < c__[14] * dmax(r__1,ab)) {
	c__[26] = sf + (float)1.;
    }
    if (c__[26] > (float)0. && dabs(c__[10]) < dabs(c__[11]) && c__[3] > (
	    float)1.5) {
	goto L280;
    }
    if ((r__1 = c__[3] - c__[2], dabs(r__1)) > (float).5) {
	goto L240;
    }
    if (c__[26] > c__[2] - (float)2.5) {
	goto L280;
    }
    se = (c__[10] + c__[21]) / c__[8];
    if (se > c__[5] && se < c__[6]) {
	goto L200;
    }
    c__[12] = c__[13];
    c__[7] *= (float).5;
    if (c__[6] - c__[5] > c__[25] * (float)100.) {
	goto L180;
    }
    if (c__[4] < (float)1.5) {
	goto L440;
    }
    goto L340;
L180:
    c__[5] = (c__[5] + c__[6]) * (float).5;
    *x = c__[9];
    goto L140;
L200:
    c__[21] = se * c__[8];
    c__[15] = c__[6] - se;
    if (c__[5] != (float)0.) {
	goto L220;
    }
    if (c__[15] < c__[25] * (float)100.) {
	goto L440;
    }
L220:
    c__[5] = se;
    goto L280;
L240:
    *x -= c__[17];
    *ind = 4;
    return 0;
L260:
    if (*fx == (float)0.) {
	goto L520;
    }
    se = *fx - c__[21];
    sf = se - c__[10];
/* Computing MAX */
    r__1 = dabs(se), r__2 = dabs(c__[10]), r__1 = max(r__1,r__2), r__2 = dabs(
	    c__[21]);
    if (dabs(sf) <= c__[25] * (float)2. * dmax(r__1,r__2)) {
	goto L280;
    }
    c__[10] = se;
    c__[3] += (float)1.;
    if (sf == (float)0.) {
	goto L160;
    }
    c__[12] = -c__[17] / sf;
    goto L160;

/*     SUCCESSFUL INNER LOOP */

L280:
    if (c__[5] == (float)0.) {
	goto L520;
    }
    c__[17] = c__[9] - *x;
    oldth = c__[6];
    c__[6] = c__[5];
    if ((r__1 = c__[4] - (float)1., dabs(r__1)) > (float).5) {
	goto L300;
    }
    c__[5] = (float)1. - c__[19];
    goto L360;
L300:
    sf = c__[17] / c__[15];
    se = (c__[18] / c__[16] - sf) / (c__[15] + c__[16]);
    if (dabs(sf) * c__[19] > dabs(c__[15]) || se == (float)0. || dabs(se) * 
	    c__[19] > (r__1 = c__[15] + c__[16], dabs(r__1))) {
	goto L310;
    }
    if (c__[3] <= c__[1] + (float).5 && c__[4] > (float)2.5) {
	c__[7] *= (float)2.;
    }
    if (dabs(se) * c__[5] > c__[7] * (r__1 = sf - se * c__[15], dabs(r__1))) {
	goto L320;
    }
    c__[5] = (float)0.;
    goto L360;
L310:
    if (c__[5] <= (float).9) {
	goto L315;
    }
    c__[5] = (float).9;
    goto L330;
L315:
    c__[5] *= (float).5;
    goto L330;
L320:
    c__[5] -= c__[7] * (r__1 = sf / se - c__[15], dabs(r__1));
    if (c__[5] < (float)0.) {
	c__[5] = (float)0.;
    }
    if (c__[5] == (float)0.) {
	goto L360;
    }
L330:
    if (oldth - c__[5] >= c__[25] * (float)100. && c__[5] >= c__[19]) {
	goto L360;
    }
L340:
    if (c__[4] > (float)2.5) {
	goto L440;
    }
    c__[19] *= (float).1;
    if (c__[19] < c__[25] * (float)100.) {
	goto L480;
    }
    c__[6] = (float)1.;
    c__[5] = (float)1. - c__[19] * (float).5;
    c__[20] = (float)0.;
    *x = c__[9];
    c__[4] = (float)1.;
    c__[7] = (float).5;
    goto L140;
L360:
    c__[16] = c__[15];
    c__[18] = c__[17];
    c__[4] += (float)1.;
    c__[20] = (float)1.;
    if (c__[5] == (float)0.) {
	c__[20] = (float)2.;
    }
    goto L140;
/*     INPUT ERROR */
L380:
    i__ = 1;
    goto L540;
/*     WRONG IND ON ENTRY */
L400:
    i__ = 2;
    goto L540;
/*     WRONG SCALE */
L420:
    i__ = 3;
    goto L540;
/*     SINGULARITY ON CONTINUATION PATH */
L440:
    if (c__[20] != (float)1.) {
	goto L460;
    }
    i__ = 4;
    goto L540;
/*     CANNOT GET STARTED */
L460:
    if (c__[20] != (float)0.) {
	goto L500;
    }
L480:
    i__ = 5;
    goto L540;
/*     CANNOT FINISH */
L500:
    i__ = 6;
    goto L540;
/*     FINISHED */
L520:
    i__ = 0;
L540:
    *ind = 0;
    *ifail = 1;
    return 0;
} /* c05axe_ */

#undef srname


#ifdef __cplusplus
	}
#endif
