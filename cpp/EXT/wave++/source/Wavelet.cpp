#include "wavelet.h"


//G[0].Set(d02doqf, 0, 1);    H[0].Set(d02soqf, 0, 1);
//G[1].Set(d04doqf, 0, 3);    H[1].Set(d04soqf, 0, 3);
//G[2].Set(d06doqf, 0, 5);    H[2].Set(d06soqf, 0, 5);
//G[3].Set(d08doqf, 0, 7);    H[3].Set(d08soqf, 0, 7);
//G[4].Set(d10doqf, 0, 9);    H[4].Set(d10soqf, 0, 9);
//G[5].Set(d12doqf, 0, 11);   H[5].Set(d12soqf, 0, 11);
//G[6].Set(d14doqf, 0, 13);   H[6].Set(d14soqf, 0, 13);
//G[7].Set(d16doqf, 0, 15);   H[7].Set(d16soqf, 0, 15);
//G[8].Set(d18doqf, 0, 17);   H[8].Set(d18soqf, 0, 17);
//G[9].Set(d20doqf, 0, 19);   H[9].Set(d20soqf, 0, 19);
//G[10].Set(c06doqf, 0, 5);   H[10].Set(c06soqf, 0, 5);
//G[11].Set(c12doqf, 0, 11);  H[11].Set(c12soqf, 0, 11);
//G[12].Set(c18doqf, 0, 17);  H[12].Set(c18soqf, 0, 17);
//G[13].Set(c24doqf, 0, 23);  H[13].Set(c24soqf, 0, 23);
//G[14].Set(c30doqf, 0, 29);  H[14].Set(c30soqf, 0, 29);
//G[15].Set(b18doqf, 0, 17);  H[15].Set(b18soqf, 0, 17);
//G[16].Set(v24doqf, 0, 23);  H[16].Set(v24soqf, 0, 23);

PQMF Wavelet::G[17] = {
    PQMF(d02doqf, 0, 1),
    PQMF(d04doqf, 0, 3),
    PQMF(d06doqf, 0, 5),
    PQMF(d08doqf, 0, 7),
    PQMF(d10doqf, 0, 9),
    PQMF(d12doqf, 0, 11),
    PQMF(d14doqf, 0, 13),
    PQMF(d16doqf, 0, 15),
    PQMF(d18doqf, 0, 17),
    PQMF(d20doqf, 0, 19),
    PQMF(c06doqf, 0, 5),
    PQMF(c12doqf, 0, 11),
    PQMF(c18doqf, 0, 17),
    PQMF(c24doqf, 0, 23),
    PQMF(c30doqf, 0, 29),
    PQMF(b18doqf, 0, 17),
    PQMF(v24doqf, 0, 23),
};
PQMF Wavelet::H[17] = {
    PQMF(d02soqf, 0, 1),
    PQMF(d04soqf, 0, 3),
    PQMF(d06soqf, 0, 5),
    PQMF(d08soqf, 0, 7),
    PQMF(d10soqf, 0, 9),
    PQMF(d12soqf, 0, 11),
    PQMF(d14soqf, 0, 13),
    PQMF(d16soqf, 0, 15),
    PQMF(d18soqf, 0, 17),
    PQMF(d20soqf, 0, 19),
    PQMF(c06soqf, 0, 5),
    PQMF(c12soqf, 0, 11),
    PQMF(c18soqf, 0, 17),
    PQMF(c24soqf, 0, 23),
    PQMF(c30soqf, 0, 29),
    PQMF(b18soqf, 0, 17),
    PQMF(v24soqf, 0, 23),
};
QMF Wavelet::G1[17] = {
    QMF(d02doqf, 0, 1),
    QMF(d04doqf, 0, 3),
    QMF(d06doqf, 0, 5),
    QMF(d08doqf, 0, 7),
    QMF(d10doqf, 0, 9),
    QMF(d12doqf, 0, 11),
    QMF(d14doqf, 0, 13),
    QMF(d16doqf, 0, 15),
    QMF(d18doqf, 0, 17),
    QMF(d20doqf, 0, 19),
    QMF(c06doqf, 0, 5),
    QMF(c12doqf, 0, 11),
    QMF(c18doqf, 0, 17),
    QMF(c24doqf, 0, 23),
    QMF(c30doqf, 0, 29),
    QMF(b18doqf, 0, 17),
    QMF(v24doqf, 0, 23),
};
QMF Wavelet::H1[17] = {
    QMF(d02soqf, 0, 1),
    QMF(d04soqf, 0, 3),
    QMF(d06soqf, 0, 5),
    QMF(d08soqf, 0, 7),
    QMF(d10soqf, 0, 9),
    QMF(d12soqf, 0, 11),
    QMF(d14soqf, 0, 13),
    QMF(d16soqf, 0, 15),
    QMF(d18soqf, 0, 17),
    QMF(d20soqf, 0, 19),
    QMF(c06soqf, 0, 5),
    QMF(c12soqf, 0, 11),
    QMF(c18soqf, 0, 17),
    QMF(c24soqf, 0, 23),
    QMF(c30soqf, 0, 29),
    QMF(b18soqf, 0, 17),
    QMF(v24soqf, 0, 23),
};

Wavelet *Wavelet::Cur = NULL;