//#pragma once

#include "lib/precompiled/ref.h"
#include "data_manip.h"




namespace DataManip {

    void *StaticSortArray = NULL;

    std::vector<double> MakeStepVector(int Num,double low,double up,int logscale, int centered){
        int N=Num ;
        double mi = logscale?log(low):low;
        double ma = logscale?log(up):up;
        double stp=ma-mi,start=mi;
        if (centered) { 
            stp/=N;
            start+=0.5*stp; 
        }
        else stp /= max(N-1,1);
        vector<double> vec(N);
        for (size_t k=0; k< (size_t)N; k++) {
            vec[k] = logscale?exp(start):start;
            start += stp;
        }
        return vec;
    }

} // namespace KcClassif {
