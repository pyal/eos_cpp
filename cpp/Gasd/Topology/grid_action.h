#ifndef Grid_Action_h
#define Grid_Action_h 1

#include "lib/data/DataVec.h"
#include "lib/data/DataTyp.h"
#include "lib/ref/class_sav.h"


#define GridIndex int

typedef DataVector<GridIndex> GridMask;
typedef DataVector<GridMask, CopyStructSlow<GridMask>> GridMaskVector;


struct GridBoundPnt {
    SpacePoint Center;
    SpacePoint NormalDir;   //(GridIndex &k);
    double Surface;         //(GridIndex &k);
    GridIndex First, Second;
    int FirstPos, SecondPos;

    GridBoundPnt() {
        Surface = 0;
        First = -1;
        Second = -1;
        FirstPos = -1;
        SecondPos = -1;
    }
    GridBoundPnt(const GridBoundPnt &pnt) {
        Center = pnt.Center;
        NormalDir = pnt.NormalDir;
        Surface = pnt.Surface;
        First = pnt.First;
        Second = pnt.Second;
        FirstPos = pnt.FirstPos;
        SecondPos = pnt.SecondPos;
    }
    GridBoundPnt &operator=(const GridBoundPnt &pnt) {
        Center = pnt.Center;
        NormalDir = pnt.NormalDir;
        Surface = pnt.Surface;
        First = pnt.First;
        Second = pnt.Second;
        FirstPos = pnt.FirstPos;
        SecondPos = pnt.SecondPos;
        return *this;
    }

    GridBoundPnt &InvertDirection() {
        ExchVar(&First, &Second, sizeof(GridIndex));
        ExchVar(&FirstPos, &SecondPos, sizeof(int));
        NormalDir = NormalDir * (-1);
        return *this;
    };
    GridIndex GetNeib(GridIndex One) {
        return (One == First) ? Second : (One == Second) ? First : -2;
    }
    //  void ChangeNeib(GridIndex oldind,GridIndex newind,int newpos)
    //   {(oldind==First)?Second=newind;SecondPos=newpos;:First=newind;FirstPos=newpos;}
    void ChangeNeib(GridIndex oldind, GridIndex newind) {
        (oldind == First) ? Second = newind : First = newind;
    }
    int NoNeibous() {
        return (First == Second) ? (Second == -1) ? 1 : 0 : 0;
    }
};

FilterIn &operator>>(FilterIn &si, GridBoundPnt &buf);
FilterOut &operator<<(FilterOut &so, GridBoundPnt &buf);


struct GridAction {
    enum { AddPnt, DelPnt, MovePnt, ModifyPnt, VarModifyPnt, VarAddPnt } ActionTypes;
    int Action;
    GridIndex Index, Param;
    void SetAction(int act, GridIndex ind, GridIndex par) {
        Action = act;
        Index = ind;
        Param = par;
    }
};
FilterIn &operator>>(FilterIn &si, GridAction &buf);
FilterOut &operator<<(FilterOut &so, GridAction &buf);


/*
Possible solutiom
 
typedef void (*GridFunc)   (GridIndex ind,void *old,void *par);
struct GridAction
 {
   GridFunc Action;
.....

And in "grid_interface.h"

template<T>
Grid  
{
   static Grid* cur;
   static void AddFunc(GridIndex i,void *old,void *par)
     {
      int k,N=cur->VarList.SetNumEl();
      for (k=0;k<N;k++) 
      if (cur->VarList[k]!=NULL) cur->VarList[k]->AddPnt(i);
     };
.............
}
And in "grid_center.cpp"
//static 
Grid<GridCenterPnt>* Grid<GridCenterPnt>::cur;

*/


#endif