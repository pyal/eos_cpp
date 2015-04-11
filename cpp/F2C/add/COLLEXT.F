

 PROGRAM COLLINS(INPUT,OUTPUT);

{Program Collins calculates the sum of single crystal and polycrystal
 Mossbauer spectra in an applied longitudinal magnetic field. Default
 is iron-57; to change this option enter N.

 Copyright by K.M. Hasselbach, K.Kulcsar, D.L. Nagy, H. Spiering, I. Vincze,
 Mainz - Budapest - Erlangen, 1986.}

{$I typedef.sys}
{$I graphix.sys}
{$I kernel.sys}
{$I windows.sys}

CONST MAXCH=1024;

LABEL INP,AVER,readLR,newETA;

TYPE AR8=ARRAY[1..8] OF REAL;
     ARY=ARRAY[1..MAXCH] OF REAL;
     ARYY=ARRAY[1..MAXCH,1..2] of real;
     ST80=string[80];
     ST14=string[14];

VAR DATA,ANZ,S,KANALZAHL,K0,KX,KY,KGZ,V,T,
    PE1,PE2,PE3,PEH,AL,BE,SNR,i,j,k: INTEGER;
    B1,B2,B3,B4,B5,Index,OK,okt: BOOLEAN;
    KAR,KRD: CHAR;
    ITS,SRW1,PP,IS,SIS,QAS,SCHRW,GAMMA,ETA,T1,ITGE,VB1,VB2,
    VB3,QUAD,HAEU,H0,H10,INT1,MUEA,MUEG,EMOSS,
    E,D1E,D2E,D3E,D4E,D12,D13,D24,D34,o,ymin,ymax,delta,
    RA,IA,RB,IB,RAB,IAB,RABS,IABS,AAS,BBS,
    E1,E2,E3,E4,M2,
    L1,L2,L3,L4,L5,L6,hkG,QUP2,X2,X,XU,sint2,cos2p,h,sint,
    X11,X22,X33,X44,R1,R2,R3,R4,I1,I2,I3,I4: REAL;
    line,title:string[80];
    VB: ARRAY[1..3] OF REAL;
    SPECTRUM,LOR: ARY;
    Y: ARYY;
    YY: ARYY;
    GEW: ARRAY[1..19] OF REAL;
    VH: ARRAY[1..57] OF REAL;
    EAGD,INT: AR8;
    Filenm:st14;
    NewFile:st14;
    FileVar:text;

FUNCTION SIGN(X: REAL): INTEGER;
         BEGIN
         IF X>0 THEN SIGN:=1 ELSE IF X<0 THEN SIGN:=-1 ELSE SIGN:=0
         END;

procedure Fitsp(len:integer;var y:aryy);
  var
    r1,r2,d12,d22,d11,s,det,a,b,f0,h,a0,a1,a2,b0,b1,b2:real;
    i:integer;

{spectrum fitting}

begin
    r1:=0; r2:=0; d11:=0; d12:=0; d22:=0;
    for i:=1 to len do
    begin  if y[i,1]>0.5 then s:=1/y[i,1] else s:=0;
           r1:=r1+s*y[i,1];
           r2:=r2+s*y[i,1]*y[i,2];
           d11:=d11+s;  d12:=d12+s*y[i,2];
           d22:=d22+s*sqr(y[i,2])
    end;
    det:=d11*d22-sqr(d12);
    a:=(r1*d22-r2*d12)/det; b:=(d11*r2-d12*r1)/det;
    f0:=0;
    for i:=1 to len do
    begin
           h:=y[i,1]-a-b*y[i,2];
           if y[i,1]>0.5 then s:=1/y[i,1] else s:=0;
           f0:=f0+s*sqr(h);
           y[i,2]:=a+b*y[i,2]
    end;
    writeln; write(' fitted parameters:  baseline='); write(a:10:4);
    writeln; write('                     area='); write(b:10:4);
    writeln; write('                     chisquare='); write(f0:10:4);
end;

procedure Writing(var Y:ARYY;Var NewFile:ST14;var DataOfSpectrum:ST80;
                    var c:integer);
var
  i:integer;
  St:ST80;
  FilVar:Text;
begin
  Assign(Filvar,NewFile);
  Rewrite(FilVar);
  writeln(Filvar,title);
  for i:=1 to c do
    begin
      Str(Y[i,2]:8:0,St);
      writeln(FilVar,St);
    end;
  Close(Filvar);
end;

procedure Plot(c,z1,z2,k:integer;Y:ARYY);
var
    j0,i0,j,kk,i,n:integer;
    deltav,j1,y1,i1:real;
    Answer:Char;
    Label Quit,Print,Two;
begin
  Answer:='n';
  kk:=2;
  EnterGraphic;                      {init the system and screen}
  Print:
  n:=1;
  DefineWorld(1,0,1000,1000,0);
  DefineWindow(1,0,0,XMaxGlb,YMaxGlb);
  SelectWorld(1);
  SelectWindow(1);
  DrawBorder;
  DefineWindow(2,0,trunc(YMaxGlb/10-15),XMaxGlb,trunc((YMaxGlb*9)/10-15));
  DefineWorld(2,0,280,180,0);
  SelectWorld(2);                 {select it}
  SelectWindow(2);
  Two:
  for i:=z1 to z2 do
    begin
     i1:=(0.5*360*i)/c; y1:=Y[i,kk];
     Drawpoint(i1,y1-1);DrawPoint(i1,y1); DrawPoint(i1,y1+1);
     Drawpoint(i1,y1-2);DrawPoint(i1,y1+2);
     DrawPoint(i1-0.5,y1);DrawPoint(i1+0.5,y1);
     Drawpoint(i1-0.25,y1);DrawPoint(i1+0.25,y1);
    end;
  if (k=2) and (n=1) then begin n:=2; kk:=1; goto Two end;
  SelectWorld(1);
  SelectWindow(1);
  DrawLine(0.5*2000*z1/c,870,0.5*2000*z2/c,870);
  GotoXY(64,24);write('velocity(mm/s)');
  deltav:=1/SRW1;
  for i:=0 to 8 do
    if (o-i*deltav)>0 then i0:=i;
    j0:=i0;
    for j:=z1 to z2 do
     begin
       if trunc(o-i0*deltav)=j then
         begin
           j1:=0.5*2000*j/c;
           DrawLine(j1,870,j1,880);
           GotoXY(round(j1/12),23);
           write(^H);
           if i0<0 then write(^H);
           if abs(j0)>2 then
           if abs(i0) in [0,2,4,6,8] then write(-i0:1);
           if abs(j0)<2 then write(-i0:1);
           i0:=i0-1;
       end;
    end;
  if Answer in ['Y','y'] then Hardcopy(False,6);
  repeat until keypressed;        {wait until a key is pressed}
  LeaveGraphic;
  writeln;
  if Answer in ['Y','y'] then goto Quit;
  Repeat
  write('Do you want to have a print?(y or n) ');
  readln (Answer);
  until Answer in ['Y','y','N','n'];
  if Answer in ['Y','y'] then
    begin
    writeln; write(' Enter spectrum title   '); readln(title);
    writeln(Lst,title);
    EnterGraphic;
    kk:=2;
    Goto Print;
  end;
Quit:  LeaveGraphic;
end;

PROCEDURE MOSSLONG(Q,ETA,HB,VHX,VHY,VHZ: REAL;
                   VAR EAGD,INT: AR8;BB: BOOLEAN);
LABEL MA1,MA2,MA3,MA4,MA5;
VAR i,K,T,S: INTEGER;
    SINS,COSS,SINT,COST,SIN2S,COS2S,
    ETA2,Q2,M,M2,A0,A1,A2,A3,D1,D2,D3,D4,
    H1,H2,H3,H4,H5,H6,
    RES0,RES1,RES2,P1,P2,S1,S2,Q0,Q1,Z,DIS,PP: REAL;
    EA,eb,DET: ARRAY[1..4] OF REAL;


BEGIN
IF ABS(HB) < 0.01 THEN HB:=0.01;
COST:=VHZ; SINT:=1-COST*COST;
IF ABS(SINT)<0.00001 THEN
     BEGIN SINS:=0; COSS:=-1; SINT:=0; END  ELSE
     BEGIN SINT:=SQRT(SINT); SINS:=VHX/SINT; COSS:=-VHY/SINT END;
SIN2S:=2*SINS*COSS;   COS2S:=2*COSS*COSS-1;
M:=-MUEA*HB/3;
ETA2:=ETA*ETA; M2:=M*M; Q2:=Q*Q;
H3:=SINT*SINT; H2:=ETA*COS2S;
A0:=(9*M2+(4*H3*(3-ETA2-2*H2)-2*(5-ETA2))*Q2)*M2+
    SQR(Q2*(1+ETA2/3));
A1:=8*Q*M2*(H3*(3+H2)-2);
A2:=-2*(5*M2+Q2*(1+ETA2/3));
RES0:=A1*A1-4*A0*A2;    RES1:=-4*A0;    RES2:=A2/3;
Q1:=RES2*RES2;Q0:=(RES0+RES2*(2*Q1-RES1))/2; Q1:=RES1/3-Q1;
 IF ABS(Q0*Q1)<1E-2 THEN Z:=0 ELSE
     BEGIN
     H1:=Q1/Q0;  DIS:=SQRT(ABS(1+H1*H1*Q1));
     PP:=ARCTAN(DIS); Z:=-2*SIGN(Q0)*SQRT(-Q1)*COS(PP/3)
     END;
Z:=(Z-RES2)/2; H1:=SQRT(ABS(Z*Z-A0));  P1:=-Z+H1;  P2:=-Z-H1;
 IF ABS(H1)<1E-5 THEN  BEGIN
    S1:=-SQRT(ABS(A2+2*Z))/2;  S2:=-S1;
                           END  ELSE  BEGIN
    S1:=-A1/4/H1;  S2:=-S1
                           END ;
H1:=SQRT(ABS(S1*S1-P1));  EA[1]:=-S1+H1;  EA[2]:=-S1-H1;
H1:=SQRT(ABS(S2*S2-P2));  EA[3]:=-S2+H1;  EA[4]:=-S2-H1;
if EA[1]>EA[3] then begin
   for i:= 1 to 4 do eb[i]:=EA[i];
   if (EA[3]>EA[2]) and (EA[2]>EA[4]) then begin
                             eb[2]:=EA[3];eb[3]:=EA[2] end;
   if EA[4]>EA[2] then begin eb[2]:=EA[3];eb[3]:=EA[4];eb[4]:=EA[2] end;
  end else begin
   eb[1]:=EA[3];
   if EA[4]>EA[1] then begin eb[2]:=EA[4];eb[3]:=EA[1];eb[4]:=EA[2] end;
   if EA[2]>EA[4] then begin eb[2]:=EA[1];eb[3]:=EA[2];eb[4]:=EA[4] end;
   if (EA[1]>EA[4]) and (EA[4]>EA[2]) then begin
                             eb[2]:=EA[1];eb[3]:=EA[4];eb[4]:=EA[2] end;
end;
for i:=1 to 4 do EA[i]:=eb[5-i];
write(' EA :  ');  for i:=1 to 4 do write(EA[i]/EMOSS:10:4); writeln;


H1:=-MUEG*HB;
 FOR K:=1 TO 4 DO  BEGIN  EAGD[K+4]:=EA[K]-H1;
                          EAGD[K]  :=EA[K]+H1     END ;

BEGIN
H2:=ETA/1.7320508*Q; H4:=COST*COST; H5:=H2*COS2S; H6:=H2*SIN2S;
H1:=((3*H4-1)-ETA*H3*COS2S)/2*Q;       H2:=1.7320508*Q;
D1:=3*M+H1; D2:=M-H1; D3:=-M-H1; D4:=-3*M+H1;
RA:=H6*SINT;IA:=-H2-H5;RB:=IA/2*H3+H5;IA:=IA*COST*SINT;IB:=COST*H6;
H1:=RA*RB;H2:=IA*IB;RAB:=H1-H2;RABS:=H1+H2;AAS:=RA*RA+IA*IA;
H1:=RA*IB;H2:=IA*RB;IAB:=H1+H2;IABS:=-H1+H2;BBS:=RB*RB+IB*IB;

 FOR K:=1 TO 4 DO
      BEGIN
     E:=EA[K]; D1E:=D1-E; D2E:=D2-E; D3E:=D3-E; D4E:=D4-E;
     H1:=D1E-D4E; H2:=D2E-D3E; H3:=AAS+BBS;
     D13:=D1E*D3E-H3; D24:=D2E*D4E-H3;
     DET[1]:=D3E*D24-AAS*H2; DET[2]:=D4E*D13-AAS*H1;
     DET[3]:=D1E*D24+AAS*H1; DET[4]:=D2E*D13+AAS*H2;
     H4:=ABS(DET[1]);   T:=1;
     FOR S:=2 TO 4 DO  BEGIN
   IF H4<ABS(DET[S]) THEN  BEGIN
  H4:=ABS(DET[S]); T:=S;     END ;   END ;
           BEGIN
           CASE T OF
                1 : GOTO MA1;
                2 : GOTO MA2;
                3 : GOTO MA3;
                4 : GOTO MA4
           END;

          MA1:   D34:=D3E*D4E-H3;
          R1:=-DET[1]; R2:= RA*D34; R3:= RB*D24;  R4:= RAB*H2;
          I1:= 0.0;      I2:=-IA*D34; I3:=-IB*D24;  I4:=-IAB*H2;
           GOTO MA5;

          MA2:   D34:=D3E*D4E-H3;
          R1:=RA*D34;   R2:=-DET[2];R3:=RABS*H1;  R4:= RB*D13;
          I1:=IA*D34;   I2:=0.0;      I3:=IABS*H1;  I4:=-IB*D13;
           GOTO MA5;

          MA3:   D12:=D1E*D2E-H3;
          R1:=RB*D24;   R2:=RABS*H1;  R3:=-DET[3];R4:=-RA*D12;
          I1:=IB*D24;   I2:=-IABS*H1; I3:=0.0;      I4:= IA*D12;
           GOTO MA5;

          MA4:  D12:=D1E*D2E-H3;
          R1:=RAB*H2;   R2:=RB*D13;   R3:=-RA*D12;  R4:=-DET[4];
          I1:=IAB*H2;   I2:=IB*D13;   I3:=-IA*D12;  I4:=0.0;
           END ;

MA5:
X11:=R1*R1+I1*I1;X22:=R2*R2+I2*I2;X33:=R3*R3+I3*I3;X44:=R4*R4+I4*I4;
H1:=(X11+X22+X33+X44);
IF(BB) THEN BEGIN
INT[K+4]:=(X11/6.0+X22/9.0+X33/18.0)/H1;
INT[K  ]:=(X22/18.0+X33/9.0+X44/6.0)/H1;
END ELSE BEGIN
INT[K+4]:=(X11/4.0+X33/12.0)/H1;
INT[K  ]:=(X22/12.0+X44/4.0)/H1;
         END;

      END ;
 END ;
 END ;

 FUNCTION LL(D,A,D0,G: REAL): REAL;
 VAR  DD,G2: REAL;
 BEGIN
 DD:=D-D0; G2:=G/2.0; LL:=A*G2/(SQR(DD)+SQR(G2))/PP  END;

Procedure SINTPHI(L,R,X,eta,h: real;var sint2,cos2p: real;
                                          var ok: boolean);

var h0,h1,h2,h3,h4,hm,c1,c2,d,a0,a1,a2,m2,
    eta2: real;
    ieta: integer;
label ende;

begin
ok:=true;
hm:=1.262;
h1:=X+L; h2:=X+R; h3:=R+L;
h0:=-(h3+2.0*h)/2.0;
write(' L2 =',X:10:4,'  <  ',h0:10:4); writeln;
if (X > h0) then begin ok:=false; goto ende end;
c1:=2.0*R+h1;
c2:=X-L;
d :=-(h1+2.0*h);
h1:=c1*c1; h2:=c2*c2; h3:=d*d;
a2:=-(2.0*h3+h1+h2)/4.0;
a1:=-d/4.0*(h1-h2);
a0:=(h3-h1)*(h3-h2)/16.0;
m2:=-(a2+2.0)/10.0;
write(' M2 =',m2:10:4,'  >  0'); writeln;
if (m2<0.0000001) then begin ok:=false; goto ende end;
eta2:=eta*eta;
h1:= 1.0+eta2/3.0;
h2:=sqrt(h1)*a1/8.0/m2+2.0;
sint2:= ((a0-9.0*m2*m2+2.0*m2*(5.0-eta2)/h1-1.0)
        /4.0/m2*h1+2.0*h2)/(9.0-eta2);
write(' sint2 =',sint2:10:4,'  >  0'); writeln;
if (sint2<0.0) then begin ok:=false; goto ende end;
if (sint2<0.000001) then
   cos2p:=0.0
 else  begin
 cos2p:= (3.0-h2/sint2)/eta;
write(' cos2p =',cos2p:10:4); writeln;
 if (abs(cos2p) > 1.0) then  ok:=false;
       end;
ende:
end;

PROCEDURE  DATAINP;
BEGIN
WRITELN;
WRITE(' Intensity ?   '); READ(ITS); WRITELN;
WRITE(' Isomer shift (mm/s) ?   '); READ(IS); WRITELN;
WRITE(' Quadrupole splitting (mm/s) ?   '); READ(QAS); WRITELN;
WRITE(' Asymmetry parameter ?   '); READ(ETA); WRITELN;
WRITE(' Applied magnetic field (T) ?   '); READ(HAEU); WRITELN;
WRITE(' Linewidth (mm/s) ?   '); READ(GAMMA); WRITELN;
SIS:=IS/SRW1+KANALZAHL/2+1;H0:=SQRT(1.0+ETA*ETA/3);
HAEU:=HAEU*10.0;
IF Abs(QAS)<1E-4  THEN  QAS:=1E-4;
QUAD:=QAS/H0*EMOSS*0.5;SCHRW:=EMOSS*SRW1;
GAMMA:=GAMMA/SRW1;
END;


BEGIN
InitGraphic; LeaveGraphic;
WRITELN(' *** Mossbauer spectra in longitudinal magnetic field ***');
WRITELN;
WRITELN(' Copyright by K.M. Hasselbach,  D.L. Nagy,');
WRITELN(' H. Spiering, Mainz - Budapest - Erlangen, 1986.');
WRITELN;
FOR  S:=1 TO MAXCH DO  LOR[S]:=0.0;
PP:=PI;
Index:=false;
MUEA:=-0.15491; MUEG:=0.09024; EMOSS:=15.257;
B1:=FALSE; B2:=FALSE; B3:=FALSE; B4:=FALSE; B5:=FALSE;
INP: WRITELN; WRITE(' *'); READ(KAR);

IF NOT (KAR IN ['C','c','S','s','P','p','I','i','M','m','L','l','E','e',
               'D','d','F','f','R','r','N','n','Z','z']) THEN
BEGIN
WRITELN;
WRITELN(' Enter (C)alibration');
WRITELN('       (M)onocrystal');
WRITELN('       (P)olycrystal');
WRITELN('       (I)nternal field polycrystal');
WRITELN('       (N)uclear transition');
WRITELN('       (D)isplay');
WRITELN('       (R)ead spectrum');
WRITELN('       (F)it spectrum');
WRITELN('       (L)ogged drive');
WRITELN('       (S)ave simulated curve');
WRITELN('       (Z)ero simulated curve');
WRITELN('       (E)nd');
GOTO INP
END;

IF KAR IN ['C','c'] THEN  BEGIN
WRITELN;
WRITE(' Number of channels ?   '); READ(KANALZAHL); WRITELN;
IF KANALZAHL < 1 THEN KANALZAHL:=1;
IF KANALZAHL > MAXCH THEN KANALZAHL:=MAXCH;
WRITE(' Calibration factor (mm/s/channel) ?   '); READ(SRW1); WRITELN;
B1:=TRUE; B2:=FALSE; B3:=FALSE; B4:=FALSE; GOTO  INP  END;

IF  KAR IN ['M','m']  THEN  BEGIN IF NOT B1 THEN BEGIN WRITELN;
                                        WRITELN( 'NO CALIBRATION');
                                        GOTO INP
                                       END;
 B3:=FALSE;
 WRITELN; WRITELN(' *** Monocrystal ***'); H10:=0;
 FOR  BE:=1 TO 3 DO
 BEGIN  WRITELN; WRITE(' ',BE:1,'. direction cosine ?   ');
 READ(VB[BE]);H10:=SQR(VB[BE])+H10  END ;

DATAINP;       WRITELN;
WRITELN(' Normalized direction cosines of H in the EFG PAS');
H10:=SQRT(H10);
FOR  BE:=1 TO 3  DO  BEGIN  VB[BE]:=VB[BE]/H10;
WRITELN(' ',VB[BE]:7:4) END ;
WRITELN;

MOSSLONG(QUAD,ETA,HAEU,VB[1],VB[2],VB[3],EAGD,INT,B5);

 FOR  S:=1 TO 8 DO
 BEGIN  T1:=EAGD[S]/SCHRW; INT1:=INT[S]*ITS ;
 FOR     T:=1 TO  KANALZAHL  DO
LOR[T]:=LOR[T]-LL(T-SIS,INT1,T1,GAMMA)
 END ;
B4:=TRUE;
 GOTO  INP
 END ;

IF  KAR IN ['I','i']  THEN  BEGIN IF NOT B1 THEN BEGIN WRITELN;
                                        WRITELN( 'NO CALIBRATION');
                                        GOTO INP
                                       END;
 B3:=FALSE;
 B5:=TRUE;
 WRITELN; WRITELN(' *** internal field - powder ***');
WRITELN;
WRITE(' Intensity ?   '); READ(ITS); WRITELN;
WRITE(' Linewidth (mm/s) ?   '); READ(GAMMA); WRITELN;
readLR:
WRITE(' Position of Line L1 (mm/s) ? '); READ(L1); writeln;
WRITE(' Position of Line L2 (mm/s) ? '); READ(L2); writeln;
WRITE(' Position of Line L3 (mm/s) ? '); READ(L3); writeln;
WRITE(' Position of Line L4 (mm/s) ? '); READ(L4); writeln;
WRITE(' Position of Line L5 (mm/s) ? '); READ(L5); writeln;
WRITE(' Position of Line L6 (mm/s) ? '); READ(L6); writeln;
IS:=(2.0*L1+L2+L3+L4+L5+2.0*L6)/8.0;
L1:=L1-IS; L2:=L2-IS; L3:=L3-IS;
L4:=L4-IS; L5:=L5-IS; L6:=L6-IS;
E1:=(+L1-L2-L3-L6)/2.0;
E2:=(-L1+L2-L3-L6)/2.0;
E3:=(-L1-L2+L3-L6)/2.0;
E4:=(L1+L2+L3+3.0*L6)/2.0;
h :=(L4-L2)/2.0;
h :=-(L1+L2+L3+L6)/2.0;
write(' Ei = ',E1:10:4,E2:10:4,E3:10:4,E4:10:4); writeln;
write('h= ',h:10:4,'    h= ',(L4-L2)/2.0*EMOSS:10:4);writeln;
M2:=MUEA/MUEG/3.0*h; M2:=M2*M2;
QAS:=(E1*E1+E2*E2+E3*E3+E4*E4)/4.0 -5.0*M2;
if (QAS<0.0) then begin QAS:=0.0; goto readLR end
              else QAS:=2.0*sqrt(QAS);

HAEU:=h*EMOSS/MUEG;
write(' isomer shift            :',IS:10:3,'mm/s' ); writeln;
write(' quadrupole splitting   :',QAS:10:3,'mm/s' ); writeln;
write(' internal magnetic field:',HAEU:10:3,'kG' ); writeln;

SIS:=IS/SRW1+KANALZAHL/2+1;

SCHRW:=EMOSS*SRW1;
GAMMA:=GAMMA/SRW1;
QUAD:=QAS*0.5;

 newETA:
 WRITELN; WRITE(' eta ?   '); read(ETA); writeln;
 SINTPHI(L1/QUAD,L6/QUAD,L2/QUAD,ETA,h/QUAD,sint2,cos2p,okt);
 if (not okt) then goto newETA;
sint:=sqrt(sint2);
VB[1]:= sqrt((cos2p+1.0)/2.0)*sint;
VB[2]:= sqrt((1.0-cos2p)/2.0)*sint;
VB[3]:= sqrt(1.0-sint2);

WRITELN;
WRITELN(' H-vector in the EFG PAS:  ',HAEU:7:2,
                      '*(',VB[1]:8:4,VB[2]:8:4,VB[3]:8:4,')  kG');

WRITELN;   WRITELN(' *** Simulation in progress ***');
H0:=SQRT(1.0+ETA*ETA/3);
QUAD:=QUAD/H0*EMOSS;

MOSSLONG(QUAD,ETA,HAEU,VB[1],VB[2],VB[3],EAGD,INT,B5);

write(' EAGD :  ');writeln;
     for i:=1 to 8 do write(EAGD[i]/EMOSS+IS:9:4);
 writeln;
write(' INT  :  ');writeln;
     for i:=1 to 8 do write(INT[i]:9:4); writeln;


 FOR  S:=1 TO 8 DO
 BEGIN  T1:=EAGD[S]/SCHRW; INT1:=INT[S]*ITS ;
 FOR  T:=1 TO  KANALZAHL  DO
LOR[T]:=LOR[T]-LL(T-SIS,INT1,T1,GAMMA)
 END ;
B4:=TRUE;
 GOTO  INP
 END ;

IF  KAR IN ['P','p'] THEN  BEGIN IF NOT B1 THEN BEGIN  WRITELN;
                                       WRITELN(' NO CALIBRATION');
                                       GOTO INP
                                       END;
B3:=FALSE;
WRITELN(' *** Polycrystal ***'); WRITELN;
AVER:     WRITE(' Fine averageing ?  (y/n)   '); READ(KRD); WRITELN;
IF KRD IN ['Y','y'] THEN  BEGIN
DATA:=19;
GEW[1]:=2.284758009E-3;
VH[1]:=0.0; VH[2]:=0.0;   VH[3]:=1.0;
GEW[2]:=8.829336405E-3;
VH[4]:=4.33E-2;  VH[5]:=1.046E-1;  VH[6]:=9.936E-1;
GEW[3]:=1.377283347E-2;
VH[7]:=5.82E-2;  VH[8]:=2.173E-1;  VH[9]:=9.744E-1;
GEW[4]:=4.193946234E-3;
VH[10]:=1.591E-1;  VH[11]:=1.591E-1;  VH[12]:=9.744E-1;
GEW[5]:=1.676326330E-2;
VH[13]:=6.67E-2;  VH[14]:=3.354E-1;  VH[15]:=9.397E-1;
GEW[6]:=1.469379460E-2;
VH[16]:=1.9E-1;  VH[17]:=2.844E-1;  VH[18]:=9.397E-1;
GEW[7]:=1.343452769E-2;
VH[19]:=0.0;  VH[20]:=4.54E-1;  VH[21]:=8.91E-1;
GEW[8]:=1.864992251E-2;
VH[22]:=1.403E-1;  VH[23]:=4.318E-1;  VH[24]:=8.91E-1;
GEW[9]:=1.603317003E-2;
VH[25]:=2.669E-1;  VH[26]:=3.673E-1;  VH[27]:=8.91E-1;
GEW[10]:=2.381719002E-2;
VH[28]:=7.3E-2;  VH[29]:=5.548E-1;  VH[30]:=8.29E-1;
GEW[11]:=1.946375979E-2;
VH[31]:=2.145E-1;  VH[32]:=5.168E-1;  VH[33]:=8.29E-1;
GEW[12]:=1.667125690E-2;
VH[34]:=3.405E-1;  VH[35]:=4.437E-1;  VH[36]:=8.29E-1;
GEW[13]:=2.347398240E-2;
VH[37]:=1.46E-1;  VH[38]:=6.396E-1;  VH[39]:=7.547E-1;
GEW[14]:=1.909708913E-2;
VH[40]:=2.847E-1;  VH[41]:=5.911E-1;  VH[42]:=7.547E-1;
GEW[15]:=9.778053926E-3;
VH[43]:=3.234E-1;  VH[44]:=6.691E-1;  VH[45]:=6.691E-1;
GEW[16]:=1.651497672E-2;
VH[46]:=4.09E-1;  VH[47]:=5.13E-1;  VH[48]:=7.547E-1;
GEW[17]:=7.406494069E-3;
VH[49]:=4.639E-1;  VH[50]:=4.639E-1;  VH[51]:=7.547E-1;
GEW[18]:=1.598672547E-2;
VH[52]:=4.636E-1;  VH[53]:=5.808E-1;  VH[54]:=6.691E-1;
GEW[19]:=9.724499459E-4;
VH[55]:=5.7735E-1;  VH[56]:=5.7735E-1;  VH[57]:=5.7735E-1;

END  ELSE  IF KRD IN ['N','n'] THEN BEGIN

DATA:=5;
GEW[1]:=0.07319050283;VH[1]:=0.1308;VH[2]:=0.6155;
VH[3]:=0.7772; GEW[2]:=0.02995519558;VH[4]:=0.3535;
VH[5]:=0.3536;VH[6]:=0.8660; GEW[3]:=0.03892568304;
VH[7]:=0.0697;VH[8]:=0.1683;VH[9]:=0.9833;
GEW[4]:=0.06298798864;VH[10]:=0.4083;VH[11]:=0.5620;
VH[12]:=0.7193; GEW[5]:=0.05726179040;VH[13]:=0.1189;
VH[15]:=0.9136; VH[14]:=0.3889
END ELSE GOTO AVER;

DATAINP;  WRITELN;
WRITELN(' *** Simulation in progress ***');

ITS:=ITS/0.7869634812; K0:=1; KX:=1; KY:=1;KGZ:=2;
 IF ETA>0.01 THEN  BEGIN K0:=2;KGZ:=5;ITS:=ITS/2.0 END ;
WRITELN;
 FOR ANZ:=1 TO DATA DO  BEGIN  ITGE:=ITS*GEW[ANZ];
T:=3*(ANZ-1);
PE1:=3; PE2:=2; PE3:=1;
 FOR AL:=1 TO K0 DO  BEGIN
PEH:=PE1; PE1:=PE2; PE2:=PEH;
 FOR  BE:=1 TO 3  DO
 BEGIN
PEH:=PE1; PE1:=PE3; PE3:=PE2; PE2:=PEH;
VB1:=VH[T+PE1];VB2:=VH[T+PE2];VB3:=VH[T+PE3];


MOSSLONG(QUAD,ETA,HAEU,VB1,VB2,VB3,EAGD,INT,B5);

 FOR S:=1 TO 8 DO
 BEGIN    T1:=EAGD[S]/SCHRW; INT1:=INT[S]*ITGE;
 FOR V:=1 TO KANALZAHL DO
 LOR[V]:=LOR[V]-LL(V-SIS,INT1,T1,GAMMA)
 END
 END
 END
 END ;
B4:=TRUE;
 GOTO INP
 END;

 IF  KAR IN ['N','n']  THEN   BEGIN
  B1:=FALSE; B2:=FALSE; B3:=FALSE; B4:=FALSE;
  WRITELN;
  WRITELN(' Parameters of the Mossbauer nucleus'); WRITELN;
  WRITE(' Transition energy (keV) ?   '); READ(EMOSS); WRITELN;
  EMOSS:=EMOSS/0.94519;
  WRITELN(' Magnetic moments (n.m.) ?  Ground state:   '); READ(MUEG);
 WRITELN;
  WRITELN('                            Excited state:   '); READ(MUEA);
 WRITELN;
  GOTO  INP
 END;

IF KAR IN['D','d'] THEN
   BEGIN IF NOT B1
         THEN BEGIN WRITELN; WRITELN(' NO CALIBRATION'); GOTO INP
              END;
         IF NOT B4
         THEN BEGIN WRITELN; WRITELN(' NO SIMULATED CURVE'); GOTO INP
              END;
         IF B2 AND NOT B3
         THEN BEGIN WRITELN; WRITELN(' PLEASE FIT FIRST'); GOTO INP
         END;
o:=KANALZAHL/2;
if not B2 then for i:=1 to KANALZAHL do YY[i,2]:=LOR[i] else
for i:=1 to KANALZAHL do for k:=1 to 2  do YY[i,k]:=Y[i,k];
ymax:=YY[1,2]; ymin:=ymax;
for i:=1 to KANALZAHL do

begin if YY[i,2]<ymin then ymin:=YY[i,2];
      if YY[i,2]>ymax then ymax:=YY[i,2];
      if B2 and (YY[i,1]<ymin) and (YY[i,1]>0) then ymin:=YY[i,1];
      if B2 and (YY[i,1]>ymax) then ymax:=YY[i,1]
end;
delta:=ymax-ymin;
for i:=1 to KANALZAHL do
begin
YY[i,2]:=Trunc(279*(ymax-YY[i,2])/delta);
if B2 then YY[i,1]:=Trunc(279*(ymax-YY[i,1])/delta)
end;
if B2 then k:=2 else k:=1;
 Plot(KANALZAHL,1,KANALZAHL,k,YY);
WRITELN;
 GOTO   INP   END;

IF  KAR IN ['S','s']  THEN begin
         IF NOT B3
         THEN BEGIN WRITELN; WRITELN(' PLEASE FIT FIRST'); GOTO INP END;
         IF NOT B4 THEN
            BEGIN
            WRITELN;
            WRITELN( 'NO SIMULATED CURVE');
            GOTO INP
            END;

   write(' Output filename (no extension) :');
   readln(NewFile);
   NewFile:=NewFile + '.ASC';
   title:='  ';
   Writing(Y,NewFile,title,KANALZAHL);
writeln;
goto INP
end;

if KAR in ['L','l'] then
begin writeln; write(' Enter new logged drive   '); readln(line);
      ChDir(line);
      goto INP
end;

if KAR in ['Z','z'] then
begin B3:=false; B4:=false;
      writeln;
      for S:=1 to KANALZAHL do LOR[S]:=0;
      writeln(' *** Simulated curve zeroed ***');
      goto INP
end;

IF KAR IN ['R','r'] THEN
begin
  B3:=false;
  writeln;
  write(' Enter filename  ');
  readln(Filenm);
  Assign(FileVar,Filenm);
  {$I-} Reset(FileVar) {$I+};
  OK:=(IOresult=0);
  if not OK then begin writeln(' Cannot find file ',Filenm); goto INP end;
  j:=0;
  while not Eof(FileVar) do
  begin
  read(FileVar,line);
  if (j=0) then title:=line
  else
  read(FileVar,Y[j,1]);
  B2:=true;
  j:=j+1;
  end;
  Close(FileVar);
GOTO INP
end;

IF KAR IN ['F','f'] THEN
   BEGIN IF NOT B2
         THEN BEGIN WRITELN; WRITELN(' NO SPECTRUM'); GOTO INP
              END;
         IF NOT B1
         THEN BEGIN WRITELN; WRITELN(' NO CALIBRATION'); GOTO INP
              END;
         IF NOT B4
         THEN BEGIN WRITELN; WRITELN(' NO SIMULATED CURVE'); GOTO INP
              END;

FOR I:=1 TO KANALZAHL DO Y[I,2]:=LOR[I];
writeln; writeln(' *** Fitting in progress ***'); writeln;
Fitsp(KANALZAHL,Y);
B3:=true;
for I:=1 to KANALZAHL do
goto INP
END;

IF  NOT (KAR IN ['E','e'])  THEN  GOTO  INP;

WRITELN; WRITELN(' Good-bye from COLLINS');

END.
