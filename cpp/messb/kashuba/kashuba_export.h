
#ifndef __KASHUBA_EXPORT_H
#define __KASHUBA_EXPORT_H

#include "..\famessb.h"
//#include ".\kashuba_appr.h"


//  =======================================================
//  ============= Polycristall case rotating lines ========
//  =======================================================
struct FAZerHQs_Rotate : FAZerBase {
    FAZerHQs_Rotate();
    //double Clc(double *par,double Xval);
    //void   Clc(double *par,double *Xval,double *Res);
    //void ParStp(double *par,double *stp);
    void ClcBase(double *par, double *Xval, double *Res);
};


//  ================================================
//  ================ Relaxation Kashuba Function ===
//  ================================================

struct FAZerQsHRelaxKashuba : FAZerBase {
    FAZerQsHRelaxKashuba();
    //double Clc(double *par,double Xval);
    //void   Clc(double *par,double *Xval,double *Res);
    //void ParStp(double *par,double *stp);
    void ClcBase(double *par, double *Xval, double *Res);
};


#endif