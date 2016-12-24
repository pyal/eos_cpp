
#ifndef Grid_Array_h
#define Grid_Array_h 1


#include "grid_interface.h"
#include "grid_mask_data.h"
#include "grid_mask.h"


//struct GridMaskVector;
//template <class T, class CpMeth = CopyStructFast<T> >
template <class T, class CpMeth>   // - already defined in grid_mask.h !!!!!!!!???????
struct GridVectorField : public FieldInterface   //public GridVar<T,CpM>
{
    //   GridVectorField(T &Def){store=NULL; DefaultValue=Def;}
    GridVectorField(
        int NumVar = 0,
        GridManipulator *stor = NULL,
        char *name = NULL,
        int Local = 0)
    //    {store=NULL;if (stor) {SetStore(stor);if (name) set_obj_name(name);}if (Local) SetLocal(1);SetDim(NumVar);}
    {
        if(name)
            set_obj_name(name);
        SetDim(NumVar);
        if(stor)
            stor->AddVar(this);
        if(Local)
            SetLocal(1);
    }
    ~GridVectorField() {
        //     fcout<<" Deleting Vect "<<get_obj_name()<<"\n";
        if(!managed()) {
            FieldInterface *field;
            for(int k = 0; k < data.SetDim(); k++) {
                field = data[k];
                data[k]->unmanage();
                data[k] = NULL;
                delete field;
            }
        }
        //     SetStore(NULL);
        FieldInterface::~FieldInterface();
    }
    void SetColumn(GridField<T, CpMeth> *col, int k) {
        data[k] = col;
    }

    int SetLocal(int ToSet = -1) {
        if(ToSet == 1) {
            for(int k = 0; k < data.SetDim(); k++)
                data[k]->unmanage();
            unmanage();
        }
        return !managed();
    };
    int SetDim(int NewDim = -1) {
        int OldDim = data.SetDim();
        if((NewDim == -1) || (NewDim == OldDim))
            return OldDim;
        data.SetDim(NewDim);
        GridManipulator *tmp = GetGridManip();
        char namp[256], buf[256];
        const char *nam = get_obj_name();
        for(int k = 0; k < NewDim; k++) {
            strcpy(namp, nam);
            strcat(namp, "_");
            strcat(namp, Itoa(k, buf, 10));
            data[k] = new GridField<T, CpMeth>(tmp, namp);
        }

        //     for (int k=0;k<NewDim;k++) data[k]=new GridField<T,CpMeth>(tmp,namp);
        SetLocal(SetLocal());
        return NewDim;
    }
    GridVectorField(const GridVectorField<T, CpMeth> &vec) {
        int k, N = vec.data.SetDim();
        data.SetDim(N);
        for(k = 0; k < N; k++)
            data[k] = new GridField<T, CpMeth>(*(vec.data[k]));
        SetStore(vec.GetGridManip());
        // To use in return statement
        SetLocal(vec.SetLocal());
    }
    GridManipulator *SetStore(GridManipulator *stor) {
        int k, N = data.SetDim();
        if(stor)
            stor->AddVar(this);
        else {
            stor = GetGridManip();
            if(!stor)
                return stor;   //for (k=0;k<N;k++) stor->DelVar(data[k]);
            stor->DelVar(this);
            return NULL;
        }
        //     if (stor) for (k=0;k<N;k++) stor->AddVar(data[k]);
        return stor;
    }
    //    {    for (int k=0;k<data.SetDim();k++)    if (data[k]!=NULL) data[k]->put_in_store(stor); return FieldInterface::SetStore(stor); }
    inline GridManipulator *SetStore() {
        return GetGridManip();
    }
    GridVectorField<T, CpMeth> &operator=(const T &vec) {
        for(int k = 0; k < data.SetDim(); k++)
            *(data[k]) = vec;
        return *this;
    }
    GridVectorField<T, CpMeth> &operator=(const GridVectorField<T, CpMeth> &vec) {
        for(int k = 0; k < data.SetDim(); k++)
            *(data[k]) = *(vec.data[k]);
        return *this;
    }

    //    ~GridVectorField()  {  Delete(); }
    //    virtual void Delete() { if (get_StoredBy()!=NULL) {for (int k=0;k<data.SetDim();k++) data[k]=NULL;} NamedObject::~NamedObject();};//     FieldInterface::~FieldInterface();

    inline T &GetPnt(GridIndex k, int Axis) {
        return data[Axis]->GetPnt(k);
    };
    DataVector<T, CpMeth> GetPnt(GridIndex k) {
        int N = SetDim();
        DataVector<T, CpMeth> ret(N);
        for(int k1 = 0; k1 < N; k1++)
            ret[k1] = data[k1]->GetPnt(k);
        return ret;
    };
    void SetPnt(DataVector<T, CpMeth> &set, GridIndex k) {
        int N = SetDim();
        for(int k1 = 0; k1 < N; k1++)
            data[k1]->GetPnt(k) = set[k1];
    };

    DataVector<T, CpMeth> GetMaskedPnt(DataVector<GridIndex> &mask, int Axis) {
        return data[Axis]->GetMaskedPnt(mask);
    };
    void SetMaskedPnt(DataVector<T, CpMeth> &ret, DataVector<GridIndex> &mask, int Axis) {
        data[Axis]->SetMaskedPnt(ret, mask);
    };

    inline GridField<T, CpMeth> *GetX(int axis) {
        return data[axis];
    }

    void AddPnt(
        GridIndex
            ax){};   //{for (int k=0;k<data.SetDim();k++) data[k]->GetPnt(ax)=DefaultValue;}
    void DelPnt(GridIndex k){};
    void ModifyPnt(GridIndex k){};
    void MovePnt(GridIndex From, GridIndex To) {
        for(int k = 0; k < data.SetDim(); k++)
            data[k]->GetPnt(To) = data[k]->GetPnt(From);
    }

    int save_data_state(FilterOut &so) {
        FieldInterface::save_data_state(so);
        so << KeyDesc("data") << data;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        FieldInterface::read_data_state(si);
        si >> StdKey >> data;
        return 1;
    };

    void SetDefault(T &Def) {
        for(int k = 0; k < data.SetDim(); k++)
            data[k]->SetDefault(Def);
    }
    void StoreChanged() {
        GridManipulator *tmp = GetGridManip();
        if(tmp) {
            for(int k = 0; k < data.SetDim(); k++)
                tmp->AddVar(data[k]);
        } else {
            FieldInterface *field;
            for(int k = 0; k < data.SetDim(); k++)
                if((field = (FieldInterface *)data[k]->get_StoredBy()) &&
                   (tmp = field->GetGridManip()))
                    tmp->DelVar(data[k]);
        }
    }
    inline GridManipulator *GetGridManip() {
        FieldInterface *ret = dynamic_cast<FieldInterface *>(get_StoredBy());
        return (ret == NULL) ? NULL : ret->GetGridManip();
    }


    DataVector<Ref<GridField<T, CpMeth>>, CopyStructRef<Ref<GridField<T, CpMeth>>>> data;

    //   MaskedData  <T,CpMeth> operator()(const GridMask &mask);
    MaskedVector<T, CpMeth> operator()(const GridMaskVector &mask);
};


template <class T, class CpMeth>
MaskedVector<T, CpMeth> GridVectorField<T, CpMeth>::operator()(
    const GridMaskVector &mask) {
    MaskedVector<T, CpMeth> ret(this, mask);
    return ret;
}


#endif