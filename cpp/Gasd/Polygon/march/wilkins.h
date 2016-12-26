#pragma once

#include "CenterFlux.h"

namespace NPolygon {

    //struct TPolyMarchDriverFluxBase : FluxCalculatorBase {
    //    TPolyMarchDriverFluxBase() : FluxCalculatorBase() {}
    //        //virtual void ClcFlux(TPolyRegion *reg, const Stroka &uName, const Stroka &fName, const Stroka &dxName, const TRegionBounds &bnds, double dT)
    //    virtual void ClcSound(TPolyRegion *reg, const Stroka &uName, const Stroka &sName,const TRegionBounds &bnds) = 0;
    //    virtual Stroka GetRvarName(TPolyRegion *reg, const Stroka &uName, const TRegionBounds &bnd) = 0;
    //    virtual Stroka GetVarName() = 0;
    //    virtual void ClcDX(TPolyRegion *reg, const Stroka dXname, const TRegionBounds &bnds) = 0;
    //    virtual void RebuildBounds(TPolyRegion *reg) = 0;
    //    virtual void RebuildTimeDone(TPolyRegion *reg, double tStp) {};
    //    virtual void InitRegion(TPolyRegion *reg) {};
    //    virtual void StepVars(TPolyRegion *reg, const Stroka deltaFlux, const Stroka marchVars, const TRegionBounds &bnds) {
    //        reg->Grid.GetMaskedData(bnds, marchVars) += reg->Grid.GetMaskedData(bnds, deltaFlux);
    //    }
    //};


    struct TPolyMarchDriverFluxGasdWilkins : TPolyMarchDriverFluxBase {
        virtual void ClcSound(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &sName,
            const TRegionBounds &bnds){};
        virtual Stroka GetRvarName(
            TPolyRegion *reg,
            const Stroka &uName,
            const TRegionBounds &bnd) {
            return "";
        };
        virtual Stroka GetVarName() {
            return "";
        };
        virtual void ClcDX(
            TPolyRegion *reg,
            const Stroka dXname,
            const TRegionBounds &bnds){};
        virtual void RebuildBounds(TPolyRegion *reg){};

        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            const Stroka &xName,
            const TRegionBounds &bnds,
            double dT){};
    };


    struct TMarchWilkins : TPolyMarchRegionBase {
    private:
        void InitFakeNum(TPolyRegion *reg) {
            reg->Grid.AddVar("FakeNum", new TGridVar<double>());
            int level = reg->Grid.GetBoundarySize();
            TRegionBounds bndAll(-level, level);
            TRegionBounds bnd(0, 0);
            TRegionBounds bnd1(1, -1);
            int len = reg->Grid.GetMaskedData(bndAll, "FakeNum").Start().Size();
            double *fake = (double *)reg->Grid.GetMaskedData(bndAll, "FakeNum")
                               .Start()
                               .GetElementPtr();
            for(int i = 0; i < 2 * level + 1; i++) {
                fake[i] = i - level;
                fake[len - 2 * level + 1 - i] = level - i;
            }
        }
        int IsVacuum(TPolyRegion *reg) {
            MatterIO *mat = SavableClass::TestType<MatterIO>(reg->MapSavableVar[EOSName]);
            return (dynamic_cast<MatterVacuum *>(mat) != NULL);
        }
        int VacuumIsClosed(TPolyRegion *reg) {
            TRegionBounds bnd(0, 0);
            double *pos =
                (double *)reg->Grid.GetMaskedData(bnd, PosName).Start().GetElementPtr();
            int size = reg->Grid.GetMaskedData(bnd, PosName).Start().Size();
            return (pos[size - 1] - pos[0] < MathZer);
        }
        void InitDenc(TPolyRegion *reg) {
            int level = reg->Grid.GetBoundarySize();
            TRegionBounds bndAll(-level, level);
            double *dencPtr = (double *)reg->Grid.GetMaskedData(bndAll, DencName)
                                  .Start()
                                  .GetElementPtr();
            MatterIO *mat = SavableClass::TestType<MatterIO>(reg->MapSavableVar[EOSName]);
            if(IsVacuum(reg))
                return;
            double denc0 = mat->DencityCold();
            for(int i = 0; i < reg->Grid.GetMaskedData(bndAll, DencName).Start().Size();
                i++)
                if(dencPtr[i] == 0)
                    dencPtr[i] = denc0;
        }
        void InitRegion(TPolyRegion *reg) {
            //if (reg->Grid.VarExists(ViscName))
            //    return;
            //InitFakeNum(reg);
            InitDenc(reg);
            reg->Grid.AddVar(VelPlusName, new TGridVar<double>());    //
            reg->Grid.AddVar(VolName, new TGridVar<double>());        //
            reg->Grid.AddVar(VolPlusName, new TGridVar<double>());    //
            reg->Grid.AddVar(PosPlusName, new TGridVar<double>());    //
            reg->Grid.AddVar(DencPlusName, new TGridVar<double>());   //

            reg->Grid.AddVar(MassName, new TGridVar<double>());   //
            //reg->Grid.AddVar(ViscPresName, new TGridVar<double>());
            reg->Grid.AddVar(VolDerivName, new TGridVar<double>());
            //reg->Grid.AddVar(MassBndName, new TGridVar<double>());
            reg->Grid.AddVar(ViscName, new TGridVar<double>());
            reg->Grid.AddVar(ViscPlusName, new TGridVar<double>());

            reg->Grid.AddVar("Fake", new TGridVar<double>());   //

            TRegionBounds bndCent(0, -1);

            reg->Grid.GetMaskedData(bndCent, VolName) =
                (reg->Grid.GetMaskedData(bndCent + 1, PosName) -
                 reg->Grid.GetMaskedData(bndCent, PosName));

            reg->Grid.GetMaskedData(bndCent, MassName) =
                reg->Grid.GetMaskedData(bndCent, DencName) *
                (reg->Grid.GetMaskedData(bndCent + 1, PosName) -
                 reg->Grid.GetMaskedData(bndCent, PosName));

            TFluxPrepare::MakeBoundedPresSound(
                reg, bndCent, bndCent, DencName, EnerName, EOSName, PresName, 1);
            TFluxPrepare::MakeBoundedPresSound(
                reg, bndCent, bndCent, DencName, EnerName, EOSName, SoundName, 0);

            reg->Grid.GetMaskedData(bndCent, ViscName) =
                reg->Grid.GetMaskedData(bndCent, PresName) * 0.;
            reg->Grid.GetMaskedData(bndCent, ViscPlusName) =
                reg->Grid.GetMaskedData(bndCent, ViscName);

            reg->Grid.GetMaskedData(bndCent, VolDerivName) =
                reg->Grid.GetMaskedData(bndCent, PresName) * 0.;

            TStp0_5 = 1;
            CurTime = 0;
            BoundaryMakerPres.SetBase(~Stroka(MinPres));
            Timer.InitTimer();
        }
        void MakeVisc(TPolyRegion *reg, const Stroka &viscName) {
            TRegionBounds bndCent(0, -1);
//            double *vel = (double *)reg->Grid.GetMaskedData(bndCent, VelName)
//                              .Start()
//                              .GetElementPtr();
            double *velPlus = (double *)reg->Grid.GetMaskedData(bndCent, VelPlusName)
                                  .Start()
                                  .GetElementPtr();
            double *denc = (double *)reg->Grid.GetMaskedData(bndCent, DencName)
                               .Start()
                               .GetElementPtr();
            double *dencPlus = (double *)reg->Grid.GetMaskedData(bndCent, DencPlusName)
                                   .Start()
                                   .GetElementPtr();
            double *pres = (double *)reg->Grid.GetMaskedData(bndCent, PresName)
                               .Start()
                               .GetElementPtr();
            double *viscP = (double *)reg->Grid.GetMaskedData(bndCent, viscName)
                                .Start()
                                .GetElementPtr();
            double *visc = (double *)reg->Grid.GetMaskedData(bndCent, ViscName)
                               .Start()
                               .GetElementPtr();
            int size = reg->Grid.GetMaskedData(bndCent, PresName).Start().Size();
            double minPres = MinPres;
            double cL = CL, c0 = C0;
            for(int i = 0; i < size; i++) {
                viscP[i] = 0;
                double dU = (velPlus[i + 1] - velPlus[i]);
                if(dU >= MathZer)
                    continue;
                double rCent = 0.5 * (denc[i] + dencPlus[i]);
                if(dencPlus[i] - MathZer <= denc[i])
                    continue;
                double pCent = max<double>(pres[i] - visc[i], minPres);
                double a = sqrt(pCent / max<double>(rCent, MathZer));
                double viscNew = rCent * (sqr(c0 * dU) + cL * a * fabs(dU));
                viscP[i] = viscNew;
            }
        }
        void SetMinPres(TPolyRegion *reg, const Stroka &presName, double minP) {
            TRegionBounds bndCent(0, -1);
            double *pres = (double *)reg->Grid.GetMaskedData(bndCent, presName)
                               .Start()
                               .GetElementPtr();
            int size = reg->Grid.GetMaskedData(bndCent, presName).Start().Size();
            for(int i = 0; i < size; i++) {
                if(pres[i] < minP)
                    pres[i] = minP + (pres[i] - minP) * MinPresCoef;
            }
        }
        double GetMaxTimeStp2Vacuum(TPolyRegion *reg) {
            //TRegionBounds bnd(0, -1);
            TRegionBounds bnd(0, 0);
            double *vel =
                (double *)reg->Grid.GetMaskedData(bnd, VelName).Start().GetElementPtr();
            double *pos =
                (double *)reg->Grid.GetMaskedData(bnd, PosName).Start().GetElementPtr();

            int size = reg->Grid.GetMaskedData(bnd, VolName).Start().Size();
            double dV = vel[size - 1] - vel[0], dX = pos[size - 1] - pos[0];
            if(dX < MathZer)
                return 1 / MathZer;
            dV = max<double>(MathZer, dV);
            return dX / dV;
        }
        void MakeTimeStep2Vacuum(TPolyRegion *reg, double tStp) {
            TRegionBounds bndX(0, 0);
            double *pos =
                (double *)reg->Grid.GetMaskedData(bndX, PosName).Start().GetElementPtr();
            //double *num = (double *)reg->Grid.GetMaskedData(bndX, "FakeNum").Start().GetElementPtr();
            int sizeX = reg->Grid.GetMaskedData(bndX, PresName).Start().Size();
            if(pos[sizeX - 1] - pos[0] < MathZer)
                pos[sizeX - 1] = (pos[0] = 0.5 * (pos[sizeX - 1] + pos[0]));
            double x0 = pos[0], xStp = (pos[sizeX - 1] - pos[0]) / (sizeX - 1);
            for(int i = -1; i < sizeX + 1; i++)
                pos[i] = x0 + xStp * (double)i;
        }


        void FillChildBoundsVacuumX(TPolyRegion *reg, const Stroka &varName) {
            TPolyRegion::TShallowIterator prevIt;
//            TPolyRegion *prevRegion = NULL;
            for(TPolyRegion::TShallowIterator curIt = reg->ShallowStart(); curIt.IsOk();
                prevIt = curIt, curIt.Next()) {
                if(!IsVacuum(curIt.CurRegion()))
                    continue;
                TRegionBounds bndX(0, 0);
                double *posPrev = (double *)prevIt.CurRegion()
                                      ->Grid.GetMaskedData(bndX, varName)
                                      .Start()
                                      .GetElementPtr();
                double *posCur = (double *)curIt.CurRegion()
                                     ->Grid.GetMaskedData(bndX, varName)
                                     .Start()
                                     .GetElementPtr();
                int sizePrev =
                    prevIt.CurRegion()->Grid.GetMaskedData(bndX, varName).Start().Size();
                int sizeCur =
                    curIt.CurRegion()->Grid.GetMaskedData(bndX, varName).Start().Size();
                curIt.Next();
                double *posNext = (double *)curIt.CurRegion()
                                      ->Grid.GetMaskedData(bndX, varName)
                                      .Start()
                                      .GetElementPtr();
//                int sizeNext =
//                    curIt.CurRegion()->Grid.GetMaskedData(bndX, varName).Start().Size();
                posCur[0] = posPrev[sizePrev - 1];
                posCur[sizeCur - 1] = posNext[0];
            }
        }
        void SetRegion(TPolyRegion::TShallowIterator &region) {
            TPolyRegion *prev = region.GetPrev(), *next = region.GetNext(),
                        *cur = region.CurRegion();
            TRegionBounds bndRgt(TRegionBounds::BNDBAD, 0),
                bndRgtMin(TRegionBounds::BNDBAD, -1), bndLft(0, TRegionBounds::BNDBAD),
                bndLftMin(-1, TRegionBounds::BNDBAD);
            if(!prev) {
                cur->Grid.GetMaskedData(bndLftMin, PresName) = MinPres;
                cur->Grid.GetMaskedData(bndLftMin, MassName) = 0;
            }
            while(next && IsVacuum(next) && VacuumIsClosed(next)) {
                region.Next();
                next = region.GetNext();
            }
            if(!next || IsVacuum(next)) {
                cur->Grid.GetMaskedData(bndRgt, PresName) = MinPres;
                cur->Grid.GetMaskedData(bndRgt, MassName) = 0;
                return;
            }
            if(IsVacuum(cur)) {
                next->Grid.GetMaskedData(bndLftMin, PresName) = MinPres;
                next->Grid.GetMaskedData(bndLftMin, MassName) = 0;
                cur->Grid.GetMaskedData(bndRgt, PosName) =
                    next->Grid.GetMaskedData(bndLft, PosName);
                cur->Grid.GetMaskedData(bndLft, PosName) =
                    prev->Grid.GetMaskedData(bndRgt, PosName);
                return;
            }
            cur->Grid.GetMaskedData(bndRgt, PresName) =
                next->Grid.GetMaskedData(bndLft, PresName);
            cur->Grid.GetMaskedData(bndRgt, MassName) =
                next->Grid.GetMaskedData(bndLft, MassName);
            next->Grid.GetMaskedData(bndLftMin, PresName) =
                cur->Grid.GetMaskedData(bndRgtMin, PresName);
            next->Grid.GetMaskedData(bndLftMin, MassName) =
                cur->Grid.GetMaskedData(bndRgtMin, MassName);

            cur->Grid.GetMaskedData(bndRgt, PosName) =
                (next->Grid.GetMaskedData(bndLft, PosName) +
                 cur->Grid.GetMaskedData(bndRgt, PosName)) *
                0.5;
            next->Grid.GetMaskedData(bndLft, PosName) =
                cur->Grid.GetMaskedData(bndRgt, PosName);
            cur->Grid.GetMaskedData(bndRgt, VelName) =
                (next->Grid.GetMaskedData(bndLft, VelName) +
                 cur->Grid.GetMaskedData(bndRgt, VelName)) *
                0.5;
            next->Grid.GetMaskedData(bndLft, VelName) =
                cur->Grid.GetMaskedData(bndRgt, VelName);
        }


        Stroka PresName, SoundName, EnerName, EOSName;
        Stroka VelName, PosName, DencName;
        Stroka VelPlusName, VolName, VolPlusName, PosPlusName, DencPlusName;
        Stroka MassName, /*MassBndName, */ VolDerivName;   //ViscPresName,
        Stroka ViscPlusName, ViscName;
        double TStp0_5, CL, C0, MarchCourant, MinPres, EnergyPresCoef, MinPresCoef;
        double CurTime;
        TRegBoundaryCircleX BoundaryMakerX;
        TRegBoundaryCircle_ZeroConst BoundaryMakerMass, BoundaryMakerPres,
            BoundaryMakerVel;
        PrintNumIterTime Timer;

    public:
        TMarchWilkins()
            : TPolyMarchRegionBase(),
              PresName("Pres"),
              SoundName("Sound"),
              EnerName("Ener"),
              EOSName("EOS"),
              VelName("Vel"),
              PosName("Pos"),
              DencName("Denc"),
              VelPlusName("VelPlus"),
              VolName("Vol"),
              VolPlusName("VolPlus"),
              PosPlusName("PosPlus"),
              DencPlusName("DencPlus"),
              MassName("Mass"),
              //, MassBndName("MassBnd")
              //, ViscPresName("ViscPres")
              VolDerivName("VolDeriv"),
              ViscPlusName("ViscPlus"),
              ViscName("Visc"),
              TStp0_5(1e5),
              CL(1),
              C0(2),
              MarchCourant(0.1),
              MinPres(1e-4),
              EnergyPresCoef(0),
              MinPresCoef(1),
              BoundaryMakerX(TRegionBounds(0, 0)),
              BoundaryMakerMass(TRegionBounds(0, -1), "0 "),
              BoundaryMakerPres(TRegionBounds(0, -1), "1e-4"),
              BoundaryMakerVel(TRegionBounds(0, -1), "0 1")
        {}
        //virtual double GetCurTime() { return CurTime;}
        virtual void InitBeforeBounds(TPolyRegion *reg) {
            InitRegion(reg);
        }
        virtual void InitAfterBounds(TPolyRegion *reg) {}
        virtual void RebuildBounds(TPolyRegion *reg) {
            for(TPolyRegion::TShallowIterator region = reg->ShallowStart(); region.IsOk();
                region.Next()) {
                SetRegion(region);
            }
        }
        double GetMaxTimeStp(TPolyRegion *reg) {
            if(IsVacuum(reg))
                return GetMaxTimeStp2Vacuum(reg);
            TRegionBounds bndCent(0, -1);
            double tStpPlus3_2 = 1 / MathZer;
            double cL = CL, c0 = C0;
            TFluxPrepare::MakeBoundedPresSound(
                reg, bndCent, bndCent, DencName, EnerName, EOSName, SoundName, 0);
            double *vol = (double *)reg->Grid.GetMaskedData(bndCent, VolName)
                              .Start()
                              .GetElementPtr();
            double *sound = (double *)reg->Grid.GetMaskedData(bndCent, SoundName)
                                .Start()
                                .GetElementPtr();
            double *vDer = (double *)reg->Grid.GetMaskedData(bndCent, VolDerivName)
                               .Start()
                               .GetElementPtr();
            reg->Grid.GetMaskedData(bndCent, "Fake") =
                reg->Grid.GetMaskedData(bndCent + 1, VelName) -
                reg->Grid.GetMaskedData(bndCent, VelName);
            double *fake = (double *)reg->Grid.GetMaskedData(bndCent, "Fake")
                               .Start()
                               .GetElementPtr();

            int size = reg->Grid.GetMaskedData(bndCent, VolName).Start().Size();
            for(int i = 0; i < size; i++) {
                double b = 0;
                if(vDer[i] < 0)
                    b = 8 * (sqr(c0) + cL) * vDer[i];
                double cStp =
                    1.5 * vol[i] / (sqrt(sqr(fabs(sound[i]) + fabs(fake[i])) + sqr(b)));
                tStpPlus3_2 = min(tStpPlus3_2, cStp);
            }
            tStpPlus3_2 = min(1.1 * TStp0_5 / MarchCourant, tStpPlus3_2);
            double tmp = max<double>(
                min(TStp0_5 / MarchCourant, 10 * tStpPlus3_2), 0.1 * tStpPlus3_2);
            double ret = 0.5 * (tStpPlus3_2 + tmp);
            return max<double>(MathZer, ret * MarchCourant);
        }
        virtual void SetNewTimeStp(double curTime, double tStp) {
            TStp0_5 = tStp;
            CurTime = curTime;
            Stroka str;
            if(Timer.PrintNow(str))
                cout << "CurTime " << curTime << "\tMakingTstp " << tStp << "\n"
                     << str.c_str() << "\n";
            //if (Timer.PrintNow())
            //	cout << " CurTime " << CurTime << " TimeStep " << TStp0_5 << "\n";
            //cout << Timer;
        };

        void MakeTimeStep(TPolyRegion *reg, double curTime, double tStp) {
            if(IsVacuum(reg))
                return MakeTimeStep2Vacuum(reg, tStp);
            TRegionBounds bnd(0, 0);
            TRegionBounds bndCent(0, -1);
            reg->Grid.GetMaskedData(bnd, VelPlusName) =
                reg->Grid.GetMaskedData(bnd, VelName) -
                (reg->Grid.GetMaskedData(bnd, PresName) -
                 reg->Grid.GetMaskedData(bnd - 1, PresName)) /
                    (reg->Grid.GetMaskedData(bnd, MassName) +
                     reg->Grid.GetMaskedData(bnd - 1, MassName)) *
                    (2 * tStp);

            reg->Grid.GetMaskedData(bnd, PosPlusName) =
                reg->Grid.GetMaskedData(bnd, PosName) +
                reg->Grid.GetMaskedData(bnd, VelPlusName) * tStp;

            reg->Grid.GetMaskedData(bndCent, VolPlusName) =
                (reg->Grid.GetMaskedData(bndCent + 1, PosPlusName) -
                 reg->Grid.GetMaskedData(bndCent, PosPlusName));

            reg->Grid.GetMaskedData(bndCent, DencPlusName) =
                reg->Grid.GetMaskedData(bndCent, MassName) /
                reg->Grid.GetMaskedData(bndCent, VolPlusName);

            // Make iteration Pressure!!! for visc, E
            MakeVisc(reg, ViscPlusName);
            reg->Grid.GetMaskedData(bndCent, EnerName) =
                reg->Grid.GetMaskedData(bndCent, EnerName) -
                (reg->Grid.GetMaskedData(bndCent, PresName) +
                 (reg->Grid.GetMaskedData(bndCent, ViscPlusName) -
                  reg->Grid.GetMaskedData(bndCent, ViscName)) *
                     EnergyPresCoef) *
                    (reg->Grid.GetMaskedData(bndCent, VolPlusName) -
                     reg->Grid.GetMaskedData(bndCent, VolName)) /
                    reg->Grid.GetMaskedData(bndCent, MassName);

            TFluxPrepare::MakeBoundedPresSound(
                reg, bndCent, bndCent, DencPlusName, EnerName, EOSName, PresName, 1);
            reg->Grid.GetMaskedData(bndCent, PresName) +=
                reg->Grid.GetMaskedData(bndCent, ViscPlusName);
            SetMinPres(reg, PresName, MinPres);
            reg->Grid.GetMaskedData(bndCent, ViscName) =
                reg->Grid.GetMaskedData(bndCent, ViscPlusName);

            // VolDerivName - done for time stp
            reg->Grid.GetMaskedData(bndCent, VolDerivName) =
                (reg->Grid.GetMaskedData(bndCent, VolPlusName) -
                 reg->Grid.GetMaskedData(bndCent, VolName)) /
                ((reg->Grid.GetMaskedData(bndCent, VolPlusName) +
                  reg->Grid.GetMaskedData(bndCent, VolName)) *
                 (0.5 * tStp));
            reg->Grid.GetMaskedData(bndCent, VolDerivName) *=
                (reg->Grid.GetMaskedData(bndCent + 1, PosPlusName) -
                 reg->Grid.GetMaskedData(bndCent, PosPlusName));


            reg->Grid.GetMaskedData(bnd, VelName) =
                reg->Grid.GetMaskedData(bnd, VelPlusName);
            reg->Grid.GetMaskedData(bnd, PosName) =
                reg->Grid.GetMaskedData(bnd, PosPlusName);
            reg->Grid.GetMaskedData(bnd, VolName) =
                reg->Grid.GetMaskedData(bnd, VolPlusName);
            reg->Grid.GetMaskedData(bnd, DencName) =
                reg->Grid.GetMaskedData(bnd, DencPlusName);
        }


        int save_data_state(FilterOut &so) {
            so << " MarchCourant " << MarchCourant << " CL " << CL << " C0 " << C0
               << " MinPres " << MinPres << SavableClass::EOLN();
            so << " EOSName " << EOSName << " PosName " << PosName << " DencName "
               << DencName << " PresName " << PresName << SavableClass::EOLN();
            so << " EnerName " << EnerName << " VelName " << VelName << " SoundName "
               << SoundName << SavableClass::EOLN();
            so << " EnergyPresCoef " << EnergyPresCoef << " MinPresCoef " << MinPresCoef
               << SavableClass::EOLN();
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> MarchCourant >> tmp >> CL >> tmp >> C0 >> tmp >> MinPres;
            si >> tmp >> EOSName >> tmp >> PosName >> tmp >> DencName >> tmp >>
                PresName >> tmp >> EnerName >> tmp >> VelName >> tmp >> SoundName;
            si >> tmp >> EnergyPresCoef >> tmp >> MinPresCoef;
            return 1;
        }
        Stroka MakeHelp();
    };

};   //namespace NPolygon {


