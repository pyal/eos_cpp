//
// reftestx.h
//

#include "lib/ref/ref.h"
#include "lib/ref/class_sav.h"
class X : public RefCount {
private:
    int x;

public:
    static int nx;
    X();
    ~X();
};

class Y : public X {
private:
    int y;

public:
    Y();
    ~Y();
};
