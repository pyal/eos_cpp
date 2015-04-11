
#pragma once

#include "PolyRegConstructor.h"


namespace NPolygon {
    struct TPolyMarchRegionBase : SavableClass {
        virtual double GetMaxTimeStp(TPolyRegion *reg) = 0;
        virtual void MakeTimeStep(TPolyRegion *reg, double timeStp) = 0;
        virtual void RebuildBounds(TPolyRegion *reg) = 0;

    };
    struct TPolyMarchRegionTest : TPolyMarchRegionBase {
        virtual double GetMaxTimeStp(TPolyRegion *reg) {
            return 1;
        };
        virtual void MakeTimeStep(TPolyRegion *reg, double timeStp) {};
        virtual void RebuildBounds(TPolyRegion *reg) {};
    };

    struct TPolyMarchBody : TPolyMarchRegionBase {
        Ref<TPolyMarchRegionBase> Marcher;
        double FromTime, ToTime, MaxTimeStep;
        Stroka ResultsFile, OutputNames;
        TPolyMarchBody();

        int save_data_state( FilterOut&so) {
            so<<" FromTime "<<FromTime<<" ToTime "<<ToTime<<" MaxTimeStep "<<MaxTimeStep<<" ResultsFile "<<ResultsFile<<" OutputNames "<< OutputNames;SavableClass::EOLN();
            so<<" MarchRegion "<<Marcher;
            return 1; 
        };
        int read_data_state(FilterIn&si) { 
            Stroka tmp;
            si>>tmp>>FromTime>>tmp>>ToTime>>tmp>>MaxTimeStep>>tmp>>ResultsFile>>tmp>>OutputNames;
            si>>tmp>>Marcher;
            return 1;
        }

        virtual double GetMaxTimeStp(TPolyRegion *reg) {
            double maxStp = 1e305;
            for(TPolyRegion::TShallowIterator it = reg->ShallowStart(); it.IsOk(); it.Next()) {
                maxStp = min(maxStp, Marcher->GetMaxTimeStp(it.CurRegion()));
            }
            return maxStp;
        };
        virtual void MakeTimeStep(TPolyRegion *reg, double timeStp) {
            for(TPolyRegion::TShallowIterator it = reg->ShallowStart(); it.IsOk(); it.Next()) {
                Marcher->MakeTimeStep(it.CurRegion(), timeStp);
            }
        };
        virtual void RebuildBounds(TPolyRegion *reg) {
            for(TPolyRegion::TShallowIterator it = reg->ShallowStart(); it.IsOk(); it.Next()) {
                Marcher->RebuildBounds(it.CurRegion());
            }
        };


        void SaveIter(fstream &outFile, double time, TPolyRegion *reg, const vector<Stroka> &outNames) {
            outFile<<"CurTime "<<time<<"\n";
            PolyRegIO::SaveRegionData(outFile, reg, outNames);
        }
        void DoIt(TPolyRegion *reg) {
            double time = FromTime;
            vector<Stroka> outNames = Str::SplitLine(OutputNames, 0, ':');
            fstream outFile(~ResultsFile, ios::out);
            outFile<<"Writing Vars:\n"<<Str::JoinLine(outNames)<<"\n";
            SaveIter(outFile, time, reg, outNames);
            while(time < ToTime) {
                RebuildBounds(reg);
                double tStp = min(MaxTimeStep, GetMaxTimeStp(reg));
                tStp = min(tStp, ToTime * (1 + 1e-15) - time);
                MakeTimeStep(reg, tStp);
                time += tStp;
                SaveIter(outFile, time, reg, outNames);
            }
            outFile.close();
        }


    };


}; // namespace NPolygon {