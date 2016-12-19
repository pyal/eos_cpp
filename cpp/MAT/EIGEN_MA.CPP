#include <lib/precompiled/math.h>
#include "eigen_ma.h"
//  Return int Error
int TestData(MatrCl &Mat,int MaxIter,double Tolerance)
  {
   int Error=0,Dimen=Mat.Dim(),Column,Row;
  if (Dimen < 1 )  Error= 1;
  if (Tolerance <= MathZer) Error= 2;
  if (MaxIter < 1) Error= 3;
  if (!Error)
    for (Row=1;Row<Dimen;Row++)
      {
       for (Column=Row+1;Column<=Dimen;Column++)
        if (fabs(Mat(Row,Column)-Mat(Column,Row)) >0.01* /*MathZer*/Tolerance)
                                                                     Error=4;
//        { Matrix not symmetric }
      };
   return Error;
  };
void Initialize(int &Iter,MatrCl &Eigenvectors)

// This procedure initializes Iter to zero  -}
// and Eigenvectors to the identity matrix. -}
  {
   Iter=0;
   Eigenvectors=Eigenvectors*0;Eigenvectors=Eigenvectors+1;
  };

void CalculateRotation(double RowRow,double RowCol ,double ColCol,  
                       double &SinTheta,double &CosTheta)

/*
{- This procedure calculates the sine and cosine of the    -}
{- angle Theta through which to rotate the matrix Mat.     -}
{- Given the tangent of 2-Theta, the tangent of Theta can  -}
{- be calculated with the quadratic formula.  The cosine   -}
{- and sine are easily calculable from the tangent. The    -}
{- rotation must be such that the Row, Column element is   -}
{- zero. RowRow is the Row,Row element; RowCol is the      -}
{- Row,Column element; ColCol is the Column,Column element -}
{- of Mat.                                                 -}
*/
  {
  double TangentTwoTheta, TangentTheta, Dummy;
  if (fabs(RowRow - ColCol) > MathZer)
    {
     TangentTwoTheta = (RowRow - ColCol) / (2 * RowCol);
     Dummy = sqrt(sqr(TangentTwoTheta) + 1);
     if (TangentTwoTheta < 0)  // Choose the root nearer to zero
        TangentTheta = -TangentTwoTheta - Dummy;
      else
        TangentTheta = -TangentTwoTheta + Dummy;
      CosTheta = 1 / sqrt(1 + sqr(TangentTheta));
      SinTheta = CosTheta * TangentTheta;
    }
  else
    {
      CosTheta = sqrt(0.5);
      if (RowCol < 0) SinTheta = -sqrt(0.5);
      else            SinTheta =  sqrt(0.5);
    }
  };

void RotateMatrix(double SinTheta,double CosTheta,
                  int Row,int Col,MatrCl &Mat)
/*
{- This procedure rotates the matrix Mat through an angle     -}
{- Theta.  The rotation matrix is the identity matrix execept -}
{- for the Row,Row; Row,Col; Col,Col; and Col,Row elements.   -}
{- The rotation will make the Row,Col element of Mat          -}
{- to be zero.                                                -}
*/
  {
   double CosSqr=sqr(CosTheta) , SinSqr=sqr(SinTheta) , SinCos,
          MatRowRow, MatColCol, MatRowCol, MatRowIndex, MatColIndex;
   int Dimen=Mat.Dim();
   SinCos = SinTheta * CosTheta;
   MatRowRow = Mat(Row, Row) * CosSqr + 2 * Mat(Row, Col) * SinCos +
               Mat(Col, Col) * SinSqr;
   MatColCol = Mat(Row, Row) * SinSqr - 2 * Mat(Row, Col) * SinCos +
               Mat(Col, Col) * CosSqr;
   MatRowCol = (Mat(Col, Col) - Mat(Row, Row)) * SinCos +
               Mat(Row, Col) * (CosSqr - SinSqr);

  for (int Index=1;Index<=Dimen;Index++)
    if ( (Index!=Row) && (Index!=Col) )
       {
        MatRowIndex = Mat(Row, Index) * CosTheta +
                      Mat(Col, Index) * SinTheta;
        MatColIndex = -Mat(Row, Index) * SinTheta +
                       Mat(Col, Index) * CosTheta;
        Mat(Row, Index) = MatRowIndex;
        Mat(Index, Row) = MatRowIndex;
        Mat(Col, Index) = MatColIndex;
        Mat(Index, Col) = MatColIndex;
       };
   Mat(Row, Row) = MatRowRow;
   Mat(Col, Col) = MatColCol;
   Mat(Row, Col) = MatRowCol;
   Mat(Col, Row) = MatRowCol;
  };
void RotateEigenvectors(double SinTheta,double CosTheta,int Row,int Col,
                        MatrCl &Eigenvectors)
/*
{- This procedure rotates the Eigenvectors matrix through an  -}
{- angle Theta.  The rotation matrix is the identity matrix   -}
{- except for the Row,Row; Row,Col; Col,Col; and Col,Row      -}
{- elements.  The Eigenvectors matrix will be the product of  -}
{- all the rotation matrices which operate on Mat.            -}
*/
  {
   double EigenvectorsRowIndex, EigenvectorsColIndex;
   int Dimen=Eigenvectors.Dim();
//  { Transform eigenvector matrix }
  for (int Index=1;Index<=Dimen;Index++)
    {
     EigenvectorsRowIndex = CosTheta * Eigenvectors(Row, Index) +
                            SinTheta * Eigenvectors(Col, Index);
     EigenvectorsColIndex = -SinTheta * Eigenvectors(Row, Index) +
                             CosTheta * Eigenvectors(Col, Index);
     Eigenvectors(Row, Index) = EigenvectorsRowIndex;
     Eigenvectors(Col, Index) = EigenvectorsColIndex;
    }
  };

double FindLargest(VecCl &EigenvectorRow)
/*
{- This procedure returns the value of -}
{- the largest element of the vector.  -}
*/
  {
   double Largest = EigenvectorRow[1];
   int Dimen= EigenvectorRow.Dim();
   for (int Term= 2;Term<=Dimen;Term++)
    if (fabs(EigenvectorRow[Term]) > fabs(Largest)) 
      Largest = EigenvectorRow[Term];
   return Largest;
  };
void NormalizeEigenvectors(MatrCl &Eigenvectors)
/*
{- This procedure normalizes the eigenvectors so   -}
{- that the largest element in each vector is one. -}
*/
  {
   int Dimen=Eigenvectors.Dim();
   VecCl RowVec;
   for (int Row = 1;Row<=Dimen;Row++){
		RowVec=MatrCl::GetRow(Eigenvectors,Row);
		MatrCl::SetRow(Eigenvectors,Row,RowVec/FindLargest(RowVec));
    }
  };

//  Return int Error
int Jacobi(MatrCl &Mat,int MaxIter,double Tolerance,
            double *Eigenvalues,MatrCl &Eigenvectors,
            int &Iter)
  {
   int Error,Done,Row,Column,Diag,Dimen=Mat.Dim() ;
   double SinTheta, CosTheta, SumSquareDiag ;
   MatrCl TmpEigenV(Dimen);
   Error=TestData(Mat, MaxIter, Tolerance);
   if (Error == 0)
     {
      Initialize(Iter, TmpEigenV);
      do
        {
        Iter++;SumSquareDiag = 0;
        for (Diag = 1;Diag<=Dimen;Diag++)
          SumSquareDiag = SumSquareDiag + sqr(Mat(Diag, Diag));
        Done = 1;
        for (Row = 1;Row<Dimen;Row++)
          {
           for (Column = Row + 1;Column<=Dimen;Column++)
             if (fabs(Mat(Row, Column)) > Tolerance * SumSquareDiag)
               {
//cout<<" Mat Before rot\n"<<Mat<<"Eigen V\n"<<TmpEigenV;
                Done = 0;

                CalculateRotation(Mat(Row, Row), Mat(Row, Column),
                                  Mat(Column, Column), SinTheta, CosTheta);
//cout<<" Sin "<<SinTheta<<" Cos "<<CosTheta;
                RotateMatrix(SinTheta, CosTheta, Row, Column, Mat);
                RotateEigenvectors(SinTheta, CosTheta, Row, Column,
                                   TmpEigenV);
//cout<<" Mat After  rot\n"<<Mat<<"Eigen V\n"<<TmpEigenV;ChRead();
               }
          }
        }
      while ((!Done) && (Iter < MaxIter));
      for (Diag = 1;Diag<=Dimen;Diag++)
        Eigenvalues[Diag] = Mat(Diag, Diag);
//cout<<" EigenVectors :\n"<<TmpEigenV;
      NormalizeEigenvectors(TmpEigenV);
      if (Iter > MaxIter) Error = 5;
     }
   Eigenvectors=TmpEigenV;
   return Error;
  };


int ReduceLQ(MatrCl &InMat,MatrCl &EigenVect,double *EigenVal,double Tol)
  {
   int N=InMat.Dim();
   VecCl DiagPl(N);//=new double[N+1];DiagPl[0]=N;
//cout<<" InMat\n"<<InMat;
   URtred2(InMat,EigenVal,DiagPl.Ptr,EigenVect,Tol);
//VecCl VEigenVal,VDiagPl;
//cout<<" Diag\n"<<(VEigenVal=EigenVal)<<"\nDiagPl\n"<<(VDiagPl=DiagPl)<<"\nMatr\n"<<EigenVect;
   int ret=URtql2(EigenVal,DiagPl.Ptr,EigenVect);
//cout<<" EigenVal\n"<<(VEigenVal=EigenVal)<<"\nDiagPl\n"<<(VDiagPl=DiagPl)<<"\nMatr\n"<<EigenVect;
//   delete DiagPl;
//cout<<" End of ReduceLQ\n";
   return ret;
  }; 
void NormMatr(CMatrCl &EigenVect,int *Use)
  {
   CVecCl Extract,ConjExtract,Work;
   for (int k=1;k<=Use[0];k++)
     {
      Extract= CMatrCl::GetCol(EigenVect,Use[k]);
      ConjExtract=Conj(Extract);
      for (int k1=k+1;k1<=Use[0];k1++)
        {
         Work=CMatrCl::GetCol(EigenVect,Use[k1]);
         Work=Work-Extract*(ConjExtract*Work);
         CMatrCl::SetCol(EigenVect,Use[k1],Work);
        }
     }
   for (int k=2;k<=Use[0];k++) 
     { 
      Work=CMatrCl::GetCol(EigenVect,Use[k]);
      Work=Work*my_comp(1/sqrt(Mod2(Work)),0);
      CMatrCl::SetCol(EigenVect,Use[k],Work);
     } 
  };   

int CReduceLQ(CMatrCl &InMat,CMatrCl &EigenVect,double *EigenVal,double Tol)
  {
   int N=InMat.Dim(),cur=0;
   MatrCl RInMat=ErmitMReal(InMat);
if (TestData(RInMat,1,Tol)) { cout<<" Matrics not simmetric\n"<<
        (RInMat-Transpon(RInMat));};
   MatrCl REigenM(N*2);
   double *REigenV=new double[2*N+1];REigenV[0]=2*N;
//   CVecCl Cmp(N);
   int *Use=new int[2*N+1];
   for (int k=1;k<=2*N;k++) Use[k]=1;

   int ret=ReduceLQ(RInMat,REigenM,REigenV,Tol*0.01);
//cout<<" Reduce ";
   Use[0]=0;
   for (int k=1;k<=2*N;k+=2)
     {
       EigenVal[++cur]=REigenV[k];
       if (fabs(REigenV[k+1]-REigenV[k])>Tol) 
         { cout<<" New vertion does not work. Maybe try the older one.";
           ret=0;
         } 
        if (fabs( fabs(REigenM(1,k))-fabs(REigenM(N+1,k+1)) )>Tol)
           Use[++Use[0]]=cur;
//    Normalize EigenVectors
       for (int k1=1;k1<=N;k1++) 
         EigenVect(k1,cur)=my_comp(REigenM(k1,k),REigenM(k1+N,k));
       if (!ret) 
           {cout<<" Error in ReduceLQ. Double root not found for"<<
                         REigenV[k]<<"   "<<REigenV[k+1]<<"  "<<k<<"\n";
           }
     }
   if (Use[0]) 
     {
//   Norm Matrics       
      NormMatr(EigenVect,Use);
     }
   if (!ret) cout<<" Ermit Matrics\n"<<RInMat<<" EigenV\n"<<REigenM;
   delete []Use;delete []REigenV;
   return ret;
  }
