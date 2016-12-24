#pragma once

#include "libw.h"


struct Wavelet {


    static PQMF G[17];
    static PQMF H[17];
    static QMF G1[17];
    static QMF H1[17];
    static Wavelet *Cur;
    double Factor;
    std::vector<double> ResultSignal;
    //wavelettype = 0..16 ; best 16?
    static real OracCost(
        const real *data,
        const integer &n,
        const real &sigma,
        const integer &k) {
        real cost = 0;
        real var = sigma * sigma;
        real temp;

        for(int i = 0; i < n; i++) {
            temp = data[i] * data[i];
            if(temp >= var * Cur->Factor * Cur->Factor)
                cost += var;
            else
                cost += temp;
        }
        return cost;
    }

    std::vector<double>
        &Construct(double *signal, int len, int wavelettype, double signalnoiseration) {
        //int MAXLEVEL = 15;
        //int DIM = (1<<MAXLEVEL);//dimensionality of vector ("signal")

        int DIM = len;
        int MAXLEVEL = int(log((double)len) / log(2.));
        DIM = (1 << MAXLEVEL);
        if(DIM < len) {
            MAXLEVEL++;
            DIM = (1 << MAXLEVEL);
        }
        Factor = (1.0 + sqrt(2 * log((double)MAXLEVEL * DIM)));
        Cur = this;

        real Noise_level = sqrt(1.0 / (signalnoiseration * DIM));
        //variance of the Gaussian noise added
        real variance = Noise_level * Noise_level;

        Interval NoisySignal(0, DIM - 1);
        for(int i = 0; i < len; i++)
            NoisySignal[i] = signal[i];
        for(int i = len; i < DIM; i++)
            NoisySignal[i] = signal[len - (i - len + 1)];

        ArrayTreePer W(MAXLEVEL);   //Wavelet packet coeffs for NoisySignal
        Analysis(
            NoisySignal,
            W,
            H[wavelettype],
            G[wavelettype],
            ConvDecPer);   //Wavelet packet analysis for NoisySignal

        ArrayTreePer Temp(MAXLEVEL);   //Temporary device to find BB.
        HedgePer BB;
        ExtractBestBasis(W, BB, OracCost, Noise_level);   //Find the best basis (BB)
        //at this point the BB for Noisy signal has been found
        //temp hedge for manipulations
        HedgePer BB_temp;
        BB_temp = BB;

        //extract coeff. in W relative to BB
        ExtractHedge(BB_temp, W);

        Interval Delta(0, DIM - 1);   //Delta values initialized to 0
        int i;
        for(i = 0; i < DIM; i++) {   //Threshold values a la Donoho
            real x = BB_temp.origin[i] * BB_temp.origin[i];
            if(x > variance * Factor * Factor)
                Delta[i] = 1;
        }
        for(i = 0; i < DIM; i++)
            BB_temp.origin[i] *= Delta[i];

        SuperposeHedge(BB_temp, Temp);

        //Inverse wavelet transform applied to thresholded coeff.
        Interval RecSignal(0, DIM);
        Synthesis(Temp, RecSignal, H[wavelettype], G[wavelettype], AdjConvDecPer);
        ResultSignal.resize(len);
        for(i = 0; i < len; i++)
            ResultSignal[i] = RecSignal[i];
        return ResultSignal;
    }
    double Clc(int i) {
        return ResultSignal[i];
    }
    std::vector<double> &ConstructPlus(
        double *signal,
        int len,
        int wavelettype,
        double signalnoiseration) {

        int DIM = len;
        int MAXLEVEL = int(log((double)len) / log(2.));
        DIM = (1 << MAXLEVEL);
        if(DIM < len) {
            MAXLEVEL++;
            DIM = (1 << MAXLEVEL);
        }
        Factor = (1.0 + sqrt(2 * log((double)MAXLEVEL * DIM)));
        Cur = this;

        real Noise_level = sqrt(1.0 / (signalnoiseration * DIM));
        //variance of the Gaussian noise added
        real variance = Noise_level * Noise_level;

        Interval NoisySignal(0, DIM - 1);
        for(int i = 0; i < len; i++)
            NoisySignal[i] = signal[i];
        for(int i = len; i < DIM; i++)
            NoisySignal[i] = signal[len - (i - len + 1)];

        Interval Out(0, DIM - 1);
        //WaveTrans(NoisySignal, Out, MAXLEVEL, H1[wavelettype], G1[wavelettype], ConvDecAper);
        WaveTrans(NoisySignal, Out, H[wavelettype], G[wavelettype], ConvDecPer);
        Interval Delta(0, DIM - 1);      //Delta values initialized to 0
        for(int i = 0; i < DIM; i++) {   //Threshold values a la Donoho
            real x = Out[i] * Out[i];
            if(x > variance * Factor * Factor)
                Delta[i] = 1;
        }
        for(int i = 0; i < DIM; i++)
            Out[i] *= Delta[i];
        Interval RecSignal(0, DIM);
        //Inverse wavelet transform applied to thresholded coeff.
        //InvWaveTrans(Out, RecSignal, MAXLEVEL, H1[wavelettype], G1[wavelettype], AdjConvDecAper);
        InvWaveTrans(Out, RecSignal, H[wavelettype], G[wavelettype], AdjConvDecPer);


        ResultSignal.resize(len);
        for(int i = 0; i < len; i++)
            ResultSignal[i] = RecSignal[i];
        return ResultSignal;
    }
};