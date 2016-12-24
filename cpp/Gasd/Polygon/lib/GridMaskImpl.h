#pragma once

#include "GridMask.h"

namespace NPolygon {

    struct TGridMaskSimple : TGridMaskBase {
    private:
        int Beg, End;

    public:
        TGridMaskSimple(int length = 0);   // : TGridMaskBase(), Beg(0), End(length) {};
        TGridMaskSimple(int beg, int end) : TGridMaskBase(), Beg(beg), End(end){};
        TGridMaskSimple &operator=(const TGridMaskSimple &right) {
            Set(right.Beg, right.End);
        }
        void Set(int beg, int end) {
            Beg = beg;
            End = end;
        }

        virtual int GetNextInterval(int &low, int &up, int reverse) {
            if(low >= 0)
                return 0;
            low = Beg;
            up = End;
            return 1;
        }
        //virtual int GetBoundaryPnt(vector<int> boundary, int coordinate, int dir) = 0;
        virtual Ref<TGridMaskBase> AddBoundary(int level) {
            int size = End - Beg;
            Set(0, End - Beg + level * 2);
            return new TGridMaskSimple(level, End - level);
        }
        virtual Ref<TGridMaskBase> Shift(int level) {
            return new TGridMaskSimple(Beg + level, End + level);
        }
        virtual void Cut(TGridMaskBase *largerMask) {
            TGridMaskSimple *mask = SavableClass::TestType<TGridMaskSimple>(
                largerMask, "mask have to be simple");
            Beg = max<int>(mask->Beg, Beg);
            End = min<int>(mask->End, End);
        }
        virtual void Normalize() {
            End = End - Beg;
            Beg = 0;
        }
        virtual int NumPoints() {
            return End - Beg;
        }
        virtual int MaxPointInd() {
            if(Beg < 0)
                throw info_except("Start index <%i> is below zero!!!\n", Beg);
            return End;
        }
        virtual Ref<TGridMaskBase> ExpandGrid(const TRegionBounds &bnds) {
            if(bnds.IsPoint())
                throw info_except("Is point, not region\n");
            End = (End - Beg) + bnds.GetRgt() - bnds.GetLft();
            Beg = 0;
            //return new TGridMaskSimple(bnds.GetRgt(), End + bnds.GetLft());
            return new TGridMaskSimple(-bnds.GetLft(), End - bnds.GetRgt());
        }
        virtual Ref<TGridMaskBase> ShrinkGrid(const TRegionBounds &bnds) {
            if(!bnds.IsPoint())
                return new TGridMaskSimple(Beg + bnds.GetLft(), End + bnds.GetRgt());
            if(bnds.NoLft())
                return new TGridMaskSimple(End + bnds.GetRgt() - 1, End + bnds.GetRgt());
            return new TGridMaskSimple(Beg + bnds.GetLft(), Beg + bnds.GetLft() + 1);
        }

        int save_data_state(FilterOut &so) {
            so << " Beg " << Beg << " End " << End;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> Beg >> tmp >> End;
            return 1;
        };
        void IterateRegion(
            const TGridMaskedData &src,
            const TGridMaskedData &dst,
            void (*doit)(int, int)) {
            TGridMaskSimple *mSrc = SavableClass::TestType<TGridMaskSimple>(src.Mask);
            TGridMaskSimple *mDst = SavableClass::TestType<TGridMaskSimple>(dst.Mask);
            if(mSrc->NumPoints() != mDst->NumPoints())
                throw info_except(
                    "Sizes of masks are different!!! Src<%i> Dst<%i>",
                    mSrc->NumPoints(),
                    mDst->NumPoints());
            size_t iSrc = mSrc->Beg, iDst = mDst->Beg;
            for(; iSrc < (size_t)mSrc->End; iSrc++, iDst++)
                doit(iSrc, iDst);
        }
        void IterateRegion(const TGridMaskedData &src, void (*doit)(int)) {
            TGridMaskSimple *mSrc = SavableClass::TestType<TGridMaskSimple>(src.Mask);
            size_t iSrc = mSrc->Beg;
            for(; iSrc < (size_t)mSrc->End; iSrc++)
                doit(iSrc);
        }
    };

    //struct TGridMaskSimple : TGridMaskBase {
    //private:
    //    int CurInd;
    //    int Beg, End;
    //public:
    //    TGridMaskSimple(int length = 0) : TGridMaskBase(), Beg(0), End(length), CurInd(0) {};
    //    TGridMaskSimple(int beg, int end) : TGridMaskBase(), Beg(beg), End(end), CurInd(beg) {
    //    };
    //    TGridMaskSimple & operator=(const TGridMaskSimple &right) {
    //        Set(right.CurInd, right.Beg, right.End);
    //    }
    //    int Set(int curind, int beg, int end) {
    //        CurInd = curind;
    //        Beg = beg;
    //        End = end;
    //    }
    //    //TGridMaskSimple(const GridMask &right);
    //    //~TGridMaskSimple();
    //    //int operator==(const TGridMaskSimple &right) const;
    //    //int operator!=(const TGridMaskSimple &right) const;

    //    int begin ()   { CurInd = Beg;return Beg;}
    //    int current () { return CurInd;}
    //    int end ()  {return End;}
    //    int next () {return ++CurInd;}
    //    int size () {return End - Beg;}
    //    int last () {CurInd = End - 1;return CurInd; }
    //    int prev () {CurInd = CurInd != Beg?CurInd--:End; return CurInd;}
    //    void shift (int shiftInd) { Beg += shiftInd; End += shiftInd; CurInd += shiftInd; }
    //    int minind() {return Beg;}
    //    int maxind() {return End;}
    //    void addbegpnt(int n) { Beg -= n;};
    //    void addendpnt(int n) { End += n;};
    //    void SetDim(int beg, int end = -1) {
    //        if (End == -1) {
    //            Beg = 0;
    //            End = beg;
    //        } else {
    //            Beg = beg;
    //            End = end;
    //        }

    //    }
    //    int set (int ind) {
    //        if (ind < Beg || ind > End)
    //            throw info_except("Bad index %i\n", ind);
    //        CurInd = ind;
    //        return CurInd;
    //    }
    //    virtual int moveInd (int ind) {
    //        int resInd = ind + CurInd;
    //        return set(resInd);
    //    }
    //    int save_data_state( FilterOut&so) {
    //        so<<" Beg "<<Beg<<" End "<<End<<" CurInd "<<CurInd<<" ";
    //        return 1;
    //    };
    //    int read_data_state(FilterIn&si) {
    //        Stroka tmp;
    //        si>>tmp>>Beg>>tmp>>End>>tmp>>CurInd;
    //        return 1;
    //    };
    //    Stroka MakeHelp() {
    //        return "Mask...";
    //    }

    //};


};   //namespace NPolygon {
