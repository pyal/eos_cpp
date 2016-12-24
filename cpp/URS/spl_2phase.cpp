#include <lib/precompiled/eos.h>

#include "spl_2phase.h"
#include "Matt_spl.h"

// ==============================================================================
// ======================== Two_Phase Region Generation ===============================
// ==============================================================================

// dat fromat:  TR_P
void SetDatP_DT(TData<double> &dat, MatterIO *Mat, double AddT, double AddD, int GetPnt) {
    char NameBin[256];
    strcpy(NameBin, "tmp_bin.dat");
    TData<real> *DatBin = NULL;
    if(GetPnt) {
        Matt_spl::ReadData(NameBin, DatBin);
        strcpy(NameBin, "tmp_bin1.dat");
    } else {
        double IntPar;
        Matt_spl::SetIntParInit(IntPar, Mat);
        DataRegister("TColumn", DatBin);
        *DatBin = dat;
        int k, Num = DatBin->I[0];
        for(k = 0; k < Num; k++) {
            DatBin->D[0][k] -= AddT;
            DatBin->D[1][k] -= AddD;
            double curT = DatBin->D[0][k], curD = DatBin->D[1][k];
            //         double curT=DatBin->D[0][k]-AddT,curD=DatBin->D[1][k]-AddD;
            //         DatBin->D[0][k]=curD;DatBin->D[1][k]=curT;
            double curE = Mat->Energy(curD, curT);
            Matt_spl::SetIntPar(IntPar, Mat, curD, curE);
            DatBin->D[2][k] = Mat->Pressure(curD, curE);
            //cout<<" n1 "<<k<<" l "<<Num<<" ";
        }
    }
    dat = *DatBin;
    delete DatBin;
    fstream outB(NameBin, ios::out);
    DataStore(outB, &dat);
    outB.close();
};

struct BinodalPnt {
    BinodalPnt(int Num) {
        BinL = new double[Num + 1];
        BinR = new double[Num + 1];
        SpinL = new double[Num + 1];
        SpinR = new double[Num + 1];
        Temp = new double[Num + 1];
        Pres = new double[Num + 1];
        BinRE = new double[Num + 1];
        dPdT = new double[Num + 1];
        Tot = Num;
    };
    ~BinodalPnt() {
        delete BinL;
        delete BinR;
        delete SpinL;
        delete SpinR;
        delete Temp;
        delete Pres;
        delete BinRE;
        delete dPdT;
    }
    double *BinL, *BinR, *SpinL, *SpinR, *BinRE, *dPdT;
    double *Temp;
    double *Pres;
    int From, To, Tot;
    double Tmin, Tmax, Dmin, Dmax;   //,Emin,Emax;
};

//void MakeSpline(CurveSpline &spl,int Nspl,double Misf,double *x,double *y,int NumX,
void MakeSpline(
    CurveSpline &spl,
    int Nspl,
    double Misf,
    double *y,
    double *x,
    int NumX,   //y(x)
    const char *err_name,
    int DebugOut = 0) {
    if(spl.Generate(Nspl, Misf, 0, x, y, NumX)) {
        if(DebugOut) {
            Matt_spl::StdOut() << " MakeSpline Control " << err_name << "\n";
            for(int k = 0; k < NumX - 1; k++) {
                double tmp = (x[k] + x[k + 1]) / 2;
                Matt_spl::StdOut()
                    << x[k] << " " << y[k] << " " << spl.Evaluate(x[k]) << "\n"
                    << tmp << " " << (y[k] + y[k + 1]) / 2 << " " << spl.Evaluate(tmp)
                    << "\n";
            }
            Matt_spl::StdOut() << "\n" << flush;
        }
        return;
    }
    cout << " GenerateTwoPhaseBnd SaveBound error Generating " << err_name << " spline\n";
    Matt_spl::StdOut() << " MakeSpline ControlProblemData " << err_name << "\n";
    for(int k = 0; k < NumX - 1; k++)
        Matt_spl::StdOut() << x[k] << " " << y[k] << "\n";
    Matt_spl::StdOut() << "\n" << flush;

    exit(0);
}

CurveSpline IntegralSpline_Glob_P_V;
int IntegralFunc_Negative, ZeroFunc_ErrorM;
double ZeroFunc_Misf, ZeroFunc_Spl_Vmin, ZeroFunc_Spl_Vmax, ZeroFunc_SplRes_Bmin,
    ZeroFunc_SplRes_Bmax;
double IntegralFunc_Add;
MatterIO *Global_Binodal_Matter;
double Global_Binodal_Temp;
double IntegralFunc_PV_Int(double v) {
    double res = IntegralSpline_Glob_P_V.Evaluate(v);
    if(IntegralFunc_Negative)
        res = -res + IntegralFunc_Add;
    else
        res += IntegralFunc_Add;
    //cout<<v<<"   "<<res<<"\n";
    return res;
};
double ZeroFunc_PV_Int(double pres) {
    //double Qags_IntSubInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
    //                  int NumSubInt,double &ResErr,int &Err);
    //ZeroFunc_ErrorM=1;
    double Vmin, Vmax, Smin = ZeroFunc_Spl_Vmin, Smax = ZeroFunc_Spl_Vmax,
                       Ds = Smax - Smin;   //,PresInt,ResErr
    Vmin = Smin - Ds / 2;
    Vmax = Smax + Ds / 2;
    //  int Err;
    IntegralFunc_Negative = 0;
    IntegralFunc_Add = -pres;
    //cout<<"Min ==========================================================\n";
    //IntegralFunc_PV_Int(Smin-Ds);IntegralFunc_PV_Int(Smin);
    Fzero(
        IntegralFunc_PV_Int,
        Smin - Ds,
        Smin,
        Vmin,
        ZeroFunc_Misf * 0.1,
        ZeroFunc_Misf * 0.1,
        100);
    if(ZeroFunc_ErrorM)
        cout << "\n\nFor Pres " << pres << " \nMinV " << Vmin << " Misf "
             << IntegralFunc_PV_Int(Vmin) << "\n"
             << flush;
    //IntegralFunc_PV_Int(Smax);IntegralFunc_PV_Int(Smax+Ds);
    Fzero(
        IntegralFunc_PV_Int,
        Smax,
        Smax + 10000 * Ds,
        Vmax,
        ZeroFunc_Misf * 0.1,
        ZeroFunc_Misf * 0.1,
        100);
    if(ZeroFunc_ErrorM)
        cout << "MaxV " << Vmax << " Misf " << IntegralFunc_PV_Int(Vmax) << "\n" << flush;
    //  PresInt=pres*(Vmax-Vmin);
    ZeroFunc_SplRes_Bmin = Vmin;
    ZeroFunc_SplRes_Bmax = Vmax;
    //if (ZeroFunc_ErrorM)
    //cout<<" ZeroFunc_PV_Int  For Pres"<<pres<<" ResultInt  "<<Qags_IntSubInt(IntegralFunc_PV_Int,Vmin,Vmax,ZeroFunc_Misf,
    //                        ZeroFunc_Misf,100,ResErr,Err)<<"\n"<<flush;


    InterfaceFreeEIO *Mat = ((MatterFreeE *)Global_Binodal_Matter)->FreeEPtr;
    double MisPmin = fabs(Mat->Pressure(1 / Vmin, Global_Binodal_Temp) - pres),
           MisPmax = fabs(Mat->Pressure(1 / Vmax, Global_Binodal_Temp) - pres);
    //if (MisPmin>ZeroFunc_Misf)
    if(MisPmin + MisPmax > ZeroFunc_Misf)
        cout << " T " << Global_Binodal_Temp << " MisPmin " << MisPmin << " MisPmax "
             << MisPmax << "\n";

    return Mat->FreeE(1 / Vmin, Global_Binodal_Temp) -
           Mat->FreeE(1 / Vmax, Global_Binodal_Temp) + pres * (Vmin - Vmax);
    //  return Qags_IntSubInt(IntegralFunc_PV_Int,Vmin,Vmax,ZeroFunc_Misf,
    //                        ZeroFunc_Misf,100,ResErr,Err);//-PresInt;
}
void GenerateAllBnd(
    double *Pres,
    double *Denc,
    int NumD,
    double T,
    BinodalPnt &BinArr,
    int BinInd,
    int NumSplD,
    double Misf,
    int From,
    int To,
    MatterIO *Mat) {
    ZeroFunc_ErrorM = 0;
    Global_Binodal_Matter = Mat;
    Global_Binodal_Temp = T;

    BinArr.Temp[BinInd] = T;
    int k;
    double *Vol = new double[NumD];
    for(k = 0; k < NumD; k++)
        Vol[k] = 1 / Denc[k];
    MakeSpline(
        IntegralSpline_Glob_P_V,
        NumSplD,
        Misf * 0.0001,
        Pres,
        Vol,
        NumD,
        " GenerateAllBnd P_V");
    IntegralFunc_Negative = 0;
    IntegralFunc_Add = 0;
    double PT_From;
    ZeroFunc_Spl_Vmin = Fmin(
        IntegralFunc_PV_Int,
        Vol[min(To + 2, NumD - 1)],
        Vol[max<int>(0, To - 2)],
        Vol[To],
        Misf * 0.1,
        PT_From);
    //cout<<"Found Pmin: "<<PT_From<<" VolMin: "<<ZeroFunc_Spl_Vmin<<"\n"<<flush;
    BinArr.SpinR[BinInd] = 1 / ZeroFunc_Spl_Vmin;
    IntegralFunc_Negative = 1;
    //cout<<"Spline_MaximumPressure =============================================\n";
    double PT_To;
    ZeroFunc_Spl_Vmax = Fmin(
        IntegralFunc_PV_Int,
        Vol[min(From + 2, NumD - 1)],
        Vol[max<int>(0, From - 2)],
        Vol[From],
        Misf * 0.1,
        PT_To);
    PT_To = -PT_To;
    //cout<<"Found Pmax: "<<PT_To<<" VolMax: "<<ZeroFunc_Spl_Vmax<<"\n"<<flush;
    BinArr.SpinL[BinInd] = 1 / ZeroFunc_Spl_Vmax;
    IntegralFunc_Negative = 0;
    double PT_est = IntegralFunc_PV_Int((ZeroFunc_Spl_Vmin + ZeroFunc_Spl_Vmax) * 0.5);
    //  MakeSpline(IntegralSpline_Glob_P_V,NumSplD,Misf*0.01,Pres,Vol,NumD," GenerateAllBnd P_V");
    ZeroFunc_Misf = Misf * 0.1;
    Fzero(ZeroFunc_PV_Int, PT_From, PT_To, PT_est, Misf * 0.1, Misf * 0.1, 100);
    //ZeroFunc_ErrorM=1;
    ZeroFunc_PV_Int(PT_est);
    //ZeroFunc_ErrorM=0;
    BinArr.Pres[BinInd] = PT_est;
    BinArr.BinR[BinInd] = 1 / ZeroFunc_SplRes_Bmin;
    BinArr.BinL[BinInd] = 1 / ZeroFunc_SplRes_Bmax;
    double Dr = BinArr.BinR[BinInd], Dl = BinArr.BinL[BinInd];
    BinArr.BinRE[BinInd] = Mat->Energy(Dr, T);
    double E0 = BinArr.BinRE[BinInd];
    //          R0=BinArr.BinR[BinInd];//,P0=Mat->Pressure(R0,E0);
    double E1 = Mat->Energy(Dl, T);
    //  BinArr.dPdT[BinInd]=-(E0-E1)/(BinArr.BinR[BinInd]-BinArr.BinL[BinInd]);
    BinArr.dPdT[BinInd] =
        -(E0 - E1) / (1 / BinArr.BinR[BinInd] - 1 / BinArr.BinL[BinInd]);
}
void FindSpinEst(
    double *Pres,
    double *Denc,
    int NumD,
    double T,
    BinodalPnt &BinArr,
    int BinInd,
    int &Good,
    int NumSplD,
    double Misf,
    MatterIO *Mat) {
    int k, FoundReg = 0, Cont = 0, From, To;
    Good = 0;
    for(k = 1; k < NumD; k++) {
        //cout<<k<<" "<<Pres[k]<<" "<<Denc[k]<<"\n";
        if(Pres[k] < Pres[k - 1]) {
            if(!FoundReg) {
                FoundReg = 1;
                BinArr.SpinL[BinInd] = Denc[k - 1];
                From = k - 1;
            }
            //       else if (!Cont)
            //        {cout<<" Error in GenerateTwoPhaseBnd FindSpinEst two phases for one density found.";
            //       cout<<"Exiting.";FoundReg=0;break;}//exit(0);}
            Cont = 1;
        } else {
            if((FoundReg) && (Cont)) {
                BinArr.SpinR[BinInd] = Denc[k];
                To = k;
            };
            Cont = 0;
        }
    }
    //cout.flush();
    //  if ( (FoundReg) && (Cont) )
    //    {cout<<" Error in GenerateTwoPhaseBnd FindSpinEst was not found Right boundary density.";
    //     cout<<"Exiting.";for (int k1=0;k1<NumD;k1++) cout<<Denc[k1]<<" "<<Pres[k1]<<"\n";
    //     cout<<" From k "<<From<<" To K "<<To<<"\n";FoundReg=0;}//exit(0);}
    if(!FoundReg) {
        Good = 0;
        return;
    }
    Good = 1;
    GenerateAllBnd(Pres, Denc, NumD, T, BinArr, BinInd, NumSplD, Misf, From, To, Mat);
};
int FormBinodalPnt(
    TData<double> &dat,
    BinodalPnt &BinArr,
    MatterIO *Mat,
    int NumSplD,
    double Misf) {
    int Tind = 0, Dind = 1;   //,BinInd=1;
    int k, Cur = 0, NumT = dat.I[Tind], NumD = dat.I[Dind], FoundReg = 0, Cont, Good = 0;
    for(k = 1; k <= NumT; k++) {
        //TColumn<double> tmp_dat;tmp_dat=dat;DataStore(StdOut,&tmp_dat);StdOut.flush();
        FindSpinEst(
            &dat.D[2][Cur],
            dat.D[1],
            NumD,
            dat.D[Tind][k - 1],
            BinArr,
            k,
            Cont,
            NumSplD,
            Misf,
            Mat);
        if((FoundReg) && (!Cont)) {
            BinArr.To = k - 1;
            Good = 1;
            break;
        }
        if((Cont) && (!FoundReg)) {
            BinArr.From = k;
            FoundReg = 1;
        }
        Cur += NumD;
    }
    if(!Good) {
        cout << "GenerateTwoPhaseBnd FormBinodalPnt warning ";
        if(!FoundReg) {
            cout << " Two Pahse region not found. Exiting.";
            return 0;
        }   //exit(0);}
        cout << " Was not found Two-Phase Upper Boundary.\n";
        BinArr.To = NumT;
    }
    BinArr.Tot = BinArr.To - BinArr.From + 1;
    return 1;
}

void SetMinMaxBinVal(BinodalPnt &BinArr) {
    int I1 = BinArr.To, I0 = BinArr.From;
    double Tmin = BinArr.Temp[I0], Tmax = BinArr.Temp[I1];
    double Dmin = BinArr.BinL[I0], Dmax = BinArr.BinR[I0];
    //  double Emin=BinArr.BinRE[I0],Emax=BinArr.BinRE[I1];
    BinArr.Tmax = Tmax;
    BinArr.Tmin = Tmin;
    BinArr.Dmax = Dmax;
    BinArr.Dmin = Dmin;
    //  BinArr.Emax=Emax;BinArr.Emin=Emin;
}
//struct TwoPhaseBoundary:InterfaceIO
// Binodal Spline
//   CurveSpline dPdT_T          ,E_T,Dr_T,Dl_T,P_T;
//   double Tmin,Tmax,Dmin,Dmax;
//   CurveSpline                  BinLftD_E,BinRgtD_E,SpinLftD_E,SpinRgtD_E;
//   double Emin,Emax;

void SaveBound(char *res_name, BinodalPnt &BinArr, int &NumSplT, double Misf) {
    MatterStable::TwoPhaseBoundary Bnd;

    //int CurveSpline::Generate(int &NumX,double &Misf,int Cont,TData<real> &dat,
    //              int Xcol,int Ycol,int Wcol)
    int PntSpl = NumSplT, Beg = BinArr.From, Num = BinArr.To - BinArr.From + 1;

    //  MakeSpline(Bnd.P_T,PntSpl,Misf,&BinArr.Temp[Beg],&BinArr.Pres[Beg],Num,"P_T",1);
    //  MakeSpline(Bnd.Dr_T,PntSpl,Misf,&BinArr.Temp[Beg],&BinArr.BinR[Beg],Num,"Dr_T",1);
    //  MakeSpline(Bnd.Dl_T,PntSpl,Misf,&BinArr.Temp[Beg],&BinArr.BinL[Beg],Num,"Dl_T",1);
    MakeSpline(
        Bnd.P_T,
        PntSpl,
        Misf * 0.0001,
        &BinArr.Pres[Beg],
        &BinArr.Temp[Beg],
        Num,
        "P_T",
        1);
    MakeSpline(
        Bnd.Dr_T, PntSpl, Misf, &BinArr.BinR[Beg], &BinArr.Temp[Beg], Num, "Dr_T", 1);
    MakeSpline(
        Bnd.Dl_T, PntSpl, Misf, &BinArr.BinL[Beg], &BinArr.Temp[Beg], Num, "Dl_T", 1);
    MakeSpline(
        Bnd.E_T, PntSpl, Misf, &BinArr.BinRE[Beg], &BinArr.Temp[Beg], Num, "E_T", 1);
    MakeSpline(
        Bnd.dPdT_T,
        PntSpl,
        Misf * 1e-6,
        &BinArr.dPdT[Beg],
        &BinArr.Temp[Beg],
        Num,
        "dPdT_T",
        1);

    MakeSpline(
        Bnd.DSl_T, PntSpl, Misf, &BinArr.SpinL[Beg], &BinArr.Temp[Beg], Num, "DSl_T", 1);
    MakeSpline(
        Bnd.DSr_T, PntSpl, Misf, &BinArr.SpinR[Beg], &BinArr.Temp[Beg], Num, "DSr_T", 1);

    //  MakeSpline(Bnd.BinLftD_E,PntSpl,Misf,&BinArr.BinL[Beg],&BinArr.BinRE[Beg],Num,"BinLftD_E",1);
    //  MakeSpline(Bnd.BinRgtD_E,PntSpl,Misf,&BinArr.BinR[Beg],&BinArr.BinRE[Beg],Num,"BinRgtD_E",1);
    //  MakeSpline(Bnd.SpinLftD_E,PntSpl,Misf,&BinArr.SpinL[Beg],&BinArr.BinRE[Beg],Num,"SpinLftD_E",1);
    //  MakeSpline(Bnd.SpinRgtD_E,PntSpl,Misf,&BinArr.SpinR[Beg],&BinArr.BinRE[Beg],Num,"SpinRgtD_E",1);
    SetMinMaxBinVal(BinArr);
    Bnd.Tmax = BinArr.Tmax;
    Bnd.Tmin = BinArr.Tmin;   //Bnd.Emax=BinArr.Emax;Bnd.Emin=BinArr.Emin;
    Bnd.Dmax = BinArr.Dmax;
    Bnd.Dmin = BinArr.Dmin;
    FilterTextOut out(res_name);
    Bnd.save_data_state(out);
}
int GenerateTwoPhaseBnd(istream &row, MatterIO *Mat) {
    Matt_spl::StdOut().open("inter_dat", ios::out);
    char tmp[256], res_name[256];
    TTable<double> dat;
    double AddT, AddD, Misf;
    int GetPnt, NumSplD, NumSplT;
    row >> tmp >> res_name >> tmp >> NumSplD >> tmp >> NumSplT >> tmp >> Misf >> tmp >>
        AddT >> tmp >> AddD >> tmp >> GetPnt;
    if(!ReadTableRow(row, (TData<real> *)&dat)) {
        cout << " Could not read param.";
        exit(0);
    }

    cout << " Entered Cfg for GenerateTwoPhaseBnd\n";   //ChRead();
    SetDatP_DT(dat, Mat, AddT, AddD, GetPnt);
    // First - Temperature, Second - Dencity
    int Tind = 0,
        //           Dind=1,
        NumT = dat.I[Tind];   //,NumD=dat.I[Dind];
    BinodalPnt BinArr(NumT + 1);
    if(FormBinodalPnt(dat, BinArr, Mat, NumSplD, Misf))
        SaveBound(res_name, BinArr, NumSplT, Misf);
    //   delete BinArr;
    Matt_spl::StdOut().close();
    return 1;
};

double AddT_Static, AddD_Static, Misf_Static, dPdT_Static, FoundC_Static, FoundA_Static,
    Dmelt_Static;
int NumSplD_Static;
TTable<double> *dat_Static;
MatterFreeE *Mat_Static;
double dPdT_Difference(double Anew) {
    FreeEBushman *Bus = (FreeEBushman *)Mat_Static->FreeEPtr.pointer();
    //   OneVarFunction *ColdComp,*ColdRelease,*TetaS,   *Be;
    //   TwoVarFunction *Cv_VT,*TetaL,*FreeM,            *Ge_VT,*CvE_VT;
    FreeM_SigmaMT *Melt = dynamic_cast<FreeM_SigmaMT *>((TwoVarFunction *)Bus->FreeM);
    //   FreeM_SigmaMT *Melt=(FreeM_SigmaMT *)Bus->FreeM;
    Melt->Am = 0;
    Melt->Bm = 0;
    Melt->Cm = 0;
    double Dmelt = Bus->Denc0m, Tmelt = Melt->Temp_m0 * 1e3;
    Melt->Am = 0;
    Melt->Bm = 0;
    Melt->Cm = 0;
    double Emelt = Mat_Static->Energy(Dmelt, Tmelt);
    double DeltaP = Mat_Static->Pressure(Dmelt, Emelt) - 1e-4;
    //DeltaP=0;
    //   double Cnew=2./3*(-DeltaP/(Bus->Rconst*Tmelt*1e-3*Dmelt)-2*Anew);
    double Cnew = 2. / 3 * (-DeltaP / (Bus->Rconst * Tmelt * 1e-3 * Dmelt) -
                            6. / 5 * Anew * Melt->PowAm);
    FoundC_Static = Cnew;

    Melt->Am = Anew;
    Melt->Bm = Cnew;
    Melt->Cm = Cnew;
    //cout<<" Pres=1e-4 "<<Mat_Static->Pressure(Dmelt,Mat_Static->Energy(Dmelt,Tmelt)) <<" A "<<Anew<<" C "<<Cnew<<"\n";

    SetDatP_DT(*dat_Static, Mat_Static, AddT_Static, AddD_Static, 0);
    BinodalPnt BinArr(2);
    FormBinodalPnt(*dat_Static, BinArr, Mat_Static, NumSplD_Static, Misf_Static);
    Dmelt_Static = BinArr.BinR[1];
    BinArr.dPdT[1] = (BinArr.Pres[1] - BinArr.dPdT[1]) / BinArr.Temp[1];
    cout << " AIter " << Anew << " dPdT got " << BinArr.dPdT[1] << " RightBnd "
         << BinArr.BinR[1] << "\n";

    TTable<double> dat1 = *dat_Static;
    Tmelt = dat1.D[0][0];
    double dT = 0.01 * Tmelt;
    for(int i = 0; i < dat1.I[0]; i++)
        dat1.D[0][i] = Tmelt + dT;
    SetDatP_DT(dat1, Mat_Static, AddT_Static, AddD_Static, 0);
    BinodalPnt BAtmp(2);
    FormBinodalPnt(dat1, BAtmp, Mat_Static, NumSplD_Static, Misf_Static);
    cout << " Direct Clc dPdT " << (BAtmp.Pres[1] - BinArr.Pres[1]) / dT << "\n";


    return BinArr.dPdT[1] - dPdT_Static;
}
double DmeltDifference(double NewD) {
    FreeEBushman *Bus = (FreeEBushman *)Mat_Static->FreeEPtr.pointer();
    FreeM_SigmaMT *Melt =
        dynamic_cast<FreeM_SigmaMT *>((TwoVarFunction *)Bus->FreeM.pointer());
    //   FreeM_SigmaMT *Melt=(FreeM_SigmaMT *)Bus->FreeM;
    Bus->Denc0m = NewD;
    double A = Melt->Am;
    Fzero(dPdT_Difference, -1, 1, A, Misf_Static * 0.1, Misf_Static * 0.1, 100);
    FoundA_Static = A;
    double ret = Dmelt_Static - NewD;
    return ret;
}

void FindBushCoefABC(
    istream &row,
    MatterIO *Mat_In,
    double dPdT,
    double SeekFrom,
    double SeekTo,
    double StpShow) {
    MatterFreeE *Mat = (MatterFreeE *)Mat_In;
    char tmp[256], res_name[256];
    double AddT, AddD, Misf;
    int GetPnt, NumSplD, NumSplT;
    TTable<double> dat;
    Mat_Static = Mat;
    row >> tmp >> res_name >> tmp >> NumSplD >> tmp >> NumSplT >> tmp >> Misf >> tmp >>
        AddT >> tmp >> AddD >> tmp >> GetPnt;
    if(!ReadTableRow(row, (TData<real> *)&dat)) {
        cout << " Could not read param.";
        exit(0);
    }
    double Tmelt =
        (dynamic_cast<FreeM_SigmaMT *>(
             (TwoVarFunction *)((FreeEBushman *)Mat_Static->FreeEPtr.pointer())->FreeM))
            ->Temp_m0 *
        1e3;
    //   double Tmelt=( (FreeM_SigmaMT *)((FreeEBushman *)Mat_Static->FreeEPtr)->FreeM )->Temp_m0*1e3;
    //   double Dcold=( (FreeEBushman *)Mat_Static->FreeEPtr.pointer() )->Denc0;
    double Dmelt = ((FreeEBushman *)Mat_Static->FreeEPtr.pointer())->Denc0m;
    double Emelt = Mat_Static->Energy(Dmelt, Tmelt);
    //   dat.SetColumn(&Tmelt,1,0);
    cout << " FindBushCoefABC Pres " << Mat_Static->Pressure(Dmelt, Emelt) << "\n";

    dat.I[0] = 1;
    dat.D[0][0] = Tmelt;
    dat.I[2] = dat.I[1];
    SetDatP_DT(dat, Mat, AddT, AddD, 0);
    AddT_Static = AddT;
    AddD_Static = AddD;
    Misf_Static = Misf;
    NumSplD_Static = NumSplD;
    dat_Static = &dat;
    dPdT_Static = dPdT;

    double A =
        (dynamic_cast<FreeM_SigmaMT *>(
             (TwoVarFunction *)((FreeEBushman *)Mat_Static->FreeEPtr.pointer())->FreeM))
            ->Am;
    //   double A=( (FreeM_SigmaMT *)((FreeEBushman *)Mat_Static->FreeEPtr)->FreeM )->Am;
    A = (SeekFrom + SeekTo) / 2;
    Fzero(
        dPdT_Difference, SeekFrom, SeekTo, A, Misf_Static * 0.1, Misf_Static * 0.1, 100);
    FoundA_Static = A;
    fstream Ftmp(res_name, ios::out);
    for(double cur = SeekFrom; cur <= SeekTo; cur += StpShow)
        Ftmp << cur << FMT << dPdT_Difference(cur) + dPdT_Static << "\n";
    //dPdT_Difference(-1.42);
    //for (int i=0;i<dat.I[1];i++) Ftmp<<dat.D[1][i]<<" "<<dat.D[2][i]<<"\n";
    //dPdT_Difference(-1.44);
    //for (i=0;i<dat.I[1];i++) Ftmp<<dat.D[1][i]<<" "<<dat.D[2][i]<<"\n";
    //   Fzero(DmeltDifference,1e-5,Dcold*1.2,Dmelt,Misf*0.1,Misf*0.1,100);
    double ErrD = 0;   //DmeltDifference(Dmelt);
    double ErrP = dPdT_Difference(FoundA_Static);
    cout << " Found A " << FoundA_Static << " B " << 0 << " C " << FoundC_Static
         << " Denc_melt " << Dmelt_Static << " ErrP " << ErrP << " ErrD " << ErrD << "\n";
};


// ==============================================================================
// ======================== Two_Phase Region Generation ===============================
// ==============================================================================

// ==================================================================================
// ======================== Sol_Liq Region Generation ===============================
// ========================  Looks like bad old variant =============================
// ==================================================================================

//struct LiquidSolidBnd:InterfaceIO
//CurveSpline Eliq_D,Esol_D,Pliq_D,Psol_D,T_D;
struct LiquidSolidBndDat {
    LiquidSolidBndDat() {
        Tot = 0;
    }
    LiquidSolidBndDat(int Num) {
        GenerateStruct(Num);
    }
    void GenerateStruct(int Num) {
        T = new double[Num + 1];
        Esol = new double[Num + 1];
        Eliq = new double[Num + 1];
        Psol = new double[Num + 1];
        Pliq = new double[Num + 1];
        D = new double[Num + 1];
        Tot = Num;
    };
    ~LiquidSolidBndDat() {
        DeleteStruct();
    }
    void DeleteStruct() {
        if(Tot > 0) {
            delete T;
            delete Esol;
            delete Eliq;
            delete Pliq;
            delete Psol;
            delete D;
            T = NULL;
            Esol = NULL;
            Eliq = NULL;
            Pliq = NULL;
            Psol = NULL;
            D = NULL;
            Tot = 0;
        }
    };
    void SetDim(int Num) {
        DeleteStruct();
        GenerateStruct(Num);
    };
    double *T, *Esol, *Eliq, *Pliq, *Psol, *D;
    int Tot;
    double Dmin, Dmax;   //,Emin,Emax;
};

double FreeCurDencity_Static;
InterfaceFreeEIO *FreeSol_Static, *FreeLiq_Static;

double DeltaFreeEnergy_Static(double Temp) {
    return FreeSol_Static->FreeE(FreeCurDencity_Static, Temp) -
           FreeLiq_Static->FreeE(FreeCurDencity_Static, Temp);
};

int FormSolLiqBound(
    TData<double> &dat,
    LiquidSolidBndDat &LiqSolBnd,
    MatterIO *MatSol,
    MatterIO *MatLiq,
    double AddT,
    double AddD,
    int GetPnt,
    double Misf) {
    FreeSol_Static = ((MatterFreeE *)MatSol)->FreeEPtr;
    FreeLiq_Static = ((MatterFreeE *)MatLiq)->FreeEPtr;
    // First - Temperature, Second - Dencity
    int Tind = 0, Dind = 1, NumT = dat.I[Tind], NumD = dat.I[Dind];
    LiqSolBnd.SetDim(NumD);
    double Tlow = dat.D[Tind][0], Tup = dat.D[Tind][NumT - 1], ResT;
    ResT = Tlow;
    for(int k = 0; k < NumD; k++) {
        double CurD = dat.D[Dind][k];
        FreeCurDencity_Static = CurD;
        if(3 <
           Fzero(DeltaFreeEnergy_Static, Tlow, Tup, ResT, Misf * 0.1, Misf * 0.1, 500)) {
            cout << " Error, could not find FormSolLiqBound ZeroFzeroIt\n";
        }
        LiqSolBnd.Esol[k] = MatSol->Energy(CurD, ResT);
        LiqSolBnd.Eliq[k] = MatLiq->Energy(CurD, ResT);
        LiqSolBnd.Psol[k] = MatSol->Pressure(CurD, LiqSolBnd.Esol[k]);
        LiqSolBnd.Pliq[k] = MatLiq->Pressure(CurD, LiqSolBnd.Eliq[k]);
        LiqSolBnd.D[k] = CurD;
        LiqSolBnd.T[k] = ResT;
    }
    LiqSolBnd.Dmin = dat.D[Dind][0];
    LiqSolBnd.Dmax = dat.D[Dind][NumD - 1];
    return 1;
};

//void MakeSpline(CurveSpline &spl,int Nspl,double Misf,double *y,double *x,int NumX,//y(x)
//                char *err_name, int DebugOut=0)

void SaveSolLiqBound(
    char *res_name,
    LiquidSolidBndDat &LiqSolBnd,
    int PntSpl,
    double Misf) {
    MatterLiqSol::LiquidSolidBnd Bnd;
    int Num = LiqSolBnd.Tot;
    MakeSpline(
        Bnd.Eliq_D, PntSpl, Misf, &LiqSolBnd.Eliq[0], &LiqSolBnd.D[0], Num, "Eliq_D", 1);
    MakeSpline(
        Bnd.Esol_D, PntSpl, Misf, &LiqSolBnd.Esol[0], &LiqSolBnd.D[0], Num, "Esol_D", 1);
    MakeSpline(
        Bnd.Pliq_D, PntSpl, Misf, &LiqSolBnd.Pliq[0], &LiqSolBnd.D[0], Num, "Pliq_D", 1);
    MakeSpline(
        Bnd.Psol_D, PntSpl, Misf, &LiqSolBnd.Psol[0], &LiqSolBnd.D[0], Num, "Psol_D", 1);
    MakeSpline(Bnd.T_D, PntSpl, Misf, &LiqSolBnd.T[0], &LiqSolBnd.D[0], Num, "T_D", 1);
    Bnd.Dmax = LiqSolBnd.Dmax;
    Bnd.Dmin = LiqSolBnd.Dmin;
    //  fstream out(res_name,ios::out);Bnd.SaveIni(out);out.close();
    FilterTextOut out(res_name);
    Bnd.save_data_state(out);
};

int GenerateSolLiqPhaseBnd(istream &row, MatterIO *MatSol) {
    Matt_spl::StdOut().open("inter_sol_liq", ios::out);
    Ref<MatterIO> MatLiq;
    char tmp[256], res_name[256];
    TTable<double> dat;
    double AddT, AddD, Misf;
    int GetPnt, NumSplD, NumSplT;

    if(!(!(MatLiq << SavableClass::Read(row)))) {
        cout << "Could not register MatLiq. Exit.\n";
        exit(0);
    }
    row >> tmp >> res_name >> tmp >> NumSplD >> tmp >> NumSplT >> tmp >> Misf >> tmp >>
        AddT >> tmp >> AddD >> tmp >> GetPnt;
    if(!ReadTableRow(row, (TData<real> *)&dat)) {
        cout << " Could not read param.";
        exit(0);
    }

    cout << " Entered Cfg for GenerateSolLiqPhaseBnd\n";   //ChRead();
    LiquidSolidBndDat LiqSolBnd;
    if(FormSolLiqBound(dat, LiqSolBnd, MatSol, MatLiq, AddT, AddD, GetPnt, Misf))
        SaveSolLiqBound(res_name, LiqSolBnd, NumSplD, Misf);
    Matt_spl::StdOut().close();
    return 1;
};

// ==================================================================================
// ======================== Sol_Liq Region Generation   =============================
// ========================  Looks like bad old variant =============================
// ========================           END               =============================
