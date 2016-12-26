
#ifndef RegGrid_h
#define RegGrid_h 1

#include "GridMask.h"
#include "RegBoundary.h"
#include "GridData.h"

namespace NPolygon {


    struct TRegGrid : SavableClass {
    public:
    private:   //## implementation
        TGridData VarsOnGrid;

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
                si >> Verify("FullBody") >> FullBody >> Verify("BaseBody") >> BaseBody >> Verify("MaxLevel") >> MaxLevel;
                return 1;
            };
        };
        RegionMaskStorage RegionMasks;
    public:
        TRegGrid() : SavableClass() {
            RegionMasks.MakeMasks(new TGridMaskSimple, 0);
        }
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
        }
        inline int GetBoundarySize() const {
            return RegionMasks.MaxLevel;
        }
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
        inline vector<Stroka> GetVarNames() {
            return VarsOnGrid.GetVarNames();
        }

        int save_data_state(FilterOut &so) {
            so << " RegionMasks ";
            RegionMasks.save_data_state(so);
            return 1;
        };
        int read_data_state(FilterIn &si) {
            si >> Verify("RegionMasks");
            RegionMasks.read_data_state(si);
            return 1;
        };
        Stroka MakeHelp() {
            return "Grid...";
        }
    };


};   //namespace NPolygon {

#endif
