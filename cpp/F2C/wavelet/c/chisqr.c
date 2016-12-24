/* chisqr.f -- translated by f2c (version 20060506).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static doublereal c_b25 = 0.;
static doublereal c_b26 = 1e300;
static doublereal c_b27 = .5;
static doublereal c_b29 = 5.;
static doublereal c_b30 = 1e-50;
static doublereal c_b31 = 1e-8;
static doublereal c_b40 = 1e-300;
static integer c__0 = 0;
static doublereal c_b125 = 1.;
static integer c__4 = 4;
static integer c__9 = 9;
static integer c__10 = 10;
static integer c__3 = 3;
static integer c__8 = 8;

/* ************************************************************************** */
/*      DCDFLIB: Available at http://www.netlib.org/random/ */

/*   ***NOTE (C.T.) *** this is a subset of DCDFLIB, which only includes */
/*            the routines used by the CHI SQUARE cumulative distribution. */

/* Modified: November 1999 by Arjan van Dijk to include IMPLICIT NONE and */
/*           to convert all routines to DOUBLE precision. */
/* ************************************************************************** */
/* ************************************************************************** */
/*                                           DCDFLIB */
/*                  Library of Fortran Routines for Cumulative Distribution */
/*                       Functions, Inverses, and Other Parameters */
/*                                    (February, 1994) */
/*                            Compiled and Written by: */
/*                                     Barry W. Brown */
/*                                      James Lovato */
/*                                      Kathy Russell */


/*                              LEGALITIES */

/* Code that appeared  in an    ACM  publication  is subject  to    their */
/* algorithms policy: */

/*        Submittal of  an  algorithm  for publication  in   one of   the  ACM */
/*        Transactions implies that unrestricted use  of the algorithm within  a */
/*        computer is permissible.     General permission  to copy and  distribute */
/*        the algorithm without fee is granted provided that the copies        are not */
/*        made  or     distributed for  direct   commercial  advantage.    The ACM */
/*        copyright notice and the title of the publication and its date appear, */
/*        and  notice is given that copying  is by permission of the Association */
/*        for Computing Machinery.  To copy otherwise, or to republish, requires */
/*        a fee and/or specific permission. */

/*        Krogh, F.  Algorithms        Policy.  ACM  Tran.   Math.  Softw.   13(1987), */
/*        183-186. */

/* We place the DCDFLIB code that we have written in the public domain. */

/*                                     NO WARRANTY */

/*        WE PROVIDE ABSOLUTELY        NO WARRANTY  OF ANY  KIND  EITHER  EXPRESSED OR */
/*        IMPLIED,  INCLUDING BUT   NOT LIMITED TO,  THE  IMPLIED  WARRANTIES OF */
/*        MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.        THE ENTIRE RISK */
/*        AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS  WITH YOU.  SHOULD */
/*        THIS PROGRAM PROVE  DEFECTIVE, YOU ASSUME  THE COST  OF  ALL NECESSARY */
/*        SERVICING, REPAIR OR CORRECTION. */

/*        IN NO        EVENT  SHALL THE UNIVERSITY  OF TEXAS OR  ANY  OF ITS COMPONENT */
/*        INSTITUTIONS INCLUDING M. D. ANDERSON HOSPITAL BE LIABLE  TO YOU FOR */
/*        DAMAGES, INCLUDING ANY  LOST PROFITS, LOST MONIES,   OR OTHER SPECIAL, */
/*        INCIDENTAL   OR  CONSEQUENTIAL DAMAGES   ARISING     OUT  OF  THE USE OR */
/*        INABILITY TO USE (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA OR */
/*        ITS ANALYSIS BEING  RENDERED INACCURATE OR  LOSSES SUSTAINED  BY THIRD */
/*        PARTIES) THE PROGRAM. */

/*        (Above NO WARRANTY modified from the GNU NO WARRANTY statement.) */

/* Subroutine */ int cdfchi_(integer *which, doublereal *p, doublereal *q, 
	doublereal *x, doublereal *df, integer *status, doublereal *bound)
{
    /* System generated locals */
    doublereal d__1;

    /* Local variables */
    doublereal fx, pq;
    logical qhi;
    doublereal cum, ccum, porq;
    logical qleft;
    extern /* Subroutine */ int dinvr_(integer *, doublereal *, doublereal *, 
	    logical *, logical *);
    logical qporq;
    extern /* Subroutine */ int cumchi_(doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    extern doublereal spmpar_(integer *);
    extern /* Subroutine */ int dstinv_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);

/* ********************************************************************** */

/*      SUBROUTINE CDFCHI( WHICH, P, Q, X, DF, STATUS, BOUND ) */
/*             Cumulative Distribution Function */
/*             CHI-Square distribution */


/*                         Function */


/*     Calculates any one parameter of the chi-square */
/*     distribution given values for the others. */


/*                         Arguments */


/*     WHICH --> Integer indicating which of the next three argument */
/*             values is to be calculated from the others. */
/*             Legal range: 1..3 */
/*             iwhich = 1 : Calculate P and Q from X and DF */
/*             iwhich = 2 : Calculate X from P,Q and DF */
/*             iwhich = 3 : Calculate DF from P,Q and X */
/*                  INTEGER WHICH */

/*     P <--> The integral from 0 to X of the chi-square */
/*            distribution. */
/*            Input range: [0, 1]. */
/*                  DOUBLE PRECISION P */

/*     Q <--> 1-P. */
/*            Input range: (0, 1]. */
/*            P + Q = 1.0. */
/*                  DOUBLE PRECISION Q */

/*     X <--> Upper limit of integration of the non-central */
/*            chi-square distribution. */
/*            Input range: [0, +infinity). */
/*            Search range: [0,1E300] */
/*                  DOUBLE PRECISION X */

/*     DF <--> Degrees of freedom of the */
/*             chi-square distribution. */
/*             Input range: (0, +infinity). */
/*             Search range: [ 1E-300, 1E300] */
/*                  DOUBLE PRECISION DF */

/*     STATUS <-- 0 if calculation completed correctly */
/*             -I if input parameter number I is out of range */
/*              1 if answer appears to be lower than lowest */
/*                search bound */
/*              2 if answer appears to be higher than greatest */
/*                search bound */
/*              3 if P + Q .ne. 1 */
/*             10 indicates error returned from cumgam.  See */
/*                references in cdfgam */
/*                  INTEGER STATUS */

/*     BOUND <-- Undefined if STATUS is 0 */

/*             Bound exceeded by parameter number I if STATUS */
/*             is negative. */

/*             Lower search bound if STATUS is 1. */

/*             Upper search bound if STATUS is 2. */


/*                         Method */


/*     Formula    26.4.19   of Abramowitz  and       Stegun, Handbook  of */
/*     Mathematical Functions   (1966) is used   to reduce the chisqure */
/*     distribution to the incomplete distribution. */

/*     Computation of other parameters involve a seach for a value that */
/*     produces  the desired  value  of P.   The search relies  on  the */
/*     monotinicity of P with the other parameter. */

/* ********************************************************************** */
/*     .. Parameters .. */
/*     .. */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Check arguments */

    if (! (*which < 1 || *which > 3)) {
	goto L30;
    }
    if (! (*which < 1)) {
	goto L10;
    }
    *bound = 1.;
    goto L20;
L10:
    *bound = 3.;
L20:
    *status = -1;
    return 0;
L30:
    if (*which == 1) {
	goto L70;
    }

/*     P */

    if (! (*p < 0. || *p > 1.)) {
	goto L60;
    }
    if (! (*p < 0.)) {
	goto L40;
    }
    *bound = 0.;
    goto L50;
L40:
    *bound = 1.;
L50:
    *status = -2;
    return 0;
L60:
L70:
    if (*which == 1) {
	goto L110;
    }

/*     Q */

    if (! (*q <= 0. || *q > 1.)) {
	goto L100;
    }
    if (! (*q <= 0.)) {
	goto L80;
    }
    *bound = 0.;
    goto L90;
L80:
    *bound = 1.;
L90:
    *status = -3;
    return 0;
L100:
L110:
    if (*which == 2) {
	goto L130;
    }

/*     X */

    if (! (*x < 0.)) {
	goto L120;
    }
    *bound = 0.;
    *status = -4;
    return 0;
L120:
L130:
    if (*which == 3) {
	goto L150;
    }

/*     DF */

    if (! (*df <= 0.)) {
	goto L140;
    }
    *bound = 0.;
    *status = -5;
    return 0;
L140:
L150:
    if (*which == 1) {
	goto L190;
    }

/*     P + Q */

    pq = *p + *q;
    if (! ((d__1 = pq - .5 - .5, abs(d__1)) > spmpar_(&c__1) * 3.)) {
	goto L180;
    }
    if (! (pq < 0.)) {
	goto L160;
    }
    *bound = 0.;
    goto L170;
L160:
    *bound = 1.;
L170:
    *status = 3;
    return 0;
L180:
L190:
    if (*which == 1) {
	goto L220;
    }

/*     Select the minimum of P or Q */

    qporq = *p <= *q;
    if (! qporq) {
	goto L200;
    }
    porq = *p;
    goto L210;
L200:
    porq = *q;
L210:

/*     Calculate ANSWERS */

L220:
    if (1 == *which) {

/*     Calculating P and Q */

	*status = 0;
	cumchi_(x, df, p, q);
	if (porq > 1.5) {
	    *status = 10;
	    return 0;
	}
    } else if (2 == *which) {

/*     Calculating X */

	*x = 5.;
	dstinv_(&c_b25, &c_b26, &c_b27, &c_b27, &c_b29, &c_b30, &c_b31);
	*status = 0;
	dinvr_(status, x, &fx, &qleft, &qhi);
L230:
	if (! (*status == 1)) {
	    goto L270;
	}
	cumchi_(x, df, &cum, &ccum);
	if (! qporq) {
	    goto L240;
	}
	fx = cum - *p;
	goto L250;
L240:
	fx = ccum - *q;
L250:
	if (! (fx + porq > 1.5)) {
	    goto L260;
	}
	*status = 10;
	return 0;
L260:
	dinvr_(status, x, &fx, &qleft, &qhi);
	goto L230;
L270:
	if (! (*status == -1)) {
	    goto L300;
	}
	if (! qleft) {
	    goto L280;
	}
	*status = 1;
	*bound = 0.;
	goto L290;
L280:
	*status = 2;
	*bound = 1e300;
L290:
L300:
	;
    } else if (3 == *which) {

/*     Calculating DF */

	*df = 5.;
	dstinv_(&c_b40, &c_b26, &c_b27, &c_b27, &c_b29, &c_b30, &c_b31);
	*status = 0;
	dinvr_(status, df, &fx, &qleft, &qhi);
L310:
	if (! (*status == 1)) {
	    goto L350;
	}
	cumchi_(x, df, &cum, &ccum);
	if (! qporq) {
	    goto L320;
	}
	fx = cum - *p;
	goto L330;
L320:
	fx = ccum - *q;
L330:
	if (! (fx + porq > 1.5)) {
	    goto L340;
	}
	*status = 10;
	return 0;
L340:
	dinvr_(status, df, &fx, &qleft, &qhi);
	goto L310;
L350:
	if (! (*status == -1)) {
	    goto L380;
	}
	if (! qleft) {
	    goto L360;
	}
	*status = 1;
	*bound = 1e-300;
	goto L370;
L360:
	*status = 2;
	*bound = 1e300;
L370:
L380:
	;
    }
    return 0;
} /* cdfchi_ */

/* Subroutine */ int cumchi_(doublereal *x, doublereal *df, doublereal *cum, 
	doublereal *ccum)
{
    doublereal a, xx;
    extern /* Subroutine */ int cumgam_(doublereal *, doublereal *, 
	    doublereal *, doublereal *);

/* ********************************************************************** */

/*     SUBROUTINE FUNCTION CUMCHI(X,DF,CUM,CCUM) */
/*             CUMulative of the CHi-square distribution */


/*                         Function */


/*     Calculates the cumulative chi-square distribution. */


/*                         Arguments */


/*     X       --> Upper limit of integration of the */
/*               chi-square distribution. */
/*                             X is DOUBLE PRECISION */

/*     DF      --> Degrees of freedom of the */
/*               chi-square distribution. */
/*                             DF is DOUBLE PRECISION */

/*     CUM <-- Cumulative chi-square distribution. */
/*                             CUM is DOUBLE PRECISIO */

/*     CCUM <-- Compliment of Cumulative chi-square distribution. */
/*                             CCUM is DOUBLE PRECISI */


/*                         Method */


/*     Calls incomplete gamma function (CUMGAM) */

/* ********************************************************************** */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Executable Statements .. */
    a = *df * .5;
    xx = *x * .5;
    cumgam_(&xx, &a, cum, ccum);
    return 0;
} /* cumchi_ */

/* Subroutine */ int cumgam_(doublereal *x, doublereal *a, doublereal *cum, 
	doublereal *ccum)
{
    extern /* Subroutine */ int gratio_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *);

/* ********************************************************************** */

/*     SUBROUTINE CUMGAM(X,A,CUM,CCUM) */
/*           Double precision cUMulative incomplete GAMma distribution */


/*                         Function */


/*     Computes   the  cumulative of    the     incomplete   gamma */
/*     distribution, i.e., the integral from 0 to X of */
/*          (1/GAM(A))*EXP(-T)*T**(A-1) DT */
/*     where GAM(A) is the complete gamma function of A, i.e., */
/*          GAM(A) = integral from 0 to infinity of */
/*                  EXP(-T)*T**(A-1) DT */


/*                         Arguments */


/*     X --> The upper limit of integration of the incomplete gamma. */
/*                            X is DOUBLE PRECISION */

/*     A --> The shape parameter of the incomplete gamma. */
/*                            A is DOUBLE PRECISION */

/*     CUM <-- Cumulative incomplete gamma distribution. */
/*                          CUM is DOUBLE PRECISION */

/*     CCUM <-- Compliment of Cumulative incomplete gamma distribution. */
/*                            CCUM is DOUBLE PRECISIO */


/*                         Method */


/*     Calls the routine GRATIO. */

/* ********************************************************************** */

/*     .. */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. External Routines .. */
/*     .. */
/*     .. Executable Statements .. */
    if (! (*x <= 0.)) {
	goto L10;
    }
    *cum = 0.;
    *ccum = 1.;
    return 0;
L10:
    gratio_(a, x, cum, ccum, &c__0);
/*     Call gratio routine */
    return 0;
} /* cumgam_ */

/* Subroutine */ int dinvr_0_(int n__, integer *status, doublereal *x, 
	doublereal *fx, logical *qleft, logical *qhi, doublereal *zsmall, 
	doublereal *zbig, doublereal *zabsst, doublereal *zrelst, doublereal *
	zstpmu, doublereal *zabsto, doublereal *zrelto)
{
    /* Format strings */
    static char fmt_10[] = "";
    static char fmt_20[] = "";
    static char fmt_90[] = "";
    static char fmt_130[] = "";
    static char fmt_200[] = "";
    static char fmt_270[] = "";

    /* System generated locals */
    doublereal d__1, d__2;

    /* Builtin functions */
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static doublereal yy, big, xlb, xhi, xub, xlo;
    static logical qup;
    static integer i99999;
    static doublereal fbig;
    static logical qbdd, qlim;
    static doublereal step;
    static logical qdum1, qdum2, qcond;
    static doublereal small;
    static logical qincr;
    static doublereal xsave;
    extern /* Subroutine */ int dzror_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, logical *, logical *), dstzr_(
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static doublereal fsmall, abstol, absstp, reltol, relstp, stpmul;

    /* Assigned format variables */
    static char *i99999_fmt;

/* ********************************************************************** */

/*     SUBROUTINE DINVR(STATUS, X, FX, QLEFT, QHI) */
/*          Double precision */
/*          bounds the zero of the function and invokes zror */
/*                  Reverse Communication */


/*                         Function */


/*     Bounds the    function  and  invokes  ZROR   to perform the   zero */
/*     finding.  STINVR   must  have   been  called  before this     routine */
/*     in order to set its parameters. */


/*                         Arguments */


/*     STATUS <--> At the beginning of a zero finding problem, STATUS */
/*               should be set to 0 and INVR invoked.       (The value */
/*               of parameters other than X will be ignored on this cal */

/*               When INVR needs the function evaluated, it will set */
/*               STATUS to 1 and return.  The value of the function */
/*               should be set in FX and INVR again called without */
/*               changing any of its other parameters. */

/*               When INVR has finished without error, it will return */
/*               with STATUS 0.  In that case X is approximately a root */
/*               of F(X). */

/*               If INVR cannot bound the function, it returns status */
/*               -1 and sets QLEFT and QHI. */
/*                    INTEGER STATUS */

/*     X <-- The value of X at which F(X) is to be evaluated. */
/*                    DOUBLE PRECISION X */

/*     FX --> The value of F(X) calculated when INVR returns with */
/*            STATUS = 1. */
/*                    DOUBLE PRECISION FX */

/*     QLEFT <-- Defined only if QMFINV returns .FALSE.  In that */
/*          case it is .TRUE. If the stepping search terminated */
/*          unsucessfully at SMALL.  If it is .FALSE. the search */
/*          terminated unsucessfully at BIG. */
/*                  QLEFT is LOGICAL */

/*     QHI <-- Defined only if QMFINV returns .FALSE.  In that */
/*          case it is .TRUE. if F(X) .GT. Y at the termination */
/*          of the search and .FALSE. if F(X) .LT. Y at the */
/*          termination of the search. */
/*                  QHI is LOGICAL */

/* ********************************************************************** */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Save statement .. */
/*     .. */
/*     .. Statement Function definitions .. */
/*     .. */
/*     .. Executable Statements .. */
    switch(n__) {
	case 1: goto L_dstinv;
	}

    if (*status > 0) {
	goto L310;
    }
    qcond = ! (small <= *x && *x <= big);
    if (qcond) {
	s_stop(" SMALL, X, BIG not monotone in INVR", (ftnlen)35);
    }
    xsave = *x;

/*     See that SMALL and BIG bound the zero and set QINCR */

    *x = small;
/*     GET-FUNCTION-VALUE */
    i99999 = 0;
    i99999_fmt = fmt_10;
    goto L300;
L10:
    fsmall = *fx;
    *x = big;
/*     GET-FUNCTION-VALUE */
    i99999 = 1;
    i99999_fmt = fmt_20;
    goto L300;
L20:
    fbig = *fx;
    qincr = fbig > fsmall;
    if (! qincr) {
	goto L50;
    }
    if (fsmall <= 0.) {
	goto L30;
    }
    *status = -1;
    *qleft = TRUE_;
    *qhi = TRUE_;
    return 0;
L30:
    if (fbig >= 0.) {
	goto L40;
    }
    *status = -1;
    *qleft = FALSE_;
    *qhi = FALSE_;
    return 0;
L40:
    goto L80;
L50:
    if (fsmall >= 0.) {
	goto L60;
    }
    *status = -1;
    *qleft = TRUE_;
    *qhi = FALSE_;
    return 0;
L60:
    if (fbig <= 0.) {
	goto L70;
    }
    *status = -1;
    *qleft = FALSE_;
    *qhi = TRUE_;
    return 0;
L70:
L80:
    *x = xsave;
/* Computing MAX */
    d__1 = absstp, d__2 = relstp * abs(*x);
    step = max(d__1,d__2);
/*      YY = F(X) - Y */
/*     GET-FUNCTION-VALUE */
    i99999 = 2;
    i99999_fmt = fmt_90;
    goto L300;
L90:
    yy = *fx;
    if (! (yy == 0.)) {
	goto L100;
    }
    *status = 0;
    return 0;
L100:
    qup = qincr && yy < 0. || ! qincr && yy > 0.;
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*     HANDLE CASE IN WHICH WE MUST STEP HIGHER */

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    if (! qup) {
	goto L170;
    }
    xlb = xsave;
/* Computing MIN */
    d__1 = xlb + step;
    xub = min(d__1,big);
    goto L120;
L110:
    if (qcond) {
	goto L150;
    }
/*      YY = F(XUB) - Y */
L120:
    *x = xub;
/*     GET-FUNCTION-VALUE */
    i99999 = 3;
    i99999_fmt = fmt_130;
    goto L300;
L130:
    yy = *fx;
    qbdd = qincr && yy >= 0. || ! qincr && yy <= 0.;
    qlim = xub >= big;
    qcond = qbdd || qlim;
    if (qcond) {
	goto L140;
    }
    step = stpmul * step;
    xlb = xub;
/* Computing MIN */
    d__1 = xlb + step;
    xub = min(d__1,big);
L140:
    goto L110;
L150:
    if (! (qlim && ! qbdd)) {
	goto L160;
    }
    *status = -1;
    *qleft = FALSE_;
    *qhi = ! qincr;
    *x = big;
    return 0;
L160:
    goto L240;
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*     HANDLE CASE IN WHICH WE MUST STEP LOWER */

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
L170:
    xub = xsave;
/* Computing MAX */
    d__1 = xub - step;
    xlb = max(d__1,small);
    goto L190;
L180:
    if (qcond) {
	goto L220;
    }
/*      YY = F(XLB) - Y */
L190:
    *x = xlb;
/*     GET-FUNCTION-VALUE */
    i99999 = 4;
    i99999_fmt = fmt_200;
    goto L300;
L200:
    yy = *fx;
    qbdd = qincr && yy <= 0. || ! qincr && yy >= 0.;
    qlim = xlb <= small;
    qcond = qbdd || qlim;
    if (qcond) {
	goto L210;
    }
    step = stpmul * step;
    xub = xlb;
/* Computing MAX */
    d__1 = xub - step;
    xlb = max(d__1,small);
L210:
    goto L180;
L220:
    if (! (qlim && ! qbdd)) {
	goto L230;
    }
    *status = -1;
    *qleft = TRUE_;
    *qhi = qincr;
    *x = small;
    return 0;
L230:
L240:
    dstzr_(&xlb, &xub, &abstol, &reltol);
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*     IF WE REACH HERE, XLB AND XUB BOUND THE ZERO OF F. */

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    *status = 0;
    goto L260;
L250:
    if (! (*status == 1)) {
	goto L290;
    }
L260:
    dzror_(status, x, fx, &xlo, &xhi, &qdum1, &qdum2);
    if (! (*status == 1)) {
	goto L280;
    }
/*     GET-FUNCTION-VALUE */
    i99999 = 5;
    i99999_fmt = fmt_270;
    goto L300;
L270:
L280:
    goto L250;
L290:
    *x = xlo;
    *status = 0;
    return 0;

L_dstinv:
/* ********************************************************************** */

/*      SUBROUTINE DSTINV( SMALL, BIG, ABSSTP, RELSTP, STPMUL, */
/*     +                  ABSTOL, RELTOL ) */
/*      Double Precision - SeT INverse finder - Reverse Communication */


/*                         Function */


/*     Concise Description - Given a monotone function F finds X */
/*     such that F(X) = Y.  Uses Reverse communication -- see invr. */
/*     This routine sets quantities needed by INVR. */

/*          More Precise Description of INVR - */

/*     F must be a monotone function, the results of QMFINV are */
/*     otherwise undefined.  QINCR must be .TRUE. if F is non- */
/*     decreasing and .FALSE. if F is non-increasing. */

/*     QMFINV will return .TRUE. if and only if F(SMALL) and */
/*     F(BIG) bracket Y, i. e., */
/*          QINCR is .TRUE. and F(SMALL).LE.Y.LE.F(BIG) or */
/*          QINCR is .FALSE. and F(BIG).LE.Y.LE.F(SMALL) */

/*     if QMFINV returns .TRUE., then the X returned satisfies */
/*     the following condition.  let */
/*             TOL(X) = MAX(ABSTOL,RELTOL*ABS(X)) */
/*     then if QINCR is .TRUE., */
/*          F(X-TOL(X)) .LE. Y .LE. F(X+TOL(X)) */
/*     and if QINCR is .FALSE. */
/*          F(X-TOL(X)) .GE. Y .GE. F(X+TOL(X)) */


/*                         Arguments */


/*     SMALL --> The left endpoint of the interval to be */
/*          searched for a solution. */
/*                  SMALL is DOUBLE PRECISION */

/*     BIG --> The right endpoint of the interval to be */
/*          searched for a solution. */
/*                  BIG is DOUBLE PRECISION */

/*     ABSSTP, RELSTP --> The initial step size in the search */
/*          is MAX(ABSSTP,RELSTP*ABS(X)). See algorithm. */
/*                  ABSSTP is DOUBLE PRECISION */
/*                  RELSTP is DOUBLE PRECISION */

/*     STPMUL --> When a step doesn't bound the zero, the step */
/*              size is multiplied by STPMUL and another step */
/*              taken.  A popular value is 2.0 */
/*                  DOUBLE PRECISION STPMUL */

/*     ABSTOL, RELTOL --> Two numbers that determine the accuracy */
/*          of the solution.  See function for a precise definition. */
/*                  ABSTOL is DOUBLE PRECISION */
/*                  RELTOL is DOUBLE PRECISION */


/*                         Method */


/*     Compares F(X) with Y for the input value of X then uses QINCR */
/*     to determine whether to step left or right to bound the */
/*     desired x.  the initial step size is */
/*          MAX(ABSSTP,RELSTP*ABS(S)) for the input value of X. */
/*     Iteratively steps right or left until it bounds X. */
/*     At each step which doesn't bound X, the step size is doubled. */
/*     The routine is careful never to step beyond SMALL or BIG.  If */
/*     it hasn't bounded X at SMALL or BIG, QMFINV returns .FALSE. */
/*     after setting QLEFT and QHI. */

/*     If X is successfully bounded then Algorithm R of the paper */
/*     'Two Efficient Algorithms with Guaranteed Convergence for */
/*     Finding a Zero of a Function' by J. C. P. Bus and */
/*     T. J. Dekker in ACM Transactions on Mathematical */
/*     Software, Volume 1, No. 4 page 330 (DEC. '75) is employed */
/*     to find the zero of the function F(X)-Y. This is routine */
/*     QRZERO. */

/* ********************************************************************** */
    small = *zsmall;
    big = *zbig;
    absstp = *zabsst;
    relstp = *zrelst;
    stpmul = *zstpmu;
    abstol = *zabsto;
    reltol = *zrelto;
    return 0;
/*      STOP '*** EXECUTION FLOWING INTO FLECS PROCEDURES ***' */
/*     TO GET-FUNCTION-VALUE */
L300:
    *status = 1;
    return 0;
L310:
    switch (i99999) {
	case 0: goto L10;
	case 1: goto L20;
	case 2: goto L90;
	case 3: goto L130;
	case 4: goto L200;
	case 5: goto L270;
    }
} /* dinvr_ */

/* Subroutine */ int dinvr_(integer *status, doublereal *x, doublereal *fx, 
	logical *qleft, logical *qhi)
{
    return dinvr_0_(0, status, x, fx, qleft, qhi, (doublereal *)0, (
	    doublereal *)0, (doublereal *)0, (doublereal *)0, (doublereal *)0,
	     (doublereal *)0, (doublereal *)0);
    }

/* Subroutine */ int dstinv_(doublereal *zsmall, doublereal *zbig, doublereal 
	*zabsst, doublereal *zrelst, doublereal *zstpmu, doublereal *zabsto, 
	doublereal *zrelto)
{
    return dinvr_0_(1, (integer *)0, (doublereal *)0, (doublereal *)0, (
	    logical *)0, (logical *)0, zsmall, zbig, zabsst, zrelst, zstpmu, 
	    zabsto, zrelto);
    }

/* Subroutine */ int dzror_0_(int n__, integer *status, doublereal *x, 
	doublereal *fx, doublereal *xlo, doublereal *xhi, logical *qleft, 
	logical *qhi, doublereal *zxlo, doublereal *zxhi, doublereal *zabstl, 
	doublereal *zreltl)
{
    /* Format strings */
    static char fmt_10[] = "";
    static char fmt_20[] = "";
    static char fmt_200[] = "";

    /* System generated locals */
    doublereal d__1, d__2;

    /* Builtin functions */
    double d_sign(doublereal *, doublereal *);

    /* Local variables */
    static doublereal a, b, c__, d__, m, p, q, w, fa, fb, fc, fd, mb, fda, 
	    fdb, tol;
    static integer ext, i99999;
    static doublereal xxhi, xxlo;
    static logical first;
    static doublereal abstol, reltol;
    static logical qrzero;

    /* Assigned format variables */
    static char *i99999_fmt;

/* ********************************************************************** */

/*     SUBROUTINE DZROR(STATUS, X, FX, XLO, XHI, QLEFT, QHI) */
/*     Double precision ZeRo of a function -- Reverse Communication */


/*                         Function */


/*     Performs the zero finding.  STZROR must have been called before */
/*     this routine in order to set its parameters. */


/*                         Arguments */


/*     STATUS <--> At the beginning of a zero finding problem, STATUS */
/*               should be set to 0 and ZROR invoked.       (The value */
/*               of other parameters will be ignored on this call.) */

/*               When ZROR needs the function evaluated, it will set */
/*               STATUS to 1 and return.  The value of the function */
/*               should be set in FX and ZROR again called without */
/*               changing any of its other parameters. */

/*               When ZROR has finished without error, it will return */
/*               with STATUS 0.  In that case (XLO,XHI) bound the answe */

/*               If ZROR finds an error (which implies that F(XLO)-Y an */
/*               F(XHI)-Y have the same sign, it returns STATUS -1.  In */
/*               this case, XLO and XHI are undefined. */
/*                    INTEGER STATUS */

/*     X <-- The value of X at which F(X) is to be evaluated. */
/*                    DOUBLE PRECISION X */

/*     FX --> The value of F(X) calculated when ZROR returns with */
/*            STATUS = 1. */
/*                    DOUBLE PRECISION FX */

/*     XLO <-- When ZROR returns with STATUS = 0, XLO bounds the */
/*             inverval in X containing the solution below. */
/*                    DOUBLE PRECISION XLO */

/*     XHI <-- When ZROR returns with STATUS = 0, XHI bounds the */
/*             inverval in X containing the solution above. */
/*                    DOUBLE PRECISION XHI */

/*     QLEFT <-- .TRUE. if the stepping search terminated unsucessfully */
/*              at XLO.  If it is .FALSE. the search terminated */
/*              unsucessfully at XHI. */
/*                  QLEFT is LOGICAL */

/*     QHI <-- .TRUE. if F(X) .GT. Y at the termination of the */
/*              search and .FALSE. if F(X) .LT. Y at the */
/*              termination of the search. */
/*                  QHI is LOGICAL */

/* ********************************************************************** */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Save statement .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function definitions .. */
/*     .. */
/*     .. Executable Statements .. */
    switch(n__) {
	case 1: goto L_dstzr;
	}

    if (*status > 0) {
	goto L280;
    }
    *xlo = xxlo;
    *xhi = xxhi;
    b = *xlo;
    *x = *xlo;
/*     GET-FUNCTION-VALUE */
    i99999 = 0;
    i99999_fmt = fmt_10;
    goto L270;
L10:
    fb = *fx;
    *xlo = *xhi;
    a = *xlo;
    *x = *xlo;
/*     GET-FUNCTION-VALUE */
    i99999 = 1;
    i99999_fmt = fmt_20;
    goto L270;

/*     Check that F(ZXLO) < 0 < F(ZXHI)  or */
/*              F(ZXLO) > 0 > F(ZXHI) */

L20:
    if (! (fb < 0.)) {
	goto L40;
    }
    if (! (*fx < 0.)) {
	goto L30;
    }
    *status = -1;
    *qleft = *fx < fb;
    *qhi = FALSE_;
    return 0;
L30:
L40:
    if (! (fb > 0.)) {
	goto L60;
    }
    if (! (*fx > 0.)) {
	goto L50;
    }
    *status = -1;
    *qleft = *fx > fb;
    *qhi = TRUE_;
    return 0;
L50:
L60:
    fa = *fx;

    first = TRUE_;
L70:
    c__ = a;
    fc = fa;
    ext = 0;
L80:
    if (! (abs(fc) < abs(fb))) {
	goto L100;
    }
    if (! (c__ != a)) {
	goto L90;
    }
    d__ = a;
    fd = fa;
L90:
    a = b;
    fa = fb;
    *xlo = c__;
    b = *xlo;
    fb = fc;
    c__ = a;
    fc = fa;
L100:
/* Computing MAX */
    d__1 = abstol, d__2 = reltol * abs(*xlo);
    tol = .5 * max(d__1,d__2);
    m = (c__ + b) * .5;
    mb = m - b;
    if (! (abs(mb) > tol)) {
	goto L240;
    }
    if (! (ext > 3)) {
	goto L110;
    }
    w = mb;
    goto L190;
L110:
    tol = d_sign(&tol, &mb);
    p = (b - a) * fb;
    if (! first) {
	goto L120;
    }
    q = fa - fb;
    first = FALSE_;
    goto L130;
L120:
    fdb = (fd - fb) / (d__ - b);
    fda = (fd - fa) / (d__ - a);
    p = fda * p;
    q = fdb * fa - fda * fb;
L130:
    if (! (p < 0.)) {
	goto L140;
    }
    p = -p;
    q = -q;
L140:
    if (ext == 3) {
	p *= 2.;
    }
    if (! (p * 1. == 0. || p <= q * tol)) {
	goto L150;
    }
    w = tol;
    goto L180;
L150:
    if (! (p < mb * q)) {
	goto L160;
    }
    w = p / q;
    goto L170;
L160:
    w = mb;
L170:
L180:
L190:
    d__ = a;
    fd = fa;
    a = b;
    fa = fb;
    b += w;
    *xlo = b;
    *x = *xlo;
/*     GET-FUNCTION-VALUE */
    i99999 = 2;
    i99999_fmt = fmt_200;
    goto L270;
L200:
    fb = *fx;
    if (! (fc * fb >= 0.)) {
	goto L210;
    }
    goto L70;
L210:
    if (! (w == mb)) {
	goto L220;
    }
    ext = 0;
    goto L230;
L220:
    ++ext;
L230:
    goto L80;
L240:
    *xhi = c__;
    qrzero = fc >= 0. && fb <= 0. || fc < 0. && fb >= 0.;
    if (! qrzero) {
	goto L250;
    }
    *status = 0;
    goto L260;
L250:
    *status = -1;
L260:
    return 0;

L_dstzr:
/* ********************************************************************** */

/*     SUBROUTINE DSTZR( XLO, XHI, ABSTOL, RELTOL ) */
/*     Double precision SeT ZeRo finder - Reverse communication version */


/*                         Function */



/*     Sets quantities needed by ZROR.  The function of ZROR */
/*     and the quantities set is given here. */

/*     Concise Description - Given a function F */
/*     find XLO such that F(XLO) = 0. */

/*          More Precise Description - */

/*     Input condition. F is a double precision function of a single */
/*     double precision argument and XLO and XHI are such that */
/*          F(XLO)*F(XHI)  .LE.        0.0 */

/*     If the input condition is met, QRZERO returns .TRUE. */
/*     and output values of XLO and XHI satisfy the following */
/*          F(XLO)*F(XHI)  .LE. 0. */
/*          ABS(F(XLO)  .LE. ABS(F(XHI) */
/*          ABS(XLO-XHI)  .LE. TOL(X) */
/*     where */
/*          TOL(X) = MAX(ABSTOL,RELTOL*ABS(X)) */

/*     If this algorithm does not find XLO and XHI satisfying */
/*     these conditions then QRZERO returns .FALSE.  This */
/*     implies that the input condition was not met. */


/*                         Arguments */


/*     XLO --> The left endpoint of the interval to be */
/*           searched for a solution. */
/*                  XLO is DOUBLE PRECISION */

/*     XHI --> The right endpoint of the interval to be */
/*           for a solution. */
/*                  XHI is DOUBLE PRECISION */

/*     ABSTOL, RELTOL --> Two numbers that determine the accuracy */
/*                    of the solution.  See function for a */
/*                    precise definition. */
/*                  ABSTOL is DOUBLE PRECISION */
/*                  RELTOL is DOUBLE PRECISION */


/*                         Method */


/*     Algorithm R of the paper 'Two Efficient Algorithms with */
/*     Guaranteed Convergence for Finding a Zero of a Function' */
/*     by J. C. P. Bus and T. J. Dekker in ACM Transactions on */
/*     Mathematical Software, Volume 1, no. 4 page 330 */
/*     (Dec. '75) is employed to find the zero of F(X)-Y. */

/* ********************************************************************** */
    xxlo = *zxlo;
    xxhi = *zxhi;
    abstol = *zabstl;
    reltol = *zreltl;
    return 0;
/*      STOP '*** EXECUTION FLOWING INTO FLECS PROCEDURES ***' */
/*     TO GET-FUNCTION-VALUE */
L270:
    *status = 1;
    return 0;
L280:
    switch (i99999) {
	case 0: goto L10;
	case 1: goto L20;
	case 2: goto L200;
    }
} /* dzror_ */

/* Subroutine */ int dzror_(integer *status, doublereal *x, doublereal *fx, 
	doublereal *xlo, doublereal *xhi, logical *qleft, logical *qhi)
{
    return dzror_0_(0, status, x, fx, xlo, xhi, qleft, qhi, (doublereal *)0, (
	    doublereal *)0, (doublereal *)0, (doublereal *)0);
    }

/* Subroutine */ int dstzr_(doublereal *zxlo, doublereal *zxhi, doublereal *
	zabstl, doublereal *zreltl)
{
    return dzror_0_(1, (integer *)0, (doublereal *)0, (doublereal *)0, (
	    doublereal *)0, (doublereal *)0, (logical *)0, (logical *)0, zxlo,
	     zxhi, zabstl, zreltl);
    }

doublereal erf_(doublereal *x)
{
    /* Initialized data */

    static doublereal c__ = .564189583547756;
    static doublereal a[5] = { 7.7105849500132e-5,-.00133733772997339,
	    .0323076579225834,.0479137145607681,.128379167095513 };
    static doublereal b[3] = { .00301048631703895,.0538971687740286,
	    .375795757275549 };
    static doublereal p[8] = { -1.36864857382717e-7,.564195517478974,
	    7.21175825088309,43.1622272220567,152.98928504694,
	    339.320816734344,451.918953711873,300.459261020162 };
    static doublereal q[8] = { 1.,12.7827273196294,77.0001529352295,
	    277.585444743988,638.980264465631,931.35409485061,
	    790.950925327898,300.459260956983 };
    static doublereal r__[5] = { 2.10144126479064,26.2370141675169,
	    21.3688200555087,4.6580782871847,.282094791773523 };
    static doublereal s[4] = { 94.153775055546,187.11481179959,
	    99.0191814623914,18.0124575948747 };

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double exp(doublereal), d_sign(doublereal *, doublereal *);

    /* Local variables */
    doublereal t, x2, ax, bot, top;

/* ----------------------------------------------------------------------- */
/*             EVALUATION OF THE REAL ERROR FUNCTION */
/* ----------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Data statements .. */
/* ------------------------- */
/* ------------------------- */
/* ------------------------- */
/* ------------------------- */
/*     .. */
/*     .. Executable Statements .. */
/* ------------------------- */
    ax = abs(*x);
    if (ax > .5) {
	goto L10;
    }
    t = *x * *x;
    top = (((a[0] * t + a[1]) * t + a[2]) * t + a[3]) * t + a[4] + 1.;
    bot = ((b[0] * t + b[1]) * t + b[2]) * t + 1.;
    ret_val = *x * (top / bot);
    return ret_val;

L10:
    if (ax > 4.) {
	goto L20;
    }
    top = ((((((p[0] * ax + p[1]) * ax + p[2]) * ax + p[3]) * ax + p[4]) * ax 
	    + p[5]) * ax + p[6]) * ax + p[7];
    bot = ((((((q[0] * ax + q[1]) * ax + q[2]) * ax + q[3]) * ax + q[4]) * ax 
	    + q[5]) * ax + q[6]) * ax + q[7];
    ret_val = .5 - exp(-(*x) * *x) * top / bot + .5;
    if (*x < 0.) {
	ret_val = -ret_val;
    }
    return ret_val;

L20:
    if (ax >= 5.8) {
	goto L30;
    }
    x2 = *x * *x;
    t = 1. / x2;
    top = (((r__[0] * t + r__[1]) * t + r__[2]) * t + r__[3]) * t + r__[4];
    bot = (((s[0] * t + s[1]) * t + s[2]) * t + s[3]) * t + 1.;
    ret_val = (c__ - top / (x2 * bot)) / ax;
    ret_val = .5 - exp(-x2) * ret_val + .5;
    if (*x < 0.) {
	ret_val = -ret_val;
    }
    return ret_val;

L30:
    ret_val = d_sign(&c_b125, x);
    return ret_val;
} /* erf_ */

doublereal erfc1_(integer *ind, doublereal *x)
{
    /* Initialized data */

    static doublereal c__ = .564189583547756;
    static doublereal a[5] = { 7.7105849500132e-5,-.00133733772997339,
	    .0323076579225834,.0479137145607681,.128379167095513 };
    static doublereal b[3] = { .00301048631703895,.0538971687740286,
	    .375795757275549 };
    static doublereal p[8] = { -1.36864857382717e-7,.564195517478974,
	    7.21175825088309,43.1622272220567,152.98928504694,
	    339.320816734344,451.918953711873,300.459261020162 };
    static doublereal q[8] = { 1.,12.7827273196294,77.0001529352295,
	    277.585444743988,638.980264465631,931.35409485061,
	    790.950925327898,300.459260956983 };
    static doublereal r__[5] = { 2.10144126479064,26.2370141675169,
	    21.3688200555087,4.6580782871847,.282094791773523 };
    static doublereal s[4] = { 94.153775055546,187.11481179959,
	    99.0191814623914,18.0124575948747 };

    /* System generated locals */
    doublereal ret_val, d__1;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    doublereal e, t, w, ax, bot, top;
    extern doublereal exparg_(integer *);

/* ----------------------------------------------------------------------- */
/*         EVALUATION OF THE COMPLEMENTARY ERROR FUNCTION */

/*          ERFC1(IND,X) = ERFC(X)          IF IND = 0 */
/*          ERFC1(IND,X) = EXP(X*X)*ERFC(X)   OTHERWISE */
/* ----------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Data statements .. */
/* ------------------------- */
/* ------------------------- */
/* ------------------------- */
/* ------------------------- */
/*     .. */
/*     .. Executable Statements .. */
/* ------------------------- */

/*                   ABS(X) .LE. 0.5 */

    ax = abs(*x);
    if (ax > .5) {
	goto L10;
    }
    t = *x * *x;
    top = (((a[0] * t + a[1]) * t + a[2]) * t + a[3]) * t + a[4] + 1.;
    bot = ((b[0] * t + b[1]) * t + b[2]) * t + 1.;
    ret_val = .5 - *x * (top / bot) + .5;
    if (*ind != 0) {
	ret_val = exp(t) * ret_val;
    }
    return ret_val;

/*                0.5 .LT. ABS(X) .LE. 4 */

L10:
    if (ax > 4.) {
	goto L20;
    }
    top = ((((((p[0] * ax + p[1]) * ax + p[2]) * ax + p[3]) * ax + p[4]) * ax 
	    + p[5]) * ax + p[6]) * ax + p[7];
    bot = ((((((q[0] * ax + q[1]) * ax + q[2]) * ax + q[3]) * ax + q[4]) * ax 
	    + q[5]) * ax + q[6]) * ax + q[7];
    ret_val = top / bot;
    goto L40;

/*                    ABS(X) .GT. 4 */

L20:
    if (*x <= -5.6) {
	goto L60;
    }
    if (*ind != 0) {
	goto L30;
    }
    if (*x > 100.) {
	goto L70;
    }
    if (*x * *x > -exparg_(&c__1)) {
	goto L70;
    }

L30:
/* Computing 2nd power */
    d__1 = 1. / *x;
    t = d__1 * d__1;
    top = (((r__[0] * t + r__[1]) * t + r__[2]) * t + r__[3]) * t + r__[4];
    bot = (((s[0] * t + s[1]) * t + s[2]) * t + s[3]) * t + 1.;
    ret_val = (c__ - t * top / bot) / ax;

/*                    FINAL ASSEMBLY */

L40:
    if (*ind == 0) {
	goto L50;
    }
    if (*x < 0.) {
	ret_val = exp(*x * *x) * 2. - ret_val;
    }
    return ret_val;
L50:
    w = *x * *x;
    t = w;
    e = w - t;
    ret_val = (.5 - e + .5) * exp(-t) * ret_val;
    if (*x < 0.) {
	ret_val = 2. - ret_val;
    }
    return ret_val;

/*             LIMIT VALUE FOR LARGE NEGATIVE X */

L60:
    ret_val = 2.;
    if (*ind != 0) {
	ret_val = exp(*x * *x) * 2.;
    }
    return ret_val;

/*             LIMIT VALUE FOR LARGE POSITIVE X */
/*                  WHEN IND = 0 */

L70:
    ret_val = 0.;
    return ret_val;
} /* erfc1_ */

doublereal exparg_(integer *l)
{
    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    integer b, m;
    doublereal lnb;
    extern integer ipmpar_(integer *);

/* -------------------------------------------------------------------- */
/*     IF L = 0 THEN  EXPARG(L) = THE LARGEST POSITIVE W FOR WHICH */
/*     EXP(W) CAN BE COMPUTED. */

/*     IF L IS NONZERO THEN  EXPARG(L) = THE LARGEST NEGATIVE W FOR */
/*     WHICH THE COMPUTED VALUE OF EXP(W) IS NONZERO. */

/*     NOTE... ONLY AN APPROXIMATE VALUE FOR EXPARG(L) IS NEEDED. */
/* -------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    b = ipmpar_(&c__4);
    if (b != 2) {
	goto L10;
    }
    lnb = .69314718055995;
    goto L40;
L10:
    if (b != 8) {
	goto L20;
    }
    lnb = 2.0794415416798;
    goto L40;
L20:
    if (b != 16) {
	goto L30;
    }
    lnb = 2.7725887222398;
    goto L40;
L30:
    lnb = log((doublereal) b);

L40:
    if (*l == 0) {
	goto L50;
    }
    m = ipmpar_(&c__9) - 1;
    ret_val = m * lnb * .99999;
    return ret_val;
L50:
    m = ipmpar_(&c__10);
    ret_val = m * lnb * .99999;
    return ret_val;
} /* exparg_ */

doublereal gam1_(doublereal *a)
{
    /* Initialized data */

    static doublereal p[7] = { .577215664901533,-.409078193005776,
	    -.230975380857675,.0597275330452234,.0076696818164949,
	    -.00514889771323592,5.89597428611429e-4 };
    static doublereal q[5] = { 1.,.427569613095214,.158451672430138,
	    .0261132021441447,.00423244297896961 };
    static doublereal r__[9] = { -.422784335098468,-.771330383816272,
	    -.244757765222226,.118378989872749,9.30357293360349e-4,
	    -.0118290993445146,.00223047661158249,2.66505979058923e-4,
	    -1.32674909766242e-4 };
    static doublereal s1 = .273076135303957;
    static doublereal s2 = .0559398236957378;

    /* System generated locals */
    doublereal ret_val;

    /* Local variables */
    doublereal d__, t, w, bot, top;

/*     ------------------------------------------------------------------ */
/*     COMPUTATION OF 1/GAMMA(A+1) - 1  FOR -0.5 .LE. A .LE. 1.5 */
/*     ------------------------------------------------------------------ */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. Data statements .. */
/*     ------------------- */
/*     ------------------- */
/*     ------------------- */
/*     ------------------- */
/*     .. */
/*     .. Executable Statements .. */
/*     ------------------- */
    t = *a;
    d__ = *a - .5;
    if (d__ > 0.) {
	t = d__ - .5;
    }
    if (t < 0.) {
	goto L40;
    } else if (t == 0) {
	goto L10;
    } else {
	goto L20;
    }

L10:
    ret_val = 0.;
    return ret_val;

L20:
    top = (((((p[6] * t + p[5]) * t + p[4]) * t + p[3]) * t + p[2]) * t + p[1]
	    ) * t + p[0];
    bot = (((q[4] * t + q[3]) * t + q[2]) * t + q[1]) * t + 1.;
    w = top / bot;
    if (d__ > 0.) {
	goto L30;
    }
    ret_val = *a * w;
    return ret_val;
L30:
    ret_val = t / *a * (w - .5 - .5);
    return ret_val;

L40:
    top = (((((((r__[8] * t + r__[7]) * t + r__[6]) * t + r__[5]) * t + r__[4]
	    ) * t + r__[3]) * t + r__[2]) * t + r__[1]) * t + r__[0];
    bot = (s2 * t + s1) * t + 1.;
    w = top / bot;
    if (d__ > 0.) {
	goto L50;
    }
    ret_val = *a * (w + .5 + .5);
    return ret_val;
L50:
    ret_val = t * w / *a;
    return ret_val;
} /* gam1_ */

doublereal gamma_(doublereal *a)
{
    /* Initialized data */

    static doublereal q[7] = { -8.32979206704073e-4,.00470059485860584,
	    .022521113103534,-.17045896931336,-.056790276197494,
	    1.13062953091122,1. };
    static doublereal r1 = 8.20756370353826e-4;
    static doublereal r2 = -5.95156336428591e-4;
    static doublereal r3 = 7.93650663183693e-4;
    static doublereal r4 = -.00277777777770481;
    static doublereal r5 = .0833333333333333;
    static doublereal pi = 3.1415926535898;
    static doublereal d__ = .41893853320467274178;
    static doublereal p[7] = { 5.39637273585445e-4,.0026193926004269,
	    .020449366759492,.0730981088720487,.279648642639792,
	    .553413866010467,1. };

    /* System generated locals */
    integer i__1;
    doublereal ret_val;

    /* Builtin functions */
    double sin(doublereal), log(doublereal), exp(doublereal);

    /* Local variables */
    doublereal g;
    integer i__, j, m, n;
    doublereal s, t, w, x, z__, bot, lnx, top;
    extern doublereal exparg_(integer *), spmpar_(integer *);

/* ----------------------------------------------------------------------- */

/*         EVALUATION OF THE GAMMA FUNCTION FOR REAL ARGUMENTS */

/*                      ----------- */

/*     GAMMA(A) IS ASSIGNED THE VALUE 0 WHEN THE GAMMA FUNCTION CANNOT */
/*     BE COMPUTED. */

/* ----------------------------------------------------------------------- */
/*     WRITTEN BY ALFRED H. MORRIS, JR. */
/*          NAVAL SURFACE WEAPONS CENTER */
/*          DAHLGREN, VIRGINIA */
/* ----------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Data statements .. */
/* -------------------------- */
/*     D = 0.5*(LN(2*PI) - 1) */
/* -------------------------- */
/* -------------------------- */
/* -------------------------- */
/*     .. */
/*     .. Executable Statements .. */
/* -------------------------- */
    ret_val = 0.;
    x = *a;
    if (abs(*a) >= 15.) {
	goto L110;
    }
/* ----------------------------------------------------------------------- */
/*            EVALUATION OF GAMMA(A) FOR ABS(A) .LT. 15 */
/* ----------------------------------------------------------------------- */
    t = 1.;
    m = (integer) (*a) - 1;

/*     LET T BE THE PRODUCT OF A-J WHEN A .GE. 2 */

    if (m < 0) {
	goto L40;
    } else if (m == 0) {
	goto L30;
    } else {
	goto L10;
    }
L10:
    i__1 = m;
    for (j = 1; j <= i__1; ++j) {
	x += -1.;
	t = x * t;
/* L20: */
    }
L30:
    x += -1.;
    goto L80;

/*     LET T BE THE PRODUCT OF A+J WHEN A .LT. 1 */

L40:
    t = *a;
    if (*a > 0.) {
	goto L70;
    }
    m = -m - 1;
    if (m == 0) {
	goto L60;
    }
    i__1 = m;
    for (j = 1; j <= i__1; ++j) {
	x += 1.;
	t = x * t;
/* L50: */
    }
L60:
    x = x + .5 + .5;
    t = x * t;
    if (t == 0.) {
	return ret_val;
    }

L70:

/*     THE FOLLOWING CODE CHECKS IF 1/T CAN OVERFLOW. THIS */
/*     CODE MAY BE OMITTED IF DESIRED. */

    if (abs(t) >= 1e-30) {
	goto L80;
    }
    if (abs(t) * spmpar_(&c__3) <= 1.0001) {
	return ret_val;
    }
    ret_val = 1. / t;
    return ret_val;

/*     COMPUTE GAMMA(1 + X) FOR  0 .LE. X .LT. 1 */

L80:
    top = p[0];
    bot = q[0];
    for (i__ = 2; i__ <= 7; ++i__) {
	top = p[i__ - 1] + x * top;
	bot = q[i__ - 1] + x * bot;
/* L90: */
    }
    ret_val = top / bot;

/*     TERMINATION */

    if (*a < 1.) {
	goto L100;
    }
    ret_val *= t;
    return ret_val;
L100:
    ret_val /= t;
    return ret_val;
/* ----------------------------------------------------------------------- */
/*            EVALUATION OF GAMMA(A) FOR ABS(A) .GE. 15 */
/* ----------------------------------------------------------------------- */
L110:
    if (abs(*a) >= 1e3) {
	return ret_val;
    }
    if (*a > 0.) {
	goto L120;
    }
    x = -(*a);
    n = (integer) x;
    t = x - n;
    if (t > .9) {
	t = 1. - t;
    }
    s = sin(pi * t) / pi;
    if (n % 2 == 0) {
	s = -s;
    }
    if (s == 0.) {
	return ret_val;
    }

/*     COMPUTE THE MODIFIED ASYMPTOTIC SUM */

L120:
    t = 1. / (x * x);
    g = ((((r1 * t + r2) * t + r3) * t + r4) * t + r5) / x;

/*     ONE MAY REPLACE THE NEXT STATEMENT WITH  LNX = ALOG(X) */
/*     BUT LESS ACCURACY WILL NORMALLY BE OBTAINED. */

    lnx = log(x);

/*     FINAL ASSEMBLY */

    z__ = x;
    g = d__ + g + (z__ - .5) * (lnx - 1.);
    w = g;
    t = g - w;
    if (w > exparg_(&c__0) * .99999) {
	return ret_val;
    }
    ret_val = exp(w) * (t + 1.);
    if (*a < 0.) {
	ret_val = 1. / (ret_val * s) / x;
    }
    return ret_val;
} /* gamma_ */

/* Subroutine */ int gratio_(doublereal *a, doublereal *x, doublereal *ans, 
	doublereal *qans, integer *ind)
{
    /* Initialized data */

    static doublereal acc0[3] = { 5e-15,5e-7,5e-4 };
    static doublereal big[3] = { 20.,14.,10. };
    static doublereal e00[3] = { 2.5e-4,.025,.14 };
    static doublereal x00[3] = { 31.,17.,9.7 };
    static doublereal alog10 = 2.30258509299405;
    static doublereal rt2pin = .398942280401433;
    static doublereal rtpi = 1.77245385090552;
    static doublereal third = .333333333333333;
    static doublereal d0[13] = { .0833333333333333,-.0148148148148148,
	    .00115740740740741,3.52733686067019e-4,-1.78755144032922e-4,
	    3.91926317852244e-5,-2.18544851067999e-6,-1.85406221071516e-6,
	    8.29671134095309e-7,-1.76659527368261e-7,6.7078535434015e-9,
	    1.02618097842403e-8,-4.38203601845335e-9 };
    static doublereal d10 = -.00185185185185185;
    static doublereal d1[12] = { -.00347222222222222,.00264550264550265,
	    -9.9022633744856e-4,2.05761316872428e-4,-4.01877572016461e-7,
	    -1.809855033449e-5,7.64916091608111e-6,-1.61209008945634e-6,
	    4.64712780280743e-9,1.37863344691572e-7,-5.7525456035177e-8,
	    1.19516285997781e-8 };
    static doublereal d20 = .00413359788359788;
    static doublereal d2[10] = { -.00268132716049383,7.71604938271605e-4,
	    2.0093878600823e-6,-1.07366532263652e-4,5.29234488291201e-5,
	    -1.27606351886187e-5,3.42357873409614e-8,1.37219573090629e-6,
	    -6.29899213838006e-7,1.42806142060642e-7 };
    static doublereal d30 = 6.49434156378601e-4;
    static doublereal d3[8] = { 2.29472093621399e-4,-4.69189494395256e-4,
	    2.67720632062839e-4,-7.56180167188398e-5,-2.3965051138673e-7,
	    1.10826541153473e-5,-5.6749528269916e-6,1.42309007324359e-6 };
    static doublereal d40 = -8.61888290916712e-4;
    static doublereal d4[6] = { 7.84039221720067e-4,-2.9907248030319e-4,
	    -1.46384525788434e-6,6.64149821546512e-5,-3.96836504717943e-5,
	    1.13757269706784e-5 };
    static doublereal d50 = -3.36798553366358e-4;
    static doublereal d5[4] = { -6.97281375836586e-5,2.77275324495939e-4,
	    -1.99325705161888e-4,6.79778047793721e-5 };
    static doublereal d60 = 5.31307936463992e-4;
    static doublereal d6[2] = { -5.92166437353694e-4,2.70878209671804e-4 };
    static doublereal d70 = 3.44367606892378e-4;

    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double log(doublereal), exp(doublereal), sqrt(doublereal);

    /* Local variables */
    doublereal c__, e, g, h__;
    integer i__;
    doublereal j, l;
    integer m, n;
    doublereal r__, s, t, u, w, y, z__, c0, c1, c2, c3, c4, c5, c6, e0, t1, 
	    x0, an, wk[20], am0, an0, a2n, b2n, acc, cma, amn;
    extern doublereal erf_(doublereal *);
    doublereal apn;
    integer max__;
    doublereal rta;
    integer iop;
    doublereal tol, sum, rtx;
    extern doublereal gam1_(doublereal *);
    doublereal a2nm1, b2nm1;
    extern doublereal rlog_(doublereal *);
    doublereal twoa;
    extern doublereal rexp_(doublereal *), erfc1_(integer *, doublereal *), 
	    gamma_(doublereal *), spmpar_(integer *);

/* ---------------------------------------------------------------------- */
/*        EVALUATION OF THE INCOMPLETE GAMMA RATIO FUNCTIONS */
/*                    P(A,X) AND Q(A,X) */

/*                   ---------- */

/*     IT IS ASSUMED THAT A AND X ARE NONNEGATIVE, WHERE A AND X */
/*     ARE NOT BOTH 0. */

/*     ANS AND QANS ARE VARIABLES. GRATIO ASSIGNS ANS THE VALUE */
/*     P(A,X) AND QANS THE VALUE Q(A,X). IND MAY BE ANY INTEGER. */
/*     IF IND = 0 THEN THE USER IS REQUESTING AS MUCH ACCURACY AS */
/*     POSSIBLE (UP TO 14 SIGNIFICANT DIGITS). OTHERWISE, IF */
/*     IND = 1 THEN ACCURACY IS REQUESTED TO WITHIN 1 UNIT OF THE */
/*     6-TH SIGNIFICANT DIGIT, AND IF IND .NE. 0,1 THEN ACCURACY */
/*     IS REQUESTED TO WITHIN 1 UNIT OF THE 3RD SIGNIFICANT DIGIT. */

/*     ERROR RETURN ... */
/*        ANS IS ASSIGNED THE VALUE 2 WHEN A OR X IS NEGATIVE, */
/*     WHEN A*X = 0, OR WHEN P(A,X) AND Q(A,X) ARE INDETERMINANT. */
/*     P(A,X) AND Q(A,X) ARE COMPUTATIONALLY INDETERMINANT WHEN */
/*     X IS EXCEEDINGLY CLOSE TO A AND A IS EXTREMELY LARGE. */
/* ---------------------------------------------------------------------- */
/*     WRITTEN BY ALFRED H. MORRIS, JR. */
/*        NAVAL SURFACE WEAPONS CENTER */
/*        DAHLGREN, VIRGINIA */
/*     -------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Data statements .. */
/*     -------------------- */
/*     -------------------- */
/*     ALOG10 = LN(10) */
/*     RT2PIN = 1/SQRT(2*PI) */
/*     RTPI   = SQRT(PI) */
/*     -------------------- */
/*     -------------------- */
/*     -------------------- */
/*     -------------------- */
/*     -------------------- */
/*     -------------------- */
/*     -------------------- */
/*     -------------------- */
/*     -------------------- */
/*     .. */
/*     .. Executable Statements .. */
/*     -------------------- */
/*     ****** E IS A MACHINE DEPENDENT CONSTANT. E IS THE SMALLEST */
/*            FLOATING POINT NUMBER FOR WHICH 1.0 + E .GT. 1.0 . */

    e = spmpar_(&c__1);

/*     -------------------- */
    if (*a < 0. || *x < 0.) {
	goto L430;
    }
    if (*a == 0. && *x == 0.) {
	goto L430;
    }
    if (*a * *x == 0.) {
	goto L420;
    }

    iop = *ind + 1;
    if (iop != 1 && iop != 2) {
	iop = 3;
    }
/* Computing MAX */
    d__1 = acc0[iop - 1];
    acc = max(d__1,e);
    e0 = e00[iop - 1];
    x0 = x00[iop - 1];

/*            SELECT THE APPROPRIATE ALGORITHM */

    if (*a >= 1.) {
	goto L10;
    }
    if (*a == .5) {
	goto L390;
    }
    if (*x < 1.1) {
	goto L160;
    }
    t1 = *a * log(*x) - *x;
    u = *a * exp(t1);
    if (u == 0.) {
	goto L380;
    }
    r__ = u * (gam1_(a) + 1.);
    goto L250;

L10:
    if (*a >= big[iop - 1]) {
	goto L30;
    }
    if (*a > *x || *x >= x0) {
	goto L20;
    }
    twoa = *a + *a;
    m = (integer) twoa;
    if (twoa != (doublereal) m) {
	goto L20;
    }
    i__ = m / 2;
    if (*a == (doublereal) i__) {
	goto L210;
    }
    goto L220;
L20:
    t1 = *a * log(*x) - *x;
    r__ = exp(t1) / gamma_(a);
    goto L40;

L30:
    l = *x / *a;
    if (l == 0.) {
	goto L370;
    }
    s = .5 - l + .5;
    z__ = rlog_(&l);
    if (z__ >= 700. / *a) {
	goto L410;
    }
    y = *a * z__;
    rta = sqrt(*a);
    if (abs(s) <= e0 / rta) {
	goto L330;
    }
    if (abs(s) <= .4) {
	goto L270;
    }

/* Computing 2nd power */
    d__1 = 1. / *a;
    t = d__1 * d__1;
    t1 = (((t * .75 - 1.) * t + 3.5) * t - 105.) / (*a * 1260.);
    t1 -= y;
    r__ = rt2pin * rta * exp(t1);

L40:
    if (r__ == 0.) {
	goto L420;
    }
    if (*x <= max(*a,alog10)) {
	goto L50;
    }
    if (*x < x0) {
	goto L250;
    }
    goto L100;

/*               TAYLOR SERIES FOR P/R */

L50:
    apn = *a + 1.;
    t = *x / apn;
    wk[0] = t;
    for (n = 2; n <= 20; ++n) {
	apn += 1.;
	t *= *x / apn;
	if (t <= .001) {
	    goto L70;
	}
	wk[n - 1] = t;
/* L60: */
    }
    n = 20;

L70:
    sum = t;
    tol = acc * .5;
L80:
    apn += 1.;
    t *= *x / apn;
    sum += t;
    if (t > tol) {
	goto L80;
    }

    max__ = n - 1;
    i__1 = max__;
    for (m = 1; m <= i__1; ++m) {
	--n;
	sum += wk[n - 1];
/* L90: */
    }
    *ans = r__ / *a * (sum + 1.);
    *qans = .5 - *ans + .5;
    return 0;

/*               ASYMPTOTIC EXPANSION */

L100:
    amn = *a - 1.;
    t = amn / *x;
    wk[0] = t;
    for (n = 2; n <= 20; ++n) {
	amn += -1.;
	t *= amn / *x;
	if (abs(t) <= .001) {
	    goto L120;
	}
	wk[n - 1] = t;
/* L110: */
    }
    n = 20;

L120:
    sum = t;
L130:
    if (abs(t) <= acc) {
	goto L140;
    }
    amn += -1.;
    t *= amn / *x;
    sum += t;
    goto L130;

L140:
    max__ = n - 1;
    i__1 = max__;
    for (m = 1; m <= i__1; ++m) {
	--n;
	sum += wk[n - 1];
/* L150: */
    }
    *qans = r__ / *x * (sum + 1.);
    *ans = .5 - *qans + .5;
    return 0;

/*             TAYLOR SERIES FOR P(A,X)/X**A */

L160:
    an = 3.;
    c__ = *x;
    sum = *x / (*a + 3.);
    tol = acc * 3. / (*a + 1.);
L170:
    an += 1.;
    c__ = -c__ * (*x / an);
    t = c__ / (*a + an);
    sum += t;
    if (abs(t) > tol) {
	goto L170;
    }
    j = *a * *x * ((sum / 6. - .5 / (*a + 2.)) * *x + 1. / (*a + 1.));

    z__ = *a * log(*x);
    h__ = gam1_(a);
    g = h__ + 1.;
    if (*x < .25) {
	goto L180;
    }
    if (*a < *x / 2.59) {
	goto L200;
    }
    goto L190;
L180:
    if (z__ > -.13394) {
	goto L200;
    }

L190:
    w = exp(z__);
    *ans = w * g * (.5 - j + .5);
    *qans = .5 - *ans + .5;
    return 0;

L200:
    l = rexp_(&z__);
    w = l + .5 + .5;
    *qans = (w * j - l) * g - h__;
    if (*qans < 0.) {
	goto L380;
    }
    *ans = .5 - *qans + .5;
    return 0;

/*             FINITE SUMS FOR Q WHEN A .GE. 1 */
/*               AND 2*A IS AN INTEGER */

L210:
    sum = exp(-(*x));
    t = sum;
    n = 1;
    c__ = 0.;
    goto L230;

L220:
    rtx = sqrt(*x);
    sum = erfc1_(&c__0, &rtx);
    t = exp(-(*x)) / (rtpi * rtx);
    n = 0;
    c__ = -.5;

L230:
    if (n == i__) {
	goto L240;
    }
    ++n;
    c__ += 1.;
    t = *x * t / c__;
    sum += t;
    goto L230;
L240:
    *qans = sum;
    *ans = .5 - *qans + .5;
    return 0;

/*              CONTINUED FRACTION EXPANSION */

L250:
/* Computing MAX */
    d__1 = e * 5.;
    tol = max(d__1,acc);
    a2nm1 = 1.;
    a2n = 1.;
    b2nm1 = *x;
    b2n = *x + (1. - *a);
    c__ = 1.;
L260:
    a2nm1 = *x * a2n + c__ * a2nm1;
    b2nm1 = *x * b2n + c__ * b2nm1;
    am0 = a2nm1 / b2nm1;
    c__ += 1.;
    cma = c__ - *a;
    a2n = a2nm1 + cma * a2n;
    b2n = b2nm1 + cma * b2n;
    an0 = a2n / b2n;
    if ((d__1 = an0 - am0, abs(d__1)) >= tol * an0) {
	goto L260;
    }

    *qans = r__ * an0;
    *ans = .5 - *qans + .5;
    return 0;

/*              GENERAL TEMME EXPANSION */

L270:
    if (abs(s) <= e * 2. && *a * e * e > .00328) {
	goto L430;
    }
    c__ = exp(-y);
    d__1 = sqrt(y);
    w = erfc1_(&c__1, &d__1) * .5;
    u = 1. / *a;
    z__ = sqrt(z__ + z__);
    if (l < 1.) {
	z__ = -z__;
    }
    if ((i__1 = iop - 2) < 0) {
	goto L280;
    } else if (i__1 == 0) {
	goto L290;
    } else {
	goto L300;
    }

L280:
    if (abs(s) <= .001) {
	goto L340;
    }
    c0 = ((((((((((((d0[12] * z__ + d0[11]) * z__ + d0[10]) * z__ + d0[9]) * 
	    z__ + d0[8]) * z__ + d0[7]) * z__ + d0[6]) * z__ + d0[5]) * z__ + 
	    d0[4]) * z__ + d0[3]) * z__ + d0[2]) * z__ + d0[1]) * z__ + d0[0])
	     * z__ - third;
    c1 = (((((((((((d1[11] * z__ + d1[10]) * z__ + d1[9]) * z__ + d1[8]) * 
	    z__ + d1[7]) * z__ + d1[6]) * z__ + d1[5]) * z__ + d1[4]) * z__ + 
	    d1[3]) * z__ + d1[2]) * z__ + d1[1]) * z__ + d1[0]) * z__ + d10;
    c2 = (((((((((d2[9] * z__ + d2[8]) * z__ + d2[7]) * z__ + d2[6]) * z__ + 
	    d2[5]) * z__ + d2[4]) * z__ + d2[3]) * z__ + d2[2]) * z__ + d2[1])
	     * z__ + d2[0]) * z__ + d20;
    c3 = (((((((d3[7] * z__ + d3[6]) * z__ + d3[5]) * z__ + d3[4]) * z__ + d3[
	    3]) * z__ + d3[2]) * z__ + d3[1]) * z__ + d3[0]) * z__ + d30;
    c4 = (((((d4[5] * z__ + d4[4]) * z__ + d4[3]) * z__ + d4[2]) * z__ + d4[1]
	    ) * z__ + d4[0]) * z__ + d40;
    c5 = (((d5[3] * z__ + d5[2]) * z__ + d5[1]) * z__ + d5[0]) * z__ + d50;
    c6 = (d6[1] * z__ + d6[0]) * z__ + d60;
    t = ((((((d70 * u + c6) * u + c5) * u + c4) * u + c3) * u + c2) * u + c1) 
	    * u + c0;
    goto L310;

L290:
    c0 = (((((d0[5] * z__ + d0[4]) * z__ + d0[3]) * z__ + d0[2]) * z__ + d0[1]
	    ) * z__ + d0[0]) * z__ - third;
    c1 = (((d1[3] * z__ + d1[2]) * z__ + d1[1]) * z__ + d1[0]) * z__ + d10;
    c2 = d2[0] * z__ + d20;
    t = (c2 * u + c1) * u + c0;
    goto L310;

L300:
    t = ((d0[2] * z__ + d0[1]) * z__ + d0[0]) * z__ - third;

L310:
    if (l < 1.) {
	goto L320;
    }
    *qans = c__ * (w + rt2pin * t / rta);
    *ans = .5 - *qans + .5;
    return 0;
L320:
    *ans = c__ * (w - rt2pin * t / rta);
    *qans = .5 - *ans + .5;
    return 0;

/*             TEMME EXPANSION FOR L = 1 */

L330:
    if (*a * e * e > .00328) {
	goto L430;
    }
    c__ = .5 - y + .5;
    w = (.5 - sqrt(y) * (.5 - y / 3. + .5) / rtpi) / c__;
    u = 1. / *a;
    z__ = sqrt(z__ + z__);
    if (l < 1.) {
	z__ = -z__;
    }
    if ((i__1 = iop - 2) < 0) {
	goto L340;
    } else if (i__1 == 0) {
	goto L350;
    } else {
	goto L360;
    }

L340:
    c0 = ((((((d0[6] * z__ + d0[5]) * z__ + d0[4]) * z__ + d0[3]) * z__ + d0[
	    2]) * z__ + d0[1]) * z__ + d0[0]) * z__ - third;
    c1 = (((((d1[5] * z__ + d1[4]) * z__ + d1[3]) * z__ + d1[2]) * z__ + d1[1]
	    ) * z__ + d1[0]) * z__ + d10;
    c2 = ((((d2[4] * z__ + d2[3]) * z__ + d2[2]) * z__ + d2[1]) * z__ + d2[0])
	     * z__ + d20;
    c3 = (((d3[3] * z__ + d3[2]) * z__ + d3[1]) * z__ + d3[0]) * z__ + d30;
    c4 = (d4[1] * z__ + d4[0]) * z__ + d40;
    c5 = (d5[1] * z__ + d5[0]) * z__ + d50;
    c6 = d6[0] * z__ + d60;
    t = ((((((d70 * u + c6) * u + c5) * u + c4) * u + c3) * u + c2) * u + c1) 
	    * u + c0;
    goto L310;

L350:
    c0 = (d0[1] * z__ + d0[0]) * z__ - third;
    c1 = d1[0] * z__ + d10;
    t = (d20 * u + c1) * u + c0;
    goto L310;

L360:
    t = d0[0] * z__ - third;
    goto L310;

/*                   SPECIAL CASES */

L370:
    *ans = 0.;
    *qans = 1.;
    return 0;

L380:
    *ans = 1.;
    *qans = 0.;
    return 0;

L390:
    if (*x >= .25) {
	goto L400;
    }
    d__1 = sqrt(*x);
    *ans = erf_(&d__1);
    *qans = .5 - *ans + .5;
    return 0;
L400:
    d__1 = sqrt(*x);
    *qans = erfc1_(&c__0, &d__1);
    *ans = .5 - *qans + .5;
    return 0;

L410:
    if (abs(s) <= e * 2.) {
	goto L430;
    }
L420:
    if (*x <= *a) {
	goto L370;
    }
    goto L380;

/*                   ERROR RETURN */

L430:
    *ans = 2.;
    return 0;
} /* gratio_ */

integer ipmpar_(integer *i__)
{
    /* Initialized data */

    static integer imach[10] = { 2,31,2147483647,2,24,-125,128,53,-1021,1024 }
	    ;

    /* System generated locals */
    integer ret_val;

/* ----------------------------------------------------------------------- */

/*     IPMPAR PROVIDES THE INTEGER MACHINE CONSTANTS FOR THE COMPUTER */
/*     THAT IS USED. IT IS ASSUMED THAT THE ARGUMENT I IS AN INTEGER */
/*     HAVING ONE OF THE VALUES 1-10. IPMPAR(I) HAS THE VALUE ... */

/*  INTEGERS. */

/*     ASSUME INTEGERS ARE REPRESENTED IN THE N-DIGIT, BASE-A FORM */

/*             SIGN ( X(N-1)*A**(N-1) + ... + X(1)*A + X(0) ) */

/*             WHERE 0 .LE. X(I) .LT. A FOR I=0,...,N-1. */

/*     IPMPAR(1) = A, THE BASE. */

/*     IPMPAR(2) = N, THE NUMBER OF BASE-A DIGITS. */

/*     IPMPAR(3) = A**N - 1, THE LARGEST MAGNITUDE. */

/*  FLOATING-POINT NUMBERS. */

/*     IT IS ASSUMED THAT THE SINGLE AND DOUBLE PRECISION FLOATING */
/*     POINT ARITHMETICS HAVE THE SAME BASE, SAY B, AND THAT THE */
/*     NONZERO NUMBERS ARE REPRESENTED IN THE FORM */

/*             SIGN (B**E) * (X(1)/B + ... + X(M)/B**M) */

/*             WHERE X(I) = 0,1,...,B-1 FOR I=1,...,M, */
/*             X(1) .GE. 1, AND EMIN .LE. E .LE. EMAX. */

/*     IPMPAR(4) = B, THE BASE. */

/*  SINGLE-PRECISION */

/*     IPMPAR(5) = M, THE NUMBER OF BASE-B DIGITS. */

/*     IPMPAR(6) = EMIN, THE SMALLEST EXPONENT E. */

/*     IPMPAR(7) = EMAX, THE LARGEST EXPONENT E. */

/*  DOUBLE-PRECISION */

/*     IPMPAR(8) = M, THE NUMBER OF BASE-B DIGITS. */

/*     IPMPAR(9) = EMIN, THE SMALLEST EXPONENT E. */

/*     IPMPAR(10) = EMAX, THE LARGEST EXPONENT E. */

/* ----------------------------------------------------------------------- */

/*     TO DEFINE THIS FUNCTION FOR THE COMPUTER BEING USED, ACTIVATE */
/*     THE DATA STATMENTS FOR THE COMPUTER BY REMOVING THE C FROM */
/*     COLUMN 1. (ALL THE OTHER DATA STATEMENTS SHOULD HAVE C IN */
/*     COLUMN 1.) */

/* ----------------------------------------------------------------------- */

/*     IPMPAR IS AN ADAPTATION OF THE FUNCTION I1MACH, WRITTEN BY */
/*     P.A. FOX, A.D. HALL, AND N.L. SCHRYER (BELL LABORATORIES). */
/*     IPMPAR WAS FORMED BY A.H. MORRIS (NSWC). THE CONSTANTS ARE */
/*     FROM BELL LABORATORIES, NSWC, AND OTHER SOURCES. */

/* ----------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. Data statements .. */

/*     MACHINE CONSTANTS FOR AMDAHL MACHINES. */

/*     DATA IMACH( 1) /   2 / */
/*     DATA IMACH( 2) /  31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /  16 / */
/*     DATA IMACH( 5) /   6 / */
/*     DATA IMACH( 6) / -64 / */
/*     DATA IMACH( 7) /  63 / */
/*     DATA IMACH( 8) /  14 / */
/*     DATA IMACH( 9) / -64 / */
/*     DATA IMACH(10) /  63 / */

/*     MACHINE CONSTANTS FOR THE AT&T 3B SERIES, AT&T */
/*     PC 7300, AND AT&T 6300. */

/*     DATA IMACH( 1) /     2 / */
/*     DATA IMACH( 2) /    31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /     2 / */
/*     DATA IMACH( 5) /    24 / */
/*     DATA IMACH( 6) /  -125 / */
/*     DATA IMACH( 7) /   128 / */
/*     DATA IMACH( 8) /    53 / */
/*     DATA IMACH( 9) / -1021 / */
/*     DATA IMACH(10) /  1024 / */

/*     MACHINE CONSTANTS FOR THE BURROUGHS 1700 SYSTEM. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   33 / */
/*     DATA IMACH( 3) / 8589934591 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   24 / */
/*     DATA IMACH( 6) / -256 / */
/*     DATA IMACH( 7) /  255 / */
/*     DATA IMACH( 8) /   60 / */
/*     DATA IMACH( 9) / -256 / */
/*     DATA IMACH(10) /  255 / */

/*     MACHINE CONSTANTS FOR THE BURROUGHS 5700 SYSTEM. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   39 / */
/*     DATA IMACH( 3) / 549755813887 / */
/*     DATA IMACH( 4) /    8 / */
/*     DATA IMACH( 5) /   13 / */
/*     DATA IMACH( 6) /  -50 / */
/*     DATA IMACH( 7) /   76 / */
/*     DATA IMACH( 8) /   26 / */
/*     DATA IMACH( 9) /  -50 / */
/*     DATA IMACH(10) /   76 / */

/*     MACHINE CONSTANTS FOR THE BURROUGHS 6700/7700 SYSTEMS. */

/*     DATA IMACH( 1) /      2 / */
/*     DATA IMACH( 2) /     39 / */
/*     DATA IMACH( 3) / 549755813887 / */
/*     DATA IMACH( 4) /      8 / */
/*     DATA IMACH( 5) /     13 / */
/*     DATA IMACH( 6) /    -50 / */
/*     DATA IMACH( 7) /     76 / */
/*     DATA IMACH( 8) /     26 / */
/*     DATA IMACH( 9) / -32754 / */
/*     DATA IMACH(10) /  32780 / */

/*     MACHINE CONSTANTS FOR THE CDC 6000/7000 SERIES */
/*     60 BIT ARITHMETIC, AND THE CDC CYBER 995 64 BIT */
/*     ARITHMETIC (NOS OPERATING SYSTEM). */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   48 / */
/*     DATA IMACH( 3) / 281474976710655 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   48 / */
/*     DATA IMACH( 6) / -974 / */
/*     DATA IMACH( 7) / 1070 / */
/*     DATA IMACH( 8) /   95 / */
/*     DATA IMACH( 9) / -926 / */
/*     DATA IMACH(10) / 1070 / */

/*     MACHINE CONSTANTS FOR THE CDC CYBER 995 64 BIT */
/*     ARITHMETIC (NOS/VE OPERATING SYSTEM). */

/*     DATA IMACH( 1) /     2 / */
/*     DATA IMACH( 2) /    63 / */
/*     DATA IMACH( 3) / 9223372036854775807 / */
/*     DATA IMACH( 4) /     2 / */
/*     DATA IMACH( 5) /    48 / */
/*     DATA IMACH( 6) / -4096 / */
/*     DATA IMACH( 7) /  4095 / */
/*     DATA IMACH( 8) /    96 / */
/*     DATA IMACH( 9) / -4096 / */
/*     DATA IMACH(10) /  4095 / */

/*     MACHINE CONSTANTS FOR THE CRAY 1, XMP, 2, AND 3. */

/*     DATA IMACH( 1) /     2 / */
/*     DATA IMACH( 2) /    63 / */
/*     DATA IMACH( 3) / 9223372036854775807 / */
/*     DATA IMACH( 4) /     2 / */
/*     DATA IMACH( 5) /    47 / */
/*     DATA IMACH( 6) / -8189 / */
/*     DATA IMACH( 7) /  8190 / */
/*     DATA IMACH( 8) /    94 / */
/*     DATA IMACH( 9) / -8099 / */
/*     DATA IMACH(10) /  8190 / */

/*     MACHINE CONSTANTS FOR THE DATA GENERAL ECLIPSE S/200. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   15 / */
/*     DATA IMACH( 3) / 32767 / */
/*     DATA IMACH( 4) /   16 / */
/*     DATA IMACH( 5) /    6 / */
/*     DATA IMACH( 6) /  -64 / */
/*     DATA IMACH( 7) /   63 / */
/*     DATA IMACH( 8) /   14 / */
/*     DATA IMACH( 9) /  -64 / */
/*     DATA IMACH(10) /   63 / */

/*     MACHINE CONSTANTS FOR THE HARRIS 220. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   23 / */
/*     DATA IMACH( 3) / 8388607 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   23 / */
/*     DATA IMACH( 6) / -127 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   38 / */
/*     DATA IMACH( 9) / -127 / */
/*     DATA IMACH(10) /  127 / */

/*     MACHINE CONSTANTS FOR THE HONEYWELL 600/6000 */
/*     AND DPS 8/70 SERIES. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   35 / */
/*     DATA IMACH( 3) / 34359738367 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   27 / */
/*     DATA IMACH( 6) / -127 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   63 / */
/*     DATA IMACH( 9) / -127 / */
/*     DATA IMACH(10) /  127 / */

/*     MACHINE CONSTANTS FOR THE HP 2100 */
/*     3 WORD DOUBLE PRECISION OPTION WITH FTN4 */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   15 / */
/*     DATA IMACH( 3) / 32767 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   23 / */
/*     DATA IMACH( 6) / -128 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   39 / */
/*     DATA IMACH( 9) / -128 / */
/*     DATA IMACH(10) /  127 / */

/*     MACHINE CONSTANTS FOR THE HP 2100 */
/*     4 WORD DOUBLE PRECISION OPTION WITH FTN4 */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   15 / */
/*     DATA IMACH( 3) / 32767 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   23 / */
/*     DATA IMACH( 6) / -128 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   55 / */
/*     DATA IMACH( 9) / -128 / */
/*     DATA IMACH(10) /  127 / */

/*     MACHINE CONSTANTS FOR THE HP 9000. */

/*     DATA IMACH( 1) /     2 / */
/*     DATA IMACH( 2) /    31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /     2 / */
/*     DATA IMACH( 5) /    24 / */
/*     DATA IMACH( 6) /  -126 / */
/*     DATA IMACH( 7) /   128 / */
/*     DATA IMACH( 8) /    53 / */
/*     DATA IMACH( 9) / -1021 / */
/*     DATA IMACH(10) /  1024 / */

/*     MACHINE CONSTANTS FOR THE IBM 360/370 SERIES, */
/*     THE ICL 2900, THE ITEL AS/6, THE XEROX SIGMA */
/*     5/7/9 AND THE SEL SYSTEMS 85/86. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /   16 / */
/*     DATA IMACH( 5) /    6 / */
/*     DATA IMACH( 6) /  -64 / */
/*     DATA IMACH( 7) /   63 / */
/*     DATA IMACH( 8) /   14 / */
/*     DATA IMACH( 9) /  -64 / */
/*     DATA IMACH(10) /   63 / */

/*     MACHINE CONSTANTS FOR THE IBM PC. */

/*      DATA imach(1)/2/ */
/*      DATA imach(2)/31/ */
/*      DATA imach(3)/2147483647/ */
/*      DATA imach(4)/2/ */
/*      DATA imach(5)/24/ */
/*      DATA imach(6)/-125/ */
/*      DATA imach(7)/128/ */
/*      DATA imach(8)/53/ */
/*      DATA imach(9)/-1021/ */
/*      DATA imach(10)/1024/ */

/*     MACHINE CONSTANTS FOR THE MACINTOSH II - ABSOFT */
/*     MACFORTRAN II. */

/*     DATA IMACH( 1) /     2 / */
/*     DATA IMACH( 2) /    31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /     2 / */
/*     DATA IMACH( 5) /    24 / */
/*     DATA IMACH( 6) /  -125 / */
/*     DATA IMACH( 7) /   128 / */
/*     DATA IMACH( 8) /    53 / */
/*     DATA IMACH( 9) / -1021 / */
/*     DATA IMACH(10) /  1024 / */

/*     MACHINE CONSTANTS FOR THE MICROVAX - VMS FORTRAN. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   24 / */
/*     DATA IMACH( 6) / -127 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   56 / */
/*     DATA IMACH( 9) / -127 / */
/*     DATA IMACH(10) /  127 / */

/*     MACHINE CONSTANTS FOR THE PDP-10 (KA PROCESSOR). */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   35 / */
/*     DATA IMACH( 3) / 34359738367 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   27 / */
/*     DATA IMACH( 6) / -128 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   54 / */
/*     DATA IMACH( 9) / -101 / */
/*     DATA IMACH(10) /  127 / */

/*     MACHINE CONSTANTS FOR THE PDP-10 (KI PROCESSOR). */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   35 / */
/*     DATA IMACH( 3) / 34359738367 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   27 / */
/*     DATA IMACH( 6) / -128 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   62 / */
/*     DATA IMACH( 9) / -128 / */
/*     DATA IMACH(10) /  127 / */

/*     MACHINE CONSTANTS FOR THE PDP-11 FORTRAN SUPPORTING */
/*     32-BIT INTEGER ARITHMETIC. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   24 / */
/*     DATA IMACH( 6) / -127 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   56 / */
/*     DATA IMACH( 9) / -127 / */
/*     DATA IMACH(10) /  127 / */

/*     MACHINE CONSTANTS FOR THE SEQUENT BALANCE 8000. */

/*     DATA IMACH( 1) /     2 / */
/*     DATA IMACH( 2) /    31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /     2 / */
/*     DATA IMACH( 5) /    24 / */
/*     DATA IMACH( 6) /  -125 / */
/*     DATA IMACH( 7) /   128 / */
/*     DATA IMACH( 8) /    53 / */
/*     DATA IMACH( 9) / -1021 / */
/*     DATA IMACH(10) /  1024 / */

/*     MACHINE CONSTANTS FOR THE SILICON GRAPHICS IRIS-4D */
/*     SERIES (MIPS R3000 PROCESSOR). */

/*     DATA IMACH( 1) /     2 / */
/*     DATA IMACH( 2) /    31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /     2 / */
/*     DATA IMACH( 5) /    24 / */
/*     DATA IMACH( 6) /  -125 / */
/*     DATA IMACH( 7) /   128 / */
/*     DATA IMACH( 8) /    53 / */
/*     DATA IMACH( 9) / -1021 / */
/*     DATA IMACH(10) /  1024 / */

/*     MACHINE CONSTANTS FOR IEEE ARITHMETIC MACHINES, SUCH AS THE AT&T */
/*     3B SERIES, MOTOROLA 68000 BASED MACHINES (E.G. SUN 3 AND AT&T */
/*     PC 7300), AND 8087 BASED MICROS (E.G. IBM PC AND AT&T 6300). */


/*     MACHINE CONSTANTS FOR THE UNIVAC 1100 SERIES. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   35 / */
/*     DATA IMACH( 3) / 34359738367 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   27 / */
/*     DATA IMACH( 6) / -128 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   60 / */
/*     DATA IMACH( 9) /-1024 / */
/*     DATA IMACH(10) / 1023 / */

/*     MACHINE CONSTANTS FOR THE VAX 11/780. */

/*     DATA IMACH( 1) /    2 / */
/*     DATA IMACH( 2) /   31 / */
/*     DATA IMACH( 3) / 2147483647 / */
/*     DATA IMACH( 4) /    2 / */
/*     DATA IMACH( 5) /   24 / */
/*     DATA IMACH( 6) / -127 / */
/*     DATA IMACH( 7) /  127 / */
/*     DATA IMACH( 8) /   56 / */
/*     DATA IMACH( 9) / -127 / */
/*     DATA IMACH(10) /  127 / */

    ret_val = imach[(0 + (0 + (*i__ - 1 << 2))) / 4];
    return ret_val;
} /* ipmpar_ */

doublereal rexp_(doublereal *x)
{
    /* Initialized data */

    static doublereal p1 = 9.14041914819518e-10;
    static doublereal p2 = .0238082361044469;
    static doublereal q1 = -.499999999085958;
    static doublereal q2 = .107141568980644;
    static doublereal q3 = -.0119041179760821;
    static doublereal q4 = 5.95130811860248e-4;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    doublereal w;

/* ----------------------------------------------------------------------- */
/*            EVALUATION OF THE FUNCTION EXP(X) - 1 */
/* ----------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Data statements .. */
/*     .. */
/*     .. Executable Statements .. */
/* ----------------------- */
    if (abs(*x) > .15) {
	goto L10;
    }
    ret_val = *x * (((p2 * *x + p1) * *x + 1.) / ((((q4 * *x + q3) * *x + q2) 
	    * *x + q1) * *x + 1.));
    return ret_val;

L10:
    w = exp(*x);
    if (*x > 0.) {
	goto L20;
    }
    ret_val = w - .5 - .5;
    return ret_val;
L20:
    ret_val = w * (.5 - 1. / w + .5);
    return ret_val;
} /* rexp_ */

doublereal rlog_(doublereal *x)
{
    /* Initialized data */

    static doublereal a = .0566749439387324;
    static doublereal b = .0456512608815524;
    static doublereal p0 = .333333333333333;
    static doublereal p1 = -.224696413112536;
    static doublereal p2 = .00620886815375787;
    static doublereal q1 = -1.27408923933623;
    static doublereal q2 = .354508718369557;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    doublereal r__, t, u, w, w1;

/*     ------------------- */
/*     COMPUTATION OF  X - 1 - LN(X) */
/*     ------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Data statements .. */
/*     ------------------- */
/*     .. */
/*     .. Executable Statements .. */
/*     ------------------- */
    if (*x < .61 || *x > 1.57) {
	goto L40;
    }
    if (*x < .82) {
	goto L10;
    }
    if (*x > 1.18) {
	goto L20;
    }

/*              ARGUMENT REDUCTION */

    u = *x - .5 - .5;
    w1 = 0.;
    goto L30;

L10:
    u = *x - .7;
    u /= .7;
    w1 = a - u * .3;
    goto L30;

L20:
    u = *x * .75 - 1.;
    w1 = b + u / 3.;

/*             SERIES EXPANSION */

L30:
    r__ = u / (u + 2.);
    t = r__ * r__;
    w = ((p2 * t + p1) * t + p0) / ((q2 * t + q1) * t + 1.);
    ret_val = t * 2. * (1. / (1. - r__) - r__ * w) + w1;
    return ret_val;


L40:
    r__ = *x - .5 - .5;
    ret_val = r__ - log(*x);
    return ret_val;
} /* rlog_ */

doublereal spmpar_(integer *i__)
{
    /* System generated locals */
    integer i__1;
    doublereal ret_val;

    /* Builtin functions */
    double pow_di(doublereal *, integer *);

    /* Local variables */
    doublereal b;
    integer m;
    doublereal w, z__, bm1, one;
    integer emin, emax;
    doublereal binv;
    integer ibeta;
    extern integer ipmpar_(integer *);

/* ----------------------------------------------------------------------- */

/*     SPMPAR PROVIDES THE SINGLE PRECISION MACHINE CONSTANTS FOR */
/*     THE COMPUTER BEING USED. IT IS ASSUMED THAT THE ARGUMENT */
/*     I IS AN INTEGER HAVING ONE OF THE VALUES 1, 2, OR 3. IF THE */
/*     SINGLE PRECISION ARITHMETIC BEING USED HAS M BASE B DIGITS AND */
/*     ITS SMALLEST AND LARGEST EXPONENTS ARE EMIN AND EMAX, THEN */

/*        SPMPAR(1) = B**(1 - M), THE MACHINE PRECISION, */

/*        SPMPAR(2) = B**(EMIN - 1), THE SMALLEST MAGNITUDE, */

/*        SPMPAR(3) = B**EMAX*(1 - B**(-M)), THE LARGEST MAGNITUDE. */

/* ----------------------------------------------------------------------- */
/*     WRITTEN BY */
/*        ALFRED H. MORRIS, JR. */
/*        NAVAL SURFACE WARFARE CENTER */
/*        DAHLGREN VIRGINIA */
/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */
/*     MODIFIED BY BARRY W. BROWN TO RETURN DOUBLE PRECISION MACHINE */
/*     CONSTANTS FOR THE COMPUTER BEING USED.  THIS MODIFICATION WAS */
/*     MADE AS PART OF CONVERTING BRATIO TO DOUBLE PRECISION */
/* ----------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    if (*i__ > 1) {
	goto L10;
    }
    b = (doublereal) ipmpar_(&c__4);
    m = ipmpar_(&c__8);
    i__1 = 1 - m;
    ret_val = pow_di(&b, &i__1);
    return ret_val;

L10:
    if (*i__ > 2) {
	goto L20;
    }
    b = (doublereal) ipmpar_(&c__4);
    emin = ipmpar_(&c__9);
    one = 1.;
    binv = one / b;
    i__1 = emin + 2;
    w = pow_di(&b, &i__1);
    ret_val = w * binv * binv * binv;
    return ret_val;

L20:
    ibeta = ipmpar_(&c__4);
    m = ipmpar_(&c__8);
    emax = ipmpar_(&c__10);

    b = (doublereal) ibeta;
    bm1 = (doublereal) (ibeta - 1);
    one = 1.;
    i__1 = m - 1;
    z__ = pow_di(&b, &i__1);
    w = ((z__ - one) * b + bm1) / (b * z__);

    i__1 = emax - 2;
    z__ = pow_di(&b, &i__1);
    ret_val = w * z__ * b * b;
    return ret_val;
} /* spmpar_ */

#ifdef __cplusplus
	}
#endif
