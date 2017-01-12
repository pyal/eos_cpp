

#pragma once

#include "matt_fac.h"


#include "mat/mtfunc.h"
struct Matter2State : MatterIO {
    Ref<MatterIO> M1, M2;
    double MinP, VolProp12, Pres_ElseDenc;   //V2=V1
    double TimeToWait;
    Matter2State();
    void SetInternalNames() {
        MatterIO::SetInternalNames();
        InputNames["TransTime"] = NULL;
    }
    virtual Stroka MakeHelp() {
        Stroka res = MatterIO::MakeHelp();
        res += Stroka(
            " InputName: TransTime - is positive - Matter is in 1 state waiting time abs(TimeToWait)\n");
        res += Stroka(
            "            TransTime - is negative - Matter is in 2 state waiting time abs(TimeToWait)\n");
        return res;
    }
    double Pressure(double Dencity, double Energy);
    double Sound(double Dencity, double Energy);
    double Temperature(double Dencity, double Energy);

    void Pressure(double *P, double *Dencity, double *Energy, int Num);
    void Sound(double *S, double *Dencity, double *Energy, int Num);
    void Temperature(double *S, double *Dencity, double *Energy, int Num);
    void ClcArray(
        double *P,
        double *Dencity,
        double *Energy,
        int Num,
        const Stroka &What);

    virtual int save_data_state(FilterOut &output) {
        output << "Pres_ElseDenc " << Pres_ElseDenc << " Pressure1_2 " << MinP
               << " TimeToWait " << TimeToWait << " Substances \n";
        output << "   ";
        SavableClass::Save(output, M1);
        output << "   ";
        SavableClass::Save(output, M2);
        return 1;
    };
    virtual int read_data_state(FilterIn &input) {
        char tmp[256];
        input >> tmp >> Pres_ElseDenc >> tmp >> MinP >> tmp >> TimeToWait >> tmp;
        if(!(M1 << SavableClass::Read(input)) || !(M2 << SavableClass::Read(input)))
            throw info_except(" Could not register matters...\n");
        return 1;
    };
    void CheckInput() {
        if(InputValStorage.find("TransTime") == InputValStorage.end())
            throw info_except("Not defined internal var TransTime\n");
    }
    virtual void ChangeInputVal(double Dencity, double Energy, double Time) {
        CheckInput();
        double Val = Pres_ElseDenc ? Pressure(Dencity, Energy) : Dencity;
        if(InputValStorage["TransTime"] >= 0)
            if(Val > MinP)
                InputValStorage["TransTime"] += Time;
            else
                InputValStorage["TransTime"] = M_MinDouble;
        else if(Val < MinP)
            InputValStorage["TransTime"] -= Time;
        else
            InputValStorage["TransTime"] = -M_MinDouble;
        if(fabs(InputValStorage["TransTime"]) > TimeToWait)
            InputValStorage["TransTime"] =
                MTSign(InputValStorage["TransTime"]) * (-M_MinDouble);
    };
    virtual void ChangeInputVal(double *Dencity, double *Energy, int Num, double Time);

private:
};

struct MatterSum : MatterIO {
    Ref<MatterIO> M1, M2;
    double MinP, VolProp12, Pres_Denc;   //V2=V1

    int NumIntPar() {
        return 1;
    }
    MatterSum();   //:MatterIO(){VolProp12=1;SetInternalNames();}
                   //virtual ~MatterSum(){delete M1;delete M2;M1=NULL;M2=NULL;}

    void ClcDenc(double Dencity, double Energy, double alpha, double &d1, double &d2) {
        d1 = alpha * (1 + VolProp12) / (alpha + 1);
        d2 = (1 + VolProp12) / (VolProp12 * (alpha + 1));
    }
    double Pressure(double Dencity, double Energy);
    double Sound(double Dencity, double Energy);
    double Temperature(double Dencity, double Energy);

    void Pressure(double *P, double *Dencity, double *Energy, int Num);
    void Sound(double *S, double *Dencity, double *Energy, int Num);
    //   void Temperature(double *S,double *Dencity,double *Energy,int Num);

    double DencityCold() {
        return min(M1->DencityCold(), M2->DencityCold());
    };
    double DencityMax() {
        return max<double>(M1->DencityMax(), M2->DencityMax());
    };

    virtual int save_data_state(FilterOut &output) {
        output << " Pres_Denc " << Pres_Denc << " Pressure1_2 " << MinP
               << " Substances \n";
        output << "   ";
        SavableClass::Save(output, M1);
        output << "   ";
        SavableClass::Save(output, M2);
        return 1;
    };
    virtual int read_data_state(FilterIn &input) {
        char tmp[256];
        input >> tmp >> Pres_Denc >> tmp >> MinP >> tmp;
        M1 << SavableClass::Read(input);
        M2 << SavableClass::Read(input);
        if(!M1 || !M2)
            throw info_except(" Could not register matters...\n");
        log_debug(" Read M1 " +  SavableClass::object2string(M1.pointer()));
        log_debug(" Read M2 " +  SavableClass::object2string(M2.pointer()));
        return 1;
    };
    void SetInternalNames() {
        MatterIO::SetInternalNames();
        InputNames["State"] = NULL;
    }
    virtual Stroka MakeHelp() {
        Stroka res = MatterIO::MakeHelp();
        res += Stroka(" InputName: State - 0  Matter is in 0 state \n");
        res += Stroka("            State - 1  Matter is in 1 state \n");
        return res;
    }

    virtual void ChangeInputVal(double Dencity, double Energy, double Time) {
        if(InputValStorage["State"] == 0) {
            if(Pres_Denc) {
                if(Pressure(Dencity, Energy) > MinP)
                    InputValStorage["State"] = 1;
            } else if(Dencity > MinP)
                InputValStorage["State"] = 1;
        }
    };
    virtual void ChangeInputVal(double *Dencity, double *Energy, int Num, double Time);
};


struct Matter2StateProb : MatterIO {
    Ref<MatterIO> M1, M2;
    double TransDenc;   //V2=V1
    double Prob0, Prob1;
    Matter2StateProb();
    void SetInternalNames() {
        MatterIO::SetInternalNames();
        InputNames["TransTime"] = NULL;
    }
    virtual Stroka MakeHelp() {
        Stroka res = MatterIO::MakeHelp();
        res += Stroka(
            " InputName: TransTime - is positive - Matter is in 1 state waiting time Prob...\n");
        res += Stroka(
            "            TransTime - is negative - Matter is in 2 state waiting time Prob...\n");
        return res;
    }
    double Pressure(double Dencity, double Energy);
    double Sound(double Dencity, double Energy);
    double Temperature(double Dencity, double Energy);

    void Pressure(double *P, double *Dencity, double *Energy, int Num) {
        ClcArray(P, Dencity, Energy, Num, "Pressure");
    }
    void Sound(double *S, double *Dencity, double *Energy, int Num) {
        ClcArray(S, Dencity, Energy, Num, "Sound");
    }
    void Temperature(double *T, double *Dencity, double *Energy, int Num) {
        ClcArray(T, Dencity, Energy, Num, "Temperature");
    }

    virtual int save_data_state(FilterOut &output) {
        output << " TransDenc " << TransDenc << " Prob0 " << Prob0 << " Prob1 " << Prob1
               << " Substances \n";
        output << "   ";
        SavableClass::Save(output, M1);
        output << "   ";
        SavableClass::Save(output, M2);
        return 1;
    };
    virtual int read_data_state(FilterIn &input) {
        char tmp[256];
        input >> tmp >> TransDenc >> tmp >> Prob0 >> tmp >> Prob1 >> tmp;
        M1 << SavableClass::Read(input);
        M2 << SavableClass::Read(input);
        if(!M1 || !M2)
            throw info_except(" Could not register matters...\n");
        return 1;
    };
    void CheckInput() {
        if(InputValStorage.find("TransTime") == InputValStorage.end())
            throw info_except("Not defined internal var TransTime\n");
    }
    virtual void ChangeInputVal(double Dencity, double Energy, double Time);
    virtual void ChangeInputVal(double *Dencity, double *Energy, int Num, double Time);

private:
    double ClcDissCoef(double *Dencity, int Num);
    void ClcArray(
        double *P,
        double *Dencity,
        double *Energy,
        int Num,
        const Stroka &What);
};
