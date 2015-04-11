      SUBROUTINE D01GAE(X, Y, N, ANS, ER, IFAIL)
C
C     THIS SUBROUTINE INTEGRATES A FUNCTION (Y) SPECIFIED
C     NUMERICALLY AT N POINTS (X), WHERE N IS AT LEAST 4,
C     OVER THE RANGE X(1) TO X(N).  THE POINTS NEED NOT BE
C     EQUALLY SPACED, BUT SHOULD BE DISTINCT AND IN ASCENDING
C     OR DESCENDING ORDER.  AN ERROR ESTIMATE IS RETURNED.
C     THE METHOD IS DUE TO GILL AND MILLER.
C
C     NAG COPYRIGHT 1975
C     MARK 5 RELEASE
C     MARK 7 REVISED IER-154 (DEC 1978)
      INTEGER P01AAE, N, IFAIL, NN, I
      DOUBLE PRECISION SRNAME
      REAL ANS, ER, H2, D3, H3, D1, H1, D2, H4, R1, R2, R3, S, R4,
     * C, X(N), Y(N)
      DATA SRNAME /'  D01GAE'/
      ANS = 0.0
      ER = 0.0
      IF (N.GE.4) GO TO 20
      IFAIL = P01AAE(IFAIL,1,SRNAME)
      RETURN
C
C     CHECK POINTS ARE STRICTLY INCREASING OR DECREASING
C
   20 H2 = X(2) - X(1)
      DO 35 I=3,N
         H3 = X(I) - X(I-1)
         IF (H2*H3) 25, 30, 35
   25    IFAIL = P01AAE(IFAIL,2,SRNAME)
         RETURN
   30    IFAIL = P01AAE(IFAIL,3,SRNAME)
         RETURN
   35 CONTINUE
C
C     INTEGRATE OVER INITIAL INTERVAL
C
      D3 = (Y(2)-Y(1))/H2
      H3 = X(3) - X(2)
      D1 = (Y(3)-Y(2))/H3
      H1 = H2 + H3
      D2 = (D1-D3)/H1
      H4 = X(4) - X(3)
      R1 = (Y(4)-Y(3))/H4
      R2 = (R1-D1)/(H4+H3)
      H1 = H1 + H4
      R3 = (R2-D2)/H1
      ANS = H2*(Y(1)+H2*(D3/2.0-H2*(D2/6.0-(H2+2.0*H3)*R3/12.0)))
      S = -(H2**3)*(H2*(3.0*H2+5.0*H4)+10.0*H3*H1)/60.0
      R4 = 0.0
C
C     INTEGRATE OVER CENTRAL PORTION OF RANGE
C
      NN = N - 1
      DO 60 I=3,NN
         ANS = ANS + H3*((Y(I)+Y(I-1))/2.0-H3*H3*(D2+R2+(H2-H4)*R3)/
     *    12.0)
         C = H3**3*(2.0*H3*H3+5.0*(H3*(H4+H2)+2.0*H4*H2))/120.0
         ER = ER + (C+S)*R4
         IF (I.NE.3) S = C
         IF (I.EQ.3) S = S + 2.0*C
         IF (I-N+1) 40, 80, 40
   40    H1 = H2
         H2 = H3
         H3 = H4
         D1 = R1
         D2 = R2
         D3 = R3
         H4 = X(I+2) - X(I+1)
         R1 = (Y(I+2)-Y(I+1))/H4
         R4 = H4 + H3
         R2 = (R1-D1)/R4
         R4 = R4 + H2
         R3 = (R2-D2)/R4
         R4 = R4 + H1
         R4 = (R3-D3)/R4
   60 CONTINUE
C
C     INTEGRATE OVER FINAL INTERVAL
C
   80 CONTINUE
      ANS = ANS + H4*(Y(N)-H4*(R1/2.0+H4*(R2/6.0+(2.0*H3+H4)*R3/
     * 12.0)))
      ER = ER - H4**3*R4*(H4*(3.0*H4+5.0*H2)+10.0*H3*(H2+H3+H4))/
     * 60.0 + S*R4
      ANS = ANS + ER
      IFAIL = 0
      RETURN
      END
