#include <lib/precompiled/eos.h>
#include "matt_caloric_spl.h"

static ClassDesc MatterCaloricSpl_ClassDesc(
    typeid(MatterCaloricSpl),
    "MatterCaloricSpl",
    1,
    "Matter_category",
    create<MatterCaloricSpl>);


MatterCaloricSpl *MatterCaloricSpl::Current;


void MatterCaloricSpl::Pressure(double *p, double *dencity, double *energy, int num) {
    PresSpl->Evaluate(energy, dencity, p, num);
};
void MatterCaloricSpl::Temperature(double *t, double *dencity, double *energy, int num) {
    if(!TempSpl) {
        memccpy(energy, t, num, sizeof(double));
        for(int k = 0; k < num; k++)
            t[k] = max<double>(StndErr, t[k]);
        return;
    }
    TempSpl->Evaluate(energy, dencity, t, num);
};
