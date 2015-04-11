/* nag.F -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/
#include "nag.h"

#ifdef __cplusplus
extern "C" {
#endif


double d01gae_(double *x, double *y, int n, double &er,int &ifail)
{
    /* Initialized data */
    char srname[20]="D01GAE";
    /* System generated locals */
    int i__1;
    double r__1, r__2,ans;

    /* Local variables */
    double c__;
    int i__;
    double s, d1, d2, d3, h1, h2, h3, h4, r1, r2, r3, r4;
    int nn;


/*     THIS SUBROUTINE INTEGRATES A FUNCTION (Y) SPECIFIED */
/*     NUMERICALLY AT N POINTS (X), WHERE N IS AT LEAST 4, */
/*     OVER THE RANGE X(1) TO X(N).  THE POINTS NEED NOT BE */
/*     EQUALLY SPACED, BUT SHOULD BE DISTINCT AND IN ASCENDING */
/*     OR DESCENDING ORDER.  AN ERROR ESTIMATE IS RETURNED. */
/*     THE METHOD IS DUE TO GILL AND MILLER. */

/*     NAG COPYRIGHT 1975 */
/*     MARK 5 RELEASE */
/*     MARK 7 REVISED IER-154 (DEC 1978) */
    /* Parameter adjustments */
    --y;
    --x;

    /* Function Body */
    ans =  0.;
    er =  0.;
    if (n >= 4) {
	goto L20;
    }
    ifail = ErrNag(ifail, 1, &srname[0]);
    return ans;

/*     CHECK POINTS ARE STRICTLY INCREASING OR DECREASING */

L20:
    h2 = x[2] - x[1];
    i__1 = n;
    for (i__ = 3; i__ <= i__1; ++i__) {
	h3 = x[i__] - x[i__ - 1];
 if ((r__1 = h2 * h3) <  0.) {
	    goto L25;
	} else if (r__1 == 0) {
	    goto L30;
	} else {
	    goto L35;
	}
L25:
 ifail = ErrNag(ifail, 2, &srname[0]);
 return ans;
L30:
 ifail = ErrNag(ifail, 3, &srname[0]);
 return ans;
L35:
	;
    }

/*     INTEGRATE OVER INITIAL INTERVAL */

    d3 = (y[2] - y[1]) / h2;
    h3 = x[3] - x[2];
    d1 = (y[3] - y[2]) / h3;
    h1 = h2 + h3;
    d2 = (d1 - d3) / h1;
    h4 = x[4] - x[3];
    r1 = (y[4] - y[3]) / h4;
    r2 = (r1 - d1) / (h4 + h3);
    h1 += h4;
    r3 = (r2 - d2) / h1;
    ans = h2 * (y[1] + h2 * (d3 /  2. - h2 * (d2 /  6. - (h2 + 
     h3 *  2.) * r3 /  12.)));
/* Computing 3rd power */
    r__1 = h2, r__2 = r__1;
    s = -(r__2 * (r__1 * r__1)) * (h2 * (h2 *  3. + h4 *  5.) + 
     h3 *  10. * h1) /  60.;
    r4 =  0.;

/*     INTEGRATE OVER CENTRAL PORTION OF RANGE */

    nn = n - 1;
    i__1 = nn;
    for (i__ = 3; i__ <= i__1; ++i__) {
 ans += h3 * ((y[i__] + y[i__ - 1]) /  2. - h3 * h3 * (d2 + r2 
  + (h2 - h4) * r3) /  12.);
/* Computing 3rd power */
	r__1 = h3, r__2 = r__1;
 c__ = r__2 * (r__1 * r__1) * (h3 *  2. * h3 + (h3 * (h4 + h2) + 
  h4 *  2. * h2) *  5.) /  120.;
 er += (c__ + s) * r4;
	if (i__ != 3) {
	    s = c__;
	}
	if (i__ == 3) {
     s += c__ *  2.;
	}
 if (i__ - n + 1 != 0) {
	    goto L40;
	} else {
	    goto L80;
	}
L40:
	h1 = h2;
	h2 = h3;
	h3 = h4;
	d1 = r1;
	d2 = r2;
	d3 = r3;
	h4 = x[i__ + 2] - x[i__ + 1];
	r1 = (y[i__ + 2] - y[i__ + 1]) / h4;
	r4 = h4 + h3;
	r2 = (r1 - d1) / r4;
	r4 += h2;
	r3 = (r2 - d2) / r4;
	r4 += h1;
	r4 = (r3 - d3) / r4;
/* L60: */
    }

/*     INTEGRATE OVER FINAL INTERVAL */

L80:
    ans += h4 * (y[n] - h4 * (r1 /  2. + h4 * (r2 /  6. + (h3 * 
      2. + h4) * r3 /  12.)));
/* Computing 3rd power */
    r__1 = h4, r__2 = r__1;
    er = er - r__2 * (r__1 * r__1) * r4 * (h4 * (h4 *  3. + h2 * (
     float)5.) + h3 *  10. * (h2 + h3 + h4)) /  60. + s * 
	    r4;
    ans += er;
    ifail = 0;
    return ans;
} /* d01gae_ */

#ifdef __cplusplus
	}
#endif

int ErrNag(int &Err,int Flag,char *Name)
  {
   cout<<" NagError in Block "<<Name<<" Flag "<<Flag<<"\n";
   Err=Flag;
   return Flag;
  }; 
