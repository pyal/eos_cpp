
#pragma once

#include "urs/matt_fac.h"
#include "mat/mtrung.h"
#include "mat/quad.h"

struct IsentropeClass  {
public:
    IsentropeClass(MatterIO *mat):Mat(mat),OldPtr(CurPtr){CurPtr=this;};//
    void SetNewMat(MatterIO *mat){Mat = mat;};
    virtual ~IsentropeClass(){Finish();};
    void Finish(){CurPtr=OldPtr;}
//To caluclate deltaU
    double IsentropeDU(double FromDenc,double FromEnergy,double ToDenc);
//To caluclate IsentropeE
    double IsentropeEnergy(double FromDenc,double FromEnergy,double ToDenc) {
        Denc=FromDenc;Energy=FromEnergy; return IsentropeEnergy(ToDenc);
    }
// To find resPres and resE on the isentrope with given pressure
    int IsentropePres(double FromDenc,double FromEnergy,double ToPres,
                        double &ResDenc,double &ResEnergy);
    private:
// To make Stp in Denc - check if necessary pressure is in lim between 
// start denc isentrope pres and a final one
    int NextPressure(double &NextDenc,double &NextEnergy,double &NextPres,
                    double ToPres,double &Stp);
    //To use in calculation of isentrope form
    MatterIO *Mat;
    double Denc,Energy,Pressure;
    IsentropeClass *OldPtr;
// To calculate isentrope fromdenc frome - to denc
    double IsentropeEnergy(double denc);

    static IsentropeClass *CurPtr;
// To calculate next isentrope point - used in IsentropeEnergy
    static double IsoentropeDeriv(double d,double e){return CurPtr->Mat->Pressure(d,e)/sqr(d);}
// Calculating dU
    static double IntegUFunc(double x);
// Finally to find zero pressure misfit
    static double PresMisf(double CurDenc);

};

struct HugoniotClass{
    HugoniotClass(MatterIO *mat)
        :Mat(mat), Pcritical(-0.01e5), NumHugIteration(10){}
    struct HugoniotData{
        double Vel,Pres,Denc,Ener,ShockVel;
        HugoniotData(double vel=0, double pres=0, double denc=0, double ener=0, double shockvel=0)
            :Vel(vel), Pres(pres), Denc(denc), Ener(ener), ShockVel(shockvel){};
    };

    HugoniotData HugDencClc(const HugoniotData &beg_pnt, double res_denc);
    HugoniotData HugPresClc(const HugoniotData &beg_pnt, double res_pres);
    HugoniotData HugPresClc_v(const HugoniotData &beg_pnt, double res_pres);


    friend struct UrsCurve_HugCheck;
private:
    void SetHugPresClcVar(const HugoniotData &beg_pnt, double res_pres,
                          double &dv, double &v1, double &MinV, double &MaxV);
    MatterIO *Mat;
    double Pcritical;
    int NumHugIteration;
    HugoniotData Start;
    double Start_de_coef, Start_r2, Start_p2;
    static HugoniotClass *CurPtr;
    static double MisHugDenc(double de) {
        double ret=CurPtr->Mat->Pressure(CurPtr->Start_r2, CurPtr->Start.Ener+de)+
            CurPtr->Start.Pres+CurPtr->Start_de_coef*de;
//cout<<de<<" "<<ret<<"\n";
        return ret;
    }
    static double MisHugPres_vs(double dv) {
        return CurPtr->MisHugPres_v(dv);
    }; 
    double MisHugPres_v(double dv) {
        double r2=1/(1/Start.Denc+dv);
        if (r2<StndErr) return 1e30;
        double ret=Mat->Pressure(r2,Start.Ener+Start_de_coef*dv)-Start_p2;
        return ret;
    }; 
    static double MisHugPres_Ds(double A) {
        return CurPtr->MisHugPres_D(A);
    }
    double MD_dP, MD_v1, MD_de, MD_p, MD_e;
    double MisHugPres_D(double A){
        double u = fabs(MD_dP)*MD_v1/A, D = A;
        u=(MD_dP>0)?u:-u;
        double v_est = MD_v1*(D-u)/D, dv_est = v_est-MD_v1;
        if (v_est<M_Eps2)
            return 1e30;
        double ret=Mat->Pressure(1/v_est,MD_e+MD_de*dv_est)-MD_p;
        return ret;
    }

};

