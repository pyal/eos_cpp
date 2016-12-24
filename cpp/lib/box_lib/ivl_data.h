
#pragma once

#include "mat/dat_fac.h"
#include "mat/spl_fac.h"
#include "mat/mtmin.h"
#include "mat/zero.h"
#include "lib/ref/data_manip.h"
#include "urs/matt_caloric_spl.h"
#include "urs/matt_ispl.h"
#include "lib/ref/refmanip.h"

#include "box_face.h"

struct IIvlParametersBase : SavableClass {
    IIvlParametersBase();
    //    : SavableClass()
    //    , NumVolPnt(100)
    //    , NumTempPnt(100)
    //    , pTVname("ivl_ptv.dat")
    //    , eTVname("ivl_etv.dat")
    //    , MinVol(1e-3)
    //    , MaxVol(1e3)
    //    , MinT(50)
    //    , MaxT(1e6) {
    //}
    double MinVol, MaxVol, MinT, MaxT, MinE, MaxE;
    virtual double Energy(double t, double v) = 0;
    virtual double Pressure(double t, double v) = 0;
    virtual double Energy(double *t, double *v, double *e, int num) = 0;
    virtual double Pressure(double *t, double *v, double *p, int num) = 0;
    virtual void CreateSplines() = 0;
    int save_data_state(FilterOut &so) {
        so << " pTVname " << pTVname << " eTVname " << eTVname << SavableClass::EOLN()
           << " NumVolPnt " << NumVolPnt << " NumTempPnt " << NumTempPnt
           << SavableClass::EOLN() << " MinVol " << MinVol << " MaxVol " << MaxVol
           << " MinT " << MinT << " MaxT " << MaxT << SavableClass::EOLN();
        return 1;
    }
    int read_data_state(FilterIn &si) {
        Stroka tmp;
        si >> tmp >> pTVname >> tmp >> eTVname >> tmp >> NumVolPnt >> tmp >> NumTempPnt >>
            tmp >> MinVol >> tmp >> MaxVol >> tmp >> MinT >> tmp >> MaxT;
        return 1;
    };
    // To be removed
    static void MakeDebugOutput(const Stroka &file, TData<double> &data) {
        fstream out(file.c_str(), ios::out);
        data.SaveIni(out);
        out.close();
    }
    void CreateIvlData(TColumn<double> &datP, TColumn<double> &datE) {
        TTable<double> tmpP, tmpE;
        std::vector<double> temp =
            DataManip::MakeStepVector(NumTempPnt, MinT, MaxT, 1, 0);
        std::vector<double> vol =
            DataManip::MakeStepVector(NumTempPnt, MinVol, MaxVol, 1, 0);
        tmpP.SetColumn(&temp[0], temp.size(), 0);
        tmpP.SetColumn(&vol[0], vol.size(), 1);
        tmpE = tmpP;
        vector<vector<double>> vec;
        File::ReadFile(pTVname.c_str(), vec, "could not read pressures!!!", 0);
        vector<double> pres = DataManip::JoinArray(vec);
        tmpP.SetColumn(&pres[0], pres.size(), 2);
        File::ReadFile(eTVname.c_str(), vec, "could not read energies!!!", 0);
        vector<double> ener = DataManip::JoinArray(vec);
        tmpE.SetColumn(&ener[0], ener.size(), 2);
        TData<double>::FindMinMax(tmpE, 2, MinE, MaxE);
        datP = tmpP;
        datE = tmpE;
    }

protected:
    int NumVolPnt, NumTempPnt;
    Stroka pTVname, eTVname;
};


struct TIvlParametersSplIO : IIvlParametersBase {
    TIvlParametersSplIO()
        : IIvlParametersBase(),
          SplP_TV(new ExtendedRegridSpline),
          SplE_TV(new ExtendedRegridSpline) {}
    int save_data_state(FilterOut &so) {
        so << " SplE_TV ";
        SavableClass::IncIdent();
        SplE_TV->save_generation_par(so);
        so << SavableClass::mmEOLN() << " SplP_TV ";
        SavableClass::IncIdent();
        SplP_TV->save_generation_par(so);
        SavableClass::DecIdent();
        return 1;
    };
    int read_data_state(FilterIn &si) {
        Stroka tmp;
        SplE_TV = new ExtendedRegridSpline();
        SplP_TV = new ExtendedRegridSpline();
        si >> tmp;
        SplE_TV->read_generation_par(si);
        si >> tmp;
        SplP_TV->read_generation_par(si);
        return 1;
    };
    //Stroka MakeHelp(){return "";}

    void CreateSplines() {
        TColumn<double> datP, datE;
        CreateIvlData(datP, datE);
        try {
            SplP_TV->Generate(1, datP, "Bad pressure - generating from ivl table");
            SplE_TV->Generate(1, datE, "Bad energy - generating from ivl table");
        } catch(stdexception &ex) {
            MakeDebugOutput(pTVname + ".grid", datP);
            MakeDebugOutput(eTVname + ".grid", datE);
            throw info_except("Caught exception:\n%s", ex.what());
        }
    }
    double Energy(double t, double v) {
        return SplE_TV->EvaluateSimple(t, v);
    }
    double Energy(double *t, double *v, double *e, int num) {
        return SplE_TV->Evaluate(t, v, e, num);
    }
    double Pressure(double t, double v) {
        return SplP_TV->EvaluateSimple(t, v);
    }
    double Pressure(double *t, double *v, double *p, int num) {
        return SplP_TV->Evaluate(t, v, p, num);
    }

private:
    Ref<ExtendedRegridSpline> SplP_TV, SplE_TV;
};


// =======================================================================
// ======================  IvlDataReader  ================================
// =======================================================================
struct TIvlDataReader : SavableClass {
    TIvlDataReader() : XFile("temper.in"), YFile("volume.in"), ZFile("pressu.tab"){};

    vector<double> GetXVector() {
        return X;
    }
    vector<double> GetYVector() {
        return Y;
    }
    vector<double> GetZVector(int col, int xFixed) {
        if(col < 0)
            return Z;
        if(xFixed) {
            if(col >= (int)Y.size())
                throw info_except("Col %i is larger then Y.size %i\n", col, Y.size());
            VecCl v(&Z[col * X.size()], Y.size());
            return v.Copy2Vector();
        }
        if(col >= (int)X.size())
            throw info_except("Col %i is larger then X.size %i\n", col, X.size());
        vector<double> ret;
        for(size_t i = 0; i < Y.size(); i++)
            ret.push_back(Z[i * X.size() + col]);
        return ret;
    }
    int save_data_state(FilterOut &so) {
        so << "XFile " << XFile << " YFile " << YFile << " ZFile " << ZFile
           << SavableClass::EOLN();
        return 1;
    }
    int read_data_state(FilterIn &si) {
        Stroka tmp;
        si >> tmp >> XFile >> tmp >> YFile >> tmp >> ZFile;
        ReadFiles();
        return 1;
    };
    void ReadFiles() {
        ReadFile(XFile, X);
        ReadFile(YFile, Y);
        ReadFile(ZFile, Z);
        if(X.size() * Y.size() != Z.size() || X.size() == 0)
            throw info_except(
                "[%s]X(%i) * [%s]Y(%i) != [%s]Z(%i)\n",
                ~XFile,
                X.size(),
                ~YFile,
                Y.size(),
                ~ZFile,
                Z.size());
    }
    void ReadFile(const Stroka &name, vector<double> &pnt) {
        ifstream in(~name, ios::in);
        double d;
        pnt.clear();
        in >> d;
        while(in) {
            pnt.push_back(d);
            in >> d;
        }
        in.close();
    }

protected:
    Stroka XFile, YFile, ZFile;
    vector<double> X, Y, Z;
};


#include <mat/spline/spl_inc.h>

struct TIvlTableDataReconstruct : SavableClass {
private:
    void Init(vector<vector<double>> &vec, const vector<double> &e) {
        vec.clear();
        vec.resize(3);
        int resDim = e.size() * E_tv_data.GetYVector().size();
        for(size_t i = 0; i < vec.size(); i++)
            vec[i].resize(resDim);
    }
    void Append(
        vector<vector<double>> &z_er,
        const vector<double> &z,
        const vector<double> &e,
        int iVol) {
        vector<double> v = E_tv_data.GetYVector();
        int vSize = v.size();
        for(size_t i = 0; i < e.size(); i++) {
            int iDenc = vSize - 1 - iVol;
            int p = iDenc + i * vSize;
            z_er[0][p] = e[i];
            z_er[1][p] = 1 / v[iVol];
            z_er[2][p] = z[i];
        }
    }
    void GenSpline(
        Ref<ISplineGenerator> spl,
        vector<double> &x,
        vector<double> &y,
        const char *errMess) {
        vector<vector<double>> gen;
        gen.push_back(x);
        gen.push_back(y);
        int err;
        if((err = spl->Generate(gen)) > 0) {
            VecCl tmp(x);
            Stroka str = "X vector \n" + tmp.ToString();
            str += "\nY vector \n" + (tmp = y).ToString();
            throw info_except(
                "%s\nCould not generate spline for data \n%s\n error %i",
                errMess,
                ~str,
                err);
        }
    }
    void Clc_pt(const vector<double> &e, vector<double> &p, vector<double> &t, int iVol) {
        p.resize(e.size());
        t.resize(e.size());
        vector<double> e_t = E_tv_data.GetZVector(iVol, 0);
        vector<double> p_t = P_tv_data.GetZVector(iVol, 0);
        vector<double> t_t = E_tv_data.GetXVector();
        GenSpline(
            T_e_spl, e_t, t_t, ~(Stroka("E_t for volume ") + Stroka::Int2Str(iVol)));
        GenSpline(
            P_t_spl, t_t, p_t, ~(Stroka("P_t for volume ") + Stroka::Int2Str(iVol)));
        Ref<ISplineCalculator> teClc = T_e_spl->GetCalculator(),
                               ptClc = P_t_spl->GetCalculator();
        for(size_t i = 0; i < e.size(); i++) {
            t[i] = ISplineCalculator::ClcSpl(teClc, e[i], "Clc temp for energy");
            p[i] = ISplineCalculator::ClcSpl(ptClc, t[i], "Clc pres for temp");
        }
    }
    void AppendPlanar(
        vector<vector<double>> &z_tv,
        const vector<double> &z,
        double temp) {
        vector<double> v = E_tv_data.GetYVector();
        int vSize = v.size();
        for(size_t iDenc = 0; iDenc < (size_t)vSize; iDenc++) {
            int iVol = vSize - 1 - iDenc;
            z_tv[0].push_back(temp);
            z_tv[1].push_back(1 / v[iVol]);
            z_tv[2].push_back(z[iVol]);
        }
    }
    vector<vector<double>> GetPlanarXYZ(TIvlDataReader &ivlData) {
        vector<double> t = ivlData.GetXVector();
        vector<vector<double>> ret(3);
        for(size_t i = 0; i < t.size(); i++)
            AppendPlanar(ret, ivlData.GetZVector(i, 1), t[i] * 1000);

        return ret;
    }

public:
    TIvlTableDataReconstruct()
        : P_t_spl(new TEncodedSplineGenerator), T_e_spl(new TEncodedSplineGenerator){};

    void GetPT(
        const vector<double> &e,
        vector<vector<double>> &pres_er,
        vector<vector<double>> &temp_er) {
        Init(pres_er, e);
        Init(temp_er, e);
        vector<double> p, t;
        vector<double> t_k;
        vector<double> v = E_tv_data.GetYVector();
        for(size_t i = 0; i < v.size(); i++) {
            Clc_pt(e, p, t, i);
            Append(pres_er, p, e, i);
            VecCl t_k = VecCl(t) * 1000;
            Append(temp_er, t_k.Copy2Vector(), e, i);
        }
    }
    void GetPE_rt(vector<vector<double>> &p_rt, vector<vector<double>> &e_rt) {
        p_rt = GetPlanarXYZ(P_tv_data);
        e_rt = GetPlanarXYZ(E_tv_data);
    }
    void GetMinMaxE(double &min, double &max) {
        VecCl e(E_tv_data.GetZVector(-1, 0));
        min = VecCl::Min(e);
        max = VecCl::Max(e);
    }
    int save_data_state(FilterOut &so) {
        so << " P_tv_data ";
        P_tv_data.save_data_state(so);
        so << " P_t_spl " << P_t_spl;
        so << " E_tv_data ";
        E_tv_data.save_data_state(so);
        so << " T_e_spl " << T_e_spl;
        return 1;
    }
    int read_data_state(FilterIn &si) {
        Stroka tmp;
        si >> tmp;
        P_tv_data.read_data_state(si);
        si >> tmp >> P_t_spl >> tmp;
        E_tv_data.read_data_state(si);
        si >> tmp >> T_e_spl;
        return 1;
    };

protected:
    TIvlDataReader P_tv_data, E_tv_data;
    Ref<ISplineGenerator> P_t_spl, T_e_spl;
};


struct TIvlTable2Spl : IBlackBoxBase {

    TIvlTable2Spl()
        : IBlackBoxBase(),
          Pspl(new TEncodedSplineGenerator(new TSpline2DGenerator)),
          Tspl(new TEncodedSplineGenerator(new TSpline2DGenerator)),
          Econverter(new TExpConverter),
          SplFile("h2.ispl"),
          SplDescription("test spline"),
          UsePT_RE_Files(0),
          P_RE_SaveFile("P_RE_file"),
          T_RE_SaveFile("T_RE_file"),
          NumEPnt(300) {}

    virtual void DoIt() {
        double minE, maxE;
        PTE_reader.GetMinMaxE(minE, maxE);
        vector<double> e = Econverter->MakeEncodedStepVector(minE, maxE, NumEPnt, 0);
        vector<vector<double>> pres_er, temp_er;
        GetPT(e, pres_er, temp_er);
        IterateMakeSpline(Pspl, pres_er, "generating pressure");
        IterateMakeSpline(Tspl, temp_er, "generating temperature");
        //Pspl->Generate(pres_er);
        //Tspl->Generate(temp_er);
        vector<double> minP, maxP, minT, maxT;
        DataManip::SetLimits(pres_er, minP, maxP);
        DataManip::SetLimits(temp_er, minT, maxT);
        TMatterSpl mat;
        SplDescription += " minD " + Stroka(minP[1]) + " maxD " + Stroka(maxP[1]) +
                          " minT " + Stroka(minT[2]) + " maxT " + Stroka(maxT[2]) +
                          " minE " + Stroka(minT[0]) + " maxE " + Stroka(maxT[0]) + " ";
        mat.SetParams(
            SplFile,
            SplDescription,
            Pspl->GetCalculator(),
            Tspl->GetCalculator(),
            minP[1],
            maxP[1]);
        mat.SaveSplFile();
    }
    int save_data_state(FilterOut &so) {

        so << " ResSplFile " << SplFile << SavableClass::EOLN();
        so << " ResSplDescription " << SplDescription << SavableClass::EOLN();
        so << " NumEPnt2NewSpl " << NumEPnt << " Econverter2NewSpl " << Econverter
           << SavableClass::EOLN();
        so << " P_RE_SaveFile " << P_RE_SaveFile << " T_RE_SaveFile " << T_RE_SaveFile
           << " UsePT_RE_Files " << UsePT_RE_Files << SavableClass::EOLN();

        so << " PTE_reader_reader " << SavableClass::EOLN();
        PTE_reader.save_data_state(so);
        so << SavableClass::EOLN();
        so << " Pspl_finGenerator " << Pspl;
        so << " Tspl_finGenerator " << Tspl;
        return 1;
    }
    int read_data_state(FilterIn &si) {
        Stroka tmp;
        char tt[20000];
        si >> tmp >> SplFile >> tmp;
        si.getline(tt, 20000);
        SplDescription = tt;
        si >> tmp >> NumEPnt >> tmp >> Econverter;
        si >> tmp >> P_RE_SaveFile >> tmp >> T_RE_SaveFile >> tmp >> UsePT_RE_Files;
        si >> tmp;
        PTE_reader.read_data_state(si);
        si >> tmp >> Pspl >> tmp >> Tspl;

        return 1;
    };

private:
    void IterateMakeSpline(
        ISplineGenerator *spl,
        vector<vector<double>> dat,
        const char *mes) {
        int res = spl->Generate(dat);
        if(res > 0)
            throw info_except("could not generate spline error: ", res);
        //ISplineGeneratorStd *s = dynamic_cast<ISplineGeneratorStd *>(spl);
        //if (!s)
        //    throw info_except("not std generator %s\n", typeid(*spl).name());
        //double mis = s->GetMisfit();
        //int c = 0;
        //while(!s->GenerateExtra(dat, mis)) {
        //    if (c++ > 10)
        //        throw info_except("Cannot make spline with misfit %f\n%s", mis, mes);
        //    mis *= 10;
        //}
    }

    void GetPT(
        const vector<double> &e,
        vector<vector<double>> &pres_er,
        vector<vector<double>> &temp_er) {
        vector<vector<double>> p_rt, e_rt;
        PTE_reader.GetPE_rt(p_rt, e_rt);
        File::WriteFile(~(P_RE_SaveFile + ".p_rt"), p_rt, NULL, NULL);
        File::WriteFile(~(P_RE_SaveFile + ".e_rt"), e_rt, NULL, NULL);
        if(!UsePT_RE_Files) {
            PTE_reader.GetPT(e, pres_er, temp_er);
            File::WriteFile(~P_RE_SaveFile, pres_er, NULL, NULL);
            File::WriteFile(~T_RE_SaveFile, temp_er, NULL, NULL);
            return;
        }
        File::ReadFile(~P_RE_SaveFile, pres_er, NULL);
        File::ReadFile(~T_RE_SaveFile, temp_er, NULL);
    }
    TIvlTableDataReconstruct PTE_reader;
    Ref<ISplineGenerator> Pspl, Tspl;
    Ref<IFunctionConverter> Econverter;
    Stroka SplFile, SplDescription;
    int UsePT_RE_Files;
    Stroka P_RE_SaveFile, T_RE_SaveFile;
    int NumEPnt;
};


//void ChangeXY(TColumn<double> &data) {

//}
//vector<CurveSplineEncode> SplArr;
//class double_compare { //: public binary_function<double, double, bool> {
//public:
//    bool operator()(const double& _Left, const double& _Right) const {
//        return _Left < _Right && (fabs(_Left - _Right) > max<double>(1e-10, fabs(_Left) + fabs(_Right))) * 1e-10;
//        //return _Left < _Right && fabs(log(_Left) - log(_Right)) > 1e-10;
//    }
//    double_compare(double _Pred) : comp(_Pred){};
//protected:
//    double comp;
//};

//map<double, int, double_compare> Vol2SplNum;
