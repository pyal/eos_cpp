#ifndef __MAT_GAUS_H
#define __MAT_GAUS_H
#include "lib\std\util.h"
#include "matrics.h"

struct GauseSolve
  {
   MatrCl Matrics,Low,Up; 
   int *PermuteRes;
   int Dimenshion;
   GauseSolve(){Dimenshion=-1;};
   ~GauseSolve(){if (Dimenshion!=-1) delete PermuteRes;};
   int NotLUSolve(MatrCl &Matr,VecCl &ConstVec,VecCl &ResultVec);

   private:
   int Initialize(MatrCl &Matr);
   int LU_Decompose();
   void SetLU(double coef,int k);
   double PivotEl(int k);
   void Change(MatrCl &matr,int i,int j);
   int LU_Solve(VecCl& ConstVec,VecCl& ResVec);
   void PermuteConstants(VecCl& ConstVec);
   void DisposeOld();
  };

/*      
//   int NotIterLU(MatrCl &matr,double *constant,double* result,
//                                            double Mist);
   void   GetDist(double *TrueConst,double *Sol1,double *dist);
   double Norm(double *Sol1);

struct QRSolve
  {
   MatrCl Matrics; 
   double *MDiag,*QDiag;
   int Dimenshion;
   int NotQRSolve(MatrCl &matr,double *res,double* (&vec_var));

   private:
   int Initialize(MatrCl &matr,double *res,double* &vec_var);
   int LU_Decompose();
   void SetLU(double coef,int k);
   double PivotEl(int k);
   void Change(MatrCl &matr,int i,int j);
   int LU_Solve();
   void PermuteConstants();
   void DisposeInter();
  };

*/


#endif