#ifndef __ROS_BIN_H
#define __ROS_BIN_H 


#include "ros_equ.h"

struct MixtureComposition
 {
  double MolVeight[2];
  int ClcInteractPart;
  Exp6Param Exp6[3];
  MixtureComposition():alpha(0.1),ClcInteractPart(1){MolVeight[0] = 1;MolVeight[1] = 2;}
  int ReadPar(istream &in)
   {
    char tmp[256];
    in>>tmp;//"ReadSubs1Par"
    if (!Exp6Read(in,Exp6[0])) return 0;in>>tmp;//ReadSubs2Par
    if (!Exp6Read(in,Exp6[1])) return 0;in>>tmp;//ReadMixtPar
    if (!Exp6Read(in,Exp6[2])) return 0;
    in>>tmp>>alpha;
    ClcInteractPart=1;
    if (!seek_eoln(in)) in>>tmp>>ClcInteractPart;
    in>>tmp>>MolVeight[0]>>tmp>>MolVeight[1];
    niu[0]=niu[1]=2e-5;L_Denc=1e4;L_FreeE=0;L_T=1e-5;
    return !(!in);
   }
  void SavePar(ostream &out)
   {
    out<<" Subs1Par   ";Exp6Save(out,Exp6[0]);
    out<<" Subs2Par   ";Exp6Save(out,Exp6[1]);
    out<<" MixturePar ";Exp6Save(out,Exp6[2]);
	out<<" alpha "<<alpha<<" ClcInteractPart "<<ClcInteractPart<<" MolVeight1 "<<MolVeight[0]<<" MolVeight2 "<<MolVeight[1]<<"  ";
   }

  inline MixtureComposition &set_Niu(double niu1,double niu2){niu[0]=niu1;niu[1]=niu2;return *this;}
  inline MixtureComposition &set_Alpha(double alpha_){alpha=alpha_;return *this;}
  inline void get_Niu(double &niu1,double &niu2){niu1=niu[0];niu2=niu[1];};
  inline double get_Alpha(){return alpha;};
  inline double get_MeanMolVeight()   {return (alpha*MolVeight[0]+(1-alpha)*MolVeight[1]);};
  inline void get_MolVeight(double &m1,double &m2){m1=MolVeight[0];m2=MolVeight[1];};

// Variable Param
  double alpha;
  double niu[2];
// For MinCalc
   double L_Denc,L_FreeE,L_T,L_alpha;//L_Niu[2];

 };

struct BinMinConfigParam
 {  
  double S_DensDia,S_Temp,S_Rconst; 
  MixtureComposition *mixt;
//  Exp6Param Exp6[3];
 };


//void ClcYHs(MixtureComposition &mixt,double *y,double &niu)
//double BinHS_Corr(MixtureComposition &mixt)
//BinMinConfigParam *BinRoss6Min;
//double CorBinMinFunc(double *niu,int n_nop)

double FreeBinaryCor(MixtureComposition &mixt,double Denc,double T);

#endif