
#pragma once

//#include "spl_interface.h"
#include "spl_1d.h"
#include "func_cvt.h"


struct TEncodedSplineCalculator : ISplineCalculator {
    TEncodedSplineCalculator() : ISplineCalculator() {}
    TEncodedSplineCalculator(
        ISplineCalculator *spl,
        const vector<Ref<IFunctionConverter>> &cvtVec)
        : ISplineCalculator(), SplineClc(spl), CvtVec(cvtVec) {
        ClcDeriveVector();
    }

    virtual int GetSplineDim() {
        if(!SplineClc)
            return -1;
        return SplineClc->GetSplineDim();
    }
    vector<double> GetBndMin() {
        return DecodePnt(SplineClc->GetBndMin());
    }
    vector<double> GetBndMax() {
        return DecodePnt(SplineClc->GetBndMax());
    }
    double GetMisfit() {
        return SplineClc->GetMisfit();
    }

    virtual int Evaluate(const vector<double> &pnt, double &res) {
        if(pnt.size() + 1 != CvtVec.size())
            throw info_except(
                "Bad pnt(%d) - evaluate(%d) dims\n", pnt.size(), CvtVec.size());
        vector<double> encPnt(pnt.size()), inPnt;
        double encRes, addPnt;
        SetPntInBound(addPnt, inPnt, pnt);
        for(size_t i = 0; i < pnt.size(); i++)
            encPnt[i] = CvtVec[i]->Encode(inPnt[i], "Bad encode pnt generator");
        int ret = SplineClc->Evaluate(encPnt, encRes);
        res = CvtVec[pnt.size()]->Decode(encRes, "Bad decode pnt result") + addPnt;
        return !ret;
    }
    virtual int Evaluate(const vector<vector<double>> &dat, vector<double> &res) {
        if(dat.size() + 1 != CvtVec.size())
            throw info_except(
                "Bad dat(%d) - evaluate(%d) dims\n", dat.size(), CvtVec.size());
        vector<double> encVec, inVec, addBnd(dat[0].size());
        DataManip::SetVector(addBnd, 0);
        vector<vector<double>> encDat;
        for(size_t i = 0; i < dat.size(); i++) {
            SetDatInBound(addBnd, inVec, dat[i], (int)i);
            CvtVec[i]->Encode(inVec, encVec, "Bad encode generator");
            encDat.push_back(encVec);
        }
        int ret = SplineClc->Evaluate(encDat, encVec);
        CvtVec[dat.size()]->Decode(encVec, res, "Bad decode result");
        res = DataManip::operator+(res, addBnd);   // No bnd correction
        return !ret;
    }

    virtual void SaveSplineData(FilterOut &out) {
        out << " FunctionConverter { " << SavableClass::ppEOLN();
        for(size_t i = 0; i < CvtVec.size(); i++)
            out << CvtVec[i] << SavableClass::EOLN();
        out << " } " << SavableClass::mmEOLN();
        out << " DeriveVecrtor ";
        DataManip::SaveTextVector(out, DeriveVector);
        out << " Spline " << SplineClc->class_name();
        SplineClc->SaveSplineData(out);
    }
    virtual void ReadSplineData(FilterIn &in) {
        Stroka tmp;
        in >> tmp;
        SavableClass::ExeptionCheck(in, "{", " FunctionConverter list test {");
        Ref<IFunctionConverter> cvt;
        CvtVec.clear();
        while(!SavableClass::TestNextChar(in, '}', 1)) {
            in >> cvt;
            CvtVec.push_back(cvt);
        }
        in >> tmp;
        DataManip::ReadTextVector(in, DeriveVector);
        in >> tmp >> tmp;
        ClassDesc *cd = ClassDesc::name_to_class_desc(~tmp);
        SplineClc << cd->create();
        if(!SplineClc)
            throw info_except("Bad Spline calculator \n");
        SplineClc->ReadSplineData(in);
    }
    Stroka MakeHelp() {
        Stroka ret = ISplineCalculator::MakeHelp();
        ret +=
            "\nInput output data is encoded by the function converters. Cuurently present converters : \n" +
            SavableClass::HelpForCategory("FunctionConverter_category");
        return ret;
    }

private:
    Ref<ISplineCalculator> SplineClc;
    vector<Ref<IFunctionConverter>> CvtVec;
    vector<double> DeriveVector;

    void SetPntInBound(double &addPnt, vector<double> &inPnt, const vector<double> &pnt) {
        vector<double> minV = GetBndMin(), maxV = GetBndMax();
        inPnt.resize(minV.size());
        addPnt = 0;
        for(size_t i = 0; i < pnt.size(); i++) {
            double val = pnt[i];
            if(val > maxV[i]) {
                addPnt += DeriveVector[i] * (val - maxV[i]);
                val = maxV[i];
            }
            if(val < minV[i]) {
                addPnt += DeriveVector[i] * (val - minV[i]);
                val = minV[i];
            }
            inPnt[i] = val;
        }
    }
    void SetDatInBound(
        vector<double> &addBnd,
        vector<double> &inDat,
        const vector<double> &dat,
        int col) {
        vector<double> minV = GetBndMin(), maxV = GetBndMax();
        double min = minV[col], max = maxV[col];
        double deriv = DeriveVector[col];
        inDat.resize(dat.size());
        for(size_t i = 0; i < dat.size(); i++) {
            double val = dat[i];
            if(val > max) {
                addBnd[i] += deriv * (val - max);
                val = max;
            }
            if(val < min) {
                addBnd[i] += deriv * (val - min);
                val = min;
            }
            inDat[i] = val;
        }
    }
    void ClcDeriveVector() {
        DeriveVector.resize(CvtVec.size() - 1);
        DataManip::SetVector(DeriveVector, 0);
        VecCl minV(GetBndMin()), maxV(GetBndMax());
        VecCl center = (minV + maxV) * 0.5, step = (maxV - minV) * 0.1;
        vector<vector<double>> dat(DeriveVector.size());
        for(int i = 1; i <= (int)DeriveVector.size(); i++) {
            VecCl up(center), down(center);
            up[i] += step[i];
            down[i] -= step[i];
            AddPnt(dat, down);
            AddPnt(dat, up);
        }
        vector<double> res;
        //cout << " dat ";DataManip::operator <<(cout, dat[0]) << "\n";
        if(!Evaluate(dat, res))
            throw info_except("could not calculate derivative\n");
        for(size_t i = 0; i < DeriveVector.size(); i++) {
            double dF = res[2 * i + 1] - res[2 * i];
            DeriveVector[i] = 0.5 * dF / step[(int)i + 1];
        }
        //cout << " Center " << center << " step " << step <<" res "; DataManip::operator<<(cout, res) << "\n";
        //cout << "got Derive ";DataManip::operator<<(cout, DeriveVector) << "\n";
    }
    void AddPnt(vector<vector<double>> &dat, VecCl &pnt) {
        for(int i = 0; i < (int)pnt.Dim(); i++)
            dat[i].push_back(pnt[i + 1]);
    }
    vector<double> DecodePnt(const vector<double> &pnt) {
        vector<double> ret(pnt);
        for(size_t i = 0; i < CvtVec.size() - 1; i++)
            ret[i] = CvtVec[i]->Decode(ret[i], "Bad decode pnt");
        return ret;
    }
};


struct TEncodedSplineGenerator : ISplineGenerator {
    TEncodedSplineGenerator();
    //    : ISplineGenerator()
    //    , SplineGen(new TSpline1DGenerator) {
    //    SetDumpCvtVec();
    //}

    TEncodedSplineGenerator(ISplineGenerator *splGen)
        : ISplineGenerator(), SplineGen(splGen) {
        SetDumpCvtVec();
    }
    virtual int GetSplineDim() {
        if(!SplineGen)
            return -1;
        return SplineGen->GetSplineDim();
    }
    virtual int Generate(vector<vector<double>> &dat, int cont = 0) {
        if(dat.size() != CvtVec.size())
            throw info_except(
                "Bad dat(%i) - converter(%i) dims, have to be %i\n",
                dat.size(),
                CvtVec.size(),
                GetSplineDim() + 1);
        vector<double> eVec;
        vector<vector<double>> eDat;
        for(size_t i = 0; i < dat.size(); i++) {
            CvtVec[i]->SetAutoEncode(dat[i]);
            CvtVec[i]->Encode(dat[i], eVec, "Bad encode generator");
            eDat.push_back(eVec);
        }
        return SplineGen->Generate(eDat, cont);
    }
    ISplineCalculator *GetCalculator() {
        return new TEncodedSplineCalculator(SplineGen->GetCalculator(), CvtVec);
    };
    void MakeGrid(
        vector<vector<double>> &grid,
        const vector<int> &numPnt,
        const vector<double> &bndMin,
        const vector<double> &bndMax) {
        vector<double> eVec;
        grid.clear();
        if(numPnt.size() != bndMin.size() || numPnt.size() != bndMax.size() ||
           numPnt.size() + 1 != CvtVec.size())
            throw info_except(
                "Bad sizes num(%d) min(%d) max(%d) cvt(%d)\n",
                numPnt.size(),
                bndMin.size(),
                bndMax.size(),
                CvtVec.size());
        for(size_t i = 0; i < CvtVec.size() - 1; i++) {
            VecCl vec = VecCl::MakeStepVector(
                numPnt[i], CvtVec[i]->Encode(bndMin[i]), CvtVec[i]->Encode(bndMax[i]), 0);
            CvtVec[i]->Decode(vec.Copy2Vector(), eVec, "grid generation");
            grid.push_back(eVec);
        }
    }
    virtual void MakeGrid(
        vector<double> &grid,
        int axisNum,
        int numPnt,
        double bndMin,
        double bndMax) {
        vector<double> eVec;
        //grid.clear();
        if(axisNum >= (int)CvtVec.size())
            throw info_except("Bad axisNum(%d)  cvt_dim(%d)\n", axisNum, CvtVec.size());
        grid = CvtVec[axisNum]->MakeEncodedStepVector(bndMin, bndMax, numPnt, 0);
        //VecCl vec = VecCl::MakeStepVector(numPnt, CvtVec[axisNum]->Encode(bndMin), CvtVec[axisNum]->Encode(bndMax), 0);
        //CvtVec[axisNum]->Decode(vec.Copy2Vector(), grid, "grid generation");
    }
    int read_data_state(FilterIn &in) {
        Stroka tmp;
        in >> tmp;
        SavableClass::ExeptionCheck(in, "{", " FunctionConverter list test {");
        Ref<IFunctionConverter> cvt;
        CvtVec.clear();
        while(!SavableClass::TestNextChar(in, '}', 1)) {
            in >> tmp >> cvt;
            CvtVec.push_back(cvt);
        }
        in >> tmp >> SplineGen;
        if(!SplineGen || GetSplineDim() + 1 != CvtVec.size())
            throw info_except(
                "Bad Spline generator or bad cvt vec dim(%i) have to be %i\n",
                CvtVec.size(),
                GetSplineDim() + 1);
        return 1;
    }
    int save_data_state(FilterOut &out) {
        out << " FunctionConverterX..Z { " << SavableClass::ppEOLN();
        for(int i = 0; i < (int)CvtVec.size() - 1; i++)
            out << ~(Stroka(" CvtX_") + Stroka::Int2Str(i) + " ") << CvtVec[i]
                << SavableClass::EOLN();
        out << " CvtY " << CvtVec[CvtVec.size() - 1] << SavableClass::EOLN();
        out << " } " << SavableClass::mmEOLN();
        out << " SplineGen " << SplineGen;
        return 1;
    }
    Stroka MakeHelp() {
        Stroka ret = ISplineGenerator::MakeHelp();
        ret +=
            "\nInput output data is encoded by the function converters. Cuurently present converters : \n" +
            SavableClass::HelpForCategory("FunctionConverter_category");
        return ret;
    }

private:
    Ref<ISplineGenerator> SplineGen;
    vector<Ref<IFunctionConverter>> CvtVec;
    void SetDumpCvtVec() {
        CvtVec.clear();
        if(!SplineGen)
            return;
        for(int i = 0; i < SplineGen->GetSplineDim() + 1; i++)
            CvtVec.push_back(new TExpConverter);
    }
};
