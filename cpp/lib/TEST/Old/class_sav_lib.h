
#ifndef _class_sav_lib_h
#define _class_sav_lib_h
#include <iostream.h>

#include "lib/ref/exenv.h"
#include "lib/ref/class.h"
#include "lib/ref/class_sav.h"
#include <iomanip.h>

#include <stdiostr.h>

#include <malloc.h>

class A : virtual public SavableClass {
private:
public:
    int i;
    A() : i(1){};
    ~A() {
        cout << "A dtor\n";
    };
    virtual void Test() {
        cout << " Test A\n";
    };
    int save_data_state(FilterOut &so) {
        so << KeyDesc("A_i") << i;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        si >> StdKey >> i;
        return 1;
    };
};


class B : public A {
private:
public:
    int ib;
    B() : ib(2){};
    ~B() {
        cout << "B dtor\n";
    };
    virtual void Test() {
        cout << " Test B\n";
    };
    int save_data_state(FilterOut &so) {
        A::save_data_state(so);
        so << KeyDesc("B_ib") << ib;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        A::read_data_state(si);
        si >> StdKey >> ib;
        return 1;
    };
};


class C : virtual public SavableClass {
private:
public:
    int i;
    C() : i(3){};
    ~C() {
        cout << "C dtor\n";
    };
    virtual void Test() {
        cout << " Test C\n";
    };
    int save_data_state(FilterOut &so) {
        so << KeyDesc("C_i") << i;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        si >> StdKey >> i;
        return 1;
    };
};


class D : public B, virtual public C {
private:
    A *atst;
    double *d;

public:
    int id;
    D() : id(4), atst(new A) {
        d = new double[5000];
    };

    ~D() {
        delete atst;
        cout << "D dtor\n";
        delete d;
    };
    virtual void Test() {
        cout << " Test D\n";
    };
    int save_data_state(FilterOut &so) {
        B::save_data_state(so);
        C::save_data_state(so);
        so << KeyDesc("D_id") << id;
        return 1;
    };
    int read_data_state(FilterIn &si) {
        B::read_data_state(si);
        C::read_data_state(si);
        si >> StdKey >> id;
        return 1;
    };
};

#endif