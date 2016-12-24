
#ifndef RegGrid_h
#define RegGrid_h 1

#include "GridMask.h"
#include "RegBoundary.h"
#include "GridData.h"

namespace NPolygon {


    struct TRegGrid : SavableClass {
    public:
        //enum PointType {Bound, Center, AllData};
    private:   //## implementation
        TGridData VarsOnGrid;

        //int GridBoundarySize;

        struct RegionMaskStorage {
            //vector<Ref<TGridMaskBase> > Masks;
            RegionMaskStorage() : MaxLevel(0){};
            map<TRegionBounds, Ref<TGridMaskBase>> MaskMap;
            Ref<TGridMaskBase> BaseBody, FullBody;
            int MaxLevel;
            void MakeMasks(Ref<TGridMaskBase> baseMask, int level) {
                FullBody << baseMask->Duplicate();
                FullBody->Normalize();
                BaseBody = FullBody->ExpandGrid(TRegionBounds(-level, level));
                MaxLevel = level;
                MaskMap.clear();
                //    Masks.resize(2*level + 1);
                //    for(size_t i = 0; i < 2 * size_t(level) + 1; i++) {
                //        Masks[i] = BaseBody->Shift(i - level);
                //    }
            }
            void CheckBnds(const TRegionBounds &bnds) {
                int lft = bnds.GetLft(), rgt = bnds.GetRgt();
                if((!bnds.NoLft() && lft + MaxLevel < 0) ||
                   (!bnds.NoRgt() && MaxLevel - rgt < 0))
                    throw info_except(
                        "Impossible mask for the grid!!! MaxLevel <%i>, Bounds <%i,%i>\n",
                        MaxLevel,
                        lft,
                        rgt);
            }
            Ref<TGridMaskBase> GetMask(const TRegionBounds &bnds) {
                if(MaskMap.find(bnds) == MaskMap.end()) {
                    CheckBnds(bnds);
                    MaskMap[bnds] = FullBody->ShrinkGrid(
                        TRegionBounds(bnds.LftAdd(MaxLevel), bnds.RgtAdd(-MaxLevel)));
                }
                return MaskMap[bnds];
            }
            int save_data_state(FilterOut &so) {
                so << " FullBody " << FullBody << " BaseBody " << BaseBody
                   << SavableClass::EOLN() << " MaxLevel " << MaxLevel
                   << SavableClass::EOLN();
                return 1;
            };
            int read_data_state(FilterIn &si) {
                Stroka tmp;
                si >> tmp >> FullBody >> tmp >> BaseBody >> tmp >> MaxLevel;
                return 1;
            };
        };
        RegionMaskStorage RegionMasks;
        //vector<RegionMasks> TypedMasks;
    public:
        TRegGrid() : SavableClass() {
            RegionMasks.MakeMasks(new TGridMaskSimple, 0);
            //TypedMasks[0].MakeMasks(new TGridMaskSimple, GridBoundarySize);
            //TypedMasks[1].MakeMasks(new TGridMaskSimple, GridBoundarySize);
        };
        //TRegGrid(const TRegGrid &right);
        //~TRegGrid();
        //TRegGrid & operator=(const TRegGrid &right);
        //int operator==(const TRegGrid &right) const;
        //int operator!=(const TRegGrid &right) const;

        //int RegSize(PointType pointType, int newSize = -1) {
        //    if (newSize != -1) {
        //        if (pointType == Center)
        //            newSize++;
        //        Base[Bound]->SetDim(newSize);
        //        Base[Center]->SetDim(newSize - 1);
        //    }
        //    return Base[pointType]->size();
        //}

        inline TGridVariablesBase *GetVar(const Stroka &name) {
            return VarsOnGrid.GetVar(name);
        }
        inline TGridMaskedData GetMaskedData(
            const TRegionBounds &bnds,
            const Stroka &name) {
            return TGridMaskedData(GetVar(name), GetMask(bnds));
        }
        inline TGridMaskedData GetMaskedData(
            const TRegionBounds &bnds,
            TGridVariablesBase *var) {
            return TGridMaskedData(var, GetMask(bnds));
        }
        inline TGridMaskBase *GetMask(const TRegionBounds &bnds) {
            return RegionMasks.GetMask(bnds);
            //if (pntType == AllData)
            //    return TypedMasks[Bound].FullBody;
            //return TypedMasks[pntType].Masks[level + GridBoundarySize];
        }
        inline int GetBoundarySize() const {
            return RegionMasks.MaxLevel;
        }
        //void GenerateTypedMasks(TGridMaskBase *baseBody_, int gridBoundarySize) {
        //    Ref<TGridMaskBase> baseBody = baseBody_;
        //    RegionMasks.MakeMasks(baseBody, gridBoundarySize);
        //    //GridBoundarySize = gridBoundarySize;
        //    //TypedMasks[0].MakeMasks(baseBody, GridBoundarySize);
        //    //Ref<TGridMaskBase> centerBody = baseBody->Shift(-1);
        //    //centerBody->Cut(baseBody);
        //    //TypedMasks[1].MakeMasks(centerBody, GridBoundarySize);
        //}
        //Data is copied...
        void SetGridBoundarySize(int gridBoundarySize) {
            Ref<TGridMaskBase> wasMask;
            wasMask << RegionMasks.BaseBody->Duplicate();
            RegionMasks.MakeMasks(wasMask, gridBoundarySize);

            //GenerateTypedMasks(TypedMasks[0].BaseBody, level);
            VarsOnGrid.Resize(
                RegionMasks.FullBody->MaxPointInd(), RegionMasks.BaseBody, wasMask);
        }
        //Data is not copied...
        void SetBaseMask(Ref<TGridMaskBase> baseMask) {
            RegionMasks.MakeMasks(baseMask, RegionMasks.MaxLevel);
            //GenerateTypedMasks(baseMask, RegionMasks.MaxLevel);
            VarsOnGrid.Resize(RegionMasks.FullBody->MaxPointInd(), NULL, NULL);
        };

        inline void AddVar(
            const Stroka &name,
            TGridVariablesBase *var,
            const TRegionBounds &bnds,
            Ref<TGridMaskBase> curMask) {
            VarsOnGrid.AddVar(name, var, GetMask(bnds), curMask);
        }
        inline void AddVar(const Stroka &name, TGridVariablesBase *var) {
            VarsOnGrid.AddVar(name, var, NULL, NULL);
        }
        inline int VarExists(const Stroka &name) const {
            return VarsOnGrid.VarExists(name);
        }
        //inline TGridMaskedData GetVar (const Stroka &name, PointType varType, int level) {
        //    return TGridMaskedData(VarsOnGrid.GetVar(name), GetMask(varType, level));
        //}
        inline vector<Stroka> GetVarNames() {
            return VarsOnGrid.GetVarNames();
        }

        int save_data_state(FilterOut &so) {
            //so<<" Bounds "<<Bounds<<SavableClass::EOLN();
            so << " RegionMasks ";
            RegionMasks.save_data_state(so);
            //so<<" CenterMasks ";TypedMasks[1].save_data_state(so);
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            //si>>tmp>>Bounds;
            si >> tmp;
            RegionMasks.read_data_state(si);
            //si>>tmp;TypedMasks[1].read_data_state(si);
            //GridBoundarySize = (TypedMasks[0].Masks.size() - 1) / 2;
            //GridBoundarySize = GridBoundarySize>0?GridBoundarySize:0;
            return 1;
        };
        Stroka MakeHelp() {
            return "Grid...";
        }
    };


};   //namespace NPolygon {

#endif
