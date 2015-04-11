      SUBROUTINE DEXINF (NUMFUN,GAMMA,C,KEY,N,T,CT,UPDATE,EMAX,U,E,
     +RESULT,ABSERR,EXTERR,BETA,MAXEER)
C***BEGIN PROLOGUE DEXINF
C***KEYWORDS Quasi-linear extrapolation, infinite integration,
C            oscillating functions, error estimation.
C***PURPOSE  To compute better estimates to a vector of approximations
C            to integrals of oscillating functions over an infinite
C            interval and to provide new and updated error estimates.
C 
C***LAST MODIFICATION 94-03-11
C 
C***DESCRIPTION The routine uses linear extrapolation to compute better
C            approximations to each component in a vector of
C            integrals. All components are assumed to be integrals of
C            oscillating functions which asymptotically have the same
C            behavior at infinity.
C            A series approach is used, assuming
C            that the terms are given with estimates of the error in
C            each term. New error estimates are computed too. The
C            routine have two options: EITHER a new extrapolation term
C            is provided and we take a new extrapolation step,
C            OR we update one previously computed term in the series and
C            therefore have to update the extrapolation tableau.
C 
C   ON ENTRY
C 
C     NUMFUN Integer.
C            The number of components of the integral.
C     GAMMA  Real.
C            Asymptotic decay of the functions; 1/x**(GAMMA), GAMMA>0.
C     C      Real.
C            Reference point: User specified subdivision point/period.
C     KEY    Integer
C            Choice of extrapolation method:
C            KEY = 0 : Euler's method.
C            KEY = 1 : Modified Euler.
C            KEY = 2 : Overholt's polynomial order 2-method.
C                      This last method is the only one that needs
C                      the value of GAMMA.
C     N      Integer
C            The number of U-terms in the series: 0,1,2,...,N.
C            Makes it possible to perform N extrapolation steps.
C     T      Real array of dimension (NUMFUN,0:EMAX)
C            Contains the last row in the extrapolation tableau for
C            each function in the vector.
C     CT     Real array of dimension NUMFUN.
C            Contains the element to the left of the diagonal element
C            in the extrapolation tableau.
C     UPDATE Integer
C            If < 0 then this is a new extrapolation step.
C            If >= 1 then this is  a step where we have to correct the
C            existing tableau. The value of UPDATE gives the index to
C            the U-element that has been modified.
C     EMAX   Integer
C            The maximum allowed number of extrapolation steps.
C     U      Real array of dimension (NUMFUN,0:N)
C 
C     E      Real array of dimension (NUMFUN,0:N)
C            The estimated errors of all U-terms in the series.
C   ON RETURN
C 
C     T      Real array of dimension (NUMFUN:0:EMAX)
C            Contains the diagonal element in the extrapolation tableau
C            for each function in the vector after the extrapolation.
C     CT     Real array of dimension NUMFUN.
C            Contains the element to the left of the diagonal element
C            in the extrapolation tableau.
C     RESULT Real array of dimension NUMFUN
C            Contains the new approximations for the NUMFUN components
C            of the integral.
C     ABSERR Real array of dimension NUMFUN.
C            Returns the global errors for all components.
C            This includes both the pure extrapolation
C            error and the effect of not knowing the U-terms exactly.
C     EXTERR Real array of dimension NUMFUN.
C            These errors are the pure extrapolation errors.
C     BETA   Real Array of dimension (EMAX +1)*(EMAX +1)
C            A table of coefficients to be used in the extrapolation
C            and the error estimation.
C     MAXEER Real.
C            The maximum extrapolation error.
C***REFERENCES
C 
C   T.O.Espelid and K.J.Overholt, DQAINF: An Algorithm for Automatic
C   Integration of Infinite Oscillating Tails,
C   submitted for publication 1993.
C 
C   K.J.Overholt: The P-algorithms for extrapolation,
C   Department of Informatics, Report No. 36, 1989.
C 
C***ROUTINES CALLED NONE
C***END PROLOGUE DEXINF
C 
C   Global variables.
C 
      INTEGER N,EMAX,UPDATE,NUMFUN,KEY
      DOUBLE PRECISION GAMMA,T(NUMFUN),CT(NUMFUN),C,U(NUMFUN,0:N),
     +E(NUMFUN,0:N),BETA(0:EMAX,0:EMAX),MAXEER,RESULT(NUMFUN),
     +ABSERR(NUMFUN),EXTERR(NUMFUN)
C 
C   Local variables.
C 
      INTEGER I,J
      DOUBLE PRECISION SAVE1,SAVE2,CONST,BASE1,BASE2,P,Q
      PARAMETER (CONST=1.D0)
C 
C  CONST heuristic constant used in the error estimation.
C 
C***FIRST EXECUTABLE STATEMENT DEXINF
C 
C   Initialize the beta-factors.
C 
      BETA(0,0)=1
      IF (KEY.EQ.2) THEN
         BASE1=GAMMA
         BASE2=MAX(GAMMA-1,4*C)
         P=2.D0
      ELSE
         BASE1=0.D0
         BASE2=MAX(0.D0,4*C)
         P=1.D0
      END IF
C 
C   Compute the new extrapolation coefficients.
C 
      IF (UPDATE.LT.0) THEN
         BETA(0,N)=1
         BETA(N,0)=1
         DO 20 I=1,N-1
            SAVE1=1
            DO 10 J=N-I,N-1
               IF (KEY.EQ.0) THEN
                  Q=0.5D0
               ELSE
                  Q=(1+(-BASE1-P*J)/(2*N+BASE2))/2.D0
               END IF
               SAVE2=SAVE1-(SAVE1-BETA(I,J))*Q
               BETA(I,J)=SAVE1
 10            SAVE1=SAVE2
            BETA(I,N)=SAVE1
 20      CONTINUE
         DO 30 J=1,N
            IF (KEY.EQ.0) THEN
               Q=0.5D0
            ELSE
               Q=(1+(-BASE1-P*(J-1))/(2*N+BASE2))/2.D0
            END IF
 30         BETA(N,J)=(1-Q)*BETA(N,J-1)
      END IF
C 
C   Extrapolate; use the extrapolation coefficients.
C 
      DO 40 J=1,NUMFUN
         T(J)=0
         CT(J)=0
         DO 40 I=N,0,-1
         CT(J)=CT(J)+BETA(I,N-1)*U(J,I)
 40      T(J)=T(J)+BETA(I,N)*U(J,I)
C 
C   Then compute the error estimates.
C   First the extrapolation error and then the U-effect.
C   The maximum extrapolation error is computed too.
C 
      MAXEER=0
      DO 60 J=1,NUMFUN
         EXTERR(J)=CONST*ABS(T(J)-CT(J))/Q
         MAXEER=MAX(EXTERR(J),MAXEER)
C 
C   Note: The last U-errors are effected by the extrapolation-process
C   Accumulate the total error in exterr.
C 
         DO 50 I=0,N
 50         EXTERR(J)=EXTERR(J)+BETA(I,N)*E(J,I)
 60   CONTINUE
C 
C   Define the results: update the results only if the error is improved
C 
      DO 70 J=1,NUMFUN
         IF ((EXTERR(J).LE.ABSERR(J)).OR.(ABSERR(J).EQ.0)) THEN
            RESULT(J)=T(J)
            ABSERR(J)=EXTERR(J)
         END IF
 70   CONTINUE
      RETURN
C 
C***END DEXINF
C 
      END
