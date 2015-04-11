
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




double& MatrCl::operator()(int i,int j)
  {return Addr[(i-1)*Dimenshion+j-1];};
MatrCl  operator*(MatrCl &mat1,MatrCl &mat2)
  {
   MatrCl Tmp(mat1.Dimenshion); 
   double s;
   for (int i=1;i<=mat1.Dimenshion;i++)
     {  
      for (int j=1;j<=mat1.Dimenshion;j++)
        {  
         s=0; 
         for (int m=1;m<=mat1.Dimenshion;m++) s+=mat1(i,m)*mat2(m,j);
         Tmp(i,j)=s;
        };
     };
   return Tmp;
  };           

MatrCl  operator*(MatrCl &matr,const double Const)
  {
   MatrCl Tmp(matr.Dimenshion);
   if (fabs(Const)<MathZer)
//cout<<" Zer Mull Mat. Const= "<<Const<<"\n";
     { for (int i=0;i<matr.Dimenshion*matr.Dimenshion;i++) Tmp.Addr[i]=0; }
   else
     { for (int i=0;i<matr.Dimenshion*matr.Dimenshion;i++)
			     Tmp.Addr[i]=matr.Addr[i]*Const;};
   return Tmp;
  };
VecCl operator*(MatrCl &mat1,VecCl &vec)
  {
   VecCl Tmp(mat1.Dimenshion);
   double s;
   for (int i=1;i<=mat1.Dimenshion;i++) 
     {
      s=0;
      for (int j=1;j<=mat1.Dimenshion;j++) s+=mat1(i,j)*vec[j]; 
      Tmp[i]=s;
     };
   return Tmp;
  };
    
VecCl operator*(VecCl &vec,MatrCl &mat1)
  {
   VecCl Tmp(mat1.Dimenshion);
   double s;
   for (int i=1;i<=mat1.Dimenshion;i++) 
     {
      s=0;
      for (int j=1;j<=mat1.Dimenshion;j++) s+=mat1(j,i)*vec[j]; 
      Tmp[i]=s;
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
   MatrCl Tmp(mat1.Dimenshion);
   Tmp=mat1;
//   movmem(matr.Addr,Tmp.Addr,sizeof(double)*matr.Dimenshion*matr.Dimenshion);
   for (int i=1;i<=mat1.Dimenshion;i++)  Tmp(i,i)+=Const;
   return Tmp;
  };           
MatrCl operator-(MatrCl& mat1,double Const)
  {
   MatrCl Tmp(mat1.Dimenshion);
   Tmp=mat1;
   for (int i=1;i<=mat1.Dimenshion;i++)  Tmp(i,i)-=Const;
   return Tmp;
  };
MatrCl operator+(MatrCl &mat1,MatrCl &mat2)
  {
   MatrCl Tmp=mat1;
   if (mat1.Dimenshion!=mat2.Dimenshion) 
     {cout<<" Matrics have different dimensions in MatrCl operator +\n";
      return Tmp;}
   for (int i=0;i<mat1.Dimenshion*mat1.Dimenshion;i++) 
        Tmp.Addr[i]+=mat2.Addr[i];
   return Tmp;
  };
   
MatrCl operator-(MatrCl &mat1,MatrCl &mat2)
  {
   MatrCl Tmp=mat1;
   if (mat1.Dimenshion!=mat2.Dimenshion) 
     {cout<<" Matrics have different dimensions in MatrCl operator -\n";
      return Tmp;}
   for (int i=0;i<mat1.Dimenshion*mat1.Dimenshion;i++) 
        Tmp.Addr[i]-=mat2.Addr[i];
   return Tmp;
  };

MatrCl& MatrCl::operator=(MatrCl &mat)
  {
   if (Dimenshion!=mat.Dimenshion) SetMatr(mat.Dimenshion);
//     {cout<<" Error coping MatrCl, Dif Dim";exit(1);};
   movmem(mat.Addr,Addr,sizeof(double)*Dimenshion*Dimenshion);
//cout<<sizeof(double)*Dimenshion*Dimenshion<<"\n";
   return *this;
  };

MatrCl& MatrCl::operator=(VecCl& vec) 
  {
   int Dimen=vec[0];
   if (Dimenshion!=Dimen) SetMatr(Dimen);
   (*this)=(*this)*0;
   for (int k=1;k<=Dimen;k++) (*this)(k,k)=vec[k];
   return (*this);
  };


ostream& operator<<(ostream &output,MatrCl& mat)
  {
//   int prevwid=output.width(10);
   for (int i=1;i<=mat.Dimenshion;i++)
     {
      for (int j=1;j<=mat.Dimenshion;j++) output<<mat(i,j)<<"  ";
      output<<"\n";
     }
//   output.width(prevwid);
   return output;
  };
istream& operator>>(istream &input,MatrCl& matr)
  {
   for (int i=1;i<=matr.Dimenshion;i++)
     { 
      for (int j=1;j<=matr.Dimenshion;j++) input>>matr(i,j);
     }
   return input;
  };
VecCl GetRow(MatrCl &Mat,int i)
  {
   int N=Mat.Dim();
   VecCl Tmp(N); 
   for (int j=1;j<=N;j++) Tmp[j]=Mat(i,j);
   return Tmp;
  };    
VecCl GetCol(MatrCl &Mat,int i)
  {
   int N=Mat.Dim();
   VecCl Tmp(N); 
   for (int j=1;j<=N;j++) Tmp[j]=Mat(j,i);
   return Tmp;
  };    

void    SetRow(MatrCl &Mat,int i,VecCl& row)
  { for (int j=1;j<=Mat.Dim();j++) Mat(i,j)=row[j]; };    
void    SetCol(MatrCl &Mat,int i,VecCl& col)
  { for (int j=1;j<=Mat.Dim();j++) Mat(j,i)=col[j]; };    
double  Trace(MatrCl &Mat)
  { double ret=0;
    for (int j=1;j<=Mat.Dim();j++) ret+=Mat(j,j);return ret; };    

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
