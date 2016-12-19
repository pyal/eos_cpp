/* fmin.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__4 = 4;

doublereal fmin_(E_fp f, real *x, real *a, real *b, real *t)
{
    /* System generated locals */
    real ret_val, r__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    real cons, d__, e, m, p, q, r__, u, v, w, t2;
    extern doublereal r1mach_(integer *);
    real sa, sb, fu, fv, fw, fx, tt, tol;


/*  FMIN FINDS AN APPROXIMATION X TO THE POINT IN */
/*  THE INTERVAL (A,B) AT WHICH F ATTAINS ITS MINIMUM, */
/*  AND RETURNS IN FMIN THE VALUE OF F AT X. */

/*  T DETERMINES A TOLERANCE OF */

/*              TOL  =  R1MACH(4) * ABS(X) + T */

/*  AND F IS NEVER EVALUATED AT TWO POINTS CLOSER */
/*  TOGETHER THAN TOL. */

/*  IF T IS INPUT .LE. ZERO, IT IS SET TO 10.*R1MACH(1) */

/*  THE METHOD USED IS A COMBINATION OF GOLDEN SEARCH */
/*  AND SUCCESSIVE PARABOLIC INTERPOLATION. */
/*  CONVERGENCE IS NEVER MUCH SLOWER THAN FOR A */
/*  FIBONACCI SEARCH. */
/*  IF F HAS A CONTINUOUS SECOND DERIVATIVE WHICH IS POSITIVE */
/*  AT THE MINIMUM ( NOT AT A OR B) THEN, IGNORING */
/*  ROUNDING ERRORS, CONVERGENCE IS SUPERLINEAR, */
/*  AND USUALLY THE ORDER IS AT LEAST 1.3247.... */

/*  THIS IS BRENT'S ALGORITHM - SEE PAGE 188 OF HIS BOOK. */

/*  A, STORED IN SA, AND B, STORED IN SB ARE */
/*  AT ANY STEP THE CURRENT BOUNDARIES FOR */
/*  THE INTERVAL CONTAINING THE MINIMUM. */

/*  X IS THE POINT AT WHICH F HAS THE LEAST VALUE */
/*  SO FAR, (OR THE POINT OF MOST RECENT EVALUATION */
/*  IF THERE IS A TIE). */

/*  W IS THE POINT WITH THE NEXT LOWEST VALUE OF F */

/*  V IS THE PREVIOUS VALUE OF W */

/*  U IS THE LAST POINT AT WHICH F HAS BEEN EVALUATED */
/*   (U IS UNDEFINED THE FIRST TIME.) */

    tt = *t;
    if (*t <= (float)0.) {
	tt = r1mach_(&c__1) * (float)10.;
    }
    if (*a < *b) {
	goto L5;
    }
    sa = *b;
    sb = *a;
    goto L8;
L5:
    sa = *a;
    sb = *b;
L8:
    cons = ((float)3. - sqrt((float)5.)) * (float).5;

/*  ARBITRARILY FOR THE FIRST STEP CHOOSE */

/*     X = A + .5(3-SQRT(5))* (B-A) */
    if ((*x<sa) || ( *x>sb))
     *x = sa + cons * (sb - sa);
    w = *x;
    v = w;
    e = (float)0.;
    fx = (*f)(x);
    fw = fx;
    fv = fw;

/*  THE MAIN LOOP STARTS HERE. */

L10:
    m = (sa + sb) * (float).5;
    tol = r1mach_(&c__4) * dabs(*x) + tt;
    t2 = tol * (float)2.;

/*  CHECK THE STOPPING CRITERION: */
/*        (M = MIDPOINT) */
/*  IF ABS(X-M) .LE. (2*TOL - .5(B-A)), */
/*  I.E. IF MAX(X-A, B-X) .LE. 2*TOL, THEN */
/*  THE PROCEDURE TERMINATES WITH X AS THE */
/*  APPROXIMATE POSITION OF THE MINIMUM. */

    if ((r__1 = *x - m, dabs(r__1)) <= t2 - (sb - sa) * (float).5) {
	goto L190;
    }
    r__ = (float)0.;
    q = r__;
    p = q;
    if (dabs(e) <= tol) {
	goto L40;
    }

/*    FIT THE PARABOLA */

/*    Q = 2((X-V)(FX-FW) - (X-W)(FX-FV)) */
/*    P = ((X-V)**2)(FX-FW) - ((X-W))**2)(FX-FV) */

    r__ = (*x - w) * (fx - fv);
    q = (*x - v) * (fx - fw);
    p = (*x - v) * q - (*x - w) * r__;
    q = (q - r__) * (float)2.;
    if (q <= (float)0.) {
	goto L20;
    }
    p = -p;
    goto L30;

L20:
    q = -q;
L30:
    r__ = e;
    e = d__;

/*  HERE E IS THE VALUE OF P/Q AT THE SECOND LAST */
/*  CYCLE; IF ABS(E) .LE. TOL, OR IF Q = 0.0. */
/*  OR IF X+P/Q LIES OUTSIDE OF (A,B), OR */
/*  ABS(P/Q) .GE. .5E, THEN A "GOLDEN */
/*  SECTION" STEP IS PERFORMED (AT 60 BELOW). */

/*  OTHERWISE A PARABOLIC INTERPOLATION */
/*  STEP IS TAKEN */

L40:
    if (dabs(p) >= (r__1 = q * (float).5 * r__, dabs(r__1))) {
	goto L60;
    }
    if (p <= q * (sa - *x) || p >= q * (sb - *x)) {
	goto L60;
    }
    d__ = p / q;
    u = *x + d__;

/*  EXCEPT F MUST NOT BE EVALUATED TOO CLOSE TO A OR B. */

/*  IF THE NEW POINT IS TOO CLOSE JUST PUT */
/*       D = +TOL   IF X.LT.M */
/*       D = -TOL   IF X.GE.M */

    if (u - sa >= t2 && sb - u >= t2) {
	goto L90;
    }
    if (*x >= m) {
	goto L50;
    }
    d__ = tol;
    goto L90;
L50:
    d__ = -tol;
    goto L90;

/*  THIS IS THE "GOLDEN SECTION" STEP: */

/*       U = .5(SQRT(5)-1)X + .5(3-SQRT(5)A   IF X.GE.M */
/*       U = .5(SQRT(5)-1)X + .5(3-SQRT(5)B   IF X.LT.M */

L60:
    if (*x >= m) {
	goto L70;
    }
    e = sb - *x;
    goto L80;
L70:
    e = sa - *x;
L80:
    d__ = cons * e;

/*     U = X+(IF ABS(D).GE.TOL THEN D, */
/*          ELSE IF D.GT.0 THEN TOL     ELSE  -TOL) */

L90:
    if (dabs(d__) < tol) {
	goto L100;
    }
    u = *x + d__;
    goto L120;
L100:
    if (d__ <= (float)0.) {
	goto L110;
    }
    u = *x + tol;
    goto L120;
L110:
    u = *x - tol;

/*  UPDATE EVERYTHING */
/*  IF FU.LE.FX THEN */
/*     BEGIN IF U.LT.X THEN B = X ELSE A = X */
/*     V = W;FV = FW;W = X;FW = FX;X = U;FX = FU */
/*     END */
/*  ELSE */
/*     BEGIN IF U.LT.X THEN A = U ELSE B = U */
/*     IF FU.LE.FW OR W = X THEN */
/*          BEGIN V = W;FV = FW;W = X;FW = FU END */
/*     ELSE IF FU.LE.FV OR V = X OR V = W THEN */
/*            BEGIN V = U; FV = FU */
/*            END */
/*     END */

L120:
    fu = (*f)(&u);
    if (fu > fx) {
	goto L150;
    }
    if (u >= *x) {
	goto L130;
    }
    sb = *x;
    goto L140;
L130:
    sa = *x;
L140:
    v = w;
    fv = fw;
    w = *x;
    fw = fx;
    *x = u;
    fx = fu;
    goto L10;

L150:
    if (u >= *x) {
	goto L160;
    }
    sa = u;
    goto L170;
L160:
    sb = u;
L170:
    if (fu > fw && w != *x) {
	goto L180;
    }
    v = w;
    fv = fw;
    w = u;
    fw = fu;
    goto L10;

L180:
    if (fu > fv && v != *x && v != w) {
	goto L10;
    }
    v = u;
    fv = fu;
    goto L10;

L190:
    ret_val = fx;
    return ret_val;
} /* fmin_ */

#ifdef __cplusplus
	}
#endif
