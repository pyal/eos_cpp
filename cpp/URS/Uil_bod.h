#ifndef __UIL_BOD_H
#define __UIL_BOD_H


#include "matt_fac.h"
//#include <malloc.h>
//#include <alloc.h>


#define MaxNumMatter 50
#define ZeroPressure 10

struct BndStruct {
    double Mass, Pressure, Velocity, Pos;   //,SoundVisc,VelocityPlus;
    int Free;
    //   	void SetBnd(double M,double P,double V,int F)  {Mass=M;Pressure=P; Velocity=V; Free=F;}
    BndStruct() {
        Free = 1;
        Mass = 0;
        Pressure = ZeroPressure;
    };
};

struct Body : RefCount {
    int NumPnt, NumIntPar;
    int UnfixAll;
    double Viscouse, Viscouse2;
    double *Pressure, *Energy, *Dencity, *Velocity, *Pos;   //,*Volume;
    double *PresL, *SounL;
    double *TmpP, *TmpD, *TmpE;
    DataVector<DataVector<double, CopyStructFast<double>>> IntPar;
    map<Stroka, double *> InputPar;
    int *Fixed;
    double *Mass;
    MatterIO *Matter;
    int UilWorkDim;
    int FstCalcP, FstCalcS;
    int IterCalcS, NumSkipS;

    int ReadData(istream &input);
    void SaveData(ostream &output);
    void Pres();
    void Sound(double *S);
    void TimeCh(double TimeStp);
    void MullVisc(double *tmp);

    void SetIntPar(int NumP, int NumI, double Visc, double Visc2, MatterIO *Mat) {
        NumPnt = NumP;
        NumIntPar = NumI;
        Viscouse = Visc;
        Viscouse2 = Visc2;
        Matter = Mat;
        FstCalcP = 1;
        FstCalcS = 1;
    };
    Body() {
        Matter = NULL;
        NumPnt = 0;
        IterCalcS = 0;
        NumSkipS = 10;
        UnfixAll = 0;
    }
    Body(
        int NPnt,
        MatterIO *Mat,
        double Vis,
        double Vis2,
        const DataVector<Stroka> &IntParNames) {
        Matter = NULL;
        NumPnt = 0;
        IterCalcS = 0;
        NumSkipS = 10;
        Create(NPnt, Mat, Vis, Vis2, IntParNames);
        UnfixAll = 0;
    }
    void DeleteBody();
    void Create(
        int NPnt,
        MatterIO *Mat,
        double Vis,
        double Vis2,
        const DataVector<Stroka> &IntParNames);
    ~Body() {
        DeleteBody();
    };
    BndStruct BndFst, BndLst;

    void BoundV(double &U0, double &Ul)   // AddU[1]*=2;AddU[Num]*=2;
    {
        if(BndFst.Free)
            U0 *= 2;
        if(BndLst.Free)
            Ul *= 2;
    };

    void SetBoundMU(Body *Fst, Body *Lst) {
        int N1;
        if((Fst == NULL) || (Fst->Mass[1] <= MathZer)) {
            BndFst.Free = 1;
            BndFst.Mass = 0;
            BndFst.Velocity = Velocity[1];
        } else {
            N1 = Fst->NumPnt - 1;
            BndFst.Free = 0;
            BndFst.Mass = Fst->Mass[N1];
            BndFst.Velocity = Fst->Velocity[N1];
        }
        if((Lst == NULL) || (Lst->Mass[1] <= MathZer)) {
            BndLst.Free = 1;
            BndLst.Mass = 0;
            BndLst.Velocity = Velocity[NumPnt - 1];
        } else {
            N1 = 1;
            BndLst.Free = 0;
            BndLst.Mass = Lst->Mass[N1];
            BndLst.Velocity = Lst->Velocity[N1];
        }
        if((UilWorkDim != 1) && (Fst == NULL)) {
            BndFst.Free = 0;
            BndFst.Mass = 1e10;
            BndFst.Velocity = 0;
        }
        int N = NumPnt;
        Mass[0] = BndFst.Mass;
        Velocity[0] = BndFst.Velocity;
        Mass[N] = BndLst.Mass;
        Velocity[N] = BndLst.Velocity;
    };

    void SetBoundPv(Body *Fst, Body *Lst) {
        int N1;
        if((Fst == NULL) || (Fst->Mass[1] <= MathZer)) {
            BndFst.Pressure = ZeroPressure;
            BndFst.Pos = 2 * Pos[1] - Pos[2];
        } else {
            N1 = Fst->NumPnt - 1;
            BndFst.Pressure = Fst->Pressure[N1];
            BndFst.Pos = Fst->Pos[N1];
        }
        if((Lst == NULL) || (Lst->Mass[1] <= MathZer)) {
            BndLst.Pressure = ZeroPressure;
            BndLst.Pos = 2 * Pos[NumPnt - 1] - Pos[NumPnt - 2];
        } else {
            N1 = 1;
            BndLst.Pressure = Lst->Pressure[N1];
            BndLst.Pos = Lst->Pos[N1];
        }
        if((UilWorkDim != 1) && (Fst == NULL)) {
            BndFst.Pressure = Pressure[1];
            BndFst.Pos = 2 * Pos[1] - Pos[2];
        }
        int N = NumPnt;
        Pressure[0] = BndFst.Pressure;
        Pressure[N] = BndLst.Pressure;
        Pos[0] = BndFst.Pos;
        Pos[N] = BndLst.Pos;
    };
    void SetBoundU(Body *Fst, Body *Lst) {
        if((Lst != NULL) && (Lst->Mass[1] > MathZer)) {
            BndLst.Velocity = Lst->Velocity[1];
            Velocity[NumPnt] = BndLst.Velocity;
        }
        if((Fst != NULL) && (Fst->Mass[1] > MathZer)) {
            BndFst.Velocity = Fst->Velocity[Fst->NumPnt - 1];
            Velocity[0] = BndFst.Velocity;
        }
        if((UilWorkDim != 1) && (Fst == NULL)) {
            BndFst.Velocity = 0;
            Velocity[0] = 0;
        }
    };
};


#endif


/*
   void SetBound(Body *Fst,Body *Lst)
     {
	     int N1;
      if (Fst==NULL) {BndFst.Free=1;BndFst.Mass=0;BndFst.Pressure=ZeroPressure;}
      else 
       {
		      N1=Fst->NumPnt-1;
		      BndFst.Free=0;
		      BndFst.Mass=Fst->Mass[N1];
	      	BndFst.Pressure=Fst->Pressure[N1];
	      	BndFst.Velocity=Fst->Velocity[N1];
      		BndFst.SoundVisc=Fst->SounL[N1]*Fst->Viscouse;
       }
      if (Lst==NULL) {BndLst.Free=1;BndLst.Mass=0;BndLst.Pressure=ZeroPressure;}
      else 
       {
      		N1=1;
	      	BndLst.Free=0;
	      	BndLst.Mass=Lst->Mass[N1];
      		BndLst.Pressure=Lst->Pressure[N1];
      		BndLst.Velocity=Lst->Velocity[N1];
		      BndLst.SoundVisc=Lst->SounL[N1]*Lst->Viscouse;
	      	BndLst.VelocityPlus=Lst->Velocity[N1+1];
       }
	     int N=NumPnt;
   	  Mass[0]=BndFst.Mass;Velocity[0]=BndFst.Velocity;Pressure[0]=BndFst.Pressure;
	     Mass[N]=BndLst.Mass;Velocity[N]=BndLst.Velocity;Pressure[N]=BndLst.Pressure;
    };


	
   void BoundMU()							// M_[Num]=0;M_[0]=0;U_[0]=U_[1];U_[Num]=U_[Num-1];
    {
	    Mass[0]=BndFst.Mass;Mass[NumPnt]=BndLst.Mass;
	    if (!BndFst.Free) Velocity[0]=BndFst.Velocity;
	    else Velocity[0]=Velocity[1];
	    if (!BndLst.Free) Velocity[NumPnt]=BndLst.Velocity;
	    else Velocity[NumPnt]=Velocity[NumPnt-1];
    };
	  
   void BoundM()							// M_[Num]=0;M_[0]=0;
    {
	    Mass[0]=BndFst.Mass;Mass[NumPnt]=BndLst.Mass;
    };

   void BoundPV()							// P_[0]=10;P_[Num]=10;
    {
	    if (!BndFst.Free) Pressure[0]-=(Velocity[1]-Velocity[0])*Dencity[0]*BndFst.SoundVisc*1e5;
	    else Pressure[0]=ZeroPressure;
	    if (!BndLst.Free) Pressure[NumPnt]-=
	              	  (BndLst.VelocityPlus-Velocity[NumPnt])*Dencity[NumPnt]*BndLst.SoundVisc*1e5;
	    else Pressure[NumPnt]=ZeroPressure;
    };

   void BoundP()							// P_[0]=10;P_[Num]=10;
    {
	    Pressure[0]=BndFst.Pressure;
	    Pressure[NumPnt]=BndLst.Pressure;
    };

*/