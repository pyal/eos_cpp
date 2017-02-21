#pragma once

#include "CenterFlux.h"
#include "wilkins.h"
namespace NPolygon {

    struct InvariantNames {
        Stroka Mass, Impulse, InternalE, PdUIntegral;
        InvariantNames() {SetDefault();}
        void SetDefault() {
            Mass = "Mass";
            Impulse = "Impulse";
            InternalE = "InternalE";
            PdUIntegral = "PdUIntegral";
        }
        void AddPostfix(const char *postFix) {
            Mass += postFix;
            Impulse += postFix;
            InternalE += postFix;
            PdUIntegral += postFix;
        }
        vector<Stroka> GetNames() {
            return vector<Stroka>{Mass, Impulse, InternalE, PdUIntegral};
        }
    };

    struct TIntegralMarch : TMarchWilkins {

    private:
        void InitRegion1(TPolyRegion *reg) {
            for(auto &set:{InvarTm.GetNames(), InvarTp.GetNames(), GasdReconstr.GetNames(), vector<Stroka>{PressureCent}}) {
                for (auto &name:set) {
                    reg->Grid.AddVar(name, new TGridVar<double>());
                }
            }
        }

        void RebuildBounds(TPolyRegion::TShallowIterator &region) {
            TMarchWilkins::RebuildBounds(region);
        }



        InvariantNames InvarTm, InvarTp;
        GasdNames GasdReconstr;
        Stroka PressureCent;
    public:
        TIntegralMarch()
            : TMarchWilkins(){
            InvarTm.AddPostfix("_cur");
            InvarTp.AddPostfix("_plus");
            GasdReconstr.AddPostfix("_rebuild");
//            VelocityCent = "VelocityCent";
            PressureCent = "PressureCent";
        };
        virtual void InitBase(TPolyRegion *head, double startTime) {
            for(TPolyRegion::TShallowIterator region = head->ShallowStart(); region.IsOk();
                region.Next()) {
                InitRegion1(region.CurRegion());
            }
            TMarchWilkins::InitBase(head, startTime);
            for(TPolyRegion::TShallowIterator region = head->ShallowStart(); region.IsOk();
                region.Next()) {
                ClcInvariantDs(region.CurRegion(), InvarTp);
            }

            SumInvariants(head, InvarTp.Mass, InvarTm.Mass);
            SumInvariants(head, InvarTp.Impulse, InvarTm.Impulse);
            SumInvariants(head, InvarTp.InternalE, InvarTm.InternalE);
            SumInvariants(head, InvarTp.PdUIntegral, InvarTm.PdUIntegral);
        }
        virtual void RebuildBoundsBase(TPolyRegion *head) {
            TMarchWilkins::RebuildBoundsBase(head);
            TRegBoundaryCircle_ZeroConst bndMaker(TRegionBounds(0, -1), "0 1");
//            head->FillChildBounds(&bndMaker, InvarTm.Mass);
//            head->FillChildBounds(&bndMaker, InvarTm.Impulse);
//            head->FillChildBounds(&bndMaker, InvarTm.InternalE);
//            head->FillChildBounds(&bndMaker, InvarTm.PdUIntegral);
            SumInvariants(head, InvarTp.PdUIntegral, InvarTm.PdUIntegral);

        }
        void MakeTimeStep(TPolyRegion *reg, double curTime, double tStp) {
            InvarsStep(reg, tStp);
            TMarchWilkins::MakeTimeStep(reg, curTime, tStp);
            TRegionBounds bndCent(0, -1);
            reg->Grid.GetMaskedData(bndCent, InvarTp.PdUIntegral) =
                    reg->Grid.GetMaskedData(bndCent, GasdTm.Pressure) *
                    (reg->Grid.GetMaskedData(bndCent + 1, GasdTm.Velocity) -
                     reg->Grid.GetMaskedData(bndCent, GasdTm.Velocity)
                    );
        }

        Stroka MakeHelp();



        void ClcInvariantDs(TPolyRegion *reg, const InvariantNames &invarDelta) {
            TRegionBounds bndCent(0, -1);

            reg->Grid.GetMaskedData(bndCent, invarDelta.Mass) =
                    reg->Grid.GetMaskedData(bndCent, Clc.MassCent);

            reg->Grid.GetMaskedData(bndCent, Clc.VelocityCent) =
                    (reg->Grid.GetMaskedData(bndCent, GasdTm.Velocity) +
                     reg->Grid.GetMaskedData(bndCent + 1, GasdTm.Velocity) ) * 0.5;

            reg->Grid.GetMaskedData(bndCent, invarDelta.Impulse) =
                    reg->Grid.GetMaskedData(bndCent, invarDelta.Mass) *
                    reg->Grid.GetMaskedData(bndCent, Clc.VelocityCent);

            reg->Grid.GetMaskedData(bndCent, invarDelta.InternalE) =
                    reg->Grid.GetMaskedData(bndCent, invarDelta.Mass) *
                    (reg->Grid.GetMaskedData(bndCent, GasdTm.Energy)
                    );

            reg->Grid.GetMaskedData(bndCent, invarDelta.PdUIntegral) =
                    reg->Grid.GetMaskedData(bndCent, GasdTm.Pressure) *
                    (reg->Grid.GetMaskedData(bndCent + 1, GasdTm.Velocity) -
                     reg->Grid.GetMaskedData(bndCent, GasdTm.Velocity)
                    );
        }
        void SumInvariants(TPolyRegion *head, const Stroka &dsName, const Stroka &varName) {
            double start = 0;
            for(TPolyRegion::TShallowIterator region = head->ShallowStart(); region.IsOk();
                region.Next()) {
                start = SumDsReg(region.CurRegion(), dsName, varName, start);
            }

        }
        double SumDsReg(TPolyRegion *reg, const Stroka &deltaName, const Stroka &sumName, double startVal) {
            TRegionBounds bnd(0, 0);
            double* delta = reg->GetDataPtr(bnd, deltaName);
            double* sum = reg->GetDataPtr(bnd, sumName);
            int len = reg->GetDataSize(bnd, deltaName);
            double s = startVal;
            for(int i = 0; i < len - 1; i++) {
                sum[i] = s;
                s += delta[i];
            }
            verify(len - 1 > 0, "Integrate zero length region");
            sum[len - 1] = s;
            return s;
        }
        void InvarsStep(TPolyRegion *reg, double tStp) {
            TRegionBounds bnd(0, 0);
            TRegionBounds pBnd(-1, -1);
            TRegionBounds impBnd(0, -1);
            TRegionBounds uCentBnd(0, -1);
//            TRegionBounds pBnd(-3, 2);
//            TRegionBounds impBnd(-2, 2);
//            TRegionBounds uCentBnd(-2, 1);

            reg->Grid.GetMaskedData(pBnd, PressureCent) =
                    (reg->Grid.GetMaskedData(pBnd, GasdTm.Pressure) +
                     reg->Grid.GetMaskedData(pBnd + 1, GasdTm.Pressure)) * 0.5;

            reg->Grid.GetMaskedData(impBnd, InvarTp.Impulse) =
                    reg->Grid.GetMaskedData(impBnd, InvarTm.Impulse) -
                    reg->Grid.GetMaskedData(impBnd, PressureCent) * tStp;

            reg->Grid.GetMaskedData(uCentBnd, Clc.VelocityCent) =
                    (reg->Grid.GetMaskedData(uCentBnd + 1, InvarTp.Impulse) -
                     reg->Grid.GetMaskedData(uCentBnd, InvarTp.Impulse)) /
                    reg->Grid.GetMaskedData(uCentBnd, Clc.MassCent);
            ZeroMassFix(reg, uCentBnd, Clc.VelocityCent, Clc.MassCent);

            reg->Grid.GetMaskedData(bnd, InvarTp.InternalE) =
                    reg->Grid.GetMaskedData(bnd, InvarTm.InternalE) -
                    reg->Grid.GetMaskedData(bnd, InvarTm.PdUIntegral)
                    * tStp;

            TRegionBounds bndCent(0, -1);
            reg->Grid.GetMaskedData(bndCent, GasdReconstr.Velocity) =
                    (reg->Grid.GetMaskedData(bndCent + 1, InvarTp.Impulse) -
                     reg->Grid.GetMaskedData(bndCent, InvarTp.Impulse)) /
                            (reg->Grid.GetMaskedData(bndCent + 1, InvarTp.Mass) -
                                    reg->Grid.GetMaskedData(bndCent + 1, InvarTp.Mass));

            reg->Grid.GetMaskedData(bndCent, GasdReconstr.Energy) =
                    (reg->Grid.GetMaskedData(bndCent + 1, InvarTp.InternalE) -
                     reg->Grid.GetMaskedData(bndCent, InvarTp.InternalE)) /
                            (reg->Grid.GetMaskedData(bndCent + 1, InvarTp.Mass) -
                                    reg->Grid.GetMaskedData(bndCent + 1, InvarTp.Mass));


        }

    };

};   //namespace NPolygon {


