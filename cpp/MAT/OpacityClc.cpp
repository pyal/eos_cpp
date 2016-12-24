
#include "lib/precompiled/math.h"

#include "opacityclc.h"
#include "onevarcurves.h"

static ClassDesc BrigtnesTemp_ClassDesc(
    typeid(BrigtnesTemp),
    "BrigtnesTemp",
    1,
    "OneVarFunction_category",
    create<BrigtnesTemp>);
static ClassDesc ColorTemp_ClassDesc(
    typeid(ColorTemp),
    "ColorTemp",
    1,
    "ManyVarFunction_category",
    create<ColorTemp>);

static ClassDesc PlancEmission_ClassDesc(
    typeid(PlancEmission),
    "PlancEmission",
    1,
    "ManyVarFunction_category",
    create<PlancEmission>);


BrigtnesTemp::BrigtnesTemp()
    : L2OSourceCal(new ManyVarFunc2OneVar),
      L2OSourceExp(new PolynomFunc),
      L2ODetector(new ManyVarFunc2OneVar),
      InterFilter(new PolynomFunc),
      ColorTempCoef(1),
      WaveLength(500),
      SourceTemp(2700),
      Opacity(1),
      ZeroFilterSignal(1e-5),
      SingleLine(1),
      CalibrationSignal(0.6) {}

BrigtnesTemp *BrigtnesTemp::CurPtr;


double BrigtnesTemp::PlancEmisE(double Temp, double Lambda) {
    double tl = Temp * Lambda * 1e-9;
    double earg = 0.0143878 / tl;
    if(earg > 300)
        return 0;
    return 1.19104 / pow(Lambda, 5) / (exp(0.0143878 / tl) - 1) *
           1e11;   // Joule/Second/nanoMeter^3
}

ColorTemp *ColorTemp::CurPtr;