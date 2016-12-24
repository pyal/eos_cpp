#ifndef __CMATRICS_H
#define __CMATRICS_H

#include <math.h>
#include "lib/std/my_comp.h"
//#include <complex.h>
/*
#include <stdio.h>
#include <fstream.h>
#include <strstream.h>
#include <const.h>
#include <stdlib.h>
#include <mem.h>
*/
#include "matrics.h"
//#include "lib/std/util.h"

struct CMatrCl;
struct CVecCl
  {
   my_comp *Ptr;

   my_comp &operator[](int k){ return Ptr[k];};
   my_comp operator[](int k)const{ return Ptr[k];};
   const CVecCl& operator=(const CVecCl& vec)
     {SetDim(vec.Dim());
      MemoryMove(vec.Ptr,Ptr,sizeof(my_comp)*(vec.Dim()+1));return (*this);};

   CVecCl& operator=(const CMatrCl &mat);


   friend CVecCl operator*(const CVecCl &vec,const  my_comp coef);
   friend CVecCl operator*(const CVecCl &vec,double coef);
   friend CVecCl operator/(const CVecCl &vec,const  my_comp coef);
   friend CVecCl operator+(const CVecCl &vec,const my_comp cons);
   friend my_comp operator*(const CVecCl &vec,const CVecCl &vec1);
   friend CVecCl operator+(const CVecCl &vec,const CVecCl &vec1);
   friend CVecCl operator-(const CVecCl &vec,const CVecCl &vec1);
   friend ostream& operator<<(ostream &output,const CVecCl &vec);
   friend istream& operator>>(istream &input,const CVecCl &vec);
//   (my_comp *) operator (my_comp *)(){return Ptr;};

   int  Dim(int NewDim=-1){if (NewDim>-1) SetDim(NewDim);
                           if (Ptr==NULL) return 0;
                           return (int)my_real(Ptr[0]);};
   int Dim()const{return (int)my_real(Ptr[0]);};
   CVecCl(int NewDim){ Ptr=NULL;SetDim(NewDim);};
   CVecCl() {Ptr=NULL;}
   CVecCl(const CVecCl& vec)
     { Ptr=NULL;*this=vec;};
   ~CVecCl()
    {
    if (Ptr!=NULL) delete Ptr;
    Ptr=NULL;};
   void SetDim(int NewDim)
     {if ((Ptr==NULL) || (NewDim!=my_real(Ptr[0])))
       {
        delete Ptr;
        Ptr=new my_comp[NewDim+1];Ptr[0]=my_comp(NewDim,0);
       }
     };
   static double ModMax(CVecCl &Vec,double NewMax);// ModMax - norm( Max(Vec[k]) )
   static double ModMax(const CVecCl &Vec);// ModMax - norm( Max(Vec[k]) )
   friend double Mod2(const CVecCl &Vec);
   friend CVecCl Conj(const CVecCl &Vec);
  };

struct CMatrCl
  {
   int DimX,DimY;
   my_comp **Addr;
   my_comp& operator()(int i,int j);
   my_comp operator()(int i,int j)const;
   CMatrCl& operator=(const CMatrCl& mat);
   CMatrCl& operator=(const CVecCl& vec);
   friend CMatrCl operator*(const CMatrCl &mat1,const CMatrCl &mat2);
   friend CMatrCl operator*(const CMatrCl &mat1,double Const);
   friend CMatrCl operator*(const CMatrCl &mat1,const my_comp Const);
   friend CVecCl operator*(const CMatrCl &mat1,const CVecCl &vec);
   friend CVecCl operator*(const CVecCl &vec,const CMatrCl &mat1);

//  Add unit Matr
   friend CMatrCl operator+(const CMatrCl &mat1, double Const);
   friend CMatrCl operator+(const CMatrCl &mat1,const my_comp Const);
   friend CMatrCl operator-(const CMatrCl &mat1,const my_comp Const);
   friend CMatrCl operator-(const CMatrCl &mat1,const CMatrCl &mat2);
   friend CMatrCl operator+(const CMatrCl &mat1,const CMatrCl &mat2);
   friend ostream& operator<<(ostream &output,const CMatrCl &mat);
   friend istream& operator>>(istream &input,CMatrCl &mat1);

   int Dim(int NewDim) {SetMatr(NewDim);return DimX;};
   int Dim() const {return DimX;};
   void DimMN(int &X,int &Y)const{ X=DimX;Y=DimY;};

   ~CMatrCl()            { DeleteMatr(); };
   CMatrCl()             { Addr=NULL;DimX=0; };
   CMatrCl(int Dim)      { Addr=NULL;SetMatr(Dim,Dim); };
   CMatrCl(int X,int Y)  { Addr=NULL;SetMatr(X,Y); };
   CMatrCl(const CMatrCl &matr) { Addr=NULL;*this=matr; };
   void SetMatr(int DX) { SetMatr(DX,DX);}
   void SetMatr(int DX,int DY,my_comp **matr=NULL)
     {
      if ( (matr==NULL) && (Addr!=NULL) && (DimX==DX) && (DimY==DY) ) return;
      DeleteMatr();
      if (matr==NULL) NewMatr(DX,DY);
      else { Addr=matr;DimX=DX;DimY=DY;cout<<" SetMatr?\n";}
     };
   void NewMatr(int DX,int DY);
   void DeleteMatr();
   static CVecCl GetRow(const CMatrCl &Mat,int i);
   static CVecCl GetCol(const CMatrCl &Mat,int i);
   static void  SetRow (CMatrCl &Mat,int i,CVecCl &row);
   static void  SetCol (CMatrCl &Mat,int i,CVecCl &col);

  };

CMatrCl Transpon(const CMatrCl &Mat);
CMatrCl Conj(const CMatrCl &Mat);
int Inverse(CMatrCl &Mat);
MatrCl ErmitMReal(const CMatrCl &Mat);
CVecCl RealVComp(const VecCl &In);
my_comp Trace(const CMatrCl &Mat);
CMatrCl DirectMultiplyOfMat(const CMatrCl &Fst,const CMatrCl &Sec);


// return max<double>(fabs(my_imag(Mat(k,k1)))+fabs(my_real(Mat(k,k1))))
double MatrNorm(const CMatrCl &Mat);


#endif

