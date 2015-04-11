      SUBROUTINE DGAINF (NUMFUN,F,B,PERIOD,GAMMA,NEVAL,IFAIL)
C 
C***BEGIN PROLOGUE DGAINF
C***KEYWORDS Linear extrapolation,
C            oscillating functions, asymptotic behavior.
C***PURPOSE  To compute an estimate of the exponent GAMMA which governs
C            the asymptotic behavior of an oscillating function:
C            F(X+PERIOD*K) appr. C(X)/(X+PERIOD*K)**GAMMA, for all X
C            and all integers K sufficiently great.
C            The function is assumed to
C            have asymptotical oscillating behavior and
C            F(X+PERIOD*(K+1/2)) approx. - F(X+PERIOD*K).
C 
C***LAST MODIFICATION 93-01-31
C 
C***REFER TO DQAINF
C 
C***DESCRIPTION Computation of the exponent GAMMA > 0 which governs
C   the asymptotic behavior of the functions F. All functions in the
C   vector F are assumed to be asymptotically periodic with the same
C   PERIOD and the same asymptotical behavior. Furthermore we assume
C   that for all functions in the vector we have
C   F(X+PERIOD*K) approx. C(X)/(X+PERIOD*K)**GAMMA, for all values of X
C   and K, such that (X+PERIOD*K) >> B. C may be zero for some values of
C   X. Assuming that C is non-zero then we have that
C   F(X+2*PERIOD*K)/F(X+PERIOD*K) approx. (1 + D)**GAMMA, with
C   D = PERIOD*K/(X+PERIOD*K).
C 
C   ON ENTRY
C 
C     NUMFUN Integer.
C            The number of components of the integral.
C     F      Externally declared subroutine for computing
C            all components of the integrand at all NP
C            evaluation points.
C            It must have parameters (X,NUMFUN,NP,FUNVLS)
C            Input parameters:
C              X      The NP x-coordinates of the evaluation points.
C              NUMFUN Integer that defines the number of
C                     components of I.
C              NP     Integer that gives the number of evaluation points
C            Output parameter:
C              FUNVLS Real array of dimension (NUMFUN,NP)
C                     that defines NUMFUN components of the integrand fo
C                     each evaluation point.
C 
C     B      Real.
C            Asymptotic behavior assumed satisfied for all X >= B and
C            for all the integrands.
C     PERIOD Real.
C            All functions are assumed to be asymptotically periodic,
C            and with the same period.
C 
C  ON RETURN
C 
C     GAMMA  Real
C            Estimated value of the rate of decay of the functions
C            when X tends to +infinity.
C     NEVAL  Integer
C            The number of function evaluations.
C     IFAIL  Integer.
C            IFAIL = 0 for normal exit.
C            IFAIL = 12 if either PERIOD is wrong or B(1) is too small.
C            IFAIL = 13 if unable to estimate GAMMA (In case KEY=2 only)
C 
C***ROUTINES CALLED F,D1MACH
C***END PROLOGUE DGAINF
C 
C   Global variables.
C 
      EXTERNAL F,D1MACH
      DOUBLE PRECISION PERIOD,GAMMA,B,D1MACH
      INTEGER NUMFUN,IFAIL,NEVAL
C 
C   Local variables.
C 
      DOUBLE PRECISION P,X,H,XB,FX,FXP,XR,FR,EPMACH,FOLD,FNEW,SIGN,XOLD,
     +XNEW
      INTEGER I,IMX,M
      PARAMETER (IMX=15)
      DOUBLE PRECISION A(0:IMX),DA,FACTOR(0:IMX),FI(0:IMX),DF,LAST
C 
C***FIRST EXECUTABLE STATEMENT DGAINF
C 
      EPMACH=D1MACH(4)
C 
C   First: compute a starting point X >= B where the function value F(X)
C   is reasonably large in absolute value. Remark: We simply want to
C   avoid that F(X) becomes approximately 0.
C 
      XOLD=B
      CALL F (XOLD,1,1,FOLD)
      P=PERIOD/4
      XNEW=XOLD
      DO 10 I=1,3,1
         XNEW=XNEW+P
         CALL F (XNEW,1,1,FNEW)
         IF (ABS(FOLD).LT.ABS(FNEW)) THEN
            XOLD=XNEW
            IF ((FOLD*FNEW).GT.0) THEN
               P=-P
            END IF
            FOLD=FNEW
         ELSE
            IF ((FOLD*FNEW).GT.0) THEN
               XNEW=XOLD
            END IF
         END IF
         P=P/2
 10   CONTINUE
      X=XOLD
      FX=FOLD
C 
C   Check that the function values do change sign
C   and that the absolute value of the function is decreasing
C   when evaluated at points which differ by PERIOD*integer/2.
C 
      CALL F (X+PERIOD/2,1,1,FXP)
      NEVAL=5
      LAST=FXP
      SIGN=FX
      FI(0)=X
      IF (((SIGN*FXP).LT.0).AND.(ABS(FX).GT.ABS(FXP))) THEN
         FR=-FXP/FX
         XR=X/(X+PERIOD/2)
         A(0)=LOG(FR)/LOG(XR)
         FACTOR(0)=A(0)*(1/ABS(FR*LOG(FR))+1/ABS(XR*LOG(XR)))*EPMACH
      ELSE
         IFAIL=12
         RETURN
      END IF
C 
C   Compute a sequence of estimates to GAMMA and extrapolate.
C 
C   FACTOR is meant to keep track of the increase in the error due to
C   the extrapolation. We assume that PERIOD is known correctly to full
C   double precision. If X is an exact local extremum of F(X) then it is
C   not important to know PERIOD with such a high precision.
C   However, since X is just a point where ABS(F(X)) is not too small
C   the quality of PERIOD becomes essential when we estimate GAMMA.
C 
      H=PERIOD/2
      XB=X
      DO 30 I=1,IMX
         H=2*H
         X=XB+H
         FI(I)=X
         CALL F (X+PERIOD/2,1,1,FXP)
         CALL F (X,1,1,FX)
         NEVAL=NEVAL+2
C 
C   Check the computed function values.
C 
         IF (((SIGN*FX).GT.0).AND.((SIGN*FXP).LT.0).AND.(ABS(FX).GT.
     +    ABS(FXP)).AND.(ABS(LAST).GT.ABS(FX))) THEN
            LAST=FXP
            FR=-FXP/FX
            XR=X/(X+PERIOD/2)
            A(I)=LOG(FR)/LOG(XR)
            FACTOR(I)=A(0)*(2**I/ABS(FR*LOG(FR))+1/ABS(XR*LOG(XR)))*
     +       EPMACH
         ELSE
C 
C   Either PERIOD is wrong or B is too small.
C 
            IFAIL=12
            RETURN
         END IF
         DO 20 M=I-1,0,-1
            DA=(A(M+1)-A(M))/((FI(I)-FI(M))/FI(M))
            DF=(FACTOR(M+1)+FACTOR(M))/((FI(I)-FI(M))/FI(M))
            A(M)=A(M+1)+DA
            FACTOR(M)=FACTOR(M+1)+DF
 20      CONTINUE
         IF (ABS(DA).LE.FACTOR(0)) THEN
            GAMMA=A(0)
C 
C   Normal return
C 
            RETURN
         END IF
 30   CONTINUE
C 
C   We did not succeed in estimating GAMMA to sufficient precision.
C 
      GAMMA=A(0)
      IFAIL=13
      RETURN
C 
C***END DGAINF
C 
      END
