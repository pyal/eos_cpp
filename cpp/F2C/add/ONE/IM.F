      PROGRAM IM
      DIMENSION P(1000),V(1000),U(1000),E(1000),UF(3000),TF(3000),
     ,X(1000)
      COMMON /RC/ R1,R2,R3,C1,C2,C3,G1,G2,G3,H1,H2,H3,I11,I21,
     ,B1,B2,B3	  
      

      character*11 filename
C     
C   PARAMETERS FOR CALCULATIONS
c
      open(unit=30,file='imn',status='old')
      open(unit=31,file='out.dat',status='new')
      read(30,1) R1,R2,R3,C1,C2,C3,B1,B2,B3,G1,G2,G3,H1,H2,H3,
     *U0,TM
  1   FORMAT(//3E12.2)
      close(30)			
C
                VIS=5.
                VIS1=0.05
                icount=10
                JMM=3000
c
                I1=60
		I11=I1-1
		DM=H1*R1/I11
		I2=I1+H2*R2/DM
		I21=I2-1
		I3=I2+H3*R3/DM
		I31=I3-1
		IMI=1
		D1=C1+B1*U0*0.5
		D2=C2+B2*U0*R1*C1/(R1*C1+R2*C2)
		TAU1=DM/(R1*D1*5.)
        	TAU2=DM/(R2*D2*5.)
                TAU=AMIN1(TAU1,TAU2)
		JM=TM/TAU
                WMAX=0.
		JT=0
		NF=0
                TMM=0.5*TM*1.E+09
		TRH=TAU/DM
		DL=H1+H2+H3
C		
	    DO 2 J=1,JMM
		UF(J)=0.
    2   TF(J)=0.
	    U(I3)=0.
		X(1)=0.
		DO 3 I=1,I11
	   	P(I)=0.
		E(I)=0.
		V(I)=1./R1
	   	U(I)=U0
3		X(I+1)=X(I)+DM/R1
		DO 4 I=I1,I21
        P(I)=0.
		E(I)=0.
		V(I)=1./R2
	   	U(I)=0.
4 		X(I+1)=X(I)+DM/R2
	    DO 5 I=I2,I31
        P(I)=0.
		E(I)=0.
		V(I)=1./R3
	   	U(I)=0.
5		X(I+1)=X(I)+DM/R3
        U(I1+1)=0.25*U0
		U(I1)=0.50*U0
		U(I1-1)=0.75*U0
                PMAX=R1*D1*U0
                PMIN=-PMAX
                KEY=0
c
		T=0.
c
c            CALL GRINIT
c            call setmod(1,3)
c            CALL PAGE(22.,16.,' ',1,0)
c            CALL REGION(5.,1.,15.,12.5,'P,X',3,1)
c            CALL LIMITS(0.,dl,PMIN,PMAX)
c            CALL AXES('X',1,5,1,'P',1,5,1,00)
c            call setpen(4)
C
CALCULATION
C
        JM=100
        WRITE_I=10
        DO 100 J=1,JM
		T=T+TAU
		IF(P(I11).LT.-5.E+08)IMI=I1
		IMI1=IMI+1      
              DO 99 I=IMI1,I31
 99           U(I)=U(I)-(P(I)-P(I-1))*TRH
C   BOUNDARY CONDITIONS (LEFT)
             U(IMI)=U(IMI)-2.*P(IMI)*TRH
C   BOUNDARY CONDITIONS (RIGHT)
              U(I3)=U(I3)+2.*P(I31)*TRH
C
                IF(KEY.EQ.1)GOTO98
        IF(ABS(U(I3)).LT.0.1)GO TO 201
		IF(JT.EQ.0) JT=J-1
  98           	UF(J-JT)=U(I3)
		TF(J-JT)=TAU*(J-JT-1)*1.E+09
		NF=NF+1
                IF(WMAX.LT.U(I3))WMAX=U(I3)
                KEY=1
  201 	CONTINUE 
				DO 400 I=IMI,I31
                                G=G1
                                IF(I.GE.I1)G=G2
                                IF(I.GE.I2)G=G3
				DU=U(I+1)-U(I)
				DV=DU*TRH
				V(I)=V(I)+DV
				VS=V(I)-0.5*DV
				AS=A(VS,I)
				S=0.5*G*DV/VS
				E(I)=(E(I)*(1.-S)-AS*DV)/(1.+S)
				P(I)=A(V(I),I)+G*E(I)/V(I)
				PVIS=VIS*(DU-ABS(DU))*DU/V(I)
     ++VIS1*(ABS(DU)-DU)*C1*ABS(P(I)/PMAX)/V(I)
      write(31,1) X(I),P(I),E(I)
  400           P(I)=P(I)+PVIS
                if(J-J/ICOUNT*ICOUNT.ne.0)GOTO 100
c                CALL lineo(X,P,i31)
  100  CONTINUE                          
      close(31)       
c         CALL ENDPG(' ')
c         CALL GRINIT
c         call setmod(1,3)
c         CALL SETPEN(2)
c         CALL PAGE(22.,16.,' ',1,0)
c         CALL REGION(5.,1.,15.,12.5,'W, T',4,1)
c         CALL LIMITS(0.,TMM,0.,WMAX)
c         CALL AXES('T, ns',5,2,1,'W, m/s',6,5,1,00)
c          call setpen(4)
c         CALL LINEO(TF,UF,NF)
c         CALL ENDPG(' ')
	  write(*,*)' Do You want to write W(t)? /1-YES, 0-NO/'
	  read(*,*)iwri
	  if(iwri.ne.1)go to 68
	  write(*,*)' Name of new file?'
	  read(*,66)filename
   66    format(a11)
         open(unit=10,file=filename,status='new')
	  write(10,67)(TF(i),UF(i),i=1,NF)
         close(10)
   67    format(2e12.3)				 				 
   68 STOP
      END
c			
c
c
      FUNCTION A(X,I)
      COMMON /RC/ R1,R2,R3,C1,C2,C3,G1,G2,G3,H1,H2,H3,I11,I21,
     ,B1,B2,B3	  
      IF(I.GT.I21)GOTO3
	  IF(I.GT.I11)GOTO2
	  R=R1
	  C=C1
	  B=B1
          G=G1
	  GOTO1
    2 R=R2
	  C=C2
	  B=B2
          G=G2
	  GOTO1
3	  R=R3
	  C=C3
	  B=B3
          G=G3
1	  Y=1.-R*X
	  A=R*C**2*Y*(1.-0.5*G*Y/(R*X))/(1.-B*Y)**2
	  RETURN
	  END	  
