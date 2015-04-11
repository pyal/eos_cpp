      SUBROUTINE SGAINT (A,B,NUMFUN,FUNSUB,DONE,MORE,EPMACH,NULL,
     +FUNVAL,BASABS,FLAG,BASVAL,RGNERR,GREATE,WORST,C,D)
C***BEGIN PROLOGUE SGAINT
C***PURPOSE  To compute basic integration rule values and
C            corresponding error estimates.
C
C***LAST MODIFICATION 94-03-10
C
C***REFER TO SQAINF
C
C***DESCRIPTION SGAINT computes basic integration rule values
C            for a vector of integrands over an interval.
C            SGAINT also computes estimates for the errors by
C            using several null rule approximations.
C   ON ENTRY
C 
C   A    Real.
C   B    Real.
C          The left and right endpoints of the interval.
C   NUMFUN Integer.
C          Number of components of the vector integrand.
C   FUNSUB Externally declared subroutine for computing
C          all components of the integrand at the given
C          evaluation point.
C            It must have parameters (X,NUMFUN,NP,FUNVLS)
C            Input parameters:
C              X      The x-coordinates of all 21 evaluation points.
C              NUMFUN Integer that defines the number of
C                     components of I.
C              NP     Integer that gives the number of evaluation points
C            Output parameter:
C              FUNVLS Real array of dimension (21,NUMFUN)
C                     that defines NUMFUN components of the integrand
C                     evaluated in all 21 evaluation points.
C 
C   DONE   Logical array of dimension NUMFUN.
C          DONE(I)=.TRUE. if function number I has been
C          integrated to the specified precision, else DONE(I)=.FALSE.
C   MORE   Logical.
C          Information about the fact that there is still work to do.
C   EPMACH Real.
C          Machine epsilon.
C   NULL   Real array of dimension (16,NUMFUN).
C          A work array.
C   FUNVAL Real array of dimension (21,NUMFUN).
C          A work array.
C   BASABS Real array of dimension NUMFUN.
C          A work array.
C   ON RETURN
C 
C   FLAG   Integer.
C          Signals that at least one interval has become too small
C          to distinguish between a rule evaluation point and an
C          endpoint.
C 
C   BASVAL Real array of dimension NUMFUN.
C          The values for the basic rule for each component
C          of the integrand.
C   RGNERR Real array of dimension NUMFUN.
C          The error estimates for each component of the integrand.
C   GREATE Real.
C          The greatest error component of the integrand.
C   WORST  Index to the greatest error in each sub-interval.
C 
C   C      Real.
C   D      Real
C          C and D are subdivision points based on
C          uniform trisection: C = A + (B-A)/3 and D = B - (B-a)/3.
C 
C***REFERENCES Espelid, T. O., Integration Rules, Null Rules and
C          Error Estimation, Report no 33, Informatics, Univ. of Bergen,
C          1988.
C 
C          Berntsen,J. and Espelid,T.O., Error estimation in Automatic
C          Quadrature Routines, ACM Trans. Math. Softw.,
C          17, 2, 233-252, 1991.
C 
C          Espelid, T. O., DQAINT: An Algorithm for Adaptive Quadrature
C          over a Collection of Finite Intervals, in Numerical
C          Integration,  Recent Developments, Software and Applications,
C          Espelid and Genz (eds),  NATO ASI Series C:
C          Mathematical and Physical Sciences - Vol. 357,
C          Kluwer, Dordrecht, The Netherlands, pages 341-342, 1992.
C 
C***ROUTINES CALLED FUNSUB
C***END PROLOGUE SGAINT
C 
C   Global variables.
C 
      EXTERNAL FUNSUB
      LOGICAL DONE(NUMFUN),MORE
      INTEGER NUMFUN,FLAG,WORST
      REAL A,B,BASVAL(NUMFUN),RGNERR(NUMFUN),NULL(16,
     +NUMFUN),GREATE,C,D,BASABS(NUMFUN),FUNVAL(21,NUMFUN),EPMACH
C 
C   Local variables.
C 
C   WTLENG The number of weights of the integration rule.
C   G      Real array of dimension WTLENG.
C          The coordinates for the generators of
C          the evaluation points.
C          The integration rule is using symmetric evaluation
C          points and has the structure (1,6,3). That is,
C          1 point of multiplicity 1,
C          6 sets of points of multiplicity 3 and
C          3 sets of points of multiplicity 6.
C          This gives totally 37 evaluation points.
C          In order to reduce the number of loops in SGAINT,
C          the 3 loops for the sets of multiplicity 6 are split
C          into 6 loops and added to the loops for the sets of
C          multiplicity 3.
C          The number of weights we have to give with
C          this splitting is 13(WTLENG).
C 
C   W      Real array of dimension (9,WTLENG).
C          The weights of the basic rule and the null rules.
C          W(1),...,W(WTLENG) are weights for the basic rule.
C 
C   X      Real array of dimension 21.
C          Evaluation points in (A,B).
C   HLEN   Real.
C          Half of intervals length
      INTEGER I,I1,I2,J,K,WTLENG,DGE1,DEGREE,NUMNUL,DIMNUL
      PARAMETER (NUMNUL=8,DIMNUL=16)
      REAL X(21),RR(NUMNUL/2-1),R,NOISE,E(NUMNUL/2),EMAX,
     +ALPHA,CONST,W(11),ABSCIS,HLEN,SAFETY,RCRIT,ABSSUM,SUM,DIFF,CENTR,
     +G(11),FACTOR,NULLW(16,11)
      PARAMETER (WTLENG=11,FACTOR=0.02D+0,SAFETY=10.E0,RCRIT=0.5E0,DGE1=
     +18,DEGREE=41,ALPHA=(DEGREE-DGE1)/2.E0)
C 
C  The abscissas to the 21 point Gauss integration rule.
C  The abscissas are given in -1,1: due to the symmetry we only specify
C  values in 0,1.
C 
      DATA (G(I),I=1,11)/0.9937521706203895D0,0.9672268385663062D0,0.
     +9200993341504008D0,0.8533633645833172D0,0.7684399634756779D0,0.
     +6671388041974123D0,0.5516188358872198D0,0.4243421202074387D0,0.
     +2880213168024010D0,0.1455618541608950D0,0.000000000000000D0/
C 
C   Weights of the 21 point Gauss quadrature rule. Same remark
C   about symmetry.
C 
      DATA (W(I),I=1,11)/0.01601722825777433D0,0.03695378977085249D0,0.
     +05713442542685720D0,0.07610011362837930D0,0.09344442345603386D0,0.
     +1087972991671483D0,0.1218314160537285D0,0.1322689386333374D0,0.
     +1398873947910731D0,0.1445244039899700D0,0.1460811336496904D0/
C 
C   Weights of the 5 symmetric nullrules of degrees 19,17,15,13,11
C   Weights of the 5 asymmetric nullrules of degrees 18,16,14,12,10
C   Same remark about symmetry as above. The nullrules are all
C   orthogonal and has the same norm as the basic rule and are
C   given with decreasingly degrees.
C 
      DATA NULLW(1,1)/0.5859224694730026D-02/
      DATA NULLW(1,2)/-0.2024707000741622D-01/
      DATA NULLW(1,3)/0.3883580247121445D-01/
      DATA NULLW(1,4)/-0.5965412595242497D-01/
      DATA NULLW(1,5)/0.8114279498343020D-01/
      DATA NULLW(1,6)/-0.1019231472030305D+00/
      DATA NULLW(1,7)/0.1207652963052454D+00/
      DATA NULLW(1,8)/-0.1366043796711610D+00/
      DATA NULLW(1,9)/0.1485698262567817D+00/
      DATA NULLW(1,10)/-0.1560150459859118D+00/
      DATA NULLW(1,11)/0.1585416482170856D+00/
      DATA NULLW(2,1)/0.1301976799706014D-01/
      DATA NULLW(2,2)/-0.4379005851020851D-01/
      DATA NULLW(2,3)/0.7990096087086482D-01/
      DATA NULLW(2,4)/-0.1138307201442027D+00/
      DATA NULLW(2,5)/0.1394263659262734D+00/
      DATA NULLW(2,6)/-0.1520456605731098D+00/
      DATA NULLW(2,7)/0.1489588260146731D+00/
      DATA NULLW(2,8)/-0.1296181347851887D+00/
      DATA NULLW(2,9)/0.9568420420614478D-01/
      DATA NULLW(2,10)/-0.5078074459100106D-01/
      DATA NULLW(2,11)/0.0000000000000000D+00/
      DATA NULLW(3,1)/0.2158184987561927D-01/
      DATA NULLW(3,2)/-0.6965227115767195D-01/
      DATA NULLW(3,3)/0.1174438965053943D+00/
      DATA NULLW(3,4)/-0.1473794001233916D+00/
      DATA NULLW(3,5)/0.1481989091733945D+00/
      DATA NULLW(3,6)/-0.1168273220215079D+00/
      DATA NULLW(3,7)/0.5890214560028095D-01/
      DATA NULLW(3,8)/0.1273585156460484D-01/
      DATA NULLW(3,9)/-0.8133037350927629D-01/
      DATA NULLW(3,10)/0.1304777802379009D+00/
      DATA NULLW(3,11)/-0.1483021322906934D+00/
      DATA NULLW(4,1)/0.3119657617222001D-01/
      DATA NULLW(4,2)/-0.9516116459787523D-01/
      DATA NULLW(4,3)/0.1431705707841666D+00/
      DATA NULLW(4,4)/-0.1462171986707822D+00/
      DATA NULLW(4,5)/0.9677919508585969D-01/
      DATA NULLW(4,6)/-0.1075583592960879D-01/
      DATA NULLW(4,7)/-0.7936141880173113D-01/
      DATA NULLW(4,8)/0.1380749552472930D+00/
      DATA NULLW(4,9)/-0.1417577117227090D+00/
      DATA NULLW(4,10)/0.8867798725104829D-01/
      DATA NULLW(4,11)/0.0000000000000000D+00/
      DATA NULLW(5,1)/0.4157639307601386D-01/
      DATA NULLW(5,2)/-0.1179114894020921D+00/
      DATA NULLW(5,3)/0.1511566572815612D+00/
      DATA NULLW(5,4)/-0.1073644825716617D+00/
      DATA NULLW(5,5)/0.4174411212397235D-02/
      DATA NULLW(5,6)/0.1012057375471417D+00/
      DATA NULLW(5,7)/-0.1472858866418607D+00/
      DATA NULLW(5,8)/0.1063772962797608D+00/
      DATA NULLW(5,9)/-0.2323645744823691D-02/
      DATA NULLW(5,10)/-0.1030910117645103D+00/
      DATA NULLW(5,11)/0.1469720414561505D+00/
      DATA NULLW(6,1)/0.5246625962340516D-01/
      DATA NULLW(6,2)/-0.1358182960749584D+00/
      DATA NULLW(6,3)/0.1386355746642898D+00/
      DATA NULLW(6,4)/-0.3967547044517777D-01/
      DATA NULLW(6,5)/-0.8983329656061084D-01/
      DATA NULLW(6,6)/0.1471801958801420D+00/
      DATA NULLW(6,7)/-0.8524048604745531D-01/
      DATA NULLW(6,8)/-0.4617298114857220D-01/
      DATA NULLW(6,9)/0.1397282757969823D+00/
      DATA NULLW(6,10)/-0.1185867937861861D+00/
      DATA NULLW(6,11)/0.0000000000000000D+00/
      DATA NULLW(7,1)/0.6363031447247006D-01/
      DATA NULLW(7,2)/-0.1472028208379138D+00/
      DATA NULLW(7,3)/0.1063757528761779D+00/
      DATA NULLW(7,4)/0.3881687506910375D-01/
      DATA NULLW(7,5)/-0.1432999618142209D+00/
      DATA NULLW(7,6)/0.9698969424297650D-01/
      DATA NULLW(7,7)/0.5209450556829039D-01/
      DATA NULLW(7,8)/-0.1455658951943161D+00/
      DATA NULLW(7,9)/0.8343286549711612D-01/
      DATA NULLW(7,10)/0.6800562635441401D-01/
      DATA NULLW(7,11)/-0.1465539124681842D+00/
      DATA NULLW(8,1)/0.7484599067063009D-01/
      DATA NULLW(8,2)/-0.1508776892345244D+00/
      DATA NULLW(8,3)/0.5853283458897407D-01/
      DATA NULLW(8,4)/0.1062457136342151D+00/
      DATA NULLW(8,5)/-0.1318732622123368D+00/
      DATA NULLW(8,6)/-0.1673118490576078D-01/
      DATA NULLW(8,7)/0.1428979325476485D+00/
      DATA NULLW(8,8)/-0.7818432195969258D-01/
      DATA NULLW(8,9)/-0.9112601052788798D-01/
      DATA NULLW(8,10)/0.1382849496064090D+00/
      DATA NULLW(8,11)/0.0000000000000000D+00/
      DATA NULLW(9,1)/0.8590167508061712D-01/
      DATA NULLW(9,2)/-0.1462121283895959D+00/
      DATA NULLW(9,3)/0.1973066649848703D-02/
      DATA NULLW(9,4)/0.1434120884358229D+00/
      DATA NULLW(9,5)/-0.6050045998747565D-01/
      DATA NULLW(9,6)/-0.1192968264851738D+00/
      DATA NULLW(9,7)/0.1063582979588903D+00/
      DATA NULLW(9,8)/0.7871971420591506D-01/
      DATA NULLW(9,9)/-0.1360664606736734D+00/
      DATA NULLW(9,10)/-0.2747426951466367D-01/
      DATA NULLW(9,11)/0.1463706054390223D+00/
      DATA NULLW(10,1)/0.9659618304508728D-01/
      DATA NULLW(10,2)/-0.1331667766592828D+00/
      DATA NULLW(10,3)/-0.5483608118503819D-01/
      DATA NULLW(10,4)/0.1395396581193282D+00/
      DATA NULLW(10,5)/0.3842219337177220D-01/
      DATA NULLW(10,6)/-0.1430606163131484D+00/
      DATA NULLW(10,7)/-0.2498840938693774D-01/
      DATA NULLW(10,8)/0.1451753725543706D+00/
      DATA NULLW(10,9)/0.1236834040097046D-01/
      DATA NULLW(10,10)/-0.1461902970879641D+00/
      DATA NULLW(10,11)/0.0000000000000000D+00/
      DATA NULLW(11,1)/0.1067392105869384D+00/
      DATA NULLW(11,2)/-0.1122928178247447D+00/
      DATA NULLW(11,3)/-0.1031959020477783D+00/
      DATA NULLW(11,4)/0.9558143541939021D-01/
      DATA NULLW(11,5)/0.1196951864405313D+00/
      DATA NULLW(11,6)/-0.7225984000378730D-01/
      DATA NULLW(11,7)/-0.1339424732473705D+00/
      DATA NULLW(11,8)/0.4492456833975673D-01/
      DATA NULLW(11,9)/0.1431238351576778D+00/
      DATA NULLW(11,10)/-0.1523606417516131D-01/
      DATA NULLW(11,11)/-0.1462742772906911D+00/
      DATA NULLW(12,1)/0.1161523191050730D+00/
      DATA NULLW(12,2)/-0.8469391287377601D-01/
      DATA NULLW(12,3)/-0.1355896186086413D+00/
      DATA NULLW(12,4)/0.2408868272651161D-01/
      DATA NULLW(12,5)/0.1460359069105494D+00/
      DATA NULLW(12,6)/0.4632194803727984D-01/
      DATA NULLW(12,7)/-0.1231814607655799D+00/
      DATA NULLW(12,8)/-0.1068762140630544D+00/
      DATA NULLW(12,9)/0.7029919038187181D-01/
      DATA NULLW(12,10)/0.1416700606593806D+00/
      DATA NULLW(12,11)/0.0000000000000000D+00/
      DATA NULLW(13,1)/0.1246701955330830D+00/
      DATA NULLW(13,2)/-0.5195253287985397D-01/
      DATA NULLW(13,3)/-0.1469123277046623D+00/
      DATA NULLW(13,4)/-0.5433985749387003D-01/
      DATA NULLW(13,5)/0.1052913655334742D+00/
      DATA NULLW(13,6)/0.1341759283651172D+00/
      DATA NULLW(13,7)/-0.2310968036052471D-02/
      DATA NULLW(13,8)/-0.1358135230198954D+00/
      DATA NULLW(13,9)/-0.1024826424015055D+00/
      DATA NULLW(13,10)/0.5656562071840163D-01/
      DATA NULLW(13,11)/0.1462174827723311D+00/
      DATA NULLW(14,1)/0.1321420280297885D+00/
      DATA NULLW(14,2)/-0.1602500237425218D-01/
      DATA NULLW(14,3)/-0.1353193782985104D+00/
      DATA NULLW(14,4)/-0.1170027382391999D+00/
      DATA NULLW(14,5)/0.1614011431557236D-01/
      DATA NULLW(14,6)/0.1330641979525424D+00/
      DATA NULLW(14,7)/0.1205891076794731D+00/
      DATA NULLW(14,8)/-0.8640919108146020D-02/
      DATA NULLW(14,9)/-0.1294253464460428D+00/
      DATA NULLW(14,10)/-0.1251272318395094D+00/
      DATA NULLW(14,11)/0.0000000000000000D+00/
      DATA NULLW(15,1)/0.1384328909795413D+00/
      DATA NULLW(15,2)/0.2088816813445404D-01/
      DATA NULLW(15,3)/-0.1025551817987029D+00/
      DATA NULLW(15,4)/-0.1456993480020755D+00/
      DATA NULLW(15,5)/-0.8041833842953963D-01/
      DATA NULLW(15,6)/0.4369891359834745D-01/
      DATA NULLW(15,7)/0.1355713757017371D+00/
      DATA NULLW(15,8)/0.1284341564046552D+00/
      DATA NULLW(15,9)/0.2777799655524739D-01/
      DATA NULLW(15,10)/-0.9304002613430708D-01/
      DATA NULLW(15,11)/-0.1461812140165950D+00/
      DATA NULLW(16,1)/0.1434250647895144D+00/
      DATA NULLW(16,2)/0.5648832390790171D-01/
      DATA NULLW(16,3)/-0.5370731005946019D-01/
      DATA NULLW(16,4)/-0.1320553898020212D+00/
      DATA NULLW(16,5)/-0.1399117916675364D+00/
      DATA NULLW(16,6)/-0.7464504070837483D-01/
      DATA NULLW(16,7)/0.2922259459390760D-01/
      DATA NULLW(16,8)/0.1177993871727999D+00/
      DATA NULLW(16,9)/0.1454239155303997D+00/
      DATA NULLW(16,10)/0.9797588771824906D-01/
      DATA NULLW(16,11)/0.0000000000000000D+00/
C 
C***FIRST EXECUTABLE STATEMENT SGAINT
C 
C  Define constants
C 
      CONST=SAFETY*RCRIT**(1-ALPHA)
C 
C 
C  Compute half-length and center of interval.
C 
      HLEN=(B-A)/2
      CENTR=A+HLEN
      X(11)=CENTR
C 
C  Compute all abscissas
C 
      DO 10 I=1,10
         ABSCIS=HLEN*G(I)
         X(I)=CENTR-ABSCIS
         X(22-I)=CENTR+ABSCIS
 10   CONTINUE
      IF ((X(21).EQ.X(20)).OR.(X(1).EQ.X(2))) THEN
         FLAG=1
      END IF
C 
C   Evaluate all NUMFUN functions in the 21 evaluation points.
C 
      CALL FUNSUB (X,NUMFUN,21,FUNVAL)
C 
C   Apply the basic rule: first center point
C 
      DO 20 J=1,NUMFUN
         BASVAL(J)=W(11)*FUNVAL(11,J)
         BASABS(J)=ABS(BASVAL(J))
 20   CONTINUE
C 
C   Apply all nullrules: first center point
C 
      DO 40 J=1,NUMFUN
         DO 30 I=1,NUMNUL
            NULL(I,J)=NULLW(I,11)*FUNVAL(11,J)
 30      CONTINUE
 40   CONTINUE
C 
C   Compute the basic rule contributions from the other points.
C 
      DO 60 J=1,NUMFUN
         DO 50 I=1,10
            SUM=FUNVAL(I,J)+FUNVAL(22-I,J)
            ABSSUM=ABS(FUNVAL(I,J))+ABS(FUNVAL(22-I,J))
            BASVAL(J)=W(I)*SUM+BASVAL(J)
            BASABS(J)=BASABS(J)+W(I)*ABSSUM
 50      CONTINUE
 60   CONTINUE
C 
C   Compute the null rule contributions from the other points.
C   Recall that one half of the nullrules is symmetric and the other
C   half is asymmetric.
C 
      DO 90 J=1,NUMFUN
         DO 80 K=1,10
            SUM=FUNVAL(K,J)+FUNVAL(22-K,J)
            DIFF=FUNVAL(K,J)-FUNVAL(22-K,J)
            DO 70 I=1,NUMNUL/2
               I2=2*I
               I1=I2-1
               NULL(I1,J)=NULL(I1,J)+NULLW(I1,K)*SUM
               NULL(I2,J)=NULL(I2,J)+NULLW(I2,K)*DIFF
 70         CONTINUE
 80      CONTINUE
 90   CONTINUE
C 
C    Scale the results with the length of the interval
C 
      DO 100 J=1,NUMFUN
         BASVAL(J)=HLEN*BASVAL(J)
         BASABS(J)=HLEN*BASABS(J)
 100  CONTINUE
C 
C    Compute errors.
C 
      GREATE=0
      DO 130 J=1,NUMFUN
         EMAX=0
         DO 110 I=1,NUMNUL/2
            I2=2*I
            I1=I2-1
            E(I)=HLEN*SQRT(NULL(I1,J)**2+NULL(I2,J)**2)
            EMAX=MAX(EMAX,E(I))
 110     CONTINUE
         R=0
         DO 120 I=1,NUMNUL/2-1
            IF (E(I+1).NE.0) THEN
               RR(I)=E(I)/E(I+1)
            ELSE
               RR(I)=1
            END IF
            R=MAX(R,RR(I))
 120     CONTINUE
         IF (R.GE.1) THEN
            RGNERR(J)=SAFETY*EMAX
         ELSE IF (R.GE.RCRIT) THEN
            RGNERR(J)=SAFETY*R*E(1)
         ELSE
            RGNERR(J)=CONST*(R**ALPHA)*E(1)
         END IF
C 
C  Check the noise level.
C 
         NOISE=50*EPMACH*BASABS(J)
         IF ((E(1).LT.NOISE).AND.(E(2).LT.NOISE)) THEN
            RGNERR(J)=NOISE
         ELSE
            RGNERR(J)=MAX(NOISE,RGNERR(J))
         END IF
C 
C  Check if this is the greatest error so far among the remaining
C  problems. One exception: If the user wants to force the code
C  to do extra work (which seems unnecessary).
C 
         IF (.NOT.(MORE.AND.DONE(J)).AND.(RGNERR(J).GT.GREATE)) THEN
            GREATE=RGNERR(J)
            WORST=J
         END IF
 130  CONTINUE
C 
C   Compute the new subdivision points.
C 
      C=A+(B-A)/3
      D=B-(B-A)/3
      RETURN
C 
C***END SGAINT
C 
      END
