/* FPKNOT.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "f2c.h"

/* Subroutine */ int fpknot_(real *x, integer *m, real *t, integer *n, real *
	fpint, integer *nrdata, integer *nrint, integer *nest, integer *
	istart)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer next, j, k, ihalf;
    real fpmax;
    integer maxpt;
    real am, an;
    integer jj, jk, jbegin, maxbeg, number, jpoint, nrx;

/*  subroutine fpknot locates an additional knot for a spline of degree */
/*  k and adjusts the corresponding parameters,i.e. */
/*    t     : the position of the knots. */
/*    n     : the number of knots. */
/*    nrint : the number of knotintervals. */
/*    fpint : the sum of squares of residual right hand sides */
/*            for each knot interval. */
/*    nrdata: the number of data points inside each knot interval. */
/*  istart indicates that the smallest data point at which the new knot */
/*  may be added is x(istart+1) */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --x;
    --nrdata;
    --fpint;
    --t;

    /* Function Body */
    k = (*n - *nrint - 1) / 2;
/*  search for knot interval t(number+k) <= x <= t(number+k+1) where */
/*  fpint(number) is maximal on the condition that nrdata(number) */
/*  not equals zero. */
    fpmax = (float)0.;
    jbegin = *istart;
    i__1 = *nrint;
    for (j = 1; j <= i__1; ++j) {
	jpoint = nrdata[j];
	if (fpmax >= fpint[j] || jpoint == 0) {
	    goto L10;
	}
	fpmax = fpint[j];
	number = j;
	maxpt = jpoint;
	maxbeg = jbegin;
L10:
	jbegin = jbegin + jpoint + 1;
/* L20: */
    }
/*  let coincide the new knot t(number+k+1) with a data point x(nrx) */
/*  inside the old knot interval t(number+k) <= x <= t(number+k+1). */
    ihalf = maxpt / 2 + 1;
    nrx = maxbeg + ihalf;
    next = number + 1;
    if (next > *nrint) {
	goto L40;
    }
/*  adjust the different parameters. */
    i__1 = *nrint;
    for (j = next; j <= i__1; ++j) {
	jj = next + *nrint - j;
	fpint[jj + 1] = fpint[jj];
	nrdata[jj + 1] = nrdata[jj];
	jk = jj + k;
	t[jk + 1] = t[jk];
/* L30: */
    }
L40:
    nrdata[number] = ihalf - 1;
    nrdata[next] = maxpt - ihalf;
    am = (real) maxpt;
    an = (real) nrdata[number];
    fpint[number] = fpmax * an / am;
    an = (real) nrdata[next];
    fpint[next] = fpmax * an / am;
    jk = next + k;
    t[jk] = x[nrx];
    ++(*n);
    ++(*nrint);
    return 0;
} /* fpknot_ */

#ifdef __cplusplus
	}
#endif
