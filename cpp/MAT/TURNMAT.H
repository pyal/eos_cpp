#ifndef __TURNMAT_H
#define __TURNMAT_H
#include "cmatrics.h"

struct TurnSpinors
{
  static CMatrCl InverseOper(const CMatrCl &mat){return Conj(Transpon(mat));}

  static CMatrCl Turn4Mat(double Teta,double Phi);
  static CMatrCl Turn2Mat(double Teta,double Phi);
  static CMatrCl FormTurn4_2Ham(double Teta,double Phi);
                                                     // Mug/Mue 
  static CMatrCl FormHHam(double Teta,double Phi,double Mug_Mue);
  static CMatrCl FormH4Ham(double H,double Teta,double Phi,double Mug,double Mue);
  static CMatrCl FormH2Ham(double H,double Teta,double Phi,double Mug,double Mue);


  static CMatrCl L_x(int N);
  static CMatrCl L_y(int N);
  static CMatrCl L_z(int N);

  //CMatrCl ITurn4Mat(double Teta,double Phi);
  //CMatrCl ITurn2Mat(double Teta,double Phi);
};
struct CombineMatr
{
  static CMatrCl ExtendMat(CMatrCl &Mat,int i);
  static CMatrCl Sum2Mat(CMatrCl &H1,CMatrCl &H2,CMatrCl &H3,CMatrCl &H4);
  static CMatrCl Sum3Mat(CMatrCl &H11,CMatrCl &H12,CMatrCl &H13,
                  CMatrCl &H21,CMatrCl &H22,CMatrCl &H23,
                  CMatrCl &H31,CMatrCl &H32,CMatrCl &H33);
};
//CMatrCl IFormTurnHam(double Teta,double Phi);
struct V3D_Angles;
struct V3D_XYZ 
{
  VecCl vec;
  V3D_XYZ():vec(3){};
  V3D_XYZ(double X,double Y,double Z):vec(3){this->x()=X;this->y()=Y;this->z()=Z;};
  V3D_XYZ(const V3D_XYZ &src):vec(src.vec){};
  V3D_XYZ(const V3D_Angles &src):vec(3){(operator =)(src);};
  V3D_XYZ& operator=(const V3D_XYZ &src){vec=src.vec;return *this;};
  V3D_XYZ& operator=(const V3D_Angles &src);
  double &x(){return vec[1];}
  double &y(){return vec[2];}
  double &z(){return vec[3];}
  double x()const{return vec[1];}
  double y()const{return vec[2];}
  double z()const{return vec[3];}
  friend ostream& operator<<(ostream &out,V3D_XYZ &ang)
  {out<<ang.vec;return out;}
  friend V3D_XYZ operator+(const V3D_XYZ &ang1,const V3D_XYZ &ang2){V3D_XYZ ret;ret.vec=ang1.vec+ang2.vec;return ret;}
  friend V3D_XYZ operator-(const V3D_XYZ &ang1,const V3D_XYZ &ang2){V3D_XYZ ret;ret.vec=ang1.vec-ang2.vec;return ret;}
  friend V3D_XYZ operator*(const V3D_XYZ &ang1,double d){V3D_XYZ ret;ret.vec=ang1.vec*d;return ret;}
  friend double operator*(const V3D_XYZ &ang1,const V3D_XYZ &ang2){return (ang1.vec*ang2.vec);}
};
// R - length, Teta, Phi
struct V3D_Angles 
{
  VecCl vec;
  V3D_Angles():vec(3){};
  V3D_Angles(double R,double Teta,double Phi):vec(3){this->r()=R;this->phi()=Phi;this->teta()=Teta;};
  V3D_Angles(const V3D_Angles &src):vec(src.vec){};
  V3D_Angles(const V3D_XYZ &src):vec(3){(operator =)(src);};
  V3D_Angles& operator=(const V3D_Angles &src){vec=src.vec;return *this;};
  V3D_Angles& operator=(const V3D_XYZ &src);

  double &r()   {return vec[1];}
  double &teta(){return vec[2];}
  double &phi() {return vec[3];}
  double r() const  {return vec[1];}
  double teta()const{return vec[2];}
  double phi()const {return vec[3];}
  friend ostream& operator<<(ostream &out,const V3D_Angles &ang)  {out<<ang.vec;return out;}
  friend V3D_Angles operator+(const V3D_Angles &ang1,const V3D_Angles &ang2){
    return V3D_Angles(V3D_XYZ(ang1)+V3D_XYZ(ang2));
  }
  friend V3D_Angles operator*(const V3D_Angles &ang1,double d){
    V3D_Angles ret(ang1);
    ret.r()*=d;
    return ret;
  }
  friend double operator*(const V3D_Angles &ang1,const V3D_Angles &ang2){return V3D_XYZ(ang1)*V3D_XYZ(ang2);}
};

struct Turn3D
{
  static double Sign(double x){if (x<0) return -1;if (x>0) return 1;return 0;}
  static double AntiSymMatr(int i,int j,int k);
  static MatrCl TurnMatrAxis(int ax,double Angle);
  static MatrCl TurnMatr(V3D_Angles &rotate);
  static MatrCl ITurnMatr(V3D_Angles &rotate);
  static V3D_XYZ TurnXYZ(const V3D_XYZ &pnt,const MatrCl &rotate);
  static V3D_Angles TurnAngles(const V3D_Angles &pnt,const MatrCl &rotate);
  //static V3D_Angles XYZ2Angles( V3D_XYZ &pnt);
  //static V3D_XYZ Angles2XYZ( V3D_Angles &pnt);
};

struct V2D_XY
{
  VecCl vec;
  V2D_XY():vec(2){};
  V2D_XY(double X,double Y):vec(2){this->x()=X;this->y()=Y;};
  V2D_XY(const V2D_XY &src):vec(src.vec){};
  V2D_XY& operator=(const V2D_XY &src){vec=src.vec;return *this;};
  double &x(){return vec[1];}
  double &y(){return vec[2];}
  double x()const{return vec[1];}
  double y()const{return vec[2];}

  static double VectorMul(const V2D_XY &v1,const V2D_XY &v2){return v1.x()*v2.y()-v1.y()*v2.x();}
  friend ostream& operator<<(ostream &out,V2D_XY &ang)
  {out<<ang.vec;return out;}
  friend V2D_XY operator+(V2D_XY &ang1,V2D_XY &ang2){V2D_XY ret;ret.vec=ang1.vec+ang2.vec;return ret;}
  friend V2D_XY operator-(V2D_XY &ang1,V2D_XY &ang2){V2D_XY ret;ret.vec=ang1.vec-ang2.vec;return ret;}
  friend V2D_XY operator*(V2D_XY &ang1,double d){V2D_XY ret;ret.vec=ang1.vec*d;return ret;}
  friend double operator*(V2D_XY &ang1,V2D_XY &ang2){return (ang1.vec*ang2.vec);}
};

#endif
