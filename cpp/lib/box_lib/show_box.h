
#pragma once


#include "box_face.h"


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
    }
};

