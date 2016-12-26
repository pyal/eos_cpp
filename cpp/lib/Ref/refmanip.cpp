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

FilterIn &operator>>(FilterIn &in, const TVerify &wrd) {
    Stroka str;
    str.read_token(in);
    verify(str == wrd.Wrd, string("Got: ") + ~str + " HadToBe " + ~wrd.Wrd + " In " + ~wrd.Message);
    log_debug(string("Verify OK:") + ~wrd.Wrd);
    return in;
}

namespace RefManip {};   //namespace RefManip {
