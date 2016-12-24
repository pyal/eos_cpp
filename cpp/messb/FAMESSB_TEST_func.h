#ifndef __FaMessb_Test_Func_H
#define __FaMessb_Test_Func_H

#include "famessb.h"

//  =======================================================
//  === Polycristall case  Teta_Phi(const)_Gamma angles ===
//  =============  No phi dependence so it is fixed =======
//  =======================================================

struct FAZerHQs_Gamma : FAZerBase {
    FAZerHQs_Gamma();
    void ClcBase(double *par, double *Xval, double *Res);
};

//  =======================================================
//  === Polycristall case  Teta_Phi  angles of H rotation==
//  =============  in QS bases                      =======
//  =======================================================
struct FAZerQsH : FAZerBase {
    FAZerQsH();
    void ClcBase(double *par, double *Xval, double *Res);
};


//  ================================================
//  ================ Average HQs  case    ==========
//  ================================================
//  Monocristal in the external field
struct FAZerHQsAver : FAZerBase {
    FAZerHQsAver();
    void ClcBase(double *par, double *Xval, double *Res);
};


#endif


/*
//  =======================================================
//  ================ Polycristall case  Eiler angles ======
//  =======================================================
struct FAZerHQs_Eiler:FAZer
  {
   FAZerHQs_Eiler();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
  };
*/