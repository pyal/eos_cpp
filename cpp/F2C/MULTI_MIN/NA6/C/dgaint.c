/* dgaint.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Table of constant values */

static doublereal c_b2 = .5;
static doublereal c_b3 = -10.5;
static integer c__21 = 21;
static doublereal c_b18 = 11.5;

/* Subroutine */ int dgaint_(doublereal *a, doublereal *b, integer *numfun, 
	S_fp funsub, logical *done, logical *more, doublereal *epmach, 
	doublereal *null, doublereal *funval, doublereal *basabs, integer *
	flag__, doublereal *basval, doublereal *rgnerr, doublereal *greate, 
	integer *worst, doublereal *c__, doublereal *d__)
{
    /* Initialized data */

    static doublereal g[11] = { .9937521706203895,.9672268385663062,
	    .9200993341504008,.8533633645833172,.7684399634756779,
	    .6671388041974123,.5516188358872198,.4243421202074387,
	    .288021316802401,.145561854160895,0. };
    static doublereal w[11] = { .01601722825777433,.03695378977085249,
	    .0571344254268572,.0761001136283793,.09344442345603386,
	    .1087972991671483,.1218314160537285,.1322689386333374,
	    .1398873947910731,.14452440398997,.1460811336496904 };
    static doublereal nullw[176]	/* was [16][11] */ = { 
	    .005859224694730026,.01301976799706014,.02158184987561927,
	    .03119657617222001,.04157639307601386,.05246625962340516,
	    .06363031447247006,.07484599067063009,.08590167508061712,
	    .09659618304508728,.1067392105869384,.116152319105073,
	    .124670195533083,.1321420280297885,.1384328909795413,
	    .1434250647895144,-.02024707000741622,-.04379005851020851,
	    -.06965227115767195,-.09516116459787523,-.1179114894020921,
	    -.1358182960749584,-.1472028208379138,-.1508776892345244,
	    -.1462121283895959,-.1331667766592828,-.1122928178247447,
	    -.08469391287377601,-.05195253287985397,-.01602500237425218,
	    .02088816813445404,.05648832390790171,.03883580247121445,
	    .07990096087086482,.1174438965053943,.1431705707841666,
	    .1511566572815612,.1386355746642898,.1063757528761779,
	    .05853283458897407,.001973066649848703,-.05483608118503819,
	    -.1031959020477783,-.1355896186086413,-.1469123277046623,
	    -.1353193782985104,-.1025551817987029,-.05370731005946019,
	    -.05965412595242497,-.1138307201442027,-.1473794001233916,
	    -.1462171986707822,-.1073644825716617,-.03967547044517777,
	    .03881687506910375,.1062457136342151,.1434120884358229,
	    .1395396581193282,.09558143541939021,.02408868272651161,
	    -.05433985749387003,-.1170027382391999,-.1456993480020755,
	    -.1320553898020212,.0811427949834302,.1394263659262734,
	    .1481989091733945,.09677919508585969,.004174411212397235,
	    -.08983329656061084,-.1432999618142209,-.1318732622123368,
	    -.06050045998747565,.0384221933717722,.1196951864405313,
	    .1460359069105494,.1052913655334742,.01614011431557236,
	    -.08041833842953963,-.1399117916675364,-.1019231472030305,
	    -.1520456605731098,-.1168273220215079,-.01075583592960879,
	    .1012057375471417,.147180195880142,.0969896942429765,
	    -.01673118490576078,-.1192968264851738,-.1430606163131484,
	    -.0722598400037873,.04632194803727984,.1341759283651172,
	    .1330641979525424,.04369891359834745,-.07464504070837483,
	    .1207652963052454,.1489588260146731,.05890214560028095,
	    -.07936141880173113,-.1472858866418607,-.08524048604745531,
	    .05209450556829039,.1428979325476485,.1063582979588903,
	    -.02498840938693774,-.1339424732473705,-.1231814607655799,
	    -.002310968036052471,.1205891076794731,.1355713757017371,
	    .0292225945939076,-.136604379671161,-.1296181347851887,
	    .01273585156460484,.138074955247293,.1063772962797608,
	    -.0461729811485722,-.1455658951943161,-.07818432195969258,
	    .07871971420591506,.1451753725543706,.04492456833975673,
	    -.1068762140630544,-.1358135230198954,-.00864091910814602,
	    .1284341564046552,.1177993871727999,.1485698262567817,
	    .09568420420614478,-.08133037350927629,-.141757711722709,
	    -.002323645744823691,.1397282757969823,.08343286549711612,
	    -.09112601052788798,-.1360664606736734,.01236834040097046,
	    .1431238351576778,.07029919038187181,-.1024826424015055,
	    -.1294253464460428,.02777799655524739,.1454239155303997,
	    -.1560150459859118,-.05078074459100106,.1304777802379009,
	    .08867798725104829,-.1030910117645103,-.1185867937861861,
	    .06800562635441401,.138284949606409,-.02747426951466367,
	    -.1461902970879641,-.01523606417516131,.1416700606593806,
	    .05656562071840163,-.1251272318395094,-.09304002613430708,
	    .09797588771824906,.1585416482170856,0.,-.1483021322906934,0.,
	    .1469720414561505,0.,-.1465539124681842,0.,.1463706054390223,0.,
	    -.1462742772906911,0.,.1462174827723311,0.,-.146181214016595,0. };

    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), sqrt(doublereal);

    /* Local variables */
    doublereal diff, hlen, emax, e[4];
    integer i__, j, k;
    doublereal r__, x[21], centr, noise, const__;
    integer i1, i2;
    doublereal rr[3], abscis, abssum, sum;

/* ***BEGIN PROLOGUE DGAINT */
/* ***PURPOSE  To compute basic integration rule values and */
/*            corresponding error estimates. */

/* ***LAST MODIFICATION 94-03-10 */

/* ***REFER TO DQAINF */

/* ***DESCRIPTION DGAINT computes basic integration rule values */
/*            for a vector of integrands over an interval. */
/*            DGAINT also computes estimates for the errors by */
/*            using several null rule approximations. */
/*   ON ENTRY */

/*   A    Real. */
/*   B    Real. */
/*          The left and right endpoints of the interval. */
/*   NUMFUN Integer. */
/*          Number of components of the vector integrand. */
/*   FUNSUB Externally declared subroutine for computing */
/*          all components of the integrand at the given */
/*          evaluation point. */
/*            It must have parameters (X,NUMFUN,NP,FUNVLS) */
/*            Input parameters: */
/*              X      The x-coordinates of all 21 evaluation points. */
/*              NUMFUN Integer that defines the number of */
/*                     components of I. */
/*              NP     Integer that gives the number of evaluation points */
/*            Output parameter: */
/*              FUNVLS Real array of dimension (21,NUMFUN) */
/*                     that defines NUMFUN components of the integrand */
/*                     evaluated in all 21 evaluation points. */

/*   DONE   Logical array of dimension NUMFUN. */
/*          DONE(I)=.TRUE. if function number I has been */
/*          integrated to the specified precision, else DONE(I)=.FALSE. */
/*   MORE   Logical. */
/*          Information about the fact that there is still work to do. */
/*   EPMACH Real. */
/*          Machine epsilon. */
/*   NULL   Real array of dimension (16,NUMFUN). */
/*          A work array. */
/*   FUNVAL Real array of dimension (21,NUMFUN). */
/*          A work array. */
/*   BASABS Real array of dimension NUMFUN. */
/*          A work array. */
/*   ON RETURN */

/*   FLAG   Integer. */
/*          Signals that at least one interval has become too small */
/*          to distinguish between a rule evaluation point and an */
/*          endpoint. */

/*   BASVAL Real array of dimension NUMFUN. */
/*          The values for the basic rule for each component */
/*          of the integrand. */
/*   RGNERR Real array of dimension NUMFUN. */
/*          The error estimates for each component of the integrand. */
/*   GREATE Real. */
/*          The greatest error component of the integrand. */
/*   WORST  Index to the greatest error in each sub-interval. */

/*   C      Real. */
/*   D      Real */
/*          C and D are subdivision points based on */
/*          uniform trisection: C = A + (B-A)/3 and D = B - (B-a)/3. */

/* ***REFERENCES Espelid, T. O., Integration Rules, Null Rules and */
/*          Error Estimation, Report no 33, Informatics, Univ. of Bergen, */
/*          1988. */

/*          Berntsen,J. and Espelid,T.O., Error estimation in Automatic */
/*          Quadrature Routines, ACM Trans. Math. Softw., */
/*          17, 2, 233-252, 1991. */

/*          Espelid, T. O., DQAINT: An Algorithm for Adaptive Quadrature */
/*          over a Collection of Finite Intervals, in Numerical */
/*          Integration,  Recent Developments, Software and Applications, */
/*          Espelid and Genz (eds),  NATO ASI Series C: */
/*          Mathematical and Physical Sciences - Vol. 357, */
/*          Kluwer, Dordrecht, The Netherlands, pages 341-342, 1992. */

/* ***ROUTINES CALLED FUNSUB */
/* ***END PROLOGUE DGAINT */

/*   Global variables. */


/*   Local variables. */

/*   WTLENG The number of weights of the integration rule. */
/*   G      Real array of dimension WTLENG. */
/*          The coordinates for the generators of */
/*          the evaluation points. */
/*          The integration rule is using symmetric evaluation */
/*          points and has the structure (1,6,3). That is, */
/*          1 point of multiplicity 1, */
/*          6 sets of points of multiplicity 3 and */
/*          3 sets of points of multiplicity 6. */
/*          This gives totally 37 evaluation points. */
/*          In order to reduce the number of loops in DGAINT, */
/*          the 3 loops for the sets of multiplicity 6 are split */
/*          into 6 loops and added to the loops for the sets of */
/*          multiplicity 3. */
/*          The number of weights we have to give with */
/*          this splitting is 13(WTLENG). */

/*   W      Real array of dimension (9,WTLENG). */
/*          The weights of the basic rule and the null rules. */
/*          W(1),...,W(WTLENG) are weights for the basic rule. */

/*   X      Real array of dimension 21. */
/*          Evaluation points in (A,B). */
/*   HLEN   Real. */
/*          Half of intervals length */

/*  The abscissas to the 21 point Gauss integration rule. */
/*  The abscissas are given in -1,1: due to the symmetry we only specify */
/*  values in 0,1. */

    /* Parameter adjustments */
    --rgnerr;
    --basval;
    --basabs;
    funval -= 22;
    null -= 17;
    --done;

    /* Function Body */

/*   Weights of the 21 point Gauss quadrature rule. Same remark */
/*   about symmetry. */


/*   Weights of the 5 symmetric nullrules of degrees 19,17,15,13,11 */
/*   Weights of the 5 asymmetric nullrules of degrees 18,16,14,12,10 */
/*   Same remark about symmetry as above. The nullrules are all */
/*   orthogonal and has the same norm as the basic rule and are */
/*   given with decreasingly degrees. */


/* ***FIRST EXECUTABLE STATEMENT DGAINT */

/*  Define constants */

    const__ = 10. * pow_dd(&c_b2, &c_b3);


/*  Compute half-length and center of interval. */

    hlen = (*b - *a) / 2;
    centr = *a + hlen;
    x[10] = centr;

/*  Compute all abscissas */

    for (i__ = 1; i__ <= 10; ++i__) {
	abscis = hlen * g[i__ - 1];
	x[i__ - 1] = centr - abscis;
	x[22 - i__ - 1] = centr + abscis;
/* L10: */
    }
    if (x[20] == x[19] || x[0] == x[1]) {
	*flag__ = 1;
    }

/*   Evaluate all NUMFUN functions in the 21 evaluation points. */

    (*funsub)(x, numfun, &c__21, &funval[22]);

/*   Apply the basic rule: first center point */

    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	basval[j] = w[10] * funval[j * 21 + 11];
	basabs[j] = (d__1 = basval[j], abs(d__1));
/* L20: */
    }

/*   Apply all nullrules: first center point */

    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	for (i__ = 1; i__ <= 8; ++i__) {
	    null[i__ + (j << 4)] = nullw[i__ + 159] * funval[j * 21 + 11];
/* L30: */
	}
/* L40: */
    }

/*   Compute the basic rule contributions from the other points. */

    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	for (i__ = 1; i__ <= 10; ++i__) {
	    sum = funval[i__ + j * 21] + funval[22 - i__ + j * 21];
	    abssum = (d__1 = funval[i__ + j * 21], abs(d__1)) + (d__2 = 
		    funval[22 - i__ + j * 21], abs(d__2));
	    basval[j] = w[i__ - 1] * sum + basval[j];
	    basabs[j] += w[i__ - 1] * abssum;
/* L50: */
	}
/* L60: */
    }

/*   Compute the null rule contributions from the other points. */
/*   Recall that one half of the nullrules is symmetric and the other */
/*   half is asymmetric. */

    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	for (k = 1; k <= 10; ++k) {
	    sum = funval[k + j * 21] + funval[22 - k + j * 21];
	    diff = funval[k + j * 21] - funval[22 - k + j * 21];
	    for (i__ = 1; i__ <= 4; ++i__) {
		i2 = i__ << 1;
		i1 = i2 - 1;
		null[i1 + (j << 4)] += nullw[i1 + (k << 4) - 17] * sum;
		null[i2 + (j << 4)] += nullw[i2 + (k << 4) - 17] * diff;
/* L70: */
	    }
/* L80: */
	}
/* L90: */
    }

/*    Scale the results with the length of the interval */

    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	basval[j] = hlen * basval[j];
	basabs[j] = hlen * basabs[j];
/* L100: */
    }

/*    Compute errors. */

    *greate = 0.;
    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	emax = 0.;
	for (i__ = 1; i__ <= 4; ++i__) {
	    i2 = i__ << 1;
	    i1 = i2 - 1;
/* Computing 2nd power */
	    d__1 = null[i1 + (j << 4)];
/* Computing 2nd power */
	    d__2 = null[i2 + (j << 4)];
	    e[i__ - 1] = hlen * sqrt(d__1 * d__1 + d__2 * d__2);
/* Computing MAX */
	    d__1 = emax, d__2 = e[i__ - 1];
	    emax = max(d__1,d__2);
/* L110: */
	}
	r__ = 0.;
	for (i__ = 1; i__ <= 3; ++i__) {
	    if (e[i__] != 0.) {
		rr[i__ - 1] = e[i__ - 1] / e[i__];
	    } else {
		rr[i__ - 1] = 1.;
	    }
/* Computing MAX */
	    d__1 = r__, d__2 = rr[i__ - 1];
	    r__ = max(d__1,d__2);
/* L120: */
	}
	if (r__ >= 1.) {
	    rgnerr[j] = emax * 10.;
	} else if (r__ >= .5) {
	    rgnerr[j] = r__ * 10. * e[0];
	} else {
	    rgnerr[j] = const__ * pow_dd(&r__, &c_b18) * e[0];
	}

/*  Check the noise level. */

	noise = *epmach * 50 * basabs[j];
	if (e[0] < noise && e[1] < noise) {
	    rgnerr[j] = noise;
	} else {
/* Computing MAX */
	    d__1 = noise, d__2 = rgnerr[j];
	    rgnerr[j] = max(d__1,d__2);
	}

/*  Check if this is the greatest error so far among the remaining */
/*  problems. One exception: If the user wants to force the code */
/*  to do extra work (which seems unnecessary). */

	if (! (*more && done[j]) && rgnerr[j] > *greate) {
	    *greate = rgnerr[j];
	    *worst = j;
	}
/* L130: */
    }

/*   Compute the new subdivision points. */

    *c__ = *a + (*b - *a) / 3;
    *d__ = *b - (*b - *a) / 3;
    return 0;

/* ***END DGAINT */

} /* dgaint_ */

#ifdef __cplusplus
	}
#endif
