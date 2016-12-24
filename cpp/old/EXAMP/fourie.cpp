
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
//#include <const.h>

#define FMT "    "

double *SetArray(int NumPnt) {
    double *tmp = new double[NumPnt + 1];
    tmp[0] = NumPnt;
}
void GenerateXY(double *&x, double *&y, double from, double upto, int NumPnt) {
    x = SetArray(NumPnt);
    y = SetArray(NumPnt);
    double Stp = (upto - from) / (NumPnt - 1), Cur = from;
    for(int k = 1; k <= NumPnt; k++) {
        x[k] = Cur;
        y[k] = log(Cur);
        Cur += Stp;
    }
};
void GenerateCoef(
    double *x,
    double *y,
    double *&SinCoef,
    double *&CosCoef,
    double &BaseVal) {
    int NumPnt = x[0];
    SinCoef = SetArray(NumPnt / 2);
    CosCoef = SetArray(NumPnt / 2);
    //  double MainHarm=2*M_PI/(x[NumPnt]-x[1]);
    double MainHarm = 2 * M_PI / NumPnt;
    for(int j = 1; j <= NumPnt / 2; j++) {
        CosCoef[j] *= 0;
        SinCoef[j] *= 0;
        for(int k = 1; k <= NumPnt; k++) {
            CosCoef[j] += y[k] * cos((j - 1) * MainHarm * k);
            SinCoef[j] += y[k] * sin((j - 1) * MainHarm * k);
        }
        CosCoef[j] *= 2 / NumPnt;
        SinCoef[j] *= 2 / NumPnt;
    }
    CosCoef[NumPnt / 2] *= 0.5;
    BaseVal = 0;
    for(int k = 1; k <= NumPnt; k++)
        BaseVal += y[k];
};


int main(int argc, char *argv[]) {
    fstream in(argv[1], ios::in);
    fstream out(argv[2], ios::out);
    double from, upto;
    int NumPnt;
    char tmp[256];
    in >> tmp >> from >> tmp >> upto >> NumPnt;
    double *SinCoef, *CosCoef, *x, *y, BaseVal, Restore;
    GenerateXY(x, y, from, upto, NumPnt);
    GenerateCoef(x, y, SinCoef, CosCoef);

    for(int k = 1; k <= SinCoef[0]; k++)
        out << x[k] << FMT << SinCoef[k] << FMT << CosCoef[k] << "\n";

    delete x;
    delete y;
    delete CosCoef;
    delete SinCoef;
    return EXIT_SUCCESS;
}
