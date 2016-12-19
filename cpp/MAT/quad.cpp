#include <lib/precompiled/math.h>

#include "quad.h"



X_func FuncStatic;
StorageFzeroFunc *FuncStoragePrev=NULL;

int NumberEvaluations;
#ifdef __cplusplus
extern "C" {
#endif
//#include "f2c.h"

extern int sqainf_(integer *numfun, U_fp funsub, real *period, real 
	*gamma, real *a, real *b, integer *wrksub, integer *emax, integer *
	minpts, integer *maxpts, real *epsabs, real *epsrel, integer *nw, 
	integer *key, integer *restar, real *result, real *abserr, integer *
	neval, integer *ifail, logical *done, real *work, integer *iwork);

extern int qawf_(E_fp f,real *a,real *omega,integer *integr,real *epsabs,real *result,real *abserr,integer *neval,
                integer *ier,integer *limlst,integer *lst,integer *leniw,integer *maxp1,integer *lenw,
                integer *iwork,real *work);
extern int qawo_(E_fp f, real *a, real *b, real *omega, integer *
	integr, real *epsabs, real *epsrel, real *result, real *abserr, 
	integer *neval, integer *ier, integer *leniw, integer *maxp1, integer 
	*lenw, integer *last, integer *iwork, real *work);

extern int qage_(E_fp f, real *a, real *b, real *epsabs, real *
	epsrel, integer *key, integer *limit, real *result, real *abserr, 
	integer *neval, integer *ier, real *alist__, real *blist, real *rlist,
	 real *elist, integer *iord, integer *last);
extern int qag_(E_fp f, real *a, real *b, real *epsabs, real *
	epsrel, integer *key, real *result, real *abserr, integer *neval, 
	integer *ier, integer *limit, integer *lenw, integer *last, integer *
	iwork, real *work);
extern int qagi_(E_fp f, real *bound, integer *inf, real *epsabs, 
	real *epsrel, real *result, real *abserr, integer *neval, integer *
	ier, integer *limit, integer *lenw, integer *last, integer *iwork, 
	real *work);
extern int qagie_(E_fp f, real *bound, integer *inf, real *epsabs, 
	real *epsrel, integer *limit, real *result, real *abserr, integer *
	neval, integer *ier, real *alist__, real *blist, real *rlist, real *
	elist, integer *iord, integer *last);

extern int qags_(E_fp f, real *a, real *b, real *epsabs, real *
	epsrel, real *result, real *abserr, integer *neval, 
	integer *ier, integer *limit, integer *lenw, integer *last, integer *
	iwork, real *work);

extern int fzero_(E_fp f, real *b, real *c__, real *r__, real *re, 
	real *ae, integer *iflag, integer *MaxIter, integer *WasIter);

extern doublereal fmin_(E_fp f, real *x, real *a, real *b, real *t);

extern  int tn_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, 
     int(*)(integer *n, doublereal *x, doublereal *f, doublereal *g)); // Subroutine //
extern  int tnbc_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, 
     int(*)(integer *n, doublereal *x, doublereal *f, doublereal *g),doublereal*,doublereal*,integer*); 

extern int lmqn_(integer *ifail, integer *n, doublereal *x, 
    	doublereal *f, doublereal *g, doublereal *w, integer *lw, S_fp sfun, 
     integer *msglvl, integer *maxit, integer *maxfun, doublereal *eta, 
    	doublereal *stepmx, doublereal *accrcy, doublereal *xtol);

extern int lmqnbc_(integer *ifail, integer *n, doublereal *x, 
	doublereal *f, doublereal *g, doublereal *w, integer *lw, S_fp sfun, 
	doublereal *low, doublereal *up, integer *ipivot, integer *msglvl, 
	integer *maxit, integer *maxfun, doublereal *eta, doublereal *stepmx, 
	doublereal *accrcy, doublereal *xtol);




//integer s_wsfe(cilist *a){return 0;}
//integer e_wsfe(void){return 0;}
//integer do_fio(ftnint *number, char *ptr, ftnlen len){return 0;}





doublereal FuncSub(real *x)  { return FuncStatic(*x);};
int FuncSubNa6(real *x, integer *numfun, integer *np, real *funvls)  
 { 
 for (long k=0;k<*numfun;k++) {for (long k1=0;k1<*np;k1++) funvls[k*(*np)+k1]=FuncStatic(x[k1]);}
  return 0;
 };



double r_sign(real *a, real *b)
{ double x; x = (*a >= 0 ? *a : - *a);return( *b >= 0 ? x : -x);}



//#include <stdio.h>
//#include <float.h>
//#include <math.h>
//doublereal r1mach_(integer *)
#define DBL_MIN_MY      DBL_MIN*2
#define DBL_MAX_MY      DBL_MAX/2
#define DBL_EPSILON_MY  DBL_EPSILON*2
#define DBL_RADIX_MY    DBL_RADIX
//doublereal r1mach_(integer *i)
//{
// real e=DBL_MIN;
// if (e==2*e)
// {
//	 switch(*i){
//	   case 1: return FLT_MIN;
//	   case 2: return FLT_MAX;
// 	  case 3: return FLT_EPSILON/FLT_RADIX;
//	   case 4: return FLT_EPSILON;
//	   case 5: return log10((double)FLT_RADIX);
//             }
// }
//
// else
// {
//	 switch(*i){
//	   case 1: return DBL_MIN;
//	   case 2: return DBL_MAX;
// 	  case 3: return DBL_EPSILON/DBL_RADIX;
//	   case 4: return DBL_EPSILON;
//	   case 5: return log10((double)DBL_RADIX);
///*	   case 1: return DBL_MIN_MY;
//	   case 2: return DBL_MAX_MY;
// 	  case 3: return DBL_EPSILON_MY/DBL_RADIX_MY;
//	   case 4: return DBL_EPSILON_MY;
//	   case 5: return log10((double)DBL_RADIX_MY);
//*/
//             }
// }
//
////	fprintf(stderr, "invalid argument: r1mach(%ld)\n", *i);
//	return 0; /* else complaint of missing return value */
//}
int	xerror_(char *msg, integer *len, integer *ier,integer *lvl,ftnlen len1){
     fprintf(stderr,"%s  \nError { Num:  %ld   Level:  %ld } \n", msg,*ier,*lvl);
     return 0;
}
//double pow_dd(double *x, double *y)
//  {
//   return pow(*x,*y);
//  };

//integer pow_ii(integer *ap, integer *bp)
//{
//	integer pow, x, n;
//	unsigned long u;
//
//	x = *ap;
//	n = *bp;
//
//	if (n <= 0) {if (n == 0 || x == 1)		return 1;
//		            if (x != -1)	return x == 0 ? 1/x : 0;
//		            n = -n;	}
//	u = n;
//	for(pow = 1; ; )
//		{	if(u & 01)		pow *= x;
//		  if(u >>= 1)	x *= x;
//		  else		break;	}
//	return(pow);
//	}

#ifdef __cplusplus
	}
#endif


int CheckLim(double &From,double &To,double &ResErr,int &Err,int &Dir)
  {
   Dir=1;
   if (From==To) {ResErr=0;Err=0;return 1;}
   if (From>To) { double tmp=From;From=To;To=tmp;Dir=-1;}
   return 0;
  };




// =========================================================================================
// =====================   New Addition     ================================================

double Qainf_OscilIntInf(X_func f,double From,double FromAssimpt,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err,double Omega,double Gamma,int Method)
 {
  integer numfun=1;
  real period=Omega,gamma=Gamma;
  integer wrksub=NumSubInt/21+1;
  real *a=new real[wrksub], *b=new real[wrksub];a[0]=From;b[0]=FromAssimpt;
//     EMAX    The maximum number of extrapolation steps. 
//     MINPTS  Minimum number of function evaluations. 
//     MAXPTS  Maximum number of function evaluations. The number of function evaluations over 
//             each sub-interval  is 21.
  integer emax=500,minpts=30,maxpts=NumSubInt;
  real epsabs=ErrorAbs,epsrel=ErrorRel;
//     NW      Defines the length of the working array WORK. 
  integer nw;//=3*NumSubInt;
  nw = numfun * 5 + 1 + (wrksub << 2) * numfun + wrksub * 3 + (emax + 5) * (emax + 5) + numfun * 38;
//     KEY     Choice of extrapolation method:
//                KEY = 0 : Euler's method. */
//                KEY = 1 : Modified Euler. */
//                KEY = 2 : Overholt's polynomial order 2-method. This last method is the only 
//                          one that needs the value of GAMMA. 
  integer key,restar=0;key=(Method==OscillMethod_Euler)?0:(Method==OscillMethod_EulerModif)?1:2;
  real result[3], abserr=0;
  integer neval, ifail;
  logical done;
//     WORK   Real array of dimension NW. 
  real *work=new real[nw];
//     IWORK  Integer array of dimension 3*WRKSUB. 
  integer *iwork=new integer[3*wrksub];

   X_func tmpFunc=FuncStatic;FuncStatic=f;
//  int ret=
   sqainf_(&numfun, reinterpret_cast<U_fp>( &FuncSubNa6), &period, &gamma, a, b, &wrksub,
                  &emax, &minpts, &maxpts, &epsabs, &epsrel, &nw, &key, &restar, 
                  &result[0], &abserr, &neval, &ifail, &done, work, iwork);
  FuncStatic=tmpFunc;
  delete []work;delete []iwork;delete []a;delete []b;
  if (ifail) {cout<<" Qainf_OscilIntInf : error "<<ifail<<" ResErr "<<abserr<<" NumEval "<<neval<<"\n";cout.flush();};
  Err=ifail;ResErr=abserr;NumberEvaluations=neval;
//cout<<" Oscil res "<<result[0]<<" neval "<<neval<<"\n";
  return result[0];
 }
//   IFAIL = 0 for normal exit, when  ABSERR(J) <=  EPSABS or ABSERR(J) <=  ABS(RESULT(J))*EPSREL 
//             with MAXPTS or less function evaluations for all values of J, from 1 to NUMFUN. 
//   IFAIL = +1 if MAXPTS was too small to obtain the required accuracy. In this case SQAINF 
//              returns values of RESULT with estimated absolute  errors ABSERR. 
//   IFAIL = -1 if EMAX was too small to obtain the required accuracy. In this case SQAINF 
//              returns values of RESULT with estimated absolute errors ABSERR. 
//   IFAIL =  2 if NUMFUN is less than 1. 
//   IFAIL =  3 if B(1) < A(1). 
//   IFAIL =  4 if unlegal PERIOD. 
//   IFAIL =  5 if MAXPTS is less than MINPTS or MINPTS < 21. 
//   IFAIL =  6 if EPSABS <= 0 and EPSREL <= 0. 
//   IFAIL =  7 if WRKSUB is too small. 
//   IFAIL =  8 if unlegal value of EMAX. 
//   IFAIL =  9 if unlegal RESTAR. 
//   IFAIL = 10 if unlegal value of KEY. 
//   IFAIL = 11 if NW is less than LIMIT (See the code DCHINF). 
//   IFAIL = 12 if either PERIOD is wrong or B(1) is too small. 
//   IFAIL = 13 if unable to estimate GAMMA (In case KEY=2 only) 




//    subroutine qawf(f,a,omega,integr,epsabs,result,abserr,neval,ier,
//     *   limlst,lst,leniw,maxp1,lenw,iwork,work)
// =========================================================================================

double Qawf_FourierIntInf(X_func f,double From,double ErrorAbs,
                  int NumSubInt,double &ResErr,int &Err,double Omega,int ToMulOnCos)
  {
   real a=From,epsabs=ErrorAbs,abserr,result,omega=Omega;
   integer integr=(ToMulOnCos)?1:2,neval,ier;

   integer limlst,lst,leniw,lenw,maxp1,*iwork;
   real *work;
   X_func tmpFunc=FuncStatic;
   FuncStatic=f;

   limlst=NumSubInt;
//  limlst -  limlst gives an upper bound on the number of  cycles, limlst.ge.3.
//            if limlst.lt.3, the routine will end with ier = 6.

//  lst    -  on return, lst indicates the number of cycles actually needed for the integration.
   leniw=3*limlst;
//  leniw  -  dimensioning parameter for iwork. on entry, (leniw-limlst)/2 equals the maximum number of
//            subintervals allowed in the partition of each cycle, leniw.ge.(limlst+2).
   maxp1=50;
//  maxp1  -  maxp1 gives an upper bound on the number of chebyshev moments which can be stored, i.e. for
//            the intervals of lengths abs(b-a)*2**(-l), l = 0,1, ..., maxp1-2, maxp1.ge.1.
   lenw=leniw*2+maxp1*25;
//  lenw   -  dimensioning parameter for work lenw must be at least leniw*2+maxp1*25.
   iwork=new integer[leniw+1];
//  iwork  -  vector of dimension at least leniw on return, iwork(k) for k = 1, 2, ..., lst
//            contain the error flags on the cycles.
   work=new real[lenw+1];
//  work   -  vector of dimension at least leniw  on return, work(1), ..., work(lst) contain the integral
//            approximations over the cycles, work(limlst+1), ..., work(limlst+lst) contain the error 
//            extimates over the cycles. further elements of work have no specific meaning for the user.
   



   qawf_(&FuncSub,&a,&omega,&integr,&epsabs,&result,&abserr,&neval,&ier,
                   &limlst,&lst,&leniw,&maxp1,&lenw,iwork,work);
   FuncStatic=tmpFunc;
   delete iwork;delete work;
   if (ier!=0) {cout<<" Qawf_FourierIntInf : error "<<ier<<" ResErr "<<abserr<<"\n";cout.flush();};
   ResErr=abserr;Err=ier;NumberEvaluations=neval;
   return result;
// neval  -  number of integrand evaluations 

//  ier = 0 normal and reliable termination of the routine. 
//  ier.gt.0 abnormal termination of the routine requested accuracy has not been achieved. 
//  ier = 1 maximum number of subdivisions allowed has been achieved. 
//      = 4 the extrapolation table constructed for convergence accelaration of the series
//          formed by the integral contributions over the cycles, does not converge to within
//          the requested accuracy. as in the case of ier = 1, it is advised to examine the array iwork 
//          which contains the error flags on the cycles.
//      = 6 the input is invalid because (integr.ne.1 and integr.ne.2) or epsabs.le.0 or limlst.lt.1 or
//          leniw.lt.(limlst+2) or maxp1.lt.1 or lenw.lt.(leniw*2+maxp1*25). result, abserr, neval, lst 
//          are set to zero.
//      = 7 bad integrand behaviour occurs within one or more of the cycles. location and type of the 
//          difficulty involved can be determined from the first lst elements of vector iwork.  here lst 
//          is the number of  cycles actually needed (see below). iwork(k) = 1 the maximum number of
//          subdivisions (=(leniw-limlst)/2) has been achieved on the  k th cycle. = 2 occurrence of roundoff 
//          error is detected and prevents the tolerance imposed on the k th cycle, from being achieved
//          on this cycle. = 3 extremely bad integrand behaviour occurs at some points of the k th cycle.
//          = 4 the integration procedure over the k th cycle does not converge (to within the required 
//          accuracy) due to roundoff in the extrapolation procedure invoked on this cycle. it is assumed 
//          that the result on this interval is the best which can be obtained. = 5 the integral over the k th
//          cycle is probably divergent or slowly convergent. it must be noted that divergence can
//          occur with any other value of iwork(k).

// last  - on return, last equals the number of subintervals produced in the 
//         subdivision process, which determines the number of significant elements 
//         actually in the work arrays. */
// abserr - estimate of the modulus of the absolute error, 
//          which should equal or exceed abs(i-result) 
}


// Was
     //    subroutine qawf(f,a,omega,integr,epsabs,result,abserr,neval,ier,
//     *   limlst,lst,leniw,maxp1,lenw,iwork,work)
//epsrel, last - added
//limlst,lst, - deleted
// subroutine dqawo(f,a,b,omega,integr,epsabs,epsrel,result,abserr,
     //*   neval,ier,leniw,maxp1,lenw,last,iwork,work)
// =========================================================================================

//double Qawf_FourierIntInf(X_func f,double From,double ErrorAbs,double ErrorRel,
//                  int NumSubInt,double &ResErr,int &Err,double Omega,int ToMulOnCos)
double Qawo_FourierIntAB(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err,double Omega,int ToMulOnCos)
  {
   real a=From,b=To,epsabs=ErrorAbs,epsrel=ErrorRel,abserr,result,omega=Omega;
   integer integr=(ToMulOnCos)?1:2,neval,ier;

   integer leniw,lenw,maxp1,*iwork,last;
   real *work;
   X_func tmpFunc=FuncStatic;
   FuncStatic=f;

   leniw=NumSubInt;
//  leniw  -  dimensioning parameter for iwork. on entry, leniw/2 equals the maximum number of
//            subintervals allowed in the partition of each cycle, leniw.ge.(2).
   maxp1=50;
//  maxp1  -  maxp1 gives an upper bound on the number of chebyshev moments which can be stored, i.e. for
//            the intervals of lengths abs(b-a)*2**(-l), l = 0,1, ..., maxp1-2, maxp1.ge.1.
   lenw=leniw*2+maxp1*25;
//  lenw   -  dimensioning parameter for work lenw must be at least leniw*2+maxp1*25.
//last   - integer on return, last equals the number of subintervals produced in the 
//         subdivision process, which determines the number of significant elements 
//         actually in the work arrays.

   iwork=new integer[leniw+1];
//iwork  - integer vector of dimension at least leniw on return, the first k elements
//         of which contain pointers to the error estimates over the subintervals, such 
//         that work(limit*3+iwork(1)), .. work(limit*3+iwork(k)) form a decreasing 
//         sequence, with limit = lenw/2 , and k = last if last.le.(limit/2+2), and 
//         k = limit+1-last otherwise.
//         furthermore, iwork(limit+1), ..., iwork(limit+last) indicate the subdivision 
//         levels of the subintervals, such that iwork(limit+i) = l means that the 
//         subinterval numbered i is of length abs(b-a)*2**(1-l).
   work=new real[lenw+1];
//work   - real vector of dimension at least lenw on return work(1), ..., work(last) 
//         contain the left end points of the subintervals in the partition of (a,b), 
//         work(limit+1), ..., work(limit+last) contain the right end points, 
//         work(limit*2+1), ..., work(limit*2+last) contain the integral 
//         approximations over the subintervals, 
//         work(limit*3+1), ..., work(limit*3+last) contain the error estimates. 
//         work(limit*4+1), ..., work(limit*4+maxp1*25) provide space for storing 
//         the chebyshev moments. note that limit = lenw/2.
   


// subroutine dqawo(f,a,b,omega,integr,epsabs,epsrel,result,abserr,
     //*   neval,ier,leniw,maxp1,lenw,last,iwork,work)

   //dqawo(f,a,b,omega,integr,epsabs,epsrel,result,abserr,neval,ier,leniw,
   //      maxp1,lenw,last,iwork,work)
   qawo_(&FuncSub,&a,&b,&omega,&integr,&epsabs,&epsrel,&result,&abserr,&neval,&ier,
                   &leniw,&maxp1,&lenw,&last,iwork,work);
   FuncStatic=tmpFunc;
   delete iwork;delete work;
   if (ier!=0) {cout<<" Qawf_FourierIntAB : error "<<ier<<" ResErr "<<abserr<<"\n";cout.flush();};
   ResErr=abserr;Err=ier;NumberEvaluations=neval;
   return result;


//on return
//   result - real approximation to the integral
//
//   abserr - real estimate of the modulus of the absolute error,
//            which should equal or exceed abs(i-result)
//
//   neval  - integer number of  integrand evaluations
//
//   ier    - integer
//            ier = 0 normal and reliable termination of the routine. it is assumed 
//                    that the requested accuracy has been achieved.
//          - ier.gt.0 abnormal termination of the routine. the estimates for integral 
//                    and error are less reliable. it is assumed that the requested 
//                    accuracy has not been achieved.
//   error messages
//            ier = 1 maximum number of subdivisions allowed (= leniw/2) has been 
//                    achieved. one can allow more subdivisions by increasing the
//                    value of leniw (and taking the according dimension adjustments 
//                    into account). however, if this yields no improvement it is 
//                    advised to analyze the integrand in order to determine the 
//                    integration difficulties. if the position of a local difficulty 
//                    can be determined (e.g. singularity, discontinuity within the
//                    interval) one will probably gain fromsplitting up the interval at 
//                    this point and calling the integrator on the subranges. if 
//                    possible, an appropriate special-purpose integrator should be used
//                    which is designed for handling the type of difficulty involved.
//                = 2 the occurrence of roundoff error is detected, which prevents the 
//                    requested tolerance from being achieved. the error may be 
//                    under-estimated.
//                = 3 extremely bad integrand behaviour occurs at some interior points 
//                    of the integration interval.
//                = 4 the algorithm does not converge. roundoff error is detected in 
//                    the extrapolation table. it is presumed that the requested 
//                    tolerance cannot be achieved due to roundoff in the extrapolation
//                    table, and that the returned result is the best which can be 
//                    obtained.
//                = 5 the integral is probably divergent, or slowly convergent. it 
//                    must be noted that divergence can occur with any other value
//                    of ier.
//                = 6 the input is invalid, because (epsabs.le.0 and 
//                    epsrel.lt.max(50*rel.mach.acc.,0.5d-28))
//                    or (integr.ne.1 and integr.ne.2), or leniw.lt.2 or maxp1.lt.1 or
//                    lenw.lt.leniw*2+maxp1*25. result, abserr, neval, last are set to
//                    zero. except when leniw, maxp1 or lenw are invalid, 
//                    work(limit*2+1), work(limit*3+1), iwork(1), iwork(limit+1) are 
//                    set to zero, work(1) is set to a and work(limit+1) to b.







}


// =========================================================================================
// =====================   New Addition     ================================================
// =======================    END    =======================================================
// =========================================================================================


//#include <ostream>
double Qage_OscilInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err)
  {
   int Dir;if (CheckLim(From,To,ResErr,Err,Dir)) return 0;
   real a,b,epsabs,epsrel,result,abserr,*alist__,*blist,*rlist,*elist;
   integer key,limit,neval,ier,last,*iord;
   X_func tmpFunc=FuncStatic;
   FuncStatic=f;
   a=From;b=To;epsabs=ErrorAbs;epsrel=ErrorRel;
   limit=NumSubInt;
// limit  dimensioning parameter for iwork; limit determines the maximum number of 
//        subintervals in the partition of the given integration interval (a,b)
   key=1;
// key  -  for choice of local integration rule a gauss-kronrod pair is used with 
//  7 - 15 points if key =1 ; 10 - 21 points if key = 2, 15 - 31 points if key = 3, 
//  20 - 41 points if key =4, 25 - 51 points if key = 5, 30 - 61 points if key.gt.5.
   alist__=new real[limit+1];
// alist   - vector of dimension at least limit, the first last  elements of which 
//           are the left end points of the subintervals in the partition 
//           of the given integration range (a,b) 
   blist=new real[limit+1];
// blist   - vector of dimension at least limit, the first last  elements of which 
//           are the right end points of the subintervals in the partition *
//           of the given integration range (a,b) */
   rlist=new real[limit+1];
// rlist   - vector of dimension at least limit, the first last  elements of which 
//           are the integral approximations on the subintervals 
   elist=new real[limit+1];
// elist   - vector of dimension at least limit, the first last  elements of which 
//           are the moduli of the absolute error estimates on the subintervals 
   iord=new integer[limit+1];
// iord    - vector of dimension at least limit, the first k elements of which are 
//           pointers to the error estimates over the subintervals, such that 
//           elist(iord(1)), ..., elist(iord(k)) form a decreasing sequence, 
//           with k = last if last.le.(limit/2+2), and k = limit+1-last otherwise 


   qage_(&FuncSub, &a, &b, &epsabs, &epsrel, &key, &limit, &result, &abserr, &neval, &ier, 
            alist__, blist, rlist,elist, iord, &last);
   FuncStatic=tmpFunc;
   delete alist__;delete blist;delete rlist;delete elist;delete iord;
   if (ier!=0) {cout<<" Qage_OscilInt : error "<<ier<<" ResErr "<<abserr<<"\n";cout.flush();};
   ResErr=abserr;Err=ier;NumberEvaluations=neval;
   return result*Dir;
// neval  -  number of integrand evaluations 

//  ier = 0 normal and reliable termination of the routine. 
//  ier.gt.0 abnormal termination of the routine requested accuracy has not been achieved. 
//  ier = 1 maximum number of subdivisions allowed has been achieved. 
//  ier = 2 the occurrence of roundoff error is detected, 
//  ier = 3 extremely bad integrand behaviour occurs at some points of the integration 
//  ier = 6 the input is invalid, because 
//          (epsabs.le.0 and epsrel.lt.max(50*rel.mach.acc.,0.5d-28)) 
//          or limit.lt.1 or lenw.lt.limit*4. 

// last  - on return, last equals the number of subintervals produced in the 
//         subdivision process, which determines the number of significant elements 
//         actually in the work arrays. */
// abserr - estimate of the modulus of the absolute error, 
//          which should equal or exceed abs(i-result) 
}




double Qag_OscilInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err)
  {
   
   int Dir;if (CheckLim(From,To,ResErr,Err,Dir)) return 0;
   real a,b,epsabs,epsrel,result,abserr,*work;
   integer key,limit,neval,ier,last,*lenw,*iwork;
   X_func tmpFunc=FuncStatic;
   FuncStatic=f;
   a=From;b=To;epsabs=ErrorAbs;epsrel=ErrorRel;
   limit=NumSubInt;
// limit  dimensioning parameter for iwork; limit determines the maximum number of 
//        subintervals in the partition of the given integration interval (a,b)
   key=1;
// key  -  for choice of local integration rule a gauss-kronrod pair is used with 
//  7 - 15 points if key =1 ; 10 - 21 points if key = 2, 15 - 31 points if key = 3, 
//  20 - 41 points if key =4, 25 - 51 points if key = 5, 30 - 61 points if key.gt.5.
   lenw=new integer[limit*4+1];lenw[0]=limit*4;
// lenw  - dimensioning parameter for work lenw must be at least limit*4. 
   iwork=new integer[limit+1];iwork[0]=limit;
// iwork - vector of dimension at least limit, the first k elements of which 
//         contain pointers to the error estimates over the subintervals, such 
//         that work(limit*3+iwork(1)),... , work(limit*3+iwork(k)) 
//         form a decreasing sequence with k = last if last.le.(limit/2+2), 
//                                     and k = limit+1-last otherwise 
   work=new real[limit*4+1];work[0]=limit*4;
// work  - vector of dimension at least lenw on return work(1), ..., work(last) 
//         contain the left end points of the subintervals in the partition of (a,b), 
//         work(limit+1), ..., work(limit+last) contain the  right end points, 
//         work(limit*2+1), ..., work(limit*2+last) contain the integral approximations over the subintervals, 
//         work(limit*3+1), ..., work(limit*3+last) contain the error estimates. 

   qag_(&FuncSub, &a, &b, &epsabs, &epsrel, &key, &result, &abserr, 
            &neval, &ier, &limit, lenw, &last, iwork, work);
   FuncStatic=tmpFunc;
   delete work;delete lenw;delete iwork;
   if (ier!=0) {cout<<" Qag_OscilInt : error "<<ier<<" ResErr: "<<abserr<<"\n";cout.flush();};
   Err=ier;ResErr=abserr;NumberEvaluations=neval;
   return result*Dir;
// neval  -  number of integrand evaluations 

//  ier = 0 normal and reliable termination of the routine. 
//  ier.gt.0 abnormal termination of the routine requested accuracy has not been achieved. 
//  ier = 1 maximum number of subdivisions allowed has been achieved. 
//  ier = 2 the occurrence of roundoff error is detected, 
//  ier = 3 extremely bad integrand behaviour occurs at some points of the integration 
//  ier = 6 the input is invalid, because 
//          (epsabs.le.0 and epsrel.lt.max(50*rel.mach.acc.,0.5d-28)) 
//          or limit.lt.1 or lenw.lt.limit*4. 

// last  - on return, last equals the number of subintervals produced in the 
//         subdivision process, which determines the number of significant elements 
//         actually in the work arrays. */
// abserr - estimate of the modulus of the absolute error, 
//          which should equal or exceed abs(i-result) 
  };


double Qagi_InfinInt(X_func f,double From,int Inf,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err)
  {
//   int Dir;if (CheckLim(From,To,ResErr,Err)) return 0;
   real bound,epsabs,epsrel,result,abserr,*work;
   integer inf,key,limit,neval,ier,last,*lenw,*iwork;
   X_func tmpFunc=FuncStatic;
   FuncStatic=f;
   bound=From;
// bound  - finite bound of integration range (has no meaning if interval is 
//          doubly-infinite) 
   inf=Inf;
// inf    - indicating the kind of integration range involved 
//                     inf = 1 corresponds to  (bound,+infinity), 
//                     inf = -1            to  (-infinity,bound), 
//                     inf = 2             to (-infinity,+infinity). 
   epsabs=ErrorAbs;epsrel=ErrorRel;
   limit=NumSubInt;
// limit  dimensioning parameter for iwork; limit determines the maximum number of 
//        subintervals in the partition of the given integration interval (a,b)
   key=1;
// key  -  for choice of local integration rule a gauss-kronrod pair is used with 
//  7 - 15 points if key =1 ; 10 - 21 points if key = 2, 15 - 31 points if key = 3, 
//  20 - 41 points if key =4, 25 - 51 points if key = 5, 30 - 61 points if key.gt.5.
   lenw=new integer[limit*4+1];lenw[0]=limit*4;
// lenw  - dimensioning parameter for work lenw must be at least limit*4. 
   iwork=new integer[limit+1];
// iwork - vector of dimension at least limit, the first k elements of which 
//         contain pointers to the error estimates over the subintervals, such 
//         that work(limit*3+iwork(1)),... , work(limit*3+iwork(k)) 
//         form a decreasing sequence with k = last if last.le.(limit/2+2), 
//                                     and k = limit+1-last otherwise 
   work=new real[limit*4+1];
// work  - vector of dimension at least lenw on return work(1), ..., work(last) 
//         contain the left end points of the subintervals in the partition of (a,b), 
//         work(limit+1), ..., work(limit+last) contain the  right end points, 
//         work(limit*2+1), ..., work(limit*2+last) contain the integral approximations over the subintervals, 
//         work(limit*3+1), ..., work(limit*3+last) contain the error estimates. 

   qagi_(FuncSub, &bound, &inf, &epsabs, &epsrel, &result, &abserr, 
          &neval, &ier, &limit, lenw, &last, iwork, work);
   FuncStatic=tmpFunc;
   delete work;delete lenw;delete iwork;
   if (ier!=0) {cout<<" Qagi_InfinInt : error "<<ier<<" ResErr: "<<abserr<<"\n";cout.flush();};
   Err=ier;ResErr=abserr;NumberEvaluations=neval;
   return result;
// neval  -  number of integrand evaluations 

//  ier = 0 normal and reliable termination of the routine. 
//  ier.gt.0 abnormal termination of the routine requested accuracy has not been achieved. 
//  ier = 1 maximum number of subdivisions allowed has been achieved. 
//  ier = 2 the occurrence of roundoff error is detected, 
//  ier = 3 extremely bad integrand behaviour occurs at some points of the integration 
//      = 4 the algorithm does not converge. roundoff error is detected in the 
//          extrapolation table. it is assumed that the requested tolerance cannot be 
//          achieved, and that the returned result is the best which can be obtained. 
//      = 5 the integral is probably divergent, or slowly convergent. it must be noted 
//          that divergence can occur with any other value of ier. 
//  ier = 6 the input is invalid, because 
//          (epsabs.le.0 and epsrel.lt.max(50*rel.mach.acc.,0.5d-28)) 
//          or limit.lt.1 or lenw.lt.limit*4. 

// last  - on return, last equals the number of subintervals produced in the 
//         subdivision process, which determines the number of significant elements 
//         actually in the work arrays. */
// abserr - estimate of the modulus of the absolute error, 
//          which should equal or exceed abs(i-result) 
  };

double Qagie_InfinInt(X_func f,double From,int Inf,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err)
  {
//   int Dir;if (CheckLim(From,To,ResErr,Err,Dir)) return 0;
   real bound,epsabs,epsrel,result,abserr,*alist__,*blist,*rlist,*elist;
   integer inf,key,limit,neval,ier,last,*iord;
   X_func tmpFunc=FuncStatic;
   FuncStatic=f;
   epsabs=ErrorAbs;epsrel=ErrorRel;
// bound  - finite bound of integration range (has no meaning if interval is 
//          doubly-infinite) 
   inf=Inf;
// inf    - indicating the kind of integration range involved 
//                     inf = 1 corresponds to  (bound,+infinity), 
//                     inf = -1            to  (-infinity,bound), 
//                     inf = 2             to (-infinity,+infinity). 
   limit=NumSubInt;
// limit  dimensioning parameter for iwork; limit determines the maximum number of 
//        subintervals in the partition of the given integration interval (a,b)
   key=1;
// key  -  for choice of local integration rule a gauss-kronrod pair is used with 
//  7 - 15 points if key =1 ; 10 - 21 points if key = 2, 15 - 31 points if key = 3, 
//  20 - 41 points if key =4, 25 - 51 points if key = 5, 30 - 61 points if key.gt.5.
   alist__=new real[limit+1];
// alist   - vector of dimension at least limit, the first last  elements of which 
//           are the left end points of the subintervals in the partition 
//           of the given integration range (a,b) 
   blist=new real[limit+1];
// blist   - vector of dimension at least limit, the first last  elements of which 
//           are the right end points of the subintervals in the partition *
//           of the given integration range (a,b) */
   rlist=new real[limit+1];
// rlist   - vector of dimension at least limit, the first last  elements of which 
//           are the integral approximations on the subintervals 
   elist=new real[limit+1];
// elist   - vector of dimension at least limit, the first last  elements of which 
//           are the moduli of the absolute error estimates on the subintervals 
   iord=new integer[limit+1];
// iord    - vector of dimension at least limit, the first k elements of which are 
//           pointers to the error estimates over the subintervals, such that 
//           elist(iord(1)), ..., elist(iord(k)) form a decreasing sequence, 
//           with k = last if last.le.(limit/2+2), and k = limit+1-last otherwise 

   qagie_(FuncSub, &bound, &inf, &epsabs, &epsrel, &limit, &result, &abserr, &neval, &ier, 
          alist__, blist, rlist, elist, iord, &last);

   FuncStatic=tmpFunc;
   delete alist__;delete blist;delete rlist;delete elist;delete iord;
   if (ier!=0) {cout<<" Qagie_InfinInt : error "<<ier<<" ResErr "<<abserr<<"\n";cout.flush();};
   ResErr=abserr;Err=ier;NumberEvaluations=neval;
   return result;
// neval  -  number of integrand evaluations 

//  ier = 0 normal and reliable termination of the routine. 
//  ier.gt.0 abnormal termination of the routine requested accuracy has not been achieved. 
//  ier = 1 maximum number of subdivisions allowed has been achieved. 
//  ier = 2 the occurrence of roundoff error is detected, 
//  ier = 3 extremely bad integrand behaviour occurs at some points of the integration 
//      = 4 the algorithm does not converge. roundoff error is detected in the 
//          extrapolation table. it is assumed that the requested tolerance cannot be 
//          achieved, and that the returned result is the best which can be obtained. 
//      = 5 the integral is probably divergent, or slowly convergent. it must be noted 
//          that divergence can occur with any other value of ier. 
//  ier = 6 the input is invalid, because 
//          (epsabs.le.0 and epsrel.lt.max(50*rel.mach.acc.,0.5d-28)) 
//          or limit.lt.1 or lenw.lt.limit*4. 

// last  - on return, last equals the number of subintervals produced in the 
//         subdivision process, which determines the number of significant elements 
//         actually in the work arrays. */
// abserr - estimate of the modulus of the absolute error, 
//          which should equal or exceed abs(i-result) 
}

double Qags_IntSubInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err)
  {
   
   int Dir;if (CheckLim(From,To,ResErr,Err,Dir)) return 0;
   real a,b,epsabs,epsrel,result,abserr,*work;
   integer limit,neval,ier,last,*lenw,*iwork;
   X_func tmpFunc=FuncStatic;
   FuncStatic=f;
   a=From;b=To;epsabs=ErrorAbs;epsrel=ErrorRel;
   limit=NumSubInt;
// limit  dimensioning parameter for iwork; limit determines the maximum number of 
//        subintervals in the partition of the given integration interval (a,b)
   lenw=new integer[limit*4+1];lenw[0]=limit*4;
// lenw  - dimensioning parameter for work lenw must be at least limit*4. 
   iwork=new integer[limit+1];iwork[0]=limit;
// iwork - vector of dimension at least limit, the first k elements of which 
//         contain pointers to the error estimates over the subintervals, such 
//         that work(limit*3+iwork(1)),... , work(limit*3+iwork(k)) 
//         form a decreasing sequence with k = last if last.le.(limit/2+2), 
//                                     and k = limit+1-last otherwise 
   work=new real[limit*4+1];work[0]=limit*4;
// work  - vector of dimension at least lenw on return work(1), ..., work(last) 
//         contain the left end points of the subintervals in the partition of (a,b), 
//         work(limit+1), ..., work(limit+last) contain the  right end points, 
//         work(limit*2+1), ..., work(limit*2+last) contain the integral approximations over the subintervals, 
//         work(limit*3+1), ..., work(limit*3+last) contain the error estimates. 

   qags_(&FuncSub, &a, &b, &epsabs, &epsrel, &result, &abserr, 
            &neval, &ier, &limit, lenw, &last, iwork, work);
   delete work;delete lenw;delete iwork;
   FuncStatic=tmpFunc;
   if (ier!=0) {cout<<" Qags_IntSubInt : error "<<ier<<" ResErr: "<<abserr<<"\n";cout.flush();};
   Err=ier;ResErr=abserr;NumberEvaluations=neval;
   return result*Dir;
// neval  -  number of integrand evaluations 

//  ier = 0 normal and reliable termination of the routine. 
//  ier.gt.0 abnormal termination of the routine requested accuracy has not been achieved. 
//  ier = 1 maximum number of subdivisions allowed has been achieved. 
//  ier = 2 the occurrence of roundoff error is detected, 
//  ier = 3 extremely bad integrand behaviour occurs at some points of the integration 
//      = 4 the algorithm does not converge. roundoff error is detected in the 
//          extrapolation table. it is presumed that the requested tolerance cannot 
//          be achieved, and that the returned result is the best which can be obtained.
//      = 5 the integral is probably divergent, or slowly convergent. it must be noted 
//          that divergence can occur with any other value of ier. 
//  ier = 6 the input is invalid, because 
//          (epsabs.le.0 and epsrel.lt.max(50*rel.mach.acc.,0.5d-28)) 
//          or limit.lt.1 or lenw.lt.limit*4. 

// last  - on return, last equals the number of subintervals produced in the 
//         subdivision process, which determines the number of significant elements 
//         actually in the work arrays. */
// abserr - estimate of the modulus of the absolute error, 
//          which should equal or exceed abs(i-result) 
  };


int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
                  int MaxIter)
  {
   real b,c,r__,re,ae;
   integer iflag,nit,dit;
   nit=MaxIter;
//   { StorageFzeroFunc *tmpDat=new StorageFzeroFunc;
//     tmpDat->Prev=FuncStoragePrev;tmpDat->ArgFunc=f;FuncStoragePrev=tmpDat;}
   X_func tmpFunc=FuncStatic;

   FuncStatic=f;
   b=From;
   c=To;
   r__=Guess;
//   R  A (better) REAL guess of a zero of F which could help in speeding up convergence.  
//      If F(B) and F(R) have opposite signs, a root will be found in the interval (B,R); 
//      if not, but F(R) and F(C) have opposite signs, a root will be found in the interval 
//      (R,C);  otherwise, the interval (B,C) will be searched for a possible root.  When no 
//      better guess is known, it is recommended that r be set to B or C, since if R is not 
//      interior to the interval (B,C), it will be ignored. 
   re=ErrorRel;
//   RE Relative error used for RW in the stopping criterion. If the requested RE is less 
//      than machine precision, then RW is set to approximately machine precision. 
//      stopping criterion,   ABS(B-C) .LE. 2.*(RW*ABS(B)+AE). 
   ae=ErrorAbs;
//   AE Absolute error used in the stopping criterion.  If the given interval (B,C) contains 
//      the origin, then a nonzero value should be chosen for AE. 

   
   fzero_(&FuncSub, &b, &c, &r__, &re, &ae, &iflag,&nit,&dit);

   FuncStatic=tmpFunc;
//   {  FuncStoragePrev=FuncStoragePrev->Prev;
//      if (FuncStoragePrev!=NULL) FuncStatic=FuncStoragePrev->ArgFunc;}

   Guess=b;//ResIter=dit;
   if (iflag==3)
     cout<<" Fzero; Flag "<<iflag<<" X "<<b<<" Func(X) "<<f(b)<<"\n"
           <<" function may be around singular point. Not zero, but From-To in given interval\n";
    
//   if (iflag==4)
//		iflag;
//      cout<<" Fzero; Flag "<<iflag<<" X "<<b<<" Func(X) "<<f(b)<<"\n"
//;//           <<" function may be around local minimum. Not zero, but From-To in given interval\n";
    
   if (iflag==5)
      cout<<" Fzero; Flag "<<iflag<<" X "<<b<<" Func(X) "<<f(b)<<" Too many iterations\n";
   return iflag;

//  IFLAG  A status code.  User must check IFLAG after each call.  Control returns to the 
//         user from FZERO in all cases. 

//         1  B is within the requested tolerance of a zero. The interval (B,C) collapsed 
//            to the requested tolerance, the function changes sign in (B,C), and 
//                   F(X) decreased in magnitude as (B,C) collapsed. 

//         2  F(B) = 0.  However, the interval (B,C) may not have collapsed to the requested 
//                       tolerance. 

//         3  B may be near a singular point of F(X). The interval (B,C) collapsed to the 
//            requested tolerance and the function changes sign in (B,C), but F(X) increased 
//            in magnitude as (B,C) collapsed, i.e. 
//            ABS(F(B out)) .GT. MAX(ABS(F(B in)),ABS(F(C in))) */

//         4  No change in sign of F(X) was found although the interval (B,C) collapsed to the 
//            requested tolerance. The user must examine this case and decide whether B is near 
//            a local minimum of F(X), or B is near a zero of even multiplicity, or neither of 
//            these. 

//          5  Too many (.GT. 500) function evaluations used. */
  };



double Fmin(X_func f,double From,double To,double Guess,double AbsErr,double &FuncXVal)
  {
   real x,a,b,t;
   X_func tmpFunc=FuncStatic;

   FuncStatic=f;
   a=From;b=To;
//   if ((Guess<From) || (Guess>To)) Guess=(From+To)/2;
   x=Guess;
//   x  A (better) REAL guess of a zero of F which could help in speeding up convergence.  
   t=AbsErr;
//cout<<" Guess "<<Guess<<"\n";
//   t  Relative error 
//   FuncXVal=fmin_(FuncSub, &x, &a, &b, &t);
   FuncXVal=fmin_(FuncSub, &x, &a, &b, &t);//FuncXVal=f(x);
   FuncStatic=tmpFunc;
//cout<<" FuncXVal "<<FuncXVal<<" x "<<x<<" f(x) "<<f(x)<<"\n";
   return x;
  };


//#include "util.h"
//#include "const.h"
//extern void MemoryMove(void *src,void *dest,int size);
//extern double MathZer;
// real=double=doublereal  int=integer
void Double2LongD(int N,double *doub,doublereal *longd,int D2L=0)
 {
  int l;
//  if ((l=sizeof(double))==sizeof(doublereal )) (D2L)?MemoryMove(doub,longd,l*N):MemoryMove(longd,doub,l*N);
//  else
   {
    if (D2L) {for (l=0;l<N;l++) longd[l]=doub[l];}
    else {for (l=0;l<N;l++) doub[l]=longd[l];}
   }
 }




double (*StaticUncMinFuncClc)(double *x,int n) ;
void (*StaticUncMinGradClcUser)(double *x,double *grad,int n,double &func) ;
double *StaticUncMinGradStp;
int NumGradCalls;
void StaticUncMinGradClcStd(double *x,double *grad,int n,double &func)
 {
  func=(*StaticUncMinFuncClc)(x,n);
//cout<<" NumGradCalls "<<NumGradCalls++<<" x1 "<<x[0]<<" x2 "<<x[1]<<" func_est "<<func<<"\n"<<flush;
  double tmp,stp;
  for (int k=0;k<n;k++) 
   {
    tmp=x[k];stp=StaticUncMinGradStp[k];x[k]+=stp;
    grad[k]=((*StaticUncMinFuncClc)(x,n)-func)/stp;
    x[k]=tmp;

//cout<<" K "<<k<<" X "<<x[k]<<" GradEst "<<grad[k]<<" stp "<<stp<<"\n";
   }
//cout<<"\n";
 }

int SubstUncMinFuncStd(integer *n, doublereal *x, doublereal *f, doublereal *g)
 {
  double f_=*f,*x_=new double[*n],*g_=new double[*n];
  Double2LongD(*n,x_,x);
  (*StaticUncMinGradClcStd)(x_,g_,*n,f_);
  Double2LongD(*n,g_,g,1); *f=f_;
  delete []x_;delete []g_;
  return 0;
 }
int SubstUncMinFuncUser(integer *n, doublereal *x, doublereal *f, doublereal *g)
 {
  double f_=*f,*x_=new double[*n],*g_=new double[*n];
  Double2LongD(*n,x_,x);
  (*StaticUncMinGradClcUser)(x_,g_,*n,f_);

//int k,N=*n;
//cout<<" Calc fun "<<f_<<"\n";
//cout<<"Calc vect \n";
//for (k=0;k<N;k++) cout<<x_[k]<<" ";
//cout<<"Found grad \n";
//for (k=0;k<N;k++) cout<<g_[k]<<" ";

  Double2LongD(*n,g_,g,1); *f=f_;
  delete []x_;delete []g_;
  return 0;
 }
  
  

  


// /*
double UncMin(int &Error,int N,double *x_start,double *x_res,double est_sol,
              double (*FuncClc)(double *x,int n) , double *GradStp,
              void (*GradClc)(double *x,double *grad,int n,double &func),
              double FunAccur,double ResErr,int MaxFun,int MaxIter,
              double *BoundMin,double *BoundMax,
              double MaxLineSearchStp,
              double LineSearchEta)
 {
//cout<<"\n ================================== StartMinFind =======================================\n";NumGradCalls=0;
   double *StaticUncMinGradStp_=StaticUncMinGradStp,ret;
   double (*StaticUncMinFuncClc_)(double *x,int n) =StaticUncMinFuncClc;
   void (*StaticUncMinGradClcUser_)(double *x,double *grad,int n,double &func) =StaticUncMinGradClcUser;
   int UseStdFunc=0,ToDeleteStp=0;

   integer msglvl=1, maxit=MaxIter, maxfun=MaxFun;
   doublereal eta=LineSearchEta, stepmx=MaxLineSearchStp, accrcy=FunAccur, xtol=ResErr;


   if (GradClc==NULL)
    {
     UseStdFunc=1;StaticUncMinFuncClc=FuncClc;
     if (GradStp==NULL) 
      {
       GradStp=new double[N];ToDeleteStp=1;
       for (int k=0;k<N;k++) if (fabs(x_res[k])<1e-30) GradStp[k]=0.01;else GradStp[k]=0.01*x_res[k];
       StaticUncMinGradStp=GradStp;
      }
     StaticUncMinGradStp=GradStp;
    }
   else StaticUncMinGradClcUser=GradClc;


   integer N_=N,Error_=Error,WorkLen_=14*N;
   doublereal ret_=est_sol,*WorkArr_=new doublereal[WorkLen_],*ResGrad_=new doublereal[N_];
   doublereal *x_res_=new doublereal[N_]; Double2LongD(N,x_start,x_res_,1);

   if (BoundMin==NULL)
    {
     if (UseStdFunc) lmqn_(&Error_, &N_, x_res_, &ret_, ResGrad_, WorkArr_, &WorkLen_, reinterpret_cast<U_fp>( SubstUncMinFuncStd)//);
                           ,&msglvl, &maxit, &maxfun, &eta, &stepmx, &accrcy, &xtol);
     else lmqn_(&Error_, &N_, x_res_, &ret_, ResGrad_, WorkArr_, &WorkLen_, reinterpret_cast<U_fp>( SubstUncMinFuncUser)//);
                           ,&msglvl, &maxit, &maxfun, &eta, &stepmx, &accrcy, &xtol);
//     if (UseStdFunc) tn_(&Error_, &N_, x_res_, &ret_, ResGrad_, WorkArr_, &WorkLen_, SubstUncMinFuncStd);
//     else tn_(&Error_, &N_, x_res_, &ret_, ResGrad_, WorkArr_, &WorkLen_, SubstUncMinFuncUser);
    }
   else
    {
     integer *ipivot_=new integer[N_];
     doublereal *BoundMin_=new doublereal[N_]; Double2LongD(N,BoundMin,BoundMin_,1);
     doublereal *BoundMax_=new doublereal[N_]; Double2LongD(N,BoundMax,BoundMax_,1);
     if (UseStdFunc) tnbc_(&Error_, &N_, x_res_, &ret_, ResGrad_, WorkArr_, &WorkLen_, SubstUncMinFuncStd,
                           BoundMin_,BoundMax_,ipivot_);
     else tnbc_(&Error_, &N_, x_res_, &ret_, ResGrad_, WorkArr_, &WorkLen_, SubstUncMinFuncUser,
                BoundMin_,BoundMax_,ipivot_);
//     if (UseStdFunc) lmqnbc_(&Error_, &N_, x_res_, &ret_, ResGrad_, WorkArr_, &WorkLen_, reinterpret_cast<U_fp>(SubstUncMinFuncStd),
//                           BoundMin_,BoundMax_,ipivot_//);
//                           ,&msglvl, &maxit, &maxfun, &eta, &stepmx, &accrcy, &xtol);
//     else lmqnbc_(&Error_, &N_, x_res_, &ret_, ResGrad_, WorkArr_, &WorkLen_, reinterpret_cast<U_fp>(SubstUncMinFuncUser),
//                BoundMin_,BoundMax_,ipivot_//);
//                           ,&msglvl, &maxit, &maxfun, &eta, &stepmx, &accrcy, &xtol);
     delete []BoundMin_;delete []BoundMax_;delete []ipivot_;
    }
//int k;
if ((Error_!=0) && ((fabs(double(ResGrad_[0]))>1) || (fabs(double(ResGrad_[N-1]))>1)) )
 {
//cout<<"\n\n Error "<<Error_<<" Found min "<<ret_<<"\n";
//cout<<"Found vect ";
//for (k=0;k<N;k++) cout<<x_res_[k]<<" ";cout<<"\n";
//cout<<"Found grad ";
//for (k=0;k<N;k++) cout<<ResGrad_[k]<<" ";cout<<"\n";
 }
   Error=Error_;ret=ret_;
   Double2LongD(N,x_res,x_res_,0);
   delete []ResGrad_;delete []WorkArr_;delete []x_res_;

   if (ToDeleteStp) delete []GradStp;

   StaticUncMinGradClcUser=StaticUncMinGradClcUser_;
   StaticUncMinFuncClc=StaticUncMinFuncClc_;
   StaticUncMinGradStp=StaticUncMinGradStp_;
   return ret;
 }
//*/
// int lmqn_(integer *ifail, integer *n, doublereal *x, 
//	doublereal *f, doublereal *g, doublereal *w, integer *lw, S_fp sfun, 

//  integer *msglvl, integer *maxit, integer *maxfun, doublereal *eta, 
//	doublereal *stepmx, doublereal *accrcy, doublereal *xtol)
