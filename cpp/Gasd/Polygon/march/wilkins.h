#pragma once

#include "CenterFlux.h"

namespace NPolygon {

    struct TMarchWilkins : TPolyMarchRegionBase {
    private:
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
        void InitRegion(TPolyRegion *reg) {
            reg->Grid.AddVar(VelPlusName, new TGridVar<double>());
            reg->Grid.AddVar(VolName, new TGridVar<double>());
            reg->Grid.AddVar(VolPlusName, new TGridVar<double>());
            reg->Grid.AddVar(PosPlusName, new TGridVar<double>());
            reg->Grid.AddVar(DencPlusName, new TGridVar<double>());

            reg->Grid.AddVar(MassName, new TGridVar<double>());
            reg->Grid.AddVar(VolDerivName, new TGridVar<double>());
            reg->Grid.AddVar(ViscName, new TGridVar<double>());
            reg->Grid.AddVar(ViscPlusName, new TGridVar<double>());

            reg->Grid.AddVar("Fake", new TGridVar<double>());

            reg->Grid.AddVar(PresPlusName, new TGridVar<double>());
            reg->Grid.AddVar(EnerPlusName, new TGridVar<double>());
            reg->Grid.AddVar(PI, new TGridVar<double>());
            reg->Grid.AddVar(II, new TGridVar<double>());
            reg->Grid.AddVar(EI, new TGridVar<double>());
            reg->Grid.AddVar(PIx, new TGridVar<double>());
            reg->Grid.AddVar(IIx, new TGridVar<double>());
            reg->Grid.AddVar(EIx, new TGridVar<double>());

            reg->Grid.AddVar(PIt, new TGridVar<double>());
            reg->Grid.AddVar(IIt, new TGridVar<double>());
            reg->Grid.AddVar(EIt, new TGridVar<double>());

            reg->Grid.AddVar(PIc, new TGridVar<double>());
            reg->Grid.AddVar(IIc, new TGridVar<double>());
            reg->Grid.AddVar(EIc, new TGridVar<double>());

            reg->Grid.AddVar(PIp, new TGridVar<double>());
            reg->Grid.AddVar(IIp, new TGridVar<double>());
            reg->Grid.AddVar(EIp, new TGridVar<double>());
            reg->Grid.AddVar(PIxp, new TGridVar<double>());
            reg->Grid.AddVar(IIxp, new TGridVar<double>());
            reg->Grid.AddVar(EIxp, new TGridVar<double>());
            reg->Grid.AddVar(Dx, new TGridVar<double>());

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
        }
        void MakeVisc(TPolyRegion *reg, const Stroka &viscName) {
            TRegionBounds bndCent(0, -1);
            double *velPlus = reg->GetDataPtr(bndCent, VelPlusName);
            double *denc = reg->GetDataPtr(bndCent, DencName);
            double *dencPlus = reg->GetDataPtr(bndCent, DencPlusName);
            double *pres = reg->GetDataPtr(bndCent, PresName);
            double *viscP = reg->GetDataPtr(bndCent, viscName);
            double *visc = reg->GetDataPtr(bndCent, ViscName);

            int size = reg->GetDataSize(bndCent, PresName);
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
            double *pres = reg->GetDataPtr(bndCent, presName);
            int size = reg->GetDataSize(bndCent, presName);
            for(int i = 0; i < size; i++) {
                if(pres[i] < minP)
                    pres[i] = minP + (pres[i] - minP) * MinPresCoef;
            }
        }
        double GetMaxTimeStp2Vacuum(TPolyRegion *reg) {
            //TRegionBounds bnd(0, -1);
            TRegionBounds bnd(0, 0);
            double *vel = reg->GetDataPtr(bnd, VelName);
            double *pos = reg->GetDataPtr(bnd, PosName);

            int size = reg->GetDataSize(bnd, VelName);
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
            int sizeX = reg->Grid.GetMaskedData(bndX, PresName).Start().Size();
            if(pos[sizeX - 1] - pos[0] < MathZer)
                pos[sizeX - 1] = (pos[0] = 0.5 * (pos[sizeX - 1] + pos[0]));
            double x0 = pos[0], xStp = (pos[sizeX - 1] - pos[0]) / (sizeX - 1);
            for(int i = -1; i < sizeX + 1; i++)
                pos[i] = x0 + xStp * (double)i;
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
        Stroka PresPlusName, EnerPlusName;
        Stroka PI, II, EI;
        Stroka PIx, IIx, EIx, PIt, IIt, EIt;
        Stroka PIc, IIc, EIc;
        Stroka PIp, IIp, EIp;
        Stroka PIxp, IIxp, EIxp;
        Stroka Dx;
        double TStp0_5;
    public:
        double CL, C0, MarchCourant;
    private:
        double MinPres, EnergyPresCoef, MinPresCoef;
        double CurTime;
        TRegBoundaryCircleX BoundaryMakerX;
        TRegBoundaryCircle_ZeroConst BoundaryMakerMass, BoundaryMakerPres,
            BoundaryMakerVel;

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
              VolDerivName("VolDeriv"),
              ViscPlusName("ViscPlus"),
              ViscName("Visc"),
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
              BoundaryMakerVel(TRegionBounds(0, -1), "0 1")
        {
            PresPlusName = "PresPlusName";
            EnerPlusName = "EnerPlusName";
            PI = "PI"; II = "II"; EI = "EI";
            PIx = "PIx"; IIx = "IIx"; EIx = "EIx";
            PIt = "PIt"; IIt = "IIt"; EIt = "EIt";
            PIc = "PIc"; IIc = "IIc"; EIc = "EIc";
            PIp = "PIp"; IIp = "IIp"; EIp = "EIp";
            PIxp = "PIxp"; IIxp = "IIxp"; EIxp = "EIxp";
            Dx = "Dx";
        }
        virtual void InitBase(TPolyRegion *head, double startTime) {
            for(TPolyRegion::TShallowIterator region = head->ShallowStart(); region.IsOk();
                region.Next()) {
                InitRegion(region.CurRegion());
            }
            RebuildBoundsBase(head);
        }
        virtual void RebuildBoundsBase(TPolyRegion *head) {
            for(TPolyRegion::TShallowIterator region = head->ShallowStart(); region.IsOk();
                region.Next()) {
                SetRegion(region);
            }
        }
        double GetMaxTimeStp(TPolyRegion *reg, double ) {
            if(IsVacuum(reg))
                return GetMaxTimeStp2Vacuum(reg);
            TRegionBounds bndCent(0, -1);
            double tStpPlus3_2 = 1 / MathZer;
            double cL = CL, c0 = C0;
            TFluxPrepare::MakeBoundedPresSound(
                reg, bndCent, bndCent, DencName, EnerName, EOSName, SoundName, 0);
            double *vol = reg->GetDataPtr(bndCent, VolName);
            double *sound = reg->GetDataPtr(bndCent, SoundName);
            double *vDer = reg->GetDataPtr(bndCent, VolDerivName);
            reg->Grid.GetMaskedData(bndCent, "Fake") =
                reg->Grid.GetMaskedData(bndCent + 1, VelName) -
                reg->Grid.GetMaskedData(bndCent, VelName);
            double *fake = reg->GetDataPtr(bndCent, "Fake");

            int size = reg->GetDataSize(bndCent, VolName);
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

        void MakeTimeStep(TPolyRegion *reg, double curTime, double tStp) {
            TStp0_5 = tStp;
            CurTime = curTime;
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
            reg->Grid.GetMaskedData(bndCent, EnerPlusName) =
                reg->Grid.GetMaskedData(bndCent, EnerName) -
                (reg->Grid.GetMaskedData(bndCent, PresName) +
                        ((reg->Grid.GetMaskedData(bndCent, ViscPlusName) -
                  reg->Grid.GetMaskedData(bndCent, ViscName))
//Adding sound correction for the next time pressure
//                        + reg->Grid.GetMaskedData(bndCent, SoundName) * 0.5 *
//                        reg->Grid.GetMaskedData(bndCent, SoundName) *
//                                (reg->Grid.GetMaskedData(bndCent, DencPlusName) - reg->Grid.GetMaskedData(bndCent, DencName))
                        ) *
                     EnergyPresCoef) *
                    (reg->Grid.GetMaskedData(bndCent, VolPlusName) -
                     reg->Grid.GetMaskedData(bndCent, VolName)) /
                    reg->Grid.GetMaskedData(bndCent, MassName);

            TFluxPrepare::MakeBoundedPresSound(
                reg, bndCent, bndCent, DencPlusName, EnerPlusName, EOSName, PresPlusName, 1);
            reg->Grid.GetMaskedData(bndCent, PresPlusName) +=
                reg->Grid.GetMaskedData(bndCent, ViscPlusName);
            SetMinPres(reg, PresPlusName, MinPres);

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

            ClcIntegrals(reg, tStp);



            reg->Grid.GetMaskedData(bndCent, ViscName) =
                    reg->Grid.GetMaskedData(bndCent, ViscPlusName);
            reg->Grid.GetMaskedData(bndCent, PresName) =
                    reg->Grid.GetMaskedData(bndCent, PresPlusName);
            reg->Grid.GetMaskedData(bndCent, EnerName) =
                    reg->Grid.GetMaskedData(bndCent, EnerPlusName);


            reg->Grid.GetMaskedData(bnd, VelName) =
                reg->Grid.GetMaskedData(bnd, VelPlusName);
            reg->Grid.GetMaskedData(bnd, PosName) =
                reg->Grid.GetMaskedData(bnd, PosPlusName);
            reg->Grid.GetMaskedData(bnd, VolName) =
                reg->Grid.GetMaskedData(bnd, VolPlusName);
            reg->Grid.GetMaskedData(bnd, DencName) =
                reg->Grid.GetMaskedData(bnd, DencPlusName);
        }

        void ClcIntegrals(TPolyRegion *reg, double tStp) {
            if(IsVacuum(reg))
                return;
            TRegionBounds bnd(0, 0);
            TRegionBounds bndCent(0, -1);
            reg->Grid.GetMaskedData(bndCent, PIx) =
                    (reg->Grid.GetMaskedData(bndCent + 1, PosName) - reg->Grid.GetMaskedData(bndCent, PosName)) * reg->Grid.GetMaskedData(bndCent, DencName);
            reg->Grid.GetMaskedData(bndCent, IIx) =
                    (reg->Grid.GetMaskedData(bndCent + 1, PosName) - reg->Grid.GetMaskedData(bndCent, PosName)) *
                       reg->Grid.GetMaskedData(bndCent, DencName) * reg->Grid.GetMaskedData(bndCent, VelName);
            reg->Grid.GetMaskedData(bndCent, EIx) =
                    (reg->Grid.GetMaskedData(bndCent + 1, PosName) - reg->Grid.GetMaskedData(bndCent, PosName)) *
                            reg->Grid.GetMaskedData(bndCent, DencName) * (reg->Grid.GetMaskedData(bndCent, VelName) * 0.5 * reg->Grid.GetMaskedData(bndCent, VelName) +
                                    reg->Grid.GetMaskedData(bndCent, EnerName));
            BuildSum(reg, PIx, PI);
            BuildSum(reg, IIx, II);
            BuildSum(reg, EIx, EI);
            reg->Grid.GetMaskedData(bndCent, PIxp) =
                    (reg->Grid.GetMaskedData(bndCent + 1, PosPlusName) - reg->Grid.GetMaskedData(bndCent, PosPlusName)) * reg->Grid.GetMaskedData(bndCent, DencPlusName);
            reg->Grid.GetMaskedData(bndCent, IIxp) =
                    (reg->Grid.GetMaskedData(bndCent + 1, PosPlusName) - reg->Grid.GetMaskedData(bndCent, PosPlusName)) *
                    reg->Grid.GetMaskedData(bndCent, DencPlusName) * reg->Grid.GetMaskedData(bndCent, VelPlusName);
            reg->Grid.GetMaskedData(bndCent, EIxp) =
                    (reg->Grid.GetMaskedData(bndCent + 1, PosPlusName) - reg->Grid.GetMaskedData(bndCent, PosPlusName)) *
                            reg->Grid.GetMaskedData(bndCent, DencPlusName) * (reg->Grid.GetMaskedData(bndCent, VelPlusName) * 0.5 * reg->Grid.GetMaskedData(bndCent, VelPlusName) +
                            reg->Grid.GetMaskedData(bndCent, EnerPlusName));
            BuildSum(reg, PIxp, PIp);
            BuildSum(reg, IIxp, IIp);
            BuildSum(reg, EIxp, EIp);
            reg->Grid.GetMaskedData(bndCent, PIx) = reg->Grid.GetMaskedData(bndCent, PIx) /
                    (reg->Grid.GetMaskedData(bndCent + 1, PosName) - reg->Grid.GetMaskedData(bndCent, PosName));
            reg->Grid.GetMaskedData(bndCent, IIx) = reg->Grid.GetMaskedData(bndCent, IIx) /
                    (reg->Grid.GetMaskedData(bndCent + 1, PosName) - reg->Grid.GetMaskedData(bndCent, PosName));
            reg->Grid.GetMaskedData(bndCent, EIx) = reg->Grid.GetMaskedData(bndCent, EIx) /
                    (reg->Grid.GetMaskedData(bndCent + 1, PosName) - reg->Grid.GetMaskedData(bndCent, PosName));

            reg->Grid.GetMaskedData(bndCent, Dx) =
                    (reg->Grid.GetMaskedData(bndCent + 1, PosPlusName) + reg->Grid.GetMaskedData(bndCent, PosPlusName) -
                    reg->Grid.GetMaskedData(bndCent + 1, PosName) - reg->Grid.GetMaskedData(bndCent, PosName)
                    ) * 0.5;

            reg->Grid.GetMaskedData(bndCent, PIt) =
                    (reg->Grid.GetMaskedData(bndCent, PIp) - reg->Grid.GetMaskedData(bndCent, PI)
                        - reg->Grid.GetMaskedData(bndCent, PIx) * reg->Grid.GetMaskedData(bndCent, Dx)
                    ) * (1/tStp);
            reg->Grid.GetMaskedData(bndCent, IIt) =
                    (reg->Grid.GetMaskedData(bndCent, IIp) - reg->Grid.GetMaskedData(bndCent, II)
                         - reg->Grid.GetMaskedData(bndCent, IIx) * reg->Grid.GetMaskedData(bndCent, Dx)
                    ) * (1/tStp);
            reg->Grid.GetMaskedData(bndCent, EIt) =
                    (reg->Grid.GetMaskedData(bndCent, EIp) - reg->Grid.GetMaskedData(bndCent, EI)
                         - reg->Grid.GetMaskedData(bndCent, EIx) * reg->Grid.GetMaskedData(bndCent, Dx)
                    ) * (1/tStp);


            reg->Grid.GetMaskedData(bndCent, PIc) =
                    reg->Grid.GetMaskedData(bndCent, PIt) + reg->Grid.GetMaskedData(bndCent, IIx);
            reg->Grid.GetMaskedData(bndCent, IIc) =
                    reg->Grid.GetMaskedData(bndCent, IIt) + (reg->Grid.GetMaskedData(bndCent, IIx) *
                    reg->Grid.GetMaskedData(bndCent, IIx) / reg->Grid.GetMaskedData(bndCent, PIx)) +
                    reg->Grid.GetMaskedData(bndCent, PresName);
            reg->Grid.GetMaskedData(bndCent, EIc) =
                    reg->Grid.GetMaskedData(bndCent, EIt) + (reg->Grid.GetMaskedData(bndCent, EIx) + reg->Grid.GetMaskedData(bndCent, PresName)) *
                    reg->Grid.GetMaskedData(bndCent, IIx) / reg->Grid.GetMaskedData(bndCent, PIx);
            SetSumBnd(reg);
        }
        void BuildSum(TPolyRegion *reg, Stroka &deltaName, Stroka &sumName) {
            TRegionBounds bnd(0, 0);
            double* delta = reg->GetDataPtr(bnd, deltaName);
            double* sum = reg->GetDataPtr(bnd, sumName);
            int len = reg->GetDataSize(bnd, deltaName);
            double s = sum[0];
            for(int i = 0; i < len - 1; i++) {
                sum[i] = s;
                s += delta[i];
            }
            verify(len - 1 > 0, "Integrate zero length region");
            sum[len - 1] = s;
        }
        void SetSumBnd(TPolyRegion *reg) {
            TPolyRegion *head = reg->Parent;
            TPolyRegion::TShallowIterator it = head->ShallowStart();
            for(;it.IsOk(); it.Next()) {
                if (it.CurRegion() == reg) break;
            }
            if (!it.IsOk()) return;
            TPolyRegion *next = it.GetNext(), *cur = it.CurRegion();
            if (cur != reg || next == NULL) return;

            TRegionBounds bndRgt(TRegionBounds::BNDBAD, 0),
                    bndRgtMin(TRegionBounds::BNDBAD, -1), bndLft(0, TRegionBounds::BNDBAD),
                    bndLftMin(-1, TRegionBounds::BNDBAD), bnd(0,0);
            next->Grid.GetMaskedData(bndLft, PI) =
                cur->Grid.GetMaskedData(bndRgt, PI);
            next->Grid.GetMaskedData(bndLft, II) =
                cur->Grid.GetMaskedData(bndRgt, II);
            next->Grid.GetMaskedData(bndLft, EI) =
                cur->Grid.GetMaskedData(bndRgt, EI);
            next->Grid.GetMaskedData(bndLft, PIp) =
                cur->Grid.GetMaskedData(bndRgt, PIp);
            next->Grid.GetMaskedData(bndLft, IIp) =
                cur->Grid.GetMaskedData(bndRgt, IIp);
            next->Grid.GetMaskedData(bndLft, EIp) =
                cur->Grid.GetMaskedData(bndRgt, EIp);
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


