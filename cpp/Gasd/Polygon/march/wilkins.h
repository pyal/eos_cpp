#pragma once

#include "CenterFlux.h"

namespace NPolygon {

    struct GasdNames {
        Stroka Velocity, Density, Energy, Pressure, Volume, PosX, Sound;
        GasdNames() {SetDefault();}
        void SetDefault() {
            Velocity = "Vel";
            Density = "Dens";
            Energy = "Ener";
            Pressure = "Pres";
            Volume = "Vol";
            PosX = "Pos";
            Sound = "Sound";
        }
        void AddPostfix(const char *postFix) {
            Velocity += postFix;
            Density += postFix;
            Energy += postFix;
            Pressure += postFix;
            Volume += postFix;
            PosX += postFix;
            Sound += postFix;
        }
        vector<Stroka> GetNames() {
            return vector<Stroka>{Velocity, Density, Energy, Pressure, Volume, PosX, Sound};
        }
    };
    struct ClcNames {
        Stroka MassCent, VolDerive, Viscouse, ViscousePlus, VelocityCent;
        ClcNames(){SetDefault();}
        void SetDefault() {
            MassCent = "MassCent";
            VolDerive = "VolDerive";
            Viscouse = "Viscouse";
            ViscousePlus = "ViscousePlus";
            VelocityCent = "VelocityCent";
        }
        vector<Stroka> GetNames() {
            return vector<Stroka>{MassCent, VolDerive, Viscouse, ViscousePlus, VelocityCent};
        }
    };
    struct TMarchWilkins : TPolyMarchRegionBase {

    protected:
        void InitRegion(TPolyRegion *reg) {
            for(auto &set:{GasdTp.GetNames(), Clc.GetNames(), vector<Stroka>{GasdTm.Volume}}) {
                for (auto &name:set) {
                    reg->Grid.AddVar(name, new TGridVar<double>());
                }
            }
            TRegionBounds bndCent(0, -1);

            reg->Grid.GetMaskedData(bndCent, GasdTm.Volume) =
                    (reg->Grid.GetMaskedData(bndCent + 1, GasdTm.PosX) -
                     reg->Grid.GetMaskedData(bndCent, GasdTm.PosX));

            reg->Grid.GetMaskedData(bndCent, Clc.MassCent) =
                    reg->Grid.GetMaskedData(bndCent, GasdTm.Density) *
                    reg->Grid.GetMaskedData(bndCent, GasdTm.Volume);

            TFluxPrepare::MakeBoundedPresSound(
                    reg, bndCent, bndCent, GasdTm.Density, GasdTm.Energy, EOSName, GasdTm.Pressure, 0);
            TFluxPrepare::MakeBoundedPresSound(
                    reg, bndCent, bndCent, GasdTm.Density, GasdTm.Energy, EOSName, GasdTm.Pressure, 1);

            reg->Grid.GetMaskedData(bndCent, Clc.VolDerive) =
                    reg->Grid.GetMaskedData(bndCent, GasdTm.Pressure) * 0.;

            TStp0_5 = 1;
            CurTime = 0;
            BoundaryMakerPres.SetBase(~Stroka(MinPres));

        }
        void SetMinPres(TPolyRegion *reg, const Stroka &presName, double minP) {
            TRegionBounds bndCent(0, -1);
            double *pres = reg->GetDataPtr(bndCent, presName);
            int size = reg->GetDataSize(bndCent, presName);
            for(int i = 0; i < size; i++) {
                if(pres[i] < minP)
                    pres[i] = minP + (pres[i] - minP) * MinPresCoef;
            }
        }




        void RebuildBounds(TPolyRegion::TShallowIterator &region) {
            TPolyRegion *prev = region.GetPrev(), *next = region.GetNext(),
                    *cur = region.CurRegion();
            TRegionBounds bndRgt(TRegionBounds::BNDBAD, 0),
                    bndRgtMin(TRegionBounds::BNDBAD, -1), bndLft(0, TRegionBounds::BNDBAD),
                    bndLftMin(-1, TRegionBounds::BNDBAD);
            if(!prev) {
                cur->Grid.GetMaskedData(bndLftMin, GasdTm.Pressure) = MinPres;
                cur->Grid.GetMaskedData(bndLftMin, Clc.MassCent) = 0;
            }
            while(next && IsVacuum(next) && VacuumIsClosed(next)) {
                region.Next();
                next = region.GetNext();
            }
            if(!next || IsVacuum(next)) {
                cur->Grid.GetMaskedData(bndRgt, GasdTm.Pressure) = MinPres;
                cur->Grid.GetMaskedData(bndRgt, Clc.MassCent) = 0;
                return;
            }
            if(IsVacuum(cur)) {
                next->Grid.GetMaskedData(bndLftMin, GasdTm.Pressure) = MinPres;
                next->Grid.GetMaskedData(bndLftMin, Clc.MassCent) = 0;
                cur->Grid.GetMaskedData(bndRgt, GasdTm.PosX) =
                        next->Grid.GetMaskedData(bndLft, GasdTm.PosX);
                cur->Grid.GetMaskedData(bndLft, GasdTm.PosX) =
                        prev->Grid.GetMaskedData(bndRgt, GasdTm.PosX);
                return;
            }
            cur->Grid.GetMaskedData(bndRgt, GasdTm.Pressure) =
                    next->Grid.GetMaskedData(bndLft, GasdTm.Pressure);
            next->Grid.GetMaskedData(bndLftMin, GasdTm.Pressure) =
                    cur->Grid.GetMaskedData(bndRgtMin, GasdTm.Pressure);

            cur->Grid.GetMaskedData(bndRgt, Clc.MassCent) =
                    next->Grid.GetMaskedData(bndLft, Clc.MassCent);
            next->Grid.GetMaskedData(bndLftMin, Clc.MassCent) =
                    cur->Grid.GetMaskedData(bndRgtMin, Clc.MassCent);

            cur->Grid.GetMaskedData(bndRgt, GasdTm.PosX) =
                    (next->Grid.GetMaskedData(bndLft, GasdTm.PosX) +
                     cur->Grid.GetMaskedData(bndRgt, GasdTm.PosX)) * 0.5;
            next->Grid.GetMaskedData(bndLft, GasdTm.PosX) =
                    cur->Grid.GetMaskedData(bndRgt, GasdTm.PosX);

            cur->Grid.GetMaskedData(bndRgt, GasdTm.Velocity) =
                    (next->Grid.GetMaskedData(bndLft, GasdTm.Velocity) +
                     cur->Grid.GetMaskedData(bndRgt, GasdTm.Velocity)) * 0.5;
            next->Grid.GetMaskedData(bndLft, GasdTm.Velocity) =
                    cur->Grid.GetMaskedData(bndRgt, GasdTm.Velocity);

        }




        Stroka EOSName;//, VolDerivName, Fake, ViscouseName, ViscousePlusName;
        double TStp0_5;
    public:
        double CL, C0, MarchCourant;
        GasdNames GasdTm, GasdTp;
        ClcNames Clc;
    private:
        double MinPres, EnergyPresCoef, MinPresCoef;
        double CurTime;
        TRegBoundaryCircleX BoundaryMakerX;
        TRegBoundaryCircle_ZeroConst BoundaryMakerMass, BoundaryMakerPres, BoundaryMakerVel;

    public:
        TMarchWilkins()
                : TPolyMarchRegionBase(),
                  EOSName("EOS"),
                  TStp0_5(1e5),
                  CL(1),
                  C0(2),
                  MarchCourant(0.1),
                  MinPres(1e-4),
                  EnergyPresCoef(1),
                  MinPresCoef(0),
                  BoundaryMakerX(TRegionBounds(0, 0)),
                  BoundaryMakerMass(TRegionBounds(0, -1), "0 "),
                  BoundaryMakerPres(TRegionBounds(0, -1), "1e-4"),
                  BoundaryMakerVel(TRegionBounds(0, 0), "0 1")
        {

            GasdTp.AddPostfix("_plus");
        }
        virtual void InitBase(TPolyRegion *head, double startTime) {
            for(TPolyRegion::TShallowIterator region = head->ShallowStart(); region.IsOk();
                region.Next()) {
                InitRegion(region.CurRegion());
            }
            head->FillChildBounds(&BoundaryMakerMass, Clc.MassCent);
            TRegBoundaryCircle_ZeroConst bndMakerU(TRegionBounds(1, 0), "0 1");
            head->FillChildBounds(&bndMakerU, GasdTm.Velocity);

            RebuildBoundsBase(head);
        }
        virtual void RebuildBoundsBase(TPolyRegion *head) {
//            head->FillChildBounds(&BoundaryMakerX, GasdTm.PosX);
//            head->FillChildBounds(&BoundaryMakerPres, GasdTm.Pressure);
//            head->FillChildBounds(&BoundaryMakerVel, GasdTm.Velocity);
            for(TPolyRegion::TShallowIterator region = head->ShallowStart(); region.IsOk();
                region.Next()) {
                RebuildBounds(region);
            }
        }
        double GetMaxTimeStp(TPolyRegion *reg, double ) {
            if(IsVacuum(reg))
                return GetMaxTimeStp2Vacuum(reg);
            TRegionBounds bndCent(0, -1);
            double tStpPlus3_2 = 1 / MathZer;
            double cL = CL, c0 = C0;
            TFluxPrepare::MakeBoundedPresSound(
                    reg, bndCent, bndCent, GasdTm.Density, GasdTm.Energy, EOSName, GasdTm.Sound, 0);
            double *vol = reg->GetDataPtr(bndCent, GasdTm.Volume);
            double *sound = reg->GetDataPtr(bndCent, GasdTm.Sound);
            double *vDer = reg->GetDataPtr(bndCent, Clc.VolDerive);
            reg->Grid.GetMaskedData(bndCent, Clc.VelocityCent) =
                    reg->Grid.GetMaskedData(bndCent + 1, GasdTm.Velocity) -
                    reg->Grid.GetMaskedData(bndCent, GasdTm.Velocity);
            double *fake = reg->GetDataPtr(bndCent, Clc.VelocityCent);

            int size = reg->GetDataSize(bndCent, GasdTm.Volume);
            for(int i = 0; i < size; i++) {
                double b = 0;
                if(vDer[i] < 0)
                    b = 8 * (sqr(c0) + cL) * vDer[i];
                double cStp =
                        1.5 * vol[i] / (sqrt(sqr(fabs(sound[i]) + fabs(fake[i])) + sqr(b)));
                tStpPlus3_2 = min(tStpPlus3_2, cStp);
            }
            tStpPlus3_2 *= MarchCourant;
            tStpPlus3_2 = min(1.1 * TStp0_5, tStpPlus3_2);
            return max<double>(MathZer, tStpPlus3_2);
        }

        void ZeroMassFix(TPolyRegion *reg, TRegionBounds &bnd, Stroka &FixVarName, Stroka &MassVarName) {
            double *fixVar = reg->GetDataPtr(bnd, FixVarName);
            double *massVar = reg->GetDataPtr(bnd, MassVarName);
            int s = reg->GetDataSize(bnd, FixVarName);
            for(int i = 0; i < s; i++) {
                if (massVar[i] < MathZer) fixVar[i] = 0;
            }

        }
        void MakeTimeStep(TPolyRegion *reg, double curTime, double tStp) {
            CurTime = curTime;
            if(IsVacuum(reg))
                return MakeTimeStep2Vacuum(reg, tStp);
            TRegionBounds bnd(0, 0);
            TRegionBounds bndCent(0, -1);

            reg->Grid.GetMaskedData(bnd, GasdTp.Velocity) =
                    reg->Grid.GetMaskedData(bnd, GasdTm.Velocity) -
                    (reg->Grid.GetMaskedData(bnd, GasdTm.Pressure) -
                     reg->Grid.GetMaskedData(bnd - 1, GasdTm.Pressure)) /
                    (reg->Grid.GetMaskedData(bnd, Clc.MassCent) +
                     reg->Grid.GetMaskedData(bnd - 1, Clc.MassCent)) *
                    (2 * tStp);

            reg->Grid.GetMaskedData(bnd, GasdTp.PosX) =
                    reg->Grid.GetMaskedData(bnd, GasdTm.PosX) +
                    reg->Grid.GetMaskedData(bnd, GasdTp.Velocity) * tStp;

            reg->Grid.GetMaskedData(bndCent, GasdTp.Volume) =
                    (reg->Grid.GetMaskedData(bndCent + 1, GasdTp.PosX) -
                     reg->Grid.GetMaskedData(bndCent, GasdTp.PosX));

            reg->Grid.GetMaskedData(bndCent, GasdTp.Density) =
                    reg->Grid.GetMaskedData(bndCent, Clc.MassCent) /
                    reg->Grid.GetMaskedData(bndCent, GasdTp.Volume);

            // Make iteration Pressure!!! for visc, E
            MakeVisc(reg);
            reg->Grid.GetMaskedData(bndCent, GasdTp.Energy) =
                    reg->Grid.GetMaskedData(bndCent, GasdTm.Energy) -
                    (reg->Grid.GetMaskedData(bndCent, GasdTm.Pressure) +
                     ((reg->Grid.GetMaskedData(bndCent, Clc.ViscousePlus) -
                       reg->Grid.GetMaskedData(bndCent, Clc.Viscouse))
//Adding sound correction for the next time pressure
//                        + reg->Grid.GetMaskedData(bndCent, SoundName) * 0.5 *
//                        reg->Grid.GetMaskedData(bndCent, SoundName) *
//                                (reg->Grid.GetMaskedData(bndCent, DencPlusName) - reg->Grid.GetMaskedData(bndCent, DencName))
                    ) *
                     EnergyPresCoef) *
                    (reg->Grid.GetMaskedData(bndCent, GasdTp.Volume) -
                     reg->Grid.GetMaskedData(bndCent, GasdTm.Volume)) /
                    reg->Grid.GetMaskedData(bndCent, Clc.MassCent);

            TFluxPrepare::MakeBoundedPresSound(
                    reg, bndCent, bndCent, GasdTp.Density, GasdTp.Energy, EOSName, GasdTp.Pressure, 1);
            reg->Grid.GetMaskedData(bndCent, GasdTp.Pressure) +=
                    reg->Grid.GetMaskedData(bndCent, Clc.ViscousePlus);
            SetMinPres(reg, GasdTp.Pressure, MinPres);

            // VolDerivName - done for time stp
            reg->Grid.GetMaskedData(bndCent, Clc.VolDerive) =
                    (reg->Grid.GetMaskedData(bndCent, GasdTp.Volume) -
                     reg->Grid.GetMaskedData(bndCent, GasdTm.Volume)) /
                    ((reg->Grid.GetMaskedData(bndCent, GasdTp.Volume) +
                      reg->Grid.GetMaskedData(bndCent, GasdTm.Volume)) *
                     (0.5 * tStp));
            reg->Grid.GetMaskedData(bndCent, Clc.VolDerive) *=
                    (reg->Grid.GetMaskedData(bndCent + 1, GasdTp.PosX) -
                     reg->Grid.GetMaskedData(bndCent, GasdTp.PosX));


            for(auto &dst_src:vector<pair<Stroka, Stroka>>{
                    {Clc.Viscouse, Clc.ViscousePlus},
                    {GasdTm.Pressure, GasdTp.Pressure},
                    {GasdTm.Energy, GasdTp.Energy},
                    {GasdTm.Density, GasdTp.Density},
                    {GasdTm.Volume, GasdTp.Volume},
            }) reg->Grid.GetMaskedData(bndCent, dst_src.first) =
                       reg->Grid.GetMaskedData(bndCent, dst_src.second);
            for(auto &dst_src:vector<pair<Stroka, Stroka>>{
                    {GasdTm.Velocity, GasdTp.Velocity},
                    {GasdTm.PosX, GasdTp.PosX},
            }) reg->Grid.GetMaskedData(bnd, dst_src.first) =
                       reg->Grid.GetMaskedData(bnd, dst_src.second);


        }

        int save_data_state(FilterOut &so) {
            so << " MarchCourant " << MarchCourant << " CL " << CL << " C0 " << C0
               << " MinPres " << MinPres << SavableClass::EOLN();
            so << " EOSName " << EOSName << " PosName " << GasdTm.PosX << " DencName "
               << GasdTm.Density << " PresName " << GasdTm.Pressure << SavableClass::EOLN();
            so << " EnerName " << GasdTm.Energy << " VelName " << GasdTm.Velocity << " SoundName "
               << GasdTm.Sound << SavableClass::EOLN();
            so << " EnergyPresCoef " << EnergyPresCoef << " MinPresCoef " << MinPresCoef
               << SavableClass::EOLN();
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> Verify("MarchCourant") >> MarchCourant >> Verify("CL") >> CL >> Verify("C0") >> C0;
            si >> Verify("MinPres") >> MinPres >> Verify("EOSName") >> EOSName >> Verify("PosName") >> GasdTm.PosX;
            si >> Verify("DencName") >> GasdTm.Density >> Verify("PresName") >> GasdTm.Pressure;
            si >> Verify("EnerName") >> GasdTm.Energy >> Verify("VelName") >> GasdTm.Velocity;
            si >> Verify("SoundName") >> GasdTm.Sound;
            si >> Verify("EnergyPresCoef")  >> EnergyPresCoef >> Verify("MinPresCoef") >> MinPresCoef;
            return 1;
        }
        Stroka MakeHelp();


        double GetMaxTimeStp2Vacuum(TPolyRegion *reg) {
            TRegionBounds bnd(0, 0);
            double *vel = reg->GetDataPtr(bnd, GasdTm.Velocity);
            double *pos = reg->GetDataPtr(bnd, GasdTm.PosX);

            int size = reg->GetDataSize(bnd, GasdTm.Velocity);
            double dV = vel[size - 1] - vel[0], dX = pos[size - 1] - pos[0];
            if(dX < MathZer)
                return 1 / MathZer;
            dV = max<double>(MathZer, dV);
            return dX / dV;
        }
        void MakeTimeStep2Vacuum(TPolyRegion *reg, double tStp) {
            TRegionBounds bndX(0, 0);
            double *pos =
                    (double *)reg->GetDataPtr(bndX, GasdTm.PosX);
            int sizeX = reg->GetDataSize(bndX, GasdTm.PosX);
            if(pos[sizeX - 1] - pos[0] < MathZer)
                pos[sizeX - 1] = (pos[0] = 0.5 * (pos[sizeX - 1] + pos[0]));
            double x0 = pos[0], xStp = (pos[sizeX - 1] - pos[0]) / (sizeX - 1);
            for(int i = -1; i < sizeX + 1; i++)
                pos[i] = x0 + xStp * (double)i;
        }
        int IsVacuum(TPolyRegion *reg) {
            MatterIO *mat = SavableClass::TestType<MatterIO>(reg->MapSavableVar[EOSName]);
            return (dynamic_cast<MatterVacuum *>(mat) != NULL);
        }
        int VacuumIsClosed(TPolyRegion *reg) {
            TRegionBounds bnd(0, 0);
            double *pos =
                    (double *)reg->GetDataPtr(bnd, GasdTm.PosX);
            int size = reg->GetDataSize(bnd, GasdTm.PosX);
            return (pos[size - 1] - pos[0] < MathZer);
        }

        void MakeVisc(TPolyRegion *reg) {
            TRegionBounds bndCent(0, -1);
            double *velPlus = reg->GetDataPtr(bndCent, GasdTp.Velocity);
            double *denc = reg->GetDataPtr(bndCent, GasdTm.Density);
            double *dencPlus = reg->GetDataPtr(bndCent, GasdTp.Density);
            double *pres = reg->GetDataPtr(bndCent, GasdTm.Pressure);
            double *viscP = reg->GetDataPtr(bndCent, Clc.ViscousePlus);
            double *visc = reg->GetDataPtr(bndCent, Clc.Viscouse);

            int size = reg->GetDataSize(bndCent, GasdTm.Pressure);
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
    };

};   //namespace NPolygon {


