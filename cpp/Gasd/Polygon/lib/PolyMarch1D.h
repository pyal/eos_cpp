
#pragma once

#include "PolyMarchBase.h"
#include "lib/std/ex_out.h"
#include "urs/matt_fac.h"

namespace NPolygon {

    struct FluxCalculatorBase : SavableClass {
    protected:
        Stroka TmpBaseName;

    public:
        FluxCalculatorBase() : SavableClass(), TmpBaseName("Flux"){};
        virtual void void SetTmpBaseName(const Stroka &tmpBaseName) {
            TmpBaseName = tmpBaseName;
        }
        virtual void InitRegion(TPolyRegion *reg, const Stroka &uName) {}
        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            double dX_dT_coef){} = 0;
    };
    struct FluxClaculatorLargange : FluxCalculatorBase {
        FluxClaculatorLargange() : FluxCalculatorBase(){};
        virtual void InitRegion(TPolyRegion *reg, const Stroka &uName){};
        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            double dX_dT_coef) {}
    };
    struct FluxClaculatorSet : SimpleFluxCalculatorBase {
        FluxClaculator() : SimpleFluxCalculatorBase() {
            SetTmpBaseName(TmpBaseName);
        }
        FluxClaculator(int fluxType, FluxCalculatorBase *baseFlux)
            : FluxCalculatorBase, FluxType(fluxType), BaseFlux(baseFlux)() {
            SetTmpBaseName(TmpBaseName);
        }
        virtual void void SetTmpBaseName(const Stroka &tmpBaseName) {
            SimpleFluxCalculatorBase::SetTmpBaseName(tmpBaseName);
            FluxByName.clear();
            FluxByName["BASE"] = TmpBaseName + "_BASE";
            FluxByName["LF"] = TmpBaseName + "_LF";
            FluxByName["RI"] = TmpBaseName + "_RI";
            FluxByName["FORCE"] = TmpBaseName + "_FORCE";
        }
        enum FluxTypeNames { Base, LaxFriedrich, Richtmyer, Force };
        Ref<SimpleFluxCalculatorBase> BaseFlux;
        int FluxType;
        map<Stroka, Stroka> FluxByName;
        Stroka FluxName(int fluxType) {
            switch
                fluxType {
                case Base:
                    return FluxByName["BASE"];
                case LaxFriedrich:
                    return FluxByName["LF"];
                case Richtmyer:
                    return FluxByName["RI"];
                case Force:
                    return FluxByName["FORCE"];
                }
            throw info_except("FluxType <%i> is unknown\n", fluxType);
        }
        void InitRegion(TPolyRegion *reg, const Stroka &uName) {
            if(!reg->Grid.VarExists(FluxByName["BASE"])) {
                TGridVariablesBase *uData =
                    reg->Grid.GetMaskedData(uName, TRegGrid::AllData, 0).Data;
                reg->Grid.AddVar(
                    FluxByName["BASE"],
                    SavableClass::TestType<TGridVariablesBase>(uData->Duplicate()));
                reg->Grid.AddVar(
                    FluxByName["LF"],
                    SavableClass::TestType<TGridVariablesBase>(uData->Duplicate()));
                reg->Grid.AddVar(
                    FluxByName["RI"],
                    SavableClass::TestType<TGridVariablesBase>(uData->Duplicate()));
            }
        }
        virtual void ClcFlux(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            double dX_dT_coef) {
            switch
                FluxType {
                case LaxFriedrich:
                    ClcFluxLF(reg, uName, fName, dX_dT_coef);
                    break;
                case Richtmyer:
                    ClcFluxRI(reg, uName, fName, dX_dT_coef);
                    break;
                case Force:
                    ClcFluxForce(reg, uName, fName, dX_dT_coef);
                    break;
                default:
                    throw info_except(
                        "Type <%i> is unknown. Possible types <LaxFriedrich Richtmyer Force>\n",
                        FluxType);
                    break;
                }
        }
        void ClcFluxLF(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            double dX_dT_coef) {
            TGridMaskedData dst = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "fName");
            baseFlux->ClcFlux(reg, uName, FluxByName["BASE"], dX_dT_coef);
            TGridMaskedData fL =
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, FluxByName["Base"]);
            TGridMaskedData fR =
                reg->Grid.GetMaskedData(TRegGrid::Center, 1, FluxByName["Base"]);
            TGridMaskedData uL = reg->Grid.GetMaskedData(TRegGrid::Center, 0, uName);
            TGridMaskedData uR = reg->Grid.GetMaskedData(TRegGrid::Center, 1, uName);
            dst = ((fL + fR) + (uL - uR) * dX_dT_coef) * 0.5;
        }
        void ClcFluxRI(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            double dX_dT_coef) {
            TGridMaskedData dst = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "fName");
            baseFlux->ClcFlux(reg, uName, FluxByName["BASE"], dX_dT_coef);
            TGridMaskedData fL =
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, FluxByName["Base"]);
            TGridMaskedData fR =
                reg->Grid.GetMaskedData(TRegGrid::Center, 1, FluxByName["Base"]);
            TGridMaskedData uL = reg->Grid.GetMaskedData(TRegGrid::Center, 0, uName);
            TGridMaskedData uR = reg->Grid.GetMaskedData(TRegGrid::Center, 1, uName);
            dst = (fL + fR + (uL - uR) / dX_dT_coef) * 0.5;
        }
        void ClcFluxForce(
            TPolyRegion *reg,
            const Stroka &uName,
            const Stroka &fName,
            double dX_dT_coef) {
            TGridMaskedData dst = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "fName");
            ClcFluxLF(reg, uName, FluxByName["LF"], dX_dT_coef);
            ClcFluxRI(reg, uName, FluxByName["RI"], dX_dT_coef);
            TGridMaskedData fLF =
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, FluxByName["LF"]);
            TGridMaskedData fRI =
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, FluxByName["RI"]);
            dst = (fLF + fRI) * 0.5;
        }
    };
    struct FlicLimiter : SavableClass {
        Stroka DQName, VarRl, VarRR;
        FlicLimiter() : DQName("flicDQ"), VarRl("flicRl"), VarRr("flicRr"){};
        enum NamedLimiterType { SUPERBEE };
        void InitRegion(TPolyRegion *reg, const Stroka &baseRvar) {
            if(!reg->Grid.VarExists(DQName)) {
                TGridVariablesBase *rData =
                    reg->Grid.GetMaskedData(baseRvar, TRegGrid::AllData, 0).Data;
                reg->Grid.AddVar(
                    DQName,
                    SavableClass::TestType<TGridVariablesBase>(rData->Duplicate()));
                reg->Grid.AddVar(
                    VarRR,
                    SavableClass::TestType<TGridVariablesBase>(rData->Duplicate()));
                reg->Grid.AddVar(
                    VarRl,
                    SavableClass::TestType<TGridVariablesBase>(rData->Duplicate()));
            }
        }

        void ClcLimiter(
            TPolyRegion *reg,
            const Stroka &baseRvar,
            const Stroka &limiterVar,
            double courantNumber) {
            InitRegion(reg, baseRvar);
            reg->Grid.GetMaskedData(TRegGrid::Center, 1, DQName) =
                reg->Grid.GetMaskedData(TRegGrid::Center, 1, baseRvar);
            reg->Grid.GetMaskedData(TRegGrid::Center, 1, DQName) -=
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, baseRvar);
            reg->Grid.GetMaskedData(TRegGrid::Center, 0, VarRl) =
                reg->Grid.GetMaskedData(TRegGrid::Center, -1, DQName) /
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, DQName);
            reg->Grid.GetMaskedData(TRegGrid::Center, 0, VarRr) =
                reg->Grid.GetMaskedData(TRegGrid::Center, 1, DQName) /
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, DQName);
            ClcNamedLimiter(reg, VarRr, courantNumber, SUPERBEE);
            ClcNamedLimiter(reg, VarRl, courantNumber, SUPERBEE);
            TGridMaskedData::TIterator rIt(
                reg->Grid.GetaskedData(varRr, TRegGrid::Center, 0)),
                lIt(reg->Grid.GetMaskedData(varRl, TRegGrid::Center, 0));
            for(; rIt.IsOk(); rIt.Next(), lIt.Next()) {
                double &r = (double *)rIt.GetElementPtr();
                double &l = (double *)lIt.GetElementPtr();
                r = min(r, l);
            }
            reg->Grid.GetMaskedData(TRegGrid::Center, 0, limiterVar) =
                reg->Grid.GetMaskedData(TRegGrid::Center, 1, varRr);
        }
        void ClcNamedLimiter(
            TPolyRegion *reg,
            const Stroka &varR,
            double courantNumber,
            int limitType) {
            double phiG = (1 - courantNumber) / (1 + courantNumber);
            switch
                limitType {
                case SUPERBEE:
                    TGridMaskedData::TIterator varIt(
                        reg->Grid.GetMaskedData(varR, TRegGrid::Center, 0));
                    for(; varIt.IsOk(); varIt.Next()) {
                        double &r = (double *)varIt.GetElementPtr();
                        if(r < 0)
                            r = 0;
                        else if(r < 0.5)
                            r = 2 * r;
                        else if(r < 1)
                            r = 1;
                        else
                            r = min(2, phiG + (1 - phiG) * r);
                    }
                default:
                    throw info_except("Limiter type <%i> - unknown\n", limitType);
                }
        }
    };
    struct FlicLimiterFlux : SavableClass {
        FlicLimiter Limiter void ClcFlux(
            TPolyRegion *reg,
            const Stroka &fluxLow,
            const Stroka &fluxHigh,
            double courantNumber,
            const Stroka &baseRvar,
            const Stroka &resFlux) {
            Limiter.ClcLimiter(reg, baseRvar, "flicLim");
            reg->Grid.GetMaskedData(TRegGrid::Center, 0, resFlux) =
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, fluxLow) +
                reg->Grid.GetMaskedData(TRegGrid::Center, 0, "flicLim") *
                    (reg->Grid.GetMaskedData(TRegGrid::Center, 0, fluxHigh) -
                     reg->Grid.GetMaskedData(TRegGrid::Center, 0, fluxLow));
        }
    };
    struct TPolyMarchU2 : TPolyMarchRegionBase {
        TRegBoundaryCircle Bnd;
        TRegBoundaryCircleX BndX;
        TPolyMarchU2();
        virtual double GetMaxTimeStp(TPolyRegion *reg) {
            return 1;
        };
        void FillPressure(TPolyRegion *reg) {
            TGridMaskedData denc = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "R");
            //TGridMaskedData pos = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "X");
            TGridMaskedData energy = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "R");
            TGridMaskedData pressure = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "P");
            MatterIO *mat = SavableClass::TestType<MatterIO>(reg->MapSavableVar["EOS"]);
            double *presPtr = (double *)pressure.Start().GetElementPtr();
            double *enerPtr = (double *)energy.Start().GetElementPtr();
            double *dencPtr = (double *)denc.Start().GetElementPtr();
            int size = denc.Start().Size();
            mat->Pressure(--presPtr, --dencPtr, --enerPtr, size);
        }
        virtual void MakeTimeStep(TPolyRegion *reg, double curTime, double timeStp) {

            //TGridMaskedData cur(this, curMask);
            //TGridMaskedData::TIterator dencIter(cur), posIter(mdata);
            //TGridVar *ptr = SavableClass::TestType<TGridVar>(mdata.Data);
            //vector<T> &tmpStor = ptr->VectorT;
            //for( ; curIter.IsOk() && rgtIter.IsOk(); curIter.Next(), rgtIter.Next())
            //    VectorT[curIter.GetElementInd()] = tmpStor[rgtIter.GetElementInd()];


            //MakeTimeStepTestTime(reg, timeStp);
        }
        void MakeTimeStepTestTime(TPolyRegion *reg, double timeStp) {
            TGridMaskedData src = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "Y");
            TGridVar<double> *srcData =
                SavableClass::TestType<TGridVar<double>>(src.Data);
            if(!reg->Grid.VarExists("Tmp"))
                reg->Grid.AddVar("Tmp", new TGridVar<double>(*srcData));

            TGridMaskedData dst = reg->Grid.GetMaskedData(TRegGrid::Bound, 0, "Tmp");
            TGridVar<double> *dstData =
                SavableClass::TestType<TGridVar<double>>(dst.Data);

            size_t numIter = (size_t)reg->GetConstantDouble("CopyNumber");
            Time_struct Time;
            switch((int)reg->GetConstantDouble("CopyType")) {
            case 1:
                cout << " SetFast NumberTimes " << numIter << " NumberElements "
                     << dst.Mask->NumPoints() << "\n";
                for(size_t i = 0; i < numIter; i++)
                    dst = src;
                //dstData->Set(src.Mask, src);
                break;
            case 2:
                cout << " Adding NumberTimes " << numIter << " NumberElements "
                     << dst.Mask->NumPoints() << "\n";
                for(size_t i = 0; i < numIter; i++)
                    dst += src;
                //dstData->Add(src.Mask, src, 1);
                break;
            case 3:
                cout << " RawSet NumberTimes " << numIter << " NumberElements "
                     << dst.Mask->NumPoints() << "\n";
                for(size_t i = 0; i < numIter; i++)
                    dstData->Set(srcData);
                break;
            case 4:
                cout << " SetByInternalIteration NumberTimes " << numIter
                     << " NumberElements " << dst.Mask->NumPoints() << "\n";
                for(size_t i = 0; i < numIter; i++)
                    dstData->SetByInternalIteration(src.Mask, src);
                break;
            case 5:
                cout << " SetByIterator NumberTimes " << numIter << " NumberElements "
                     << dst.Mask->NumPoints() << "\n";
                for(size_t i = 0; i < numIter; i++)
                    dstData->SetByIterator(src.Mask, src);
                break;
            default:
                throw info_except(
                    "CopyType have to be in 1..3 and is - %i\n",
                    (int)reg->GetConstantDouble("CopyType"));
            }
            cout << "Done in " << Time << "\n";
        };
        virtual void RebuildBounds(TPolyRegion *reg) {
            vector<Stroka> names = reg->ShallowStart().CurRegion()->Grid.GetVarNames();
            for(size_t i = 0; i < names.size(); i++) {
                if(names[i] == "X")
                    reg->FillChildBounds(&BndX, names[i]);
                if(names[i] == "Y")
                    reg->FillChildBounds(&Bnd, names[i]);
            }
        };
    };

};   //namespace NPolygon {
