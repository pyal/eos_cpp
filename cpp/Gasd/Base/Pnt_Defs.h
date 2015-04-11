#ifndef _Pnt_Defs_h
#define _Pnt_Defs_h 1

#include "lib\data\DataVec.h"
#include "lib\data\DataTyp.h"
#include "lib\ref\class_sav.h"

typedef DataVector<double> SpacePoint;
typedef DataVector<int> IndexPoint;

class MeshPoint: SavableClass{
}
class Grid: SavableClass{
	class VarConstructor{
		enum{Add,Del,Change} Action;
		struct ActionStruct{
			Action ToDo;
			int PntNum;
			int GridNum;
			}
		void PntMod(ActionStruct todo, Grid* grid,const char *name);
	};
	struct Var{
//		enum {Double,Int} Types;
	    DataVector<SparceArray<double> >   Darrs;
		DataVector<SparceArray<int> >      Iarrs;
		SparceArray<double> *GetD(int N,int NonIsCrit=1);
		SparceArray<int> *GetI(int N,int NonIsCrit=1);
		Ref<VarConstructor> constr;
//		SparceArray<double> *GetD(Stroka name,int NonIsCrit=1);
//		SparceArray<int> *GetI(Stroka name,int NonIsCrit=1);
		Var(int Ni, int Nd, VarContructor *constr);
		void PutOnGridBag(GridBag *);

	}
    struct DataInd{int ArrNum, Type;Ref<GridConstructor> constr;}
    map<Stroka,SparceArray<DataInd> >   ArrayNames;
    map<Stroka, Ref<SavableClass> >     DataNames;
//    enum{Add,Del,Change} Action;
    Ref<GridConstructor> AllGridConstr;//
public:
    void PntMod(ActionStruct todo);
    int GetCurGridNum();
    int GetCurGridNum();
}
class GridBag : Grid{
    SparceArray<GridBag>    Grids;
}