#ifndef __FRE_ROS_H
#define __FRE_ROS_H 


#include "fre_fac.h"
#include "ros_equ.h"


struct FreeEPureRoss:FreeEIOFind {
	Ref<Exp6Param> par;
	double MolVeight,Zero,HiT;
	FreeEPureRoss():FreeEIOFind("material.cfg","FreeEPureRossH2")
		{par=new Exp6Param;}
   void DeleteVar(){}
   ~FreeEPureRoss(){DeleteVar();}
   double FreeE(double Denc,double T){
     double kt=M_Rconst/MolVeight*T;
     return Zero+kt*HiT+Ross6Config(*par,MolVeight,Denc,T);
   };
   void ReadSubstancePar(istream &in)
     { 
      char tmp[256];
	  in>>tmp>>MolVeight>>tmp>>Zero>>tmp>>HiT;
	  par = new Exp6Param();
      in>>tmp;Exp6Read(in,*par);
//cout<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT<<"\n";
     };
   void SaveSubstancePar(ostream &out) { 
	   out<<" MolVeight "<<MolVeight<<" Zero "<<Zero<<" HiT "<<HiT;
	   out<<" \nExp6Part   ";Exp6Save(out,*par);
     };
  }; 

struct FreeERossN2:FreeEIOFind {
   Exp6Param *par;
   FreeEIdeal *Fid1;
   FreeEIdeal *Fid2;
   double L_Denc,L_Temp,L_FE;
   int DoubleState,AddElectronN,AddElectronN2,DissRoss,MetalE;
   double DissA,DissV0,DissE;
   FreeERossN2();//{par=NULL;Fid1=NULL;Fid2=NULL;}
   void DeleteVar(){delete par;par=NULL;delete Fid1;Fid1=NULL;
                    delete Fid2;Fid2=NULL;}
   ~FreeERossN2(){DeleteVar();}
   double FreeE(double Denc,double T)
   {
//	   if ((fabs(Denc-L_Denc)<0.01*StndErr*Denc) && 
//		   (fabs(T-L_Temp)<0.01*StndErr*T)) return L_FE;
      double Ros=Ross6Config(*par,Fid1->MolVeight,Denc,T);
	  double Id1=Fid1->FreeE(Denc,T);
      double ret=Id1+Ros;
	  double AddN2E=0,AddNE=0;
	  if (AddElectronN2) AddN2E=RossElectronN2_E(T);

	  if (DoubleState) 
	  {
        double Id2=Fid2->FreeE(Denc,T);
		double E2=Ros+Id2;
//		if (DissRos)
		  {
    		double a=DissA,v0=DissV0,Eb=DissE,v=Fid1->MolVeight/Denc;
	    	if (v<=v0) Eb-=a*sqr(v0-v);
//cout<<" Eb "<<Eb<<" v "<<v;//<<"\n";
			Eb=Eb*M_eV_K*M_Rconst/Fid1->MolVeight;
		    E2+=Eb;
		  }
		if (AddElectronN) AddNE=RossElectronN_E(T,MetalE);
		ret=DissEnergy(E2+AddNE,ret+AddElectronN2*AddN2E,Fid1->MolVeight,T);
	  }
//	  L_Denc=Denc;L_Temp=T;L_FE=ret;
//cout<<" FreeE "<<ret<<" Denc "<<Denc<<" Temp "<<T<<"\n";
      return ret;
	  };
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
   void ReadSubstancePar(istream &in)
     { 
      char tmp[256];delete Fid1;delete Fid2;
	  Fid1=new FreeEIdeal;Fid2=new FreeEIdeal;
      in>>tmp;Fid1->ReadSubstancePar(in);
      in>>tmp;Exp6Read(in,par);
      in>>tmp;Fid2->ReadSubstancePar(in);
	  in>>tmp>>DoubleState>>tmp>>AddElectronN2>>tmp>>AddElectronN>>tmp>>MetalE
		  >>tmp>>DissRoss>>tmp>>DissE>>tmp>>DissV0>>tmp>>DissA;
	  L_Denc=-1;L_Temp=-1;L_FE=0;
     };
   void SaveSubstancePar(ostream &out) { 
	   out<<" IdealPart1 ";Fid1->SaveSubstancePar(out);
	   out<<" \nExp6Part   ";Exp6Save(out,*par);
	   out<<" \nIdealPart2 ";Fid2->SaveSubstancePar(out);
	   out<<" \nDoubleState "<<DoubleState <<" AddElectronN2 "<<AddElectronN2 
		   <<" AddElectronN "<<AddElectronN<<" MetalE "<<MetalE <<" DissRoss "<<DissRoss 
		   <<" DissE "<<DissE <<" DissV0 "<<DissV0 <<" DissA "<<DissA<<"\n";
     };
	virtual void SetInternalNames(){
		FreeEIOFind::SetInternalNames();
        OutputNames["DisDegree"]=NULL;
  	}
	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
        if (CheckPresence(OutputNames, Output) && Output==Stroka("DisDegree")){
            FreeE(CheckPresence(Input, "Dencity"),CheckPresence(Input, "Temperature"));
            return DisDegree;
        }
		return FreeEIOFind::OutputValClc(Input, Output);
	}
//#ifdef INTERNAL_FUNC
//   void Int_Par(double *Input,double *Output)
//   {FreeE(Input[1],Input[2]);Output[1]=DisDegree;};
//#endif

  }; 

struct DissEnergyStruct{
	DissEnergyStruct():DisE(4.735), DisV0(11.13055){Dis[0]=0; Dis[1]=0.013; Dis[2]=0.00732;}
	double Dis[3],DisV0,DisE,MolVeight;
	int ReadPar(istream &in,double MolVeight_){ 
		char tmp[256];in>>tmp>>DisE>>tmp>>DisV0>>tmp>>Dis[0]>>tmp>>Dis[1]>>tmp>>Dis[2];
		MolVeight=MolVeight_; return 1; 
	}
	void SavePar(ostream &out){ 
		out<<" DisE "<<DisE <<" DisV0 "<<DisV0 <<" Dis0 "<<Dis[0] <<" Dis1 "<<Dis[1] <<" Dis2 "<<Dis[2]<<"  ";
	}
	double EnergyShift(double Denc,double T){
   		double v0=DisV0,Eb=DisE,v=MolVeight/max(Denc,1e-10),dV=v-v0,dV2=sqr(dV),dV3=dV2*dV;
		if (v<=v0) Eb+=Dis[0]*dV+Dis[1]*dV2+Dis[2]*dV3;
  			Eb=Eb*M_eV_K*M_Rconst/MolVeight;
		return Eb;
	}
 };


struct FreeERossH2:FreeEIOFind {
	Exp6Param *par;
	FreeEIdeal *Fid1;
	FreeEIdeal *Fid2;
	DissEnergyStruct Dis;
	int DoubleState;
	FreeERossH2():FreeEIOFind("material.cfg","FreeERossH2DisGood"), DoubleState(1)
		{par=new Exp6Param;Fid1=new FreeEIdeal;Fid2=new FreeEIdeal;SetInternalNames();}
   void DeleteVar(){delete par;par=NULL;delete Fid1;Fid1=NULL;
                    delete Fid2;Fid2=NULL;}
   ~FreeERossH2(){DeleteVar();}
   double FreeE(double Denc,double T)
   {
     double Ros=Ross6Config(*par,Fid1->MolVeight,Denc,T);
	    double Id1=Fid1->FreeE(Denc,T);
     double ret=Id1+Ros;
     if (DoubleState) 
      {
       double Id2=Fid2->FreeE(Denc,T);
     		double E2=Ros+Id2;
       ret=DissEnergy(E2+Dis.EnergyShift(Denc,T),ret,Fid1->MolVeight,T);
      }
     return ret;
	};
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
   void ReadSubstancePar(istream &in)
     { 
      char tmp[256];delete Fid1;delete Fid2;
   	  Fid1=new FreeEIdeal;Fid2=new FreeEIdeal;
      in>>tmp;Fid1->ReadSubstancePar(in);
      in>>tmp;Exp6Read(in,par);
      in>>tmp;Fid2->ReadSubstancePar(in);
      in>>tmp>>DoubleState;
      Dis.ReadPar(in,Fid1->MolVeight);
     };
   void SaveSubstancePar(ostream &out) { 
	   out<<" IdealPart1 ";Fid1->SaveSubstancePar(out);
	   out<<" \nExp6Part   ";Exp6Save(out,*par);
	   out<<" \nIdealPart2 ";Fid2->SaveSubstancePar(out);
	   out<<" \nDoubleState "<<DoubleState <<"\n";
	   Dis.SavePar(out);
     };
	virtual void SetInternalNames(){
		FreeEIOFind::SetInternalNames();
        OutputNames["DisDegree"]=NULL;
  	}
	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
        if (CheckPresence(OutputNames, Output) && Output==Stroka("DisDegree")){
            FreeE(CheckPresence(Input, "Dencity"),CheckPresence(Input, "Temperature"));
            return DisDegree;
        }
		return FreeEIOFind::OutputValClc(Input, Output);
	}
//#ifdef INTERNAL_FUNC
//   void Int_Par(double *Input,double *Output)
//   {FreeE(Input[1],Input[2]);Output[1]=DisDegree;};
//#endif


  }; 
   
struct FreeERossH2_DisLim0:FreeEIOFind {
	Exp6Param *par;
	FreeEIdeal *Fid1;
	FreeEIdeal *Fid2;
	FreeEMetHCold *Fmet;
	DissEnergyStruct Dis;

	int DisLim0,DisPolynom,DisMet;
	double AddMet;
   	FreeERossH2_DisLim0():FreeEIOFind("material.cfg","FreeERossH2DisGoodMet"), 
		DisMet(0), DisPolynom(1), DisLim0(1), AddMet(1506)
		{par = new Exp6Param; Fid1 = new FreeEIdeal; Fid2 = new FreeEIdeal; Fmet = new FreeEMetHCold;SetInternalNames();}

   void DeleteVar(){delete par;par=NULL;delete Fid1;Fid1=NULL;
                    delete Fid2;Fid2=NULL;delete Fmet;Fmet=NULL;}
   ~FreeERossH2_DisLim0(){DeleteVar();}
   double FreeE(double Denc,double T)
   {
    double Ros=Ross6Config(*par,Fid1->MolVeight,Denc,T);
	   double Id1=Fid1->FreeE(Denc,T);
    double ret=Id1+Ros;

    if ( (DisPolynom) || (DisMet) )
     {
       double Id2=Fid2->FreeE(Denc,T);
     		double E2=Ros+Id2;
       double Eb=max(Dis.EnergyShift(Denc,T),0);
//    	  double v0=DissV0,Eb=DissE,v=Fid1->MolVeight/Denc,dV=v-v0;
//	      if (v<=v0) Eb+=Diss1*dV+Diss2*sqr(dV)+Diss3*dV*dV*dV;
//       Eb=max(Eb,0);
       double DisCoef=Eb/Dis.EnergyShift(0,T);
//cout<<" Eb "<<Eb<<" v "<<v<<"\n";
//    		Eb=Eb*M_eV_K*M_Rconst/Fid1->MolVeight;
      if (DisPolynom)		E2+=Eb;
      else E2=Fmet->FreeE(Denc,T)+Ros+AddMet;
//      double DisCoef=max(0,E2-ret)/Dis.EnergyShift(0,T);
    		double DiE=DissEnergy(E2,ret,Fid1->MolVeight,T);
//cout<<Denc<<" "<<DisCoef<<" "<<" "<<Eb<<" "<<E2<<"  "<<ret<<" "<<DiE<<"\n";
      if (DisLim0) 
          ret=DiE*DisCoef+(1-DisCoef)*ret;
//          ret=DiE*DisCoef+(1-DisCoef)*E2;
      else ret=DiE;
	    }

    return ret;
	  };
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
void OutputCompare(int N);
   void ReadSubstancePar(istream &in)
     { 
      char tmp[256];DeleteVar();
	  Fid1=new FreeEIdeal;Fid2=new FreeEIdeal;Fmet=new FreeEMetHCold;
      in>>tmp;Fid1->ReadSubstancePar(in);
      in>>tmp;Exp6Read(in,par);
      in>>tmp;Fid2->ReadSubstancePar(in);
      in>>tmp;Fmet->ReadSubstancePar(in);
      in>>tmp>>DisMet>>tmp>>DisPolynom>>tmp>>DisLim0>>tmp>>AddMet;
      Dis.ReadPar(in,Fid1->MolVeight);
     };
   void SaveSubstancePar(ostream &out) { 
	   out<<" IdealPart1 ";Fid1->SaveSubstancePar(out);
	   out<<" \nExp6Part   ";Exp6Save(out,*par);
	   out<<" \nIdealPart2 ";Fid2->SaveSubstancePar(out);
	   out<<" \nMetalPart  ";Fmet->SaveSubstancePar(out);
	   out<<" \nDisMet "<<DisMet <<" DisPolynom "<<DisPolynom<<" DisLim0 "<<DisLim0<<" AddMet "<<AddMet<<"\n";
       Dis.SavePar(out);
     };
	virtual void SetInternalNames(){
		FreeEIOFind::SetInternalNames();
        OutputNames["DisDegree"]=NULL;
  	}
	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
        if (CheckPresence(OutputNames, Output) && Output==Stroka("DisDegree")){
            FreeE(CheckPresence(Input, "Dencity"),CheckPresence(Input, "Temperature"));
            return DisDegree;
        }
		return FreeEIOFind::OutputValClc(Input, Output);
	}
//#ifdef INTERNAL_FUNC
//   void Int_Par(double *Input,double *Output)
//   {FreeE(Input[1],Input[2]);Output[1]=DisDegree;};
//#endif


  }; 
struct FreeERossH2_Minim:FreeERossH2_DisLim0{
   	FreeERossH2_Minim(): FreeERossH2_DisLim0(),eos(MET){}
    enum EOSTYPE{MET,ROS1,ROS2,ROSID1,ROSID2,METID2,LIQWEAK};
    EOSTYPE eos;
    double FreeE(double Denc,double T){
        if (eos==LIQWEAK){
            return FreeERossH2_DisLim0::FreeE(Denc,T);
        }
        if (eos==MET){
            return Fmet->FreeE(Denc,T);
        }
        if (eos==METID2){
            return Fmet->FreeE(Denc,T)+Fid2->FreeE(Denc,T);
        }
        if (eos==ROS1){
            return Ross6Config(*par,Fid1->MolVeight,Denc,T);
        }
        if (eos==ROS2){
            return Ross6Config(*par,Fid2->MolVeight,Denc,T);
        }
        if (eos==ROSID1){
            return Ross6Config(*par,Fid1->MolVeight,Denc,T)+Fid1->FreeE(Denc,T);
        }
        if (eos==ROSID2){
            return Ross6Config(*par,Fid2->MolVeight,Denc,T)+Fid2->FreeE(Denc,T);
        }
        return 0;
    }

};

struct FreeERossXe:FreeEIOFind {
	Exp6Param *par;
	FreeEIdeal *Fid1;
	int Ionize,Boltsman;
	double DissA,DissV0,DissE,DegenerDo,DegenerUp;
   	FreeERossXe():FreeEIOFind("material.cfg","XeStr1"), Ionize(1), Boltsman(0), 
		DegenerDo(3), DegenerUp(5), DissE(8.26415), DissV0(27.9302), DissA(0.0188)
		{par = new Exp6Param; Fid1 = new FreeEIdeal; }
   void DeleteVar(){delete par;par=NULL;delete Fid1;Fid1=NULL;}
   ~FreeERossXe(){DeleteVar();}
   double FreeE(double Denc,double T)
   {
    double Ros=Ross6Config(*par,Fid1->MolVeight,Denc,T);
	   double Id1=Fid1->FreeE(Denc,T);
    double ret=Id1+Ros;
	   if (Ionize) 
    {
		   double E2=ret;
   		double a=DissA,v0=DissV0,Eb=DissE,v=Fid1->MolVeight/Denc;
    	if (v<=v0) Eb-=a*sqr(v0-v);
		   Eb=max(0,Eb);
//		double Eb=DissE*log(13.13/Denc);
  		 if (Boltsman)
     {
//cout<<" Eb "<<Eb<<" v "<<v;//<<"\n";
		    Eb=Eb*M_eV_K*M_Rconst/Fid1->MolVeight;
	     E2+=Eb;
		    ret=IonizeEnergy(E2,ret,Fid1->MolVeight,T,DegenerUp*DegenerDo,1/Denc);
     }
// It seems after upgrade of integrals, sign changed?
		   else ret+=FreeESemiConductor(Eb,DegenerDo,DegenerUp,Fid1->MolVeight,Denc,T);
//		   else ret-=FreeESemiConductor(Eb,DegenerDo,DegenerUp,Fid1->MolVeight,Denc,T);
	   }
//cout<<" FreeE "<<ret<<" Denc "<<Denc<<" T "<<T<<"\n";

      return ret;
   };
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
   void ReadSubstancePar(istream &in)
     { 
      char tmp[256];//delete Fid1;Fid1=new FreeEIdeal;
      in>>tmp;Fid1->ReadSubstancePar(in);
      in>>tmp;Exp6Read(in,par);
	  in>>tmp>>Ionize>>tmp>>Boltsman>>tmp>>DegenerDo>>tmp>>DegenerUp>>tmp>>DissE>>tmp>>DissV0>>tmp>>DissA;
     };
   void SaveSubstancePar(ostream &out) { 
	   out<<" IdealPart1 ";Fid1->SaveSubstancePar(out);
	   out<<" \nExp6Part   ";Exp6Save(out,*par);
	   out<<" \nIonize "<<Ionize<<" Boltsman "<<Boltsman <<" DegenerDo "<<DegenerDo <<" DegenerUp "<<DegenerUp
		   <<" DissE "<<DissE <<" DissV0 "<<DissV0 <<" DissA "<<DissA<<"\n";
     };

  }; 
  
#include "ros_bin.h" 

// Binary mixtures
struct FreeERossBin:FreeEIOFind {
	MixtureComposition par;
	FreeEIdeal Fid[3];
	int DisIsPossible,DisIsConst,CorBin;
	DissEnergyStruct Dis;
   	FreeERossBin():FreeEIOFind("material.cfg","FreeERossBinH2HeJup"), 
	   DisIsPossible(1),DisIsConst(0),CorBin(1){SetInternalNames(); }

   double FreeE(double Denc,double T){


    double al=par.get_Alpha(),miu_=par.get_MeanMolVeight();
    double miu1,miu2;par.get_MolVeight(miu1,miu2);


    double Ros=0;
    if (CorBin==1)      Ros=FreeBinaryCor(par,Denc,T);
    else if (CorBin==0) Ros=Ross6Config(par.Exp6[0],miu1,Denc*miu1/miu_,T);

	   double Id1=al*Fid[0].FreeE(Denc*al*miu1/miu_,T);
	   double Id2=(1-al)*Fid[1].FreeE(Denc*(1-al)*miu2/miu_,T);
    double ret=Id1+Id2 +Ros;
//cout<<" FreeE "<<ret<<" Denc "<<Denc<<" T "<<T<<"\n";
     if (DisIsPossible) 
      {
//       double miu3=Fid[2].MolVeight;
   	   double Id3=Fid[2].FreeE(Denc*al*miu1/miu_,T);
       Id1/=al;
       double Shift=max(Dis.EnergyShift(Denc*miu1/miu_,T),0),DisCoef=Shift/Dis.EnergyShift(0,T);
       if (DisIsConst) DisCoef=1;
     		double DisE=DissEnergy(Id3+Shift,Id1,miu1,T);
       DisE=DisE*DisCoef+(1-DisCoef)*Id1;
       ret=Id2+Ros+DisE*al;
      }

      return ret;
   };
   double DencityCold() {return StndErr;};
   double DencityMax()  {return 1e2;};
   void ReadSubstancePar(istream &in){ 
      char tmp[256];
      in>>tmp;Fid[0].ReadSubstancePar(in);
      in>>tmp;Fid[1].ReadSubstancePar(in);
      in>>tmp;par.ReadPar(in);
      in>>tmp>>DisIsPossible;
      DisIsConst=0;
      CorBin=1;
      if (!seek_eoln(in)) {
        in>>tmp>>DisIsConst;
        if (!seek_eoln(in)) in>>tmp>>CorBin;
       }
      if (DisIsPossible){
        in>>tmp;Fid[2].ReadSubstancePar(in);
        Dis.ReadPar(in,Fid[0].MolVeight);
       }
   }
   void SaveSubstancePar(ostream &out) { 
	   out<<" IdealPart1 ";Fid[0].SaveSubstancePar(out);
	   out<<" \nIdealPart2 ";Fid[1].SaveSubstancePar(out);
	   out<<" \nMistureComposition ";par.SavePar(out);
	   out<<" \nDisIsPossible "<<DisIsPossible<<" DisIsConst "<<DisIsConst<<" CorBin "<<CorBin;
	   out<<" \nIdealPart3 ";Fid[2].SaveSubstancePar(out);
       out<<"\n";Dis.SavePar(out);out<<"\n";
     };
	virtual void SetInternalNames(){
		FreeEIOFind::SetInternalNames();
        OutputNames["DisDegree"]=NULL;
  	}
	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
        if (CheckPresence(OutputNames, Output) && Output==Stroka("DisDegree")){
            FreeE(CheckPresence(Input, "Dencity"),CheckPresence(Input, "Temperature"));
            return DisDegree;
        }
		return FreeEIOFind::OutputValClc(Input, Output);
	}
//#ifdef INTERNAL_FUNC
//   void Int_Par(double *Input,double *Output)
//   {FreeE(Input[1],Input[2]);Output[1]=DisDegree;};
//#endif

  }; 
   

#endif
