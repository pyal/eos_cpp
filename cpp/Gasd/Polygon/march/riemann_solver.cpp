#include "riemann_solver.h"


static ClassDesc NPolygon_TMarchExactRieman_ClassDesc(
        typeid(NPolygon::TMarchExactRieman),
        "NPolygon::TMarchExactRieman",
        1,
        "NPolygon::RegionMarch_category",
        create<NPolygon::TMarchExactRieman>);


vector<string> NPolygon::GasdFieldNames = {"Pressure", "Energy", "Position", "Density", "Velocity"};

Stroka NPolygon::TMarchExactRieman::MakeHelp() {
    Stroka ret("Rieman solver for 2 body problem");
    return ret;
}
