#include <lib/precompiled/math.h>

#include "spline.h"

long SearchXI(long N,long M,double U,double *X)
  {
   long i,j,k;
   if (U<X[1]) i=1; 
// {$IFDEF ERROR}  write('[S- ',U,']');   {$ENDIF}  } 
   else
      if (U>X[N]) i=N;
// {$IFDEF ERROR}  if (U>X[M]) then write('[S+ ',U,']');   {$ENDIF}  } else
      else
        {
          i=1;
          j=N;
// { двоичный поиск }
          while ((j-i) > 1) 
            {
             k=(int)( ((double)(i+j))/2 );
             if (U<X[k]) j=k; else i=k;
            }
         } 
    return i;
  };
void Spline(long N,double *X,double *Y,double *B,double *C,double *D)
/*
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
*/
  {
   double T;
   if (N<2) return;
   if (N>2) 
     {
// { Построить трехдиагональную систему:
//  B - диагональ, C - наддиагональ, D - правые части }
       D[1]=X[2]-X[1];
       C[2]=(Y[2]-Y[1])/D[1];
       for (int i=2;i<N;i++)
        {
          D[i]=X[i+1]-X[i];
          B[i]=2*(D[i-1]+D[i]);
          C[i+1]=(Y[i+1]-Y[i])/D[i];
          C[i]=C[i+1]-C[i];
        };
// { Граничные условия. 3-и производные в точках X[1] и X[N]
//   вычисляются с помощью разделенных разностей }
       B[1]=-D[1];
       B[N]=-D[N-1];
       C[1]=0;
       C[N]=0;
       if (N!=3)  
         {
          C[1]=C[3]/(X[4]-X[2])-C[2]/(X[3]-X[1]);
          C[N]=C[N-1]/(X[N]-X[N-2])-C[N-2]/(X[N-1]-X[N-3]);
          C[1]=C[1]*(D[1]*D[1])/(X[4]-X[1]);
          C[N]=C[N]*(D[N-1]*D[N-1])/(X[N]-X[N-3]);
         };
// { прямой ход }
       for (int i=2;i<=N;i++)
         {
          T=D[i-1]/B[i-1];
          B[i]=B[i]-T*D[i-1];
          C[i]=C[i]-T*C[i-1];
         };
// { обратная подстановка }
       C[N]=C[N]/B[N];
       for (int j=2;j< N;j++)  
         C[N-j]=(C[N-j]-D[N-j]*C[N-j+1])/B[N-j];
// { вычислить коэффициенты полиномов }
       B[N]=(Y[N]-Y[N-1])/D[N-1]+D[N-1]*(C[N-1]+2*C[N]);
       for (int i=1;i<N;i++)
        {
          B[i]=(Y[i+1]-Y[i])/D[i]-D[i]*(C[i+1]+2*C[i]);
          D[i]=(C[i+1]-C[i])/D[i];
          C[i]=3*C[i];
        };
       C[N]=3*C[N];
       D[N]=D[N-1];
     } 
   else //{N>2}
      {
       B[1]=(Y[2]-Y[1])/(X[2]-X[1]);
       C[1]=0;
       D[1]=0;
       B[2]=B[1];
       C[2]=0;
       D[2]=0;
      }
  }; //{ Spline }

double Seval(long N,double U,double *X,double *Y,double *B,double *C,
                                                            double *D)
/*  
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
*/
  {
   double DX;
// { поиск отрезка }
   long i=SearchXI(N,N,U,X);
// { вычислить сплайн }
   DX=U-X[i];
   return Y[i]+DX*(B[i]+DX*(C[i]+DX*D[i]));
 };// { Seval }


