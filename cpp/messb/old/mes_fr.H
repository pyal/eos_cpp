

#ifndef __MES_FR_H
#define __MES_FR_H
#include "messb.h"
#include "cmatrics.h"


struct MessRelaxClcPar
 {
  double EndTime;
  CVecCl RelInt;
  CVecCl Freq;
 };

struct MessRelaxInpPar
 {
  int NumHam;
  VecCl Teta,Phi,Times;
  double BegTime;
//  double *Teta,*Phi,*Times;
  MessRelaxInpPar(int N=0) {DeleteStruct(0);ResetDim(N);}
  ~MessRelaxInpPar(){DeleteStruct();};

  void ResetDim(int N)
   { 
     if (N!=NumHam)
      {Teta.SetDim(N);Phi.SetDim(N);Times.SetDim(N);NumHam=N;}
   };
    
  void DeleteStruct(int DelVar=1)
   {if (DelVar) {Teta.SetDim(0);Phi.SetDim(0);Times.SetDim(0);}
    NumHam=0;
   };
  MessRelaxInpPar& operator=(MessRelaxInpPar& par)
   {
    NumHam=par.NumHam;
    Teta=par.Teta;Phi=par.Phi;Times=par.Times;
    return (*this);
   };  

 };

struct RelaxAverageStruct
 {
  int NumToAverage,NumTimes;
  double EndTime;
  double MeanRotTime;
//  double TetaPlane,PhiPlane,RotPlane;
//  double BestDir;
//  double HMaxStp;
  double FurieDiscr;
  double SingleRotation,StrongShort,OutPhi;
  double Teta0,Phi0;
 };
  


void RelaxOneHamInt(MessRelaxInpPar &RelaxPar,
                    double H,double Qs,double Eta,double Is,double Wid,
                    double *x,double *y);
void GenerateRelaxPar(MessRelaxInpPar &RelaxPar,RelaxAverageStruct &Aver);
void RelaxHamInt(RelaxAverageStruct &Aver,
                 double H,double Qs,double Eta,double Is,double Wid,
                 double *x,double *y);

/*
*/
#endif
