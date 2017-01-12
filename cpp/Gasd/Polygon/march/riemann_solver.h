#pragma once

#include "CenterFlux.h"

namespace NPolygon {


    enum GasdField {
        Pressure,
        Energy,
        Position,
        Density,
        Velocity,
        Size
    };
    extern vector<string> GasdFieldNames;
    struct RiemannSolutionBase : SavableClass {
        virtual void Solve(vector<double*> &fields, double time) = 0;
        //TODO fixit
        virtual double GetMaxClcTime() {return 1;}
        static pair<RiemannSolutionBase*, RiemannSolutionBase*> BuildRiemanSolution(
                const vector<double*> &reg1, const vector<double*> &reg2,
                                                        MatterIO *mat1, MatterIO *mat2){
            return pair<RiemannSolutionBase*, RiemannSolutionBase*>(NULL, NULL);
        };
    };
    struct RiemannIsentrope: RiemannSolutionBase {
        MatterIO * Mat;
        vector<double> MinData;
        double MinSound, MaxSound;
        double TraverceShiftIntegral;
        vector<vector<double>> InitialParams;
        void Solve(vector<double*> &fields, double time) {
            double maxPos = MaxSound * time;
            double minPos = MinSound * time;
            for(size_t i = 0; i < InitialParams[0].size(); i++) {
                if (InitialParams[Position][i] > maxPos) {
                    SetOriginal(fields, time, i);
                } else {
                    if (InitialParams[Position][i] < minPos) {
                        SetMinimal(fields, time, i);
                    } else {
                        SetIsentrope(fields, time, i);
                    }
                }
            }

        }
        void SetOriginal(vector<double*> &fields, double time, int i) {
            for(size_t j = 0; j < Size; j++) {
                fields[j][i + 1] = InitialParams[j][i];
            }
            fields[Position][i + 1] += InitialParams[Velocity][i] * time;
        }
        void SetMinimal(vector<double*> &fields, double time, int i) {
            for(size_t j = 0; j < Size; j++) {
                fields[j][i + 1] = MinData[j];
            }
            double origPos = InitialParams[Position][i];
            double origTime = origPos / MaxSound;
            double minTime = origPos / MinSound;
            double shiftPos = TraverceShiftIntegral * (minTime - origTime);
            fields[Position][i + 1] = origPos + shiftPos +
                                      InitialParams[Velocity][i] * origTime + MinData[Velocity] * (time - minTime);
        }
        void SetIsentrope(vector<double*> &fields, double time, int i) {
            double origPos = InitialParams[Position][i];
            double origTime = origPos / MaxSound;
//            double isentTime = time - origTime;
            double isentSound = origPos / time;
            double traverceShift;
            vector<double> isentParams = FindIsentParams(Mat, InitialParams, isentSound, traverceShift);
            for(size_t j = 0; j < Size; j++) {
                fields[j][i + 1] = isentParams[j];
            }
            double shiftPos = traverceShift * (time - origTime);
            fields[Position][i + 1] = origPos + shiftPos +
                                      InitialParams[Velocity][i] * origTime;
        }
        vector<double> FindIsentParams(MatterIO *mat, const vector<vector<double>> &InitialParams, double isentSound, double &traverceShift) {

            return InitialParams[0];
        }
    };



    struct RiemannHugoniot: RiemannSolutionBase {
        vector<double> HugData;
        double ShockVel;
        vector<vector<double>> InitialParams;
        RiemannHugoniot(const vector<double> &hugData, double shockVel, const vector<vector<double>> &initialParams):
            HugData(hugData), ShockVel(shockVel), InitialParams(initialParams){}
        void Solve(vector<double*> &fields, double time) {
            double hugPos = ShockVel * time;
            for(size_t i = 0; i < InitialParams[0].size(); i++) {
                if (InitialParams[Position][i] > hugPos) {
                    SetOriginal(fields, time, i);
                } else {
                    SetShock(fields, time, i);
                }
            }
        }
        void SetOriginal(vector<double*> &fields, double time, int i) {
            for(size_t j = 0; j < Size; j++) {
                fields[j][i + 1] = InitialParams[j][i];
            }
            fields[Position][i + 1] += InitialParams[Velocity][i] * time;
        }
        void SetShock(vector<double*> &fields, double time, int i) {
            for(size_t j = 0; j < Size; j++) {
                fields[j][i + 1] = HugData[j];
            }
            double origPos = InitialParams[Position][i];
            double origVel = InitialParams[Velocity][i];
            double origTime = origPos / ShockVel;
            fields[Position][i + 1] = origPos +  origVel * origTime + origVel * (time - origTime);
        }
    };
    struct TMarchExactRieman : TPolyMarchRegionBase {
        double StartTime = 0;

        virtual double GetMaxTimeStp(TPolyRegion *child, double curTime) {
            RiemannSolutionBase *sol = SavableClass::TestType<RiemannSolutionBase>(child->MapSavableVar["RiemannSolver"]);
            return sol->GetMaxClcTime();
        }
        virtual void MakeTimeStep(TPolyRegion *reg, double curTime, double timeStp) {
            RiemannSolutionBase *sol = SavableClass::TestType<RiemannSolutionBase>(reg->MapSavableVar["RiemannSolver"]);
            auto fields = GetFields(reg);
            sol->Solve(fields, curTime);
        }
        virtual void InitBase(TPolyRegion *baseNode, double startTime) {
            StartTime = startTime;
            TPolyRegion *fst = baseNode->ShallowStart().CurRegion() , *sec = baseNode->ShallowStart().GetNext();
            vector<double*> fstdata = GetFields(fst);
            vector<double*> secdata = GetFields(sec);
            MatterIO *fstmat = SavableClass::TestType<MatterIO>(fst->MapSavableVar["EOS"]);
            MatterIO *secmat = SavableClass::TestType<MatterIO>(sec->MapSavableVar["EOS"]);

            pair<RiemannSolutionBase*, RiemannSolutionBase*> solvers = RiemannSolutionBase::BuildRiemanSolution(fstdata, secdata,
                                                                                         fstmat, secmat);
            fst->MapSavableVar["RiemannSolver"] = solvers.first;
            sec->MapSavableVar["RiemannSolver"] = solvers.second;
        }
        virtual void RebuildBoundsBase(TPolyRegion *baseNode) {
        }

    private:
        //TODO
        vector<double*> GetFields(TPolyRegion *reg) {
            //{"Pressure", "Energy", "Position", "Density", "Velocity"};
            TRegionBounds bnd(0, 0);
            vector<double*> ret;
            ret.push_back((double*)reg->Grid.GetMaskedData(bnd, "Pres").Start().GetElementPtr());
            ret.push_back((double*)reg->Grid.GetMaskedData(bnd, "Pos").Start().GetElementPtr());
            ret.push_back((double*)reg->Grid.GetMaskedData(bnd, "Dens").Start().GetElementPtr());
            ret.push_back((double*)reg->Grid.GetMaskedData(bnd, "Vel").Start().GetElementPtr());
            return ret;
        }
    public:


        int save_data_state(FilterOut &so) {
            return 1;
        };
        int read_data_state(FilterIn &si) {
            return 1;
        }
        Stroka MakeHelp();

    private:

    };
}