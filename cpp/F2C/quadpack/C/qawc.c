/* qawc.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Table of constant values */

static integer c__26 = 26;

/* Subroutine */ int qawc_(E_fp f, real *a, real *b, real *c__, real *epsabs, 
	real *epsrel, real *result, real *abserr, integer *neval, integer *
	ier, integer *limit, integer *lenw, integer *last, integer *iwork, 
	real *work)
{
    extern /* Subroutine */ int qawce_(E_fp, real *, real *, real *, real *, 
	    real *, integer *, real *, real *, integer *, integer *, real *, 
	    real *, real *, real *, integer *, integer *);
    integer l1, l2, l3;
    extern /* Subroutine */ int xerror_(char *, integer *, integer *, integer 
	    *, ftnlen);
    integer lvl;

/* ***begin prologue  qawc */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a2a1,j4 */
/* ***keywords  automatic integrator, special-purpose, */
/*             cauchy principal value, */
/*             clenshaw-curtis, globally adaptive */
/* ***author  piessens,robert ,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  the routine calculates an approximation result to a */
/*            cauchy principal value i = integral of f*w over (a,b) */
/*            (w(x) = 1/((x-c), c.ne.a, c.ne.b), hopefully satisfying */
/*            following claim for accuracy */
/*            abs(i-result).le.max(epsabe,epsrel*abs(i)). */
/* ***description */

/*        computation of a cauchy principal value */
/*        standard fortran subroutine */
/*        real version */


/*        parameters */
/*         on entry */
/*            f      - real */
/*                     function subprogram defining the integrand */
/*                     function f(x). the actual name for f needs to be */
/*                     declared e x t e r n a l in the driver program. */

/*            a      - real */
/*                     under limit of integration */

/*            b      - real */
/*                     upper limit of integration */

/*            c      - parameter in the weight function, c.ne.a, c.ne.b. */
/*                     if c = a or c = b, the routine will end with */
/*                     ier = 6 . */

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
/*                     estimate or the modulus of the absolute error, */
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
/*                             adjustments into account). however, if */
/*                             this yields no improvement it is advised */
/*                             to analyze the integrand in order to */
/*                             determine the integration difficulties. */
/*                             if the position of a local difficulty */
/*                             can be determined (e.g. singularity, */
/*                             discontinuity within the interval) one */
/*                             will probably gain from splitting up the */
/*                             interval at this point and calling */
/*                             appropriate integrators on the subranges. */
/*                         = 2 the occurrence of roundoff error is detec- */
/*                             ted, which prevents the requested */
/*                             tolerance from being achieved. */
/*                         = 3 extremely bad integrand behaviour occurs */
/*                             at some points of the integration */
/*                             interval. */
/*                         = 6 the input is invalid, because */
/*                             c = a or c = b or */
/*                             (epsabs.le.0 and */
/*                              epsrel.lt.max(50*rel.mach.acc.,0.5d-28)) */
/*                             or limit.lt.1 or lenw.lt.limit*4. */
/*                             result, abserr, neval, last are set to */
/*                             zero. exept when lenw or limit is invalid, */
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

/*           lenw   - integer */
/*                    dimensioning parameter for work */
/*                    lenw must be at least limit*4. */
/*                    if lenw.lt.limit*4, the routine will end with */
/*                    ier = 6. */

/*            last  - integer */
/*                    on return, last equals the number of subintervals */
/*                    produced in the subdivision process, which */
/*                    determines the number of significant elements */
/*                    actually in the work arrays. */

/*         work arrays */
/*            iwork - integer */
/*                    vector of dimension at least limit, the first k */
/*                    elements of which contain pointers */
/*                    to the error estimates over the subintervals, */
/*                    such that work(limit*3+iwork(1)), ... , */
/*                    work(limit*3+iwork(k)) form a decreasing */
/*                    sequence, with k = last if last.le.(limit/2+2), */
/*                    and k = limit+1-last otherwise */

/*            work  - real */
/*                    vector of dimension at least lenw */
/*                    on return */
/*                    work(1), ..., work(last) contain the left */
/*                     end points of the subintervals in the */
/*                     partition of (a,b), */
/*                    work(limit+1), ..., work(limit+last) contain */
/*                     the right end points, */
/*                    work(limit*2+1), ..., work(limit*2+last) contain */
/*                     the integral approximations over the subintervals, */
/*                    work(limit*3+1), ..., work(limit*3+last) */
/*                     contain the error estimates. */

/* ***references  (none) */
/* ***routines called  qawce,xerror */
/* ***end prologue  qawc */




/*         check validity of limit and lenw. */

/* ***first executable statement  qawc */
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

/*         prepare call for qawce. */

    l1 = *limit + 1;
    l2 = *limit + l1;
    l3 = *limit + l2;
    qawce_((E_fp)f, a, b, c__, epsabs, epsrel, limit, result, abserr, neval, 
	    ier, &work[1], &work[l1], &work[l2], &work[l3], &iwork[1], last);

/*         call error handler if necessary. */

    lvl = 0;
L10:
    if (*ier == 6) {
	lvl = 1;
    }
    if (*ier != 0) {
	xerror_("abnormal return from  qawc", &c__26, ier, &lvl, (ftnlen)26);
    }
    return 0;
} /* qawc_ */

#ifdef __cplusplus
	}
#endif
