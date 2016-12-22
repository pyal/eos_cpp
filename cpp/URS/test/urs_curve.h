
#pragma once


#include "lib/Data/VecRes.h"
#include "lib/Ref/Stroka.h"
#include <list>
#include <map>
#include "lib/Ref/str_manip.h"

#include "lib/ref/file_manip.h"

struct SavableDouble : SavableClass {
    SavableDouble(double dat) :
		Dat(dat){
	};
    double Dat;
	virtual int save_data_state( FilterOut&so) {
		so.precision(12);
		so.width(17);
		so << Dat;
		return 1; 
	};
	virtual int read_data_state(FilterIn&si) {
		si >> Dat;
		return 1;
	};
};


struct URS_Curve : SavableClass {
	URS_Curve()
		: NumIter(0)
		, out(new Output) {
		Vars[Stroka("ClcVar")] = new ClcVar();
	};
    int save_data_state( FilterOut&so);
    int read_data_state(FilterIn&si);

	struct ClcVar : SavableClass {
        ClcVar()
			: Dat(NULL)
			, Res(0){
		};
		virtual void ClcStart(URS_Curve* Data) {
			Dat = Data;
			LastStp.clear();
			Res = 0;
		};
		virtual void ClcEnd(){};
// normally not to be redefined
        virtual Ref<SavableClass> GetRes(const Stroka &SubName){
			CheckStp(SubName, this);
			return new SavableDouble(Res);
		}
//	    virtual int save_data_state( FilterOut&so) {return 1; };
//		virtual int read_data_state(FilterIn&si){ return 1;};
		//virtual string print_help(){
		//	FilterTextOut tmp("urs_curve::clcvar::print_help",DataSource::Memory);
		//	tmp.SetRefMask(SavableClass::UnlinkedObjects);
		//	MakeHelp(tmp);
		//	return string(DataSource::GetStr("urs_curve::clcvar::print_help").c_str());
		//}
		virtual Stroka MakeHelp(){
            Stroka res = "Class for calculating of the results.\n";
			//Ref<ClcVar> form = new ClcVar();
   //         res += SavableClass::object2string(form);
			return res;
		}
	protected:
// To be used for help generation..........
		//virtual void MakeHelp(FilterTextOut &out){
		//	out<<"Class for calculating of the results.\n";
		//	Ref<ClcVar> form = new ClcVar();
		//	out<<form;
		//}
		void CheckStp(const Stroka &subName, void *obj){
			int curStp = Dat->CurItet;
            ObjStpStruct &objStp = LastStp[obj];
            map<Stroka, int>::iterator it = objStp.Stps.find(subName + "::");
            if ( it == objStp.Stps.end() || curStp!=it->second ){
                ClcNewStp(curStp, subName);
                objStp.Stps[subName + "::"] = curStp;
                objStp.CurStp = curStp;
			}
		};
        int FirstVarStpClc(void *obj){
			int curStp = Dat->CurItet;
            ObjStpStruct &objStp = LastStp[obj];
            return (objStp.CurStp != curStp);
        }
// It is assistance to work with different data..........
		double GetVar(const Stroka name){
			Ref<SavableClass> tmp = Dat->CalculateVar(name);
			Ref<SavableDouble> d = dynamic_cast<SavableDouble*>((SavableClass*)tmp);
			if (!d) 
				throw info_except("Could not get double from res\n");
			return d->Dat;
		}
// Working function!!!
		virtual void ClcNewStp(int curStp, const Stroka &subName) {
			Res = curStp;
		};

        URS_Curve *Dat;
        double Res;
		//int LastStp;
        struct ObjStpStruct {
            ObjStpStruct()
				: CurStp(-1) {
			};
            map<Stroka, int> Stps;
            int CurStp;
        };
		map<void*, ObjStpStruct> LastStp;
	};
	struct Output : SavableClass {
        Output()
			: OutFile("")
			, Dat(NULL) {
		};
		virtual void OutStart(int numIter, URS_Curve* data) {
            Dat = data;
            out.OpenBuf(OutFile.c_str(),1);
            for(list<Stroka>::iterator it = OutNames.begin();it!=OutNames.end();it++) {
                Ref<ClcVar> clc = Dat->GetClcVar(*it);
                if (!clc) 
                    throw info_except("variable %s is not of ClcVar type...\n",it->c_str());
                out<<it->c_str()<<"\t";
            }
            out<<"\n";
        }
		virtual void OutStep() {
            for(list<Stroka>::iterator it = OutNames.begin();it != OutNames.end(); it++) {
                Ref<SavableClass> dat = Dat->CalculateVar(*it);
                if (dat) {
                    dat->save_data_state(out);
					out << " ";
                } 
            }
            out << "\n";
			out.flush();
        }
        virtual void OutEnd() {
			out.CloseBuf(0);
		};
	    virtual int save_data_state(FilterOut &so) {
            so << "FileToStore" << OutFile;
            so << " VectorOfNames { ";
            for(list<Stroka>::iterator it = OutNames.begin(); it!=OutNames.end(); it++)
                so << *it << " ";
            so << " }\n";
            return 1; 
        };
		virtual int read_data_state(FilterIn &si){ 
            char tmp[256];
            si >> tmp >> OutFile >> tmp >> tmp >> tmp;
            while (stricmp(tmp,"}") != 0) {
                OutNames.push_back(tmp);
                si >> tmp;
            }
            return 1;
        };
	    virtual Stroka MakeHelp(){
            //FilterTextOut tmp("urs_curve::output::print_help",DataSource::Memory);
            //tmp.SetRefMask(SavableClass::UnlinkedObjects);
            Stroka res = "Class for printing the results.\ninput: OutFileName ParNames\n";
            //Ref<Output> form = new Output();
            //form->OutFile = "TestOut";
            //form->OutNames.push_back("TestClcVarToOut");
            //tmp<<form;
            //return string(DataSource::GetStr("urs_curve::output::print_help").c_str());
            return res;
        }
	protected:
        list<Stroka> OutNames;
        Stroka OutFile;
        URS_Curve *Dat;
        FilterTextOut out;
	};



    void Do(){
        out->OutStart(NumIter, this);
        //for (list<Stroka>::iterator it = Vectors.begin();it!=Vectors.end();it++)
        //    GetClcVar(*it)->ClcStart(NumIter,this);
		for (map<Stroka, Ref<SavableClass> >::iterator it = Vars.begin();it!=Vars.end();it++){
			Ref<ClcVar> var = GetClcVar(it->first);
			if (var)
				var->ClcStart(this);
		}
        for (CurItet=0;CurItet<NumIter;CurItet++)
            out->OutStep();
        out->OutEnd();
		for (map<Stroka, Ref<SavableClass> >::iterator it = Vars.begin();it!=Vars.end();it++){
			Ref<ClcVar> var = GetClcVar(it->first);
			if (var)
				var->ClcEnd();
		}
    }
   // void DoStep(int CurStp){
   //     for (list<Stroka>::iterator it = Vectors.begin();it!=Vectors.end();it++)
			//GetClcVar(*it)->ClcStep(CurStp);
   //     out->OutStep(CurStp);
   // };


    virtual Ref<ClcVar> GetClcVar(const Stroka &name){
        Stroka SubName,VarName = MakeNames(name, SubName);
        Ref<ClcVar> ret = dynamic_cast<ClcVar*>((DescribedClass*)Vars[VarName]);
        return ret;
    }
    virtual Ref<SavableClass> CalculateVar(const Stroka &name){
        Stroka SubName, VarName = MakeNames(name, SubName);
        Ref<ClcVar> ret = dynamic_cast<ClcVar*>((DescribedClass*)Vars[VarName]);
		if (ret==0)
            return NULL;
        return ret->GetRes(SubName);
    }
	virtual Ref<SavableClass> GetVar(const Stroka &name){
        Stroka SubName,VarName = MakeNames(name, SubName);
        return Vars[VarName];
		//if (ret==0)
  //          throw info_except("Var %s is not of ClcVar class var\n",name.c_str());
  //      return ret;
    }
	virtual void SetVar(const Stroka &name,Ref<SavableClass> v){
        Stroka SubName,VarName = MakeNames(name, SubName);
        Vars[VarName] = v;
    }
// For unstd init help....
    virtual void SetOutput(Output *out_) {
        out = out_;
    }
	Stroka MakeNames(const Stroka name, Stroka &subName){
		Stroka tmp(name);//, 0, name.find_first_of(".",0));
		subName = tmp.substr(tmp.find_first_of(".") + 1);
        return tmp.substr(0, tmp.find_first_of("."));//VarName 
	}
    Stroka MakeHelp(){
        Stroka res;
        res += Stroka("Class for drawing gasd curves.\n")+
			"Class have 2 sections - Variables and Output\n"+
			"In the section variables - write any number of named classes from category URS_Curve_Var_category and URS_Curve_ClcVar_category\n"+
			"In the section output - write any number of classes for output\n"+
			"Final NumIter - number of times to make calculation - output\n"+
			"Each iteration - URS_Curve_Var_category and URS_Curve_ClcVar_category are calculated (really when Output class asks there value\n"+
			"and the result is printed by the output class\n"+
			"Listing classes in the known categories \n";

        //Ref<URS_Curve> form = new URS_Curve;form->out = new Output;
        //form->Vars[Stroka("ClcVar")] = new ClcVar();
        //res += SavableClass::object2string( form ) + "\n";
        res += SavableClass::HelpForCategory("URS_Curve_Var_category");
        res += SavableClass::HelpForCategory("URS_Curve_ClcVar_category");
        res += SavableClass::HelpForCategory("URS_Curve_Output_category");

        return res;
    }
//    virtual string print_help() {
//        FilterTextOut tmp("urs_curve::print_help",DataSource::Memory);
//        tmp.SetRefMask(SavableClass::UnlinkedObjects);
////        tmp.SetRefMask(SavableClass::SingleFileStorage);
//        Ref<URS_Curve> form = new URS_Curve;
//        form->out = new Output;
//        //form->Vectors.push_back(Stroka("ClcVar"));
//        form->Vars[Stroka("ClcVar")] = new ClcVar();
//        tmp<<form;
//
//		tmp<<"Class for drawing gasd curves.\n"<<
//			"First input gasd params. All fixed variables (to be used in calculation), and calculation ones\n"<<
//			"After input Clc names - names of variables there value will be calculated each step\n"<<
//            "and output class - class which will write the results\n";
//        ClassDesc::WriteHelpOnCategory(tmp,"URS_Curve_Output_category",ClassDesc::PrintHelp);
//        ClassDesc::WriteHelpOnCategory(tmp,"URS_Curve_ClcVar_category",ClassDesc::PrintHelp);
//        ClassDesc::WriteHelpOnCategory(tmp,"URS_Curve_Var_category",ClassDesc::PrintHelp);
//        return string(DataSource::GetStr("urs_curve::print_help").c_str());
//	}
	int SetNumIter(int num = -1) {
		if (num!=-1)
			NumIter = num;
		return NumIter;
	}
    int NumIter,CurItet;
protected:
	map<Stroka, Ref<SavableClass> > Vars;
    //list<Stroka> Vectors;
    Ref<Output> out;

};


//==============================================================================================
//============================    UrsCurve_StepClc       =======================================
//==============================================================================================

struct UrsCurve_StepClc : URS_Curve::ClcVar {
    UrsCurve_StepClc() : MinVal(0), MaxVal(100), NumDivStp(100), LogScale(0), NumSame(1), Centered(0){
	};
	virtual void ClcStart(URS_Curve* data){
		URS_Curve::ClcVar::ClcStart(data);
		int NumStp = Dat->NumIter;
        if (NumSame * NumDivStp == 0 || NumStp % (NumSame * NumDivStp) != 0)
            throw info_except(" NumStp %i is not product of NumSame %i and NumDivStp %i \n", NumStp, NumSame, NumDivStp);
    };
//    virtual Ref<SavableClass> GetRes(){return new SavableDouble(Res);}
    virtual int save_data_state( FilterOut &so) { 
        so << "MinVal" << MinVal << "MaxVal" << MaxVal << "NumDivStp" << NumDivStp
          << "LogScale" << LogScale << "NumSame" << NumSame << "Centered" << Centered;
        return 1; 
    };
	virtual int read_data_state(FilterIn &si){ 
        char tmp[256];
        si >> tmp >> MinVal >> tmp >> MaxVal >> tmp >> NumDivStp
          >> tmp >> LogScale >> tmp >> NumSame >> tmp >> Centered;
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "Class for making same step axis.\n";
        return res;
	}
protected:
	virtual void ClcNewStp(int curStp, const Stroka &subName) {
        if (!FirstVarStpClc(this))
            return;
        int cur = NumSame == 1 ? curStp % NumDivStp : curStp / NumSame;
        double nDiv = Centered ? NumDivStp : NumDivStp - 1;
        double mi = LogScale ? log(MinVal) : MinVal, 
               ma = LogScale ? log(MaxVal) : MaxVal, stp = (ma - mi) / max(nDiv, 1);
        if (Centered)
            mi += 0.5 * stp;
        Res = mi + stp * cur;
        if (LogScale)
            Res = exp(Res);
	}
    double MinVal, MaxVal ;
    int NumDivStp, LogScale, NumSame, Centered; 

};

//==============================================================================================
//====================================    EOS_Savable  =========================================
//==============================================================================================

#include "urs\matt_fac.h"
#include "urs\fre_fac.h"

struct EOS_Savable : SavableClass {
    Ref<MatterIO> Mat;
	EOS_Savable() : Mat(NULL){
	};
	EOS_Savable(MatterIO *mat) 
		: Mat(mat){
	};
    ~EOS_Savable(){
	};//{delete Mat;};
	virtual int save_data_state( FilterOut &so) {
        so << Mat;
        //SavableClass* m = dynamic_cast<SavableClass*>((MatterIO*)Mat);
        //if (!SavableClass::Save(so,m))
        //    throw info_except("Could not save matter\n");
		return !(!so);
    };
	virtual int read_data_state(FilterIn &si){ 
        si >> Mat;
        if (!(Mat))
            throw info_except("Could not read matter\n");
		return 1;
    }
	//virtual string print_help(){
 //       FilterTextOut tmp("urs_curve::EOS_Savable::print_help",DataSource::Memory);
 //       tmp.SetRefMask(SavableClass::UnlinkedObjects);
	//	tmp<<"Internal var for making EOS calculation.\n";
 //       Ref<EOS_Savable> form = new EOS_Savable();
	//	form->Mat = new MatterABu();
 //       tmp<<form;
 //       return string(DataSource::GetStr("urs_curve::EOS_Savable::print_help").c_str());
 //   }
	virtual Stroka MakeHelp() {
		Stroka res = "Internal var for making EOS calculation.\n";
        return res;
    }
};

//==============================================================================================
//============================    UrsCurve_EOS_RoT  - base   ===================================
//==============================================================================================

struct UrsCurve_EOS_RoT : URS_Curve::ClcVar {
	UrsCurve_EOS_RoT() 
		: URS_Curve::ClcVar()
		, NameDenc("Dencity")
		, NameTemp("Temperature")
		, NameMatter("Mat"){
	};
	virtual void ClcStart(URS_Curve* data){
		URS_Curve::ClcVar::ClcStart(data);
    	Ref<URS_Curve::ClcVar> denc, temp;
		denc = Dat->GetClcVar(NameDenc);
		temp = Dat->GetClcVar(NameTemp);
		Mat  = dynamic_cast<EOS_Savable*>((SavableClass*)Dat->GetVar(NameMatter));
		if (!Mat || !denc || !temp)
			throw info_except("Could not get Matter?%i Denc?%i or Temperature?%i..\n",
                Mat == NULL, denc == NULL, temp == NULL);
		if (!Mat->Mat)
			throw info_except("Matter's matter - poorly defined...\n");
    };

    virtual int save_data_state( FilterOut &so) { 
		so << "NameDenc" << NameDenc.c_str() << "NameTemp" << NameTemp.c_str() << "NameMatter" << NameMatter.c_str();
        return 1; 
    };
	virtual int read_data_state(FilterIn &si){ 
        char tmp[256];
        si >> tmp >> NameDenc >> tmp >> NameTemp >> tmp >> NameMatter;
        return 1; 
    };
    Stroka MakeHelp(){
        Stroka res = "Class for FreeE Ro_T calculations. Input pars: Dencity, Temperature\n";
        return res;
    }
protected:
    Stroka NameDenc, NameTemp, NameMatter;
   	Ref<EOS_Savable> Mat;
};

//==============================================================================================
//============================    UrsCurve_EOS_RoE  - base   ===================================
//==============================================================================================

struct UrsCurve_EOS_RoE : URS_Curve::ClcVar {
	UrsCurve_EOS_RoE() 
		:URS_Curve::ClcVar()
		, NameDenc("Dencity")
		, NameEner("Energy")
		, NameMatter("Mat"){
	};
	virtual void ClcStart(URS_Curve* data){
		URS_Curve::ClcVar::ClcStart(data);
    	Ref<URS_Curve::ClcVar> denc, ener;
		denc = Dat->GetClcVar(NameDenc);
		ener = Dat->GetClcVar(NameEner);
		Mat  = dynamic_cast<EOS_Savable*>((SavableClass*)Dat->GetVar(NameMatter));
		if (!Mat || !denc || !ener)
			throw info_except("Could not get Matter Denc or Energy..\n");
		if (!Mat->Mat)
			throw info_except("Matter's matter - poorly defined...\n");
    };

    virtual int save_data_state( FilterOut&so) { 
		so << "NameDenc" << NameDenc.c_str() << "NameEnergy" << NameEner.c_str() << "NameMatter" << NameMatter.c_str();
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si >> tmp >> NameDenc >> tmp >> NameEner >> tmp >> NameMatter;
        return 1; 
    };
    Stroka MakeHelp(){
        Stroka res = "Class for caloric Ro_E calculations. Input pars: Dencity, Energy\n";
        return res;
    }

protected:
    Stroka NameDenc, NameEner, NameMatter;
   	Ref<EOS_Savable> Mat;
};

//==============================================================================================
//============================    UrsCurve_EOS_Ro   - base   ===================================
//==============================================================================================

struct UrsCurve_EOS_Ro : URS_Curve::ClcVar {
	UrsCurve_EOS_Ro() 
		: URS_Curve::ClcVar()
		, NameDenc("Dencity")
		, NameMatter("Mat"){
	};
	virtual void ClcStart(URS_Curve* data){
		URS_Curve::ClcVar::ClcStart(data);
    	Ref<URS_Curve::ClcVar> denc;
		denc = Dat->GetClcVar(NameDenc);
		Mat  = dynamic_cast<EOS_Savable*>((SavableClass*)Dat->GetVar(NameMatter));
		if (!Mat || !denc )
			throw info_except("Could not get Matter or Denc ..\n");
		if (!Mat->Mat)
			throw info_except("Matter's matter - poorly defined...\n");
    };

    virtual int save_data_state( FilterOut &so) { 
		so << "NameDenc" << NameDenc.c_str() << "NameMatter" << NameMatter.c_str();
        return 1; 
    };
	virtual int read_data_state(FilterIn &si){ 
        char tmp[256];
        si >> tmp >> NameDenc >> tmp >> NameMatter;
        return 1; 
    };
protected:
    Stroka NameDenc, NameMatter;
   	Ref<EOS_Savable> Mat;
};


//==============================================================================================
//============================      UrsCurve_Caloric         ===================================
//==============================================================================================
struct UrsCurve_Caloric:UrsCurve_EOS_RoE{
    //enum ClcTypes{Pressure,Sound,Temperature};
#define  NumClcNames 3

	UrsCurve_Caloric():UrsCurve_EOS_RoE(){};
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
		for (int k=0;k<NumClcNames;k++)
            if (SubName==ClcNames[k])
				return ClcName(k);
		return 	new SavableDouble( Mat->Mat->OutputValClc(
                GetVar(NameDenc),
                GetVar(NameEner), SubName ));
	}
	virtual Stroka MakeHelp(){
        char tmp[256];
        Stroka res = UrsCurve_EOS_RoE::MakeHelp();
        res += Stroka("Have ") + itoa(NumClcNames,tmp,10) + " internal vars for output:";
		for (int k = 0; k < NumClcNames; k++)
			res += ClcNames[k] + " ";
		res += ".\n";
        return res;
	}
	
protected:
	virtual void ClcNewStp(int curStp, const Stroka &subName){
        if (!FirstVarStpClc(this))
            return;
		for (int k = 0;k < NumClcNames; k++)
			eos_res[k] = 0;
	}
	Ref<SavableClass> eos_res[NumClcNames];

	static double(*eos_clc[NumClcNames])(double,double) ;
	static Stroka ClcNames[NumClcNames]; 
	static 	MatterIO *MatStat;

	static double ClcPressure(double r,double e) {
		return MatStat->Pressure(r,e);
	}
	static double ClcSound(double r,double e) {
		return MatStat->Sound(r,e);
	}
	static double ClcTemp(double r,double e) { 
		return MatStat->Temperature(r,e);
	}

private:
    virtual Ref<SavableClass> ClcName(int k){
		if (!eos_res[k]){
			MatStat = Mat->Mat;
			eos_res[k] = new SavableDouble( eos_clc[k](
                GetVar(NameDenc),
                GetVar(NameEner)) );
		}
		return eos_res[k];
	}

};


//==============================================================================================
//============================      UrsCurve_FreeE       =======================================
//==============================================================================================
struct UrsCurve_FreeE:UrsCurve_EOS_RoT{
#undef NumClcNames
#define  NumClcNames 6
//Stroka UrsCurve_FreeE::ClcNames[NumClcNames]={"Pressure","Energy","Sound","FreeE"}; 

	UrsCurve_FreeE():UrsCurve_EOS_RoT(){};
	virtual void ClcStart(URS_Curve* Data){
		UrsCurve_EOS_RoT::ClcStart(Data);
		if (!dynamic_cast<MatterFreeE*>((MatterIO*)Mat->Mat))
            throw info_except("UrsCurve_FreeE works only with MatterFreeE base class. %s is wrong type\n",Mat->Mat->class_name());
		FreeClc = dynamic_cast<MatterFreeE*>((MatterIO*)Mat->Mat)->FreeEPtr;

	}
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
		for (int k=0;k<NumClcNames;k++){
            if (SubName==ClcNames[k])
				return ClcName(k);
		}
		//MatStat = Mat->Mat;
		//FreeStat = FreeClc;
		return 	new SavableDouble( FreeClc->OutputValClc(
                GetVar(NameDenc),
                GetVar(NameTemp), SubName ));

//		throw info_except("Cannot calculate variable %s\n",SubName.c_str());
	}
	virtual Stroka MakeHelp(){
        char tmp[256];
        Stroka res = UrsCurve_EOS_RoT::MakeHelp();
        res += Stroka("Have ") + itoa(NumClcNames,tmp,10) + " internal vars for output:";
		for (int k=0;k<NumClcNames;k++)
			res += ClcNames[k] + " ";
		res += ".\n";
        return res;
	}
	
protected:
	//virtual void MakeHelp(FilterTextOut &out){
	//	out<<"Class for FreeE EOS calculation. Have "<<NumClcNames<<" internal vars for output:";
	//	for (int k=0;k<NumClcNames;k++)
	//		out<<ClcNames[k].c_str();
	//	out<<".\n";
	//	Ref<UrsCurve_FreeE> form = new UrsCurve_FreeE();
 //       out<<form;
	//}
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
		for (int k=0;k<NumClcNames;k++)
			eos_res[k] = 0;
	}
	Ref<SavableClass> eos_res[NumClcNames];
	InterfaceFreeEIO *FreeClc;

	static double(*eos_clc[NumClcNames])(double,double) ;
	static Stroka ClcNames[NumClcNames]; 
	static 	MatterIO *MatStat;
	static 	InterfaceFreeEIO *FreeStat;

	static double ClcPressure(double r,double t){ return FreeStat->Pressure(r,t);}
	static double ClcSound(double r,double t){ return MatStat->Sound(r, FreeStat->Energy(r,t));}
	static double ClcEnergy(double r,double t){ return FreeStat->Energy(r,t);}
	static double ClcFreeE(double r,double t){ return FreeStat->FreeE(r,t);}
	static double ClcFreeG(double r,double t){ return FreeStat->FreeE(r,t)+FreeStat->Pressure(r,t)/r;}
	static double ClcEntropy(double r,double t){ 
        double dt = 30;
        return -(FreeStat->FreeE(r,t+dt)-FreeStat->FreeE(r,t))/dt;
    }

private:
    virtual Ref<SavableClass> ClcName(int k){
		if (!eos_res[k]){
			MatStat = Mat->Mat;
			FreeStat = FreeClc;
			eos_res[k] = new SavableDouble( eos_clc[k](
                GetVar(NameDenc),
                GetVar(NameTemp)) );
		}
		return eos_res[k];
	}

};



//==============================================================================================
//============================    UrsCurve_EOS_FindE     =======================================
//==============================================================================================

struct UrsCurve_EOS_FindE:UrsCurve_EOS_RoT{
	UrsCurve_EOS_FindE():UrsCurve_EOS_RoT(){};
	
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
        if (SubName!="Energy" && SubName!="FindE" )
            throw info_except("Cannot calculate var %s. Use to clc Enery only\n",SubName.c_str());
		CheckStp(SubName, this);
		return 	new SavableDouble( Res );
//		throw info_except("Cannot calculate variable %s\n",SubName.c_str());
	}
	virtual Stroka MakeHelp(){
        Stroka res = "Get Dencity and Temperature var - result - Energy.\n";
        return res;
	}
protected:
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
		Res = Mat->Mat->Energy(GetVar(NameDenc),
                               GetVar(NameTemp));
	}
};


//==============================================================================================
//============================    UrsCurve_EOS_Isoentrope    ===================================
//==============================================================================================

#include "urs/urs_std_curve.h"

struct UrsCurve_EOS_Isoentrope:UrsCurve_EOS_Ro{
	UrsCurve_EOS_Isoentrope():UrsCurve_EOS_Ro(),StartE(0),StartU(0),ClcU(1){};
    virtual void ClcStart(URS_Curve* Data){
        UrsCurve_EOS_Ro::ClcStart(Data);
        PrevR = PrevE = -1;
    };

    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        if (SubName=="Energy") 
            return new SavableDouble(ResE);
        else if (SubName=="Velocity") 
            return new SavableDouble(ResU);
		    else throw info_except("Cannot calculate variable %s\n",SubName.c_str());
	}
    virtual int save_data_state( FilterOut&so) { 
        UrsCurve_EOS_Ro::save_data_state(so);
        so<<"StartE"<<StartE<<"StartU"<<StartU<<"ClcU"<<ClcU;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        UrsCurve_EOS_Ro::read_data_state(si);
        char tmp[256];
        si>>tmp>>StartE>>tmp>>StartU>>tmp>>ClcU;
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "Get Dencity  var and StartE StartU - result - \n"+
            Stroka("Isentrope Energy and Isentrope Velocity. If ClcU =0 - isentrope dU will not be calculated\n")+
            Stroka("Variables for output: Energy, Velocity\n");
        return res;
	}
protected:
	//virtual void MakeHelp(FilterTextOut &out){
 //       out<<"Get Dencity  var and StartE StartU - result - \n"<<
 //            "Isentrope Energy and Isentrope Velocity. If ClcU =0 - isentrope dU will not be calculated\n"<<
 //            "Variables for output: Energy, Velocity\n";
 //       Ref<UrsCurve_EOS_Isoentrope> form = new UrsCurve_EOS_Isoentrope();
 //       out<<form;
	//}
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
        if (PrevE==-1 && PrevR==-1){
            PrevE = StartE; PrevR = GetVar(NameDenc);
            ResE = StartE;
            ResU = StartU;
            return;
        }
        IsentropeClass ise(Mat->Mat);
        double Denc = GetVar(NameDenc);
        if (ClcU)
            ResU += ise.IsentropeDU(PrevR ,PrevE , Denc);
		PrevE = ResE = ise.IsentropeEnergy(PrevR, PrevE, Denc);
        PrevR = Denc;
	}
    double StartE,StartU;
    double PrevE,PrevR;
    double ResE, ResU;
    int ClcU;
};


//==============================================================================================
//============================    UrsCurve_EOS_Hugniot    ======================================
//==============================================================================================

struct UrsCurve_EOS_Hugoniot:UrsCurve_EOS_Ro{
	UrsCurve_EOS_Hugoniot():UrsCurve_EOS_Ro(), RecalcPres(1), PressureDependece(1){};
    virtual void ClcStart(URS_Curve* Data){
        UrsCurve_EOS_Ro::ClcStart(Data);
        if (RecalcPres)
            Start.Pres = Mat->Mat->Pressure(Start.Denc,Start.Ener);
    };
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        if (SubName=="Energy") 
            return new SavableDouble(Result.Ener);
        else if (SubName=="Velocity") 
            return new SavableDouble(Result.Vel);
        else if (SubName=="ShockVelocity") 
            return new SavableDouble(Result.ShockVel);
        else if (SubName=="Pressure") 
            return new SavableDouble(Result.Pres);
        else if (SubName=="Dencity") 
            return new SavableDouble(Result.Denc);
		else throw info_except("Cannot calculate variable %s\n",SubName.c_str());
	}

    virtual int save_data_state( FilterOut&so) { 
        UrsCurve_EOS_Ro::save_data_state(so);
        so<<"StartDenc"<<Start.Denc<<"StartEner"<<Start.Ener<<"StartPres"<<Start.Pres<<
            "StartVel"<<Start.Vel<<"PressureDependece"<<PressureDependece<<"RecalcPres"<<RecalcPres;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        UrsCurve_EOS_Ro::read_data_state(si);
        double StartVel,StartPres,StartDenc,StartEner;
        char tmp[256];
        si>>tmp>>StartDenc>>tmp>>StartEner>>tmp>>StartPres>>tmp>>StartVel>>tmp>>PressureDependece>>tmp>>RecalcPres;
        Start = HugoniotClass::HugoniotData(StartVel,StartPres,StartDenc,StartEner,0);
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "From Pressure var(or Dencity if  PressureDependence=1) and Start (E,Ro,U,P) params make hugoniot. Write also if we have to recalculate start Pressure(RecalcPres=1)\nResult for output - Hugoniot: Energy Velocity ShockVelocity Pressure Dencity.\n";
        return res;
	}
protected:
	//virtual void MakeHelp(FilterTextOut &out){
 //       out<<"From Pressure var(or Dencity if  PressureDependence=1) and Start (E,Ro,U,P) params make hugoniot. Write also if we have to recalculate start Pressure(RecalcPres=1)\nResult for output - Hugoniot: Energy Velocity ShockVelocity Pressure Dencity.\n";
 //       Ref<UrsCurve_EOS_Hugoniot> form = new UrsCurve_EOS_Hugoniot();
 //       out<<form;
	//}
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
        HugoniotClass hug(Mat->Mat);
		if (!PressureDependece)
			Result = hug.HugDencClc(Start, GetVar(NameDenc));
		else
			Result = hug.HugPresClc(Start, GetVar(NameDenc));
    }
    HugoniotClass::HugoniotData Start, Result;
    int RecalcPres, PressureDependece;
};

//==============================================================================================
//============================      UrsCurve_FileReader      ===================================
//==============================================================================================
struct UrsCurve_FileReader:URS_Curve::ClcVar{

    UrsCurve_FileReader()
		: URS_Curve::ClcVar()
		, DefaultValue(0)
		, in(0)
		, FileName("input.dat") {
		DataNames.push("X");
		DataNames.push("Y");
	};
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        for (int k=0;k<DataNames.SetNumEl();k++)
            if (SubName==DataNames[k])
                return new SavableDouble(CurLine[k]);
		throw info_except("Cannot calculate variable %s\n",SubName.c_str());
	}
    virtual void ClcStart(URS_Curve* Data){
        URS_Curve::ClcVar::ClcStart(Data);
        File::close(in);
        in = File::open(FileName.c_str(), "r", "FileReader::ClasStart");
    };
    virtual void ClcEnd(){File::close(in);};

    virtual int save_data_state( FilterOut&so) { 
        so<<"FileName"<<FileName.c_str()<<"DataNames { ";
        for(int k=0;k<DataNames.SetNumEl();k++)
            so<<DataNames[k]<<" ";
        so<<" } DefaultValue "<<DefaultValue;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si >> tmp >> FileName >> tmp;
		SavableClass::ExeptionCheck(si, "{", "no begining...");
        DataNames.Release();
		while (! SavableClass::TestNextChar(si, '}', 1)) {
            si >> tmp;
            DataNames.push(Stroka(tmp));
        }
        si >> tmp >> DefaultValue;
        CurLine.SetNumEl(DataNames.SetNumEl());
        File::close(in);
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "Class for reading of external file. Input file name and names for columns\n";
        return res;
	}
protected:
	//virtual void MakeHelp(FilterTextOut &out){
	//	out<<"Class for reading of external file. Input file name and names for columns\n";
	//	Ref<UrsCurve_FileReader> form = new UrsCurve_FileReader();
 //       form->FileName = "test.dat";
 //       form->DataNames.push(Stroka("X"));
 //       form->DataNames.push(Stroka("Y"));
 //       out<<form;
	//}
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (CurStp==0 && !in)
            throw info_except("Could not open file %s\n",FileName.c_str());
        if (!FirstVarStpClc(this))
            return;
        for (int k=0;k<CurLine.SetNumEl();k++)
            CurLine[k] = DefaultValue;
        Stroka str;
        while(File::GetLine(in, str)) {
            vector<Stroka> line = Str::SplitLine(str);
            if (line.size()<(size_t)CurLine.SetNumEl()){
                cout<<"Bad line its length is "<<line.size()<<" and have to be "<<CurLine.SetNumEl()<<"Line is:\n"<<str<<"\n";
                continue;
            }
            int Bad = 0;
            for(int i = 0; i < CurLine.SetNumEl(); i++)
                if (!IsDouble(line[i].c_str(), CurLine[i], 1)){
                    Bad = 1;
                    cout<<"Str "<<line[i]<<" is not double.Bad line:\n"<<str<<"\n";
                    i = CurLine.SetNumEl() + 1;
                }
            if (!Bad)
                return;
        }
    };

private:
    ResizeVector<Stroka> DataNames;
    ResizeVector<double> CurLine;
    Stroka FileName;
    FILE *in;
    double DefaultValue;
};




//==============================================================================================
//============================      UrsCurve_XY_Test         ===================================
//==============================================================================================

struct UrsCurve_XY_Test:URS_Curve::ClcVar{
    struct Result:SavableClass{
        Result(double dat1, double dat2,int toprint=1):Dat1(dat1), Dat2(dat2),ToPrint(toprint){};
        double Dat1, Dat2;
        int ToPrint;
	    virtual int save_data_state( FilterOut&so) {if (ToPrint) so<<Dat1<<Dat2;return 1; };
	    virtual int read_data_state(FilterIn&si){ if (ToPrint) si>>Dat1>>Dat2;return 1;};
    };
	UrsCurve_XY_Test():URS_Curve::ClcVar(),NameY1("Y1"),NameY2("Y2"),MaxMeanError(1e-6),CheckMaxSingleError(0){};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
    	Ref<URS_Curve::ClcVar> Y1,Y2;
		Y1 = Dat->GetClcVar(NameY1);
		Y2 = Dat->GetClcVar(NameY2);
		if (!Y1 || !Y2)
            throw info_except("Could not get Y1 or Y2 curve. Y1 %s Y2 %s\n",NameY1.c_str(), NameY2.c_str());
        CurPnt = 0;
        CurError = 0;
        MaxError = 0;
    };
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        return new Result(GetVar(NameY1), GetVar(NameY2),SubName!="None");
	}
    virtual void ClcEnd(){
        if (CheckMaxSingleError) {
            if (MaxError>MaxMeanError){
                throw info_except("MaxError is too big!!!. Error is %13.6e and have to be %13.6e\n",MaxError,MaxMeanError);
                return;
            }
        } else
            if (CurError/CurPnt>MaxMeanError)
                throw info_except("MeanError is too big!!!. Error is %13.6e and have to be %13.6e\n",CurError/CurPnt,MaxMeanError);
    };

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameY1"<<NameY1.c_str()<<"NameY2"<<NameY2.c_str()
            <<"MaxMeanError"<<MaxMeanError<<"CheckMaxSingleError"<<CheckMaxSingleError;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si>>tmp>>NameY1>>tmp>>NameY2>>tmp>>MaxMeanError>>tmp>>CheckMaxSingleError;
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "Class for testing the result. Output Name = None - not to print anything, only test\n";
        res += "If CheckMaxSingleError==0:\n";
        res += "If Sum{|X1-X2|/max(|X1|,|X2|)}/NumPnt>MaxMeanError - throw an exception. \n";
        res += "If CheckMaxSingleError==1:\n";
        res += "If Max{|X1-X2|/max(|X1|,|X2|)}>MaxMeanError - throw an exception. \n";
        return res;
	}
protected:
	//virtual void MakeHelp(FilterTextOut &out){
	//	out<<"Class for testing the result. If Sum(sqr(X1-X2))/NumPnt>MaxMeanError - throw an exception\n";
	//	Ref<UrsCurve_XY_Test> form = new UrsCurve_XY_Test();
 //       out<<form;
	//}
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
        CurPnt++;
        //CurError += sqr((GetVar(NameY1) - GetVar(NameY2))/
        //    max(max(fabs(GetVar(NameY1)),fabs(GetVar(NameY2))),M_MinDouble2));
        double CurMis = fabs(GetVar(NameY1) - GetVar(NameY2))/
            max(max(fabs(GetVar(NameY1)),fabs(GetVar(NameY2))),M_MinDouble2);
        CurError += CurMis;
        MaxError = max(MaxError, CurMis);
//cout<<GetVar(NameY1)<<" "<<GetVar(NameY2)<<" "<<CurError<<"\n";
    }
protected:
    Stroka NameY1, NameY2;
    double MaxMeanError, CurError, MaxError;
    int CurPnt, CheckMaxSingleError;
};


//==============================================================================================
//============================      UrsCurve_HugCheck         ===================================
//==============================================================================================

struct UrsCurve_HugCheck:UrsCurve_EOS_Ro{
	UrsCurve_HugCheck():UrsCurve_EOS_Ro(){};
    //virtual void ClcStart(URS_Curve* Data){
    //    UrsCurve_EOS_Ro::ClcStart(Data);
    //    //HugoniotClass hug(Mat->Mat);
    //    //double dv, v1, MindV, MaxdV;
    //    //hug.SetHugPresClcVar(Start, FinPres,dv,v1,MindV,MaxdV);
    //    //cout<<"Estimated dV "<<dv<<" v1 "<<v1<<" MindV "<<MindV<<" MaxdV "<<MaxdV<<"\n";
    //};
 //   virtual Ref<SavableClass> GetRes(const Stroka &SubName){
 //       return new SavableDouble(Result);
	//}

    virtual int save_data_state( FilterOut&so) { 
        UrsCurve_EOS_Ro::save_data_state(so);
        so<<"StartDenc"<<Start.Denc<<"StartEner"<<Start.Ener<<"StartPres"<<Start.Pres<<
            "StartVel"<<Start.Vel<<"FinPres"<<FinPres;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        UrsCurve_EOS_Ro::read_data_state(si);
        double StartVel,StartPres,StartDenc,StartEner;
        char tmp[256];
        si>>tmp>>StartDenc>>tmp>>StartEner>>tmp>>StartPres>>tmp>>StartVel>>tmp>>FinPres;
        Start = HugoniotClass::HugoniotData(StartVel,StartPres,StartDenc,StartEner,0);
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "PressureHug Misfit for different dV(var).\n";
        return res;
	}
protected:
	//virtual void MakeHelp(FilterTextOut &out){
 //       out<<"PressureHug Misfit for different dV(var).\n";
 //       Ref<UrsCurve_EOS_Hugoniot> form = new UrsCurve_EOS_Hugoniot();
 //       out<<form;
	//}
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
        HugoniotClass hug(Mat->Mat);
        //double dv, v1, MindV, MaxdV;
        //hug.SetHugPresClcVar(Start, FinPres,dv,v1,MindV,MaxdV);
    hug.Start = Start;
    hug.MD_dP = FinPres - Start.Pres;
    hug.MD_v1 = 1/Start.Denc;
    hug.MD_de = -(Start.Pres+FinPres)/2;
    hug.MD_p = FinPres;
    hug.MD_e = Start.Ener;
    hug.CurPtr = &hug;

        Res = hug.MisHugPres_D(GetVar(NameDenc));
    }
    HugoniotClass::HugoniotData Start, Result;
    double FinPres;
};

//==============================================================================================
//============================      UrsCurve_MatFreeSplConstr     ==============================
//==============================================================================================

#include "mat/spl_fac.h"
struct UrsCurve_SplConstr:URS_Curve::ClcVar{
    struct Result:SavableClass{
        Result(){};
	    virtual int save_data_state( FilterOut&so) {return 1; };
	    virtual int read_data_state(FilterIn&si){ return 1;};
    };
	UrsCurve_SplConstr():URS_Curve::ClcVar(),NameX("X_Temperature2FreeESpl"),NameY("Y_Dencity2FreeESpl"),NameZ("Z_FreeE2FreeESpl"),
		ResSplineName("XY_Z_spline.spl"),SplineDescription("Zero_Spline"),spl(new ExtendedRegridSpline()){};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
    	Ref<URS_Curve::ClcVar> x,y,z;
		x = Dat->GetClcVar(NameX); y = Dat->GetClcVar(NameY); z = Dat->GetClcVar(NameZ);
		if (!x || !y || !z)
            throw info_except("Could not get X or Y or Z curve. X %s Y %s Z %s\n",NameX.c_str(), NameY.c_str(), NameZ.c_str());
    };

    virtual void ClcEnd(){
		TColumn<real> dat;
		dat.SetColumn((real*)X, X.SetNumEl(), 0);
		dat.SetColumn((real*)Y, Y.SetNumEl(), 1);
		dat.SetColumn((real*)Z, Z.SetNumEl(), 2);
        if (dat.I[2] == 0)
            throw info_except("There is no data in the input file n1<%i> n2<%i> n3<%i> datN<%i>\n", dat.I[0], dat.I[1], dat.I[2], dat.N);
		
		int ret = spl->Generate(0,dat);
		if (ret!=0)
            throw info_except("Could not generate spline!!!. Return code is %i \n",ret);
		FilterTextOut out(ResSplineName.c_str());
		out<<"Raw_SplineDescription_Str"<<SplineDescription<<"\n";
		out<<"GeneratedSpline\n";
		out.SetRefMask(SavableClass::SimpleEdit);
		out<<spl;
    };
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        return new Result();
	}

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameX"<<NameX.c_str()<<"NameY"<<NameY.c_str()<<"NameZ"<<NameZ.c_str()<<"ResSplineName"<<ResSplineName.c_str()<<"Raw_SplineDescription_Str"<<SplineDescription.c_str();
		spl->save_generation_par(so);
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
		si>>tmp>>NameX>>tmp>>NameY>>tmp>>NameZ>>tmp>>ResSplineName>>tmp>>SplineDescription;
		spl->read_generation_par(si);
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "Class for generating  XY_Z spline.\nResult spline file can be used by MatterFreeSpl - the same format\n";
		res += " Spline Format :\nRaw_SplineDescription_Str Any_Str_All_This_Is_Skipped_Up_To_GeneratedSpline_Str\n";
		res += "GeneratedSpline SplineXYIO_Class_In_Simple_Out_Format\n";
		res += "Ideally - create new class - for storing - retrieving FreeSpl file - now it is stored by this class, used by MatterFreeSpl class...\n";
        return res;
	}
protected:

    virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
		X.push(GetVar(NameX));Y.push(GetVar(NameY));Z.push(GetVar(NameZ));
    }
protected:
    Stroka NameX, NameY, NameZ;
	Ref<SplineXYIO> spl;
	Stroka ResSplineName,SplineDescription;
	ResizeVector<real> X,Y,Z;


};





//==============================================================================================
//============================    Classes to work with famessb  ================================
//==============================================================================================



//==============================================================================================
//============================    UrsCurve_VarStorage       ====================================
//==============================================================================================

struct UrsCurve_VarStorage:URS_Curve::ClcVar{
    UrsCurve_VarStorage(){};

    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		Ref<ResizeVector<double> > vec = GetName(SubName);
		if (Dat->CurItet>=vec->SetNumEl())
			throw info_except("Cannot calculate index %i for variable %s num el is only %i\n",Dat->CurItet,SubName.c_str(),vec->SetNumEl());
		return new SavableDouble(vec->operator[](Dat->CurItet));
	}

	virtual int save_data_state( FilterOut&so) { 
		so<<" Variables { ";
		for(map<Stroka, Ref<ResizeVector<double> > >::iterator it = Vars.begin();it!=Vars.end();it++)
			so<<it->first<<" ";
		so<<" } ";
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si>>tmp>>tmp>>tmp;
		Vars.clear();
        while (stricmp(tmp,"}")!=0){
            Vars[Stroka(tmp)] = new ResizeVector<double>();
            si>>tmp;
        }
        return 1; 
    };
	virtual void SetCurRes(const Stroka &SubName,SavableClass *cl){
		Ref<ResizeVector<double> > vec = GetName(SubName);
		SavableDouble *doub = dynamic_cast<SavableDouble*>(cl);
		if (!doub)
			throw info_except("Cannot set the variable of type %s it have to be SavableDouble\n",cl->class_name());
		vec->operator[](Dat->CurItet) = doub->Dat;
	}

	Ref<ResizeVector<double> > GetVar(const char *name){
		Ref<ResizeVector<double> > vec = GetName(Stroka(name));

        return vec;
	}
	void SetVar(const char *name, Ref<ResizeVector<double> > vec){
		Vars[Stroka(name)] = vec;
	}
	int DelVar(const char *name){
		Vars.erase(Vars.find(Stroka(name)));
	}
	void EmptyVars(){
		for(map<Stroka, Ref<ResizeVector<double> > >::iterator it = Vars.begin();it!=Vars.end();it++)
			it->second->Release();
	}

	virtual Stroka MakeHelp(){
        Stroka res = "Class for storing variables ( can to set them before...).\n";
        return res;
	}
protected:
	map<Stroka, Ref<ResizeVector<double> > > Vars;
	//virtual void MakeHelp(FilterTextOut &out){
 //       out<<"Class for storing variables ( can to set them before...).\n";
 //       Ref<UrsCurve_VarStorage> form = new UrsCurve_VarStorage();
	//	form->Vars["Stor1"] = NULL;
	//	form->Vars["Stor2"] = NULL;
 //       out<<form;
	//}
	//virtual void ClcNewStp(int CurStp, const Stroka &SubName){
 //   };
	Ref<ResizeVector<double> > GetName(const Stroka &SubName){
		map<Stroka, Ref<ResizeVector<double> > >::iterator it = Vars.find(SubName);
		if (it==Vars.end())
			throw info_except("Cannot calculate variable %s\n",SubName.c_str());
		return it->second;
	}


};


//==============================================================================================
//============================    UrsCurve_Output2Store     ====================================
//==============================================================================================

struct UrsCurve_Output2Store:URS_Curve::Output{
    UrsCurve_Output2Store(){};
    UrsCurve_Output2Store(const char* clcName, const char* storeName){
        vector<Stroka> clc = Str::SplitLine(clcName), store = Str::SplitLine(storeName);
        if (clc.size() != store.size())
            throw info_except("size clc: %i store: %i\n", clc.size(), store.size());
        for(size_t i = 0; i < clc.size(); i++)
            OutNames.push_back(Out(~clc[i], ~store[i]));
    };
	virtual void OutStart(int NumIter, URS_Curve* Data){
        Dat = Data;
        for(list<Out>::iterator it = OutNames.begin();it!=OutNames.end();it++){
			Ref<URS_Curve::ClcVar> clc = Dat->GetClcVar(it->GetName);
            if (!clc) 
                throw info_except("variable %s is not of ClcVar type...\n",it->GetName.c_str());
        }
    }
	virtual void OutStep(){
        for(list<Out>::iterator it = OutNames.begin();it!=OutNames.end();it++){
            Ref<SavableClass> dat= Dat->CalculateVar(it->GetName);
            if (dat){
		        Stroka SubName,VarName = Dat->MakeNames(it->SetName, SubName);
				UrsCurve_VarStorage *stor = dynamic_cast<UrsCurve_VarStorage*>((SavableClass*)Dat->GetVar(it->SetName));
				if (!stor) 
					throw info_except("variable %s is not of UrsCurve_VarStorage type...\n",it->SetName.c_str());
				stor->SetCurRes(SubName,dat);
            }
        }
	}
    virtual void OutEnd(){};
	virtual int save_data_state( FilterOut&so) {
        so<<" ClcName_StoreName { ";
        for(list<Out>::iterator it = OutNames.begin();it!=OutNames.end();it++)
            so<<it->GetName<<" "<<it->SetName<<"    ";
        so<<" }\n";
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256],tmp1[256];
        si>>tmp>>tmp>>tmp;
        while (stricmp(tmp,"}")!=0) {
			si>>tmp1;
            OutNames.push_back(Out(tmp,tmp1));
            si>>tmp;
        }
        return 1;
    };
	//virtual string print_help(){
	//	Stroka category = DataSource::GenerateUnicName("help");
	//	FilterTextOut tmp(category.c_str(),DataSource::Memory);
 //       tmp.SetRefMask(SavableClass::UnlinkedObjects);
	//	tmp<<"Class for saving the results.\ninput: ClcNames ToSaveInNames(UrsCurve_VarStorage)\n";
 //       Ref<UrsCurve_Output2Store> form = new UrsCurve_Output2Store();
	//	form->OutNames.push_back(Out("ClcVar1", "StoreVar.Var1"));
	//	form->OutNames.push_back(Out("ClcVar2", "StoreVar.Var2"));
 //       tmp<<form;
	//	return string(DataSource::GetStr(category.c_str()).c_str());
 //   }
    virtual Stroka MakeHelp(){
		Stroka res = "Class for saving the results.\ninput: ClcNames ToSaveInNames(UrsCurve_VarStorage)\n";
		return res;
    }
protected:
	struct Out{
		Out(const char *get, const char *set):GetName(get),SetName(set){}
		Stroka GetName,SetName;
	};
    list<Out> OutNames;
    URS_Curve *Dat;

};




//==============================================================================================
//============================      UrsCurve_FreeInputOutput     ===============================
//==============================================================================================
struct UrsCurve_InOut:URS_Curve::ClcVar{
	UrsCurve_InOut():URS_Curve::ClcVar(),NameMatter("Mat"){
        InputVar_Mat["Denc"] = "Dencity";
        InputVar_Mat["Ener"] = "Energy";
    };
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
        Stroka ErrStr;
		if ( !(Mat  = dynamic_cast<EOS_Savable*>((SavableClass*)Dat->GetVar(NameMatter))) )
		    ErrStr += " Cannot get input matter variable " + NameMatter + "\n";
		map<Stroka,void*> inN = Mat->Mat->GetInputNamesMap();
		    
        for(map<Stroka,Stroka>::iterator it = InputVar_Mat.begin();it!=InputVar_Mat.end();it++){
            if ( !dynamic_cast<URS_Curve::ClcVar*>((SavableClass*)Dat->GetVar(it->first)) )
                ErrStr += "cannot find input variable " + it->first + "\n";
            if (Mat && inN.find(it->second)==inN.end())
                ErrStr += "Matter does not use variable " + it->second + "\n";
        }
		if (ErrStr.length()!=0)
			throw info_except("Were found errors :%s\n",ErrStr.c_str());
        ResultBuf.clear();
    };

    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
		if (ResultBuf.find(SubName)!=ResultBuf.end())
		    return new SavableDouble( ResultBuf[SubName] ); 
        map<Stroka,double> in;
        for(map<Stroka,Stroka>::iterator it = InputVar_Mat.begin();it!=InputVar_Mat.end();it++)
            in[it->second] = GetVar(it->first);
        ResultBuf[SubName]  = Mat->Mat->OutputValClc(in, SubName );
		return 	new SavableDouble( ResultBuf[SubName] );
	}

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameMatter"<<NameMatter.c_str()<<"Input_Mat2Clc_Name (";
		for(map<Stroka,Stroka>::iterator it = InputVar_Mat.begin();it!=InputVar_Mat.end();it++)
		    so<<it->first.c_str()<<it->second.c_str()<<"   ";
		so<<")";
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si>>tmp>>NameMatter>>tmp;
        InputVar_Mat.clear();
        TestNextWord(si,"(");
        while( !(!si) && !SavableClass::TestNextChar(si,')') ){
            Stroka NameVar, NameInt;
            si>>NameInt>>NameVar;
            InputVar_Mat[NameInt] = NameVar;
        }
        //si>>tmp;
        return !(!si); 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "Class for any EOS calculation (any number of input data and output data - depending on Matter...)\n";
        return res;
	}
protected:
	//virtual void MakeHelp(FilterTextOut &out){
	//	out<<"Class for any EOS calculation (any number of input data and output data - depending on Matter...)\n";
	//	Ref<UrsCurve_InOut> form = new UrsCurve_InOut();
 //       out<<form;
	//}
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
	    ResultBuf.clear();
	}
	map<Stroka,double> ResultBuf;
    map<Stroka,Stroka> InputVar_Mat;
    Stroka NameMatter;
   	Ref<EOS_Savable> Mat;
};










#include "mat\OneVarCurves.h"
struct UrsCurve_OneVarFunction:URS_Curve::ClcVar{
	UrsCurve_OneVarFunction():URS_Curve::ClcVar(),NameX("X"), Func(new Cold_Vinet), StartIntPnt(1) {};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
    	Ref<URS_Curve::ClcVar> X;
		X = Dat->GetClcVar(NameX);
		if (!X )
			throw info_except("Could not get Xvar ..\n");
    };

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameX"<<NameX.c_str()<<"OneVarFunction";
        SavableClass::Save(so, Func);
        so<<"StartIntPnt"<<StartIntPnt<<" ClcIntegral "<<ClcIntegral;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si>>tmp>>NameX>>tmp;
        Func << SavableClass::Read(si);
        si>>tmp>>StartIntPnt>>tmp>>ClcIntegral;
        if (!si)
            throw info_except(" Could not read object...Ups\n");
        return 1; 
    };


    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        if (SubName=="Val") 
            return new SavableDouble(ResV);
        else if (SubName=="Integral") 
            return new SavableDouble(ResI);
        else if (SubName=="Derivative") 
            return new SavableDouble(ResD);
		    else throw info_except("Cannot calculate variable %s. Calculate [Val, Derivative, Integral]\n",SubName.c_str());
	}
	virtual Stroka MakeHelp(){
        Stroka res = "Get X  var and StartIntPnt  - result - \n"+
            Stroka("Val Integral Derivative\n")+
            Stroka("Variables for output: Val, Integral, Derivative\n")+
            Stroka("OneVarFunction result. Input - OneVarFunction_category function\n")+
            Stroka("OneVarFunction_category functions:\n\n\n") + SavableClass::HelpForCategory("OneVarFunction_category");
        return res;
	}


protected:
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
        double X = GetVar(NameX);
        ResV = Func->Calculate(X);
        ResI = 0;
        if (ClcIntegral)
            ResI = Func->Integral(StartIntPnt, X);
        ResD = Func->Derivative(X);
	}

    Stroka NameX;
    Ref<OneVarFunction> Func;
    double StartIntPnt;
    double ResV, ResI, ResD;
    int ClcIntegral;
};


struct UrsCurve_NamedOneVarFunction:URS_Curve::ClcVar{
	UrsCurve_NamedOneVarFunction():URS_Curve::ClcVar(),NameX("X"), Func(new Cold_Vinet) {};
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
    	Ref<URS_Curve::ClcVar> X;
		X = Dat->GetClcVar(NameX);
		if (!X )
			throw info_except("Could not get Xvar ..\n");
    };

    virtual int save_data_state( FilterOut&so) { 
		so<<"NameX"<<NameX.c_str()<<"NamedOneVarFunction";
        SavableClass::Save(so, Func);
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si>>tmp>>NameX>>tmp;
        Func << SavableClass::Read(si);
        return 1; 
    };


    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        return new SavableDouble(Calculated[SubName]);
        //return new SavableDouble(Func->Calculate(GetVar(NameX), SubName));
	}
	virtual Stroka MakeHelp(){
        Stroka res = "Get X  var  - result - \n"+
            Stroka("NamedOneVarFunction result. Input - NamedOneVarFunction_category function\n")+
            Stroka("NamedOneVarFunction_category functions:\n\n\n") + SavableClass::HelpForCategory("NamedOneVarFunction_category");

        return res;
	}


protected:
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        Calculated[SubName] = Func->Calculate(GetVar(NameX), SubName);
	}

    Stroka NameX;
    map<Stroka, double> Calculated;
    Ref<NamedOneVarFunction> Func;
};


struct UrsCurve_ManyVarFunction:URS_Curve::ClcVar{
    UrsCurve_ManyVarFunction():URS_Curve::ClcVar(),Func(new Spl2DGenerator) {
        NameXvar.push_back("Xvar");
        NameXval.push_back("X");
    };
	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
        for(size_t i = 0;i<NameXvar.size();i++)
            if (!Dat->GetClcVar(NameXvar[i]))
                throw info_except("Can not find ClcVar <%s>\n", NameXvar[i].c_str());
    };

    virtual int save_data_state( FilterOut&so) { 

        so<<" InputVal_Var:Clc { ";
        for(size_t i = 0;i<NameXvar.size();i++)
            so<<NameXvar[i]+":"+NameXval[i];
        so<<" } ManyVarFunction ";
        SavableClass::Save(so, Func);
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        NameXvar.clear();
        NameXval.clear();
        si>>tmp>>tmp>>tmp;
        while (stricmp(tmp,"}")!=0 && !(!si) ){
            vector<Stroka> vec = Str::SplitLine(tmp,0,':');
            if (vec.size()!=2)
                throw info_except("Wrong input have to be of type <a:b> and is <%s>\n", tmp);
            NameXvar.push_back(vec[0]);
            NameXval.push_back(vec[1]);
            si>>tmp;
        }
        si>>tmp;
        Func << SavableClass::Read(si);
        return 1; 
    };


    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
		if (Calculated.find(SubName) == Calculated.end())
			return 0;
        return new SavableDouble(Calculated[SubName]);
	}
	virtual Stroka MakeHelp(){
        Stroka res = "Input X  vars  - result - arbitrary...\n"+
            Stroka("ManyVarFunction result. Input - ManyVarFunction_category function\n")+
            Stroka("ManyVarFunction_category functions:\n\n\n") + SavableClass::HelpForCategory("ManyVarFunction_category");
        return res;
	}


protected:
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (FirstVarStpClc(this)){
            Calculated.clear();
            for(size_t i = 0;i<NameXvar.size();i++)
                Xvar[NameXval[i]] = GetVar(NameXvar[i]);
        }
        Calculated[SubName] = Func->Calculate(Xvar, SubName);
	}

    vector<Stroka> NameXvar, NameXval;
    map<Stroka, double> Calculated, Xvar;
    Ref<ManyVarFunction> Func;
};


