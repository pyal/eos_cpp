/* qawfe.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Table of constant values */

static real c_b4 = (float)0.;
static integer c__1 = 1;

/* Subroutine */ int qawfe_(U_fp f, real *a, real *omega, integer *integr, 
	real *epsabs, integer *limlst, integer *limit, integer *maxp1, real *
	result, real *abserr, integer *neval, integer *ier, real *rslst, real 
	*erlst, integer *ierlst, integer *lst, real *alist__, real *blist, 
	real *rlist, real *elist, integer *iord, integer *nnlog, real *chebmo)
{
    /* Initialized data */

    static real p = (float).9;
    static real pi = (float)3.1415926535897932;

    /* System generated locals */
    integer chebmo_dim1, chebmo_offset, i__1;
    real r__1, r__2;

    /* Local variables */
    real fact, epsa;
    extern /* Subroutine */ int qelg_(integer *, real *, real *, real *, real 
	    *, integer *);
    integer last, nres;
    real psum[52];
    integer l;
    extern /* Subroutine */ int qagie_(U_fp, real *, integer *, real *, real *
	    , integer *, real *, real *, integer *, integer *, real *, real *,
	     real *, real *, integer *, integer *);
    real cycle;
    extern /* Subroutine */ int qawoe_(U_fp, real *, real *, real *, integer *
	    , real *, real *, integer *, integer *, integer *, real *, real *,
	     integer *, integer *, integer *, real *, real *, real *, real *, 
	    integer *, integer *, integer *, real *);
    integer ktmin;
    real c1, c2, uflow, p1;
    extern doublereal r1mach_(integer *);
    real res3la[3];
    integer numrl2;
    real dl, ep;
    integer ll;
    real abseps, correc;
    integer momcom;
    real reseps, errsum, dla, drl, eps;
    integer nev;

/* ***begin prologue  qawfe */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a3a1 */
/* ***keywords  automatic integrator, special-purpose, */
/*             fourier integrals, */
/*             integration between zeros with dqawoe, */
/*             convergence acceleration with dqelg */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           dedoncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  the routine calculates an approximation result to a */
/*            given fourier integal */
/*            i = integral of f(x)*w(x) over (a,infinity) */
/*             where w(x) = cos(omega*x) or w(x) = sin(omega*x), */
/*            hopefully satisfying following claim for accuracy */
/*            abs(i-result).le.epsabs. */
/* ***description */

/*        computation of fourier integrals */
/*        standard fortran subroutine */
/*        real version */

/*        parameters */
/*         on entry */
/*            f      - real */
/*                     function subprogram defining the integrand */
/*                     function f(x). the actual name for f needs to */
/*                     be declared e x t e r n a l in the driver program. */

/*            a      - real */
/*                     lower limit of integration */

/*            omega  - real */
/*                     parameter in the weight function */

/*            integr - integer */
/*                     indicates which weight function is used */
/*                     integr = 1      w(x) = cos(omega*x) */
/*                     integr = 2      w(x) = sin(omega*x) */
/*                     if integr.ne.1.and.integr.ne.2, the routine will */
/*                     end with ier = 6. */

/*            epsabs - real */
/*                     absolute accuracy requested, epsabs.gt.0 */
/*                     if epsabs.le.0, the routine will end with ier = 6. */

/*            limlst - integer */
/*                     limlst gives an upper bound on the number of */
/*                     cycles, limlst.ge.1. */
/*                     if limlst.lt.3, the routine will end with ier = 6. */

/*            limit  - integer */
/*                     gives an upper bound on the number of subintervals */
/*                     allowed in the partition of each cycle, limit.ge.1 */
/*                     each cycle, limit.ge.1. */

/*            maxp1  - integer */
/*                     gives an upper bound on the number of */
/*                     chebyshev moments which can be stored, i.e. */
/*                     for the intervals of lengths abs(b-a)*2**(-l), */
/*                     l=0,1, ..., maxp1-2, maxp1.ge.1 */

/*         on return */
/*            result - real */
/*                     approximation to the integral x */

/*            abserr - real */
/*                     estimate of the modulus of the absolute error, */
/*                     which should equal or exceed abs(i-result) */

/*            neval  - integer */
/*                     number of integrand evaluations */

/*            ier    - ier = 0 normal and reliable termination of */
/*                             the routine. it is assumed that the */
/*                             requested accuracy has been achieved. */
/*                     ier.gt.0 abnormal termination of the routine. the */
/*                             estimates for integral and error are less */
/*                             reliable. it is assumed that the requested */
/*                             accuracy has not been achieved. */
/*            error messages */
/*                    if omega.ne.0 */
/*                     ier = 1 maximum number of  cycles  allowed */
/*                             has been achieved., i.e. of subintervals */
/*                             (a+(k-1)c,a+kc) where */
/*                             c = (2*int(abs(omega))+1)*pi/abs(omega), */
/*                             for k = 1, 2, ..., lst. */
/*                             one can allow more cycles by increasing */
/*                             the value of limlst (and taking the */
/*                             according dimension adjustments into */
/*                             account). */
/*                             examine the array iwork which contains */
/*                             the error flags on the cycles, in order to */
/*                             look for eventual local integration */
/*                             difficulties. if the position of a local */
/*                             difficulty can be determined (e.g. */
/*                             singularity, discontinuity within the */
/*                             interval) one will probably gain from */
/*                             splitting up the interval at this point */
/*                             and calling appropriate integrators on */
/*                             the subranges. */
/*                         = 4 the extrapolation table constructed for */
/*                             convergence acceleration of the series */
/*                             formed by the integral contributions over */
/*                             the cycles, does not converge to within */
/*                             the requested accuracy. as in the case of */
/*                             ier = 1, it is advised to examine the */
/*                             array iwork which contains the error */
/*                             flags on the cycles. */
/*                         = 6 the input is invalid because */
/*                             (integr.ne.1 and integr.ne.2) or */
/*                              epsabs.le.0 or limlst.lt.3. */
/*                              result, abserr, neval, lst are set */
/*                              to zero. */
/*                         = 7 bad integrand behaviour occurs within one */
/*                             or more of the cycles. location and type */
/*                             of the difficulty involved can be */
/*                             determined from the vector ierlst. here */
/*                             lst is the number of cycles actually */
/*                             needed (see below). */
/*                             ierlst(k) = 1 the maximum number of */
/*                                           subdivisions (= limit) has */
/*                                           been achieved on the k th */
/*                                           cycle. */
/*                                       = 2 occurrence of roundoff error */
/*                                           is detected and prevents the */
/*                                           tolerance imposed on the */
/*                                           k th cycle, from being */
/*                                           achieved. */
/*                                       = 3 extremely bad integrand */
/*                                           behaviour occurs at some */
/*                                           points of the k th cycle. */
/*                                       = 4 the integration procedure */
/*                                           over the k th cycle does */
/*                                           not converge (to within the */
/*                                           required accuracy) due to */
/*                                           roundoff in the */
/*                                           extrapolation procedure */
/*                                           invoked on this cycle. it */
/*                                           is assumed that the result */
/*                                           on this interval is the */
/*                                           best which can be obtained. */
/*                                       = 5 the integral over the k th */
/*                                           cycle is probably divergent */
/*                                           or slowly convergent. it */
/*                                           must be noted that */
/*                                           divergence can occur with */
/*                                           any other value of */
/*                                           ierlst(k). */
/*                    if omega = 0 and integr = 1, */
/*                    the integral is calculated by means of dqagie */
/*                    and ier = ierlst(1) (with meaning as described */
/*                    for ierlst(k), k = 1). */

/*            rslst  - real */
/*                     vector of dimension at least limlst */
/*                     rslst(k) contains the integral contribution */
/*                     over the interval (a+(k-1)c,a+kc) where */
/*                     c = (2*int(abs(omega))+1)*pi/abs(omega), */
/*                     k = 1, 2, ..., lst. */
/*                     note that, if omega = 0, rslst(1) contains */
/*                     the value of the integral over (a,infinity). */

/*            erlst  - real */
/*                     vector of dimension at least limlst */
/*                     erlst(k) contains the error estimate corresponding */
/*                     with rslst(k). */

/*            ierlst - integer */
/*                     vector of dimension at least limlst */
/*                     ierlst(k) contains the error flag corresponding */
/*                     with rslst(k). for the meaning of the local error */
/*                     flags see description of output parameter ier. */

/*            lst    - integer */
/*                     number of subintervals needed for the integration */
/*                     if omega = 0 then lst is set to 1. */

/*            alist, blist, rlist, elist - real */
/*                     vector of dimension at least limit, */

/*            iord, nnlog - integer */
/*                     vector of dimension at least limit, providing */
/*                     space for the quantities needed in the subdivision */
/*                     process of each cycle */

/*            chebmo - real */
/*                     array of dimension at least (maxp1,25), providing */
/*                     space for the chebyshev moments needed within the */
/*                     cycles */

/* ***references  (none) */
/* ***routines called  qagie,qawoe,qelg,r1mach */
/* ***end prologue  qawfe */





/*            the dimension of  psum  is determined by the value of */
/*            limexp in subroutine qelg (psum must be */
/*            of dimension (limexp+2) at least). */

/*           list of major variables */
/*           ----------------------- */

/*           c1, c2    - end points of subinterval (of length */
/*                       cycle) */
/*           cycle     - (2*int(abs(omega))+1)*pi/abs(omega) */
/*           psum      - vector of dimension at least (limexp+2) */
/*                       (see routine qelg) */
/*                       psum contains the part of the epsilon */
/*                       table which is still needed for further */
/*                       computations. */
/*                       each element of psum is a partial sum of */
/*                       the series which should sum to the value of */
/*                       the integral. */
/*           errsum    - sum of error estimates over the */
/*                       subintervals, calculated cumulatively */
/*           epsa      - absolute tolerance requested over current */
/*                       subinterval */
/*           chebmo    - array containing the modified chebyshev */
/*                       moments (see also routine qc25f) */

    /* Parameter adjustments */
    --ierlst;
    --erlst;
    --rslst;
    --nnlog;
    --iord;
    --elist;
    --rlist;
    --blist;
    --alist__;
    chebmo_dim1 = *maxp1;
    chebmo_offset = 1 + chebmo_dim1 * 1;
    chebmo -= chebmo_offset;

    /* Function Body */

/*           test on validity of parameters */
/*           ------------------------------ */

/* ***first executable statement  qawfe */
    *result = (float)0.;
    *abserr = (float)0.;
    *neval = 0;
    *lst = 0;
    *ier = 0;
    if (*integr != 1 && *integr != 2 || *epsabs <= (float)0. || *limlst < 3) {
	*ier = 6;
    }
    if (*ier == 6) {
	goto L999;
    }
    if (*omega != (float)0.) {
	goto L10;
    }

/*           integration by qagie if omega is zero */
/*           -------------------------------------- */

    if (*integr == 1) {
	qagie_((U_fp)f, &c_b4, &c__1, epsabs, &c_b4, limit, result, abserr, 
		neval, ier, &alist__[1], &blist[1], &rlist[1], &elist[1], &
		iord[1], &last);
    }
    rslst[1] = *result;
    erlst[1] = *abserr;
    ierlst[1] = *ier;
    *lst = 1;
    goto L999;

/*           initializations */
/*           --------------- */

L10:
    l = dabs(*omega);
    dl = (real) ((l << 1) + 1);
    cycle = dl * pi / dabs(*omega);
    *ier = 0;
    ktmin = 0;
    *neval = 0;
    numrl2 = 0;
    nres = 0;
    c1 = *a;
    c2 = cycle + *a;
    p1 = (float)1. - p;
    eps = *epsabs;
    uflow = r1mach_(&c__1);
    if (*epsabs > uflow / p1) {
	eps = *epsabs * p1;
    }
    ep = eps;
    fact = (float)1.;
    correc = (float)0.;
    *abserr = (float)0.;
    errsum = (float)0.;

/*           main do-loop */
/*           ------------ */

    i__1 = *limlst;
    for (*lst = 1; *lst <= i__1; ++(*lst)) {

/*           integrate over current subinterval. */

	dla = (real) (*lst);
	epsa = eps * fact;
	qawoe_((U_fp)f, &c1, &c2, omega, integr, &epsa, &c_b4, limit, lst, 
		maxp1, &rslst[*lst], &erlst[*lst], &nev, &ierlst[*lst], &last,
		 &alist__[1], &blist[1], &rlist[1], &elist[1], &iord[1], &
		nnlog[1], &momcom, &chebmo[chebmo_offset]);
	*neval += nev;
	fact *= p;
	errsum += erlst[*lst];
	drl = (r__1 = rslst[*lst], dabs(r__1)) * (float)50.;

/*           test on accuracy with partial sum */

	if (errsum + drl <= *epsabs && *lst >= 6) {
	    goto L80;
	}
/* Computing MAX */
	r__1 = correc, r__2 = erlst[*lst];
	correc = dmax(r__1,r__2);
	if (ierlst[*lst] != 0) {
/* Computing MAX */
	    r__1 = ep, r__2 = correc * p1;
	    eps = dmax(r__1,r__2);
	}
	if (ierlst[*lst] != 0) {
	    *ier = 7;
	}
	if (*ier == 7 && errsum + drl <= correc * (float)10. && *lst > 5) {
	    goto L80;
	}
	++numrl2;
	if (*lst > 1) {
	    goto L20;
	}
	psum[0] = rslst[1];
	goto L40;
L20:
	psum[numrl2 - 1] = psum[ll - 1] + rslst[*lst];
	if (*lst == 2) {
	    goto L40;
	}

/*           test on maximum number of subintervals */

	if (*lst == *limlst) {
	    *ier = 1;
	}

/*           perform new extrapolation */

	qelg_(&numrl2, psum, &reseps, &abseps, res3la, &nres);

/*           test whether extrapolated result is influenced by */
/*           roundoff */

	++ktmin;
	if (ktmin >= 15 && *abserr <= (errsum + drl) * (float).001) {
	    *ier = 4;
	}
	if (abseps > *abserr && *lst != 3) {
	    goto L30;
	}
	*abserr = abseps;
	*result = reseps;
	ktmin = 0;

/*           if ier is not 0, check whether direct result (partial */
/*           sum) or extrapolated result yields the best integral */
/*           approximation */

	if (*abserr + correc * (float)10. <= *epsabs || *abserr <= *epsabs && 
		correc * (float)10. >= *epsabs) {
	    goto L60;
	}
L30:
	if (*ier != 0 && *ier != 7) {
	    goto L60;
	}
L40:
	ll = numrl2;
	c1 = c2;
	c2 += cycle;
/* L50: */
    }

/*         set final result and error estimate */
/*         ----------------------------------- */

L60:
    *abserr += correc * (float)10.;
    if (*ier == 0) {
	goto L999;
    }
    if (*result != (float)0. && psum[numrl2 - 1] != (float)0.) {
	goto L70;
    }
    if (*abserr > errsum) {
	goto L80;
    }
    if (psum[numrl2 - 1] == (float)0.) {
	goto L999;
    }
L70:
    if (*abserr / dabs(*result) > (errsum + drl) / (r__1 = psum[numrl2 - 1], 
	    dabs(r__1))) {
	goto L80;
    }
    if (*ier >= 1 && *ier != 7) {
	*abserr += drl;
    }
    goto L999;
L80:
    *result = psum[numrl2 - 1];
    *abserr = errsum + drl;
L999:
    return 0;
} /* qawfe_ */

#ifdef __cplusplus
	}
#endif
