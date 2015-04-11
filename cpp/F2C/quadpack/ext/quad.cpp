
#include "quad.h"
X_func FuncStatic;

int NumberEvaluations;
#ifdef __cplusplus
extern "C" {
#endif
//#include "f2c.h"

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



doublereal FuncSub(real *x)  { return FuncStatic(*x);};


double r_sign(real *a, real *b)
{ double x; x = (*a >= 0 ? *a : - *a);return( *b >= 0 ? x : -x);}



//#include <stdio.h>
//#include <float.h>
//#include <math.h>
//doublereal r1mach_(integer *)
doublereal r1mach_(integer *i)
{
 real e=DBL_MIN;
 if (e==2*e)
 {
	 switch(*i){
	   case 1: return FLT_MIN;
	   case 2: return FLT_MAX;
 	  case 3: return FLT_EPSILON/FLT_RADIX;
	   case 4: return FLT_EPSILON;
	   case 5: return log10((double)FLT_RADIX);
             }
 }
 else
 {
	 switch(*i){
	   case 1: return DBL_MIN;
	   case 2: return DBL_MAX;
 	  case 3: return DBL_EPSILON/DBL_RADIX;
	   case 4: return DBL_EPSILON;
	   case 5: return log10((double)DBL_RADIX);
             }
 }
//	fprintf(stderr, "invalid argument: r1mach(%ld)\n", *i);
	return 0; /* else complaint of missing return value */
}
int	xerror_(char *msg, integer *len, integer *ier,integer *lvl,ftnlen len1)
{
// fprintf(stderr,"%s  Error:  %ld   Level:  %ld \n", msg,*ier,*lvl);
 return 0;
}

#ifdef __cplusplus
	}
#endif


double pow_dd(double *x, double *y)
  {
   return pow(*x,*y);
  };
int CheckLim(double &From,double &To,double &ResErr,int &Err,int &Dir)
  {
   Dir=1;
   if (From==To) {ResErr=0;Err=0;return 1;}
   if (From>To) { double tmp=From;From=To;To=tmp;Dir=-1;}
   return 0;
  };

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
   Guess=b;//ResIter=dit;
   if (iflag==3)
     cout<<" Fzero; Flag "<<iflag<<" X "<<b<<" Func(X) "<<f(b)<<"\n"
           <<" function may be around singular point. Not zero, but From-To in given interval\n";
    
   if (iflag==4)
;//      cout<<" Fzero; Flag "<<iflag<<" X "<<b<<" Func(X) "<<f(b)<<"\n"
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

