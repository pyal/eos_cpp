
#pragma once

#include "urs_curve.h"
#include "urs/matt_2phase.h"

//==============================================================================================
//============================    UrsCurve_EOS_PT_Equilib  - base   ============================
//==============================================================================================

struct UrsCurve_EOS_PT_Equilib:URS_Curve::ClcVar{
	UrsCurve_EOS_PT_Equilib():URS_Curve::ClcVar(),NameTemp("Temperature"),
		NameMatterLowP("MatLowP"),NameMatterHghP("MatHghP"){};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
		//ClcNewStp(-1);
    	Ref<URS_Curve::ClcVar> Temp;
		Temp = Dat->GetClcVar(NameTemp);
		MatLowP  = dynamic_cast<EOS_Savable*>((SavableClass*)Dat->GetVar(NameMatterLowP));
		MatHghP  = dynamic_cast<EOS_Savable*>((SavableClass*)Dat->GetVar(NameMatterHghP));
		if (!MatLowP || !MatHghP || !Temp)
			throw info_except("Could not get MatLowP?%i MatHghP?%i Temperature?%i..\n",
                MatLowP==NULL, MatHghP==NULL, Temp==NULL);
		if (!MatHghP->Mat || !MatLowP->Mat)
			throw info_except("Matter's MatHghP?%i or MatLowP?%i - matter - poorly defined.\n",
				MatHghP->Mat==NULL, MatLowP->Mat==NULL);
	};

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameTemp"<<NameTemp.c_str()
			<<"NameMatterHghP"<<NameMatterHghP.c_str()<<"NameMatterLowP"<<NameMatterLowP.c_str();
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
		si>>tmp>>NameTemp>>tmp>>NameMatterHghP>>tmp>>NameMatterLowP;
        return 1; 
    };
protected:
    Stroka NameTemp,NameMatterLowP,NameMatterHghP;
   	Ref<EOS_Savable> MatLowP,MatHghP;
};

//==============================================================================================
//============================    UrsCurve_PT_Bnd_Constructor       ============================
//==============================================================================================

struct UrsCurve_PT_Bnd_Constructor:UrsCurve_EOS_PT_Equilib{
	UrsCurve_PT_Bnd_Constructor();
	//:UrsCurve_EOS_PT_Equilib(),ClcError(1e-6),FindPDencFrom(1e-5), FindPDencTo(5),
	//	FindGLowPres(0),FindGHghPres(1e4),StartPres(100){
	//	ClcVarNames["Pressure_T"] = 0;
	//	ClcVarNames["DencityLowP_T"] = 1;
	//	ClcVarNames["DencityHghP_T"] = 2;
	//	ClcVarNames["EnergyLowP_T"] = 3;
	//	ClcVarNames["EnergyHghP_T"] = 4;
	//};
//Stroka UrsCurve_FreeE::ClcNames[NumClcNames]={"Pressure_T","EnergyLowP_T","EnergyHghP_T","DencityLowP_T","DencityHghP_T",

	virtual void ClcStart(URS_Curve* Data){
		UrsCurve_EOS_PT_Equilib::ClcStart(Data);
		if (!dynamic_cast<MatterFreeE*>((MatterIO*)MatLowP->Mat) || 
			!dynamic_cast<MatterFreeE*>((MatterIO*)MatHghP->Mat) )
            throw info_except("UrsCurve_FreeE works only with MatterFreeE base class. %s or %s  is wrong type\n",
				MatLowP->Mat->class_name(), MatHghP->Mat->class_name());
		FreeClcLowP = dynamic_cast<MatterFreeE*>((MatterIO*)MatLowP->Mat)->FreeEPtr;
		FreeClcHghP = dynamic_cast<MatterFreeE*>((MatterIO*)MatHghP->Mat)->FreeEPtr;
		LastPres = StartPres;
	}
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
		if (ClcVarNames.find(SubName)==ClcVarNames.end())
			throw info_except("Cannot calculate variable %s\n",SubName.c_str());
		ClcBndPnt();
		return eos_res[ ClcVarNames[SubName] ];
	}
    virtual int save_data_state( FilterOut&so) { 
		UrsCurve_EOS_PT_Equilib::save_data_state(so);
		so<<"ClcError"<<ClcError<<"FindPDencFrom"<<FindPDencFrom<<"FindPDencTo"<<FindPDencTo
			<<"FindGLowPres"<<FindGLowPres<<"FindGHghPres"<<FindGHghPres<<"StartPres"<<StartPres;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
		UrsCurve_EOS_PT_Equilib::read_data_state(si);
        char tmp[256];
		si>>tmp>>ClcError>>tmp>>FindPDencFrom>>tmp>>FindPDencTo>>tmp>>FindGLowPres>>tmp>>FindGHghPres>>tmp>>StartPres;
        return 1; 
    };
	
	virtual Stroka MakeHelp(){
        Stroka res = UrsCurve_EOS_PT_Equilib::MakeHelp();
        char tmp[256];
		res += Stroka("Class for FreeE Bnd calculation. Have ") + Itoa(NumClcNames,tmp,10) + " internal vars for output:";
		for (map<Stroka,int>::iterator it = ClcVarNames.begin();it!=ClcVarNames.end();it++)
			res += it->first + " ";
		res += Stroka(".\n");
		Ref<UrsCurve_PT_Bnd_Constructor> form = new UrsCurve_PT_Bnd_Constructor();
        res += SavableClass::object2string(form);
        return res;
	}
protected:
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
		for (map<Stroka,int>::iterator it = ClcVarNames.begin();it!=ClcVarNames.end();it++)
			eos_res[it->second] = 0;
	}
	double ClcError,FindPDencFrom, FindPDencTo,FindGLowPres,FindGHghPres,StartPres;

	Ref<SavableClass> eos_res[5];
	InterfaceFreeEIO *FreeClcLowP,*FreeClcHghP;

	static double ResDencLow,ResDencHgh,FindPres,CurTemp;
	static 	InterfaceFreeEIO *FindMat;

	static double DeltaPres(double Denc){return FindPres-FindMat->Pressure(Denc,CurTemp);}

	static double StaticClcError, StaticDencLow, StaticDencHgh, StaticDencFrom, StaticDencTo;;
	static 	InterfaceFreeEIO *StaticFreeLowPStat, *StaticFreeHghPStat;

	static double DeltaG(double Pres){
		FindPres = Pres;
		FindMat=StaticFreeLowPStat;
		int err;
		if (4<(err=Fzero(DeltaPres,StaticDencFrom,StaticDencTo,StaticDencLow,StaticClcError*10,StaticClcError,500)) )
			{cout<<" Bad FZero PressureLow in UrsCurve_PT_Bnd_Constructor::DeltaG \nfound Denc "
				<<StaticDencLow<<" Mis "<<DeltaPres(StaticDencLow)<<" err "<<err<<"\n";}
//cout<<" found Denc "<<StaticDencLow<<" Mis "<<DeltaPres(StaticDencLow)<<" err "<<err<<"\n";
		FindMat=StaticFreeHghPStat;
		if (4<(err=Fzero(DeltaPres,StaticDencFrom,StaticDencTo,StaticDencHgh,StaticClcError*10,StaticClcError,500)) )
			{cout<<" Bad FZero PressureHgh in UrsCurve_PT_Bnd_Constructor::DeltaG \nfound Denc "
				<<StaticDencHgh<<" Mis "<<DeltaPres(StaticDencHgh)<<" err "<<err<<"\n";}
//cout<<" found Denc "<<StaticDencHgh<<" Mis "<<DeltaPres(StaticDencHgh)<<" err "<<err<<"\n";
        double GHgh = StaticFreeLowPStat->FreeE(StaticDencLow,CurTemp)+StaticFreeLowPStat->Pressure(StaticDencLow,CurTemp)/StaticDencLow;
        double GLow = StaticFreeHghPStat->FreeE(StaticDencHgh,CurTemp)+StaticFreeHghPStat->Pressure(StaticDencHgh,CurTemp)/StaticDencHgh;
        double ret = GHgh-GLow;
//cout<<" Pres "<<FindPres<<" MisF "<<ret*10<<"\n";
		return ret*10;
	}

private:
	map<Stroka,int> ClcVarNames;
//Stroka UrsCurve_FreeE::ClcNames[NumClcNames]={"Pressure_T","EnergyLowP_T","EnergyHghP_T","DencityLowP_T","DencityHghP_T");
	double LastPres;
	void ClcBndPnt(){
			if (eos_res[0]!=NULL)
				return;
			StaticFreeLowPStat = FreeClcLowP;
			StaticFreeHghPStat = FreeClcHghP;
			StaticClcError	   = ClcError;
			StaticDencFrom		= FindPDencFrom;
			StaticDencTo		= FindPDencTo;
			CurTemp = GetVar(NameTemp);
			int err;
			if (4<=(err=Fzero(DeltaG,FindGLowPres,FindGHghPres,LastPres,StaticClcError,StaticClcError,1000)) )
				{cout<<" Bad FZero DeltaG in UrsCurve_PT_Bnd_Constructor::ClcBndPnt \nTemp "<<CurTemp<<" found Pres "
					<<LastPres<<" Mis "<<DeltaG(LastPres)<<" err "<<err<<"\n";}
//cout<<" found Pres "<<LastPres<<" Mis "<<DeltaG(LastPres)<<" err "<<err<<"\n";cout.flush();
			eos_res[ClcVarNames["Pressure_T"] ] = new SavableDouble(LastPres);
			eos_res[ClcVarNames["DencityLowP_T"] ] = new SavableDouble(StaticDencLow);
			eos_res[ClcVarNames["DencityHghP_T"] ] = new SavableDouble(StaticDencHgh);
			eos_res[ClcVarNames["EnergyLowP_T"] ] = new SavableDouble(StaticFreeLowPStat->Energy(StaticDencLow,CurTemp));
			eos_res[ClcVarNames["EnergyHghP_T"] ] = new SavableDouble(StaticFreeHghPStat->Energy(StaticDencHgh,CurTemp));
	}

};


//==============================================================================================
//============================    UrsCurve_PT_Bnd_Binary     ===================================
//==============================================================================================
struct UrsCurve_PT_Bnd_Binary:URS_Curve::ClcVar{
    struct Result:SavableClass{
        Result(double t, double h,double l):Temp(t), Dhgh(h), Dlow(l){};
        double Temp, Dhgh, Dlow;
	    virtual int save_data_state( FilterOut&so) {so<<Temp<<Dhgh<<Dlow;return 1; };
	    virtual int read_data_state(FilterIn&si){ si>>Temp>>Dhgh>>Dlow;return 1;};
    };
	UrsCurve_PT_Bnd_Binary():URS_Curve::ClcVar(),NameTemp("Temperature"),
		NameLowD("MatLowD"),NameHghD("MatHghD"),NameFileToGenerate("HghLow_bnd.bin"),ClcError(1e-6){};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
    	Ref<URS_Curve::ClcVar> Temp,LowD,HghD;
		Temp = Dat->GetClcVar(NameTemp);
		LowD  = Dat->GetClcVar(NameLowD);
		HghD  = Dat->GetClcVar(NameHghD);
		if (!LowD || !HghD || !Temp)
			throw info_except("Could not get LowD?%i HghD?%i Temperature?%i..\n", !LowD, !HghD, !Temp);
		T.SetNumEl(0);H.SetNumEl(0);L.SetNumEl(0);
	};
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        return new Result(GetVar(NameTemp), GetVar(NameHghD), GetVar(NameLowD));
	}
    virtual void ClcEnd(){
		if (H.SetNumEl()<=1)
			return;
		Ref<FreeELiqSol_Bnd> bnd = FreeELiqSol_Bnd::Generate((double*)H,(double*)L,(double*)T,H.SetNumEl(),ClcError);
		FreeELiqSol_Bnd::SaveFile(NameFileToGenerate.c_str(), bnd.pointer());
    };

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameTemp"<<NameTemp.c_str()<<"NameHghD"<<NameHghD.c_str()<<"NameLowD"<<NameLowD.c_str()
			<<"NameFileToGenerate"<<NameFileToGenerate.c_str()<<"ClcError"<<ClcError;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
		si>>tmp>>NameTemp>>tmp>>NameHghD>>tmp>>NameLowD>>tmp>>NameFileToGenerate>>tmp>>ClcError;
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = URS_Curve::ClcVar::MakeHelp();
        res += Stroka("Binary Bnd file generator - stores results for boundary in the binary format...\n");
        //Ref<UrsCurve_PT_Bnd_Binary> form = new UrsCurve_PT_Bnd_Binary();
        //res += SavableClass::object2string(form);
        return res;

	}
protected:
    Stroka NameTemp,NameLowD,NameHghD,NameFileToGenerate;
	ResizeVector<double> T,H,L;
	double ClcError;
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
		T.push(GetVar(NameTemp)); L.push(GetVar(NameLowD)); H.push(GetVar(NameHghD));
    }
};
















//==============================================================================================
//============================    UrsCurve_EOS_PT_UnstBnd     ==================================
//==============================================================================================

struct UrsCurve_PT_UnstBnd_Clc:URS_Curve::ClcVar{
	UrsCurve_PT_UnstBnd_Clc():URS_Curve::ClcVar(), NameTemp("Temperature"),
        NameMatter("MatterUnstable"),Dmin(1e-8), Dmax(10), NumPnt(100), NumSplPnt(100), Misfit(1e-6){};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
		//ClcNewStp(-1);
    	Ref<URS_Curve::ClcVar> Temp;
		Temp = Dat->GetClcVar(NameTemp);
		EOS_Savable* MatFreeEEnv = dynamic_cast<EOS_Savable*>((SavableClass*)Dat->GetVar(NameMatter));
        MatterFreeE* MatFree = NULL;
        MatUnst = NULL;
        if (MatFreeEEnv ){
            MatFree = dynamic_cast<MatterFreeE*>((MatterIO*)MatFreeEEnv->Mat);
            if (MatFree)
                MatUnst = dynamic_cast<FreeEIO*>((InterfaceFreeEIO*)MatFree->FreeEPtr);
        }

		if (!MatUnst || !Temp)
			throw info_except("Could not get MatUnst?%i MatFreeE?%i EOS_Savable?%i Temperature?%i..\n", MatUnst==NULL, MatFree==NULL, MatFreeEEnv==NULL,Temp==NULL);
        StpDone = -1;
	};
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        if (Results.find(SubName)==Results.end()){
            Stroka Names;for(map<Stroka, double>::iterator it = Results.begin();it!=Results.end();it++) Names += it->first+ " ";
            throw info_except("Cannot calculate variable %s\nKnown names are :%s\n",SubName.c_str(), Names.c_str());
        }
        return new SavableDouble(Results[SubName]);
	}

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameTemp"<<NameTemp.c_str()<<"NameMatter"<<NameMatter.c_str();
        so<<"Dmin"<<Dmin<<"Dmax"<<Dmax<<"NumPnt"<<NumPnt<<"NumSplPnt"<<NumSplPnt<<"Misfit"<<Misfit;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
		si>>tmp>>NameTemp>>tmp>>NameMatter;
        si>>tmp>>Dmin>>tmp>>Dmax>>tmp>>NumPnt>>tmp>>NumSplPnt>>tmp>>Misfit;
        return 1; 
    };
    Stroka GetVars(){
        Stroka ret;
        vector<Stroka> names = MatterStable::TwoPhaseBoundary::GetBndNames();
        for(int i=0;i<(int)names.size();i++)
            ret += names[i] + " ";
        return ret;
    }
	virtual Stroka MakeHelp(){
        Stroka res = URS_Curve::ClcVar::MakeHelp();
        res += Stroka("Calculates unstable boundary - to use it for generation of unstable boundary binary file byUrsCurve_PT_UnstBnd_Binary . \nGenerated Boundary vars:")+ GetVars() + " \n";
        return res;
	}
protected:
    Stroka NameTemp,NameMatter;
   	Ref<FreeEIO> MatUnst;
    map<Stroka, double> Results;
    double Dmin, Dmax, NumPnt, NumSplPnt, Misfit;
    int StpDone;
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (StpDone==CurStp)
            return;
        Results = MatterStable::TwoPhaseBoundary::ClcBnd(GetVar(NameTemp), Dmin, Dmax, NumPnt, NumSplPnt, MatUnst, Misfit);
        StpDone = CurStp;
    }
};

struct UrsCurve_PT_UnstBnd_Binary:URS_Curve::ClcVar{
	UrsCurve_PT_UnstBnd_Binary():URS_Curve::ClcVar(),NameBounds("UnstBnd"), ResFileName("UnstBndFile"), NumSplPnt(100), SplMisfit(1e-6){};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
    	Ref<URS_Curve::ClcVar> Bnd = Dat->GetClcVar(NameBounds);
        StpDone = -1;
        DataMap.clear();
        vector<double> tmp;
        vector<Stroka> names = MatterStable::TwoPhaseBoundary::GetBndNames();
        for(int i=0;i<(int)names.size();i++)
            DataMap[names[i]] = tmp;
		if (!Bnd)
			throw info_except("Could not get Bnd..\n");
	};
    struct Result:SavableClass{
        Result(){};
	    virtual int save_data_state( FilterOut&so) {return 1; };
	    virtual int read_data_state(FilterIn&si){ return 1;};
    };

    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        return new Result;
	}

    virtual void ClcEnd(){
        if (DataMap.begin()->second.size()<=1)
            return;
        Stroka desc = MatterStable::TwoPhaseBoundary::SetBnd(DataMap, ResFileName, SplMisfit, NumSplPnt);
        cout<<" UnstBnd bin creation results:\n"<<desc.c_str()<<"\n";
    };

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameBounds"<<NameBounds.c_str()<<"ResFileName"<<ResFileName.c_str();
        so<<"NumSplPnt"<<NumSplPnt<<"SplMisfit"<<SplMisfit;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
		si>>tmp>>NameBounds>>tmp>>ResFileName;
        si>>tmp>>NumSplPnt>>tmp>>SplMisfit;
        return 1; 
    };
    Stroka GetVars(){
        Stroka ret;
        vector<Stroka> names = MatterStable::TwoPhaseBoundary::GetBndNames();
        for(int i=0;i<(int)names.size();i++)
            ret += names[i] + " ";
        return ret;
    }
	virtual Stroka MakeHelp(){
        Stroka res = URS_Curve::ClcVar::MakeHelp();
        res += Stroka("Generates unstable boundary file for the given boundary data - to use it in MatterStable EOS. \nUses Boundary vars:")+ GetVars() + " \nPrints - nothing...\n";
        return res;
	}
protected:
    Stroka NameBounds, ResFileName;
    map<Stroka, vector<double> > DataMap;
    double NumSplPnt, SplMisfit;
    int StpDone;
        
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (StpDone == CurStp)
            return;
        StpDone = CurStp;
        for(map<Stroka, vector<double> >::iterator it = DataMap.begin();it!=DataMap.end();it++)
            DataMap[it->first].push_back(GetVar(NameBounds+"."+it->first));
    }
};














//==============================================================================================
//============================    UrsCurve_PT_clc            ===================================
//==============================================================================================
struct UrsCurve_PT_clc:URS_Curve::ClcVar{
	UrsCurve_PT_clc():URS_Curve::ClcVar(),NameTemp("Temperature"),NamePres("Pressure"),
		NameMatter("Matter"),ClcError(1e-6), MinDenc(1e-5), MaxDenc(10){};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
    	Ref<URS_Curve::ClcVar> Temp,Pres;
        Ref<EOS_Savable> Matter;
        Temp = Dat->GetClcVar(NameTemp);
		Pres = Dat->GetClcVar(NamePres);
		Matter  = dynamic_cast<EOS_Savable*>((SavableClass*)Dat->GetVar(NameMatter));
		if (!Matter || !Pres || !Temp)
			throw info_except("Could not get Matter<%s>?%i Pressure<%s>?%i Temperature<%s>?%i..\n", 
            NameMatter.c_str(), !Matter, NamePres.c_str(), !Pres, NameTemp.c_str(), !Temp);
        if ( !Matter->Mat )
			throw info_except("Matter's  - matter - poorly defined.\n");
		if (!dynamic_cast<MatterFreeE*>((MatterIO*)Matter->Mat))
            throw info_except("UrsCurve_PT_clc works only with MatterFreeE base class. %s is wrong type\n",Matter->Mat->class_name());
		FreeClc = dynamic_cast<MatterFreeE*>((MatterIO*)Matter->Mat)->FreeEPtr;
        StatDenc = MinDenc;
	};
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        if (Results.find(SubName)==Results.end())
            throw info_except("Cannot calculate var <%s>. Class can calculate only <%s>\n", SubName.c_str(), GetVars().c_str());
        return new SavableDouble(Results[SubName]);
	}
    virtual void ClcEnd(){
		//if (H.SetNumEl()<=1)
		//	return;
		//Ref<FreeELiqSol_Bnd> bnd = FreeELiqSol_Bnd::Generate((double*)H,(double*)L,(double*)T,H.SetNumEl(),ClcError);
		//FreeELiqSol_Bnd::SaveFile(NameFileToGenerate.c_str(), bnd.pointer());
    };

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameTemp"<<NameTemp.c_str()<<"NamePres"<<NamePres.c_str()<<"NameMatter"<<NameMatter.c_str()
			<<"MinDenc"<<MinDenc<<"MaxDenc"<<MaxDenc<<"ClcError"<<ClcError;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
		si>>tmp>>NameTemp>>tmp>>NamePres>>tmp>>NameMatter>>tmp>>MinDenc>>tmp>>MaxDenc>>tmp>>ClcError;
        return 1; 
    };

	virtual Stroka MakeHelp(){
        Stroka res = URS_Curve::ClcVar::MakeHelp();
        res += Stroka("For given PT calculates - ")+ GetVars() + " \n";
        //Ref<UrsCurve_PT_Bnd_Binary> form = new UrsCurve_PT_Bnd_Binary();
        //res += SavableClass::object2string(form);
        return res;

	}
protected:
    Stroka NameTemp,NamePres,NameMatter;
    Ref<InterfaceFreeEIO> FreeClc;
	double ClcError, MinDenc, MaxDenc;
    map<Stroka, double> Results;
    Stroka GetVars(){ return " FreeG FreeE Dencity Eneregy "; }
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
        double Temp = GetVar(NameTemp), Pres = GetVar(NamePres);
        double Denc = FindDenc(Pres, Temp);
        Results["FreeE"] = FreeClc->FreeE(Denc, Temp);
        Results["Dencity"] = Denc;
        Results["Eneregy"] = FreeClc->Energy(Denc, Temp);
        Results["FreeG"] =  Results["FreeE"] + Pres/Denc;
    }

    double StatPres, StatTemp, StatDenc;
    static UrsCurve_PT_clc* CurPtr;
	double FindDenc(double Pres, double Temp){
		StatPres = max<double>(Pres,1e-6);
        StatTemp = max<double>(Temp,1);
        StatDenc = max<double>(StatDenc, MinDenc);
        UrsCurve_PT_clc *Old = CurPtr;
        CurPtr = this;
		int err;
		if (4<(err=Fzero(DeltaP,MinDenc,MaxDenc,StatDenc,ClcError,ClcError,500)) )
			{cout<<" Bad FZero in UrsCurve_PT_clc::DeltaG \nfound Denc "
				<<StatDenc<<" Mis "<<DeltaP(StatDenc)<<" err "<<err<<"\n";}
        CurPtr = Old;
        return StatDenc;
	}

    static double DeltaP(double Denc){
        double y = log(max<double>(1e-200,CurPtr->FreeClc->Pressure(Denc, CurPtr->StatTemp))/CurPtr->StatPres);
//cout<<" Denc "<<Denc<<" y "<<y<<" Temp "<<CurPtr->StatTemp<<" Pres "<<CurPtr->StatPres<<"\n";
        return y;
    }
};

