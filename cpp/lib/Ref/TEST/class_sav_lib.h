
#ifndef _class_sav_lib_h
#define _class_sav_lib_h

#include "lib/std/Util.h"
#include "lib/Ref/class.h"
#include "lib/Ref/class_sav.h"
#include "lib/std/unit_test.h"

class AA : virtual public SavableClass {
private:
public:
    int i;
    AA() : i(1){};
    ~AA() {
        cout << "AA dtor\n";
    };
    virtual void Test() {
        cout << " Test AA\n";
    };
    int save_data_state(FilterOut &so) {
        so << KeyDesc("AA_i") << i;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        si >> StdKey >> i;
        return 1;
    };
};


class BB : public AA {
private:
public:
    int ib;
    BB() : ib(2){};
    ~BB() {
        cout << "BB dtor\n";
    };
    virtual void Test() {
        cout << " Test BB\n";
    };
    int save_data_state(FilterOut &so) {
        AA::save_data_state(so);
        so << KeyDesc("BB_ib") << ib;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        AA::read_data_state(si);
        si >> StdKey >> ib;
        return 1;
    };
};


class CC : virtual public SavableClass {
private:
public:
    int i;
    CC() : i(3){};
    ~CC() {
        cout << "CC dtor\n";
    };
    virtual void Test() {
        cout << " Test CC\n";
    };
    int save_data_state(FilterOut &so) {
        so << KeyDesc("CC_i") << i;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        si >> StdKey >> i;
        return 1;
    };
};


class DD : public BB, virtual public CC {
private:
    AA *atst;
    double *d;

public:
    int id;
    DD() : atst(new AA), id(4) {
        d = new double[5000];
    };

    ~DD() {
        delete atst;
        cout << "DD dtor\n";
        delete d;
    };
    virtual void Test() {
        cout << " Test DD\n";
    };
    int save_data_state(FilterOut &so) {
        BB::save_data_state(so);
        CC::save_data_state(so);
        so << KeyDesc("DD_id") << id;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        BB::read_data_state(si);
        CC::read_data_state(si);
        si >> StdKey >> id;
        return 1;
    };
};

#endif