
#pragma once

#include "gasd/polygon/lib/PolyMarchBase.h"
#include "gasd/polygon/lib/GridVectorVarImpl.h"
//#include "lib/std/ex_out.h"
#include "urs/matt_fac.h"

namespace NPolygon {

    struct TFluxPrepare {
        //static void MakePressure(TPolyRegion *reg, const TRegionBounds &bnds,
        //                const Stroka &dens, const Stroka &ener, const Stroka &eos, const Stroka &pres) {
        //    TGridMaskedData denc = reg->Grid.GetMaskedData(bnds, dens);
        //    TGridMaskedData energy = reg->Grid.GetMaskedData(bnds, ener);
        //    TGridMaskedData pressure = reg->Grid.GetMaskedData(bnds, pres);
        //    MatterIO *mat = SavableClass::TestType<MatterIO>(reg->MapSavableVar[eos]);
        //    double *presPtr = (double*)pressure.Start().GetElementPtr();
        //    double *enerPtr = (double*)energy.Start().GetElementPtr();
        //    double *dencPtr = (double*)denc.Start().GetElementPtr();
        //    int size = denc.Start().Size();
        //    mat->Pressure(--presPtr, --dencPtr, --enerPtr, size);
        //}
        //static void MakeSoundVar(TPolyRegion *reg, const TRegionBounds &bnds,
        //                const Stroka &dens, const Stroka &ener, const Stroka &eos, const Stroka &s) {
        //    TGridMaskedData denc = reg->Grid.GetMaskedData(bnds, dens);
        //    TGridMaskedData energy = reg->Grid.GetMaskedData(bnds, ener);
        //    TGridMaskedData sound = reg->Grid.GetMaskedData(bnds, s);
        //    MatterIO *mat = SavableClass::TestType<MatterIO>(reg->MapSavableVar[eos]);
        //    double *sPtr = (double*)sound.Start().GetElementPtr();
        //    double *enerPtr = (double*)energy.Start().GetElementPtr();
        //    double *dencPtr = (double*)denc.Start().GetElementPtr();
        //    int size = denc.Start().Size();
        //    mat->Sound(--sPtr, --dencPtr, --enerPtr, size);
        //    sPtr++;
        //    for(size_t i = 0; i < (size_t)size; i++)
        //        sPtr[i] = max<double>(0.3, sPtr[i]);
        //}
        static void SetVectorVar(
            TPolyRegion *reg,
            const Stroka &varNames,
            const Stroka &vecName) {
            TGridVectorVar<double> *vec = new TGridVectorVar<double>;
            reg->Grid.AddVar(vecName, vec);
            vector<Stroka> varN = Str::SplitLine(varNames);
            for(size_t i = 0; i < varN.size(); i++)
                vec->Push(reg->Grid.GetVar(varN[i]));
        }
        static void MakeBoundedPresSound(
            TPolyRegion *reg,
            const TRegionBounds &bndsClc,
            const TRegionBounds &bndsLimit,
            const Stroka &dens,
            const Stroka &ener,
            const Stroka &eos,
            const Stroka &res,
            int resIsPres,
            double minPressure = 1e-4,
            double minSound = 1) {
            TGridMaskedData denc = reg->Grid.GetMaskedData(bndsClc, dens);
            TGridMaskedData energy = reg->Grid.GetMaskedData(bndsClc, ener);
            TGridMaskedData result = reg->Grid.GetMaskedData(bndsClc, res);
            double *resPtr = (double *)result.Start().GetElementPtr();
            double *enerPtr = (double *)energy.Start().GetElementPtr();
            double *dencPtr = (double *)denc.Start().GetElementPtr();
            int curPos = 0;
            int sizeAll = denc.Start().Size();
            int sizeLft = bndsLimit.GetLft() - bndsClc.GetLft();
            int sizeRgt = bndsClc.GetRgt() - bndsLimit.GetRgt();
            int sizeBase = sizeAll - max<int>(0, sizeLft) - max<int>(0, sizeRgt);
            if(sizeLft > 0) {
                TPolyRegion *prev = FindNextRegion(reg, -1);
                curPos = ClcEOSPressure(
                    resPtr, enerPtr, dencPtr, prev, eos, 0, sizeLft, resIsPres, minPressure, minSound);
            }
            curPos = ClcEOSPressure(
                resPtr, enerPtr, dencPtr, reg, eos, curPos, sizeBase, resIsPres, minPressure, minSound);
            if(sizeRgt > 0) {
                TPolyRegion *next = FindNextRegion(reg, 1);
                ClcEOSPressure(
                    resPtr, enerPtr, dencPtr, next, eos, curPos, sizeRgt, resIsPres, minPressure, minSound);
            }
        }
        static int ClcEOSPressure(
            double *resPtr,
            double *enerPtr,
            double *dencPtr,
            TPolyRegion *reg,
            const Stroka &eos,
            int begPos,
            int size,
            int resIsPres,
            double minPressure,
            double minSound) {
            int lastPos = size + begPos + 1;
            double minVal = resIsPres ? minPressure : minSound;
            if(reg == NULL) {
                for(int i = begPos; i < lastPos; i++)
                    resPtr[i] = minVal;
                return lastPos;
            }
            MatterIO *mat = SavableClass::TestType<MatterIO>(reg->MapSavableVar[eos]);
            if(resIsPres) {
                mat->Pressure(
                    &resPtr[begPos - 1],
                    &dencPtr[begPos - 1],
                    &enerPtr[begPos - 1],
                    size);
            }
            else {
                mat->Sound(
                    &resPtr[begPos - 1],
                    &dencPtr[begPos - 1],
                    &enerPtr[begPos - 1],
                    size);
            }
            for (int i = begPos; i < lastPos; i++)
                resPtr[i] = max<double>(minVal, resPtr[i]);
            return lastPos;
        }
        static TPolyRegion *FindNextRegion(TPolyRegion *reg, int direction) {
            if(direction == 0)
                return reg;
            TPolyRegion::TShallowIterator it(reg->ParentRegion());
            for(; it.IsOk() && it.CurRegion() != reg; it.Next())
                ;
            if(!it.IsOk())
                return NULL;
            direction > 0 ? it.Next() : it.Prev();
            if(!it.IsOk())
                return NULL;
            return it.CurRegion();
        }
    };

};   //  namespace NPolygon {