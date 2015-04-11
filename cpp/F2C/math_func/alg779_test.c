/* alg779_test.f -- translated by f2c (version 20060506).
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

/* Main program */ int MAIN__(void)
{
    /* Initialized data */

    static doublereal armp5m[45] = { -70.,1.,3.975449735908647e-31,-50.,1.,
	    1.928749847963918e-22,-25.,1.,1.388794386482764e-11,-16.,1.,
	    1.125351657643426e-7,-10.,1.,4.53984723608055e-5,-15.,2.,
	    5.528681621776332e-4,-5.,1.,.006706019989268209,-4.,1.,
	    .01808192299139939,-3.,1.,.04810263533220408,-5.,2.,
	    .07761872459482337,-2.,1.,.1236656218012099,-3.,2.,
	    .1933053702550534,-1.,1.,.2940276176114512,-1.,2.,
	    .4312314419263971,-1.,4.,.5137933107879051 };
    static doublereal armp5p[45] = { 0.,1.,.6048986434216304,1.,4.,
	    .7033904262554945,3.,4.,.916228088973455,5.,4.,1.138567662773306,
	    3.,2.,1.249323347852712,2.,1.,1.464294589087629,5.,2.,
	    1.666312883435831,3.,1.,1.853485088601518,4.,1.,2.185869690623812,
	    5.,1.,2.472987622482944,15.,2.,3.065170710136853,10.,1.,
	    3.552779239536617,20.,1.,5.041018507535329,50.,1.,
	    7.97753085858187,100.,1.,11.28332744292768 };
    static doublereal ar0p5m[45] = { -70.,1.,3.975449735908647e-31,-50.,1.,
	    1.928749847963918e-22,-25.,1.,1.388794386489583e-11,-16.,1.,
	    1.125351702418007e-7,-10.,1.,4.539920105264133e-5,-15.,2.,
	    5.529762498941281e-4,-5.,1.,.006721954314505913,-4.,1.,
	    .01819820350836711,-3.,1.,.04893370569649578,-5.,2.,
	    .07980385454073086,-2.,1.,.1292985133200756,-3.,2.,
	    .2073981870320298,-1.,1.,.3277951592607115,-1.,2.,
	    .5075371035546378,-1.,4.,.6254781023007525 };
    static doublereal ar0p5p[45] = { 0.,1.,.7651470246254079,1.,4.,
	    .9285439960386521,3.,4.,1.332760974545192,5.,4.,1.846345674539936,
	    3.,2.,2.144860877583114,2.,1.,2.823721277401584,5.,2.,
	    3.606975377950373,3.,1.,4.487547421351709,4.,1.,6.511567592754791,
	    5.,1.,8.844208895242954,15.,2.,15.7962899021874,10.,1.,
	    24.08465696463765,20.,1.,67.49151222165892,50.,1.,
	    266.0928125213626,100.,1.,752.3455915521961 };
    static doublereal ar1p5m[45] = { -70.,1.,3.975449735908647e-31,-50.,1.,
	    1.928749847963918e-22,-25.,1.,1.388794386492992e-11,-16.,1.,
	    1.125351724805299e-7,-10.,1.,4.539956540456176e-5,-15.,2.,
	    5.530303045971386e-4,-5.,1.,.006729940909014693,-4.,1.,
	    .01825672758510827,-3.,1.,.04935661279068416,-5.,2.,
	    .08092801162975109,-2.,1.,.1322467822517724,-3.,2.,
	    .2149728258474116,-1.,1.,.3466747947990574,-1.,2.,
	    .5526495259473541,-1.,4.,.6938463564509715 };
    static doublereal ar1p5p[45] = { 0.,1.,.8671998890121841,1.,4.,
	    1.078398154927105,3.,4.,1.639285310155971,5.,4.,2.429425545811454,
	    3.,2.,2.927749412793217,2.,1.,4.165414459868322,5.,2.,
	    5.768879312210517,3.,1.,7.78861077029597,4.,1.,13.26048817729107,
	    5.,1.,20.91446740276263,15.,2.,51.40754705793215,10.,1.,
	    101.005100843326,20.,1.,546.5630100657602,50.,1.,
	    5332.353566687146,100.,1.,30108.67168135487 };
    static doublereal ar2p5m[45] = { -70.,1.,3.975449735908647e-31,-50.,1.,
	    1.928749847963918e-22,-25.,1.,1.388794386494697e-11,-16.,1.,
	    1.125351735998945e-7,-10.,1.,4.539974758252285e-5,-15.,2.,
	    5.530573355642952e-4,-5.,1.,.006733940699471496,-4.,1.,
	    .01828611841328075,-3.,1.,.04957056753774198,-5.,2.,
	    .08150092750915113,-2.,1.,.1337669290459733,-3.,2.,
	    .218949517846093,-1.,1.,.3568591418689825,-1.,2.,
	    .5779521605410087,-1.,4.,.7331500647735718 };
    static doublereal ar2p5p[45] = { 0.,1.,.927553577773948,1.,4.,
	    1.169902452380993,3.,4.,1.840904997614609,5.,4.,2.847383501681993,
	    3.,2.,3.515475574749159,2.,1.,5.274621712621,5.,2.,
	    7.741874848838761,3.,1.,11.11289952271169,4.,1.,21.46870822801143,
	    5.,1.,38.36174573401834,15.,2.,125.1403440400588,10.,1.,
	    311.3376418507202,20.,1.,3186.735096831266,50.,1.,
	    76426.64612783266,100.,1.,860954.9737352777 };
    static integer iout = 6;

    /* Format strings */
    static char fmt_1000[] = "(///25x,\002TESTING FDM0P5\002)";
    static char fmt_1040[] = "(/5x,\002X-VALUE\002,7x,\002EXACT VALUE\002,"
	    "8x,\002COMP. VALUE\002,9x,\002REL. ERROR\002)";
    static char fmt_1010[] = "(///25x,\002TESTING FDP0P5\002)";
    static char fmt_1020[] = "(///25x,\002TESTING FDP1P5\002)";
    static char fmt_1030[] = "(///25x,\002TESTING FDP2P5\002)";
    static char fmt_1100[] = "(2x,f10.4,3x,d16.7,3x,d16.7,3x,d16.7)";

    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);

    /* Local variables */
    static integer n1, n2, n3;
    static doublereal xval;
    extern doublereal fdm0p5_(doublereal *);
    static doublereal fdval;
    extern doublereal fdp0p5_(doublereal *), fdp1p5_(doublereal *), fdp2p5_(
	    doublereal *);
    static doublereal exval;
    static integer ntest;
    static doublereal relerr;
    static integer numfun;

    /* Fortran I/O blocks */
    static cilist io___11 = { 0, 0, 0, fmt_1000, 0 };
    static cilist io___12 = { 0, 0, 0, fmt_1040, 0 };
    static cilist io___13 = { 0, 0, 0, fmt_1010, 0 };
    static cilist io___14 = { 0, 0, 0, fmt_1040, 0 };
    static cilist io___15 = { 0, 0, 0, fmt_1020, 0 };
    static cilist io___16 = { 0, 0, 0, fmt_1040, 0 };
    static cilist io___17 = { 0, 0, 0, fmt_1030, 0 };
    static cilist io___18 = { 0, 0, 0, fmt_1040, 0 };
    static cilist io___27 = { 0, 0, 0, fmt_1100, 0 };



/*   This program runs a very simple test procedure to assess */
/*   the accuracy of the 4 Fermi-Dirac functions, in double */
/*   precision form. */

/*   To run the test program load the 4 Fermi-Dirac functions */
/*   FDM0P5, FDP0P5, FDP1P5, FDP2P5, and the Chebyshev series */
/*   evaluation function CHEVAL. These should have been put */
/*   into double-precision form according to the instructions */
/*   in the codes. */

/*   The INTEGER variable IOUT defines the output stream for the */
/*   results. It is currently set to 6, but can be easily changed. */

/*   The complete program can then be compiled and run. */


/*   Define the arguments and exact function values. */


/*   Define the DATA output stream. */


/*   Run the tests */

    for (numfun = 1; numfun <= 8; ++numfun) {
	if (numfun == 1) {
	    io___11.ciunit = iout;
	    s_wsfe(&io___11);
	    e_wsfe();
	    io___12.ciunit = iout;
	    s_wsfe(&io___12);
	    e_wsfe();
	}
	if (numfun == 3) {
	    io___13.ciunit = iout;
	    s_wsfe(&io___13);
	    e_wsfe();
	    io___14.ciunit = iout;
	    s_wsfe(&io___14);
	    e_wsfe();
	}
	if (numfun == 5) {
	    io___15.ciunit = iout;
	    s_wsfe(&io___15);
	    e_wsfe();
	    io___16.ciunit = iout;
	    s_wsfe(&io___16);
	    e_wsfe();
	}
	if (numfun == 7) {
	    io___17.ciunit = iout;
	    s_wsfe(&io___17);
	    e_wsfe();
	    io___18.ciunit = iout;
	    s_wsfe(&io___18);
	    e_wsfe();
	}
	for (ntest = 1; ntest <= 15; ++ntest) {
	    n1 = ntest * 3 - 2;
	    n2 = n1 + 1;
	    n3 = n2 + 1;
	    if (numfun == 1) {
		xval = armp5m[n1 - 1] / armp5m[n2 - 1];
		exval = armp5m[n3 - 1];
		fdval = fdm0p5_(&xval);
	    }
	    if (numfun == 2) {
		xval = armp5p[n1 - 1] / armp5p[n2 - 1];
		exval = armp5p[n3 - 1];
		fdval = fdm0p5_(&xval);
	    }
	    if (numfun == 3) {
		xval = ar0p5m[n1 - 1] / ar0p5m[n2 - 1];
		exval = ar0p5m[n3 - 1];
		fdval = fdp0p5_(&xval);
	    }
	    if (numfun == 4) {
		xval = ar0p5p[n1 - 1] / ar0p5p[n2 - 1];
		exval = ar0p5p[n3 - 1];
		fdval = fdp0p5_(&xval);
	    }
	    if (numfun == 5) {
		xval = ar1p5m[n1 - 1] / ar1p5m[n2 - 1];
		exval = ar1p5m[n3 - 1];
		fdval = fdp1p5_(&xval);
	    }
	    if (numfun == 6) {
		xval = ar1p5p[n1 - 1] / ar1p5p[n2 - 1];
		exval = ar1p5p[n3 - 1];
		fdval = fdp1p5_(&xval);
	    }
	    if (numfun == 7) {
		xval = ar2p5m[n1 - 1] / ar2p5m[n2 - 1];
		exval = ar2p5m[n3 - 1];
		fdval = fdp2p5_(&xval);
	    }
	    if (numfun == 8) {
		xval = ar2p5p[n1 - 1] / ar2p5p[n2 - 1];
		exval = ar2p5p[n3 - 1];
		fdval = fdp2p5_(&xval);
	    }
	    relerr = (d__1 = (fdval - exval) / exval, abs(d__1));
	    io___27.ciunit = iout;
	    s_wsfe(&io___27);
	    do_fio(&c__1, (char *)&xval, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&exval, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&fdval, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&relerr, (ftnlen)sizeof(doublereal));
	    e_wsfe();
/* L100: */
	}
/* L200: */
    }
    return 0;
} /* MAIN__ */

/* Main program alias */ int testfd_ () { MAIN__ (); return 0; }
