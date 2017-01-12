

#pragma once

#include "gasd/polygon/lib/PolyMarchBase.h"
#include "lib/std/ex_out.h"
#include "urs/matt_fac.h"
#include "lib/std/const.h"
#include "FluxPrepare.h"

namespace NPolygon {

    struct FluxCalculatorBase : SavableClass {
        //protected:
        //    Stroka TmpBaseName;
    public:
        FluxCalculatorBase() : SavableClass() {}   //, TmpBaseName("Flux"){};
        //virtual void SetTmpBaseName(const Stroka &tmpBaseName) {
        //    TmpBaseName = tmpBaseName;
        //}
        //virtual void InitRegion(TPolyRegion *reg, const Stroka &uName) {}
        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            const Stroka &xName,
            const TRegionBounds &bnds,
            double dT) = 0;
    };
    // given ( 0, 0 ) use (-1, 0)  -> (0, 0)
    struct FluxLF : FluxCalculatorBase {
        Stroka CalculatedFlux;
        FluxLF(const Stroka &calculatedFlux) : CalculatedFlux(calculatedFlux){};
        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            const Stroka &dxName,
            const TRegionBounds &bnds,
            double dT) {
            TRegionBounds bndsLft = bnds - 1, bndsRgt = bnds;
            TGridMaskedData flux(reg->Grid.GetMaskedData(bnds, fName));
            flux = reg->Grid.GetMaskedData(bndsLft, uName);
            flux.operator-=(reg->Grid.GetMaskedData(bndsRgt, uName));
            flux.operator*=(reg->Grid.GetMaskedData(bndsLft, dxName));
            flux.operator/=(dT);

            flux.operator+=(reg->Grid.GetMaskedData(bndsRgt, CalculatedFlux));
            flux.operator+=(reg->Grid.GetMaskedData(bndsLft, CalculatedFlux));
            flux.operator*=(0.5);
        }
    };
    // given ( 0, 0 ) use (-1, 0)  -> (0, 0)
    struct FluxRI : FluxCalculatorBase {
        Stroka CalculatedFlux, TmpFluxName;
        FluxCalculatorBase *FluxCalculator;
        FluxRI(const Stroka &calculatedFlux, FluxCalculatorBase *fluxCalculator)
            : CalculatedFlux(calculatedFlux),
              TmpFluxName("TmpFluxRIwau"),
              FluxCalculator(fluxCalculator){};
        void SetFluxCalculator(FluxCalculatorBase *fluxCalculator) {
            FluxCalculator = fluxCalculator;
        }
        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            const Stroka &dxName,
            const TRegionBounds &bnds,
            double dT) {
            TRegionBounds bndsLft = bnds - 1, bndsRgt = bnds;
            if(!reg->Grid.VarExists(TmpFluxName))
                reg->Grid.AddVar(
                    TmpFluxName,
                    SavableClass::TestType<TGridVariablesBase>(
                        reg->Grid.GetVar(uName)->Duplicate()));

            TGridMaskedData flux(reg->Grid.GetMaskedData(bnds, TmpFluxName));
            flux = reg->Grid.GetMaskedData(bndsLft, CalculatedFlux);
            flux -= reg->Grid.GetMaskedData(bndsRgt, CalculatedFlux);
            flux /= reg->Grid.GetMaskedData(bndsLft, dxName);
            flux *= dT;

            flux += reg->Grid.GetMaskedData(bndsLft, uName);
            flux += reg->Grid.GetMaskedData(bndsRgt, uName);
            flux *= 0.5;
            FluxCalculator->ClcFlux(reg, TmpFluxName, fName, dxName, bnds, dT);
            //FluxCalculator->ClcFlux(reg, uName, fName, dxName, bndsLft, dT);
        }
    };
    // given ( 0, 0 ) use (-1, 0)  -> (0, 0)
    struct FluxForce : FluxCalculatorBase {
        Stroka CalculatedRI, CalculatedLF;
        Ref<FluxCalculatorBase> FluxCalculator;
        FluxForce(const Stroka &calculatedLF, const Stroka &calculatedRI)
            : CalculatedRI(calculatedRI), CalculatedLF(calculatedLF){};

        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            const Stroka &dxName,
            const TRegionBounds &bnds,
            double dT) {
            //TRegionBounds bndsLft = bnds - 1, bndsRgt = bnds;
            TGridMaskedData flux(reg->Grid.GetMaskedData(bnds, fName));
            flux = reg->Grid.GetMaskedData(bnds, CalculatedLF);
            flux += reg->Grid.GetMaskedData(bnds, CalculatedRI);
            flux *= 0.5;
        }
    };


    struct FlicLimiter : SavableClass {
        Stroka DQName, VarRl, VarRr, VarTmp;
        FlicLimiter()
            : DQName("flicDQ"), VarRl("flicRl"), VarRr("flicRr"), VarTmp("flicTmp"){};
        int IsVector(
            TGridVar<double> *&var,
            TGridVectorVar<double> *&vec,
            TGridVariablesBase *dat) {
            var = dynamic_cast<TGridVar<double> *>(dat);
            vec = dynamic_cast<TGridVectorVar<double> *>(dat);
            if(!vec && !var)
                throw info_except("Bad var %s\n", typeid(*dat).name());
            return vec != NULL;
        }

        enum NamedLimiterType { SUPERBEE };
        void InitRegion(TPolyRegion *reg, const Stroka &baseRvar) {
            if(!reg->Grid.VarExists(DQName)) {
                TGridVariablesBase *rData = reg->Grid.GetVar(baseRvar);
                reg->Grid.AddVar(
                    DQName,
                    SavableClass::TestType<TGridVariablesBase>(rData->Duplicate()));
                reg->Grid.AddVar(
                    VarRr,
                    SavableClass::TestType<TGridVariablesBase>(rData->Duplicate()));
                reg->Grid.AddVar(
                    VarRl,
                    SavableClass::TestType<TGridVariablesBase>(rData->Duplicate()));
                reg->Grid.AddVar(VarTmp, new TGridVar<double>());
            }
        }

        // Use baseRvar(- 1, +2), limiterVar(0,0)
        // Result - (0,0)
        void ClcLimiter(
            TPolyRegion *reg,
            const Stroka &baseRvar,
            const Stroka &limiterVar,
            const Stroka &sound,
            const Stroka &dX,
            const TRegionBounds &bndsCenter,
            double tStp) {
            InitRegion(reg, baseRvar);
            TRegionBounds bndsPlus(bndsCenter.GetLft() - 1, bndsCenter.GetRgt() + 1);

            reg->Grid.GetMaskedData(bndsPlus, DQName) =
                reg->Grid.GetMaskedData(bndsPlus, baseRvar);
            reg->Grid.GetMaskedData(bndsPlus, DQName) -=
                reg->Grid.GetMaskedData(bndsPlus - 1, baseRvar);
            reg->Grid.GetMaskedData(bndsCenter, VarRl) =
                reg->Grid.GetMaskedData(bndsCenter - 1, DQName) /
                reg->Grid.GetMaskedData(bndsCenter, DQName);
            reg->Grid.GetMaskedData(bndsCenter, VarRr) =
                reg->Grid.GetMaskedData(bndsCenter + 1, DQName) /
                reg->Grid.GetMaskedData(bndsCenter, DQName);

            double courantNumber = 0;
            TGridVariablesBase *dxVar =
                reg->Grid.GetVar(dX);   //*soundVar = reg->Grid.GetVar(sound),
            for(TGridMaskedData::TIterator soundIt(
                    reg->Grid.GetMaskedData(bndsCenter, sound));
                soundIt.IsOk();
                soundIt.Next())
                courantNumber = max<double>(
                    courantNumber,
                    *(double *)soundIt.GetElementPtr() * tStp /
                        *(double *)soundIt.GetElementPtr(dxVar));

            cout << "Calculated CFL " << courantNumber << "\n";

            TGridVar<double> *varRlcalc, *varRrcalc;
            TGridVectorVar<double> *vecRlcalc, *vecRrcalc;
            reg->Grid.GetMaskedData(bndsCenter, limiterVar) =
                reg->Grid.GetMaskedData(bndsCenter, limiterVar) * 0 + 10;

            IsVector(varRlcalc, vecRlcalc, reg->Grid.GetVar(VarRl));
            if(IsVector(varRrcalc, vecRrcalc, reg->Grid.GetVar(VarRr))) {
                for(size_t i = 0; i < vecRrcalc->Dimension(); i++) {
                    SetLimiter(
                        reg,
                        limiterVar,
                        (*vecRrcalc)[i],
                        SUPERBEE,
                        bndsCenter,
                        courantNumber);
                    SetLimiter(
                        reg,
                        limiterVar,
                        (*vecRlcalc)[i],
                        SUPERBEE,
                        bndsCenter,
                        courantNumber);
                }
            } else {
                SetLimiter(
                    reg, limiterVar, varRrcalc, SUPERBEE, bndsCenter, courantNumber);
                SetLimiter(
                    reg, limiterVar, varRlcalc, SUPERBEE, bndsCenter, courantNumber);
            }
        }
        void SetLimiter(
            TPolyRegion *reg,
            const Stroka &limName,
            TGridVariablesBase *varRcalc,
            int limitType,
            const TRegionBounds &bndsCenter,
            double courantNumber) {
            ClcNamedLimiter(reg, VarTmp, varRcalc, SUPERBEE, bndsCenter, courantNumber);
            TGridVariablesBase *varTmp = reg->Grid.GetVar(VarTmp);

            for(TGridMaskedData::TIterator it(
                    reg->Grid.GetMaskedData(bndsCenter, limName));
                it.IsOk();
                it.Next()) {
                double &r = *(double *)it.GetElementPtr();
                r = min<double>(r, *(double *)it.GetElementPtr(varTmp));
            }
        }

        void ClcNamedLimiter(
            TPolyRegion *reg,
            const Stroka &varResName,
            TGridVariablesBase *varR,
            int limitType,
            const TRegionBounds &bndsCenter,
            double courantNumber) {
            double phiG = (1 - courantNumber) / (1 + courantNumber);
            TGridVariablesBase *varRes = reg->Grid.GetVar(varResName);

            switch(limitType) {
            case SUPERBEE:
                for(TGridMaskedData::TIterator varIt(
                        reg->Grid.GetMaskedData(bndsCenter, varR));
                    varIt.IsOk();
                    varIt.Next()) {
                    double r = *(double *)varIt.GetElementPtr();
                    double &res = *(double *)varIt.GetElementPtr(varRes);
                    if(r <= 0)
                        res = 0;
                    else if(r <= 0.5)
                        res = 2 * r;
                    else if(r <= 1)
                        res = 1;
                    else if(r > 1 && r < 1 / MathZer)
                        res = min<double>(2, phiG + (1 - phiG) * r);
                    else
                        res = 1;   // r = -INF
                }
                break;
            default:
                throw info_except("Limiter type <%i> - unknown\n", limitType);
            }
        }
    };
    struct FlicLimiterFlux : SavableClass {
        FlicLimiter Limiter;
        //TRegionBounds bndsCenter;
        void Init(TPolyRegion *reg, const Stroka &flux) {
            if(!reg->Grid.VarExists("flicLim")) {
//                TGridVariablesBase *rData = reg->Grid.GetVar(flux);
                reg->Grid.AddVar("flicLim", new TGridVar<double>());
                //reg->Grid.AddVar("flicLim", SavableClass::TestType<TGridVariablesBase>(rData->Duplicate()));
            }
        }
        // Use baseRvar(- 1, +2), limiterVar(0,0)
        // Result - (0,0)
        void ClcFlux(
            TPolyRegion *reg,
            const Stroka &fluxLow,
            const Stroka &fluxHigh,
            const Stroka &sound,
            const Stroka &dX,
            const Stroka &baseRvar,
            const Stroka &resFlux,
            const TRegionBounds &bndsCenter,
            double tStp) {
            Init(reg, fluxLow);
            Limiter.ClcLimiter(reg, baseRvar, "flicLim", sound, dX, bndsCenter, tStp);
            //reg->Grid.GetMaskedData(bndsCenter, resFlux) = reg->Grid.GetMaskedData(bndsCenter, fluxLow) +
            //    (reg->Grid.GetMaskedData(bndsCenter, fluxHigh) - reg->Grid.GetMaskedData(bndsCenter, fluxLow)) *
            //    reg->Grid.GetMaskedData(bndsCenter, "flicLim") * 0.5;
            reg->Grid.GetMaskedData(bndsCenter, resFlux) =
                reg->Grid.GetMaskedData(bndsCenter, fluxHigh);
            reg->Grid.GetMaskedData(bndsCenter, resFlux) -=
                reg->Grid.GetMaskedData(bndsCenter, fluxLow);
            reg->Grid.GetMaskedData(bndsCenter, resFlux) *=
                reg->Grid.GetMaskedData(bndsCenter, "flicLim");
            reg->Grid.GetMaskedData(bndsCenter, resFlux) +=
                reg->Grid.GetMaskedData(bndsCenter, fluxLow);
        }
    };
    struct ClcGasdStep {
        // P R E U
        // TVarsLagrange(1/R U E+U2/2)
        // FluxVarsLagrange(U P PU)
        void CreateVars(TPolyRegion *reg) {}
        void CreateBoundary(TPolyRegion *reg) {}
        double ClcTimeStp(TPolyRegion *reg) { return 1;}
        void MakeTimeStp(TPolyRegion *reg) {}
    };

    struct TPolyMarchU2 : TPolyMarchRegionBase {
        struct FluxClaculatorU2 : FluxCalculatorBase {
            FluxClaculatorU2()
                : FluxCalculatorBase(), SoundSpeed(1), NegativeSound(0), LinearFlux(0){};
            virtual void ClcFlux(
                TPolyRegion *reg,
                const Stroka &uName,
                const Stroka &fName,
                const Stroka &dxName,
                const TRegionBounds &bnds,
                double dT) {
                TGridMaskedData flux(reg->Grid.GetMaskedData(bnds, fName));
                flux = reg->Grid.GetMaskedData(bnds, uName);
                if(!LinearFlux)
                    flux *= flux;
                if(NegativeSound)
                    flux *= -SoundSpeed;
                else
                    flux *= SoundSpeed;
            }
            void ClcSound(
                TPolyRegion *reg,
                const Stroka &uName,
                const Stroka &sName,
                const TRegionBounds &bnds) {
                TGridMaskedData sound(reg->Grid.GetMaskedData(bnds, sName));
                if(!LinearFlux)
                    sound = reg->Grid.GetMaskedData(bnds, uName);
                else
                    sound = sound * 0 + 0.5;
                if(NegativeSound)
                    sound *= (-2 * SoundSpeed);
                else
                    sound *= (2 * SoundSpeed);
            }
            int save_data_state(FilterOut &so) {
                so << " SoundSpeed " << SoundSpeed << " NegativeSound " << NegativeSound
                   << " LinearFlux " << LinearFlux;
                return 1;
            };
            int read_data_state(FilterIn &si) {
                Stroka tmp;
                si >> tmp >> SoundSpeed >> tmp >> NegativeSound >> tmp >> LinearFlux;
                return 1;
            }

            double SoundSpeed;
            int NegativeSound, LinearFlux;
        };


        double MinPossibleTimeStp;
        FlicLimiterFlux ClcFlic;
        FluxForce ClcForce;
        FluxClaculatorU2 ClcU2;
        FluxLF ClcLF;
        FluxRI ClcRI;
        TRegionBounds BndCenteredU, BndBoundX, BndFluxU, BndTime;
        TRegBoundaryCircle BoundaryMakerY;
        TRegBoundaryCircleX BoundaryMakerX;
        TPolyMarchU2();   //: ClcU2(), ClcRI("FluxVar", &ClcU2), ClcLF("FluxVar"), ClcForce("FluxLF", "FluxRI") {}

        void Init(TPolyRegion *reg) {
            TGridMaskedData src(reg->Grid.GetMaskedData(TRegionBounds(-1, 1), "Y"));
            TGridVar<double> *srcData =
                SavableClass::TestType<TGridVar<double>>(src.Data);
            if(!reg->Grid.VarExists("FluxVar")) {
                //reg->Grid.AddVar("TVar", new TGridVar<double>(*srcData));
                reg->Grid.AddVar("FluxVar", new TGridVar<double>(*srcData));
                reg->Grid.AddVar("FluxLF", new TGridVar<double>(*srcData));
                reg->Grid.AddVar("FluxRI", new TGridVar<double>(*srcData));
                reg->Grid.AddVar("FluxForce", new TGridVar<double>(*srcData));
                reg->Grid.AddVar("FluxFlic", new TGridVar<double>(*srcData));
                reg->Grid.AddVar("dX", new TGridVar<double>(*srcData));
                reg->Grid.AddVar("C05", new TGridVar<double>(*srcData));
                BndCenteredU.Set(0, -1);
                BndBoundX.Set(0, 0);
                BndFluxU.Set(0, 1);
                BndTime.Set(-1, 1);


                BndFluxU.Set(-1, 1);

                BndBoundX.Set(0, 1);

                TGridMaskedData dX(reg->Grid.GetMaskedData(BndTime, "dX"));
                dX = reg->Grid.GetMaskedData(BndTime + 1, "X");
                dX -= reg->Grid.GetMaskedData(BndTime, "X");

                TGridMaskedData::TIterator itY =
                    reg->Grid.GetMaskedData(TRegionBounds(-2, 2), "Y").Start();
                TGridMaskedData::TIterator itX =
                    reg->Grid.GetMaskedData(TRegionBounds(-2, 2), "X").Start();
                TGridMaskedData::TIterator itE =
                    reg->Grid.GetMaskedData(TRegionBounds(-2, 2), "X").Last();
                double X0 = *(double *)reg->Grid.GetMaskedData(TRegionBounds(), "X")
                                 .Start()
                                 .GetElementPtr();
                double X1 = *(double *)reg->Grid.GetMaskedData(TRegionBounds(), "X")
                                 .Last()
                                 .GetElementPtr();
                for(; itX.IsOk(); itX.Next(), itY.Next()) {
                    *(double *)itY.GetElementPtr() =
                        5 +
                        5 * sin(2 * M_PI / (X1 - X0) *
                                (*(double *)itX.GetElementPtr() - X0));
                }
            }
        }
        // X(-1, 2)  Y(-1, 2)
        void RebuildBoundsBase(TPolyRegion *reg) {
            reg->FillChildBounds(&BoundaryMakerX, "X");
            reg->FillChildBounds(&BoundaryMakerY, "Y");
        }
        // Use dX(-1,1) Y(-1,1)
        double GetMaxTimeStp(TPolyRegion *reg, double ) {
            Init(reg);
            TGridMaskedData dX(reg->Grid.GetMaskedData(BndTime, "dX"));
            ClcU2.ClcSound(reg, "Y", "C05", BndTime);
            TGridMaskedData c05(reg->Grid.GetMaskedData(BndTime, "C05"));
            double maxTIStp = MathZer;
            for(TGridMaskedData::TIterator itC05(c05); itC05.IsOk(); itC05.Next()) {
                maxTIStp = max<double>(
                    maxTIStp,
                    fabs(
                        *(double *)itC05.GetElementPtr() /
                        *(double *)itC05.GetElementPtr(dX.Data)));
            }
            MinPossibleTimeStp = 1 / maxTIStp;
            return MinPossibleTimeStp * MarchCourant;
        }

        void MakeTimeStep(TPolyRegion *reg, double curTime, double tStp) {
            // Input Y(-1, 1)
            // Result - FluxVar(-1, 1)

            ClcU2.ClcFlux(reg, "Y", "FluxVar", "dX", BndFluxU, tStp);

            // Input Y(0, 1), FluxVar(0,1) dX(0,0)
            // Result - Flux*(0, 0)

            ClcLF.ClcFlux(reg, "Y", "FluxLF", "dX", BndBoundX, tStp);
            ClcRI.ClcFlux(reg, "Y", "FluxRI", "dX", BndBoundX, tStp);
            ClcForce.ClcFlux(reg, "Y", "FluxForce", "dX", BndBoundX, tStp);

            // Input Y(- 1, +2), Flux*(0,0)
            // Result - Y(0,-1)

            //ClcFlic.ClcFlux(reg, "FluxForce", "FluxRI", "C05", "dX", "Y", "FluxFlic", BndBoundX, tStp);
            ClcFlic.ClcFlux(
                reg,
                "FluxForce",
                "FluxRI",
                "C05",
                "dX",
                "Y",
                "FluxFlic",
                TRegionBounds(0, 0),
                tStp);
            SetDeltaFluxes(reg, "FluxVar FluxLF FluxRI FluxForce FluxFlic");

            reg->Grid.GetMaskedData(BndCenteredU, "Y") -=
                reg->Grid.GetMaskedData(BndCenteredU, MarchFlux) /
                reg->Grid.GetMaskedData(BndCenteredU, "dX") * tStp;
        }

        void SetDeltaFluxes(TPolyRegion *reg, const Stroka &fluxes) {
            vector<Stroka> vecF = Str::SplitLine(fluxes);
            for(size_t i = 0; i < vecF.size(); i++) {
                reg->Grid.GetMaskedData(BndCenteredU, vecF[i]) =
                    reg->Grid.GetMaskedData(BndCenteredU + 1, vecF[i]) -
                    reg->Grid.GetMaskedData(BndCenteredU, vecF[i]);
            }
        }
        Stroka MarchFlux;
        double MarchCourant;
        virtual void InitBase(TPolyRegion *baseNode, double startTime){};
        int save_data_state(FilterOut &so) {
            so << " MarchFlux(FluxVar:FluxLF:FluxRI:FluxForce:FluxFlic) " << MarchFlux
               << " MarchCourant " << MarchCourant;
            so << SavableClass::EOLN();
            ClcU2.save_data_state(so);
            so << SavableClass::EOLN();
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> MarchFlux >> tmp >> MarchCourant;
            ClcU2.read_data_state(si);
            return 1;
        }
    };


    struct TPolyMarchDriverFluxBase : FluxCalculatorBase {
        TPolyMarchDriverFluxBase() : FluxCalculatorBase() {}
        virtual void ClcSound(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &sName,
            const TRegionBounds &bnds) = 0;
        virtual Stroka GetRvarName(
            TPolyRegion *reg,
            const Stroka &uName,
            const TRegionBounds &bnd) = 0;
        virtual Stroka GetVarName() = 0;
        virtual void ClcDX(
            TPolyRegion *reg,
            const Stroka dXname,
            const TRegionBounds &bnds) = 0;
        virtual void RebuildBounds(TPolyRegion *reg) = 0;
        virtual void RebuildTimeDone(TPolyRegion *reg, double tStp){};
        virtual void InitRegion(TPolyRegion *reg){};
        virtual void StepVars(
            TPolyRegion *reg,
            const Stroka deltaFlux,
            const Stroka marchVars,
            const TRegionBounds &bnds) {
            reg->Grid.GetMaskedData(bnds, marchVars) +=
                reg->Grid.GetMaskedData(bnds, deltaFlux);
        }
    };

    struct TPolyMarchDriverFluxU2 : TPolyMarchDriverFluxBase {
        double SoundSpeed;
        int NegativeSound, LinearFlux;
        Stroka Xname, Yname;
        TRegBoundaryCircle BoundaryMakerY;
        TRegBoundaryCircleX BoundaryMakerX;
        TPolyMarchDriverFluxU2()
            : TPolyMarchDriverFluxBase(),
              SoundSpeed(1),
              NegativeSound(0),
              LinearFlux(0),
              Xname("X"),
              Yname("Y"),
              BoundaryMakerY(TRegionBounds(0, -1))
        {};
        virtual Stroka GetRvarName(
            TPolyRegion *reg,
            const Stroka &uName,
            const TRegionBounds &bnd) {
            return Yname;
        };
        virtual Stroka GetVarName() {
            return Yname;
        };
        virtual void ClcDX(
            TPolyRegion *reg,
            const Stroka dXname,
            const TRegionBounds &bnd) {
            TGridMaskedData dX(reg->Grid.GetMaskedData(bnd, dXname));
            dX = reg->Grid.GetMaskedData(bnd + 1, Xname);
            dX -= reg->Grid.GetMaskedData(bnd, Xname);
        }

        virtual void RebuildBounds(TPolyRegion *reg) {
            reg->FillChildBounds(&BoundaryMakerX, "X");
            reg->FillChildBounds(&BoundaryMakerY, "Y");
        }

        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            const Stroka &dxName,
            const TRegionBounds &bnds,
            double dT) {
            TGridMaskedData flux(reg->Grid.GetMaskedData(bnds, fName));
            flux = reg->Grid.GetMaskedData(bnds, uName);
            if(!LinearFlux)
                flux *= flux;
            if(NegativeSound)
                flux *= -SoundSpeed;
            else
                flux *= SoundSpeed;
        }
        void ClcSound(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &sName,
            const TRegionBounds &bnds) {
            TGridMaskedData sound(reg->Grid.GetMaskedData(bnds, sName));
            if(!LinearFlux)
                sound = reg->Grid.GetMaskedData(bnds, uName);
            else
                sound = sound * 0 + 0.5;
            if(NegativeSound)
                sound *= (-2 * SoundSpeed);
            else
                sound *= (2 * SoundSpeed);
        }
        int save_data_state(FilterOut &so) {
            so << " SoundSpeed " << SoundSpeed << " NegativeSound " << NegativeSound
               << " LinearFlux " << LinearFlux;
            so << "  Xname " << Xname << " Yname " << Yname;

            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> SoundSpeed >> tmp >> NegativeSound >> tmp >> LinearFlux;
            si >> tmp >> Xname >> tmp >> Yname;
            return 1;
        }
    };


    struct TPolyMarchDriver : TPolyMarchRegionBase {
        double MinPossibleTimeStp;
        FlicLimiterFlux ClcFlic;
        FluxForce ClcForce;
        Ref<TPolyMarchDriverFluxBase> ClcBaseFlux;
        FluxLF ClcLF;
        FluxRI ClcRI;
        TRegionBounds BndCenteredU, BndBoundX, BndFluxU, BndTime;
        TPolyMarchDriver()
            : TPolyMarchRegionBase(),
              ClcForce("FluxLF", "FluxRI"),
              ClcBaseFlux(new TPolyMarchDriverFluxU2()),
              ClcLF("FluxVar"),
              ClcRI("FluxVar", ClcBaseFlux),
              MarchFlux("FluxLF"),
              MarchCourant(0.5) {}

        virtual void InitBase(TPolyRegion *reg, double ) {
//            if(!reg->Grid.VarExists("FluxVar")) {
                ClcBaseFlux->InitRegion(reg);
                TGridMaskedData src(reg->Grid.GetMaskedData(
                    TRegionBounds(-1, 1), ClcBaseFlux->GetVarName()));

                reg->Grid.AddVar(
                    "FluxVar",
                    SavableClass::TestType<TGridVariablesBase>(src.Data->Duplicate()));
                reg->Grid.AddVar(
                    "FluxLF",
                    SavableClass::TestType<TGridVariablesBase>(src.Data->Duplicate()));
                reg->Grid.AddVar(
                    "FluxRI",
                    SavableClass::TestType<TGridVariablesBase>(src.Data->Duplicate()));
                reg->Grid.AddVar(
                    "FluxForce",
                    SavableClass::TestType<TGridVariablesBase>(src.Data->Duplicate()));
                reg->Grid.AddVar(
                    "FluxFlic",
                    SavableClass::TestType<TGridVariablesBase>(src.Data->Duplicate()));
                reg->Grid.AddVar("dX", new TGridVar<double>());
                reg->Grid.AddVar("C05", new TGridVar<double>());
                reg->Grid.AddVar(
                    "TimedDeltaFlux",
                    SavableClass::TestType<TGridVariablesBase>(src.Data->Duplicate()));
                BndCenteredU.Set(0, -1);
                BndBoundX.Set(0, 1);
                BndFluxU.Set(-1, 1);
                BndTime.Set(-1, 1);
                ClcBaseFlux->ClcDX(reg, "dX", BndTime);
//            }
        }

        void RebuildBoundsBase(TPolyRegion *reg) {
            ClcBaseFlux->RebuildBounds(reg);
        }
        // Use dX(-1,1) Y(-1,1)
        double GetMaxTimeStp(TPolyRegion *reg, double curTime) {
            InitBase(reg, curTime);
            TGridMaskedData dX(reg->Grid.GetMaskedData(BndTime, "dX"));
            ClcBaseFlux->ClcSound(reg, ClcBaseFlux->GetVarName(), "C05", BndTime);
            TGridMaskedData c05(reg->Grid.GetMaskedData(BndTime, "C05"));
            double maxTIStp = MathZer;
            for(TGridMaskedData::TIterator itC05(c05); itC05.IsOk(); itC05.Next()) {
                maxTIStp = max<double>(
                    maxTIStp,
                    fabs(
                        *(double *)itC05.GetElementPtr() /
                        *(double *)itC05.GetElementPtr(dX.Data)));
            }
            MinPossibleTimeStp = 1 / maxTIStp;
            return max<double>(MathZer, MinPossibleTimeStp * MarchCourant);
        }

        void MakeTimeStep(TPolyRegion *reg, double curTime, double tStp) {
            Stroka yName = ClcBaseFlux->GetVarName(),
                   rName = ClcBaseFlux->GetRvarName(reg, yName, BndBoundX);

            ClcBaseFlux->ClcFlux(reg, yName, "FluxVar", "dX", BndFluxU, tStp);

            ClcLF.ClcFlux(reg, yName, "FluxLF", "dX", BndBoundX, tStp);
            ClcRI.ClcFlux(reg, yName, "FluxRI", "dX", BndBoundX, tStp);
            ClcForce.ClcFlux(reg, yName, "FluxForce", "dX", BndBoundX, tStp);

            ClcFlic.ClcFlux(
                reg,
                "FluxForce",
                "FluxRI",
                "C05",
                "dX",
                rName,
                "FluxFlic",
                TRegionBounds(0, 0),
                tStp);
            SetDeltaFluxes(reg, "FluxVar FluxLF FluxRI FluxForce FluxFlic");

            reg->Grid.GetMaskedData(BndCenteredU, "TimedDeltaFlux") =
                ((reg->Grid.GetMaskedData(BndCenteredU, MarchFlux) /
                  reg->Grid.GetMaskedData(BndCenteredU, "dX")) *
                 (-tStp));
            ClcBaseFlux->StepVars(reg, "TimedDeltaFlux", yName, BndCenteredU);
            ClcBaseFlux->RebuildTimeDone(reg, tStp);
        }

        void SetDeltaFluxes(TPolyRegion *reg, const Stroka &fluxes) {
            vector<Stroka> vecF = Str::SplitLine(fluxes);
            for(size_t i = 0; i < vecF.size(); i++) {
                reg->Grid.GetMaskedData(BndCenteredU, vecF[i]) =
                    reg->Grid.GetMaskedData(BndCenteredU + 1, vecF[i]) -
                    reg->Grid.GetMaskedData(BndCenteredU, vecF[i]);
            }
        }
        Stroka MarchFlux;
        double MarchCourant;
        int save_data_state(FilterOut &so) {
            so << " MarchFlux(FluxVar:FluxLF:FluxRI:FluxForce:FluxFlic) " << MarchFlux
               << " MarchCourant " << MarchCourant;
            so << SavableClass::EOLN();
            so << " ClcBaseFlux " << ClcBaseFlux;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> MarchFlux >> tmp >> MarchCourant;
            si >> tmp >> ClcBaseFlux;
            ClcRI.SetFluxCalculator(ClcBaseFlux);
            return 1;
        }
        Stroka MakeHelp() {
            Stroka ret =
                " Basic class for time step calculation - constructing time step and modifed fluxes\n";
            ret +=
                "For Sound and basic FLux calculation uses FluxCalculatorBase class.\n";
            ret +=
                Stroka(
                    "~-\nRegistered NPolygon::FluxCalculator_category objects:\n\n~+\n") +
                SavableClass::HelpForCategory("NPolygon::FluxCalculator_category");
            ret + "~-\n";
            return ret;
        }
    };


    struct TPolyMarchDriverFluxGasdLagrangeImitation : TPolyMarchDriverFluxBase {
        Stroka Xname, DensName, VelName, EnerName, PresName, EOSName;
        Stroka CombiVar;
        TRegBoundaryCircle BoundaryMakerY;
        TRegBoundaryCircleX BoundaryMakerX;

        double SoundSpeed;
        int LinearFlux, NegativeSound;

        TPolyMarchDriverFluxGasdLagrangeImitation()
            : TPolyMarchDriverFluxBase(),
              Xname("X"),
              DensName("Density"),
              VelName("Velocity"),
              EnerName("Energy"),
              PresName("Pressure"),
              EOSName("EOS"),
              CombiVar("DencVelEner"),
              BoundaryMakerY(TRegionBounds(0, -1)),
              SoundSpeed(1),
              LinearFlux(0),
              NegativeSound(0){};
        virtual Stroka GetRvarName(
            TPolyRegion *reg,
            const Stroka &uName,
            const TRegionBounds &bnd) {
            return PresName;
        };
        virtual Stroka GetVarName() {
            return CombiVar;
        };
        virtual void ClcDX(
            TPolyRegion *reg,
            const Stroka dXname,
            const TRegionBounds &bnd) {
            TGridMaskedData dX(reg->Grid.GetMaskedData(bnd, dXname));
            dX = reg->Grid.GetMaskedData(bnd + 1, Xname);
            dX -= reg->Grid.GetMaskedData(bnd, Xname);
        }

        virtual void RebuildBounds(TPolyRegion *reg) {
            for(TPolyRegion::TShallowIterator region = reg->ShallowStart(); region.IsOk();
                region.Next())
                InitRegion(region.CurRegion());

            reg->FillChildBounds(&BoundaryMakerX, Xname);
            reg->FillChildBounds(&BoundaryMakerY, GetVarName());
        }
        void RebuildTimeDone(TPolyRegion *reg, double tStp){};
        virtual void InitRegion(TPolyRegion *reg) {
            //TGridMaskedData src(reg->Grid.GetMaskedData(TRegionBounds(-1,1), ClcBaseFlux->GetVarName()));
            if(!reg->Grid.VarExists(CombiVar)) {
                TFluxPrepare::SetVectorVar(
                    reg,
                    DensName + " " + VelName + " " + EnerName + " " + PresName,
                    CombiVar);
            }
        }

        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            const Stroka &dxName,
            const TRegionBounds &bnds,
            double dT) {
            TGridMaskedData flux(reg->Grid.GetMaskedData(bnds, fName));
            flux = reg->Grid.GetMaskedData(bnds, uName);
            if(!LinearFlux)
                flux *= flux;
            if(NegativeSound)
                flux *= -SoundSpeed;
            else
                flux *= SoundSpeed;
        }
        void ClcSound(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &sName,
            const TRegionBounds &bnds) {
            TGridMaskedData sound(reg->Grid.GetMaskedData(bnds, sName));
            TGridVectorVar<double> *vec =
                SavableClass::TestType<TGridVectorVar<double>>(reg->Grid.GetVar(uName));
            TGridVar<double> *dat = (*vec)[3];

            if(!LinearFlux)
                sound = reg->Grid.GetMaskedData(bnds, dat);
            else
                sound = sound * 0 + 0.5;
            if(NegativeSound)
                sound *= (-2 * SoundSpeed);
            else
                sound *= (2 * SoundSpeed);
        }
        int save_data_state(FilterOut &so) {
            so << " SoundSpeed " << SoundSpeed << " NegativeSound " << NegativeSound
               << " LinearFlux " << LinearFlux << SavableClass::EOLN();
            so << " Xname " << Xname << " DensName " << DensName << " VelName " << VelName
               << " EnerName " << EnerName << " PresName " << PresName << " EOSName "
               << EOSName << SavableClass::EOLN();

            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> SoundSpeed >> tmp >> NegativeSound >> tmp >> LinearFlux;
            si >> tmp >> Xname >> tmp >> DensName >> tmp >> VelName >> tmp >> EnerName >>
                tmp >> PresName >> tmp >> EOSName;
            return 1;
        }
    };


    struct TPolyMarchDriverFluxGasdLagrange : TPolyMarchDriverFluxBase {
        Stroka Xname, DencName, VelName, EnerName, PresName, EOSName;
        Stroka CombiVar;
        //TRegBoundarySame BoundaryMakerY;
        TRegBoundaryCircleX BoundaryMakerX;
        TRegBoundaryGasd BoundaryMakerY;
        TRegBoundaryCircle_ZeroConst BoundaryMakerMass;
        //TRegBoundarySame BoundaryMakerMass;

        Stroka MassName, InvDencName, EnergySumName, SoundName, VelPrevName, CombiRVar,
            TmpVarName;
        Stroka LimiterVarVector;
        int SetBrookDencity;
        double BrookFinalDencity;

        TPolyMarchDriverFluxGasdLagrange()
            : TPolyMarchDriverFluxBase(),
              Xname("X"),
              DencName("Density"),
              VelName("Velocity"),
              EnerName("Energy"),
              PresName("Pressure"),
              EOSName("EOS"),
              CombiVar("DencVelEner"),
              BoundaryMakerX(TRegionBounds(0, -1)),
              BoundaryMakerY(TRegionBounds(0, -1)),
              BoundaryMakerMass(TRegionBounds(0, -1), "0 0.5"),
              MassName("Mass"),
              InvDencName("InvDenc"),
              EnergySumName("EnergySum"),
              SoundName("Sound"),
              VelPrevName("VelPrev"),
              CombiRVar("DencVelEner_Limiter"),
              TmpVarName("TmpVar"),
              SetBrookDencity(1),
              BrookFinalDencity(7.89)
        {
            LimiterVarVector = EnerName + " " + DencName + " " + PresName + " " +
                               VelName + " " + SoundName;
        };
        virtual Stroka GetRvarName(
            TPolyRegion *reg,
            const Stroka &uName,
            const TRegionBounds &bnd) {
            return CombiRVar;
        };
        virtual Stroka GetVarName() {
            return CombiVar;
        };
        virtual void ClcDX(
            TPolyRegion *reg,
            const Stroka dXname,
            const TRegionBounds &bnd) {
            reg->Grid.GetMaskedData(bnd, dXname) = reg->Grid.GetMaskedData(bnd, MassName);
        }

        virtual void RebuildBounds(TPolyRegion *reg) {
            for(TPolyRegion::TShallowIterator region = reg->ShallowStart(); region.IsOk();
                region.Next())
                InitRegion(region.CurRegion());


            reg->FillChildBounds(&BoundaryMakerX, Xname);
            reg->FillChildBounds(&BoundaryMakerY, GetVarName());
            reg->FillChildBounds(&BoundaryMakerMass, MassName);
        }

        void RebuildTimeDone(TPolyRegion *reg, double tStp) {
            int level = reg->Grid.GetBoundarySize();
            TRegionBounds bnd(-level, level);
            //TRegionBounds bnd(0, -1);
            reg->Grid.GetMaskedData(bnd, DencName) =
                (reg->Grid.GetMaskedData(bnd, InvDencName) * 0 + 1) /
                reg->Grid.GetMaskedData(bnd, InvDencName);
            reg->Grid.GetMaskedData(bnd, Xname) +=
                (reg->Grid.GetMaskedData(bnd, VelName) +
                 reg->Grid.GetMaskedData(bnd, VelPrevName)) *
                (0.5 * tStp);
            reg->Grid.GetMaskedData(bnd, EnerName) =
                reg->Grid.GetMaskedData(bnd, EnergySumName) -
                (reg->Grid.GetMaskedData(bnd, VelName) *
                 reg->Grid.GetMaskedData(bnd, VelName)) *
                    0.5;
            reg->Grid.GetMaskedData(bnd, VelPrevName) =
                reg->Grid.GetMaskedData(bnd, VelName);
        };

        void SetStartVel(TPolyRegion *reg) {
            int numEl = 0;
            double s = reg->Grid.GetVar(VelName)->Size();
            TGridVariablesBase *denc = reg->Grid.GetVar(DencName);
            for(TGridMaskedData::TIterator it =
                    reg->Grid.GetMaskedData(TRegionBounds(-2, 2), VelName);
                it.IsOk();
                it.Next()) {
                if(numEl++ > s * 1 / 2) {
                    *(double *)it.GetElementPtr(denc) = BrookFinalDencity;
                }
            }
        }
        virtual void InitRegion(TPolyRegion *reg) {
            if(!reg->Grid.VarExists(CombiVar)) {
                reg->Grid.AddVar(MassName, new TGridVar<double>());
                reg->Grid.AddVar(InvDencName, new TGridVar<double>());
                reg->Grid.AddVar(EnergySumName, new TGridVar<double>());
                //reg->Grid.AddVar(SoundName, new TGridVar<double>());
                reg->Grid.AddVar(VelPrevName, new TGridVar<double>());
                reg->Grid.AddVar(TmpVarName, new TGridVar<double>());
                TRegionBounds bnd(0, -1);
                if(SetBrookDencity)
                    SetStartVel(reg);
                reg->Grid.GetMaskedData(bnd, MassName) =
                    reg->Grid.GetMaskedData(bnd, DencName) *
                    (reg->Grid.GetMaskedData(bnd + 1, Xname) -
                     reg->Grid.GetMaskedData(bnd, Xname));
                reg->Grid.GetMaskedData(bnd, InvDencName) =
                    (reg->Grid.GetMaskedData(bnd, InvDencName) * 0 + 1) /
                    reg->Grid.GetMaskedData(bnd, DencName);
                reg->Grid.GetMaskedData(bnd, Xname) =
                    (reg->Grid.GetMaskedData(bnd + 1, Xname) +
                     reg->Grid.GetMaskedData(bnd, Xname)) *
                    0.5;


                reg->Grid.GetMaskedData(bnd, VelPrevName) =
                    reg->Grid.GetMaskedData(bnd, VelName);

                reg->Grid.GetMaskedData(bnd, EnergySumName) =
                    reg->Grid.GetMaskedData(bnd, EnerName) +
                    (reg->Grid.GetMaskedData(bnd, VelName) *
                     reg->Grid.GetMaskedData(bnd, VelName)) *
                        0.5;

                TFluxPrepare::SetVectorVar(
                    reg, InvDencName + " " + VelName + " " + EnergySumName, CombiVar);

                //TFluxPrepare::MakePressure(reg, bnd, DencName, EnerName, EOSName, PresName);
                TFluxPrepare::MakeBoundedPresSound(
                    reg,
                    bnd,
                    TRegionBounds(0, -1),
                    DencName,
                    EnerName,
                    EOSName,
                    PresName,
                    1);
                ClcSound(reg, CombiVar, SoundName, bnd);

                TFluxPrepare::SetVectorVar(reg, LimiterVarVector, CombiRVar);
            }
        }
        //virtual void StepVars(TPolyRegion *reg, const Stroka deltaFlux, const Stroka marchVars, const TRegionBounds &bnds) {
        //    //TGridVectorVar<double> *vecMarch = SavableClass::TestType<TGridVectorVar<double> >(reg->Grid.GetVar(marchVars));
        //    //TGridVectorVar<double> *vecStep = SavableClass::TestType<TGridVectorVar<double> >(reg->Grid.GetVar(deltaFlux));
        //    ////for(size_t i = 1; i < 3; i++)
        //    ////    reg->Grid.GetMaskedData(bnds, (*vecMarch)[i]) += reg->Grid.GetMaskedData(bnds, (*vecStep)[i]);
        //    //double clcCoef = 1;
        //    //for(TGridMaskedData::TIterator dencIt(reg->Grid.GetMaskedData(bnds, (*vecMarch)[0])) ; dencIt.IsOk(); dencIt.Next()) {
        //    //    double &invDenc = *(double*)dencIt.GetElementPtr(), deltaF = *(double*)dencIt.GetElementPtr((*vecStep)[0]);
        //    //    double x = deltaF / invDenc;
        //    //    if (fabs(x) > 0.1)
        //    //        clcCoef = max<double>(clcCoef, 40 * fabs(x));
        //    //    //invDenc = exp(log(invDenc) + x);
        //    //    //invDenc = exp(log(invDenc) + x - x * x / 2);
        //    //}
        //    //if (clcCoef != 1) {
        //    //    cout<<"Limiting timeStp : "<<clcCoef<<"\n";
        //    //    reg->Grid.GetMaskedData(bnds, marchVars) += (reg->Grid.GetMaskedData(bnds, deltaFlux) * (1/clcCoef));
        //    //} else
        //        reg->Grid.GetMaskedData(bnds, marchVars) += reg->Grid.GetMaskedData(bnds, deltaFlux);
        //}

        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            const Stroka &dxName,
            const TRegionBounds &bnds,
            double dT) {
            TGridVectorVar<double> *uVec =
                SavableClass::TestType<TGridVectorVar<double>>(reg->Grid.GetVar(uName));

            reg->Grid.GetMaskedData(bnds, DencName) =
                (reg->Grid.GetMaskedData(bnds, (*uVec)[0]) * 0 + 1) /
                reg->Grid.GetMaskedData(bnds, (*uVec)[0]);
            reg->Grid.GetMaskedData(bnds, EnerName) =
                reg->Grid.GetMaskedData(bnds, (*uVec)[2]) -
                (reg->Grid.GetMaskedData(bnds, (*uVec)[1]) *
                 reg->Grid.GetMaskedData(bnds, (*uVec)[1])) *
                    0.5;

            TFluxPrepare::MakeBoundedPresSound(
                reg,
                bnds,
                TRegionBounds(0, -1),
                DencName,
                EnerName,
                EOSName,
                PresName,
                1);


            TGridVectorVar<double> *flux =
                SavableClass::TestType<TGridVectorVar<double>>(reg->Grid.GetVar(fName));
            reg->Grid.GetMaskedData(bnds, (*flux)[0]) =
                reg->Grid.GetMaskedData(bnds, (*uVec)[1]) * (-1);
            reg->Grid.GetMaskedData(bnds, (*flux)[1]) =
                reg->Grid.GetMaskedData(bnds, PresName);
            reg->Grid.GetMaskedData(bnds, (*flux)[2]) =
                reg->Grid.GetMaskedData(bnds, PresName) *
                reg->Grid.GetMaskedData(bnds, (*uVec)[1]);
        }
        void ClcSound(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &sName,
            const TRegionBounds &bnds) {
            TGridVectorVar<double> *uVec =
                SavableClass::TestType<TGridVectorVar<double>>(reg->Grid.GetVar(uName));

            reg->Grid.GetMaskedData(bnds, DencName) =
                (reg->Grid.GetMaskedData(bnds, (*uVec)[0]) * 0 + 1) /
                reg->Grid.GetMaskedData(bnds, (*uVec)[0]);
            reg->Grid.GetMaskedData(bnds, EnerName) =
                reg->Grid.GetMaskedData(bnds, (*uVec)[2]) -
                (reg->Grid.GetMaskedData(bnds, (*uVec)[1]) *
                 reg->Grid.GetMaskedData(bnds, (*uVec)[1])) *
                    0.5;

            //TFluxPrepare::MakeSoundVar(reg, bnds, DencName, EnerName, EOSName, sName);
            TFluxPrepare::MakeBoundedPresSound(
                reg, bnds, TRegionBounds(0, -1), DencName, EnerName, EOSName, sName, 0);


            //TGridMaskedData vel = reg->Grid.GetMaskedData(bndsClc, VelName);
            //TGridMaskedData energy = reg->Grid.GetMaskedData(bndsClc, ener);
            //TGridMaskedData result = reg->Grid.GetMaskedData(bndsClc, res);
            //double *resPtr = (double*)result.Start().GetElementPtr();
            //double *enerPtr = (double*)energy.Start().GetElementPtr();
            //double *dencPtr = (double*)denc.Start().GetElementPtr();
            //int curPos = 0;
            //int sizeAll = denc.Start().Size();
            //int sizeLft = bndsLimit.LftShift - bndsClc.LftShift;
            //int sizeRgt = bndsClc.RgtShift - bndsLimit.RgtShift;
            //int sizeBase = sizeAll - max<int>(0, sizeLft) - max<int>(0, sizeRgt);
            //if (sizeLft > 0) {
            //    TPolyRegion *prev = FindNextRegion(reg, -1);
            //    curPos = ClcEOSPressure(resPtr, enerPtr, dencPtr, prev, eos, 0, sizeLft, resIsPres);
            //}


            if(sName != SoundName)
                reg->Grid.GetMaskedData(bnds, SoundName) =
                    reg->Grid.GetMaskedData(bnds, sName);
            reg->Grid.GetMaskedData(bnds, TmpVarName).Abs(reg->Grid.GetMaskedData(bnds + 1, VelName) -
                                                          reg->Grid.GetMaskedData(bnds, VelName));
//            Abs(reg->Grid.GetMaskedData(bnds, TmpVarName),
//                (reg->Grid.GetMaskedData(bnds + 1, VelName) -
//                 reg->Grid.GetMaskedData(bnds, VelName)));
            reg->Grid.GetMaskedData(bnds, sName) +=
                reg->Grid.GetMaskedData(bnds, TmpVarName);


            reg->Grid.GetMaskedData(bnds, sName) *=
                reg->Grid.GetMaskedData(bnds, DencName);
        }
        int save_data_state(FilterOut &so) {
            so << " Xname " << Xname << " DencName " << DencName << " VelName " << VelName
               << " EnerName " << EnerName << " PresName " << PresName << " EOSName "
               << EOSName << " SoundName " << SoundName << SavableClass::EOLN();
            so << " LimiterVarVector(EnerName:PresName:VelName) "
               << Str::JoinLine(Str::SplitLine(LimiterVarVector, 0), ':')
               << SavableClass::EOLN();
            so << " SetBrookDencity " << SetBrookDencity << " BrookFinalDencity "
               << BrookFinalDencity << SavableClass::EOLN();

            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> Xname >> tmp >> DencName >> tmp >> VelName >> tmp >> EnerName >>
                tmp >> PresName >> tmp >> EOSName >> tmp >> SoundName >> tmp >>
                LimiterVarVector;
            si >> tmp >> SetBrookDencity >> tmp >> BrookFinalDencity;
            LimiterVarVector =
                Str::JoinLine(Str::SplitLine(LimiterVarVector, 0, ':'), ' ');
            return 1;
        }
    };


};   //namespace NPolygon {
