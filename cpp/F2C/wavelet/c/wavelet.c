/* wavelet.f -- translated by f2c (version 20060506).
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
#include "lib\std\f2c.h"

/* Table of constant values */

static integer c__9 = 9;
static integer c__1 = 1;
static doublereal c_b12 = 2.;
static doublereal c_b17 = -.25;
static doublecomplex c_b25 = {0.,1.};
static integer c__2 = 2;

/* **************************************************************************** */

/* WAVEPACK:  routines to compute the wavelet transform of a time series, */
/*            and significance levels. */

/* Written by:   Christopher Torrence and Gilbert P. Compo */

/* Available from:  http://paos.colorado.edu/research/wavelets/ */

/* Requires the following packages:   CFFTPACK, CHISQR */

/* Notes: */

/*  (1) All routines are written in single precision (DOUBLE PRECISION), */
/*      except for CHISQR, which requires double precision input. */
/*      Single precision should be sufficient for most applications. */

/*  (2) The CFFTPACK and CHISQR routines were not written by us, */
/*      and no guarentees are made as to their reliability or efficiency. */

/*  (3) No provision is made for output to files or to graphics packages. */
/*      The user is expected to call these routines from within */
/*      their own programs. See sample program "wavetest.f". */

/*  (4) Little error checking is done. Check your input carefully. */
/*      The programs are not completely ANSI compatible, so use caution. */

/*  (5) Time series are currently limited to 65535 points. */


/* Reference: Torrence, C. and G. P. Compo, 1998: A Practical Guide to */
/*            Wavelet Analysis. <I>Bull. Amer. Meteor. Soc.</I>, 79, 61-78. */

/* Notice: Please acknowledge the use of this software in any publications: */
/*    ``Wavelet software was provided by C. Torrence and G. Compo, */
/*      and is available at URL: http://paos.colorado.edu/research/wavelets/''. */

/*  Copyright (C) 1998, Christopher Torrence */
/* This software may be used, copied, or redistributed as long as it is not */
/* sold and this copyright notice is reproduced on each copy made.  This */
/* routine is provided as is without any express or implied warranties */
/* whatsoever. */


/* Modified: November 1999 by Arjan van Dijk to include IMPLICIT NONE and */
/*           to convert all routines to DOUBLE precision. */
/* **************************************************************************** */
/* **************************************************************************** */
/* WAVELET: computes the wavelet transform of a time series, */
/*          with appropriate parameters. */


/* INPUTS: */

/*  n [INT] = the number of points in "y". */

/*  y [DOUBLE PRECISION] = the time series of length "n". */

/*  dt [DOUBLE PRECISION] = amount of time between each Y value, i.e. the sampling time. */

/*  mother [INT] = An integer giving the mother wavelet to use. */
/*                   0='Morlet' */
/*                   1='Paul' */
/*                   2='DOG' (derivative of Gaussian) */
/*               If (mother<0 or >2) then default is 'Morlet'. */

/*  param [DOUBLE PRECISION] = mother wavelet parameter. If <0 then default is used. */
/*            For 'Morlet' this is k0 (wavenumber), default is 6. */
/*            For 'Paul' this is m (order), default is 4. */
/*            For 'DOG' this is m (m-th derivative), default is 2. */


/*  s0 [DOUBLE PRECISION] = the smallest scale of the wavelet.  Typically = 2*dt. */
/*         Note: for accurate reconstruction and variance computation */
/*             set s0=dt for Morlet; s0=dt/4 for Paul */

/*  dj [DOUBLE PRECISION] = the spacing between discrete scales. Typically = 0.25. */
/*         A smaller # will give better scale resolution, but be slower. */

/*  jtot [INT] = the # of scales. */
/*             Scales range from s0 up to s0*2^[(jtot-1)*dj], */
/*             Typically jtot=1+(LOG2(n dt/s0))/dj */

/*  npad [INT] = the total number of points (including padding) to */
/*             use for the wavelet transform. Typically this is some */
/*             power of 2. It must be greater or equal to "n". */
/*             If npad>n, then zeroes are padded onto the end */
/*             of the time series. */


/* OUTPUTS: */

/*  wave [DCMPLX(n,jtot)] = 2D array of the real & imaginary parts */
/*                 of the wavelet transform, versus time & scale. */
/*                 CABS(wave) gives the WAVELET amplitude, */
/*                 ATAN2(AIMAG(wave),DBLE(wave)) gives WAVELET phase. */
/*                 The WAVELET power spectrum is CABS(wave)**2. */

/*  scale [DOUBLE PRECISION(jtot)] = the wavelet scales that were used. */

/*  period [DOUBLE PRECISION(jtot)] = the "Fourier" periods (in time units) corresponding */
/*            to "scale". */

/*  coi [DOUBLE PRECISION(n)] = the e-folding factor used for the cone of influence. */


/* REQUIRES:   WAVE_FUNCTION, CFFTPACK */

/*  Copyright (C) 1998, Christopher Torrence and Gilbert P. Compo */
/* This software may be used, copied, or redistributed as long as it is not */
/* sold and this copyright notice is reproduced on each copy made.  This */
/* routine is provided as is without any express or implied warranties */
/* whatsoever. */
/* Subroutine */ int wavelet_(integer *n, doublereal *y, doublereal *dt, 
	integer *mother, doublereal *param, doublereal *s0, doublereal *dj, 
	integer *jtot, integer *npad, doublecomplex *wave, doublereal *scale, 
	doublereal *period, doublereal *coi)
{
    /* System generated locals */
    integer wave_dim1, wave_offset, i__1, i__2, i__3, i__4, i__5;
    doublereal d__1;
    doublecomplex z__1;

    /* Builtin functions */
    double atan(doublereal);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle();
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    doublecomplex daughter[65535];
    integer i__, j, k;
    doublereal pi, coi1;
    doublecomplex yfft[65535];
    doublereal freq1;
    extern /* Subroutine */ int cfftb_(integer *, doublecomplex *, doublereal 
	    *), cfftf_(integer *, doublecomplex *, doublereal *), cffti_(
	    integer *, doublereal *);
    doublereal ymean, kwave[65535], wsave[262155];
    extern /* Subroutine */ int wave_function__(integer *, doublereal *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublecomplex *);
    doublereal period1;

    /* Fortran I/O blocks */
    static cilist io___2 = { 0, 6, 0, 0, 0 };


/* ** initialize work arrays */
    /* Parameter adjustments */
    --coi;
    --y;
    --period;
    --scale;
    wave_dim1 = *n;
    wave_offset = 1 + wave_dim1;
    wave -= wave_offset;

    /* Function Body */
    pi = atan(1.) * 4.;
    if (*npad < *n) {
	s_wsle(&io___2);
	do_lio(&c__9, &c__1, "**WAVELET: \"npad\" must be greater than or eq\
ual to \"n\"", (ftnlen)54);
	e_wsle();
	return 0;
    }
    if (*mother < 0 || *mother > 2) {
	*mother = 0;
    }
/* ** find the time-series mean & remove it */
    ymean = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ymean += y[i__];
/* L10: */
    }
    ymean /= *n;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = i__ - 1;
	d__1 = y[i__] - ymean;
	yfft[i__2].r = d__1, yfft[i__2].i = 0.;
/* L15: */
    }
/* ** if desired, pad with extra zeroes */
    i__1 = *npad;
    for (i__ = *n + 1; i__ <= i__1; ++i__) {
	i__2 = i__ - 1;
	yfft[i__2].r = 0., yfft[i__2].i = 0.;
/* L20: */
    }
/* ** find the FFT of the time series [Eqn(3)] */
    cffti_(npad, wsave);
    cfftf_(npad, yfft, wsave);
    i__1 = *npad;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k - 1;
	i__3 = k - 1;
	d__1 = (doublereal) (*npad);
	z__1.r = yfft[i__3].r / d__1, z__1.i = yfft[i__3].i / d__1;
	yfft[i__2].r = z__1.r, yfft[i__2].i = z__1.i;
/* L30: */
    }
/* ** construct the wavenumber array [Eqn(5)] */
    freq1 = pi * 2. / ((doublereal) (*npad) * *dt);
    kwave[0] = 0.;
    i__1 = *npad / 2 + 1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	kwave[i__ - 1] = ((doublereal) i__ - 1.) * freq1;
/* L40: */
    }
    i__1 = *npad;
    for (i__ = *npad / 2 + 2; i__ <= i__1; ++i__) {
	kwave[i__ - 1] = -kwave[*npad - i__ + 1];
/* L50: */
    }
/* **----- Main wavelet loop */
/*      PRINT '(A8,2A12,/,5X,27("-"))','j','scale','period' */
    i__1 = *jtot;
    for (j = 1; j <= i__1; ++j) {
	d__1 = (doublereal) (j - 1) * *dj;
	scale[j] = *s0 * pow_dd(&c_b12, &d__1);
	wave_function__(npad, dt, mother, param, &scale[j], kwave, &period1, &
		coi1, daughter);
	period[j] = period1;
/* **    multiply the daughter by the time-series FFT */
	i__2 = *npad;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = k - 1;
	    i__4 = k - 1;
	    i__5 = k - 1;
	    z__1.r = daughter[i__4].r * yfft[i__5].r - daughter[i__4].i * 
		    yfft[i__5].i, z__1.i = daughter[i__4].r * yfft[i__5].i + 
		    daughter[i__4].i * yfft[i__5].r;
	    daughter[i__3].r = z__1.r, daughter[i__3].i = z__1.i;
/* L60: */
	}
/* **    inverse FFT [Eqn(4)] */
	cfftb_(npad, daughter, wsave);
/* **    store the wavelet transform, discard zero-padding at end */
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = i__ + j * wave_dim1;
	    i__4 = i__ - 1;
	    wave[i__3].r = daughter[i__4].r, wave[i__3].i = daughter[i__4].i;
/* L70: */
	}
/*        PRINT '(I8,2F12.3)',j,scale(j),period(j) */
/* L100: */
    }
/* **----- end loop */
/* ** construct the cone of influence */
    i__1 = (*n + 1) / 2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	coi[i__] = coi1 * *dt * ((doublereal) i__ - 1.);
	coi[*n - i__ + 1] = coi[i__];
/* L110: */
    }
    return 0;
} /* wavelet_ */

/* **************************************************************************** */
/* WAVE_FUNCTION: computes the daughter wavelets for a particular */
/*              wavelet function, with appropriate parameters. */


/* INPUTS: */

/*  nk [INT] = the number of points in "kwave" */

/*  dt [DOUBLE PRECISION] = amount of time between each Y value, i.e. the sampling time. */

/*  mother [INT] = An integer giving the mother wavelet to use. */
/*                   0='Morlet' */
/*                   1='Paul' */
/*                   2='DOG' (derivative of Gaussian) */

/*  param [DOUBLE PRECISION] = mother wavelet parameter. If <0 then default is used. */
/*            For 'Morlet' this is k0 (wavenumber), default is 6. */
/*            For 'Paul' this is m (order), default is 4. */
/*            For 'DOG' this is m (m-th derivative), default is 2. */

/*  scale1 [DOUBLE PRECISION] = the wavelet scale used to construct the daughter. */

/*  kwave [DOUBLE PRECISION(n)] = vector of wavenumbers, used to construct daughter. */


/* OUTPUTS: */

/*  period1 [DOUBLE PRECISION] = the "Fourier" period (in time units) that corresponds */
/*            to "scale1". */

/*  coi1 [DOUBLE PRECISION] = the e-folding factor used for the cone of influence. */

/*  daughter [DCMPLX(nk)] = real & imaginary parts of the wavelet function */
/*                    at "scale1" and "kwave". */


/* REQUIRES:   FACTORIAL, CHISQR */


/* Reference: Tables 1 & 2 in */
/*            Torrence, C. and G. P. Compo, 1998: A Practical Guide to */
/*            Wavelet Analysis. <I>Bull. Amer. Meteor. Soc.</I>, 79, 61-78. */

/*  Copyright (C) 1998, Christopher Torrence and Gilbert P. Compo */
/* This software may be used, copied, or redistributed as long as it is not */
/* sold and this copyright notice is reproduced on each copy made.  This */
/* routine is provided as is without any express or implied warranties */
/* whatsoever. */
/* Subroutine */ int wave_function__(integer *nk, doublereal *dt, integer *
	mother, doublereal *param, doublereal *scale1, doublereal *kwave, 
	doublereal *period1, doublereal *coi1, doublecomplex *daughter)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2, d__3;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal), pow_dd(doublereal *, 
	    doublereal *), exp(doublereal), pow_di(doublereal *, integer *);
    void pow_zi(doublecomplex *, doublecomplex *, integer *);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    integer k, m;
    extern integer factorial_(integer *);
    doublereal pi, sk;
    doublecomplex norm;
    extern doublereal gamma_(doublereal *);
    doublereal expnt, fourier_factor__;

    /* Parameter adjustments */
    --daughter;
    --kwave;

    /* Function Body */
    pi = atan(1.) * 4.;
    if (*mother == 0) {
/* *******************************************   Morlet wavelet */
	if (*param < 0.) {
	    *param = 6.;
	}
	d__1 = sqrt(pi * 2. * *scale1 / *dt) * pow_dd(&pi, &c_b17);
	norm.r = d__1, norm.i = 0.;
	i__1 = *nk / 2 + 1;
	for (k = 1; k <= i__1; ++k) {
/* Computing 2nd power */
	    d__1 = *scale1 * kwave[k] - *param;
	    expnt = d__1 * d__1 * -.5;
	    i__2 = k;
	    d__1 = exp(expnt);
	    z__1.r = d__1 * norm.r, z__1.i = d__1 * norm.i;
	    daughter[i__2].r = z__1.r, daughter[i__2].i = z__1.i;
/* L10: */
	}
	i__1 = *nk;
	for (k = *nk / 2 + 2; k <= i__1; ++k) {
	    i__2 = k;
	    daughter[i__2].r = 0., daughter[i__2].i = 0.;
/* L20: */
	}
/* Computing 2nd power */
	d__1 = *param;
	fourier_factor__ = pi * 4. / (*param + sqrt(d__1 * d__1 + 2.));
	*period1 = *scale1 * fourier_factor__;
	*coi1 = fourier_factor__ / sqrt(2.);
    } else if (*mother == 1) {
/* *******************************************   Paul wavelet */
	if (*param < 0.) {
	    *param = 4.;
	}
	m = (integer) (*param);
	i__1 = (m << 1) - 1;
	d__1 = sqrt(pi * 2. * *scale1 / *dt) * (pow_di(&c_b12, &m) / sqrt((
		doublereal) (m * factorial_(&i__1))));
	norm.r = d__1, norm.i = 0.;
	i__1 = *nk / 2 + 1;
	for (k = 1; k <= i__1; ++k) {
	    expnt = -(*scale1) * kwave[k];
	    i__2 = k;
	    d__2 = *scale1 * kwave[k];
	    d__1 = pow_di(&d__2, &m);
	    z__2.r = d__1 * norm.r, z__2.i = d__1 * norm.i;
	    d__3 = exp(expnt);
	    z__1.r = d__3 * z__2.r, z__1.i = d__3 * z__2.i;
	    daughter[i__2].r = z__1.r, daughter[i__2].i = z__1.i;
/* L30: */
	}
	i__1 = *nk;
	for (k = *nk / 2 + 2; k <= i__1; ++k) {
	    i__2 = k;
	    daughter[i__2].r = 0., daughter[i__2].i = 0.;
/* L40: */
	}
	fourier_factor__ = pi * 4. / (m * 2. + 1.);
	*period1 = *scale1 * fourier_factor__;
	*coi1 = fourier_factor__ * sqrt(2.);
    } else if (*mother == 2) {
/* *******************************************   DOG wavelet */
	if (*param < 0.) {
	    *param = 2.;
	}
	m = (integer) (*param);
	d__2 = m + .5;
	d__1 = sqrt(pi * 2. * *scale1 / *dt) * sqrt(1. / gamma_(&d__2));
	norm.r = d__1, norm.i = 0.;
	z__2.r = -norm.r, z__2.i = -norm.i;
	pow_zi(&z__3, &c_b25, &m);
	z__1.r = z__2.r * z__3.r - z__2.i * z__3.i, z__1.i = z__2.r * z__3.i 
		+ z__2.i * z__3.r;
	norm.r = z__1.r, norm.i = z__1.i;
	i__1 = *nk;
	for (k = 1; k <= i__1; ++k) {
	    sk = *scale1 * kwave[k];
	    i__2 = k;
	    d__1 = pow_di(&sk, &m);
	    z__2.r = d__1 * norm.r, z__2.i = d__1 * norm.i;
/* Computing 2nd power */
	    d__3 = sk;
	    d__2 = exp(d__3 * d__3 * -.5);
	    z__1.r = d__2 * z__2.r, z__1.i = d__2 * z__2.i;
	    daughter[i__2].r = z__1.r, daughter[i__2].i = z__1.i;
/* L50: */
	}
	fourier_factor__ = pi * 2. * sqrt(2. / (m * 2. + 1.));
	*period1 = *scale1 * fourier_factor__;
	*coi1 = fourier_factor__ / sqrt(2.);
    } else {
	s_stop("", (ftnlen)0);
    }
    return 0;
} /* wave_function__ */

/* **************************************************************************** */
/* FACTORIAL: compute the factorial (n!) of an integer n */
/*  Copyright (C) 1998, Christopher Torrence */
integer factorial_(integer *n)
{
    /* System generated locals */
    integer ret_val, i__1;

    /* Local variables */
    integer i__;

    ret_val = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ret_val *= i__;
/* L10: */
    }
    return ret_val;
} /* factorial_ */

/* **************************************************************************** */
/* WAVE_SIGNIF: computes the significance levels for a wavelet transform. */


/* INPUTS: */

/*  isigtest [INT] = 0, 1, or 2. */

/*          If 0, then just do a regular chi-square test, */
/*              i.e. Eqn (18) from Torrence & Compo. */
/*          If 1, then do a "time-average" test, i.e. Eqn (23). */
/*              In this case, DOF(j) should be set to NA, the number */
/*              of local wavelet spectra that were averaged together */
/*              at each scale. For the Global Wavelet Spectrum, */
/*              this would be dof(j)=N-scale(j), */
/*              where N is the number of points in your time series. */
/*          If 2, then do a "scale-average" test, i.e. Eqns (25)-(28). */
/*              In this case, "dof(1)" and "dof(2)" should be set to the */
/*              smallest (S1) and largest (S2) scales that were averaged */
/*              together, respectively. */
/*              e.g. if you scale-averaged scales between 2 and 8, */
/*                   then dof(1)=2.0 and dof(2)=8.0 */


/*  n [INT] = the number of points in "y". */

/*  y [DOUBLE PRECISION] = the time series of length "n". */

/*  dt [DOUBLE PRECISION] = amount of time between each Y value, i.e. the sampling time. */

/*  mother [INT] = An integer giving the mother wavelet to use. */
/*                   0='Morlet' */
/*                   1='Paul' */
/*                   2='DOG' (derivative of Gaussian) */

/*  param [DOUBLE PRECISION] = mother wavelet parameter. */

/*  s0 [DOUBLE PRECISION] = the smallest scale of the wavelet. */

/*  dj [DOUBLE PRECISION] = the spacing between discrete scales. */

/*  jtot [INT] = the # of scales. */

/*  scale [DOUBLE PRECISION(jtot)] = the wavelet scales that were used. */

/*  period [DOUBLE PRECISION(jtot)] = the "Fourier" periods corresponding to "scale". */

/*  lag1 [DOUBLE PRECISION] = lag 1 Autocorrelation, used for signif levels. */
/*              Default is 0.0, which corresponds to white-noise. */

/*  siglvl [DOUBLE PRECISION] = significance level to use. Default is 0.05 (the "5%" level) */

/*  dof [DOUBLE PRECISION(jtot)] = degrees-of-freedom for signif test. */
/*     IF SIGTEST=0, then the input dof is ignored. */
/*     IF SIGTEST=1, then dof(j) = NA, the number of times averaged together. */
/*     IF SIGTEST=2, then dof(1)=S1, dof(2)=S2, the range of scales averaged. */


/* OUTPUTS: */

/*  dof [DOUBLE PRECISION(jtot)] = degrees-of-freedom that were actually used. */
/*     IF SIGTEST=0, then dof(j) = 2 (or 1 for the 'DOG') */
/*     IF SIGTEST=1, then dof(j) = degrees-of-freedom versus scale. */
/*     IF SIGTEST=2, then dof(1)=degrees-of-freedom, dof(2...jtot)=0.0 */

/*  fft_theor [DOUBLE PRECISION(jtot)] = theoretical red-noise spectrum vs scale. */
/*     IF SIGTEST=2, then fft_theor(1) = the average spectrum from S1-->S2 */
/*                   fft_theor(2...jtot) = 0.0 */

/*  signif [DOUBLE PRECISION(jtot)] = significance levels vs scale. */
/*     IF SIGTEST=2, then signif(1) = the significance level */
/*                   signif(2...jtot) = 0.0 */

/*  ymean [DOUBLE PRECISION] = the mean of the time series. */

/*  variance [DOUBLE PRECISION] = the variance of the time series. */

/*  Cdelta [DOUBLE PRECISION] = the constant "Cdelta" for the mother wavelet (Table 2). */

/*  psi0[DOUBLE PRECISION] = the constant 'psi(0)' for the mother wavelet (Table 2) */

/* REQUIRES:   CHISQR */

/*  Copyright (C) 1998, Christopher Torrence and Gilbert P. Compo */
/* This software may be used, copied, or redistributed as long as it is not */
/* sold and this copyright notice is reproduced on each copy made.  This */
/* routine is provided as is without any express or implied warranties */
/* whatsoever. */
/* Subroutine */ int wave_signif__(integer *isigtest, integer *n, doublereal *
	y, doublereal *dt, integer *mother, doublereal *param, doublereal *dj,
	 integer *jtot, doublereal *scale, doublereal *period, doublereal *
	lag1, doublereal *siglvl, doublereal *dof, doublereal *fft_theor__, 
	doublereal *signif, doublereal *ymean, doublereal *variance, 
	doublereal *cdelta, doublereal *psi0)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double atan(doublereal), pow_dd(doublereal *, doublereal *);
    /* Subroutine */ int s_stop(char *, ftnlen);
    double cos(doublereal);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle();
    double sqrt(doublereal), log(doublereal), exp(doublereal);

    /* Local variables */
    doublereal gammafac;
    integer i__, j, m;
    doublereal p, q, fft_theor1__, pi, dj0;
    integer navg;
    doublereal smid, savg;
    integer javg1, javg2;
    doublereal freq1, bound;
    extern /* Subroutine */ int cdfchi_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, integer *, doublereal *);
    doublereal dofmin, chisqr;
    integer status;

    /* Fortran I/O blocks */
    static cilist io___35 = { 0, 6, 0, 0, 0 };
    static cilist io___36 = { 0, 6, 0, 0, 0 };
    static cilist io___39 = { 0, 6, 0, 0, 0 };


    /* Parameter adjustments */
    --y;
    --signif;
    --fft_theor__;
    --dof;
    --period;
    --scale;

    /* Function Body */
    pi = atan(1.) * 4.;
    if (*siglvl <= (float)0.) {
	*siglvl = .05;
    }
    if (*lag1 <= 0.) {
	*lag1 = 0.;
    }
    *cdelta = -1.;
    gammafac = -1.;
    dj0 = -1.;
    *psi0 = -1.;
    if (*mother == 0) {
/* *******************************************   Morlet wavelet */
	dofmin = 2.;
	if (*param == 6.) {
	    *cdelta = .776;
	    gammafac = 2.32;
	    dj0 = .6;
	    *psi0 = pow_dd(&pi, &c_b17);
	}
    } else if (*mother == 1) {
/* *******************************************   Paul wavelet */
	m = (integer) (*param);
	dofmin = 2.;
	if (m == 4) {
	    *cdelta = 1.132;
	    gammafac = 1.17;
	    dj0 = 1.5;
	    *psi0 = 1.079;
	}
    } else if (*mother == 2) {
/* *******************************************   DOG wavelet */
	m = (integer) (*param);
	dofmin = 1.;
	if (m == 2) {
	    *cdelta = 3.541;
	    gammafac = 1.43;
	    dj0 = 1.4;
	    *psi0 = .867;
	} else if (m == 6) {
	    *cdelta = 1.966;
	    gammafac = 1.37;
	    dj0 = .97;
	    *psi0 = .884;
	}
    } else {
	s_stop("", (ftnlen)0);
    }
/* ** find the time-series variance */
    *ymean = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	*ymean += y[i__];
/* L10: */
    }
    *ymean /= *n;
    *variance = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	d__1 = y[i__] - *ymean;
	*variance += d__1 * d__1;
/* L15: */
    }
    *variance /= (doublereal) (*n);
/* ** construct theoretical red(white)-noise power spectrum [Eqn(16)] */
/* - 1.D0) */
    i__1 = *jtot;
    for (j = 1; j <= i__1; ++j) {
	freq1 = *dt / period[j];
/* Computing 2nd power */
	d__1 = *lag1;
/* Computing 2nd power */
	d__2 = *lag1;
	fft_theor__[j] = *variance * (1. - d__1 * d__1) / (1. - *lag1 * 2. * 
		cos(freq1 * 2. * pi) + d__2 * d__2);
/* L20: */
    }
    q = *siglvl;
    p = 1. - q;
    if (*isigtest == 0) {
/* *******************************************   no smoothing, dof=dofmin */
/*   see Eqn(18) */
	i__1 = *jtot;
	for (j = 1; j <= i__1; ++j) {
	    dof[j] = dofmin;
	    d__1 = dofmin;
	    cdfchi_(&c__2, &p, &q, &chisqr, &d__1, &status, &bound);
	    signif[j] = fft_theor__[j] * chisqr / dofmin;
/* L30: */
	}
    } else if (*isigtest == 1) {
/* ***********************************   time-averaged, dof depend on scale */
	if (gammafac <= 0.) {
	    s_wsle(&io___35);
	    do_lio(&c__9, &c__1, "**WAVE_SIGNIF: \"gammafac\" undefined for \
this wavelet", (ftnlen)52);
	    e_wsle();
	    return 0;
	}
	i__1 = *jtot;
	for (j = 1; j <= i__1; ++j) {
	    if (dof[j] < (float)1.) {
		dof[j] = 1.;
	    }
/*   see Eqn(23) */
/* Computing 2nd power */
	    d__1 = dof[j] * *dt / gammafac / scale[j];
	    dof[j] = dofmin * sqrt(d__1 * d__1 + 1.);
	    if (dof[j] < dofmin) {
		dof[j] = dofmin;
	    }
	    d__1 = dof[j];
	    cdfchi_(&c__2, &p, &q, &chisqr, &d__1, &status, &bound);
	    signif[j] = fft_theor__[j] * chisqr / dof[j];
/* L40: */
	}
    } else if (*isigtest == 2) {
/* ***********************************   scale-averaged, dof depend on scale */
	if (*cdelta <= (float)0.) {
	    s_wsle(&io___36);
	    do_lio(&c__9, &c__1, "**WAVE_SIGNIF: \"Cdelta\" and \"dj0\" unde\
fined for this wavelet", (ftnlen)60);
	    e_wsle();
	    return 0;
	}
	javg1 = 0;
	javg2 = 0;
	i__1 = *jtot;
	for (j = 1; j <= i__1; ++j) {
	    if (scale[j] >= dof[1] && javg1 == 0) {
		javg1 = j;
	    }
	    if (scale[j] <= dof[2]) {
		javg2 = j;
	    }
/* L50: */
	}
	if (javg1 == 0 || javg2 == 0 || javg1 > javg2) {
	    s_wsle(&io___39);
	    do_lio(&c__9, &c__1, "**WAVE_SIGNIF: Scales in \"dof(1)\" & \"do\
f(2)\" are out of range.", (ftnlen)62);
	    e_wsle();
	    return 0;
	}
	navg = javg2 - javg1 + 1;
/*   see Eqn(25) */
	savg = 0.;
	i__1 = javg2;
	for (j = javg1; j <= i__1; ++j) {
	    savg += 1. / scale[j];
/* L60: */
	}
	savg = 1. / savg;
/*   see Eqn(27) */
	fft_theor1__ = 0.;
	i__1 = javg2;
	for (j = javg1; j <= i__1; ++j) {
	    fft_theor1__ += fft_theor__[j] / scale[j];
/* L70: */
	}
	fft_theor__[1] = savg * fft_theor1__;
/*   see Eqn(28) */
	smid = exp((log(scale[javg1]) + log(scale[javg2])) * .5);
/* Computing 2nd power */
	d__1 = navg * *dj / dj0;
	dof[1] = dofmin * navg * savg / smid * sqrt(d__1 * d__1 + 1);
/*   see Eqn(26) */
	d__1 = dof[1];
	cdfchi_(&c__2, &p, &q, &chisqr, &d__1, &status, &bound);
	signif[1] = *dj * *dt / *cdelta / savg * fft_theor__[1] * chisqr / 
		dof[1];
	i__1 = *jtot;
	for (j = 2; j <= i__1; ++j) {
	    dof[j] = 0.;
	    fft_theor__[j] = 0.;
	    signif[j] = 0.;
/* L80: */
	}
    } else {
	s_stop("", (ftnlen)0);
    }
    return 0;
} /* wave_signif__ */

#ifdef __cplusplus
	}
#endif
