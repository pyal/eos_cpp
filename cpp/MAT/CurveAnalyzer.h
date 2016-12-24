#pragma once

#include "lib/ref/data_manip.h"
#include "lib/ref/stroka.h"
#include <vector>
#include "specfunc.h"


using namespace std;

struct CurveAnalyzer {
    struct AtomCurve {
        double X0, A0, A1, Sigma;
        double B0, B1, B2;
        int iBeg, iEnd;
        double xEnd, xBeg;
        double MaxMisfit;
        double Delta2Model;   //, Delta2ModelPlus;
        void MakeModelB(double xLft, double xRgt, double yLft, double yRgt) {
            double x0 = xLft, x2 = xRgt;
            double y0 = yLft, y2 = yRgt;
            double denom = sqr(x0 - x2);
            B0 = (-6 * A0 * x0 * x2 + x2 * y0 * (2 * x0 + x2) + x0 * y2 * (x0 + 2 * x2)) /
                 denom;
            B1 = (6 * A0 * (x0 + x2) - 2 * (x2 * (2 * y0 + y2) + x0 * (y0 + 2 * y2))) /
                 denom;
            B2 = 3 * (y0 + y2 - 2 * A0) / denom;
        }

        double ClcModelB(double x) {
            //           double x0 = iBeg + (iEnd - iBeg - 1)*0.5;
            return B0 + B1 * x + B2 * x * x;
        }

        void ClcParams(
            int beg,
            int end,
            const vector<double> &xar,
            const vector<double> &yar,
            double maxmisfit) {
            Set((xar[beg] + xar[end - 1]) * 0.5,
                (yar[beg] + yar[end - 1]) * 0.5,
                0,
                0,
                beg,
                end,
                xar[beg],
                xar[end - 1],
                maxmisfit);
            if(beg == end - 1)
                return;
            double a0, a1, x0;
            vector<double> modelmisfit = ClcDerivConst(x0, a0, a1, beg, end, xar, yar);
            Delta2Model = 0;
            for(size_t i = 0; i < modelmisfit.size(); i++)
                Delta2Model += sqr(modelmisfit[i]);
            Set(x0,
                a0,
                a1,
                sqrt(Delta2Model / modelmisfit.size()),
                beg,
                end,
                xar[beg],
                xar[end - 1],
                maxmisfit);
        }

        static bool SameModel(AtomCurve &fst, AtomCurve &sec, double Coef) {
            double a1 = fst.A0, a2 = sec.A0, s1 = fst.Sigma, s2 = sec.Sigma;
            //                    n1 = (fst.iEnd - fst.iBeg), n2 = (sec.iEnd - sec.iBeg);
            double d2 = fabs(a1 - a2);
            if(d2 < Coef * s1 && d2 < Coef * s2)
                return 1;
            return 0;
        }
        double ClcModel(double x) {
            double x0 = iBeg + (iEnd - iBeg - 1) * 0.5;
            return A0 + A1 * (x - x0);
        }

    private:
        void Set(
            double x0,
            double a0,
            double a1,
            double sigma,
            int beg,
            int end,
            double xbeg,
            double xend,
            double maxmisfit) {
            iBeg = beg;
            iEnd = end;
            xBeg = xbeg;
            xEnd = xend;
            X0 = x0;
            A0 = a0;
            A1 = a1;
            Sigma = sigma;
            MaxMisfit = maxmisfit;
        }
        vector<double> ClcDerivConst(
            double &x0,
            double &a0,
            double &a1,
            int beg,
            int end,
            const vector<double> &xar,
            const vector<double> &yar) {
            int N = end - beg;
            double Mean = (N - 1) * 0.5;
            x0 = beg + Mean;
            double sXY = 0, sX2 = 0, sY = 0;
            for(int i = beg; i < end; i++) {
                double x = i - x0, y = yar[i];
                sY += y;
                sXY += (x * y);
                sX2 += x * x;
            }
            a0 = sY / N;
            a1 = sXY / sX2;
            vector<double> vec;
            //cout<<beg<<" "<<end<<" "<<a0<<" "<<a1<<"\n";
            DataManip::CopyVector(vec, yar, beg, end);
            //x0 = Mean;
            //x0 = Mean;
            for(size_t i = 0; i < vec.size(); i++) {
                //vec[i] -= ClcModel(i + iBeg);
                vec[i] -= (a0 + a1 * (i - x0 + beg));
            }
            return vec;
        }
    };
    struct AtomOpt {
        AtomCurve *Fst, *Sec;
        vector<double> *Xar, *Yar;
        void SetDat(
            AtomCurve &fst,
            AtomCurve &sec,
            vector<double> &xar,
            vector<double> &yar) {
            Fst = &fst;
            Sec = &sec;
            Xar = &xar;
            Yar = &yar;
        }
        double Clc1(AtomCurve *atom, int iPnt) {
            double mist;
            mist = sqr(atom->ClcModel(iPnt) - (*Yar)[iPnt]);
            return mist;
        }
        double Clc(int pnt, bool PlusStp) {
            double m1 = Clc1(Fst, pnt), m2 = Clc1(Sec, pnt);
            return (!PlusStp) ? m1 - m2 : m2 - m1;
        }
        void SetMiddle(int Mid) {
            Fst->ClcParams(Fst->iBeg, Mid, *Xar, *Yar, Fst->MaxMisfit);
            Sec->ClcParams(Mid, Sec->iEnd, *Xar, *Yar, Sec->MaxMisfit);
        }
        int OptBnd(
            AtomCurve &fst,
            AtomCurve &sec,
            vector<double> &xar,
            vector<double> &yar) {
            SetDat(fst, sec, xar, yar);
            int Mid = Fst->iEnd;
            double mp = Clc(Mid + 1, true), mm = Clc(Mid - 1, false);
            if(mp <= 0 && mm <= 0)
                return 1;
            double Stp = (mp > mm) ? 1 : -1, shift;
            Mid += Stp;
            do {
                Mid += Stp;
                shift = Clc(Mid, (Stp > 0));
            } while(shift > 0 && abs(Mid - Fst->iBeg) > 2 && abs(Mid - Sec->iEnd) > 2);
            //cout<<Fst->iBeg<<" "<<Fst->iEnd<<" "<<Sec->iEnd<<" "<<Mid<<"\n";
            SetMiddle(Mid);
            //cout<<Fst->iBeg<<" "<<Fst->iEnd<<" "<<Sec->iEnd<<" "<<Mid<<"\n";
            if(abs(Mid - Sec->iEnd) <= 2)
                return 0;
            return 1;
        }
    };
    vector<AtomCurve> Atoms;
    //CurveAnalyzer& InitDatSimple(vector<double> &xar, vector<double> &yar, int StpL, double MaxMisfit, int MinNumPnt, double SameCoef){
    //    int num = (int)xar.size()/StpL;
    //    vector<double> pnts = DataManip::MakeStepVector(num, 0, xar.size(), 0, 0);
    //    Atoms.resize(pnts.size()-1);
    //    for(size_t i = 0; i < Atoms.size(); i++)
    //        Atoms[i].ClcParams(pnts[i], pnts[i+1], xar, yar, MaxMisfit);
    //    return *this;
    //}
    CurveAnalyzer &InitDat(
        vector<double> &xar,
        vector<double> &yar,
        int StpL,
        double MaxMisfit,
        int MaxNumPnt,
        double SameCoef) {
        int num = max<int>(2, (int)xar.size() / StpL);
        vector<double> pnts = DataManip::MakeStepVector(num, 0, xar.size(), 0, 0);
        vector<AtomCurve> atoms(pnts.size() - 1);
        for(size_t i = 0; i < atoms.size(); i++)
            atoms[i].ClcParams(pnts[i], pnts[i + 1], xar, yar, MaxMisfit);
        AtomOpt optim;
        Atoms.clear();
        Atoms.push_back(AtomCurve());
        Atoms[0].ClcParams(atoms[0].iBeg, atoms[0].iEnd, xar, yar, MaxMisfit);
        for(size_t i = 1; i < atoms.size(); i++) {
            int lastres = (int)Atoms.size() - 1;
            int regbeg = atoms[i].iBeg;
            //cout<<lastres<<"\n";
            while(i < atoms.size() &&
                  AtomCurve::SameModel(
                      Atoms[lastres], atoms[i], SameCoef / sqrt((double)StpL)) &&
                  atoms[i].iEnd - regbeg < MaxNumPnt) {
                //while (i < atoms.size() && AtomCurve::SameModel(Atoms[lastres], atoms[i], MaxMisfit)){
                Atoms[lastres].ClcParams(
                    Atoms[lastres].iBeg, atoms[i].iEnd, xar, yar, MaxMisfit);
                i++;
            }
            //cout<<Atoms[lastres].iBeg<<" "<<Atoms[lastres].iEnd<<"\n";
            if(i >= atoms.size())
                break;
            Atoms.push_back(AtomCurve());
            Atoms[lastres + 1].ClcParams(
                atoms[i].iBeg, atoms[i].iEnd, xar, yar, MaxMisfit);
            //cout<<lastres+1<<" plus\n";
            //cout<<Atoms[lastres+1].iBeg<<" "<<Atoms[lastres+1].iEnd<<" plus\n";
            //if (atoms[i].iEnd-regbeg>MaxNumPnt)
            //    break;
            while(!optim.OptBnd(Atoms[lastres], Atoms[lastres + 1], xar, yar)) {
                if(i == atoms.size() - 1) {
                    Atoms.resize(Atoms.size() - 1);
                    Atoms[lastres].ClcParams(
                        Atoms[lastres].iBeg, atoms[i].iEnd, xar, yar, MaxMisfit);
                    return *this;
                } else {
                    Atoms[lastres].ClcParams(
                        Atoms[lastres].iBeg, atoms[i].iEnd, xar, yar, MaxMisfit);
                    i++;
                    Atoms[lastres + 1].ClcParams(
                        atoms[i - 1].iEnd, atoms[i].iEnd, xar, yar, MaxMisfit);
                }
            }
        }
        MakeModelB();
        return *this;
    }
    void MakeModelB() {
        for(size_t i = 0; i < Atoms.size(); i++) {
            double x0 = Atoms[i].iBeg - 0.5, x2 = Atoms[i].iEnd - 0.5;
            double y0 = Atoms[i].ClcModel(x0), y2 = Atoms[i].ClcModel(x2);
            y0 = (i != 0) ? 0.5 * (Atoms[i - 1].ClcModel(x0) + y0) : y0;
            y2 = (i != Atoms.size() - 1) ? 0.5 * (Atoms[i + 1].ClcModel(x2) + y2) : y2;
            Atoms[i].MakeModelB(x0, x2, y0, y2);
        }
    }
    vector<vector<double>> FormResult(
        const vector<double> &Xval,
        const vector<double> &Yval,
        Stroka &Desc) {
        Desc = "X Y mY mM mD1 mMis MisModel";
        vector<vector<double>> res(8);
        for(size_t iatom = 0; iatom < Atoms.size(); iatom++) {
            for(int i = Atoms[iatom].iBeg; i < Atoms[iatom].iEnd; i++) {
                AtomCurve &atom = Atoms[iatom];
                res[0].push_back(Xval[i]);
                res[1].push_back(Yval[i]);
                res[2].push_back(atom.ClcModel(i));
                res[3].push_back(atom.ClcModelB(i));
                res[4].push_back(atom.A0);
                res[5].push_back(atom.A1);
                res[6].push_back(atom.Sigma);
                res[7].push_back(atom.Delta2Model);
            }
        }
        //size_t i = Xval.size()-1;
        //        res[0].push_back(Xval[i]);
        //        res[1].push_back(Yval[i]);
        //        res[2].push_back(Yval[i]);
        //        res[3].push_back(Yval[i]);
        //        res[4].push_back(0);
        //        res[5].push_back(0);
        //        res[6].push_back(0);
        //        res[7].push_back(0);

        return res;
    }
};
