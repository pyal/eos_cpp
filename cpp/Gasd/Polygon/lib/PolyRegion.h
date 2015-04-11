
#ifndef PolyRegion_h
#define PolyRegion_h 1


#include "GridMask.h"
#include "RegBoundary.h"
#include "RegGrid.h"

namespace NPolygon {


    struct TPolyRegion : SavableClass {

    public:
        map<Stroka, Ref<SavableClass> > MapSavableVar;
        map<Stroka, double> MapDouble;
        TRegGrid Grid;



        TPolyRegion();// : SavableClass() {};

        double GetConstantDouble(const Stroka &name) {
            TPolyRegion *base = GetRootRegion();
            map<Stroka, double>::iterator it = base->MapDouble.find(name);
            if (it == base->MapDouble.end())
                throw info_except("Could not find name <%s>\n", ~name);
            return it->second;
        }
        int ConstantDoubleExists(const Stroka &name) {
            TPolyRegion *base = GetRootRegion();
            map<Stroka, double>::iterator it = base->MapDouble.find(name);
            if (it == base->MapDouble.end())
                return 0;
            return 1;
        }
        SavableClass *GetConstantVariable(const Stroka &name) {
            TPolyRegion *base = GetRootRegion();
            map<Stroka, Ref<SavableClass> >::iterator it = base->MapSavableVar.find(name);
            if (it == base->MapSavableVar.end())
                throw info_except("Could not find name <%s>\n", ~name);
            return it->second;
        }
        int ConstantVariableExists(const Stroka &name) {
            TPolyRegion *base = GetRootRegion();
            map<Stroka, Ref<SavableClass> >::iterator it = base->MapSavableVar.find(name);
            if (it == base->MapSavableVar.end())
                return 0;
            return 1;
        }
        void SetConstantDouble(const Stroka &name, double val) {
            GetRootRegion()->MapDouble[name] = val;
        }
        void SetConstantVariable(const Stroka &name, SavableClass *val) {
            GetRootRegion()->MapSavableVar[name] = val;
        }

//Vau!!! Very bad - regions can have arbitrary neighbours, have to store them, put info to the boundary...
        void FillChildBounds (TRegBoundaryBase *bnd, const Stroka &varName) {
            TShallowIterator prevRegion = ShallowEnd(), nextRegion = ShallowStart(), curRegion;
            for(; prevRegion.IsOk() || nextRegion.IsOk(); prevRegion = nextRegion, nextRegion.Next()) {
                TPolyRegion *regPrev = NULL, *regNext = NULL;
                if (prevRegion.IsOk())
                    regPrev = prevRegion.CurRegion();
                if (nextRegion.IsOk())
                    regNext = nextRegion.CurRegion();
                if (prevRegion.IsOk())
                    bnd->FillBoundMinus(varName, regPrev, regNext);
                    //bnd->FillBounds(varName, regPrev, regNext, regPrev);
                if (nextRegion.IsOk())
                    bnd->FillBoundPlus(varName, regPrev, regNext);
                    //bnd->FillBounds(varName, regPrev, regNext, regNext);
            }
        }
        //void AddChildGridVar (const Stroka &varName);
        //void DelChildGridVar (const Stroka &varName);


        //GridMask GetParentBlockMask ();

        int save_data_state( FilterOut&so) {
            so<<" Grid ";Grid.save_data_state(so);
            so<<" Vars { ";
            for(map<Stroka, Ref<SavableClass> >::iterator it = MapSavableVar.begin(); it != MapSavableVar.end(); it++)
                so<<it->first<<"\n"<<SavableClass::object2string(it->second)<<"\n";
            so<<" } ";
            so<<" Childs "<<Childs<<" Parent "<<Parent;
            return 1; 
        };
        int read_data_state(FilterIn&si) { 
            Stroka tmp;
            si>>tmp;Grid.read_data_state(si);
            si>>tmp>>tmp;
            MapSavableVar.clear();
            while(!(!si) && !SavableClass::TestNextChar(si, '}')) {
                Stroka name;
                si>>name;
                MapSavableVar[name] = SavableClass::Read(si);
            }
            si>>tmp;operator>>(si, Childs)>>tmp;
            Parent = SavableClass::TestType<TPolyRegion>(si.getobject());
            return 1;
        };
        Stroka MakeHelp() {
            return "Polygon...";
        }
        //struct TDeepIterator {
        //    list<Ref<TPolyRegion> >::iterator it;
        //    TPolyRegion *reg, *main;
        //    Iterator(TPolyRegion *main_, TPolyRegion *reg_, const list<Ref<TPolyRegion> >::iterator &it_) 
        //        : it(it_)
        //        , reg(reg_)
        //        , main(main_){}
        //    inline bool operator==(const Iterator &iter) {
        //        return (iter.it == it && iter.reg == reg && iter.main == main);
        //    }
        //    inline bool operator!=(const Iterator &iter) {
        //        return !operator==(iter);
        //    }
        //}; 
        //struct TDeepIterator {
        //};
        struct TShallowIterator {
            friend struct TPolyRegion;
        private:
            Ref<TPolyRegion> Base;
            list<Ref<TPolyRegion> >::iterator Iter;
            //iterator<bidirectional_iterator_tag, Ref<TPolyRegion> > Iter;
            //iterator_traits<list<Ref<TPolyRegion> >::iterator>::iterator_category Iter;
            //bidirectional_iterator_tag IterBi;
        protected:
            inline void Last(TPolyRegion *base) {
                Base = base;
                if (Base) {
                    Iter = Base->Childs.end();
                    Iter--;
                    //Iter = (Base->Childs.rbegin()).base();
                    //iterator<bidirectional_iterator_tag, Ref<TPolyRegion> > it = Base->Childs.rbegin();
                    //Iter = it.base();
                    //Iter = it.base();
                    ////Iter = (Base->Childs.rbegin()--).base();
                    //Iter = (Base->Childs.rbegin()++).base();
                }
            }
        public:
            TShallowIterator() {};
            TShallowIterator(TPolyRegion *base) {
                Start(base);
            };
            TShallowIterator(const TShallowIterator &iter) 
                : Base(iter.Base)
                , Iter(iter.Iter) {
            };
            TShallowIterator& operator=(const TShallowIterator &it) {
                Base = it.Base;
                Iter = it.Iter;
                return *this;
            }
            inline void End() {
                if (Base)
                    Iter = Base->Childs.end();
            }
            inline void Start(TPolyRegion *base) {
                Base = base;
                if (Base)
                    Iter = Base->Childs.begin();
            }
            inline int IsOk() {
                return (!(!Base) && Iter != Base->Childs.end());
            }
            inline int Next() {
                if (!IsOk())
                    return 0;
                Iter++;
                //return 1;
                return IsOk();
            }
            inline int Prev() {
                if (!Base)
                    return 0;
				if (Iter == Base->Childs.begin()) {
					Iter = Base->Childs.end();
					return 0;
				}
                Iter--;
                return IsOk();
            }
			inline TPolyRegion * GetNext() {
				TShallowIterator it(*this);
				if (!it.Next())
					return NULL;
				return it.CurRegion();
			}
			inline TPolyRegion * GetPrev() {
				TShallowIterator it(*this);
				if (!it.Prev())
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

        //Iteratir StartRegion() {
        //    TPolyRegion *p = this;
        //    list<Ref<TPolyRegion> >::iterator it = p->Childs.begin();
        //    while((*it)->Childs.size() != 0) {
        //        p = *it;
        //        it = (*it)->Childs.begin();
        //    }
        //    return Iterator(this, p, it);
        //}
        //inline TShallowIterator ShallowEnd() {
        //    TShallowIterator it(this);
        //    it.End();
        //    return it;
        //}

        //Iterator StartRegion() {
        //    TPolyRegion *p = this;
        //    list<Ref<TPolyRegion> >::iterator it = p->Childs.begin();
        //    while((*it)->Childs.size() != 0) {
        //        p = *it;
        //        it = (*it)->Childs.begin();
        //    }
        //    return Iterator(this, p, it);
        //}
        //Iterator EndRegion() {
        //    TPolyRegion *p = this;
        //    list<Ref<TPolyRegion> >::iterator it = p->Childs.end();
        //    //while((*it)->Childs.size() != 0) {
        //    //    p = *it;
        //    //    it = (*it)->Childs.end();
        //    //}
        //    return Iterator(this, p, it);
        //}
        //static Iterator NextRegion(Iterator it) {
        //    if (it.it == it.reg->Childs.end()) {
        //        if (it.reg->Parent == it.main->Parent)
        //            return it.reg->EndRegion();
        //        throw info_except("Wau - not implemented yet\n");
        //    }
        //    if ((*it.it)->Childs.size() != 0 )
        //        throw info_except("Wau there are childs\n");
        //    if (++it.it == it.reg->Childs.end()) {
        //        while(1) {
        //            if (it.reg->Parent == it.main->Parent)
        //                return it.reg->EndRegion();
        //            throw info_except("Wau - not implemented yet\n");
        //            //TPolyRegion *p = it.reg->Parent;
        //            //list<Ref<TPolyRegion> >::iterator i = p->Childs.begin();
        //            //while(*i != it.reg)
        //            //    i++;
        //            ////i++;
        //            //if (++i != p->Childs.end())
        //            //    return Iterator(it.main, p, i);
        //        }
        //    }
        //    //throw info_except("Wau impossible position!!!\n");
        //    return it;
        //}
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

    //    friend TShallowIterator;
    //protected:
        list<Ref<TPolyRegion> > Childs;
        TPolyRegion* Parent;
    };


}; //namespace NPolygon {

#endif


        //TPolyRegion(const TPolyRegion &right);
        //~TPolyRegion();
        //TPolyRegion & operator=(const TPolyRegion &right);
        //int operator==(const TPolyRegion &right) const;
        //int operator!=(const TPolyRegion &right) const;
