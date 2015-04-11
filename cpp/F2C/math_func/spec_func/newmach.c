/* newmach.f -- translated by f2c (version 20060506).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "lib/std/f2c.h"

/* Subroutine */ int machar_(integer *ibeta, integer *it, integer *irnd, 
	integer *ngrd, integer *machep, integer *negep, integer *iexp, 
	integer *minexp, integer *maxexp, doublereal *eps, doublereal *epsneg,
	 doublereal *xmin, doublereal *xmax)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal a, b;
    static integer i__, j, k;
    static doublereal t, y, z__;
    static integer iz, mx;
    static doublereal one, two, beta, temp, zero, temp1, betah, tempa;
    static integer itemp, nxres;
    static doublereal betain;

/* ---------------------------------------------------------------------- */
/*  This Fortran 77 subroutine is intended to determine the parameters */
/*   of the floating-point arithmetic system specified below.  The */
/*   determination of the first three uses an extension of an algorithm */
/*   due to M. Malcolm, CACM 15 (1972), pp. 949-951, incorporating some, */
/*   but not all, of the improvements suggested by M. Gentleman and S. */
/*   Marovich, CACM 17 (1974), pp. 276-277.  An earlier version of this */
/*   program was published in the book Software Manual for the */
/*   Elementary Functions by W. J. Cody and W. Waite, Prentice-Hall, */
/*   Englewood Cliffs, NJ, 1980. */

/*  The program as given here must be modified before compiling.  If */
/*   a single (double) precision version is desired, change all */
/*   occurrences of CS (CD) in columns 1 and 2 to blanks. */

/*  Parameter values reported are as follows: */

/*       IBETA   - the radix for the floating-point representation */
/*       IT      - the number of base IBETA digits in the floating-point */
/*                 significand */
/*       IRND    - 0 if floating-point addition chops */
/*                 1 if floating-point addition rounds, but not in the */
/*                   IEEE style */
/*                 2 if floating-point addition rounds in the IEEE style */
/*                 3 if floating-point addition chops, and there is */
/*                   partial underflow */
/*                 4 if floating-point addition rounds, but not in the */
/*                   IEEE style, and there is partial underflow */
/*                 5 if floating-point addition rounds in the IEEE style, */
/*                   and there is partial underflow */
/*       NGRD    - the number of guard digits for multiplication with */
/*                 truncating arithmetic.  It is */
/*                 0 if floating-point arithmetic rounds, or if it */
/*                   truncates and only  IT  base  IBETA digits */
/*                   participate in the post-normalization shift of the */
/*                   floating-point significand in multiplication; */
/*                 1 if floating-point arithmetic truncates and more */
/*                   than  IT  base  IBETA  digits participate in the */
/*                   post-normalization shift of the floating-point */
/*                   significand in multiplication. */
/*       MACHEP  - the largest negative integer such that */
/*                 1.0+FLOAT(IBETA)**MACHEP .NE. 1.0, except that */
/*                 MACHEP is bounded below by  -(IT+3) */
/*       NEGEPS  - the largest negative integer such that */
/*                 1.0-FLOAT(IBETA)**NEGEPS .NE. 1.0, except that */
/*                 NEGEPS is bounded below by  -(IT+3) */
/*       IEXP    - the number of bits (decimal places if IBETA = 10) */
/*                 reserved for the representation of the exponent */
/*                 (including the bias or sign) of a floating-point */
/*                 number */
/*       MINEXP  - the largest in magnitude negative integer such that */
/*                 FLOAT(IBETA)**MINEXP is positive and normalized */
/*       MAXEXP  - the smallest positive power of  BETA  that overflows */
/*       EPS     - FLOAT(IBETA)**MACHEP. */
/*       EPSNEG  - FLOAT(IBETA)**NEGEPS. */
/*       XMIN    - the smallest non-vanishing normalized floating-point */
/*                 power of the radix, i.e.,  XMIN = FLOAT(IBETA)**MINEXP */
/*       XMAX    - the largest finite floating-point number.  In */
/*                 particular  XMAX = (1.0-EPSNEG)*FLOAT(IBETA)**MAXEXP */
/*                 Note - on some machines  XMAX  will be only the */
/*                 second, or perhaps third, largest number, being */
/*                 too small by 1 or 2 units in the last digit of */
/*                 the significand. */

/*  Latest modification: May 30, 1989 */

/*  Author: W. J. Cody */
/*          Mathematics and Computer Science Division */
/*          Argonne National Laboratory */
/*          Argonne, IL 60439 */

/* ---------------------------------------------------------------------- */
/* S    REAL */
/* ---------------------------------------------------------------------- */
/* S    CONV(I) = REAL(I) */
    one = 1.;
    two = one + one;
    zero = one - one;
/* ---------------------------------------------------------------------- */
/*  Determine IBETA, BETA ala Malcolm. */
/* ---------------------------------------------------------------------- */
    a = one;
L10:
    a += a;
    temp = a + one;
    temp1 = temp - a;
    if (temp1 - one == zero) {
	goto L10;
    }
    b = one;
L20:
    b += b;
    temp = a + b;
    itemp = (integer) (temp - a);
    if (itemp == 0) {
	goto L20;
    }
    *ibeta = itemp;
    beta = (doublereal) (*ibeta);
/* ---------------------------------------------------------------------- */
/*  Determine IT, IRND. */
/* ---------------------------------------------------------------------- */
    *it = 0;
    b = one;
L100:
    ++(*it);
    b *= beta;
    temp = b + one;
    temp1 = temp - b;
    if (temp1 - one == zero) {
	goto L100;
    }
    *irnd = 0;
    betah = beta / two;
    temp = a + betah;
    if (temp - a != zero) {
	*irnd = 1;
    }
    tempa = a + beta;
    temp = tempa + betah;
    if (*irnd == 0 && temp - tempa != zero) {
	*irnd = 2;
    }
/* ---------------------------------------------------------------------- */
/*  Determine NEGEP, EPSNEG. */
/* ---------------------------------------------------------------------- */
    *negep = *it + 3;
    betain = one / beta;
    a = one;
    i__1 = *negep;
    for (i__ = 1; i__ <= i__1; ++i__) {
	a *= betain;
/* L200: */
    }
    b = a;
L210:
    temp = one - a;
    if (temp - one != zero) {
	goto L220;
    }
    a *= beta;
    --(*negep);
    goto L210;
L220:
    *negep = -(*negep);
    *epsneg = a;
/* ---------------------------------------------------------------------- */
/*  Determine MACHEP, EPS. */
/* ---------------------------------------------------------------------- */
    *machep = -(*it) - 3;
    a = b;
L300:
    temp = one + a;
    if (temp - one != zero) {
	goto L320;
    }
    a *= beta;
    ++(*machep);
    goto L300;
L320:
    *eps = a;
/* ---------------------------------------------------------------------- */
/*  Determine NGRD. */
/* ---------------------------------------------------------------------- */
    *ngrd = 0;
    temp = one + *eps;
    if (*irnd == 0 && temp * one - one != zero) {
	*ngrd = 1;
    }
/* ---------------------------------------------------------------------- */
/*  Determine IEXP, MINEXP, XMIN. */

/*  Loop to determine largest I and K = 2**I such that */
/*         (1/BETA) ** (2**(I)) */
/*  does not underflow. */
/*  Exit from loop is signaled by an underflow. */
/* ---------------------------------------------------------------------- */
    i__ = 0;
    k = 1;
    z__ = betain;
    t = one + *eps;
    nxres = 0;
L400:
    y = z__;
    z__ = y * y;
/* ---------------------------------------------------------------------- */
/*  Check for underflow here. */
/* ---------------------------------------------------------------------- */
    a = z__ * one;
    temp = z__ * t;
    if (a + a == zero || abs(z__) >= y) {
	goto L410;
    }
    temp1 = temp * betain;
    if (temp1 * beta == z__) {
	goto L410;
    }
    ++i__;
    k += k;
    goto L400;
L410:
    if (*ibeta == 10) {
	goto L420;
    }
    *iexp = i__ + 1;
    mx = k + k;
    goto L450;
/* ---------------------------------------------------------------------- */
/*  This segment is for decimal machines only. */
/* ---------------------------------------------------------------------- */
L420:
    *iexp = 2;
    iz = *ibeta;
L430:
    if (k < iz) {
	goto L440;
    }
    iz *= *ibeta;
    ++(*iexp);
    goto L430;
L440:
    mx = iz + iz - 1;
/* ---------------------------------------------------------------------- */
/*  Loop to determine MINEXP, XMIN. */
/*  Exit from loop is signaled by an underflow. */
/* ---------------------------------------------------------------------- */
L450:
    *xmin = y;
    y *= betain;
/* ---------------------------------------------------------------------- */
/*  Check for underflow here. */
/* ---------------------------------------------------------------------- */
    a = y * one;
    temp = y * t;
    if (a + a == zero || abs(y) >= *xmin) {
	goto L460;
    }
    ++k;
    temp1 = temp * betain;
    if (temp1 * beta != y || temp == y) {
	goto L450;
    } else {
	nxres = 3;
	*xmin = y;
    }
L460:
    *minexp = -k;
/* ---------------------------------------------------------------------- */
/*  Determine MAXEXP, XMAX. */
/* ---------------------------------------------------------------------- */
    if (mx > k + k - 3 || *ibeta == 10) {
	goto L500;
    }
    mx += mx;
    ++(*iexp);
L500:
    *maxexp = mx + *minexp;
/* ---------------------------------------------------------------------- */
/*  Adjust IRND to reflect partial underflow. */
/* ---------------------------------------------------------------------- */
    *irnd += nxres;
/* ---------------------------------------------------------------------- */
/*  Adjust for IEEE-style machines. */
/* ---------------------------------------------------------------------- */
    if (*irnd >= 2) {
	*maxexp += -2;
    }
/* ---------------------------------------------------------------------- */
/*  Adjust for machines with implicit leading bit in binary */
/*  significand, and machines with radix point at extreme */
/*  right of significand. */
/* ---------------------------------------------------------------------- */
    i__ = *maxexp + *minexp;
    if (*ibeta == 2 && i__ == 0) {
	--(*maxexp);
    }
    if (i__ > 20) {
	--(*maxexp);
    }
    if (a != y) {
	*maxexp += -2;
    }
    *xmax = one - *epsneg;
    if (*xmax * one != *xmax) {
	*xmax = one - beta * *epsneg;
    }
    *xmax /= beta * beta * beta * *xmin;
    i__ = *maxexp + *minexp + 3;
    if (i__ <= 0) {
	goto L520;
    }
    i__1 = i__;
    for (j = 1; j <= i__1; ++j) {
	if (*ibeta == 2) {
	    *xmax += *xmax;
	}
	if (*ibeta != 2) {
	    *xmax *= beta;
	}
/* L510: */
    }
L520:
    return 0;
/* ---------- Last line of MACHAR ---------- */
} /* machar_ */

