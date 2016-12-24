#include <lib/precompiled/math.h>


#include "matrics.h"

VecCl operator*(const VecCl &vec,double coef)
  { VecCl ret=vec;
    if (fabs(coef)>MathZer){for (int k=1;k<=ret.Dim();k++) ret.Ptr[k]*=coef;}
    else  { for (int k=1;k<=ret.Dim();k++) ret.Ptr[k]=0;}
    return ret; };
VecCl operator/(const VecCl &vec,double coef)
  { VecCl ret=vec;
    if (fabs(coef)>MathZer){for (int k=1;k<=ret.Dim();k++) ret.Ptr[k]/=coef;}
    else  { for (int k=1;k<=ret.Dim();k++) ret.Ptr[k]*=MathInf;
	    cout<<"Error......... Zero Division in VecCl\n";}
    return ret; };
VecCl operator+(const VecCl &vec,double cons){ 
	VecCl ret=vec;for (int k=1;k<=ret.Dim();k++) ret.Ptr[k]+=cons;
    return ret;};
VecCl operator-(const VecCl &vec,double cons){ 
	VecCl ret=vec;for (int k=1;k<=ret.Dim();k++) ret.Ptr[k]-=cons;
    return ret;};
double operator*(const VecCl &vec,const VecCl &vec1){ 
	if (vec.Dim()!=vec1.Dim())
		throw info_except("Dims of multiplying vectors are different d1=%i d2=%i\n",vec.Dim(),vec1.Dim());
	double s=0;for (int k=1;k<=vec.Dim();k++) s+=vec.Ptr[k]*vec1.Ptr[k];
    return s; };
VecCl operator+(const VecCl &vec,const VecCl &vec1) { 
	if (vec.Dim()!=vec1.Dim())
		throw info_except("Dims of adding vectors are different d1=%i d2=%i\n",vec.Dim(),vec1.Dim());
	VecCl ret=vec;for (int k=1;k<=ret.Dim();k++) ret.Ptr[k]+=vec1.Ptr[k];
    return ret;};
VecCl operator-(const VecCl &vec,const VecCl &vec1){ 
	if (vec.Dim()!=vec1.Dim())
		throw info_except("Dims of substracting vectors are different d1=%i d2=%i\n",vec.Dim(),vec1.Dim());
	VecCl ret=vec;for (int k=1;k<=ret.Dim();k++) ret.Ptr[k]-=vec1.Ptr[k];
    return ret;};
ostream& operator<<(ostream &output, const VecCl &vec)
  { for (int k=1;k<=vec.Dim();k++) output<<vec.Ptr[k]<<"   ";
   output<<"\n";
   return output;};
istream& operator>>(istream &input,VecCl &vec)
  { for (int k=1;k<=vec.Dim();k++) input>>vec.Ptr[k];
   return input;};

double VecCl::ModMax(const VecCl &Vec){   // ModMax - fabs( Max(Vec[k]) )
   double s=MathZer, tmp;
   for (int k=1;k<=Vec.Dim();k++) 
	   if (s<(tmp = ::fabs(Vec[k]))) s=tmp;

   return s;
  };
double VecCl::ModMax(VecCl &Vec,double NewMax){   // ModMax - fabs( Max(Vec[k]) )
	double OldMax = ModMax(Vec);
	double Coef = OldMax<=MathZer?1:OldMax;
	Vec = Vec*NewMax/Coef;
	return OldMax*NewMax/Coef;
  };
VecCl& VecCl::operator=(const MatrCl &mat)
  {
   int Dx,Dy;mat.GetDimMN(Dx,Dy);
   if (Dx!=Dy) { cout<<" VecCl::operator=(MatrCl &mat) - mat is not square, cannot get diag\n";Dx=min(Dx,Dy);}
   SetDim(Dx);
   for (int k=1;k<=Dx;k++) Ptr[k]=mat.Get(k,k);
   return (*this);
  };

VecCl VecCl::fabs(const VecCl &Vec)
 {
  VecCl ret = Vec;
  for (int k=1;k<=ret.Dim();k++) 
	  ret[k]=::fabs(ret[k]);
  return ret;
 };

VecCl VecCl::MakeStepVector(int Num,double low,double up,int centered)
{
  int N=Num ;
  double stp=up-low,start=low;
  if (centered) { stp/=N;start+=0.5*stp; }
  else stp/=max<int>(N-1,1);
  VecCl vec(N);
  for (int k=1;k<=N;k++) {vec[k]=start;start+=stp;}
  return vec;
}



void MatrCl::NewMatr(int DX,int DY)
  {DimX=DX;DimY=DY;
   Addr=new double*[DimX];
   for (int k=0;k<DimX;k++) Addr[k]=new double[DimY];
  };
void MatrCl::DeleteMatr()
  {
   if (Addr!=NULL) 
     { for (int k=0;k<DimX;k++) delete(Addr[k]); delete Addr;Addr=NULL;}
   DimX=0;
   };

double& MatrCl::operator()(int i,int j) {return Addr[i-1][j-1];};
double MatrCl::operator()(int i,int j) const {return Addr[i-1][j-1];};
MatrCl  operator*(const MatrCl &mat1,const MatrCl &mat2)
  {
   int X=mat1.DimX,Y=mat1.DimY,X1=mat2.DimX,Y1=mat2.DimY;
   MatrCl Tmp(X,Y1); 
   if (Y!=X1) { cout<<" cannot mul matrics. Dif. dim\n";return Tmp;}
   double s,**a=mat1.Addr,**a1=mat2.Addr,**t=Tmp.Addr;
   for (int i=0;i<X;i++)
     {  
      for (int j=0;j<Y1;j++)
        {  
         s=0; 
         for (int m=0;m<Y;m++) s+=a[i][m]*a1[m][j];
         t[i][j]=s;
        };
     };
   return Tmp;
  };           

MatrCl  operator*(const MatrCl &matr, double Const)
  {
   int X=matr.DimX,Y=matr.DimY,i,j;
   MatrCl Tmp(X,Y);
   double **t=Tmp.Addr,**a=matr.Addr;
   if (fabs(Const)<MathZer)
     { for (i=0;i<X;i++) {for (j=0;j<Y;j++) t[i][j]=0; }}
   else
     { for (i=0;i<X;i++) {for (j=0;j<Y;j++) t[i][j]=Const*a[i][j]; }}
   return Tmp;
  };
VecCl operator*(const MatrCl &mat1,const VecCl &vec)
  {
   int X=mat1.DimX,Y=mat1.DimY,i,j;
   double **a=mat1.Addr,s;
   VecCl Tmp(X);
   if (vec.Dim()!=Y) { 
       cout<<" Dif Dim in Mat("<<X<<","<<Y<<")*Vec("<<vec.Dim()<<") \n";
       Tmp=vec;
       return Tmp;
   }
   for (i=0;i<X;i++) 
     {
      s=0;
      for (j=0;j<Y;j++) s+=a[i][j]*vec[j+1]; 
      Tmp[i+1]=s;
     };
   return Tmp;
  };
    
VecCl operator*(const VecCl &vec,const MatrCl &mat1)
  {
   int X=mat1.DimX,Y=mat1.DimY,i,j;
   double **a=mat1.Addr,s;
   VecCl Tmp(Y);
   if (vec.Dim()!=X) { cout<<" Dif Dim in Vec("<<vec.Dim()<<")*Mat("<<X<<","<<Y<<") \n";Tmp=vec;return Tmp;}
   for (i=0;i<Y;i++) 
     {
      s=0;
      for (j=0;j<X;j++) s+=a[j][i]*vec[j+1]; 
      Tmp[i+1]=s;
     };
   return Tmp;
  };
    
/*
double* operator*(MatrCl &matr,const double *Vector)
  {
   double* Tmp=new double[matr.Dimenshion+1]; Tmp[0]=matr.Dimenshion;
   double s;
   for (int i=1;i<=matr.Dimenshion;i++)
     {  
      s=0; 
      for (int m=1;m<=matr.Dimenshion;m++) s+=matr(i,m)*Vector[m];
      Tmp[i]=s;
     };       
   return Tmp;
  };           
*/
MatrCl operator+(const MatrCl& mat1, double Const)
  {
   MatrCl Tmp=mat1;
   if (mat1.DimX!=mat1.DimY) 
     { cout<<" Matrics not square;cannot in Mat+Const\n";return Tmp;}
   double **a=Tmp.Addr;
   for (int i=0;i<mat1.DimX;i++)  a[i][i]+=Const;
   return Tmp;
  };           
MatrCl operator-(const MatrCl& mat1,double Const)
  {
   MatrCl Tmp=mat1;
   double **a=Tmp.Addr;
   if (mat1.DimX!=mat1.DimY) 
     { cout<<" Matrics not square;cannot in Mat-Const\n";return Tmp;}
   for (int i=0;i<mat1.DimX;i++)  a[i][i]-=Const;
   return Tmp;
  };
MatrCl operator+(const MatrCl &mat1,const MatrCl &mat2)
  {
   MatrCl Tmp=mat1;
   double **a=Tmp.Addr,**a1=mat2.Addr;
   if ( (mat1.DimX!=mat2.DimX) || (mat1.DimY!=mat2.DimY) )
     {cout<<" Matrics have different dimensions in MatrCl operator +\n";
      return Tmp;}
   for (int i=0;i<mat1.DimX;i++)  
     {for (int i1=0;i1<mat1.DimY;i1++) a[i][i1]+=a1[i][i1];}
   return Tmp;
  };
   
MatrCl operator-(const MatrCl &mat1,const MatrCl &mat2)
  {
   MatrCl Tmp=mat1;
   double **a=Tmp.Addr,**a1=mat2.Addr;
   if ( (mat1.DimX!=mat2.DimX) || (mat1.DimY!=mat2.DimY) )
     {cout<<" Matrics have different dimensions in MatrCl operator -\n";
      return Tmp;}
   for (int i=0;i<mat1.DimX;i++)  
     {for (int i1=0;i1<mat1.DimY;i1++) a[i][i1]-=a1[i][i1];}
   return Tmp;
  };

MatrCl& MatrCl::operator=(const MatrCl &mat)
  {
   SetMatr(mat.DimX,mat.DimY);
   int s=sizeof(double)*DimY;
   double **a=mat.Addr;
   for (int i=0;i<mat.DimX;i++) MemoryMove(a[i],Addr[i],s);
   return *this;
  };

MatrCl& MatrCl::operator=(const VecCl& vec) 
  {
   int Dimen=vec.Dim();SetMatr(Dimen);
   (*this)=(*this)*0;
   for (int k=0;k<Dimen;k++) Addr[k][k]=vec.Ptr[k+1];
   return (*this);
  };


ostream& operator<<(ostream &output,const MatrCl& mat)
  {
   double **a=mat.Addr;
   for (int i=0;i<mat.DimX;i++)
     {
      for (int j=0;j<mat.DimY;j++) output<<a[i][j]<<"  ";
      output<<"\n";
     }
   return output;
  };
istream& operator>>(istream &input,MatrCl& mat)
  {
   double **a=mat.Addr;
   for (int i=0;i<mat.DimX;i++)
     { 
      for (int j=0;j<mat.DimY;j++) input>>a[i][j];
     }
   return input;
  };
VecCl MatrCl::GetRow(const MatrCl &Mat,int i)
  {
   i--;
   int N=Mat.DimY;
   VecCl Tmp(N); 
   MemoryMove(Mat.Addr[i],&Tmp[1],sizeof(double)*N);
   return Tmp;
  };    
VecCl MatrCl::GetCol(const MatrCl &Mat,int i)
  {
   i--;
   int N=Mat.DimX;
   double **a=Mat.Addr;
   VecCl Tmp(N); 
   for (int j=0;j<N;j++) Tmp[j+1]=a[j][i];
   return Tmp;
  };    

void    MatrCl::SetRow(MatrCl &Mat,int i, const VecCl& row)
  { //MemoryMove(&row[1],Mat.Addr[i-1],sizeof(double)*Mat.DimY);
      double **a=Mat.Addr;i--;
      for (int j=0;j<Mat.DimY;j++) a[i][j]= row[j+1];
       };
void    MatrCl::SetCol(MatrCl &Mat,int i, const VecCl& col)
  { double **a=Mat.Addr;i--;for (int j=0;j<Mat.DimX;j++) a[j][i]=col[j+1]; };    
double  Trace(const MatrCl &Mat)
  { double ret=0,**a=Mat.Addr;
    int N=Mat.DimX;
    if (N!=Mat.DimY) { cout<<" Trace Matr not diagonal\n";return 0;}
    for (int j=0;j<N;j++) ret+=a[j][j];return ret; };

int Inverse(MatrCl &Mat)
  {
   int Good=1,Dimen=Mat.DimX;
   if (Dimen!=Mat.DimY) { cout<<" Trace Matr not diagonal\n";return 0;}
   MatrCl a(Dimen),b(Dimen);
   double rbu;
   if (Dimen==1) 
     if (fabs(rbu=Mat(1,1))<MathZer) Good=0; else Mat(1,1)=1/rbu;
   else 
     {
      a=Mat;
      for (int k=1;k<Dimen;k++)
        {
         rbu=Trace(a)/k;
         b=a-rbu;
         a=Mat*b;
//cout<<" Trace "<<rbu<<"\n";
        }
      rbu=Trace(a)/Dimen;
//cout<<" Trace "<<rbu<<"\n";
      if (fabs(rbu)<MathZer) Good=0; else Mat=b*(1/rbu);
    }
  return Good;
 };
MatrCl Transpon(const MatrCl &Mat)
  {
   int X=Mat.DimX,Y=Mat.DimY;
   MatrCl Ret(Y,X);
   double **a=Ret.Addr,**a1=Mat.Addr;
   for (int k=0;k<Y;k++)
     { for (int k1=0;k1<X;k1++) a[k][k1]=a1[k1][k]; }
   return Ret;
  };

double MatrNorm(const MatrCl &Mat)
 {
  double max=0,val;
  for (int x=1;x<=Mat.DimX;x++)
   {
    for (int y=1;y<=Mat.DimY;y++)
     {
      val=fabs(Mat(x,y));if (val>max) max=val;
     }
   }
  return max;
 };

MatrCl DirectMultiplyOfMat(const MatrCl &Fst,const MatrCl &Sec)
 {
  MatrCl ret(Fst.DimX*Sec.DimX,Fst.DimY*Sec.DimY);
  int Nx=Fst.DimX,Ny=Fst.DimY;
  for ( int i=1;i<=Sec.DimX;i++)
   {
    for ( int i1=1;i1<=Sec.DimY;i1++)
     {
      for ( int k=1;k<=Fst.DimX;k++)
       {
        for ( int k1=1;k1<=Fst.DimY;k1++)
          ret((i-1)*Nx+k,(i1-1)*Ny+k1)=Fst(k,k1)*Sec(i,i1);
       }
     }
   }
  return ret;
 };






MatrCl VectorNormalizeMatr(const VecCl &Lft,const MatrCl &Mat,const VecCl Rgt)
  {
   int X=Mat.DimX,Y=Mat.DimY;
   if ( (Lft.Dim()!=X) || (Rgt.Dim()!=Y) ) 
     { cout<<" Error NormVector.Mat.X = "<<X<<" Mat.Y "<<Y
           <<" Lft "<<Lft[0]<<" Rgt "<<Rgt[0]<<" \n";return Mat;}

   MatrCl Ret=Mat;
   double **a=Ret.Addr;
   for (int k=0;k<X;k++)
     { for (int k1=0;k1<Y;k1++) a[k][k1]*=Lft[k+1]*Rgt[k1+1]; }
   return Ret;
  };

















/*
double* MatMulVec(MatrCl &mat1,double *vec)
  {
   double *Ret=new double[mat1.Dimenshion+1];
   Ret[0]=mat1.Dimenshion;
   double s;
   for (int i=1;i<=Ret[0];i++)
     {
      s=0;
      for (int j=1;j<=Ret[0];j++) s+=mat1(i,j)*vec[j];
      Ret[i]=s;
     };
   return Ret;
  };
double* VecMulMat(double *vec,MatrCl &mat1)
  {
   double *Ret=new double[sizeof(double)*mat1.Dimenshion+1];
   Ret[0]=mat1.Dimenshion;
   double s;
   for (int i=1;i<=Ret[0];i++)
     { 
      s=0;
      for (int j=1;j<=Ret[0];i++) s+=mat1(j,i)*vec[j];
      Ret[i]=s;
     };
   return Ret;
  };    









*/
