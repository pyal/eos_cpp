#include "lib/std/deffunc.h"
//#include "cmatrics.h"
// Kvitovs module

#ifndef __MTQUAD_H
#define __MTQUAD_H

//typedef CVecCl (*Vec_func)   (double x);

extern int NumRecIntIterations;
double QuNC10(X_func Func,double Up,double Low);

//{ Recursive quadrature at interval [A,B] (QuNC10 quadrature) }
double RecInt(X_func Func,double Low,double Up,double AbsErr);

// Unlimited Integrals
// Procedures for integrating of one-dimensional function
//=============================================================================
// Input and Output variables:
//  Func - function under integral
//  A    - left bound
//  B    - right bound
//  AbsErr - absolute error of integral
//  MaxFunc - maximum number of function calculation
//   ( when MaxFunc reached, then integrating interrupted )
//  Flag - error variable, if Flag=0 then integral is O'k, else 
//    Flag point to calculated part of integral, where integrating interrupted 
// Infinity integrals:
//  InFinZero - minimum value of integral at interval,while integrating not interrupted 
//  NumZero - number of intervals with integral < InFinZero  
// Abbreviation of quadrature formules:
// nc* - Newton-Cotes quadrature of order *
// lb* - Lobbatto quadrature of order *
//=============================================================================}
double InFinQunc10(X_func Func,double A,double AbsErr,double InFinZero,
                   int NumZero,long MaxFunc,int &Flag);
double InFinQulb9(X_func Func,double A,double AbsErr,double InFinZero,
                   int NumZero,long MaxFunc,int &Flag);
//CVecCl InFinQulb9(Vec_func Func,double A,double AbsErr,double InFinZero,
//                   int NumZero,long MaxFunc,int &Flag);
#endif
