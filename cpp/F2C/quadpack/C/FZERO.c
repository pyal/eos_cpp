/* FZERO.F -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Table of constant values */

static integer c__4 = 4;
static real c_b3 = (float)1.;

/* DECK FZERO */
/* Subroutine */ int fzero_(E_fp f, real *b, real *c__, real *r__, real *re, 
	real *ae, integer *iflag, integer *MaxIter, integer *WasIter)
{
    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
    double r_sign(real *, real *);

    /* Local variables */
    real acmb, acbs, a, p, q, t, z__;
    integer kount;
    extern doublereal r1mach_(integer *);
    real fa, fb, fc;
    integer ic;
    real aw, er, fx, fz, rw, cmb, tol;

/* ***BEGIN PROLOGUE  FZERO */
/* ***PURPOSE  Search for a zero of a function F(X) in a given interval */
/*            (B,C).  It is designed primarily for problems where F(B) */
/*            and F(C) have opposite signs. */
/* ***LIBRARY   SLATEC */
/* ***CATEGORY  F1B */
/* ***TYPE      SINGLE PRECISION (FZERO-S, DFZERO-D) */
/* ***KEYWORDS  BISECTION, NONLINEAR EQUATIONS, ROOTS, ZEROS */
/* ***AUTHOR  Shampine, L. F., (SNLA) */
/*           Watts, H. A., (SNLA) */
/* ***DESCRIPTION */

/*     FZERO searches for a zero of a REAL function F(X) between the */
/*     given REAL values B and C until the width of the interval (B,C) */
/*     has collapsed to within a tolerance specified by the stopping */
/*     criterion, */
/*        ABS(B-C) .LE. 2.*(RW*ABS(B)+AE). */
/*     The method used is an efficient combination of bisection and the */
/*     secant rule and is due to T. J. Dekker. */

/*     Description Of Arguments */

/*   F     :EXT   - Name of the REAL external function.  This name must */
/*                  be in an EXTERNAL statement in the calling program. */
/*                  F must be a function of one REAL argument. */

/*   B     :INOUT - One end of the REAL interval (B,C).  The value */
/*                  returned for B usually is the better approximation */
/*                  to a zero of F. */

/*   C     :INOUT - The other end of the REAL interval (B,C) */

/*   R     :OUT   - A (better) REAL guess of a zero of F which could help */
/*                  in speeding up convergence.  If F(B) and F(R) have */
/*                  opposite signs, a root will be found in the interval */
/*                  (B,R); if not, but F(R) and F(C) have opposite signs, */
/*                  a root will be found in the interval (R,C); */
/*                  otherwise, the interval (B,C) will be searched for a */
/*                  possible root.  When no better guess is known, it is */
/*                  recommended that r be set to B or C, since if R is */
/*                  not interior to the interval (B,C), it will be */
/*                  ignored. */

/*   RE    :IN    - Relative error used for RW in the stopping criterion. */
/*                  If the requested RE is less than machine precision, */
/*                  then RW is set to approximately machine precision. */

/*   AE    :IN    - Absolute error used in the stopping criterion.  If */
/*                  the given interval (B,C) contains the origin, then a */
/*                  nonzero value should be chosen for AE. */

/*   IFLAG :OUT   - A status code.  User must check IFLAG after each */
/*                  call.  Control returns to the user from FZERO in all */
/*                  cases. */

/*                1  B is within the requested tolerance of a zero. */
/*                   The interval (B,C) collapsed to the requested */
/*                   tolerance, the function changes sign in (B,C), and */
/*                   F(X) decreased in magnitude as (B,C) collapsed. */

/*                2  F(B) = 0.  However, the interval (B,C) may not have */
/*                   collapsed to the requested tolerance. */

/*                3  B may be near a singular point of F(X). */
/*                   The interval (B,C) collapsed to the requested tol- */
/*                   erance and the function changes sign in (B,C), but */
/*                   F(X) increased in magnitude as (B,C) collapsed, i.e. */
/*                     ABS(F(B out)) .GT. MAX(ABS(F(B in)),ABS(F(C in))) */

/*                4  No change in sign of F(X) was found although the */
/*                   interval (B,C) collapsed to the requested tolerance. */
/*                   The user must examine this case and decide whether */
/*                   B is near a local minimum of F(X), or B is near a */
/*                   zero of even multiplicity, or neither of these. */

/*                5  Too many (.GT. 500) function evaluations used. */

/* ***REFERENCES  L. F. Shampine and H. A. Watts, FZERO, a root-solving */
/*                 code, Report SC-TM-70-631, Sandia Laboratories, */
/*                 September 1970. */
/*               T. J. Dekker, Finding a zero by means of successive */
/*                 linear interpolation, Constructive Aspects of the */
/*                 Fundamental Theorem of Algebra, edited by B. Dejon */
/*                 and P. Henrici, Wiley-Interscience, 1969. */
/* ***ROUTINES CALLED  R1MACH */
/* ***REVISION HISTORY  (YYMMDD) */
/*   700901  DATE WRITTEN */
/*   890531  Changed all specific intrinsics to generic.  (WRB) */
/*   890531  REVISION DATE from Version 3.2 */
/*   891214  Prologue converted to Version 4.0 format.  (BAB) */
/*   920501  Reformatted the REFERENCES section.  (WRB) */
/* ***END PROLOGUE  FZERO */
/* ***FIRST EXECUTABLE STATEMENT  FZERO */

/*   ER is two times the computer unit roundoff value which is defined */
/*   here by the function R1MACH. */

    er = r1mach_(&c__4) * (float)2.;

/*   Initialize. */

    z__ = *r__;
    if (*r__ <= dmin(*b,*c__) || *r__ >= dmax(*b,*c__)) {
	z__ = *c__;
    }
    rw = dmax(*re,er);
    aw = dmax(*ae,(float)0.);
    ic = 0;
    t = z__;
    fz = (*f)(&t);
    fc = fz;
    t = *b;
    fb = (*f)(&t);
    kount = 2;
    if (r_sign(&c_b3, &fz) == r_sign(&c_b3, &fb)) {
	goto L1;
    }
    *c__ = z__;
    goto L2;
L1:
    if (z__ == *c__) {
	goto L2;
    }
    t = *c__;
    fc = (*f)(&t);
    kount = 3;
    if (r_sign(&c_b3, &fz) == r_sign(&c_b3, &fc)) {
	goto L2;
    }
    *b = z__;
    fb = fz;
L2:
    a = *c__;
    fa = fc;
    acbs = (r__1 = *b - *c__, dabs(r__1));
/* Computing MAX */
    r__1 = dabs(fb), r__2 = dabs(fc);
    fx = dmax(r__1,r__2);

L3:
    if (dabs(fc) >= dabs(fb)) {
	goto L4;
    }

/*   Perform interchange. */

    a = *b;
    fa = fb;
    *b = *c__;
    fb = fc;
    *c__ = a;
    fc = fa;

L4:
    cmb = (*c__ - *b) * (float).5;
    acmb = dabs(cmb);
    tol = rw * dabs(*b) + aw;

/*   Test stopping criterion and function count. */

    if (acmb <= tol) {
	goto L10;
    }
    if (fb == (float)0.) {
	goto L11;
    }
    if (kount >= *MaxIter) {
	goto L14;
    }

/*   Calculate new iterate implicitly as B+P/Q, where we arrange */
/*   P .GE. 0.  The implicit form is used to prevent overflow. */

    p = (*b - a) * fb;
    q = fa - fb;
    if (p >= (float)0.) {
	goto L5;
    }
    p = -p;
    q = -q;

/*   Update A and check for satisfactory reduction in the size of the */
/*   bracketing interval.  If not, perform bisection. */

L5:
    a = *b;
    fa = fb;
    ++ic;
    if (ic < 4) {
	goto L6;
    }
    if (acmb * (float)8. >= acbs) {
	goto L8;
    }
    ic = 0;
    acbs = acmb;

/*   Test for too small a change. */

L6:
    if (p > dabs(q) * tol) {
	goto L7;
    }

/*   Increment by TOLerance. */

    *b += r_sign(&tol, &cmb);
    goto L9;

/*   Root ought to be between B and (C+B)/2. */

L7:
    if (p >= cmb * q) {
	goto L8;
    }

/*   Use secant rule. */

    *b += p / q;
    goto L9;

/*   Use bisection (C+B)/2. */

L8:
    *b += cmb;

/*   Have completed computation for new iterate B. */

L9:
    t = *b;
    fb = (*f)(&t);
    ++kount;

/*   Decide whether next step is interpolation or extrapolation. */

    if (r_sign(&c_b3, &fb) != r_sign(&c_b3, &fc)) {
	goto L3;
    }
    *c__ = a;
    fc = fa;
    goto L3;

/*   Finished.  Process results for proper setting of IFLAG. */

L10:
    if (r_sign(&c_b3, &fb) == r_sign(&c_b3, &fc)) {
	goto L13;
    }
    if (dabs(fb) > fx) {
	goto L12;
    }
    *iflag = 1;*WasIter=kount;
    return 0;
L11:
    *iflag = 2;*WasIter=kount;
    return 0;
L12:
    *iflag = 3;*WasIter=kount;
    return 0;
L13:
    *iflag = 4;*WasIter=kount;
    return 0;
L14:
    *iflag = 5;*WasIter=kount;
    return 0;
} /* fzero_ */

#ifdef __cplusplus
	}
#endif
