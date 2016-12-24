


#pragma once


#include "PolyRegion.h"

#include "lib/ref/refmanip.h"
#include "lib/ref/str_manip.h"
#include "GridVarImpl.h"
#include "GridVectorVarImpl.h"
#include "GridMaskImpl.h"

namespace NPolygon {
    struct TSimpleContructor : SavableClass {
    public:
        struct TRegData : SavableClass {
            struct TGridVar {
                Stroka Name;
                TGridVar(){};
                TGridVar(const char *name, double minval, double maxval)
                    : Name(name), MinVal(minval), MaxVal(maxval){};
                double MinVal, MaxVal;
                friend FilterIn &operator>>(FilterIn &si, TGridVar &dat) {
                    si >> dat.Name >> dat.MinVal >> dat.MaxVal;
                    return si;
                }
                friend FilterOut &operator<<(FilterOut &so, const TGridVar &dat) {
                    so << " " << dat.Name << " " << dat.MinVal << " " << dat.MaxVal
                       << " ";
                    return so;
                }
            };
            int RegionGridSize;
            list<Ref<SavableClass>> RegVars;
            list<Stroka> RegVarNames, RegDoubleNames;
            list<double> RegDoubles;
            list<TGridVar> GridVars;

            TRegData() : SavableClass(), RegionGridSize(0) {}
            void Clear() {
                RegVars.clear();
                RegVarNames.clear();
                GridVars.clear();
                RegDoubleNames.clear();
                RegDoubles.clear();
            }
            TRegData(const char *str, SavableClass *ptr, int dim, const TGridVar &var)
                : SavableClass(), RegionGridSize(dim) {
                RegVarNames.push_back(str);
                RegVars.push_back(ptr);
                GridVars.push_back(var);
            }
            void SetData(Ref<TPolyRegion> reg) {
                list<Ref<SavableClass>>::iterator itVars = RegVars.begin();
                list<Stroka>::iterator itNames = RegVarNames.begin();
                if(RegVarNames.size() != RegVars.size())
                    throw info_except(
                        "RegVarNames.size<%i> != RegVars.size<%i>",
                        RegVarNames.size(),
                        RegVars.size());
                for(; itVars != RegVars.end(); itVars++, itNames++) {
                    reg->MapSavableVar[*itNames] = *itVars;
                }
                list<double>::iterator itDoubs = RegDoubles.begin();
                list<Stroka>::iterator itDoubNames = RegDoubleNames.begin();
                if(RegDoubles.size() != RegDoubleNames.size())
                    throw info_except(
                        "RegDoubles.size<%i> != RegDoubleNames.size<%i>",
                        RegDoubles.size() != RegDoubleNames.size());
                for(; itDoubs != RegDoubles.end(); itDoubs++, itDoubNames++)
                    reg->MapDouble[*itDoubNames] = *itDoubs;

                Ref<TGridMaskBase> mask = new TGridMaskSimple(RegionGridSize);
                reg->Grid.SetBaseMask(mask);
                for(list<TGridVar>::iterator it = GridVars.begin(); it != GridVars.end();
                    it++) {
                    reg->Grid.AddVar(
                        it->Name,
                        new NPolygon::TGridVar<double>(DataManip::MakeStepVector(
                            RegionGridSize, it->MinVal, it->MaxVal, 0, 0)),
                        TRegionBounds(0, 0),
                        mask);
                }
            }
            int save_data_state(FilterOut &so) {
                so << " RegionGridSize " << RegionGridSize << SavableClass::EOLN()
                   << " RegVars " << RegVars << SavableClass::EOLN() << " RegVarNames "
                   << RegVarNames << SavableClass::EOLN() << " RegDoubles " << RegDoubles
                   << SavableClass::EOLN() << " RegDoubleNames " << RegDoubleNames
                   << SavableClass::EOLN() << " GridVars(NameMinValMaxVal) " << GridVars;
                return 1;
            };
            int read_data_state(FilterIn &si) {
                Stroka tmp;
                si >> tmp >> RegionGridSize;
                si >> tmp >> RegVars;
                si >> tmp >> RegVarNames;
                si >> tmp >> RegDoubles;
                si >> tmp >> RegDoubleNames;
                si >> tmp >> GridVars;
                return 1;
            }
            Stroka MakeHelp() {
                return "Stores object and its name";
            }
        };
        list<Ref<TRegData>> Childs;
        Ref<TRegData> BaseRegion;
        //list<Stroka> GridNames;
        //Ref<TRegBoundaryBase> Bound;
        Stroka DataFile;
        int RegionBoundarySize;

        TSimpleContructor();   // : SavableClass() {};
        void Clear() {
            Childs.clear();
            //GridNames.clear();
            BaseRegion = NULL;
            //Bound = NULL;
        }
        Ref<TPolyRegion> MakeRegion() {
            Ref<TPolyRegion> ret = new TPolyRegion;
            BaseRegion->SetData(ret);
            for(list<Ref<TRegData>>::iterator it = Childs.begin(); it != Childs.end();
                it++) {
                Ref<TPolyRegion> child = new TPolyRegion;
                (*it)->SetData(child);
                ret->AddChild(child);
            }
            ////////ret->MakeChildBounds(Bound);
            ret->SetGridBoundarySize(RegionBoundarySize);
            return ret;
        }
        //void GetRegionStruct(TPolyRegion *){};
        //void SaveRegionData(ostream &out, TPolyRegion *region, Stroka &varNames) {
        //    vector<Stroka> vecStr = Str::SplitLine(varNames, 0);
        //    if (vecStr.size() == 0)
        //        return;
        //    vector<TGridMaskedData> vecDat(vecStr.size());
        //    for (TPolyRegion::Iterator iter = region->StartRegion(); iter != region->EndRegion(); iter = region->NextRegion(iter)) {
        //        out<<varNames<<"\n";
        //        for(size_t i = 0; i < vecStr.size(); i++) {
        //            vecDat[i] = (*iter.it)->Grid.GetMaskedData(TRegGrid::Bound, 0, vecStr[i]);
        //        }
        //        if (vecDat.size() == 0)
        //            return;
        //        TGridMaskBase::Iterator pntInd;
        //        for(int curPnt = pntInd.Start(vecDat[0].Mask); curPnt != pntInd.End(); curPnt = pntInd.Next()) {
        //            for(size_t i = 0; i < vecDat.size(); i++) {
        //                out<<*((double*)vecDat[i].GetElementPtr(curPnt))<<" ";
        //            }
        //            out<<"\n";
        //        }
        //    }
        //}


        int save_data_state(FilterOut &so) {
            //so<<" GridNames "<<GridNames<<SavableClass::EOLN();
            //so<<" Boundary "<<Bound<<SavableClass::EOLN();
            so << " BaseRegion " << BaseRegion << SavableClass::EOLN();
            so << " Childs " << Childs << SavableClass::EOLN();
            so << " DataFile " << DataFile << " RegionBoundarySize "
               << RegionBoundarySize;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Clear();
            char tmp[1000];
            //si>>tmp>>GridNames;
            //si>>tmp>>Bound;
            si >> tmp >> BaseRegion;
            si >> tmp >> Childs;
            si >> tmp >> DataFile >> tmp >> RegionBoundarySize;
            return 1;
        };
        Stroka MakeHelp() {
            return "Class for generation of calculation regions...";
        }
    };

    struct PolyRegIO {
        static TGridVariablesBase *GetVectorElement(
            const Stroka &name,
            TPolyRegion *reg) {
            size_t lftParent = (size_t)(-1), rgtParent = -1;
            if((lftParent = name.find("[")) == (size_t)(-1) ||
               (rgtParent = name.find("]")) == (size_t)(-1))
                return NULL;
            Stroka n(name, 0, lftParent);
            int i = atoi(~Stroka(name, lftParent + 1, rgtParent));
            TGridVectorVar<double> *vec = SavableClass::TestType<TGridVectorVar<double>>(
                reg->Grid.GetVar(n), "Write only vectors of double\n");
            return (*vec)[i];
        }
        static void SaveRegionData(
            ostream &out,
            Ref<TPolyRegion> region,
            const vector<Stroka> &vecStr,
            const TRegionBounds &bnds) {
            //vector<Stroka> vecStr = Str::SplitLine(varNames, 0);
            if(vecStr.size() == 0)
                return;
            vector<TGridMaskedData::TIterator> vecDat(vecStr.size());
            for(TPolyRegion::TShallowIterator iter = region->ShallowStart(); iter.IsOk();
                iter.Next()) {
                for(size_t i = 0; i < vecStr.size(); i++) {
                    TPolyRegion *reg = iter.CurRegion();
                    TGridVariablesBase *var;
                    if((var = GetVectorElement(vecStr[i], reg)) != NULL)
                        vecDat[i] = reg->Grid.GetMaskedData(bnds, var).Start();
                    else
                        vecDat[i] = reg->Grid.GetMaskedData(bnds, vecStr[i]).Start();
                }
                if(vecDat.size() == 0)
                    return;
                char buf[256];
                while(vecDat[0].IsOk()) {
                    for(size_t i = 0; i < vecDat.size(); i++) {
                        sprintf(buf, "%8.5g ", *((double *)vecDat[i].GetElementPtr()));
                        out << buf;
                        //out<<*((double*)vecDat[i].GetElementPtr())<<"\t";
                        vecDat[i].Next();
                    }
                    out << "\n";
                }
            }
        }
    };
};   //namespace NPolygon {
