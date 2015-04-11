/* sgaint.f -- translated by f2c (version 20000118).
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

/* Subroutine */ int sgaint_(real *a, real *b, integer *numfun, S_fp funsub, 
	logical *done, logical *more, real *epmach, real *null, real *funval, 
	real *basabs, integer *flag__, real *basval, real *rgnerr, real *
	greate, integer *worst, real *c__, real *d__)
{
    /* Initialized data */

    static real g[11] = { (float).9937521706203895,(float).9672268385663062,(
	    float).9200993341504008,(float).8533633645833172,(float)
	    .7684399634756779,(float).6671388041974123,(float)
	    .5516188358872198,(float).4243421202074387,(float)
	    .288021316802401,(float).145561854160895,(float)0. };
    static real w[11] = { (float).01601722825777433,(float).03695378977085249,
	    (float).0571344254268572,(float).0761001136283793,(float)
	    .09344442345603386,(float).1087972991671483,(float)
	    .1218314160537285,(float).1322689386333374,(float)
	    .1398873947910731,(float).14452440398997,(float).1460811336496904 
	    };
    static real nullw[176]	/* was [16][11] */ = { (float)
	    .005859224694730026,(float).01301976799706014,(float)
	    .02158184987561927,(float).03119657617222001,(float)
	    .04157639307601386,(float).05246625962340516,(float)
	    .06363031447247006,(float).07484599067063009,(float)
	    .08590167508061712,(float).09659618304508728,(float)
	    .1067392105869384,(float).116152319105073,(float).124670195533083,
	    (float).1321420280297885,(float).1384328909795413,(float)
	    .1434250647895144,(float)-.02024707000741622,(float)
	    -.04379005851020851,(float)-.06965227115767195,(float)
	    -.09516116459787523,(float)-.1179114894020921,(float)
	    -.1358182960749584,(float)-.1472028208379138,(float)
	    -.1508776892345244,(float)-.1462121283895959,(float)
	    -.1331667766592828,(float)-.1122928178247447,(float)
	    -.08469391287377601,(float)-.05195253287985397,(float)
	    -.01602500237425218,(float).02088816813445404,(float)
	    .05648832390790171,(float).03883580247121445,(float)
	    .07990096087086482,(float).1174438965053943,(float)
	    .1431705707841666,(float).1511566572815612,(float)
	    .1386355746642898,(float).1063757528761779,(float)
	    .05853283458897407,(float).001973066649848703,(float)
	    -.05483608118503819,(float)-.1031959020477783,(float)
	    -.1355896186086413,(float)-.1469123277046623,(float)
	    -.1353193782985104,(float)-.1025551817987029,(float)
	    -.05370731005946019,(float)-.05965412595242497,(float)
	    -.1138307201442027,(float)-.1473794001233916,(float)
	    -.1462171986707822,(float)-.1073644825716617,(float)
	    -.03967547044517777,(float).03881687506910375,(float)
	    .1062457136342151,(float).1434120884358229,(float)
	    .1395396581193282,(float).09558143541939021,(float)
	    .02408868272651161,(float)-.05433985749387003,(float)
	    -.1170027382391999,(float)-.1456993480020755,(float)
	    -.1320553898020212,(float).0811427949834302,(float)
	    .1394263659262734,(float).1481989091733945,(float)
	    .09677919508585969,(float).004174411212397235,(float)
	    -.08983329656061084,(float)-.1432999618142209,(float)
	    -.1318732622123368,(float)-.06050045998747565,(float)
	    .0384221933717722,(float).1196951864405313,(float)
	    .1460359069105494,(float).1052913655334742,(float)
	    .01614011431557236,(float)-.08041833842953963,(float)
	    -.1399117916675364,(float)-.1019231472030305,(float)
	    -.1520456605731098,(float)-.1168273220215079,(float)
	    -.01075583592960879,(float).1012057375471417,(float)
	    .147180195880142,(float).0969896942429765,(float)
	    -.01673118490576078,(float)-.1192968264851738,(float)
	    -.1430606163131484,(float)-.0722598400037873,(float)
	    .04632194803727984,(float).1341759283651172,(float)
	    .1330641979525424,(float).04369891359834745,(float)
	    -.07464504070837483,(float).1207652963052454,(float)
	    .1489588260146731,(float).05890214560028095,(float)
	    -.07936141880173113,(float)-.1472858866418607,(float)
	    -.08524048604745531,(float).05209450556829039,(float)
	    .1428979325476485,(float).1063582979588903,(float)
	    -.02498840938693774,(float)-.1339424732473705,(float)
	    -.1231814607655799,(float)-.002310968036052471,(float)
	    .1205891076794731,(float).1355713757017371,(float)
	    .0292225945939076,(float)-.136604379671161,(float)
	    -.1296181347851887,(float).01273585156460484,(float)
	    .138074955247293,(float).1063772962797608,(float)
	    -.0461729811485722,(float)-.1455658951943161,(float)
	    -.07818432195969258,(float).07871971420591506,(float)
	    .1451753725543706,(float).04492456833975673,(float)
	    -.1068762140630544,(float)-.1358135230198954,(float)
	    -.00864091910814602,(float).1284341564046552,(float)
	    .1177993871727999,(float).1485698262567817,(float)
	    .09568420420614478,(float)-.08133037350927629,(float)
	    -.141757711722709,(float)-.002323645744823691,(float)
	    .1397282757969823,(float).08343286549711612,(float)
	    -.09112601052788798,(float)-.1360664606736734,(float)
	    .01236834040097046,(float).1431238351576778,(float)
	    .07029919038187181,(float)-.1024826424015055,(float)
	    -.1294253464460428,(float).02777799655524739,(float)
	    .1454239155303997,(float)-.1560150459859118,(float)
	    -.05078074459100106,(float).1304777802379009,(float)
	    .08867798725104829,(float)-.1030910117645103,(float)
	    -.1185867937861861,(float).06800562635441401,(float)
	    .138284949606409,(float)-.02747426951466367,(float)
	    -.1461902970879641,(float)-.01523606417516131,(float)
	    .1416700606593806,(float).05656562071840163,(float)
	    -.1251272318395094,(float)-.09304002613430708,(float)
	    .09797588771824906,(float).1585416482170856,(float)0.,(float)
	    -.1483021322906934,(float)0.,(float).1469720414561505,(float)0.,(
	    float)-.1465539124681842,(float)0.,(float).1463706054390223,(
	    float)0.,(float)-.1462742772906911,(float)0.,(float)
	    .1462174827723311,(float)0.,(float)-.146181214016595,(float)0. };

    /* System generated locals */
    integer i__1;
    real r__1, r__2;
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), sqrt(doublereal);

    /* Local variables */
    real diff, hlen, emax, e[4];
    integer i__, j, k;
    real r__, x[21], centr, noise, const__;
    integer i1, i2;
    real rr[3], abscis, abssum, sum;

/* ***BEGIN PROLOGUE SGAINT */
/* ***PURPOSE  To compute basic integration rule values and */
/*            corresponding error estimates. */

/* ***LAST MODIFICATION 94-03-10 */

/* ***REFER TO SQAINF */

/* ***DESCRIPTION SGAINT computes basic integration rule values */
/*            for a vector of integrands over an interval. */
/*            SGAINT also computes estimates for the errors by */
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
/* ***END PROLOGUE SGAINT */

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
/*          In order to reduce the number of loops in SGAINT, */
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


/* ***FIRST EXECUTABLE STATEMENT SGAINT */

/*  Define constants */

    const__ = (float)10. * pow_dd(&c_b2, &c_b3);


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
	basabs[j] = (r__1 = basval[j], dabs(r__1));
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
	    abssum = (r__1 = funval[i__ + j * 21], dabs(r__1)) + (r__2 = 
		    funval[22 - i__ + j * 21], dabs(r__2));
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

    *greate = (float)0.;
    i__1 = *numfun;
    for (j = 1; j <= i__1; ++j) {
	emax = (float)0.;
	for (i__ = 1; i__ <= 4; ++i__) {
	    i2 = i__ << 1;
	    i1 = i2 - 1;
/* Computing 2nd power */
	    r__1 = null[i1 + (j << 4)];
/* Computing 2nd power */
	    r__2 = null[i2 + (j << 4)];
	    e[i__ - 1] = hlen * sqrt(r__1 * r__1 + r__2 * r__2);
/* Computing MAX */
	    r__1 = emax, r__2 = e[i__ - 1];
	    emax = dmax(r__1,r__2);
/* L110: */
	}
	r__ = (float)0.;
	for (i__ = 1; i__ <= 3; ++i__) {
	    if (e[i__] != (float)0.) {
		rr[i__ - 1] = e[i__ - 1] / e[i__];
	    } else {
		rr[i__ - 1] = (float)1.;
	    }
/* Computing MAX */
	    r__1 = r__, r__2 = rr[i__ - 1];
	    r__ = dmax(r__1,r__2);
/* L120: */
	}
	if (r__ >= (float)1.) {
	    rgnerr[j] = emax * (float)10.;
	} else if (r__ >= (float).5) {
	    rgnerr[j] = r__ * (float)10. * e[0];
	} else {
	    d__1 = (doublereal) r__;
	    rgnerr[j] = const__ * pow_dd(&d__1, &c_b18) * e[0];
	}

/*  Check the noise level. */

	noise = *epmach * 50 * basabs[j];
	if (e[0] < noise && e[1] < noise) {
	    rgnerr[j] = noise;
	} else {
/* Computing MAX */
	    r__1 = noise, r__2 = rgnerr[j];
	    rgnerr[j] = dmax(r__1,r__2);
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

/* ***END SGAINT */

} /* sgaint_ */

#ifdef __cplusplus
	}
#endif
