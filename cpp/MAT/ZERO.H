#ifndef __ZERO_H
#define __ZERO_H

#include "lib/std/deffunc.h"
    
double ZeroIn(double AX,double BX,X_func F,double Tol);
/* { ��楤�� �����⢫�� ���� ��� �㭪樨 F(x) � ���ࢠ�� [AX,BX] 
   �室��� ���ଠ��:
    AX - ����� �࠭��
    BX - �ࠢ�� �࠭��
    F  - �㭪��
    Tol - ����।���������
   ��室��� ���ଠ��:
    ZeroIn ������ X �� ���஬ F(X)=0
   ��� �஢�ન �।����������, �� F(AX) � F(BX) ����� ��⨢�������� �����
   ����� �� �訡�� - 4*MachEps*abs(X)+Tol, ��� MachEps - ��設��� �筮���
 }
 */
int ZeroMullerIt(X_func Func,double &CurPnt,double Tol,int MaxIter);
int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter);
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down) ;
int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
          double Up,double Down,double MaxStp=MathInf,double BegStpMull=0.01) ;


#endif
