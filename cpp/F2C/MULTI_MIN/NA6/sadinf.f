      SUBROUTINE SADINF (NUMFUN,FUNSUB,PERIOD,GAMMA,A,B,WRKSUB,EMAX,
     +MINPTS,MAXPTS,EPSABS,EPSREL,RESTAR,LENW,KEY,RESULT,ABSERR,NEVAL,
     +NSUB,IFAIL,DONE,VALUES,ERRORS,GREATE,C,D,U,E,T,CT,EXTERR,BETA,
     +UNEW,UERR,WORK2,WORST,LIST,UPOINT)
C***BEGIN PROLOGUE SADINF
C***KEYWORDS Quasi-linear extrapolation, infinite integration,
C            oscillating functions, adaptive strategy.
C***PURPOSE  To compute better estimates to a vector of approximations
C            to integrals of oscillating functions over an infinite
C            interval.
C 
C***LAST MODIFICATION 94-03-10
C 
C***REFER TO SAQINF
C 
C***DESCRIPTION Computation of integrals over an infinite interval
C   by combining extrapolation and adaptive strategies.
C 
C   SADINF uses extrapolation on a sum of integrals over subintervals
C   Each subinterval has length P = PERIOD/2 and the function involved i
C   assumed to be asymptotically periodic with period PERIOD.
C   This routine will EITHER take another extrapolation step
C   in order to reduce the pure extrapolation error, OR
C   subdivide the interval in the collection with greatest estimated
C   error. In both cases estimate(s) of the integral(s) and the error(s)
C   to the new sub-interval(s) are computed; this process continues
C   until the error request is met or MAXPTS function evaluations have
C   been used.
C 
C   The 21 point rule is: Gauss.
C 
C   ON ENTRY
C 
C     NUMFUN Integer.
C            The number of components of the integral.
C     FUNSUB Externally declared subroutine for computing
C            all components of the integrand at all NP
C            evaluation points.
C            It must have parameters (X,NUMFUN,FUNVLS)
C            Input parameters:
C              X      The x-coordinates of the NP evaluation points.
C              NUMFUN Integer that defines the number of
C                     components of I.
C              NP     Integer that gives the number of evaluation points
C            Output parameter:
C              FUNVLS Real array of dimension NUMFUN
C                     that defines NUMFUN components of the integrand.
C 
C     PERIOD Real.
C            All functions are assumed to have the same asymptotic
C            period.
C     GAMMA  Real.
C            All functions are assumed to decay as c/x**GAMMA,
C            when x >> 1 and we go in steps of length PERIOD,
C            (GAMMA > 0).
C     A      Real array of dimension WRKSUB.
C     B      Real array of dimension WRKSUB.
C            A(1) is the left endpoint of integration.
C            B(1) >= A(1) is a user specified subdivision point.
C            We assume that the assumptions on which this code is
C            based hold for x >= B(1). We will not extrapolate based
C            on the function values between A(1) and B(1).
C     WRKSUB Integer.
C            Defines the length of the arrays A and B .
C     EMAX   Integer.
C            The maximum number of extrapolation steps.
C     MINPTS Integer.
C            Minimum number of function evaluations.
C     MAXPTS
C            Maximum number of function evaluations.
C     EPSABS Real.
C            Requested absolute error.
C     EPSREL Real.
C            Requested relative error.
C 
C     RESTAR Integer.
C            If RESTAR = 0, this is the first attempt to compute
C            the integral.
C            If RESTAR = 1,
C            then we restart a previous attempt.
C            In this case the only parameters for SAQINF that may
C            be changed (with respect to the previous call of SAQINF)
C            are MINPTS, MAXPTS, EPSABS, EPSREL and RESTAR.
C     LENW   Integer.
C            Length of the workspace WORK2.
C     KEY    Integer
C            Choice of extrapolation method:
C            KEY = 0 : Euler's method.
C            KEY = 1 : Modified Euler.
C            KEY = 2 : Overholt's polynomial order 2-method.
C                      This last method is the only one that needs
C                      GAMMA.
C 
C   ON RETURN
C 
C     RESULT Real array of dimension NUMFUN.
C            Approximations to all components of the integral.
C     ABSERR Real array of dimension NUMFUN.
C            Estimates of absolute errors.
C     NEVAL  Integer.
C            The number of function evaluations.
C     NSUB   Integer.
C            The number of intervals in the data structure.
C     A      Real array of dimension WRKSUB.
C     B      Real array of dimension WRKSUB.
C            On exit A and B contains the endpoints of the intervals
C            used to produce the approximations to the integrals.
C            Warning: A(1) and B(1) may have changed due to
C            adaptive subdivision of the first interval.
C     IFAIL  Integer.
C            IFAIL = 0 for normal exit.
C            IFAIL = +1 if MAXPTS was too small for SADINF
C                      to obtain the required accuracy. In this case
C                     SADINF returns values of RESULT with estimated
C                     absolute errors ABSERR.
C            IFAIL = -1 if EMAX was too small for SADINF
C                      to obtain the required accuracy. In this case
C                     SADINF returns values of RESULT with estimated
C                     absolute errors ABSERR.
C 
C     DONE   Logical array of dimension NUMFUN.
C            On exit : DONE(I)=.TRUE. if function number I has been
C            integrated to the specified precision, else DONE(I)=.FALSE.
C 
C     VALUES Real array of dimension (NUMFUN,WRKSUB).
C            The estimated components of the integrals over the
C            sub-intervals.
C     ERRORS Real array of dimension (NUMFUN,WRKSUB).
C            The estimated errors over the sub-intervals.
C     GREATE Real array of dimension WRKSUB.
C            The greatest error in each sub-interval.
C     C      Real array of dimension WRKSUB.
C            The left sub-division points of all intervals in the heap.
C     D      Real array of dimension WRKSUB.
C            The right sub-division points of all intervals in the heap.
C     U      Real array of dimension (NUMFUN,WRKSUB)
C            contains the terms in series.
C     E      Real array of dimension (NUMFUN,WRKSUB)
C            contains the estimated errors in each U-term.
C     T      Real array of dimension NUMFUN.
C            Dummy parameter.
C     CT     Real array of dimension NUMFUN.
C            Dummy parameter.
C     EXTERR Real array of dimension NUMFUN.
C            These errors are associated with the region (LEFT,infinity)
C            and they are the pure extrapolation errors.
C     BETA   Real array of dimension (0:EMAX,0:EMAX).
C            Dummy parameter.
C     UNEW   Real array of dimension NUMFUN.
C            This gives the next terms in the series (new extrapolation
C            step) else it is the correction to the U-values (updating).
C     UERR   Real array of dimension NUMFUN.
C            The estimated errors of all U-terms in the series.
C     WORK2  Real array of dimension LENW.
C            Work array used in SGAINT.
C     WORST  Integer array of dimension WRKSUB.
C            Index to the greatest error in each sub-interval.
C     LIST   Integer array used in STRINT of dimension WRKSUB.
C            Is a partially sorted list, where LIST(1) is the top
C            element in a heap of sub-intervals.
C     UPOINT Integer array of dimension WRKSUB.
C            Is an array of pointers to where in the U-sequence
C            a region belongs. This information is used when updating
C            the corresponding U-term after a subdivision.
C 
C***ROUTINES CALLED STRINT, SGAINT, SEXINF, R1MACH.
C***END PROLOGUE SADINF
C 
C   Global variables.
C 
      EXTERNAL FUNSUB,R1MACH
      INTEGER NUMFUN,LENW,RESTAR,NEVAL,WRKSUB,NSUB,IFAIL,
     +LIST(WRKSUB),KEY,WORST(WRKSUB),UPOINT(WRKSUB),EMAX,
     +MAXPTS,MINPTS
      REAL A(WRKSUB),B(WRKSUB),EPSABS,EPSREL,RESULT(NUMFUN),
     +ABSERR(NUMFUN),VALUES(NUMFUN,WRKSUB),PERIOD,ERRORS(NUMFUN,WRKSUB),
     +GREATE(WRKSUB),WORK2(LENW),C(WRKSUB),D(WRKSUB),U(NUMFUN,WRKSUB),
     +E(NUMFUN,WRKSUB),GAMMA,EXTERR(NUMFUN),T(NUMFUN),CT(NUMFUN),
     +BETA(0:EMAX,0:EMAX),UNEW(NUMFUN),UERR(NUMFUN),R1MACH
      LOGICAL DONE(NUMFUN)
C 
C   Local variables.
C 
C   SBRGNS is the number of stored sub-intervals.
C   NDIV   The number of sub-intervals to be divided in each main step.
C   POINTR Pointer to the position in the data structure where
C          the new sub-intervals are to be stored.
C   G      is the homogeneous coordinates of the integration rule.
C   W      is the weights of the integration rule and the null rules.
C   TOP    is a pointer to the top element in the heap of sub-intervals.
C   AOLD   Left-endpoint of the interval to be processed.
C   BOLD   Right-endpoint of the interval to be processed.
C   FLAG   Signals small interval.
C   MORE   Signals more work to do.
C   P      Half a period.
C   NUMU   The number of U-terms.
C   NUM    The number of points in the basic rule.
C   NUMNUL The number nullrules used.
C   UPDATE Index to U-term to be updated. If UPDATE < 0, then this means
C          a new extrapolation step.
C   VACANT Index to vacant position in the data-structure.
C   EPMACH Machine epsilon
C   CC     Ratio: the user specified subdivision point B(1)/PERIOD.
      INTEGER I,J,K,JJ,NUMINT,ONE,NUMU,VACANT,UPDATE
      INTEGER SBRGNS,I1,I2,I3,L1,L2,L3
      INTEGER NDIV,POINTR,INDEX,TOP,FLAG,NUM,NUMNUL
      REAL AOLD,BOLD,GREAT,P,LEFT,MAXEER,EPMACH,CC
      LOGICAL MORE
      SAVE MAXEER,NUMU,LEFT,P,CC
      PARAMETER (NUMINT=2,NUM=21,NUMNUL=16)
C 
C***FIRST EXECUTABLE STATEMENT SADINF
C 
C   Define machine epsilon.
C 
      EPMACH=R1MACH(4)
C 
C   Set some pointers for WORK2.
C 
      L1=1
      L2=L1+NUMNUL*NUMFUN
      L3=L2+NUM*NUMFUN
C 
      IF (RESTAR.EQ.1) THEN
         SBRGNS=NSUB
         GO TO 90
      ELSE
         FLAG=0
         CC=B(1)/PERIOD
      END IF
C 
C   Initiate SBRGNS, DONE, MORE, P, A and B.
C 
      MORE=.TRUE.
      SBRGNS=0
      DO 10 J=1,NUMFUN
         DONE(J)=.FALSE.
 10   CONTINUE
      P=PERIOD/2
      A(2)=B(1)
      B(2)=A(2)+P
      LEFT=B(2)
C 
C   Initialize E and U
C 
      DO 20 J=1,NUMFUN
         DO 20 I=1,WRKSUB
         E(J,I)=0
 20      U(J,I)=0
C 
C   Apply the rule procedure  over the two first intervals.
C 
      IF (A(1).EQ.B(1)) THEN
         DO 30 I=1,NUMFUN
            VALUES(I,1)=0
 30         ERRORS(I,1)=0
         GREATE(1)=0
         WORST(1)=1
         ONE=2
         SBRGNS=1
         UPOINT(1)=1
      ELSE
         ONE=1
      END IF
      DO 40 I=ONE,2
         CALL SGAINT (A(I),B(I),NUMFUN,FUNSUB,DONE,MORE,EPMACH,
     +    WORK2(L1),WORK2(L2),WORK2(L3),FLAG,VALUES(1,I),
     +    ERRORS(1,I),GREATE(I),WORST(I),C(I),D(I))
         NEVAL=NEVAL+NUM
         SBRGNS=SBRGNS+1
         UPOINT(I)=I
 40   CONTINUE
C 
C   Initialize RESULT, U, E and NUMU.
C 
      DO 60 I=1,2
         DO 50 J=1,NUMFUN
            U(J,I)=VALUES(J,I)
            E(J,I)=ERRORS(J,I)
 50      CONTINUE
 60   CONTINUE
      NUMU=2
C 
C   Store results in heap.
C 
      DO 80 I=1,NUMINT
         GREAT=0.D0
         DO 70 J=1,NUMFUN
            IF (ERRORS(J,I).GT.GREAT) THEN
               GREAT=ERRORS(J,I)
               WORST(I)=J
            END IF
 70      CONTINUE
         GREATE(I)=GREAT
         C(I)=A(I)+(B(I)-A(I))/3
         D(I)=B(I)-(B(I)-A(I))/3
         INDEX=I
         CALL STRINT (2,INDEX,GREATE,LIST,I)
 80   CONTINUE
C 
C   Extrapolate for the first time based on these 2 terms.
C   This will initialize the global error as well, and
C   MAXEER: the greatest extrapolation error.
C   Finally RESULT will be initialized.
C 
      UPDATE=-1
      CALL SEXINF (NUMFUN,GAMMA,CC,KEY,NUMU-1,T,CT,UPDATE,EMAX,U,E,
     +RESULT,ABSERR,EXTERR,BETA,MAXEER)
C 
C   Check for termination.
C 
      IF (MORE.OR.(NEVAL.LT.MINPTS)) THEN
         GO TO 90
      END IF
      IFAIL=0
      GO TO 170
C 
C***End initialization.
C 
C***Begin loop while the error is too great,
C 
C    First we check the ranking of the TOP element.
C    Then we determine if we will create a new term by integrating
C    from LEFT to LEFT + P and then extrapolate, or subdivide the TOP
C    element in the heap of subregions.
C 
C 
 90   TOP=LIST(1)
C 
C   New term?
C 
      IF (GREATE(TOP).LT.(MAXEER)) THEN
C 
C   We want to extrapolate. Check if we
C   are allowed to take a new extrapolation step.
C 
         IF (NUMU-1.GT.EMAX) THEN
            IFAIL=-1
            GO TO 170
         ELSE
            VACANT=0
            NDIV=1
         END IF
      ELSE
         VACANT=TOP
         NDIV=3
      END IF
C 
C   Check if NEVAL+NDIV*NUM is less than or equal to MAXPTS:
C   MAXPTS is the maximum number of function evaluations that are
C   allowed to be computed.
C 
      IF (NEVAL+NDIV*NUM.LE.MAXPTS) THEN
C 
C   When we pick an interval to divide in three, then one of the new
C   sub-intervals is stored in the original interval's position in the
C   data structure.
C 
C   Let POINTR point to the first free position in the data structure.
C 
C   Determine if this is a new extrapolation step or an update.
C   UPDATE will point to the element in the
C   U-series to be corrected or created.
C 
         IF (VACANT.EQ.0) THEN
            POINTR=SBRGNS+1
            NUMU=NUMU+1
            UPDATE=NUMU
            INDEX=POINTR
            A(INDEX)=LEFT
            LEFT=LEFT+P
            B(INDEX)=LEFT
            TOP=INDEX
            DO 100 J=1,NUMFUN
               UERR(J)=0
 100           UNEW(J)=0
         ELSE
            UPDATE=UPOINT(VACANT)
C 
C   Save the endpoints.
C 
            AOLD=A(TOP)
            BOLD=B(TOP)
C 
C   Remove the top element from the heap.(The value of K does not matter
C 
            POINTR=SBRGNS+2
            CALL STRINT (1,SBRGNS,GREATE,LIST,K)
C 
C   Compute the three new intervals.
C 
            I1=TOP
            I2=POINTR-1
            I3=POINTR
            A(I1)=AOLD
            B(I1)=C(TOP)
            A(I2)=B(I1)
            B(I2)=D(TOP)
            A(I3)=B(I2)
            B(I3)=BOLD
            INDEX=VACANT
            DO 110 J=1,NUMFUN
               UERR(J)=-ERRORS(J,INDEX)
 110           UNEW(J)=-VALUES(J,INDEX)
         END IF
C 
C   Apply basic rule.
C 
         DO 130 I=1,NDIV
            CALL SGAINT (A(INDEX),B(INDEX),NUMFUN,FUNSUB,DONE,MORE,
     +       EPMACH,WORK2(L1),WORK2(L2),WORK2(L3),FLAG,
     +       VALUES(1,INDEX),ERRORS(1,INDEX),GREATE(INDEX),
     +       WORST(INDEX),C(INDEX),D(INDEX))
            UPOINT(INDEX)=UPDATE
C 
C   Compute the U-term
C 
            DO 120 J=1,NUMFUN
               UNEW(J)=UNEW(J)+VALUES(J,INDEX)
               UERR(J)=UERR(J)+ERRORS(J,INDEX)
 120        CONTINUE
            INDEX=SBRGNS+I+1
 130     CONTINUE
         NEVAL=NEVAL+NDIV*NUM
C 
C   Compute the E and U terms (These may be new terms or terms that
C   have to be updated),
C 
         DO 140 J=1,NUMFUN
            U(J,UPDATE)=U(J,UPDATE)+UNEW(J)
            E(J,UPDATE)=E(J,UPDATE)+UERR(J)
 140     CONTINUE
C 
C   Do the extrapolation and compute the global results and errors.
C   UPDATE is used to signal an extrapolation step.
C 
         IF (VACANT.EQ.0) THEN
            UPDATE=-1
         END IF
         CALL SEXINF (NUMFUN,GAMMA,CC,KEY,NUMU-1,T,CT,UPDATE,EMAX,U,E,
     +    RESULT,ABSERR,EXTERR,BETA,MAXEER)
C 
C   Check each integration problem and set DONE(J)=.TRUE. if they
C   are finished. MORE will signal if there is more to do.
C 
         MORE=.FALSE.
         DO 150 J=1,NUMFUN
            IF (ABSERR(J).LE.EPSREL*ABS(RESULT(J)).OR.ABSERR(J).LE.
     +       EPSABS) THEN
               DONE(J)=.TRUE.
            ELSE
               MORE=.TRUE.
               DONE(J)=.FALSE.
            END IF
 150     CONTINUE
C 
C   Store results in heap.
C 
         INDEX=TOP
         DO 160 I=1,NDIV
            JJ=SBRGNS+I
            CALL STRINT (2,JJ,GREATE,LIST,INDEX)
            INDEX=SBRGNS+I+1
 160     CONTINUE
         SBRGNS=SBRGNS+NDIV
C 
C   Check for termination.
C 
         IF (MORE.OR.(NEVAL.LT.MINPTS)) THEN
            GO TO 90
         END IF
C 
C   Else we did not succeed with the
C   given value of MAXPTS. Note: We do not use IFAIL to signal
C   FLAG in the current implementation.
C 
      ELSE
         IFAIL=+1
      END IF
C 
 170  NSUB=SBRGNS
      RETURN
C 
C***END SADINF
C 
      END
