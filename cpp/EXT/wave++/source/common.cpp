#include "common.h"
#include <fstream>
#include <assert.h>
#include <iostream>

integer Log2(integer r) {
    integer temp = 0;
    r >>= 1;
    while(r > 0) {
        r >>= 1;
        temp++;
    }
    return temp;
}
//////////////////////////////////////////////////////////////////////////////
int is_pow_of_2(integer r) {
    return (((r >> Log2(r)) << Log2(r)) == r);
}
///////////////////////////////////////////////////////////////////////////////
integer NumOfData(const char *filename) {
    std::ifstream glug(filename);
    if(!glug) {
        std::cout << "Can't open input file " << filename << ". Exiting." << std::endl;
        exit(1);
    }
    real x;
    integer total = 0;
    while(!glug.eof()) {
        glug >> x;
        total++;
    }
    return total;
}

///////////////////////////////////////////////////////////////////////////////
