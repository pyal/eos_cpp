/* algtst.f -- translated by f2c (version 20060506).
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
/* FORTRAN 77 program to test ALGAMA or DLGAMA */

/*   Method: */

/*      Accuracy tests compare function values against values */
/*      generated with the duplication formula. */

/*   Data required */

/*      None */

/*   Subprograms required from this package */

/*      MACHAR - An environmental inquiry program providing */
/*               information on the floating-point arithmetic */
/*               system.  Note that the call to MACHAR can */
/*               be deleted provided the following five */
/*               parameters are assigned the values indicated */

/*               IBETA  - The radix of the floating-point system */
/*               IT     - The number of base-ibeta digits in the */
/*                        significant of a floating-point number */
/*               EPS    - The smallest positive floating-point */
/*                        number such that 1.0+EPS .NE. 1.0 */
/*               XMIN   - The smallest non-vanishing floating-point */
/*                        integral power of the radix */
/*               XMAX   - The largest finite floating-point number */

/*      REN(K) - A function subprogram returning random real */
/*               numbers uniformly distributed over (0,1) */


/*    Intrinsic functions required are: */

/*         ABS, ANINT, DBLE, LOG, MAX, REAL, SQRT */

/*  Reference: "Performance evaluation of programs related */
/*              to the real gamma function", W. J. Cody, */
/*              submitted for publication. */

/*  Latest modification: June 16, 1988 */

/*  Authors: W. J. Cody and L. Stoltz */
/*           Mathematics and Computer Science Division */
/*           Argonne National Laboratory */
/*           Argonne, IL 60439 */

/* -------------------------------------------------------------------- */
/* Main program */ int MAIN__(void)
{
    /* Initialized data */

    static doublereal c1 = -.072364942924700087072;
    static doublereal p6875 = .6875;
    static doublereal p875 = .875;
    static doublereal p3125 = 1.3125;
    static doublereal p625 = 1.625;
    static doublereal x99 = -999.;
    static doublereal xp99 = .99;
    static integer iout = 6;
    static doublereal c2 = .69314718055994530942;
    static doublereal c3 = .0056471805599453094172;
    static doublereal zero = 0.;
    static doublereal half = .5;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal ten = 10.;
    static doublereal sixten = 16.;

    /* Format strings */
    static char fmt_1000[] = "(\0021Test of LGAMA(X) vs LN(2*SQRT(PI))-2X*LN"
	    "(2)+\002,\002LGAMA(2X)-LGAMA(X+1/2)\002//)";
    static char fmt_1001[] = "(\0021Test of LGAMA(X) vs LN(2*SQRT(PI))-(2X-1"
	    ")*LN(2)+\002,\002LGAMA(X-1/2)-LGAMA(2X-1)\002//)";
    static char fmt_1002[] = "(\0021Test of LGAMA(X) vs -LN(2*SQRT(PI))+X*LN"
	    "(2)+\002,\002LGAMA(X/2)+LGAMA(X/2+1/2)\002//)";
    static char fmt_1010[] = "(i7,\002 Random arguments were tested from the"
	    " interval (\002,f5.1,\002,\002,f5.1,\002)\002//)";
    static char fmt_1011[] = "(\002  LGAMA(X) was larger\002,i6,\002 times"
	    ",\002/14x,\002 agreed\002,i6,\002 times, and\002/10x,\002was sma"
	    "ller\002,i6,\002 times.\002//)";
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
    static char fmt_1041[] = "(\002 LGAMA (\002,e13.6,\002) = \002,e13.6//)";
    static char fmt_1050[] = "(\0021Test of Error Returns\002///)";
    static char fmt_1053[] = "(\002 LGAMA will be called with the argumen"
	    "t\002,e13.6,/\002 This should not trigger an error message\002//)"
	    ;
    static char fmt_1061[] = "(\002 LGAMA returned the value\002,e13.6///)";
    static char fmt_1052[] = "(\002 LGAMA will be called with the argumen"
	    "t\002,e13.6,/\002 This should trigger an error message\002//)";
    static char fmt_1100[] = "(\002 This concludes the tests\002)";

    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double log(doublereal), d_nint(doublereal *), sqrt(doublereal);
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static doublereal a, b;
    static integer i__, j, n;
    static doublereal u, v, w, x, y, z__;
    static integer k1, k2, k3;
    static doublereal r6, r7, cl, xc;
    static integer it, jt;
    static doublereal xl, xn, zz, del, ait;
    extern doublereal ren_(integer *);
    static doublereal eps;
    static real all9;
    static doublereal beta;
    static integer ngrd, irnd, iexp;
    static doublereal xmin, xmax;
    static integer ibeta, negep;
    extern doublereal dlgama_(doublereal *);
    static doublereal albeta;
    extern /* Subroutine */ int machar_(integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static integer machep;
    static doublereal epsneg;
    static integer minexp, maxexp;

    /* Fortran I/O blocks */
    static cilist io___56 = { 0, 0, 0, fmt_1000, 0 };
    static cilist io___57 = { 0, 0, 0, fmt_1001, 0 };
    static cilist io___58 = { 0, 0, 0, fmt_1002, 0 };
    static cilist io___59 = { 0, 0, 0, fmt_1010, 0 };
    static cilist io___60 = { 0, 0, 0, fmt_1011, 0 };
    static cilist io___61 = { 0, 0, 0, fmt_1020, 0 };
    static cilist io___63 = { 0, 0, 0, fmt_1021, 0 };
    static cilist io___64 = { 0, 0, 0, fmt_1022, 0 };
    static cilist io___65 = { 0, 0, 0, fmt_1023, 0 };
    static cilist io___66 = { 0, 0, 0, fmt_1022, 0 };
    static cilist io___67 = { 0, 0, 0, fmt_1025, 0 };
    static cilist io___68 = { 0, 0, 0, fmt_1040, 0 };
    static cilist io___69 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___70 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___71 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___72 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___73 = { 0, 0, 0, fmt_1050, 0 };
    static cilist io___74 = { 0, 0, 0, fmt_1053, 0 };
    static cilist io___75 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___76 = { 0, 0, 0, fmt_1053, 0 };
    static cilist io___77 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___78 = { 0, 0, 0, fmt_1052, 0 };
    static cilist io___79 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___80 = { 0, 0, 0, fmt_1052, 0 };
    static cilist io___81 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___82 = { 0, 0, 0, fmt_1052, 0 };
    static cilist io___83 = { 0, 0, 0, fmt_1061, 0 };
    static cilist io___84 = { 0, 0, 0, fmt_1100, 0 };


/* S    REAL  ALGAMA, */
/* ------------------------------------------------------------------ */
/*   C1 = 0.5 - LN(SQRT(PI)) */
/*   C2 = LN(2) */
/*   C3 = LN(2) - 11/16 */
/* ------------------------------------------------------------------ */
/* S    DATA C1,C2/-7.2364942924700087072E-2,6.9314718055994530942E-1/, */
/* S   1     C3,ZERO,HALF/5.6471805599453094172E-3,0.0E0,0.5E0/, */
/* S   2     ONE,TWO,TEN,SIXTEN/1.0E0,2.0E0,10.0E0,16.0E0/, */
/* S   3     P6875,P875,P3125,P625/0.6875E0,0.875E0,1.3125E0,1.625E0/, */
/* S   4     X99,XP99/-999.0E0,0.99E0/ */
/* ------------------------------------------------------------------ */
/*  Statement functions for conversion */
/* ------------------------------------------------------------------ */
/* S    CONV(I) = REAL(I) */
/* S    FUNC(X) = ALGAMA(X) */
/* ------------------------------------------------------------------ */
/*  Determine machine parameters and set constants */
/* ------------------------------------------------------------------ */
    machar_(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &
	    maxexp, &eps, &epsneg, &xmin, &xmax);
    beta = (doublereal) ibeta;
    albeta = log(beta);
    ait = (doublereal) it;
    a = zero;
    b = p875;
    n = 2000;
    xn = (doublereal) n;
    jt = 0;
/* ----------------------------------------------------------------- */
/*  Determine largest argument for DLGAMA by iteration */
/* ----------------------------------------------------------------- */
    cl = xp99 * xmax;
    z__ = -cl / x99;
L80:
    zz = cl / (log(z__) - one);
    if ((d__1 = zz / z__ - one, abs(d__1)) > two * beta * eps) {
	z__ = zz;
	goto L80;
    }
    cl = zz;
/* ----------------------------------------------------------------- */
/*  Random argument accuracy tests */
/* ----------------------------------------------------------------- */
    for (j = 1; j <= 3; ++j) {
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
/*  Use duplication formula */
/* ----------------------------------------------------------------- */
	    if (j != 3) {
		if (j == 1) {
		    z__ = x + half;
		    x = z__ - half;
		    y = x + x;
		} else {
		    x += x;
		    x *= half;
		    y = x + x - one;
		    z__ = x - half;
		}
		u = dlgama_(&x);
		w = y - half - half;
		d__1 = w * sixten;
		zz = d_nint(&d__1) / sixten;
		w -= zz;
		v = half - zz * p6875 - c1 - w * p6875 - c3 * (w + zz);
		v = v + dlgama_(&y) - dlgama_(&z__);
	    } else {
		z__ = x * half + half;
		y = z__ - half;
		x = y + y;
		u = dlgama_(&x);
		v = c1 + (x - half - half) * c2 + dlgama_(&y) + dlgama_(&z__) 
			- half;
	    }
/* -------------------------------------------------------------------- */
/*  Accumulate results */
/* -------------------------------------------------------------------- */
	    w = (u - v) / u;
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
	if (j == 1) {
	    io___56.ciunit = iout;
	    s_wsfe(&io___56);
	    e_wsfe();
	} else if (j == 2) {
	    io___57.ciunit = iout;
	    s_wsfe(&io___57);
	    e_wsfe();
	} else {
	    io___58.ciunit = iout;
	    s_wsfe(&io___58);
	    e_wsfe();
	}
	io___59.ciunit = iout;
	s_wsfe(&io___59);
	do_fio(&c__1, (char *)&n, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&a, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&b, (ftnlen)sizeof(doublereal));
	e_wsfe();
	io___60.ciunit = iout;
	s_wsfe(&io___60);
	do_fio(&c__1, (char *)&k1, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&k2, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&k3, (ftnlen)sizeof(integer));
	e_wsfe();
	io___61.ciunit = iout;
	s_wsfe(&io___61);
	do_fio(&c__1, (char *)&it, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	e_wsfe();
	if (r6 != zero) {
	    w = log((abs(r6))) / albeta;
	} else {
	    w = all9;
	}
	io___63.ciunit = iout;
	s_wsfe(&io___63);
	do_fio(&c__1, (char *)&r6, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&xc, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* Computing MAX */
	d__1 = ait + w;
	w = max(d__1,zero);
	io___64.ciunit = iout;
	s_wsfe(&io___64);
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
	if (r7 != zero) {
	    w = log((abs(r7))) / albeta;
	} else {
	    w = all9;
	}
	io___65.ciunit = iout;
	s_wsfe(&io___65);
	do_fio(&c__1, (char *)&r7, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* Computing MAX */
	d__1 = ait + w;
	w = max(d__1,zero);
	io___66.ciunit = iout;
	s_wsfe(&io___66);
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* ------------------------------------------------------------------ */
/*  Initialize for next test */
/* ------------------------------------------------------------------ */
	a = p3125;
	b = p625;
	if (j == 2) {
	    a = two + two;
	    b = ten + ten;
	}
/* L300: */
    }
/* ----------------------------------------------------------------- */
/*  Special tests */
/*  First test with special arguments */
/* ----------------------------------------------------------------- */
    io___67.ciunit = iout;
    s_wsfe(&io___67);
    e_wsfe();
    io___68.ciunit = iout;
    s_wsfe(&io___68);
    e_wsfe();
    z__ = eps;
    zz = dlgama_(&z__);
    io___69.ciunit = iout;
    s_wsfe(&io___69);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    z__ = half;
    zz = dlgama_(&z__);
    io___70.ciunit = iout;
    s_wsfe(&io___70);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    z__ = one;
    zz = dlgama_(&z__);
    io___71.ciunit = iout;
    s_wsfe(&io___71);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    z__ = two;
    zz = dlgama_(&z__);
    io___72.ciunit = iout;
    s_wsfe(&io___72);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
/* ----------------------------------------------------------------- */
/*  Test of error returns */
/* ----------------------------------------------------------------- */
    io___73.ciunit = iout;
    s_wsfe(&io___73);
    e_wsfe();
    z__ = xmin;
    io___74.ciunit = iout;
    s_wsfe(&io___74);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    e_wsfe();
    zz = dlgama_(&z__);
    io___75.ciunit = iout;
    s_wsfe(&io___75);
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    z__ = cl;
    io___76.ciunit = iout;
    s_wsfe(&io___76);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    e_wsfe();
    zz = dlgama_(&z__);
    io___77.ciunit = iout;
    s_wsfe(&io___77);
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    z__ = -one;
    io___78.ciunit = iout;
    s_wsfe(&io___78);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    e_wsfe();
    zz = dlgama_(&z__);
    io___79.ciunit = iout;
    s_wsfe(&io___79);
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    z__ = zero;
    io___80.ciunit = iout;
    s_wsfe(&io___80);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    e_wsfe();
    zz = dlgama_(&z__);
    io___81.ciunit = iout;
    s_wsfe(&io___81);
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    z__ = xp99 * xmax;
    io___82.ciunit = iout;
    s_wsfe(&io___82);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    e_wsfe();
    zz = dlgama_(&z__);
    io___83.ciunit = iout;
    s_wsfe(&io___83);
    do_fio(&c__1, (char *)&zz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___84.ciunit = iout;
    s_wsfe(&io___84);
    e_wsfe();
    s_stop("", (ftnlen)0);
/* ----------------------------------------------------------------- */
/* ---------- Last line of ALGAMA/DLGAMA test program ---------- */
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

