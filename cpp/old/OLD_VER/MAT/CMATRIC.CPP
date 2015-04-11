#include <math.h>
#include "cmatrics.h"
#define FMT "   ("<<setw(13)<<setprecision(4)
#define FMT1 ","<<setw(13)<<setprecision(4)

CVecCl operator*(CVecCl &vec,my_comp coef)
  { CVecCl ret=vec;
    if (abs(coef)>MathZer)
      {for (int k=1;k<=real(ret.Ptr[0]);k++) ret.Ptr[k]*=coef;}
    else  { for (int k=1;k<=real(ret.Ptr[0]);k++) ret.Ptr[k]=my_comp(0,0);}
    return ret; };
CVecCl operator/(CVecCl &vec,my_comp coef)
  { CVecCl ret=vec;
    if (abs(coef)>MathZer)
      {for (int k=1;k<=real(ret.Ptr[0]);k++) ret.Ptr[k]/=coef;}
    else  
      { for (int k=1;k<=real(ret.Ptr[0]);k++) ret.Ptr[k]=my_comp(MathInf,MathInf);
        cout<<"Error......... Zero Division in CVecCl/n";}
    return ret; };
CVecCl operator+(CVecCl &vec,my_comp cons)
  { CVecCl ret=vec;for (int k=1;k<=real(ret.Ptr[0]);k++) ret.Ptr[k]+=cons;
    return ret;};
my_comp operator*(CVecCl &vec,CVecCl &vec1)
  { my_comp s=my_comp(0,0);
    for (int k=1;k<=real(vec.Ptr[0]);k++) s+=vec.Ptr[k]*vec1.Ptr[k];
    return s; };
CVecCl operator+(CVecCl &vec,CVecCl &vec1)
  { CVecCl ret=vec;for (int k=1;k<=real(ret.Ptr[0]);k++) ret.Ptr[k]+=vec1.Ptr[k];
    return ret;};
CVecCl operator-(CVecCl &vec,CVecCl &vec1)
  { CVecCl ret=vec;for (int k=1;k<=real(ret.Ptr[0]);k++) ret.Ptr[k]-=vec1.Ptr[k];
    return ret;};
ostream& operator<<(ostream &output,CVecCl &vec)
  { for (int k=1;k<=real(vec.Ptr[0]);k++) output<<vec.Ptr[k]<<"   ";
   output<<"\n";
   return output;};
istream& operator>>(istream &input,CVecCl &vec)
  { for (int k=1;k<=real(vec.Ptr[0]);k++) input>>vec.Ptr[k];
   return input;};
double ModMax(CVecCl &Vec,double NewMax)   // ModMax - fabs( Max(Vec[k]) )
  {
   double s=-1,t;
   my_comp *pnt=Vec.Ptr;
   int N=real(pnt[0]);
//   for (int k=1;k<=N;k++) if (s<abs(pnt[k])) s=abs(pnt[k]);
   for (int k=1;k<=N;k++) if (s<(t=norm(pnt[k]))) s=t;
   if (s<MathZer) s=0;
   if ((NewMax!=0) && (s!=0))
     {
      s=NewMax/s;
      for (int k=1;k<=N;k++) pnt[k]*=s;
      s=NewMax;
     }
   return s;
  };
double Mod2(CVecCl &Vec)
  {
   my_comp *pnt=Vec.Ptr;
   double sum=0;
   int N=real(pnt[0]);
   for (int k=1;k<=N;k++) sum+=norm(pnt[k]);
   return sum;
  };
CVecCl Conj(CVecCl &Vec)
  {CVecCl ret=Vec;for (int k=1;k<=real(Vec.Ptr[0]);k++) 
     ret.Ptr[k]=my_comp(real(Vec.Ptr[k]),-imag(Vec.Ptr[k]));return ret;}



my_comp& CMatrCl::operator()(int i,int j)
  {return Addr[(i-1)*Dimenshion+j-1];};
CMatrCl  operator*(CMatrCl &mat1,CMatrCl &mat2)
  {
   CMatrCl Tmp(mat1.Dimenshion);
   my_comp s;
   for (int i=1;i<=mat1.Dimenshion;i++)
     {
      for (int j=1;j<=mat1.Dimenshion;j++)
        {
         s=my_comp(0,0);
         for (int m=1;m<=mat1.Dimenshion;m++) s+=mat1(i,m)*mat2(m,j);
         Tmp(i,j)=s;
        };
     };
   return Tmp;
  };

CMatrCl  operator*(CMatrCl &matr,my_comp Const)
  {
   CMatrCl Tmp(matr.Dimenshion);
   if (abs(Const)<MathZer)
//cout<<" Zer Mull Mat. Const= "<<Const<<"\n";
     { for (int i=0;i<matr.Dimenshion*matr.Dimenshion;i++) Tmp.Addr[i]=0; }
   else
     { for (int i=0;i<matr.Dimenshion*matr.Dimenshion;i++)
			     Tmp.Addr[i]=matr.Addr[i]*Const;};
   return Tmp;
  };
CVecCl operator*(CMatrCl &mat1,CVecCl &vec)
  {
   CVecCl Tmp(mat1.Dimenshion);
   my_comp s;
   for (int i=1;i<=mat1.Dimenshion;i++)
     {
      s=my_comp(0,0);
      for (int j=1;j<=mat1.Dimenshion;j++) s+=mat1(i,j)*vec[j];
      Tmp[i]=s;
     };
   return Tmp;
  };

CVecCl operator*(CVecCl &vec,CMatrCl &mat1)
  {
   CVecCl Tmp(mat1.Dimenshion);
   my_comp s;
   for (int i=1;i<=mat1.Dimenshion;i++)
     {
      s=my_comp(0,0);
      for (int j=1;j<=mat1.Dimenshion;j++) s+=mat1(j,i)*vec[j];
      Tmp[i]=s;
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
   CMatrCl Tmp(mat1.Dimenshion);
   Tmp=mat1;
//   movmem(matr.Addr,Tmp.Addr,sizeof(my_comp)*matr.Dimenshion*matr.Dimenshion);
   for (int i=1;i<=mat1.Dimenshion;i++)  Tmp(i,i)+=Const;
   return Tmp;
  };
CMatrCl operator-(CMatrCl& mat1,my_comp Const)
  {
   CMatrCl Tmp(mat1.Dimenshion);
   Tmp=mat1;
   for (int i=1;i<=mat1.Dimenshion;i++)  Tmp(i,i)-=Const;
   return Tmp;
  };
CMatrCl operator-(CMatrCl& mat1,CMatrCl& mat2)
  {
   int Dimen=mat1.Dim();
   CMatrCl a(Dimen);
   my_comp *re1=mat1.Addr,*re2=mat2.Addr,*wr=a.Addr;
   for (int k=0;k<Dimen*Dimen;k++) wr[k]=re1[k]-re2[k];
   return a;
  };
CMatrCl operator+(CMatrCl& mat1,CMatrCl& mat2)
  {
   int Dimen=mat1.Dim();
   CMatrCl a(Dimen);
   my_comp *re1=mat1.Addr,*re2=mat2.Addr,*wr=a.Addr;
   for (int k=0;k<Dimen*Dimen;k++) wr[k]=re1[k]+re2[k];
   return a;
  };
CMatrCl& CMatrCl::operator=(CMatrCl &mat)
  {
   if (Dimenshion!=mat.Dimenshion) SetMatr(mat.Dimenshion);
//     {cout<<" Error coping CMatrCl, Dif Dim";exit(1);};
   movmem(mat.Addr,Addr,sizeof(my_comp)*Dimenshion*Dimenshion);
   return *this;
  };
CMatrCl& CMatrCl::operator=(CVecCl& vec)
  {
   int Dimen=real(vec[0]);
   if (Dimenshion!=Dimen) SetMatr(Dimen);
   (*this)=(*this)*0;
   for (int k=1;k<=Dimen;k++) (*this)(k,k)=vec[k];
   return (*this);
  };


ostream& operator<<(ostream &output,CMatrCl& mat)
  {
//   int prevwid=output.width(10);
   for (int i=1;i<=mat.Dimenshion;i++)
     {
      for (int j=1;j<=mat.Dimenshion;j++) 
        output<<FMT<<real(mat(i,j))<<FMT1<<imag(mat(i,j))<<")";
      output<<"\n";
     }
//   output.width(prevwid);
   return output;
  };
istream& operator>>(istream &input,CMatrCl& matr)
  {
   for (int i=1;i<=matr.Dimenshion;i++)
     {
      for (int j=1;j<=matr.Dimenshion;j++) input>>matr(i,j);
     }
   return input;
  };
CVecCl GetRow(CMatrCl &Mat,int i)
  {
   int N=Mat.Dim();
   CVecCl Tmp(N); 
   for (int j=1;j<=N;j++) Tmp[j]=Mat(i,j);
   return Tmp;
  };    
CVecCl GetCol(CMatrCl &Mat,int i)
  {
   int N=Mat.Dim();
   CVecCl Tmp(N); 
   for (int j=1;j<=N;j++) Tmp[j]=Mat(j,i);
   return Tmp;
  };    

void    SetRow(CMatrCl &Mat,int i,CVecCl& row)
  { for (int j=1;j<=Mat.Dim();j++) Mat(i,j)=row[j]; };    
void    SetCol(CMatrCl &Mat,int i,CVecCl& col)
  { for (int j=1;j<=Mat.Dim();j++) Mat(j,i)=col[j]; };    
my_comp  Trace(CMatrCl &Mat)
  { my_comp ret=0;
    for (int j=1;j<=Mat.Dim();j++) ret+=Mat(j,j);return ret; };    
CMatrCl Transpon(CMatrCl &Mat)
  {
   int Dimen=Mat.Dim();
   CMatrCl a(Dimen);
   if (Dimen==1) return Mat;
   else
     {
      for (int k=1;k<=Dimen;k++)
        { for (int k1=1;k1<=Dimen;k1++) a(k,k1)=Mat(k1,k); }
     }
   return a;
  };
CMatrCl Conj(CMatrCl &Mat)
  {
   int Dimen=Mat.Dim();
   CMatrCl a(Dimen);
   my_comp *re=Mat.Addr,*wr=a.Addr;
   for (int k=0;k<Dimen*Dimen;k++) wr[k]=conj(re[k]);
   return a;
  };

int Inverse(CMatrCl &Mat)
  {
   int Good=1,Dimen=Mat.Dim();
   CMatrCl a(Dimen),b(Dimen);
   my_comp rbu;
   if (Dimen==1)
     if (abs(rbu=Mat(1,1))<MathZer) Good=0; else Mat(1,1)=1/rbu;
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
      if (abs(rbu)<MathZer) Good=0; else Mat=b*(1/rbu);
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
          r=real(Mat(k,k1));
          Ret(k,k1)=r; Ret(k+Dimen,k1+Dimen)=r;  
          r=imag(Mat(k,k1));
          Ret(k+Dimen,k1)=r; Ret(k,k1+Dimen)=-r;  
         }
      }
   return Ret;
  };

CVecCl RealVComp(VecCl &In)
  {
   int N=In[0]/2;
   CVecCl Ret(N);
   for (int k=1;k<=N;k++) Ret[k]=my_comp(In[k],In[k+N]);
   return Ret;
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
