#ifndef __AUT_FAC_H
#define __AUT_FAC_H

#include "matt_reg.h"
#include "aut_hug.h"
//#include <malloc.h>

#define MaxNumMatter 10
#define MaxNumPoints 10000
#define BndPressure 1e-4

struct Shock_Par
  {
   Int_Par Med;
   double Pos,Time,Dvel;
  }; 

struct One_Cell
  {
// Sh1 - from Fst bound D going delta Med
// Sh2 - from Sec bound D going delta Med
// Sh0 - Fst parameters
   Shock_Par Sh1,Sh2,Sh0;
   double CurTime;
   double Length,Mass,Position;
   One_Cell(){CurTime=0;};
   friend Shock_Par SetMeanPar(One_Cell &Par,Int_Par *Set);
//   friend Int_Par GetPar1(One_Cell Par);
//   friend Int_Par GetPar2(One_Cell Par);
   friend void SetCurTim(One_Cell &Par,double Tim);

   friend int Interact(One_Cell &Fst,One_Cell &Sec,
                       MatterIO *MFst,MatterIO *MSec,double Tim);
   friend ostream& operator<<(ostream &output,One_Cell &Par);
   friend istream& operator>>(istream &input,One_Cell &Par);

  }; 

struct AutIO:InterfaceIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName() {return name;}

   int NumPnt,NumMatter,BndMatter[MaxNumMatter+1],BadPar;
   double CurTime;
   MatterIO *Matter[MaxNumMatter+1];
   One_Cell *Data;

   AutIO(){Data=NULL;NumPnt=0;BadPar=1;for (int k=1;k<=MaxNumMatter;k++) Matter[k]=NULL;};

   int ReadIni(istream &input);
   void SaveIni(ostream &output);
   void ReadPar(istream &input);
   void SavePar(ostream &output);

   friend void FormClc(AutIO &dat);
   friend void FormSav(AutIO &dat);
   friend void ClcTim(double TimeClc,AutIO &dat);
   friend int GetNumMatter(int k,AutIO &dat);    
   friend int SetCurTim(int k,AutIO &dat);    

//   int GetNumMatter(int k);    
//   RegionIO& operator=(RegionData& Dat);

   void InterfaceIODelete();
   ~AutIO(){InterfaceIODelete();};
  };

#endif
