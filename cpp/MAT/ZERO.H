#ifndef __ZERO_H
#define __ZERO_H

#include "lib\std\deffunc.h"
    
double ZeroIn(double AX,double BX,X_func F,double Tol);
/* { Процедура осуществляет поиск нуля функции F(x) в интервале [AX,BX] 
   Входная информация:
    AX - левая граница
    BX - правая граница
    F  - функция
    Tol - неопределенность
   Выходная информация:
    ZeroIn вычисляет X при котором F(X)=0
   Без проверки предполагается, что F(AX) и F(BX) имеют противоположные знаки
   Допуск на ошибку - 4*MachEps*abs(X)+Tol, где MachEps - машинная точность
 }
 */
int ZeroMullerIt(X_func Func,double &CurPnt,double Tol,int MaxIter);
int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter);
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down) ;
int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
          double Up,double Down,double MaxStp=MathInf,double BegStpMull=0.01) ;


#endif
