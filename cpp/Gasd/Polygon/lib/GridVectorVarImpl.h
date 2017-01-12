#pragma once

#include "GridVarImpl.h"

namespace NPolygon {


    template <class T>
    struct TGridVectorVar : TGridVariablesBase {
    private:
        int IsVector(
            TGridVar<T> *&var,
            TGridVectorVar<T> *&vec,
            TGridVariablesBase *dat) {
            var = dynamic_cast<TGridVar<T> *>(dat);
            vec = dynamic_cast<TGridVectorVar<T> *>(dat);
            if(!vec && !var)
                throw info_except("Bad var %s\n", typeid(*dat).name());
            if(vec && VectorVar.size() != vec->VectorVar.size())
                throw info_except(
                    "Sizes are different <%i> <%i>\n",
                    VectorVar.size(),
                    vec->VectorVar.size());
            //VectorVar.resize(vec->VectorVar.size());
            return vec != NULL;
        }

    protected:
        virtual void Set(TGridMaskBase *curMask, const TGridMaskedData &mdata) {
            TGridVar<T> *var;
            TGridVectorVar<T> *vec;
            if(!IsVector(var, vec, mdata.Data)) {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    VectorVar[i]->Set(curMask, TGridMaskedData(var, mdata.Mask));
            } else {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    VectorVar[i]->Set(
                        curMask, TGridMaskedData(vec->VectorVar[i], mdata.Mask));
            }
        }
        virtual void Set(TGridMaskBase *mask, double val) {
            for(size_t i = 0; i < VectorVar.size(); i++)
                VectorVar[i]->Set(mask, val);
        }

        virtual TGridMaskedData Add(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata,
            int inPlace) {
            TGridVar<T> *var;
            TGridVectorVar<T> *vec;
            TGridMaskedData ret(this, mask);
            if(!inPlace)
                ret.Data = new TGridVectorVar(*this);
            TGridVectorVar<T> *retData =
                dynamic_cast<TGridVectorVar<T> *>(ret.Data.pointer());
            if(!IsVector(var, vec, mdata.Data)) {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    retData->VectorVar[i]->Add(mask, TGridMaskedData(var, mdata.Mask), 1);
            } else {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    retData->VectorVar[i]->Add(
                        mask, TGridMaskedData(vec->VectorVar[i], mdata.Mask), 1);
            }
            return ret;
        }
        virtual TGridMaskedData Add(TGridMaskBase *mask, double val, int inPlace) {
            TGridMaskedData ret(this, mask);
            if(!inPlace)
                ret.Data = new TGridVectorVar(*this);
            TGridVectorVar<T> *retData =
                dynamic_cast<TGridVectorVar<T> *>(ret.Data.pointer());
            for(size_t i = 0; i < VectorVar.size(); i++)
                retData->VectorVar[i]->Add(mask, val, 1);
            return ret;
        }
        virtual TGridMaskedData Sub(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata,
            int inPlace) {
            TGridVar<T> *var;
            TGridVectorVar<T> *vec;
            TGridMaskedData ret(this, mask);
            if(!inPlace)
                ret.Data = new TGridVectorVar(*this);
            TGridVectorVar<T> *retData =
                dynamic_cast<TGridVectorVar<T> *>(ret.Data.pointer());
            if(!IsVector(var, vec, mdata.Data)) {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    retData->VectorVar[i]->Sub(mask, TGridMaskedData(var, mdata.Mask), 1);
            } else {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    retData->VectorVar[i]->Sub(
                        mask, TGridMaskedData(vec->VectorVar[i], mdata.Mask), 1);
            }
            return ret;
        }
        virtual TGridMaskedData Mul(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata,
            int inPlace) {
            TGridVar<T> *var;
            TGridVectorVar<T> *vec;
            TGridMaskedData ret(this, mask);
            if(!inPlace)
                ret.Data = new TGridVectorVar(*this);
            TGridVectorVar<T> *retData =
                dynamic_cast<TGridVectorVar<T> *>(ret.Data.pointer());
            if(!IsVector(var, vec, mdata.Data)) {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    retData->VectorVar[i]->Mul(mask, TGridMaskedData(var, mdata.Mask), 1);
            } else {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    retData->VectorVar[i]->Mul(
                        mask, TGridMaskedData(vec->VectorVar[i], mdata.Mask), 1);
            }
            return ret;
        }
        virtual TGridMaskedData Mul(TGridMaskBase *mask, double val, int inPlace) {
            TGridMaskedData ret(this, mask);
            if(!inPlace)
                ret.Data = new TGridVectorVar(*this);
            TGridVectorVar<T> *retData =
                dynamic_cast<TGridVectorVar<T> *>(ret.Data.pointer());
            for(size_t i = 0; i < VectorVar.size(); i++)
                retData->VectorVar[i]->Mul(mask, val, 1);
            return ret;
        }
        virtual TGridMaskedData Div(
            TGridMaskBase *mask,
            const TGridMaskedData &mdata,
            int inPlace) {
            TGridVar<T> *var;
            TGridVectorVar<T> *vec;
            TGridMaskedData ret(this, mask);
            if(!inPlace)
                ret.Data = new TGridVectorVar(*this);
            TGridVectorVar<T> *retData =
                dynamic_cast<TGridVectorVar<T> *>(ret.Data.pointer());
            if(!IsVector(var, vec, mdata.Data)) {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    retData->VectorVar[i]->Div(mask, TGridMaskedData(var, mdata.Mask), 1);
            } else {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    retData->VectorVar[i]->Div(
                        mask, TGridMaskedData(vec->VectorVar[i], mdata.Mask), 1);
            }
            return ret;
        }
        virtual TGridMaskedData Div(double val, TGridMaskBase *mask, int inPlace) {
            TGridMaskedData ret(this, mask);
            if(!inPlace)
                ret.Data = new TGridVectorVar(*this);
            TGridVectorVar<T> *retData =
                dynamic_cast<TGridVectorVar<T> *>(ret.Data.pointer());
            for(size_t i = 0; i < VectorVar.size(); i++)
                retData->VectorVar[i]->Div(val, mask, 1);
            return ret;
        }
        virtual TGridMaskedData Abs(TGridMaskBase *mask, const TGridMaskedData &mdata) {
            TGridMaskedData ret(this, mask);
            TGridVectorVar<T> *retData =
                dynamic_cast<TGridVectorVar<T> *>(ret.Data.pointer());
            for(size_t i = 0; i < VectorVar.size(); i++)
                retData->VectorVar[i]->Abs(mask, mdata);
            return ret;
        }

        vector<Ref<TGridVar<T>>> VectorVar;
        int VectorSizes;

    public:
        TGridVectorVar(int dim = 0) : VectorSizes(dim){};
        TGridVectorVar(const TGridVectorVar &var) : VectorSizes(var.VectorSizes) {
            for(size_t i = 0; i < var.VectorVar.size(); i++)
                VectorVar.push_back(new TGridVar<T>(*var.VectorVar[i]));
        };
        void Push(TGridVariablesBase *dat) {
            Ref<TGridVar<T>> data = SavableClass::TestType<TGridVar<T>>(dat);
            if(data->Size() != VectorSizes)
                throw info_except(
                    "Sizes are different <%i> and <%i>\n", data->Size(), VectorSizes);
            VectorVar.push_back(data);
        }
        //void Set(Ref<TGridVar> data, size_t i) {
        //}
        //TGridVectorVar(const vector<T> &vec) : VectorT(vec) {};
        virtual void Set(TGridVariablesBase *vars) {
            TGridVar<T> *var;
            TGridVectorVar<T> *vec;
            if(!IsVector(var, vec, vars)) {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    VectorVar[i]->Set(vars);
            } else {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    VectorVar[i]->Set(vec->VectorVar[i]);
            }
        }
        //virtual TGridMaskedData Mul(TGridMaskBase *mask, const TGridMaskedData &mdata, int inPlace) = 0;
        //virtual TGridMaskedData Mul(TGridMaskBase *mask, double c, int inPlace) = 0;
        //virtual TGridMaskedData Div(TGridMaskBase *mask, const TGridMaskedData &mdata, int inPlace) = 0;
        //virtual TGridMaskedData Div(TGridMaskBase *mask, double c, int inPlace) = 0;


        Ref<TGridVar<T>> &operator[](size_t k) {
            return VectorVar[k];
        };
        inline size_t Dimension() {
            return VectorVar.size();
        }
        virtual void Resize(int dim, TGridMaskBase *curMask, TGridMaskBase *wasMask) {
            VectorSizes = dim;
            for(size_t i = 0; i < VectorVar.size(); i++)
                VectorVar[i]->Resize(dim, curMask, wasMask);
            //throw info_except("Are you shure?\n");
        }
        virtual int Size() {
            return VectorSizes;
        }
        virtual void *GetElementPtr(size_t k) {
            throw info_except("Cannot get element ptr for vector of vars..\n");
            //return &(VectorT[k]);
        }
        virtual void SetElement(size_t k, TGridVariablesBase *data, size_t maskK) {
            //throw info_except("Cannot get element ptr for vector of vars..\n");

            TGridVar<T> *var;
            TGridVectorVar<T> *vec;
            if(!IsVector(var, vec, data)) {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    VectorVar[i]->SetElement(k, var, maskK);
            } else {
                for(size_t i = 0; i < VectorVar.size(); i++)
                    VectorVar[i]->SetElement(k, vec->VectorVar[i], maskK);
            }
        }

        int save_data_state(FilterOut &so) {
            so << " { ";
            SavableClass::IncIdent();
            for(size_t i = 0; i < VectorVar.size(); i++)
                so << SavableClass::EOLN() << VectorVar[i];
            so << SavableClass::mmEOLN() << " } ";
            return 1;
        };
        int read_data_state(FilterIn &si) {
            VectorVar.clear();
            VectorSizes = 0;
            Stroka tmp;
            SavableClass::ExceptionCheck(si, "{");
            Ref<TGridVar<T>> ptr;
            while(!SavableClass::TestNextChar(si, '}')) {
                si >> ptr;
                VectorVar.push_back(ptr);
            }
            if(VectorVar.size() > 0)
                VectorSizes = VectorVar[0]->Size();
            return 1;
        };


        //double operator[](size_t k)const{ return Ptr[k];};
    };


};   //namespace NPolygon {
