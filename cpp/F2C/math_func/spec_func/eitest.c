/* eitest.f -- translated by f2c (version 20060506).
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
/* FORTRAN 77 program to test EI, EONE, and EXPEI. */

/*  Method: */

/*     Accuracy test compare function values against local Taylor's */
/*     series expansions.  Derivatives for Ei(x) are generated from */
/*     the recurrence relation using a technique due to Gautschi */
/*     (see references).  Special argument tests are run with the */
/*     related functions E1(x) and exp(-x)Ei(x). */

/*  Data required */

/*     None */

/*  Subprograms required from this package */

/*     MACHAR - An environmental inquiry program providing */
/*              information on the floating-point arithmetic */
/*              system.  Note that the call to MACHAR can */
/*              be deleted provided the following five */
/*              parameters are assigned the values indicated */

/*              IBETA  - The radix of the floating-point system */
/*              IT     - The number of base-ibeta digits in the */
/*                       significant of a floating-point number */
/*              XMAX   - The largest finite floating-point number */

/*     REN(K) - A function subprogram returning random real */
/*              numbers uniformly distributed over (0,1) */


/*  Intrinsic functions required are: */

/*      ABS, AINT, DBLE, LOG, MAX, REAL, SQRT */

/*  References: "The use of Taylor series to test accuracy of */
/*               function programs", Cody, W. J., and Stoltz, L., */
/*               submitted for publication. */

/*              "Recursive computation of certain derivatives - */
/*               A study of error propagation", Gautschi, W., and */
/*               Klein, B. J., Comm. ACM 13 (1970), 7-9. */

/*              "Remark on Algorithm 282", Gautschi, W., and Klein, */
/*               B. J., Comm. ACM 13 (1970), 53-54. */

/*  Latest modification: May 30, 1989 */

/*  Author: W. J. Cody */
/*          Mathematics and Computer Science Division */
/*          Argonne National Laboratory */
/*          Argonne, IL 60439 */

/* ------------------------------------------------------------------ */
/* Main program */ int MAIN__(void)
{
    /* Initialized data */

    static doublereal zero = 0.;
    static doublereal fiv12 = 512.;
    static doublereal rem = -7.424779065800051695596e-5;
    static integer iout = 6;
    static doublereal fourth = .25;
    static doublereal one = 1.;
    static doublereal four = 4.;
    static doublereal six = 6.;
    static doublereal ten = 10.;
    static doublereal x0 = .3725;
    static doublereal x99 = -999.;
    static doublereal p0625 = .0625;

    /* Format strings */
    static char fmt_1000[] = "(\0021Test of Ei(x) vs series expansion\002//)";
    static char fmt_1010[] = "(i7,\002 Random arguments were tested from the"
	    " interval (\002,f7.3,\002,\002,f7.3,\002)\002//)";
    static char fmt_1011[] = "(\002     EI(X) was larger\002,i6,\002 times"
	    ",\002)";
    static char fmt_1015[] = "(14x,\002 agreed\002,i6,\002 times, and\002/10"
	    "x,\002was smaller\002,i6,\002 times.\002//)";
    static char fmt_1020[] = "(\002 There are\002,i4,\002 base\002,i4,\002 s"
	    "ignificant digits in a floating-point number\002//)";
    static char fmt_1021[] = "(\002 The maximum relative error of\002,e15.4"
	    ",\002 = \002,i4,\002 **\002,f7.2/4x,\002occurred for X =\002,e13"
	    ".6)";
    static char fmt_1022[] = "(\002 The estimated loss of base\002,i4,\002 s"
	    "ignificant digits is\002,f7.2//)";
    static char fmt_1023[] = "(\002 The root mean square relative error wa"
	    "s\002,e15.4,\002 = \002,i4,\002 **\002,f7.2)";
    static char fmt_1040[] = "(//\002 Test of special arguments\002//)";
    static char fmt_1041[] = "(\002   EI (\002,e13.6,\002) = \002,e13.6//)";
    static char fmt_1042[] = "(\002 The relative error is\002,e15.4,\002 ="
	    " \002,i4,\002 **\002,f7.2/)";
    static char fmt_1050[] = "(\002 Test of Error Returns\002///)";
    static char fmt_1052[] = "(\002 EONE will be called with the argument"
	    "\002,e13.6,/\002 This should not underflow\002//)";
    static char fmt_1062[] = "(\002 EONE returned the value\002,e13.6///)";
    static char fmt_1053[] = "(\002 EONE will be called with the argument"
	    "\002,e13.6,/\002 This should underflow\002//)";
    static char fmt_1054[] = "(\002 EI will be called with the argument\002,"
	    "e13.6,/\002 This should not overflow\002//)";
    static char fmt_1064[] = "(\002 EI returned the value\002,e13.6///)";
    static char fmt_1055[] = "(\002 EI will be called with the argument\002,"
	    "e13.6,/\002 This should overflow\002//)";
    static char fmt_1056[] = "(\002 EXPEI will be called with the argumen"
	    "t\002,e13.6,/\002 This should not underflow\002//)";
    static char fmt_1065[] = "(\002 EXPEI returned the value\002,e13.6///)";
    static char fmt_1100[] = "(\002 This concludes the tests\002)";

    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double log(doublereal), sqrt(doublereal);
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);
    double d_int(doublereal *);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static doublereal a, b, d__[26];
    static integer i__, j, k, n;
    static doublereal u, v, w, x, y, z__, c1;
    static integer k1, k2, k3, n1;
    static doublereal r6, r7;
    extern doublereal ei_(doublereal *);
    static doublereal en, xc, dx;
    static integer it, jt;
    static doublereal xl, xn, del, ait;
    extern doublereal ren_(integer *);
    static doublereal eps, sum, two, xnp1, beta;
    extern doublereal eone_(doublereal *);
    static doublereal xbig;
    static integer ngrd, irnd;
    static doublereal xden, xlge;
    static integer iexp;
    static doublereal xmin, xmax, xnum;
    static integer ibeta, negep;
    extern doublereal expei_(doublereal *);
    extern /* Subroutine */ int dsubn_(doublereal *, integer *, doublereal *, 
	    doublereal *);
    static doublereal albeta;
    extern /* Subroutine */ int machar_(integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static integer machep;
    static doublereal epsneg;
    static integer minexp, maxexp;

    /* Fortran I/O blocks */
    static cilist io___56 = { 0, 0, 0, fmt_1000, 0 };
    static cilist io___57 = { 0, 0, 0, fmt_1010, 0 };
    static cilist io___58 = { 0, 0, 0, fmt_1011, 0 };
    static cilist io___59 = { 0, 0, 0, fmt_1015, 0 };
    static cilist io___60 = { 0, 0, 0, fmt_1020, 0 };
    static cilist io___61 = { 0, 0, 0, fmt_1021, 0 };
    static cilist io___62 = { 0, 0, 0, fmt_1022, 0 };
    static cilist io___63 = { 0, 0, 0, fmt_1023, 0 };
    static cilist io___64 = { 0, 0, 0, fmt_1022, 0 };
    static cilist io___65 = { 0, 0, 0, fmt_1040, 0 };
    static cilist io___66 = { 0, 0, 0, fmt_1041, 0 };
    static cilist io___67 = { 0, 0, 0, fmt_1042, 0 };
    static cilist io___68 = { 0, 0, 0, fmt_1022, 0 };
    static cilist io___69 = { 0, 0, 0, fmt_1050, 0 };
    static cilist io___76 = { 0, 0, 0, fmt_1052, 0 };
    static cilist io___77 = { 0, 0, 0, fmt_1062, 0 };
    static cilist io___78 = { 0, 0, 0, fmt_1053, 0 };
    static cilist io___79 = { 0, 0, 0, fmt_1062, 0 };
    static cilist io___81 = { 0, 0, 0, fmt_1054, 0 };
    static cilist io___82 = { 0, 0, 0, fmt_1064, 0 };
    static cilist io___83 = { 0, 0, 0, fmt_1055, 0 };
    static cilist io___84 = { 0, 0, 0, fmt_1064, 0 };
    static cilist io___85 = { 0, 0, 0, fmt_1056, 0 };
    static cilist io___86 = { 0, 0, 0, fmt_1065, 0 };
    static cilist io___87 = { 0, 0, 0, fmt_1055, 0 };
    static cilist io___88 = { 0, 0, 0, fmt_1064, 0 };
    static cilist io___89 = { 0, 0, 0, fmt_1100, 0 };


/* S    REAL */
/* ------------------------------------------------------------------ */
/* S    DATA ZERO,FOURTH,ONE,FOUR,SIX/0.0E0,0.25E0,1.0E0,4.0E0,6.0E0/, */
/* S   1     TEN,X0,X99,P0625/10.0E0,0.3725E0,-999.0E0,0.0625E0/, */
/* S   2     FIV12,REM/512.0E0,-7.424779065800051695596E-5/ */
/* ------------------------------------------------------------------ */
/*  Statement functions for conversion */
/* ------------------------------------------------------------------ */
/* S    CONV(I) = REAL(I) */
/* ------------------------------------------------------------------ */
/*  Determine machine parameters and set constants */
/* ------------------------------------------------------------------ */
    machar_(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &
	    maxexp, &eps, &epsneg, &xmin, &xmax);
    beta = (doublereal) ibeta;
    albeta = log(beta);
    ait = (doublereal) it;
    dx = -p0625;
    a = fourth + dx;
    b = x0 + dx;
    n = 2000;
    n1 = 25;
    xn = (doublereal) n;
    jt = 0;
/* ----------------------------------------------------------------- */
/*  Random argument accuracy tests */
/* ----------------------------------------------------------------- */
    for (j = 1; j <= 8; ++j) {
	k1 = 0;
	k3 = 0;
	xc = zero;
	r6 = zero;
	r7 = zero;
	del = (b - a) / xn;
	xl = a;
	i__1 = n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    y = del * ren_(&jt) + xl;
	    x = y - dx;
	    y = x + dx;
/* ----------------------------------------------------------------- */
/*  Test Ei against series expansion */
/* ----------------------------------------------------------------- */
	    v = ei_(&x);
	    z__ = ei_(&y);
	    sum = zero;
	    u = x;
	    dsubn_(&u, &n1, &xmax, d__);
	    en = (doublereal) n1 + one;
	    sum = d__[n1] * dx / en;
	    for (k = n1; k >= 1; --k) {
		en -= one;
		sum = (sum + d__[k - 1]) * dx / en;
/* L100: */
	    }
	    u = v + sum;
/* -------------------------------------------------------------------- */
/*  Accumulate results */
/* -------------------------------------------------------------------- */
	    w = z__ - u;
	    w /= z__;
	    if (w > zero) {
		++k1;
	    } else if (w < zero) {
		++k3;
	    }
	    w = abs(w);
	    if (w > r6) {
		r6 = w;
		xc = y;
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
	io___56.ciunit = iout;
	s_wsfe(&io___56);
	e_wsfe();
	io___57.ciunit = iout;
	s_wsfe(&io___57);
	do_fio(&c__1, (char *)&n, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&a, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&b, (ftnlen)sizeof(doublereal));
	e_wsfe();
	io___58.ciunit = iout;
	s_wsfe(&io___58);
	do_fio(&c__1, (char *)&k1, (ftnlen)sizeof(integer));
	e_wsfe();
	io___59.ciunit = iout;
	s_wsfe(&io___59);
	do_fio(&c__1, (char *)&k2, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&k3, (ftnlen)sizeof(integer));
	e_wsfe();
	io___60.ciunit = iout;
	s_wsfe(&io___60);
	do_fio(&c__1, (char *)&it, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	e_wsfe();
	if (r6 != zero) {
	    w = log((abs(r6))) / albeta;
	} else {
	    w = x99;
	}
	io___61.ciunit = iout;
	s_wsfe(&io___61);
	do_fio(&c__1, (char *)&r6, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&xc, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* Computing MAX */
	d__1 = ait + w;
	w = max(d__1,zero);
	io___62.ciunit = iout;
	s_wsfe(&io___62);
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
	if (r7 != zero) {
	    w = log((abs(r7))) / albeta;
	} else {
	    w = x99;
	}
	io___63.ciunit = iout;
	s_wsfe(&io___63);
	do_fio(&c__1, (char *)&r7, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* Computing MAX */
	d__1 = ait + w;
	w = max(d__1,zero);
	io___64.ciunit = iout;
	s_wsfe(&io___64);
	do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* ------------------------------------------------------------------ */
/*  Initialize for next test */
/* ------------------------------------------------------------------ */
	if (j == 1) {
	    dx = -dx;
	    a = x0 + dx;
	    b = six;
	} else if (j <= 4) {
	    a = b;
	    b += b;
	} else if (j == 5) {
	    a = -fourth;
	    b = -one;
	} else if (j == 6) {
	    a = b;
	    b = -four;
	} else {
	    a = b;
	    b = -ten;
	}
/* L300: */
    }
/* ----------------------------------------------------------------- */
/*  Special tests.  First, check accuracy near the zero of Ei(x) */
/* ----------------------------------------------------------------- */
    io___65.ciunit = iout;
    s_wsfe(&io___65);
    e_wsfe();
    x = (four - one) / (four + four);
    y = ei_(&x);
    io___66.ciunit = iout;
    s_wsfe(&io___66);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    z__ = (y - (four + one) / fiv12 - rem) / y;
    if (z__ != zero) {
	w = log((abs(z__))) / albeta;
    } else {
	w = x99;
    }
    io___67.ciunit = iout;
    s_wsfe(&io___67);
    do_fio(&c__1, (char *)&z__, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
    e_wsfe();
/* Computing MAX */
    d__1 = ait + w;
    w = max(d__1,zero);
    io___68.ciunit = iout;
    s_wsfe(&io___68);
    do_fio(&c__1, (char *)&ibeta, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
    e_wsfe();
/* ----------------------------------------------------------------- */
/*  Check near XBIG, the largest argument acceptable to EONE, i.e., */
/*    the negative of the smallest argument acceptable to EI. */
/*    Determine XBIG with Newton iteration on the equation */
/*                  EONE(x) = XMIN. */
/* --------------------------------------------------------------------- */
    io___69.ciunit = iout;
    s_wsfe(&io___69);
    e_wsfe();
    two = one + one;
    v = sqrt(eps);
    c1 = (doublereal) minexp * log(beta);
    xn = -c1;
L320:
    xnum = -xn - log(xn) + log(one + one / xn) - c1;
    xden = -(xn * xn + xn + xn + two) / (xn * (xn + one));
    xnp1 = xn - xnum / xden;
    w = (xn - xnp1) / xnp1;
    if (abs(w) > v) {
	xn = xnp1;
	goto L320;
    }
    xbig = xnp1;
    d__1 = ten * xbig;
    x = d_int(&d__1) / ten;
    io___76.ciunit = iout;
    s_wsfe(&io___76);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = eone_(&x);
    io___77.ciunit = iout;
    s_wsfe(&io___77);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = xbig * (one + v);
    io___78.ciunit = iout;
    s_wsfe(&io___78);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = eone_(&x);
    io___79.ciunit = iout;
    s_wsfe(&io___79);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
/* --------------------------------------------------------------------- */
/*  Check near XMAX, the largest argument acceptable to EI.  Determine */
/*    XLGE with Newton iteration on the equation */
/*                  EI(x) = XMAX. */
/* --------------------------------------------------------------------- */
    c1 = (doublereal) maxexp * log(beta);
    xn = c1;
L330:
    xnum = xn - log(xn) + log(one + one / xn) - c1;
    xden = (xn * xn - two) / (xn * (xn + one));
    xnp1 = xn - xnum / xden;
    w = (xn - xnp1) / xnp1;
    if (abs(w) > v) {
	xn = xnp1;
	goto L330;
    }
    xlge = xnp1;
    d__1 = ten * xlge;
    x = d_int(&d__1) / ten;
    io___81.ciunit = iout;
    s_wsfe(&io___81);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = ei_(&x);
    io___82.ciunit = iout;
    s_wsfe(&io___82);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = xlge * (one + v);
    io___83.ciunit = iout;
    s_wsfe(&io___83);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = ei_(&x);
    io___84.ciunit = iout;
    s_wsfe(&io___84);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
/* --------------------------------------------------------------------- */
/*  Check with XHUGE, the largest acceptable argument for EXPEI */
/* --------------------------------------------------------------------- */
    if (xmin * xmax <= one) {
	x = xmax;
    } else {
	x = one / xmin;
    }
    io___85.ciunit = iout;
    s_wsfe(&io___85);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = expei_(&x);
    io___86.ciunit = iout;
    s_wsfe(&io___86);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    x = zero;
    io___87.ciunit = iout;
    s_wsfe(&io___87);
    do_fio(&c__1, (char *)&x, (ftnlen)sizeof(doublereal));
    e_wsfe();
    y = ei_(&x);
    io___88.ciunit = iout;
    s_wsfe(&io___88);
    do_fio(&c__1, (char *)&y, (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___89.ciunit = iout;
    s_wsfe(&io___89);
    e_wsfe();
    s_stop("", (ftnlen)0);
/* ----------------------------------------------------------------- */
/* ---------- Last line of EI test program ---------- */
    return 0;
} /* MAIN__ */

/* Subroutine */ int dsubn_(doublereal *x, integer *nmax, doublereal *xmax, 
	doublereal *d__)
{
    /* Initialized data */

    static doublereal b4 = .85777;
    static doublereal b5 = 1.0125;
    static doublereal b6 = .775;
    static doublereal zero = 0.;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal ten = 10.;
    static doublereal c0 = 2.7183;
    static doublereal c1 = 46.7452;
    static doublereal b0 = 5.7941e-5;
    static doublereal b1 = -.00176148;
    static doublereal b2 = .0208645;
    static doublereal b3 = -.129013;

    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double exp(doublereal), log(doublereal);

    /* Local variables */
    static doublereal e;
    static integer j, n;
    static doublereal p, q, t, z__;
    static integer n0, n1;
    static doublereal x1, en;
    static integer lim, mini;
    static logical bool1, bool2;

/* ------------------------------------------------------------------- */
/* Translation of Gautschi'f CACM Algorithm 282 for */
/*   derivatives of Ei(x). */

/*  Intrinsic functions required are: */

/*      ABS, EXP, INT, LOG, MIN */

/* ------------------------------------------------------------------- */
/* S    REAL */
/* S    DATA ZERO/0.0E0/,ONE/1.0E0/,TWO/2.0E0/,TEN/10.0E0/ */
/* S    DATA C0/2.7183E0/,C1/4.67452E1/ */
/* S    DATA B0/5.7941E-5/,B1/-1.76148E-3/,B2/2.08645E-2/, */
/* S   1     B3/-1.29013E-1/,B4/8.5777E-1/,B5/1.0125E0/,B6/7.75E-1/ */
/* ------------------------------------------------------------------- */
    x1 = abs(*x);
    n0 = (integer) x1;
    e = exp(*x);
    d__[0] = e / *x;
    bool1 = *x < zero || x1 <= two;
    bool2 = n0 < *nmax;
    mini = min(n0,*nmax);
    if (bool1) {
	lim = *nmax;
    } else {
	lim = mini;
    }
    n = 1;
    en = one;
L50:
    d__[n] = (e - en * d__[n - 1]) / *x;
    ++n;
    en += one;
    if (x1 < one) {
	if ((d__1 = d__[n - 1], abs(d__1)) < (d__2 = *xmax * *x / en, abs(
		d__2)) && n <= lim) {
	    goto L50;
	}
    } else {
	if ((d__1 = d__[n - 1] / *x, abs(d__1)) < *xmax / en && n <= lim) {
	    goto L50;
	}
    }
    i__1 = lim;
    for (j = n; j <= i__1; ++j) {
	d__[n] = zero;
/* L100: */
    }
    if (! bool1 && bool2) {
	t = (x1 + c1) / (c0 * x1);
	if (t < ten) {
	    t = ((((b0 * t + b1) * t + b2) * t + b3) * t + b4) * t + b5;
	} else {
	    z__ = log(t) - b6;
	    p = (b6 - log(z__)) / (one + z__);
	    p = one / (one + p);
	    t = t * p / z__;
	}
	n1 = (integer) (c0 * x1 * t - one);
	if (n1 < *nmax) {
	    n1 = *nmax;
	}
	q = one / *x;
	en = one;
	i__1 = n1 + 1;
	for (n = 1; n <= i__1; ++n) {
	    q = -en * q / *x;
	    en += one;
/* L120: */
	}
	i__1 = n0 + 1;
	for (n = n1; n >= i__1; --n) {
	    en -= one;
	    q = (e - *x * q) / en;
	    if (n <= *nmax) {
		d__[n] = q;
	    }
/* L140: */
	}
    }
    return 0;
/* ---------- Last line of DSUBN ---------- */
} /* dsubn_ */

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

