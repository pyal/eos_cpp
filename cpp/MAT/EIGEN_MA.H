#ifndef __EIGN_MA_H
#define __EIGN_MA_H
/*
#include <stdio.h>
#include <fstream.h>
#include <strstream.h>
#include <math.h>
#include <const.h>
#include <stdlib.h>
#include <mem.h>
*/
#include "matrics.h"
#include "cmatrics.h"
#include "lib/std/util.h"
#include "uralgol.h"

//  Return int Error
int Jacobi(MatrCl &Mat,int MaxIter,double Tolerance,
            double *Eigenvalues,MatrCl &Eigenvectors,
            int &Iter);

/*
{-     Input: Dimen, Mat, MaxIter, Tolerance                                -}
{-    Output: Eigenvalues, Eigenvector, Iter, Error                         -}
{-                                                                          -}
{-   Purpose: The eigensystem of a symmetric matrix can be                  -}
{-            computed much more simply than the                            -}
{-            eigensystem of an arbitrary matrix.  The                      -}
{-            cyclic Jacobi method is an iterative                          -}
{-            technique for approximating the complete                      -}
{-            eigensystem of a symmetric matrix to a given                  -}
{-            tolerance. The method consists of multiplying                 -}
{-            the matrix, Mat, by a series of rotation                      -}
{-            matrices, r@-[i].  The rotation matrices are                  -}
{-            chosen so that the elements of the upper                      -}
{-            triangular part of Mat are systematically                     -}
{-            annihilated.  That is, r@-[1] is chosen so                    -}
{-            that Mat[1, 1] is identically zero; r@-[2] is                 -}
{-            chosen so that Mat[1, 2] is identically zero;                 -}
{-            etc.  Since each operation will probably                      -}
{-            change the value of elements annihilated in                   -}
{-            previous operations, the method is iterative.                 -}
{-            Eventually, the matrix will be diagonal. The                  -}
{-            eigenvalues will be the elements along the                    -}
{-            diagonal of the matrix and the eigenvectors                   -}
{-            will be the rows of the matrix created by the                 -}
{-            product of all the rotation matrices r@-[i].                  -}
{-                                                                          -}
{-            The user inputs the matrix, tolerance and maximum             -}
{-            number of iterations. The procedure returns the               -}
{-            eigenvalues and eigenvectors (or error code) of the           -}
{-            matrix.                                                       -}
{-                                                                          -}
{-   Global Variables:  Dimen        : integer   Dimension of square matrix -}
{-                      Mat          : TNmatrix  Square matrix              -}
{-                      MaxIter      : integer   Maximum number of          -}
{-                                               Iterations                 -}
{-                      Tolerance    : real      Tolerance in answer        -}
{-                      Eigenvalues  : TNvector  Eigenvalues of Mat         -}
{-                      Eigenvectors : TNmatrix  Eigenvectors of Mat        -}
{-                      Iter         : integer   Number of iterations       -}
{-                      Error        : byte      Error code                 -}
{-                                                                          -}
{-             Errors:  0: No error                                         -}
{-                      1: Dimen < 1                                        -}
{-                      2: Tolerance < TNNearlyZero                         -}
{-                      3: MaxIter < 1                                      -}
{-                      4: Mat not symmetric                                -}
{-                      5: Iter > MaxIter                                   -}
{-                                                                          -}
*/
void NormalizeEigenvectors(MatrCl &Eigenvectors);
/*
{- This procedure normalizes the eigenvectors so   -}
{- that the largest element in each vector is one. -}
*/
void tred2(MatrCl &InMat,double *Diag,double *DiagPl,MatrCl &Conv,double Tol);

int tql2(double *Diag,double *DiagPl,MatrCl &Conv);

//  Eigenvectors are in columns
int ReduceLQ(MatrCl &InMat,MatrCl &EigenVect,double *EigenVal,double Tol);
//void imtql2

int CReduceLQ(CMatrCl &InMat,CMatrCl &EigenVect,double *EigenVal,double Tol);

#endif
 
