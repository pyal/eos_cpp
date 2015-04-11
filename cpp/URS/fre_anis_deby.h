#pragma once

#include "fre_fac.h"
//#include "mat\NdimMin.h"
//#include "mat\fun_fac.h"
#include "fre_ion.h"
#include "clc_anis_deby.h"



// ===========================================================================================================
// =============================        FreeEIonisation       ================================================
// ===========================================================================================================

//struct FreeEIonClc : FreeEIOFind {
//    FreeEIonClc() : FreeEIOFind(), IonAddition(new ClcSimpleIon), MatFreeE(new FreeEIdeal) {};
//
//    double FreeE(double Denc, double T) {
//        if ((T<MathZer) || (Denc<MathZer)) { 
//            cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;
//        }
//        return IonAddition->ClcFreeE(Denc, T) + MatFreeE->FreeE(Denc, T);
//
//    }
//
//    void ReadSubstancePar(istream &in) {
//        char tmp[256];
//        SavableClass *pAdd, *pMat;
//        in>>tmp>>pAdd>>tmp>>pMat;
//        if (!(IonAddition<<pAdd) || !(MatFreeE<<pMat))
//            throw info_except("Could not read matter or ionisation method\n");
//        if (!in)
//            throw info_except("Could not read matter...\n");
//    }
//    void SaveSubstancePar(ostream &out) {
//        out<<" IonisationClc "<<IonAddition<<" Matter "<<MatFreeE;
//    }
//
//
//
//	virtual void SetInternalNames(){
//		FreeEIOFind::SetInternalNames();
//		int tmp = 1;
//        map<Stroka, double> names = IonAddition->GetInternalParams();
//        for (map<Stroka, double>::iterator it = names.begin(); it != names.end(); it++)
//            OutputNames[it->first] = &tmp;
//  	}
//	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
//        map<Stroka, double> names = IonAddition->GetInternalParams();
//        for (map<Stroka, double>::iterator it = names.begin(); it != names.end(); it++)
//            if (it->first == Output){
//                IonAddition->ClcFreeE(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"));
//                return IonAddition->GetInternalParams()[Output];
//            }
//		return FreeEIOFind::OutputValClc(Input, Output);
//	}
//
//
//    Stroka MakeHelp(){
//        Stroka res = FreeEIOFind::MakeHelp() + "\nClass for calculation of FreeE of matter plus ionisation .\n";
//        
//        return res;
//    }
//
//    void ReadSubstancePar(istream &in) {
//        FreeEIonAdd::ReadSubstancePar(in);
//        char tmp[256];
//        SavableClass *ptrEl, *ptrMat;
//        in>>tmp>>ptrEl>>tmp>>ptrMat>>tmp>>ZeroTemp>>tmp>>ElDencErr>>tmp>>DeltaMulCoef;
//        if (!in || !(ElFreeE<<ptrEl) || !(MatFreeE<<ptrMat))
//            throw info_except("Could not read matter...\n");
//        Electron = new PureIon_StrictClc(ElFreeE);
//    }
//    void SaveSubstancePar(ostream &out) {
//        FreeEIonAdd::SaveSubstancePar(out);
//        out<<" ElectronFreeE "<<ElFreeE<<" MatterFreeE "<<MatFreeE<<" ZeroTemp "<<ZeroTemp<<" ElDencErr "<<ElDencErr<<" DeltaMulCoef_crack "<<DeltaMulCoef;
//    }
//
//
//private:
//    Ref<ClcIonisationBase> IonAddition;
//    Ref<InterfaceFreeEIO> MatFreeE;
//
//    
//};


// ===========================================================================================================
// =============================   END  FreeESimpleIon        ================================================
// ===========================================================================================================















#include "fre_ion.h"

struct FreeEIonAnisimov : ClcIonisation::FreeEIonStdIO {
    FreeEIonAnisimov();
    double FreeEMol(double Denc,double T) {
         if ((T<MathZer) || (Denc<MathZer)) { 
             cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";
             return 0;
         }
         return IonClc.ClcFreeE(Denc, T);
    }
    vector<int> L_num, N_num;
    int CalcMethod;
    void ReadSubstancePar(istream &in) {
        FreeEIonStdIO::ReadSubstancePar(in);
        char tmp[256];
        SavableClass *ptrEl, *ptrMat;
        in>>tmp>>ptrEl>>tmp>>ptrMat;//>>tmp>>ZeroTemp>>tmp>>ElDencErr>>tmp>>DeltaMulCoef;
        if (!in || !(ElFreeE<<ptrEl) || !(MatFreeE<<ptrMat))
            throw info_except("Could not read matter...\n");
        in>>tmp>>CalcMethod;
        in>>tmp;
        N_num.clear();L_num.clear();
        TestNextWord(in, "{");
        while(!TestNextChar(in,'}',1) && !(!in) ){
            double n,l;
            in>>tmp>>n>>tmp>>l;
            N_num.push_back(n);
            L_num.push_back(l);
        }
        if (N_num.size()!=IonPot.size())
            throw info_except("Sizes of IonLevel<%i> and N_num<%i> arrays are different\n", IonPot.size(), N_num.size());

        Electron = new ClcIonisation::ElectronStub;
        Electron->Init(ElFreeE, MolVeight);
        IonClc.Init(IonPot, Gfactor, N_num, L_num, Electron.pointer(), CalcMethod, MolVeight);
    }
    void SaveSubstancePar(ostream &out) {
        FreeEIonStdIO::SaveSubstancePar(out);
        out<<" ElectronFreeE "<<ElFreeE<<" MatterFreeE "<<MatFreeE;
        out<< "CalcMethod "<<CalcMethod;
        out<<" N_L_numbers2Levels { ";
        for(size_t k=0;k<N_num.size();k++)
            out<<" N_num("<<k<<") "<<N_num[k]<<" L_num "<<Gfactor[k];
        out<<" } ";

        //<<" ZeroTemp "<<ZeroTemp<<" ElDencErr "<<ElDencErr<<" DeltaMulCoef_crack "<<DeltaMulCoef;
    }


    Stroka MakeHelp(){
        Stroka res = FreeEIonStdIO::MakeHelp() + "Calc FreeE taking into account ions. Aprox - interaction part for ions - the same. Uses EOS of Mat, Electrons... There are 3 modes : DebyAnis Deby NoDeby [0,1,2] - Deby correction + anisimov lowering of potentials; DebyCorrection; FixedIonPot\n";
        return res;
    }
protected:
    vector<double> GetIonNums(double Denc, double T){
        IonClc.ClcFreeE(Denc/MolVeight, T);
        return IonClc.ClcIonNum();
    }
private:
    Ref<ClcIonisation::ElectronStub> Electron;
    ClcIonisation::MathAnisIon IonClc;
    //map<Stroka, double> ClcData;
    Ref<InterfaceFreeEIO> ElFreeE;
    Ref<InterfaceFreeEIO> MatFreeE;
    //double MolVeight;
    //double ZeroTemp, ElDencErr, DeltaMulCoef;
};








//struct FreeEIonOneMat : FreeEIonAdd{
//    double FreeE(double Denc,double T) {
//     if ((T<MathZer) || (Denc<MathZer)) 
//        { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
//     ClcDataStruct FixData = MakeStartData(Denc, T);
//     double ElDenc = ClcElDenc(FixData);
//     vector<double> lev = ClcIonLevels(FixData, ClcElDenc(FixData));
//     double fStrict = ClcFreeEStrict(FixData, lev, ElDenc);
//     return fStrict;
//
//    }
//    void ReadSubstancePar(istream &in) {
//        FreeEIonAdd::ReadSubstancePar(in);
//        char tmp[256];
//        SavableClass *ptrEl, *ptrMat;
//        in>>tmp>>ptrEl>>tmp>>ptrMat>>tmp>>ZeroTemp>>tmp>>ElDencErr>>tmp>>DeltaMulCoef;
//        if (!in || !(ElFreeE<<ptrEl) || !(MatFreeE<<ptrMat))
//            throw info_except("Could not read matter...\n");
//        Electron = new PureIon_StrictClc(ElFreeE);
//    }
//    void SaveSubstancePar(ostream &out) {
//        FreeEIonAdd::SaveSubstancePar(out);
//        out<<" ElectronFreeE "<<ElFreeE<<" MatterFreeE "<<MatFreeE<<" ZeroTemp "<<ZeroTemp<<" ElDencErr "<<ElDencErr<<" DeltaMulCoef_crack "<<DeltaMulCoef;
//    }
//
//
//    Stroka MakeHelp(){
//        Stroka res = FreeEIonAdd::MakeHelp() + "Calc FreeE taking into account ions. Aprox - interaction part for ions - the same. Uses EOS of Mat, Electrons...\n"+
//        FreeEIOFind::MakeHelp();
//        return res;
//    }
//
//protected:
//    VecCl GetIonNum(double Denc, double T){
//         ClcDataStruct FixData = MakeStartData(Denc, T);
//         double ElDenc = ClcElDenc(FixData);
//         return ClcIonNum(FixData, ClcIonLevels(FixData, ElDenc));
//    }
//private:
//    struct ClcDataStruct{
//        double FixMatE, FixMatT, FixMatD;
//    };
//    double ClcFreeESimple1(const ClcDataStruct &FixDat, const vector<double> &IonLev, double ElDenc){
//        double kt=M_Rconst/MolVeight*FixDat.FixMatT;
//         double add = 0;
//         for(size_t k=0;k<IonLev.size();k++)
//             if (-IonLev[k]/FixDat.FixMatT>-100)
//                add +=Degeneration[k]*exp(-IonLev[k]/FixDat.FixMatT);
//         if (add<MathZer)
//             add = StndErr;
//         double ret=FixDat.FixMatE-kt*log(add);
//         return ret;
//    }
//    double ClcFreeESimple(const ClcDataStruct &FixDat, const vector<double> &IonLev, double ElDenc){
//        double kt=M_Rconst/MolVeight*FixDat.FixMatT;
//        double kt_T=M_Rconst/MolVeight;
//        VecCl LevNum = ClcIonNum(FixDat, IonLev);
//        double Sum = 0;
//        for(int k = 1;k<=LevNum.Dim();k++){
//            Sum += LevNum[k]*(kt_T*IonLev[k-1]-kt*(log(LevNum[k])+log(Degeneration[k-1])));
//        }
//        return FixDat.FixMatE+Sum;
//    }
//    double ClcFreeEStrict(const ClcDataStruct &FixDat, const vector<double> &IonLev, double ElDenc){
//        double kt=M_Rconst/MolVeight*FixDat.FixMatT;
//        double kt_T=M_Rconst/MolVeight;
//        VecCl LevNum = ClcIonNum(FixDat, IonLev);
//        double Sum = 0;
//        for(int k = 1;k<=LevNum.Dim();k++){
//            Sum += LevNum[k]*(kt_T*IonLevels[k-1]-kt*(-log(LevNum[k])+log(Degeneration[k-1])));
//        }
//        Electron->SetElDenc(FixDat.FixMatT, ElDenc);
//        Sum += Electron->ElFreeE()*ElDenc/FixDat.FixMatD;
//        Sum += kt*log(Degeneration[0]); // It is  included already in Fmat...
//        return FixDat.FixMatE+Sum;
//    }
//    Ref<PureIon_StrictClc> Electron;
//
//    Ref<InterfaceFreeEIO> ElFreeE;
//    Ref<InterfaceFreeEIO> MatFreeE;
//    double ZeroTemp, ElDencErr, DeltaMulCoef;
//    ClcDataStruct MakeStartData(double Denc, double Temp){
//        ClcDataStruct ret;
//        ret.FixMatD = Denc;
//        ret.FixMatT = Temp;
//        ret.FixMatE = MatFreeE->FreeE(Denc, Temp);
//        return ret;
//    }
//    double ClcElDenc(const ClcDataStruct &FixDat){
//        double resDenc = LastElDencStatic, ErrorAbs = ElDencErr, ErrorRel = ElDencErr;
//        int err, MaxIter = 1000;
//        FreeEIonOneMat* OldPtr = CurPtr;
//        CurPtr = this;
//        FixDatStore = FixDat;
//        if (3<(err=Fzero(DencMisfitStatic, -60 ,log(FixDat.FixMatD*IonLevels.size()) , resDenc, ErrorAbs, ErrorRel, MaxIter))){
//            if ( fabs(resDenc+60)>1 || DencMisfitStatic(resDenc)<0 ){
//			cout<<" Bad FZero GetElDenc in FreeEIonOneMat \nfound Denc "
//				<<exp(resDenc)<<" Mis "<<DencMisfitStatic(resDenc)<<" err "<<err<<" logD "<<resDenc<<"\n";
//            }
//        }
//        CurPtr = OldPtr;
//        LastElDencStatic = resDenc;
//        return exp(resDenc);
//    }
//    static FreeEIonOneMat* CurPtr;
//    static double LastElDencStatic;
//    static double DencMisfitStatic(double ElDenc){
//        return CurPtr->DencMisfit(ElDenc);
//    }
//    ClcDataStruct FixDatStore;
//    double DencMisfit(double logElDenc){
//        double ElDenc = exp(logElDenc);
//        double ClcD = ClcElDenc(FixDatStore, ElDenc);
//        return log(ElDenc/ClcD);
//    }
//    double ClcElDenc(const ClcDataStruct &FixDat, double ElDenc){
//        VecCl IonNum = ClcIonNum(FixDat, ClcIonLevels(FixDat, ElDenc));
//        double ElCoef = 0;
//        for(int k=2;k<=IonNum.Dim();k++)
//            ElCoef += (k-1)*IonNum[k];
//        return FixDat.FixMatD*ElCoef;
//    }
//    vector<double> ClcIonLevels(const ClcDataStruct &FixDat, double ElDenc){
//        vector<double> lev(IonLevels.size());
//        Electron->SetElDenc(FixDat.FixMatT, ElDenc);
//        double Shift = Electron->FreeENumDeriv(0);
//        double Coef = MolVeight/M_Rconst;
//        for(int k=0;k<(int)IonLevels.size();k++)
//            lev[k] = IonLevels[k]+(Electron->FreeENumDeriv(k)-Shift)*Coef*k;
//            //lev[k] = IonLevels[k]+(Electron->FreeENumDeriv(k)-Shift)*Coef;
//        return lev;
//    }
//
//    VecCl ClcIonNum(const ClcDataStruct &FixDat, const vector<double> &lev){
//        VecCl ret(int(lev.size())), l(int(lev.size()));
//        for(int k=1;k<=int(lev.size());k++)
//            l[k] = lev[k-1];
//        double min = VecCl::Min(l);
//        l = l-min;
//        for(int k=1;k<=int(lev.size());k++){
//             if (l[k]/FixDat.FixMatT<200)
//                 ret[k]=Degeneration[k-1]*exp(-l[k]/FixDat.FixMatT);
//             else 
//                 ret[k] = Degeneration[k-1]*exp(-200.);
//        }
//        ret = ret/(ret*(ret*0+1));
//        return ret;
//    }
//
//    
//};
//














