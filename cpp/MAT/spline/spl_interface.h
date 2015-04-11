
#pragma once

//#include <mat\fun_fac.h>
//#include <mat\spl_fac.h>
#include <lib/ref/data_manip.h>
#include <lib/ref/refmanip.h>
#include <lib/ref/class_sav.h>

#include <lib/ref/file_manip.h>

struct ISplineCalculator : SavableClass {
    ISplineCalculator()
        : SplFileName("SingleFileStorage") {
    }
    virtual int GetSplineDim() = 0;
    //    return SplineDim;
    //}
    virtual vector<double> GetBndMin() = 0;
    virtual vector<double> GetBndMax() = 0;
    virtual double GetMisfit() = 0;
    int NoExternalFile() {
       return (SplFileName == "SingleFileStorage");
    }
    virtual void SaveSplineData(FilterOut &out) = 0;
    virtual void ReadSplineData(FilterIn &in) = 0;
    //virtual void SaveSplineData() {
    //   fstream out(~SplFileName, ios::out);
    //   SaveSplineData(out);
    //   out.close();
    //}

    virtual int  Evaluate (const vector<vector<double> > &dat, vector<double> &res) = 0;
    virtual int  Evaluate (const vector<double> &dat, double &res) {
        vector<vector<double> > datV(dat.size());
        vector<double> resV;
        for(size_t i = 0; i < datV.size(); i++)
            datV[i].push_back(dat[i]);

        //datV.push_back(dat);
        //DataManip::CopyVector(datV[0], dat);
        int ret = Evaluate(datV, resV);
        res = resV[0];
        return ret;
    }
    static double ClcSpl(ISplineCalculator *spl, double x, const char *err_mes = NULL) {
        vector<double> xV(1);
        double y;
        xV[0] = x;
        int err;
        if (!(err = spl->Evaluate(xV, y)) && err_mes )
            throw info_except("Could not evaluate spline for x = %f y = %f res = %i\nDescription : %s", x, y, err, err_mes);
        return y;
    }
// FIXED !!!! use ReadSplineData
    virtual int save_data_state(FilterOut&so) {
       so << "SplFileName " << SplFileName;
       if (NoExternalFile())
            SaveSplineData(so);
       else
           if (File::GetFileSize(~SplFileName) == -1) {
               FilterTextOut out(~SplFileName);
               out.SetRefMask(SavableClass::SimpleEdit);
               SaveSplineData(out);
           }
       return 1; 
    }
    virtual int read_data_state(FilterIn&si) {
       Stroka tmp;
       si >> tmp >> SplFileName;
       if (NoExternalFile())
           ReadSplineData(si);
       else {
           FilterTextIn in(~SplFileName);
           in.SetRefMask(SavableClass::SimpleEdit);
           ReadSplineData(in);
       }
       return 1;
    }
    Stroka MakeHelp() {
        Stroka ret("Spline calculation class. Internal spline data is stored in the separate file SplFileName. If SplFileName==SingleFileStorage - spline data is taken from the current config ");
        return ret;
    }

protected:
    Stroka SplFileName;
//    int SplineDim;
};



struct ISplineGenerator : SavableClass {
    virtual int  Generate(vector<vector<double> > &dat, int Cont = 0) = 0;
    virtual ISplineCalculator* GetCalculator() = 0;
    virtual void MakeGrid(vector<vector<double> > &grid, const vector<int> &numPnt, const vector<double> &bndMin, const vector<double> &bndMax) {
    }
    virtual int GetSplineDim() = 0;
    //    return SplineDim;
    //}
//private:
//    int SplineDim;
};
 









struct ISplineCalculatorStd : ISplineCalculator {
    virtual vector<double> GetBndMin() {
        return Min;
    };
    virtual vector<double> GetBndMax() {
        return Max;
    };
    virtual double GetMisfit() {
        return SplMisfit;
    }

    //virtual int  Evaluate (const vector<vector<double> > &dat, vector<double> &res);
    virtual void SaveSplineData(FilterOut &out) {
        out << " Description " << ~SplineDescr << ~SavableClass::EOLN();
        out << " SplMisfit " << SplMisfit << " SplineCoefsDim " << (int)SplineCoefs.size() << ~SavableClass::ppEOLN();
        for(int i = 0; i < (int)SplineCoefs.size() -1; i++) {
            out << " Axis " << i << " Size " << (int)SplineCoefs[i].size() << " Min " << Min[i] << " Max " << Max[i];
            if (i != SplineCoefs.size() - 2)
                out << SavableClass::EOLN();
            else
                out << SavableClass::mmEOLN();
        }
        for(size_t i = 0; i < SplineCoefs.size(); i++)
            DataManip::SaveTextVector(out, SplineCoefs[i]) << SavableClass::EOLN();
    }
    virtual void ReadSplineData(FilterIn &in) {
        Stroka tmp;
        int coefsSize;
        in >> tmp >> SplineDescr >> tmp >> SplMisfit >> tmp >> coefsSize;
        SplineCoefs.resize(coefsSize);
        Min.resize(coefsSize - 1);
        Max.resize(coefsSize - 1);
        for(int i = 0; i < coefsSize - 1; i++) {
            int s;
            in >> tmp >> tmp >> tmp >> s >> tmp >> Min[i] >> tmp >> Max[i];
            //SplineCoefs[i].resize(s);
        }
        for(int i = 0; i < coefsSize; i++)
            DataManip::ReadTextVector(in, SplineCoefs[i]);
    }


    void SetData(const char *splineFile, const char *splineDescr, const vector<double> &minV, const vector<double> &maxV, 
        double splMisfit, const vector<vector<double> > &splineCoefs) {
        DataManip::CopyVector(Min, minV);
        DataManip::CopyVector(Max, maxV);
        SplMisfit = splMisfit;
        SplineDescr = splineDescr;
        SplineFile = splineFile;
        DataManip::CopyVector(SplineCoefs, splineCoefs);
    }


protected:
    // last dat vector - function F[xi] = y val
    vector<double> Min, Max;
    double SplMisfit;
    Stroka SplineDescr, SplineFile;
    vector<vector<double> > SplineCoefs;
};









struct ISplineGeneratorStd : ISplineGenerator {
    ISplineGeneratorStd()
        : GenerationMisfit(1e-6)
        , SplineName("t.ispl")
        , SplineDescr("No_spline") {
    }

    virtual int  GenerateExtra(vector<vector<double> > &dat, double misfit) {
        double old = GenerationMisfit;
        GenerationMisfit = misfit;
        int ret;
        try { ret = Generate(dat, 0); } 
        catch (stdexception &/*ex*/) { }
        GenerationMisfit = old;
        return ret;
    }
    double GetMisfit() {
        return GenerationMisfit;
    }
    //int  Generate(vector<vector<double> > &dat, int Cont = -1);
    virtual ISplineCalculator* GetCalculator() {
        return SplineCalculator;
    }

    virtual void MakeGrid(vector<vector<double> > &grid, const vector<int> &numPnt, const vector<double> &bndMin, const vector<double> &bndMax) {
        int splineDim = GetSplineDim();
        if (numPnt.size() != splineDim || bndMin.size() != splineDim || bndMax.size() != splineDim)
            throw info_except("Grid size is zero\n");
        grid.resize(splineDim);
        for(int i = 0; i < splineDim; i++)
            grid[i] = DataManip::MakeStepVector(numPnt[i], bndMin[i], bndMax[i], 0, 0);
    }
    int read_data_state(FilterIn &in) {
        Stroka tmp;
        in >> tmp >> GenerationMisfit >> tmp >> SplineName >> tmp >> SplineDescr;
        return 1;
    }
    int save_data_state(FilterOut &out) {
        out << " GenerationMisfit " << GenerationMisfit << " SplineName " << SplineName << " SplineDescr " << SplineDescr;
        return 1;
    }
protected:
    Ref<ISplineCalculatorStd> SplineCalculator;
    double GenerationMisfit;
    Stroka SplineName, SplineDescr;
};


