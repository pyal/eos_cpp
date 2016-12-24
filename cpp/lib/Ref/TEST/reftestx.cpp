//
// reftestx.cc
//

//#ifdef __GNUG__
//#pragma implementation "reftmpl.h"
//#pragma implementation "reftestx.h"
//#endif

#include "reftestx.h"

//#ifdef __GNUG__
//typedef Ref<X> forced_implementation_of_RefX;
//#endif

int X::nx = 0;

X::X() : x(0) {
    nx++;
}
X::~X() {
    nx--;
}

Y::Y() : y(1) {}
Y::~Y() {}
