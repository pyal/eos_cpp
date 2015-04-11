/* gamtst.f -- translated by f2c (version 20060506).
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

/* Table of constant values */

static integer c__1 = 1;

/* ------------------------------------------------------------------ */
/* FORTRAN 77 program to test GAMMA or DGAMMA */

/*   Method: */

/*      Accuracy tests compare function values against values */
/*      generated with the duplication formula. */

/*   Data required */

/*      None */

/*   Subprograms required from this package */

/*     MACHAR - An environmental inquiry program providing */
/*              information on the floating-point arithmetic */
/*              system.  Note that the call to MACHAR can */
/*              be deleted provided the following five */
/*              parameters are assigned the values indicated */

/*              IBETA  - The radix of the floating-point system */
/*              IT     - The number of base-ibeta digits in the */
/*                       significant of a floating-point number */
/*              EPS    - The smallest positive floating-point */
/*                       number such that 1.0+EPS .NE. 1.0 */
/*              XMIN   - The smallest non-vanishing floating-point */
/*                       integral power of the radix */
/*              XMAX   - The largest finite floating-point number */

/*     REN(K) - A function subprogram returning random real */
/*              numbers uniformly distributed over (0,1) */


/*  Intrinsic functions required are: */

/*      ABS, DBLE, INT, LOG, MAX, REAL, SQRT */

/*  Reference: "Performance evaluation of programs related */
/*              to the real gamma function", W. J. Cody, */
/*              submitted for publication. */

/*  Latest modification: April 13, 1989 */

/*  Authors: W. J. Cody and L. Stoltz */
/*           Mathematics and Computer Science Division */
/*           Argonne National Laboratory */
/*           Argonne, IL 60439 */

/* -------------------------------------------------------------------- */
/* Main program */ int MAIN__(void)
{
    /* Initialized data */

    static doublereal c1 = .28209479177387814347;
    static integer iout = 6;
    static doublereal c2 = .91893853320467274178;
    static doublereal zero = 0.;
    static doublereal half = .5;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal ten = 10.;
    static doublereal x99 = -999.;
    static doublereal xp99 = .99;

    /* Format strings */
    static char fmt_1000[] = "(\0021Test of GAMMA(X) vs Duplication Formul"
	    "a\002//)";
    static char fmt_1010[] = "(i7,\002 Random arguments were tested from the"
	    " interval (\002,f7.3,\002,\002,f7.3,\002)\002//)";
    static char fmt_1011[] = "(\002 GAMMA(X) was larger\002,i6,\002 times"
	    ",\002/13x,\002 agreed\002,i6,\002 times, and\002/9x,\002was smal"
	    "ler\002,i6,\002 times.\002//)";
    static char fmt_1020[] = "(\002 There are\002,i4,\002 base\002,i4,\002 s"
	    "ignificant digits in a floating-point number\002//)";
    static char fmt_1021[] = "(\002 The maximum relative error of\002,e15.4"
	    ",\002 = \002,i4,\002 **\002,f7.2/4x,\002occurred for X =\002,e13"
	    ".6)";
    static char fmt_1022[] = "(\002 The estimated loss of base\002,i4,\002 s"
	    "ignificant digits is\002,f7.2//)";
    static char fmt_1023[] = "(\002 The root mean square relative error wa"
	    "s\002,e15.4,\002 = \002,i4,\002 **\002,f7.2)";
    static char fmt_1025[] = "(\0021Special Tests\002//)";
    static char fmt_1040[] = "(//\002 Test of special arguments\002//)";
    static char fmt_1041[] = "(\002 GAMMA (\002,e13.6,\002) = \002,e13.6//)";
    static char fmt_1050[] = "(\0021Test of Error Returns\002///)";
    static char fmt_1052[] = "(\002 GAMMA will be called with the argumen"
	    "t\002,e13.6,/\002 This should trigger an error message\002//)";
    static char fmt_1061[] = "(\002 GAMMA returned the value\002,e13.6///)";
    static char fmt_1100[] = "(\002 This concludes the tests\002)";

    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double log(doublereal), pow_di(doublereal *, integer *), pow_dd(
	    doublereal *, doublereal *), sqrt(doublereal);
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static doublereal a, b, c__;
    static integer i__, j, n;
    static doublereal w, x, y, z__;
    static integer k1, k2, k3;
    static doublereal r6, r7, cl, xc;
    static integer it, jt;
    static doublereal xl;
    static integer nx;
    static doublereal xn, xp, zz, del, ait;
    extern doublereal ren_(integer *);
    static doublereal eps, xph, xxn, beta;
    static integer ngrd, irnd;
    static doublereal alnx;
    static integer iexp;
    static doublereal xmin, xmax, xnum;
    static integer ibeta, negep;
    static doublereal xminv;
    extern doublereal dgamma_(doublereal *);
    static doublereal albeta;
    extern /* Subroutine */ int machar_(integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static integer machep;
    static doublereal epsneg;
    static integer minexp, maxexp;

    /* Fortran I/O blocks */
    static cilist io___55 = { 0, 0, 0, fmt_1000, 0 };
    static cilist io___56 = { 0, 0, 0, fmt_1010, 0 };
    static cilist io___57 = { 0, 0, 0, fmt_1011, 0 };
    static cilist io___58 = { 0, 0, 0, fmt_1020, 0 };
    static cilist io___59 = { 0, 0, 0, fmt_1021, 0 };
    static cilist io___60 = { 0, 0, 0, fmt_1022, 0 };
    static cilist io___61 = { 0, 0, 0, fmt_1023, 0 };
    static cilist io___62 = { 0, 0, 0, fmt_1022, 0 };
    static cilist io___63 = { 0, 0, 0, fmt_1025, 0 };
    static cilist io___64 = { 0, 0, 0, fmt_1040, 0 };
    static cilist io___66 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___67 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___68 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___69 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___70 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___71 = { 0, 0, 0, fmt_1050, 0 };
    static cilist io___72 = { 0, 0, 0, fmt_1052, 0 };
    static cilist io___73 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___74 = { 0, 0, 0, fmt_1052, 0 };
    static cilist io___75 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___76 = { 0, 0, 0, fmt_1052, 0 };
    static cilist io___77 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___78 = { 0, 0, 0, fmt_1052, 0 };
    static cilist io___79 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___80 = { 0, 0, 0, fmt_1100, 0 };


/* S    REAL  GAMMA, */
/* S    DATA C1,C2/2.8209479177387814347E-1,9.1893853320467274178E-1/, */
/* S   1     ZERO,HALF,ONE,TWO,TEN/0.0E0,0.5E0,1.0E0,2.0E0,10.0E0/, */
/* S   2     X99,XP99/-999.0E0,0.99E0/ */
/* ------------------------------------------------------------------ */
/*  Statement functions for conversion */
/* ------------------------------------------------------------------ */
/* S    CONV(I) = REAL(I) */
/* S    FUNC(X) = GAMMA(X) */
/* ------------------------------------------------------------------ */
/*  Determine machine parameters and set constants */
/* ------------------------------------------------------------------ */
    machar_(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &
	    maxexp, &eps, &epsneg, &xmin, &xmax);
    beta = (doublereal) ibeta;
    albeta = log(beta);
    ait = (doublereal) it;
    a = zero;
    b = two;
    n = 2000;
    xn = (doublereal) n;
    jt = 0;
/* ----------------------------------------------------------------- */
/*  Determine smallest argument for GAMMA */
/* ----------------------------------------------------------------- */
    if (xmin * xmax < one) {
	xminv = one / xmax;
    } else {
	xminv = xmin;
    }
/* ----------------------------------------------------------------- */
/*  Determine largest argument for GAMMA by Newton iteration */
/* ----------------------------------------------------------------- */
    cl = log(xmax);
    xp = half * cl;
    cl = c2 - cl;
L50:
    x = xp;
    alnx = log(x);
    xnum = (x - half) * alnx - x + cl;
    xp = x - xnum / (alnx - half / x);
    if ((d__1 = xp - x, abs(d__1)) / x >= ten * eps) {
	goto L50;
    }
    cl = xp;
/* ----------------------------------------------------------------- */
/*  Random argument accuracy tests */
/* ----------------------------------------------------------------- */
    for (j = 1; j <= 4; ++j) {
	k1 = 0;
	k3 = 0;
	xc = zero;
	r6 = zero;
	r7 = zero;
	del = (b - a) / xn;
	xl = a;
	i__1 = n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x = del * ren_(&jt) + xl;
/* ----------------------------------------------------------------- */
/*  Use duplication formula for X not close to the zero */
/* ----------------------------------------------------------------- */
	    xph = x * half + half;
	    xp = xph - half;
	    x = xp + xp;
	    nx = (integer) x;
	    xxn = (doublereal) nx;
	    d__1 = x - xxn;
	    c__ = pow_di(&two, &nx) * pow_dd(&two, &d__1);
	    z__ = dgamma_(&x);
	    zz = c__ * c1 * dgamma_(&xp) * dgamma_(&xph);
/* -------------------------------------------------------------------- */
/*  Accumulate results */
/* -------------------------------------------------------------------- */
	    w = (z__ - zz) / z__;
	    if (w > zero) {
		++k1;
	    } else if (w < zero) {
		++k3;
	    }
	    w = abs(w);
	    if (w > r6) {
		r6 = w;
		xc = x;
	    }
	    r7 += w * w;
	    xl += del;
/* L200: */
	}
/* ------------------------------------------------------------------ */
/*  Gather and print statistics for test */
/* ------------------------------------------------------------------ */
	k2 = n - k3 - k1;
	r7 = sqrt(r7 / xn);
	io___55.ciunit = iout;
	s_wsfe(&io___55);
	e_wsfe();
	io___56.ciunit = iout;
	s_wsfe(&io___56);
	do_fio(&c__1, (char *)&n, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&a, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&b, (ftnlen)sizeof(doublereal));
	e_wsfe();
	io___57.ciunit = iout;
	s_wsfe(&io___57);
	do_fio(&c__1, (char *)&k1, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&k2, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&k3, (ftnlen)sizeof(integer));
	e_wsfe();
	io___58.ciunit = iout;
	s_wsfe(&io___58);
	do_fio(&c__1, (char *)&it, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	e_wsfe();
	if (r6 != zero) {
	    w = log((abs(r6))) / albeta;
	} else {
	    w = x99;
	}
	io___59.ciunit = iout;
	s_wsfe(&io___59);
	do_fio(&c__1, (char *)&r6, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&xc, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* Computing MAX */
	d__1 = ait + w;
	w = max(d__1,zero);
	io___60.ciunit = iout;
	s_wsfe(&io___60);
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
	if (r7 != zero) {
	    w = log((abs(r7))) / albeta;
	} else {
	    w = x99;
	}
	io___61.ciunit = iout;
	s_wsfe(&io___61);
	do_fio(&c__1, (char *)&r7, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* Computing MAX */
	d__1 = ait + w;
	w = max(d__1,zero);
	io___62.ciunit = iout;
	s_wsfe(&io___62);
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* ------------------------------------------------------------------ */
/*  Initialize for next test */
/* ------------------------------------------------------------------ */
	a = b;
	if (j == 1) {
	    b = ten;
	} else if (j == 2) {
	    b = cl - half;
	} else {
	    a = -(ten - half) * half;
	    b = a + half;
	}
/* L300: */
    }
/* ----------------------------------------------------------------- */
/*  Special tests */
/*  First test with special arguments */
/* ----------------------------------------------------------------- */
    io___63.ciunit = iout;
    s_wsfe(&io___63);
    e_wsfe();
    io___64.ciunit = iout;
    s_wsfe(&io___64);
    e_wsfe();
    x = -half;
    y = dgamma_(&x);
    io___66.ciunit = iout;
    s_wsfe(&io___66);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = xminv / xp99;
    y = dgamma_(&x);
    io___67.ciunit = iout;
    s_wsfe(&io___67);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = one;
    y = dgamma_(&x);
    io___68.ciunit = iout;
    s_wsfe(&io___68);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = two;
    y = dgamma_(&x);
    io___69.ciunit = iout;
    s_wsfe(&io___69);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = cl * xp99;
    y = dgamma_(&x);
    io___70.ciunit = iout;
    s_wsfe(&io___70);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
/* ----------------------------------------------------------------- */
/*  Test of error returns */
/* ----------------------------------------------------------------- */
    io___71.ciunit = iout;
    s_wsfe(&io___71);
    e_wsfe();
    x = -one;
    io___72.ciunit = iout;
    s_wsfe(&io___72);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = dgamma_(&x);
    io___73.ciunit = iout;
    s_wsfe(&io___73);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = zero;
    io___74.ciunit = iout;
    s_wsfe(&io___74);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = dgamma_(&x);
    io___75.ciunit = iout;
    s_wsfe(&io___75);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = xminv * (one - eps);
    io___76.ciunit = iout;
    s_wsfe(&io___76);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = dgamma_(&x);
    io___77.ciunit = iout;
    s_wsfe(&io___77);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = cl * (one + eps);
    io___78.ciunit = iout;
    s_wsfe(&io___78);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = dgamma_(&x);
    io___79.ciunit = iout;
    s_wsfe(&io___79);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___80.ciunit = iout;
    s_wsfe(&io___80);
    e_wsfe();
    s_stop("", (ftnlen)0);
/* ----------------------------------------------------------------- */
/* ---------- Last line of GAMMA test program ---------- */
    return 0;
} /* MAIN__ */

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

doublereal ren_(integer *k)
{
    /* Initialized data */

    static integer iy = 100001;
    static doublereal one = 1.;
    static doublereal c1 = 2796203.;
    static doublereal c2 = 1e-6;
    static doublereal c3 = 1e-12;

    /* System generated locals */
    doublereal ret_val;

    /* Local variables */
    static integer j;

/* --------------------------------------------------------------------- */
/*  Random number generator - based on Algorithm 266 by Pike and */
/*   Hill (modified by Hansson), Communications of the ACM, */
/*   Vol. 8, No. 10, October 1965. */

/*  This subprogram is intended for use on computers with */
/*   fixed point wordlength of at least 29 bits.  It is */
/*   best if the floating-point significand has at most */
/*   29 bits. */

/*  Latest modification: May 30, 1989 */

/*  Author: W. J. Cody */
/*          Mathematics and Computer Science Division */
/*          Argonne National Laboratory */
/*          Argonne, IL 60439 */

/* --------------------------------------------------------------------- */
/* S    REAL             CONV,C1,C2,C3,ONE,REN */
/* S    DATA ONE,C1,C2,C3/1.0E0,2796203.0E0,1.0E-6,1.0E-12/ */
/* --------------------------------------------------------------------- */
/*  Statement functions for conversion between integer and float */
/* --------------------------------------------------------------------- */
/* S    CONV(J) = REAL(J) */
/* --------------------------------------------------------------------- */
    j = *k;
    iy *= 125;
    iy -= iy / 2796203 * 2796203;
    ret_val = (doublereal) iy / c1 * (one + c2 + c3);
    return ret_val;
/* ---------- Last card of REN ---------- */
} /* ren_ */

