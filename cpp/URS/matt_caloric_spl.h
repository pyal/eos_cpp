#pragma once


#include "lib\std\deffunc.h"
#include "lib\std\const.h"
#include "matt_fac.h"
//#include "fre_fac.h"

#include "mat\dat_fac.h"
#include "mat\dat_reg.h"
#include "mat\spl_fac.h"
//#include "fre_bus.h"



struct MatterCaloricSpl : MatterIO {
    Stroka CaloricSplineName;
    Ref<SplineXYIO> PresSpl,TempSpl;
    double DencCold, DencMax;

    MatterCaloricSpl()
        : MatterIO()
        , LastE(50)
        , CaloricSplineName("h2e_ros1.spl")
        , DencCold(1e-10)
        , DencMax(100) {
        SoundDencityCoef = 50;
    }
    MatterCaloricSpl(const Stroka &name, double dencCold, double dencMax, SplineXYIO *presSpl, SplineXYIO *tempSpl) 
        : MatterIO()
        , LastE(50)
        , CaloricSplineName(name)
        , PresSpl(presSpl)
        , TempSpl(tempSpl)
        , DencCold(dencCold)
        , DencMax(dencMax) {
        SoundDencityCoef = 50;
    }

    virtual double DencityCold(){return DencCold;}
    virtual double DencityMax(){return DencMax;}
    double Pressure(double dencity,double energy) { 
        double res;
        int er = PresSpl->Evaluate(&energy, &dencity, &res, 1);
        if (er!=0) {
            res=1;
            cout<<" Very bad pressure for dencity "<<dencity<<" energy "<<energy<<"\n";
        }
        return res;
    }
      
   double Temperature(double dencity,double energy) { 
        double res;
        if (!TempSpl) 
            return 1;
        int er = TempSpl->Evaluate(&energy, &dencity, &res, 1);
        if (er!=0) {
            res=1;
            cout<<" Very bad tempreature for dencity "<<dencity<<" energy "<<energy<<"\n";
        }
        return res;
    }
    double St_Denc, St_Temp, LastE;
    static MatterCaloricSpl* Current;
    static double MisE(double e) { 
        return log(fabs(Current->Temperature(Current->St_Denc, e) / Current->St_Temp));
        //return Current->Temperature(Current->St_Denc, e) - Current->St_Temp;
    }
    double Energy(double dencity,double temperature) {
        St_Denc = dencity;
        St_Temp = temperature;
        MatterCaloricSpl *Old = Current;
        Current=this;

        double minE, maxE, minD, maxD;
        TempSpl->GetBoundary(minE, maxE, minD, maxD);
        if (!In_Lim(minE, maxE, LastE))
            LastE = (minE + maxE) * 0.5;
        if (!Fzero(MisE, minE, maxE, LastE, M_Eps2*10, M_Eps2*10, 1000)) {
            cout<<" Bad Fzero    in MatterCaloricSpl::Energy Not Zero\nfound E "
                <<LastE<<" Mis "<<MisE(LastE)<<"\n";
        }
//VecCl tT(100), tD(100), tE(100), tP(100);tE = VecCl::MakeStepVector(100, minE, maxE, 1);tD = tD * 0 + dencity;
//Temperature(&tT[1], &tD[1], &tE[1], 100);
//Pressure(&tP[1], &tD[1], &tE[1], 100);
//cout<<tD<<"\n"<<tE<<"\nT:\n"<<tT<<"\nP:\n"<<tP<<"\n\n";
        Current = Old;
        return LastE;
    }	                                         

    void Temperature(double *t,double *dencity,double *energy,int num);
    void Pressure(double *p,double *dencity,double *energy,int num);
    int read_data_state(FilterIn &in) {
        in >> CaloricSplineName;
        ReadSplFile();
        return 1;
    };
    int save_data_state(FilterOut &out){ 
        out << "  " << CaloricSplineName << " ";
        return 1;
    };
    void ReadSplFile() {
        FilterTextIn sp(CaloricSplineName.c_str());
        if ( (!sp) || (seek_eof(sp)) )
            throw info_except(" There is no such MatterCaloricSpline file named %s\n", CaloricSplineName.c_str());
        sp.SetRefMask(SavableClass::SimpleEdit);
        char tmp[256];
        sp >> tmp >> DencCold >> tmp >> DencMax;
        SavableClass *o1, *o2;
        sp >> tmp >> o1 >> tmp >> o2;
        PresSpl << o1;
        TempSpl << o2;
    }
    void SaveSplFile() {
        FilterTextOut sp(CaloricSplineName.c_str());
        if (!sp)
            throw info_except(" There is no such MatterCaloricSpline file named %s\n", CaloricSplineName.c_str());
        sp.SetRefMask(SavableClass::SimpleEdit);
        sp << " DencCold " << DencCold << " DencMax " << DencMax<<"\n";
        sp << " PressureSpline " << PresSpl << " TemperatureSpl " << TempSpl;
    }

    virtual Stroka MakeHelp(){
        Stroka res = MatterIO::MakeHelp();
        res += Stroka("Caloric spline - new format\n");
        return res;
    }
};
 
