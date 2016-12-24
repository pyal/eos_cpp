#include "lib/precompiled/urs_curve_lib.h"
#include "urs_std_curve.h"


IsentropeClass *IsentropeClass::CurPtr;

double IsentropeClass::IsentropeDU(double FromDenc, double FromEnergy, double ToDenc) {
    double ResErr;
    int Err;
    Denc = FromDenc;
    Energy = FromEnergy;
    CurPtr = this;
    return Qags_IntSubInt(
        IntegUFunc, FromDenc, ToDenc, StndErr, StndErr, 10, ResErr, Err);
}
int IsentropeClass::IsentropePres(
    double FromDenc,
    double FromEnergy,
    double ToPres,
    double &ResDenc,
    double &ResEnergy) {
    int MaxNum = 10000, Num = 0;
    double Pres = Mat->Pressure(FromDenc, FromEnergy), NextDenc = FromDenc,
           NextEnergy = FromEnergy;
    double Stp = (Pres > ToPres) ? -1 : 1;
    Stp *= FromDenc * 0.1;
    do {
        FromDenc = NextDenc;
        FromEnergy = NextEnergy;
    } while(NextPressure(NextDenc, NextEnergy, Pres, ToPres, Stp) && (++Num < MaxNum));
    double Err = 1e-6;
    ResDenc = 0.5 * (NextDenc + FromDenc);
    ResEnergy = 0.5 * (NextEnergy + FromEnergy);
    if(Num >= MaxNum)
        return 0;
    Denc = FromDenc;
    Energy = FromEnergy;
    Pressure = ToPres;
    CurPtr = this;
    Fzero(PresMisf, FromDenc, NextDenc, ResDenc, Err, Err, MaxNum);
    ResEnergy = IsentropeEnergy(ResDenc);
    return 1;
}
int IsentropeClass::NextPressure(
    double &NextDenc,
    double &NextEnergy,
    double &NextPres,
    double ToPres,
    double &Stp) {
    double StartPres = NextPres, StartDenc = NextDenc, StartEnergy = NextEnergy;
    NextDenc += Stp;
    Stp *= 1.2;
    if(NextDenc <= 0)
        NextDenc = M_Eps2;
    NextEnergy = IsentropeEnergy(StartDenc, StartEnergy, NextDenc);
    NextPres = Mat->Pressure(NextDenc, NextEnergy);
    if(NextDenc <= 1.1 * M_Eps2)
        return 0;
    return (!In_Lim(ToPres, StartPres, NextPres, 1));
}
double IsentropeClass::IsentropeEnergy(double denc) {
    double ResDenc, ResEnergy, Stp = max<double>(1e-2 * fabs(Denc - denc), 1e-4);
    int res;
    CurPtr = this;
    if((res = NotRungCurt(
            IsoentropeDeriv, Denc, Energy, denc, ResDenc, ResEnergy, Stp)) != 0) {
        cout << " Bad in  NotRungCurt " << res << " FromDenc= " << Denc << " FromEnergy "
             << Energy << " ToDenc " << denc << " ResDenc " << ResDenc << " ResEnergy "
             << ResEnergy << " Stp " << Stp << "\n";
    }
    return ResEnergy;
}
//double IsentropeClass::IntegUFunc_Old(double x){
//    double E=CurPtr->IsentropeEnergy(x);
//    CurPtr->Denc=x;CurPtr->Energy=E;
//    double ret=max<double>(CurPtr->Mat->Sound(x,E), 1e-3)/x;
//    return ret;
//}
double IsentropeClass::IntegUFunc(double x) {
    double E = CurPtr->IsentropeEnergy(x);
    CurPtr->Denc = x;
    CurPtr->Energy = E;
    double ret = max<double>(CurPtr->Mat->Pressure(x, E), 1e-6) / sqr(x);
    return ret;
}

double IsentropeClass::PresMisf(double CurDenc) {
    double E = CurPtr->IsentropeEnergy(CurDenc);
    double ret = CurPtr->Mat->Pressure(CurDenc, E) - CurPtr->Pressure;
    return ret;
}

HugoniotClass *HugoniotClass::CurPtr;

HugoniotClass::HugoniotData HugoniotClass::HugDencClc(
    const HugoniotClass::HugoniotData &beg_pnt,
    double res_denc) {
    // de - begining energy. Sometimes have to be lifted
    double de = 180;
    Start = beg_pnt;
    Start_de_coef = 2 / (1 / res_denc - 1 / Start.Denc);
    //    Start_de_coef = 2/(1/r2-1/r1);
    Start_r2 = res_denc;

    if(fabs(Start.Denc - res_denc) < StndErr) {
        HugoniotData ret = beg_pnt;
        ret.ShockVel = Mat->Sound(ret.Denc, ret.Ener);
        ret.Pres = Mat->Pressure(ret.Denc, ret.Ener);
        return ret;
    }
    CurPtr = this;
    if(3 < Fzero(MisHugDenc, -500, 500, de, StndErr, StndErr, 100))
        if(3 < Fzero(MisHugDenc, -50000, 50000, de, StndErr, StndErr, 100)) {
            cout << " Error, could not find HugDenc ZeroFzeroIt\n";
        }
    //if (3<Fzero(MisHugDenc,30,1000,de,StndErr,StndErr,100))
    //    { cout <<" Error, could not find HugDenc ZeroFzeroIt\n";}
    HugoniotData ret(0, -(Start.Pres + Start_de_coef * de), res_denc, Start.Ener + de, 0);
    //cout<<" Hugonio!!!! ";p2=PressurePorous(r2,e2);
    //cout<<" Hugonio Discrep "<<p2+(Stat_p0+Stat_coef*de)<<"\n";
    double tmp =
        sqrt(max<double>(0, -(ret.Pres - Start.Pres) / (1 / ret.Denc - 1 / Start.Denc)));
    ret.ShockVel = Start.Vel - tmp / Start.Denc;
    ret.Vel = tmp / ret.Denc + ret.ShockVel;
    ret.Vel *= -1;
    ret.ShockVel *= -1;
    return ret;
}
void HugoniotClass::SetHugPresClcVar(
    const HugoniotClass::HugoniotData &beg_pnt,
    double res_pres,
    double &dv,
    double &v1,
    double &MinV,
    double &MaxV) {

    Start = beg_pnt;
    Start_p2 = res_pres;
    Start_de_coef = -(Start.Pres + res_pres) / 2;
    v1 = 1 / Start.Denc;
    CurPtr = this;
    //    double dP = Mat->Pressure(1/v1)-res_pres;
    double A = 1.4, dP = res_pres - Start.Pres, u = sqrt(fabs(dP) * v1 / A), D = A * u;
    u = (dP > 0) ? u : -u;
    double v_est = v1 * (D - u) / D, dv_est = v_est - v1, dv_est0 = dv_est;
    //cout<<" dv_est "<<dv_est<<"\n";
    double dP_start = MisHugPres_v(0);
    double dP_est = MisHugPres_v(dv_est), dP_est0 = dP_est;
    int k = 0, MaxIter = 20;
    while(dP_start * dP_est > 0 && k++ < MaxIter) {
        dv_est /= 1.05;
        dP_est = MisHugPres_v(dv_est);
    }
    if(k >= MaxIter) {
        k = 0;
        dv_est = dv_est0;
        dP_est = dP_est0;
        while(dP_start * dP_est > 0 && k++ < MaxIter) {
            dv_est *= 1.05;
            dP_est = MisHugPres_v(dv_est);
        }
    }
    MinV = min(0, dv_est);
    MaxV = max<double>(0, dv_est);
    dv = dv_est * 0.9;
}
HugoniotClass::HugoniotData HugoniotClass::HugPresClc_v(
    const HugoniotClass::HugoniotData &beg_pnt,
    double res_pres) {
    if(fabs(Mat->Pressure(beg_pnt.Denc, beg_pnt.Ener) - res_pres) < StndErr * 10) {
        HugoniotData ret = beg_pnt;
        ret.ShockVel = Mat->Sound(ret.Denc, ret.Ener);
        ret.Pres = res_pres;
        return ret;
    };
    //    Start = beg_pnt;
    //    Start_p2 = res_pres;
    //    Start_de_coef = -(Start.Pres+res_pres)/2;
    //    double v1 = 1/Start.Denc, dv=0.01*v1;
    //    double k=0.2;int MaxN=200;
    ////    while ( Mat->Pressure(1/(v1-dv),Start.Ener+Start_de_coef*dv)<=Pcritical &&
    //    while ( Mat->Pressure(1/(v1-dv),Start.Ener+Start_de_coef*dv)<=res_pres &&
    //            --MaxN>0 )
    //	    if (v1<(1+k)*dv)
    //            k=k*0.01;
    //        else dv+=k*dv;
    //    dv=-dv;
    //    CurPtr = this;

    double dv, v1, MindV, MaxdV;
    SetHugPresClcVar(beg_pnt, res_pres, dv, v1, MindV, MaxdV);
    int res;
    if(3 <=
       (res = Fzero(
            HugoniotClass::MisHugPres_vs, MindV, MaxdV, dv, M_Eps2, M_Eps2, 10000))) {
        cout << " Error, could not find HugDenc ZeroFzeroIt\n";
        cout << " Error: " << res << "\n";
        cout << "Found dv " << dv << "\n";
    }
    HugoniotData ret(0, res_pres, 1 / (v1 + dv), Start.Ener + Start_de_coef * dv, 0);
    double tmp = sqrt(max<double>(0, -(res_pres - Start.Pres) / (1 / ret.Denc - v1)));
    ret.ShockVel = Start.Vel - tmp / Start.Denc;
    ret.Vel = tmp / ret.Denc + ret.ShockVel;
    ret.Vel *= -1;
    ret.ShockVel *= -1;
    return ret;
};

HugoniotClass::HugoniotData HugoniotClass::HugPresClc(
    const HugoniotClass::HugoniotData &beg_pnt,
    double res_pres) {
    if(fabs(Mat->Pressure(beg_pnt.Denc, beg_pnt.Ener) - res_pres) < StndErr * 10) {
        HugoniotData ret = beg_pnt;
        ret.ShockVel = Mat->Sound(ret.Denc, ret.Ener);
        ret.Pres = res_pres;
        return ret;
    };
    Start = beg_pnt;
    MD_dP = res_pres - beg_pnt.Pres;
    MD_v1 = 1 / beg_pnt.Denc;
    MD_de = -(beg_pnt.Pres + res_pres) / 2;
    MD_p = res_pres;
    MD_e = beg_pnt.Ener;
    double MinA = sqrt(fabs(MD_dP) * MD_v1) * 1.000001, MaxA = 10 + 3 * MinA,
           A = 1.4 * MinA;
    CurPtr = this;

    double dP_min = MisHugPres_D(MinA);
    int k = 0, MaxIter = 300;
    while(dP_min < 0 && k++ < MaxIter) {
        MinA *= 1.01;
        dP_min = MisHugPres_D(MinA);
    }

    //    double dP_max = MisHugPres_D(MaxA);
    ////cout<<" P "<<res_pres<<"\n";
    ////cout<<MinA<<" "<<MaxA<<" "<<dP_min<<" "<<dP_max<<"\n";
    //    MaxIter+=k;
    //    if (dP_min*dP_max>0){
    //        MaxA = MinA*1.01;
    //        dP_max = MisHugPres_D(MaxA);
    ////cout<<MinA<<" "<<MaxA<<" "<<dP_min<<" "<<dP_max<<"\n";
    //        while (dP_min*dP_max>0 && k++<MaxIter){
    //            MinA = MaxA;dP_min=dP_max;
    ////cout<<MinA<<" "<<MaxA<<" "<<dP_min<<" "<<dP_max<<"\n";
    //            MaxA *= 1.01;
    //            dP_max = MisHugPres_D(MaxA);
    //        }
    //    }


    A = (MinA + MaxA) * 0.5;
    int res;

    if(3 <= (res = Fzero(
                 HugoniotClass::MisHugPres_Ds, MinA, MaxA, A, M_Eps2, M_Eps2, 10000))) {
        cout << " Error, could not find HugDenc ZeroFzeroIt\n";
        cout << " Error: " << res << "\n";
        cout << "Found A " << A << " FoundMis " << MisHugPres_D(A) << " IterDone " << k
             << "\n";
    }
    //cout<<"Found A "<<A<<" FoundMis "<<MisHugPres_D(A)<<" IterDone "<<k<<"\n";
    double u = fabs(MD_dP) * MD_v1 / A, D = A;
    u = (MD_dP > 0) ? u : -u;
    double v_est = MD_v1 * (D - u) / D, dv = v_est - MD_v1;

    HugoniotData ret(u + Start.Vel, res_pres, 1 / v_est, Start.Ener + MD_de * dv, D);
    return ret;
};
