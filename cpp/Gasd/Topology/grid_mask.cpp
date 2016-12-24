
#include "Grid_Mask.h"
#include "Grid_Interface.h"
#include "grid_array.h"


//MaskManipulatorGeneral
DescribedClass *create_MaskManipulatorGeneral_cd() {
    return new MaskManipulatorGeneral;
}
static ClassDesc MaskManipulatorGeneral_cd(
    typeid(MaskManipulatorGeneral),
    "MaskManipulatorGeneral",
    1,
    "Grid",
    &create_MaskManipulatorGeneral_cd);
//MaskManipulatorStruct
DescribedClass *create_MaskManipulatorStruct_cd() {
    return new MaskManipulatorStruct;
}
static ClassDesc MaskManipulatorStruct_cd(
    typeid(MaskManipulatorStruct),
    "MaskManipulatorStruct",
    1,
    "Grid",
    &create_MaskManipulatorStruct_cd);
// ===========================================================================
// =========================     MaskManipulatorStruct     =================================
// ===========================================================================

void MaskManipulatorStruct::Init() {
    AreaStoreInterface *ar = dynamic_cast<AreaStoreInterface *>(get_StoredBy());
    if(ar == NULL) {
        fcout << "Very bad. MaskManipulatorStruct::Init Area - is NULL\n";
        abort();
    };
    GridManipulator *grid = ar->GetGridManip();
    if(grid == NULL) {
        fcout << "Very bad. MaskManipulatorStruct::Init - grid is NULL\n";
        abort();
    };
    GridMask mask;
    if(ar->GetMaskGen() != NULL)
        mask = ar->GetMaskGen()->MaskAll();
    SetNewGrid(grid, mask);
};

void MaskManipulatorStruct::SetNewGrid(GridManipulator *grid, GridMask &mask) {
    if(Grid != NULL) {
        Center->unmanage();
        delete Center;
        Neibous->unmanage();
        delete Neibous;
    }
    Grid = grid;
    MaskMain = mask;
    Dim = Grid->GetGridPnt(MaskMain[0]).SetDim();
    Center = new GridVectorField<double>(Dim, Grid, "GridVarCenter");
    Neibous =
        new GridVectorField<int, CopyStructSlow<int>>(2 * Dim, Grid, "GridVarNeibous");
    int N = MaskMain.SetDim();
    for(int k = 0; k < N; k++) {
        GridIndex i = MaskMain[k];
        Center->SetPnt(Grid->GetGridPnt(i), i);
        Neibous->SetPnt(Grid->GetNeibous(i), i);
    }
    Cvt = SetCvt(MaskMain);
}
DataVector<int> MaskManipulatorStruct::SetCvt(GridMask &CvtTo) {
    DataVector<int> ret(Grid->NumPnt(1) + 1);
    ret = -1;
    for(int k = 0; k < CvtTo.SetDim(); k++)
        ret[CvtTo[k]] = k;
    return ret;
};
//  NCvt=Grid->NumPnt(1)+1;
//  Cvt.SetDim(NCvt);Cvt=-1;
//  for (int k=0;k<MaskMain.SetDim();k++) Cvt[MaskMain[k]]=k;
// };

void MaskManipulatorStruct::SetGridVal(GridVectorField<double> *pos) {
    GetCenter(pos);
    for(int k = 0; k < MaskMain.SetDim(); k++) {
        GridIndex i = MaskMain[k];
        Grid->SetGridPnt(Center->GetPnt(i), i);
    }
}
GridVectorField<double> *MaskManipulatorStruct::GetCenter(GridVectorField<double> *pos) {
    if(pos != NULL)
        if(pos->SetDim() != Dim) {
            fcout << " Error in MaskManipulatorStruct::GetCenter.";
            fcout.flush();
            abort();
        } else
            *Center = *pos;
    return Center;
};


GridMask MaskManipulatorStruct::Shift(const GridMask &mask, int Axis, int Level) {
    GridMask ret = mask;
    int N = ret.SetDim(), sh = 2 * Axis;
    if(Level > 0)
        sh += 1;
    GridIndex i_new, i_old;
    for(int k1 = 0; k1 < N; k1++) {
        for(int k = 0; k < abs(Level); k++) {
            if(((i_old = ret[k1]) >= 0) && ((i_new = Cvt[i_old]) >= 0))
                ret[k1] = Neibous->GetPnt(i_new, sh);
            else {
                ret[k1] = -1;
                break;
            }
        }
    }
    return ret;
}
GridMaskVector MaskManipulatorStruct::Shift(const GridMask &mask, int Level) {
    GridMaskVector ret(Dim);
    for(int k = 0; k < Dim; k++)
        ret[k] = Shift(mask, k, Level);
    return ret;
}

GridMaskVector MaskManipulatorStruct::Shift(const GridMaskVector &mask, int Level) {
    int D = mask.SetDim();
    if(D > Dim) {
        fcout << " Very bad in GridMaskVector MaskManipulatorStruct::Shift.\n";
        fcout.flush();
        abort();
    }
    GridMaskVector ret(D);
    for(int k = 0; k < D; k++)
        ret[k] = Shift(mask[k], k, Level);
    return ret;
}

// ===========================================================================
// =======================  End   MaskManipulatorStruct      =================
// ===========================================================================


// ===========================================================================
// =======================   MaskManipulatorGeneral   =========================
// ===========================================================================

void MaskManipulatorGeneral::Init() {
    AreaStoreInterface *ar = dynamic_cast<AreaStoreInterface *>(get_StoredBy());
    if(ar == NULL) {
        fcout << "Very bad. MaskManipulatorGeneral::Init Area - is NULL\n";
        abort();
    };
    grid = ar->GetGridManip();
    if(grid == NULL) {
        fcout << "Very bad. MaskManipulatorGeneral::Init - grid is NULL\n";
        abort();
    };
};

GridMask MaskManipulatorGeneral::MaskBoundary(int level) {
    GridMask ret, neib;
    if(level < 0)
        return ret;
    GridIndex i;
    int n = 0;
    if(level == 0) {
        n = 0;
        ret.SetDim(grid->NumPnt());
        for(i = grid->StartPnt(); i >= 0; i = grid->NextPnt(i)) {
            neib = grid->GetNeibous(i);
            for(int k1 = 0; k1 < neib.SetDim(); k1++)
                if(neib[k1] < 0) {
                    ret[n] = i;
                    n++;
                    break;
                }
        }
        ret.Resize(n);
        return ret;
    }
    GridMask cur = MaskBoundary(0);
    ret.SetDim(grid->NumPnt());
    int N = cur.SetDim();
    ;
    for(int or = 1; or <= level; or ++) {
        n = 0;
        for(int k = 0; k < N; k++) {
            neib = grid->GetNeibous(cur[k]);
            if(IsIndexNew(i = cur[k], ret, n)) {
                ret[n] = i;
                n++;
            }
            for(int k1 = 0; k1 < neib.SetDim(); k1++)
                if(((i = neib[k1]) >= 0) && (IsIndexNew(i, ret, n))) {
                    ret[n] = i;
                    n++;
                }
        }
        cur = ret;
        N = n;
    }
    ret.Resize(n);
    //fcout<<ret<<"\n";
    return ret;
};

GridMask MaskManipulatorGeneral::MaskAll() {
    GridMask ret(grid->NumPnt());
    GridIndex i;
    int k = 0;
    for(i = grid->StartPnt(); i >= 0; i = grid->NextPnt(i)) {
        ret[k] = i;
        k++;
    }
    return ret;
}
GridMask MaskManipulatorGeneral::ShiftMask(GridMask &mask, int dir, int level, int sum) {
    GridMask neib, ret;
    GridIndex i;
    if(level <= 0)
        return mask;
    if(level == 1) {
        int N = mask.SetDim(), n = 0;
        ret.SetDim(grid->NumPnt());
        for(int k = 0; k < N; k++) {
            i = mask[k];
            if(i < 0) {
                ret[n] = i;
                n++;
                continue;
            }
            if((sum) && (IsIndexNew(i, ret, n))) {
                ret[n] = i;
                n++;
            }
            neib = grid->GetNeibous(i);
            if(neib.SetDim() > dir) {
                if(IsIndexNew(neib[dir], ret, n)) {
                    ret[n] = neib[dir];
                    n++;
                }
            } else if(!sum) {
                ret[n] = -1;
                n++;
            }
        }
        ret.Resize(n);
        return ret;
    }
    GridMask cur = ShiftMask(mask, dir, level - 1, sum);
    //fcout<<"\n Shift mask "<<mask<<" lev "<<level-1<<"\nRes "<<cur;
    //  cur=ShiftMask(cur,dir,1,sum);
    //fcout<<"\n Shift lev "<<level<<"\nRes "<<cur;
    //  return cur;//ShiftMask(cur,dir,1,sum);
    return ShiftMask(cur, dir, 1, sum);
}
GridMask MaskManipulatorGeneral::AddMask(GridMask &one, GridMask &two) {
    int k, N2 = two.SetDim(), N1 = one.SetDim(), n = N1;
    GridMask ret = one;
    ret.Resize(grid->NumPnt());
    for(k = 0; k < N2; k++)
        if(IsIndexNew(two[k], one, N1)) {
            ret[n] = two[k];
            n++;
        }
    ret.Resize(n);
    return ret;
}
GridMask MaskManipulatorGeneral::SumMask(GridMask &one, GridMask &two) {
    int N2 = two.SetDim(), N1 = one.SetDim();
    GridMask ret = one;
    ret.Resize(N1 + N2);
    ret.SetData((GridIndex *)two, N1);
    return ret;
}
GridMask MaskManipulatorGeneral::RemoveMask(GridMask &was, GridMask &toremov) {
    int k, N2 = toremov.SetDim(), N1 = was.SetDim(), n = 0;
    GridMask ret(grid->NumPnt());
    for(k = 0; k < N1; k++)
        if(IsIndexNew(was[k], toremov, N2)) {
            ret[n] = was[k];
            n++;
        }
    ret.Resize(n);
    return ret;
}
//  GridMask OrderMask(GridMask &mask);

int MaskManipulatorGeneral::save_data_state(FilterOut &so) {
    so << KeyDesc("Grid_ptr") << grid;
    return 1;
};
int MaskManipulatorGeneral::read_data_state(FilterIn &si) {
    si >> StdKey >> grid;
    return 1;
};

int MaskManipulatorGeneral::IsIndexNew(GridIndex &i, GridMask &arr, int Len) {
    if(i < 0)
        return 1;
    int k2;
    for(k2 = 0; k2 < Len; k2++)
        if(arr[k2] == i) {
            k2 = -10;
            break;
        }
    return !(k2 == -10);
}


// ===========================================================================
// ======================  MaskManipulatorGeneral   =================================
// ===========================================================================


//inline GridMask operator+(const GridMask &m,const GridMaskShift &i)
// {return i.shift->Shift(m,i.Axis,i.Level); }
//inline GridMaskVector operator+(const GridMask &m,const GridMaskShift &i)
// { return i.shift->Shift(m,i.Level); }
