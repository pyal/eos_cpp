#pragma once

#include "gasd/polygon/lib/PolyMarchBase.h"
#include "mat/spl_fac.h"
#include "mat/fun_fac.h"
#include "urs/matt_fac.h"
#include "lib/ref/file_manip.h"

namespace NPolygon {


    struct TPolyMarchPlusBase : TPolyMarchRegionBase {
    protected:
        Ref<TPolyMarchRegionBase> Marcher;

    public:
        TPolyMarchPlusBase();
        //	: Marcher(new NPolygon::TMarchWilkins) {
        //}
        virtual double GetMaxTimeStp(TPolyRegion *reg) {
            return Marcher->GetMaxTimeStp(reg);
        }
        virtual void MakeTimeStep(TPolyRegion *reg, double curTime, double timeStp) {
            Marcher->MakeTimeStep(reg, curTime, timeStp);
        }
        void RebuildBounds(TPolyRegion *reg) {
            Marcher->RebuildBounds(reg);
        }
        virtual void InitBeforeBounds(TPolyRegion *reg) {
            Marcher->InitBeforeBounds(reg);
        }
        virtual void InitAfterBounds(TPolyRegion *reg) {
            Marcher->InitAfterBounds(reg);
        }
        virtual void SetNewTimeStp(double curTime, double timeStp) {
            Marcher->SetNewTimeStp(curTime, timeStp);
        }
        int save_data_state(FilterOut &so) {
            so << " MarchRegion " << Marcher;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> Marcher;
            return 1;
        }
        Stroka MakeHelp() {
            Stroka help = "Base class - not to be used....\n";
            return help;
        }
    };

    struct TPolyMarchPlusE : TPolyMarchPlusBase {
    private:
        double YmeanL;
        double IntegralInt;   //, Ysigma2Norm, Xsigma2Norm;
        double Tmin, Tmax,
            TsplMisfit;   // use for march calculation (not original int, beg, end)
        double Xsigma, Xcenter,
            Ysigma;   // use for march calculation (not original int, beg, end)
        Stroka TimeDepFile;
        Stroka RegCoefName, EnergyAddName, PosName;
        //Ref<TPolyMarchRegionBase> Marcher;
        double MinReadT, MaxReadT;//, IntegralReadT;
        double TimeIntNormCoef;   //
        CurveSpline TimeDepSpl;

        struct TGausFunc : OneVarFunction {
            double Center, Sigma, Norm;
            TGausFunc(double center, double sigma)
                : OneVarFunction(), Center(center), Sigma(sigma) {
                Norm = sqrt(M_PI * Sigma);
            }
            double Calculate(double x) {
                if(x < Center - 3 * Sigma || x > Center + 3 * Sigma)
                    return 0;
                double res = exp(-sqr((x - Center) / Sigma));
                return res / Norm;   // integral int is 1
            }
        };


        double Tint(
            double sx,
            double sMin,
            double sMax,
            double gMin,
            double gMax,
            CurveSpline &gSpl) {
            if(sx < sMin || sx > sMax)
                return 0;
            double shift = (sx - sMin) / (sMax - sMin);   // / sMax;
            shift = gMin + shift * (gMax - gMin);
            double spaceI = gSpl.Evaluate(shift);
            //cout << sx << " " << spaceI << " " << 0 << "\n";
            return spaceI;
        }
        void ClcAddition(TPolyRegion *reg, double curTime, double curStp) {
            double regCoef = reg->MapDouble[RegCoefName];
            double TimeCoef =
                Tint(curTime + 0.5 * curStp, Tmin, Tmax, MinReadT, MaxReadT, TimeDepSpl) *
                curStp * TimeIntNormCoef * regCoef;
            TRegionBounds bndCent(0, -1);
            double *pos = (double *)reg->Grid.GetMaskedData(bndCent, PosName)
                              .Start()
                              .GetElementPtr();
            double *ener = (double *)reg->Grid.GetMaskedData(bndCent, EnergyAddName)
                               .Start()
                               .GetElementPtr();
            int size = reg->Grid.GetMaskedData(bndCent, PosName).Start().Size();
            TGausFunc gaus(Xcenter, Xsigma);
            for(int i = 0; i < size; i++) {
                ener[i] += gaus.Calculate((pos[i] + pos[i + 1]) * 0.5) * TimeCoef;
            }
        }

    public:
        TPolyMarchPlusE();
        //virtual double GetMaxTimeStp(TPolyRegion *reg) {
        //    return Marcher->GetMaxTimeStp(reg);
        //}
        virtual void MakeTimeStep(TPolyRegion *reg, double curTime, double timeStp) {
            TPolyMarchPlusBase::MakeTimeStep(reg, curTime, timeStp);
            //Marcher->MakeTimeStep(reg, curTime, timeStp);
            ClcAddition(reg, curTime, timeStp);
        }
        //      void RebuildBounds(TPolyRegion *reg) {
        //          Marcher->RebuildBounds(reg);
        //      }
        //virtual void InitBeforeBounds(TPolyRegion *reg){
        //	Marcher->InitBeforeBounds(reg);
        //}
        //virtual void InitAfterBounds(TPolyRegion *reg){
        //	Marcher->InitAfterBounds(reg);
        //}
        //virtual void SetNewTimeStp(double curTime, double timeStp){
        //	Marcher->SetNewTimeStp(curTime, timeStp);
        //}
        int save_data_state(FilterOut &so) {
            so << " IntegralInt "
               << IntegralInt;   // <<" Ysigma2Norm " << Ysigma2Norm <<" Xsigma2Norm " << Xsigma2Norm << SavableClass::EOLN();
            so << " TimeDepFile " << TimeDepFile << " Tmin " << Tmin << " Tmax " << Tmax
               << " TsplMisfit " << TsplMisfit << SavableClass::EOLN();
            so << " Xsigma " << Xsigma << " Xcenter " << Xcenter << " Ysigma " << Ysigma
               << SavableClass::EOLN();
            so << " PosName " << PosName << " EnergyAddName " << EnergyAddName
               << " RegCoefName " << RegCoefName << SavableClass::EOLN();
            return TPolyMarchPlusBase::save_data_state(so);
            //so << " MarchRegion " << Marcher;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> IntegralInt;   // >> tmp >> Ysigma2Norm >> tmp >> Xsigma2Norm;
            si >> tmp >> TimeDepFile >> tmp >> Tmin >> tmp >> Tmax >> tmp >> TsplMisfit;
            si >> tmp >> Xsigma >> tmp >> Xcenter >> tmp >> Ysigma;
            si >> tmp >> PosName >> tmp >> EnergyAddName >> tmp >> RegCoefName;
            TPolyMarchPlusBase::read_data_state(si);
            //si>>tmp>>Marcher;

            CurveSpline::SplineFromData(
                ~TimeDepFile, TimeDepSpl, TsplMisfit, "Time dependence spline");
            map<Stroka, double> bnd = TimeDepSpl.GetBoundary();
            MinReadT = bnd["Xmin"];
            MaxReadT = bnd["Xmax"];
            double tNorm = SplineXIO::Integrate(MinReadT, MaxReadT, 1e-7, &TimeDepSpl);
            //tNorm *= (Tmin - Tmax) / (MinReadT - MaxReadT) ;
            tNorm *= (Tmax - Tmin);
            tNorm *= TGausFunc(0, Ysigma).Integral(0, YmeanL) *
                     2;   // / TGausFunc(0, Ysigma).Integral(0, 10 * Ysigma);
            TimeIntNormCoef =
                1e-9 / tNorm * IntegralInt * 4637;   //3.326 kJ for he if int 1e9;
            //TimeIntNormCoef = 1e-9 / tNorm * IntegralInt * 2.71;//3.326 kJ for he if int 1e9;
            //TimeIntNormCoef *= SpaceInt * TimeInt;
            return 1;
        }
        Stroka MakeHelp() {
            Stroka help =
                "Class for external energy input to the marcher scheme. Uses marcher for time step clc.";
            help += " Marcher is of the same type - NPolygon::RegionMarch_category.\n";
            //help += " Registered NPolygon::RegionMarch_category objects:\n\n~+\n" + SavableClass::HelpForCategory("NPolygon::RegionMarch_category");
            //help += "~-\n";
            return help;
        }
    };

    struct TPolyMarchPlusGase : TPolyMarchPlusBase {
    private:
        int SrcRegion, DstRegion;
        Ref<CurveSpline> T_DensLow, T_DensHigh, T_ELow;
        Stroka EnerName, DensName, EOSName, PresName, MassName, SplDataName, PosName,
            VelName;
        double DensLowCoef, SplitMassCoef;
        int InLim(
            double dens,
            double ener,
            MatterIO *mat,
            double &resDens,
            double &resE) {
            map<Stroka, double> bndHigh = T_DensHigh->GetBoundary();
            map<Stroka, double> bndLow = T_DensLow->GetBoundary();
            //cout << "Testing boiling r=" << dens << " e=" << ener << " vacuum r=" << resDens << " e=" << resE <<"\n";
            double t = mat->Temperature(dens, ener);
            if(t > bndLow["Xmax"] || t < bndLow["Xmin"])
                return 0;
            double d, e;
            if(dens < (d = T_DensLow->Evaluate(t)) || dens > T_DensHigh->Evaluate(t))
                return 0;
            d *= DensLowCoef;
            if(d < resDens)
                return 0;
            e = T_ELow->Evaluate(t);
            //cout << " got in lim dens " << dens << " temp " << t << " lowD*coef " << d << " lowE " << e << " wasD " << resDens << " wasE " << resE << "\n";
            resE = ((d - resDens) * e + resDens * resE) / d;
            resDens = d;
            return 1;
        }
        void CheckBounds(TPolyRegion *src, TPolyRegion *dst, int LeftToRight) {
            TRegionBounds bndRgt(TRegionBounds::BNDBAD, 0),
                bndRgtMin(TRegionBounds::BNDBAD, -1), bndLft(0, TRegionBounds::BNDBAD),
                bndLftMin(-1, TRegionBounds::BNDBAD),
                bndLftPlus(1, TRegionBounds::BNDBAD);
            double bndDens, bndE, setDens, setE;
            if(LeftToRight) {
                bndDens = *(double *)TGridMaskedData::TIterator(
                               src->Grid.GetMaskedData(bndRgtMin, DensName))
                               .GetElementPtr();
                bndE = *(double *)TGridMaskedData::TIterator(
                            src->Grid.GetMaskedData(bndRgtMin, EnerName))
                            .GetElementPtr();
                //bndDens = src->Grid.GetMaskedData(bndRgtMin, DensName);
                //bndE = src->Grid.GetMaskedData(bndRgtMin, EnerName);
                MatterIO *mat =
                    SavableClass::TestType<MatterIO>(src->MapSavableVar[EOSName]);
                setDens = *(double *)TGridMaskedData::TIterator(
                               dst->Grid.GetMaskedData(bndLft, DensName))
                               .GetElementPtr();
                setE = *(double *)TGridMaskedData::TIterator(
                            dst->Grid.GetMaskedData(bndLft, EnerName))
                            .GetElementPtr();
                if(!InLim(bndDens, bndE, mat, setDens, setE))
                    return;
                dst->Grid.GetMaskedData(bndLft, DensName) = setDens;
                dst->Grid.GetMaskedData(bndLft, EnerName) = setE;
                dst->Grid.GetMaskedData(bndLft, PresName) = mat->Pressure(setDens, setE);
                dst->Grid.GetMaskedData(bndLft, MassName) =
                    (dst->Grid.GetMaskedData(bndLftPlus, PosName) -
                     dst->Grid.GetMaskedData(bndLft, PosName)) *
                    setDens;
                CheckSplit(dst);
            } else {
                throw info_except(" not implemented yet!!!\n");
            }
        }
        void CheckSplit(TPolyRegion *reg) {
            TRegionBounds bndM(0, -1);
            double maxM = 0, minM = 1e100;
            TGridMaskedData::TIterator minIt, maxIt;
            for(TGridMaskedData::TIterator it(reg->Grid.GetMaskedData(bndM, MassName));
                it.IsOk();
                ++it) {
                double m = *(double *)(it).GetElementPtr();
                if(m > maxM) {
                    maxM = m;
                    maxIt = it;
                }
                if(m < minM) {
                    minM = m;
                    minIt = it;
                }
            }
            if(maxM > SplitMassCoef * minM)
                return;
            if(!(minIt + 1).IsOk())
                --minIt;
            MakeSplit(reg, maxIt, minIt);
        }
        void MakeSplit(
            TPolyRegion *reg,
            TGridMaskedData::TIterator splitCell,
            TGridMaskedData::TIterator lftJoin) {
            double splitMass =
                *(double *)splitCell.GetElementPtr(reg->Grid.GetVar(MassName)) * 0.5;
            double mLft = *(double *)lftJoin.GetElementPtr(reg->Grid.GetVar(MassName));
            double mRgt =
                *(double *)(lftJoin + 1).GetElementPtr(reg->Grid.GetVar(MassName));
            double joinedMass = mLft + mRgt;

            double splitDens =
                *(double *)splitCell.GetElementPtr(reg->Grid.GetVar(DensName));
            double joinedDens =
                joinedMass /
                (mLft / *(double *)lftJoin.GetElementPtr(reg->Grid.GetVar(DensName)) +
                 mRgt /
                     *(double *)(lftJoin + 1).GetElementPtr(reg->Grid.GetVar(DensName)));

            double splitE =
                *(double *)splitCell.GetElementPtr(reg->Grid.GetVar(EnerName));
            double joinedE =
                (mLft * *(double *)lftJoin.GetElementPtr(reg->Grid.GetVar(EnerName)) +
                 mRgt *
                     *(double *)(lftJoin + 1).GetElementPtr(reg->Grid.GetVar(EnerName))) /
                joinedMass;

            double splitU = *(double *)splitCell.GetElementPtr(reg->Grid.GetVar(VelName));
            double joinedU =
                (mLft * *(double *)lftJoin.GetElementPtr(reg->Grid.GetVar(VelName)) +
                 mRgt *
                     *(double *)(lftJoin + 1).GetElementPtr(reg->Grid.GetVar(VelName))) /
                joinedMass;

            double splitP =
                *(double *)splitCell.GetElementPtr(reg->Grid.GetVar(PresName));
            double joinedP =
                (mLft * *(double *)lftJoin.GetElementPtr(reg->Grid.GetVar(PresName)) +
                 mRgt *
                     *(double *)(lftJoin + 1).GetElementPtr(reg->Grid.GetVar(PresName))) /
                joinedMass;

            double posLft = *(double *)splitCell.GetElementPtr(reg->Grid.GetVar(PosName));
            double posRgt =
                *(double *)(splitCell + 1).GetElementPtr(reg->Grid.GetVar(PosName));

            SplitJoin(
                reg, splitCell, lftJoin, MassName, splitMass, splitMass, joinedMass, 1);
            SplitJoin(
                reg, splitCell, lftJoin, DensName, splitDens, splitDens, joinedDens, 1);
            SplitJoin(reg, splitCell, lftJoin, EnerName, splitE, splitE, joinedE, 1);
            SplitJoin(reg, splitCell, lftJoin, VelName, splitU, splitU, joinedU, 1);
            SplitJoin(reg, splitCell, lftJoin, PresName, splitE, splitP, joinedP, 1);
            SplitJoin(
                reg,
                splitCell,
                lftJoin,
                PosName,
                (posLft + posRgt) * 0.5,
                posRgt,
                joinedE,
                0);
        }
        void SplitJoin(
            TPolyRegion *reg,
            TGridMaskedData::TIterator splitCell,
            TGridMaskedData::TIterator lftJoin,
            const Stroka &varName,
            double splitLft,
            double splitRgt,
            double joinedVal,
            int setJoined = 1) {
            TRegionBounds bndFull(
                -reg->Grid.GetBoundarySize(), reg->Grid.GetBoundarySize());
            TGridMaskedData::TIterator src(reg->Grid.GetMaskedData(bndFull, varName)),
                dst(src);
            //TGridMaskedData::TIterator dst(src);
            if(splitCell == lftJoin) {
                dst = splitCell;
                *(double *)dst.GetElementPtr() = splitLft;
                *(double *)(dst + 1).GetElementPtr() = splitRgt;
                return;
            }
            if(splitCell < lftJoin) {
                dst = src = lftJoin;
                if(setJoined)
                    *(double *)(dst + 1).GetElementPtr() = joinedVal;
                --src;
                for(; src != splitCell; --src, --dst)
                    dst.SetElement(src);
            } else {
                dst = src = lftJoin + 1;
                if(setJoined)
                    *(double *)(dst - 1).GetElementPtr() = joinedVal;
                ++src;
                for(; src != splitCell; ++src, ++dst)
                    dst.SetElement(src);
            }
            dst = splitCell;
            *(double *)dst.GetElementPtr() = splitLft;
            *(double *)(dst + 1).GetElementPtr() = splitRgt;
        }

    public:
        TPolyMarchPlusGase()
            : TPolyMarchPlusBase(),
              SrcRegion(1),
              DstRegion(2),
              T_DensLow(NULL),
              T_DensHigh(NULL),
              T_ELow(NULL),
              EnerName("Energy"),
              DensName("Density"),
              EOSName("Matter"),
              PresName("Pressure"),
              MassName("Mass"),
              SplDataName("Bin_T2DensLowHighEHigh"),
              PosName("Position"),
              DensLowCoef(0.9){};
        void RebuildBounds(TPolyRegion *reg) {
            TPolyRegion *src = NULL, *dst = NULL;
            int i = 1;
            for(TPolyRegion::TShallowIterator it = reg->ShallowStart(); it.IsOk();
                it.Next(), i++) {
                if(i == SrcRegion)
                    src = it.CurRegion();
                if(i == DstRegion)
                    dst = it.CurRegion();
                //Marcher->InitBeforeBounds(it.CurRegion());
            }
            if(!src || !dst)
                throw info_except("wrong region in TPolyMarchPlusGase!!!\n");
            CheckBounds(src, dst, DstRegion > SrcRegion);
            Marcher->RebuildBounds(reg);
        }

        int save_data_state(FilterOut &so) {
            so << " SrcRegion " << SrcRegion << " DstRegion " << DstRegion
               << " DensLowCoef " << DensLowCoef << SavableClass::EOLN();
            so << " EnerName " << EnerName << " DensName " << DensName << " EOSName "
               << EOSName << " PresName " << PresName;
            so << " MassName " << MassName << " PosName " << PosName << " VelName "
               << VelName << SavableClass::EOLN();
            so << " SplitMassCoef " << SplitMassCoef << " SplDataName " << SplDataName
               << SavableClass::EOLN();
            return TPolyMarchPlusBase::save_data_state(so);
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> SrcRegion >> tmp >> DstRegion >> tmp >> DensLowCoef;
            si >> tmp >> EnerName >> tmp >> DensName >> tmp >> EOSName >> tmp >>
                PresName >> tmp >> MassName >> tmp >> PosName;
            si >> tmp >> VelName >> tmp >> SplitMassCoef >> tmp >> SplDataName;
            vector<Ref<SavableClass>> spl = File::ReadConf(~SplDataName, 3, 1);
            T_DensLow << spl[0];
            T_DensHigh << spl[1];
            T_ELow << spl[2];
            return TPolyMarchPlusBase::read_data_state(si);
        }
        Stroka MakeHelp() {
            Stroka help =
                "Class imitating  gas - liquid separation. Src,Dst regions are counted from 1. SplDataName - contains 3 spline - T_DensLow, T_DensHigh, T_ELow. Uses marcher for time step clc.\n";
            help += " Marcher is of the same type - NPolygon::RegionMarch_category.\n";
            //help += " Registered NPolygon::RegionMarch_category objects:\n\n~+\n" + SavableClass::HelpForCategory("NPolygon::RegionMarch_category");
            //help += "~-\n";
            return help;
        }
    };
};