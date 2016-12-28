
#pragma once

#include <cpp/lib/std/EX_OUT.H>
#include "PolyRegConstructor.h"


namespace NPolygon {
    struct TPolyMarchRegionBase : SavableClass {
        virtual double GetMaxTimeStp(TPolyRegion *reg) = 0;
        virtual void MakeTimeStep(TPolyRegion *reg, double curTime, double timeStp) = 0;
        virtual void RebuildBounds(TPolyRegion *reg) = 0;
        virtual void InitBeforeBounds(TPolyRegion *reg) = 0;
        virtual void InitAfterBounds(TPolyRegion *reg) = 0;
        virtual void SetNewTimeStp(double curTime, double timeStp) = 0;
    };
    struct TPolyMarchTestZero : TPolyMarchRegionBase {
        virtual double GetMaxTimeStp(TPolyRegion *reg) {
            return 1;
        };
        virtual void MakeTimeStep(TPolyRegion *reg, double curTime, double timeStp){};
        virtual void RebuildBounds(TPolyRegion *reg){};
        virtual void InitBeforeBounds(TPolyRegion *reg){};
        virtual void InitAfterBounds(TPolyRegion *reg){};
        virtual void SetNewTimeStp(double curTime, double timeStp){};
    };

    struct TPolyMarchBody : TPolyMarchRegionBase {
        Ref<TPolyMarchRegionBase> Marcher;
        double FromTime, ToTime, MaxTimeStep;
        double OutputTime;
        Stroka ResultsFile, OutputNames;
        TPolyMarchBody();
        //int OutPntType;
        TRegionBounds OutputBounds;

        int save_data_state(FilterOut &so) {
            so << " FromTime " << FromTime << " ToTime " << ToTime << " MaxTimeStep "
               << MaxTimeStep << SavableClass::EOLN();
            so << " OutputBounds_{ ";
            OutputBounds.save_data_state(so);
            so << " } " << SavableClass::EOLN();
            so << " OutputTime " << OutputTime;
            so << " ResultsFile " << ResultsFile << " OutputNames " << OutputNames
               << SavableClass::EOLN();
            so << " MarchRegion " << Marcher;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp >> FromTime >> tmp >> ToTime >> tmp >> MaxTimeStep >> tmp;
            OutputBounds.read_data_state(si);
            SavableClass::ExeptionCheck(si, "}");
            si >> tmp >> OutputTime;
            si >> tmp >> ResultsFile >> tmp >> OutputNames;
            si >> tmp >> Marcher;
            return 1;
        }
        Stroka MakeHelp() {
            Stroka help =
                "Class is a main driver for time marching calculation. Uses marcher for time step clc.";
            help += " Marcher is of the type NPolygon::RegionMarch_category.\n";
            help += " Registered NPolygon::RegionMarch_category objects:\n\n~+\n" +
                    SavableClass::HelpForCategory("NPolygon::RegionMarch_category");
            help += "~-\n";
            return help;
        }

        virtual double GetMaxTimeStp(TPolyRegion *reg) {
            double maxStp = 1e305;
            for(TPolyRegion::TShallowIterator it = reg->ShallowStart(); it.IsOk();
                it.Next()) {
                maxStp = min(maxStp, Marcher->GetMaxTimeStp(it.CurRegion()));
            }
            return maxStp;
        };
        virtual void MakeTimeStep(TPolyRegion *reg, double curTime, double timeStp) {
            Marcher->SetNewTimeStp(curTime, timeStp);
            for(TPolyRegion::TShallowIterator it = reg->ShallowStart(); it.IsOk();
                it.Next()) {
                Marcher->MakeTimeStep(it.CurRegion(), curTime, timeStp);
            }
        };
        virtual void InitAfterBounds(TPolyRegion *reg) {
            for(TPolyRegion::TShallowIterator it = reg->ShallowStart(); it.IsOk();
                it.Next()) {
                Marcher->InitAfterBounds(it.CurRegion());
            }
        };
        virtual void RebuildBounds(TPolyRegion *reg) {
            Marcher->RebuildBounds(reg);
        }
        virtual void InitBeforeBounds(TPolyRegion *reg) {
            for(TPolyRegion::TShallowIterator it = reg->ShallowStart(); it.IsOk();
                it.Next()) {
                Marcher->InitBeforeBounds(it.CurRegion());
            }
        };
        virtual void SetNewTimeStp(double curTime, double timeStp) {}


        void SaveIter(
            fstream &outFile,
            double time,
            TPolyRegion *reg,
            const vector<Stroka> &outNames,
            const TRegionBounds &bnds) {
            log_debug(string("Time: ") + to_string(time));
            outFile << "CurTime " << time << "\n";
            PolyRegIO::SaveRegionData(outFile, reg, outNames, bnds);
        }
        void DoIt(TPolyRegion *reg) {
            Stroka timeStr;
            PrintNumIterTime Timer;
            double lastTime = 0;
            double time = FromTime;
            double outTime = time + OutputTime;
            vector<Stroka> outNames = Str::SplitLine(OutputNames, 0, ':');
            fstream outFile(~ResultsFile, ios::out);
            outFile << "Writing Vars:\n" << Str::JoinLine(outNames) << "\n";
            InitBeforeBounds(reg);
            RebuildBounds(reg);
            InitAfterBounds(reg);
            GetMaxTimeStp(reg);
            SaveIter(outFile, time, reg, outNames, OutputBounds);
            while(time < ToTime) {
                //cout << time << "I\n";
                double regStp = GetMaxTimeStp(reg);
                double tStp = min(MaxTimeStep, regStp);
                tStp = min(tStp, ToTime * (1 + 1e-15) - time);
                tStp = min(tStp, (outTime - time) * (1 + 1e-15));
                MakeTimeStep(reg, time, tStp);
                time += tStp;
                RebuildBounds(reg);
                if(time >= outTime) {
                    SaveIter(outFile, time, reg, outNames, OutputBounds);
                    outTime = time + OutputTime;
                }

                if(Timer.PrintNow(timeStr)) {
                    log_info(Stroka("CurT ") + time + " tStp " + ((time - lastTime) / Timer.getIterCircle())
                             + "\t" + timeStr);
                    lastTime = time;
                }
            }
            log_info("Done " + Timer.PrintLast());
            outFile.close();
        }
    };

};   // namespace NPolygon {