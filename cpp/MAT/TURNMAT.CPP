#include <lib/precompiled/math.h>

#include "turnmat.h"

#define Pi  M_PI

CMatrCl TurnSpinors::Turn4Mat(double Teta,double Phi)
  {
   CMatrCl Mat(4);
   Mat=Mat*my_comp(0);
   double c,s,c2,sq3,r1,r2,r3,r4,r5,r6;
   c=cos(Teta*Pi/360);s=sin(Teta*Pi/360);c2=cos(Teta*Pi/180);
   sq3=sqrt(3.);
   r1=c*c*c; r2=c*c*s*sq3; r3=c*s*s*sq3; r4=s*s*s;
   r5=-c*(1-3*c2)/2;r6=s*(1+3*c2)/2;

   Phi*=Pi/180;
   my_comp e1=my_comp(cos(1.5*Phi),sin(1.5*Phi)),
           e2=my_comp(cos(0.5*Phi),sin(0.5*Phi)),e3=conj(e2),e4=conj(e1);
   Mat(1,1)= r1*e1; Mat(1,2)= r2*e2; Mat(1,3)= r3*e3; Mat(1,4)= r4*e4;
   Mat(2,1)=-r2*e1; Mat(2,2)= r5*e2; Mat(2,3)= r6*e3; Mat(2,4)= r3*e4;
   Mat(3,1)= r3*e1; Mat(3,2)=-r6*e2; Mat(3,3)= r5*e3; Mat(3,4)= r2*e4;
   Mat(4,1)=-r4*e1; Mat(4,2)= r3*e2; Mat(4,3)=-r2*e3; Mat(4,4)= r1*e4;
   return Mat;
  };

CMatrCl TurnSpinors::Turn2Mat(double Teta,double Phi)
  {
   CMatrCl Mat(2);
   Mat=Mat*my_comp(0);
   double c,s;
   c=cos(Teta*Pi/360);s=sin(Teta*Pi/360);
   Phi*=Pi/180;my_comp e1=my_comp(cos(0.5*Phi),sin(0.5*Phi)),e2=conj(e1);
   Mat(1,1)=c*e1;    Mat(1,2)=s*e2;
   Mat(2,1)=-s*e1;   Mat(2,2)=c*e2;
   return Mat;
  };
CMatrCl TurnSpinors::FormTurn4_2Ham(double Teta,double Phi)
  {
   CMatrCl Mat41(4),Mat42(4),Mat43(4),Mat44(4);
   Mat41=Turn4Mat(Teta,Phi);Mat44=Mat41;Mat42=Mat42*0;Mat43=Mat42;
   Mat41=CombineMatr::Sum2Mat(Mat41,Mat42,Mat43,Mat44);
   CMatrCl Mat2(2);Mat2=Turn2Mat(Teta,Phi);Mat2=CombineMatr::ExtendMat(Mat2,4);
   return Mat2*Mat41;
  };


CMatrCl TurnSpinors::L_x(int N)
  {CMatrCl ret(N);ret=ret*0;
   double L=((double)N-1)*0.5,M;M=L;
   for (int k=2;k<=N;k++)
     {
      my_comp d=sqrt((L+M)*(L-M+1))*0.5;
      ret(k,k-1)=d;ret(k-1,k)=d;
      M-=1;
     };
   return ret;
  }; 

CMatrCl TurnSpinors::L_y(int N)
  {CMatrCl ret(N);ret=ret*0;
   double L=((double)N-1)*0.5,M;M=L;
   for (int k=2;k<=N;k++)
     {
      my_comp d=my_comp(0,sqrt((L+M)*(L-M+1))*0.5);
      ret(k,k-1)=d;ret(k-1,k)=-d;
      M-=1;
     };
   return ret;
  }; 
CMatrCl TurnSpinors::L_z(int N)
  {CMatrCl ret(N);ret=ret*0;
   double L=((double)N-1)*0.5,M;M=L;
   for (int k=1;k<=N;k++)
     {
      ret(k,k)=my_comp(M,0);
      M-=1;
     };
   return ret;
  }; 
CMatrCl TurnSpinors::FormHHam(double Teta,double Phi,double Mug_Mue)
  {
   double t=Teta*Pi/180,p=Phi*Pi/180,st=sin(t);
   double z=cos(t);
   double x=st*cos(p);
   double y=st*sin(p);

   CMatrCl Ex=L_z(4)*z+L_x(4)*x+L_y(4)*y;
   CMatrCl Gr=(L_z(2)*z+L_x(2)*x+L_y(2)*y)*(-Mug_Mue);
   CMatrCl Zer(4);Zer=Zer*0;
   Gr=CombineMatr::ExtendMat(Gr,4);Zer=CombineMatr::Sum2Mat(Ex,Zer,Zer,Ex);
   return Zer+Gr;
  };
CMatrCl TurnSpinors::FormH4Ham(double H,double Teta,double Phi,double Mug,double Mue)
  {
   double t=Teta*Pi/180,p=Phi*Pi/180,st=sin(t);
   double z=cos(t);
   double x=st*cos(p);
   double y=st*sin(p);

   return (L_z(4)*z+L_x(4)*x+L_y(4)*y)*(H*fabs(1/Mug)*Mue);
  };
CMatrCl TurnSpinors::FormH2Ham(double H,double Teta,double Phi,double Mug,double Mue)
  {
   double t=Teta*Pi/180,p=Phi*Pi/180,st=sin(t);
   double z=cos(t);
   double x=st*cos(p);
   double y=st*sin(p);

   return (L_z(2)*z+L_x(2)*x+L_y(2)*y)*(-H*fabs(1/Mug)*Mug);
  };



CMatrCl CombineMatr::ExtendMat(CMatrCl &Mat,int i)
  {
   int N=Mat.Dim();
   CMatrCl Set(N*i);
   for (int k=1;k<=N;k++)
     {
      for (int k1=1;k1<=N;k1++)
        {
         for (int k2=1;k2<=i;k2++)
           {
            for (int k3=1;k3<=i;k3++) Set((k-1)*i+k2,(k1-1)*i+k3)=0;
            Set((k-1)*i+k2,(k1-1)*i+k2)=Mat(k,k1);
           }
        }
     }
   return Set;
  };

CMatrCl CombineMatr::Sum2Mat(CMatrCl &H1,CMatrCl &H2,CMatrCl &H3,CMatrCl &H4)
  {
   int x,y;H1.DimMN(x,y);
   CMatrCl Ret(2*x,2*y);
   for (int k=1;k<=x;k++)
     {
      for (int k1=1;k1<=y;k1++)
        {
         Ret(k,k1)=H1(k,k1)    ;Ret(k,k1+y)=H2(k,k1);
         Ret(k+x,k1)=H3(k,k1)  ;Ret(k+x,k1+y)=H4(k,k1);
        }
     }
   return Ret;
  };

CMatrCl CombineMatr::Sum3Mat(CMatrCl &H11,CMatrCl &H12,CMatrCl &H13,
                CMatrCl &H21,CMatrCl &H22,CMatrCl &H23,
                CMatrCl &H31,CMatrCl &H32,CMatrCl &H33)
  {
   int x,y;H11.DimMN(x,y);
   CMatrCl Ret(3*x,3*y);
   for (int k=1;k<=x;k++)
     {
      for (int k1=1;k1<=y;k1++)
        {
         Ret(k,k1)=H11(k,k1)    ;Ret(k,k1+y)=H12(k,k1)    ;Ret(k,k1+2*y)=H13(k,k1);
         Ret(k+x,k1)=H21(k,k1)  ;Ret(k+x,k1+y)=H22(k,k1)  ;Ret(k+x,k1+2*y)=H23(k,k1);
         Ret(k+2*x,k1)=H31(k,k1);Ret(k+2*x,k1+y)=H32(k,k1);Ret(k+2*x,k1+2*y)=H33(k,k1);
        }
     }
   return Ret;
  };


V3D_XYZ& V3D_XYZ::operator=(const V3D_Angles &src)
{
  double c=M_PI/180;
  double xy=sin(src.teta()*c);
  z()=cos(src.teta()*c);
  x()=xy*cos(src.phi()*c);
  y()=xy*sin(src.phi()*c);
  vec=vec*src.r();
  return *this;
};
V3D_Angles& V3D_Angles::operator=(const V3D_XYZ &src)
{
  double c=180/M_PI,r_=sqrt(VecCl(src.vec)*VecCl(src.vec)),xy=sqrt(sqr(src.x())+sqr(src.y()));
  vec=vec*0;
  r()=r_;
  if (r_<MathZer) return *this;
  teta()=c*acos(src.z()/r_);
  if (xy<MathZer) return *this;
  phi()=c*acos(src.x()/xy);
  if (src.y()<0) phi()=-phi();
  return *this;
};






double Turn3D::AntiSymMatr(int i,int j,int k)
{
  return -Sign(i-j)*Sign(j-k)*Sign(k-i);
}
MatrCl Turn3D::TurnMatrAxis(int ax,double Angle)
{
  MatrCl ret(3);
  double sc=sin(Angle*M_PI/180),cc=cos(Angle*M_PI/180);
  for (int k=1;k<=3;k++)
  {
    for (int k1=1;k1<=3;k1++)
    {
      ret(k,k1)=AntiSymMatr(k,k1,ax)*sc;
    }
    if (k==ax) ret(k,k)=1;else ret(k,k)=cc;
  }
  return ret;
}
MatrCl Turn3D::TurnMatr(V3D_Angles &rotate)
{
  return TurnMatrAxis(3,rotate.phi())*TurnMatrAxis(2,rotate.teta());
}
MatrCl Turn3D::ITurnMatr(V3D_Angles &rotate)
{
//  return Transpon(TurnMatrAxis(3,rotate.phi())*TurnMatrAxis(2,rotate.teta()));
  return Transpon(TurnMatr(rotate));
}
V3D_XYZ Turn3D::TurnXYZ(const V3D_XYZ &pnt,const MatrCl &rotate)
{
  V3D_XYZ ret;
//for (int k=1;k<=3;k++){for (int k1=1;k1<=3;k1++) if (fabs(rotate(k,k1))<1e-10) rotate(k,k1)=0;}
//cout<<" rotate Matr: \n"<<rotate<<" rotate_pnt :"<<pnt<<" res "<<rotate*pnt.vec;
  ret.vec=rotate*pnt.vec;
  return ret;
};
V3D_Angles Turn3D::TurnAngles(const V3D_Angles &pnt,const MatrCl &rotate)
{
  return V3D_Angles(TurnXYZ(V3D_XYZ(pnt),rotate));
};

//V3D_Angles Turn3D::XYZ2Angles( V3D_XYZ &pnt)
//{
//  V3D_Angles ret;
//  double c=180/M_PI,r=sqrt(pnt.vec*pnt.vec),xy=sqrt(sqr(pnt.x())+sqr(pnt.y()));
//  ret.r()=r;
//  if (r<MathZer) return V3D_Angles(r,0,0);
//  ret.teta()=c*acos(pnt.z()/r);
//  if (xy<MathZer) return V3D_Angles(r,ret.teta(),0);
//  ret.phi()=c*acos(pnt.x()/xy);
//  if (pnt.y()<0) ret.phi()=-ret.phi();
//  //ret.phi()=c*acos(pnt.y()/xy);
//  //if (pnt.x()<0) ret.phi()=-ret.phi();
//  return ret;
//};
//V3D_XYZ Turn3D::Angles2XYZ( V3D_Angles &pnt)
//{
//  V3D_XYZ ret;
//  double c=M_PI/180;
//  double xy=sin(pnt.teta()*c);
//  ret.z()=cos(pnt.teta()*c);
//  ret.x()=xy*cos(pnt.phi()*c);
//  ret.y()=xy*sin(pnt.phi()*c);
//  //ret.x()=xy*sin(pnt.phi()*c);
//  //ret.y()=xy*cos(pnt.phi()*c);
//  ret.vec=ret.vec*pnt.r();
//  return ret;
//};
//


/*
void main()
  {
   double Teta;cin>>Teta;
   CMatrCl Mat=FormTurnHam(Teta);
   cout<<FormTurnHam(-Teta)*Mat;
  };
CMatrCl ITurn4Mat(double Teta,double Phi)
  {
   CMatrCl Mat(4);
   Mat=Mat*0;
   double c,s,c2,sq3,r1,r2,r3,r4,r5,r6;
   c=cos(Teta*Pi/360);s=sin(Teta*Pi/360);c2=cos(Teta*Pi/180);
   sq3=sqrt(3.);
   r1=c*c*c; r2=c*c*s*sq3; r3=c*s*s*sq3; r4=s*s*s;
   r5=-c*(1-3*c2)/2;r6=s*(1+3*c2)/2;

   Phi*=Pi/180;
   my_comp e1=my_comp(cos(1.5*Phi),sin(1.5*Phi)),
           e2=my_comp(cos(0.5*Phi),sin(0.5*Phi)),e3=conj(e2),e4=conj(e1);
   Mat(1,1)= r1*e1; Mat(1,2)= r2*e1; Mat(1,3)= r3*e1; Mat(1,4)= r4*e1;
   Mat(2,1)=-r2*e2; Mat(2,2)= r5*e2; Mat(2,3)= r6*e2; Mat(2,4)= r3*e2;
   Mat(3,1)= r3*e3; Mat(3,2)=-r6*e3; Mat(3,3)= r5*e3; Mat(3,4)= r2*e3;
   Mat(4,1)=-r4*e4; Mat(4,2)= r3*e4; Mat(4,3)=-r2*e4; Mat(4,4)= r1*e4;
   return Mat;
  };

CMatrCl ITurn2Mat(double Teta,double Phi)
  {
   CMatrCl Mat(2);
   Mat=Mat*0;
   double c,s;
   c=cos(Teta*Pi/360);s=sin(Teta*Pi/360);
   Phi*=Pi/180;my_comp e1=my_comp(cos(0.5*Phi),sin(0.5*Phi)),e2=conj(e1);
   Mat(1,1)=c*e1;    Mat(1,2)=s*e1;
   Mat(2,1)=-s*e2;   Mat(2,2)=c*e2;
   return Mat;
  };

CMatrCl IFormTurnHam(double Teta,double Phi)
  {
   CMatrCl Mat41(4),Mat42(4),Mat43(4),Mat44(4);
   Mat41=ITurn4Mat(Teta,Phi);Mat44=Mat41;Mat42=Mat42*0;Mat43=Mat42;
   Mat41=Sum2Mat(Mat41,Mat42,Mat43,Mat44);
   CMatrCl Mat2(2);Mat2=ITurn2Mat(Teta,Phi);Mat2=ExtendMat(Mat2,4);
//cout<<" Delta Matr:\n"<<(Mat2*Mat41)-(Mat41*Mat2);
   return Mat2*Mat41;
  };

*/


