#include <lib/precompiled/eos.h>
#include "matt_fac.h"

static ClassDesc MatterABu_ClassDesc(typeid(MatterABu),"MatterABu",1,"Matter_category",create<MatterABu>);
static ClassDesc MatterGas_ClassDesc(typeid(MatterGas),"MatterGas",1,"Matter_category",create<MatterGas>);
static ClassDesc MatterBornMaier_ClassDesc(typeid(MatterBornMaier),"MatterBornMaier",1,"Matter_category",create<MatterBornMaier>);
static ClassDesc MatterMurnaganBerch_ClassDesc(typeid(MatterMurnaganBerch),"MatterMurnaganBerch",1,"Matter_category",create<MatterMurnaganBerch>);
static ClassDesc MatterZubarev_ClassDesc(typeid(MatterZubarev),"MatterZubarev",1,"Matter_category",create<MatterZubarev>);
static ClassDesc MatterVacuum_ClassDesc(typeid(MatterVacuum),"MatterVacuum",1,"Matter_category",create<MatterVacuum>);

double MatterIO::Sound(double Dencity,double Energy) {
    double dr = Dencity * StndErr * SoundDencityCoef, r = Dencity;
   if (r<MathZer) { cout << "MatterIO::Sound; Dencity<MathZer :" << Dencity << "\n";return 0;}
   double P=Pressure(r,Energy);
   double Pp = Pressure(r+dr,Energy+P*dr/(r*r));Pp -= P;
   double S = sqrt( max<double>(0.1, Pp)/dr );
   return S;
  };

//#ifndef InternalPointsUrs
//
//void   MatterIO::Pressure   (double *P,double *Dencity,double *Energy,int Num)
//  {		for (int k=1;k<=Num;k++) { P[k]=Pressure(Dencity[k],Energy[k]);}}
//void   MatterIO::Sound      (double *S,double *Dencity,double *Energy,int Num)
//  {		for (int k=1;k<=Num;k++) { S[k]=Sound(Dencity[k],Energy[k]);}}
//void   MatterIO::Temperature(double *T,double *Dencity,double *Energy,int Num)
//  {		for (int k=1;k<=Num;k++) { T[k]=Temperature(Dencity[k],Energy[k]);}}
//
//#else

void   MatterIO::Pressure(double *P,double *Dencity,double *Energy,int Num){
	int k;
	if (NumInputVal()==2){
		for ( k=1;k<=Num;k++) { 
		    P[k]=Pressure(Dencity[k],Energy[k]);
//cout<<P[k]<<" ";       		
		}
//cout<<"\n";		
    } else {
        InputVectorStorage.erase(Stroka("Dencity"));InputVectorStorage.erase(Stroka("Energy"));
        for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
            if (!it->second)
                throw info_except("Have not defined vals for %s\n",it->first.c_str());
		for ( k=1;k<=Num;k++){
            for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
                InputValStorage[it->first] = it->second[k];
       		P[k]=Pressure(Dencity[k],Energy[k]);
//cout<<P[k]<<" ";       		
		}
//cout<<"\n";		
        InputVectorStorage[Stroka("Dencity")] = Dencity;
        InputVectorStorage[Stroka("Energy")] = Energy;
	}
};
void   MatterIO::Sound(double *S,double *Dencity,double *Energy,int Num){
	int k;
	if (NumInputVal()==2) {
		for ( k=1;k<=Num;k++) { 
		    S[k]=Sound(Dencity[k],Energy[k]);
//cout<<S[k]<<" ";       		
		}
//cout<<"\n";		
	} else {
        InputVectorStorage.erase(Stroka("Dencity"));InputVectorStorage.erase(Stroka("Energy"));
        for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
            if (!it->second)
                throw info_except("Have not defined vals for %s\n",it->first.c_str());
		for ( k=1;k<=Num;k++){
            for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
                InputValStorage[it->first] = it->second[k];
       		S[k]=Sound(Dencity[k],Energy[k]);
//cout<<S[k]<<" ";       		
		}
//cout<<"\n";		
        InputVectorStorage[Stroka("Dencity")] = Dencity;
        InputVectorStorage[Stroka("Energy")] = Energy;
	}
};
void   MatterIO::Temperature(double *T,double *Dencity,double *Energy,int Num){
	int k;
	if (NumInputVal()==2)
		for ( k=1;k<=Num;k++) { T[k]=Temperature(Dencity[k],Energy[k]);}
	else {
        InputVectorStorage.erase(Stroka("Dencity"));InputVectorStorage.erase(Stroka("Energy"));
        for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
            if (!it->second)
                throw info_except("Have not defined vals for %s\n",it->first.c_str());
		for ( k=1;k<=Num;k++){
            for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
                InputValStorage[it->first] = it->second[k];
            T[k]=Temperature(Dencity[k],Energy[k]);
		}
        InputVectorStorage[Stroka("Dencity")] = Dencity;
        InputVectorStorage[Stroka("Energy")] = Energy;
	}
};

void MatterIO::ChangeInputVal(double *Dencity,double *Energy,
                              int Num,double Time){
	int k;
    if (NumInputVal()!=2){
        InputVectorStorage.erase(Stroka("Dencity"));InputVectorStorage.erase(Stroka("Energy"));
        for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
            if (!it->second)
                throw info_except("Have not defined vals for %s\n",it->first.c_str());
		for ( k=1;k<=Num;k++){
            for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
                InputValStorage[it->first] = it->second[k];
		    ChangeInputVal(Dencity[k],Energy[k],Time);
            for(map<Stroka,double*>::iterator it=InputVectorStorage.begin();it!=InputVectorStorage.end();it++)
                it->second[k] = InputValStorage[it->first];
		}
        InputVectorStorage[Stroka("Dencity")] = Dencity;
        InputVectorStorage[Stroka("Energy")] = Energy;
	}
};



int InterfaceFindSubstanceIO::GetMatter(){
    fstream in(FileName.c_str(),ios::in);
    char tmp[256];
    while (1){
        if (seek_eof(in))
            return 0;
        in>>tmp;
        if (Stricmp(tmp,Substance.c_str())!=0)
            in.getline(tmp,256);
        else 
            break;
    }
    ReadSubstancePar(in); 
	if (!in) 
		throw info_except("Could not read params for Substance:%s in file:%s\n",~Substance,~FileName);
	in.close();
    return 1;
};

double MatterABu::Temperature(double Dencity,double Energy)
  {
   double P=Pressure(Dencity,Energy);
   double ret=0.5*P*(1/DencityCold()-1/Dencity);
   return ret;
  };

double MatterABu::Pressure(double Dencity,double Energy) // P(x,e)
  {
   if (Dencity<MathZer) 
			  {cout<<" Dencity is negative. double MatterABu::Pressure.";return 0;}
   double X=1-Denc/Dencity;
//   if (1-B*X<MathZer) return MathInf;
   double Ph=Denc*sqr(A)*X/sqr(1-B*X);
#ifdef Pc_ABU_ZERO
Ph=max<double>(-1e-4,Ph);
#endif
   double Eh=0.5*Ph*X/Denc;
// Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
   return Ph+Grun*Denc*(Energy-Eh);  // Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
  };
double MatterABu::Sound(double Dencity,double Energy){
   if (Dencity<MathZer) 
       return StndErr;
		//{cout<<" Dencity is negative. double MatterABu::Pressure.";return StndErr;}
   double X=1-Denc/Dencity;
   double Bx1 = pow(B*X-1,3), Gx = Grun*X;
   double Arg = sqr(1-X)*(2*Energy*sqr(Grun)*Bx1+A*A*(-2+sqr(Gx)-
       B*X*(2+Gx*(Gx-2))));
   double S2 = 0.5*Arg/Bx1;
   if (S2<0)
       return StndErr;
//cout<<"S "<<sqrt(S2)<<" est "<<MatterIO::Sound(Dencity,Energy)<<" Dencity "<<Dencity<<" Energy "<<Energy<<"\n";
   return sqrt(S2);

};
void MatterABu::Sound(double *S,double *Dencity,double *Energy,int Num){		
	double G2 = sqr(Grun), A2 = sqr(A);
	for (int k=1;k<=Num;k++)
		if (Dencity[k]<MathZer) 
            S[k]=StndErr;
		else {
			double X=1-Denc/Dencity[k];
            double Bx1 = pow(B*X-1,3), Gx = Grun*X;
            double Arg = sqr(1-X)*(2*Energy[k]*G2*Bx1+A2*(-2+sqr(Gx)-
                B*X*(2+Gx*(Gx-2))));
            double S2 = 0.5*Arg/Bx1;
            if (S2<0)
                S[k] = StndErr;
            else
                S[k] = sqrt(S2);
//if (fabs(S[k]-MatterIO::Sound(Dencity[k],Energy[k]))>1e-2)
//cout<<"S "<<S[k]<<" est "<<MatterIO::Sound(Dencity[k],Energy[k])<<" Dencity "<<Dencity[k]<<" Energy "<<Energy[k]<<"\n";
		}
};			
void MatterABu::Pressure(double *P,double *Dencity,double *Energy,int Num){		
	double C=Denc*sqr(A),X,Ph,Eh,V1=0.5/Denc,G=Grun*Denc;
	for (int k=1;k<=Num;k++) { 
		if (Dencity[k]<MathZer) P[k]=0;//{cout<<" Dencity is negative. void MatterABu::Pressure.";P[k]=0;}
		else {
			X=1-Denc/Dencity[k]; Ph=C*X/sqr(1-B*X);  
#ifdef Pc_ABU_ZERO
Ph=max<double>(-1e-4,Ph);
#endif
			Eh=Ph*X*V1;
			P[k]=Ph+G*(Energy[k]-Eh);  // Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro
		}
	}
};			
void MatterVacuum::Pressure(double *P,double *Dencity,double *Energy,int Num)
{ for (int k=1;k<=Num;k++)  P[k]=0;};


























/*

int URRegister(fstream &in,MatterABu *&Urs)
  {
   char name[256]; in>>name;
   MatterABu *Func;
   int ret=1;
   if (!Stricmp(name,"D_Uappr")) {Func=(MatterABu*)new MatterABu;}
     else ret=0;
   if (!ret)  { cout<<" There is no such urs "<<name<<"\n";  }
   else
      {
       if (Urs!=NULL) delete Urs;
       Urs=Func;
       Urs->Input(in);
      }
//cout<<" Mem after FAZerIORegister  "<<coreleft()<<"\n";
   return ret;
  };

int FreeStp(Thermo Par,double dU,double &Lft,double &Rgt)
  {
   double X=Par.X,Xstp; 
   Matter m=MatTab.GetMatter(Par.Type);
   if (!Control(Par)) {BadAdia=1;return 0;}
//   Par=AdiaH(Par);Par=AdiaH(Par,Par.U+dU);
//   if (!Control(Par)) {BadAdia=1;cout<<"FreeStp\n";return 0;}
   if (dU>0) 
	 	 {
		  Xstp=(1/m.Bconst-Par.X)/2;X+=Xstp;
			int k=0,max=10;
			while ((FreeMisfit(Par,X,dU)>0) && (k<max)) {Xstp/=2;X+=Xstp;k++;}
      if (k==max)  {BadAdia=1;cout<<"FreeStp+.Control k\n";ChRead();return 0;}
      Lft=X-Xstp;Rgt=X;
		 }
   else
	 	{
     Xstp=max<double>(Par.X,1/m.Bconst-Par.X);//Par.X/1000;
     X-=Xstp;
		 int k=0,max=500;
     while ((FreeMisfit(Par,X,dU)>0) && (k<max)) 
       {
//        Xstp*=10;
        X-=Xstp;k++;}
     if (k==max)  {BadAdia=1;cout<<"FreeStp-.Control k\n";ChRead();return 0;}
     Lft=X;Rgt=X+Xstp;
		}
//   Xstp=X*0.1;//(Par.X-1)   *1000;
//   Xstp=0.0001;
   return 1;
  };

*/
