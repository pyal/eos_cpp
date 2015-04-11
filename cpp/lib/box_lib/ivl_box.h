
#pragma once

#include "box_face.h"

#include "ivl_data.h"









struct MatterConverterSplIO : IBlackBoxBase {
    MatterConverterSplIO();
    //    : BlackBoxBase()
    //    , MisfitFindE(1e-6)
    //    , NumEGridPnt(100)
    //    , NumVGridPnt(100)
    //    , EOSspl("ivl_cvt.spl")
    //    , IvlParam(new TIvlParametersSplIO)
    //    , Pspl(new ExtendedRegridSpline)
    //    , Tspl(new ExtendedRegridSpline) {
    //}
    int save_data_state( FilterOut&so) {
        so << " IvlParam " << IvlParam << SavableClass::EOLN() <<
              " Pspl "; SavableClass::IncIdent(); Pspl->save_generation_par(so); so << SavableClass::mmEOLN() <<
              " Tspl "; SavableClass::IncIdent(); Tspl->save_generation_par(so); so << SavableClass::mmEOLN() <<
              " MisfitFindE " << MisfitFindE << " NumEGridPnt " << NumEGridPnt << " NumVGridPnt " << NumVGridPnt << 
              " EOSspl " << EOSspl;
        return 1; 
    };
    int read_data_state(FilterIn&si) { 
        Stroka tmp;
        Pspl = new ExtendedRegridSpline;
        Tspl = new ExtendedRegridSpline;
        SavableClass *o1;
        si >> tmp >> o1 >> tmp; Pspl->read_generation_par(si); si >> tmp; Tspl->read_generation_par(si);
        si >> tmp >> MisfitFindE >> tmp >> NumEGridPnt >> tmp >> NumVGridPnt;
        si >> tmp >> EOSspl;
        IvlParam << o1;
        return 1;
    };
    Stroka MakeHelp(){
        Stroka ret = "From ivl data files: v,t + p_tv e_tv - construct eos p_re t_re\n";
        ret += " Uses 1d splines to find t_e p_t for given v, 2d-splines to construct eos\n";
        ret += "Splines present in the system:\n" + SavableClass::HelpForCategory("SplineGenerator_category");
        return ret;
    }
    void DoIt() {
        IvlParam->CreateSplines();
        TColumn<double> pDat, tDat;
        MakeTable(pDat, Pspl);
        GenerateP(pDat);
        MakeTable(tDat, Tspl);
        GenerateT(tDat);
        try {
            Pspl->Generate(1, pDat, "error generating P");
            Tspl->Generate(1, tDat, "error generating T");
        } catch (stdexception &ex) {
            IIvlParametersBase::MakeDebugOutput(EOSspl + ".pgrid", pDat);
            IIvlParametersBase::MakeDebugOutput(EOSspl + ".tgrid", tDat);
            throw info_except("Exception caught:\n%s\n", ex.what());
        }
            TData<double> psDat = pDat;
            TData<double> tsDat = tDat;
            Pspl->Evaluate(psDat);
            Tspl->Evaluate(tsDat);
            pDat.AddColumn(psDat.D[2], psDat.I[2]);
            tDat.AddColumn(tsDat.D[2], tsDat.I[2]);

            IIvlParametersBase::MakeDebugOutput(EOSspl + ".er_ps", pDat);
            IIvlParametersBase::MakeDebugOutput(EOSspl + ".er_ts", tDat);

        SaveSpline();
    }
private:
    static MatterConverterSplIO *CurPtr;
    double SearchVol, SearchE;
    double LastT;

    Ref<IIvlParametersBase> IvlParam;
    Ref<ExtendedRegridSpline> Pspl, Tspl;
    double MisfitFindE;
    int NumEGridPnt, NumVGridPnt;
    Stroka EOSspl;

    void SaveSpline() {
        MatterCaloricSpl save(EOSspl, 1 / IvlParam->MaxVol, 1 / IvlParam->MinVol, Pspl, Tspl);
        save.SaveSplFile();
    }
    void MakeTable(TColumn<double> &dat, Ref<ExtendedRegridSpline> &spl) {
        TTable<double> tmp;
        spl->SetGrid(NumEGridPnt, IvlParam->MinE, IvlParam->MaxE, NumVGridPnt, 1 / IvlParam->MaxVol, 1 / IvlParam->MinVol, tmp);
        dat = tmp;
    }
    void GenerateP(TColumn<double> &dat) {
        for(int i = 0; i < dat.I[0]; i++) {
            dat.D[2][i] = FindPres(dat.D[0][i], dat.D[1][i], 1);
        }
    }
    void GenerateT(TColumn<double> &dat) {
        for(int i = 0; i < dat.I[0]; i++) {
            dat.D[2][i] = FindPres(dat.D[0][i], dat.D[1][i], 0);
        }
    }
    double FindPres(double e, double r, int getPres) {
        SearchVol = 1 / r;
        SearchE = e;
        if (!In_Lim(LastT, IvlParam->MinT, IvlParam->MaxT))
            LastT = (IvlParam->MinT + IvlParam->MaxT) * 0.5;
        MatterConverterSplIO *oldPtr = CurPtr;
        CurPtr = this;
        double ErrorRel = MisfitFindE, ErrorAbs = ErrorRel;
        int MaxIter = 1000, err;
        if (3<(err = Fzero(EnergyMisfitStatic, IvlParam->MinT, IvlParam->MaxT, LastT, ErrorAbs, ErrorRel, MaxIter)) && err != 4 ){
            if ( fabs(EnergyMisfitStatic(LastT)) > 10 * MisfitFindE ) {
		        cout<<" Bad FZero FindPres in MatterSplConverter \nfound Temp "
			        <<LastT<<" Mis "<<EnergyMisfitStatic(LastT)<<" err "<<err<<"\n";
VecCl tT(100), tV(100), tE(100);tT = VecCl::MakeStepVector(100, IvlParam->MinT, IvlParam->MaxT, 0);tV = tV * 0 + SearchVol;
IvlParam->Energy(&tT[1], &tV[1], &tE[1], 100);
cout<<tT<<"\n"<<tV<<"\n"<<tE<<"\n";
            }
        }
        double res = LastT;
        if (getPres)
            res = IvlParam->Pressure(LastT, SearchVol);
        return res;
        //SplP_VT, SplE_VT
    }
    static double EnergyMisfitStatic(double temp) {
        double en = CurPtr->IvlParam->Energy(temp, CurPtr->SearchVol);
        return CurPtr->SearchE - en;
    }
};















