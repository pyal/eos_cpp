
#ifndef GridMask_h
#define GridMask_h 1

#include "lib/ref/class_sav.h"
#include "lib/ref/refmanip.h"
#include "lib/ref/data_manip.h"
#include <list>

//using namespace RefManip;
//using namespace DataManip;

namespace NPolygon {

    struct TGridMaskedData;
    struct TRegionBounds {
        static int BNDBAD;

    private:
        int LftShift, RgtShift;

    public:
        TRegionBounds(int lftShift = 0, int rgtShift = 0) {
            Set(lftShift, rgtShift);
        }
        //LftShift(lftShift), RgtShift(rgtShift){}
        void Set(int lftShift, int rgtShift) {
            LftShift = lftShift;
            RgtShift = rgtShift;
        };
        TRegionBounds(const TRegionBounds &bnds) {
            LftShift = bnds.LftShift;
            RgtShift = bnds.RgtShift;
        };
        TRegionBounds Lft(int i) {
            return TRegionBounds(LftShift != BNDBAD ? LftShift + i : LftShift, RgtShift);
        }
        TRegionBounds Rgt(int i) {
            return TRegionBounds(LftShift, RgtShift != BNDBAD ? RgtShift + i : RgtShift);
        }
        int GetLft() const {
            return LftShift;
        }
        int GetRgt() const {
            return RgtShift;
        }
        int LftAdd(int i) const {
            return LftShift != BNDBAD ? LftShift + i : LftShift;
        }
        int RgtAdd(int i) const {
            return RgtShift != BNDBAD ? RgtShift + i : RgtShift;
        }
        int IsPoint() const {
            return (LftShift == BNDBAD || RgtShift == BNDBAD);
        }
        int NoLft() const {
            return (LftShift == BNDBAD);
        }
        int NoRgt() const {
            return (RgtShift == BNDBAD);
        }
        static inline int GetHash(const TRegionBounds &bnd) {
            return bnd.LftShift * 2048 + bnd.RgtShift;
        }
        inline int operator==(const TRegionBounds &rgt) const {
            return (LftShift == rgt.LftShift && RgtShift == rgt.RgtShift);
        }
        inline int operator<=(const TRegionBounds &rgt) const {
            return (GetHash(*this) <= GetHash(rgt));
        }
        inline int operator>=(const TRegionBounds &rgt) const {
            return (GetHash(*this) >= GetHash(rgt));
        }
        inline int operator<(const TRegionBounds &rgt) const {
            return (GetHash(*this) < GetHash(rgt));
        }
        inline int operator>(const TRegionBounds &rgt) const {
            return (GetHash(*this) > GetHash(rgt));
        }
        int save_data_state(FilterOut &so) {
            so << " LftShift " << LftShift << " RgtShift " << RgtShift << " ";
            return 1;
        }
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> LftShift >> tmp >> RgtShift;
            return 1;
        }
        //friend TRegionBounds operator+(const TRegionBounds &rgt, int i);
        //friend TRegionBounds operator-(const TRegionBounds &rgt, int i);
    };
    TRegionBounds operator+(const TRegionBounds &rgt, int i);
    TRegionBounds operator-(const TRegionBounds &rgt, int i);


    struct TGridMaskBase : SavableClass {
    public:
        TGridMaskBase(){};
        //GridMask(const GridMask &right);
        //~GridMask();
        //GridMask & operator=(const GridMask &right);
        //int operator==(const GridMask &right) const;
        //int operator!=(const GridMask &right) const;

        //virtual int GetBoundaryPnt(vector<int> boundary, int coordinate, int dir) = 0;
        // returns Mask for the Base(without new boundary) on the new Grid
        //virtual TGridMaskBase* AddBoundary(int level) = 0;
        virtual int GetNextInterval(int &low, int &up, int reverse) = 0;
        virtual Ref<TGridMaskBase> Shift(int level) = 0;
        virtual void Cut(TGridMaskBase *largerMask) = 0;
        virtual int NumPoints() = 0;
        virtual int MaxPointInd() = 0;
        virtual void Normalize() = 0;
        virtual Ref<TGridMaskBase> ExpandGrid(const TRegionBounds &bnds) = 0;
        virtual void IterateRegion(
            const TGridMaskedData &src,
            const TGridMaskedData &dst,
            void (*doit)(int, int)) = 0;
        virtual Ref<TGridMaskBase> ShrinkGrid(const TRegionBounds &bnds) = 0;


        // returns Mask for the Base on the old Grid
        //virtual TGridMaskBase* RemoveBoundary(int level) = 0;

        ////virtual int begin () = 0;
        ////virtual int current () = 0;
        ////virtual int end () = 0;
        ////virtual int next () = 0;
        ////virtual int size () = 0;
        ////virtual int last () = 0;
        ////virtual int prev () = 0;
        ////virtual int moveInd (int ind) {
        ////    if (ind > 0) {
        ////        for(int i = 0; i < ind ; i++)
        ////            next();
        ////    } else {
        ////        for(int i = 0; i < -ind ; i++)
        ////            prev();
        ////    }
        ////    return current();
        ////}
        ////virtual int set (int ind) = 0;
        //////void shift (int shiftInd) = 0;
        //////int minind() = 0;
        //////int maxind() = 0;
        //////void addbegpnt(int n) = 0;
        //////void addendpnt(int n) = 0;
    };

    struct TGridMaskedData;
    struct TGridVariablesBase : SavableClass {
        friend struct TGridMaskedData;
//        friend TGridMaskedData operator+(const TGridMaskedData &dat, double c);
//        friend TGridMaskedData operator+(
//            TGridMaskedData &dst,
//            const TGridMaskedData &src);
//        friend void operator+=(const TGridMaskedData &dat, double c);
//        friend void operator+=(
//            TGridMaskedData &dst,
//            const TGridMaskedData &src);

//        friend TGridMaskedData operator-(const TGridMaskedData &dat, double c);
//        friend TGridMaskedData operator-(
//            TGridMaskedData &dst,
//            const TGridMaskedData &src);
//
//        friend TGridMaskedData operator*(const TGridMaskedData &dat, double c);
//        friend TGridMaskedData operator*(
//            TGridMaskedData &dst,
//            const TGridMaskedData &src);
//        friend void operator*=(const TGridMaskedData &dat, double c);
//        friend void operator*=(
//            TGridMaskedData &dst,
//            const TGridMaskedData &src);

//        friend TGridMaskedData operator/(const TGridMaskedData &dat, double c);
//        friend TGridMaskedData operator/(
//            TGridMaskedData &dst,
//            const TGridMaskedData &src);

        friend TGridMaskedData operator/(double c, const TGridMaskedData &dat);
//        friend TGridMaskedData Abs(TGridMaskedData &dst, const TGridMaskedData &src);

    protected:
        virtual TGridMaskedData Add(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata,
            int inPlace) = 0;
        virtual TGridMaskedData Add(TGridMaskBase *mask, double c, int inPlace) = 0;
        virtual TGridMaskedData Sub(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata,
            int inPlace) = 0;
        virtual TGridMaskedData Mul(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata,
            int inPlace) = 0;
        virtual TGridMaskedData Mul(TGridMaskBase *mask, double c, int inPlace) = 0;
        virtual TGridMaskedData Div(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata,
            int inPlace) = 0;
        virtual TGridMaskedData Div(double c, TGridMaskBase *mask, int inPlace) = 0;

        virtual TGridMaskedData Abs(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata) = 0;

        virtual void Set(TGridMaskBase *mask, double val) = 0;
        virtual void Set(TGridMaskBase *mask, const TGridMaskedData &mdata) = 0;

    public:
        //virtual TGridMaskedData Div(TGridMaskBase *mask, double c, int inPlace) = 0;

        virtual void Set(TGridVariablesBase *vars) = 0;
        virtual void Resize(int dim, TGridMaskBase *curMask, TGridMaskBase *wasMask) = 0;
        virtual int Size() = 0;
        virtual void *GetElementPtr(size_t k) = 0;
        virtual void SetElement(size_t k, TGridVariablesBase *data, size_t maskK) = 0;

        //double &operator[](size_t k){ return Ptr[k];};
        //double operator[](size_t k)const{ return Ptr[k];};
    };


    struct TGridMaskedData {
        //private:
        Ref<TGridVariablesBase> Data;
        Ref<TGridMaskBase> Mask;

    public:
        TGridMaskedData(){};
        TGridMaskedData(const TGridMaskedData &right)
            : Data(right.Data), Mask(right.Mask){};
        TGridMaskedData(TGridVariablesBase *data, TGridMaskBase *mask)
            : Data(data), Mask(mask) {}
        TGridMaskedData &operator=(const TGridMaskedData &right) {
            //throw info_except("Here were are - copying mask or data - that is the question!!!\n");
            //Data = right.Data;
            //Mask = right.Mask;
            Data->Set(Mask, right);
            return *this;
        }
        TGridMaskedData &operator=(double right) {
            Data->Set(Mask, right);
            return *this;
        }
        inline TGridMaskedData &CopyData(const TGridMaskedData &right) {
            Data->Set(Mask, right);
            return *this;
        }
        inline int operator!() const {
            return (Data == NULL || Mask == NULL);
        }
        inline void Clear() {
            Data = NULL;
            Mask = NULL;
            ;
        }
        inline void SetMask(const TGridMaskedData &right) {
            Data = right.Data;
            Mask = right.Mask;
        }
//        friend TGridMaskedData operator+(const TGridMaskedData &dat, double c);
//        friend TGridMaskedData operator+(TGridMaskedData &dst, const TGridMaskedData &src);
        TGridMaskedData operator+(double c) const;
        TGridMaskedData operator+(const TGridMaskedData &src) const;
        TGridMaskedData operator-(double c) const;
        TGridMaskedData operator-(const TGridMaskedData &src) const;
        TGridMaskedData operator*(double c) const ;
        TGridMaskedData operator*(const TGridMaskedData &src) const;

        void operator+=(double c);
        void operator+=(const TGridMaskedData &src);
        void operator-=(double c);
        void operator-=(const TGridMaskedData &src);
        void operator*=(double c);
        void operator*=(const TGridMaskedData &src);
        void operator/=(double c);
        void operator/=(const TGridMaskedData &src);
        TGridMaskedData operator/(double c) const;
        TGridMaskedData operator/(const TGridMaskedData &src) const;

        void Abs(const TGridMaskedData &src);
        //inline void* GetElementPtr(size_t k) {
        //    return Data->GetElementPtr(k);
        //}
        //inline void SetElement(size_t k, TGridMaskedData maskData, size_t maskK) {
        //    return Data->SetElement(k, maskData.Data, maskK);
        //}

        //////~TGridMaskedData();
        //////int operator==(const TGridMaskedData &right) const;
        //////int operator!=(const TGridMaskedData &right) const;

        //friend TGridMaskedData operator*(const TGridMaskedData &dat,double c);
        //friend TGridMaskedData operator/(const TGridMaskedData &dat,double c);
        //friend TGridMaskedData operator-(const TGridMaskedData &dat,double c);

        //friend TGridMaskedData operator*(TGridMaskedData &dat, const TGridMaskedData &dat);
        //friend TGridMaskedData operator/(TGridMaskedData &dat, const TGridMaskedData &dat);


        //friend TGridMaskedData operator-(TGridMaskedData &dat, const TGridMaskedData &dat);

        //friend ostream& operator<<(ostream &output,const VecCl &vec);
        //friend istream& operator>>(istream &input,VecCl &vec);


        //inline TGridMaskedDataIterator Start();
        //inline TGridMaskedDataIterator Last();
        // to be done - moving by arrays....
        //struct TArrayIterator {
        //};
        struct TIterator {
            friend struct TGridMaskedData;

        private:
            int Low, Up;
            int CurInd;
            TGridMaskedData *Mask;

        protected:
            inline void End() {
                Mask->Clear();
            }

        public:
            TIterator() : Mask(new TGridMaskedData){};
            TIterator(const TGridMaskedData &mask) : Mask(new TGridMaskedData) {
                Start(mask);
            };
            TIterator(const TIterator &iter)
                : Low(iter.Low),
                  Up(iter.Up),
                  CurInd(iter.CurInd),
                  Mask(new TGridMaskedData) {
                Mask->SetMask(*iter.Mask);
            };
            TIterator &operator=(const TIterator &it) {
                Low = it.Low;
                Up = it.Up;
                CurInd = it.CurInd;
                Mask->SetMask(*it.Mask);
                return *this;
            }
            ~TIterator() {
                delete Mask;
            }
            inline int operator==(const TIterator &lft) {
                //return (Low == lft.Low && Up == lft.Up && CurInd == lft.CurInd);
                return (CurInd == lft.CurInd);
            }
            inline int operator<(const TIterator &lft) {
                return (CurInd < lft.CurInd);
            }
            inline int operator>(const TIterator &lft) {
                return (CurInd > lft.CurInd);
            }
            inline int operator<=(const TIterator &lft) {
                return (CurInd <= lft.CurInd);
            }
            inline int operator>=(const TIterator &lft) {
                return (CurInd >= lft.CurInd);
            }
            inline TIterator &operator++() {
                Next();
                return *this;
            }
            inline TIterator &operator--() {
                Prev();
                return *this;
            }
            inline int operator!=(const TIterator &lft) {
                //return (Low == lft.Low && Up == lft.Up && CurInd == lft.CurInd);
                return (CurInd != lft.CurInd);
            }
            inline TIterator &Start(const TGridMaskedData &mask) {
                Low = Up = -1;
                Mask->SetMask(mask);
                if(!IsOk() || Mask->Mask->NumPoints() == 0 ||
                   !Mask->Mask->GetNextInterval(Low, Up, 0))
                    Mask->Clear();
                else
                    CurInd = Low;
                return *this;
            }
            inline TIterator &Last(const TGridMaskedData &mask) {
                Low = Up = -1;
                Mask->SetMask(mask);
                if(!IsOk() || Mask->Mask->NumPoints() == 0 ||
                   !Mask->Mask->GetNextInterval(Low, Up, 1)) {
                    Mask->Clear();
                    return *this;
                }
                CurInd = Up - 1;
                if(CurInd < Low)
                    throw info_except("Bad ind\n");
                return *this;
            }
            inline int IsOk() {
                return !(!*Mask);
            }
            inline void Next() {
                CurInd++;
                if(CurInd >= Up) {
                    if (!Mask->Mask->GetNextInterval(Low, Up, 0))
                        Mask->Clear();
                    else
                        CurInd = Low;
                }
            }
            inline int NextSlow() {
                if(!IsOk())
                    return 0;
                CurInd++;
                if(CurInd >= Up) {
                    if (!Mask->Mask->GetNextInterval(Low, Up, 0))
                        Mask->Clear();
                    else
                        CurInd = Low;
                }
                return IsOk();
            }
            TIterator operator+(int level) {
                TIterator ret(*this);
                ret.Move(level);
                return ret;
            }
            TIterator operator-(int level) {
                TIterator ret(*this);
                ret.Move(-level);
                return ret;
            }
            inline void Move(int level) {
                if(level > 0)
                    for(int i = 0; i < abs(level) && IsOk(); i++)
                        Next();
                else
                    for(int i = 0; i < abs(level) && IsOk(); i++)
                        Prev();
                //return IsOk();
            }

            inline void Prev() {
                //if (!IsOk())
                //    return ;
                CurInd--;
                while(1) {
                    if(CurInd < Low) {
                        if(!Mask->Mask->GetNextInterval(Low, Up, 1)) {
                            Mask->Clear();
                            return;
                        } else
                            CurInd = Up - 1;
                    } else
                        return;
                }
                //if (CurInd < Low)
                //    Mask->Clear();
                //throw info_except("Bad ind\n");
                //return IsOk();
            }
            inline int Size() {
                if(!IsOk())
                    return 0;
                return Mask->Mask->NumPoints();
            }
            inline int GetElementInd() {
                //if (!IsOk())
                //    throw info_except("Bad mask!!!\n");
                return CurInd;
            }
            inline void *GetElementPtr() {
                //if (!IsOk())
                //    throw info_except("Bad mask!!!\n");
                return Mask->Data->GetElementPtr(CurInd);
            }
            inline void *GetElementPtr(TGridVariablesBase *var) {
                return var->GetElementPtr(CurInd);
            }
            inline void SetElement(const TIterator &it) {
                return Mask->Data->SetElement(CurInd, it.Mask->Data, it.CurInd);
            }
            inline void SetElement(TGridVariablesBase *var) {
                return Mask->Data->SetElement(CurInd, var, CurInd);
            }
            //inline void SetElement(const TGridMaskedData &mask) {
            //    return Mask->Data->SetElement(CurInd, mask.Data, CurInd);
            //}
        };


        inline TIterator Start() {
            return TIterator(*this);
        }
        inline TIterator Last() {
            TIterator it;
            it.Last(*this);
            return it;
        }
    };


    //struct TGridMaskedDataIterator {
    //    friend struct TGridMaskedData;
    //private:
    //    int Low, Up;
    //    int CurInd;
    //    TGridMaskedData Mask;
    //    //Iterator(int low, int up, int curind, int curpos, TGridMaskBase* mask)
    //    //    : Low(low)
    //    //    , Up(up)
    //    //    , CurInd(curind)
    //    //    , CurPos(curpos)
    //    //    , Mask(mask) {
    //    //};
    //    //Iterator() : Low(-1), Up(-1), CurInd(-1), CurPos(-1) {
    //    //};
    //protected:
    //    inline void Last(const TGridMaskedData &mask) {
    //        Low = Up = -1;
    //        Mask.SetMask(mask);
    //        if (!Mask || Mask.Mask->NumPoints() == 0 || !Mask.Mask->GetNextInterval(Low, Up, 1)) {
    //            Mask.Clear();
    //            return ;
    //        }
    //        CurInd = Up - 1;
    //        if (CurInd < Low)
    //            throw info_except("Bad ind\n");
    //    }
    //    inline void End() {
    //        Mask.Clear();
    //    }
    //public:
    //    TGridMaskedDataIterator() {};
    //    TGridMaskedDataIterator(const TGridMaskedData &mask) {
    //        Start(mask);
    //    };
    //    TGridMaskedDataIterator(const TGridMaskedDataIterator &iter)
    //        : Low(iter.Low)
    //        , Up(iter.Up)
    //        , CurInd(iter.CurInd)
    //        , Mask(iter.Mask) {
    //    };
    //    TGridMaskedDataIterator& operator=(const TGridMaskedDataIterator &it) {
    //        Low = it.Low;
    //        Up = it.Up;
    //        CurInd = it.CurInd;
    //        Mask.SetMask(it.Mask);
    //    }
    //    inline void Start(const TGridMaskedData &mask) {
    //        Mask.SetMask(mask);
    //        if (!Mask || Mask.Mask->NumPoints() == 0 || !Mask.Mask->GetNextInterval(Low, Up, 0)) {
    //            Mask.Clear();
    //            return ;
    //        }
    //        CurInd = Low;
    //    }
    //    inline int IsOk() {
    //        return !(!Mask);
    //    }
    //    inline int Next() {
    //        if (!Mask)
    //            return 0;
    //        CurInd++;
    //        if (CurInd >= Up)
    //            if (!Mask.Mask->GetNextInterval(Low, Up, 0))
    //                Mask.Clear();
    //            else
    //                CurInd = Low;
    //        return IsOk();
    //    }

    //    inline int Prev() {
    //        if (!Mask)
    //            return 0;
    //        CurInd--;
    //        if (CurInd < Low)
    //            if (!Mask.Mask->GetNextInterval(Low, Up, 1))
    //                Mask.Clear();
    //            else
    //                CurInd = Up - 1;
    //        if (CurInd < Low)
    //            throw info_except("Bad ind\n");
    //        return IsOk();
    //    }
    //    inline int Size() {
    //        if (!Mask)
    //            return 0;
    //        return Mask.Mask->NumPoints();
    //    }
    //    inline void* GetElementPtr() {
    //        return Mask.Data->GetElementPtr(CurInd);
    //    }
    //    inline void SetElement(const TGridMaskedDataIterator &it) {
    //        return Mask.Data->SetElement(CurInd, it.Mask.Data, it.CurInd);
    //    }
    //};


};   //namespace NPolygon {

#endif
