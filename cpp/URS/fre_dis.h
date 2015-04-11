#pragma once

#include "fre_fac.h"
#include "mat\NdimMin.h"
#include "mat\fun_fac.h"


//struct PureDis_StrictClc:RefCount{
////Calculate vector of pressures of pure subst
//    VecCl PressureVector(double Temp, const VecCl &RelatVolumesCoefs)=0;
//};
//Calculates FreeE of mixture, derivative of the freeE
struct PureDis_StrictClc:RefCount{
    struct Param:RefCount{
        VecCl RelatNumbers, RelatVolumesCoefs, FreeEPure;
        double Denc, Temp, Pressure;
        Param(double denc, double temp, double pressure, const VecCl &relatnumbers, const VecCl &relatvolumescoefs,
            const VecCl &freeepure){
            RelatNumbers = relatnumbers; RelatVolumesCoefs = relatvolumescoefs;FreeEPure = freeepure;
            Denc = denc; Temp = temp;Pressure = pressure;
        }
        Param(const Param &par):Denc(par.Denc), Temp(par.Temp), Pressure(par.Pressure), 
            RelatNumbers(par.RelatNumbers), RelatVolumesCoefs(par.RelatVolumesCoefs), FreeEPure(par.FreeEPure){};
        Param &operator=(const Param &par){
            Denc = par.Denc; Temp = par.Temp; Pressure = par.Pressure;
            RelatNumbers = par.RelatNumbers; RelatVolumesCoefs = par.RelatVolumesCoefs; FreeEPure = par.FreeEPure;
            return *this;
        }
    };
    PureDis_StrictClc(const DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &chemsubst, const VecCl &substveights)
        :ChemSubst(chemsubst), SubstVeights(substveights){};
////Calculate coef for volumeCoefs - to get mole dencities  ( RelatVolumesCoefs, Denc, RelatNumbers)
	double  ClcVolCoef(const Param &par){
		double Coef = par.Denc*(par.RelatNumbers*par.RelatVolumesCoefs)/(par.RelatNumbers*SubstVeights);
		return 1/Coef;
	}
//Calculate vector of dencities of pure subst  ( RelatVolumesCoefs, Denc, RelatNumbers)
	VecCl ClcSubstDenc(const Param &par){
		VecCl tmp(par.RelatVolumesCoefs*ClcVolCoef(par));
        VecCl SubstDenc =  VecCl::ComponentDiv(SubstVeights,tmp);
		return SubstDenc;
		//VecCl SubstDenc =  VecCl::ComponentDiv(RelatVolumesCoefs*0+1,RelatVolumesCoefs);
		//return SubstDenc;
	}
//Calculate vector of Relat Masses of pure subst  ( RelatNumbers)
	VecCl ClcMassCoef(const Param &par){

        VecCl MassCoef = VecCl::ComponentMul(par.RelatNumbers, SubstVeights);
		MassCoef = MassCoef/(MassCoef*(MassCoef*0+1));
		return MassCoef;
	}
//Calculate vector of pressures of pure subst  (Temp, RelatVolumesCoefs, Denc, RelatNumbers)
    VecCl PressureVector(const Param &par){
//cout<<" Denc "<<Denc<<" Temp "<<" RelatNum "<<RelatNumbers<<" RelatVolCoef "<<RelatVolumesCoefs;
		VecCl SubstDenc = ClcSubstDenc(par);
//cout<<" SubstDenc "<<SubstDenc;
		VecCl pres(SubstVeights.Dim());
		for (int k=1;k<=pres.Dim();k++){
			pres[k] = ChemSubst[k-1]->Pressure(SubstDenc[k], par.Temp);
		}
//cout<<" ResultPressures!! "<<pres<<flush;
        return pres;
	}
//Calculate vector of FreeE of pure subst (Temp, RelatVolumesCoefs, Denc, RelatNumbers)
	VecCl FreeEVectorPure(const Param &par){
		VecCl SubstDenc = ClcSubstDenc(par);
		VecCl ret = SubstVeights*0;
		for(int k=1;k<=SubstVeights.Dim();k++)
			ret[k] = ChemSubst[k-1]->FreeE(SubstDenc[k],par.Temp);
		return ret;			
	}
//Vector of FreeE for 1 gramm of final matter FreeEStrict +  mixture correction 
//    (RelatNumbers  RelatVolumesCoefs  FreeEPure  Temp )
	VecCl FreeEVectorCor(const Param &par){
        //VecCl ret = par.FreeEPure + GetMixtureCorrection(par);
        VecCl pure = VecCl::ComponentMul(VecCl::ComponentMul(par.FreeEPure,SubstVeights),par.RelatNumbers)/ 
            (par.RelatNumbers * SubstVeights);
        VecCl ret = pure + GetMixtureCorrection(par);
		return ret;			
	}
//Clc Full FreeE known strict vals - for 1 gramm of matter  (RelatNumbers  RelatVolumesCoefs  FreeEPure  Temp )
	double FreeEMixt(const Param &par){
        VecCl FreeECor = FreeEVectorCor(par);
        return FreeECor * (FreeECor*0+1);
	}
//   Clc derivative (RelatNumbers  RelatVolumesCoefs  FreeEPure  Temp  Denc  Pressure)
    VecCl FreeENumDeriv(const Param &par_){
        Param par = par_;
// We are not calculating FreeE of subs on each step!!!!!!!!!!!!! - may be error - but faster....
        par.FreeEPure = FreeEVectorPure(par);
        //FreeECor[k]*M0/par.RelatNumbers[k]
        double M0 = par.RelatNumbers * SubstVeights;
        return FreeENumDeriv( par, VecCl::ComponentDiv(FreeEVectorCor(par)*M0,par.RelatNumbers) );
    }
    static VecCl LogNum2RelatNum(const VecCl &LogNum){
        VecCl Num = LogNum - VecCl::Max(LogNum);
        double MinV = log(M_MinDouble2);
        for(int i = 1;i<=LogNum.Dim();i++)
            Num[i] = (Num[i]>=MinV)?exp(Num[i]):0;
        double Coef = VecCl::Max(LogNum), MaxV = -log(M_MinDouble2);
        if (fabs(Coef)<MaxV)
            Num = Num * exp(Coef);
        return Num;
    }
    VecCl FreeELogNumDeriv(const Param &par_, const VecCl &LogNum){
        Param par = par_;
        par.RelatNumbers = LogNum2RelatNum(LogNum);
// We are not calculating FreeE of subs on each step!!!!!!!!!!!!! - may be error - but faster....
        par.FreeEPure = FreeEVectorPure(par);

        VecCl pure = VecCl::ComponentMul(par.FreeEPure,SubstVeights);
        double V0 = par.RelatNumbers*par.RelatVolumesCoefs;
        VecCl Cor(SubstVeights.Dim());
		for(int k=1;k<=SubstVeights.Dim();k++)
            Cor[k] = LogNum[k] + log(max(par.RelatVolumesCoefs[k]/V0,MathZer));
		double kt_Mole=M_Rconst*par.Temp;
        Cor = Cor*kt_Mole;
        VecCl FreeECor = pure + Cor;
        return FreeENumDeriv(par, FreeECor);

    }
    VecCl FreeENumDeriv(const Param &par_, const VecCl &FreeELogNumCor_){
        Param par = par_;
// We are not calculating FreeE - do it on the higher level - if needed
        //par.FreeEPure = FreeEVectorPure(par);
//FreeECor[k]*M0/par.RelatNumbers[k]
//cout<<" FreeELogNumCor_ "<<FreeELogNumCor_;
        VecCl FreeECor = FreeELogNumCor_;
        VecCl MassCoef = ClcMassCoef(par);
        double M0 = par.RelatNumbers * SubstVeights;
        double V0 = par.RelatNumbers*par.RelatVolumesCoefs;
        double SumN_V0 = par.RelatNumbers*(par.RelatNumbers*0+1)/V0;
        double FreeEFull = FreeEMixt(par);
		double kt_Mole=M_Rconst*par.Temp;
        double VMulCoef = M0/V0/par.Denc;
        VecCl ret(FreeECor.Dim());
		for(int k=1;k<=ret.Dim();k++){
            ret[k] = kt_Mole*(1-par.RelatVolumesCoefs[k]*SumN_V0 ) + 
                //FreeECor[k]*M0/par.RelatNumbers[k] - FreeEFull*SubstVeights[k] -
                FreeECor[k] - FreeEFull*SubstVeights[k] -
                par.Pressure*(SubstVeights[k]/par.Denc-VMulCoef*par.RelatVolumesCoefs[k]);
        }
//cout<<" FreeENumDeriv::ret "<<ret;
        return ret/M0;
    }

    VecCl ClcNumDerive(const Param &par, int FreeEDerive){
        Param p1=par,p2=par, p=par; 
        if (FreeEDerive)
            p.FreeEPure = FreeEVectorPure(p);
        double Ex = FreeEMixt(p);
        VecCl d(SubstVeights.Dim());
        for(int k=1;k<=d.Dim();k++) {
            p2.RelatNumbers=p1.RelatNumbers = par.RelatNumbers;
            double stp = M_Eps2*10;
            p2.RelatNumbers[k]+=stp;
            p1.RelatNumbers[k]-=stp;
            if (FreeEDerive){
                p2.FreeEPure = FreeEVectorPure(p2);
                p1.FreeEPure = FreeEVectorPure(p1);
            }
            //d[k] = (FreeEMixt(p2)-Ex)/stp;
            d[k] = (FreeEMixt(p2)-FreeEMixt(p1))/stp/2;
        }
        return d;
    }
// for 1 gramm of final matter  ( RelatNumbers  RelatVolumesCoefs  Temp  )
	VecCl GetMixtureCorrection(const Param &par){
		double kt_Mole=M_Rconst*par.Temp;
        double V0 = par.RelatNumbers*par.RelatVolumesCoefs;
        double M0 = par.RelatNumbers*SubstVeights;
        VecCl Cor(SubstVeights.Dim());
		for(int k=1;k<=SubstVeights.Dim();k++){
            //Cor[k] = par.RelatNumbers[i]*log(max(par.RelatNumbers[i]*par.RelatVolumesCoefs[i]/V0,MathZer))/SubstVeights[k];
            Cor[k] = par.RelatNumbers[k]*log(max(par.RelatNumbers[k]*par.RelatVolumesCoefs[k]/V0,MathZer));
        }
        Cor = Cor*kt_Mole/M0;
		return Cor;
	}
    VecCl Veights(){
        return SubstVeights;
    }
    DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > GetChemSubst(){
        return ChemSubst;
    };
private:
    VecCl SubstVeights;
	DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;

};

// Minimization base class
struct PureDis_Minim_Base:SavableClass{
    virtual VecCl Do(double Denc, double Temp, double Pressure, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs, const VecCl &FreeE_NoMixt, PureDis_StrictClc *FreeEMixt, const VecCl &ChemCoefs)=0;
};


struct PureDis_MinimNum:PureDis_Minim_Base{
    PureDis_MinimNum():PureDis_Minim_Base(),StartGuess(0.5), MaxVal(-log(M_MinDouble2)), MinVal(log(M_MinDouble2)), ErrorAbs(M_Eps2), ErrorRel(M_Eps2), MaxIter(50){
        //MinVal = Encode(M_Eps*1000);
        //MaxVal = Encode(1-M_Eps*1000);
    };

    int CheckLarge(PureDis_StrictClc::Param *par, double &Guess){
		double kt_Mole=M_Rconst*par->Temp;
        VecCl veight = PureDisClc->Veights();
        double M0 = veight*(veight*0+1);
        double Coef = (par->FreeEPure[1]*veight[1]-par->FreeEPure[2]*veight[2])/(M_Rconst*par->Temp*M0);
//        if (fabs(Coef)<100){
//            Guess = 0.5;
//            return 0;
//        }
        double Val = log(100.);
        if (ClcRelatNumbers(ChemCoefs, MaxVal)[1]>0)
            Guess = (Coef>0)? -Val:Val;
        else
            Guess = (Coef>0)? Val:-Val;
        return 0;
//cout<<"Coef "<<Coef<<" Masses "<<ClcRelatNumbers(ChemCoefs, Decode(Guess));
//        return 1;
    }
    virtual VecCl Do(double Denc, double Temp, double Pressure, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs, const VecCl &FreeE_NoMixt, PureDis_StrictClc *ClcFreeEMixt, const VecCl &chemcoefs){
        curPar  = new PureDis_StrictClc::Param(Denc, Temp, Pressure, RelatNumbers, RelatVolumesCoefs, FreeE_NoMixt);
        PureDisClc = ClcFreeEMixt;
        ChemCoefs = chemcoefs;
        curPar->FreeEPure = PureDisClc->FreeEVectorPure(*curPar);
        double Guess = Encode(StartGuess);
        if (CheckLarge(curPar, Guess))
            return ClcRelatNumbers(ChemCoefs, Decode(Guess));
        int err;
        PureDis_MinimNum *OldPtr = StaticPtr;
		StaticPtr = this;
        //if (4<(err=Fzero(ExecuteStatic, Encode(MinVal) ,Encode(MaxVal) , Guess, ErrorAbs, ErrorRel, MaxIter)))
        if (4<(err=Fzero(ExecuteStatic, MinVal, MaxVal, Guess, ErrorAbs, ErrorRel, MaxIter)))
			{cout<<" Bad FZero PureDis_MinimNum in PureDis_MinimNum::PureDis_MinimNum \nfound X "
				<<Decode(Guess)<<" Mis "<<ExecuteStatic(Guess)<<" err "<<err<<"\n";}
double Mis = ExecuteStatic(Guess);
        StaticPtr = OldPtr;
        double LogN;
        int ShiftFromUnit;
        DecodeLog(Guess, LogN, ShiftFromUnit);
        VecCl LogNum = ClcLogNumbers(ChemCoefs, LogN, ShiftFromUnit);
        curPar->RelatNumbers = PureDis_StrictClc::LogNum2RelatNum(LogNum);

		//double ret = Decode(Guess);
//if (Mis>1e-3)
//cout<<" CodeReact "<<Guess<<" Mis "<<Mis<<" RelN "<<curPar->RelatNumbers<<"\n";
        return curPar->RelatNumbers;
    }
private:
    static PureDis_MinimNum*StaticPtr;
    Ref<PureDis_StrictClc::Param> curPar;
    VecCl ChemCoefs;
    Ref<PureDis_StrictClc> PureDisClc;
    double StartGuess, ErrorAbs, ErrorRel, MaxIter, MinVal, MaxVal;


    static double ExecuteStatic(double x){
        return StaticPtr->Execute(x);
    }
    double Encode(double x){
        double MaxD = fabs(log(M_MinDouble2));
        x = ( (x<=M_MinDouble2)?M_MinDouble2:((x>=1-M_Eps)?1-M_Eps:x) );
        x = log(x/(1-x));
        return x;
    };
    double Decode(double par){
        double MaxP = fabs(log(M_Eps*10));
        if (fabs(par)>MaxP)
            par = (par>0)?1:0;
        else
            par = 1/(1+exp(-par));
        //if (fabs(par)<1e-17)
        //    return 0;
        return par;
    };
    void DecodeLog(double par, double &LogX, int &ShiftFromUnit ){
        ShiftFromUnit = 0;
        double MaxP = fabs(log(M_Eps*1000));
        if (par<-MaxP){
            LogX = par;
            return;
        }
        if (fabs(par)<=MaxP){
            LogX = par - log(1+exp(par));
            return;
        }
        //if (par>MaxP)
        ShiftFromUnit = 1;
        LogX = -par;
    };



private:
    VecCl ClcRelatNumbers(const VecCl &ChemCoefs, double ReactProp){
        VecCl RelN(ChemCoefs.Dim());
        for(int k = 1;k<=RelN.Dim();k++)
                RelN[k] = (ChemCoefs[k]>0)?ChemCoefs[k]*ReactProp:ChemCoefs[k]*(ReactProp-1);
        return RelN;
    }
    VecCl ClcLogNumbers(const VecCl &ChemCoefs, double LogProp, int Invert){
        VecCl RelNLog(ChemCoefs.Dim());
        double MaxP = fabs(log(M_Eps*10));
        if (!Invert){
            for(int k = 1;k<=RelNLog.Dim();k++){
                double add = 0;
                if (ChemCoefs[k]>0)
                    add = LogProp;
                else
                    if (LogProp>-MaxP)
                        add = log(1-exp(LogProp));
                    else 
                        add = 0;
                RelNLog[k] = log(fabs(ChemCoefs[k])) + add;
                //RelNLog[k] = log(fabs(ChemCoefs[k])) + (ChemCoefs[k]>0)?LogProp:((LogProp>-MaxP)?log(1-exp(LogProp)):0);
            }
        }
        else{
            for(int k = 1;k<=RelNLog.Dim();k++) {
                double add = 0;
                if (ChemCoefs[k]<0)
                    add = LogProp;
                else
                    if (LogProp>-MaxP)
                        add = log(1-exp(LogProp));
                    else 
                        add = 0;
                RelNLog[k] = log(fabs(ChemCoefs[k])) + add;
                //RelNLog[k] = log(fabs(ChemCoefs[k])) + (ChemCoefs[k]<0)?LogProp:((LogProp>-MaxP)?log(1-exp(LogProp)):0);
            }
        }

        return RelNLog;
    }
    double Execute(double par){
        double LogN;
        int ShiftFromUnit;
        DecodeLog(par, LogN, ShiftFromUnit);
        VecCl LogNum = ClcLogNumbers(ChemCoefs, LogN, ShiftFromUnit);
        curPar->RelatNumbers = PureDis_StrictClc::LogNum2RelatNum(LogNum);
        VecCl DerV(PureDisClc->FreeELogNumDeriv(*curPar, LogNum));
        //VecCl DerV(PureDisClc->FreeENumDeriv(*curPar));
//cout<<" Log Der "<<DerV<<" Log RelNum "<<curPar->RelatNumbers;
//        curPar->RelatNumbers = ClcRelatNumbers(ChemCoefs, Decode(par));
//        DerV = PureDisClc->FreeENumDeriv(*curPar);
//cout<<" Norm Der "<<DerV<<" Norm RelNum "<<curPar->RelatNumbers;
//DerV = PureDisClc->FreeELogNumDeriv(*curPar, LogNum);
//cout<<" par "<<par<<" react "<<x<<" num "<<curPar->RelatNumbers<<" DerV "<<DerV<<" res "<<DerV*ChemCoefs<<"\n";
//cout<<" par "<<par<<"\n logNum "<<LogNum<<" num "<<curPar->RelatNumbers<<" res "<<DerV*ChemCoefs<<" DerV "<<DerV<<"\n";
//cout<<" CheCoefs "<<ChemCoefs;
        return DerV*ChemCoefs;
    }

};







//struct PureDis_MinimNum:PureDis_Minim_Base{
//    PureDis_MinimNum():PureDis_MinimNum_Base(){
//    };
//
//    VecCl ClcRelatNumbers(const VecCl &ChemCoefs, double ReactProp){
//        VecCl RelN(ChemCoefs.Dim());
//        for(int k = 1;k<=RelN.Dim();k++)
//                RelN[k] = (ChemCoef[k]>0)?ChemCoef[k]*ReactProp:ChemCoef[k]*(ReactProp-1);
//        return RelN;
//    }
//    virtual VecCl Do(double Denc, double Temp, double Pressure, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs, const VecCl &FreeE_NoMixt, PureDis_StrictClc *ClcFreeEMixt, const VecCl &chemcoefs){
//        return FindRelatNumbers(Denc, Temp, Pressure, RelatNumbers, RelatVolumesCoefs, FreeE_NoMixt, ClcFreeEMixt, chemcoefs);
//    }
//    virtual VecCl FindRelatNumbers(double Denc, double Temp, double Pressure, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs, const VecCl &FreeE_NoMixt, PureDis_StrictClc *ClcFreeEMixt, const VecCl &chemcoefs){
//        curPar  = new PureDis_StrictClc::Param(Denc, Temp, Pressure, RelatNumbers, RelatVolumesCoefs, FreeE_NoMixt);
//        PureDisClc = ClcFreeEMixt;
//        ChemCoefs = chemcoefs;
//        double Guess = 0.5;
//        Guess = Encode(Guess);
//        double ErrorAbs, ErrorRel;ErrorRel=ErrorAbs=1e-8;
//        int err, MaxIter = 100;
//        PureDis_MinimNum *OldPtr = StaticPtr;
//		StaticPtr = this;
//        if (4<(err=Fzero(ExecuteStatic, -35 ,35 , Guess, ErrorAbs, ErrorRel, MaxIter)))
//			{cout<<" Bad FZero FindDissMass1 in SingleDiss_MinimFunc::FindDissMass1 \nfound X "
//				<<Decode(Guess)<<" Mis "<<ExecuteStatic(Guess)<<" err "<<err<<"\n";}
//        StaticPtr = OldPtr;
//		double ret = Decode(Guess);
//        return ClcRelatNumbers(ChemCoefs, ret);
//    }
//private:
//    static Dis_RelatNum_MinimFunc_Single *StaticPtr;
//    Ref<PureDis_StrictClc::Param> curPar;
//    VecCl ChemCoefs;
//    Ref<PureDis_StrictClc> PureDisClc;
//
//
//    //VecCl FindRelatNumbersRet(double x){
//    //    VecCl Res(2);Res[1] = x;Res[2] = 1-x;
//    //    return Res;
//    //}
//    static double ExecuteStatic(double x){
//        return StaticPtr->Execute(x);
//    }
//    double Execute(double par){
//        double x = Decode(par);
//        curPar->SetRelatNumbers(ClcRelatNumbers(ChemCoefs, Decode(x)));
//        VecCl DerV(PureDisClc->FreeENumDeriv(curPar));
//
//        return DerV*ChemCoefs;
//    }
//    static double Encode(double x){
//        x = x<=MathZer?MathZer:(x>=1-MathZer?1-MathZer:x);
//        x = log(x/(1-x));
//        return x;
//    };
//    static double Decode(double par){
//        par = 1/(1+exp(-par));
//        if (fabs(par)<1e-17)
//            return 0;
//        return par;
//    };
//};


struct PureDis_Minim1Pres:PureDis_Minim_Base{
    PureDis_Minim1Pres():PureDis_Minim_Base(),StartGuess(1), MaxVal(2), MinVal(0.5), ErrorAbs(M_Eps2), ErrorRel(M_Eps2), MaxIter(100), FixedVolumeCoefs(0), MaxDenc(1e5){
        MinimNum.SetDim(2);
        MinimNum[1] = 1;
        MinimNum[2] = 2;
    };
    int read_data_state(FilterIn &si){
        char tmp[256];
        si>>tmp>>MinVal>>tmp>>MaxVal>>tmp>>MaxDenc>>tmp>>FixedVolumeCoefs;
        if (!si)
            throw info_except("Could not read class\n");
        return 1;
    }
    int save_data_state(FilterOut &so){
        so<<" MinVal "<<MinVal<<" MaxVal "<<MaxVal<<" MaxDenc "<<MaxDenc<<" FixedVolumeCoef "<<FixedVolumeCoefs;
        if (!so)
            throw info_except("Could not save class\n");
        return 1;
    }
    Stroka MakeHelp(){
        Stroka res = "Class for finding volume coefs of 2 matters...";
        return res;
    }



    void SetMinimNum(const VecCl &minimNum){
        MinimNum = minimNum;
    };
    virtual VecCl Do(double Denc, double Temp, double Pressure, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs, const VecCl &FreeE_NoMixt, PureDis_StrictClc *ClcFreeEMixt, const VecCl &chemcoefs){
        StartGuess = SetStartGuess(RelatVolumesCoefs);
        curPar  = new PureDis_StrictClc::Param(Denc, Temp, Pressure, RelatNumbers, RelatVolumesCoefs, FreeE_NoMixt);
        PureDisClc = ClcFreeEMixt;
        ChemCoefs = chemcoefs;
        if (FixedVolumeCoefs!=0) {
            //PureDis_Minim1Pres*OldPtr = StaticPtr;
		    //StaticPtr = this;
            VecCl ret = ClcResult(FixedVolumeCoefs);
            //StaticPtr = OldPtr;
            return ret;
        }
        double Guess = Encode(StartGuess);
        int err;
        PureDis_Minim1Pres*OldPtr = StaticPtr;
		StaticPtr = this;
        InitMinim();
        if (4<(err=Fzero(ExecuteStatic, Encode(MinVal) ,Encode(MaxVal) , Guess, ErrorAbs, ErrorRel, MaxIter)))
			{cout<<" Bad FZero PureDis_Minim1Pres in SingleDiss_MinimFunc::PureDis_Minim1Pres \nfound X "
				<<Decode(Guess)<<" Mis "<<ExecuteStatic(Guess)<<" err "<<err<<"\n";}
        StaticPtr = OldPtr;
		double ret = Decode(Guess);
//VecCl VolCoef = ClcResult(ret);
//VecCl pres = PureDisClc->PressureVector(*curPar);
//VecCl denc = PureDisClc->ClcSubstDenc(*curPar);
//if ((pres[1]-pres[2])/max(pres[1],M_Eps2)>1e-3)
//cout<<" temp "<<curPar->Temp<<" meanRo "<<curPar->Denc<<"\nDencVec "<<denc<<" pres "<<pres<<" volC "<<VolCoef; 
        return ClcResult(ret);
    }
private:
    static PureDis_Minim1Pres *StaticPtr;
    Ref<PureDis_StrictClc::Param> curPar;
    VecCl ChemCoefs;
    Ref<PureDis_StrictClc> PureDisClc;
    double StartGuess, ErrorAbs, ErrorRel, MaxIter, MinVal, MaxVal, MaxDenc, FixedVolumeCoefs;


    static double ExecuteStatic(double x){
        return StaticPtr->Execute(x);
    }
    double Encode(double x){
        //x = x<=MathZer?MathZer:x;
        //x = log(x);
        return x;
    };
    double Decode(double par){
        //par = exp(par);
        return par;
    };
    double EncodeRes(const VecCl &res){
        if (VecCl::Min(res)+ AddPressure<M_Eps2){
            AddPressure = -VecCl::Min(res)*1.1;
cout<<"Shifting AddPressure...\n";
        }
        VecCl tmp = res + AddPressure;
        return log(tmp[1]/tmp[2]);
    };



private:
    VecCl MinimNum;
    double AddPressure;
    void InitMinim(){
        VecCl presMin = GetPressures(MinVal),presMax = GetPressures(MaxVal);
        double minV = min(VecCl::Min(presMin),VecCl::Min(presMax));
        if (minV<M_Eps2+10)
            AddPressure = -minV*1.1+1;
        else
            AddPressure = 0;
    }
    double Execute(double par){
        double x = Decode(par);
        VecCl pres = GetPressures(x);
//cout<<" par "<<par<<" x "<<x<<" pres "<<pres<<" res "<<EncodeRes(pres)<<"\n";
        return EncodeRes(pres);
    }
    VecCl GetPressures(double VolCoef){
        ClcResult(VolCoef);
        //VecCl pres = PureDisClc->PressureVector(*curPar);
		VecCl SubstDenc = PureDisClc->ClcSubstDenc(*curPar);
		VecCl pres(2);
		for (int k=1;k<=pres.Dim();k++){
            pres[k] = PureDisClc->GetChemSubst()[k-1]->Pressure(min(SubstDenc[k], MaxDenc), curPar->Temp);
		}
        VecCl res = MinimNum;
        res[1] = pres[res[1]];
        res[2] = pres[res[2]];
        return res;
    }
    VecCl ClcResult(double VolCoef){
        curPar->RelatVolumesCoefs[MinimNum[2]] = curPar->RelatVolumesCoefs[MinimNum[1]]*VolCoef;
        return curPar->RelatVolumesCoefs;
    }
    double SetStartGuess(const VecCl &startval){
        double SGuess = startval[MinimNum[2]]/max(startval[MinimNum[1]],M_Eps);
        SGuess = max(min(StartGuess, MaxVal),MinVal);
        return SGuess ;
    }

};











//
//struct DisFindMinimE_Base;

// ======================   Old classes start here  ==============================
// ===============================================================================
struct DisMixture_Energy:RefCount{
    DisMixture_Energy(const DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &chemsubst, const VecCl &substveights)
        :ChemSubst(chemsubst), SubstVeights(substveights){};
//do not work
    double ClcFreeE(double Temp, VecCl &Denc, VecCl &AtomCoefs){
        return 0;
    }
//Calculate vector of dencities of pure subst
	VecCl ClcSubstDenc(double Denc,  const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
		double Coef = (RelatNumbers*RelatVolumesCoefs)/(RelatNumbers*SubstVeights)*Denc;
		VecCl tmp(RelatVolumesCoefs);
		VecCl SubstDenc =  VecCl::ComponentDiv(SubstVeights,tmp) * Coef;
		return SubstDenc;
	}
	VecCl ClcMassCoef(const VecCl &RelatNumbers){
		double Coef = RelatNumbers*SubstVeights;
		//VecCl MassCoef = VecCl::ComponentMul(RelatNumbers, SubstVeights) / Coef;
		VecCl MassCoef = VecCl::ComponentMul(RelatNumbers, SubstVeights);
		MassCoef = MassCoef/(MassCoef*(MassCoef*0+1));
		return MassCoef;
	}
//Calculate vector of pressures of pure subst
    VecCl PressureVector(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
//cout<<" Denc "<<Denc<<" Temp "<<" RelatNum "<<RelatNumbers<<" RelatVolCoef "<<RelatVolumesCoefs;
		VecCl SubstDenc = ClcSubstDenc(Denc,  RelatNumbers, RelatVolumesCoefs);
//cout<<" SubstDenc "<<SubstDenc;
		VecCl pres(SubstVeights.Dim());
		for (int k=1;k<=pres.Dim();k++){
			pres[k] = ChemSubst[k-1]->Pressure(SubstDenc[k], Temp);
		}
//cout<<" ResultPressures!! "<<pres<<flush;
        return pres;
	}
//Calculate vector of FreeE of pure subst
	VecCl FreeEVectorPure(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
		VecCl SubstDenc = ClcSubstDenc(Denc,  RelatNumbers, RelatVolumesCoefs);
		VecCl ret = SubstVeights*0;
		for(int k=1;k<=SubstVeights.Dim();k++)
			ret[k] = ChemSubst[k-1]->FreeE(SubstDenc[k],Temp);
		return ret;			
	}
	VecCl FreeEVectorCor(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
		VecCl SubstDenc = ClcSubstDenc(Denc,  RelatNumbers, RelatVolumesCoefs);
		VecCl ret = SubstVeights*0;
		for(int k=1;k<=SubstVeights.Dim();k++){
			ret[k] = ChemSubst[k-1]->FreeE(SubstDenc[k],Temp);
			ret[k] += GetMixtureCorrection(RelatNumbers[k],Temp,SubstVeights[k]);
			//ret[k] -= GetMixtureCorrection(RelatNumbers[k],Temp,SubstVeights[k]);
		}
		return ret;			
	}
//Clc Full FreeE known strict vals
	double FreeECor(const VecCl &FreeEPure, const VecCl &RelatNumbers, double Temp){
        double ret = 0;
        VecCl Mas = ClcMassCoef(RelatNumbers);
		for(int k=1;k<=SubstVeights.Dim();k++){
			ret += Mas[k]*(FreeEPure[k] + GetMixtureCorrection(RelatNumbers[k],Temp,SubstVeights[k]));
		}
		return ret;			
	}
    

	static double GetMixtureCorrection(double RelatNumber, double Temp, double SubstVeight){
		double kt=M_Rconst/SubstVeight*Temp;
        double F=kt*log(max(RelatNumber,MathZer));
		return F;
	}
    VecCl Veights(){
        return SubstVeights;
    }
private:
    VecCl SubstVeights;
	DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
};


struct Dis_RelatNum_MinimFunc_Base:SavableClass{
    virtual VecCl FindRelatNumbers(const VecCl &FreeE_NoMixt, double Temp, const VecCl &RelatNumbers, const VecCl &veights)=0;
};

struct Dis_RelatNum_MinimFunc_Single:Dis_RelatNum_MinimFunc_Base{
    Dis_RelatNum_MinimFunc_Single():Dis_RelatNum_MinimFunc_Base(){
    };

    VecCl FindRelatNumbers(const VecCl &FreeE_NoMixt, double Temp, const VecCl &RelatNumbers, const VecCl &veights){
        SetVeights(veights);
		double arg = (FreeE_NoMixt[1]-FreeE_NoMixt[2])*Veights[1]*Veights[2]/(M_Rconst*Temp);
		if (arg>100)
			return FindRelatNumbersRet(0);
		if (arg<-100)
			return FindRelatNumbersRet(1);
        double ChemCoef = exp(arg);
        Find_ChemCoef = ChemCoef;
        double Guess = (RelatNumbers[1]<=0||RelatNumbers[1]>=1)?0.5:RelatNumbers[1];
        Guess = Encode(Guess);
        double ErrorAbs, ErrorRel;ErrorRel=ErrorAbs=1e-8;
        int err, MaxIter = 100;
        Dis_RelatNum_MinimFunc_Single *OldPtr = StaticPtr;
		StaticPtr = this;
        if (4<(err=Fzero(ExecuteStatic, -35 ,35 , Guess, ErrorAbs, ErrorRel, MaxIter)))
			{cout<<" Bad FZero FindDissMass1 in SingleDiss_MinimFunc::FindDissMass1 \nfound X "
				<<Decode(Guess)<<" Mis "<<ExecuteStatic(Guess)<<" err "<<err<<"\n";}
        StaticPtr = OldPtr;
		double ret = Decode(Guess);
        return FindRelatNumbersRet(ret);
    }
private:
    static Dis_RelatNum_MinimFunc_Single *StaticPtr;
    VecCl Veights;
    double Find_ChemCoef;

    void SetVeights(const VecCl &veights){
        Veights = veights;
        if (Veights.Dim()!=2)
            throw info_except(" ReactCoef_DissMinimFunc -> works only in 2 dims. Trying to set init dim=%i\n",Veights.Dim());
    }
    VecCl FindRelatNumbersRet(double x){
        VecCl Res(2);Res[1] = x;Res[2] = 1-x;
        return Res;
    }
    static double ExecuteStatic(double x){
        return StaticPtr->Execute(x);
    }
    double Execute(double par){
        double x = Decode(par);
        double ret = pow(1-x,Veights[1])*pow(x,-Veights[2])-Find_ChemCoef;
        return ret;
    }
    static double Encode(double x){
        x = x<=MathZer?MathZer:(x>=1-MathZer?1-MathZer:x);
        x = log(x/(1-x));
        return x;
    };
    static double Decode(double par){
        par = 1/(1+exp(-par));
        if (fabs(par)<1e-17)
            return 0;
        return par;
    };
};




struct Dis_RelatNum_MinimFunc_SingleH2:Dis_RelatNum_MinimFunc_Base{
    Dis_RelatNum_MinimFunc_SingleH2():Dis_RelatNum_MinimFunc_Base(){
    };

    VecCl FindRelatNumbers(const VecCl &FreeE_NoMixt, double Temp, const VecCl &RelatNumbers, const VecCl &veights){
        SetVeights(veights);
		double arg = (FreeE_NoMixt[1]-FreeE_NoMixt[2])*Veights[1]*Veights[2]/(M_Rconst*Temp);
		if (arg>100)
			return FindRelatNumbersRet(0);
		if (arg<-100)
			return FindRelatNumbersRet(1);
        double Xa = 2/(1+sqrt(1+4*exp(arg)));
        //double Xa = 1/sqrt(1+4*exp(arg));
        return FindRelatNumbersRet(Xa);

    }
private:
    VecCl Veights;
    double Find_ChemCoef;

    void SetVeights(const VecCl &veights){
        Veights = veights;
        if (Veights.Dim()!=2)
            throw info_except(" ReactCoef_DissMinimFunc -> works only in 2 dims. Trying to set init dim=%i\n",Veights.Dim());
    }
    VecCl FindRelatNumbersRet(double x){
        VecCl Res(2);Res[1] = x;Res[2] = 1-x;
        //VecCl Res(2);Res[1] = 2*x/(1+x);Res[2] = 1-Res[1];
        return Res;
    }
};
// Any number dissosiation minimization function
//struct MatrDiss_MinimFunc:RefCount{
//    VecCl DissDenc, Veights;
//    MatrDiss_MinimFunc(const VecCl &veights)
//        :Veights(veights){};
//	double GetMisfit(double Denc, double Temp, const VecCl &FreeE_NoMixt, VecCl &ResDenc, int LowInd, int UpInd){
//		VecCl veight(2);veight[1] = Veights[LowInd];veight[2] = Veights[UpInd];
//		VecCl free(2);free[1] = FreeE_NoMixt[LowInd];free[2] = FreeE_NoMixt[UpInd];
//		double denc = ResDenc[LowInd] + ResDenc[UpInd];
////cout<<" Start Denc "<<ResDenc<<"denc "<<denc<<"\n";
//		ResDenc[LowInd] = SingleDiss_MinimFunc(veight).FindDissFstDenc(free,denc,Temp,ResDenc[LowInd]);
////cout<<" res1Denc "<<ResDenc[LowInd];
//		double mis = ResDenc[UpInd]+ResDenc[LowInd]-denc;
//		ResDenc[UpInd] = denc - ResDenc[LowInd];
////cout<<" res2Denc "<<ResDenc[UpInd];
////		return SingleDiss_MinimFunc::Encode(mis);
//		return fabs(mis);
//	}
//	VecCl FindDissDenc(double Denc, double Temp, const VecCl &FreeE_NoMixt, VecCl *GuessDenc=NULL){
//		VecCl ResDenc;
//		if (FreeE_NoMixt.Dim()<1)
//			return VecCl(0);
//		if (FreeE_NoMixt.Dim()==1)
//			return VecCl(Denc);
//		if (GuessDenc)
//			ResDenc = (*GuessDenc);
//		else
//			ResDenc = FreeE_NoMixt*0 + Denc/FreeE_NoMixt.Dim();
//		vector<int> SortInd = VecCl::Sort(FreeE_NoMixt);
//		double Misfit=1;
//		while (Misfit>StndErr){
//			Misfit = 0;
//			for(size_t	k=1;k<SortInd.size();k++)
//				Misfit += GetMisfit(Denc, Temp, FreeE_NoMixt, ResDenc, SortInd[0], SortInd[k]); 
//		}
//		DissDenc = ResDenc;
//		return ResDenc;
//	}
//	static double GetMixtureFreeE(const VecCl &FreeE_NoMixt, double Denc, double Temp, const VecCl &Veights, const VecCl &DissDenc){
//		double ret = 0;
//		for (int k=1;k<=FreeE_NoMixt.Dim();k++)
//			ret += (FreeE_NoMixt[k]+SingleDiss_MinimFunc::GetMixtureCorrection(DissDenc[k],Temp,Veights[k]))*DissDenc[k];
//		return ret/Denc;
//	}
//};
//
//
//
//
// Base class for next minimization functions
struct DisSamePresFunc{
//Used by DisSamePresFinder
   	virtual VecCl Pressures(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs)=0;

//Used by ReactCoefMinimFunc
	//virtual double ChemMulti_FreeE(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs)=0;
 //   virtual VecCl ChemMulti_ClcRelatNumbers(const VecCl &ReactCoef)=0;

};
// minimazing volumes.
struct DisSamePresFinder:RefCount{
    DisSamePresFinder(DisSamePresFunc *freeclc, double denc, double temp,
        const VecCl &relatnumbers, const VecCl &relatvolumescoefs)
        :FreeClc(freeclc), Denc(denc), Temp(temp), RelatNumbers(relatnumbers){
            if (RelatVolumesCoefs.Dim()!=relatvolumescoefs.Dim())
                RelatVolumesCoefs=relatvolumescoefs;
            RelatNumbers = RelatNumbers/sqrt(RelatNumbers*RelatNumbers);
            VolMinimCvt.clear();
            for(int i=1;i<=RelatNumbers.Dim();i++){
                VolMinimCvt.push_back(i);
                RelatNumbers[i] = max(pow(MathZer,0.25), RelatNumbers[i]);
            }
        };
    ~DisSamePresFinder(){CurPtr = OldPtr;};

    VecCl Minimize(){
        if (VolMinimCvt.size()==1){
            return RelatVolumesCoefs*0+1;
        }
        OldPtr = CurPtr;CurPtr = this;
        VecCl RelatVol = RelatVolumesCoefs;
        ToAddPressure=0;
        VecCl tmp = FreeClc->Pressures(Denc, Temp, RelatNumbers, RelatVol);
        double mi = VecCl::Min(tmp);
        ToAddPressure = mi>0?0:(-mi+5)*20;
        
        double StartT = Temp, T = Temp;
		int i;
        for(i=1;i<=20;i++)
            if (MinimizeTemp(T, RelatVol)){
                //if (i>1)
                //    PrintErr(Encode(RelatVol));
                break;
            }
            else{
                T = T*1.1+5;
                RelatVol = RelatVolumesCoefs;
            }
        if (i>=20)
            PrintErr("Too many iterations ",Encode(RelatVol));
        else if (i>1){
//cout<<"Temp+ found good, trying to find exact temp...\n";
//PrintErr(Encode(RelatVol));
            T = StartT;Temp = StartT;
//PrintErr(Encode(RelatVol));
            VecCl Best = RelatVol;
            if (MinimizeTemp(T, Best)|| MinimizeTemp(T, Best)|| MinimizeTemp(T, Best))
                RelatVol = Best;
            else {
                VecCl par;double err;
                TestOK(Encode(RelatVol), Encode(Best), par, err);
                RelatVol = Decode(par);
                PrintErr("Could not recalculate for start temp",Encode(RelatVol));
            }
//cout<<"found ::\n";                
//PrintErr(Encode(Best));
        }
        Temp = StartT;
        RelatVol = RelatVol/sqrt(RelatVol*RelatVol);
        RelatVolumesCoefs = RelatVol;
        CurPtr = OldPtr;
        return RelatVol;
    }
private:
    static DisSamePresFinder *CurPtr;
    DisSamePresFinder *OldPtr;

    double Denc,Temp;
    VecCl RelatNumbers, RelatVolumesCoefs;
    vector<int> VolMinimCvt;
    DisSamePresFunc *FreeClc;
    double ToAddPressure;
    static void ExecuteStatic(VecCl &par, VecCl &res){
        CurPtr->Execute(par,res);
    }
    void Execute(const VecCl &par, VecCl &res){
        VecCl par_(par);
        for(int k=1;k<=par.Dim();k++)
            par_[k] = fabs(par[k])>50?(par[k]>0?50:-50):par[k];
        par_ = Decode(par_);
//cout<<" par "<<par<<" Decoded pars "<<par_;
        res = FreeClc->Pressures(Denc, Temp, RelatNumbers, par_)+ToAddPressure;
//cout<<" res "<<res;
        res = EncodeRes(res);
//cout<<" Encoded res "<<res<<flush;
    }
    VecCl Encode(const VecCl &par){
        VecCl res((int)VolMinimCvt.size());
        for(size_t i=0;i<VolMinimCvt.size();i++)
            res[(int)i+1] = par[VolMinimCvt[i]];
        for(int k=1;k<=res.Dim();k++)
            res[k] = log(res[k]);
        return res;
    }
    VecCl Decode(const VecCl &par){
        VecCl res(par.Dim());
        for(int k=1;k<=par.Dim();k++)
            res[k] = fabs(par[k])>log(1/MathZer)?(par[k]>0?1/MathZer:MathZer):exp(par[k]);
//              res[k] = exp(par[k]);
        VecCl res1 = RelatVolumesCoefs;
        for(size_t i=0;i<VolMinimCvt.size();i++)
            res1[VolMinimCvt[i]] = res[(int)i+1];
        return res1;
    }
    VecCl EncodeRes(const VecCl &par){
        if (par.Dim()==1)
            return 0;
        VecCl res_((int)VolMinimCvt.size());
        for(size_t i=0;i<VolMinimCvt.size();i++)
            res_[(int)i+1] = par[VolMinimCvt[i]];
        VecCl res(res_.Dim()-1), par_;
        double minim = VecCl::Min(res_);
        if (minim<=0)
            par_ = res_ -(minim-1)*(1+0.1);
        else
            par_ = res_;
//cout<<" making same sign minim "<<minim<<"\nwas "<<res_<<"now "<<par_<<"\n";
        for(int k=1;k<=res.Dim();k++)
            res[k] =  10*log(par_[k+1]/par_[k]) ;
        return res;
    }
    int Static1D_N;
    VecCl Static1D_Par;

    double OneDMinimmize(double x){
        Static1D_Par[Static1D_N] = x;
        VecCl res;
        Execute(Static1D_Par, res);
        return res[Static1D_N];
    };
    static double OneDMinimmizeStatic(double x){
        return CurPtr->OneDMinimmize(x);
    }
    VecCl Minimize1D(const VecCl &MinimPar){
        VecCl res;
        Static1D_Par = MinimPar;
        for(Static1D_N=1;Static1D_N<MinimPar.Dim();Static1D_N++){
            double cv = MinimPar[Static1D_N];
            Fzero(OneDMinimmizeStatic,cv-10, cv+10,cv, 1e-7, 1e-7, 1000);
        }
        return Static1D_Par;
    }
    VecCl MinimizeND(const VecCl &MinimPar_){
        VecCl y = RelatNumbers*0, w, MinimPar = MinimPar_;
        y = EncodeRes(y);w = y*0 + 1;
        double FuncError=1e-4, ResError=FuncError;
        double Min1D_Coef=1.2;
        int Min1D_MaxLinSrch=5,Min1D_MaxRetry=5;
        MinFindNDimClass MinNd(
            new EigenVectMinim(new Derive2DHi2(w,y,ExecuteStatic),
                                new MinFind1DClass(OneDMinimmizeStatic,ResError,FuncError,
                                    Min1D_Coef,Min1D_MaxLinSrch,Min1D_MaxRetry),
                                1e-8,1e-15,FuncError,2) ,
            ResError);
        VecCl MinimStp(MinimPar.Dim());
        MinimStp = MinimStp*0 + 0.2;
        int Cont = 1, MaxIt = 15, k;
        double FoundMisfit = 1e6;
        for (k=1;((k<=MaxIt)&&Cont);k++)
            Cont=MinNd.Execute(MinimPar,MinimStp,-1,FoundMisfit);
        return MinimPar;
    }

    VecCl LimitRelat(const VecCl &RelatVol, double minCoef){
        double max = VecCl::Max(RelatVol);
        double min = max*minCoef;
        VecCl tmp(RelatVol.Dim());
        for(int k=1;k<=RelatVol.Dim();k++)
            tmp[k]=RelatVol[k]>min?RelatVol[k]:min;

        if (min>MathZer)
            tmp = tmp/max;
        return tmp;
    }
    VecCl EncodePlus(const VecCl &RelatVol){
        //return Encode(LimitRelat(RelatVol, 0.001));
        return Encode(RelatVol);
    }
    VecCl DecodePlus(const VecCl &RelatVol){
        //return LimitRelat(Decode(RelatVol), 0.001);
        return Decode(RelatVol);
    }



    int MinimizeTemp(double T, VecCl &RelatVol){
        Temp = T;
        VecCl MinimStart = EncodePlus(RelatVol), MinimNew;
        double ErrStart;
        MinimNew = MinimizeND(MinimStart);
        if (TestOK(MinimStart, MinimNew, MinimStart, ErrStart) ){
            RelatVol = DecodePlus(MinimStart);
//cout<<"\n1OK\n";
//cout<<" OK "<<ErrStart<<" T "<<Temp<<" Denc "<<Denc<<" par "<<RelatVol;
//TestOK(MinimStart,ErrStart);
//cout<<" Retested res "<<ErrStart<<"\n";
            return 1;
        }
        MinimNew = Minimize1D(MinimStart);
        if (TestOK(MinimStart, MinimNew, MinimStart, ErrStart)){
            RelatVol = DecodePlus(MinimStart);
//cout<<"\n2OK\n";
//cout<<" OK "<<ErrStart<<" T "<<Temp<<" Denc "<<Denc<<" par "<<RelatVol;
//TestOK(MinimStart,ErrStart);
//cout<<" Retested res "<<ErrStart<<"\n";
            return 1;
        }
        MinimNew = MinimizeND(MinimStart);
        if (TestOK(MinimStart, MinimNew, MinimStart, ErrStart) ){
            RelatVol = DecodePlus(MinimStart);
//cout<<"\n3OK\n";
//cout<<" OK "<<ErrStart<<" T "<<Temp<<" Denc "<<Denc<<" par "<<RelatVol;
//TestOK(MinimStart,ErrStart);
//cout<<" Retested res "<<ErrStart<<"\n";
            return 1;
        }
        MinimNew = MinimizeND(MinimStart);
        if (TestOK(MinimStart, MinimNew, MinimStart, ErrStart) || ErrStart<1e-4){
            RelatVol = DecodePlus(MinimStart);
//cout<<"\n4OK\n";
//cout<<" OK "<<ErrStart<<" T "<<Temp<<" Denc "<<Denc<<" par "<<RelatVol;
//TestOK(MinimStart,ErrStart);
//cout<<" Retested res "<<ErrStart<<"\n";
            return 1;
        }
        RelatVol = DecodePlus(MinimStart);
        return 0;
    }
    void PrintErr(int Start=1){
//#ifdef _DEBUG
        if (Start){
            cout<<" Very bad : \n";
            cout<<"Denc	"<<Denc<<" Temp	"<<Temp<<"\n";
        } else
            cout<<" \n\n\n";
//#endif
    }
    void PrintErr(const char *help,const VecCl &par){
//#ifdef _DEBUG
        VecCl RelVolCoef = Decode(par);
        double Mist;
        TestOK(par, Mist);
        cout<<" Very bad : "<<help<<"\n";
        cout<<"Denc	"<<Denc<<" Temp	"<<Temp<<"\n";
        VecCl P = FreeClc->Pressures(Denc, Temp, RelatNumbers, RelVolCoef);
        cout<<" Mistake "<<Mist<<"\n";
        cout<<"Relat pres "<<P;
        cout<<"Relat volume	"<<RelVolCoef;
        cout<<"Relat Numbers "<<RelatNumbers<<"\n";
        cout<<" \n\n\n";
//#endif
    }
    int TestOK(const VecCl &par1, const VecCl &par2, VecCl &par, double &Mist){
        double mist1, mist2;
        TestOK(par1,mist1);
        TestOK(par2,mist2);
        if (mist1<mist2)
            par = par1;
        else 
            par = par2;
        Mist = min(mist1,mist2);
        if (Mist<1e-5)
            return 1;
        return 0;
    }

    int TestOK(const VecCl &par,double &Mist){
        VecCl RelVolCoef = Decode(par);
        VecCl P = FreeClc->Pressures(Denc, Temp, RelatNumbers, RelVolCoef);
        VecCl test = EncodeRes(P);
        //double c = VecCl::ModMax(P);
        //for(int i=1;i<=P.Dim();i++){
        //    P[i] = log(fabs(P[i]/c));
        //}
        //Mist = sqrt(P*P);
        Mist = sqrt(test*test);
        if (Mist>1e-4)
            return 0;
        return 1;
    }
    //static VecCl RelatVolumesCoefs;
    //DisSamePresFinder *FreeClc;
};

//======================================================================================
//============================  Finding freeE for dissosiation =========================
//===================  base class for using in main FreeEChemMatterMultiV ==============
//======================================================================================
struct DisFindMinimE_Base:SavableClass{
	//MatrCl ChemReact;
	//VecCl StartSubstCoef, SubstVeights;
	//DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
 //   VecCl RelatVolumeCoef,SubstMixtures;
    DisFindMinimE_Base():SavableClass(){}
	virtual void Init(const VecCl &substveights, 
			const DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &chemsubst, 
			const VecCl &substmixtures, const VecCl &relatvolumecoef, const MatrCl &chemreact, 
			const VecCl &startsubstcoef)=0;
	//{
	//	SubstVeights = substveights;
	//	ChemSubst = chemsubst;
	//	SubstMixtures = substmixtures;
	//	RelatVolumeCoef = relatvolumecoef;
	//	ChemReact = chemreact;
	//	StartSubstCoef = startsubstcoef;
	//}
	virtual VecCl GetResultRelatMasses()=0;
	virtual VecCl GetResultVolumeCoef()=0;
	virtual VecCl GetResultPressures()=0;
	virtual VecCl GetResultFreeEPure()=0;
	virtual VecCl GetResultFreeECorr()=0;
	virtual double ClcFreeE(double Denc, double Temp)=0;
};


//new version of minim e
struct PureDis_FindE:DisFindMinimE_Base{//
	MatrCl ChemReact;
	//VecCl StartSubstCoef, SubstVeights;
	//DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
 //   VecCl RelatVolumeCoef,SubstMixtures;
    VecCl RelatVolumeCoef;
    PureDis_FindE():MinPres(new PureDis_Minim1Pres),FixedNumVolCoefs(0){};//:DisFindMinimE_Base():SavableClass(){}
	virtual void Init(const VecCl &substveights, 
			const DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &chemsubst, 
			const VecCl &substmixtures, const VecCl &relatvolumecoef, const MatrCl &chemreact, 
			const VecCl &startsubstcoef){
        MixtE = new PureDis_StrictClc(chemsubst, substveights);
        Params = new PureDis_StrictClc::Param(0, 0, 0, startsubstcoef, relatvolumecoef, relatvolumecoef);
		//SubstVeights = substveights;
		//ChemSubst = chemsubst;
		//SubstMixtures = substmixtures;
		RelatVolumeCoef = relatvolumecoef;
		ChemReact = chemreact;
        CheckStart();
		//StartSubstCoef = startsubstcoef;
	}
    virtual VecCl GetResultRelatMasses(){return ResultRelatMasses;}
    virtual VecCl GetResultVolumeCoef() {return ResultVolumeCoefs;}
    virtual VecCl GetResultPressures()  {return ResultPressures;}
    virtual VecCl GetResultFreeEPure()  {return ResultFreeEPure;}
    virtual VecCl GetResultFreeECorr()  {return ResultFreeECorr;}
    virtual double ClcFreeE(double Denc, double Temp){
        Params->Denc = Denc;Params->Temp = Temp;
        PureDis_MinimNum MinNum;
        if (VecCl::Min( Params->RelatVolumesCoefs[1] )<M_Eps)
            Params->RelatVolumesCoefs = RelatVolumeCoef;
        Params->RelatVolumesCoefs = Params->RelatVolumesCoefs/Params->RelatVolumesCoefs[1];
        double err = 1;
        int numIt=0;
        VecCl Pres;

        Params->FreeEPure = MixtE->FreeEVectorPure(*Params);
        if (FixedNumVolCoefs){
            VecCl RelNum(2);RelNum = RelNum*0 + 1;
            Params->RelatVolumesCoefs = MinPres->Do(Denc, Temp, 0, RelNum, Params->RelatVolumesCoefs, VecCl(2), MixtE, VecCl(2));
        }
        while(numIt++<15 && err>M_Eps2*10){
            if (!FixedNumVolCoefs)
                Params->RelatVolumesCoefs = MinPres->Do(Denc, Temp, 0, Params->RelatNumbers, Params->RelatVolumesCoefs, VecCl(2), MixtE, VecCl(2));
            Params->FreeEPure = MixtE->FreeEVectorPure(*Params);
            Pres = MixtE->PressureVector(*Params);
            Params->RelatNumbers = MinNum.Do(Denc, Temp, (Pres[1]+Pres[2])*0.5, Params->RelatNumbers, Params->RelatVolumesCoefs, Params->FreeEPure , MixtE, MatrCl::GetRow(ChemReact,1));
            ResultPressures = MixtE->PressureVector(*Params);
            err = sqrt((ResultPressures-ResultPressures[1])*(ResultPressures-ResultPressures[1]))/sqrt(ResultPressures*ResultPressures);
            if (FixedNumVolCoefs)
                break;
        }


        ResultRelatMasses = MixtE->ClcMassCoef(*Params);
        ResultVolumeCoefs = Params->RelatVolumesCoefs;
        //ResultPressures = MixtE->PressureVector(*Params);
        ResultPressures = MixtE->PressureVector(*Params);
        ResultFreeEPure = MixtE->FreeEVectorPure(*Params);
        ResultFreeECorr = MixtE->FreeEVectorCor(*Params);

#ifdef _DEBUG
VecCl P = ResultPressures;
double Mist = sqrt((P-P[1])*(P-P[1]))/sqrt(P*P);
if (Mist>1e-5){
    cout<<"Denc	"<<Denc<<" Temp	"<<Temp<<" Mistake "<<Mist<<"\n";
    cout<<"Relat pres "<<P;
    cout<<"Relat volume	"<<ResultVolumeCoefs;
    cout<<"Relat masses "<<ResultRelatMasses;
    cout<<"Relat numbers "<<Params->RelatNumbers;
    cout<<"	 LastFreeE	"<<MixtE->FreeEMixt(*Params)<<" : "<<MixtE->FreeEVectorPure(*Params)<<"\n";
}
#endif
        return MixtE->FreeEMixt(*Params);
    }
    int read_data_state(FilterIn &si){
        char tmp[256];
        si>>tmp>>FixedNumVolCoefs>>tmp>>MinPres;
        if (!si || !MinPres)
            throw info_except("Could not read MinPres=%i \n",!MinPres);
        return 1;
    }
    int save_data_state(FilterOut &so){
        so<<" FixedNumVolCoefs "<<FixedNumVolCoefs<<" MinPres "<<MinPres;
        if (!so)
            throw info_except("Could not save class\n");
        return 1;
    }
    Stroka MakeHelp(){
        Stroka res = "Class for finding dissosiation fraction + relat volumes of fractions. Use log(Vi/V0) correction\n";
        return res;
    }

private:
    Ref<PureDis_Minim1Pres> MinPres;//MinPres.SetStart();
    Ref<PureDis_StrictClc> MixtE;
    Ref<PureDis_StrictClc::Param> Params;
    int FixedNumVolCoefs ;
    VecCl ResultRelatMasses, ResultVolumeCoefs, ResultPressures, ResultFreeEPure, ResultFreeECorr;
    void CheckStart(){
        if ( ChemReact.DimX!=1 || ChemReact.DimY!=2 )
            throw info_except("Bad dims: DimX %i DimY %i\n",ChemReact.DimX,ChemReact.DimY);

    }
};


// Class for minimizing FreeE of substances with dissosiation
struct DisFindMinimE : DisFindMinimE_Base, DisSamePresFunc{

    DisFindMinimE():DisFindMinimE_Base(), DisSamePresFunc(), MinimizeVolumes(1), MinimRelatNum(new Dis_RelatNum_MinimFunc_Single){};

	virtual void Init(const VecCl &substveights, 
		const DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &chemsubst, 
		const VecCl &, const VecCl &relatvolumecoef, const MatrCl &, const VecCl &){
            FreeEClc = new DisMixture_Energy(chemsubst, substveights);
    		StartRelatVolumes = relatvolumecoef;
		//SubstMixtures = substmixtures;
		//ChemReact = chemreact;
		//StartSubstCoef = startsubstcoef;
	}

	VecCl Pressures(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
//VecCl ret = FreeEClc->PressureVector(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
//cout<<"Denc "<<Denc<<" Temp "<<Temp<<" RelatNum\n"<<RelatNumbers<<" RelatVolCoef\n"<<RelatVolumesCoefs<<" Pressures:\n"<<ret<<"\n\n";
        return FreeEClc->PressureVector(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
    }

    double ClcFreeE(double Denc, double Temp){
        VecCl RelatNumbers, RelatVolumesCoefs;
        ClcStart(RelatNumbers, RelatVolumesCoefs, Denc, Temp);
        if (MinimizeVolumes)
            MinimRelatVolume(Denc,Temp,RelatNumbers,RelatVolumesCoefs);
        double resFreeE = MinimDissFreeE(Denc,Temp,RelatNumbers,RelatVolumesCoefs);
        if (MinimizeVolumes){
            MinimRelatVolume(Denc,Temp,RelatNumbers,RelatVolumesCoefs);
            resFreeE = MinimDissFreeE(Denc,Temp,RelatNumbers,RelatVolumesCoefs);
            MinimRelatVolume(Denc,Temp,RelatNumbers,RelatVolumesCoefs);
            resFreeE = MinimDissFreeE(Denc,Temp,RelatNumbers,RelatVolumesCoefs);
            StartRelatVolumes = RelatVolumesCoefs;
        }
#ifdef _DEBUG
VecCl P = Pressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
double Mist = sqrt((P-P[1])*(P-P[1]))/sqrt(P*P);
if (Mist>1e-5){
    cout<<"Denc	"<<Denc<<" Temp	"<<Temp<<" Mistake "<<Mist<<"\n";
    cout<<"Relat pres "<<Pressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
    cout<<"Relat volume	"<<RelatVolumesCoefs;
    cout<<"Relat Numbers "<<RelatNumbers;
    cout<<"	 LastFreeE	"<<resFreeE<<" : "<<FreeEClc->FreeEVectorPure(Denc, Temp, RelatNumbers, RelatVolumesCoefs)<<"\n";
}
#endif
		
		//ResultRelatMasses = FreeEClc->ClcMassCoef(RelatNumbers);
        //ResultVolumesCoefs = RelatVolumesCoefs/sqrt(RelatVolumesCoefs*RelatVolumesCoefs);
        //ResultPressures = Pressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
		//ResultFreeE = FreeEClc->FreeEVectorPure
		SetLstClc(RelatNumbers, RelatVolumesCoefs, Denc, Temp);
        return resFreeE;
    }
	virtual VecCl GetResultVolumeCoef(){return LstClcRelatVolumesCoefs/sqrt(LstClcRelatVolumesCoefs*LstClcRelatVolumesCoefs);}
    virtual VecCl GetResultRelatMasses(){return FreeEClc->ClcMassCoef(LstClcRelatNumbers);}
    virtual VecCl GetResultPressures(){return Pressures(LstClcDenc, LstClcTemp, LstClcRelatNumbers, LstClcRelatVolumesCoefs);}
	virtual VecCl GetResultFreeEPure(){return FreeEClc->FreeEVectorPure(LstClcDenc, LstClcTemp, LstClcRelatNumbers,LstClcRelatVolumesCoefs);}
	virtual VecCl GetResultFreeECorr(){return FreeEClc->FreeEVectorCor(LstClcDenc, LstClcTemp, LstClcRelatNumbers,LstClcRelatVolumesCoefs);}


    int read_data_state(FilterIn &si){
        char tmp[256];
        si>>tmp>>MinimizeVolumes>>tmp>>MinimRelatNum;
        if (!si || !MinimRelatNum)
            throw info_except("Could not read MinimizeVolumes=%i or MinimRelatNum\n",MinimizeVolumes);
        return DisFindMinimE_Base::read_data_state(si);
    }
    int save_data_state(FilterOut &so){
        so<<" MinimizeVolumes "<<MinimizeVolumes<<" MinimRelatNum "<<MinimRelatNum;
        if (!so)
            throw info_except("Could not save class\n");
        return DisFindMinimE_Base::save_data_state(so);
    }
    Stroka MakeHelp(){
        Stroka res = "Class for finding dissosiation fraction + relat volumes of fractions. \nFor dissosiation fraction clc is used Dis_RelatNum_MinimFunc_category class.\nCurrently in the system are present " + SavableClass::HelpForCategory("Dis_RelatNum_MinimFunc_category");
        return res;
    }
private:
    void MinimRelatVolume(double Denc, double Temp, const VecCl &RelatNumbers, VecCl &RelatVolumesCoefs){
        DisSamePresFinder func(this, Denc, Temp, RelatNumbers, RelatVolumesCoefs);
        RelatVolumesCoefs = func.Minimize();
    }

	double MinimDissFreeE(double Denc, double Temp, VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
        //DisSingleDiss_MinimFunc func(FreeEClc->Veights());
        VecCl FePure = FreeEClc->FreeEVectorPure(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
		RelatNumbers = MinimRelatNum->FindRelatNumbers(FePure, Temp, RelatNumbers,FreeEClc->Veights());
		return FreeEClc->FreeECor(FePure, RelatNumbers, Temp);
    };
	void ClcStart(VecCl &RelatNumbers, VecCl &RelatVolumeCoefs, double Denc, double Temp){
        RelatNumbers = FreeEClc->Veights()*0 + 1./FreeEClc->Veights().Dim();
        RelatVolumeCoefs =  FreeEClc->Veights()*0 + 1;
	}
	void SetLstClc(VecCl &RelatNumbers, VecCl &RelatVolumesCoefs, double Denc, double Temp){
		LstClcRelatNumbers = RelatNumbers; LstClcRelatVolumesCoefs = RelatVolumesCoefs; LstClcDenc = Denc; LstClcTemp = Temp;
	};
	int MinimizeVolumes;
	//VecCl ResultRelatMasses,ResultVolumesCoefs, ResultPressures, ResultFreeE;

	VecCl LstClcRelatNumbers, LstClcRelatVolumesCoefs;
	double LstClcDenc, LstClcTemp;
    VecCl StartRelatVolumes;
    Ref<DisMixture_Energy> FreeEClc;
    Ref<Dis_RelatNum_MinimFunc_Base> MinimRelatNum;
};

// Main ChemReact EOS class!!!!
struct FreeEDis:FreeEIOFind{
	//FreeEChemMatterMultiV():FreeEIOFind("material.cfg","MatterChemH2Met"),FreeMinim(new DissMultiV){
	//	SubstMap[Stroka("M1")]=NamedSubst("M1",new FreeEIdeal(),2,1,1,1);
	//	SubstMap[Stroka("M2")]=NamedSubst("M2",new FreeEIdeal(),2,0,1,1);
	//	ChemEq eq(Stroka("M1"),-1);eq.AddSubst(Stroka("M2"),1);
	//	ChemEqs.push_back(eq);
	//	ClcStartParams();
	//}
    FreeEDis();

	void CheckReactCoef(MatrCl &ChemReact, VecCl &SubstVeights, 
		DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &ChemSubst){
		for(int k=1;k<=ChemReact.DimX;k++){
			double Res = MatrCl::GetRow(ChemReact,k)*SubstVeights;
			if (fabs(Res)>MathZer){
				FilterTextOut out("tmp",DataSource::Memory);
				out<<"\nBad chem react "<<k<<" result is: "<<Res<<"\n";
				out<<"Vector Of Chem React Coef "<<MatrCl::GetRow(ChemReact,k);
				out<<"Vector Subst Veights      "<<SubstVeights<<"Matters:\n";
				for (int i=0;i<ChemReact.DimY;i++) 
					out<<dynamic_cast<FreeEIOFind*>((InterfaceFreeEIO*)ChemSubst[i])->Finder.Substance<<" ";
				out<<"\nNames\n";
				for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
					iter!=SubstMap.end();iter++,k++)
						out<<iter->first.c_str()<<" ";
				out<<"\n";
				throw info_except(DataSource::GetStr("tmp").c_str());
				
			}
		}
	}
	void ClcStartParams(){
		MatrCl ChemReact;
		VecCl StartSubstCoef, SubstVeights;
		DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > ChemSubst;
        VecCl RelatVolumeCoef,SubstMixtures;

		int NumReact = (int)ChemEqs.size(), NumSubst = (int)SubstMap.size();
		ChemSubst.SetDim(NumSubst);
		SubstVeights.SetDim(NumSubst);
        RelatVolumeCoef.SetDim(NumSubst);
		StartSubstCoef.SetDim(NumSubst);
        SubstMixtures.SetDim(NumSubst);
		int k = 0;
		for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
			iter!=SubstMap.end();iter++,k++){
			ChemSubst[k] = iter->second.Subst;
			SubstInd[iter->first] = k;
			SubstVeights[k+1] = iter->second.MolVeight;
			StartSubstCoef[k+1] = iter->second.StartProportion;
            RelatVolumeCoef[k+1] = iter->second.RelatVolumeCoef;
            SubstMixtures[k+1] = iter->second.SubstMixtures;
		}
		ChemReact.SetMatr(NumReact, NumSubst);
		ChemReact = ChemReact*0.;
		for (int i = 1;i<=NumReact;i++){
			for (int j = 0;j<(int)ChemEqs[i-1].SubstName.size();j++){
				ChemReact(i,SubstInd[ ChemEqs[i-1].SubstName[j] ]+1) = ChemEqs[i-1].SubstCoef[j];
			}
		}
		CheckReactCoef(ChemReact,SubstVeights,ChemSubst);
		FreeMinim->Init(SubstVeights, ChemSubst, SubstMixtures, RelatVolumeCoef, ChemReact, StartSubstCoef);
		SetInternalNames();
	}
	virtual void SetInternalNames(){
		FreeEIOFind::SetInternalNames();
		int k = 1;
		for (map<Stroka, NamedSubst>::iterator iter = SubstMap.begin();
			iter!=SubstMap.end();iter++,k++){
                OutputNames[iter->first+"_M"] = &k;
                OutputNames[iter->first+"_V"] = &k;
                OutputNames[iter->first+"_P"] = &k;
                OutputNames[iter->first+"_Fp"] = &k;
                OutputNames[iter->first+"_Fc"] = &k;
			}
//#ifdef _DEBUG
//        FilterTextOut out("aa",DataSource::Memory);
//		save_data_state(out);out<<"\n";
//        SaveSubstancePar(out);out<<"\n=============================\n";
//		cout<<DataSource::GetStr("aa").c_str();
//#endif
  	}
	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
		void *ptr;
		if (CheckPresence(OutputNames, Output, ptr) && ptr!=NULL){
			FreeE(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"));
			int k1 = SubstInd[Stroka(Output,0,Output.length()-2)];
			int k2 = SubstInd[Stroka(Output,0,Output.length()-3)];
			Stroka Test1 = Stroka(Output,Output.length()-1,Output.length());
			Stroka Test2 = Stroka(Output,Output.length()-2,Output.length());
            if (Test1==Stroka("V"))
                return FreeMinim->GetResultVolumeCoef()[k1+1];
            if (Test1==Stroka("M"))
    			return FreeMinim->GetResultRelatMasses()[k1+1];
            if (Test1==Stroka("P"))
    			return FreeMinim->GetResultPressures()[k1+1];
            if (Test2==Stroka("Fp"))
    			return FreeMinim->GetResultFreeEPure()[k2+1];
            if (Test2==Stroka("Fc"))
    			return FreeMinim->GetResultFreeECorr()[k2+1];
		}

		return FreeEIOFind::OutputValClc(Input, Output);
	}
    double FreeE(double Denc,double T) {
		if (!FreeMinim)
			throw info_except("Minimizing class is not inited...\n");
		return FreeMinim->ClcFreeE(Denc,T);
    }
    void ReadSubstancePar(istream &in) {
		char tmp[256];
        in>>tmp;
		//if ( !(FreeMinim << SavableClass::Read(in)) )
		SavableClass *t = SavableClass::Read(in);
		if ( !(FreeMinim << t) )
			throw info_except("Could not read FreeMinim\n");
		in>>tmp>>tmp;
		NamedSubst sbst;
		SubstMap.clear();
		ChemEqs.clear();
		while (sbst.Read(in)){
			SubstMap[sbst.Name] = sbst;
			in>>tmp;
			if (stricmp(tmp,"}")==0) break;
			if (stricmp(tmp,";")!=0) 
				throw info_except("Read substance %s - expected '; } ' after it but got %s\n",sbst.Name.c_str(),tmp);
		}

		in>>tmp>>tmp;
		ChemEq eq;
		while (eq.Read(in)){
			ChemEqs.push_back(eq);
		}
		ClcStartParams();
    }
    void SaveSubstancePar(ostream &out) {
		out<<" EnterMinimClass ";SavableClass::Save(out,FreeMinim);
		out<<" EnterList_SubsName_FreeESubstance_SubsMolVeight { ";
		for(map<Stroka,NamedSubst>::iterator iter=SubstMap.begin();iter!=SubstMap.end();iter++){
			iter->second.Save(out);out<<" ; ";
		}
        out<<" } \n EnterListOfVectors_Vector_SubsName_SubsCoef { ";
		for(size_t k=0;k<ChemEqs.size();k++)
			ChemEqs[k].Save(out);
		out<<" } \n";
    }
    Stroka MakeHelp(){
        Stroka res = FreeEIOFind::MakeHelp();
		res += "Old technology, bad working... Use ClcDissociation::FreeEDis2 instead.\n";
        res += "Additionaly this class can calculate FreeE - total freE; M_i - mass of i-th component ; V_i - volume of i-th component ; P_i - pressure of i-th component ; Fp_i - FreeE pure of i-th component ; ; Fc_i - FreeE corrected(no entropy) of i-th component\n";
        res += "Calculate dissosiation of matters - adding to the free E of pure mat - entropy mixture addition. Pure free E calc under assumption that relat denc of matters are the same. For minim of E used base class DisFindMinimE_Base. Currently in the system we have classes of this category:\n";
        res += SavableClass::HelpForCategory("DisFindMinimE_category");
        return res;
    }
private:
	Ref<DisFindMinimE_Base> FreeMinim;
	struct NamedSubst{
		Stroka Name;
		Ref<InterfaceFreeEIO> Subst;
		double MolVeight,StartProportion;
        double RelatVolumeCoef,SubstMixtures;
		NamedSubst(const NamedSubst &sub):Name(sub.Name),Subst(sub.Subst),MolVeight(sub.MolVeight),StartProportion(sub.StartProportion){};
		NamedSubst():Name(""),Subst(NULL),MolVeight(0),StartProportion(0),RelatVolumeCoef(1){};
		NamedSubst(const Stroka &name,InterfaceFreeEIO *subst,double molveight,double startproportion, double relatvolumecoef, int substmixtures)
			:Name(name),Subst(subst),MolVeight(molveight),StartProportion(startproportion),RelatVolumeCoef(relatvolumecoef),SubstMixtures(substmixtures){};
		int  Read(istream &in) {
			char tmp[256];
			in>>tmp;
            if (stricmp(tmp,"}")==0) return 0;
			in>>Name>>tmp>>MolVeight>>tmp>>StartProportion>>tmp>>RelatVolumeCoef>>tmp>>SubstMixtures>>tmp;
            if (!(Subst<<SavableClass::Read(in)))
				throw info_except("Could not convert matter with Name %s to InterfaceFreeEIO\n",Name.c_str());
			return 1;
		}
		void Save(ostream &out) {
			out<<" Name "<<Name.c_str()<<" MolVeight "<<MolVeight<<" StartProportion "<<StartProportion<<" RelatVolumeCoef "<<RelatVolumeCoef<<" SubstMixtures "<<SubstMixtures<<" MatterFreeE ";
			SavableClass::Save(out,Subst);
		}
	};
	struct ChemEq{
		vector<Stroka> SubstName;
		vector<double> SubstCoef;
		ChemEq(){};
		ChemEq(const ChemEq &eq):SubstName(eq.SubstName), SubstCoef(eq.SubstCoef){};
		ChemEq(const Stroka &substname, double substcoef) { AddSubst(substname,substcoef); };
		void AddSubst(const Stroka &substname, double substcoef){
			SubstName.push_back(substname);SubstCoef.push_back(substcoef);
		};
		int  Read(istream &in) {
			SubstName.clear();SubstCoef.clear();
			char tmp[256];
			in>>tmp;
            if (stricmp(tmp,"}")==0) return 0;
			if (stricmp(tmp,"{")!=0) 
				throw info_except("Expected { and got %s\n",tmp);
			in>>tmp;
			while (stricmp(tmp,"}")!=0){
				Stroka Name = tmp;
				SubstName.push_back(tmp);
				double d;in>>d;
				SubstCoef.push_back(d);
				in>>tmp;
			}
			return 1;
		}
		void Save(ostream &out) {
			out<<" { ";
			for (size_t k=0;k<SubstName.size();k++)
				out<<"  "<<SubstName[k].c_str()<<" "<<SubstCoef[k]<<"    ";
			out<<" }\n";
		}

	};
	map<Stroka,NamedSubst> SubstMap;
    vector<ChemEq> ChemEqs;
	map<Stroka,int> SubstInd;
};




//struct DisFindH2Minim:DisFindMinimE_Base{
//    DisFindH2Minim():DisFindMinimE_Base(), MinimRelatNum(new Dis_RelatNum_MinimFunc_Single){}
//	virtual void Init(const VecCl &substveights, 
//		const DataVector< Ref< InterfaceFreeEIO >, CopyStructRef<Ref<InterfaceFreeEIO> > > &chemsubst, 
//		const VecCl &, const VecCl &relatvolumecoef, const MatrCl &, const VecCl &){
//            FreeEClc = new DisMixture_Energy(chemsubst, substveights);
//    		StartRelatVolumes = relatvolumecoef*0 + 1.;
//            if (substveights.Dim()!=2 || substveights[1]!=1 || substveights[2]!=2)
//                throw info_except("Bad SubstVeights - we clc only H2-H diss with 1 - H and 2 - H2");
//	}
//	VecCl Pressures(double Denc, double Temp, const VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
//        return FreeEClc->PressureVector(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
//    }
//
//    double ClcFreeE(double Denc, double Temp){
//        VecCl RelatNumbers, RelatVolumesCoefs;
//        ClcStart(RelatNumbers, RelatVolumesCoefs, Denc, Temp);
//        double resFreeE = MinimDissFreeE(Denc,Temp,RelatNumbers,RelatVolumesCoefs);
//#ifdef _DEBUG
//VecCl P = Pressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
//double Mist = sqrt((P-P[1])*(P-P[1]))/sqrt(P*P);
//if (Mist>1e-5){
//    cout<<"Denc	"<<Denc<<" Temp	"<<Temp<<" Mistake "<<Mist<<"\n";
//    cout<<"Relat pres "<<Pressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
//    cout<<"Relat volume	"<<RelatVolumesCoefs;
//    cout<<"Relat Numbers "<<RelatNumbers;
//    cout<<"	 LastFreeE	"<<resFreeE<<" : "<<FreeEClc->FreeEVectorPure(Denc, Temp, RelatNumbers, RelatVolumesCoefs)<<"\n";
//}
//#endif
//		ResultRelatMasses = FreeEClc->ClcMassCoef(RelatNumbers);
//        ResultVolumesCoefs = RelatVolumesCoefs/sqrt(RelatVolumesCoefs*RelatVolumesCoefs);
//        ResultPressures = Pressures(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
//        return resFreeE;
//    }
//	virtual VecCl GetResultVolumeCoef(){return ResultVolumesCoefs;}
//    virtual VecCl GetResultRelatMasses(){return ResultRelatMasses;}
//    virtual VecCl GetResultPressures(){return ResultPressures;}
//
//    int read_data_state(FilterIn &si){
//        char tmp[256];
//        si>>tmp>>MinimizeVolumes>>tmp>>MinimRelatNum;
//        if (!si || !MinimRelatNum)
//            throw info_except("Could not read MinimizeVolumes=%i or MinimRelatNum\n",MinimizeVolumes);
//        return DisFindMinimE_Base::read_data_state(si);
//    }
//    int save_data_state(FilterOut &so){
//        so<<" MinimizeVolumes "<<MinimizeVolumes<<" MinimRelatNum "<<MinimRelatNum;
//        if (!so)
//            throw info_except("Could not save class\n");
//        return DisFindMinimE_Base::save_data_state(so);
//    }
//
//private:
//
//	double MinimDissFreeE(double Denc, double Temp, VecCl &RelatNumbers, const VecCl &RelatVolumesCoefs){
//        VecCl FePure = FreeEClc->FreeEVectorPure(Denc, Temp, RelatNumbers, RelatVolumesCoefs);
//		RelatNumbers = MinimRelatNum->FindRelatNumbers(FePure, Temp, RelatNumbers,FreeEClc->Veights());
//		return FreeEClc->FreeECor(FePure, RelatNumbers, Temp);
//    };
//	void ClcStart(VecCl &RelatNumbers, VecCl &RelatVolumeCoefs, double Denc, double Temp){
//        RelatVolumeCoefs =  FreeEClc->Veights()*0 + 1;
//        RelatNumbers.SetDim(2);
//        VecCl Fpure = FreeEClc->FreeEVectorPure(Denc, Temp, RelatVolumeCoefs, RelatVolumeCoefs);
//        double q = exp((Fpure[1] - Fpure[2])*2/(Temp*M_Rconst));
//        double Xa = 2/(1+sqrt(1+4*q));
//        RelatNumbers[1] = Xa;
//        RelatNumbers[2] = 1-Xa;
//	}
//	VecCl ResultRelatMasses,ResultVolumesCoefs, ResultPressures;
//    VecCl StartRelatVolumes;
//    int MinimizeVolumes;
//    Ref<DisMixture_Energy> FreeEClc;
//    Ref<Dis_RelatNum_MinimFunc_Base> MinimRelatNum;
//};
