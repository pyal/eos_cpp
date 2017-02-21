
#ifndef PolyRegion_h
#define PolyRegion_h 1


#include "GridMask.h"
#include "RegBoundary.h"
#include "RegGrid.h"

namespace NPolygon {


    struct TPolyRegion : SavableClass {

    public:
        map<Stroka, Ref<SavableClass>> MapSavableVar;
        map<Stroka, double> MapDouble;
        TRegGrid Grid;


        TPolyRegion();   // : SavableClass() {};

        double GetConstantDouble(const Stroka &name) {
            TPolyRegion *base = GetRootRegion();
            map<Stroka, double>::iterator it = base->MapDouble.find(name);
            if(it == base->MapDouble.end())
                throw info_except("Could not find name <%s>\n", ~name);
            return it->second;
        }
        int ConstantDoubleExists(const Stroka &name) {
            TPolyRegion *base = GetRootRegion();
            map<Stroka, double>::iterator it = base->MapDouble.find(name);
            if(it == base->MapDouble.end())
                return 0;
            return 1;
        }
        SavableClass *GetConstantVariable(const Stroka &name) {
            TPolyRegion *base = GetRootRegion();
            map<Stroka, Ref<SavableClass>>::iterator it = base->MapSavableVar.find(name);
            if(it == base->MapSavableVar.end())
                throw info_except("Could not find name <%s>\n", ~name);
            return it->second;
        }
        int ConstantVariableExists(const Stroka &name) {
            TPolyRegion *base = GetRootRegion();
            map<Stroka, Ref<SavableClass>>::iterator it = base->MapSavableVar.find(name);
            if(it == base->MapSavableVar.end())
                return 0;
            return 1;
        }
        void SetConstantDouble(const Stroka &name, double val) {
            GetRootRegion()->MapDouble[name] = val;
        }
        void SetConstantVariable(const Stroka &name, SavableClass *val) {
            GetRootRegion()->MapSavableVar[name] = val;
        }

        double * GetDataPtr(const TRegionBounds &bnds, const Stroka &varName) {
            return (double *)Grid.GetMaskedData(bnds, varName).Start().GetElementPtr();
        };
        int GetDataSize(const TRegionBounds &bnds, const Stroka &varName) {
            return Grid.GetMaskedData(bnds, varName).Start().Size();
        };
        //Vau!!! Very bad - regions can have arbitrary neighbours, have to store them, put info to the boundary...
        void FillChildBounds(TRegBoundaryBase *bnd, const Stroka &varName) {
            TShallowIterator prevRegion = ShallowEnd(), nextRegion = ShallowStart(),
                             curRegion;
            for(; prevRegion.IsOk() || nextRegion.IsOk();
                prevRegion = nextRegion, nextRegion.Next()) {
                TPolyRegion *regPrev = NULL, *regNext = NULL;
                if(prevRegion.IsOk())
                    regPrev = prevRegion.CurRegion();
                if(nextRegion.IsOk())
                    regNext = nextRegion.CurRegion();
                if(prevRegion.IsOk())
                    bnd->FillBoundMinus(varName, regPrev, regNext);
                if(nextRegion.IsOk())
                    bnd->FillBoundPlus(varName, regPrev, regNext);
            }
        }

        int save_data_state(FilterOut &so) {
            so << " Grid ";
            Grid.save_data_state(so);
            so << " Vars { ";
            for(map<Stroka, Ref<SavableClass>>::iterator it = MapSavableVar.begin();
                it != MapSavableVar.end();
                it++)
                so << it->first << "\n"
                   << SavableClass::object2string(it->second) << "\n";
            so << " } ";
            so << " Childs " << Childs << " Parent " << Parent;
            return 1;
        };
        int read_data_state(FilterIn &si) {
            Stroka tmp;
            si >> tmp;
            Grid.read_data_state(si);
            si >> tmp >> tmp;
            MapSavableVar.clear();
            while(!(!si) && !SavableClass::TestNextChar(si, '}')) {
                Stroka name;
                si >> name;
                MapSavableVar[name] = SavableClass::Read(si);
            }
            si >> tmp;
            operator>>(si, Childs) >> tmp;
            Parent = SavableClass::TestType<TPolyRegion>(si.getobject());
            return 1;
        };
        Stroka MakeHelp() {
            return "Polygon...";
        }

        struct TShallowIterator {
            friend struct TPolyRegion;

        private:
            Ref<TPolyRegion> Base;
            list<Ref<TPolyRegion>>::iterator Iter;
        protected:
            inline void Last(TPolyRegion *base) {
                Base = base;
                if(Base) {
                    Iter = Base->Childs.end();
                    Iter--;
                }
            }

        public:
            TShallowIterator(){};
            TShallowIterator(TPolyRegion *base) {
                Start(base);
            };
            TShallowIterator(const TShallowIterator &iter)
                : Base(iter.Base), Iter(iter.Iter){};
            TShallowIterator &operator=(const TShallowIterator &it) {
                Base = it.Base;
                Iter = it.Iter;
                return *this;
            }
            inline void End() {
                if(Base)
                    Iter = Base->Childs.end();
            }
            inline void Start(TPolyRegion *base) {
                Base = base;
                if(Base)
                    Iter = Base->Childs.begin();
            }
            inline int IsOk() {
                return (!(!Base) && Iter != Base->Childs.end());
            }
            inline int Next() {
                if(!IsOk())
                    return 0;
                Iter++;
                return IsOk();
            }
            inline int Prev() {
                if(!Base)
                    return 0;
                if(Iter == Base->Childs.begin()) {
                    Iter = Base->Childs.end();
                    return 0;
                }
                Iter--;
                return IsOk();
            }
            inline TPolyRegion *GetNext() {
                TShallowIterator it(*this);
                if(!it.Next())
                    return NULL;
                return it.CurRegion();
            }
            inline TPolyRegion *GetPrev() {
                TShallowIterator it(*this);
                if(!it.Prev())
                    return NULL;
                return it.CurRegion();
            }

            inline TPolyRegion *CurRegion() {
                return *Iter;
            }
        };


        TPolyRegion *GetRootRegion() {
            TPolyRegion *p = this;
            while(p->Parent != NULL)
                p = p->Parent;
            return p;
        }
        inline TShallowIterator ShallowStart() {
            return TShallowIterator(this);
        }
        inline TShallowIterator ShallowLast() {
            TShallowIterator it;
            it.Last(this);
            return it;
        }
        inline TShallowIterator ShallowEnd() {
            TShallowIterator it;
            it.End();
            return it;
        }

        TShallowIterator AddChild(Ref<TPolyRegion> reg) {
            reg->Parent = this;
            Childs.push_back(reg);
            TShallowIterator it;
            it.Last(this);
            return it;
        }
        void ClearChilds() {
            Childs.clear();
        }
        TPolyRegion *ParentRegion() {
            return Parent;
        }
        void SetGridBoundarySize(int level) {
            for(TShallowIterator it = ShallowStart(); it.IsOk(); it.Next())
                it.CurRegion()->SetGridBoundarySize(level);
            Grid.SetGridBoundarySize(level);
        }
        TPolyRegion *GetNextRegion() {
            TPolyRegion *head = this->Parent;
            verify(head, ~("There is no parent region: " + Str::Obj2Str(this)));
            TPolyRegion::TShallowIterator it = head->ShallowStart();
            for(;it.IsOk(); it.Next()) {
                if (it.CurRegion() == this) break;
            }
            if (!it.IsOk()) return nullptr;
            return it.GetNext();
        }
        TPolyRegion *GetPreviousRegion() {
            TPolyRegion *head = this->Parent;
            verify(head, ~("There is no parent region: " + Str::Obj2Str(this)));
            TPolyRegion *prev = nullptr;
            TPolyRegion::TShallowIterator it = head->ShallowStart();
            for(;it.IsOk(); it.Next()) {
                if (it.CurRegion() == this) break;
                prev = it.CurRegion();
            }
            verify(it.IsOk(), ~("Region was not found in the list of parent childs.\nRegion: " + Str::Obj2Str(this)));
            return it.GetNext();
        }

        list<Ref<TPolyRegion>> Childs;
        TPolyRegion *Parent;
    };


};   //namespace NPolygon {

#endif

