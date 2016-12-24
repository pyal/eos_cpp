
#ifndef SparceArray_h
#define SparceArray_h 1

#include "VecRes.h"


struct SparceArrayInd : public SavableClass {
private:
    ResizeVector<int> sparce_ind;

public:
    SparceArrayInd() {
        int i = 0;
        sparce_ind.SetFill(&i);
    };
    ~SparceArrayInd(){};

    SparceArrayInd(const SparceArrayInd &right) {
        sparce_ind = right.sparce_ind;
    }
    SparceArrayInd &operator=(const SparceArrayInd &right) {
        sparce_ind = right.sparce_ind;
        return *this;
    };


    int Num(int CountDeleted = 0);
    // Add one to obtain array Len
    int MaxNum();
    inline int SetSize(int FullSizeNew) {
        return sparce_ind.SetNumEl(FullSizeNew);
    };
    inline int Add(int pos = -1) {
        if(pos == -1)
            pos = NextEmp(-1);
        sparce_ind[pos] = 1;
        return pos;
    }
    inline void Del(int pos) {
        sparce_ind[pos] = 0;
    }
    inline void Move(int frpos, int topos) {
        Add(topos);
        Del(frpos);
    };

    inline const int IsEmpty(int i) const {
        return (i >= 0) ? !sparce_ind[i] : 1;
    };
    inline int Start() {
        return -1;
    }
    int Next(int i);
    int Prev(int i);
    inline int StartEmp() {
        return -1;
    }
    int NextEmp(int i);
    int PrevEmp(int i);
    virtual ResizeVector<int> GetSparceInd() {
        return sparce_ind;
    };
    virtual int save_data_state(FilterOut &so) {
        so << KeyDesc("SparceInd") << sparce_ind;
        return 1;
    }
    virtual int read_data_state(FilterIn &si) {
        si >> StdKey >> sparce_ind;
        return 1;
    }

protected:
private:
    //   const ResizeVector<int> get_sparce_ind () const;void set_sparce_ind (ResizeVector<int> value);
};


template <class T, class CpM = CopyStructFast<T>>
struct SparceArray : public SavableClass   //## Inherits: <unnamed>%3EED8F8D01AB
{
public:
    SparceArray(){};
    virtual ~SparceArray() {
        data.Delete();
    };
    SparceArray(const SparceArray<T, CpM> &right) {
        data = right.data;
        index = right.index;
    }
    SparceArray<T, CpM> &operator=(const SparceArray<T, CpM> &right) {
        data = right.data;
        index = right.index;
        return *this;
    }

    SparceArrayInd *GetInd() {
        return &index;
    };
    void SetEmpty(T *emp) {
        data.SetFill(emp);
    }
    const T *GetEmpty() const {
        return data.GetFill();
    }

    int Shrink(int movedata = 0);
    int SetSize(int FullSizeNew) {
        data.SetNumEl(FullSizeNew);
        return index.SetSize(FullSizeNew);
    }
    inline const int IsEmpty(int i) const {
        return index.IsEmpty(i);
    }
    inline T &operator[](int i) {
        if(IsEmpty(i))
            i = Add(i);
        return data[i];
    }
    virtual inline int Add(int pos = -1) {
        pos = index.Add(pos);
        return pos;
    }
    virtual inline int Add(T &dat, int pos = -1) {
        pos = index.Add(pos);
        data[pos] = dat;
        return pos;
    }
    virtual inline void Del(int pos) {
        index.Del(pos);
        T *tmp;
        if((tmp = data.GetFill()) != NULL)
            data[pos] = *tmp;
    }
    virtual inline void Move(int frpos, int topos) {
        Add(data[frpos], topos);
        Del(frpos);
    }
    virtual int save_data_state(FilterOut &so) {
        so << KeyDesc("data") << data << KeyDesc("index") << index;
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        si >> StdKey >> data >> StdKey >> index;
        return 1;
    };


protected:
private:   //## implementation
    ResizeVector<T, CpM> data;
    SparceArrayInd index;
};


// Parameterized Class SparceArray


template <class T, class CpM>
int SparceArray<T, CpM>::Shrink(int movedata) {
    if(movedata) {
        int emp = index.NextEmp(-1), pnt = index.Prev(-1);
        while(emp < pnt) {
            Move(pnt, emp);
            emp = index.NextEmp(emp);
            pnt = index.Prev(pnt);
        }
    }
    int Nrest = index.MaxNum() + 1;
    SetSize(Nrest);
    return Nrest;
};


#endif
