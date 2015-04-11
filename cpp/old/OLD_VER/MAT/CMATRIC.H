#ifndef __CMATRICS_H
#define __CMATRICS_H

#include <math.h>
//#include <complex.h>
#include "my_comp.h"
#include <stdio.h>
#include <fstream.h>
#include <strstream.h>
#include <const.h>
#include <stdlib.h>
#include <mem.h>
#include "matrics.h"
#include "util.h"

struct CVecCl
  {
   my_comp *Ptr;

   my_comp &operator[](int k){ return Ptr[k];};
   CVecCl& operator=(CVecCl& vec)
     {SetDim(real(vec[0]));movmem(vec.Ptr,Ptr,sizeof(my_comp)*(real(vec[0])+1));return (*this);};

   friend CVecCl operator*(CVecCl &vec,my_comp coef);
   friend CVecCl operator/(CVecCl &vec,my_comp coef);
   friend CVecCl operator+(CVecCl &vec,my_comp cons);
   friend my_comp operator*(CVecCl &vec,CVecCl &vec1);
   friend CVecCl operator+(CVecCl &vec,CVecCl &vec1);
   friend CVecCl operator-(CVecCl &vec,CVecCl &vec1);
   friend ostream& operator<<(ostream &output,CVecCl &vec);
   friend istream& operator>>(istream &input,CVecCl &vec);
//   (my_comp *) operator (my_comp *)(){return Ptr;};

   int  Dim(int NewDim=-1){if (NewDim>-1) SetDim(NewDim);
                           if (Ptr==NULL) return 0;
                           return real(Ptr[0]);};
   CVecCl(int NewDim){ Ptr=NULL;SetDim(NewDim);};
   CVecCl() {Ptr=NULL;}
   CVecCl(CVecCl& vec)
     { Ptr=NULL;SetDim(real(vec[0]));movmem(vec.Ptr,Ptr,sizeof(my_comp)*(real(vec[0])+1));};
   ~CVecCl(){if (Ptr!=NULL) delete Ptr;};
   void SetDim(int NewDim)
     {if ((Ptr==NULL) || (NewDim!=real(Ptr[0])))
       {
        if (Ptr!=NULL) delete Ptr;
        Ptr=new my_comp[NewDim+1];Ptr[0]=my_comp(NewDim,0);
       }
     };
   static double ModMax(CVecCl &Vec,double NewMax);// ModMax - norm( Max(Vec[k]) )
   static double ModMax(const CVecCl &Vec);// ModMax - norm( Max(Vec[k]) )
   friend double Mod2(CVecCl &Vec);
   friend CVecCl Conj(CVecCl &Vec);
  };

struct CMatrCl
  {
   int Dimenshion;
   my_comp *Addr;
   my_comp& operator()(int i,int j);
   CMatrCl& operator=(CMatrCl& mat);
   CMatrCl& operator=(CVecCl& vec);
   friend CMatrCl operator*(CMatrCl &mat1,CMatrCl &mat2);
   friend CMatrCl operator*(CMatrCl &mat1,my_comp Const);
   friend CVecCl operator*(CMatrCl &mat1,CVecCl &vec);
   friend CVecCl operator*(CVecCl &vec,CMatrCl &mat1);

//  Add unit Matr
   friend CMatrCl operator+(CMatrCl &mat1,my_comp Const);
   friend CMatrCl operator-(CMatrCl &mat1,my_comp Const);
   friend CMatrCl operator-(CMatrCl &mat1,CMatrCl &mat2);
   friend CMatrCl operator+(CMatrCl &mat1,CMatrCl &mat2);
   friend ostream& operator<<(ostream &output,CMatrCl &mat);
   friend istream& operator>>(istream &input,CMatrCl &mat1);

//   friend my_comp* MatMulVec(CMatrCl &mat1,my_comp *vec);
//   friend my_comp* VecMulMat(my_comp *vec,CMatrCl &mat1);

//   my_comp *operator*(const my_comp *Vector);

   friend CVecCl GetRow(CMatrCl &Mat,int i);
   friend CVecCl GetCol(CMatrCl &Mat,int i);
   friend void  SetRow (CMatrCl &Mat,int i,CVecCl &row);
   friend void  SetCol (CMatrCl &Mat,int i,CVecCl &col);
   friend my_comp Trace(CMatrCl &Mat);
   friend CMatrCl Transpon(CMatrCl &Mat);
   friend CMatrCl Conj(CMatrCl &Mat);
   int Dim(int NewDim=-1){if (NewDim>=0) SetMatr(NewDim);return Dimenshion;};

   ~CMatrCl()   { if (Addr!=NULL)  delete Addr;};
   CMatrCl(){Addr=NULL;Dimenshion=0;};
   CMatrCl(int Dim)
     {Dimenshion=Dim;
      Addr=new my_comp [Dimenshion*Dimenshion];
     };
   CMatrCl(CMatrCl &matr)
     { Dimenshion=0;Addr=NULL;*this=matr; };
   void SetMatr(int Dim,my_comp *matr=NULL)
     {if (Addr!=NULL) delete Addr;
      if (matr==NULL) Addr=new my_comp[Dim*Dim];
      else Addr=matr;
      Dimenshion=Dim;};
  };
int Inverse(CMatrCl &Mat);
MatrCl ErmitMReal(CMatrCl &Mat);
CVecCl RealVComp(VecCl &In);


#endif

