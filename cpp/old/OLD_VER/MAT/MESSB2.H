#ifndef __MESSB_H
#define __MESSB_H
#include "cmatrics.h"
#include "matrics.h"
#include "eigen_ma.h"
#include "uralgol.h"
#include "turnmat.h"
#include "util.h"
#include "mtquad.h"


int HQsMatClc(double Teta,double Phi,double H,double Qs,double IS,double Eta,
           double *Intencity,double *Position);
int HQsMatMonoClc(double Teta,double Phi,double H,double Qs,double IS,
                  double Eta,double TetaH_View,double PhiH_View,
                  double *Intencity,double *Position);


double HQsInt(double *Intencity,double *Position,double Wid,double x);
void HQsInt(double *Intencity,double *Position,double Wid,double *x,double *y);

void HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
              double Hc,double Teta_V,double *x,double *y);
double HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
              double Hc,double Teta_V,double x);


//  ========== Internal =======================
// Standart Ham for ( H // z ; Qs (Teta,Phi) z )
void QsHMat(CMatrCl &Mat,double Teta,double Phi,
             double H,double Qs,double Eta,double IS);
// Ham for  ( H (Teta_H,Phi_H) z ; Qs (Teta,Phi) z )
void R01HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Teta_H,double Phi_H);
// Standart Ham for ( H // z ; Qs (Teta,Phi) z ) constructed as for relax
void R0HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid);
//  ========== Internal =======================

// =======================================================
// = Ham   for relaxing  =================================
// =======================================================
// =============  Form Full Relax +-H matrics  =====
void R2HUpHDown(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown);
// =============  Form Full Relax  0H matrics ===
void R2HUpHZer(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown);
// =============  Form Full Relax3   matrics ===
void R3Turn120(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double T1,double T2,double T3,
           double Alph_zx,double Alph_xy);
// =============  Form Full Relax  Free1 - Free2 matrics ===
void R2Free(CMatrCl &Mat,
             double Teta1,double Phi1,double H01,double Qs1,double Eta1,
             double Is1,double Wid1,double Alph_H_zx1,double Alph_H_xy1,
             double Teta2,double Phi2,double H02,double Qs2,double Eta2,
             double Is2,double Wid2,double Alph_H_zx2,double Alph_H_xy2,
             double Tup,double Tdown);

// =======================================================
// = Amp   for relaxing  =================================
// =======================================================

void R2HQsAmpVec(double t1,double t2,CMatrCl &AmpL,CMatrCl &AmpR);
//  H (Alph_zx,Alph_xy) z
void R3HQsAmpVec(double Alph_zx1,double Alph_xy1,
                    double Alph_zx2,double Alph_xy2,
                    double Alph_zx3,double Alph_xy3,
                                   double t1,double t2,double t3,
                                   CMatrCl &AmpL,CMatrCl &AmpR);
//  H(1&2) (Alph_zx,Alph_xy) z
void R2HQsAmpVec(double Alph_zx1,double Alph_xy1,
                    double Alph_zx2,double Alph_xy2,
                                   double t1,double t2,
                                   CMatrCl &AmpL,CMatrCl &AmpR);
// =======================================================
// = Int   for relaxing  =================================
// =======================================================
/*
//H - not turned
double RInt(CMatrCl &Mat,CVecCl &AmpL,CMatrCl &AmpR,double x,double Wid1);
void RInt(CMatrCl &Mat,CVecCl &AmpL,CMatrCl &AmpR,double *x,double *y,double Wid1);
*/
// H - turned
double RIntTurn(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double x,double Wid1);
void RIntTurn(CMatrCl &Mat,CMatrCl &AmpL,CMatrCl &AmpR,double *x,double *y,double Wid1);



void R2CircleInt(double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Teta_H,double Phi_H, 
                    double *x,double *y,double TCircle);



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
*/
