//
// reftestx.h
//

#include "ref.h"

class X : public RefCount {
  private:
    int x;
  public:
    static int nx;
    X();
    ~X();
};

class Y: public X {
  private:
    int y;
  public:
    Y();
    ~Y();
};


