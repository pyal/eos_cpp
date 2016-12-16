#include "lib/precompiled/urs_curve_lib.h"

#include "urs_opac_curve.h"


static ClassDesc URS_Curve_ClassDesc(typeid(UrsCurve_BrightTempClc),"UrsCurve_BrightTempClc",1,"URS_Curve_ClcVar_category",create<UrsCurve_BrightTempClc>);



UrsCurve_BrightTempClc::UrsCurve_BrightTempClc():
        OpacityDifCalib2Measure(1), WaveLength(500), CalibTemp(2000), CalibSignal(0.5), 
        UseFixedCalibEmissivity(1), InterFilterFormName ("InterFilterForm"),
        DetectorEmissivityName("DetectorEmissivityFile"), CalibEmissivityName("CalibEmissivityFile"), VarSignalName("File.x"){};

double UrsCurve_BrightTempClc::PlancEmisE(double Temp, double Lambda){
    double tl = Temp*Lambda*1e-9;
    return 1.19104/pow(Lambda, 5)/(exp(0.0143878/tl)-1)*1e11;// Joule/Second/nMeter^3
}

UrsCurve_BrightTempClc *UrsCurve_BrightTempClc::CurPtr;

