#ifndef __URALGOL_H
#define __URALGOL_H

//#include <complex.h>
#include "lib/std/my_comp.h"
#include <math.h>
#include "cmatrics.h"
#include "matrics.h"
#include "lib/std/util.h"
//   ������ᮭ ������ " ��ࠢ�筨� �����⬮� �� �몥 �����"

//        =================================================
//        ============    LQ  method   ====================
//        =================================================
//  �८�ࠧ������ ����宫줥� ��� ᨬ�����᪮� ������. ��� 194
void URtred2(MatrCl &InMat,double *Diag,double *DiagPl,MatrCl &Conv,
                                                        double Tol);
//  QL - ������ ��� ��夨������쭮� ������. ��� 208
int URtql2(double *Diag,double *DiagPl,MatrCl &Conv);



//        =================================================
//        ===  ����� ��楤�� ��� �襭�� ��⥬ ��-�� ====          
//        =======  � �ந����쭮� ����楩.  ==============
//        =================================================

//  ���������� �ந�������� � �㬬�஢���� �� � ������� �筮����
//                         ��� 43

long double URinnerpod(int l,int s,int u,long double c,const VecCl &a,const VecCl &b);

//     ���������� ��室��� ������ �� �ந�������� 2 ��㣮����
//  � �ᯮ�짮������ �����⬠ ���� + �������� det(A). ��� 97
//  � d1,d2 - ࠭� ������ det(a)=d1*2^(d2)
int URunsymdet(MatrCl &a,double eps,double &d1,int &d2,int *rear);

void URunsymsol(MatrCl &a,int *rear,VecCl &b);

// r - Number Vect to solve, a - Matrics to solve, aa - LU decomposed matrics
// rear - array of rearangements, b - array of vectors to solve, 
// eps -⠪�� �� :  1-eps=1, ��� double ->  eps=1e-14
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
         
//  ������஢���� ��⮤ 类�� ��� �ந����쭮� ������
//  � ��砥 ᮢ������� ��୥� �室������ �祭� ���������
//  ��� 356
//  ��砫쭠� ����� ࠧ��頥��� � ���ᨢ� a
//  �� ��室� - � a - �� ��������� - ᮡ�⢥��� ���祭��
//              � t - ᮡ�⢥��� �����
int URComeig(CMatrCl &a,CMatrCl &Lft,CMatrCl &Rgt);
//int URComeig(CMatrCl &a,CMatrCl &t);

#endif
