#ifndef __MATT_FAC_H
#define __MATT_FAC_H

#include "matt_cfg.h"
#include "lib/std/deffunc.h"
#include "lib/std/stdexception.h"
#include "mat/dat_fac.h"
#include "lib/ref/class_sav.h"

#include "matt_tools.h"

#include "lib/Data/VecRes.h"


struct FreeFuncClc:SavableClass{
	void GetInternalNames(map<Stroka, void *> &Input, map<Stroka, void *> &Output){
		Input = InputNames; Output = OutputNames;
	}

    FreeFuncClc(){};//SetInternalNames();};

    virtual void SetInternalNames()=0;
//        {throw info_except("OOPS!!!\n");};//{InputNames.clear();OutputNames.clear();};

	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
		throw info_except(" Output %s - is unknown\n", Output.c_str());
		return 0;
	}
	virtual double OutputValClc(const map<Stroka,double*> &Input, const Stroka &Output){
		throw info_except(" Output %s - is unknown\n", Output.c_str());
		return 0;
	}
    virtual Stroka GetOutputNames(){
        Stroka res;
		for (map<Stroka, void*>::iterator iter = OutputNames.begin();iter!=OutputNames.end();iter++) 
		    res += iter->first+Stroka(" ");
        return res;
    }
    virtual Stroka GetInputNames(){
        Stroka res;
		for (map<Stroka, void*>::iterator iter = InputNames.begin();iter!=InputNames.end();iter++) 
		    res += iter->first+Stroka(" ");
        return res;
    }
    virtual map<Stroka, void *> GetInputNamesMap()     {return  InputNames;}
    virtual map<Stroka, void *> GetOutputNamesMap()    {return  OutputNames;}

	virtual void StoreInputVal(const map<Stroka, double> &Input){
		InputValStorage = Input;
	}
	virtual void StoreInputVal(const map<Stroka, double*> &Input){
		InputVectorStorage = Input;
	}

// 2 Std Input par - Dencity Energy(Temperature)
    virtual int NumInputVal(){return (int)InputNames.size();};
	virtual void ChangeInputVal(double Dencity,double Energy,double Time){};
	virtual void ChangeInputVal(double *Dencity,double *Energy,int Num,
                                                                   double Time){};


	//virtual string print_help(){
	//	FilterTextOut tmp("MatterIO::print_help",DataSource::Memory);
	//	tmp.SetRefMask(SavableClass::UnlinkedObjects);
	//	MakeHelp(tmp);
	//	return string(DataSource::GetStr("MatterIO::print_help").c_str());
	//}
// Not to save data...
  //  virtual void MakeHelp(FilterTextOut&out){
		//out<<"-----------   "<<this->class_name()<<"   --------------------------\n";
		//out<<"Create:\n\t";
		//SavableClass::SavableClassStore(out,this);
  //      out<<"\nOutputClcNames: "<<GetOutputNames().c_str();
  //      out<<"\nInputClcNames: "<<GetInputNames().c_str();
		//out<<"\n----------------------------------------------\n";
  //  }
    virtual Stroka MakeHelp(){
        Stroka res = Stroka("OutputClcNames: ") + GetOutputNames() + "\n";
        res += Stroka("InputClcNames: ") + GetInputNames() + "\n";
        return res;
    }

    //virtual Stroka save_data_state( ){
    //    FilterTextOut out("FreeFuncClc::save_data_state::aa",DataSource::Memory);
    //    //SavableClass::save_data_state(out);
    //    ((SavableClass*)this)->save_data_state(out);
    //    Stroka ret = DataSource::GetStr("FreeFuncClc::save_data_state::aa").c_str();
    //    return ret;
    //}

  protected:
	map<Stroka, double> InputValStorage;
	map<Stroka, double*> InputVectorStorage;
	map<Stroka, void*> InputNames, OutputNames;

	double CheckPresence(const map<Stroka,double> &dat, const Stroka &name){
		map<Stroka,double>::const_iterator it = dat.find(name);
		if (it==dat.end())
			throw info_except("Name %s was not found...\n",name.c_str());
		return it->second;
	}
	double* CheckPresence(const map<Stroka,void*> &dat, const Stroka &name){
		map<Stroka,void*>::const_iterator it = dat.find(name);
		if (it==dat.end())
			throw info_except("Name %s was not found...\n",name.c_str());
		return (double*)it->second;
	}
	double* CheckPresence(const map<Stroka,double*> &dat, const Stroka &name){
		map<Stroka,double*>::const_iterator it = dat.find(name);
		if (it==dat.end())
			throw info_except("Name %s was not found...\n",name.c_str());
		return it->second;
	}
	int CheckPresence(const map<Stroka,double> &dat, const Stroka &name, double &val){
		map<Stroka,double>::const_iterator it = dat.find(name);
		if (it==dat.end())
			return 0;
		val = it->second;
		return 1;
	}
	int CheckPresence(const map<Stroka,double*> &dat, const Stroka &name, double *&val){
		map<Stroka,double*>::const_iterator it = dat.find(name);
		if (it==dat.end())
			return 0;
		val = it->second;
		return 1;
	}
	int CheckPresence(const map<Stroka,void*> &dat, const Stroka &name, void *&val){
		map<Stroka, void*>::const_iterator it = dat.find(name);
		if (it==dat.end())
			return 0;
		val = it->second;
		return 1;
	}
};
struct MatterIO : FreeFuncClc {
    virtual void Pressure(double *P,double *Dencity,double *Energy,int Num);
    virtual void Sound(double *S,double *Dencity,double *Energy,int Num);
    virtual void Temperature(double *T,double *Dencity,double *Energy,int Num);
    virtual double Pressure(double Dencity,double Energy)=0;
    virtual double Sound(double Dencity,double Energy);
    virtual double Temperature(double Dencity,double Energy){return 0;};
    virtual double DencityCold(){return 1e-10;}
    virtual double DencityMax(){return 100;}
    virtual double Energy(double Dencity,double Temperature){return 0;};

    MatterIO()
        : FreeFuncClc()
        , SoundDencityCoef(1)  {
            ((MatterIO*)this)->SetInternalNames();
    };
	virtual void SetInternalNames(){
        SetInternalNames(InputNames);
        //InputNames.clear();InputNames["Dencity"] = NULL;InputNames["Energy"] = NULL;
        OutputNames.clear();
		OutputNames["Pressure"] = NULL;OutputNames["Temperature"] = NULL;
		OutputNames["Sound"] = NULL;
	}
	void SetInternalNames(map<Stroka,void*> &Input){
        Input.clear();Input["Dencity"] = NULL;Input["Energy"] = NULL;
	}
    //vector<Stroka> GetAddedNames
	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
		StoreInputVal(Input);
		ChangeInputVal( CheckPresence(Input, "Dencity"), CheckPresence(Input, "Energy"), 0 );
		if (Output=="Pressure")
			return Pressure(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Energy") );
		if (Output=="Temperature")
			return Temperature(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Energy") );
		if (Output=="Sound")
			return Sound(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Energy") );
		if (Output=="Energy")
			return Energy(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature") );
        Stroka OutNames = Stroka("\nFor Class: ")+((SavableClass*)this)->class_name();
        OutNames += Stroka("\nName_ToClc: ") + Stroka(Output) + Stroka(" is unknown.\n NamesPresent: ") + GetOutputNames();
		throw info_except("%s",OutNames.c_str());
		return 0;
	}
	using FreeFuncClc::OutputValClc;
	virtual double OutputValClc(double Dencity,double Energy, const Stroka &Output){
		if (Output=="Pressure")
			return Pressure(Dencity,Energy);
		if (Output=="Temperaure")
			return Temperature(Dencity,Energy);
		if (Output=="Sound")
			return Sound(Dencity,Energy);
		map<Stroka,double> par;par["Dencity"] = Dencity;par["Energy"] = Energy;
		return OutputValClc(par, Output);
	}
    virtual void OutputValClc(const map<Stroka, double* > &Input, const Stroka &Output, double *OutDat,int Num){
		StoreInputVal(Input);
		if (Output=="Pressure")
			return Pressure(OutDat, CheckPresence(Input, "Dencity"), CheckPresence(Input, "Energy"), Num );
		if (Output=="Temperaure")
			return Temperature(OutDat, CheckPresence(Input, "Dencity"), CheckPresence(Input, "Energy"), Num );
		if (Output=="Sound")
			return Sound(OutDat, CheckPresence(Input, "Dencity"), CheckPresence(Input, "Energy"), Num );
        Stroka OutNames = Stroka("\nName_ToClc: ") + Stroka(Output) + Stroka(" is unknown.\n NamesPresent: ") + GetOutputNames();
        OutNames +=Stroka("\nSubstanceParams: ")+SavableClass::object2string(this);
		throw info_except("%s",OutNames.c_str());
	}
	virtual void ChangeInputVal(double Dencity,double Energy,double Time){};
	virtual void ChangeInputVal(double *Dencity,double *Energy,int Num,
			                                                        double Time);
	//virtual Stroka MakeHelp(){
 //       Stroka res = Stroka("OutputClcNames: ") + GetOutputNames() + "\n";
 //       res += Stroka("nInputClcNames: ") + GetInputNames() + "\n";
 //       return res;
	//}
  protected:
    double SoundDencityCoef;

  };


struct InterfaceFindSubstanceIO:virtual SavableClass{
   Stroka FileName,Substance;
   InterfaceFindSubstanceIO(){}
   void SetInputFiles(const char *file, const char *subst){
       FileName = file;Substance = subst;
   }

   virtual int save_data_state( FilterOut&output){
      output<<" Material_File "<<FileName<<" Substance "<<Substance<<" ";return 1;
   };
   virtual int read_data_state(FilterIn&input){
      char tmp[256];
      input>>tmp>>FileName>>tmp>>Substance;
      if (!GetMatter()){
            cout<<" Could not find match for matter "<<Substance<<" in file "
                <<FileName<<" Zero par set\n";
			throw info_except("Bad Substance-Matter;\n Substance:%s FileName:%s\n",Substance.c_str(),FileName.c_str());
        }
      return 1;
     };
   virtual void ReadSubstancePar(istream &in)=0;
   virtual void SaveSubstancePar(ostream &out){out<<"Not implemented save params func\n";};
   //Stroka SaveSubstancePar(){
   //     FilterTextOut out("InterfaceFindSubstanceIO::SaveSubstancePar::aa",DataSource::Memory);
   //     SaveSubstancePar(out);
   //     return Stroka(DataSource::GetStr("InterfaceFindSubstanceIO::SaveSubstancePar::aa").c_str());
   //};
   int GetMatter();
 //  	virtual string print_help_find(){
	//	FilterTextOut tmp("InterfaceFindSubstanceIO::print_help",DataSource::Memory);
	//	tmp.SetRefMask(SavableClass::UnlinkedObjects);
	//	MakeInterfaceFindSubstanceIOHelp(tmp);
	//	return string(DataSource::GetStr("InterfaceFindSubstanceIO::print_help").c_str());
	//}
 //   void MakeInterfaceFindSubstanceIOHelp(FilterTextOut &out){
 //       out<<"Class reference can be read-saved by :\n";
 //       SavableClass::SavableClassStore(out,this);
 //       out<<"\nClass parameters to be defined are:\n";
 //       SaveSubstancePar(out);out<<"\n";
 //   }

};
//==================================================================
//================  Matters Definitions ============================
//==================================================================

//
//  units for pressure : (gram*km^2)/(cm^3*c^2)  GPa
//  units for energy   : (km/c)^2
//  units for dencity  :  gram/cm^3
//  units for velocity :  km/c   
//
struct MatterIOFind:MatterIO{
    FindSubstanceIO Finder;
	MatterIOFind(const char *file,const char *subst):MatterIO(),Finder(file,subst){};
	virtual void ReadSubstancePar(istream &in)=0;
	virtual void SaveSubstancePar(ostream &out)=0;
    Stroka SaveSubstancePar(){
        FilterTextOut out("MatterIOFind::SaveSubstancePar::aa",DataSource::Memory);
        SaveSubstancePar(out);
        return Stroka(DataSource::GetStr("MatterIOFind::SaveSubstancePar::aa").c_str());
    }
	virtual int read_data_state(FilterIn &input){
		input>>Finder;
		//ReadSubstancePar(*(Finder.GetReadStream()));
		istream *in = dynamic_cast<istream*>(Finder.GetReadStream());
		ReadSubstancePar(*in);
		if ( !(*in) )
			throw info_except("Could not read in file <%s>  matter <%s> For class <%s>\n",Finder.FileName.c_str(),Finder.Substance.c_str(), class_name());
		return 1;
	}
	virtual int save_data_state( FilterOut &output){
		output<<Finder;
		return 1;
	}
	//virtual void MakeHelp(FilterTextOut&out){
	//	out<<"-----------   "<<this->class_name()<<"   --------------------------\n";
	//	out<<"Create:\n\t";
	//	SavableClass::SavableClassStore(out,this);
 //       out<<"\nOutputClcNames: "<<GetOutputNames().c_str();
 //       out<<"\nInputClcNames: "<<GetInputNames().c_str();
	//	out<<"\nConfiguration parameters are:\n\t";
	//	SaveSubstancePar(out);out<<"\n----------------------------------------------\n";
	//}
	virtual Stroka MakeHelp(){
        Stroka res = MatterIO::MakeHelp();
        res += Stroka("Configuration parameters are:\n\t") + SaveSubstancePar();
        return res;
	}
};
struct MatterABu:MatterIOFind {
   //static const char * const name;
   //virtual const char *InterfaceIOName()  {return name;}
   MatterABu():MatterIOFind("material.cfg","Steel-Tern"),Denc(7.89),A(4.553),B(1.482),Grun(1.69){}
   double Denc,A,B,Grun;

   double Pressure(double Dencity,double Energy);
   double Sound(double Dencity,double Energy);
   double DencityCold()                       {return Denc;};
   double DencityMax()                        {return Denc*B/(fabs(B-1)+StndErr);};
   double Temperature(double Dencity,double Energy);
   void Pressure(double *P,double *Dencity,double *Energy,int Num);
   void Sound(double *S,double *Dencity,double *Energy,int Num);
	void ReadSubstancePar(istream &in)
		{char tmp[256];in>>tmp>>tmp>>Denc>>tmp>>A>>tmp>>B>>tmp>>Grun;};
	void SaveSubstancePar(ostream &out)
		{out<<Finder.Substance<<" \tDenc "<<Denc<<" A "<<A<<" B "<<B<<" Grun "<<Grun<<" ";};
  };

struct MatterGas:MatterIOFind {

	double Gamma;
	MatterGas():MatterIOFind("material.cfg","GasOneAtom"), Gamma(1.5) {};
	double Pressure(double Dencity,double Energy) 
		{return (Gamma-1)*Dencity*Energy;}
   double DencityCold()         {return MathZer*10;};
   double DencityMax()          {return 1/MathZer;};
   double Temperature(double Dencity,double Energy){return 0;};
//			double Sound(double Dencity,double Energy) {return sqrt(Gamma*(Gamma-1)*Energy);}
	void ReadSubstancePar(istream &in)  {char tmp[256];in>>tmp>>Gamma;};
	void SaveSubstancePar(ostream &out)  {out<<" Gamma "<<Gamma<<" ";};
  };

// ? I think Ecold - hear is wrong. Y=pow(r/r0,1/3) - so Pcold - wrong
// Goto p.315 Kanel'. Udarno-Volnovie iavlenia v kondensirovannih sredah.
struct MatterBornMaier:MatterIOFind {

	MatterBornMaier():MatterIOFind("material.cfg","not_implemented_yet"), B0(0),B1(0),B2(0),Denc(0),Grun(0){};
	double B0,B1,B2,Denc,Grun,k_const,zero_const;
	double Pressure(double Dencity,double Energy) 
		{double y=Dencity/Denc;
				double Pc= B0*y*y*exp(B1*(1-y))-B2*sqr(sqr(y));
				double Ec=B0*Denc/(B1*(k_const-y))*exp(B1*(k_const-y))+
						    B2*Denc/3*y*sqr(y)-zero_const;
				return Pc+Grun*Dencity*(Energy-Ec);
			}
   double DencityCold()         {return Denc;};
   double DencityMax()          {return 100;};
   double Temperature(double Dencity,double Energy){return 0;};
//			double Sound(double Dencity,double Energy) {return sqrt(Gamma*(Gamma-1)*Energy);}
	void ReadSubstancePar(istream &in){
        char tmp[256];in>>tmp>>Denc>>tmp>>B0>>tmp>>B1>>tmp>>B2>>tmp>>Grun;
		k_const=1-log(B2/B0)/B1;
		zero_const=B2*Denc*(1/log(B0/B2)+1/3);
    }
	void SaveSubstancePar(ostream &out)  {
		out<<" Denc "<<Denc<<" B0 "<<B0<<" B1 "<<B1<<" B2 "<<B2<<" Grun "<<Grun<<" ";
	};
  };

struct MatterMurnaganBerch:MatterIOFind {
	MatterMurnaganBerch():MatterIOFind("material.cfg","MurnaganGeks"), B0(13.2),B1(6.4),Denc(1.72),Grun(2.4){};
	double B0,B1,Denc,Grun,k_9,k_7,k_5;
	double Pressure(double Dencity,double Energy){
        double y=pow(Dencity/Denc,1./3),y2=sqr(y),y4=sqr(y2);
		double Pc= (k_5+k_7*y2+k_9*y4)*y4*y;
		double Ec=3/pow(Denc,5)*(k_5/2*(y2-1)+k_7/4*(y4-1)+k_9/6*(y4*y2-1));
		return Pc+Grun*Dencity*(Energy-Ec);
	}
   double DencityCold()         {return Denc;};
   double DencityMax()          {return 100;};
   double Temperature(double Dencity,double Energy){return 0;};
	void ReadSubstancePar(istream &in){
        char tmp[256];in>>tmp>>Denc>>tmp>>B0>>tmp>>B1>>tmp>>Grun;
		k_5=-3./2*B0*(1+3./4*(4-B1));
		k_9=-3./2*B0*3./4*(4-B1);
		k_7=-k_5-k_9;
	}
	void SaveSubstancePar(ostream &out)  {
		out<<" Denc "<<Denc<<" B0 "<<B0<<" B1 "<<B1<<" Grun "<<Grun<<" ";
	};
  };
// Formulas - from Utkin.
struct MatterZubarev:MatterIOFind{
	MatterZubarev():MatterIOFind("material.cfg","ZubarevGeks"), Denc(1.6),k(2),Bc(0.6887743),n(2.83),D(8.05),Grun(0.3){};
	double Denc,k,Bc,n,D,Grun;
	double Q,Ph,DencH,ac,Ac;
	double Pressure(double Dencity,double Energy) {
        double y=Dencity/DencH;
		double Pv=Ph*(Ac*exp(-ac/y)*(1-Grun*y/ac)+Bc*pow(y,k)*(1-Grun/(k-1)));
		return Pv+Grun*Dencity*(Energy+Q);
	}
    double DencityCold()         {return Denc;};
    double DencityMax()          {return 100;};
    double Temperature(double Dencity,double Energy){return 0;};
	void ReadSubstancePar(istream &in){
        char tmp[256];in>>tmp>>Denc>>tmp>>k>>tmp>>Bc>>tmp>>n>>tmp>>D>>tmp>>Grun;
		DencH=Denc*(n+1)/n;
		Ph=Denc*sqr(D)/(n+1);
		ac=(n-k*Bc)/(1-Bc);
		Ac=(1-Bc)*exp(ac);
		Q=Ph/DencH*((1-Bc)/ac+Bc/(k-1)-1/(2*n));
	}
	void SaveSubstancePar(ostream &out)  {
		out<<" Denc "<<Denc<<" k "<<k<<" Bc "<<Bc<<" n "<<n<<" D "<<D<<" Grun "<<Grun<<" ";
	};
  };



struct MatterVacuum:MatterIO
  {
   //static const char * const name;
   //virtual const char *InterfaceIOName()  {return name;}

   double Pressure(double Dencity,double Energy){return 0;};
   double Sound(double Dencity,double Energy){return 0;};
   double DencityCold()                       {return 0;};
   double DencityMax()                        {return 0;};
   void Pressure(double *P,double *Dencity,double *Energy,int Num);
	//virtual void MakeHelp(FilterTextOut&out){
	//	out<<"-----------   "<<this->class_name()<<"   --------------------------\n";
	//	out<<"Create:\n\t";
	//	SavableClass::SavableClassStore(out,this);
	//	out<<"\n----------------------------------------------\n";
	//}
  };




//#endif // MatterBornMaier_Register
//#ifdef MatterZubarev_Register
/*
struct MatterZubarev:virtual InterfaceFindSubstanceIO,virtual MatterIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}

			double A,a,B,n,Denc,Grun,zero_const;
//			MatterGas(){};
			double Pressure(double Dencity,double Energy) 
			  {double v=1/Dencity;
						double Ps= A*exp(-a*v)+B*pow(v,-n);
						double Es=B/(1-n)*pow(v,1-n)-A/a*exp(-a*v)-zero_const;
						return Ps+Grun*Dencity*(Energy-Es);
					}
   double DencityCold()         {return Denc;};
   double DencityMax()          {return 100;};
   double Temperature(double Dencity,double Energy){return 0;};
//			double Sound(double Dencity,double Energy) {return sqrt(Gamma*(Gamma-1)*Energy);}
			void ReadSubstancePar(fstream &in)
			  {char tmp[256];in>>tmp>>Denc>>tmp>>A>>tmp>>a>>tmp>>B>>tmp>>n>>tmp>>Grun;
						zero_const=B/(1-n)*pow(1/Denc,1-n)-A/a*exp(-a/Denc);
					}
  };
*/
//#endif // MatterZubarev_Register
////#ifdef MatterZubarev_Register

#endif

