/* wavetest.f -- translated by f2c (version 20060506).
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

static doublereal c_b1 = 2.;
static integer c__5 = 5;
static integer c__504 = 504;
static integer c__1 = 1;
static doublereal c_b10 = .25;
static integer c__44 = 44;
static integer c__9 = 9;
static integer c__3 = 3;

/* **************************************************************************** */
/* WAVETEST: Example Fortran program for WAVELET, using NINO3 SST dataset */

/* COMPILE:   f77 chisqr.f cfftpack.f wavelet.f wavetest.f */

/* See "http://paos.colorado.edu/research/wavelets/" */

/*  Copyright (C) 1998, Christopher Torrence and Gilbert P. Compo */
/* This software may be used, copied, or redistributed as long as it is not */
/* sold and this copyright notice is reproduced on each copy made.  This */
/* routine is provided as is without any express or implied warranties */
/* whatsoever. */

/* Modified: November 1999 by Arjan van Dijk to include IMPLICIT NONE and */
/*           to convert all routines to DOUBLE precision. */
/* **************************************************************************** */
/* Main program */ int MAIN__()
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3;
    cilist ci__1;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    double atan(doublereal), log(doublereal);
    integer i_dnnt(doublereal *);
    double pow_di(doublereal *, integer *);
    integer f_open(olist *), s_rsle(cilist *), do_lio(integer *, integer *, 
	    char *, ftnlen), e_rsle(), f_clos(cllist *), s_wsfe(cilist *), 
	    do_fio(integer *, char *, ftnlen), e_wsfe();
    double z_abs(doublecomplex *);
    integer s_wsle(cilist *), e_wsle();
    double d_imag(doublecomplex *), atan2(doublereal, doublereal), sqrt(
	    doublereal);

    /* Local variables */
    doublereal variance, savg_dof__[44];
    integer isigtest, i__, j;
    doublereal global_ws__[44], fft_theor__[44], recon_sst__[504], pi, 
	    recon_mean__, recon_vari__, dof[44], coi[504], sst[504], lag1, 
	    savg_signif__[44];
    extern /* Subroutine */ int wave_signif__(integer *, integer *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     integer *, doublereal *, doublereal *, doublereal *, doublereal *
	    , doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    doublereal psi0;
    integer npad;
    doublecomplex wave[22176]	/* was [504][44] */;
    integer javg1, javg2;
    doublereal scale[44], param, ymean, global_signif__[44];
    integer ibase2;
    doublereal cdelta, signif[44], period[44];
    integer mother;
    doublereal siglvl;
    extern /* Subroutine */ int wavelet_(integer *, doublereal *, doublereal *
	    , integer *, doublereal *, doublereal *, doublereal *, integer *, 
	    integer *, doublecomplex *, doublereal *, doublereal *, 
	    doublereal *);
    doublereal sstenso[504];

    /* Fortran I/O blocks */
    static cilist io___6 = { 0, 11, 0, 0, 0 };
    static cilist io___31 = { 0, 6, 0, 0, 0 };
    static cilist io___32 = { 0, 6, 0, 0, 0 };
    static cilist io___33 = { 0, 6, 0, 0, 0 };
    static cilist io___34 = { 0, 6, 0, 0, 0 };
    static cilist io___35 = { 0, 6, 0, 0, 0 };
    static cilist io___36 = { 0, 6, 0, 0, 0 };
    static cilist io___37 = { 0, 6, 0, 0, 0 };
    static cilist io___38 = { 0, 6, 0, 0, 0 };


/* these parameters depend on the particular time series */
/* Note: for accurate reconstruction and wavelet-derived variance */
/*     do not pad with zeroes, set s0=dt (for Paul set s0=dt/4), and use */
/*     a large "jtot" (even though the extra scales will be within */
/*     the cone of influence). */
/*     For plotting purposes, it is only necessary to use */
/*     s0=2dt (for Morlet) and "jtot" from Eqn(10) Torrence&Compo(1998). */
    pi = atan(1.) * 4.;
    d__1 = log(504.) / log(2.);
    ibase2 = i_dnnt(&d__1) + 1;
    npad = (integer) pow_di(&c_b1, &ibase2);
/*      npad = n  ! this is for no padding with zeroes */
/* *************************************************** Wavelet transform */
/* ** let the WAVELET subroutine choose the defaults for these: */
    mother = 0;
    param = 6.;
/* ** read in the NINO3 SST data */
    o__1.oerr = 0;
    o__1.ounit = 11;
    o__1.ofnmlen = 13;
    o__1.ofnm = "sst_nino3.dat";
    o__1.orl = 0;
    o__1.osta = "old";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    s_rsle(&io___6);
    do_lio(&c__5, &c__504, (char *)&sst[0], (ftnlen)sizeof(doublereal));
    e_rsle();
    cl__1.cerr = 0;
    cl__1.cunit = 11;
    cl__1.csta = 0;
    f_clos(&cl__1);
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(/,\"sst(1)=\",F6.2,\"  sst(n) = \",F6.2,/)";
    s_wsfe(&ci__1);
    do_fio(&c__1, (char *)&sst[0], (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sst[503], (ftnlen)sizeof(doublereal));
    e_wsfe();
/* ** get the wavelet transform */
    wavelet_(&c__504, sst, &c_b10, &mother, &param, &c_b10, &c_b10, &c__44, &
	    npad, wave, scale, period, coi);
/* *************************************************** Significance testing */
/* ** local significance test */
    isigtest = 0;
    lag1 = .72;
    siglvl = .05;
    wave_signif__(&isigtest, &c__504, sst, &c_b10, &mother, &param, &c_b10, &
	    c__44, scale, period, &lag1, &siglvl, dof, fft_theor__, signif, &
	    ymean, &variance, &cdelta, &psi0);
/* ** global wavelet spectrum & significance test */
    isigtest = 1;
    lag1 = .72;
    siglvl = .05;
    for (j = 1; j <= 44; ++j) {
	for (i__ = 1; i__ <= 504; ++i__) {
/* Computing 2nd power */
	    d__1 = z_abs(&wave[i__ + j * 504 - 505]);
	    global_ws__[j - 1] += d__1 * d__1;
/* L20: */
	}
	global_ws__[j - 1] /= 504;
	dof[j - 1] = 504 - scale[j - 1];
/* L10: */
    }
    wave_signif__(&isigtest, &c__504, sst, &c_b10, &mother, &param, &c_b10, &
	    c__44, scale, period, &lag1, &siglvl, dof, fft_theor__, 
	    global_signif__, &ymean, &variance, &cdelta, &psi0);
/* ** scale-average time series & significance test */
    isigtest = 2;
    lag1 = .72;
    siglvl = .05;
/*    scale average between 2 and 7.9 years */
    savg_dof__[0] = 2.;
    savg_dof__[1] = 7.9;
/*    find the "j"-values that correspond to savg_dof(1) & savg_dof(2) */
    javg1 = 0;
    javg2 = 0;
    for (j = 1; j <= 44; ++j) {
	if (scale[j - 1] >= savg_dof__[0] && javg1 == 0) {
	    javg1 = j;
	}
	if (scale[j - 1] <= savg_dof__[1]) {
	    javg2 = j;
	}
/* L30: */
    }
/*   call wave_signif first, to get the value of "Cdelta" */
    wave_signif__(&isigtest, &c__504, sst, &c_b10, &mother, &param, &c_b10, &
	    c__44, scale, period, &lag1, &siglvl, savg_dof__, fft_theor__, 
	    savg_signif__, &ymean, &variance, &cdelta, &psi0);
/*   construct the scale-averaged time series [Eqn(24)] */
    for (i__ = 1; i__ <= 504; ++i__) {
	sstenso[i__ - 1] = 0.;
	i__1 = javg2;
	for (j = javg1; j <= i__1; ++j) {
/* Computing 2nd power */
	    d__1 = z_abs(&wave[i__ + j * 504 - 505]);
	    sstenso[i__ - 1] += d__1 * d__1 / scale[j - 1];
/* L60: */
	}
	sstenso[i__ - 1] = sstenso[i__ - 1] * .0625 / cdelta;
/* L50: */
    }
/* ************************************************************* print results */
    s_wsle(&io___31);
    do_lio(&c__9, &c__1, " n=", (ftnlen)3);
    do_lio(&c__3, &c__1, (char *)&c__504, (ftnlen)sizeof(integer));
    e_wsle();
    s_wsle(&io___32);
    do_lio(&c__9, &c__1, " dt=", (ftnlen)4);
    do_lio(&c__5, &c__1, (char *)&c_b10, (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___33);
    do_lio(&c__9, &c__1, " mother=", (ftnlen)8);
    do_lio(&c__3, &c__1, (char *)&mother, (ftnlen)sizeof(integer));
    e_wsle();
    s_wsle(&io___34);
    do_lio(&c__9, &c__1, " param=", (ftnlen)7);
    do_lio(&c__5, &c__1, (char *)&param, (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___35);
    do_lio(&c__9, &c__1, " s0=", (ftnlen)4);
    do_lio(&c__5, &c__1, (char *)&c_b10, (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___36);
    do_lio(&c__9, &c__1, " dj=", (ftnlen)4);
    do_lio(&c__5, &c__1, (char *)&c_b10, (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsle(&io___37);
    do_lio(&c__9, &c__1, " jtot=", (ftnlen)6);
    do_lio(&c__3, &c__1, (char *)&c__44, (ftnlen)sizeof(integer));
    e_wsle();
    s_wsle(&io___38);
    do_lio(&c__9, &c__1, " npad=", (ftnlen)6);
    do_lio(&c__3, &c__1, (char *)&npad, (ftnlen)sizeof(integer));
    e_wsle();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(/,\"Let w = wave(n/2,j)\",/)";
    s_wsfe(&ci__1);
    e_wsfe();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(A4,7A10)";
    s_wsfe(&ci__1);
    do_fio(&c__1, "j", (ftnlen)1);
    do_fio(&c__1, "Scale", (ftnlen)5);
    do_fio(&c__1, "Period", (ftnlen)6);
    do_fio(&c__1, "ABS(w)^2", (ftnlen)8);
    do_fio(&c__1, "phase(w)", (ftnlen)8);
    do_fio(&c__1, "5%signif", (ftnlen)8);
    do_fio(&c__1, "Global", (ftnlen)6);
    do_fio(&c__1, "GWS5%sig", (ftnlen)8);
    e_wsfe();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(I4,7F10.3)";
    s_wsfe(&ci__1);
    for (j = 1; j <= 44; ++j) {
	do_fio(&c__1, (char *)&j, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&scale[j - 1], (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&period[j - 1], (ftnlen)sizeof(doublereal));
/* Computing 2nd power */
	d__2 = z_abs(&wave[j * 504 - 253]);
	d__1 = d__2 * d__2;
	do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	d__3 = atan2(d_imag(&wave[j * 504 - 253]), (doublereal) wave[j * 504 
		- 253].r) * 180. / pi;
	do_fio(&c__1, (char *)&d__3, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&signif[j - 1], (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&global_ws__[j - 1], (ftnlen)sizeof(doublereal))
		;
	do_fio(&c__1, (char *)&global_signif__[j - 1], (ftnlen)sizeof(
		doublereal));
    }
    e_wsfe();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(/,A,F10.3)";
    s_wsfe(&ci__1);
    do_fio(&c__1, " Scale-average degrees of freedom = ", (ftnlen)36);
    do_fio(&c__1, (char *)&savg_dof__[0], (ftnlen)sizeof(doublereal));
    e_wsfe();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(A,F10.3,/)";
    s_wsfe(&ci__1);
    do_fio(&c__1, " Scale-avg 5% significance level  = ", (ftnlen)36);
    do_fio(&c__1, (char *)&savg_signif__[0], (ftnlen)sizeof(doublereal));
    e_wsfe();
/* ************************************************************ Reconstruction */
/* ** construct the wavelet derived variance (Parseval's theorem)  [Eqn(14)] */
/*   Cdelta & psi0 are returned from WAVE_SIGNIF */
    recon_vari__ = 0.;
    for (i__ = 1; i__ <= 504; ++i__) {
	for (j = 1; j <= 44; ++j) {
/* Computing 2nd power */
	    d__1 = z_abs(&wave[i__ + j * 504 - 505]);
	    recon_vari__ += d__1 * d__1 / scale[j - 1];
/* L1000: */
	}
/* L900: */
    }
    recon_vari__ = recon_vari__ * .0625 / (cdelta * 504);
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(A,F14.5)";
    s_wsfe(&ci__1);
    do_fio(&c__1, " Reconstructed variance=", (ftnlen)24);
    do_fio(&c__1, (char *)&recon_vari__, (ftnlen)sizeof(doublereal));
    e_wsfe();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(A,F14.5)";
    s_wsfe(&ci__1);
    do_fio(&c__1, " Original variance   =", (ftnlen)22);
    do_fio(&c__1, (char *)&variance, (ftnlen)sizeof(doublereal));
    e_wsfe();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(A,F14.5,A,/)";
    s_wsfe(&ci__1);
    do_fio(&c__1, " Ratio = ", (ftnlen)9);
    d__1 = recon_vari__ / variance;
    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, " (this is low due to padding with zeroes)", (ftnlen)41);
    e_wsfe();
/* ** reconstruct the time series [Eqn(11)] */
/*   check mean and RMS difference of reconstructed time series */
    recon_mean__ = 0.;
    recon_vari__ = 0.;
    for (i__ = 1; i__ <= 504; ++i__) {
	recon_sst__[i__ - 1] = 0.;
	for (j = 1; j <= 44; ++j) {
	    i__1 = i__ + j * 504 - 505;
	    recon_sst__[i__ - 1] += wave[i__1].r / sqrt(scale[j - 1]);
/* L1200: */
	}
	recon_sst__[i__ - 1] = sqrt(.25) * .25 * recon_sst__[i__ - 1] / (
		cdelta * psi0);
/* Computing 2nd power */
	d__1 = sst[i__ - 1] - ymean - recon_sst__[i__ - 1];
	recon_vari__ += d__1 * d__1;
	recon_mean__ += recon_sst__[i__ - 1];
/* L1100: */
    }
    recon_mean__ /= 504;
    recon_vari__ = sqrt(recon_vari__ / 504);
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(A,F14.6)";
    s_wsfe(&ci__1);
    do_fio(&c__1, " Reconstructed mean=", (ftnlen)20);
    do_fio(&c__1, (char *)&recon_mean__, (ftnlen)sizeof(doublereal));
    e_wsfe();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(A,F14.6)";
    s_wsfe(&ci__1);
    do_fio(&c__1, " Original mean   =", (ftnlen)18);
    do_fio(&c__1, (char *)&ymean, (ftnlen)sizeof(doublereal));
    e_wsfe();
    ci__1.cierr = 0;
    ci__1.ciunit = 6;
    ci__1.cifmt = "(A,F14.6,/)";
    s_wsfe(&ci__1);
    do_fio(&c__1, " Root-mean-square difference of time series=", (ftnlen)44);
    do_fio(&c__1, (char *)&recon_vari__, (ftnlen)sizeof(doublereal));
    e_wsfe();
    return 0;
} /* MAIN__ */

/* Main program alias */ int wavetest_ () { MAIN__ (); return 0; }
#ifdef __cplusplus
	}
#endif
