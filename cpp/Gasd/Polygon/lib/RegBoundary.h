
#ifndef RegBoundary_h
#define RegBoundary_h 1

#include "GridMaskImpl.h"
#include "lib/ref/str_manip.h"

namespace NPolygon {

    struct TPolyRegion;
    struct TRegBoundaryBase : SavableClass {

    public:
        TRegBoundaryBase() : SavableClass(){};
        //TRegBoundaryBase(const TRegBoundaryBase &right);
        //~TRegBoundaryBase();
        //TRegBoundaryBase & operator=(const TRegBoundaryBase &right);
        //int operator==(const TRegBoundaryBase &right) const;
        //int operator!=(const TRegBoundaryBase &right) const;

        virtual void FillBoundPlus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus) = 0;
        virtual void FillBoundMinus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus) = 0;
        //virtual void FillBounds(const Stroka &varName, TPolyRegion *RegMinus, TPolyRegion *RegPlus, TPolyRegion *RegCur) = 0;
    };

    struct TRegBoundaryCircle : TRegBoundaryBase {
    protected:   //## implementation
        TRegionBounds BaseBounds;

    public:
        TRegBoundaryCircle() : TRegBoundaryBase() {}
        TRegBoundaryCircle(const TRegionBounds &baseBounds)
            : TRegBoundaryBase(), BaseBounds(baseBounds) {}
        void SetBase(const TRegionBounds &baseBounds) {
            BaseBounds = baseBounds;
        }


        virtual void FillBoundPlus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
        virtual void FillBoundMinus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
        //void FillBounds(const Stroka &varName, TPolyRegion *RegMinus, TPolyRegion *RegPlus, TPolyRegion *RegCur);
    };
    struct TRegBoundaryCircleX : TRegBoundaryCircle {
    private:   //## implementation
    public:
        TRegBoundaryCircleX() : TRegBoundaryCircle() {}
        TRegBoundaryCircleX(const TRegionBounds &baseBounds)
            : TRegBoundaryCircle(baseBounds) {}

        virtual void FillBoundPlus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
        virtual void FillBoundMinus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
        //void FillBounds(const Stroka &varName, TPolyRegion *RegMinus, TPolyRegion *RegPlus, TPolyRegion *RegCur);
    };

    struct TRegBoundarySame : TRegBoundaryCircle {
    public:
        TRegBoundarySame() : TRegBoundaryCircle() {}
        TRegBoundarySame(const TRegionBounds &baseBounds)
            : TRegBoundaryCircle(baseBounds) {}

        virtual void FillBoundPlus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
        virtual void FillBoundMinus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
    };
    struct TRegBoundaryCircle_ZeroConst : TRegBoundaryCircle {
        vector<double> ZeroBndValCoefs;

    public:
        TRegBoundaryCircle_ZeroConst(const char *zeroBndValCoefs) : TRegBoundaryCircle() {
            SetBase(zeroBndValCoefs);
            //vector<Stroka> str = Str::SplitLine(zeroBndValCoefs, 0);
            //for(size_t i = 0; i < str.size(); i++)
            //    ZeroBndValCoefs.push_back(atof(~str[i]));
        }
        TRegBoundaryCircle_ZeroConst(
            const TRegionBounds &baseBounds,
            const char *zeroBndValCoefs)
            : TRegBoundaryCircle(baseBounds) {
            SetBase(zeroBndValCoefs);
            //vector<Stroka> str = Str::SplitLine(zeroBndValCoefs, 0);
            //for(size_t i = 0; i < str.size(); i++)
            //    ZeroBndValCoefs.push_back(atof(~str[i]));
        }
        void SetBase(const char *zeroBndValCoefs) {
            vector<Stroka> str = Str::SplitLine(zeroBndValCoefs, 0);
            ZeroBndValCoefs.clear();
            for(size_t i = 0; i < str.size(); i++)
                ZeroBndValCoefs.push_back(atof(~str[i]));
        }
        virtual void FillBoundPlus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
        virtual void FillBoundMinus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
    };

    struct TRegBoundaryGasd : TRegBoundaryCircle {
    public:
        TRegBoundaryGasd() : TRegBoundaryCircle(){};
        TRegBoundaryGasd(const TRegionBounds &bnds) : TRegBoundaryCircle(bnds){};

        virtual void FillBoundPlus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
        virtual void FillBoundMinus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
    };
    struct TRegBoundaryWilkins : TRegBoundaryBase {
    protected:   //## implementation
        TRegionBounds BaseBounds;
        vector<double> ZeroBndValCoefs;

    public:
        TRegBoundaryWilkins() : TRegBoundaryBase(){};
        TRegBoundaryWilkins(const TRegionBounds &bnds)
            : TRegBoundaryBase(), BaseBounds(bnds){};
        void SetCoefs(const char *zeroBndValCoefs) {
            vector<Stroka> str = Str::SplitLine(zeroBndValCoefs, 0);
            ZeroBndValCoefs.resize(str.size());
            for(size_t i = 0; i < str.size(); i++)
                ZeroBndValCoefs[atof(~str[i])];
        }
        double ClcBndVal(double x) {
            double ret = 0;
            for(size_t i = 0; i < ZeroBndValCoefs.size(); i++)
                ret += pow(x, (int)i) * ZeroBndValCoefs[i];
            return ret;
        }
        virtual void FillBoundPlus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
        virtual void FillBoundMinus(
            const Stroka &varName,
            TPolyRegion *RegMinus,
            TPolyRegion *RegPlus);
    };

};   //namespace NPolygon {

#endif
