#include "util.h"

#include "matrics.h"

VecCl operator*(VecCl &vec,double coef)
  { VecCl ret=vec;
    if (fabs(coef)>MathZer){for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]*=coef;}
    else  { for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]=0;}
    return ret; };
VecCl operator/(VecCl &vec,double coef)
  { VecCl ret=vec;
    if (fabs(coef)>MathZer){for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]/=coef;}
    else  { for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]*=MathInf;
	    cout<<"Error......... Zero Division in VecCl/n";}
    return ret; };
VecCl operator+(VecCl &vec,double cons)
  { VecCl ret=vec;for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]+=cons;
    return ret;};
double operator*(VecCl &vec,VecCl &vec1)
  { double s=0;for (int k=1;k<=vec.Ptr[0];k++) s+=vec.Ptr[k]*vec1.Ptr[k];
    return s; };
VecCl operator+(VecCl &vec,VecCl &vec1)
  { VecCl ret=vec;for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]+=vec1.Ptr[k];
    return ret;};
VecCl operator-(VecCl &vec,VecCl &vec1)
  { VecCl ret=vec;for (int k=1;k<=ret.Ptr[0];k++) ret.Ptr[k]-=vec1.Ptr[k];
    return ret;};
ostream& operator<<(ostream &output,VecCl &vec)
  { for (int k=1;k<=vec.Ptr[0];k++) output<<vec.Ptr[k]<<"   ";
   output<<"\n";
   return output;};
istream& operator>>(istream &input,VecCl &vec)
  { for (int k=1;k<=vec.Ptr[0];k++) input>>vec.Ptr[k];
   return input;};
double ModMax(VecCl &Vec,double NewMax)   // ModMax - fabs( Max(Vec[k]) )
  {
   double s=-1,*pnt=Vec.Ptr;
   int N=pnt[0];
   for (int k=1;k<=N;k++) if (s<fabs(pnt[k])) s=fabs(pnt[k]);
   if (fabs(s)<MathZer) s=0;
   if ((NewMax!=0) && (s!=0))
     {
      s=NewMax/s;
      for (int k=1;k<=N;k++) pnt[k]*=s;
      s=NewMax;
     }
   return s;
  };




void MatrCl::NewMatr(int DX,int DY)
  {DimX=DX;DimY=DY;
   Addr=new double*[DimX];
   for (int k=0;k<DimX;k++) Addr[k]=new double[DimY];
  };
void MatrCl::DeleteMatr()
  {
   if (Addr!=NULL) for (int k=0;k<DimX;k++) delete(Addr[k]);
   delete Addr;DimX=0;
   };

double& MatrCl::operator()(int i,int j) {return Addr[i-1][j-1];};
MatrCl  operator*(MatrCl &mat1,MatrCl &mat2)
  {
   int X=mat1.DimX,Y=mat1.DimY,X1=mat2.DimX,Y1=mat2.DimY;
   MatrCl Tmp(Y,X1); 
   if (X!=Y1) { cout<<" cannot mul matrics. Dif. dim\n";return Tmp;}
   double s,**a=mat1.Addr,**a1=mat2.Addr,**t=Tmp.Addr;
   for (int i=0;i<Y;i++)
     {  
      for (int j=0;j<X1;j++)
        {  
         s=0; 
         for (int m=0;m<X;m++) s+=a[i][m]*a1[m][j];
         t[i][j]=s;
        };
     };
   return Tmp;
  };           

MatrCl  operator*(MatrCl &matr,const double Const)
  {
   int X=matr.DimX,Y=matr.DimY,i,j;
   MatrCl Tmp(X,Y);
   double **t=Tmp.Addr,**a=matr.Addr;
   if (fabs(Const)<MathZer)
//cout<<" Zer Mull Mat. Const= "<<Const<<"\n";
     { for (i=0;i<X;i++) {for (j=0;j<Y;j++) t[i][j]=0; }}
   else
     { for (i=0;i<X;i++) {for (j=0;j<Y;j++) t[i][j]=Const*a[i][j]; }}
cout<<" Mull\n"<<Tmp<<"\n";ChRead();
   return Tmp;
  };
VecCl operator*(MatrCl &mat1,VecCl &vec)
  {
   int X=mat1.DimX,Y=mat1.DimY,i,j;
   double **a=mat1.Addr;
   VecCl Tmp(X);
   if (vec[0]!=Y) { cout<<" Dif Dim in Mat*Vec ";Tmp=vec;return Tmp;}
   double s;
   for (i=0;i<X;i++) 
     {
      s=0;
      for (j=0;j<Y;j++) s+=a[i][j]*vec[j+1]; 
      Tmp[i+1]=s;
     };
   return Tmp;
  };
    
VecCl operator*(VecCl &vec,MatrCl &mat1)
  {
   int X=mat1.DimX,Y=mat1.DimY,i,j;
   double **a=mat1.Addr;
   VecCl Tmp(Y);
   double s;
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
MatrCl operator+(MatrCl& mat1,const double Const)
  {
   MatrCl Tmp=mat1;
   if (mat1.DimX!=mat1.DimY) 
     { cout<<" Matrics not square;cannot do Mat+Const\n";return Tmp;}
   double **a=Tmp.Addr;
//   movmem(matr.Addr,Tmp.Addr,sizeof(double)*matr.Dimenshion*matr.Dimenshion);
   for (int i=0;i<mat1.DimX;i++)  a[i][i]+=Const;
   return Tmp;
  };           
MatrCl operator-(MatrCl& mat1,double Const)
  {
   MatrCl Tmp=mat1;
   double **a=Tmp.Addr;
   if (mat1.DimX!=mat1.DimY) 
     { cout<<" Matrics not square;cannot do Mat-Const\n";return Tmp;}
   for (int i=0;i<mat1.DimX;i++)  a[i][i]-=Const;
   return Tmp;
  };
MatrCl operator+(MatrCl &mat1,MatrCl &mat2)
  {
   MatrCl Tmp=mat1;
   double **a=Tmp.Addr,**a1=mat2.Addr;
   if ( (mat1.DimX!=mat2.DimX) || (mat1.DimY!=mat2.DimY) )
     {cout<<" Matrics have different dimensions in MatrCl operator +\n";
      return Tmp;}
   for (int i=0;i<mat1.DimX;i++)  
     for (int i1=0;i1<mat1.DimY;i1++) a[i][i1]+=a1[i][i1];
   return Tmp;
  };
   
MatrCl operator-(MatrCl &mat1,MatrCl &mat2)
  {
   MatrCl Tmp=mat1;
   double **a=Tmp.Addr,**a1=mat2.Addr;
   if ( (mat1.DimX!=mat2.DimX) || (mat1.DimY!=mat2.DimY) )
     {cout<<" Matrics have different dimensions in MatrCl operator -\n";
      return Tmp;}
   for (int i=0;i<mat1.DimX;i++)  
     for (int i1=0;i1<mat1.DimY;i1++) a[i][i1]-=a1[i][i1];
   return Tmp;
  };

MatrCl& MatrCl::operator=(MatrCl &mat)
  {
cout<<" operator= \n"<<mat<<"\n"<<" DimX "<<DimX<<" Addr "<<Addr<<"\n";
ChRead();
   SetMatr(mat.DimX,mat.DimY);
   int s=sizeof(double)*DimY;
   double **a=mat.Addr;
   for (int i=0;i<mat.DimX;i++) movmem(a[i],(*this).Addr[i],s);
cout<<" operator= \n"<<(*this)<<"\n";
//cout<<sizeof(double)*Dimenshion*Dimenshion<<"\n";
   return *this;
  };

MatrCl& MatrCl::operator=(VecCl& vec) 
  {
   int Dimen=vec[0];SetMatr(Dimen);
   (*this)=(*this)*0;
   for (int k=0;k<Dimen;k++) Addr[k][k]=vec[k+1];
   return (*this);
  };


ostream& operator<<(ostream &output,MatrCl& mat)
  {
//   int prevwid=output.width(10);
   double **a=mat.Addr;
   for (int i=0;i<mat.DimX;i++)
     {
      for (int j=0;j<mat.DimY;j++) output<<a[i][j]<<"  ";
      output<<"\n";
     }
//   output.width(prevwid);
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
VecCl GetRow(MatrCl &Mat,int i)
  {
   int N=Mat.DimY;
   VecCl Tmp(N); 
   movmem(Mat.Addr[i],&Tmp[1],sizeof(double)*N);
   return Tmp;
  };    
VecCl GetCol(MatrCl &Mat,int i)
  {
   int N=Mat.DimX;
   double **a=Mat.Addr;
   VecCl Tmp(N); 
   for (int j=0;j<N;j++) Tmp[j+1]=a[j][i];
   return Tmp;
  };    

void    SetRow(MatrCl &Mat,int i,VecCl& row)
  { movmem(&row[1],Mat.Addr[i],sizeof(double)*Mat.DimY);};
void    SetCol(MatrCl &Mat,int i,VecCl& col)
  { double **a=Mat.Addr;for (int j=0;j<Mat.DimX;j++) a[j][i]=col[j+1]; };    
double  Trace(MatrCl &Mat)
  { double ret=0,**a=Mat.Addr;
    int N=Mat.DimX;
    if (N!=Mat.DimY) { cout<<" Trace Matr not diagonal/n";return 0;}
    for (int j=0;j<N;j++) ret+=a[j][j];return ret; };

int Inverse(MatrCl &Mat)
  {
   int Good=1,Dimen=Mat.Dim();
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
MatrCl Transpon(MatrCl &Mat)
  {
   int Dimen=Mat.Dim();
   MatrCl a(Dimen);
   if (Dimen==1) return Mat;
   else 
     {
      for (int k=1;k<=Dimen;k++)
        { for (int k1=1;k1<=Dimen;k1++) a(k,k1)=Mat(k1,k); }
     }
   return a;
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
