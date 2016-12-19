#include <lib/precompiled/math.h>
#include "uralgol.h"




//        =================================================
//        ============    LQ  method   ====================
//        =================================================

void URtred2(MatrCl &InMat,double *Diag,double *DiagPl,MatrCl &Conv,double Tol)
  {
   int N=InMat.Dim(),i,j,k,l;
   double f,g,h,hh;
   Conv=InMat;
   for (i=N;i>=2;i--)
     {
      l=i-2;f=Conv(i,i-1);
      g=0;for(k=1;k<=l;k++) g+=sqr(Conv(i,k));
      h=g+sqr(f);
//    Преобразование Хаусхольдера не выполняется и происходит
//    переход к метке skip если параметр g слишком мал, чтобы
//    гарантировать ортогональность
      if (g<=Tol) { DiagPl[i]=f;h=0;
//cout<<" Skip \n";
                                     goto skip;}
      l++;
      if (f>=0) g=-sqrt(h);else g=sqrt(h);
      DiagPl[i]=g;
      h-=f*g; Conv(i,i-1)=f-g; f=0;
      for (j=1;j<=l;j++)
        {
         Conv(j,i)=Conv(i,j)/h;g=0;
//    формирование вектора InMat*u
         for (k=1;k<=j;k++) g+=Conv(j,k)*Conv(i,k);
         for (k=j+1;k<=l;k++) g+=Conv(k,j)*Conv(i,k);
//    формирование вектора p
         DiagPl[j]=g/h;f+=g*Conv(j,i);
        }
//    вычисление параметра K
      hh=f/(h+h);
//    преобразование матицы InMat
      for (j=1;j<=l;j++)
        {
         f=Conv(i,j);g=DiagPl[j]-hh*f;DiagPl[j]=g;
         for (k=1;k<=j;k++) Conv(j,k)-=(f*DiagPl[k]+g*Conv(i,k));
        }
  skip:
      Diag[i]=h;
//cout<<" Matr "<<N-i+1<<" = \n"<<Conv;
     }
   Diag[1]=0;DiagPl[1]=0;
//    накопление матриц преобразования
   for (i=1;i<=N;i++)
     {
      if (Diag[i]!=0) 
        {
         for (j=1;j<i;j++) 
           {
            g=0;
            for (k=1;k<i;k++) g+=Conv(i,k)*Conv(k,j);
            for (k=1;k<i;k++) Conv(k,j)-=g*Conv(k,i);
           }
        }
//else {cout<<"Zer\n";}
      Diag[i]=Conv(i,i);Conv(i,i)=1;
      for (j=1;j<i;j++) {Conv(i,j)=0;Conv(j,i)=0;}
     }
//cout<<" Result Mattr \n"<<Conv;
  };


int URtql2(double *Diag,double *DiagPl,MatrCl &Conv)
  {
   int i,j,k,l,m,N=Conv.Dim();
   double b,c,f,g,h,p,r,s;
//   double MinDouble=MathZer; - was set to
   double MinDouble=M_Eps;
   for (i=2;i<=N;i++) DiagPl[i-1]=DiagPl[i];
   f=0;b=0;DiagPl[N]=0;
   for (l=1;l<=N;l++)
     {
      j=0;h=MinDouble*(fabs(Diag[l])+fabs(DiagPl[l]));
      if (b<h) b=h;
//cout<<" b= "<<b<<"\n";
//    это определение пренебрежимо малого элемента
      for (m=l;m<=N;m++) if (fabs(DiagPl[m])<=b) goto cont;
  cont:
//cout<<" m= "<<m<<" l= "<<l;
      if (m==l) goto root;
  nextit:
      if (j==30) return 0;
      j++;
//    формирование сдвига
      g=Diag[l];p=(Diag[l+1]-g)/(2*DiagPl[l]);
      r=sqrt(sqr(p)+1);
      if (p<0) Diag[l]=DiagPl[l]/(p-r);
      else Diag[l]=DiagPl[l]/(p+r);
      h=g-Diag[l];
      for (i=l+1;i<=N;i++) Diag[i]-=h;
      f+=h;
//    QL - преобразование
      p=Diag[m];c=1;s=0;
      for (i=m-1;i>=l;i--)
        {
         g=c*DiagPl[i];h=c*p;
         if (fabs(p)>=fabs(DiagPl[i])) 
           {
            c=DiagPl[i]/p;r=sqrt(sqr(c)+1);
            DiagPl[i+1]=s*p*r;
            s=c/r;c=1/r;
           }
         else
           {
            c=p/DiagPl[i];r=sqrt(sqr(c)+1);
            DiagPl[i+1]=s*DiagPl[i]*r;
            s=1/r;c/=r;
           }
         p=c*Diag[i]-s*g;
         Diag[i+1]=h+s*(c*g+s*Diag[i]);
//     формирование собственног вектора
         for (k=1;k<=N;k++)
           {
            h=Conv(k,i+1);
            Conv(k,i+1)=s*Conv(k,i)+c*h;
            Conv(k,i)=c*Conv(k,i)-s*h;
           }
        }
      DiagPl[l]=s*p;Diag[l]=c*p;
      if (fabs(DiagPl[l])>b) goto nextit;
  root:
      Diag[l]+=f;
//cout<<" l= "<<l<<" Diag= "<<Diag[l]<<"\n";
     }
//VecCl VDiag(Diag);
//cout<<" Diag "<<VDiag;VDiag.Ptr=NULL;
//     размещение собственных значений и собственных векторов
   for (i=1;i<=N;i++)
     {
      k=i;p=Diag[i];
      for (j=i+1;j<=N;j++)
        if (Diag[j]<p) {k=j;p=Diag[j];}
        if (k!=i)
          {
           Diag[k]=Diag[i];Diag[i]=p;
           for (j=1;j<=N;j++)
             {p=Conv(j,i);Conv(j,i)=Conv(j,k);Conv(j,k)=p;}
          }
      }
    return 1;
  };    
        









long double URinnerpod(int l,int s,int u,long double c,const VecCl &a,const VecCl &b)
  {
   long double sum=c;
   for (int k=l;k<=u;k+=s) sum+=(long double)a[k]*(long double)b[k];
   return sum;
  };

//     Разложение исходной матрицы на произведение 2 треугольных
//  с использованием алгоритма Краута + вычисляется det(A). стр 97

int URunsymdet(MatrCl &a,double eps,double &d1,int &d2,int *rear)
  {
   int i,j,k,l,N=a.Dim();
   double x,y,*tmp=new double[N+1];tmp[0]=N;

   for (i=1;i<=N;i++)
      tmp[i]=1/sqrt(URinnerpod(1,1,N,0,MatrCl::GetRow(a,i),MatrCl::GetRow(a,i)));
   d1=1;d2=0;
//   d1=0;d2=1;
   for (k=1;k<=N;k++)
     {
//cout<<" k= "<<k<<"\n";
      l=k;x=0;
      for (i=k;i<=N;i++)
        {
         y=URinnerpod(1,1,k-1,-a(i,k),MatrCl::GetRow(a,i),MatrCl::GetCol(a,k));
//cout<<" i= "<<i<<"\n";
         a(i,k)=-y;y=fabs(y*tmp[i]);
         if (y>x) {x=y;l=i;}
        }
      if (l!=k) 
        {
         d1=-d1;
         for (j=1;j<=N;j++) { y=a(k,j); a(k,j)=a(l,j); a(l,j)=y; }
         tmp[l]=tmp[k];
        }
      rear[k]=l;
      d1*=a(k,k);
      if (x<8*eps) {delete []tmp;return 0;}
    l1:
//cout<<" d1= "<<d1<<"\n";
      if (fabs(d1)>=1)  {  d1*=0.0625;d2+=4;goto l1; }
    l2:
//cout<<"d1= "<<d1<<"\n";
      if (fabs(d1)<0.0625)  {  d1*=16;d2-=4;goto l2; }
      x=-1/a(k,k);
      for (j=k+1;j<=N;j++)
         a(k,j)=x*URinnerpod(1,1,k-1,-a(k,j),MatrCl::GetRow(a,k),MatrCl::GetCol(a,j));
     }
   delete []tmp;
   return 1;
  };   


void URunsymsol(MatrCl &a,int *rear,VecCl &b)
  {
   int i,N=a.Dim();//j,k,
   double x;
//      перестановка строк матрицы правых частей b
   for (i=1;i<=N;i++)
     if (rear[i]!=i) 
       { x=b[i];b[i]=b[rear[i]];b[rear[i]]=x;} 
//      решение системы  Ly=b
   for (i=1;i<=N;i++)
      b[i]=-URinnerpod(1,1,i-1,-b[i],MatrCl::GetRow(a,i),b)/a(i,i);
//cout<<L*b-B;MatrCl B1=b;
//      решение системы  Ux=y
   for (i=N;i>=1;i--)
      b[i]=-URinnerpod(i+1,1,N,-b[i],MatrCl::GetRow(a,i),b);
//cout<<U*b-B1<<"\n";

//cout<<" Result b\n"<<b;
  };

// r - Number Vect to solve, a - Matrics to solve, aa - LU decomposed matrics
// rear - array of rearangements, b - array of vectors to solve, eps - misfit
// x - array of result vect, bb - array of misfit of result vect, l - num iter
int URunsymaccsol(MatrCl &a,MatrCl &aa,int *rear,VecCl &b,double eps,
                  VecCl &x,VecCl &bb,int &l)
  {
   int i,d2,N=a.Dim();//j,k,
   double d0,d1,x_max,bb_max;//,c,cc
   x=x*0;bb=b;
   l=0;d0=0;
 l3:
   URunsymsol(aa,rear,bb);
   l++;d2=0;d1=0;
   x=x+bb;

   x_max=0;bb_max=0;
   for (i=1;i<=N;i++)
     {
      if (fabs(x[i])>x_max)   x_max =fabs( x[i]);
      if (fabs(bb[i])>bb_max) bb_max=fabs(bb[i]);
      bb[i]=-URinnerpod(1,1,N,-b[i],MatrCl::GetRow(a,i),x);
     }
   if (bb_max>d1*x_max) d1=bb_max/x_max;
   if (bb_max>2*eps*x_max) d2=1;
//cout<<" bb_max "<<bb_max<<" eps "<<eps<<" x_max "<<x_max<<" d2 "<<d2<<"\n";
//cout<<" d1 "<<d1<<" d0 "<<d0<<" l "<<l<<"\n";
   if ((d1>d0/2) && (l!=1)) return 0;
   d0=d1;
   if (d2==1) goto l3;
   return 1;
  };

int SolveAxb(MatrCl &a,VecCl &b,VecCl &x,double Tol)
  {
//   double q=1,q2=1+q/2;while (q2>1) {q/=2;q2=1+q/2;}
//   long double q1=1;while (((long double)(1+q1/2)-1)>0) q1/=2;
//   Tol=q*a.Dim();cout<<" Tol = "<<Tol<<"\n";
   double d1;
   int N=a.Dim(),l;
   MatrCl LU=a;
   VecCl Mis(N);
   int d2,*rear=new int[N+1];rear[0]=N;
   int ret=URunsymdet(LU,Tol,d1,d2,rear);
   if (!ret) {delete []rear;return ret;}
//cout<<" Matrics \n"<<a<<" Det= "<<d1*pow(2,d2)<<" LU decompose \n"<<LU;
//MatrCl L=LU,U=LU;for (int k=1;k<=N;k++) 
//{for (int k1=1;k1<k;k1++) {U(k,k1)=0;L(k1,k)=0;} L(k,k)=LU(k,k);U(k,k)=1;}
//cout<<L<<U<<" L*U= \n"<<L*U;
   ret=URunsymaccsol(a,LU,rear,b,Tol,x,Mis,l);
//cout<<" Iter done "<<l<<" Error "<<ret<<" Input vector \n"<<b
//    <<" Result vector \n"<<x <<" Misfit \n"<<Mis<<" Delta\n"<<a*x-b
//    <<" Matrics a\n"<<a;
   delete []rear;
   return ret;
  };








// ======================================================
// =============  Complex solution  =====================
// ======================================================


my_comp URCinnerpod(int l,int s,int u,my_comp c,const CVecCl &a,const CVecCl &b)
  {
   long double re=my_real(c),im=my_imag(c);
   my_comp tmp;
   for (int k=l;k<=u;k+=s) { tmp=a[k]*b[k];re+=my_real(tmp);im+=my_imag(tmp);}
   return my_comp(re,im);
  };

//     Разложение исходной матрицы на произведение 2 треугольных
//  с использованием алгоритма Краута + вычисляется det(A). стр 97

int URCunsymdet(CMatrCl &a,double eps,my_comp &d1,int &d2,int *rear)
  {
   int i,j,k,l,N=a.Dim();
   my_comp x,y;
   double *tmp=new double[N+1];tmp[0]=N;
   double xd,yd;

   for (i=1;i<=N;i++)
      tmp[i]=1/sqrt(fabs(URCinnerpod(1,1,N,0,CMatrCl::GetRow(a,i),CMatrCl::GetRow(a,i))));
   d1=my_comp(1,0);d2=0;
//   d1=0;d2=1;
   for (k=1;k<=N;k++)
     {
//cout<<" k= "<<k<<"\n";
      l=k;xd=0;
      for (i=k;i<=N;i++)
        {
         y=URCinnerpod(1,1,k-1,-a(i,k),CMatrCl::GetRow(a,i),CMatrCl::GetCol(a,k));
//cout<<" i= "<<i<<"\n";
         a(i,k)=-y;yd=fabs(y*tmp[i]);
         if (yd>xd) {xd=yd;l=i;}
        }
      if (l!=k) 
        {
         d1=-d1;
         for (j=1;j<=N;j++) { y=a(k,j); a(k,j)=a(l,j); a(l,j)=y; }
         tmp[l]=tmp[k];
        }
      rear[k]=l;
      d1*=a(k,k);
      if (xd<8*eps) 
        {delete []tmp;cout<<"Det=0. Cannot complete LU decomposion\n";return 0;}
    l1:
//cout<<" d1= "<<d1<<"\n";
      if (fabs(d1)>=1)  {  d1*=0.0625;d2+=4;goto l1; }
    l2:
//cout<<"d1= "<<d1<<"\n";
      if (fabs(d1)<0.0625)  {  d1*=16;d2-=4;goto l2; }
      x=-1/a(k,k);
      for (j=k+1;j<=N;j++)
         a(k,j)=x*URCinnerpod(1,1,k-1,-a(k,j),CMatrCl::GetRow(a,k),CMatrCl::GetCol(a,j));
     }
   delete []tmp;
   return 1;
  };   


void URCunsymsol(CMatrCl &a,int *rear,CVecCl &b)
  {
   int i,N=a.Dim();//j,k,
   my_comp x;
//      перестановка строк матрицы правых частей b
   for (i=1;i<=N;i++)
     if (rear[i]!=i) 
       { x=b[i];b[i]=b[rear[i]];b[rear[i]]=x;} 
//      решение системы  Ly=b
   for (i=1;i<=N;i++)
      b[i]=-URCinnerpod(1,1,i-1,-b[i],CMatrCl::GetRow(a,i),b)/a(i,i);
//cout<<L*b-B;MatrCl B1=b;
//      решение системы  Ux=y
   for (i=N;i>=1;i--)
      b[i]=-URCinnerpod(i+1,1,N,-b[i],CMatrCl::GetRow(a,i),b);
//cout<<U*b-B1<<"\n";

//cout<<" Result b\n"<<b;
  };

// r - Number Vect to solve, a - Matrics to solve, aa - LU decomposed matrics
// rear - array of rearangements, b - array of vectors to solve, eps - misfit
// x - array of result vect, bb - array of misfit of result vect, l - num iter
int URCunsymaccsol(CMatrCl &a,CMatrCl &aa,int *rear,CVecCl &b,double eps,
                  CVecCl &x,CVecCl &bb,int &l)
  {
   int i,d2,N=a.Dim();//j,k,
   double d0,d1,x_max,bb_max;//c,cc,
   x=x*0;bb=b;
   l=0;d0=0;
 l3:
   URCunsymsol(aa,rear,bb);
   l++;d2=0;d1=0;
   x=x+bb;

   x_max=0;bb_max=0;
   for (i=1;i<=N;i++)
     {
      if (fabs(x[i])>x_max)   x_max =fabs( x[i]);
      if (fabs(bb[i])>bb_max) bb_max=fabs(bb[i]);
      bb[i]=-URCinnerpod(1,1,N,-b[i],CMatrCl::GetRow(a,i),x);
     }
   if (bb_max>d1*x_max) d1=bb_max/x_max;
   if (bb_max>2*eps*x_max) d2=1;
//cout<<" bb_max "<<bb_max<<" eps "<<eps<<" x_max "<<x_max<<" d2 "<<d2<<"\n";
//cout<<" d1 "<<d1<<" d0 "<<d0<<" l "<<l<<"\n";
   if ((d1>d0/2) && (l!=1)) return 0;
   d0=d1;
   if (d2==1) goto l3;
   return 1;
  };

int CSolveAxb(CMatrCl &a,CVecCl &b,CVecCl &x,double Tol)
  {
//   double q=1,q2=1+q/2;while (q2>1) {q/=2;q2=1+q/2;}
//   long double q1=1;while (((long double)(1+q1/2)-1)>0) q1/=2;
//   Tol=q*a.Dim();cout<<" Tol = "<<Tol<<"\n";
   my_comp d1;
   int N=a.Dim(),l;
   CMatrCl LU=a;
   CVecCl Mis(N);
   int d2,*rear=new int[N+1];rear[0]=N;
   int ret=URCunsymdet(LU,Tol,d1,d2,rear);
   if (!ret) {delete []rear;return ret;}
//cout<<" Matrics \n"<<a<<" Det= "<<d1*pow(2,d2)<<" LU decompose \n"<<LU;
//CMatrCl L=LU,U=LU;for (int k=1;k<=N;k++) {for (int k1=1;k1<k;k1++) 
//{U(k,k1)=0;L(k1,k)=0;} L(k,k)=LU(k,k);U(k,k)=my_comp(1,0);}
//cout<<L<<U<<" L*U= \n"<<L*U;
   ret=URCunsymaccsol(a,LU,rear,b,Tol,x,Mis,l);
//cout<<" Iter done "<<l<<" Error "<<ret<<" Input vector \n"<<b
//    <<" Result vector \n"<<x <<" Misfit \n"<<Mis<<" Delta\n"<<a*x-b
//    <<" Matrics a\n"<<a;
   delete []rear;
   return ret;
  };


int URComeig(CMatrCl &a,CMatrCl &Lft,CMatrCl &Rgt)
  {
   my_comp ctmp,c1,c2,s1,s2,ch,cg,ce,cb,cd,cki,cmi;
   double tem,tau,g,hj,te,tee,max,s,c,isw,de,d,root1,root2,sa,ca,cotx,cot2x;
   double sig,sx,cx,e,b,nd,eta,tse,cos2a,sin2a,tep,root,sh,cb1;//chsb,hr,hi,
   double sb,nc,tanh,ch1;
   int i,j,k,m,it,mark=0,N=a.Dim();
   double *en=new double[N+1];
   double eps=1e-14;
   Lft=a*0;Lft=Lft+1;Rgt=Lft;
//CMatrCl Contr=a;
// проверка числа выполненных операций
   for (it=1;it<=35;it++)
     {
      if (mark) goto done;
// критерий сходимости
      tau=0;
      for (k=1;k<=N;k++) 
        {
         tem=0;
         for (i=1;i<=N;i++) 
           if (i!=k) tem+=fabs(my_real(a(i,k)))+fabs(my_imag(a(i,k)));
         tau+=tem;
         en[k]=tem+fabs(my_real(a(k,k)))+fabs(my_imag(a(k,k)));
        }
// перестановка строк и столбцов
      for (k=1;k<N;k++) 
        {
          max=en[k];i=k;
          for (j=k+1;j<=N;j++)
            if (en[j]>max) {max=en[j];i=j;}
          if (i!=k)
            {
             en[i]=en[k];
             for (j=1;j<=N;j++)
               { 
                ctmp=a(k,j);a(k,j)=a(i,j);a(i,j)=ctmp;
                ctmp=Rgt(k,j);Rgt(k,j)=Rgt(i,j);Rgt(i,j)=ctmp;
                }
             for (j=1;j<=N;j++)
               { 
                ctmp=a(j,k);a(j,k)=a(j,i);a(j,i)=ctmp;
                ctmp=Lft(j,k);Lft(j,k)=Lft(j,i);Lft(j,i)=ctmp;
                }
            }
        }
      if (tau<100*eps) goto done;
// перебор элементов матрицы
      mark=1;
      for (k=1;k<N;k++) 
        {
         for (m=k+1;m<=N;m++) 
           {
            ch=my_comp(0);g=0;hj=0;
            for (i=1;i<=N;i++) 
              if ((i!=k) && (i!=m))
                {
                 ch+=a(k,i)*conj(a(m,i))-a(i,k)*conj(a(i,m));
                 te=norm(a(i,k))+norm(a(m,i));
                 tee=norm(a(k,i))+norm(a(i,m));
                 g+=te+tee;hj+=tee-te;
                }
              cb=a(k,m)+a(m,k);
              ce=a(k,m)-a(m,k);
              cd=a(k,k)-a(m,m);
              te= sqr(my_real(cb))+sqr(my_imag(ce))+sqr(my_real(cd));
              tee=sqr(my_imag(cb))+sqr(my_real(ce))+sqr(my_imag(cd));
              if (te>=tee)
                {
                 isw=1;c=my_real(cb);s=my_imag(ce);d=my_real(cd);de=my_imag(cd);
                 root2=sqrt(te);
                }
              else
                {
                 isw=-1;c=my_imag(cb);s=-my_real(ce);d=my_imag(cd);de=my_real(cd);
                 root2=sqrt(tee);
                }
              root1=sqrt(s*s+c*c);
              sig=-1;if (d>=0) sig=1;
              sa=0;ca=-1;if (c>=0) ca=1;
              if (root1<eps)
                {
                 sx=0;sa=0;cx=1;ca=1;
                 if (isw>0) e=my_real(ce);else e=my_imag(ce);
                 if (isw>0) b=my_imag(cb);else b=-my_real(cb);
                 nd=sqr(d)+sqr(de);
                 goto enter1;
                }
              if (fabs(s)>eps) {ca=c/root1;sa=s/root1;}
              cot2x=d/root1;
              cotx=cot2x+(sig*sqrt(1+sqr(cot2x)));
              sx=sig/sqrt(1+sqr(cotx));cx=sx*cotx;
// определение параметров вращения
              eta=(my_real(ce)*my_real(cb)+my_imag(ce)*my_imag(cb))/root1;
              tse=(my_real(cb)*my_imag(cb)-my_real(ce)*my_imag(ce))/root1;
              te =sig*(-root1*de+tse*d)/root2;
              tee=(d*de+root1*tse)/root2;
              nd =sqr(root2)+sqr(tee);
              tee=hj*cx*sx;cos2a=sqr(ca)-sqr(sa);
              sin2a=2*ca*sa;tem=my_real(ch)*cos2a+my_imag(ch)*sin2a;
              tep=my_imag(ch)*cos2a-my_real(ch)*sin2a;
              ch =sqr(cx)*ch+my_comp(-sqr(sx)*tem-ca*tee,sqr(sx)*tep-sa*tee);
              b=isw*te*ca+eta*sa;
              e=ca*eta-isw*te*sa;
         enter1:
              s=my_real(ch)-sig*root2*e;
              c=my_imag(ch)-sig*root2*b;
              root=sqrt(sqr(c)+sqr(s));
              if (root<eps) {cb1=1;ch1=1;sb=0;sh=0;goto trans;}
              cb1=-c/root;sb=s/root;
              tee=cb1*b-e*sb;nc=sqr(tee);
              tanh=root/(g+2*(nc+nd));
              ch1=1/sqrt(1-sqr(tanh));
              sh=ch1*tanh;
// подготовка к преобразованию
          trans:
              tem=sx*sh*(sa*cb1-sb*ca);
              c1=my_comp(cx*ch1-tem,-sx*sh*(ca*cb1+sa*sb));
              c2=my_comp(cx*ch1+tem,my_imag(c1));
              tep=sx*ch1*ca;tem=cx*sh*sb;
              s1=my_comp(tep-tem,0);s2=my_comp(-tep-tem,0);
              tep=sx*ch1*sa;tem=cx*sh*cb1;
              s1=my_comp(my_real(s1),tep+tem);s2=my_comp(my_real(s2),tep-tem);
// проверка выполнять преобразование или нет
              tem=fabs(s1);tep=fabs(s2);
              if ( (tem>eps) || (tep>eps) )
                {
                 mark=0;
// левое преобразование
                 for (i=1;i<=N;i++)
                   {
                    cki=a(k,i);cmi=a(m,i);
                    a(k,i)=c1*cki+s1*cmi;
                    a(m,i)=s2*cki+c2*cmi;
                    cki=Rgt(k,i);cmi=Rgt(m,i);
                    Rgt(k,i)=c1*cki+s1*cmi;
                    Rgt(m,i)=s2*cki+c2*cmi;
                   }
// правое преобразование
                 for (i=1;i<=N;i++)
                   {
                    cki=a(i,k);cmi=a(i,m);
                    a(i,k)=c2*cki-s2*cmi;
                    a(i,m)=-s1*cki+c1*cmi;
                    cki=Lft(i,k);cmi=Lft(i,m);
                    Lft(i,k)=c2*cki-s2*cmi;
                    Lft(i,m)=-s1*cki+c1*cmi;
                   }
                }
           }
        }
     }
 done:
//cout<<Contr-Lft*a*Rgt;
   delete []en;
   return 1;
  }                  


                 

