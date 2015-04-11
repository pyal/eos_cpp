#include <math.h>
//#include <complex.h>
#include "my_comp.h"
#include "matrics.h"
#include "cmatrics.h"
#include <malloc.h>
#include <conio.h>
#include <iostream.h>
#include <string.h>
//#include "fobj.h"
#include "util.h"
#include <stdlib.h>
//#include "eigen_ma.h"
//#define Dimen 8
#include "uralgol.h"

void main()
  {
   int Dimen;
cin>>Dimen;
//   MatrCl B(Dimen);

   CMatrCl A(Dimen);
   CVecCl Solve(Dimen),Solution(Dimen);
   cout<<coreleft()<<"\n";
   randomize();
   int i;
/*   for (int i=1;i<=Dimen;i++)
     {
      for (int j=i;j<=Dimen;j++)
        {(A)(i,j)=(double)rand()/RAND_MAX*10;(A)(j,i)=(A)(i,j);}
     }
*/
   for (i=1;i<=Dimen;i++)
     {  
      for (int j=1;j<=Dimen;j++)  
        {
         A(i,j)=my_comp( (double)rand()/RAND_MAX*10,
                         (double)rand()/RAND_MAX*10 );
        }
      Solve[i]=my_comp( (double)rand()/RAND_MAX*10,
                        (double)rand()/RAND_MAX*10 );
     }   
//   cout<<"Matr A\n"<<(*A);ChRead();
//   B=A;
//   cout<<"Error= "<<Inverse(B)<<"\n";
//   cout<<" A= \n"<<A<<"inv\n"<<B<<"ret\n"<<A*B;
//   cout<<" Vector to find "<<B*GetCol(Solve,1);
//   ChRead();
cout<<"Solve \n";
   CSolveAxb(A,Solve,Solution,1e-15);


   cout<<coreleft()<<"\n";

  };
