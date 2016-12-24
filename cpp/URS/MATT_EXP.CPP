#include <lib/precompiled/eos.h>
#include "matt_exp.h"

static ClassDesc MatterABuAdd_ClassDesc(typeid(MatterABuAdd),"MatterABuAdd",1,"Matter_category",create<MatterABuAdd>);
static ClassDesc MatterABuExp_ClassDesc(typeid(MatterABuExp),"MatterABuExp",1,"Matter_category",create<MatterABuExp>);

double MatterABuExp::SubT, MatterABuExp::DivT,   MatterABuExp::Teta0,
       MatterABuExp::Denc0,MatterABuExp::Energy0,MatterABuExp::Eh0, 
       MatterABuExp::Th0;
CurveSpline *MatterABuExp::Cv_ss,*MatterABuExp::G_ss;

   
void MatterABuExp::ReadSubstancePar(istream &in)

  {char tmp[256];
   TData<real> *data=NULL;
   in>>tmp>>tmp>>Denc>>tmp>>NumX>>tmp>>Misfit>>tmp>>AddP
     >>tmp>>AddE>>tmp>>AddT>>tmp>>MinT>>tmp>>MinV>>tmp>>DencHisch>>tmp>>DataFile;
// in tmp file in the format:   U           S         D        Gamma  T   Cv
// to                          log(Denc)  log(Ph)  log(Eh)     Grun 
//                              0           1         2         3
   InputTDataF(DataFile.c_str(),data);
   if (data->N!=6) 
     { cout<<"wrong file format in MatterABuExp file: "<<tmp<<"\n"; return ;}
   in.getline(tmp,256);Hisch.ReadSubstancePar(in);
   double U,D,P,E,De,T;
   for (int k=0;k<data->I[0];k++) 
     {
      D=data->D[2][k];U=data->D[0][k];T=data->D[4][k];
      P=Denc*D*U;E=0.5*sqr(U);De=Denc*D/(D-U);
      data->D[0][k]=log(De);data->D[1][k]=log(P+AddP); data->D[2][k]=log(E+AddE);
      data->D[4][k]=log(T+AddT);
//      data->D[0][k]=(De);data->D[1][k]=P; data->D[2][k]=E;
     } 
   delete Ph_s;delete Eh_s;delete G_s;delete T_s;delete Teta0_s;delete Cv_s;
   Ph_s=new CurveSpline;Eh_s=new CurveSpline;G_s=new CurveSpline;T_s=new CurveSpline;
   Teta0_s=new CurveSpline; Cv_s=new CurveSpline;
   int n=NumX;
   double Mis=Misfit;
   Ph_s->Generate(n,Mis,0,*data,0,1);n=NumX;Mis=Misfit;
   Eh_s->Generate(n,Mis,0,*data,0,2);n=NumX;Mis=Misfit;
   G_s->Generate(n,Mis,0,*data,0,3);n=NumX;Mis=Misfit;
   T_s->Generate(n,Mis,0,*data,0,4);n=NumX;Mis=Misfit;
   double integ;
   G_ss=G_s;
   for (int k=0;k<data->I[0];k++) 
     {
      D=exp(data->D[0][k]);
      integ=RecInt(GrInt,MinV,1/D,Mis*0.01);
      T_s->Evaluate(&D,&T,1);T=exp(T)-AddT;
      data->D[1][k]=-integ/log(T/MinT);
//      data->D[1][k]=integ+log(T/MinT);
//cout<<" V "<<1/D<<" Teta "<<data->D[1][k]<<"\n";
//      data->D[1][k]=Teta;
     } 
   Teta0_s->Generate(n,Mis,0,*data,0,1);n=NumX;Mis=Misfit;
   Cv_s->Generate(n,Mis,0,*data,1,5);n=NumX;Mis=Misfit;
cout<<" Constructed\n";
fstream out("tmp",ios::out);OutputTDataF(out,*data);out.close();
   delete data;
   };

void MatterABuExp::SaveSubstancePar(ostream &out){
	out<<Finder.Substance<<" \tDenc "<<Denc<<" NumX "<<NumX<<" Misfit "<<Misfit<<" AddP "<<AddP
		<<" AddE "<<AddE<<" AddT "<<AddT<<" MinT "<<MinT<<" MinV "<<MinV<<" DencHisch "<<DencHisch
		<<" DataFile "<<DataFile<<"\n";
	Hisch.SaveSubstancePar(out);
};

