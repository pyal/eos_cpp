#pragma once

#include "lib/std/deffunc.h"
//#include "lib/std/interfac.h"
#include "lib/std/const.h"
#include "matt_fac.h"
#include "fre_fac.h"
//#include "matt_reg.h"

#include "mat/dat_fac.h"
#include "mat/dat_reg.h"
#include "mat/spl_fac.h"
#include "fre_bus.h"

//#include "h2_liqui.h"
//#include "matt_adi.h"

struct MatterFreeSpl : FreeEIO {
    Stroka spl_name;
    Ref<SplineXYIO> F_rt;
    MatterFreeSpl();   //: FreeEIO(), spl_name("FreeE.spl"), F_rt(NULL){};
    void SetTLimits(double &minT, double &maxT, double &stpTcoef, double &stpTmin) {
        if(!F_rt)
            throw info_except("No spline now? Setting limits!!!\n");
        double Dl, Du, misf = F_rt->GetMisfit();
        F_rt->GetBoundary(minT, maxT, Dl, Du);
        stpTcoef = min(sqrt(misf), 0.1);
        stpTmin = 0.1 * minT;
    }

    double FreeE(double Denc, double Temp) {
        double res, T = Temp < StndErr ? StndErr : Temp,
                    D = Denc < StndErr ? StndErr : Denc;
        if(!F_rt)
            throw info_except("No spline now? T %f D %f...\n", Temp, Denc);
        //		int er=F_rt->Evaluate(&D,&T,&res,1);
        double Tl, Tu, Dl, Du;
        F_rt->GetBoundary(Tl, Tu, Dl, Du);
        double DerCoef = 1e-4;
        Tl *= (1 + DerCoef);
        Tu *= (1 - DerCoef);
        if(Temp > Tu || Temp < Tl) {
            double FstT, SecT;
            if(Temp < Tl) {
                FstT = Tl;
                SecT = Tl * (1 + DerCoef);
            } else {
                FstT = Tu * (1 - DerCoef);
                SecT = Tu;
            }
            double FstF, SecF;
            F_rt->Evaluate(&FstT, &D, &FstF, 1);
            F_rt->Evaluate(&SecT, &D, &SecF, 1);
            double ret = FstF + (SecF - FstF) / (SecT - FstT) * (Temp - FstT);
            //cout<<" Tl "<<Tl<<" Tu "<<Tu<<" T "<<Temp<<" FstF "<<FstF<<" FstT "<<FstT<<" SecF "<<SecF<<" SecT "<<SecT<<" ret "<<ret<<"\n";
            return ret;
        }
        int er = F_rt->Evaluate(&T, &D, &res, 1);
        if(er != 0) {
            res = 1;
            cout << " Very Bad!!\n";
        }
        return res;
    }
    virtual int read_data_state(FilterIn &input) {
        input >> spl_name;
        FilterTextIn in(spl_name.c_str());
        char tmp[256];
        in >> tmp;
        while(Stricmp(tmp, "GeneratedSpline") != 0 && in)
            in >> tmp;
        if(!in)
            throw info_except("Wrong spline format. Spline file: %s\n", spl_name.c_str());
        in.SetRefMask(SavableClass::SimpleEdit);
        //in.SetRefMask(SavableClass::UnlinkedObjects);
        in >> F_rt;
        if(!in || !F_rt)
            throw info_except("Could not read spline???\n");
        return 1;
    }
    virtual int save_data_state(FilterOut &output) {
        output << spl_name;
        return 1;
    }
};
