#ifndef test_vec_h
#define test_vec_h

#include "lib/data/DataVec.h"
#include "lib/ref/class_sav.h"
#include "lib/std/util.h"

struct TestY;
struct TestX : SavableClass {
    TestX(TestY *ptr = NULL);
    ~TestX();
    Ref<TestY> y;
    int save_data_state(FilterOut &so);
    int read_data_state(FilterIn &si);
};

struct TestY : SavableClass {
    TestY(TestX *ptr = NULL);
    ~TestY();
    TestX *x;
    int save_data_state(FilterOut &so);
    int read_data_state(FilterIn &si);
};

#endif
