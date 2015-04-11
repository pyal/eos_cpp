#include <lib\precompiled\math.h>
#include <math.h>
#include "cmatrics.h"
//#define FMT "   ("<<setw(13)<<setprecision(4)
//#define FMT1 ","<<setw(13)<<setprecision(4)
#define FMC  " ("<<setw(4)<<setprecision(2)
#define FMC1 ","<<setw(4)<<setprecision(2)

CVecCl& CVecCl::operator=(const CMatrCl &mat)
  {
   int Dx,Dy;mat.DimMN(Dx,Dy);
   if (Dx!=Dy) { cout<<" CVecCl::operator=(CMatrCl &mat) - mat is not square, cannot get diag\n";Dx=min(Dx,Dy);}
   SetDim(Dx);
   for (int k=1;k<=Dx;k++) Ptr[k]=mat(k,k);
   return (*this);
  };

CVecCl operator*(CVecCl &vec,my_comp coef)
  { CVecCl ret=vec;
    if (fabs(coef)>MathZer)
      {for (int k=1;k<=my_real(ret.Ptr[0]);k++) ret.Ptr[k]*=coef;}
    else  { for (int k=1;k<=my_real(ret.Ptr[0]);k++) ret.Ptr[k]=my_comp(0,0);}
    return ret; };
CVecCl operator/(CVecCl &vec,my_comp coef)
  { CVecCl ret=vec;
    if (fabs(coef)>MathZer)
      {for (int k=1;k<=my_real(ret.Ptr[0]);k++) ret.Ptr[k]/=coef;}
    else  
      { for (int k=1;k<=my_real(ret.Ptr[0]);k++) ret.Ptr[k]=my_comp(MathInf,MathInf);
        cout<<"Error......... Zero Division in CVecCl/n";}
    return ret; };
CVecCl operator+(CVecCl &vec,my_comp cons)
  { CVecCl ret=vec;for (int k=1;k<=my_real(ret.Ptr[0]);k++) ret.Ptr[k]+=cons;
    return ret;};
my_comp operator*(CVecCl &vec,CVecCl &vec1)
  { my_comp s=my_comp(0,0);
    for (int k=1;k<=my_real(vec.Ptr[0]);k++) s+=vec.Ptr[k]*vec1.Ptr[k];
    return s; };
CVecCl operator+(CVecCl &vec,CVecCl &vec1)
  { CVecCl ret=vec;for (int k=1;k<=my_real(ret.Ptr[0]);k++) ret.Ptr[k]+=vec1.Ptr[k];
    return ret;};
CVecCl operator-(CVecCl &vec,CVecCl &vec1)
  { CVecCl ret=vec;for (int k=1;k<=my_real(ret.Ptr[0]);k++) ret.Ptr[k]-=vec1.Ptr[k];
    return ret;};
ostream& operator<<(ostream &output,CVecCl &vec)
  { for (int k=1;k<=my_real(vec.Ptr[0]);k++) output<<vec.Ptr[k]<<"   ";
   output<<"\n";
   return output;};
istream& operator>>(istream &input,CVecCl &vec)
  { for (int k=1;k<=my_real(vec.Ptr[0]);k++) input>>vec.Ptr[k];
   return input;};


double CVecCl::ModMax(CVecCl &Vec,double NewMax){   // ModMax - fabs( Max(Vec[k]) )
	double OldMax = ModMax(Vec);
	double Coef = OldMax<=MathZer?1:OldMax;
	Vec = Vec*NewMax/Coef;
	return OldMax*NewMax/Coef;
  };
double CVecCl::ModMax(const CVecCl &Vec){   // ModMax - fabs( Max(Vec[k]) )
   double s=MathZer;
   for (int k=1;k<=Vec.Dim();k++) 
	   if (s<norm(Vec[k])) s=norm(Vec[k]);
   return s;
  };
//double ModMax(CVecCl &Vec,double NewMax)   // ModMax - fabs( Max(Vec[k]) )
//  {
//   double s=-1,t;
//   my_comp *pnt=Vec.Ptr;
//   int N=my_real(pnt[0]);
////   for (int k=1;k<=N;k++) if (s<abs(pnt[k])) s=abs(pnt[k]);
//   for (int k=1;k<=N;k++) if (s<(t=norm(pnt[k]))) s=t;
//   if (s<MathZer) s=0;
//   if ((NewMax!=0) && (s!=0))
//     {
//      s=NewMax/s;
//      for (int k=1;k<=N;k++) pnt[k]*=s;
//      s=NewMax;
//     }
//   return s;
//  };
double Mod2(CVecCl &Vec)
  {
   my_comp *pnt=Vec.Ptr;
   double sum=0;
   int N=my_real(pnt[0]);
   for (int k=1;k<=N;k++) sum+=norm(pnt[k]);
   return sum;
  };
CVecCl Conj(CVecCl &Vec)
  {CVecCl ret=Vec;for (int k=1;k<=my_real(ret.Ptr[0]);k++) 
         ret.Ptr[k]=conj(ret.Ptr[k]);
     return ret;}


void CMatrCl::NewMatr(int DX,int DY)
  {DimX=DX;DimY=DY;
   Addr=new my_comp*[DimX];
   for (int k=0;k<DimX;k++) Addr[k]=new my_comp[DimY];
  };
void CMatrCl::DeleteMatr()
  {
   if (Addr!=NULL) 
     { for (int k=0;k<DimX;k++) delete(Addr[k]); delete Addr;Addr=NULL;}
   DimX=0;
   };

my_comp& CMatrCl::operator()(int i,int j) {return Addr[i-1][j-1];};
my_comp CMatrCl::operator()(int i,int j) const {return Addr[i-1][j-1];};
 
CMatrCl  operator*(CMatrCl &mat1,CMatrCl &mat2)
  {
   int X=mat1.DimX,Y=mat1.DimY,X1=mat2.DimX,Y1=mat2.DimY;
   CMatrCl Tmp(X,Y1); 
   if (Y!=X1) 
    { cout<<" cannot mul matrics. Dif. dim\n";return Tmp;}
   my_comp s,**a=mat1.Addr,**a1=mat2.Addr,**t=Tmp.Addr,zer=my_comp(0,0);
   for (int i=0;i<X;i++)
     {  
      for (int j=0;j<Y1;j++)
        {  
         s=zer; 
         for (int m=0;m<Y;m++) s+=a[i][m]*a1[m][j];
         t[i][j]=s;
        };
     };
   return Tmp;
  };           

CMatrCl  operator*(CMatrCl &matr,my_comp Const)
  {
   int X=matr.DimX,Y=matr.DimY,i,j;
   CMatrCl Tmp(X,Y);
   my_comp **t=Tmp.Addr,**a=matr.Addr,zer=my_comp(0,0);
   if (fabs(Const)<MathZer)
     { for (i=0;i<X;i++) {for (j=0;j<Y;j++) t[i][j]=zer; }}
   else
     { for (i=0;i<X;i++) {for (j=0;j<Y;j++) t[i][j]=Const*a[i][j]; }}
   return Tmp;
  };
CVecCl operator*(CMatrCl &mat1,CVecCl &vec)
  {
   int X=mat1.DimX,Y=mat1.DimY,i,j;
   my_comp **a=mat1.Addr,zer=my_comp(0,0),s;
   CVecCl Tmp(X);
   if (my_real(vec[0])!=Y) { cout<<" Dif Dim in Mat*Vec ";Tmp=vec;return Tmp;}
   for (i=0;i<X;i++) 
     {
      s=zer;
      for (j=0;j<Y;j++) s+=a[i][j]*vec[j+1]; 
      Tmp[i+1]=s;
     };
   return Tmp;
  };

CVecCl operator*(CVecCl &vec,CMatrCl &mat1)
  {
   int X=mat1.DimX,Y=mat1.DimY,i,j;
   my_comp **a=mat1.Addr,zer=my_comp(0,0),s;
   CVecCl Tmp(Y);
   if (my_real(vec[0])!=X) { cout<<" Dif Dim in Vec*Mat ";Tmp=vec;return Tmp;}
   for (i=0;i<Y;i++) 
     {
      s=0;
      for (j=0;j<X;j++) s+=a[j][i]*vec[j+1]; 
      Tmp[i+1]=s;
     };
   return Tmp;
  };

/*
my_comp* operator*(CMatrCl &matr,const my_comp *Vector)
  {
   my_comp* Tmp=new my_comp[matr.Dimenshion+1]; Tmp[0]=matr.Dimenshion;
   my_comp s;
   for (int i=1;i<=matr.Dimenshion;i++)
     {
      s=0;
      for (int m=1;m<=matr.Dimenshion;m++) s+=matr(i,m)*Vector[m];
      Tmp[i]=s;
     };
   return Tmp;
  };
*/
CMatrCl operator+(CMatrCl& mat1,my_comp Const)
  {
   CMatrCl Tmp=mat1;
   if (mat1.DimX!=mat1.DimY) 
     { cout<<" Matrics not square;cannot in Mat+Const\n";return Tmp;}
   my_comp **a=Tmp.Addr;
   for (int i=0;i<mat1.DimX;i++)  a[i][i]+=Const;
   return Tmp;
  };           
CMatrCl operator-(CMatrCl& mat1,my_comp Const)
  {
   CMatrCl Tmp=mat1;
   if (mat1.DimX!=mat1.DimY) 
     { cout<<" Matrics not square;cannot in Mat-Const\n";return Tmp;}
   my_comp **a=Tmp.Addr;
   for (int i=0;i<mat1.DimX;i++)  a[i][i]-=Const;
   return Tmp;
  };           
CMatrCl operator+(CMatrCl& mat1,CMatrCl& mat2)
  {
   CMatrCl Tmp=mat1;
   my_comp **a=Tmp.Addr,**a1=mat2.Addr;
   if ( (mat1.DimX!=mat2.DimX) || (mat1.DimY!=mat2.DimY) )
     {cout<<" Matrics have different dimensions in MatrCl operator +\n";
      return Tmp;}
   for (int i=0;i<mat1.DimX;i++)  
     {for (int i1=0;i1<mat1.DimY;i1++) a[i][i1]+=a1[i][i1];}
   return Tmp;
  };
CMatrCl operator-(CMatrCl& mat1,CMatrCl& mat2)
  {
   CMatrCl Tmp=mat1;
   my_comp **a=Tmp.Addr,**a1=mat2.Addr;
   if ( (mat1.DimX!=mat2.DimX) || (mat1.DimY!=mat2.DimY) )
     {cout<<" Matrics have different dimensions in MatrCl operator -\n";
      return Tmp;}
   for (int i=0;i<mat1.DimX;i++)  
     {for (int i1=0;i1<mat1.DimY;i1++) a[i][i1]-=a1[i][i1];}
   return Tmp;
  };
CMatrCl& CMatrCl::operator=(const CMatrCl &mat)
  {
   //if (this==NULL) {return *(new CMatrCl(mat));}
   //if (this==NULL) {return *(this=(new CMatrCl(mat)));}
   SetMatr(mat.DimX,mat.DimY);
   int s=sizeof(my_comp)*DimY;
   my_comp **a=mat.Addr;
   for (int i=0;i<mat.DimX;i++) MemoryMove(a[i],Addr[i],s);
   return *this;
  };
CMatrCl& CMatrCl::operator=(CVecCl& vec)
  {
   int Dimen=my_real(vec[0]);SetMatr(Dimen);
   (*this)=(*this)*my_comp(0,0);
   for (int k=0;k<Dimen;k++) Addr[k][k]=vec[k+1];
   return (*this);
  };


ostream& operator<<(ostream &output,CMatrCl& mat)
  {
   my_comp **a=mat.Addr,t;
   double t1,t2;
   for (int i=0;i<mat.DimX;i++)
     {
      for (int j=0;j<mat.DimY;j++) 
        {t=a[i][j];t1=my_real(t);t2=my_imag(t);
         if (fabs(t1)<1e-8) t1=0;
         if (fabs(t2)<1e-8)  t2=0;
         output<<FMC<<t1<<FMC1<<t2<<")";
        }
      output<<"\n";
     }
   return output;
  };
istream& operator>>(istream &input,CMatrCl& matr)
  {
   my_comp **a=matr.Addr;
   for (int i=0;i<matr.DimX;i++)
     { 
      for (int j=0;j<matr.DimY;j++) input>>a[i][j];
     }
   return input;
  };
CVecCl CMatrCl::GetRow(CMatrCl &Mat,int i)
  {
   i--;
   int N=Mat.DimY;
   CVecCl Tmp(N); 
   MemoryMove(Mat.Addr[i],&Tmp[1],sizeof(my_comp)*N);
   return Tmp;
  };    
CVecCl CMatrCl::GetCol(CMatrCl &Mat,int i)
  {
   i--;
   int N=Mat.DimX;
   my_comp **a=Mat.Addr;
   CVecCl Tmp(N); 
   for (int j=0;j<N;j++) Tmp[j+1]=a[j][i];
   return Tmp;
  };    

void    CMatrCl::SetRow(CMatrCl &Mat,int i,CVecCl& row)
  { MemoryMove(&row[1],Mat.Addr[i-1],sizeof(my_comp)*Mat.DimY);};
void    CMatrCl::SetCol(CMatrCl &Mat,int i,CVecCl& col)
  { my_comp **a=Mat.Addr;i--;for (int j=0;j<Mat.DimX;j++) a[j][i]=col[j+1]; };    
my_comp  Trace(CMatrCl &Mat)
  { my_comp ret=my_comp(0,0),**a=Mat.Addr;
    int N=Mat.DimX;
    if (N!=Mat.DimY) { cout<<" Trace Matr not diagonal/n";return 0;}
    for (int j=0;j<N;j++) ret+=a[j][j];return ret; };
CMatrCl Transpon(CMatrCl &Mat)
  {
   int X=Mat.DimX,Y=Mat.DimY;
   CMatrCl Ret(Y,X);
   my_comp **a=Ret.Addr,**a1=Mat.Addr;
   for (int k=0;k<Y;k++)
     { for (int k1=0;k1<X;k1++) a[k][k1]=a1[k1][k]; }
   return Ret;
  };

CMatrCl Conj(CMatrCl &Mat)
  {
   int X=Mat.DimX,Y=Mat.DimY;
   CMatrCl Ret(X,Y);
   my_comp **a1=Mat.Addr,**a=Ret.Addr;
   for (int k=0;k<X;k++) {for (int k1=0;k1<Y;k1++) a[k][k1]=conj(a1[k][k1]);}
   return Ret;
  };

int Inverse(CMatrCl &Mat)
  {
   int Good=1,Dimen=Mat.DimX;
   if (Dimen!=Mat.DimY) { cout<<" Trace Matr not diagonal/n";return 0;}
   CMatrCl a(Dimen),b(Dimen);
   my_comp rbu;
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

MatrCl ErmitMReal(CMatrCl &Mat)
  {
   int Dimen=Mat.Dim();
   MatrCl Ret(Dimen*2);
   double r;
   for (int k=1;k<=Dimen;k++)
      {
       for (int k1=1;k1<=Dimen;k1++)
         {
          r=my_real(Mat(k,k1));
          Ret(k,k1)=r; Ret(k+Dimen,k1+Dimen)=r;  
          r=my_imag(Mat(k,k1));
          Ret(k+Dimen,k1)=r; Ret(k,k1+Dimen)=-r;  
         }
      }
   return Ret;
  };

CVecCl RealVComp(VecCl &In)
  {
   int N=In.Dim()/2;
   CVecCl Ret(N);
   for (int k=1;k<=N;k++) Ret[k]=my_comp(In[k],In[k+N]);
   return Ret;
  };


double MatrNorm(CMatrCl &Mat)
 {
  double max=0;
  for (int x=1;x<=Mat.DimX;x++)
   {
    for (int y=1;y<=Mat.DimY;y++)
     {
      my_comp val=Mat(x,y);
      double cur=fabs(my_imag(val))+fabs(my_real(val));
      if (cur>max) max=cur;
     }
   }
  return max;
 };


CMatrCl DirectMultiplyOfMat(CMatrCl &Fst,CMatrCl &Sec)
 {
  CMatrCl ret(Fst.DimX*Sec.DimX,Fst.DimY*Sec.DimY);
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













/*
my_comp* MatMulVec(CMatrCl &mat1,my_comp *vec)
  {
   my_comp *Ret=new my_comp[mat1.Dimenshion+1];
   Ret[0]=mat1.Dimenshion;
   my_comp s;
   for (int i=1;i<=Ret[0];i++)
     {
      s=0;
      for (int j=1;j<=Ret[0];j++) s+=mat1(i,j)*vec[j];
      Ret[i]=s;
     };
   return Ret;
  };
my_comp* VecMulMat(my_comp *vec,CMatrCl &mat1)
  {
   my_comp *Ret=new my_comp[sizeof(my_comp)*mat1.Dimenshion+1];
   Ret[0]=mat1.Dimenshion;
   my_comp s;
   for (int i=1;i<=Ret[0];i++)
     { 
      s=0;
      for (int j=1;j<=Ret[0];i++) s+=mat1(j,i)*vec[j];
      Ret[i]=s;
     };
   return Ret;
  };    









*/
