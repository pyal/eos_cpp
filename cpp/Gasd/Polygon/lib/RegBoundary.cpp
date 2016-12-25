#include "lib/precompiled/poly_lib.h"

#include "RegBoundary.h"
#include "PolyRegion.h"
//#include "GridMask.h"
#include "GridVectorVarImpl.h"

static ClassDesc NPolygon_TRegBoundaryCircle_ClassDesc(
    typeid(NPolygon::TRegBoundaryCircle),
    "NPolygon::TRegBoundaryCircle",
    1,
    "NPolygon::Boundary_category",
    create<NPolygon::TRegBoundaryCircle>);
static ClassDesc NPolygon_TRegBoundaryCircleX_ClassDesc(
    typeid(NPolygon::TRegBoundaryCircleX),
    "NPolygon::TRegBoundaryCircleX",
    1,
    "NPolygon::Boundary_category",
    create<NPolygon::TRegBoundaryCircleX>);

namespace NPolygon {


    void TRegBoundaryCircle::FillBoundPlus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        if(RegMinus == NULL)
            RegMinus = RegPlus->Parent->ShallowLast().CurRegion();

        int level = RegPlus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator minus =
            RegMinus->Grid.GetMaskedData(TRegionBounds(-level, level), varName).Last();
        TGridMaskedData::TIterator plus =
            RegPlus->Grid.GetMaskedData(TRegionBounds(-level, level), varName).Start();
        minus.Prev();   // shifting by one - boundary points are the same for regions...
        //plus.Move(level - 1);
        plus.Move(BaseBounds.LftAdd(level - 1));
        minus.Move(BaseBounds.RgtAdd(-level + 1));
        //plus.Move(level + BaseBounds.LftShift - 1);
        //minus.Move(-(level - BaseBounds.RgtShift) + 1);
        //for(size_t i = 0; i < (size_t)level; i++) {
        while(plus.IsOk()) {
            plus.SetElement(minus);
            minus.Prev();
            plus.Prev();
        }
    }
    void TRegBoundaryCircle::FillBoundMinus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        if(RegPlus == NULL)
            RegPlus = RegMinus->Parent->ShallowStart().CurRegion();

        int level = RegMinus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator minus =
            RegMinus->Grid.GetMaskedData(TRegionBounds(-level, level), varName).Last();
        TGridMaskedData::TIterator plus =
            RegPlus->Grid.GetMaskedData(TRegionBounds(-level, level), varName).Start();
        plus.Move(BaseBounds.LftAdd(level - 1));
        minus.Move(BaseBounds.RgtAdd(-level + 1));
        //plus.Move(level + BaseBounds.LftShift - 1);
        //minus.Move(1 - (level - BaseBounds.RgtShift));
        //minus.Move(1 - (level - BaseBounds.RgtShift));
        plus.Next();   // shifting by one - boundary points are the same for regions...
        //for(size_t i = 0; i < (size_t)level; i++) {
        while(minus.IsOk()) {
            //plus.SetElement(minus);
            minus.SetElement(plus);
            minus.Next();
            plus.Next();
        }
    }


    void TRegBoundaryCircleX::FillBoundPlus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        if(RegMinus == NULL)
            RegMinus = RegPlus->Parent->ShallowLast().CurRegion();

        int level = RegPlus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator minus =
            RegMinus->Grid.GetMaskedData(TRegionBounds(-level, level), varName).Last();
        TGridMaskedData::TIterator plus =
            RegPlus->Grid.GetMaskedData(TRegionBounds(-level, level), varName).Start();
        plus.Move(BaseBounds.LftAdd(level));
        minus.Move(BaseBounds.RgtAdd(-level));
        //minus.Move(-(level - BaseBounds.RgtShift));
        //plus.Move(level + BaseBounds.LftShift);
        //minus.Move(-level );
        //plus.Move( level  );

        double posPlus = *(double *)plus.GetElementPtr(),
               posMinus = *(double *)minus.GetElementPtr();
        //for(size_t i = 0; i < (size_t)level; i++) {
        //minus.Prev();
        //plus.Prev();
        for(; minus.IsOk() && plus.IsOk(); minus.Prev(), plus.Prev()) {
            double posMinus1 = *(double *)minus.GetElementPtr();
            *(double *)plus.GetElementPtr() = posPlus + posMinus1 - posMinus;
            posPlus += posMinus1 - posMinus;
            posMinus = posMinus1;
        }
    }
    void TRegBoundaryCircleX::FillBoundMinus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        if(RegPlus == NULL)
            RegPlus = RegMinus->Parent->ShallowStart().CurRegion();

        int level = RegMinus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator minus =
            RegMinus->Grid.GetMaskedData(TRegionBounds(-level, level), varName).Last();
        TGridMaskedData::TIterator plus =
            RegPlus->Grid.GetMaskedData(TRegionBounds(-level, level), varName).Start();
        plus.Move(BaseBounds.LftAdd(level));
        minus.Move(BaseBounds.RgtAdd(-level));
        //minus.Move(-(level - BaseBounds.RgtShift));
        //plus.Move(level + BaseBounds.LftShift);
        //minus.Move(-level );
        //plus.Move( level  );
        double posPlus = *(double *)plus.GetElementPtr(),
               posMinus = *(double *)minus.GetElementPtr();
        //for(size_t i = 0; i < (size_t)level; i++) {
        //minus.Next();
        //plus.Next();
        for(; minus.IsOk() && plus.IsOk(); minus.Next(), plus.Next()) {
            double posPlus1 = *(double *)plus.GetElementPtr();
            *(double *)minus.GetElementPtr() = posMinus + posPlus1 - posPlus;
            posMinus += posPlus1 - posPlus;
            posPlus = posPlus1;
        }
    }

    void TRegBoundarySame::FillBoundPlus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        int level = RegPlus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator base =
            RegPlus->Grid.GetMaskedData(BaseBounds, varName).Start();

        for(TGridMaskedData::TIterator plus =
                RegPlus->Grid.GetMaskedData(TRegionBounds(-level, 0), varName);
            plus != base;
            plus.Next()) {
            plus.SetElement(base);
        }
    }

    void TRegBoundarySame::FillBoundMinus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        int level = RegMinus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator base =
            RegMinus->Grid.GetMaskedData(BaseBounds, varName).Last();
        for(TGridMaskedData::TIterator minus =
                RegMinus->Grid.GetMaskedData(TRegionBounds(0, level), varName).Last();
            minus != base;
            minus.Prev()) {
            minus.SetElement(base);
        }
    }

    void TRegBoundaryCircle_ZeroConst::FillBoundPlus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        if(RegMinus != NULL)
            return TRegBoundaryCircle::FillBoundPlus(varName, RegMinus, RegPlus);
        int level = RegPlus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator base =
            RegPlus->Grid.GetMaskedData(BaseBounds, varName).Start();
        double val = *(double *)base.GetElementPtr(), set = 0;
        for(size_t i = 0; i < ZeroBndValCoefs.size(); i++)
            set += pow(val, (int)i) * ZeroBndValCoefs[i];

        for(TGridMaskedData::TIterator plus =
                RegPlus->Grid.GetMaskedData(TRegionBounds(-level, 0), varName);
            plus != base;
            plus.Next()) {
            *(double *)plus.GetElementPtr() = set;
        }
    }

    void TRegBoundaryCircle_ZeroConst::FillBoundMinus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        if(RegPlus != NULL)
            return TRegBoundaryCircle::FillBoundMinus(varName, RegMinus, RegPlus);
        int level = RegMinus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator base =
            RegMinus->Grid.GetMaskedData(BaseBounds, varName).Last();
        double val = *(double *)base.GetElementPtr(), set = 0;
        for(size_t i = 0; i < ZeroBndValCoefs.size(); i++)
            set += pow(val, (int)i) * ZeroBndValCoefs[i];
        for(TGridMaskedData::TIterator minus =
                RegMinus->Grid.GetMaskedData(TRegionBounds(0, level), varName).Last();
            minus != base;
            minus.Prev()) {
            *(double *)minus.GetElementPtr() = set;
        }
    }


    void TRegBoundaryGasd::FillBoundPlus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        //map<Stroka, Stroka> VarMap = Str::ReadParams(varName, "Vel InvDenc EnerSum");
        if(RegMinus != NULL)
            return TRegBoundaryCircle::FillBoundPlus(varName, RegMinus, RegPlus);
        int level = RegPlus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator start =
            RegPlus->Grid.GetMaskedData(BaseBounds, varName).Start();
//        TGridVectorVar<double> *invDencVelEnersum =
//            SavableClass::TestType<TGridVectorVar<double>>(RegPlus->Grid.GetVar(varName));
        for(TGridMaskedData::TIterator it =
                RegPlus->Grid.GetMaskedData(TRegionBounds(-level, level), varName);
            it.IsOk() && it != start;
            it.Next()) {
            it.SetElement(start);
            //*(double*)it.GetElementPtr((*invDencVelEnersum)[0]) = *(double*)start.GetElementPtr((*invDencVelEnersum)[0]); // invDenc
            //*(double*)it.GetElementPtr((*invDencVelEnersum)[1]) = *(double*)start.GetElementPtr((*invDencVelEnersum)[1]); // Velocity
            //*(double*)it.GetElementPtr((*invDencVelEnersum)[2]) = *(double*)start.GetElementPtr((*invDencVelEnersum)[2]);//0; // Energy
            ////*(double*)it.GetElementPtr((*invDencVelEnersum)[2]) = 0; // Energy
        }
    }
    void TRegBoundaryGasd::FillBoundMinus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        if(RegPlus != NULL)
            return TRegBoundaryCircle::FillBoundMinus(varName, RegMinus, RegPlus);
        int level = RegMinus->Grid.GetBoundarySize();
        TGridMaskedData::TIterator end =
            RegMinus->Grid.GetMaskedData(BaseBounds, varName).Last();
//        TGridVectorVar<double> *invDencVelEnersum =
//            SavableClass::TestType<TGridVectorVar<double>>(
//                RegMinus->Grid.GetVar(varName));
        for(TGridMaskedData::TIterator it =
                RegMinus->Grid.GetMaskedData(TRegionBounds(-level, level), varName)
                    .Last();
            it.IsOk() && it != end;
            it.Prev()) {
            it.SetElement(end);
            //*(double*)it.GetElementPtr((*invDencVelEnersum)[0]) = *(double*)end.GetElementPtr((*invDencVelEnersum)[0]);//1/MathZer; // invDenc
            //*(double*)it.GetElementPtr((*invDencVelEnersum)[1]) = *(double*)end.GetElementPtr((*invDencVelEnersum)[1]); // Velocity
            //*(double*)it.GetElementPtr((*invDencVelEnersum)[2]) = *(double*)end.GetElementPtr((*invDencVelEnersum)[2]);//0; // Energy
            //////////*(double*)it.GetElementPtr((*invDencVelEnersum)[2]) = 0; // Energy
        }
    }


    void TRegBoundaryWilkins::FillBoundMinus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        int level = RegMinus->Grid.GetBoundarySize();
        double bndVal =
            ClcBndVal(*(double *)RegMinus->Grid.GetMaskedData(BaseBounds, varName)
                           .Last()
                           .GetElementPtr());
        // curM + bndLen = level
        int curM = BaseBounds.GetRgt() + 1, bndLen = level - curM,
            curP = BaseBounds.GetLft() - bndLen - 1;
        for(TGridMaskedData::TIterator
                itM = RegMinus->Grid.GetMaskedData(TRegionBounds(curP, curM), varName)
                          .Last();
            itM.IsOk();
            itM.Next(), curP++, curM++) {
            if(RegPlus != NULL) {
                TGridMaskedData::TIterator itP =
                    RegPlus->Grid.GetMaskedData(TRegionBounds(curM, curP), varName)
                        .Start();
                itM.SetElement(itP);
            } else
                *(double *)itM.GetElementPtr() = bndVal;
        }
    }

    void TRegBoundaryWilkins::FillBoundPlus(
        const Stroka &varName,
        TPolyRegion *RegMinus,
        TPolyRegion *RegPlus) {
        int level = RegPlus->Grid.GetBoundarySize();
        double bndVal =
            ClcBndVal(*(double *)RegPlus->Grid.GetMaskedData(BaseBounds, varName)
                           .Start()
                           .GetElementPtr());
        // curP - bndLen = -level
        int curP = BaseBounds.GetLft() - 1, bndLen = level + curP,
            curM = BaseBounds.GetRgt() + bndLen + 1;
        for(TGridMaskedData::TIterator
                itP = RegMinus->Grid.GetMaskedData(TRegionBounds(curP, curM), varName)
                          .Start();
            itP.IsOk();
            itP.Prev(), curP--, curM--) {
            if(RegMinus != NULL) {
                TGridMaskedData::TIterator itM =
                    RegPlus->Grid.GetMaskedData(TRegionBounds(curM, curP), varName)
                        .Last();
                itP.SetElement(itM);
            } else
                *(double *)itP.GetElementPtr() = bndVal;
        }
    }


};   //namespace NPolygon {


//int TRegBoundaryCircle::save_data_state( FilterOut&so) {
//    return 1;
//};
//int TRegBoundaryCircle::read_data_state(FilterIn&si) {
//    return 1;
//};

//void TRegBoundaryCircle::FillBounds(const Stroka &varName, TPolyRegion *RegMinus, TPolyRegion *RegPlus, TPolyRegion *RegCur) {
//    TPolyRegion * regMinus = RegCur, *regPlus = RegCur;
//    int level = RegCur->Grid.GetBoundarySize();
//    if (RegMinus == NULL)
//        regMinus = RegCur->Parent->ShallowLast().CurRegion();
//    if (RegPlus == NULL)
//        regPlus = RegCur->Parent->ShallowStart().CurRegion();

//    TGridMaskedData::TIterator minus, plus;
//    if (RegCur == RegPlus) {
//        plus = regPlus->Grid.GetMaskedData(TRegGrid::Bound, -level, varName).Start();
//        minus = regMinus->Grid.GetMaskedData(TRegGrid::Bound, 0, varName).Last();

//    } else {
//        plus = regPlus->Grid.GetMaskedData(TRegGrid::Bound, 0, varName).Start();
//        minus = regMinus->Grid.GetMaskedData(TRegGrid::Bound, level, varName).Last();
//    }
//    for(size_t i = 0; i < (size_t)level; i++) {
//        if (RegCur == RegPlus)
//            minus.Prev();
//        else
//            plus.Next();
//    }
//    for(size_t i = 0; i < (size_t)level; i++) {
//        if (RegCur == RegPlus) {
//            plus.SetElement(minus);
//            minus.Next();
//            plus.Next();
//        }
//        else {
//            minus.SetElement(plus);
//            minus.Prev();
//            plus.Prev();
//        }
//    }
//
//}
//void TRegBoundaryCircleX::FillBounds(const Stroka &varName, TPolyRegion *RegMinus, TPolyRegion *RegPlus, TPolyRegion *RegCur) {
//    TPolyRegion * regMinus = RegCur, *regPlus = RegCur;
//    int level = RegCur->Grid.GetBoundarySize();
//    if (RegMinus == NULL)
//        regMinus = RegCur->Parent->ShallowLast().CurRegion();
//    if (RegPlus == NULL)
//        regPlus = RegCur->Parent->ShallowStart().CurRegion();

//    TGridMaskedData::TIterator minus, plus;
//    if (RegCur == RegPlus) {
//        plus = regPlus->Grid.GetMaskedData(TRegGrid::Bound, -level, varName).Start();
//        minus = regMinus->Grid.GetMaskedData(TRegGrid::Bound, 0, varName).Last();

//    } else {
//        plus = regPlus->Grid.GetMaskedData(TRegGrid::Bound, 0, varName).Start();
//        minus = regMinus->Grid.GetMaskedData(TRegGrid::Bound, level, varName).Last();
//    }
//    for(size_t i = 0; i < (size_t)level; i++) {
//        if (RegCur == RegPlus)
//            plus.Next();
//        else
//            minus.Prev();
//    }
//    double posPlus = *(double*)plus.GetElementPtr(), posMinus = *(double*)minus.GetElementPtr();
//    for(size_t i = 0; i < (size_t)level; i++) {
//        if (RegCur == RegPlus) {
//            minus.Prev();
//            double posMinus1 = *(double*)minus.GetElementPtr();
//            *(double*)plus.GetElementPtr() = posPlus + posMinus1 - posMinus;
//            posPlus += posMinus1 - posMinus;
//            posMinus = posMinus1;
//            plus.Prev();
//        }
//        else {
//            plus.Next();
//            double posPlus1 = *(double*)plus.GetElementPtr();
//            *(double*)minus.GetElementPtr() = posMinus + posPlus1 - posPlus;
//            posMinus += posPlus1 - posPlus;
//            posPlus = posPlus1;
//            minus.Next();
//        }
//    }
//
//}
