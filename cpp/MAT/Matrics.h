#ifndef __MATRICS_H
#define __MATRICS_H
/*
#include <stdio.h>
#include <fstream.h>
#include <strstream.h>
#include <math.h>
#include <const.h>
#include <stdlib.h>
#include <mem.h>
*/
#include "lib/std/util.h"
#include "lib/std/stdexception.h"
#include <vector>
#include <list>
#include <float.h>

#include "lib/ref/stroka.h"


struct MatrCl;

struct VecCl
  {
   double *Ptr;

   double &operator[](int k){ 
#ifdef _DEBUG
	   if (k>Dim() || k<1)
		   throw info_except("Trying to get element %i for array with dim %i\n",k,Dim());
#endif
	   return Ptr[k];
   };
   double operator[](int k)const{ 
#ifdef _DEBUG
	   if (k>Dim() || k<1)
		   throw info_except("Trying to get element %i for array with dim %i\n",k,Dim());
#endif
	   return Ptr[k];
   };
   //double &operator[](size_t k){ return Ptr[k];};
   //double operator[](size_t k)const{ return Ptr[k];};
   VecCl& operator=(const VecCl& vec) {
       SetDim(vec.Dim());
       if (Ptr) 
           MemoryMove(vec.Ptr,Ptr,sizeof(double)*(vec.Dim()+1));
       return (*this);
   };  
   VecCl& operator=(double *vec) {
       SetDim((int)vec[0]);
       MemoryMove(vec,Ptr,sizeof(double)*((int)vec[0]+1));
       return (*this);
   };  
   VecCl& operator=(const vector<double> &vec) {
       if (vec.size() == 0)
           Ptr = NULL;
       else {
           SetDim((int)vec.size());
           MemoryMove(&vec[0], &Ptr[1],sizeof(double)*vec.size());
       }
       return (*this);  
   }
   void Copy2Double(double *dst)
   {
     if (dst[0]!=Ptr[0]) {cout<<" Incompatible type in VecCl::Copy2Double\n";return;}
     MemoryMove(Ptr,dst,sizeof(double)*((int)Ptr[0]+1));
   }
   void Copy2Vector(vector<double> &vec){
       vec.resize(Dim());
       MemoryMove(&Ptr[1], &vec[0],sizeof(double)*Dim());
   }
   vector<double> Copy2Vector(){
       vector<double> vec(Dim());
       MemoryMove(&Ptr[1], &vec[0],sizeof(double)*Dim());
       return vec;
   }
   VecCl ExtractSubVector(int i,int j){VecCl ret(j-i+1);MemoryMove(&Ptr[i],&ret.Ptr[1],sizeof(double)*(j-i+1));return ret;}
   VecCl& operator=(const MatrCl &mat);

   friend VecCl operator*(const VecCl &vec,double coef);
   friend VecCl operator/(const VecCl &vec,double coef);
   friend VecCl operator+(const VecCl &vec,double cons);
   friend VecCl operator-(const VecCl &vec,double cons);
   friend double operator*(const VecCl &vec,const VecCl &vec1);
   friend VecCl operator+(const VecCl &vec,const VecCl &vec1);
   friend VecCl operator-(const VecCl &vec,const VecCl &vec1);
   friend ostream& operator<<(ostream &output,const VecCl &vec);
   friend istream& operator>>(istream &input,VecCl &vec);
//   (double *) operator (double *)(){return Ptr;};

   int  Dim()const{if (Ptr) return (int)Ptr[0];return 0;};
   int  Dim(int NewDim=-1){if (NewDim>-1) SetDim(NewDim);
                           if (Ptr==NULL) return 0;
                           return Ptr[0];};
   VecCl(int NewDim) : Ptr(NULL) { SetDim(NewDim);};
   VecCl() {Ptr=NULL;}
   VecCl(double *Addr){Ptr=Addr;};
   VecCl(double *Addr, int len) : Ptr(NULL) {
        SetDim(len);
        MemoryMove(Addr,&Ptr[1],sizeof(double)*len);
   };
   VecCl(const VecCl& vec) : Ptr(NULL) { 
       if (vec.Ptr == NULL) 
           return;
       SetDim(vec.Dim());
       MemoryMove(vec.Ptr,Ptr,sizeof(double)*(vec.Dim()+1));
   };
   VecCl(const vector<double> &vec) : Ptr(NULL) {
        *this = vec;
   }
   ~VecCl() {
       if (Ptr!=NULL) 
           delete Ptr;
       Ptr=NULL;
   };
   void SetDim(int NewDim)
     {//double *pnt=Vec.Ptr;
      if ((Ptr==NULL) || (NewDim!=Dim()))
       {
        if (Ptr!=NULL) delete Ptr;
		if (NewDim<0)
			throw info_except("Cannot create vector with negative dimensions: %i\n",NewDim);
		if (NewDim){
            Ptr=new double[NewDim+1];Ptr[0]=NewDim;
        } else
            Ptr = NULL;
       }
     };
// Depricated ?
   static double ModMax(const VecCl &Vec);
   static double ModMax(VecCl &Vec, double NewMax);
                                   // ModMax - fabs( Max(Vec[k]) )
// This are OK
   
   static VecCl MakeStepVector(int Num,double low,double up,int centered);
   static VecCl ComponentMul(const VecCl &fst,const VecCl &sec){
	   if (fst.Dim()!=sec.Dim())
		   throw info_except("Dims are different fst.Dim=%i sec.Dim=%i\n",fst.Dim(),sec.Dim());
	   VecCl res = fst;
	   for(int k = 1; k <= fst.Dim(); k++)
			res[k] *= sec[k];
	   return res;
   }
   static VecCl ComponentDiv(const VecCl &fst,const VecCl &sec){
	   if (fst.Dim()!=sec.Dim())
		   throw info_except("Dims are different fst.Dim=%i sec.Dim=%i\n",fst.Dim(),sec.Dim());
	   VecCl res = fst;
	   for(int k=1;k<=fst.Dim();k++)
			res[k] /= sec[k];
	   return res;
   }
   static double Max(const VecCl &Vec){
       double ret = -1/MathZer;
       for(int k=1;k<=Vec.Dim();k++)
           if (ret<Vec[k])
               ret = Vec[k];
       return ret;
   }
   static double Min(const VecCl &Vec){
       double ret = 1/MathZer;
       for(int k=1;k<=Vec.Dim();k++)
           if (ret>Vec[k])
               ret = Vec[k];
       return ret;
   }
   static VecCl fabs(const VecCl &Vec);
   struct SortCl{
	   double Val;
	   int Ind;
	   SortCl(int ind, double val,int AscendingOrder):Val(val),Ind(ind){
		   if (AscendingOrder)
			   Val = -Val;
	   };
		bool operator>(const	SortCl& _Right) const{	
			return (Val >	_Right.Val);
		}
   };
   static vector<int> Sort(const VecCl &Vec, int AscendingOrder = 1){
	   list<SortCl> lst;
	   for(int k=1;k<=Vec.Dim();k++)
		   lst.push_back(SortCl(k,Vec[k],AscendingOrder));
	   lst.sort(greater<SortCl>());
	   vector<int> ind;
	   for(list<SortCl>::iterator iter = lst.begin();iter!=lst.end();iter++)
		   ind.push_back(iter->Ind);
	   return ind;
   }
   int CheckInfinity(double ReplaceMaxVal){
       int ret = 0;
	   for(int k=1;k<=this->Dim();k++)
//           if (!_finite(Ptr[k])){
           if (!finite(Ptr[k])){
               Ptr[k]=Ptr[k]>0?ReplaceMaxVal:-ReplaceMaxVal;
               ret = 1;
           }
       return ret;
   }
   Stroka ToString() {
        const int BufSize = 100000;
        char buf[BufSize+1];
        std::strstream out(buf,BufSize);
        out<<*this;
        return Stroka(buf,0,out.pcount());
   }
   double SumElements() {
       double ret = 0;
       for(int k = 1; k <= Dim(); k++)
           ret += Ptr[k];
       return ret;
   }
   

  };
struct MatrCl
  {
   int DimX,DimY;
   double **Addr;
// i - Row, j - Col
   double& operator()(int i,int j);
   MatrCl& operator=(const MatrCl& mat);
   MatrCl& operator=(const VecCl& vec);
   friend MatrCl operator*(MatrCl &mat1,MatrCl &mat2);
   friend MatrCl operator*(MatrCl &mat1,const double Const);
   friend VecCl operator*(MatrCl &mat1,VecCl &vec);
   friend VecCl operator*(VecCl &vec,MatrCl &mat1);

//  Add unit Matr
   friend MatrCl operator+(MatrCl &mat1,const double Const);
   friend MatrCl operator-(MatrCl &mat1,double Const);
   friend MatrCl operator+(MatrCl &mat1,MatrCl &mat2);
   friend MatrCl operator-(MatrCl &mat1,MatrCl &mat2);
   friend ostream& operator<<(ostream &output,const MatrCl &mat);
   friend istream& operator>>(istream &input, MatrCl &mat1);

   int Dim(int NewDim=-1){if (NewDim>=0) SetMatr(NewDim);return DimX;};
   void GetDimMN(int &X,int &Y)const { X=DimX;Y=DimY;};

   ~MatrCl()            { DeleteMatr(); };
   MatrCl()             { Addr=NULL;DimX=0;DimY=0; };
   MatrCl(int Dim)      { Addr=NULL;SetMatr(Dim,Dim); };
   MatrCl(int X,int Y)  { Addr=NULL;SetMatr(X,Y); };
   MatrCl(const MatrCl &matr) { Addr=NULL;*this=matr; };
   void SetMatr(int DX) { SetMatr(DX,DX);}
   void SetMatr(int DX,int DY,double **matr=NULL)
     {
      if ( (matr==NULL) && (Addr!=NULL) && (DimX==DX) && (DimY==DY) ) return;
      DeleteMatr();
      if (matr==NULL) NewMatr(DX,DY);
      else { Addr=matr;DimX=DX;DimY=DY;cout<<" SetMatr?\n";}
     };
   void NewMatr(int DX,int DY);
   void DeleteMatr();
   double Get(int i,int j) const {return Addr[i-1][j-1];};
   static VecCl GetRow(MatrCl &Mat,int i);
   static VecCl GetCol(MatrCl &Mat,int i);
   static void  SetRow(MatrCl &Mat,int i,VecCl &row);
   static void  SetCol(MatrCl &Mat,int i,VecCl &col);

  };
MatrCl Transpon(MatrCl &Mat);
int Inverse(MatrCl &Mat);
double Trace(MatrCl &Mat);
MatrCl DirectMultiplyOfMat(MatrCl &Fst,MatrCl &Sec);
double MatrNorm(MatrCl &Mat);



//MatrCl VectorNormalizeMatr(VecCl Lft,MatrCl &Mat,VecCl Rgt);
MatrCl VectorNormalizeMatr(const VecCl &Lft,const MatrCl &Mat,const VecCl Rgt);
                                                 
#endif




