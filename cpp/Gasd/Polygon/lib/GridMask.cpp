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


    TGridMaskedData operator+(const TGridMaskedData &dat,double c) {
        return dat.Data->Add(dat.Mask, c, 0);
    }
    TGridMaskedData operator+(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Add(dst.Mask, src, 0);
    }
    TGridMaskedData operator+=(const TGridMaskedData &dat,double c) {
        return dat.Data->Add(dat.Mask, c, 1);
    }
    TGridMaskedData operator+=(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Add(dst.Mask, src, 1);
    }



    TGridMaskedData operator-(const TGridMaskedData &dat,double c) {
        return dat.Data->Add(dat.Mask, -c, 0);
    }
    TGridMaskedData operator-(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Sub(dst.Mask, src, 0);
    }
    TGridMaskedData operator-=(const TGridMaskedData &dat,double c) {
        return dat.Data->Add(dat.Mask, -c, 1);
    }
    TGridMaskedData operator-=(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Sub(dst.Mask, src, 1);
    }


    TGridMaskedData operator*(const TGridMaskedData &dat,double c) {
        return dat.Data->Mul(dat.Mask, c, 0);
    }
    TGridMaskedData operator*(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Mul(dst.Mask, src, 0);
    }
    TGridMaskedData operator*=(const TGridMaskedData &dat,double c) {
        return dat.Data->Mul(dat.Mask, c, 1);
    }
    TGridMaskedData operator*=(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Mul(dst.Mask, src, 1);
    }


    TGridMaskedData operator/(const TGridMaskedData &dat,double c) {
        return dat.Data->Mul(dat.Mask, 1/c, 0);
    }
    TGridMaskedData operator/(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Div(dst.Mask, src, 0);
    }
    TGridMaskedData operator/=(const TGridMaskedData &dat,double c) {
        return dat.Data->Mul(dat.Mask, 1/c, 1);
    }
    TGridMaskedData operator/=(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Div(dst.Mask, src, 1);
    }
    TGridMaskedData operator/(double c, const TGridMaskedData &dat) {
        return dat.Data->Div(c ,dat.Mask, 0);
    }

    TGridMaskedData Abs(TGridMaskedData &dst, const TGridMaskedData &src) {
        return dst.Data->Abs(src.Mask, src);
    }


}; //namespace NPolygon {