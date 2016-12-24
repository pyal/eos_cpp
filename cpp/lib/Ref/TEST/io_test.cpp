

#include "lib/std/unit_test.h"
#include "lib/Ref/class_sav.h"

namespace io_test {

    string test_str = string(R"(
Heloo 1e-008 sdf)") + '\n' +
                      '\r' + R"(
234 eeeeeeeee 234524)" +
                      '\f' + '\t' + R"(
dfg	2.455533	ddddddddd)" +
                      '\n' + '\r' + R"(
345e-5)" + '\n' + '\r' +
                      R"(

")" + '\n' + '\r' + R"( dfgsd	678
    )";
    using p = pair<string, double>;
    void IoTest() {
        memstreambuf read_buf((char *)test_str.c_str(), test_str.size());
        FilterTextIn in(&read_buf, false);
        vector<pair<string, double>> test = {p{"Heloo", 1e-8},
                                             p{"sdf", 234},
                                             p{"eeeeeeeee", 234524},
                                             p{"dfg", 2.455533},
                                             p{"ddddddddd", 345e-5}};
        for(auto kv : test) {
            char tmp[256];
            double d;
            in >> tmp >> d;
            assert(tmp == kv.first);
            assert(d == kv.second);
        }
    }
    UnitTest unit_RefCountTest("io_test", IoTest);
}
