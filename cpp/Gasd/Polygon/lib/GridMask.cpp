#include "lib/precompiled/poly_lib.h"


#include "GridMask.h"

namespace NPolygon {

    //TGridMaskedDataIterator TGridMaskedData::Start() {
    //    return TGridMaskedDataIterator(*this);
    //}
    //TGridMaskedDataIterator TGridMaskedData::Last() {
    //    TGridMaskedDataIterator it;
    //    it.Last(*this);
    //    return it;
    //}
    int TRegionBounds::BNDBAD = -23456;
    TRegionBounds operator+(const TRegionBounds &rgt, int i) {
        return TRegionBounds(rgt.LftAdd(i), rgt.RgtAdd(i));
    }
    TRegionBounds operator-(const TRegionBounds &rgt, int i) {
        return TRegionBounds(rgt.LftAdd(-i), rgt.RgtAdd(-i));
    }


    TGridMaskedData TGridMaskedData::operator+(double c) const {
        return Data->Add(Mask, c, 0);
    }
    TGridMaskedData TGridMaskedData::operator+(const TGridMaskedData &src) const {
        return Data->Add(Mask, src, 0);
    }
    void TGridMaskedData::operator+=(double c) {
        Data->Add(Mask, c, 1);
    }
    void TGridMaskedData::operator+=(const TGridMaskedData &src) {
        Data->Add(Mask, src, 1);
    }


    TGridMaskedData TGridMaskedData::operator-(double c) const {
        return Data->Add(Mask, -c, 0);
    }
    TGridMaskedData TGridMaskedData::operator-(const TGridMaskedData &src) const {
        return Data->Sub(Mask, src, 0);
    }
    void TGridMaskedData::operator-=(double c) {
        Data->Add(Mask, -c, 1);
    }
    void TGridMaskedData::operator-=(const TGridMaskedData &src) {
        Data->Sub(Mask, src, 1);
    }


    TGridMaskedData TGridMaskedData::operator*(double c) const {
        return Data->Mul(Mask, c, 0);
    }
    TGridMaskedData TGridMaskedData::operator*(const TGridMaskedData &src) const {
        return Data->Mul(Mask, src, 0);
    }
    void TGridMaskedData::operator*=(double c) {
        Data->Mul(Mask, c, 1);
    }
    void TGridMaskedData::operator*=(const TGridMaskedData &src) {
        Data->Mul(Mask, src, 1);
    }

    TGridMaskedData TGridMaskedData::operator/(double c) const {
        return Data->Mul(Mask, 1.0 / c, 0);
    }
    TGridMaskedData TGridMaskedData::operator/(const TGridMaskedData &src) const {
        return Data->Div(Mask, src, 0);
    }

    void TGridMaskedData::operator/=(double c) {
        Data->Mul(Mask, 1 / c, 1);
    }
    void TGridMaskedData::operator/=(const TGridMaskedData &src) {
        Data->Div(Mask, src, 1);
    }
    TGridMaskedData operator/(double c, const TGridMaskedData &dat) {
        return dat.Data->Div(c, dat.Mask, 0);
    }

    void TGridMaskedData::Abs(const TGridMaskedData &src) {
        Data->Abs(Mask, src);
    }


};   //namespace NPolygon {