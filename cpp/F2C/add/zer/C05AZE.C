/* C05AZE.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "f2c.h"

/* Table of constant values */

static real c_b13 = (float)1.;
static real c_b16 = (float)0.;

/* Subroutine */ int c05aze_(real *x, real *y, real *fx, real *tolx, integer *
	ir, real *c__, integer *ind, integer *ifail)
{
    /* Initialized data */

    static struct {
	char e_1[8];
	doublereal e_2;
	} equiv_11 = { {' ', ' ', 'C', '0', '5', 'A', 'Z', 'E'}, 0. };

#define srname (*(doublereal *)&equiv_11)


    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
    double r_sign(real *, real *), sqrt(doublereal);

    /* Local variables */
    real diff, diff1, diff2;
    extern integer p01aae_(integer *, integer *, doublereal *);
    integer i__;
    extern doublereal x02aae_(real *), x02abe_(real *);
    logical t;
    real ab, rel, max__, tol;

/*     MARK 8 RE-ISSUE. NAG COPYRIGHT 1979. */
/*     .. SCALAR ARGUMENTS .. */
/*     .. ARRAY ARGUMENTS .. */
/*     .. */
/*     .. LOCAL SCALARS .. */
/*     .. FUNCTION REFERENCES .. */
/*     .. */
    /* Parameter adjustments */
    --c__;

    /* Function Body */
    i__ = 0;
    if (*ind > 0 && *ind <= 4 || *ind == -1) {
	goto L20;
    }
/*     USER NOT CHECKED IND OR CHANGED IT */
    i__ = 2;
    *ind = 0;
    goto L640;
L20:
    if (*tolx > (float)0. && (*ir == 0 || *ir == 1 || *ir == 2)) {
	goto L40;
    }
    i__ = 3;
    *ind = 0;
    goto L640;
L40:
    rel = (float)1.;
    ab = (float)1.;
    if (*ir == 1) {
	rel = (float)0.;
    }
    if (*ir == 2) {
	ab = (float)0.;
    }
    if (*ind == -1) {
	goto L80;
    }
    switch (*ind) {
	case 1:  goto L60;
	case 2:  goto L100;
	case 3:  goto L180;
	case 4:  goto L480;
    }
L60:
    c__[3] = *x;
    *ind = 2;
    return 0;
L80:
    c__[3] = *x;
L100:
    if (*fx != (float)0.) {
	goto L140;
    }
L120:
    *y = *x;
    *ind = 0;
    i__ = 0;
    goto L640;
L140:
    c__[4] = *fx;
    c__[15] = dabs(*fx);
    c__[16] = (float)0.;
    *x = *y;
    *y = c__[3];
    c__[2] = c__[4];
    c__[5] = *x;
    if (*ind == -1) {
	goto L160;
    }
    *ind = 3;
    return 0;
L160:
    *fx = c__[1];
    *ind = 3;
L180:
    if (*fx == (float)0.) {
	goto L120;
    }
    if (r_sign(&c_b13, fx) != r_sign(&c_b13, &c__[2])) {
	goto L200;
    }
    *ind = 0;
    i__ = 1;
    goto L640;
L200:
    c__[6] = *fx;
    c__[13] = sqrt(x02aae_(&c_b16));
/* Computing MAX */
    r__1 = c__[15], r__2 = dabs(*fx);
    c__[15] = dmax(r__1,r__2);
    c__[14] = x02abe_(&c_b16);
    c__[16] = (float)0.;
L220:
    c__[1] = c__[5];
    c__[2] = c__[6];
    c__[17] = (float)0.;
L240:
    if (dabs(c__[2]) >= dabs(c__[4])) {
	goto L280;
    }
    if (c__[1] == c__[5]) {
	goto L260;
    }
    c__[7] = c__[5];
    c__[8] = c__[6];
L260:
    c__[5] = c__[3];
    c__[6] = c__[4];
    *x = c__[1];
    c__[3] = *x;
    c__[4] = c__[2];
    c__[1] = c__[5];
    c__[2] = c__[6];
L280:
/* Computing MAX */
    r__1 = ab, r__2 = rel * dabs(c__[3]);
    tol = *tolx * (float).5 * dmax(r__1,r__2);
    diff2 = (c__[1] - c__[3]) * (float).5;
    c__[12] = diff2;
    diff2 += c__[3];
    if (c__[12] == (float)0.) {
	goto L340;
    }
    if (dabs(c__[12]) <= tol) {
	goto L580;
    }
    if (c__[17] < (float)2.5) {
	goto L300;
    }
    c__[11] = c__[12];
    goto L460;
L300:
    tol *= r_sign(&c_b13, &c__[12]);
    diff1 = (c__[3] - c__[5]) * c__[4];
    if (c__[17] > (float)1.5) {
	goto L320;
    }
    diff = c__[6] - c__[4];
    goto L380;
L320:
    if (c__[7] != c__[3] && c__[7] != c__[5]) {
	goto L360;
    }
L340:
    *ind = 0;
    i__ = 5;
    goto L640;
L360:
    c__[9] = (c__[8] - c__[4]) / (c__[7] - c__[3]);
    c__[10] = (c__[8] - c__[6]) / (c__[7] - c__[5]);
    diff1 = c__[10] * diff1;
    diff = c__[9] * c__[6] - c__[10] * c__[4];
L380:
    if (diff1 >= (float)0.) {
	goto L400;
    }
    diff1 = -diff1;
    diff = -diff;
L400:
    if (dabs(diff1) > c__[14] && diff1 > diff * tol) {
	goto L420;
    }
    c__[11] = tol;
    goto L460;
L420:
    if (diff1 >= c__[12] * diff) {
	goto L440;
    }
    c__[11] = diff1 / diff;
    goto L460;
L440:
    c__[11] = c__[12];
L460:
    c__[7] = c__[5];
    c__[8] = c__[6];
    c__[5] = c__[3];
    c__[6] = c__[4];
    c__[3] += c__[11];
    *x = c__[3];
    *y = c__[1];
    *ind = 4;
    return 0;
L480:
    if (*fx == (float)0.) {
	goto L120;
    }
    c__[4] = *fx;
    max__ = dabs(*fx);
    if (c__[13] * max__ <= c__[15]) {
	goto L500;
    }
    if (c__[16] == (float)1.) {
	c__[16] = (float)-1.;
    }
    if (c__[16] == (float)0.) {
	c__[16] = (float)1.;
    }
    goto L520;
L500:
    c__[16] = (float)0.;
L520:
    if (c__[2] >= (float)0.) {
	goto L540;
    }
    t = c__[4] <= (float)0.;
    goto L560;
L540:
    t = c__[4] >= (float)0.;
L560:
    if (t) {
	goto L220;
    }
    i__ = (integer) (c__[17] + (float).1);
    ++i__;
    if (c__[11] == c__[12]) {
	i__ = 0;
    }
    c__[17] = (real) i__;
    goto L240;
L580:
    if (c__[16] >= (float)0.) {
	goto L600;
    }
    i__ = 4;
    goto L620;
L600:
    *y = c__[1];
    i__ = 0;
L620:
    *ind = 0;
L640:
    *ifail = p01aae_(ifail, &i__, &srname);
    return 0;
} /* c05aze_ */

#undef srname


#ifdef __cplusplus
	}
#endif
