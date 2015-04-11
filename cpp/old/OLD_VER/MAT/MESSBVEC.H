#ifndef __MESSB_H
#define __MESSB_H
#include "cmatrics.h"
#include "matrics.h"
#include "eigen_ma.h"
#include "uralgol.h"

#include "util.h"


int HQsMatClc(double Teta,double Phi,double H,double Qs,double IS,double Eta,
           double *Intencity,double *Position);
double HQsInt(double *Intencity,double *Position,double Wid,double x);
void HQsInt(double *Intencity,double *Position,double Wid,double *x,double *y);



void R0HQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid);
void R0HQsInt(CMatrCl &Mat,double *x,double *y);
double R0HQsInt(CMatrCl &Mat,double x);

void RHQs(CMatrCl &Mat,double Teta,double Phi,double H,double Qs,double Eta,
           double Is,double Wid,double Tup,double Tdown);
void RHQsInt(CMatrCl &Mat,double *x,double *y);
double RHQsInt(CMatrCl &Mat,double x);

#endif

