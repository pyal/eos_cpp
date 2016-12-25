#ifndef __SPLINE_H
#define  __SPLINE_H
//#include "fobj.h"

//long SearchXI(long N,long M;double U,double *X);
// { Searching of interval X[i] < U < X[i+1] }

void Spline(long N,double *X,double *Y,double *B,double *C,double *D);
// { Building of ordinary cubic spline S= Y + BX + CX^2 + DX^3 } 
  
double Seval(long N,double U,double *X,double *Y,double *B,double *C,
                                                            double *D);
// { Calculating value of ordinary cubic spline S= Y + BX + CX^2 + DX^3 } 
  

#endif
  
/*
uses MtType;
{$I MathDef.}
{=========================================================================
   Procedures for making spline interpolation
--------------------------------------------------------------------------
 Input & Output variables:
 N - number of points
 X,Y - arrays of abscissas & ordinate points
 B,C,D - arrays of spline coefficients
 M - array of second derivation 
 P,Q - arrays of stiffness coefficients (use in rational spline)
 U - current value of abscise point, where spline value must be calculated
 Z - array of corrected ordinate points (use in smoothing spline)
 RelError - relative width of error passage (use in smoothing spline)
===========================================================================}

function SearchXI(N,M:word; var U:RealType; X:array of RealType):word;
 { Searching of interval X[i] < U < X[i+1] }

procedure Spline(N:word; X,Y:array of RealType; var B,C,D:array of RealType);
 { Building of ordinary cubic spline S:= Y + BX + CX^2 + DX^3 } 
  
function Seval(N:word; var U:RealType; X,Y,B,C,D:array of RealType) :RealType;
 { Calculating value of ordinary cubic spline S:= Y + BX + CX^2 + DX^3 } 
  
function CellInter(N:word; var U:RealType; X,Y:array of RealType) :RealType;
 { Cell interpolating } 
  
function LinInter(N:word; var U:RealType; X,Y:array of RealType) :RealType;
 { Linear interpolation } 
  
procedure MSpline(N:word; X,Y:array of RealType; var M:array of RealType);
 { Building of cubic spline, using second derivation } 

procedure MSplineDrv(N:word; X,Y:array of RealType; var M:array of RealType;
   DerivA,DerivB:RealType);
 { Building of cubic spline, using second derivation, exist first
    derivation on boundary } 
  
function MSeval(N:word; U:RealType; X,Y,M:array of RealType) :RealType;
 { Calculating value of cubic spline, using second derivation } 

function MSevalWS(i:word; U:RealType; X,Y,M:array of RealType) :RealType;
 { Calculating value of cubic spline without search, using second derivation } 
  
procedure RatPQSpline(N:word; X,Y,P,Q:array of RealType; var M:array of RealType);
 { Building of rational spline, using second derivation, 
  assymmetrical stiffness coefficients } 
  
function RatPQSeval(N:word; var U:RealType; X,Y,M,P,Q:array of RealType) :RealType;
 { Calculating value of rational spline, using second derivation, 
  assymmetrical stiffness coefficients } 
  
procedure RatSpline(N:word; X,Y,P:array of RealType; var M:array of RealType);
 { Building of rational spline, using second derivation, 
  symmetrical stiffness coefficients } 
  
function RatSeval(N:word; var U:RealType; X,Y,M,P:array of RealType) :RealType;
 { Calculating value of rational spline, using second derivation, 
  symmetrical stiffness coefficients } 
  
procedure SmoothSpline(N:word; X,Y:array of RealType; var Z,M:array of RealType;
  RelError:RealType);
 { Building of smoothing spline, using second derivation }

function AutoMSpline(Func:FunType; A,B,AbsErr:RealType; MaxIter:word;
   var N:word; var X,Y,M:array of RealType):boolean;
 { Automatic of cubic spline, using second derivation } 

const
  RelativeOn:boolean= Yes;   
{-------------------------------------------------------------------------}

implementation

function SearchXI(N,M:word; var U:RealType; X:array of RealType):word;
  var
    i,j,k: word;
  begin
    if (U<X[1]) then begin i:=1; 
 {$IFDEF ERROR}  write('[S- ',U,']');   {$ENDIF}  end else
      begin
       if (U>X[N]) then begin i:=N;
 {$IFDEF ERROR}  if (U>X[M]) then write('[S+ ',U,']');   {$ENDIF}  end else
         begin
          i:=1;
          j:=N;
 { двоичный поиск }
          while ((j-i) > 1) do
            begin
              k:=(i+j) shr 1;
              if (U<X[k]) then j:=k else i:=k;
            end;
         end;
      end; 
    SearchXI:=i;
  end;

procedure Spline(N:word; X,Y:array of RealType;
             var B,C,D:array of RealType);
 {
  Вычисляются коэффициенты B[i], C[i] и D[i], i=1..N для кубического
  интерполяционного сплайна вида:
  S(x) = Y[i]+B[i]*(X-X[i])+C[i]*(X-X[i])**2+D[i]*(X-X[i])**3
  для X[i] = X = X[i+1]
  Входная информация:
  N - число заданных точек или узлов (N>=2)
  X - абсциссы узлов в строго возрастающеми порядке
  Y - ординаты узлов
  Выходная информация:
  B,C,D - массивы коэффициентов сплайна
 }
 var
  i,j:integer;
  T : RealType;
 begin
   if (N<2) then exit;
   if (N>2) then
     begin
 { Построить трехдиагональную систему:
  B - диагональ, C - наддиагональ, D - правые части }
       D[1]:=X[2]-X[1];
       C[2]:=(Y[2]-Y[1])/D[1];
       for i:=2 to N-1 do
        begin
          D[i]:=X[i+1]-X[i];
          B[i]:=2*(D[i-1]+D[i]);
          C[i+1]:=(Y[i+1]-Y[i])/D[i];
          C[i]:=C[i+1]-C[i];
        end;
 { Граничные условия. 3-и производные в точках X[1] и X[N]
   вычисляются с помощью разделенных разностей }
       B[1]:=-D[1];
       B[N]:=-D[N-1];
       C[1]:=0;
       C[N]:=0;
       if (N<>3) then
        begin
          C[1]:=C[3]/(X[4]-X[2])-C[2]/(X[3]-X[1]);
          C[N]:=C[N-1]/(X[N]-X[N-2])-C[N-2]/(X[N-1]-X[N-3]);
          C[1]:=C[1]*sqr(D[1])/(X[4]-X[1]);
          C[N]:=C[N]*sqr(D[N-1])/(X[N]-X[N-3]);
        end;
 { прямой ход }
       for i:=2 to N do
        begin
          T:=D[i-1]/B[i-1];
          B[i]:=B[i]-T*D[i-1];
          C[i]:=C[i]-T*C[i-1];
        end;
 { обратная подстановка }
       C[N]:=C[N]/B[N];
       for j:=1 to N-1 do
        C[N-j]:=(C[N-j]-D[N-j]*C[N-j+1])/B[N-j];
 { вычислить коэффициенты полиномов }
       B[N]:=(Y[N]-Y[N-1])/D[N-1]+D[N-1]*(C[N-1]+2*C[N]);
       for i:=1 to N-1 do
        begin
          B[i]:=(Y[i+1]-Y[i])/D[i]-D[i]*(C[i+1]+2*C[i]);
          D[i]:=(C[i+1]-C[i])/D[i];
          C[i]:=3*C[i];
        end;
       C[N]:=3*C[N];
       D[N]:=D[N-1];
     end else {N>2}
     begin
       B[1]:=(Y[2]-Y[1])/(X[2]-X[1]);
       C[1]:=0;
       D[1]:=0;
       B[2]:=B[1];
       C[2]:=0;
       D[2]:=0;
     end;
 end; { Spline }


function Seval(N:word; var U:RealType; X,Y,B,C,D:array of RealType) :RealType;
  
 { Эта функция вычисляет значение кубического сплайна
  Seval= Y[i]+B[i]*(U-X[i])+C[i]*(U-X[i])**2+D[i]*(U-X[i])**3,
  где X[i] < U < X[i+1]. Используется схема Горнера.
  Если U < X[1], то берется значение i=1, если U > X[N], то i=N
  Входная информация:
  N  - число заданных точек
  U  - абсцисса для которой вычисляется значение сплайна
  X,Y  - массивы заданных абсцисс и ординат
  B,C,D  - массивы коэффициентов сплайна
  }

 var
  DX : RealType;
  i:word;
 begin
 { поиск отрезка }
   i:=SearchXI(N,N,U,X);
 { вычислить сплайн }
   DX:=U-X[i];
   Seval:=Y[i]+DX*(B[i]+DX*(C[i]+DX*D[i]));
 end; { Seval }

function CellInter(N:word; var U:RealType; X,Y:array of RealType) :RealType;
 var
  i:word;
 begin
 { поиск отрезка }
   i:=SearchXI(N,N,U,X);
   CellInter:=Y[i];
 end; { CellInter }

function LinInter(N:word; var U:RealType; X,Y:array of RealType) :RealType;
 {
  Эта функция вычисляет значение функции линейной интерполяцией
  Входная информация:
  N  - число заданных точек
  U  - абсцисса для которой вычисляется значение сплайна
  X,Y  - массивы заданных абсцисс и ординат
  }

 var
  i:word;
 begin
 { поиск отрезка }
   i:=SearchXI(N-1,N,U,X);
   LinInter:=Y[i]+(U-X[i])*(Y[i+1]-Y[i])/(X[i+1]-X[i]);
 end; { LinInter }

procedure MSpline(N:word; X,Y:array of RealType; var M:array of RealType);
 var
  H,U,V: RealTypeArray;
  i:word;
  t:RealType;
 begin
  for i:=1 to N-1 do H[i]:=X[i+1]-X[i];
  for i:=2 to N-1 do M[i]:=6*((Y[i+1]-Y[i])/H[i]-(Y[i]-Y[i-1])/H[i-1]);

  t:=(H[1]+2*H[2]);
  V[2]:=(H[1]-H[2])/t;
  U[2]:=H[2]*M[2]/(H[1]+H[2])/t;
  for i:=3 to N-2 do
    begin
      t:=2*(H[i-1]+H[i])+H[i-1]*V[i-1];
      V[i]:=-H[i]/t;
      U[i]:=(M[i]-H[i-1]*U[i-1])/t;
    end;  
  t:=H[N-2]-H[N-1];
  V[N-1]:=0;
  U[N-1]:=(H[N-2]*M[N-1]/(H[N-2]+H[N-1])-t*U[N-2])/
    ((2*H[N-2]+H[N-1])+t*V[N-2]);

  M[N-1]:=U[N-1];
  for i:=N-2 downto 2 do  M[i]:=U[i]+V[i]*M[i+1];

  M[1]:=((H[1]+H[2])*M[2]-H[1]*M[3])/H[2];
  M[N]:=((H[N-2]+H[N-1])*M[N-1]-H[N-1]*M[N-2])/H[N-2];
 end;  {MSpline}

procedure MSplineDrv(N:word; X,Y:array of RealType; var M:array of RealType;
   DerivA,DerivB:RealType);
 { Building of cubic spline, using second derivation, exist first
    derivation on boundary } 
 var
  H,U,V: RealTypeArray;
  i:word;
  t:RealType;
 begin
  for i:=1 to N-1 do H[i]:=X[i+1]-X[i];
  for i:=2 to N-1 do M[i]:=6*((Y[i+1]-Y[i])/H[i]-(Y[i]-Y[i-1])/H[i-1]);

  V[1]:=-0.5;
  U[1]:=3*((Y[2]-Y[1])/H[1]-DerivA)/H[1];
  for i:=2 to N-1 do
    begin
      t:=2*(H[i-1]+H[i])+H[i-1]*V[i-1];
      V[i]:=-H[i]/t;
      U[i]:=(M[i]-H[i-1]*U[i-1])/t;
    end;  
  V[N]:=0;
  U[N]:=(6*(DerivB-(Y[N]-Y[N-1])/H[N-1])/H[N-1]-U[N-1])/(2+V[N-1]);

  M[N]:=U[N];
  for i:=N-1 downto 1 do  M[i]:=U[i]+V[i]*M[i+1];
 end;  {MSplineDrv}

function MSeval(N:word; U:RealType; X,Y,M:array of RealType) :RealType;
 var
  i:word;
  t,h : RealType;
 begin
 { поиск отрезка }
   i:=SearchXI(N-1,N,U,X);
 { вычислить сплайн }
    h:=X[i+1]-X[i];
    t:=(U-X[i])/h;
    MSeval:=(1.0-t)*(Y[i]-sqr(h)*t*((2.0-t)*M[i]+(1.0+t)*M[i+1])/6.0)+Y[i+1]*t;
 end; { MSeval }

function MSevalWS(i:word; U:RealType; X,Y,M:array of RealType) :RealType;
 var
  t,h : RealType;
 begin
 { вычислить сплайн }
    h:=X[i+1]-X[i];
    t:=(U-X[i])/h;
    MSevalWS:=(1.0-t)*(Y[i]-sqr(h)*t*((2.0-t)*M[i]+(1.0+t)*M[i+1])/6.0)+Y[i+1]*t;
 end; { MSeval Without Search }

procedure RatPQSpline(N:word; X,Y,P,Q:array of RealType; var M:array of RealType);
 var
  H,U,V,Qd,Pd: RealTypeArray;
  i:word;
  t:RealType;
 begin
  for i:=1 to N-1 do H[i]:=X[i+1]-X[i];
  for i:=2 to N-1 do M[i]:=2*((Y[i+1]-Y[i])/H[i]-(Y[i]-Y[i-1])/H[i-1]);
  for i:=1 to N do
    begin
      Qd[i]:=3+Q[i]*(3+Q[i]);
      Pd[i]:=3+P[i]*(3+P[i]);
    end;  

  t:=H[2]/Qd[2]*(2+Q[2]+sqr(H[1]/H[2])*(1+Q[1])*(1+Q[2]*Qd[2]))+
     H[1]/Pd[1]*(2+P[1]+               (1+Q[1])*(1+P[1]*Pd[1]));
  V[2]:=-H[2]/Pd[2]*(1-sqr(H[1]/H[2])*(1+Q[1])/(1+P[2]))/t;
  U[2]:=M[2]/t;

  for i:=3 to N-2 do
    begin
      t:=H[i-1]*(2+P[i-1])/Pd[i-1]+H[i]*(2+Q[i])/Qd[i]+H[i-1]*V[i-1]/Qd[i-1];
      V[i]:=-H[i]/Pd[i]/t;
      U[i]:=(M[i]-H[i-1]*U[i-1]/Qd[i-1])/t;
    end;  

  t:=H[N-2]/Qd[N-2]*(1-sqr(H[N-1]/H[N-2])*(1+P[N-1])/(1+Q[N-2]));
  V[N-1]:=0;
  U[N-1]:=(M[N-1]-t*U[N-2])/(t*V[N-2]+
    H[N-2]/Pd[N-2]*(2+P[N-2]+sqr(H[N-1]/H[N-2])*(1+P[N-1])*(1+P[N-2]*Pd[N-2]))+
    H[N-1]/Qd[N-1]*(2+Q[N-1]+                   (1+P[N-1])*(1+Q[N-1]*Qd[N-1])));

  M[N-1]:=U[N-1];
  for i:=N-2 downto 2 do  M[i]:=U[i]+V[i]*M[i+1];

  M[1]:=(1+Q[1])*Qd[1]*(
    M[2]*(   (P[1]+  1/Pd[1])+  H[1]*  (Q[2]+  1/Qd[2])  /H[2])
   -M[3]*  H[1]  /(H[2]*  (1+P[2])*  Pd[2])  );
  M[N]:=(1+P[N-1])*Pd[N-1]*(
    M[N-1]*( (Q[N-1]+1/Qd[N-1])+H[N-1]*(P[N-2]+1/Pd[N-2])/H[N-2])
   -M[N-2]*H[N-1]/(H[N-2]*(1+Q[N-2])*Qd[N-2])  );
 end;  { Rational Spline}

function RatPQSeval(N:word; var U:RealType; X,Y,M,P,Q:array of RealType) :RealType;
 var
  i:word;
  h,t,t1 : RealType;
 begin
 { поиск отрезка }
   i:=SearchXI(N-1,N,U,X);
 { вычислить сплайн }
    h:=X[i+1]-X[i];
    t:=(U-X[i])/h;
    t1:=1.0-t;
    if (t<0) then
      begin
        RatPQSeval:=Y[i]+t*(Y[i+1]-Y[i])+0.5*sqr(h)*(
          M[i+1]*t* (sqr(t) /(1+P[i]*t1)-1)/(3+P[i]*(3+P[i]))  );
      end else  
    if (t1<0) then
      begin
        RatPQSeval:=Y[i]+t*(Y[i+1]-Y[i])+0.5*sqr(h)*(
          M[i]*  t1*(sqr(t1)/(1+Q[i]*t) -1)/(3+Q[i]*(3+Q[i]))  );
      end else  
    RatPQSeval:=Y[i]+t*(Y[i+1]-Y[i])+0.5*sqr(h)*(
      M[i]*  t1*(sqr(t1)/(1+Q[i]*t) -1)/(3+Q[i]*(3+Q[i]))+
      M[i+1]*t* (sqr(t) /(1+P[i]*t1)-1)/(3+P[i]*(3+P[i]))  );
 end; { Rational Seval }

procedure RatSpline(N:word; X,Y,P:array of RealType; var M:array of RealType);
 var
  H,U,V,Pd: RealTypeArray;
  i:word;
  t:RealType;
 begin
  for i:=1 to N-1 do H[i]:=X[i+1]-X[i];
  for i:=2 to N-1 do M[i]:=2*((Y[i+1]-Y[i])/H[i]-(Y[i]-Y[i-1])/H[i-1]);
  for i:=1 to N do Pd[i]:=3+P[i]*(3+P[i]);

  t:=H[2]/Pd[2]*(2+P[2]+sqr(H[1]/H[2])*(1+P[1])*(1+P[2]*Pd[2]))+
     H[1]/Pd[1]*(2+P[1]+               (1+P[1])*(1+P[1]*Pd[1]));
  V[2]:=-H[2]/Pd[2]*(1-sqr(H[1]/H[2])*(1+P[1])/(1+P[2]))/t;
  U[2]:=M[2]/t;

  for i:=3 to N-2 do
    begin
      t:=H[i-1]*(2+P[i-1]+V[i-1])/Pd[i-1]+H[i]*(2+P[i])/Pd[i];
      V[i]:=-H[i]/Pd[i]/t;
      U[i]:=(M[i]-H[i-1]*U[i-1]/Pd[i-1])/t;
    end;  

  t:=H[N-2]/Pd[N-2]*(1-sqr(H[N-1]/H[N-2])*(1+P[N-1])/(1+P[N-2]));
  V[N-1]:=0;
  U[N-1]:=(M[N-1]-t*U[N-2])/(t*V[N-2]+
    H[N-2]/Pd[N-2]*(2+P[N-2]+sqr(H[N-1]/H[N-2])*(1+P[N-1])*(1+P[N-2]*Pd[N-2]))+
    H[N-1]/Pd[N-1]*(2+P[N-1]+                   (1+P[N-1])*(1+P[N-1]*Pd[N-1])));

  M[N-1]:=U[N-1];
  for i:=N-2 downto 2 do  M[i]:=U[i]+V[i]*M[i+1];

  M[1]:=(1+P[1])*Pd[1]*(
    M[2]*(   (P[1]+  1/Pd[1])+  H[1]*  (P[2]+  1/Pd[2])  /H[2])
   -M[3]*  H[1]  /(H[2]*  (1+P[2])*  Pd[2])  );
  M[N]:=(1+P[N-1])*Pd[N-1]*(
    M[N-1]*( (P[N-1]+1/Pd[N-1])+H[N-1]*(P[N-2]+1/Pd[N-2])/H[N-2])
   -M[N-2]*H[N-1]/(H[N-2]*(1+P[N-2])*Pd[N-2])  );
 end;  { Rational Spline}

function RatSeval(N:word; var U:RealType; X,Y,M,P:array of RealType) :RealType;
 var
  i:word;
  h,t,t1 : RealType;
 begin
 { поиск отрезка }
   i:=SearchXI(N-1,N,U,X);
 { вычислить сплайн }
    h:=X[i+1]-X[i];
    t:=(U-X[i])/h;
    t1:=1.0-t;
    if (t<0) then
      begin
        RatSeval:=Y[i]+t*(Y[i+1]-Y[i])+0.5*sqr(h)*(
          M[i+1]*t*(sqr(t)/(1+P[i]*t1)-1) 
          )/(3+P[i]*(3+P[i]));
      end else  
    if (t1<0) then
      begin
        RatSeval:=Y[i]+t*(Y[i+1]-Y[i])+0.5*sqr(h)*(
          M[i]*t1*(sqr(t1)/(1+P[i]*t)-1)
          )/(3+P[i]*(3+P[i]));
      end else  
    RatSeval:=Y[i]+t*(Y[i+1]-Y[i])+0.5*sqr(h)*(
      M[i]*t1*(sqr(t1)/(1+P[i]*t)-1)+ M[i+1]*t*(sqr(t)/(1+P[i]*t1)-1) 
      )/(3+P[i]*(3+P[i]));
 end; { Rational Seval }

procedure SmoothSpline(N:word; X,Y:array of RealType; var Z,M:array of RealType;
  RelError:RealType);

 var
  H,G: RealTypeArray;

  procedure IterSmooth(R:RealType);
    var
      A,B,C,V,U,W:RealTypeArray;
      S:RealType;
      i:word;
    begin
      for i:=2 to N-1 do A[i]:=(h[i-1]+h[i])/3+
        R*(1/sqr(h[i-1])+sqr(1/h[i-1]+1/h[i])+1/sqr(h[i]));
      for i:=2 to N-2 do B[i]:=h[i]/6-(R*(1/h[i-1]+2/h[i]+1/h[i+1]))/h[i];
      for i:=2 to N-3 do C[i]:=R/(h[i]*h[i+1]);
      {}
      A[1]:=1;
      B[1]:=0;
      C[1]:=0;
      A[N]:=1;
      B[N-1]:=0;
      C[N-2]:=0;
      {}
      W[1]:=A[1];
      U[1]:=B[1]/A[1];
      V[1]:=C[1]/A[1];
      Z[1]:=G[1];
      W[2]:=A[2]-U[1]*B[1];
      S:=B[2]-U[1]*C[1];
      U[2]:=S/A[2];
      V[2]:=C[2]/A[2];
      Z[2]:=G[2]-U[1]*Z[1];
      for i:=3 to N do
        begin
          W[i]:=A[i]-V[i-2]*C[i-2]-U[i-1]*S;
          S:=B[i]-U[i-1]*C[i-1];
          U[i]:=S/W[i];
          V[i]:=C[i]/W[i];
          Z[i]:=G[i]-V[i-2]*Z[i-2]-U[i-1]*Z[i-1];
        end;  
      {}
      M[N]:=Z[N]/W[N];
      M[N-1]:=Z[N-1]/W[N-1]-U[N-1]*M[N];
      for i:=N-2 downto 1 do M[i]:=Z[i]/W[i]-U[i]*M[i+1]-V[i]*M[i+2];
      Z[1]:=(M[2]-M[1])/h[1];
      for i:=2 to N-1 do Z[i]:=(M[i+1]-M[i])/h[i]-(M[i]-M[i-1])/h[i-1];
      Z[N]:=-(M[N]-M[N-1])/h[N-1];
    end;  
    
 var
  i,j:word;
  Ok:boolean;
  R,Rmax,Rmax2,S:RealType;
 begin
  for i:=1 to N-1 do h[i]:=X[i+1]-X[i];
  for i:=1 to N-1 do G[i]:=(Y[i+1]-Y[i])/h[i];
  for i:=N-1 downto 2 do G[i]:=G[i]-G[i-1];  G[1]:=0;  G[N]:=0;
  if (RelError<>0) then R:=1 else R:=0;
  {}
  Ok:=false;
  Rmax2:=0;
  repeat
    IterSmooth(R);
    Rmax:=0;
    for i:=1 to N do
      begin
        if (Y[i]<>0) then S:=abs(Z[i]/Y[i]) else S:=0;
        if (S>Rmax) then Rmax:=S;
      end;  
    if (RelError<>0) then Rmax:=Rmax*R/RelError else Rmax:=0.9;
    if (Rmax>0.8) and (Rmax<1) then Ok:=true else 
      begin
        if (RelError>1) and (Rmax<Rmax2) then Ok:=true else 
          begin
            R:=R*0.9/Rmax;
            Rmax2:=Rmax;
          end; 
      end;
  until(Ok);  
  {}
  for i:=1 to N do Z[i]:=Y[i]-R*Z[i]; 
 end;  { Smooth Spline}


function AutoMSpline(Func:FunType; A,B,AbsErr:RealType; MaxIter:word;
   var N:word; var X,Y,M:array of RealType):boolean;
 { Automatic of cubic spline, using second derivation } 
 var
   i,L,CurIter:word;
   CurEps,XOld,YOld,XNew,YNew:RealTypeArray;
   h,EpsMax,EpsCrit:RealType;
   Ok,NewPoint:boolean;
 begin
    AutoMSpline:=false;
    h:=(B-A)/(N-1);
    for i:=1 to N-1 do
      begin
        XOld[i]:=A+h*(i-1);
        YOld[i]:=Func(XOld[i]);
        XOld[N+i]:=XOld[i]+0.5*h;
        YOld[N+i]:=Func(XOld[i]+0.5*h);
      end;
    XOld[N]:=B;
    YOld[N]:=Func(B);
    Ok:=false;
    CurIter:=0;
    repeat
      inc(CurIter);
      MSpline(N,XOld,YOld,M);
      EpsMax:=0;
      for i:=1 to N-1 do
        begin
          h:=0.5*(XOld[i+1]+XOld[i]);
          CurEps[i]:=abs(YOld[N+i]-MSeval(N,h,X,Y,M));
          if (CurEps[i]>EpsMax) then EpsMax:=CurEps[i];
        end;
      Ok:=(CurIter>=MaxIter) or (EpsMax<AbsErr) or (N>ArraySize div 4);
      if (Not Ok) then
        begin
          i:=0;
          L:=0;
          NewPoint:=false;
          repeat
            inc(i);
            inc(L);
            XNew[L]:=XOld[i];
            YNew[L]:=YOld[i];
            if (L>1) then if (not NewPoint) then
                begin
                  XNew[N+L-1]:=XOld[N+i-1];
                  YNew[2*N+L-1]:=YOld[N+i-1];
                end else
                begin
                  h:=0.5*(XNew[L]+XNew[L-1]);
                  XNew[2*N+L-1]:=h;
                  YNew[2*N+L-1]:=Func(h);
                end;
            NewPoint:=false;
            if (CurEps[i]>AbsErr) then
              begin
                inc(L);
                XNew[L]:=XOld[N+i];
                YNew[L]:=YOld[N+i];
                h:=0.5*(XNew[L]+XNew[L-1]);
                XNew[2*N+L-1]:=h;
                YNew[2*N+L-1]:=Func(h);
                NewPoint:=true;
              end;
          until(i=N);
          i:=(L+1)*SizeOf(RealType);
          Move(XNew,X,i);
          Move(YNew,Y,i);
          i:=L*SizeOf(RealType);
          Move(XNew[2*N+1],XOld[L+1],i);
          Move(YNew[2*N+1],YOld[L+1],i);
          N:=L;
         writeln('Iteration= ',CurIter,'  Error=',EpsMax,' N= ',N);
        end;
    until(Ok);
    i:=(N+1)*SizeOf(RealType);
    Move(XOld,X,i);
    Move(YOld,Y,i);
 end; 

end.

*/
