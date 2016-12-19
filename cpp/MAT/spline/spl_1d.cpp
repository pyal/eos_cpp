#include <lib/precompiled/math.h>
#include "spl_1d.h"
#include <mat/spl_fac.h>


static ClassDesc TSpline1DCalculator_ClassDesc(typeid(TSpline1DCalculator),"TSpline1DCalculator",1,"SplineCalculator_category",create<TSpline1DCalculator>);
static ClassDesc TSpline1DGenerator_ClassDesc(typeid(TSpline1DGenerator),"TSpline1DGenerator",1,"SplineGenerator_category",create<TSpline1DGenerator>);

 


#ifdef __cplusplus
extern "C" {
#endif

extern int splev_(real *, integer *,real *,integer *,real *,real *, integer *,integer *);
extern int curfit_(integer *,integer *,real *,real *,real *,real *,real *, integer *,real *,integer *,integer *,real *,
                   real *,real *, real *,integer *,integer *,integer *);

//extern int gcvspl_(real *x, real *y, integer *ny, real *wx, real *wy, integer *m, integer *n, integer *k, 
//                   integer *md, real *val, real *c__, integer *nc, real *wk, integer *ier);
//
//extern real splder_(integer *ider, integer *m, integer *n, real *t,	real *x, real *c__, integer *l, real *q);

#ifdef __cplusplus
}
#endif







int  TSpline1DCalculator::Evaluate (const vector<vector<double> > &dat, vector<double> &res) {
    if (dat.size() != 1 || dat[0].size() == 0) 
        throw info_except("Bad dimention of the dat file\n");
    res.resize(dat[0].size());
    integer polynom_degree = SplineOrder, Num = dat[0].size(), SX=SplineCoefs[0].size(), ier;
    splev_(&SplineCoefs[0][0], &SX, &SplineCoefs[1][0], &polynom_degree, (double*)&dat[0][0], &res[0], &Num, &ier);
    if (ier!=0) {
        cout<<" Bad Evaluate Spl "<<ier<<"\n";
    }
    return ier;
}


int  TSpline1DGenerator::Generate(vector<vector<double> > &dat1, int Cont) {
    int splineDim = GetSplineDim();
    if (!(dat1.size() == splineDim + 1 || dat1.size() == splineDim + 2)) 
        throw info_except("Bad grid size() is %i and SplineDim is %i\n", dat1.size(), splineDim);
    TSpline1DCalculator *spline = new TSpline1DCalculator(SplineOrder);
    SplineCalculator = spline;
    vector<vector<double> > dat;
    CheckMinXStep(dat, dat1);

//cout<<" CurveSpline::Generate \n";
    vector<real> weight = DataManip::MakeStepVector(dat[1].size(), 1, 1, 0, 0), minV, maxV;
    int delWeight = !(dat.size() == splineDim + 2);
    if  (!delWeight)
        DataManip::CopyVector(weight, dat[2]);
    DataManip::SetLimits<real>(dat, minV, maxV);
    minV.resize(1);
    maxV.resize(1);
    vector<vector<real> > splineCoefs(2);
    splineCoefs[0].resize(NumX);
    splineCoefs[1].resize(NumX);

    integer NumXs = NumX, Conti = Cont, NumXd = dat[0].size(), polynom_degree = SplineOrder, Result, NumXs_r;
    integer NRWork=NumXd*(polynom_degree+1)+NumX*(3*polynom_degree+7), NIWork = NumX;
    vector<integer> iWork(NIWork);
    vector<real> rWork(NRWork);
    real MisfR = GenerationMisfit, Mis_ret;
    curfit_(&Conti, &NumXd, &dat[0][0], &dat[1][0], &weight[0], &minV[0], &maxV[0], &polynom_degree,
            &MisfR, &NumXs, &NumXs_r, &splineCoefs[0][0], &splineCoefs[1][0], &Mis_ret, &rWork[0], &NRWork, &iWork[0], &Result);


    if (Mis_ret == 0 || GenerationMisfit < Mis_ret * 0.1 || IsNan(Mis_ret)) {
cout<<"X " << Str::Vec2Str(dat[0]) << "\nY " << Str::Vec2Str(dat[1]) <<"\nZ "<< Str::Vec2Str(weight) <<"\n";
        throw info_except("Could not make spline. Error %g have to be %g\n", Mis_ret, GenerationMisfit);
    }
    if (Result == 10 || Result == 1000) {
cout<<"X " << Str::Vec2Str(dat[0]) << "\nY " << Str::Vec2Str(dat[1]) <<"\nZ "<< Str::Vec2Str(weight) <<"\n";
        throw info_except("problems in CurveSpline::Generate; Result %i\n", Result);
    }
        //cout<<" problems in CurveSpline::Generate; Result: "<<Result<<"\n";
    splineCoefs[0].resize(NumXs_r);
    splineCoefs[1].resize(NumXs_r);

    spline->SetData(~SplineName, ~SplineDescr, minV, maxV, Mis_ret, splineCoefs);
    return Result;

}



