#include <lib/precompiled/math.h>

/* C05AZE.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/
#include "mtzero.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "lib/std/f2c.h"

/* Table of constant values */

static double c_b13 = 1.;
//static double c_b16 = 0.;
double r_sign1(double *a,double *b)
{
 double x;
 x = (*a >= 0 ? *a : - *a);
 return( *b >= 0 ? x : -x);
 };

/* Subroutine */ int c05aze_(double *x, double *y, double *fx, double *tolx, integer *
	ir, double *c__, integer *ind, integer *ifail)
{
    /* Initialized data */

    /* System generated locals */
    double r__1, r__2;

    /* Builtin functions */
//    double r_sign(double *, double *), sqrt(doubledouble);

    /* Local variables */
    double diff, diff1, diff2;
    integer i__;
    logical t;
    double ab, rel, max__, tol;

/*     MARK 8 RE-ISSUE. NAG COPYRIGHT 1979. */
/*     .. SCALAR ARGUMENTS .. */
/*     .. ARRAY ARGUMENTS .. */
/*     .. */
/*     .. LOCAL SCALARS .. */
/*     .. FUNCTION REFERENCES .. */
/*     .. */
    /* Parameter adjustments */
    --c__;

    /* Function Body */
    i__ = 0;
    if (*ind > 0 && *ind <= 4 || *ind == -1) {
	goto L20;
    }
/*     USER NOT CHECKED IND OR CHANGED IT */
    i__ = 2;
    *ind = 0;
    goto L640;
L20:
    if (*tolx > 0. && (*ir == 0 || *ir == 1 || *ir == 2)) {
	goto L40;
    }
    i__ = 3;
    *ind = 0;
    goto L640;
L40:
    rel = 1.;
    ab = 1.;
    if (*ir == 1) {
	rel = 0.;
    }
    if (*ir == 2) {
	ab = 0.;
    }
    if (*ind == -1) {
	goto L80;
    }
    switch (*ind) {
	case 1:  goto L60;
	case 2:  goto L100;
	case 3:  goto L180;
	case 4:  goto L480;
    }
L60:
    c__[3] = *x;
    *ind = 2;
    return 0;
L80:
    c__[3] = *x;
L100:
    if (*fx != 0.) {
	goto L140;
    }
L120:
    *y = *x;
    *ind = 0;
    i__ = 0;
    goto L640;
L140:
    c__[4] = *fx;
    c__[15] = dabs(*fx);
    c__[16] = 0.;
    *x = *y;
    *y = c__[3];
    c__[2] = c__[4];
    c__[5] = *x;
    if (*ind == -1) {
	goto L160;
    }
    *ind = 3;
    return 0;
L160:
    *fx = c__[1];
    *ind = 3;
L180:
    if (*fx == 0.) {
	goto L120;
    }
    if (r_sign1(&c_b13, fx) != r_sign1(&c_b13, &c__[2])) {
	goto L200;
    }
    *ind = 0;
    i__ = 1;
    goto L640;
L200:
    c__[6] = *fx;
    c__[13] = sqrt(MIN_EPS);
/* Computing MAX */
    r__1 = c__[15], r__2 = dabs(*fx);
    c__[15] = dmax(r__1,r__2);
    c__[14] = MIN_NUMBER;
    c__[16] = 0.;
L220:
    c__[1] = c__[5];
    c__[2] = c__[6];
    c__[17] = 0.;
L240:
    if (dabs(c__[2]) >= dabs(c__[4])) {
	goto L280;
    }
    if (c__[1] == c__[5]) {
	goto L260;
    }
    c__[7] = c__[5];
    c__[8] = c__[6];
L260:
    c__[5] = c__[3];
    c__[6] = c__[4];
    *x = c__[1];
    c__[3] = *x;
    c__[4] = c__[2];
    c__[1] = c__[5];
    c__[2] = c__[6];
L280:
/* Computing MAX */
    r__1 = ab, r__2 = rel * dabs(c__[3]);
    tol = *tolx * .5 * dmax(r__1,r__2);
    diff2 = (c__[1] - c__[3]) * .5;
    c__[12] = diff2;
    diff2 += c__[3];
    if (c__[12] == 0.) {
	goto L340;
    }
    if (dabs(c__[12]) <= tol) {
	goto L580;
    }
    if (c__[17] < 2.5) {
	goto L300;
    }
    c__[11] = c__[12];
    goto L460;
L300:
    tol *= r_sign1(&c_b13, &c__[12]);
    diff1 = (c__[3] - c__[5]) * c__[4];
    if (c__[17] > 1.5) {
	goto L320;
    }
    diff = c__[6] - c__[4];
    goto L380;
L320:
    if (c__[7] != c__[3] && c__[7] != c__[5]) {
	goto L360;
    }
L340:
    *ind = 0;
    i__ = 5;
    goto L640;
L360:
    c__[9] = (c__[8] - c__[4]) / (c__[7] - c__[3]);
    c__[10] = (c__[8] - c__[6]) / (c__[7] - c__[5]);
    diff1 = c__[10] * diff1;
    diff = c__[9] * c__[6] - c__[10] * c__[4];
L380:
    if (diff1 >= 0.) {
	goto L400;
    }
    diff1 = -diff1;
    diff = -diff;
L400:
    if (dabs(diff1) > c__[14] && diff1 > diff * tol) {
	goto L420;
    }
    c__[11] = tol;
    goto L460;
L420:
    if (diff1 >= c__[12] * diff) {
	goto L440;
    }
    c__[11] = diff1 / diff;
    goto L460;
L440:
    c__[11] = c__[12];
L460:
    c__[7] = c__[5];
    c__[8] = c__[6];
    c__[5] = c__[3];
    c__[6] = c__[4];
    c__[3] += c__[11];
    *x = c__[3];
    *y = c__[1];
    *ind = 4;
    return 0;
L480:
    if (*fx == 0.) {
	goto L120;
    }
    c__[4] = *fx;
    max__ = dabs(*fx);
    if (c__[13] * max__ <= c__[15]) {
	goto L500;
    }
    if (c__[16] == 1.) {
	c__[16] = -1.;
    }
    if (c__[16] == 0.) {
	c__[16] = 1.;
    }
    goto L520;
L500:
    c__[16] = 0.;
L520:
    if (c__[2] >= 0.) {
	goto L540;
    }
    t = c__[4] <= 0.;
    goto L560;
L540:
    t = c__[4] >= 0.;
L560:
    if (t) {
	goto L220;
    }
    i__ = (integer) (c__[17] + .1);
    ++i__;
    if (c__[11] == c__[12]) {
	i__ = 0;
    }
    c__[17] = (double) i__;
    goto L240;
L580:
    if (c__[16] >= 0.) {
	goto L600;
    }
    i__ = 4;
    goto L620;
L600:
    *y = c__[1];
    i__ = 0;
L620:
    *ind = 0;
L640:
    *ifail = 1;
    return 0;
} /* c05aze_ */

/* Subroutine */ int c05axe_(double *x, double *fx, double *tol, integer *ir, double *
	scale, double *c__, integer *ind, integer *ifail)
{
    /* System generated locals */
    double r__1, r__2;

    /* Builtin functions */
//    double sqrt(doubledouble);

    /* Local variables */
    integer i__;
    double oldth, ab, se, sf, rel;

/*     MARK 8 RELEASE. NAG COPYRIGHT 1979. */
/*     MARK 9 REVISED. IER-301 (SEP 1981). */
/*     USES METHOD OF OF SWIFT AND LINDFIELD, */
/*     C.J.VOL 21. MINK=C(1),MAXK=C(2). */
/*     .. SCALAR ARGUMENTS .. */
/*     .. ARRAY ARGUMENTS .. */
/*     .. */
/*     .. LOCAL SCALARS .. */
/*     .. FUNCTION REFERENCES .. */
/*     .. */
    /* Parameter adjustments */
    --c__;
//	int count=0;
//LBeg:
//	count++;if (count>MaxCount) {*ifail=10;return 1;}

    /* Function Body */
    if (*tol <= 0.) {
	goto L380;
    }
    if (*x + *scale == *x) {
	goto L420;
    }
    if (*ind < -1 || *ind > 4) {
	goto L400;
    }
    if (*ir < 0 || *ir > 2) {
	goto L380;
    }
    ab = 1.;
    rel = 1.;
    if (*ir == 1) {
	rel = 0.;
    }
    if (*ir == 2) {
	ab = 0.;
    }
    i__ = *ind + 2;
    switch (i__) {
	case 1:  goto L40;
	case 2:  goto L400;
	case 3:  goto L20;
	case 4:  goto L40;
	case 5:  goto L80;
	case 6:  goto L260;
    }
L20:
    *ind = 2;//*fx=fun_clc(*x);goto LBeg;
    return 0;
L40:
    if (*fx == 0.) {
	goto L520;
    }
    c__[1] = 3.;
    c__[2] = 8.;
    c__[4] = 1.;
    c__[6] = 1.;
    c__[7] = .5;
    c__[8] = *fx;
    c__[22] = *x;
    c__[25] = MIN_EPS;
    c__[19] = sqrt(c__[25]);
    c__[5] = 1. - c__[19] * .5;
    c__[23] = c__[19] * *scale;
    c__[20] = 0.;
L60:
    *x = c__[22] + c__[23];
    if (*x == c__[22]) {
	goto L100;
    }
    *ind = 3;//*fx=fun_clc(*x);goto LBeg;
    return 0;
L80:
    if (*fx == 0.) {
	goto L520;
    }
    c__[24] = *fx - c__[8];
    *x = c__[22];
/* Computing MAX */
    r__1 = dabs(*fx), r__2 = dabs(c__[8]);
    if (dabs(c__[24]) > c__[25] * 100. * dmax(r__1,r__2)) {
	goto L120;
    }
L100:
    c__[23] *= 10.;
    if (dabs(c__[23]) > dabs(*scale)) {
	goto L420;
    }
    goto L60;
L120:
    c__[12] = c__[23] / c__[24];

/*     CONTINUATION OUTER LOOP */

L140:
    c__[14] = sqrt(*tol);
    if (c__[5] == 0.) {
	c__[14] = *tol;
    }
    c__[3] = 0.;
    c__[9] = *x;
    c__[13] = c__[12];
    c__[15] = c__[6] - c__[5];
    if (c__[15] < c__[25] * 100.) {
	goto L440;
    }
    c__[21] = c__[8] * c__[5];
    c__[10] = c__[15] * c__[8];
    c__[11] = c__[10];
    c__[26] = 0.;

/*     CONTINUATION INNER LOOP */

L160:
    c__[17] = c__[12] * c__[10];
    sf = c__[26];
    c__[26] = 0.;
/* Computing MAX */
    r__1 = rel * dabs(*x);
    if (dabs(c__[17]) < c__[14] * dmax(r__1,ab)) {
	c__[26] = sf + 1.;
    }
    if (c__[26] > 0. && dabs(c__[10]) < dabs(c__[11]) && c__[3] > (
	    double)1.5) {
	goto L280;
    }
    if ((r__1 = c__[3] - c__[2], dabs(r__1)) > .5) {
	goto L240;
    }
    if (c__[26] > c__[2] - 2.5) {
	goto L280;
    }
    se = (c__[10] + c__[21]) / c__[8];
    if (se > c__[5] && se < c__[6]) {
	goto L200;
    }
    c__[12] = c__[13];
    c__[7] *= .5;
    if (c__[6] - c__[5] > c__[25] * 100.) {
	goto L180;
    }
    if (c__[4] < 1.5) {
	goto L440;
    }
    goto L340;
L180:
    c__[5] = (c__[5] + c__[6]) * .5;
    *x = c__[9];
    goto L140;
L200:
    c__[21] = se * c__[8];
    c__[15] = c__[6] - se;
    if (c__[5] != 0.) {
	goto L220;
    }
    if (c__[15] < c__[25] * 100.) {
	goto L440;
    }
L220:
    c__[5] = se;
    goto L280;
L240:
    *x -= c__[17];
    *ind = 4;//*fx=fun_clc(*x);goto LBeg;
    return 0;
L260:
    if (*fx == 0.) {
	goto L520;
    }
    se = *fx - c__[21];
    sf = se - c__[10];
/* Computing MAX */
    r__1 = dabs(se), r__2 = dabs(c__[10]), r__1 = max(r__1,r__2), r__2 = dabs(
	    c__[21]);
    if (dabs(sf) <= c__[25] * 2. * dmax(r__1,r__2)) {
	goto L280;
    }
    c__[10] = se;
    c__[3] += 1.;
    if (sf == 0.) {
	goto L160;
    }
    c__[12] = -c__[17] / sf;
    goto L160;

/*     SUCCESSFUL INNER LOOP */

L280:
    if (c__[5] == 0.) {
	goto L520;
    }
    c__[17] = c__[9] - *x;
    oldth = c__[6];
    c__[6] = c__[5];
    if ((r__1 = c__[4] - 1., dabs(r__1)) > .5) {
	goto L300;
    }
    c__[5] = 1. - c__[19];
    goto L360;
L300:
    sf = c__[17] / c__[15];
    se = (c__[18] / c__[16] - sf) / (c__[15] + c__[16]);
    if (dabs(sf) * c__[19] > dabs(c__[15]) || se == 0. || dabs(se) * 
	    c__[19] > (r__1 = c__[15] + c__[16], dabs(r__1))) {
	goto L310;
    }
    if (c__[3] <= c__[1] + .5 && c__[4] > 2.5) {
	c__[7] *= 2.;
    }
    if (dabs(se) * c__[5] > c__[7] * (r__1 = sf - se * c__[15], dabs(r__1))) {
	goto L320;
    }
    c__[5] = 0.;
    goto L360;
L310:
    if (c__[5] <= .9) {
	goto L315;
    }
    c__[5] = .9;
    goto L330;
L315:
    c__[5] *= .5;
    goto L330;
L320:
    c__[5] -= c__[7] * (r__1 = sf / se - c__[15], dabs(r__1));
    if (c__[5] < 0.) {
	c__[5] = 0.;
    }
    if (c__[5] == 0.) {
	goto L360;
    }
L330:
    if (oldth - c__[5] >= c__[25] * 100. && c__[5] >= c__[19]) {
	goto L360;
    }
L340:
    if (c__[4] > 2.5) {
	goto L440;
    }
    c__[19] *= .1;
    if (c__[19] < c__[25] * 100.) {
	goto L480;
    }
    c__[6] = 1.;
    c__[5] = 1. - c__[19] * .5;
    c__[20] = 0.;
    *x = c__[9];
    c__[4] = 1.;
    c__[7] = .5;
    goto L140;
L360:
    c__[16] = c__[15];
    c__[18] = c__[17];
    c__[4] += 1.;
    c__[20] = 1.;
    if (c__[5] == 0.) {
	c__[20] = 2.;
    }
    goto L140;
/*     INPUT ERROR */
L380:
    i__ = 1;
    goto L540;
/*     WRONG IND ON ENTRY */
L400:
    i__ = 2;
    goto L540;
/*     WRONG SCALE */
L420:
    i__ = 3;
    goto L540;
/*     SINGULARITY ON CONTINUATION PATH */
L440:
    if (c__[20] != 1.) {
	goto L460;
    }
    i__ = 4;
    goto L540;
/*     CANNOT GET STARTED */
L460:
    if (c__[20] != 0.) {
	goto L500;
    }
L480:
    i__ = 5;
    goto L540;
/*     CANNOT FINISH */
L500:
    i__ = 6;
    goto L540;
/*     FINISHED */
L520:
    i__ = 0;
L540:
    *ind = 0;
    *ifail = 1;
    return 0;
} /* c05axe_ */



#ifdef __cplusplus
	}
#endif

int FindZeroAB(X_func f,double A,double B,double &X_res,double &F_res,
               double Tol,int MaxCount,int TypeEnd)
{
 double c[18];
 double x=A,y=B,fx,tol=Tol;
 integer ir=TypeEnd,ind=-1,ifail; // ind=1 - fx!=f(x); ind=-1 fx==f(x)
 int ret,count=0;
 // TypeEnd= 0,1,2
//int c05aze_(double *x, double *y, double *fx, double *tolx, integer *
// ir, double *c__, integer *ind, integer *ifail)
 while ((ind!=0) && (count<MaxCount)) 
	{fx=f(x);ret=c05aze_(&x,&y,&fx,&tol,&ir,c,&ind,&ifail);}
 X_res=x;F_res=fx;if (count>=MaxCount) return 0;
 return ifail;
}; 

int FindZeroPos(X_func f,double &X_res,double &F_res,double Tol,
                double Deriv_scale,int MaxCount,int TypeEnd)
{
 double c[30];
 double x=X_res,fx,tol=Tol,scale=Deriv_scale;
 integer ir=TypeEnd,ind=-1,ifail; // ind=1 - fx!=f(x); ind=-1 fx==f(x)
 int ret,count=0;
 // TypeEnd= 0,1,2
// int c05axe_(double *x, double *fx, double *tol, integer *ir, double *
// scale, double *c__, integer *ind, integer *ifail)
 while ((ind!=0) && (count++<MaxCount))
	{fx=f(x);ret=c05axe_(&x,&fx,&tol,&ir,&scale,c,&ind,&ifail);
//cout<<" X "<<x<<" FX "<<fx<<"\n";
 }
cout<<" Zero Count "<<count<<"\n";
 X_res=x;F_res=fx;
 if (fabs(fx)>StndErr) cout<<" FindZero Pos fx="<<fx<<" ifail "<<ifail<<" x "<<x<<"\n";cout.flush();
 return ifail;
}; 
