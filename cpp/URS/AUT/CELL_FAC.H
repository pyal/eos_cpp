#ifndef __CELL_FAC_H
#define __CELL_FAC_H

#include "matt_reg.h"
#include <malloc.h>
//#include <alloc.h>


#define MaxNumMatter 10
//struct RegionData;

struct RegionIO:InterfaceIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName() {return name;}

   int NumPnt,NumMatter,BndMatter[MaxNumMatter+1],NumIntPar;
   double *Pressure,*Energy,*Dencity,*Velocity,*Pos;//,*Length;
   double *PresL,*SounL;
   double *TmpP,*TmpD,*TmpE;
			double **IntPar;
   int *Fixed;
//  Masses between points;   Mass[k] - mass between Pos[k+1] & Pos[k]
   double *Mass;
   MatterIO *Matter[MaxNumMatter+1];
   double LftPnt,RgtPnt,Viscouse,Viscouse2,TimeStability;
   int LftFree,RgtFree;
   int FstCalcP,FstCalcS;
   RegionIO(){Pressure=NULL;NumIntPar=0;}
   int ReadIni(fstream &input);
   void SaveIni(fstream &output);

//   double Pres(MatterIO *Mat,double Denc,double En,double Vel,double dV=0);
//   double Sound(MatterIO *Mat,double Denc,double En,double Vel);

//   double Pres(int k,double dV=0); // Dencity,Energy - ? Velocity
//   double Sound(int k);    // Dencity,Energy 
			void Pres();
   void MullVisc(double *tmp);
			void Sound(double *S);
			void TimeCh(double TimeStp);
//   double Pres(int k,RegionData &Dat,double dV=0); // Dencity,Energy - ? Velocity
//   double Sound(int k,RegionData &Dat);    // Dencity,Energy 
   int GetNumMatter(int k);    
//   ReadPar(fstream &input){};
//   SavePar(fstream &output){};
   void InterfaceIODelete();
    void ExtendBnd()
     {
      if (NumPnt>1)
        {
//         Pos[0]=2*Pos[1]-Pos[2]; Pos[NumPnt+1]=2*Pos[NumPnt]-Pos[NumPnt-1];
         Pos[0]=Pos[1]-Mass[1]/Dencity[1]; 
									Pos[NumPnt+1]=Pos[NumPnt]+Mass[NumPnt]/Dencity[NumPnt];
         Pressure[0]=Pressure[1];Pressure[NumPnt+1]=Pressure[NumPnt];
         Dencity [0]=Dencity [1];Dencity [NumPnt+1]=Dencity [NumPnt];
         Energy  [0]=Energy  [1];Energy  [NumPnt+1]=Energy  [NumPnt];
         Velocity[0]=Velocity[1];Velocity[NumPnt+1]=Velocity[NumPnt];
         Mass    [0]=Mass    [1];Mass    [NumPnt+1]=Mass    [NumPnt];
         if (LftFree) { Pressure[0]=0;Energy[0]=0;Mass[0]=0;}
//         if (RgtFree) { Pressure[NumPnt+1]=0;Energy[NumPnt+1]=0;}
         if (RgtFree) { Pressure[NumPnt]=0;Energy[NumPnt]=0;Mass[NumPnt]=0;}
        } 
     }; 
//    RegionIO& operator=(RegionData& Dat);
   ~RegionIO(){InterfaceIODelete();};
  };

/*

struct RegionData
  {
   int NumPnt;
   double *Pressure,*Energy,*Dencity,*Velocity,*Pos;//,*Length;
   int DeletedPnt;
   RegionData(){DeletedPnt=1;};
//   RegionData(RegionIO &Reg){DeletedPnt=1;GetReg(Reg);}
   ~RegionData(){SetNull();};
   void SetNull()
     {
      if (!DeletedPnt)
        {
         delete Pressure;Pressure=NULL;delete Energy  ;Energy  =NULL;
         delete Dencity ;Dencity =NULL;delete Velocity;Velocity=NULL;
         delete Pos     ;Pos     =NULL;//delete Length  ;Length  =NULL;
         NumPnt=0;
         DeletedPnt=1;
        }
     };

   RegionData& operator=(RegionIO& Reg);
   void SetData(int Num)
     {
      SetNull();NumPnt=Num;
      Pos     =new double[NumPnt+2];                                    
      Pressure=new double[NumPnt+2];                                    
      Dencity =new double[NumPnt+2];                                    
      Energy  =new double[NumPnt+2];                                    
      Velocity=new double[NumPnt+2];                                    
      DeletedPnt=0;
     };
  };
*/

#endif
