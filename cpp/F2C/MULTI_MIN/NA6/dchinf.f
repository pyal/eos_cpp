      SUBROUTINE DCHINF (NUMFUN,FUNSUB,PERIOD,GAMMA,A,B,MINPTS,MAXPTS,
     +EPSABS,EPSREL,WRKSUB,NW,EMAX,KEY,RESTAR,NEVAL,IFAIL)
C***BEGIN PROLOGUE DCHINF
C***REFER TO DQAINF
C***PURPOSE  DCHINF checks the validity of the input parameters to
C            DQAINF.
C 
C***LAST MODIFICATION 93-05-05
C 
C***DESCRIPTION
C   DCHINF computes IFAIL as a function of the input
C   parameters to DQAINF, and checks the validity of the input
C   parameters to DQAINF.
C 
C   ON ENTRY
C 
C     NUMFUN Integer.
C            Number of components of the integral.
C     FUNSUB Externally declared subroutine for computing
C            all components of the integrand at the given
C            evaluation point.
C            It must have parameters (X,NUMFUN,FUNVLS)
C            Input parameters:
C              X      Real array of dimension 21 defining the
C                     x-coordinates of the evaluation points.
C              NUMFUN Integer that defines the number of
C                     components of I.
C              NP     Integer that gives the number of evaluation points
C            Output parameter:
C              FUNVLS Real array of dimension (21,NUMFUN) that defines
C                     the function values in the 21 evaluation points
C                     for the NUMFUN components of the integrand.
C     PERIOD Real.
C            All functions are assumed to have the same asymptotic
C            period.
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
C     MINPTS Integer.
C            Minimum number of function evaluations.
C     MAXPTS Integer.
C            Maximum number of function evaluations.
C            The number of function evaluations over each sub-interval
C            is NUM = 21.
C     EPSABS Real.
C            Requested absolute error.
C     EPSREL Real.
C            Requested relative error.
C     WRKSUB Integer.
C            Defines the length of the arrays A and B. (And thus the
C            maximum number of subregions allowed.)
C     NW     Integer.
C            Defines the length of the working array WORK.
C     EMAX   Integer.
C            The maximum number of extrapolation steps. (At least one
C            step!)
C     KEY    Integer.
C            Choice of extrapolation method:
C            KEY = 0 : Euler's method.
C            KEY = 1 : Modified Euler.
C            KEY = 2 : Overholt's polynomial order 2-method.
C                      This last method is the only one that needs
C                      GAMMA.
C     RESTAR Integer.
C            If RESTAR = 0, this is the first attempt to compute
C            the integral.
C            If RESTAR = 1,
C            then we restart a previous attempt.
C            RESTAR is allowed to take these two values only.
C   ON RETURN
C 
C     NEVAL  Integer.
C            The number of function evaluations.
C     IFAIL  Integer.
C            IFAIL = 0 for normal exit.
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
C 
C***ROUTINES CALLED DGAINF
C***END PROLOGUE DCHINF
C 
C   Global variables.
C 
      INTEGER NUMFUN,MINPTS,MAXPTS,NW,RESTAR,EMAX,NEVAL,WRKSUB,KEY,
     +IFAIL
      DOUBLE PRECISION A(WRKSUB),B(WRKSUB),EPSABS,EPSREL,PERIOD,GAMMA
      EXTERNAL DGAINF,FUNSUB
C 
C   Local variables.
C 
      INTEGER LIMIT,NUMNUL,NUM
      DOUBLE PRECISION WIDTH
      PARAMETER (NUMNUL=16,NUM=21)
C 
C***FIRST EXECUTABLE STATEMENT DCHINF
C 
      IFAIL=0
C 
C   Check on legal NUMFUN.
C 
      IF (NUMFUN.LT.1) THEN
         IFAIL=2
         RETURN
      END IF
C 
C   Check on legal length of the first interval of integration.
C 
      WIDTH=B(1)-A(1)
      IF (WIDTH.LT.0) THEN
         IFAIL=3
         RETURN
      END IF
C 
C   Check on legal sign of PERIOD.
C 
      IF (PERIOD.LE.0) THEN
         IFAIL=4
         RETURN
      END IF
C 
C   Check on MAXPTS >= MINPTS.
C 
      IF ((MAXPTS.LT.MINPTS).OR.(MINPTS.LT.NUM)) THEN
         IFAIL=5
         RETURN
      END IF
C 
C   Check on legal accuracy requests.
C 
      IF (EPSABS.LE.0.AND.EPSREL.LE.0) THEN
         IFAIL=6
         RETURN
      END IF
C 
C   Check on legal WRKSUB.
C 
      IF (NUM*WRKSUB.LT.MAXPTS) THEN
         IFAIL=7
         RETURN
      END IF
C 
C   Check on legal value of EMAX.
C 
      IF (EMAX.LT.1) THEN
         IFAIL=8
         RETURN
      END IF
C 
C    Check on legal RESTAR.
C 
      IF (RESTAR.NE.0.AND.RESTAR.NE.1) THEN
         IFAIL=9
         RETURN
      ELSE IF (RESTAR.EQ.0) THEN
         NEVAL=0
      END IF
C 
C    Check on legal KEY.
C 
      IF (KEY.NE.0.AND.KEY.NE.1.AND.KEY.NE.2) THEN
         IFAIL=10
         RETURN
      END IF
C 
C   Check on big enough NW.
C 
      LIMIT=1+5*NUMFUN+4*WRKSUB*NUMFUN+3*WRKSUB+(EMAX+1)**2+(NUMNUL+1+
     +NUM)*NUMFUN
      IF (NW.LT.LIMIT) THEN
         IFAIL=11
         RETURN
      END IF
C 
C   If the user gives GAMMA <= 0 and KEY = 2 then DQAINF will try to
C   estimate GAMMA.
C   If the subroutine estimates GAMMA successfully within its error
C   bound, IFAIL will remain 0 while GAMMA is given the new value.
C   The routine DGAINF may detect errors:
C 
C   IFAIL = 12 indicates that either
C   B is too small or the PERIOD is wrong, or maybe the function does no
C   have the desired properties. (NOTE: The code checks ONLY the first
C   function in the vector, assuming that if this is correct then the
C   other functions share its properties.)
C 
C   IFAIL = 13 : No errors where detected, but the code was unable to
C   compute GAMMA sufficiently accurate. Remedy: use KEY = 1 instead.
C 
      IF ((GAMMA.LE.0).AND.(KEY.EQ.2)) THEN
         CALL DGAINF (NUMFUN,FUNSUB,B(1),PERIOD,GAMMA,NEVAL,IFAIL)
         RETURN
      END IF
C 
      RETURN
C 
C***END DCHINF
C 
      END
