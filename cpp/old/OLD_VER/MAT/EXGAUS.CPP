#include "matrics.h"
#include "gaussolv.h"
#include <malloc.h>
#include <conio.h>
#include <iostream.h>
#include <string.h>
#include "fobj.h"
#include "util.h"

#define MatS 3

 void main()
  {
   VecCl constant(MatS),results(MatS),answer(MatS);
   int i,j;

   cout<<coreleft()<<"\n";
   MatrCl (*A)=new MatrCl(MatS);

   randomize();
   for (i=1;i<=MatS;i++)
     { for (j=1;j<=MatS;j++) (*A)(i,j)=i*j-3*i*i+10+0.1*sqrt(j);
       constant[i]=rand()-RAND_MAX/2;
     }
   cout<<" Before new Sol mem: "<<coreleft()<<"\n";
   GauseSolve *Sol=new GauseSolve;

   cout<<(*A)<<"\n"<<"To get vector:"<<constant;        
   ChRead();
   if ( (i=Sol->NotLUSolve(*A,constant,results)) ) cout<<" ERROR="<<i<<"  !!!!!!!\n";
   cout<<" Got answer :"<<results;
   
   cout<<" Then vector will be :"<<(*A)*results;

   cout<<coreleft()<<"\n";
   delete Sol;
   cout<<" After del Sol mem: "<<coreleft()<<"\n";
   delete A;

   cout<<coreleft()<<"\n";

  };

/*
void main()
  {
   cout<<coreleft()<<"\n";
   int i;
   MatrCl A;
   double tmp[4][4]={{1,2,3,4},{-1,2,4,-3},{2,4,5,-2},{4,3,2,1}};
   A.SetMatr(4,(double*)&tmp);
   GauseSolve Sol;
//   for (int i=1;i<=4;i++)
//     {for (int j=1;j<=4;j++) (A)(i,j)=100-((i-1)*4+j);}
   double constant[]={4,1,2,3,4};
   double *results;

   cout<<(A)<<"\n"<<"To get vector:";
ChRead();
   for (i=1;i<=4;i++) { cout<<constant[i]<<"  ";}; cout<<"\n";
   if (Sol.NotLUSolve(A,constant,results   )) cout<<" ERROR  !!!!!!!";
//   cout<<(A)<<"\n";ChRead();
   cout<<" Got answer :";
   for (i=1;i<=4;i++) { cout<<results[i]<<"  ";};cout<<"\n";
   double *answer=MatMulVec(A,results);
   cout<<" Then vector will be :";
   for (i=1;i<=4;i++) { cout<<answer[i]<<"  ";};cout<<"\n";
   delete answer;delete results;
   cout<<coreleft()<<"\n";

   cout<<coreleft()<<"\n";

  };
  
     {
      cout<<" A  \n"<<*A<<"\n";
      VecCl tmp=(*A)*constant;
      cout<<" tmp \n"<<tmp<<"\n";
      MatrCl tm=(*A)*1;
      cout<<" tm \n"<<(*A)*1<<"\n";
     }
  
  */
