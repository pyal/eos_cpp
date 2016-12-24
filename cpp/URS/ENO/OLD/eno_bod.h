#ifndef __ENO_BOD_H
#define __ENO_BOD_H


#include "matt_reg.h"
#include <malloc.h>


#define MaxNumMatter 10
#define ZeroPressure 10

struct BndStruct {
    double Mass, Pressure, Velocity, Pos;   //,SoundVisc,VelocityPlus;
    int Free;
    BndStruct() {
        Free = 1;
        Mass = 0;
        Pressure = ZeroPressure;
    };
};

#define MaxPolynomOrder 6
struct Poli3CoefStruct {
    double a[MaxPolynomOrder], pos[MaxPolynomOrder];
    int k[MaxPolynomOrder];
};

struct Body {
    int NumPnt, NumIntPar;
    double Viscouse, Viscouse2;
    double *Pressure, *Energy, *Dencity, *Velocity, *Pos;   //,*Volume;
    double *PresL, *SounL;
    double *TmpP, *TmpD, *TmpE;
    double **IntPar;
    Poli3CoefStruct *Poli3Coef;
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
    }
    Body(int NPnt, int NIntPar, MatterIO *Mat, double Vis, double Vis2) {
        Matter = NULL;
        NumPnt = 0;
        IterCalcS = 0;
        NumSkipS = 10;
        Create(NPnt, NIntPar, Mat, Vis, Vis2);
    }
    void DeleteBody();
    void Create(int NPnt, int NIntPar, MatterIO *Mat, double Vis, double Vis2);
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
