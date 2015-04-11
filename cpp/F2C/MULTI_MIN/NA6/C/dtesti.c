/* dtesti.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Table of constant values */

static integer c__9 = 9;
static integer c__1 = 1;
static integer c__5 = 5;
static integer c__3 = 3;
static integer c__100 = 100;
static integer c__2306 = 2306;
static integer c__2 = 2;
static integer c__25 = 25;


/*   The following program will approximate three integrals over the */
/*   interval (0,infinity) and print out the values of the estimated */
/*   integrals, the estimated errors, the number of function evaluations */
/*   and IFAIL. */

/* Main program */ MAIN__()
{
    /* Format strings */
    static char fmt_10[] = "(\002 \002,3e22.14)";

    /* Builtin functions */
    double asin(doublereal);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(), s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), 
	    e_wsfe();

    /* Local variables */
    logical done[3];
    doublereal work[2306], a[100], b[100], gamma;
    integer ifail, neval, iwork[300];
    doublereal pi;
    extern /* Subroutine */ int dqainf_(integer *, U_fp, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, integer *, 
	    integer *, logical *, doublereal *, integer *);
    doublereal epsabs, abserr[3], period, epsrel;
    integer restar;
    extern /* Subroutine */ int funsub_(...);
    integer minpts, maxpts;
    doublereal result[3];

    /* Fortran I/O blocks */
    static cilist io___11 = { 0, 6, 0, 0, 0 };
    static cilist io___12 = { 0, 6, 0, 0, 0 };
    static cilist io___13 = { 0, 6, 0, 0, 0 };
    static cilist io___14 = { 0, 6, 0, 0, 0 };
    static cilist io___22 = { 0, 6, 0, 0, 0 };
    static cilist io___23 = { 0, 6, 0, 0, 0 };
    static cilist io___24 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___25 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___26 = { 0, 6, 0, 0, 0 };
    static cilist io___27 = { 0, 6, 0, 0, 0 };
    static cilist io___28 = { 0, 6, 0, 0, 0 };
    static cilist io___29 = { 0, 6, 0, 0, 0 };
    static cilist io___30 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___31 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___32 = { 0, 6, 0, 0, 0 };



/*   This demo-program has calls to the main driver: DQAINF */
/*   Through this main driver the rest of the modules are activated: */

/*   level 1: DQAINF */

/*   level 2: DCHINF and  DADINF */

/*   level 3: DGAINF, DGAINT, DEXINF, DTRINT */

/*   level 4: D1MACH (from blas in netlib) and FUNSUB (user specified) */


/*   For the user's convenience we give a value to several parameters */
/*   through the parameter statement. */
/*    The following two parameters the user should not change, */
/*   unless the user changes the code appropriately: */


/*   The following three parameters the user may choose and these have */
/*   to remain unchanged through the entire run: */
/*   NUMFUN: the number of functions you want to integrate; */
/*   WRKSUB: the maximum number of subregions allowed; */
/*   EMAX:   the maximum number of extrapolation steps; */


/*   Based on the previous 5 parameters we can now compute the */
/*   size of the main working space: NW. */


/*   Next we choose the method to be used */
/*   Three options: KEY = 0(Euler), 1(Euler modified), 2(Overholt's */
/*   P-order 2) */


/*   We need to choose WRKSUB  such that: */
/*   WRKSUB >= MAXPTS/NUM (We have NUM=21 fixed in this implementation) */
/*   This simply means that we have enough space to achieve MAXPTS */
/*   function evaluations. By choosing WRKSUB bigger than */
/*   this lower bound we may later change MAXPTS and RESTAR and restart */
/*   the computations from the point where the last run stopped. */
/*   The reason for stopping may have been that MAXPTS was too small */
/*   to achieve the requested error. */
/*          With our choice WRKSUB = 100 then any value of MAXPTS */
/*   between MINPTS  and 2100 will be accepted by the code. Choosing */
/*   MAXPTS = 1000 then allows us to restart with a greater value */
/*   if necessary. */
/*   Note: For a given problem it may happen that the code stops after */
/*   reaching say MAXPTS=2100 function evaluations, having used less than */
/*   WRKSUB = 100 subregions. The reason for this is that if a given */
/*   region needs to be further subdivided then the old region will not */
/*   be stored while the function values computed over that region */
/*   will be counted. */
/*      The example gave the following output on */
/*   a SUN Sparc station 10: */

/*   A=  0. B=    3.0000000000000 */
/*   GAMMA=    1.0000000000000 PERIOD=    6.2831853071796 */
/*   MAXPTS=  150 WRKSUB=  100 NW=  2306 */
/*   KEY=  2 EPSREL=    1.0000000000000D-09 */

/*   Result and error: problem no. 1, 2 and 3: */
/*      0.12431084921897E+01  0.71825718819736E+00  0.48923485452174E+00 */
/*      0.59475024208688E-05  0.35781998629104E-06  0.94496089623798E-07 */
/*   NEVAL=  147 IFAIL=  1 */

/*   New attempt using the restart feature, with MAXPTS=  300 */
/*   Result and error: problem no. 1, 2 and 3: */
/*      0.12431084850988E+01  0.71825716313760E+00  0.48923480269063E+00 */
/*      0.38596743952420E-10  0.44266003080563E-10  0.81963473649869E-10 */
/*   NEVAL=  273 IFAIL=  0 */

    pi = asin(1.) * 2;

/*   We specify the period and the decay parameter GAMMA>0. If */
/*   the value of GAMMA < 0 then the code will attempt to estimate */
/*   GAMMA in the case KEY = 2. GAMMA is not used by the other two */
/*   methods. */

    period = pi * 2;
    gamma = 1.;

/*   Next we give the left endpoint of integration and the point b */
/*   from where we expect the theory to be valid. Advice: Since b */
/*   is not uniquely defined please do some experiments with */
/*   different values of b to gain  experience. */
/*   The effect one achieves by increasing b depends on the value of KEY. */

    a[0] = 0.;
    b[0] = 3.;

/*  This is a first attempt: set RESTAR, give MINPTS, MAXPTS and the */
/*  absolute and relative error requests. */

    restar = 0;
    minpts = 42;
    maxpts = 150;
    epsabs = 0.;
    epsrel = 1e-9;

/*  Note: these four parameters and RESTAR may be changed in a restart run */

    s_wsle(&io___11);
    do_lio(&c__9, &c__1, "A=", (ftnlen)2);
    do_lio(&c__5, &c__1, (char *)&a[0], (ftnlen)sizeof(doublereal));
    do_lio(&c__9, &c__1, " B=", (ftnlen)3);
    do_lio(&c__5, &c__1, (char *)&b[0], (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___12);
    do_lio(&c__9, &c__1, "GAMMA=", (ftnlen)6);
    do_lio(&c__5, &c__1, (char *)&gamma, (ftnlen)sizeof(doublereal));
    do_lio(&c__9, &c__1, " PERIOD=", (ftnlen)8);
    do_lio(&c__5, &c__1, (char *)&period, (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___13);
    do_lio(&c__9, &c__1, "MAXPTS=", (ftnlen)7);
    do_lio(&c__3, &c__1, (char *)&maxpts, (ftnlen)sizeof(integer));
    do_lio(&c__9, &c__1, " WRKSUB=", (ftnlen)8);
    do_lio(&c__3, &c__1, (char *)&c__100, (ftnlen)sizeof(integer));
    do_lio(&c__9, &c__1, " NW=", (ftnlen)4);
    do_lio(&c__3, &c__1, (char *)&c__2306, (ftnlen)sizeof(integer));
    e_wsle();
    s_wsle(&io___14);
    do_lio(&c__9, &c__1, "KEY=", (ftnlen)4);
    do_lio(&c__3, &c__1, (char *)&c__2, (ftnlen)sizeof(integer));
    do_lio(&c__9, &c__1, " EPSREL=", (ftnlen)8);
    do_lio(&c__5, &c__1, (char *)&epsrel, (ftnlen)sizeof(doublereal));
    e_wsle();
    dqainf_(&c__3, (U_fp)funsub_, &period, &gamma, a, b, &c__100, &c__25, &
	    minpts, &maxpts, &epsabs, &epsrel, &c__2306, &c__2, &restar, 
	    result, abserr, &neval, &ifail, done, work, iwork);
    s_wsle(&io___22);
    e_wsle();
    s_wsle(&io___23);
    do_lio(&c__9, &c__1, "Result and error: problem no. 1, 2 and 3:", (ftnlen)
	    41);
    e_wsle();
    s_wsfe(&io___24);
    do_fio(&c__3, (char *)&result[0], (ftnlen)sizeof(doublereal));
    e_wsfe();
    s_wsfe(&io___25);
    do_fio(&c__3, (char *)&abserr[0], (ftnlen)sizeof(doublereal));
    e_wsfe();
    s_wsle(&io___26);
    do_lio(&c__9, &c__1, "NEVAL=", (ftnlen)6);
    do_lio(&c__3, &c__1, (char *)&neval, (ftnlen)sizeof(integer));
    do_lio(&c__9, &c__1, " IFAIL=", (ftnlen)7);
    do_lio(&c__3, &c__1, (char *)&ifail, (ftnlen)sizeof(integer));
    e_wsle();

/*   Test if this attempt was successful */

    if (ifail == 1) {

/*   We make a new attempt by increasing MAXPTS and using the */
/*   restarting feature by setting RESTAR = 1. We keep MINPTS */
/*   EPSABS and EPSREL unchanged in this example.. */

	restar = 1;
	maxpts = 300;
	s_wsle(&io___27);
	e_wsle();
	s_wsle(&io___28);
	do_lio(&c__9, &c__1, "New attempt using the restart feature, with MA\
XPTS=", (ftnlen)51);
	do_lio(&c__3, &c__1, (char *)&maxpts, (ftnlen)sizeof(integer));
	e_wsle();
	dqainf_(&c__3, (U_fp)funsub_, &period, &gamma, a, b, &c__100, &c__25, 
		&minpts, &maxpts, &epsabs, &epsrel, &c__2306, &c__2, &restar, 
		result, abserr, &neval, &ifail, done, work, iwork);
	s_wsle(&io___29);
	do_lio(&c__9, &c__1, "Result and error: problem no. 1, 2 and 3:", (
		ftnlen)41);
	e_wsle();
	s_wsfe(&io___30);
	do_fio(&c__3, (char *)&result[0], (ftnlen)sizeof(doublereal));
	e_wsfe();
	s_wsfe(&io___31);
	do_fio(&c__3, (char *)&abserr[0], (ftnlen)sizeof(doublereal));
	e_wsfe();
	s_wsle(&io___32);
	do_lio(&c__9, &c__1, "NEVAL=", (ftnlen)6);
	do_lio(&c__3, &c__1, (char *)&neval, (ftnlen)sizeof(integer));
	do_lio(&c__9, &c__1, " IFAIL=", (ftnlen)7);
	do_lio(&c__3, &c__1, (char *)&ifail, (ftnlen)sizeof(integer));
	e_wsle();
    }
    return 0;
} /* MAIN__ */

/* Subroutine */ int funsub_(doublereal *x, integer *numfun, integer *np, 
	doublereal *funvls)
{
    /* System generated locals */
    integer funvls_dim1, funvls_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sin(doublereal), cos(doublereal);

    /* Local variables */
    integer i__, j;
    doublereal y;


/*     This subroutine must be provided by the user for computing */
/*            all components of the integrand at the given */
/*            evaluation points. */
/*            It must have parameters (X,NUMFUN,NP,FUNVLS) */
/*            Input parameters: */
/*              X      Real array of dimension NP (= 21) defining the */
/*                     x-coordinates of the evaluation points. */
/*              NUMFUN Integer that defines the number of */
/*                     components of the integral I. */
/*              NP     Integer that gives the number of evaluation points */
/*                     in the quadrature rule used (Gauss, 21 point rule). */
/*            Output parameter: */
/*              FUNVLS Real array of dimension (NP,NUMFUN) that defines */
/*                     the function values in the 21 evaluation points */
/*                     for the NUMFUN components of the integrand. */

/*     Note that we may save computer time when integrating */
/*     several functions simultaneously over the same interval */
/*     if we take advantage of the functions' similarities. */

    /* Parameter adjustments */
    funvls_dim1 = *np;
    funvls_offset = 1 + funvls_dim1 * 1;
    funvls -= funvls_offset;
    --x;

    /* Function Body */
    i__1 = *np;
    for (i__ = 1; i__ <= i__1; ++i__) {
	y = sin(x[i__]) * 2 + x[i__] * cos(x[i__]);
	i__2 = *numfun;
	for (j = 1; j <= i__2; ++j) {
/* Computing 2nd power */
	    d__1 = x[i__];
	    funvls[i__ + j * funvls_dim1] = y / (j + d__1 * d__1);
/* L10: */
	}
    }
    return 0;
} /* funsub_ */

/* Main program alias */ int dtesti_ () { MAIN__ (); return 0; }
#ifdef __cplusplus
	}
#endif
