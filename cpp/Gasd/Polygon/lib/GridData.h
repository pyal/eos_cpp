
#ifndef GridData_h
#define GridData_h 1


// GridMask
#include "GridMask.h"

namespace NPolygon {

struct TRegGrid;
struct TGridData : SavableClass {

  protected:
      int GridSize;
      map<Stroka, Ref<TGridVariablesBase> > MapGridVars;
      virtual void Resize(int size, TGridMaskBase *curMask, TGridMaskBase *wasMask) {
          GridSize = size;
          for(map<Stroka, Ref<TGridVariablesBase> >::iterator it = MapGridVars.begin(); it != MapGridVars.end(); it++)
              it->second->Resize(size, curMask, wasMask);
      }
      friend struct TRegGrid;
  public:

      TGridData() : SavableClass(), GridSize(0){};
      //TGridData(const TGridData &right);
      //~TGridData();

      //TGridData & operator=(const TGridData &right);
      //int operator==(const TGridData &right) const;
      //int operator!=(const TGridData &right) const;

      //void Rebuild (GridMask* wasFull, GridMask* wasBody, GridMask* nowFull, GridMask* nowBody);
      void AddVar (const Stroka &name, TGridVariablesBase *var, TGridMaskBase *curMask, TGridMaskBase *wasMask) {
          map<Stroka, Ref<TGridVariablesBase> >::iterator it = MapGridVars.find(name);
          if (it != MapGridVars.end())
              throw info_except(~(Stroka("Adding var ") + name + " already is present!!!\n"));
          var->Resize(GridSize, curMask, wasMask);
          MapGridVars[name] = var;
      }
      inline int VarExists(const Stroka &name) const {
          return (MapGridVars.find(name) != MapGridVars.end());
      }
      inline TGridVariablesBase* GetVar (const Stroka &name) {
          map<Stroka, Ref<TGridVariablesBase> >::iterator it = MapGridVars.find(name);
          if (it == MapGridVars.end())
              throw info_except(~(Stroka("Reading var ") + name + " absent!!!\n"));
          return MapGridVars[name];
      }
      vector<Stroka> GetVarNames() {
          vector<Stroka> ret;
          for(map<Stroka, Ref<TGridVariablesBase> >::iterator it = MapGridVars.begin(); it != MapGridVars.end(); it++)
              ret.push_back(it->first);
          return ret;
      }

};



}; //namespace NPolygon {


#endif
