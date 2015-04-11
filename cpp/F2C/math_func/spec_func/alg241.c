// inverse error function 
//doublereal ppnd7_(real *p, integer *ifault)              prec 1e-7
// doublereal ppnd16_(doublereal *p, integer *ifault)      prec 1e-16

/* alg241.f -- translated by f2c (version 20060506).
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

/*   http://lib.stat.cmu.edu/apstat/241 */
/*   IERF functions ( inverse error function Err=1e-16) */
/*   according to the paper */
/*   http://home.online.no/~pjacklam/notes/invnorm/ */
/*   published: */
/*   http://home.online.no/~pjacklam/notes/invnorm/impl/lea/lea.c */
doublereal ppnd7_(real *p, integer *ifault)
{
    /* System generated locals */
    real ret_val;

    /* Builtin functions */
    double log(doublereal), sqrt(doublereal);

    /* Local variables */
    real q, r__;


/* 	ALGORITHM AS241  APPL. STATIST. (1988) VOL. 37, NO. 3, 477- */
/* 	484. */

/* 	Produces the normal deviate Z corresponding to a given lower */
/* 	tail area of P; Z is accurate to about 1 part in 10**7. */

/* 	The hash sums below are the sums of the mantissas of the */
/* 	coefficients.   They are included for use in checking */
/* 	transcription. */


/* 	Coefficients for P close to 0.5 */

/* 	HASH SUM AB    32.31845 77772 */

/* 	Coefficients for P not close to 0, 0.5 or 1. */

/* 	HASH SUM CD    15.76149 29821 */

/* 	Coefficients for P near 0 or 1. */

/* 	HASH SUM EF    19.40529 10204 */

    *ifault = 0;
    q = *p - (float).5;
    if (dabs(q) <= (float).425) {
	r__ = (float).180625 - q * q;
	ret_val = q * (((r__ * (float)59.10937472 + (float)159.29113202) * 
		r__ + (float)50.434271938) * r__ + (float)3.3871327179) / (((
		r__ * (float)67.1875636 + (float)78.757757664) * r__ + (float)
		17.895169469) * r__ + (float)1.);
	return ret_val;
    } else {
	if (q < (float)0.) {
	    r__ = *p;
	} else {
	    r__ = (float)1. - *p;
	}
	if (r__ <= (float)0.) {
	    *ifault = 1;
	    ret_val = (float)0.;
	    return ret_val;
	}
	r__ = sqrt(-log(r__));
	if (r__ <= (float)5.) {
	    r__ += (float)-1.6;
	    ret_val = (((r__ * (float).17023821103 + (float)1.3067284816) * 
		    r__ + (float)2.75681539) * r__ + (float)1.4234372777) / ((
		    r__ * (float).12021132975 + (float).7370016425) * r__ + (
		    float)1.);
	} else {
	    r__ += (float)-5.;
	    ret_val = (((r__ * (float).017337203997 + (float).42868294337) * 
		    r__ + (float)3.081226386) * r__ + (float)6.657905115) / ((
		    r__ * (float).012258202635 + (float).24197894225) * r__ + 
		    (float)1.);
	}
	if (q < (float)0.) {
	    ret_val = -ret_val;
	}
	return ret_val;
    }
    return ret_val;
} /* ppnd7_ */



doublereal ppnd16_(doublereal *p, integer *ifault)
{
    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double log(doublereal), sqrt(doublereal);

    /* Local variables */
    doublereal q, r__;


/* 	ALGORITHM AS241  APPL. STATIST. (1988) VOL. 37, NO. 3 */

/* 	Produces the normal deviate Z corresponding to a given lower */
/* 	tail area of P; Z is accurate to about 1 part in 10**16. */

/* 	The hash sums below are the sums of the mantissas of the */
/* 	coefficients.   They are included for use in checking */
/* 	transcription. */


/* 	Coefficients for P close to 0.5 */

/* 	HASH SUM AB    55.88319 28806 14901 4439 */

/* 	Coefficients for P not close to 0, 0.5 or 1. */

/* 	HASH SUM CD    49.33206 50330 16102 89036 */

/* 	Coefficients for P near 0 or 1. */

/* 	HASH SUM EF    47.52583 31754 92896 71629 */

    *ifault = 0;
    q = *p - .5;
    if (abs(q) <= .425) {
	r__ = .180625 - q * q;
	ret_val = q * (((((((r__ * 2509.0809287301226727 + 
		33430.575583588128105) * r__ + 67265.770927008700853) * r__ + 
		45921.953931549871457) * r__ + 13731.693765509461125) * r__ + 
		1971.5909503065514427) * r__ + 133.14166789178437745) * r__ + 
		3.387132872796366608) / (((((((r__ * 5226.495278852854561 + 
		28729.085735721942674) * r__ + 39307.89580009271061) * r__ + 
		21213.794301586595867) * r__ + 5394.1960214247511077) * r__ + 
		687.1870074920579083) * r__ + 42.313330701600911252) * r__ + 
		1.);
	return ret_val;
    } else {
	if (q < 0.) {
	    r__ = *p;
	} else {
	    r__ = 1. - *p;
	}
	if (r__ <= 0.) {
	    *ifault = 1;
	    ret_val = 0.;
	    return ret_val;
	}
	r__ = sqrt(-log(r__));
	if (r__ <= 5.) {
	    r__ += -1.6;
	    ret_val = (((((((r__ * 7.7454501427834140764e-4 + 
		    .0227238449892691845833) * r__ + .24178072517745061177) * 
		    r__ + 1.27045825245236838258) * r__ + 
		    3.64784832476320460504) * r__ + 5.7694972214606914055) * 
		    r__ + 4.6303378461565452959) * r__ + 
		    1.42343711074968357734) / (((((((r__ * 
		    1.05075007164441684324e-9 + 5.475938084995344946e-4) * 
		    r__ + .0151986665636164571966) * r__ + 
		    .14810397642748007459) * r__ + .68976733498510000455) * 
		    r__ + 1.6763848301838038494) * r__ + 
		    2.05319162663775882187) * r__ + 1.);
	} else {
	    r__ += -5.;
	    ret_val = (((((((r__ * 2.01033439929228813265e-7 + 
		    2.71155556874348757815e-5) * r__ + 
		    .0012426609473880784386) * r__ + .026532189526576123093) *
		     r__ + .29656057182850489123) * r__ + 
		    1.7848265399172913358) * r__ + 5.4637849111641143699) * 
		    r__ + 6.6579046435011037772) / (((((((r__ * 
		    2.04426310338993978564e-15 + 1.4215117583164458887e-7) * 
		    r__ + 1.8463183175100546818e-5) * r__ + 
		    7.868691311456132591e-4) * r__ + .0148753612908506148525) 
		    * r__ + .13692988092273580531) * r__ + 
		    .59983220655588793769) * r__ + 1.);
	}
	if (q < 0.) {
	    ret_val = -ret_val;
	}
	return ret_val;
    }
    return ret_val;
} /* ppnd16_ */

#ifdef __cplusplus
	}
#endif
