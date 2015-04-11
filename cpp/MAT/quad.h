#ifndef QUAD_H
#define QUAD_H


#include "lib\std\util.h"
//#include <stdlib.h>
//#include <ostream.h>
#include <float.h>
#include <math.h>
#include "lib\std\f2c.h"


#ifdef __cplusplus
extern "C" {
#endif

//doublereal _r1mach_(integer *i);
//
//int	xerror_(char *msg, integer *len, integer *ier,integer *lvl, ftnlen len1);

extern doublereal NashMin_Mach_Epsilon;
//integer s_wsfe(cilist *a);	
//integer e_wsfe(void);
//integer do_fio(ftnint *number, char *ptr, ftnlen len);

#ifdef __cplusplus
	}
#endif


typedef double (*X_func)     (double par);
struct StorageFzeroFunc
{
 StorageFzeroFunc *Prev;
 X_func ArgFunc;
};

enum OscillMethod{ OscillMethod_Euler,OscillMethod_EulerModif,OscillMethod_Overholt} ;
double Qainf_OscilIntInf(X_func f,double From,double FromAssimpt,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err,double Omega,double Gamma,int Method);


double Qawf_FourierIntInf(X_func f,double From,double ErrorAbs,
                  int NumSubInt,double &ResErr,int &Err,double Omega,int ToMulOnCos=1);
double Qawo_FourierIntAB(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err,double Omega,int ToMulOnCos=1);

double Qag_OscilInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

double Qage_OscilInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

double Qagi_InfinInt(X_func f,double From,int Inf,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

double Qagie_InfinInt(X_func f,double From,int Inf,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

double Qags_IntSubInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
                  int MaxIter);

double Fmin(X_func f,double From,double To,double Guess,double AbsErr,double &FuncXVal);


extern int NumberEvaluations; // For Fzero | Fmin ?


//double UncMin(int &Error,int N,double *x_start,double *x_res,double est_sol,
//              double (*FuncClc)(double *x,int n) , double *GradStp=NULL,
//              void (*GradClc)(double *x,double *grad,int n,double &func) =NULL);

//double UncMin(int &Error,int N,double *x_start,double *x_res,double est_sol,
//              double (*FuncClc)(double *x,int n) , double *GradStp=NULL,
//              void (*GradClc)(double *x,double *grad,int n,double &func) =NULL,
//              double *BoundMin=NULL,double *BoundMax=NULL);
///*
double UncMin(int &Error,int N,double *x_start,double *x_res,double est_sol,
              double (*FuncClc)(double *x,int n) , double *GradStp=NULL,
              void (*GradClc)(double *x,double *grad,int n,double &func)=NULL,
              double FunAccur=1e-9,double ResErr=1e-4,int MaxFun=150,int MaxIter=5,
              double *BoundMin=NULL,double *BoundMax=NULL,
              double MaxLineSearchStp=10,
              double LineSearchEta=0.25);
//*/
//extern  doublereal NashMin_Mach_Epsilon=2e-16;

#endif








/*
      subroutine qawf(f,a,omega,integr,epsabs,result,abserr,neval,ier,
     *   limlst,lst,leniw,maxp1,lenw,iwork,work)
c***begin prologue  qawf
c***date written   800101   (yymmdd)
c***revision date  830518   (yymmdd)
c***category no.  h2a3a1
c***keywords  automatic integrator, special-purpose,fourier
c             integral, integration between zeros with dqawoe,
c             convergence acceleration with dqext
c***author  piessens,robert ,appl. math. & progr. div. - k.u.leuven
c           de doncker,elise,appl. math & progr. div. - k.u.leuven
c***purpose  the routine calculates an approximation result to a given
c            fourier integral
c            i = integral of f(x)*w(x) over (a,infinity)
c            where w(x) = cos(omega*x) or w(x) = sin(omega*x).
c            hopefully satisfying following claim for accuracy
c            abs(i-result).le.epsabs.
c***description
c
c        computation of fourier integrals
c        standard fortran subroutine
c        real version
c
c
c        parameters
c         on entry
c            f      - real
c                     function subprogram defining the integrand
c                     function f(x). the actual name for f needs to be
c                     declared e x t e r n a l in the driver program.
c
c            a      - real
c                     lower limit of integration
c
c            omega  - real
c                     parameter in the integrand weight function
c
c            integr - integer
c                     indicates which of the weight functions is used
c                     integr = 1      w(x) = cos(omega*x)
c                     integr = 2      w(x) = sin(omega*x)
c                     if integr.ne.1.and.integr.ne.2, the routine
c                     will end with ier = 6.
c
c            epsabs - real
c                     absolute accuracy requested, epsabs.gt.0.
c                     if epsabs.le.0, the routine will end with ier = 6.
c
c         on return
c            result - real
c                     approximation to the integral
c
c            abserr - real
c                     estimate of the modulus of the absolute error,
c                     which should equal or exceed abs(i-result)
c
c            neval  - integer
c                     number of integrand evaluations
c
c            ier    - integer
c                     ier = 0 normal and reliable termination of the
c                             routine. it is assumed that the requested
c                             accuracy has been achieved.
c                     ier.gt.0 abnormal termination of the routine.
c                             the estimates for integral and error are
c                             less reliable. it is assumed that the
c                             requested accuracy has not been achieved.
c            error messages
c                    if omega.ne.0
c                     ier = 1 maximum number of cycles allowed
c                             has been achieved, i.e. of subintervals
c                             (a+(k-1)c,a+kc) where
c                             c = (2*int(abs(omega))+1)*pi/abs(omega),
c                             for k = 1, 2, ..., lst.
c                             one can allow more cycles by increasing
c                             the value of limlst (and taking the
c                             according dimension adjustments into
c                             account). examine the array iwork which
c                             contains the error flags on the cycles, in
c                             order to look for eventual local
c                             integration difficulties.
c                             if the position of a local difficulty
c                             can be determined (e.g. singularity,
c                             discontinuity within the interval) one
c                             will probably gain from splitting up the
c                             interval at this point and calling
c                             appropriate integrators on the subranges.
c                         = 4 the extrapolation table constructed for
c                             convergence accelaration of the series
c                             formed by the integral contributions over
c                             the cycles, does not converge to within
c                             the requested accuracy.
c                             as in the case of ier = 1, it is advised
c                             to examine the array iwork which contains
c                             the error flags on the cycles.
c                         = 6 the input is invalid because
c                             (integr.ne.1 and integr.ne.2) or
c                              epsabs.le.0 or limlst.lt.1 or
c                              leniw.lt.(limlst+2) or maxp1.lt.1 or
c                              lenw.lt.(leniw*2+maxp1*25).
c                              result, abserr, neval, lst are set to
c                              zero.
c                         = 7 bad integrand behaviour occurs within
c                             one or more of the cycles. location and
c                             type of the difficulty involved can be
c                             determined from the first lst elements of
c                             vector iwork.  here lst is the number of
c                             cycles actually needed (see below).
c                             iwork(k) = 1 the maximum number of
c                                          subdivisions (=(leniw-limlst)
c                                          /2) has been achieved on the
c                                          k th cycle.
c                                      = 2 occurrence of roundoff error
c                                          is detected and prevents the
c                                          tolerance imposed on the k th
c                                          cycle, from being achieved
c                                          on this cycle.
c                                      = 3 extremely bad integrand
c                                          behaviour occurs at some
c                                          points of the k th cycle.
c                                      = 4 the integration procedure
c                                          over the k th cycle does
c                                          not converge (to within the
c                                          required accuracy) due to
c                                          roundoff in the extrapolation
c                                          procedure invoked on this
c                                          cycle. it is assumed that the
c                                          result on this interval is
c                                          the best which can be
c                                          obtained.
c                                      = 5 the integral over the k th
c                                          cycle is probably divergent
c                                          or slowly convergent. it must
c                                          be noted that divergence can
c                                          occur with any other value of
c                                          iwork(k).
c                    if omega = 0 and integr = 1,
c                    the integral is calculated by means of dqagie,
c                    and ier = iwork(1) (with meaning as described
c                    for iwork(k),k = 1).
c
c         dimensioning parameters
c            limlst - integer
c                     limlst gives an upper bound on the number of
c                     cycles, limlst.ge.3.
c                     if limlst.lt.3, the routine will end with ier = 6.
c
c            lst    - integer
c                     on return, lst indicates the number of cycles
c                     actually needed for the integration.
c                     if omega = 0, then lst is set to 1.
c
c            leniw  - integer
c                     dimensioning parameter for iwork. on entry,
c                     (leniw-limlst)/2 equals the maximum number of
c                     subintervals allowed in the partition of each
c                     cycle, leniw.ge.(limlst+2).
c                     if leniw.lt.(limlst+2), the routine will end with
c                     ier = 6.
c
c            maxp1  - integer
c                     maxp1 gives an upper bound on the number of
c                     chebyshev moments which can be stored, i.e. for
c                     the intervals of lengths abs(b-a)*2**(-l),
c                     l = 0,1, ..., maxp1-2, maxp1.ge.1.
c                     if maxp1.lt.1, the routine will end with ier = 6.
c            lenw   - integer
c                     dimensioning parameter for work
c                     lenw must be at least leniw*2+maxp1*25.
c                     if lenw.lt.(leniw*2+maxp1*25), the routine will
c                     end with ier = 6.
c
c         work arrays
c            iwork  - integer
c                     vector of dimension at least leniw
c                     on return, iwork(k) for k = 1, 2, ..., lst
c                     contain the error flags on the cycles.
c
c            work   - real
c                     vector of dimension at least
c                     on return,
c                     work(1), ..., work(lst) contain the integral
c                      approximations over the cycles,
c                     work(limlst+1), ..., work(limlst+lst) contain
c                      the error extimates over the cycles.
c                     further elements of work have no specific
c                     meaning for the user.
c

*/