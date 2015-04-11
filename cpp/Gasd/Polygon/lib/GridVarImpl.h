#pragma once

#include "GridMask.h"

namespace NPolygon {

    void TGridVarNothingToDo();

    template<class T>
    struct TGridVar : TGridVariablesBase {
    private:
        virtual void IterateFunction(const TGridMaskedData &dst, const TGridMaskedData &src, void (*func)(T&dst, T&src)) {
            if (dst.Mask->NumPoints() != src.Mask->NumPoints())
                throw info_except("Masks sizes are different. Dst %i Src %i\n", dst.Mask->NumPoints(), src.Mask->NumPoints());
            int curL = -1, curU = -1, wasL = -1, wasU = -1;
            int wasX = -1;
            TGridVar *ptrsrc = SavableClass::TestType<TGridVar>(src.Data);
            TGridVar *ptrdst = SavableClass::TestType<TGridVar>(dst.Data);
            vector<T> &srcV = ptrsrc->VectorT, &dstV = ptrdst->VectorT;
            TGridMaskBase *wasMask = src.Mask, *curMask = dst.Mask;
            TGridVar *Old = CurrentPtr;
            CurrentPtr = this;
            while(curMask->GetNextInterval(curL, curU, 0)) {
                for(int curX = curL; curX < curU; curX++) {
                    if (wasX >= wasU - 1) {
                        if (!(wasMask->GetNextInterval(wasL, wasU, 0)))
                            throw info_except("Dims are different!!! maskWas:\n%s\nMaskNow:\n%s\n", ~SavableClass::object2string(wasMask), ~SavableClass::object2string(wasMask));
                        wasX = wasL;
                    } else
                        wasX++;
                    func(dstV[curX], srcV[wasX]);
                }
            }
            CurrentPtr = Old;

        }
        virtual void IterateFunction(const TGridMaskedData &mdata, void (*func)(T&dst)) {
            int curL = -1, curU = -1;//, wasL = -1, wasU = -1;
            int wasX = -1;
            TGridVar *ptr = SavableClass::TestType<TGridVar>(mdata.Data);
            vector<T> &tmpStor = ptr->VectorT;
            TGridMaskBase *curMask = mdata.Mask;
            TGridVar *Old = CurrentPtr;
            CurrentPtr = this;
            while(mdata.Mask->GetNextInterval(curL, curU, 0)) {
                for(int curX = curL; curX < curU; curX++) 
                    func(tmpStor[curX]);
            }
            CurrentPtr = Old;
        }

    protected:
    public:
        virtual void Set(TGridMaskBase *curMask, const TGridMaskedData &mdata) {
            if (curMask->NumPoints() != mdata.Mask->NumPoints())
                throw info_except("Masks sizes are different. Dst %i Src %i\n", curMask->NumPoints(), mdata.Mask->NumPoints());
            int curL = -1, curU = -1, wasL = -1, wasU = -1;
            int wasX = -1;
            TGridVar *ptr = SavableClass::TestType<TGridVar>(mdata.Data);
            vector<T> &tmpStor = ptr->VectorT;
            TGridMaskBase *wasMask = mdata.Mask;
            while(curMask->GetNextInterval(curL, curU, 0)) {
                for(int curX = curL; curX < curU; curX++) {
                    if (wasX >= wasU - 1) {
                        if (!(wasMask->GetNextInterval(wasL, wasU, 0)))
                            throw info_except("Dims are different!!! maskWas:\n%s\nMaskNow:\n%s\n", ~SavableClass::object2string(wasMask), ~SavableClass::object2string(wasMask));
                        wasX = wasL;
                    } else
                        wasX++;
                    VectorT[curX] = tmpStor[wasX];
                }
            }
        }

        virtual void Set(TGridMaskBase *mask, double val) {
            StaticDouble = val;
            IterateFunction(TGridMaskedData(this, mask), StaticSetDouble);
        }

        virtual TGridMaskedData Add(TGridMaskBase *mask, const TGridMaskedData &mdata, int inPlace) {
            TGridMaskedData ret(this, mask);
            if (!inPlace)
                ret.Data = new TGridVar(*this);
            IterateFunction(ret, mdata, StaticAddVector);
            return ret;
        }
        virtual TGridMaskedData Add(TGridMaskBase *mask, double val, int inPlace) {
            StaticDouble = val;
            TGridMaskedData ret(this, mask);
            if (!inPlace)
                ret.Data = new TGridVar(*this);
            IterateFunction(ret, StaticAddDouble);
            return ret;
        }
        virtual TGridMaskedData Sub(TGridMaskBase *mask, const TGridMaskedData &mdata, int inPlace) {
            TGridMaskedData ret(this, mask);
            if (!inPlace)
                ret.Data = new TGridVar(*this);
            IterateFunction(ret, mdata, StaticSubVector);
            return ret;
        }
        virtual TGridMaskedData Mul(TGridMaskBase *mask, const TGridMaskedData &mdata, int inPlace) {
            TGridMaskedData ret(this, mask);
            if (!inPlace)
                ret.Data = new TGridVar(*this);
            IterateFunction(ret, mdata, StaticMulVector);
            return ret;
        }
        virtual TGridMaskedData Mul(TGridMaskBase *mask, double val, int inPlace) {
            StaticDouble = val;
            TGridMaskedData ret(this, mask);
            if (!inPlace)
                ret.Data = new TGridVar(*this);
            IterateFunction(ret, StaticMulDouble);
            return ret;
        }
        virtual TGridMaskedData Div(TGridMaskBase *mask, const TGridMaskedData &mdata, int inPlace) {
            TGridMaskedData ret(this, mask);
            if (!inPlace)
                ret.Data = new TGridVar(*this);
            IterateFunction(ret, mdata, StaticDivVector);
            return ret;
        }
        virtual TGridMaskedData Div(double val, TGridMaskBase *mask, int inPlace) {
            StaticDouble = val;
            TGridMaskedData ret(this, mask);
            if (!inPlace)
                ret.Data = new TGridVar(*this);
            IterateFunction(ret, StaticDivDouble);
            return ret;
        }
        virtual TGridMaskedData Abs(TGridMaskBase *mask, const TGridMaskedData &mdata) {
            TGridMaskedData ret(this, mask);
            IterateFunction(ret, mdata, StaticAbsVector);
            return ret;
        }
    protected:
        vector<T> VectorT;
    public:
        TGridVar(int dim = 0) : VectorT(dim) {};
        TGridVar(const TGridVar &var) : VectorT(var.VectorT) {};
        TGridVar(const vector<T> &vec) : VectorT(vec) {};
        virtual void Set(TGridVariablesBase *vars) {
            TGridVar *ptr = SavableClass::TestType<TGridVar>(vars);
            memmove(&VectorT[0], &ptr->VectorT[0], VectorT.size() * sizeof(T));
        }

        static TGridVar *CurrentPtr;
        static double StaticDouble;
        TGridVar *SrcPtr;
        static void StaticSetVector(T &dst, T &src) {
            dst = src;
        }
        static void StaticSetDouble(T &dst) {
            dst = CurrentPtr->StaticDouble;
        }
        static void StaticAddVector(T &dst, T &src) {
            dst += src;
        }
        static void StaticAddDouble(T &dst) {
            dst += CurrentPtr->StaticDouble;
        }
        static void StaticSubVector(T &dst, T &src) {
            dst -= src;
        }
        static void StaticMulVector(T &dst, T &src) {
            dst *= src;
        }
        static void StaticMulDouble(T &dst) {
            dst *= CurrentPtr->StaticDouble;
        }
        static void StaticDivVector(T &dst, T &src) {
            dst /= src;
        }
        static void StaticDivDouble(T &dst) {
            dst = CurrentPtr->StaticDouble / dst;
        }
        static int fabs(int x) {
            return abs(x);
        }
        static void StaticAbsVector(T &dst, T &src) {
            dst = fabs(src);
        }
        
        //static void StaticSetVector(int iSrc, int iDst) {
        //    CurrentPtr->VectorT[iDst] = CurrentPtr->SrcPtr->VectorT[iSrc];
        //}
        //void SetByMask(TGridMaskBase *curMask, const TGridMaskedData &mdata) {
        //    TGridVar *Old = CurrentPtr;
        //    CurrentPtr = this;
        //    SrcPtr = SavableClass::TestType<TGridVar>(mdata.Data);
        //    curMask->IterateRegion(TGridMaskedData(this, curMask), mdata, StaticSetVector);
        //    CurrentPtr = Old;
        //}

        void SetByInternalIteration(TGridMaskBase *curMask, const TGridMaskedData &mdata) {
            IterateFunction(TGridMaskedData(this, curMask), mdata, StaticSetVector);
        }
        void SetByIterator(TGridMaskBase *curMask, const TGridMaskedData &mdata) {
            if (curMask->NumPoints() != mdata.Mask->NumPoints())
                throw info_except("Sizes of masks are different!!! Src<%i> Dst<%i>", curMask->NumPoints(), mdata.Mask->NumPoints());
            TGridMaskedData cur(this, curMask);
            TGridMaskedData::TIterator curIter(cur), rgtIter(mdata);
            TGridVar *ptr = SavableClass::TestType<TGridVar>(mdata.Data);
            vector<T> &tmpStor = ptr->VectorT;
            for( ; curIter.IsOk() && rgtIter.IsOk(); curIter.Next(), rgtIter.Next()) 
                VectorT[curIter.GetElementInd()] = tmpStor[rgtIter.GetElementInd()];
        }



        T &operator[](size_t k){ return Data[k];};
        virtual void Resize(int dim, TGridMaskBase *curMask, TGridMaskBase *wasMask) {
            if (curMask==NULL || wasMask==NULL) {
                if (VectorT.size() != dim)
                    VectorT.resize(dim);
                return;
            }
                
            Ref<TGridVar<T> > tmpStor = new TGridVar<T>(*this);
            VectorT.resize(dim);
            Set(curMask, TGridMaskedData(tmpStor, wasMask));
        }
        virtual int Size() {
            return VectorT.size();
        }
        virtual void* GetElementPtr(size_t k) {
            return &(VectorT[k]);
        }
        virtual void SetElement(size_t k, TGridVariablesBase *data, size_t maskK) {
            TGridVar *ptr = SavableClass::TestType<TGridVar>(data);
//cout<<"Setting "<<k<<" by "<<maskK<<"\n";
            VectorT[k] = ptr->VectorT[maskK];

        }

        int save_data_state( FilterOut&so){
            so<<" { ";
            for(size_t i = 0; i < VectorT.size(); i++)
                so<<VectorT[i]<<" ";
            so<<" } ";
            return 1; 
        };
        int read_data_state(FilterIn&si) {
            VectorT.clear();
            SavableClass::ExeptionCheck(si, "{");
            T data;
            while (!(!si) && !SavableClass::TestNextChar(si, '}')) {
                si>>data;
                VectorT.push_back(data);
            }
            return !(!si);
        };


    };
    template<class T>
    TGridVar<T> *TGridVar<T>::CurrentPtr = NULL;
    template<class T>
    double TGridVar<T>::StaticDouble;


}; //namespace NPolygon {
 