#ifndef __MESSB_H
#define __MESSB_H
#include "mat\cmatrics.h"
#include "mat\matrics.h"
#include "mat\eigen_ma.h"
#include "mat\uralgol.h"
#include "mat\turnmat.h"
#include "lib\std\util.h"
#include "mat\mtquad.h"

#include "lib\ref\class_sav.h"
//


//#define Old_HamHTurned_Z_Qs

struct HamData:SavableClass
{
  double Teta,Phi,H,Qs,Eta,Is,Wid;
  HamData():Teta(0),Phi(0),H(0),Qs(0),Eta(0),Is(0),Wid(0){};
  HamData(double teta,double phi,double h,double qs,double eta,double is,double wid):Teta(teta),Phi(phi),H(h),Qs(qs),Eta(eta),Is(is),Wid(wid){};
  HamData(const HamData &src){(*this)=src;}
  //HamData(HamData *src){(*this)=*src;}
  HamData& operator=(const HamData &src) { Teta=src.Teta;Phi=src.Phi;H=src.H;Qs=src.Qs;Eta=src.Eta;Is=src.Is;Wid=src.Wid;return (*this);};
  virtual ~HamData(){};

// Pure matrics, without any addition
  virtual CMatrCl Make4Ham(){return CMatrCl(4)*my_comp(0,0);};
  virtual CMatrCl Make2Ham();
// Current Make8Ham - good for H//Z ( after defining Make4Ham )
// Make8Ham is done with addition (Is,-Wid/2)
  virtual CMatrCl Make8Ham();

  static const double mms_tesl,mu_e,mu_g;
protected:
  void FormMessbInput(double &h,double &teta,double &phi)
  {  h=H/mms_tesl;teta=Teta*M_PI/180;phi=Phi*M_PI/180; };
};

struct Ham_Qs_Eiler:HamData
{
  Ham_Qs_Eiler(double teta,double phi,double h,double qs,double eta,double is,double wid):HamData(teta,phi,h,qs,eta,is,wid){};
  Ham_Qs_Eiler(const Ham_Qs_Eiler &src){this->HamData::operator=(*(HamData*)&src);}
  Ham_Qs_Eiler(const HamData &src){this->HamData::operator=(src);}
//  Ham_Qs_Eiler& operator=(const HamData &src) { Teta=src.Teta;Phi=src.Phi;H=src.H;Qs=src.Qs;Eta=src.Eta;Is=src.Is;Wid=src.Wid;return (*this);};
  CMatrCl Make4Ham();
};
struct Ham_Qs_Gamma:HamData
{
  double Gamma;
  Ham_Qs_Gamma(const Ham_Qs_Gamma &src){this->HamData::operator=(*(HamData*)&src);Gamma=src.Gamma;}
  Ham_Qs_Gamma(const HamData &src,double gamma):Gamma(gamma){this->HamData::operator=(src);}
  CMatrCl Make4Ham();
};
//struct Ham_Z_QsTurned:HamData
//{
//  double H_teta,H_phi;
//  CMatrCl Make4Ham(){return CMatrCl(4)*0;};
//}
struct Ham_Z_Turned_Qs_Gamma:Ham_Qs_Gamma
{
  double H_teta,H_phi;
  Ham_Z_Turned_Qs_Gamma(const Ham_Z_Turned_Qs_Gamma &src):
            Ham_Qs_Gamma(src),H_teta(src.H_teta),H_phi(src.H_phi){};
  Ham_Z_Turned_Qs_Gamma(const Ham_Qs_Gamma &src,double h_teta,double h_phi):
            Ham_Qs_Gamma(src),H_teta(h_teta),H_phi(h_phi){};
  virtual CMatrCl Make4Ham();//{return CMatrCl(4)*0;};
};


struct HamHTurned_Z_Qs:HamData // Qs_z//Z Qs_x//X Qs_y//Y
{
  double H_teta,H_phi; 
  HamHTurned_Z_Qs(const HamHTurned_Z_Qs &src)
        {this->HamData::operator=(*(HamData*)&src);H_teta=src.H_teta;H_phi=src.H_phi;}
  HamHTurned_Z_Qs(const HamData &src,double h_teta,double h_phi):
                  H_teta(h_teta),H_phi(h_phi){this->HamData::operator=(src);}
  CMatrCl Make4Ham();
  CMatrCl Make2Ham();
//  CMatrCl Make8Ham();
};
struct HamHTurned_Z_Free_Qs_Eiler:HamHTurned_Z_Qs
{
  HamHTurned_Z_Free_Qs_Eiler(const HamHTurned_Z_Free_Qs_Eiler &src)
    :HamHTurned_Z_Qs(*(HamHTurned_Z_Qs*)&src){}
  HamHTurned_Z_Free_Qs_Eiler(const HamData &src,double h_teta,double h_phi)
    :HamHTurned_Z_Qs(src,h_teta,h_phi){}
  CMatrCl Make8Ham();
};
struct HamHTurned_Z_Free_Qs_Gamma:HamHTurned_Z_Qs
{
  double Gamma;
  HamHTurned_Z_Free_Qs_Gamma(const HamHTurned_Z_Free_Qs_Gamma &src)
    :HamHTurned_Z_Qs(*(HamHTurned_Z_Qs*)&src),Gamma(src.Gamma){}
  HamHTurned_Z_Free_Qs_Gamma(const HamData &src,double gamma,double h_teta,double h_phi)
    :HamHTurned_Z_Qs(src,h_teta,h_phi),Gamma(gamma){}
  CMatrCl Make8Ham();
};












struct Solver:RefCount
{
  virtual void Solve(double *x,double *Res)=0;
  static  VecCl MakeAmpVecPoly_4() {  VecCl ret(4);ret[1]=0.25;ret[2]=1./6;ret[3]=1./12;ret[4]=0; return ret; }
  static CMatrCl MakeAmpMatrPoly_8_3()
  {
    CMatrCl Ret(8,3);Ret=Ret*0;
    my_comp d3=my_comp(sqrt(1./3),0),d2=my_comp(2*sqrt(1./3),0);
    Ret(1,1)=1; Ret(3,1)=d3;Ret(8,1)=1;Ret(6,1)=d3;
    Ret(1,2)=1; Ret(3,2)=-d3;Ret(8,2)=-1;Ret(6,2)=d3;
    Ret(2,3)=d2; Ret(7,3)=d2;
    Ret=Ret*sqrt(1./8);
    return Ret;
  }; 
  static void GenerateAmp_DirectSol(vector<CMatrCl> &AmpL,vector<CMatrCl> &AmpR)
  {
    AmpL.clear();AmpR.clear();AmpR.resize(3);AmpL.resize(3);
    AmpR[0].SetMatr(2,4);AmpR[0]=AmpR[0]*0;AmpR[1]=AmpR[0];AmpR[2]=AmpR[0];
    my_comp d3=my_comp(sqrt(2./(3*8)),0),d2=my_comp(sqrt(4./(3*8)),0),
            d1=my_comp(sqrt(6./(3*8)),0);

    AmpR[0](1,1)=d1;AmpR[0](2,2)=d3;
    AmpR[1](1,3)=d3;AmpR[1](2,4)=d1;
    AmpR[2](1,2)=d2;AmpR[2](2,3)=d2;
    AmpL[0]=Transpon(AmpR[0]);AmpL[1]=Transpon(AmpR[1]);AmpL[2]=Transpon(AmpR[2]);
  }

};
struct Solver_H_Z_poly:Solver
{
  Ref<HamData> Ham;
  Solver_H_Z_poly(Ref<HamData> ham):Ham(ham){};
  void Solve(double *x,double *Res);
  virtual void SolveIntPos(CMatrCl &Hamilt,VecCl &Int,VecCl &Pos);
  virtual VecCl MakeAmpVec() {return MakeAmpVecPoly_4();}//{  VecCl ret(4);ret[1]=0.25;ret[2]=1./6;ret[3]=1./12;ret[4]=0; return ret; }
  virtual void IntPos(CMatrCl &EigenVect,VecCl &EigenVal,VecCl &Int,VecCl &Pos);
  virtual void Intencity(VecCl &Int,VecCl &Pos,double *x,double *res);
};
struct Solver_H_Z_poly_kashuba:Solver_H_Z_poly
{
  double T,Tbkt,A,W_coef;
  Solver_H_Z_poly_kashuba(Ref<HamData> ham,double t,double tbkt,double a,double w_coef):
                          Solver_H_Z_poly(ham),T(t),Tbkt(tbkt),A(a),W_coef(w_coef){};
  virtual void Intencity(VecCl &Int,VecCl &Pos,double *x,double *res);
};

struct Solver_H_Z_mono:Solver_H_Z_poly
{
  double TetaH_View,PhiH_View;
  Solver_H_Z_mono(Ref<HamData> ham,double tetah_view,double phih_view)
    :Solver_H_Z_poly(ham),TetaH_View(tetah_view),PhiH_View(phih_view){};
  virtual VecCl MakeAmpVec();
  virtual void IntPos(CMatrCl &EigenVect,VecCl &EigenVal,VecCl &Int,VecCl &Pos);
};

struct Solver_H_Free_poly_Base:Solver
{
  virtual void Intencity(CVecCl &Int,CVecCl &Pos,double *x,double *Res);
};
struct Solver_H_Free_poly:Solver_H_Free_poly_Base
{
  Ref<HamData> Ham;
  Solver_H_Free_poly():Ham(NULL){};
  Solver_H_Free_poly(const Ref<HamData> &ham):Ham(ham){};
  void Solve(double *x,double *Res);
  virtual void SolveIntPos(CMatrCl &Hamilt,CVecCl &Int,CVecCl &Pos);
  virtual CMatrCl MakeAmpMatr(){return MakeAmpMatrPoly_8_3();}
  virtual CMatrCl MakeAmpMatrR(){return MakeAmpMatr();}
  virtual CMatrCl MakeAmpMatrL(){return MakeAmpMatr();}
//  virtual void Intencity(CVecCl &Int,CVecCl &Pos,double *x,double *Res);
  virtual void IntClc(CMatrCl &Lft,CMatrCl &Rgt,CVecCl &Int);

};
struct Solver_Blume_Nax_poly:Solver_H_Free_poly
{
  vector<Ref<HamData> > Ham;
  VecCl Times;
  int Nham;
  Solver_Blume_Nax_poly(const vector<Ref<HamData> > &ham,const VecCl &times)
    :Ham(ham),Times(times),Nham(Times[0]){};
  void Solve(double *x,double *Res);
  virtual CMatrCl MakeAmpMatrLR(int Left);
  virtual CMatrCl MakeAmpMatrR(){return MakeAmpMatrLR(0);}
  virtual CMatrCl MakeAmpMatrL(){return MakeAmpMatrLR(1);}
  virtual CMatrCl MakeMatr();
};


struct Solver_Free2_4:Solver_H_Free_poly_Base
{
  Ref<HamData> Ham;
  Solver_Free2_4(HamData *ham):Ham(ham){};
  void Solve(double *x,double *Res);
  virtual void GenerateIntPos(CVecCl &Int,CVecCl &Pos);
  virtual void SolveCurState();
  virtual void GenerateAmp(vector<CMatrCl> &AmpL,vector<CMatrCl> &AmpR){GenerateAmp_DirectSol(AmpL,AmpR);}

protected:
  CMatrCl UeLft,UeRgt,UgLft,UgRgt;
  CVecCl UeEig,UgEig;
};












#endif














/*
void RHQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown);
void RHQsInt(CMatrCl &Mat,double *x,double *y,double Tup,double Tdown,double Wid1);
double RHQsInt(CMatrCl &Mat,double x,double Tup,double Tdown,double Wid1);

// =======================================================
// = Ham  Int for 3    relaxing Hi+120 -> now not works ==
// =======================================================
void R3HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double T1,double T2,double T3,
           double Alph_zx,double Alph_xy);
double R3HQsInt(CMatrCl &Mat,double x,double Alph_zx,double Alph_xy,
                                double T1,double T2,double T3,double Wid1);
void R3HQsInt(CMatrCl &Mat,double *x,double *y,double Alph_zx,double Alph_xy,
                                double T1,double T2,double T3,double Wid1);


// =================================================
// = Ham  Int for relaxing +0H -> now works ========
// =================================================
void RHQs0Qs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown);
void RHQs0QsInt(CMatrCl &Mat,double *x,double *y,double Tup,double Tdown,double Wid1);
double RHQs0QsInt(CMatrCl &Mat,double x,double Tup,double Tdown,double Wid1);


// ===================================================
// = Ham  Int for free relaxing H1  H2 -> now works ==
// ===================================================
void RFree(CMatrCl &Mat,
             double Teta1,double Phi1,double H1,double Qs1,double Eta1,
             double Is1,double Wid1,double Alph_H_zx1,double Alph_H_xy1,
             double Teta2,double Phi2,double H2,double Qs2,double Eta2,
             double Is2,double Wid2,double Alph_H_zx2,double Alph_H_xy2,
             double Tup,double Tdown);
double RFreeInt(CMatrCl &Mat,double x,double Tup,double Tdown,double Wid1);
void RFreeInt(CMatrCl &Mat,double *x,double *y,double Tup,double Tdown,double Wid1);


//  Calculates intencities for the Ham -> QsHMat || R01HQs
//   it turns standart vectors for normalizing Vec // H
//   This turn is not necessary. ??????? Why?
// ==============================================
// =============  Control Block. Not to be used =
// ==============================================
double QsHInt(CMatrCl Mat,double Teta,double Phi,double x);
void QsHInt(CMatrCl Mat,double Teta,double Phi,double *x,double *y);
//   Control example without relaxating
void R0HQsInt(CMatrCl &Mat,double *x,double *y);
double R0HQsInt(CMatrCl &Mat,double x);

void R2HUpHDown(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown);
// =============  Form Full Relax  0H matrics ===
void R2HUpHZer(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown);
// =============  Form Full Relax3   matrics ===
void R3Turn120(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double T1,double T2,double T3,
           double Alph_zx,double Alph_xy);

//  H (Alph_zx,Alph_xy) z
//void R3HQsAmpVec(double Alph_zx1,double Alph_xy1,
//                    double Alph_zx2,double Alph_xy2,
//                    double Alph_zx3,double Alph_xy3,
//                                   double t1,double t2,double t3,
//                                   CMatrCl &AmpL,CMatrCl &AmpR);
//  H(1&2) (Alph_zx,Alph_xy) z
//void R2HQsAmpVec(double Alph_zx1,double Alph_xy1,
//                    double Alph_zx2,double Alph_xy2,
//                                   double t1,double t2,
//                                   CMatrCl &AmpL,CMatrCl &AmpR);
*/

//// Res ham: Z//H
//// Qs main axis system is rotated, first:
////      Phi - rot around Qs_z
////                                 then:
////     Teta - rot around new Fix_y
//// result - Eiler angles
//void HQsEiler_Mat(CMatrCl &Mat,double Teta,double Phi,
//            double H,double Qs,double Eta);
//// Res ham: Z//H
//// Just now corrected !!!!!!!
//// Qs main axis system is rotated, first:
////     Teta - rot around Fix_x
////                                 then:
////      Phi - rot around Fix_z
//// result - for axial Field ( H//Z ) does not depend on Phi
////                                   for any angles Teta, values Eta
////          axis X' - remain always in X-Y plain
//// To be deleted
//void HQsMat(CMatrCl &Mat,double Teta,double Phi,
//            double H,double Qs,double Eta);
//// Res ham: Z//H
//// Qs main axis system is rotated, first:
////     Gamma - rot around Fix_z
////                                 then
////     Teta - rot around Fix_x
////                                 then:
////      Phi - rot around Fix_z
//// result - for axial Field ( H//Z ) does not depend on Phi
////                                   for any angles Teta, values Eta
////          axis X' - remain always in X-Y plain
//// and then effectively we rotate the result around new Z' - by Gamma -
////          result depends on Gamma          
//void HQsGamma_Mat(CMatrCl &Mat,double Teta,double Phi,
//            double H,double Qs,double Eta,double Gamma);
//
//// To be deleted
//int HQsMatClc(double Teta,double Phi,double H,double Qs,double IS,double Eta,
//           double *Intencity,double *Position);
//// new one instead
//int HQsMatClc(CMatrCl &Mat,double H,double IS, double *Intencity,double *Position);
//
//// To be deleted
////int HQsMatMonoClc(double Teta,double Phi,double H,double Qs,double IS,
////                  double Eta,double TetaH_View,double PhiH_View,
////                  double *Intencity,double *Position);
////new one instead
//int HQsMatMono3Clc(double Teta,double Gamma,double Phi,double H,double Qs,double IS,
//                  double Eta,double TetaH_View,double PhiH_View,
//                  double *Intencity,double *Position);
//
//
//
//
//
//double HQsInt(double *Intencity,double *Position,double Wid,double x);
//void HQsInt(double *Intencity,double *Position,double Wid,double *x,double *y);
//
//double HQsInt_Rotate(double *Intencity,double *Position,double Wid,
//					 double T,double Tbkt,double A,double W_coef,double x);
//void HQsInt_Rotate(double *Intencity,double *Position,double Wid,
//				   double T,double Tbkt,double A,double W_coef,double *x,double *y);
//
///*
//void HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
//              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
//              double Hc,double Teta_V,double *x,double *y);
//double HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
//              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
//              double Hc,double Teta_V,double x);
//*/
//
////  ========== Internal =======================
//// Standart Ham for ( H // z ; Qs (Teta,Phi) z )
//void QsHMat(CMatrCl &Mat,double Teta,double Phi,
//             double H,double Qs,double Eta,double IS,
//             CMatrCl *Mat3=NULL,CMatrCl *Mat1=NULL);
////             double H,double Qs,double Eta,double IS);
//
//// Ham for  ( H (Teta_H,Phi_H) z ; Qs (Teta,Phi) z )
//void R01HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
//           double Is,double Wid,double Teta_H,double Phi_H);
//// Standart Ham for ( H // z ; Qs (Teta,Phi) z ) constructed as for relax
//void R0HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
//           double Is,double Wid);
////  ========== Internal =======================
//
//// =======================================================
//// = Ham   for relaxing  =================================
//// =======================================================
//// =============  Form Full Relax +-H matrics  =====
//// =============  Form Full Relax  Free1 - Free2 matrics ===
//void R2Free(CMatrCl &Mat,
//             double Teta1,double Phi1,double H01,double Qs1,double Eta1,
//             double Is1,double Wid1,double Alph_H_zx1,double Alph_H_xy1,
//             double Teta2,double Phi2,double H02,double Qs2,double Eta2,
//             double Is2,double Wid2,double Alph_H_zx2,double Alph_H_xy2,
//             double Tup,double Tdown);
//
////void RnRelax(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
////           double Is,double Wid,double *Time,int N);
//void RnRelax(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
//           double Is,double Wid,VecCl &time,VecCl &field,VecCl &A_zx,VecCl &A_xy);
//CMatrCl RnRelax(CMatrCl *Mat,double *time,int n);
//// =======================================================
//// = Amp   for relaxing  =================================
//// =======================================================
//
//void R2HQsAmpVec(double t1,double t2,CMatrCl &AmpL,CMatrCl &AmpR);
//void RnHQsAmpVec(int n,double *Time,CMatrCl &AmpL,CMatrCl &AmpR);
//// =======================================================
//// = Int   for relaxing  =================================
//// =======================================================
///*
////H - not turned
//double RInt(CMatrCl &Mat,CVecCl &AmpL,CMatrCl &AmpR,double x,double Wid1);
//void RInt(CMatrCl &Mat,CVecCl &AmpL,CMatrCl &AmpR,double *x,double *y,double Wid1);
//*/
//// H - turned
//double RIntTurn(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double x,double Wid1);
//void RIntTurn(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double *x,double *y,double Wid1);
//
//// For relaxating calculation
//CMatrCl R0HQsAmpVec_();
//void GetVecIntTurn(CMatrCl &Mat,CVecCl &Int,CVecCl &Pos,
//                   CMatrCl &AmpL,CMatrCl &AmpR,
//                   CMatrCl *retEig=NULL,CMatrCl *retLft=NULL,CMatrCl *retRgt=NULL);
//
//
////void R2CircleInt(double Teta,double Phi,double H,double Qs,double Eta,
////           double Is,double Wid,double Teta_H,double Phi_H, 
////                    double *x,double *y,double TCircle);
//
//void IntPos(CMatrCl &EigenVect,double *EigenVal,double *Intencity,
//                               double *Position,double H,double IS,VecCl Amp2);
//
//
//
