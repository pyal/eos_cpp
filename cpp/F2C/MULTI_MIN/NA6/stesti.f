C
C   The following program will approximate three integrals over the
C   interval (0,infinity) and print out the values of the estimated
C   integrals, the estimated errors, the number of function evaluations
C   and IFAIL.
C
      PROGRAM STESTI
C
C   This demo-program has calls to the main driver: SQAINF
C   Through this main driver the rest of the modules are activated:
C
C   level 1: SQAINF
C                               
C   level 2: SCHINF and  SADINF
C                              
C   level 3: SGAINF, SGAINT, SEXINF, STRINT
C
C   level 4: R1MACH (from blas in netlib) and FUNSUB (user specified)
C
      EXTERNAL FUNSUB
      INTEGER NUMFUN,NW,MINPTS,MAXPTS,KEY,RESTAR
      INTEGER WRKSUB,EMAX,NUMNUL,NUM
C
C   For the user's convenience we give a value to several parameters
C   through the parameter statement. 
C    The following two parameters the user should not change,
C   unless the user changes the code appropriately:
C
      PARAMETER (NUM=21,NUMNUL=16)
C
C   The following three parameters the user may choose and these have
C   to remain unchanged through the entire run:
C   NUMFUN: the number of functions you want to integrate;
C   WRKSUB: the maximum number of subregions allowed;
C   EMAX:   the maximum number of extrapolation steps;
C
      PARAMETER (NUMFUN=3,WRKSUB=100,EMAX=25)
C
C   Based on the previous 5 parameters we can now compute the
C   size of the main working space: NW.
C
      PARAMETER (NW=1+5*NUMFUN+4*WRKSUB*NUMFUN+3*WRKSUB+(EMAX+1)**2+
     +(NUMNUL+1+NUM)*NUMFUN)
C
C   Next we choose the method to be used:
C   Three options: KEY = 0(Euler), 1(Euler modified), 2(Overholt's
C   P-order 2)
C
      PARAMETER (KEY=2)
C
C   We need to choose WRKSUB  such that:
C   WRKSUB >= MAXPTS/NUM (We have NUM=21 fixed in this implementation)
C   This simply means that we have enough space to achieve MAXPTS
C   function evaluations. By choosing WRKSUB bigger than
C   this lower bound we may later change MAXPTS and RESTAR and restart
C   the computations from the point where the last run stopped.
C   The reason for stopping may have been that MAXPTS was too small
C   to achieve the requested error.
C          With our choice WRKSUB = 100 then any value of MAXPTS
C   between MINPTS and 2100 will be accepted by the code. Choosing
C   MAXPTS = 1000 then allows us to restart with a greater value
C   if necessary.
C   Note: For a given problem it may happen that the code stops after
C   reaching say MAXPTS=2100 function evaluations, having used less than
C   WRKSUB = 100 subregions. The reason for this is that if a given
C   region needs to be further subdivided then the old region will not
C   be stored while the function values computed over that region
C   will be counted.
C      The example gave the following output on
C   a SUN Sparc station 10:
C 
C   A=  0. B=    3.00000
C   GAMMA=    1.00000 PERIOD=    6.28319
C   MAXPTS=  300 WRKSUB=  100 NW=  2306
C   KEY=  2 EPSREL=    1.00000E-04
C
C   Result and error: problem no. 1, 2 and 3:
C        0.12431083E+01    0.71825701E+00    0.48923481E+00
C        0.25305973E-04    0.27054797E-04    0.26731776E-04
C   NEVAL=  126 IFAIL=  0
C 
      REAL A(WRKSUB),B(WRKSUB),EPSABS,EPSREL,GAMMA,PERIOD,
     +RESULT(NUMFUN),ABSERR(NUMFUN),WORK(NW),PI
      INTEGER NEVAL,IFAIL,IWORK(3*WRKSUB)
      LOGICAL DONE(NUMFUN)
      PI=2*ASIN(1.E0)
C
C   We specify the period and the decay parameter GAMMA>0. If
C   the value of GAMMA < 0 then the code will attempt to estimate
C   GAMMA in the case KEY = 2. GAMMA is not used by the other two
C   methods.
C
      PERIOD=2*PI
      GAMMA=1.0E0
C
C   Next we give the left endpoint of integration and the point b
C   from where we expect the theory to be valid. Advice: Since b
C   is not uniquely defined please do some experiments with
C   different values of b to gain  experience.
C   The effect one achieves by increasing b depends on the value of KEY. 
C
      A(1)=0
      B(1)=3
C
C  This is a first attempt: set RESTAR, give MINPTS, MAXPTS and the
C  absolute and relative error requests.
C
      RESTAR=0
      MINPTS=42
      MAXPTS=300
      EPSABS=0
      EPSREL=1.E-4
C
C  Note: these four parameters and RESTAR may be changed in a restart run
C
      WRITE (*,*) 'A=',A(1),' B=',B(1)
      WRITE (*,*) 'GAMMA=',GAMMA,' PERIOD=',PERIOD
      WRITE (*,*) 'MAXPTS=',MAXPTS,' WRKSUB=',WRKSUB,' NW=',NW
      WRITE (*,*) 'KEY=',KEY,' EPSREL=',EPSREL
      CALL SQAINF (NUMFUN,FUNSUB,PERIOD,GAMMA,A,B,WRKSUB,EMAX,MINPTS,
     +MAXPTS,EPSABS,EPSREL,NW,KEY,RESTAR,RESULT,ABSERR,NEVAL,IFAIL,DONE,
     +WORK,IWORK)
      WRITE (*,*)
      WRITE (*,*) 'Result and error: problem no. 1, 2 and 3:'
      WRITE (*,10) RESULT
      WRITE (*,10) ABSERR
      WRITE (*,*) 'NEVAL=',NEVAL,' IFAIL=',IFAIL
 10   FORMAT (' ',3E16.8)
      END
      SUBROUTINE FUNSUB (X,NUMFUN,NP,FUNVLS)
      INTEGER NUMFUN,I,NP,J
      REAL X(NP),FUNVLS(NP,1),Y
C
C     This subroutine must be provided by the user for computing
C            all components of the integrand at the given
C            evaluation points.
C            It must have parameters (X,NUMFUN,NP,FUNVLS)
C            Input parameters:
C              X      Real array of dimension NP (= 21) defining the
C                     x-coordinates of the evaluation points.
C              NUMFUN Integer that defines the number of
C                     components of the integral I.
C              NP     Integer that gives the number of evaluation points
C                     in the quadrature rule used (Gauss, 21 point rule).
C            Output parameter:
C              FUNVLS Real array of dimension (NP,NUMFUN) that defines
C                     the function values in the 21 evaluation points
C                     for the NUMFUN components of the integrand.
C 
C     Note that we may save computer time when integrating
C     several functions simultaneously over the same interval
C     if we take advantage of the functions' similarities.
C 
      DO 10 I=1,NP
         Y=(2*SIN(X(I))+X(I)*COS(X(I)))
         DO 10 J=1,NUMFUN
         FUNVLS(I,J)=Y/(J+X(I)**2)
 10   CONTINUE
      RETURN
      END
