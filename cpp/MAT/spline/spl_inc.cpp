#include <lib\precompiled\math.h>
#include "spl_inc.h"

//static ClassDesc TExpConverter_ClassDesc(typeid(TExpConverter),"TExpConverter",1,"FunctionConverter_category",create<TExpConverter>);

static ClassDesc TSpline2DCalculator_ClassDesc(typeid(TSpline2DCalculator),"TSpline2DCalculator",1,"SplineCalculator_category",create<TSpline2DCalculator>);
static ClassDesc TSpline2DGenerator_ClassDesc(typeid(TSpline2DGenerator),"TSpline2DGenerator",1,"SplineGenerator_category",create<TSpline2DGenerator>);










#ifdef __cplusplus
extern "C" {
#endif
extern int regrid_(integer *, integer *, real *, integer *, real *, real *, real *, real *, real *, real *, integer *,
                   integer *, real *, integer *, integer *, integer *, real *, integer *, real *, real *, real *, real *, 
                   integer *, integer *, integer *, integer *);
extern int bispev_(real *, integer *, real *, integer *, real *, integer *, integer *, real *, integer *, real *,
                   integer *, real *, real *, integer *, integer *, integer *, integer *);

extern int splev_(real *, integer *,real *,integer *,real *,real *, integer *,integer *);
extern int curfit_(integer *,integer *,real *,real *,real *,real *,real *, integer *,real *,integer *,integer *,real *,
                   real *,real *, real *,integer *,integer *,integer *);

extern int gcvspl_(real *x, real *y, integer *ny, real *wx, real *wy, integer *m, integer *n, integer *k, 
                   integer *md, real *val, real *c__, integer *nc, real *wk, integer *ier);

extern real splder_(integer *ider, integer *m, integer *n, real *t,	real *x, real *c__, integer *l, real *q);

#ifdef __cplusplus
}
#endif

int  TSpline2DCalculator::Evaluate (const vector<vector<double> > &dat, vector<double> &res) {
    if (dat.size() != 2 || dat[0].size() == 0) 
        throw info_except("Bad dimention of the dat file\n");
    real rx,ry,rz;
    integer lwk = 8, liw = 2, ier, n1 = 1, ny = 1, SX = SplineCoefs[0].size(), SY = SplineCoefs[1].size();
    vector<real> wk(lwk);
    vector<integer> iw(liw);
    integer px = 3, py = 3;
    res.resize(dat[0].size());
    for (int k = 0; k < (int)dat[0].size(); k++) { 
        rx=dat[0][k];ry=dat[1][k];
        bispev_(&SplineCoefs[0][0], &SX, &SplineCoefs[1][0], &SY, &SplineCoefs[2][0], &px, &py, &rx, &n1, &ry, &ny, &rz,
                &wk[0], &lwk, &iw[0], &liw, &ier);
        if (ier != 0) 
            throw info_except("BadSplineResult(%i) x(%d) y(%d) z(%d)\n", ier, rx, ry, rz);
        res[k]=rz;
    }  
    return ier;
  };

int  TSpline2DGenerator::Generate(vector<vector<double> > &dat, int Cont) {
    if (dat.size() != SplineDim + 1 ) 
        throw info_except("Bad grid size() is %i and SplineDim is %i\n", dat.size(), SplineDim);
    TSpline2DCalculator *spline = new TSpline2DCalculator();
    SplineCalculator = spline;
    vector<vector<real> > tab, splineCoefs(3);
    splineCoefs[0].resize(NumX);
    splineCoefs[1].resize(NumY);
    splineCoefs[2].resize(NumX * NumY);
    DataManip::XYPlanar2Table(tab, dat);
    integer NumXi = NumX, NumYi = NumY, Conti = Cont, px = 3, py = 3, DX = dat[0].size(), DY = dat[1].size();
    integer SX, SY;
    integer NRWork = 4 + NumX * (DY + px * 2 + 5) + NumY * (2 * px + 5) + (px + 1) * (DX + DY) + NumX + DY,
            NIWork = 3 + DX + DY + NumX + NumY, Result;

    vector<integer> IWork(NIWork);
    vector<real> RWork(NRWork), minV, maxV;
    DataManip::SetLimits<real>(tab, minV, maxV);
    minV.resize(2);
    maxV.resize(2);
    real SumSquareResiduals, MisfR = GenerationMisfit;
    regrid_(&Conti, &DX, &dat[0][0], &DY, &dat[1][0], &dat[2][0],
            &minV[0], &maxV[0], &minV[1], &maxV[1], &px, &py,
            &MisfR, &NumXi, &NumYi, &SX, &splineCoefs[0][0], &SY, &splineCoefs[1][0], &splineCoefs[2][0],
            &SumSquareResiduals, &RWork[0], &NRWork, &IWork[0], &NIWork, &Result);
    if (SumSquareResiduals == 0 || GenerationMisfit < SumSquareResiduals * 0.5 || _isnan(SumSquareResiduals))
        throw info_except("Could not make spline. Error %g have to be %g\n", SumSquareResiduals, GenerationMisfit);
    splineCoefs[0].resize(SX);
    splineCoefs[1].resize(SY);
    splineCoefs[2].resize((SX - px - 1) * (SY - py - 1));
    spline->SetData(~SplineName, ~SplineDescr, minV, maxV, SumSquareResiduals, splineCoefs);
    return Result;
}












