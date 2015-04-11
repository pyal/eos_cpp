      SUBROUTINE SQAINF (NUMFUN,FUNSUB,PERIOD,GAMMA,A,B,WRKSUB,EMAX,
     +MINPTS,MAXPTS,EPSABS,EPSREL,NW,KEY,RESTAR,RESULT,ABSERR,NEVAL,
     +IFAIL,DONE,WORK,IWORK)
C***BEGIN PROLOGUE SQAINF
C***DATE WRITTEN   930515   (YYMMDD)
C***REVISION DATE  940310   (YYMMDD)
C***CATEGORY NO. H2B2A1/H2B2A2
C***AUTHORS
C            Terje O. Espelid, Department of Informatics,
C            University of Bergen,  Hoyteknologisenteret.
C            N-5020 Bergen, Norway
C            Email..  terje@ii.uib.no
C 
C            Kjell J. Overholt, Department of Informatics,
C            University of Bergen,  Hoyteknologisenteret.
C            N-5020 Bergen, Norway
C            Email..  kjell@ii.uib.no
C 
C***PURPOSE  To compute several one-dimensional integrals over
C            an infinite region. All of the integrands are assumed to be
C            oscillatory, with the same asymptotic periodic behavior.
C            This routine is the driving routine for the integration
C            routine SADINF.
C 
C***KEYWORDS Quadrature, one dimensional, oscillatory integrands,
C            infinite region, extrapolation, globally adaptive,
C            periodic functions.
C 
C***DESCRIPTION
C 
C   The routine calculates an approximation to a given vector of
C   infinite integrals
C 
C            I (F ,F ,...,F      ) DX,
C                1  2      NUMFUN
C 
C   where the interval of integration is [A,infinity), where A is
C   supplied by the user, and with
C   B as a possible subdivision point also supplied by the user:
C   A <= B. We assume that all functions have the same oscillatory
C   behavior for values of X => B.
C 
C   Here F = F (X), J = 1,2,...,NUMFUN.
C         J   J
C 
C   A globally adaptive strategy, combined with extrapolation,
C   is applied in order to compute approximations RESULT(J),
C   for each component J of I, the following claim for accuracy:
C 
C    ABS(I(J)-RESULT(J)).LE.MAX(EPSABS,EPSREL*ABS(I(J)))
C 
C   SQAINF is a driver for the integration routine SADINF.
C 
C   SADINF either (1) repeatedly subdivides the interval with greatest
C   estimated errors; then estimates the integrals and the errors over
C   the new sub-intervals and finally updates the extrapolation tableau,
C   until the error request is met or MAXPTS function
C   evaluations have been used, or (2) decides instead to compute
C   a new term: the integral over the next half-period and then
C   perform a new extrapolation step.
C 
C   Only one basic integration rule (21 points) is offered: Gauss rule.
C 
C   Three different extrapolation methods are offered through the
C   parameter KEY:: Euler's method, a modification of Eulers method and
C   Overholt's P-order 2-method.
C 
C   If the values of the input parameters EPSABS or EPSREL are selected
C   great enough, the chosen integration rule is applied over the two
C   first intervals [A,B] and [B,B+PERIOD/2] for each
C   function in the vector and then one extrapolation step is performed
C   to give approximations RESULT(J), J = 1, 2, ..., NUMFUN.
C   No further subdivisions and no
C   more new terms will then be computed.
C 
C   When SQAINF computes estimates to a vector of integrals,
C   then all components of the vector are given the same treatment.
C   That is, I(F ) and I(F ) for J not equal to K, are estimated with
C              J         K
C   the same subdivision of the original interval of integration
C   and the same extrapolation scheme is applied.
C   For integrals with enough similarity, we may save time by applying
C   SQAINF to all integrands in one call. For integrals that varies
C   continuously as functions of some parameter, the estimates
C   produced by SQAINF will also vary continuously when the same
C   subdivision is applied to all components. This will generally not
C   be the case when the different components are given separate
C   treatment.
C 
C   On the other hand this feature should be used with caution when the
C   different components of the integrals require clearly different
C   subdivisions.
C 
C   ON ENTRY
C 
C     NUMFUN Integer.
C            Number of components of the integral.
C     FUNSUB Externally declared subroutine for computing
C            all components of the integrand at the given
C            evaluation point.
C            It must have parameters (X,NUMFUN,NP,FUNVLS)
C            Input parameters:
C              X      Real array of dimension 21 defining the
C                     x-coordinates of the evaluation points.
C              NUMFUN Integer that defines the number of
C                     components of the integral I.
C              NP     Integer that gives the number of evaluation points
C            Output parameter:
C              FUNVLS Real array of dimension (21,NUMFUN) that defines
C                     the function values in the 21 evaluation points
C                     for the NUMFUN components of the integrand.
C     PERIOD Real.
C            All functions are assumed to have the same asymptotic
C            period > 0.
C     GAMMA  Real.
C            All functions are assumed to decay as c/x**GAMMA,
C            when x >> 1 and we go in steps of length PERIOD,
C            (GAMMA > 0).
C     A      Real array of dimension WRKSUB.
C     B      Real array of dimension WRKSUB.
C            A(1) and B(1) are the left endpoint and right endpoint
C            of the first interval,  A(1) <= B(1). B(1) is chosen by the
C            user and it is assumed that the integrand oscillates for
C            X >= B(1). Asymptotic period is PERIOD. Thus oscillations
C            are expected to be observed for points of distance PERIOD/2
C            A(2), B(2), etc. are defined by the code, and as a warning:
C            the code may change the value of A(1) and B(1).
C     WRKSUB Integer.
C            Defines the length of the arrays A and B. (And thus the max
C            number of subregions allowed.)
C     EMAX   Integer.
C            The maximum number of extrapolation steps.
C     MINPTS Integer.
C            Minimum number of function evaluations.
C     MAXPTS Integer.
C            Maximum number of function evaluations.
C            The number of function evaluations over each sub-interval
C            is 21.
C     EPSABS Real.
C            Requested absolute error.
C     EPSREL Real.
C            Requested relative error.
C     NW     Integer.
C            Defines the length of the working array WORK.
C            We let
C     KEY    Integer
C            Choice of extrapolation method:
C            KEY = 0 : Euler's method.
C            KEY = 1 : Modified Euler.
C            KEY = 2 : Overholt's polynomial order 2-method.
C                      This last method is the only one that needs
C                      the value of GAMMA.
C     RESTAR Integer.
C            If RESTAR = 0, this is the first attempt to compute
C            the integral.
C            If RESTAR = 1,
C            then we restart a previous attempt.
C            In this case the only parameters for SQAINF that may
C            be changed (with respect to the previous call of SQAINF)
C            are MINPTS, MAXPTS, EPSABS, EPSREL and RESTAR.
C 
C   ON RETURN
C 
C     RESULT Real array of dimension NUMFUN.
C            Approximations to all components of the integral.
C     ABSERR Real array of dimension NUMFUN.
C            Estimates of absolute errors.
C     NEVAL  Integer.
C            Number of function evaluations used by SQAINF.
C     IFAIL  Integer.
C            IFAIL = 0 for normal exit, when
C 
C              ABSERR(J) <=  EPSABS or
C              ABSERR(J) <=  ABS(RESULT(J))*EPSREL with MAXPTS or less
C              function evaluations for all values of J,
C              from 1 to NUMFUN.
C 
C            IFAIL = +1 if MAXPTS was too small
C              to obtain the required accuracy. In this case SQAINF
C              returns values of RESULT with estimated absolute
C              errors ABSERR.
C            IFAIL = -1 if EMAX was too small
C              to obtain the required accuracy. In this case SQAINF
C              returns values of RESULT with estimated absolute
C              errors ABSERR.
C            IFAIL =  2 if NUMFUN is less than 1.
C            IFAIL =  3 if B(1) < A(1).
C            IFAIL =  4 if unlegal PERIOD.
C            IFAIL =  5 if MAXPTS is less than MINPTS or MINPTS < 21.
C            IFAIL =  6 if EPSABS <= 0 and EPSREL <= 0.
C            IFAIL =  7 if WRKSUB is too small.
C            IFAIL =  8 if unlegal value of EMAX.
C            IFAIL =  9 if unlegal RESTAR.
C            IFAIL = 10 if unlegal value of KEY.
C            IFAIL = 11 if NW is less than LIMIT (See the code DCHINF).
C            IFAIL = 12 if either PERIOD is wrong or B(1) is too small.
C            IFAIL = 13 if unable to estimate GAMMA (In case KEY=2 only)
C     A      Real array of dimension WRKSUB.
C     B      Real array of dimension WRKSUB.
C            On exit A and B contains the endpoints of the intervals
C            used to produce the approximations to the integrals.
C     DONE   Logical array of dimension NUMFUN.
C            On exit : DONE(I)=.TRUE. if function number I has been
C            integrated to the specified precision, else DONE(I)=.FALSE.
C            (Note that IFAIL = 1 just tells you
C            that something is wrong, however most of the integrals in
C            the vector may have been computed to the specified precisio
C     WORK   Real array of dimension NW.
C            Used as working storage.
C            WORK(NW) = NSUB, the number of sub-intervals in the data
C            structure.
C            Let NW >= 1+5*NUMFUN+4*WRKSUB*NUMFUN
C                     +3*WRKSUB+(EMAX+1)**2 + (NUMNUL+3+NUM)*NUMFUN)
C            Then
C            WORK(1),...,WORK(NUMFUN*WRKSUB) contain the estimated
C              components of the integrals over the sub-intervals.
C            WORK(NUMFUN*WRKSUB+1),...,WORK(2*NUMFUN*WRKSUB) contain
C              the estimated errors over the sub-intervals.
C            WORK(2*NUMFUN*WRKSUB+1),...,WORK(2*NUMFUN*
C              WRKSUB+WRKSUB) contain the greatest errors
C              in each sub-interval.
C            WORK(2*WRKSUB*NUMFUN+WRKSUB+1),...,WORK(2*WRKSUB*NUMFUN
C               +2*WRKSUB) contain the left sub-division point in each
C               sub-interval.
C            WORK(2*WRKSUB*NUMFUN+2*WRKSUB+1),...,WORK(2*WRKSUB*NUMFUN
C               +3*WRKSUB) contain the right sub-division point in each
C               sub-interval.
C            WORK((2*NUMFUN+3)*WRKSUB+1),...,WORK(NW) is used as
C              temporary storage in SADINF.
C     IWORK  Integer array of dimension 3*WRKSUB.
C            Used as working storage.
C 
C***LONG DESCRIPTION
C 
C   The information for each interval is contained in the data
C   structures A,B, WORK and IWORK. A and B contain the endpoints of
C   the intervals. When passed on to SADINF, WORK is split into four
C   arrays VALUES, ERRORS, GREATE and WORK2. VALUES contains the
C   estimated values of the integrals. ERRORS contains the estimated
C   errors. GREATE contains the greatest estimated error for each
C   interval. The data structures for the intervals are in SADINF
C   organized as a heap, and the size of GREATE(I) defines the
C   position of interval I in the heap. The heap is maintained by the
C   program STRINF and we use a partially ordered list of pointers,
C   saved in IWORK. When passed on to SADINF, IWORK is split into three
C   arrays WORST, LIST and UPOINT. LIST is a partially ordered list
C   such that GREATE(LIST(1)) is the maximum worst case error estimate
C   for all sub-intervals in our data-structure. In WORST the index to
C   the integral with the greatest error-estimate is kept. UPOINT  is
C   an array of pointers to where in the U-sequence  a region belongs.
C   This information is used when updating  the corresponding U-term
C   after a subdivision.
C 
C   The subroutine for estimating the integral and the error over each
C   sub-interval, SGAINT, uses WORK2 as a work array.
C 
C***REFERENCES
C 
C   T.O.Espelid and K.J.Overholt, DQAINF: An Algorithm for Automatic
C   Integration of Infinite Oscillating Tails, submitted for publ. 1993.
C 
C***ROUTINES CALLED SCHINF,SADINF
C***END PROLOGUE SQAINF
C 
C   Parameters
C 
C 
C   Global variables.
C 
      EXTERNAL FUNSUB
      INTEGER WRKSUB
      INTEGER NUMFUN,MINPTS,MAXPTS,NW,RESTAR,NEVAL,IFAIL,IWORK(3*WRKSUB)
     +,KEY,EMAX
      REAL A(WRKSUB),B(WRKSUB),EPSABS,EPSREL,RESULT(NUMFUN),
     +ABSERR(NUMFUN),WORK(NW),PERIOD,GAMMA
      LOGICAL DONE(NUMFUN)
C 
C   Local variables.
C 
      INTEGER NSUB,LENW,NUM,NUMNUL
      INTEGER I1,I2,I3,I4,I5,I6,I7,I8,I9,I10,I11,I12,I13,I14
      PARAMETER (NUM=21,NUMNUL=16)
C 
C***FIRST EXECUTABLE STATEMENT DQAINF
C 
C   Check the input parameters.
C 
      CALL SCHINF (NUMFUN,FUNSUB,PERIOD,GAMMA,A,B,MINPTS,MAXPTS,EPSABS,
     +EPSREL,WRKSUB,NW,EMAX,KEY,RESTAR,NEVAL,IFAIL)
      IF (IFAIL.NE.0) THEN
         RETURN
      END IF
C 
C   Split up the work space.
C 
      I1=1
      I2=I1+WRKSUB*NUMFUN
      I3=I2+WRKSUB*NUMFUN
      I4=I3+WRKSUB
      I5=I4+WRKSUB
      I6=I5+WRKSUB
      I7=I6+WRKSUB*NUMFUN
      I8=I7+WRKSUB*NUMFUN
      I9=I8+NUMFUN
      I10=I9+NUMFUN
      I11=I10+NUMFUN
      I12=I11+(EMAX+1)*(EMAX+1)
      I13=I12+NUMFUN
      I14=I13+NUMFUN
C 
C   On restart runs the number of sub-intervals from the
C   previous call is assigned to NSUB.
C 
      IF (RESTAR.EQ.1) THEN
         NSUB=WORK(NW)
      END IF
C 
C   Compute the size of the temporary work space needed in DADINF.
C 
      LENW=(NUMNUL+1+NUM)*NUMFUN
      CALL SADINF (NUMFUN,FUNSUB,PERIOD,GAMMA,A,B,WRKSUB,EMAX,MINPTS,
     +MAXPTS,EPSABS,EPSREL,RESTAR,LENW,KEY,RESULT,ABSERR,NEVAL,NSUB,
     +IFAIL,DONE,WORK(I1),WORK(I2),WORK(I3),WORK(I4),WORK(I5),WORK(I6),
     +WORK(I7),WORK(I8),WORK(I9),WORK(I10),WORK(I11),WORK(I12),WORK(I13)
     +,WORK(I14),IWORK(1),IWORK(1+WRKSUB),IWORK(1+2*WRKSUB))
      WORK(NW)=NSUB
      RETURN
C 
C***END SQAINF
C 
      END
