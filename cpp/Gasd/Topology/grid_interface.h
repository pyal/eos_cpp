#ifndef Grid_Interface_h
#define Grid_Interface_h 1

#include "Grid_Action.h"
#include "Grid_Mask.h"
#include "Grid_Manip.h"


template <class T, class CpM>
struct MaskedData;
template <class T, class CpM>
struct MaskedVector;
// Concrete scheme of realization of variable as an array
// Type specific standart procedures for a variable are defined
template <class T, class CpM = CopyStructFast<T>>
struct GridField : public FieldInterface   //public GridVar<T,CpM>
{
    GridField(GridManipulator *stor = NULL, char *name = NULL, int Local = 0) {
        data.SetPolicy(50, -50, 0, NULL);
        if(stor)
            stor->AddVar(this);
        if(name)
            set_obj_name(name);
        if(Local)
            unmanage();
    }
    GridField(const GridField<T, CpM> &vec) {
        put_in_store(vec.get_StoredBy());
        // To use in return statement
        if(!vec.managed())
            unmanage();
        data = vec.data;
    }
    GridField<T, CpM> &operator=(const T &vec) {
        data = vec;
        return *this;
    }
    GridField<T, CpM> &operator=(const GridField<T, CpM> &vec) {
        data = vec.data;
        return *this;
    }
    MaskedData<T, CpM> operator()(const GridMask &mask);
    MaskedVector<T, CpM> operator()(const GridMaskVector &mask);


    inline virtual T &GetPnt(GridIndex k) {
        return data[k];
    };
    virtual DataVector<T, CpM> GetMaskedPnt(DataVector<GridIndex> &mask);
    virtual void SetMaskedPnt(DataVector<T, CpM> &ret, DataVector<GridIndex> &mask);


    inline virtual void AddPnt(GridIndex k){};   //{data[k]=DefaultValue;}
    inline virtual void DelPnt(GridIndex k){};
    inline virtual void ModifyPnt(GridIndex k){};
    inline virtual void MovePnt(GridIndex From, GridIndex To) {
        data[To] = data[From];
    }

    inline virtual int save_data_state(FilterOut &so) {
        FieldInterface::save_data_state(so);
        so << KeyDesc("data") << data;
        return 1;
    };
    inline virtual int read_data_state(FilterIn &si) {
        FieldInterface::read_data_state(si);
        si >> StdKey >> data;
        return 1;
    };

    inline virtual void SetDefault(T &Def) {
        data.SetFill(&Def);
    }
    inline virtual void StoreChanged() {
        if(GetGridManip())
            data.SetNumEl(GetGridManip()->NumPnt(1));
    }   //else data.SetNumEl(0);}

    inline GridManipulator *GetGridManip() {
        FieldInterface *ret = dynamic_cast<FieldInterface *>(get_StoredBy());
        if(ret == NULL)
            //      {fcout<<"Very bad. GridField::GetGridManip - is NULL\n";abort();};
            return NULL;
        return ret->GetGridManip();
    }


    ResizeVector<T, CpM> data;
};


// General class for keeping consistent references,
// after insertion,deleting,moving of points ( work under object supporting standart func?)
// and keeping a list of variables ( attributes ) for stored points
template <class T, class CpM = CopyStructSlow<T>>
struct Grid : public GridField<T, CpM>, public NamedObjectStoreAbstract {
    Grid() : GridField<T, CpM>(), Frozen(0) {
        VarList.set_DefaultStorage(this);
    };

    virtual T &GetPnt(GridIndex k) {
        if(k == -1)
            return OldGridKValue;
        return data[k];
    };


    virtual void AddPnt(GridIndex k) {
        ExEnv::err() << " Grid::AddPnt(k) - useless? is it not ? " << endl;
        abort();
    }
    virtual void DelPnt(GridIndex k) {
        OldGridKValue = data[k];
        SetAction(GridAction::DelPnt, k);
    };
    virtual void MovePnt(GridIndex From, GridIndex To) {
        data[To] = data[From];
        SetAction(GridAction::MovePnt, From, To);
    }
    virtual void ModifyPnt(GridIndex k) {
        ExEnv::err() << " Grid::ModifyPnt(k) - useless? is it not ? " << endl;
        abort();
    }

    // Instead AddPnt(k) ModifyPnt(k) - for Grid is used
    // AddPnt(k,val) ModifyPnt(k,val)
    virtual void ModifyPnt(GridIndex k, T &val) {
        OldGridKValue = data[k];
        data[k] = val;
        SetAction(GridAction::ModifyPnt, k);
    };
    virtual void AddPnt(GridIndex k, T &val) {
        data[k] = val;
        SetAction(GridAction::AddPnt, k);
    }

    inline GridManipulator *GetGridManip() {
        AreaStoreInterface *ret = dynamic_cast<AreaStoreInterface *>(get_StoredBy());
        if(ret == NULL) {
            fcout << "Very bad. Grid::GetGridManip - is NULL\n";
            abort();
        };
        return ret->GetGridManip();
    }

    virtual int save_data_state(FilterOut &so);
    virtual int read_data_state(FilterIn &si);

    virtual int GridFrozen(int frozen = -1);

    // from NamedObjectStoreAbstract
    inline void DeleteObject(NamedObject *obj) {
        CheckFrozen("DeleteObject");
        VarList.DeleteObject(obj);
    };
    inline int Add(NamedObject *dat, int pos = -1) {
        CheckFrozen("Add");
        return VarList.Add(dat, pos);
    };

    T OldGridKValue;


    NamedStore VarList;
    SparceArrayInd StoredPnt;

private:
    inline void CheckFrozen(char *func) {
        if(Frozen) {
            ExEnv::err() << " Error in " << func << "  Grid Frozen. Unfrozing first "
                         << endl;
            GridFrozen(0);
        }
    }
    void VarAddAction(int act, GridIndex k, GridIndex To = -1);
    void SetAction(int act, GridIndex k, GridIndex To = -1, int froze = -1);

    virtual void VarAddPnt(GridIndex k);
    virtual void VarDelPnt(GridIndex k);
    virtual void VarMovePnt(GridIndex From, GridIndex To);
    virtual void VarModifyPnt(GridIndex k);


    //   virtual void InitVar(FieldInterface *NewVar);
protected:
    // After Freezing - is saved to DeletedFrozenList
    // and this one is updated without deletion;
    int Frozen;
    SparceArrayInd FrozenPnt;
    ResizeVector<GridAction> actlist;
    ResizeVector<T, CpM> oldparlist;
};

struct BoundaryInterfaceStd : public BoundaryInterface {
    inline void Init() {
        AreaStoreInterface *ar = dynamic_cast<AreaStoreInterface *>(get_StoredBy());
        if(ar == NULL) {
            fcout << "Very bad. GridManipulatorStd::Init Area - is NULL\n";
            abort();
        };
        grid_manip = ar->GetGridManip();
    }

    // For all points - forming bnd
    virtual void GetPntBound(GridIndex pnt, int pntpos, GridBoundPnt &bnd);
    // One point is boundary ( -1 - is a neibour ); bnd First,Second,Pos are already formed by GetPntBound
    virtual void GetBoundary(GridBoundPnt &bnd);

    inline virtual int save_data_state(FilterOut &so) {
        BoundaryInterface::save_data_state(so);
        so << KeyDesc("grid_manip") << (SavableClass *)grid_manip;
        return 1;
    };
    inline virtual int read_data_state(FilterIn &si) {
        BoundaryInterface::read_data_state(si);
        SavableClass *tmp;
        si >> StdKey >> tmp;
        grid_manip = dynamic_cast<GridManipulator *>(tmp);
        return 1;
    };

    GridManipulator *grid_manip;
};


// T - is some Grid child
template <class T>
struct GridManipulatorStd : public GridManipulator {
    //   inline virtual FieldInterface *GetGridField(){return store;}
protected:
    T *grid;
    BoundaryInterface *bound;
    NamedStore *gvar;
    SparceArrayInd *ind;
public:
    void Init();
    inline FieldInterface *GetGridField() {
        return grid;
    };
    inline BoundaryInterface *GetBound() {
        return bound;
    };


    // Field Manipulator
    virtual inline int AddVar(FieldInterface *NewVar) {
        return grid->Add(NewVar);
    }
    virtual inline void DelVar(FieldInterface *NewVar) {
        if(grid)
            grid->DeleteObject(NewVar);
        else {
            ExEnv::err()
                << " Trying to delete var from grid==NULL in GridManipulatorStd\n";
        }
    }
    virtual inline FieldInterface *GetVar(char *name) {
        return (FieldInterface *)gvar->GetPtr(gvar->Name2Pos(name));
    }
    virtual inline FieldInterface *GetVar(int Num) {
        return (FieldInterface *)gvar->GetPtr(Num);
    }
    virtual inline int GetVar(FieldInterface *get2var) {
        return get2var->get_StoredByNum();
    }


    virtual int save_data_state(FilterOut &so) {
        so << KeyDesc("Grid_ptr") << (SavableClass *)grid << KeyDesc("Bound_ptr")
           << (SavableClass *)bound;
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        SavableClass *gr, *bn;
        si >> StdKey >> gr >> StdKey >> bn;
        grid = dynamic_cast<T *>(gr);
        bound = dynamic_cast<BoundaryInterface *>(bn);
        if(grid != NULL) {
            gvar = &(grid->VarList);
            ind = gvar->GetInd();
        }
        return 1;
    };

    // if CountDeleted - return MaxInd of grid
    virtual inline int NumPnt(int CountDeleted = 0) {
        return ind->Num(CountDeleted);
    };
    virtual inline GridIndex NextPnt(GridIndex i) {
        return ind->Next(i);
    };

    virtual inline GridIndex StartPnt() {
        return ind->Next(-1);
    };
    virtual inline GridIndex StartEmptyIndex() {
        return ind->NextEmp(-1);
    };
    virtual inline GridIndex NextEmptyIndex(GridIndex i) {
        return ind->NextEmp(i);
    };

    virtual inline int GridFrozen(int frozen = -1) {
        return grid->GridFrozen(frozen);
    };
    virtual inline void GetPntBound(GridIndex pnt, int pntpos, GridBoundPnt &bnd) {
        bound->GetPntBound(pnt, pntpos, bnd);
    };

    void PrintAllGridVars(FilterOut &out) {
        int cur = -1, i = 0;
        NamedObject *tmp;
        SparceArrayInd *in_ = gvar->GetInd();
        while((cur = in_->Next(cur)) >= 0) {
            out << ++i << " " << cur << " ";
            if((tmp = gvar->GetPtr(cur)) != NULL)
                out << "managed " << tmp->managed() << " "

                    << "Nref " << (int)tmp->nreference() << " " << tmp->get_obj_name();
            else
                out << " Bad - NULL - ptr in store ";
            out << "\n";
            out.flush();
        }
    }


    // Procedures Exact store type dependent
    // To be defined in childs

    //   virtual SpacePoint GetGridPnt(GridIndex k)=0;
    //   virtual void SetGridPnt(SpacePoint newVal,GridIndex k)=0;
    //   virtual DataVector<SpacePoint> GetGridMaskedPnt(DataVector<GridIndex> &mask)=0;
    //   virtual void SetGridMaskedPnt(DataVector<SpacePoint> &data,DataVector<GridIndex> &mask)=0;
    //   virtual DataVector<GridIndex> GetNeibous(GridIndex k)=0;
};


// =====================================================================
// =================         GridField<T>   =====================
// =====================================================================
#include "grid_mask_data.h"
#include "grid_mask_vector.h"
template <class T, class CpM>
MaskedData<T, CpM> GridField<T, CpM>::operator()(const GridMask &mask) {
    MaskedData<T, CpM> ret(this, mask);
    return ret;
}
template <class T, class CpM>
MaskedVector<T, CpM> GridField<T, CpM>::operator()(const GridMaskVector &mask) {
    MaskedVector<T, CpM> ret(this, mask);
    return ret;
}

template <class T, class CpM>
DataVector<T, CpM> GridField<T, CpM>::GetMaskedPnt(DataVector<GridIndex> &mask) {
    int k, N = mask.SetDim();
    DataVector<T, CpM> ret(N);
    // if (N==0) {ret=data;return ret;}
    for(k = 0; k < N; k++)
        ret[k] = data[mask[k]];
    return ret;
};
template <class T, class CpM>
void GridField<T, CpM>::SetMaskedPnt(
    DataVector<T, CpM> &ret,
    DataVector<GridIndex, CopyStructSlow<GridIndex>> &mask) {
    int k, N = mask.SetDim();
    // if (N==0) data=ret; else
    {
        for(k = 0; k < N; k++)
            data[mask[k]] = ret[k];
    }
};


// =====================================================================
// =================         GridField<T>   =====================
// =====================================================================

// =====================================================================
// =================           Grid<T>          =====================
// =====================================================================

template <class T, class CpM>
void Grid<T, CpM>::VarAddAction(int act, GridIndex k, GridIndex To) {
    int N = actlist.SetNumEl();
    oldparlist[N] = OldGridKValue;
    actlist[N].SetAction(act, k, To);
};

template <class T, class CpM>
void Grid<T, CpM>::SetAction(int act, GridIndex k, GridIndex To, int froze) {
    if(froze == -1)
        froze = Frozen;
    switch(act) {
    case(GridAction::AddPnt):
        StoredPnt.Add(k);
        if(!froze)
            VarAddPnt(k);
        else
            VarAddAction(act, k, To);
        break;
    case(GridAction::DelPnt):
        if(!froze) {
            StoredPnt.Del(k);
            VarDelPnt(k);
        } else
            VarAddAction(act, k, To);
        break;
    case(GridAction::MovePnt):
        StoredPnt.Add(To);
        if(!froze) {
            StoredPnt.Del(k);
            VarMovePnt(k, To);
        } else
            VarAddAction(act, k, To);
        break;
    case(GridAction::ModifyPnt):
        if(!froze)
            VarModifyPnt(k);
        else
            VarAddAction(act, k, To);
        break;
    default:
        ExEnv::err() << " In SetAction unknown action ! Abort." << endl;
        abort();
    }
};


template <class T, class CpM>
void Grid<T, CpM>::VarAddPnt(GridIndex i) {
    SparceArrayInd *var_ind = VarList.GetInd();
    int cur = -1;
    while((cur = var_ind->Next(cur)) >= 0)
        ((FieldInterface *)VarList.GetPtr(cur))->AddPnt(i);
};
template <class T, class CpM>
void Grid<T, CpM>::VarDelPnt(GridIndex i) {
    SparceArrayInd *var_ind = VarList.GetInd();
    int cur = -1;
    while((cur = var_ind->Next(cur)) >= 0)
        ((FieldInterface *)VarList.GetPtr(cur))->DelPnt(i);
};
template <class T, class CpM>
void Grid<T, CpM>::VarMovePnt(GridIndex From, GridIndex To) {
    SparceArrayInd *var_ind = VarList.GetInd();
    int cur = -1;
    while((cur = var_ind->Next(cur)) >= 0)
        ((FieldInterface *)VarList.GetPtr(cur))->MovePnt(From, To);
};
template <class T, class CpM>
void Grid<T, CpM>::VarModifyPnt(GridIndex i) {
    SparceArrayInd *var_ind = VarList.GetInd();
    int cur = -1;
    while((cur = var_ind->Next(cur)) >= 0)
        ((FieldInterface *)VarList.GetPtr(cur))->ModifyPnt(i);
};


template <class T, class CpM>
int Grid<T, CpM>::save_data_state(FilterOut &so) {
    if(Frozen) {
        ExEnv::err() << " Error Trying to save Frozen Grid. Unfrozing first " << endl;
        GridFrozen(0);
    }
    GridField<T, CpM>::save_data_state(so);
    so << KeyDesc("StoredPnt");
    so << StoredPnt;
    so << KeyDesc("VarList");
    so << VarList;
    return 1;
};
template <class T, class CpM>
int Grid<T, CpM>::read_data_state(FilterIn &si) {
    //  if (Frozen) {ExEnv::err()<<" Error Trying to read to Frozen Grid. Unfrozing first "<<endl;GridFrozen(0);}
    Frozen = 0;
    GridField<T, CpM>::read_data_state(si);
    si >> StdKey >> StoredPnt >> StdKey >> VarList;
    return 1;
};

template <class T, class CpM>
int Grid<T, CpM>::GridFrozen(int frozen) {
    if(frozen == -1)
        return Frozen;
    if(Frozen == frozen) {
        ExEnv::err() << " Not single attempt to set FrozenGrid to state " << frozen
                     << endl;
        return Frozen;
    }
    if(frozen)
        FrozenPnt = StoredPnt;
    else {
        StoredPnt = FrozenPnt;   // Reseting Deleted List with deletion
        int k, N = actlist.SetNumEl();
        for(k = 0; k < N; k++) {
            GridAction act = actlist[k];
            OldGridKValue = oldparlist[k];
            SetAction(act.Action, act.Index, act.Param, 0);
        }
    }
    actlist.SetNumEl(0);
    oldparlist.SetNumEl(0);
    Frozen = frozen;
    return Frozen;
};

// =====================================================================
// ===================           Grid<T>          ======================
// =====================================================================

// =====================================================================
// =================     GridManipulatorStd<T>     =====================
// =====================================================================


template <class T>
void GridManipulatorStd<T>::Init() {
    AreaStoreInterface *ar = dynamic_cast<AreaStoreInterface *>(get_StoredBy());
    if(ar == NULL) {
        fcout << "Very bad. GridManipulatorStd::Init Area - is NULL\n";
        abort();
    };
    grid = dynamic_cast<T *>(ar->GetGridField());
    bound = ar->GetBound();
    if(grid == NULL) {
        fcout << "Very bad. GridManipulatorStd::Init - grid is NULL\n";
        abort();
    };
    gvar = &(grid->VarList);
    ind = &(grid->StoredPnt);
};


#endif


/*
#ifndef Grid_Interface_h
#define Grid_Interface_h 1

#include "Grid_Action.h"
#include "Grid_Mask.h"



struct FieldInterface;

// General procedures for all types of grids are defined
struct GridManipulator : public SavableClass
 {

//   GridManipulator(FieldInterface *stor):store(stor){};
//   GridManipulator():store(NULL){};
//   GridManipulator(const GridManipulator &stor){store=stor.store;};
//   GridManipulator &operator=(const GridManipulator &stor){store=stor.store;return *this;};

   GridManipulator(){mask_manip=new MaskManipulatorGeneral(this);};
   virtual ~GridManipulator(){};

// For GridIndex==-1 - return OldPnt Par
   virtual SpacePoint GetGridPnt(GridIndex k)=0;
   virtual void SetGridPnt(SpacePoint newVal,GridIndex k)=0;
   virtual DataVector<SpacePoint,CopyStructSlow<SpacePoint> > GetGridMaskedPnt(DataVector<GridIndex> &mask)=0;
   virtual void SetGridMaskedPnt(DataVector<SpacePoint,CopyStructSlow<SpacePoint> > &data,DataVector<GridIndex> &mask)=0;
//   virtual DataVector<GridIndex> ConstructMask(FieldInterface *dat)=0;

// For GridIndex==-1 - return OldPnt Par
   virtual DataVector<GridIndex, CopyStructSlow<GridIndex> > GetNeibous(GridIndex k)=0;

   virtual FieldInterface* GetStore(FieldInterface *newStor=NULL)=0;
   virtual MaskManipulatorGeneral* GetMask(MaskManipulatorGeneral *newMask=NULL){if (newMask!=NULL) mask_manip=newMask;return mask_manip;}


   virtual int AddVar(FieldInterface *NewVar)=0;
   virtual void DelVar(FieldInterface *NewVar)=0;
   virtual FieldInterface* GetVar(char *name)=0;
   virtual FieldInterface* GetVar(int Num)=0;
   virtual int GetVar(FieldInterface* get2var)=0;
//   virtual FieldInterface* NextVar(int i); virtual int StartVar();

   virtual int NumPnt(int CountDeleted=0)=0; // if CountDeleted - return MaxInd of grid
   virtual GridIndex NextPnt(GridIndex i)=0;
   virtual GridIndex StartPnt() =0;
   virtual GridIndex StartEmptyIndex()=0;
   virtual GridIndex NextEmptyIndex(GridIndex i)=0;


// Frozen - means making a block of linked point addition
// All Points have to be added (deleted) first before sending
// information about changes to Vars linked Grid
// Frozen operations : AddVar DelVar save_data_state()
   virtual int GridFrozen(int frozen=-1)=0;//{if (frozen!=-1) Frozen=frozen;return Frozen;};

   virtual void GetPntBound(GridIndex pnt,int pntpos,GridBoundPnt &bnd)=0;

   Ref<MaskManipulatorGeneral> mask_manip;
 };

// General procedures for all types of variables on grid are defined
struct FieldInterface : public SavableClass
 {

   virtual void AddPnt(GridIndex k){};    //Only After
   virtual void DelPnt(GridIndex k){};    //Only After (what about before ?)
   virtual void ModifyPnt(GridIndex k){}; //Only After - grid structure not changed !!!
   virtual void MovePnt(GridIndex From,GridIndex To){}; //Only After


// Mask may be position dependent ( not in array order ) simple 0 1 - do not work
//   virtual void SetMask(DataVector<GridIndex> &mask)=0;

   FieldInterface():store(NULL){DataName[0]=0;};
   FieldInterface(GridManipulator *stor){store=NULL;SetStore(stor);DataName[0]=0;};
   virtual ~FieldInterface(){store=NULL;}

// Delete var from the current store, generate var in the new store
   virtual GridManipulator* SetStore(GridManipulator *stor);//{store=stor;}
   virtual GridManipulator* SetStore() const {return store;}
// To be Called each time var is setted to the new store   
   virtual void SetStoreData(){};

// Grids will redefine it
   virtual void SetGridManip(GridManipulator *newManip){};
   virtual GridManipulator* SetGridManip(){return NULL;};

   virtual int save_data_state (FilterOut &so)  {so<<KeyDesc("DataName")<<DataName<<KeyDesc("Store")<<store;return 1;};
// {  return 1;}

   virtual int read_data_state (FilterIn &si)   {si>>StdKey>>&(DataName[0])>>StdKey>>store;return 1;};

   char * SetDataName(char *new_name=NULL)   { if (new_name!=NULL) strcpy(DataName,new_name);  return DataName; };
 protected:// ?
//   GridManipulator *store;
   Ref<GridManipulator> store;
   char DataName[20];

 };
/*
// Type specific procedures for a variable are defined
// for any Store variable as empty ones
template <class T,class CpM=CopyStructFast<T> >
struct GridVar : public FieldInterface
 {
   virtual T& GetPnt(GridIndex k)=0;
   virtual DataVector<T,CpM> GetMaskedPnt(DataVector<GridIndex> &mask)=0;
   virtual void SetMaskedPnt(DataVector<T,CpM> &data,DataVector<GridIndex> &mask)=0;
//   virtual T* GetPnt();
 };
* /
template <class T,class CpM >
struct MaskedData;
template <class T,class CpM >
struct MaskedVector;
// Concrete scheme of realization of variable as an array
// Type specific standart procedures for a variable are defined
template <class T,class CpM=CopyStructFast<T> >
struct GridField : public FieldInterface //public GridVar<T,CpM>
 {
//   GridField(){store=NULL; data.SetPolicy(50,-50,0,&DefaultValue);}
   GridField(T &Def){store=NULL; DefaultValue=Def;data.SetPolicy(50,-50,0,&DefaultValue);}
   GridField(GridManipulator *stor=NULL,char *name=NULL,int Local=0)
    {store=NULL;data.SetPolicy(50,-50,0,&DefaultValue);
     if (stor) {SetStore(stor);if (name) SetDataName(name);}
     if (Local) unmanage();
    }
   GridField(const GridField<T,CpM> &vec) 
    { 
      SetStore(vec.SetStore());
// To use in return statement
      if (!vec.managed()) unmanage();
      data=vec.data;
    }
   GridField<T,CpM>& operator = (const T& vec){data=vec;return *this;}
   GridField<T,CpM>& operator = (const GridField<T,CpM>& vec) {data=vec.data;return *this;}
   MaskedData<T,CpM> operator()(const GridMask &mask);
   MaskedVector<T,CpM> operator()(const GridMaskVector &mask);
    
    ~GridField()  {  Delete(); }
   virtual void Delete() { if (store!=NULL) store->DelVar(this); store=NULL;};//     FieldInterface::~FieldInterface();

   inline virtual T& GetPnt(GridIndex k){return data[k];};
   virtual DataVector<T,CpM> GetMaskedPnt(DataVector<GridIndex> &mask);
   virtual void SetMaskedPnt(DataVector<T,CpM> &ret,DataVector<GridIndex> &mask);


   virtual void AddPnt(GridIndex k) {data[k]=DefaultValue;}
   virtual void DelPnt(GridIndex k)   {};
   virtual void ModifyPnt(GridIndex k){};
   virtual void MovePnt(GridIndex From,GridIndex To){data[To]=data[From];}

   virtual int save_data_state (FilterOut &so);//  {FieldInterface::save_data_state(so); so<<KeyDesc("DefaultValue")<<DefaultValue<<KeyDesc("data")<<data;return 1;};
   virtual int read_data_state (FilterIn &si);//  {FieldInterface::read_data_state(si); si>>StdKey>>DefaultValue>>StdKey>>data;return 1;};

   virtual void SetDefault(T &Def) {data.SetPolicy(50,-50,0,&Def);DefaultValue=Def;}
   virtual void SetStoreData(){data.SetNumEl(store->NumPnt(1));}

   ResizeVector<T,CpM> data;
   T DefaultValue;
 };


// General class for keeping consistent references,
// after insertion,deleting,moving of points ( work under object supporting standart func?)
// and keeping a list of variables ( attributes ) for stored points 
template <class T,class CpM=CopyStructSlow<T> >
struct Grid : public GridField< T,CpM >
 {
   Grid() :GridField< T,CpM  >(),grid_manip(NULL),Frozen(0)  { int k=1; DeletedList.SetPolicy(50,-50,0,&k); };
   Grid(T &Def):GridField< T,CpM  >(Def),grid_manip(NULL),Frozen(0) { int k=1; DeletedList.SetPolicy(50,-50,0,&k);};
   ~Grid() {Delete();}
   virtual void Delete()
    {FieldInterface *tmp;for (int k=0;k<VarList.SetNumEl();k++) 
    {tmp=VarList[k];if (tmp) tmp->SetStore(NULL);}}//grid_manip=NULL; }

   virtual T& GetPnt(GridIndex k){if (k==-1) return OldGridKValue;return data[k];};

   virtual void AddPnt(GridIndex k){ ExEnv::err()<<" Grid::AddPnt(k) - useless? is it mot ? "<<endl;abort();}
   virtual void DelPnt(GridIndex k) {OldGridKValue=data[k];SetAction(GridAction::DelPnt,k);};
   virtual void MovePnt(GridIndex From,GridIndex To) {data[To]=data[From];SetAction(GridAction::MovePnt,From,To); }
   virtual void ModifyPnt(GridIndex k){ ExEnv::err()<<" Grid::ModifyPnt(k) - useless? is it mot ? "<<endl;abort();}

// Instead AddPnt(k) ModifyPnt(k) - for Grid is used
// AddPnt(k,val) ModifyPnt(k,val)
   virtual void ModifyPnt(GridIndex k,T& val){OldGridKValue=data[k];data[k]=val;SetAction(GridAction::ModifyPnt,k);};
   virtual void AddPnt(GridIndex k,T& val)   {data[k]=val;SetAction(GridAction::AddPnt,k);}

   virtual int NumPnt(int CountDeleted=0); // if CountDeleted - return MaxInd of grid
   virtual GridIndex NextPnt(GridIndex i);
   virtual GridIndex StartPnt() {return NextPnt(-1);}
   virtual GridIndex StartEmptyIndex()   {return NextEmptyIndex(-1);}
   virtual GridIndex NextEmptyIndex(GridIndex i);

   virtual int AddVar(FieldInterface *NewVar);
   virtual void DelVar(FieldInterface *NewVar);
   virtual FieldInterface* GetVar(char *name);
   virtual FieldInterface* GetVar(int Num);
   virtual int GetVar(FieldInterface* get2var);
//   virtual FieldInterface* NextVar(int i); virtual int StartVar();
//   virtual DataVector<GridIndex> ConstructMask(FieldInterface *dat);

   virtual void SetGridManip(GridManipulator *newManip);
   virtual GridManipulator* SetGridManip(){return grid_manip;};

   virtual int save_data_state (FilterOut &so);
   virtual int read_data_state (FilterIn &si);

   virtual int GridFrozen(int frozen=-1);
// For all points - forming bnd
   virtual void GetPntBound(GridIndex pnt,int pntpos,GridBoundPnt &bnd);
// One point is boundary ( -1 - is a neibour ); bnd First,Second,Pos are already formed by GetPntBound
   virtual void GetBoundary(GridBoundPnt &bnd);


   T OldGridKValue;
  private:

   void VarAddAction(int act,GridIndex k,GridIndex To=-1);
   void SetAction(int act,GridIndex k,GridIndex To=-1,int froze=-1);

   virtual void VarAddPnt(GridIndex k);
   virtual void VarDelPnt(GridIndex k);
   virtual void VarMovePnt(GridIndex From,GridIndex To);
   virtual void VarModifyPnt(GridIndex k);
   ResizeVector< Ref<FieldInterface>,CopyStructRef<Ref<FieldInterface> > > VarList;


//   virtual void InitVar(FieldInterface *NewVar);
  protected:


// After Freezing - is saved to DeletedFrozenList 
// and this one is updated without deletion;
   ResizeVector<GridIndex> DeletedList;
   Ref<GridManipulator> grid_manip;

   int Frozen;
   ResizeVector<GridAction> actlist;
   ResizeVector<T,CpM> oldparlist;
   ResizeVector<GridIndex> DeletedFrozenList;

 };





// T - is some Grid child
template <class T>
struct GridManipulatorStd : public GridManipulator
 {

   GridManipulatorStd(T *stor):GridManipulator(),store(stor){};
   GridManipulatorStd():GridManipulator(),store(NULL){};
   GridManipulatorStd(const GridManipulatorStd &stor){store=stor.store;mask_manip=stotr.mask_manip;};
   GridManipulatorStd &operator=(const GridManipulatorStd &stor){store=stor.store;mask_manip=stotr.mask_manip;return *this;};

   virtual FieldInterface* GetStore(FieldInterface *newStor=NULL)
     {if (newStor!=NULL) store=dynamic_cast<T*>(newStor); return store;};


   virtual int AddVar(FieldInterface *NewVar){return store->AddVar(NewVar);}
   virtual void DelVar(FieldInterface *NewVar)
     {if (store) store->DelVar(NewVar);
      else {ExEnv::err()<<" Trying to delete var from store==NULL in GridManipulatorStd\n";}}
   virtual FieldInterface* GetVar(char *name){return store->GetVar(name);}
   virtual FieldInterface* GetVar(int Num){return store->GetVar(Num);}
   virtual int GetVar(FieldInterface* get2var){return store->GetVar(get2var);}
//   virtual FieldInterface* NextVar(int i); virtual int StartVar();
//   virtual DataVector<GridIndex> ConstructMask(FieldInterface *dat){return store->ConstructMask(dat);}


   virtual int save_data_state (FilterOut &so)
     {so<<KeyDesc("Grid_ptr")<<(SavableClass*)store; return 1;};
   virtual int read_data_state (FilterIn &si)
     {SavableClass* tmp;si>>StdKey>>tmp;  store=dynamic_cast<T*>(tmp);return 1;};

// if CountDeleted - return MaxInd of grid
   virtual int NumPnt (int CountDeleted=0){return store->NumPnt(CountDeleted);};
   virtual GridIndex NextPnt(GridIndex i){return store->NextPnt(i);};
   virtual GridIndex StartPnt() {return store->StartPnt();};
   virtual GridIndex StartEmptyIndex()  {return store->StartEmptyIndex();};
   virtual GridIndex NextEmptyIndex(GridIndex i){return store->NextEmptyIndex(i);};

   virtual int GridFrozen(int frozen=-1)
    { return store->GridFrozen(frozen);};
   virtual void GetPntBound(GridIndex pnt,int pntpos,GridBoundPnt &bnd)
    {store->GetPntBound(pnt,pntpos,bnd);};
 protected:
   T* store;

// Procedures Exact store type dependent
// To be defined in childs

//   virtual SpacePoint GetGridPnt(GridIndex k)=0;
//   virtual void SetGridPnt(SpacePoint newVal,GridIndex k)=0;
//   virtual DataVector<SpacePoint> GetGridMaskedPnt(DataVector<GridIndex> &mask)=0;
//   virtual void SetGridMaskedPnt(DataVector<SpacePoint> &data,DataVector<GridIndex> &mask)=0;
//   virtual DataVector<GridIndex> GetNeibous(GridIndex k)=0;
 };



struct TriangulatorInterface : public SavableClass  
 {
  TriangulatorInterface():gridptr(NULL){};
  virtual ~TriangulatorInterface(){DeleteGrid();};
  virtual FieldInterface *GetGrid(){return gridptr;};
  virtual FieldInterface *GetGrid(FieldInterface *grid){gridptr=grid;return gridptr;};
  virtual void DeleteGrid()   { delete gridptr;gridptr=NULL;};
  
  virtual FieldInterface *CreateGrid()=0;

// One point is boundary ( -1 - is a second neibour ); bnd First,Second,Pos are already formed by GetPntBound
  virtual void GetBoundary(GridBoundPnt &bnd)=0;

  virtual int save_data_state (FilterOut &so)
     {so<<KeyDesc("gridptr")<<(SavableClass*)gridptr; return 1;};
  virtual int read_data_state (FilterIn &si)
     {SavableClass* tmp;si>>StdKey>>tmp;  gridptr=dynamic_cast<FieldInterface*>(tmp);return 1;};
  FieldInterface *gridptr;
 };


















// =====================================================================
// =================         GridField<T>   =====================
// =====================================================================
#include "grid_mask_data.h"
#include "grid_mask_vector.h"
template <class T,class CpM>
MaskedData<T,CpM> GridField< T, CpM>::operator()(const GridMask &mask)
 { MaskedData<T,CpM> ret(this,mask);return ret;}
template <class T,class CpM>
MaskedVector<T,CpM> GridField< T, CpM>::operator()(const GridMaskVector &mask)
 { MaskedVector<T,CpM> ret(this,mask);return ret;}
  
template <class T,class CpM>
DataVector<T,CpM> GridField< T, CpM>::GetMaskedPnt(DataVector<GridIndex> &mask)
{
 int k,N=mask.SetDim();
 DataVector<T, CpM> ret(N);
// if (N==0) {ret=data;return ret;}
 for (k=0;k<N;k++) ret[k]=data[mask[k]];
 return ret;
};
template <class T,class CpM>
void GridField< T, CpM>::SetMaskedPnt(DataVector<T, CpM> &ret,DataVector<GridIndex> &mask)
{
 int k,N=mask.SetDim();
// if (N==0) data=ret; else 
  {for (k=0;k<N;k++) data[mask[k]]=ret[k];}
};

template <class T,class CpM>
int GridField< T, CpM>::save_data_state (FilterOut &so)
  {
   FieldInterface::save_data_state(so);
   so<<KeyDesc("DefaultValue")<<DefaultValue<<KeyDesc("data")<<data;
   return 1;
  };
template <class T,class CpM>
int GridField< T, CpM>::read_data_state (FilterIn &si)
  {
   FieldInterface::read_data_state(si);
   si>>StdKey>>DefaultValue;
   si>>StdKey>>data;
   return 1;
  };

// =====================================================================
// =================         GridField<T>   =====================
// =====================================================================

// =====================================================================
// =================           Grid<T>          =====================
// =====================================================================
template <class T,class CpM>
int Grid<T,CpM>::NumPnt (int CountDeleted)
 {
  int k,N=DeletedList.SetNumEl(),del=0;
  if (CountDeleted) return N;
  for (k=0;k<N;k++) 
   if (DeletedList[k]) del++;return N-del;
 }
template <class T,class CpM>
GridIndex Grid<T,CpM>::NextPnt(GridIndex i)
 {
  int k,N=DeletedList.SetNumEl();
  for (k=i+1;k<N;k++) 
   if (!DeletedList[k]) return k;return -1;
 }
template <class T,class CpM>
GridIndex Grid<T,CpM>::NextEmptyIndex(GridIndex i)
 {
  int k,N=DeletedList.SetNumEl();
  for (k=i+1;k<N;k++) 
   if (DeletedList[k]) return k;
  return N;
 }


template <class T,class CpM>
int Grid<T,CpM>::AddVar(FieldInterface *NewVar)
 {
  if (Frozen) 
    {ExEnv::err()<<" Error Trying to AddVar to Frozen Grid. Unfrozing first "<<endl;GridFrozen(0);}
  int k,N=VarList.SetNumEl();
  for (k=0;k<N;k++) if (VarList[k]==NULL) {VarList[k]=NewVar;return k;};
  VarList[N]=NewVar;
  return N;
 }
template <class T,class CpM>
void Grid<T,CpM>::DelVar(FieldInterface *NewVar)
 {
  if (Frozen) 
    {ExEnv::err()<<" Error Trying to DelVar from Frozen Grid. Unfrozing first "<<endl;GridFrozen(0);}
  int k=GetVar(NewVar);
  if (k!=-1) VarList[k]=NULL;
 }
template <class T,class CpM>
FieldInterface* Grid<T,CpM>::GetVar(char *name)
 {
  int k,N=VarList.SetNumEl();
  FieldInterface *cur;
  for (k=0;k<N;k++)  
   if ( ((cur=VarList[k])!=NULL) && (Stricmp(cur->SetDataName(),name)==0) )
     return cur;
  return NULL;
 }
  
template <class T,class CpM>
FieldInterface* Grid<T,CpM>::GetVar(int Num)
 {
  int N=VarList.SetNumEl();
  if ((Num>0) && (Num<N)) return VarList[Num];
  return NULL;
 }

template <class T,class CpM>
int Grid<T,CpM>::GetVar(FieldInterface* get2var)
 {
  int k,N=VarList.SetNumEl();
  for (k=0;k<N;k++)  if (VarList[k]==get2var) return k;
  return -1;
 }



template <class T,class CpM>
void Grid<T,CpM>::VarAddPnt(GridIndex i)
 {
  int k,N=VarList.SetNumEl();
  for (k=0;k<N;k++) 
   if (VarList[k]!=NULL) VarList[k]->AddPnt(i);
 };
template <class T,class CpM>
void Grid<T,CpM>::VarDelPnt(GridIndex i)
 {
  int k,N=VarList.SetNumEl();
  for (k=0;k<N;k++) 
   if (VarList[k]!=NULL) VarList[k]->DelPnt(i);
 }
template <class T,class CpM>
void Grid<T,CpM>::VarMovePnt(GridIndex From,GridIndex To)
 {
  int k,N=VarList.SetNumEl();
  for (k=0;k<N;k++) 
   if (VarList[k]!=NULL) VarList[k]->MovePnt(From,To);
 }
template <class T,class CpM>
void Grid<T,CpM>::VarModifyPnt(GridIndex i)
 {
  int k,N=VarList.SetNumEl();
  for (k=0;k<N;k++) 
   if (VarList[k]!=NULL) VarList[k]->ModifyPnt(i);
 }

template <class T,class CpM>
void Grid<T,CpM>::SetGridManip(GridManipulator *newManip)
 {
  int k,N=VarList.SetNumEl();
  for (k=0;k<N;k++) if (VarList[k]!=NULL) VarList[k]->SetStore(newManip);
  grid_manip=newManip;
 }
  

template <class T,class CpM>
int Grid<T,CpM>::save_data_state (FilterOut &so)
 {
  if (Frozen) {ExEnv::err()<<" Error Trying to save Frozen Grid. Unfrozing first "<<endl;GridFrozen(0);}
  GridField<T,CpM >::save_data_state(so);
  so<<KeyDesc("DeletedList")<<DeletedList;
  so<<KeyDesc("grid_manip")<<grid_manip;
  so<<KeyDesc("VarList")<<VarList;
  return 1;
 };
template <class T,class CpM>
int Grid<T,CpM>::read_data_state (FilterIn &si)
 {
//  if (Frozen) {ExEnv::err()<<" Error Trying to read to Frozen Grid. Unfrozing first "<<endl;GridFrozen(0);}
  Frozen=0;
  GridField<T,CpM >::read_data_state(si);
  si>>StdKey>>DeletedList>>StdKey>>grid_manip>>StdKey>>VarList;
  return 1;
 };

template <class T,class CpM>
int Grid<T,CpM>::GridFrozen(int frozen)
 {
  if (frozen==-1) return Frozen;
  if (Frozen==frozen) {ExEnv::err()<<" Not single attempt to set FrozenGrid to state "<<frozen<<endl;return Frozen;}
  if (frozen) DeletedFrozenList=DeletedList;
  else
   {
    DeletedList=DeletedFrozenList; // Reseting Deleted List with deletion
    int k,N=actlist.SetNumEl();
    for (k=0;k<N;k++) 
     {
      GridAction act=actlist[k];
      OldGridKValue=oldparlist[k];
      SetAction(act.Action,act.Index,act.Param,0);
     }
   }
  actlist.SetNumEl(0);oldparlist.SetNumEl(0); 
  Frozen=frozen;
  return Frozen;
 };
template <class T,class CpM>
void Grid<T,CpM>::VarAddAction(int act,GridIndex k,GridIndex To)
 {
  int N=actlist.SetNumEl();
  oldparlist[N]=OldGridKValue;
  actlist[N].SetAction(act,k,To);
 };

template <class T,class CpM>
void Grid<T,CpM>::SetAction(int act,GridIndex k,GridIndex To,int froze)
 {
  if (froze==-1) froze=Frozen;
  switch(act)
   {
    case (GridAction::AddPnt) :
     DeletedList[k]=0;
     if (!froze) VarAddPnt(k);else VarAddAction(act,k,To);break;
    case (GridAction::DelPnt):
     if (!froze) {DeletedList[k]=1;VarDelPnt(k);}
     else VarAddAction(act,k,To);break;
    case (GridAction::MovePnt) :
     DeletedList[To]=0;
     if (!froze) {DeletedList[k]=1;VarMovePnt(k,To);}
     else VarAddAction(act,k,To);break;
    case (GridAction::ModifyPnt) :
     if (!froze) VarModifyPnt(k);
     else VarAddAction(act,k,To);break;
    default:ExEnv::err()<<" In SetAction unknown action ! Abort."<<endl;abort();
   }
 };

template <class T,class CpM>
void Grid<T,CpM>::GetPntBound(GridIndex k,int kpos,GridBoundPnt &bnd)
 {
  GridManipulator *Center_manip=SetGridManip();
  DataVector<GridIndex> k1_ind,k_ind;
  k_ind=Center_manip->GetNeibous(k);

  GridIndex k1=k_ind[kpos];
  bnd.First=k;bnd.FirstPos=kpos;
  bnd.Second=k1;bnd.SecondPos=-1;
  if (k1==-1) {GetBoundary(bnd);return ;}

  k1_ind=Center_manip->GetNeibous(k1);
  for (int k1pos=0;k1pos<k1_ind.SetDim();k1pos++)
    if (k1_ind[k1pos]==k) {bnd.SecondPos=k1pos;goto Good;}
       ExEnv::err()<<" CouldNot Find Linked Reference to point in  Grid<T>::GetPntBound."<<endl;abort();
Good:;

  SpacePoint k_pnt,k1_pnt,delta_pnt;
  k_pnt=Center_manip->GetGridPnt(k);
  k1_pnt=Center_manip->GetGridPnt(k1);

  bnd.Surface=0;
  bnd.Center=(k1_pnt+k_pnt)*0.5;
  delta_pnt=k_pnt-k1_pnt;double norm=delta_pnt*delta_pnt;
  if (norm>MathZer) bnd.NormalDir=delta_pnt/sqrt(norm);
  else bnd.NormalDir=delta_pnt*0;

 }

// Second point is boundary ( -1 - is a neibour )
template <class T,class CpM>
void Grid<T,CpM>::GetBoundary(GridBoundPnt &bnd)
 {
  if (bnd.Second!=-1)
   { ExEnv::err()<<" Point is not boundary in  Grid<T>::GetBoundary."<<endl;abort();}

  GridManipulator *Center_manip=SetGridManip();
  bnd.Surface=0;
  bnd.Center=Center_manip->GetGridPnt(bnd.First);
  bnd.NormalDir=bnd.Center*0.+1.;
 };
  



#endif
*/

/*
template <class T,class CpM>
DataVector<GridIndex> Grid<T,CpM>::ConstructMask(FieldInterface *dat)
 {
//  int k;//=GetVar(dat);
  GridField<int> *mask_arr=dynamic_cast<GridField<int>* >(dat);
  if (mask_arr==NULL) { ExEnv::err()<<" Grid<T>::ConstructMask dat - cannot be converted to GridField<GridIndex>. Error. \n"<<endl;abort();}
  GridManipulator *grid=mask_arr->SetStore();
  int N=grid->NumPnt(),n=0;
  DataVector<GridIndex> ret(N);
  GridIndex i;//=grid->StartPnt();
  for (i=grid->StartPnt();i>=0;i=grid->NextPnt(i)) if (mask_arr->GetPnt(i)!=0) {ret[n]=i;n++;}
  ret.Resize(n);
  return ret;
 }

*/