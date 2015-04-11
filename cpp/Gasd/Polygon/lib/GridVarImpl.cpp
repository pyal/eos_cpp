#include "lib/precompiled/poly_lib.h"


#include "GridVarImpl.h"


static ClassDesc NPolygon_TGridVar_Double_ClassDesc(typeid(NPolygon::TGridVar<double>),"NPolygon::TGridVar_Double",1,"NPolygon::TGridVar_category",create<NPolygon::TGridVar<double> >);



namespace NPolygon {
    void TGridVarNothingToDo(){
        int a(1), b(1);
        a+=b;
    };

}; //namespace NPolygon { 