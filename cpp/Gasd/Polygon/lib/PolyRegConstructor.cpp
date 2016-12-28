#include <cpp/URS/MATT_FAC.H>
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


    TSimpleContructor::TSimpleContructor() : SavableClass(), RegionBoundarySize(1) {
        Childs.push_back(
            new TRegData({"EOS"}, {new MatterABu}, 100, {
                    TRegData::TGridVar("Pos", 0, 1),
                    TRegData::TGridVar("Denc", 7.89, 7.89),
                    TRegData::TGridVar("Pres", 0.0001, 0.0001),
                    TRegData::TGridVar("Ener", 0, 0),
                    TRegData::TGridVar("Vel", 5, 5),
                    TRegData::TGridVar("Sound", 4, 4)
            }));
        BaseRegion = new TRegData();
        DataFile = "NULL";
    };


};   //namespace NPolygon {
