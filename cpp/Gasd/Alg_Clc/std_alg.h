
#ifndef Std_Alg_h
#define Std_Alg_h 1

#include "lib/data/DataVec.h"
#include "lib/data/DataTyp.h"


int GetHuman(FilterIn &in, RefBase &ref);


struct Algorithm_MainAction : SavableClass {
    int NumArg;
    char **CharArg;
    Algorithm_MainAction() : NumArg(-1), CharArg(NULL){};
    virtual ~Algorithm_MainAction(){};

    virtual void WriteRes() = 0;
    virtual int SetParam(int argc, char **argv);
    virtual void PrintHelp();
};

struct Algorithm_TimeWrite : SavableClass {
    double StartTime;

    virtual double GetStartTime() {
        return StartTime;
    }
    virtual double GetStartTime(double Time) {
        StartTime = Time;
        return StartTime;
    }
    virtual int GetNextTime(double &Time, double MaxTimeStp, double BoundWaveAmp) {
        Time = StartTime;
        return 0;
    }
    virtual void print(ostream & = ExEnv::out()) const {
        fcout
            << " Algorithm_TimeWrite : Used to get next time for gasd.\n Empty one (only base head defs), can be used to make one step calculations\n";
        fcout << " Its format : " << (SavableClass *)this << "\n";
        fcout.flush();
    }
    virtual int save_data_state(FilterOut &so) {
        so << KeyDesc("StartTime") << StartTime;
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        si >> StdKey >> StartTime;
        GetStartTime(StartTime);
        return !(si.fail());
    };
};


struct Algorithm_MakeStp : SavableClass {
    double StartTime;
    Algorithm_MainAction *main_ptr;
    Algorithm_TimeWrite *time_ptr;
    //  Algorithm_MainAction *main_ptr;
    //  Algorithm_TimeWrite *time_ptr;

    virtual int SetParam(
        Algorithm_MainAction *m_ptr,
        Algorithm_TimeWrite *t_ptr,
        FilterIn *in_ptr,
        FilterOut *out_ptr) {
        main_ptr = m_ptr;
        time_ptr = t_ptr;
        return 1;
    }
    virtual void MakeTimeStp(
        double TimeWas,
        double TimeNew,
        double &MaxTimeStp,
        double &BoundWaveAmp) {
        BoundWaveAmp = 0;
        MaxTimeStp = 1e6;
    }
    virtual void WriteRes(double TimeNew, FilterOut &out) {
        out << "CurTime" << TimeNew << "\n";
    };


    virtual double GetStartTime(double time) {
        StartTime = time;
        return StartTime;
    }
    virtual void print(ostream & = ExEnv::out()) const {
        fcout
            << " Algorithm_MakeStp : Used to do main calculations for gasd, and to write results.\n Empty one (only base head defs), can be used to output times produced by Algorithm_TimeWrite\n";
        fcout << " Its format : " << (SavableClass *)this << "\n";
        fcout.flush();
    }
};


// =============================================================================
// ===============  Concrete Gasd Classes  =====================================
// =============================================================================


struct Algorithm_MainAction_Gasd : Algorithm_MainAction {
    Ref<Algorithm_TimeWrite> time_ptr;
    Ref<Algorithm_MakeStp> clc_ptr;

    FilterTextIn *in_ptr;
    FilterTextOut *out_ptr;

    Algorithm_MainAction_Gasd() : Algorithm_MainAction(), in_ptr(NULL), out_ptr(NULL){};
    ~Algorithm_MainAction_Gasd() {
        Delete();
    }
    void Delete() {
        delete in_ptr;
        in_ptr = NULL;
        delete out_ptr;
        out_ptr = NULL;
        clc_ptr = NULL;
        time_ptr = NULL;
    }
    virtual int SetParam(int argc, char **argv);
    virtual void WriteRes();
    virtual void PrintHelp();
};


struct Algorithm_TimeWrite_Gasd : Algorithm_TimeWrite {
    double EndTime, TimeWriteStp, BoundWaveCoef, StabilityCoef, IncreaseStepCoef;
    double NewTimeWrite, OldStp, OldTimeWrite;
    int NumIter, NumWriteSteps;

    virtual double GetStartTime(double Time) {
        StartTime = Time;
        OldTimeWrite = Time;
        NewTimeWrite = Time + TimeWriteStp;
        NumIter = 1;
        OldStp = 1e50;
        NumWriteSteps = 0;
        return StartTime;
    }
    virtual int GetNextTime(double &Time, double MaxTimeStp, double BoundWaveAmp);

    virtual int save_data_state(FilterOut &so) {
        so << KeyDesc("StartTime") << StartTime << KeyDesc("EndTime") << EndTime
           << KeyDesc("TimeWriteStp") << TimeWriteStp << KeyDesc("StabilityCoef")
           << StabilityCoef << KeyDesc("IncreaseStepCoef") << IncreaseStepCoef
           << KeyDesc("BoundWaveCoef") << BoundWaveCoef;
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        si >> StdKey >> StartTime >> StdKey >> EndTime >> StdKey >> TimeWriteStp >>
            StdKey >> StabilityCoef >> StdKey >> IncreaseStepCoef >> StdKey >>
            BoundWaveCoef;
        GetStartTime(StartTime);
        return 1;
    };
    ~Algorithm_TimeWrite_Gasd(){};

    virtual void print(ostream & = ExEnv::out()) const {
        fcout
            << " Algorithm_TimeWrite_Gasd \n Used to get next time for gasd.\n Standart time step calculation algorithm\n";
        fcout << " Its format : " << (SavableClass *)this << "\n";
    }
};


struct Config_Gasd : SavableClass {
    //  int AccuracySpace,AccuracyTime,CalculationMethod;
    virtual int IsGood() {
        return 1;
    };
    virtual SavableClass *CreateCopy() {
        return NULL;
    };
    virtual int ReadData(FilterIn &in, FilterOut &out) {
        return 1;
    };
    virtual void MakeTimeStp(
        double TimeWas,
        double TimeNew,
        double &MaxTimeStp,
        double &BoundWaveAmp) {
        BoundWaveAmp = 0;
        MaxTimeStp = 1e6;
    }

    virtual void WriteRes(double TimeNew, FilterOut &out) {
        out << "CurTime" << TimeNew << "\n";
    };
    virtual double GetStartTime() {
        return 0;
    }

    virtual void print(ostream & = ExEnv::out()) const {
        fcout
            << "Config_Gasd\n Zero class, only header defs. Used to read Gasd Par and to make main calculations\n";
        fcout << " Its format : " << (SavableClass *)this;
    };
};


struct Algorithm_MakeStp_Gasd : Algorithm_MakeStp {
    Ref<Config_Gasd> gasd;
    int ChangeStart;

    Algorithm_MakeStp_Gasd() : ChangeStart(0){};
    virtual double GetStartTime(double time) {
        if(ChangeStart)
            time = gasd->GetStartTime();
        return Algorithm_MakeStp::GetStartTime(time);
    };
    virtual int SetParam(
        Algorithm_MainAction *m_ptr,
        Algorithm_TimeWrite *t_ptr,
        FilterIn *in_ptr,
        FilterOut *out_ptr) {
        Algorithm_MakeStp::SetParam(m_ptr, t_ptr, in_ptr, out_ptr);
        //    *in_ptr>>StdKey;if (!GetHuman(*in_ptr,gasd)) return 0;
        //    *out_ptr<<"Config_Gasd\n"<<(SavableClass*)gasd;
        if(!gasd->ReadData(*in_ptr, *out_ptr))
            return 0;

        return 1;

        Ref<Config_Gasd> gasd_old;
        gasd_old << gasd->CreateCopy();
        while((!seek_eof(*in_ptr)) && (gasd->ReadData(*in_ptr, *out_ptr))) {
            gasd_old << gasd->CreateCopy();
            ChangeStart = 1;
        }
        if(!gasd->IsGood())
            if(!gasd_old->IsGood())
                return 0;
            else
                gasd << gasd_old->CreateCopy();
        return 1;
    }

    virtual void MakeTimeStp(
        double TimeWas,
        double TimeNew,
        double &MaxTimeStp,
        double &BoundWaveAmp) {
        gasd->MakeTimeStp(TimeWas, TimeNew, MaxTimeStp, BoundWaveAmp);
    }
    virtual void WriteRes(double TimeNew, FilterOut &out) {
        gasd->WriteRes(TimeNew, out);
    };

    virtual void print(ostream & = ExEnv::out()) const {
        fcout
            << "Algorithm_MakeStp_Gasd\n Makes iterative read and save of grid values\n";
        fcout
            << " To do this calls Config_Gasd object\n Its format : NOT_NULL Algorithm_MakeStp_Gasd { ConfigGasd Config_Gasd_Object }\n";
        fcout << " Currently present Config_Gasd_Category classes\n";
        fcout << SavableClass::HelpForCategory("Config_Gasd_Category", 1).c_str();
    };
    virtual int save_data_state(FilterOut &so) {
        so << "Config_Gasd" << (SavableClass *)gasd;
        return !so.fail();
    }

    virtual int read_data_state(FilterIn &si) {
        si >> StdKey;
        GetHuman(si, gasd);
        return gasd != NULL;
    }
};


#endif


/*
struct Algorithm_TimeStepStd
 {
  void StartParClc(Body &WorkBody,int Order);
  void CellFixed(Body &WorkBody);//,double &PresDeriv);
  void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability);
  void CalcPosE(Body &WorkBody,double TimeStp);
 }
*/
