/* dqainf.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Subroutine */ int dqainf_(integer *numfun, U_fp funsub, doublereal *period,
	 doublereal *gamma, doublereal *a, doublereal *b, integer *wrksub, 
	integer *emax, integer *minpts, integer *maxpts, doublereal *epsabs, 
	doublereal *epsrel, integer *nw, integer *key, integer *restar, 
	doublereal *result, doublereal *abserr, integer *neval, integer *
	ifail, logical *done, doublereal *work, integer *iwork)
{
    integer lenw, nsub, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, 
	    i13, i14;
    extern /* Subroutine */ int dadinf_(integer *, U_fp, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, integer *, 
	    integer *, integer *, logical *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, integer *), dchinf_(integer *, U_fp, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *);

/* ***BEGIN PROLOGUE DQAINF */
/* ***DATE WRITTEN   930515   (YYMMDD) */
/* ***REVISION DATE  940310   (YYMMDD) */
/* ***CATEGORY NO. H2B2A1/H2B2A2 */
/* ***AUTHORS */
/*            Terje O. Espelid, Department of Informatics, */
/*            University of Bergen,  Hoyteknologisenteret. */
/*            N-5020 Bergen, Norway */
/*            Email..  terje@ii.uib.no */

/*            Kjell J. Overholt, Department of Informatics, */
/*            University of Bergen,  Hoyteknologisenteret. */
/*            N-5020 Bergen, Norway */
/*            Email..  kjell@ii.uib.no */

/* ***PURPOSE  To compute several one-dimensional integrals over */
/*            an infinite region. All of the integrands are assumed to be */
/*            oscillatory, with the same asymptotic periodic behavior. */
/*            This routine is the driving routine for the integration */
/*            routine DADINF. */

/* ***KEYWORDS Quadrature, one dimensional, oscillatory integrands, */
/*            infinite region, extrapolation, globally adaptive, */
/*            periodic functions. */

/* ***DESCRIPTION */

/*   The routine calculates an approximation to a given vector of */
/*   infinite integrals */

/*            I (F ,F ,...,F      ) DX, */
/*                1  2      NUMFUN */

/*   where the interval of integration is [A,infinity), where A is */
/*   supplied by the user, and with */
/*   B as a possible subdivision point also supplied by the user: */
/*   A <= B. We assume that all functions have the same oscillatory */
/*   behavior for values of X => B. */

/*   Here F = F (X), J = 1,2,...,NUMFUN. */
/*         J   J */

/*   A globally adaptive strategy, combined with extrapolation, */
/*   is applied in order to compute approximations RESULT(J), */
/*   for each component J of I, the following claim for accuracy: */

/*    ABS(I(J)-RESULT(J)).LE.MAX(EPSABS,EPSREL*ABS(I(J))) */

/*   DQAINF is a driver for the integration routine DADINF. */

/*   DADINF either (1) repeatedly subdivides the interval with greatest */
/*   estimated errors; then estimates the integrals and the errors over */
/*   the new sub-intervals and finally updates the extrapolation tableau, */
/*   until the error request is met or MAXPTS function */
/*   evaluations have been used, or (2) decides instead to compute */
/*   a new term: the integral over the next half-period and then */
/*   perform a new extrapolation step. */

/*   Only one basic integration rule (21 points) is offered: Gauss rule. */

/*   Three different extrapolation methods are offered through the */
/*   parameter KEY:: Euler's method, a modification of Eulers method and */
/*   Overholt's P-order 2-method. */

/*   If the values of the input parameters EPSABS or EPSREL are selected */
/*   great enough, the chosen integration rule is applied over the two */
/*   first intervals [A,B] and [B,B+PERIOD/2] for each */
/*   function in the vector and then one extrapolation step is performed */
/*   to give approximations RESULT(J), J = 1, 2, ..., NUMFUN. */
/*   No further subdivisions and no */
/*   more new terms will then be computed. */

/*   When DQAINF computes estimates to a vector of integrals, */
/*   then all components of the vector are given the same treatment. */
/*   That is, I(F ) and I(F ) for J not equal to K, are estimated with */
/*              J         K */
/*   the same subdivision of the original interval of integration */
/*   and the same extrapolation scheme is applied. */
/*   For integrals with enough similarity, we may save time by applying */
/*   DQAINF to all integrands in one call. For integrals that varies */
/*   continuously as functions of some parameter, the estimates */
/*   produced by DQAINF will also vary continuously when the same */
/*   subdivision is applied to all components. This will generally not */
/*   be the case when the different components are given separate */
/*   treatment. */

/*   On the other hand this feature should be used with caution when the */
/*   different components of the integrals require clearly different */
/*   subdivisions. */

/*   ON ENTRY */

/*     NUMFUN Integer. */
/*            Number of components of the integral. */
/*     FUNSUB Externally declared subroutine for computing */
/*            all components of the integrand at the given */
/*            evaluation point. */
/*            It must have parameters (X,NUMFUN,NP,FUNVLS) */
/*            Input parameters: */
/*              X      Real array of dimension 21 defining the */
/*                     x-coordinates of the evaluation points. */
/*              NUMFUN Integer that defines the number of */
/*                     components of the integral I. */
/*              NP     Integer that gives the number of evaluation points */
/*            Output parameter: */
/*              FUNVLS Real array of dimension (21,NUMFUN) that defines */
/*                     the function values in the 21 evaluation points */
/*                     for the NUMFUN components of the integrand. */
/*     PERIOD Real. */
/*            All functions are assumed to have the same asymptotic */
/*            period > 0. */
/*     GAMMA  Real. */
/*            All functions are assumed to decay as c/x**GAMMA, */
/*            when x >> 1 and we go in steps of length PERIOD, */
/*            (GAMMA > 0). */
/*     A      Real array of dimension WRKSUB. */
/*     B      Real array of dimension WRKSUB. */
/*            A(1) and B(1) are the left endpoint and right endpoint */
/*            of the first interval,  A(1) <= B(1). B(1) is chosen by the */
/*            user and it is assumed that the integrand oscillates for */
/*            X >= B(1). Asymptotic period is PERIOD. Thus oscillations */
/*            are expected to be observed for points of distance PERIOD/2 */
/*            A(2), B(2), etc. are defined by the code, and as a warning: */
/*            the code may change the value of A(1) and B(1). */
/*     WRKSUB Integer. */
/*            Defines the length of the arrays A and B. (And thus the max */
/*            number of subregions allowed.) */
/*     EMAX   Integer. */
/*            The maximum number of extrapolation steps. */
/*     MINPTS Integer. */
/*            Minimum number of function evaluations. */
/*     MAXPTS Integer. */
/*            Maximum number of function evaluations. */
/*            The number of function evaluations over each sub-interval */
/*            is 21. */
/*     EPSABS Real. */
/*            Requested absolute error. */
/*     EPSREL Real. */
/*            Requested relative error. */
/*     NW     Integer. */
/*            Defines the length of the working array WORK. */
/*            We let */
/*     KEY    Integer */
/*            Choice of extrapolation method: */
/*            KEY = 0 : Euler's method. */
/*            KEY = 1 : Modified Euler. */
/*            KEY = 2 : Overholt's polynomial order 2-method. */
/*                      This last method is the only one that needs */
/*                      the value of GAMMA. */
/*     RESTAR Integer. */
/*            If RESTAR = 0, this is the first attempt to compute */
/*            the integral. */
/*            If RESTAR = 1, */
/*            then we restart a previous attempt. */
/*            In this case the only parameters for DQAINF that may */
/*            be changed (with respect to the previous call of DQAINF) */
/*            are MINPTS, MAXPTS, EPSABS, EPSREL and RESTAR. */

/*   ON RETURN */

/*     RESULT Real array of dimension NUMFUN. */
/*            Approximations to all components of the integral. */
/*     ABSERR Real array of dimension NUMFUN. */
/*            Estimates of absolute errors. */
/*     NEVAL  Integer. */
/*            Number of function evaluations used by DQAINF. */
/*     IFAIL  Integer. */
/*            IFAIL = 0 for normal exit, when */

/*              ABSERR(J) <=  EPSABS or */
/*              ABSERR(J) <=  ABS(RESULT(J))*EPSREL with MAXPTS or less */
/*              function evaluations for all values of J, */
/*              from 1 to NUMFUN. */

/*            IFAIL = +1 if MAXPTS was too small */
/*              to obtain the required accuracy. In this case DQAINF */
/*              returns values of RESULT with estimated absolute */
/*              errors ABSERR. */
/*            IFAIL = -1 if EMAX was too small */
/*              to obtain the required accuracy. In this case DQAINF */
/*              returns values of RESULT with estimated absolute */
/*              errors ABSERR. */
/*            IFAIL =  2 if NUMFUN is less than 1. */
/*            IFAIL =  3 if B(1) < A(1). */
/*            IFAIL =  4 if unlegal PERIOD. */
/*            IFAIL =  5 if MAXPTS is less than MINPTS or MINPTS < 21. */
/*            IFAIL =  6 if EPSABS <= 0 and EPSREL <= 0. */
/*            IFAIL =  7 if WRKSUB is too small. */
/*            IFAIL =  8 if unlegal value of EMAX. */
/*            IFAIL =  9 if unlegal RESTAR. */
/*            IFAIL = 10 if unlegal value of KEY. */
/*            IFAIL = 11 if NW is less than LIMIT (See the code DCHINF). */
/*            IFAIL = 12 if either PERIOD is wrong or B(1) is too small. */
/*            IFAIL = 13 if unable to estimate GAMMA (In case KEY=2 only) */
/*     A      Real array of dimension WRKSUB. */
/*     B      Real array of dimension WRKSUB. */
/*            On exit A and B contains the endpoints of the intervals */
/*            used to produce the approximations to the integrals. */
/*     DONE   Logical array of dimension NUMFUN. */
/*            On exit : DONE(I)=.TRUE. if function number I has been */
/*            integrated to the specified precision, else DONE(I)=.FALSE. */
/*            (Note that IFAIL = 1 just tells you */
/*            that something is wrong, however most of the integrals in */
/*            the vector may have been computed to the specified precisio */
/*     WORK   Real array of dimension NW. */
/*            Used as working storage. */
/*            WORK(NW) = NSUB, the number of sub-intervals in the data */
/*            structure. */
/*            Let NW >= 1+5*NUMFUN+4*WRKSUB*NUMFUN */
/*                     +3*WRKSUB+(EMAX+1)**2 + (NUMNUL+3+NUM)*NUMFUN) */
/*            Then */
/*            WORK(1),...,WORK(NUMFUN*WRKSUB) contain the estimated */
/*              components of the integrals over the sub-intervals. */
/*            WORK(NUMFUN*WRKSUB+1),...,WORK(2*NUMFUN*WRKSUB) contain */
/*              the estimated errors over the sub-intervals. */
/*            WORK(2*NUMFUN*WRKSUB+1),...,WORK(2*NUMFUN* */
/*              WRKSUB+WRKSUB) contain the greatest errors */
/*              in each sub-interval. */
/*            WORK(2*WRKSUB*NUMFUN+WRKSUB+1),...,WORK(2*WRKSUB*NUMFUN */
/*               +2*WRKSUB) contain the left sub-division point in each */
/*               sub-interval. */
/*            WORK(2*WRKSUB*NUMFUN+2*WRKSUB+1),...,WORK(2*WRKSUB*NUMFUN */
/*               +3*WRKSUB) contain the right sub-division point in each */
/*               sub-interval. */
/*            WORK((2*NUMFUN+3)*WRKSUB+1),...,WORK(NW) is used as */
/*              temporary storage in DADINF. */
/*     IWORK  Integer array of dimension 3*WRKSUB. */
/*            Used as working storage. */

/* ***LONG DESCRIPTION */

/*   The information for each interval is contained in the data */
/*   structures A,B, WORK and IWORK. A and B contain the endpoints of */
/*   the intervals. When passed on to DADINF, WORK is split into four */
/*   arrays VALUES, ERRORS, GREATE and WORK2. VALUES contains the */
/*   estimated values of the integrals. ERRORS contains the estimated */
/*   errors. GREATE contains the greatest estimated error for each */
/*   interval. The data structures for the intervals are in DADINF */
/*   organized as a heap, and the size of GREATE(I) defines the */
/*   position of interval I in the heap. The heap is maintained by the */
/*   program DTRINF and we use a partially ordered list of pointers, */
/*   saved in IWORK. When passed on to DADINF, IWORK is split into three */
/*   arrays WORST, LIST and UPOINT. LIST is a partially ordered list */
/*   such that GREATE(LIST(1)) is the maximum worst case error estimate */
/*   for all sub-intervals in our data-structure. In WORST the index to */
/*   the integral with the greatest error-estimate is kept. UPOINT  is */
/*   an array of pointers to where in the U-sequence  a region belongs. */
/*   This information is used when updating  the corresponding U-term */
/*   after a subdivision. */

/*   The subroutine for estimating the integral and the error over each */
/*   sub-interval, DGAINT, uses WORK2 as a work array. */

/* ***REFERENCES */

/*   T.O.Espelid and K.J.Overholt, DQAINF: An Algorithm for Automatic */
/*   Integration of Infinite Oscillating Tails, submitted for publ. 1993. */

/* ***ROUTINES CALLED DCHINF,DADINF */
/* ***END PROLOGUE DQAINF */

/*   Parameters */


/*   Global variables. */


/*   Local variables. */


/* ***FIRST EXECUTABLE STATEMENT DQAINF */

/*   Check the input parameters. */

    /* Parameter adjustments */
    --done;
    --abserr;
    --result;
    --iwork;
    --b;
    --a;
    --work;

    /* Function Body */
    dchinf_(numfun, (U_fp)funsub, period, gamma, &a[1], &b[1], minpts, maxpts,
	     epsabs, epsrel, wrksub, nw, emax, key, restar, neval, ifail);
    if (*ifail != 0) {
	return 0;
    }

/*   Split up the work space. */

    i1 = 1;
    i2 = i1 + *wrksub * *numfun;
    i3 = i2 + *wrksub * *numfun;
    i4 = i3 + *wrksub;
    i5 = i4 + *wrksub;
    i6 = i5 + *wrksub;
    i7 = i6 + *wrksub * *numfun;
    i8 = i7 + *wrksub * *numfun;
    i9 = i8 + *numfun;
    i10 = i9 + *numfun;
    i11 = i10 + *numfun;
    i12 = i11 + (*emax + 1) * (*emax + 1);
    i13 = i12 + *numfun;
    i14 = i13 + *numfun;

/*   On restart runs the number of sub-intervals from the */
/*   previous call is assigned to NSUB. */

    if (*restar == 1) {
	nsub = (integer) work[*nw];
    }

/*   Compute the size of the temporary work space needed in DADINF. */

    lenw = *numfun * 38;
    dadinf_(numfun, (U_fp)funsub, period, gamma, &a[1], &b[1], wrksub, emax, 
	    minpts, maxpts, epsabs, epsrel, restar, &lenw, key, &result[1], &
	    abserr[1], neval, &nsub, ifail, &done[1], &work[i1], &work[i2], &
	    work[i3], &work[i4], &work[i5], &work[i6], &work[i7], &work[i8], &
	    work[i9], &work[i10], &work[i11], &work[i12], &work[i13], &work[
	    i14], &iwork[1], &iwork[*wrksub + 1], &iwork[(*wrksub << 1) + 1]);
    work[*nw] = (doublereal) nsub;
    return 0;

/* ***END DQAINF */

} /* dqainf_ */

#ifdef __cplusplus
	}
#endif
