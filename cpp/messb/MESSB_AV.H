#ifndef __MESSB_AV_H
#define __MESSB_AV_H
#include "messb.h"


struct SortClass
{
  vector<int> code;
  int AscendingOrder;
  SortClass(VecCl &vec,int ascendingorder=1):AscendingOrder(ascendingorder)
    {MakeCode(vec);}
  double *Ptr;
  static SortClass *Cur;
  static bool greater(int el1,int el2){bool more=(Cur->Ptr[el1]>Cur->Ptr[el2]);return (Cur->AscendingOrder)?more:!more;}
  void MakeCode(VecCl &vec);
  void Code(VecCl &vec);
  void Decode(VecCl &vec);
  void CheckVector(VecCl &vec);
};




struct SphereAnglesGenerator:RefCount
{
  int SphereNum,RotNum,SphereQuater;
  double RotAngle;
  SphereAnglesGenerator(int spherenum,int rotnum,int spherequater,double rotangle):
                        SphereNum(spherenum),RotNum(rotnum),
                        SphereQuater(spherequater),RotAngle(rotangle){};

  vector<V3D_Angles> Angles;
  int CurAngle;
  void InitSphereSerie();
// ang.r - probability of this angles, Sum(ang[i].r)=1
  int NextSphereAngle(V3D_Angles &ang)
  {
    if (CurAngle>=(int)Angles.size()) {RestartSphereSerie();return 0;}
    ang=Angles[CurAngle];CurAngle++;return 1;
  };
  void RestartSphereSerie(){CurAngle=0;};
  void MakeRotAngles(V3D_Angles &rot_ax,V3D_Angles &view_ax,vector<V3D_Angles> &rot_ang);

};

struct AnisotropeGenerator:RefCount
{
  double Hc;
  V3D_Angles Hint;
  V3D_XYZ A;
  static AnisotropeGenerator *CurPtr;
  static double FindAnisC(double c);
  V3D_XYZ CurrentH;

  AnisotropeGenerator(double hcore, const V3D_Angles &h_internal, const V3D_XYZ &a_tensor):
                      Hc(hcore),Hint(h_internal),A(a_tensor){};
  void GetResH(V3D_Angles &Hrnd,V3D_Angles &Hres);

};
struct Solver_H_Z_mono_multiView:Solver_H_Z_mono
{
  vector<V3D_Angles> View;

  Solver_H_Z_mono_multiView(Ref<Ham_Z_Turned_Qs_Gamma> h,vector<V3D_Angles> &view):
                            Solver_H_Z_mono(Ref<HamData>(h),0,0),View(view){};
  void Solve(double *x,double *Res);

};
struct PolyCrystallAverage:Solver
{
  PolyCrystallAverage(SphereAnglesGenerator* angles,AnisotropeGenerator* anis,
                      Ham_Qs_Gamma *ham,double hext):Angles(angles),Anis(anis),
                                                    Ham(ham),Hext(hext){};
  Ref<SphereAnglesGenerator> Angles;
  Ref<AnisotropeGenerator> Anis;
  Ref<Ham_Qs_Gamma> Ham;
  double Hext;

  void Solve(double *x,double *y);
private:


};


#endif

//
//void HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
//              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
//              double Hc,double Teta_V,int Add,int FineAver,int Sum8,double *x,double *y);
//double HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
//              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
//              double Hc,double Teta_V,int Add,int FineAver,int Sum8,double x);

