/* qags.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Table of constant values */

static integer c__26 = 26;

/* Subroutine */ int qags_(E_fp f, real *a, real *b, real *epsabs, real *
	epsrel, real *result, real *abserr, integer *neval, integer *ier, 
	integer *limit, integer *lenw, integer *last, integer *iwork, real *
	work)
{
    extern /* Subroutine */ int qagse_(E_fp, real *, real *, real *, real *, 
	    integer *, real *, real *, integer *, integer *, real *, real *, 
	    real *, real *, integer *, integer *);
    integer l1, l2, l3;
    extern /* Subroutine */ int xerror_(char *, integer *, integer *, integer 
	    *, ftnlen);
    integer lvl;

/* ***begin prologue  qags */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a1a1 */
/* ***keywords  automatic integrator, general-purpose, */
/*             (end-point) singularities, extrapolation, */
/*             globally adaptive */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & prog. div. - k.u.leuven */
/* ***purpose  the routine calculates an approximation result to a given */
/*            definite integral  i = integral of f over (a,b), */
/*            hopefully satisfying following claim for accuracy */
/*            abs(i-result).le.max(epsabs,epsrel*abs(i)). */
/* ***description */

/*        computation of a definite integral */
/*        standard fortran subroutine */
/*        real version */


/*        parameters */
/*         on entry */
/*            f      - real */
/*                     function subprogram defining the integrand */
/*                     function f(x). the actual name for f needs to be */
/*                     declared e x t e r n a l in the driver program. */

/*            a      - real */
/*                     lower limit of integration */

/*            b      - real */
/*                     upper limit of integration */

/*            epsabs - real */
/*                     absolute accuracy requested */
/*            epsrel - real */
/*                     relative accuracy requested */
/*                     if  epsabs.le.0 */
/*                     and epsrel.lt.max(50*rel.mach.acc.,0.5d-28), */
/*                     the routine will end with ier = 6. */

/*         on return */
/*            result - real */
/*                     approximation to the integral */

/*            abserr - real */
/*                     estimate of the modulus of the absolute error, */
/*                     which should equal or exceed abs(i-result) */

/*            neval  - integer */
/*                     number of integrand evaluations */

/*            ier    - integer */
/*                     ier = 0 normal and reliable termination of the */
/*                             routine. it is assumed that the requested */
/*                             accuracy has been achieved. */
/*                     ier.gt.0 abnormal termination of the routine */
/*                             the estimates for integral and error are */
/*                             less reliable. it is assumed that the */
/*                             requested accuracy has not been achieved. */
/*            error messages */
/*                     ier = 1 maximum number of subdivisions allowed */
/*                             has been achieved. one can allow more sub- */
/*                             divisions by increasing the value of limit */
/*                             (and taking the according dimension */
/*                             adjustments into account. however, if */
/*                             this yields no improvement it is advised */
/*                             to analyze the integrand in order to */
/*                             determine the integration difficulties. if */
/*                             the position of a local difficulty can be */
/*                             determined (e.g. singularity, */
/*                             discontinuity within the interval) one */
/*                             will probably gain from splitting up the */
/*                             interval at this point and calling the */
/*                             integrator on the subranges. if possible, */
/*                             an appropriate special-purpose integrator */
/*                             should be used, which is designed for */
/*                             handling the type of difficulty involved. */
/*                         = 2 the occurrence of roundoff error is detec- */
/*                             ted, which prevents the requested */
/*                             tolerance from being achieved. */
/*                             the error may be under-estimated. */
/*                         = 3 extremely bad integrand behaviour */
/*                             occurs at some points of the integration */
/*                             interval. */
/*                         = 4 the algorithm does not converge. */
/*                             roundoff error is detected in the */
/*                             extrapolation table. it is presumed that */
/*                             the requested tolerance cannot be */
/*                             achieved, and that the returned result is */
/*                             the best which can be obtained. */
/*                         = 5 the integral is probably divergent, or */
/*                             slowly convergent. it must be noted that */
/*                             divergence can occur with any other value */
/*                             of ier. */
/*                         = 6 the input is invalid, because */
/*                             (epsabs.le.0 and */
/*                              epsrel.lt.max(50*rel.mach.acc.,0.5d-28) */
/*                             or limit.lt.1 or lenw.lt.limit*4. */
/*                             result, abserr, neval, last are set to */
/*                             zero.except when limit or lenw is invalid, */
/*                             iwork(1), work(limit*2+1) and */
/*                             work(limit*3+1) are set to zero, work(1) */
/*                             is set to a and work(limit+1) to b. */

/*         dimensioning parameters */
/*            limit - integer */
/*                    dimensioning parameter for iwork */
/*                    limit determines the maximum number of subintervals */
/*                    in the partition of the given integration interval */
/*                    (a,b), limit.ge.1. */
/*                    if limit.lt.1, the routine will end with ier = 6. */

/*            lenw  - integer */
/*                    dimensioning parameter for work */
/*                    lenw must be at least limit*4. */
/*                    if lenw.lt.limit*4, the routine will end */
/*                    with ier = 6. */

/*            last  - integer */
/*                    on return, last equals the number of subintervals */
/*                    produced in the subdivision process, detemines the */
/*                    number of significant elements actually in the work */
/*                    arrays. */

/*         work arrays */
/*            iwork - integer */
/*                    vector of dimension at least limit, the first k */
/*                    elements of which contain pointers */
/*                    to the error estimates over the subintervals */
/*                    such that work(limit*3+iwork(1)),... , */
/*                    work(limit*3+iwork(k)) form a decreasing */
/*                    sequence, with k = last if last.le.(limit/2+2), */
/*                    and k = limit+1-last otherwise */

/*            work  - real */
/*                    vector of dimension at least lenw */
/*                    on return */
/*                    work(1), ..., work(last) contain the left */
/*                     end-points of the subintervals in the */
/*                     partition of (a,b), */
/*                    work(limit+1), ..., work(limit+last) contain */
/*                     the right end-points, */
/*                    work(limit*2+1), ..., work(limit*2+last) contain */
/*                     the integral approximations over the subintervals, */
/*                    work(limit*3+1), ..., work(limit*3+last) */
/*                     contain the error estimates. */



/* ***references  (none) */
/* ***routines called  qagse,xerror */
/* ***end prologue  qags */





/*         check validity of limit and lenw. */

/* ***first executable statement  qags */
    /* Parameter adjustments */
    --iwork;
    --work;

    /* Function Body */
    *ier = 6;
    *neval = 0;
    *last = 0;
    *result = (float)0.;
    *abserr = (float)0.;
    if (*limit < 1 || *lenw < *limit << 2) {
	goto L10;
    }

/*         prepare call for qagse. */

    l1 = *limit + 1;
    l2 = *limit + l1;
    l3 = *limit + l2;

    qagse_((E_fp)f, a, b, epsabs, epsrel, limit, result, abserr, neval, ier, &
	    work[1], &work[l1], &work[l2], &work[l3], &iwork[1], last);

/*         call error handler if necessary. */

    lvl = 0;
L10:
    if (*ier == 6) {
	lvl = 1;
    }
    if (*ier != 0) {
	xerror_("abnormal return from  qags", &c__26, ier, &lvl, (ftnlen)26);
    }
    return 0;
} /* qags_ */

#ifdef __cplusplus
	}
#endif