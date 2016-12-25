#ifndef __URALGOL_H
#define __URALGOL_H

//#include <complex.h>
#include "lib/std/my_comp.h"
#include <math.h>
#include "cmatrics.h"
#include "matrics.h"
#include "lib/std/util.h"
//   Уилкинсон Райнаш " Справочник алгоритмов на языке Алгол"

//        =================================================
//        ============    LQ  method   ====================
//        =================================================
//  Преобразование Хаусхольдера для симметрической матрицы. стр 194
void URtred2(MatrCl &InMat,double *Diag,double *DiagPl,MatrCl &Conv,
                                                        double Tol);
//  QL - алгоритм для трехдиагональной матрицы. стр 208
int URtql2(double *Diag,double *DiagPl,MatrCl &Conv);



//        =================================================
//        ===  Набор процедур для решения систем ур-ий ====
//        =======  с произвольной матрицей.  ==============
//        =================================================

//  накопление произведений и суммирование их с двойной точностью
//                         стр 43

long double URinnerpod(int l,int s,int u,long double c,const VecCl &a,const VecCl &b);

//     Разложение исходной матрицы на произведение 2 треугольных
//  с использованием алгоритма Краута + вычисляется det(A). стр 97
//  в d1,d2 - ранг матрицы det(a)=d1*2^(d2)
int URunsymdet(MatrCl &a,double eps,double &d1,int &d2,int *rear);

void URunsymsol(MatrCl &a,int *rear,VecCl &b);

// r - Number Vect to solve, a - Matrics to solve, aa - LU decomposed matrics
// rear - array of rearangements, b - array of vectors to solve, 
// eps -такое что :  1-eps=1, для double ->  eps=1e-14
// x - array of result vect, bb - array of misfit of result vect, l - num iter
int URunsymaccsol(MatrCl &a,MatrCl &aa,int *rear,VecCl &b,double eps,
                  MatrCl &x,MatrCl &bb,int &l);

int SolveAxb(MatrCl &a,VecCl &b,VecCl &x,double Tol=1e-14);
         


my_comp URCinnerpod(int l,int s,int u,
                         my_comp c,CVecCl &a,CVecCl &b);
int URCunsymdet(CMatrCl &a,double eps,my_comp &d1,int &d2,int *rear);
void URCunsymsol(CMatrCl &a,int *rear,CVecCl &b);
int URCunsymaccsol(CMatrCl &a,CMatrCl &aa,int *rear,CVecCl &b,double eps,
                  CVecCl &x,CVecCl &bb,int &l);

int CSolveAxb(CMatrCl &a,CVecCl &b,CVecCl &x,double Tol=1e-14);
         
//  Модифицированный метод якоби для произвольной матрицы
//  в случае совпадающих корней сходимость очень медленная
//  стр 356
//  Начальная Матрица размещается в массиве a
//  на выходе - в a - по диагонали - собственные значения
//              в t - собственные вектора
int URComeig(const CMatrCl &a,CMatrCl &Lft,CMatrCl &Rgt);
//int URComeig(CMatrCl &a,CMatrCl &t);

#endif
