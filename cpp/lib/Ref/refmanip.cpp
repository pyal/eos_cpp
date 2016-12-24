#include <lib/precompiled/Ref.h>


#include "refmanip.h"


FilterOut &operator<<(FilterOut &out, const Stroka &str) {
    out << ~str;
    return out;
}
FilterIn &operator>>(FilterIn &in, Stroka &str) {
    str.read_token(in);
    return in;
}

namespace RefManip {};   //namespace RefManip {
