#include "lib/precompiled/poly_lib.h"


#include "PolyRegConstructor.h"

namespace NPolygon {

    static ClassDesc NPolygon_SimpleContructor_ClassDesc(
        typeid(TSimpleContructor),
        "NPolygon::TSimpleContructor",
        1,
        "NPolygon::Constructor_category",
        create<TSimpleContructor>);
    static ClassDesc NPolygon_SimpleContructor_TRegData_ClassDesc(
        typeid(TSimpleContructor::TRegData),
        "NPolygon::TSimpleContructor::TRegData",
        1,
        "",
        create<TSimpleContructor::TRegData>);


    TSimpleContructor::TSimpleContructor() : SavableClass(), RegionBoundarySize(0) {
        //list<Ref<RegData> > Childs;
        //Ref<RegData> BaseRegion;
        //list<Stroka> GridNames;
        //Ref<TRegBoundaryBase> Bound;
        Childs.push_back(
            new TRegData("EOS", new TRegData, 10, TRegData::TGridVar("X", 0, 1)));
        BaseRegion = new TRegData("EOS", new TRegData, 10, TRegData::TGridVar("X", 0, 1));
        //GridNames.push_back("X");
        //Bound = new TRegBoundaryCircle();
        DataFile = "NULL";
    };


};   //namespace NPolygon {
