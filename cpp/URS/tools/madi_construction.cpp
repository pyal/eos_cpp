#include "lib\precompiled\Madi_construction.h"

#include "lib/data/DataVec.h"
#include "lib/ref/CommandLine.h"
#include "urs/matt_fac.h"
#include "mat/quad.h"
struct MakeConstruction{

	MakeConstruction(const char *name){ReadParam(name);};

	struct Result:SavableClass{
		DataVector<double, CopyStructFast<double> > U_P;
		double Hardness, D;
		double Pinter, Uinter, Rinter;
		double Pstart, Ustart, Rstart;
		static void Clc(Result *res, double d, double pstart, double ustart, double rstart,
						double (*Func)(double)){
			res->Pstart = pstart; res->Ustart = ustart; res->Rstart = rstart;
			res->D = d;
			res->Hardness = res->Rstart * res->D;
			res->U_P = DataVector<double, CopyStructFast<double> >::MakeStepVector(Discr,Ufr,Uto,1);
			for(int k=0;k<res->U_P.SetDim();k++)
				res->U_P[k] = Func(res->U_P[k]);
		}
		static void Fin(Result *res, double pinter, double uinter, double rinter){
			res->Pinter = pinter; res->Uinter = uinter; res->Rinter = rinter;
		}
		static double Ufr,Uto,Discr;
	};
	DataVector<Result> U_P;
	Result Mat1Result,Mat2Result;
	string GrphName;
	void ReadParam(const char *name){
//		fstream in(name,ios::in);
		FilterTextIn inf(name);
		char tmp[256];
		MatterIO *M1, *M2;
		inf>>tmp;//Matter1=U1=0
		if (NULL==(M1 = dynamic_cast<MatterIO*>(SavableClass::Read(inf)))) throw info_except("Could not register matter 1");
		inf>>tmp;//Matter2
        if (NULL==(M2 = dynamic_cast<MatterIO*>(SavableClass::Read(inf)))) throw info_except("Could not register matter 2");
			
		Mat1 = dynamic_cast<MatterABu*>(M1);
		Mat2 = dynamic_cast<MatterABu*>(M2);
		if (!Mat1 || !Mat2 )
			throw info_except("Matters are not ABu type");
		inf>>tmp>>U2>>tmp>>Ro>>tmp>>L>>tmp>>tmp;
		GrphName = tmp;
		inf>>tmp;
//		FilterTextIn inf(name);
		inf>>Times;
		U_P.SetDim(Times.SetDim());
//fcout<<" Times "<<Times;
	}
	struct Clc{
		void Init(MatterABu *matter,double umat0, double pmat0, double uinter,
			double dinter, double pinter, double rinter, int fstmat){
			Matter = matter;Umat0=umat0;Pmat0=pmat0;Uinter=uinter;Dinter=dinter;Pinter=pinter;
			Rinter=rinter;FstMat = fstmat;
		}
		void GetPar(double u, double &pinter, double &rinter){
			pinter = PressureInter(u);
			double dU = FstMat?u-Uinter:Uinter-u;
cout<<" Dinter "<<Dinter<<" dU "<<dU<<" Unew "<<u<<" Uold "<<Uinter<<"\n";
			rinter = Rinter*Dinter/(Dinter-dU);
		}
		double MisP(double u){
			return PressureMat(u)-PressureInter(u);
		}
		double PressureMat(double U){
			double dU = FstMat?Umat0-U:U-Umat0;
//cout<<" U "<<U<<" dUmat "<<dU<<" D "<<Matter->A+Matter->B*dU<<" P "<<(Matter->A+Matter->B*dU)*dU*Matter->Denc<<" U0 "<<Umat0<<"\n";
			return (Matter->A+Matter->B*dU)*dU*Matter->Denc;
		}
		double PressureInter(double U){
			double dU = FstMat?U-Uinter:Uinter-U;
			return Rinter*dU*Dinter+Pinter;
		}
	private:
		MatterABu *Matter;
		double Umat0,Pmat0,Uinter,Dinter,Pinter,Rinter;
		int FstMat;
	};
	static Clc clc;
	static double MisP(double u)		 {return clc.MisP(u);}
	static double PressureMat(double u)  {return clc.PressureMat(u);}
	static double PressureInter(double u){return clc.PressureInter(u);}
	void OutputRes(){
		fstream out(GrphName.c_str(),ios::out);
		if (U_P.SetDim()<1) throw info_except("There is no data to output\n");
		out<<" Pnt Us Ps Rs Ue Pe Re D\n";
		for (int k1=0;k1<U_P.SetDim();k1++) {
			out<<" a ";
			out<<k1<<" "<<U_P[k1].Pstart<<" "<<U_P[k1].Ustart<<" "<<U_P[k1].Rstart<<" ";
			out<<U_P[k1].Pinter<<" "<<U_P[k1].Uinter<<" "<<U_P[k1].Rinter<<" "<<U_P[k1].D<<"\n";
		}
		out<<" U P_Mat1 P_Mat2 P_Pnts...\n";
		DataVector<double, CopyStructFast<double> > U = DataVector<double, CopyStructFast<double> >::MakeStepVector(Result::Discr,Result::Ufr,Result::Uto,1);
	
		for (int k=0;k<U_P[0].U_P.SetDim();k++){
			out<<U[k]<<" "<<Mat1Result.U_P[k]<<" "<<Mat2Result.U_P[k]<<" ";
			for (int k1=0;k1<U_P.SetDim();k1++) out<<U_P[k1].U_P[k]<<" ";
			out<<"\n";
		}
	}
	void Do(){
		int Fst = 1;
		double uinter=0,dinter=0,pinter=0,linter = L,rinter=Ro;
		Result::Ufr = 0; Result::Uto = U2; Result::Discr = 100;
		clc.Init(Fst?Mat1:Mat2,Fst?U2:0,0,uinter,dinter,pinter,rinter,Fst);
		Result::Clc(&Mat1Result,dinter,pinter,uinter,rinter,PressureMat);
		Fst = !Fst;
		clc.Init(Fst?Mat1:Mat2,Fst?U2:0,0,uinter,dinter,pinter,rinter,Fst);
		Result::Clc(&Mat2Result,dinter,pinter,uinter,rinter,PressureMat);
		Fst = !Fst;

		for(int k = 0;k<Times.SetDim();k++){
			dinter = linter/Times[k];
cout<<" Found U "<<uinter<<" P "<<pinter<<" Ro "<<rinter<<" L "<<linter<<" D "<<dinter<<"\n";
			clc.Init(Fst?Mat1:Mat2,Fst?U2:0,0,uinter,dinter,pinter,rinter,Fst);
			Result::Clc(&(U_P[k]),dinter,pinter,uinter,rinter,PressureInter);
			uinter = U2/2;
			if (!Fzero(MisP,0,U2,uinter,1e-6,1e-6,1000)) 
				cout<<"Fzero error\n";
			clc.GetPar(uinter,pinter,rinter);
			Result::Fin(&(U_P[k]),pinter,uinter,rinter);
			linter = L*Ro/rinter;
			Fst = !Fst;
		}
cout<<" Found U "<<uinter<<" P "<<pinter<<" Ro "<<rinter<<" L "<<linter<<" D "<<0<<"\n";
		OutputRes();
	}
	~MakeConstruction(){
		delete Mat1;delete Mat2;
	}
private:
	MatterABu *Mat1,*Mat2;
	double U1,U2, Ro, L;
	DataVector<double, CopyStructFast<double> > Times;
};

double MakeConstruction::Result::Ufr, MakeConstruction::Result::Uto, MakeConstruction::Result::Discr;
MakeConstruction::Clc MakeConstruction::clc;

int main( int argc, const char *argv[] )
{
    SetLeakTest();
    try {
        NRef::CommandLineExe Cmd("Usage: madi_construction config.file [/key]  \nconfig file example:\nmatter1\nMatterABu   Matter_File material.cfg    Substance  Steel-Tern\nmatter2\nMatterABu   Matter_File material.cfg    Substance  Z-Al203_ \nMat2U  5\nMatInterRo 0.01\nMatInterL  10\nOutPut madi_construction.res\nTimes NumPnt 8 Data\n1.575 0.2556 0.078 0.0368 0.0219 0.0154 0.01211 0.01027\n");
        Cmd.Add('h',"PrintHelp"," - print this help \n");
//        Cmd.Add('p',"PrintDiscrepency"," - print discrepency estimations \n");
//        Cmd.Add('a',"AllResults"," - not to delete good results \n");
//        Cmd.MainHelp += GenerateDetailedHelp();

        Cmd.Analize(argc, argv);
//        if (Cmd.Get("PrintDiscrepency").Exist) TestCase_Famessb_Static::PrintDiscrepencyEstimations=1;
//        if (Cmd.Get("AllResults").Exist) TestCase_Famessb_Static::DeleteGoodResults=0;
        if (Cmd.Argc!=2 || Cmd.Get("PrintHelp").Exist){
            cout<<Cmd.MakeHelp();
            return 0;
        } 
		MakeConstruction Constr(argv[1]);
		Constr.Do();


//        TestSuite::RunTest("TestCase_Famessb_Static");
        }
    CATCHMAINEXCEPTION(" Main test failed ");
    LeakTest();
    return 0;
}
