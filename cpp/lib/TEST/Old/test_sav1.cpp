
#include "lib/data/DataVec.h"
//#include "IndBase.h"
#include "lib/data/DataArr.h"

#include "lib/data/DataTyp.h"
#include "lib/data/VecRes.h"

#define StorageIndex int


struct CenterStorePnt : public SavableClass {
public:
    CenterStorePnt() : IndNum(0), Coord(0), Neibous(0), Volume(0){};
    CenterStorePnt(const SpacePoint &Spnt, int Npnt = 0)
        : IndNum(Npnt), Coord(Spnt), Volume(0), Neibous(0){};
    CenterStorePnt(const CenterStorePnt &pnt) {
        IndNum = pnt.IndNum;
        Coord = pnt.Coord;
        Volume = pnt.Volume;
        Neibous = pnt.Neibous;
    }
    CenterStorePnt &operator=(const CenterStorePnt &pnt) {
        IndNum = pnt.IndNum;
        Coord = pnt.Coord;
        Volume = pnt.Volume;
        Neibous = pnt.Neibous;
        return *this;
    }
    virtual int save_data_state(FilterOut &so) {
        so << KeyDesc("IndNum") << IndNum << KeyDesc("Coord") << Coord
           << KeyDesc("Volume") << Volume << KeyDesc("Neibous") << Neibous;
        return 1;
    }
    virtual int read_data_state(FilterIn &si) {
        si >> StdKey >> IndNum >> StdKey >> Coord >> StdKey >> Volume >> StdKey >>
            Neibous;
        return 1;
    }

    //  protected://  private:

    StorageIndex IndNum;
    SpacePoint Coord;
    double Volume;
    DataVector<Ref<CenterStorePnt>> Neibous;

    CenterStorePnt &Set(const SpacePoint &Spnt, int ind, double vol) {
        Coord = Spnt;
        Volume = vol;
        IndNum = ind;
        return *this;
    }
    CenterStorePnt &Set(DataVector<Ref<CenterStorePnt>> &neib) {
        Neibous = neib;
        return *this;
    }
};


struct Storage : public SavableClass {
    virtual int save_data_state(FilterOut &so) {
        so << KeyDesc("data") << data;
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        si >> StdKey >> data;
        return 1;
    };
    ResizeVector<CenterStorePnt, CopyStructSlow<CenterStorePnt>> data;
    //   ResizeVector<CenterStorePnt > data;
};

static ClassDesc CenterStorePnt_cd(typeid(CenterStorePnt), "CenterStorePnt");
static ClassDesc Storage_cd(typeid(Storage), "Storage");

void CreateStorage(Storage &stor, SpaceBound &bound, IndBound &index) {
    CenterStorePnt Cent;
    int k, k1, N = index.NumPnt(), N0 = index.Low.SetDim();
    IndexPoint dim = index.Size(), ind, tmp_ind;
    SpacePoint add = bound.Low, mul = bound.Size(), pnt(N0);
    for(k = 0; k < N0; k++)
        mul[k] /= dim[k];
    DataVector<StorageIndex> neib(2 * N0);
    double vol = mul[0];
    for(k = 1; k < N0; k++)
        vol *= mul[k];
    for(k = 0; k < N; k++) {
        ind = index.Int2Pnt(k);
        //fcout<<"\nind"<<ind;
        for(k1 = 0; k1 < N0; k1++)
            pnt[k1] = add[k1] + (ind[k1] - index.Low[k1]) * mul[k1];
        for(k1 = 0; k1 < N0; k1++) {
            tmp_ind = ind;
            tmp_ind[k1] = ind[k1] - 1;
            neib[2 * k1] = index.Pnt2Int(tmp_ind);
            tmp_ind[k1] = ind[k1] + 1;
            neib[2 * k1 + 1] = index.Pnt2Int(tmp_ind);
        }

        //fcout<<"\nPnt"<<pnt<<"k"<<k<<"neib"<<neib<<"\n";
        stor.data[k] = Cent.Set(pnt, k, vol);
        //     stor.data[k]=Cent.Set(pnt,k,vol,neib);
        //fcout<<stor.data[k]<<"\n";
    }
};


//typedef
void TestStorage() {
    Storage stor;
    SpacePoint low(2), up(2);
    low = 0.;
    up = 1.;
    IndexPoint lowi(2), upi(2);
    lowi = (int)0;
    upi = (int)30;
    SpaceBound bound(low, up);
    IndBound index(lowi, upi);
    //fcout<<"Bound\n"<<bound<<"\nindex\n"<<index;
    CreateStorage(stor, bound, index);
    fcout << stor;
}

void test_sav() {
    //fcout<<" Test";fcout.flush();
    //  TestVector();
    //  IndRegBase reg;
    //  TestArray();
    TestStorage();
};
