#ifndef __MATRICS_H
#define __MATRICS_H

#include <stdio.h>
#include <fstream.h>
#include <strstream.h>
#include <math.h>
#include <const.h>
#include <stdlib.h>
#include <mem.h>

struct VecCl
  {
   double *Ptr;

   double &operator[](int k){ return Ptr[k];};
   VecCl& operator=(VecCl& vec)
     {SetDim(vec[0]);movmem(vec.Ptr,Ptr,sizeof(double)*(vec[0]+1));return (*this);};  

   friend VecCl operator*(VecCl &vec,double coef);
   friend VecCl operator/(VecCl &vec,double coef);
   friend VecCl operator+(VecCl &vec,double cons);
   friend double operator*(VecCl &vec,VecCl &vec1);
   friend VecCl operator+(VecCl &vec,VecCl &vec1);
   friend VecCl operator-(VecCl &vec,VecCl &vec1);
   friend ostream& operator<<(ostream &output,VecCl &vec);
   friend istream& operator>>(istream &input,VecCl &vec);
//   (double *) operator (double *)(){return Ptr;};

   int  Dim(int NewDim=-1){if (NewDim>-1) SetDim(NewDim);
                           if (Ptr==NULL) return 0;
                           return Ptr[0];};
   VecCl(int NewDim){ Ptr=NULL;SetDim(NewDim);};
   VecCl() {Ptr=NULL;}
   VecCl(double *Addr){Ptr=Addr;};
   VecCl(VecCl& vec)
     { Ptr=NULL;SetDim(vec[0]);movmem(vec.Ptr,Ptr,sizeof(double)*(vec[0]+1));};
   ~VecCl(){if (Ptr!=NULL) delete Ptr;};
   void SetDim(int NewDim)
     {//double *pnt=Vec.Ptr;
      if ((Ptr==NULL) || (NewDim!=Ptr[0]))
       {
        if (Ptr!=NULL) delete Ptr;
        Ptr=new double[NewDim+1];Ptr[0]=NewDim;
       }
     };
   friend double ModMax(VecCl &Vec,double NewMax=0);
                                   // ModMax - fabs( Max(Vec[k]) )
  };
struct MatrCl
  {
   int DimX,DimY;
   double **Addr;
   double& operator()(int i,int j);
   MatrCl& operator=(MatrCl& mat);
   MatrCl& operator=(VecCl& vec);
   friend MatrCl operator*(MatrCl &mat1,MatrCl &mat2);
   friend MatrCl operator*(MatrCl &mat1,const double Const);
   friend VecCl operator*(MatrCl &mat1,VecCl &vec);
   friend VecCl operator*(VecCl &vec,MatrCl &mat1);

//  Add unit Matr
   friend MatrCl operator+(MatrCl &mat1,const double Const);
   friend MatrCl operator-(MatrCl &mat1,double Const);
   friend MatrCl operator+(MatrCl &mat1,MatrCl &mat2);
   friend MatrCl operator-(MatrCl &mat1,MatrCl &mat2);
   friend ostream& operator<<(ostream &output,MatrCl &mat);
   friend istream& operator>>(istream &input,MatrCl &mat1);

//   friend double* MatMulVec(MatrCl &mat1,double *vec);
//   friend double* VecMulMat(double *vec,MatrCl &mat1);

//   double *operator*(const double *Vector);

   friend VecCl GetRow(MatrCl &Mat,int i);
   friend VecCl GetCol(MatrCl &Mat,int i);
   friend void  SetRow(MatrCl &Mat,int i,VecCl &row);
   friend void  SetCol(MatrCl &Mat,int i,VecCl &col);
   friend double Trace(MatrCl &Mat);

   int Dim(int NewDim=-1){if (NewDim>=0) SetMatr(NewDim);return DimX;};
   void DimMN(int &X,int &Y){ X=DimX;Y=DimY;};

   ~MatrCl()            { DeleteMatr(); };
   MatrCl()             { Addr=NULL;DimX=0; };
   MatrCl(int Dim)      { Addr=NULL;SetMatr(Dim,Dim); };
   MatrCl(int X,int Y)  { Addr=NULL;SetMatr(X,Y); };
   MatrCl(MatrCl &matr) { *this=matr; };
   void SetMatr(int DX) { SetMatr(DX,DX);}
   void SetMatr(int DX,int DY,double **matr=NULL)
     {
      if ( (matr==NULL) && (Addr!=NULL) && (DimX==DX) && (DimY==DY) ) return;
      DeleteMatr();
      if (matr==NULL) NewMatr(DX,DY);
      else { Addr=matr;DimX=DX;DimY=DY;}
     };
   void NewMatr(int DX,int DY);
   void DeleteMatr();
  };
MatrCl Transpon(MatrCl &Mat);
int Inverse(MatrCl &Mat);
#endif




