#include "matrics.h"
#include <malloc.h>
#include <conio.h>
#include <iostream.h>
#include <string.h>
//#include "fobj.h"
#include "util.h"
#include <stdlib.h>
#include "eigen_ma.h"
#include "uralgol.h"

void main(int argc,char *argv[])
  {
   double MatDim=5;
   cout<<coreleft()<<"\n";
//   double Eigenvalues[MatDim+1];Eigenvalues[0]=MatDim;
//   MatrCl A(MatDim),Eigenvectors(MatDim),B(MatDim),C(MatDim);
   cout<<coreleft()<<"\n";
   int Iter;
   randomize();
//   A(1,1)=1;A(1,2)=0.99;A(1,3)=0.868;A(2,2)=1;A(2,3)=0.7912;A(3,3)=1;
//A(1,1)=1;A(1,2)=-1;A(1,3)=-1;A(1,4)= 2;
//A(2,1)=2;A(2,2)= 3;A(2,3)= 0;A(2,4)=-4;
//A(3,1)=1;A(3,2)= 1;A(3,3)=-2;A(3,4)=-2;
//A(4,1)=1;A(4,2)= 1;A(4,3)= 0;A(4,4)=-1;
/*
   for (int i=1;i<=MatDim;i++)
     {
      for (int j=i;j<=MatDim;j++)
        {A(i,j)=(double)rand()/RAND_MAX*10;
          A(j,i)=A(i,j);}
     }
*/
fstream in(argv[1],ios::in);
in>>MatDim;
   CMatrCl A(MatDim),B(MatDim),C(MatDim);
in>>A;  cout<<A<<"\n\n";
URComeig(A,B,C);
cout<<A<<"\n"<<B<<"\n"<<B*A*C<<"\n"<<B*C;
     B=A;
//   Inverse(B);
//   cout<<"Inverted \n"<<B<<"Result Mult \n"<<A*B;ChRead();B=A;
//   cout<<"Error "<<Jacobi(A,1000,1e-6,&Eigenvalues[0],Eigenvectors,Iter);
//   cout<<" Iter "<<Iter<<"\n";
//   cout<<"Error "<<ReduceLQ(A,Eigenvectors,&Eigenvalues[0],1e-6);
//   cout<<" Mem "<<coreleft()<<"\n";
in.close();
//   cout<<" Mat Beg \n"<<B;
/*
//cout<<" Eigen before Transpon \n"<<Eigenvectors;
Transpon(Eigenvectors);
C=Eigenvectors;Inverse(C);
//   cout<<" BegMat \n"<<Eigenvectors*A*C;
   cout<<" EigMat \n"<<Eigenvectors;
cout<<" Values\n";
   for (int k=1;k<=MatDim;k++) cout<<Eigenvalues[k]<<"   ";
   cout<<"\n";
   cout<<coreleft()<<"\n";

   cout<<"Error "<<Jacobi(A,1000,1e-6,&Eigenvalues[0],Eigenvectors,Iter);
   cout<<" Iter "<<Iter<<"\n";
//   cout<<" Mat Beg \n"<<B;
//cout<<" Eigen before Transpon \n"<<Eigenvectors;
Transpon(Eigenvectors);
C=Eigenvectors;Inverse(C);
//   cout<<" BegMat \n"<<Eigenvectors*A*C;
   cout<<" EigMat \n"<<Eigenvectors;
//cout<<" Eigen \n"<<Eigenvectors;ChRead();
//   cout<<"Mul\n"<<B*Eigenvectors.GetCol(1);
//   cout<<"Mul\n"<<B*Eigenvectors;
cout<<" Values\n";
   for (k=1;k<=MatDim;k++) cout<<Eigenvalues[k]<<"   ";
   cout<<"\n";
   cout<<coreleft()<<"\n";
//   delete A;
//   delete Eigenvectors;
   cout<<coreleft()<<"\n";
*/
  };
