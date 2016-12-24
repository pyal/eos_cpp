#ifndef __FRE_FAC_H
#define __FRE_FAC_H 


#include "lib/std/util.h"
#include "lib/std/stdexception.h"
#include "matt_fac.h"
#include "h2_liqui.h"
#include "empi_equ.h"
#include "mat/mtzero.h"
#include "mat/Quad.h"
#include "mat/matrics.h"
#include "vector"
#include "matt_tools.h"
#include "util/code_gen/calculator/calculator_std.h"


//#define NEWTON_TEMPERATURE_ITERATION 
//#define FZERO_TEMPERATURE_ITERATION 
#define FZERO_LOGTEMPERATURE_ITERATION 
//#ifdef MULTIDIM_TEMPERATURE_ITERATION

struct InterfaceFreeEIO : FreeFuncClc {
   virtual double FreeE(double Dencity,double Temperature)=0;
   virtual double Pressure(double Dencity,double Temperature)=0;
   virtual double Energy(double Dencity,double Temperature)=0;
   friend int FreeERegister(istream &in,InterfaceFreeEIO *&Func);
   friend int FreeEStore(ostream &out,InterfaceFreeEIO *&Func);
   virtual  void FreeE(double *F,double *Dencity,double *Temperature,int Num);
   virtual  void Pressure(double *P,double *Dencity,double *Temperature,int Num);
   virtual  void Energy(double *E,double *Dencity,double *Temperature,int Num);
   virtual void SetTLimits(double &minT, double &maxT, double &stpTcoef, double &stpTmin);

	//virtual string print_help(){
	//	FilterTextOut tmp("InterfaceFreeEIO::print_help",DataSource::Memory);
	//	tmp.SetRefMask(SavableClass::UnlinkedObjects);
	//	MakeHelp(tmp);
	//	return string(DataSource::GetStr("InterfaceFreeEIO::print_help").c_str());
	//}
	//virtual void MakeHelp(FilterTextOut&out){
	//	out<<"-----------   "<<this->class_name()<<"   --------------------------\n";
	//	out<<"Create:\n\t";
	//	SavableClass::SavableClassStore(out,this);
 //       out<<"\nOutputClcNames: "<<OutputSubstancePar().c_str();
	//	out<<"\n----------------------------------------------\n";
	//}
    //virtual Stroka save_data_state( ){
    //    FilterTextOut out("MatterIO::save_data_state::aa",DataSource::Memory);
    //    SavableClass::save_data_state(out);
    //    return Stroka(DataSource::GetStr("MatterIO::save_data_state::aa").c_str());
    //}

// From mat_fac.h ...........
   InterfaceFreeEIO() : FreeFuncClc() {
       SetInternalNames();
   };
	virtual void SetInternalNames(){
        //InputNames.clear();InputNames["Dencity"] = NULL;InputNames["Temperature"] = NULL;
        SetInternalNames(InputNames);
		OutputNames.clear();
        OutputNames["Pressure"] = NULL; OutputNames["FreeE"] = NULL; OutputNames["Energy"] = NULL;
	}
	void SetInternalNames(map<Stroka,void*> &Input){
        Input.clear();Input["Dencity"] = NULL;Input["Temperature"] = NULL;
	}
	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
		if (Output=="Pressure")
			return Pressure(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature") );
		if (Output=="Energy")
			return Energy(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature") );
		if (Output=="FreeE")
			return FreeE(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature") );

        Stroka OutNames = Stroka("\nName_ToClc: ") + Stroka(Output) + Stroka(" is unknown.\n NamesPresent: ") + GetOutputNames();
        OutNames +=Stroka("\nSubstanceParams: ")+SavableClass::object2string(this);
		throw info_except("%s",OutNames.c_str());
		return 0;
	}
    using FreeFuncClc::OutputValClc;
	virtual double OutputValClc(double Dencity,double Temperature, const Stroka &Output){
		if (Output=="Pressure")
			return Pressure(Dencity,Temperature);
		if (Output=="Energy")
			return Energy(Dencity,Temperature);
		if (Output=="FreeE")
			return FreeE(Dencity,Temperature);
		map<Stroka,double> par;par["Dencity"] = Dencity;par["Temperature"] = Temperature;
		return OutputValClc(par, Output);
	}
    //virtual Stroka MakeHelp(){
    //    Stroka res = Stroka("OutputClcNames: ") + GetOutputNames().c_str() + "\n";
    //    res += Stroka("InputClcNames: ") + GetInputNames().c_str() + "\n";
    //    return res;
    //}

    //Stroka OutputSubstancePar(){FilterTextOut out("aa",DataSource::Memory);SaveSubstancePar(out);return Stroka(DataSource::GetStr("aa").c_str());};

//#ifdef INTERNAL_FUNC
//   virtual void Int_Par(double *Input,double *Output){return ;};
//#endif
  };

struct FreeEIO : InterfaceFreeEIO
  {
  FreeEIO():InterfaceFreeEIO(){};
   double Pressure(double Dencity,double Temperature)
     {double dr=0.001*Dencity;
	  double P=sqr(Dencity)/dr*
             (FreeE(Dencity+dr*0.5,Temperature)-FreeE(Dencity-dr*0.5,Temperature));
      return P;};
   double Energy(double Dencity,double Temperature)
     {double dr=max<double>(0.005*Temperature,10);
      double E=-sqr(Temperature)/dr*
             (FreeE(Dencity,Temperature+dr)/(Temperature+dr)-
                FreeE(Dencity,Temperature)/Temperature);
      return E;};

  };

struct FreeEIOFind:FreeEIO {
    FreeEIOFind() : FreeEIO() {}
    FindSubstanceIO Finder;
	FreeEIOFind(const char *file,const char *subst):FreeEIO(),Finder(file,subst){};
	virtual void ReadSubstancePar(istream &in)=0;
	virtual void SaveSubstancePar(ostream &out)=0;
	virtual int read_data_state(FilterIn &input){
		input>>Finder;
		istream *in = dynamic_cast<istream*>(Finder.GetReadStream());
        try {
		    ReadSubstancePar(*in);
        } catch(stdexception &ex) {
            throw info_except("Got an error %s.\n %s",ex.what(), ReadingError(in).c_str());
        }
		if ( !(*in) )
			throw info_except(ReadingError(in).c_str());

		return 1;
	}
	virtual int save_data_state( FilterOut &output){
		output<<Finder;
		return 1;
	}
    double DencityCold() {return StndErr;};
    double DencityMax()  {return 1e2;};
    Stroka SaveSubstancePar(){
        FilterTextOut out("FreeEIOFind:FreeEIO::aa",DataSource::Memory);
        SaveSubstancePar(out);
        return Stroka(DataSource::GetStr("FreeEIOFind:FreeEIO::aa").c_str());
    }
	virtual Stroka MakeHelp(){
        Stroka res = FreeEIO::MakeHelp();
        res += Stroka("Configuration parameters are:\n\t") + SaveSubstancePar();
        return res;
	}
private:
    virtual Stroka ReadingError(istream *in){
        Stroka ret("Could not read in file <" + Finder.FileName + ">  matter <" + Finder.Substance + 
            "> For class <" + class_name() + ">\n");
        if (in == NULL)
            return ret + "Input stream is NULL\n";
        ret += SavableClass::ShowStreamPosition(in);
        return ret;
    };

};


// ========================================================
// ========================================================
// ========================================================

struct MatterFreeE:MatterIO{
    Ref<InterfaceFreeEIO> FreeEPtr;
    double E,D,L_T,L_E,L_D;
        //,Zero_Temp;
    double MinimT2EstpCoef, MinimT2EstpMin, MinimTmin, MinimTmax;
    static MatterFreeE* Current;
    static double AbsoluteMinTemperature;
    double TemperatureFind(double En,double Den,double Low,double Up,
                                    double Start,double Err,int NumIt);
    static double DeltaFree(double T){
       return Current->FreeEPtr->Energy(Current->D,T)-Current->E;
    }
    static double DeltaFree(VecCl &T)  { return sqr(DeltaFree(T[1]));}
    static double DeltaFreeLog(double lnT){
        double dE=Current->FreeEPtr->Energy(Current->D,exp(lnT))-Current->E;
//cout<<" DF T "<<lnT<<" T "<<exp(lnT)<<" D "<<Current->D<<" E "<<Current->E<<" dE "<<dE<<"\n";
        return dE;//Current->FreeEPtr->Energy(Current->D,exp(lnT))-Current->E;
    }
    static double DeltaFreeLog(VecCl &T)  { return sqr(DeltaFreeLog(T[1]));}


    void Pressure(double *P,double *Dencity,double *Energy,int Num);
    void Sound(double *S,double *Dencity,double *Energy,int Num);
    void Temperature(double *T,double *Dencity,double *Energy,int Num);

    double Pressure(double Dencity,double Energy);
    double Sound(double Dencity,double Energy);
    double Temperature(double Dencity,double Energy);
    double Energy(double Dencity,double Temperature);
    double DencityCold(){return 1e-4;}
    double DencityMax(){return 100;}
   
//  For calculation  Temperature to energy - delicate parameters Zero_Temp  L_T
    MatterFreeE();//{FreeEPtr=new FreeEIdeal;L_T=1000;L_D=0;L_E=0;Zero_Temp=Zer_T;};
    virtual int save_data_state( FilterOut&so) {return SavableClass::Save(so,FreeEPtr);};
    virtual int read_data_state(FilterIn&si) { 
        if (!(FreeEPtr << SavableClass::Read(si)))
            return 0;
        FreeEPtr->SetTLimits(MinimTmin, MinimTmax, MinimT2EstpCoef, MinimT2EstpMin);
//cout << MinimTmin << " " << MinimTmax << " " << MinimT2EstpCoef << " " << MinimT2EstpMin << "\n";
        return 1;
    };
    //~MatterFreeE(){delete FreeEPtr;FreeEPtr=NULL;};
 //   virtual void MakeHelp(FilterTextOut&out){
	//	out<<"-----------   "<<this->class_name()<<"   --------------------\n";
	//	out<<"Create:\n\t";SavableClass::SavableClassStore(out,this);
	//	out<<"\n----------------------------------------------\n";
	//}
	void SetNewMatter(InterfaceFreeEIO *cur){
        FreeEPtr=cur;
		SetInternalNames();
    	map<Stroka, void*> input, output;
		FreeEPtr->GetInternalNames(input, output);
		input.erase("Temperature");
        for(map<Stroka, void*>::iterator it = input.begin();it!=input.end();it++)
            InputNames[it->first] = it->second;
        for(map<Stroka, void*>::iterator it = output.begin();it!=output.end();it++)
            OutputNames[it->first] = it->second;
        FreeEPtr->SetTLimits(MinimTmin, MinimTmax, MinimT2EstpCoef, MinimT2EstpMin);
//cout << MinimTmin << " " << MinimTmax << " " << MinimT2EstpCoef << " " << MinimT2EstpMin << "\n";
	}

	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
		double Dencity, Temperature, Energy;
		FreeEPtr->StoreInputVal(Input);

		if (!CheckPresence(Input, "Dencity",Dencity))
			throw info_except("Dencity var was not found...\n");
		if ( CheckPresence(Input, "Temperature",Temperature) );
		else if ( CheckPresence(Input, "Energy",Energy) )
			Temperature = this->Temperature(Dencity, Energy);
		else throw info_except(" Energy and Temperature vars are not defined...\n");
		if (Output=="Temperature")
			return Temperature;
		if (Output=="Sound"){
			if (!CheckPresence(Input, "Energy",Energy))
				Energy = FreeEPtr->Energy(Dencity,Temperature);
			return Sound(Dencity, Energy);
		}
		if (Output=="Energy")
			return FreeEPtr->Energy(Dencity, Temperature);
		if (Output=="Pressure")
			return FreeEPtr->Pressure(Dencity, Temperature);
		if (Output=="FreeE")
			return FreeEPtr->FreeE(Dencity, Temperature);
		
		map<Stroka,double> in = Input;in["Temperature"] = Temperature;
		return FreeEPtr->OutputValClc(in, Output);
	}

};	

//===================================================
//=========  Real Free Functions   ==================
//===================================================
struct FreeETmp:FreeEIOFind
  {
      FreeETmp():FreeEIOFind("material.cfg","FreeETmpH2"),
          MolVeight(2),Zero(0), CvId(1.5), HiT(0), Tvib(6390), Trot(170.8){};
    double MolVeight,Zero,CvId,HiT,Tvib,Trot;
	double FreeE(double Denc,double T){
		double Evol=log(Denc/MolVeight);
	    double kt=M_Rconst/MolVeight*T;
		double F=Zero+kt*(Evol-CvId*log(T)+HiT);
		return F;
	}
    void ReadSubstancePar(istream &in){
        char tmp[256];
        in>>tmp>>CvId>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT>>tmp>>Tvib>>tmp>>Trot;
    };
    void SaveSubstancePar(ostream &out){
        out<<" CvId "<<CvId<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT<<" Tvib "<<Tvib<<" Trot "<<Trot<<" ";
    }
  }; 

//struct CalculatorProgam;
//#include "urs\test\urs_curve_calculator.h"
struct FreeEFormula:FreeEIOFind{
    FreeEFormula():FreeEIOFind("material.cfg","FreeEFormula"),EndSymbol("EOF"),
        PrgStr("double MolVeight,CvId,kT,Evol;CvId=1.5;MolVeight=2;kT=M_Rconst/MolVeight*Temp;Evol=log(Denc/MolVeight);FreeE=kT*(Evol-CvId*log(Temp));")
        {InitProgram();};
    double FreeE(double Denc,double T);
    void ReadSubstancePar(istream &in){
        Stroka str;
        PrgStr = "";
        in>>str>>EndSymbol>>str;
        while (str!=EndSymbol){
            PrgStr += str;
            if (seek_eoln(in))
                PrgStr += "\n";
            else
                PrgStr += " ";
            in>>str;
        }
        InitProgram();
    };
    void InitProgram();
    void SaveSubstancePar(ostream &out){
        out<<"EndSymbol "<<EndSymbol<<" "<<PrgStr<<" "<<EndSymbol<<" ";
    }
private:
    Stroka EndSymbol, PrgStr;
    CalculatorProgam prg;

};
struct FreeEIdeal:FreeEIOFind {
    FreeEIdeal():FreeEIOFind("material.cfg","FreeEIdealH2"),
          MolVeight(2),Zero(0), CvId(1.5), HiT(0), Tvib(6390), Trot(170.8){};
    double MolVeight,Zero,CvId,HiT,Tvib,Trot;
    double FreeE(double Denc,double T);
    void ReadSubstancePar(istream &in){
        char tmp[256];
        in>>tmp>>CvId>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT>>tmp>>Tvib>>tmp>>Trot;
        HiT += ClcAddHiT(MolVeight,CvId);
//cout<<"File "<<Finder.FileName<<" Matter "<<Finder.Substance<<" CvId "<<CvId<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT-ClcAddHiT(MolVeight,CvId)<<" Tvib "<<Tvib<<" Trot "<<Trot<<"\n";
    };
    void SaveSubstancePar(ostream &out){
        out<<" CvId "<<CvId<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT-ClcAddHiT(MolVeight,CvId)<<" Tvib "<<Tvib<<" Trot "<<Trot<<" ";
    }
    static double ClcAddHiT(double molveight, double cvid);
}; 
struct FreeEIdealElectron:FreeEIOFind{
    FreeEIdealElectron():FreeEIOFind("material.cfg","FreeEIdealElectronH2"),
          MolVeight(2),Zero(0), HiT(0){};
    double MolVeight,Zero,HiT;
    double FreeE(double Denc,double T);
    void ReadSubstancePar(istream &in){
        char tmp[256];
        in>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT;
        HiT += ClcAddHiT(MolVeight);
    };
    void SaveSubstancePar(ostream &out){
        out<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT-ClcAddHiT(MolVeight)<<" ";
    }
    static double ClcAddHiT(double molveight);
    virtual Stroka MakeHelp() {return Stroka("Ideal electron gas EOS. g-factor - included/n") + FreeEIOFind::MakeHelp() ;}
}; 

struct FreeEVanDerVals:FreeEIOFind
  {
      FreeEVanDerVals():FreeEIOFind("material.cfg","VanDerValsN2"),
          A(0.10875),B(1.0965),MolVeight(28),Zero(0),CvId(2.5),HiT(0){};
   double A,B,MolVeight,Zero,CvId,HiT;
   double FreeE(double Denc,double T);
   double DencityCold() {return StndErr;};
   double DencityMax()  {return B-MathZer;};
   void ReadSubstancePar(istream &in);
   void SaveSubstancePar(ostream &out);
  }; 
struct FreeEVirial:FreeEIOFind{
    FreeEVirial():FreeEIOFind("material.cfg","Nothing"),
        T0(0),V0(0),MolVeight(0),Zero(0),CvId(0),HiT(0),NCi(2),He_delta(0){Ci[0]=Ci[1]=Ci[2]=0;};
   double T0,V0,MolVeight,Zero,CvId,HiT;
   double Ci[10];
   int NCi,He_delta;
   double FreeE(double Denc,double T);
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
   void ReadSubstancePar(istream &in);
   void SaveSubstancePar(ostream &out);
  }; 
///*
struct FreeEH2Liquid:FreeEIOFind{
    FreeEH2Liquid():FreeEIOFind("h2_param.cfg","H2_GrigorievTurn") { SetConstants(); };
   double FreeE(double Denc,double T) {return FLiquid( Denc, T);  };
   void ReadSubstancePar(istream &in) {H2ReadPar(in);}
   void SaveSubstancePar(ostream &out){H2SavePar(out);}
  }; 
struct FreeEH2Solid:FreeEIOFind{

   FreeEH2Solid():FreeEIOFind("h2_param.cfg","H2_GrigorievTurn"){ SetConstants(); };
   double FreeE(double Denc,double T) {return FSolid(Denc, T); };
   void ReadSubstancePar(istream &in) {H2ReadPar(in);}
   void SaveSubstancePar(ostream &out){H2SavePar(out);}
  }; 


// To continue  from here!!!!!!!!!!!!!!!!!!!!!!!!!!
struct FreeEEmpiSolid:FreeEIOFind{
    FreeEEmpiSolid():FreeEIOFind("h2_param.cfg","no_help"){};
   EmpiParamStruct Empi;
   double FreeE(double Denc,double T) {return EmpiFSolid(Denc, T, &Empi); };
   void ReadSubstancePar(istream &in) {EmpiH2ReadPar(in,&Empi);}
   void SaveSubstancePar(ostream &out){EmpiH2SavePar(out,&Empi);}
  }; 

struct FreeEEmpiLiquidPres:FreeEEmpiSolid{
   EmpiParamStruct Empi;
   FreeEEmpiLiquidPres():FreeEEmpiSolid(){SetInternalNames();};
   double FreeE(double Denc,double T) {return EmpiFDisPres( Denc, T,&Empi);};
	virtual void SetInternalNames(){
		FreeEIOFind::SetInternalNames();
        OutputNames["DisDegree"]=NULL;
  	}
	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
		if (CheckPresence(OutputNames, Output) && Output==Stroka("DisDegree"))
            return EmpiFDisPresDegree(CheckPresence(Input, "Dencity"),CheckPresence(Input, "Temperature"),&Empi);
		return FreeEIOFind::OutputValClc(Input, Output);
	}

  }; 
struct FreeEEmpiLiquid:FreeEEmpiLiquidPres{
//   EmpiParamStruct Empi;
    FreeEEmpiLiquid():FreeEEmpiLiquidPres(){};
   double FreeE(double Denc,double T) {return EmpiFDis( Denc, T,&Empi);};
//#ifdef INTERNAL_FUNC
//   void Int_Par(double *Input,double *Output)
//   {Output[1]=EmpiFDisDegree(Input[1],Input[2],&Empi);};
//#endif

  }; 
struct FreeEMetHCold:FreeEIOFind{
    FreeEMetHCold():FreeEIOFind("material.cfg","FreeEMetHColdTransl"),
        AddEG(1),AddLDA(1),AddOCP(1),AddDeb(1),AddTransl(1),DebDenc0(0.09),DebKoef(2190){};
   int AddEG,AddLDA,AddOCP,AddDeb,AddTransl;
    double DebDenc0,DebKoef;
   double FreeE(double Denc,double T);
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e5;};
   void ReadSubstancePar(istream &in);
   void SaveSubstancePar(ostream &out);
  }; 

struct FreeEElectron:FreeEIOFind {
    double MolVeight,Zero,HiT,NumEl;
    FreeEElectron():FreeEIOFind("material.cfg","FreeEH2Electron"),MolVeight(2),Zero(0),HiT(0),NumEl(1){}
    double FreeE(double Denc,double T){
        if ((T<MathZer) || (Denc<MathZer)) 
            { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
        double kt=M_Rconst/MolVeight*T;
        double Beta = pow(M_PI/3,2./3)*M_Mass_Electron_eV*M_eV_K/sqr(M_PlankCros_K);
        double El = -Beta/2*T*pow(MolVeight/(Denc*M_Na),2./3)/sqr(M_C);
	    double F=Zero+kt*(El+HiT);
        return F;
    }
    //double DencityCold() {return StndErr;};
    //double DencityMax()  {return 1e2;};
    void ReadSubstancePar(istream &in){
        char tmp[256];
        in>>tmp>>NumEl>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT;
    }
    void SaveSubstancePar(ostream &out) {
        out<<" NumEl "<<NumEl<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT<<" ";
    }
    virtual Stroka MakeHelp() {return Stroka("EOS of gas of electrons in solid body under low temper compared to temper of Fermi. Depricated - use FreeEElectronStrict instead\n") + FreeEIOFind::MakeHelp() ;};
}; 

#endif

//    For H2
//  t(krit)=33.23
//  p(krit)=13.16
//  v(krit)=63.8
//  Esub=0.7 kDj/g

//  VanDerVals
//                    N*b    N*N*a
//  F=Fid - N*T*ln(1- ---) - -----
//                     V       V
// (P+sqr(N/V)*a)*(V-N*b)=NT
// E=Eid-N*N*a/V
// for crit point: my approcs
// Pc*b+R*Tc=sqrt(3*Pc*a)
// (Pc*b+R*Tc)/(3*Pc)=Vc
// Landau crit point
// T��=(8/27*A/B)     /R?
// V��=3B
// P��=A/(27*B*B)

/*
#include "fre_ros.h"

struct FreeERoss:InterfaceFindSubstanceIO,FreeEIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}
   RossParam *par;

   FreeERoss(){par=NULL;}
   void DeleteVar(){delete par;par=NULL;}
   ~FreeERoss(){DeleteVar();}

   double FreeE(double Denc,double T){return FreeSSVT(par,Denc,T);};
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
   void ReadSubstancePar(fstream &in){ FreeERossRead(in,par);};

  }; 
double EVanDerVals(double Denc,double T) // With turn Cv=5/2
  {
   return ColdParam.Rconst*(T-ValsParam.A*Denc);
  };
double PVanDerVals(double Denc,double T) // With turn Cv=5/2
  {
   return ColdParam.Rconst*T/(1/Denc-ValsParam.B)-ValsParam.A*sqr(Denc);
  };
double FVanDerVals(double Denc,double T)
  {
//                    N*b    N*N*a
//  F=Fid - N*T*ln(1- ---) - -----
//                     V       V
   double A=ValsParam.A,B=ValsParam.B,dE=ValsParam.FreeDelta;
   double F=ColdParam.Rconst*(2.5*T-log(1-B*Denc)-A*Denc)-dE;
   return F;
  };
double FLiquid(double Denc,double T)
  {
   double Ecold=,Teta=,z=,b=,T0=,TetaTurn=;
   double F=Ecold+ColdParam.Rconst*
        (9/8*Teta+3*T*(log(Teta/T*sqrt(1+z))-b)+3*T0*f-T*log(T*0.5/TetaTurn));
   return F;
  };

double FSolid(double Denc,double T)
  {
   double Ecold=,Teta=,TetaTurn=,Deby=;
   double F=Ecold+ColdParam.Rconst*
        (9/8*Teta+T*(3*log(1-exp(Teta/T))-Deby-log(T*0.5/TetaTurn)));
   return F;
  };

int CurState(double r,double e)
  { return 2;};  // 1 - Gas, 2 - Liquid, 3 - Solid
*/


