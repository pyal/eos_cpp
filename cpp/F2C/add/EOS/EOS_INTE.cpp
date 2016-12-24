#include "eos_inte.h"

int EOSCurNum;
EOSstruct EOSArr[MaxNumEos];

int EOSReadStruct(EOSstruct *eos, FILE *hand) {
    char name[256];
    fscanf(hand, "%s", &name);

    if(Stricmp(name, "MatterABu") == 0) {
        eos->Pressure = PressureABu;
        eos->Sound = SoundABu;
        eos->Temperature = TemperatureABu;
        eos->ReadIni = ReadABu;
        eos->DeleteIni = DeleteABu;
        strcpy(eos->name, "MatterABu");
    }

    else if(Stricmp(name, "MatterGas") == 0) {
        eos->Pressure = PressureGas;
        eos->Sound = SoundGas;
        eos->Temperature = TemperatureGas;
        eos->ReadIni = ReadGas;
        eos->DeleteIni = DeleteGas;
        strcpy(eos->name, "MatterGas");
    }

    else if(Stricmp(name, "MatterSpl") == 0) {
        eos->Pressure = PressureSpl;
        eos->Sound = SoundSpl;
        eos->Temperature = TemperatureSpl;
        eos->ReadIni = ReadSpl;
        eos->DeleteIni = DeleteSpl;
        strcpy(eos->name, "MatterSpl");
    }

    else
        return 0; /*{ cout<<" EOSReadStruct; There is no such function \n"<<name<<"\n";return 0; }*/
    return eos->ReadIni(hand, &(eos->IntPar));
};

int EOSRead(int N, FILE *hand) {
    for(int k = 1; k <= N; k++)
        if(!EOSReadStruct(&EOSArr[k], hand)) {
            for(int k1 = 1; k1 < k; k1++) {
                EOSArr[k1].DeleteIni(&EOSArr[k1].IntPar);
            }
            /*delete EOSArr[k1].name;*/
            return 0;
        }
    EOSCurNum = k;
    return 1;
};
void EOSDelete() {
    for(int k1 = 1; k1 <= EOSCurNum; k1++) {
        EOSArr[k1].DeleteIni(&EOSArr[k1].IntPar);
    };
};
/*delete EOSArr[k1].name;*/


double EOSPressure(double D, double E, int N) {
    if((N > EOSCurNum) || (N < 1))
        return 0;
    return EOSArr[N].Pressure(D, E, EOSArr[N].IntPar);
}
double EOSTemperature(double D, double E, int N) {
    if((N > EOSCurNum) || (N < 1))
        return 0;
    return EOSArr[N].Temperature(D, E, EOSArr[N].IntPar);
}
double EOSSound(double D, double E, int N) {
    if((N > EOSCurNum) || (N < 1))
        return 0;
    return EOSArr[N].Sound(D, E, EOSArr[N].IntPar);
}


/*
//=================================================================
//=============  Test  ============================================
//=================================================================
*/
void main(int argc, char *argv[]) {
    FILE *in = fopen(argv[1], "r");
    int NumMatter, N;
    fscanf(in, "%i", &NumMatter);
    if(!EOSRead(NumMatter, in))
        exit(0);
    double D, E, P, T, S;
    printf("Density   Energy  Pressure   Temperature   Sound  Number \n");
    while(!feof(in)) {
        fscanf(in, "%lf %lf %i", &D, &E, &N);
        P = EOSPressure(D, E, N);
        S = EOSSound(D, E, N);
        T = EOSTemperature(D, E, N);
        printf("%lf %lf %lf %lf %lf %i \n", D, E, P, T, S, N);
    };
};
