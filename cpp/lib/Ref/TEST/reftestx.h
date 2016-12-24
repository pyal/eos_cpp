//
// reftestx.h
//

#include "lib/Ref/ref.h"
#include "lib/std/unit_test.h"
class X : public RefCount {
  private:
    int x;
  public:
    static int nx;
    X();
    int get(){return x;}
    ~X();
};

class Y: public X {
  private:
    int y;
  public:
    Y();
    int get(){return y;}
    ~Y();
};

